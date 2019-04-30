/*
 * job_resource_transforms.test.cpp
 *
 *  Created on: Mar 26, 2019
 *      Author: Thomas Figueroa
 */
#include <iostream>

#include <crect/details/job_resource_comparisons.hpp>
#include <crect/details/job_resource_transformations.hpp>
#include <crect/details/job_resource_definitions.hpp>
#include <crect/details/interrupt_definitions.hpp>

namespace job_resource_transforms_test
{

// Dummy objects
struct OBJ_A {};
struct OBJ_B {};

inline OBJ_A objA {};
inline OBJ_B objB {};

int run()
{
    // Dummy ISR
    using isrA      = crect::make_isr<nullptr, 0>;
    using isrA_copy = crect::make_isr<nullptr, 0>;
    using isrB      = crect::make_isr<nullptr, 1>;

    // Dummy resources
    using resA      = crect::make_resource<CRECT_OBJECT_LINK(objA)>;
    using resB      = crect::make_resource<CRECT_OBJECT_LINK(objB)>;

    // Jobs
    using jobA      = crect::job<0, isrA, resA, resB>;
    using jobA_copy = crect::job<0, isrA_copy, resA>;
    using jobB      = crect::job<1, isrB, resB>;
    using jobC      = crect::job<2, isrA, resB>;

    using namespace crect::details;
    namespace tmp = boost::tmp;

    //BOOST_TEST(( job_to_priority<jobA>::value == 0 ));
    //BOOST_TEST(( job_to_priority<jobB>::value == 1 ));

    //using unpacked_1 = kvasir::mpl::eager::at<crect::job_to_resource<jobA>, 0>;
    //using unpacked_2 = kvasir::mpl::eager::at<crect::job_to_resource<jobA>, 1>;
	//using unpacked_3 = kvasir::mpl::eager::at<crect::job_to_resource<jobB>, 0>;
    //using unpacked_4 = kvasir::mpl::eager::at<crect::job_to_resource<jobC>, 0>;


    //BOOST_TEST(( std::is_same_v<tmp::i0_<crect::job_to_resource<jobA>>::object, resA::object> ));
    //BOOST_TEST(( std::is_same_v<tmp::i1_<crect::job_to_resource<jobA>>::object, resB::object> ));

	//BOOST_TEST(( !merge_resources<unpacked_1, unpacked_2>::object )); // Expect static assert here

	//BOOST_TEST(( std::is_same_v<merge_resources<unpacked_3, unpacked_4>::object, resB::object> ));
    
    return 0;
}

} // end namespace job_resource_transforms_test