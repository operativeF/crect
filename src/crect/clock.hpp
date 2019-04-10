//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <chrono>
#include <type_traits>

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"

namespace crect::time
{

struct DWT_Dummy
{
  uint32_t CYCCNT = 10;
};

static constexpr auto DWT__ = DWT_Dummy{};


/**
 * @brief     The definition of the system clock.
 * @details   It is based on the DWT cycle counter for calculating time, this
 *            means that system_clock::now() needs to be called at least once
 *            per overflow of the DWT, else the time will not be correct.
 *            It is also possible to replace this function with another timer
 *            implementation.
 */
template<int32_t F_CPU = __F_CPU, auto* const DWT_Counter = &DWT__>
struct sys_clock
{
  /** @brief  Definition of the duration of timer ticks running at MCU clock. */
  using duration = std::chrono::duration< int64_t, std::ratio< 1, F_CPU > >;
  using small_duration =
      std::chrono::duration< int32_t, std::ratio< 1, F_CPU > >;
  using rep                   = typename duration::rep;
  using period                = typename duration::period;
  using time_point            = std::chrono::time_point< sys_clock >;
  static const bool is_steady = false;

  /**
   * @brief   Extraction of the current time in clock ticks using the DWT.
   *
   * @note    This function MUST be called more often than the time for an
   *          overflow of the DWT counter, else the system time will be
   *          incorrect.
   *
   * @note    This is a shared resouce, must be accessed from within a lock.
   *
   * @return  Returns the current time.
   */
  static time_point now() noexcept;
};

template<int32_t F_CPU, auto* const DWT_>
typename sys_clock<F_CPU, DWT_>::time_point sys_clock<F_CPU, DWT_>::now() noexcept
{
  /* Holds the current offset from start time due to DWT overflows. */
  static uint32_t base = 0;

  /* Holds the old DWT value to check for overflows. */
  static uint32_t old_dwt = 0;

  uint32_t dwt = DWT_->CYCCNT;

  /* If the DWT has overflowed, update the base. */
  if (old_dwt > dwt)
    base += 1;

  /* Save old DWT. */
  old_dwt = dwt;

  return time_point(duration(
    (static_cast<uint64_t>(base) << 32U) + static_cast<uint64_t>(dwt)
  ));
}

using system_clock = sys_clock<>;

} /* END namespace crect::time */


namespace crect
{
/**
 * @brief   Convenience definition of the clock resource.
 */
using Rsystem_clock = resource<
    std::integral_constant< decltype(&time::system_clock::now),
                                    &time::system_clock::now >,
    false >;

} /* END namespace crect */
