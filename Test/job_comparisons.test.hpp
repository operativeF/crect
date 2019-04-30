/*
 * job_comparisons.test.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: Thomas Figueroa
 */
#include <memory>

#include <crect/details/job_resource_comparisons.hpp>
#include <crect/details/job_resource_definitions.hpp>
#include <crect/details/interrupt_definitions.hpp>

namespace job_comparisons_test
{

// Dummy objects
struct OBJ_A {};
struct OBJ_B {};

inline void JOB_A() {};

int run()
{    
    static OBJ_A objA {};
    static OBJ_B objB {};
    
    using namespace crect::details;

    namespace tmp = boost::tmp;

    // Dummy ISR
    using isrA      = crect::make_isr<JOB_A, 2>;
    using isrA_copy = crect::make_system_isr<-1>;
    using isrB      = crect::make_system_isr<-1>;

    // Dummy resources
    using resA      = crect::make_resource<CRECT_OBJECT_LINK(objA)>;
    using resB      = crect::make_resource<CRECT_OBJECT_LINK(objB)>;

    // Jobs
    using jobA      = crect::job<0, isrA, resA>;
    using jobA_copy = crect::job<0, isrA_copy, resA>;
    using jobB      = crect::job<1, isrB, resB>;

    tmp::bool_<false>{} = _same_job_id<jobA>::f<jobA_copy>{};

    // Check type here:
    constexpr auto isr_v = jobA::isr::value;
    //BOOST_TEST( isr_v == &JOB_A );
    tmp::bool_<false>{} = _same_job_id<jobA>::f<jobB>{};

    //BOOST_TEST( jobA_copy::isr::value == nullptr );
    //BOOST_TEST( jobB::isr::value == nullptr );

    bool_<true>{}  = _same_resource_id<resA>::f<resA>{};
    bool_<false>{} = _same_resource_id<resA>::f<resB>{};

    bool_<false>{} = _different_resource_id<resA>::f<resA>{};
    bool_<true>{}  = _different_resource_id<resA>::f<resB>{};

    bool_<false>{} = _different_resource_id_2r<resA, resA>{};
    bool_<true>{}  = _different_resource_id_2r<resA, resB>{};

    return 0;
}

} // end namespace job_comparisons_test