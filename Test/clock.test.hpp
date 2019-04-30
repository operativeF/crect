/*
 * clock.test.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: Thomas Figueroa
 */

#include <crect/clock.hpp>

#include <chrono>
#include <iostream>
#include <limits>

namespace clock_test
{

int run()
{
    using std::chrono::milliseconds;

    struct DWT_Dummy
    {
        uint64_t CYCCNT = 0;
    };

    static DWT_Dummy sys_dwt;

    using test_sys_clock = crect::time::sys_clock<__F_CPU, &sys_dwt>;

    auto time_now = std::chrono::time_point_cast<milliseconds>(test_sys_clock::now());

    std::cout << time_now.time_since_epoch().count() << '\n';

    sys_dwt.CYCCNT = 1000000;

    auto time_100 = std::chrono::time_point_cast<milliseconds>(test_sys_clock::now());
    std::cout << time_100.time_since_epoch().count() << '\n';

    sys_dwt.CYCCNT = std::numeric_limits<uint64_t>::max();

    auto time_max = std::chrono::time_point_cast<milliseconds>(test_sys_clock::now());
    std::cout << time_max.time_since_epoch().count() << '\n';

    sys_dwt.CYCCNT = 100000;

    auto time_reset = std::chrono::time_point_cast<milliseconds>(test_sys_clock::now());
    std::cout << time_reset.time_since_epoch().count() << '\n';

    return 0;
}

} // end namespace clock_test
