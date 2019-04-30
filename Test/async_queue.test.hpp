/*
 * claim.test.cpp
 *
 *  Created on: Apr 29, 2019
 *      Author: Thomas Figueroa
 */

#include <cassert>
#include <chrono>
#include <iostream>

#include <crect/async/async_queue.hpp>
#include <crect/clock.hpp>

namespace async_queue_test
{

struct DWT_Dummy
{
    uint64_t CYCCNT = 0;
};

int run()
{
    static DWT_Dummy sys_dwt;

    using test_sys_clock = crect::time::sys_clock<__F_CPU, &sys_dwt>;

    std::cout << "Initializing..." << '\n';

    auto time_now = test_sys_clock::now();

    sys_dwt.CYCCNT += 10;

    std::cout << "Queue initializing..." << '\n';

    crect::async_queue<10, test_sys_clock> test_queue;

    auto time_later = test_sys_clock::now();

    sys_dwt.CYCCNT += 10;

    crect::isr_id first_job  = 1;
    crect::isr_id second_job = 2;
    crect::isr_id third_job  = 3;

    std::cout << "Inserting into queue...." << '\n';

    auto time_again = test_sys_clock::now();

    test_queue.insert(time_now, first_job);
    test_queue.insert(time_later, second_job);
    test_queue.insert(time_again, third_job);

    // Print the queue order
    std::cout << test_queue.front()->job_isr_id << '\n';
    test_queue.pop_front();

    std::cout << test_queue.front()->job_isr_id << '\n';
    test_queue.pop_front();

    std::cout << test_queue.front()->job_isr_id << '\n';
    test_queue.pop_front();

    test_queue.insert(time_now, first_job);
    test_queue.insert(time_later, second_job);

    // Try to overload the queue

    return 0;
}

} // end namespace async_queue_test
