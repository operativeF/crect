//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <chrono>
#include <array>
#include <memory>
#include "crect/clock.hpp"
//#include "crect/timer.hpp"

/**
 * @brief Unhandled exception definition.
 */
extern "C" inline void _unhandled_exception(void)
{
  while(1);
}

namespace crect
{

using isr_id = std::uint32_t;

/**
 * @brief   The linked list that holds future job executions.
 *
 * @tparam N  The size of the async queue.
 */
template <unsigned N, typename TClock>
class async_queue
{
public:

private:
  using time_point = typename TClock::time_point;

  /** @brief Definition of each queue element. */
  struct queue_element {
    time_point baseline;  /**< Time for execution. */
    isr_id job_isr_id = 0;                      /**< The job's ISR ID to pend. */
    queue_element *next = nullptr;                      /**< The next element. */
  };

  using p_queue = std::array<queue_element, N>;

  /** @brief The queue. */
  p_queue _queue;

  /** @brief Pointer to the head element. */
  queue_element *_head = nullptr;

  /** @brief Pointer to the free element. */
  queue_element *_free = _queue.data();

public:

  /**
  * @brief   Constructor, initializes the queue.
  */
  async_queue()
  {
    /**/
    for (unsigned i = 0; i < N - 1; i++)
    {
        _queue[i].next = &_queue[i + 1];
    }

    _queue[N - 1].next = nullptr;
  }

  /**
  * @brief   Push an element onto the queue.
  *
  * @param[in] time    The time for the ISR to execute.
  * @param[in] id  The ISR to execute at said time.
  */
  void insert(time_point time, isr_id id)
  {
    auto _new = _free;


    /* Out of elements, panic. */
    if (_free == nullptr)
      _unhandled_exception();


    /* Fill in the new node. */
    _new->baseline = time;
    _new->job_isr_id = id;
    _free = _free->next;

    if (_head == nullptr || _head->baseline >= time)
    {
      /* First in list, place and pend the handler. */
      _new->next = _head;
      _head = _new;

      //timer::pend();
    }
    else
    {
      /* Queue is not empty, sort the element into the queue. */
      auto _current = _head;

      while ((_current->next != nullptr) && (_current->next->baseline < time))
        _current = _current->next;

      _new->next = _current->next;
      _current->next = _new;
    }
  }

  /**
  * @brief   Pop the head of the queue.
  */
  auto pop_front()
  {
    auto _current = _head;

    if (_head != nullptr)
    {
      _head = _head->next;
      _current->next = _free;
      _free = _current;
    }

    return _head;
  }

  /**
  * @brief   Get the head of the queue.
  *
  * @return   Head of the queue.
  */
  auto front()
  {
    return _head;
  }

};

} /* END namespace crect */
