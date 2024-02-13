#ifndef DATETIMEDELTA_H
#define DATETIMEDELTA_H

#include <cstdint>

class datetimedelta_t
{
public:
    datetimedelta_t(int64_t seconds = 0, int64_t minutes = 0, int64_t hours = 0, int64_t days = 0, int64_t months = 0,
                    int64_t years = 0)
        : seconds(seconds), minutes(minutes), hours(hours), days(days), months(months), years(years)
    {
    }

    int64_t get_seconds() const { return seconds; }
    int64_t get_minutes() const { return minutes; }
    int64_t get_hours() const { return hours; }
    int64_t get_days() const { return days; }
    int64_t get_months() const { return months; }
    int64_t get_years() const { return years; }

private:
    int64_t seconds;
    int64_t minutes;
    int64_t hours;
    int64_t days;
    int64_t months;
    int64_t years;
};

#endif //DATETIMEDELTA_H
