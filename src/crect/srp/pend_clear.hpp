//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "stm32f411xe.h"

namespace crect
{

template<auto* const nLocal>
struct NVIC_Access
{
    template<typename Job>
    constexpr void pend()
    {
        using ISRn = typename Job::isr::index;
        nLocal->ISPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 31UL));
    }

    constexpr void pend(unsigned id)
    {
        nLocal->ISPR[id >> 5UL] = (1UL << (id & 31UL));
    }

    template <typename Job>
    constexpr void clear()
    {
      using ISRn = typename Job::isr::index;
      nLocal->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 31UL));
    }

    /**
     * @brief Clear a pending crect job.
     *
     * @param[in] id    The interrupt ID to clear.
     */
    constexpr void clear(unsigned id)
    {
      nLocal->ICPR[id >> 5UL] = (1UL << (id & 31UL));
    }
};

} /* END namespace crect */
