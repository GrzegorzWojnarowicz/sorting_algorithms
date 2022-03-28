#include "../inc/sort_functions.hpp"
const int number_of_elements[5] = {10000, 50000, 100000, 500000, 1000000};

int main()
{
    for (size_t i = 0; i <= 4; i++)
    {
        sort_get_time_check(number_of_elements[i]);
    }
}