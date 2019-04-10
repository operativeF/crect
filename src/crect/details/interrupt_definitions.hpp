//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

#include <boost/tmp.hpp>

#include "kvasir/mpl/mpl.hpp"

namespace crect::details
{

using namespace boost::tmp;

/**
 * @brief ISR function pointer alias.
 */
using isr_function_pointer = void (*)(void);

/**
 * @brief ISR index alias.
 */
template <int I>
using idx = std::integral_constant<int, I>;

/**
 * @brief Specialization of an ISR type, peripheral ISR definition.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <isr_function_pointer P, int I>
struct isr
{
  using is_sys_isr = std::is_same<decltype(P), std::nullptr_t>;
  
  using type =
    if_<
      is_<is_sys_isr>,
        if_<is_<int_<I>, less_<int_<0>>>,
          always_<isr<nullptr, I>>,
          always_<std::false_type>
        >,
        if_<is_<int_<I>, not_<less_<int_<0>>>>,
          always_<isr<P, I>>,
          always_<std::false_type>
        >    
    >;

  static constexpr const isr_function_pointer value = P;
  //using type = isr<P, idx<I>>;
  using index = idx<I>;
};
} // END namespace crect::details

namespace crect
{
/**
 * @brief Wrapper of an ISR type for ease of use.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <details::isr_function_pointer P, int I>
using make_isr = details::isr<P, I>;

/**
 * @brief Wrapper of an System ISR type for ease of use.
 *
 * @tparam I  Index value.
 */
template <int I>
using make_system_isr = details::isr<nullptr, I>;

/**
 * @brief Default ISR definition, statically defined. A simple forever loop.
 */
class DefaultISR
{
public:
  static void UnusedISR()
  {
    while (1);
  }
};

} /* END namespace crect */
