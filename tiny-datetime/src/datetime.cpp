#include "datetime.hpp"

const datetime_t datetime_t::EPOCH(0, 0, 0, 1, 1, 1970);
const uint8_t datetime_t::DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
