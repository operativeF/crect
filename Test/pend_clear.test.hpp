/*
 * claim.test.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: Thomas Figueroa
 */

#include <cstdint>
#include <iostream>

#include <crect/srp/pend_clear.hpp>
#include <crect/details/job_resource_comparisons.hpp>
#include <crect/details/job_resource_definitions.hpp>
#include <crect/details/interrupt_definitions.hpp>

namespace pend_clear_test
{

// Dummy objects
struct OBJ_A {};
struct OBJ_B {};

static OBJ_A objA {};
static OBJ_B objB {};

struct NVIC_type
{
    std::uint32_t ISER[8U];
    std::uint32_t ICER[8U];
    std::uint32_t ISPR[8U];
    std::uint32_t ICPR[8U];
};

int run()
{
    static NVIC_type nvic_local;

    // Dummy job functions
    void job1();
    void job2();

    // Dummy ISR
    using isrA      = crect::make_isr<job1, 0>;
    using isrA_copy = crect::make_isr<job2, 0>;
    using isrB      = crect::make_isr<nullptr, 1>;

    // Dummy resources
    using resA      = crect::make_resource<CRECT_OBJECT_LINK(objA)>;
    using resB      = crect::make_resource<CRECT_OBJECT_LINK(objB)>;

    // Jobs
    using jobA      = crect::job<0, isrA, resA>;
    using jobA_copy = crect::job<0, isrA_copy, resA>;
    using jobB      = crect::job<1, isrB, resB>;

    crect::NVIC_Access<&nvic_local> nvic_base;

    nvic_base.pend(jobA{});
    if(nvic_local.ISPR[jobA::isr::index::value] != 1 )
    {
        std::cout << "jobA has not been pended correctly." << '\n';
    }

    nvic_base.clear(jobA{});
    if(nvic_local.ICPR[jobA::isr::index::value] != 1 )
    {
        std::cout << "jobA has not been cleared correctly." << '\n';
    }

	nvic_base.pend(jobA_copy{});
    if(nvic_local.ISPR[jobA_copy::isr::index::value] != 1 )
    {
        std::cout << "jobA_copy has not been pended correctly." << '\n';
    }

    nvic_base.clear(jobA_copy{});
    if(nvic_local.ICPR[jobA_copy::isr::index::value] != 1 )
    {
        std::cout << "jobA_copy has not been cleared correctly." << '\n';
    }

    return 0;
}

} // end namespace pend_clear_test
