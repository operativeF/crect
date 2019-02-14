//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <chrono>
#include <type_traits>

#include "TIM1.hpp"

#include "Register/Register.hpp"

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/srp/locks.hpp"

namespace crect
{
namespace time
{

#define __F_ALT (20000)

template<typename TCntl, typename TCnth = TCntl>
static constexpr auto sCounter = []()
{
	if constexpr(std::is_same_v<TCntl, TCnth>)
	{
	  return apply(read(TCntl{}));
	}
	else
	{
	  return (apply(read(TCnth{})) << 32U) + apply(read(TCntl{}));
	}
};

static auto counter_vale = sCounter<decltype(Kvasir::Tim1Cnt::cnt)>();

struct system_clock_general
{
  /** @brief Definition of the duration of timer ticks running at alternative (32-bit) clock. */
  using duration       = std::chrono::duration< int64_t, std::ratio< 1, __F_ALT> >;
  using small_duration = std::chrono::duration< int32_t, std::ratio< 1, __F_ALT > >;
  using rep            = typename duration::rep;
  using period         = typename duration::period;
  using time_point     = std::chrono::time_point< system_clock_general >;

  static const bool is_steady = false;

  /**
   * @brief   Extraction of the current time in clock ticks using a 32-bit timer.
   *
   * @note    This function MUST be called more often than the time for an
   *          overflow of the 32-bit counter, else the system time will be
   *          incorrect.
   *
   * @note    This is a shared resouce, must be accessed from within a lock.
   *
   * @return  Returns the current time.
   */
  static time_point now() noexcept
  {
    static uint32_t base = 0;

    static uint32_t old_tim = 0;

    uint32_t tim = counter_vale;

    if(old_tim > tim)
    {
      base += 1;
    }

    old_tim = tim;

    return time_point(duration((static_cast<uint64_t>(base) << 32U) +
      static_cast<uint64_t>(tim)));
  }
};

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


using Asystem_clock = resource<
kvasir::mpl::integral_constant< decltype(&time::system_clock_general::now),
								&time::system_clock_general::now >,
								false >;

/**
 * @brief   Convenience definition of the clock resource.
 */
using Rsystem_clock = resource<
    kvasir::mpl::integral_constant< decltype(&time::system_clock::now),
                                    &time::system_clock::now >,
    false >;

using SysClock = Asystem_clock;

} /* END namespace crect */
