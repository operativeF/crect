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
#include "crect/srp/locks.hpp"

namespace crect
{
namespace time
{
/**
 * @brief     The definition of the system clock.
 * @details   It is based on the DWT cycle counter for calculating time, this
 *            means that system_clock::now() needs to be called at least once
 *            per overflow of the DWT, else the time will not be correct.
 *            It is also possible to replace this function with another timer
 *            implementation.
 */
struct system_clock
{
  /** @brief  Definition of the duration of timer ticks running at MCU clock. */
  using duration = std::chrono::duration< int64_t, std::ratio< 1, __F_CPU > >;
  using small_duration =
      std::chrono::duration< int32_t, std::ratio< 1, __F_CPU > >;
  using rep                   = duration::rep;
  using period                = duration::period;
  using time_point            = std::chrono::time_point< system_clock >;
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

} /* END namespace time */

/**
 * @brief   Convenience definition of the clock resource.
 */
using Rsystem_clock = resource<
    std::integral_constant< decltype(&time::system_clock::now),
                                    &time::system_clock::now >,
    false >;

} /* END namespace crect */
