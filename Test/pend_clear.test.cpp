/*
 * claim.test.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: Thomas Figueroa
 */

#include <cstdint>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include <crect/srp/pend_clear.hpp>
#include <crect/details/job_resource_comparisons.hpp>
#include <crect/details/job_resource_definitions.hpp>
#include <crect/details/interrupt_definitions.hpp>

BOOST_AUTO_TEST_CASE(pend_clear_test)
{
    // Dummy job functions
    void job1();
    void job2();

    // Dummy ISR
    using isrA      = crect::make_isr<job1, 0>;
    using isrA_copy = crect::make_isr<job2, 0>;
    using isrB      = crect::make_isr<nullptr, 1>;

    // Dummy objects
    struct OBJ_A {};
    struct OBJ_B {};

    static OBJ_A objA {};
    static OBJ_B objB {};


    // Dummy resources
    using resA      = crect::make_resource<CRECT_OBJECT_LINK(objA)>;
    using resB      = crect::make_resource<CRECT_OBJECT_LINK(objB)>;

    // Jobs
    using jobA      = crect::job<0, isrA, resA>;
    using jobA_copy = crect::job<0, isrA_copy, resA>;
    using jobB      = crect::job<1, isrB, resB>;

    struct NVIC_type
    {
        std::uint32_t ISER[8U];
        std::uint32_t ICER[8U];
        std::uint32_t ISPR[8U];
        std::uint32_t ICPR[8U];
    };

    static NVIC_type nvic_local;

    crect::NVIC_Access<&nvic_local> nvic_base;

    nvic_base.pend(jobA{});
    BOOST_TEST(( nvic_local.ISPR[jobA::isr::index::value] == 1 ));
    nvic_base.clear(jobA{});
    BOOST_TEST(( nvic_local.ICPR[jobA::isr::index::value] == 1 ));

	nvic_base.pend(jobA_copy{});
    BOOST_TEST(( nvic_local.ISPR[jobA_copy::isr::index::value] == 1 ));
    nvic_base.clear(jobA_copy{});
    BOOST_TEST(( nvic_local.ICPR[jobA::isr::index::value] == 1 ));

}
