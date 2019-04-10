//          Copyright Emil Fresk 2017.
//                    Thomas Figueroa 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>

#include "kvasir/mpl/mpl.hpp"
#include "crect/utils.hpp"
#include <boost/tmp.hpp>

namespace crect
{
/**
 * @brief Job maximum priority.
 */
using max_priority = boost::tmp::uint_<(1U << __NVIC_PRIO_BITS) - 1>;

/**
 * @brief Job type definition.
 *
 * @tparam Prio   Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template < unsigned Prio, typename ISR, typename... Res >
struct job
{
  using prio      = boost::tmp::uint_<Prio>;
  using isr       = ISR;
  using resources = kvasir::mpl::eager::flatten< kvasir::mpl::list< Res... > >;

  /* Using < for now, comes from setting basepri = 0 has no effect. */
  static_assert(Prio < max_priority::value,
                "Priority is higher than the max allowed");
};

/**
 * @brief Resource type definition - base case (should not come here).
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Unique   Flag to indicate if it is a unique resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template < typename Object, bool Unique, typename... Jobs >
struct resource
{
  static_assert(boost::tmp::call_v<boost::tmp::always_<Object>>,
                "The object is not an integral_constant or memory_mapper");
};

/**
 * @brief Resource type definition - integral_constant version. Used for any
 *        object that is not memory mapped.
 *
 * @tparam T        Type of the pointer to resource.
 * @tparam V        Pointer to resource.
 * @tparam Unique   Flag to indicate if it is a unique resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template < auto V, bool Unique, typename... Jobs >
struct resource< std::integral_constant< decltype(V), V >, Unique, Jobs... >
{
  using is_unique = boost::tmp::bool_< Unique >;
  using jobs      = kvasir::mpl::eager::flatten< kvasir::mpl::list< Jobs... > >;

  /**
   * @brief     Converts the integral_constant to a reference to the object.
   *
   * @return    The reference to the object.
   */
  static std::remove_pointer_t< decltype(V) >& as_object() noexcept
  {
    return *V;
  }

  using object = std::integral_constant< decltype(V), V >;

  static_assert((std::is_pointer_v< util::get_integral_type< object > > ||
                 std::is_null_pointer_v< object >),
                "The type of the object must be a pointer.");

  static_assert(std::is_null_pointer_v< object > == false,
                "The object cannot be nullptr, does not make sense.");
};

/**
 * @brief Resource type definition - memory_mapper version. Used for an
 *        object that is memory mapped.
 *
 * @tparam T        Type of the memory mapped object.
 * @tparam Address  Address to the memory mapped object.
 * @tparam Unique   Flag to indicate if it is a unique resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template < typename T, std::uintptr_t Address, bool Unique, typename... Jobs >
struct resource< util::memory_mapper< T, Address >, Unique, Jobs... >
{
  using is_unique = boost::tmp::bool_< Unique >;
  using jobs      = kvasir::mpl::eager::flatten< kvasir::mpl::list< Jobs... > >;

  /**
   * @brief     Converts the memory_mapper to an reference to the object.
   *
   * @return    The reference to the object.
   */
  static T& as_object() noexcept
  {
    return *reinterpret_cast< T* >(Address);
  }

  static_assert(Address == 0,
                "The memory mapped object's address cannot be 0.");

  static_assert(Address % 4 != 0,
                "The memory mapped object's address is not 4 byte aligned.");
};

/**
 * @brief Resource convenience type definition.
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Jobs     Parameter pack of jobs.
 */
template < typename Object, typename... Jobs >
using make_resource = resource< Object, false, Jobs... >;

/**
 * @brief Unique resource convenience type definition.
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Jobs     Parameter pack of jobs.
 */
template < typename Object, typename... Jobs >
using make_unique_resource = resource< Object, true, Jobs... >;

/**
 * @brief Convenience define for creating an object link until we have
 *        auto templates.
 */
#define CRECT_OBJECT_LINK(obj) \
  std::integral_constant< decltype(&obj), (&obj) >

/**
 * @brief Convenience define for creating an object link to memory mapped
 *        peripherals.
 */
#define CRECT_MEMORY_MAPPED_OBJECT(type, address) \
  util::memory_mapper< type, (address) >

} /* END namespace crect */
