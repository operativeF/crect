//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>

#include "kvasir/mpl/mpl.hpp"
#include "crect_system_config.hpp"

namespace crect
{
namespace util
{
namespace details
{

/**
 * @brief Get the type of an mpl::integral_constant, base case
 *        that always will fail.
 *
 * @tparam Ts   Parameter pack.
 */
template <typename... Ts>
struct get_integral_type_impl
{
  static_assert(kvasir::mpl::eager::always_false<Ts...>::value,
                "The input is not an integral_constant.");
};

/**
 * @brief Get the type of an mpl::integral_constant, general case.
 *
 * @tparam T    Type of the integral_constant.
 * @tparam val  Value of the integral_constant.
 */
template <auto val>
struct get_integral_type_impl<std::integral_constant<decltype(val), val>>
{
  using f = decltype(val);
};

/**
 * @brief Get the type of an mpl::integral_constant, special case for nullptr.
 */
template <>
struct get_integral_type_impl<
    std::integral_constant<decltype(nullptr), nullptr>>
{
  using f = decltype(nullptr);
};

/**
 * @brief Get the type information from an function, base case.
 *
 * @tparam T    Some type.
 */
template <typename T>
struct function_traits_impl;

/**
 * @brief Get the type information from an function, implementation.
 *
 * @tparam Fun    Type of the operator() of the function.
 * @tparam RType  Return type of the function.
 * @tparam Args   Argument parameter pack of the function.
 */
template <typename Fun, typename RType, typename... Args>
struct function_traits_impl<RType( Fun::* )( Args... ) const>
//    Return type ( Function pointer type )( arguments ) const
//                                                    (const because reasons...)
{
  using n_args = std::integral_constant<unsigned, sizeof...(Args)>;
  using return_type = RType;

  template <unsigned I>
  using arg = kvasir::mpl::eager::at< kvasir::mpl::list< Args... >, I >;
};
} /* END namespace details */

/**
 * @brief Get the type of an mpl::integral_constant.
 *
 * @tparam T  Integral_constant to get type from.
 */
template <typename T>
using get_integral_type = typename details::get_integral_type_impl<T>::f;

/**
 * @brief Get the type information from an function.
 *
 * @note  Only works if the function uses auto.
 *
 * @tparam Fun    Function type.
 */
template <typename Fun>
using function_traits =
    details::function_traits_impl<decltype( &Fun::operator() )>;

template <class T, std::uintptr_t ADDRESS>
struct memory_mapper
{
  static_assert(std::is_pod_v<T>, "T must be POD");
};

/**
 * @brief   Takes a user priority (0 = lowest, increasing numbers means higher
 *          priority) and transforms to NVIC priority (0 = highest, increasing
 *          numbers means lower priority).
 *
 * @return  The transformed priority.
 */
constexpr auto priority_to_NVIC_priority(unsigned priority)
{
  /* Max priority */
  auto N = (1U << __NVIC_PRIO_BITS) - 1U;
  return ((N - priority) << (8U - __NVIC_PRIO_BITS)) & 0xFFU;
}

/**
 * @brief  A convenience function for transforming strings to a hashed value.
 *
 * @param[in] txt   A string with '\0' termination.
 */
constexpr uint32_t hashit(const char *txt)
{
  uint32_t h = 0;
  while (*txt != '\0')
    h = h * 1572869 + *txt++;

  return h;
}

} /* END namespace util */
} /* END namespace crect */
