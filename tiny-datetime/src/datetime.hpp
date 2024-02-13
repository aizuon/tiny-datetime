#ifndef DATETIME_H
#define DATETIME_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include "datetimedelta.hpp"

class datetime_t
{
public:
    datetime_t(uint8_t seconds = 0, uint8_t minutes = 0, uint8_t hours = 0, uint8_t days = 0, uint8_t months = 0,
               uint16_t years = 0)
        : seconds(seconds), minutes(minutes), hours(hours), days(days), months(months), years(years)
    {
    }

    uint8_t get_seconds() const { return seconds; }
    uint8_t get_minutes() const { return minutes; }
    uint8_t get_hours() const { return hours; }
    uint8_t get_days() const { return days; }
    uint8_t get_months() const { return months; }
    uint16_t get_years() const { return years; }

    uint8_t weekday() const
    {
        uint16_t q = days;
        uint16_t m = (months < 3) ? months + 12 : months;
        uint16_t K = years % 100;
        uint16_t J = years / 100;

        if (m > 12)
        {
            K -= 1;
        }

        uint16_t h = (q + (13 * (m + 1) / 5) + K + (K / 4) + (J / 4) + 5 * J) % 7;

        uint8_t day_of_week = ((h + 5) % 7) + 1;

        return day_of_week;
    }

    datetime_t operator+(const datetimedelta_t& delta) const
    {
        datetime_t result = *this;

        result.seconds += delta.get_seconds();
        result.minutes += result.seconds / SECONDS_IN_MINUTE;
        result.seconds %= SECONDS_IN_MINUTE;

        result.minutes += delta.get_minutes();
        result.hours += result.minutes / MINUTES_IN_HOUR;
        result.minutes %= MINUTES_IN_HOUR;

        result.hours += delta.get_hours();
        result.days += result.hours / HOURS_IN_DAY;
        result.hours %= HOURS_IN_DAY;

        result.days += delta.get_days();
        while (result.days > days_in_month(result.years, result.months))
        {
            result.days -= days_in_month(result.years, result.months);
            result.months++;
            if (result.months > MONTHS_IN_YEAR)
            {
                result.months = 1;
                result.years++;
            }
        }

        result.months += delta.get_months();
        while (result.months > MONTHS_IN_YEAR)
        {
            result.months -= MONTHS_IN_YEAR;
            result.years++;
        }

        result.years += delta.get_years();

        return result;
    }

    datetimedelta_t operator-(const datetime_t& other) const
    {
        int64_t seconds_diff = static_cast<int64_t>(this->seconds) - other.seconds;
        int64_t minutes_diff = static_cast<int64_t>(this->minutes) - other.minutes;
        int64_t hours_diff = static_cast<int64_t>(this->hours) - other.hours;
        int64_t days_diff = static_cast<int64_t>(this->days) - other.days;
        int64_t months_diff = static_cast<int64_t>(this->months) - other.months;
        int64_t years_diff = static_cast<int64_t>(this->years) - other.years;

        if (seconds_diff < 0)
        {
            seconds_diff += SECONDS_IN_MINUTE;
            minutes_diff--;
        }
        if (minutes_diff < 0)
        {
            minutes_diff += MINUTES_IN_HOUR;
            hours_diff--;
        }
        if (hours_diff < 0)
        {
            hours_diff += HOURS_IN_DAY;
            days_diff--;
        }
        if (days_diff < 0)
        {
            if (months_diff == 0)
            {
                months_diff = MONTHS_IN_YEAR - 1;
                years_diff--;
            }
            else
            {
                months_diff--;
            }
            days_diff += days_in_month(other.years, other.months - 1);
        }
        if (months_diff < 0)
        {
            months_diff += MONTHS_IN_YEAR;
            years_diff--;
        }

        return datetimedelta_t(seconds_diff, minutes_diff, hours_diff, days_diff, months_diff, years_diff);
    }

    uint64_t to_unix_timestamp() const
    {
        uint64_t timestamp = 0;

        for (uint16_t y = EPOCH.years; y < years; ++y)
        {
            timestamp += DAYS_IN_YEAR + (is_leap_year(y) ? 1 : 0);
        }

        for (uint8_t m = EPOCH.months; m < months; ++m)
        {
            timestamp += days_in_month(years, m);
        }

        timestamp += days - EPOCH.days;

        timestamp *= HOURS_IN_DAY * MINUTES_IN_HOUR * SECONDS_IN_MINUTE;

        timestamp += hours * MINUTES_IN_HOUR * SECONDS_IN_MINUTE;
        timestamp += minutes * SECONDS_IN_MINUTE;
        timestamp += seconds;

        return timestamp;
    }

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(4) << static_cast<int>(years) << "-"
                << std::setw(2) << static_cast<int>(months) << "-"
                << std::setw(2) << static_cast<int>(days) << " "
                << std::setw(2) << static_cast<int>(hours) << ":"
                << std::setw(2) << static_cast<int>(minutes) << ":"
                << std::setw(2) << static_cast<int>(seconds);
        return oss.str();
    }

    static const datetime_t EPOCH;

private:
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    uint8_t months;
    uint16_t years;

    static constexpr uint8_t SECONDS_IN_MINUTE = 60;
    static constexpr uint8_t MINUTES_IN_HOUR = 60;
    static constexpr uint8_t HOURS_IN_DAY = 24;
    static constexpr uint8_t MONTHS_IN_YEAR = 12;
    static constexpr uint16_t DAYS_IN_YEAR = 365;
    const static uint8_t DAYS_IN_MONTH[];

    static bool is_leap_year(uint16_t year)
    {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }

    static uint8_t days_in_month(uint16_t year, uint8_t month)
    {
        return DAYS_IN_MONTH[month - 1] + (month == 2 && is_leap_year(year));
    }

    static uint8_t days_in_year(uint16_t year)
    {
        return DAYS_IN_YEAR + static_cast<uint8_t>(is_leap_year(year));
    }
};

#endif //DATETIME_H
