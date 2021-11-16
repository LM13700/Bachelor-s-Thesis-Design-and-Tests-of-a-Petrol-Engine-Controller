/*===========================================================================*
 * File:        timers.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        13.11.2021
 * Brief:       Timers
 *===========================================================================*/
#ifndef _TIMERS_H_
#define _TIMERS_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "stm32f411xe.h"

#include "utils.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define TIMER_TIM_CLOCK                         (SystemCoreClock)

#define TIMER_MS_IN_S                           (1000.0F)
#define TIMER_MS_TO_TIMER_REG_VALUE(_MS_)       (Utils_FloatToUint32(((_MS_) / TIMER_MS_IN_S) * (float)TIMER_TIM_CLOCK))

/* TIM3, TIM4 -> 16bit */
/* TIM2, TIM5 -> 32bit */
#define TIMER_IGNITION                          (TIM2)
#define TIMER_SPEED                             (TIM3)
#define TIMER_INJECTOR                          (TIM5)

#define TIMER_SPEED_TIMER_MAX_VAL               (UINT16_MAX)

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * EXPORTED GLOBAL VARIABLES SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * EXPORTED FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/


#endif
/* end of file */
