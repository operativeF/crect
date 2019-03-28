/*
 * main_tests.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: Thomas Figueroa
 */

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE main_test

#include <boost/test/unit_test.hpp>
#include <crect/details/job_resource_comparisons.hpp>
#include <crect/details/job_resource_definitions.hpp>
#include <crect/details/interrupt_definitions.hpp>


BOOST_AUTO_TEST_CASE(job_comp_tests)
{
	BOOST_TEST_MESSAGE("Testing Job Resource Comparisons");

    // Dummy ISR
    using isrA      = crect::make_isr<nullptr, 0>;
    using isrA_copy = crect::make_isr<nullptr, 0>;
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

    using namespace crect::details;

    BOOST_TEST_MESSAGE("Testing jobs with same IDs");
    BOOST_TEST(( _same_job_id<jobA>::f<jobA_copy>::value ));
    BOOST_TEST(( !_same_job_id<jobA>::f<jobB>::value ));

    BOOST_TEST_MESSAGE("Testing jobs with same resource IDs");
    BOOST_TEST(( _same_resource_id<resA>::f<resA>::value ));
    BOOST_TEST(( !_same_resource_id<resA>::f<resB>::value ));

    BOOST_TEST_MESSAGE("Testing jobs with different resource IDs");
    BOOST_TEST(( !_different_resource_id<resA>::f<resA>::value ));
    BOOST_TEST(( _different_resource_id<resA>::f<resB>::value ));

    BOOST_TEST(( !_different_resource_id_2r<resA, resA>::value ));
    BOOST_TEST(( _different_resource_id_2r<resA, resB>::value ));
}
