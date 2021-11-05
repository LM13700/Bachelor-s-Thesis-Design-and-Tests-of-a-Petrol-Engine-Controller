/*===========================================================================*
 * File:        common_include.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        13.09.2021
 * Brief:       Common include
 *===========================================================================*/
#ifndef _COMMON_INCLUDE_H_
#define _COMMON_INCLUDE_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "stm32f411xe.h"

#include "stdbool.h"
#include "stdint.h"
#include "utils.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define EXTI_GetPendingTrigger(_TRIGGER_)           (EXTI->PR & _TRIGGER_)
#define EXTI_ClearPendingTrigger(_TRIGGER_)         (EXTI->PR |= _TRIGGER_)

#if DEBUG
#define WaitForInterrupt()                            __NOP()
#else
#define WaitForInterrupt()                            __WFI()
#endif

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef enum Common_Result_Tag
{
    COMMON_OK,
    COMMON_NOT_OK

} Common_Result_T;

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
