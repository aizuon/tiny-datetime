#include "datetime.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    datetime_t my_birthday(0, 15, 9, 19, 5, 1998);
    std::cout << "My birthday: " << my_birthday.to_string() << std::endl;

    uint64_t timestamp = my_birthday.to_unix_timestamp();
    std::cout << "My birthday Unix timestamp: " << timestamp << std::endl;

    return 0;
}
