//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "stm32f411xe.h"
#include <crect/vector_table.hpp>

namespace crect
{

template<auto* const nvic_ptr>
struct NVIC_Access
{
    template<typename Job>
    constexpr void pend(const Job&)
    {
        using ISRn = typename Job::isr::index;
        nvic_ptr->ISPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 31UL));
    }

    /*
    template<typename JobF>
    constexpr void pend(const JobF&)
    {
        // Find the job associated with the function.
        using job_pend_candidate = kvasir::mpl::find<JobF, system_job_list>;

    }
    */

    constexpr void pend(unsigned id)
    {
        nvic_ptr->ISPR[id >> 5UL] = (1UL << (id & 31UL));
    }

    template <typename Job>
    constexpr void clear(const Job&)
    {
      using ISRn = typename Job::isr::index;
      nvic_ptr->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 31UL));
    }

    /*
    template<typename JobF>
    constexpr void clear(const JobF&)
    {
        // Find the job associated with the function.

    }
    */

    /**
     * @brief Clear a pending crect job.
     *
     * @param[in] id    The interrupt ID to clear.
     */
    constexpr void clear(unsigned id)
    {
      nvic_ptr->ICPR[id >> 5UL] = (1UL << (id & 31UL));
    }
};

} /* END namespace crect */
