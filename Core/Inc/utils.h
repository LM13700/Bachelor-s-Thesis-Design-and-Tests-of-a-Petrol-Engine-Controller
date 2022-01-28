/*===========================================================================*
 * File:        utils.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        10.10.2021
 * Brief:       Utils
 *===========================================================================*/
#ifndef _UTILS_H_
#define _UTILS_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "stm32f411xe.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define EXTI_GetPendingTrigger(_TRIGGER_)                       (EXTI->PR & (_TRIGGER_))
#define EXTI_ClearPendingTrigger(_TRIGGER_)                     (EXTI->PR |= (_TRIGGER_))

#if DEBUG
#define WaitForInterrupt()                                      __NOP()
#else
#define WaitForInterrupt()                                      __WFI()
#endif

#define ITOA(_NUMBER_, _BASE_)                                  Utils_Itoa((_NUMBER_), (_BASE_))

/* DIFFERENCE = MINUEND - SUBTRAHEND */
#define UTILS_CIRCULAR_DIFFERENCE(_MIN_, _SUB_, _LOOP_VAL_)     (((_MIN_) >= (_SUB_)) ? ((_MIN_) - (_SUB_)) :    \
                                                                 (((_LOOP_VAL_) - (_SUB_)) + (_MIN_)))

#define UTILS_CIRCULAR_ADDITION(_VAL1_, _VAL2_, _LOOP_VAL_)     ((((_VAL1_) + (_VAL2_)) >= (_LOOP_VAL_)) ?       \
                                                                 (((_VAL1_) + (_VAL2_)) - (_LOOP_VAL_)) :        \
                                                                 ((_VAL1_) + (_VAL2_)))

#define UTILS_PERCENTAGE_CONVERTER                              (100U)

#define UTILS_CONVERT_TO_MILI_MULTIPL                           (1000.0F)

/* Convert Kelvins to Celsius */
#define UTILS_CONVERT_K_TO_C(_K_)                               (((float)(_K_)) - 273.15F)
/* Convert Celsius to Kelvins */
#define UTILS_CONVERT_C_TO_K(_C_)                               (((float)(_C_)) + 273.15F)

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef enum Result_Tag
{
    OK,
    NOT_OK

} Result_T;

/*===========================================================================*
 *
 * EXPORTED FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Converts number to string with corresponding number base
 * param[in]:   number - number to convert
 * param[in]:   base - number base
 * param[out]:  None
 * return:      char* - pointer to the output string
 * details:     None
 *===========================================================================*/
char* Utils_Itoa(unsigned int number, unsigned int base);

/*===========================================================================*
 * brief:       Converts float value to uint32_t with rounding
 * param[in]:   inValue - a float value to be converted
 * param[out]:  None
 * return:      uint32_t - result
 * details:     When input value is negative, zero will be returned
 * 
 *              WARNING: This function is really simple thus don't consider many cases,
 *              some roundings may be incorrect. On the other hand it's fast.
 *===========================================================================*/
uint32_t Utils_FloatToUint32(float inValue);


#endif
/* end of file */
