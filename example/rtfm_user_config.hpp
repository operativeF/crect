
#pragma once

#include "rtfm/rtfm_srp.hpp"

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

#include "rtfm/rtfm_utils.hpp"

void job1(void);
void job2(void);

using R1 = rtfm::MakeVirtualResource<char>;

using J1 = rtfm::Job<
              rtfm::util::hashit("Job1"), // Unique ID
              1,                          // Priority
              rtfm::MakeISR<job1, 1>,     // ISR connection and location
              R1,                         // Possible resource claims
              rtfm::Rsystem_clock,
              rtfm::Rasync
            >;

using J2 = rtfm::Job<
              rtfm::util::hashit("Job2"), // Unique ID
              2,                          // Priority
              rtfm::MakeISR<job2, 2>,     // ISR connection and location
              R1,                         // Possible resource claims
              rtfm::Rsystem_clock,
              rtfm::Rasync
            >;

using Jasync = rtfm::Job<
              rtfm::util::hashit("JobAsync"),     // Unique ID
              0,                                  // Priority
              rtfm::MakeSystemISR<SysTick_IRQn>,  // ISR connection and location
              rtfm::Rasync, rtfm::Rsystem_clock   // Possible resource claims
            >;

/****************************************************************************
 * End User Job and Resource defines.
 ****************************************************************************/


namespace rtfm
{

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

using system_job_list = kvasir::mpl::list<Jasync, J1, J2>;

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

} /* END namespace rtfm */
