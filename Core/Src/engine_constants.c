/*===========================================================================*
 * File:        engine_constants.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        16.09.2021
 * Brief:       Common include
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "engine_constants.h"

#include "timers.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define ENGCON_SECONDS_IN_MINUTE                (60.0F)

#define ENGCON_CALCULATE_RPM(_TIM_REG_VALUE_)   (ENGCON_SECONDS_IN_MINUTE * ((_TIM_REG_VALUE_) / TIMER_TIM_CLOCK) *    \
                                                 ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO)

/*===========================================================================*
 *
 * LOCAL TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * GLOBAL VARIABLES AND CONSTANTS SECTION
 *
 *===========================================================================*/

/* Current engine angle in degrees, 0 means beginning of the first piston combustion stroke */
static volatile float engcon_engine_angle = ENGINE_CONST_ANGLE_UNKNOWN;
/* Current engine raw speed value - obtained from input caputre timer registers difference */
/* Conversion to RPM is done in this module to decrease execution time of TIM3 ISR */
static uint32_t engcon_engine_speed_raw = ENGINE_CONST_SPEED_RAW_UNKNOWN;
/* Calculated engine speed value in RPM */
static float engcon_engine_speed_rpm = ENGINE_CONST_SPEED_UNKNOWN;

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: ENGCON_GetEngineAngle
 *===========================================================================*/
float ENGCON_GetEngineAngle(void)
{
    return engcon_engine_angle;
}

/*===========================================================================*
 * Function: ENGCON_GetEngineSpeed
 *===========================================================================*/
float ENGCON_GetEngineSpeed(void)
{
    if (ENGINE_CONST_SPEED_RAW_UNKNOWN == engcon_engine_speed_raw)
    {
        engcon_engine_speed_rpm = ENGINE_CONST_SPEED_UNKNOWN;
    }
    else if (ENGINE_CONST_SPEED_UNKNOWN == engcon_engine_speed_rpm)
    {
        engcon_engine_speed_rpm = ENGCON_CALCULATE_RPM(engcon_engine_speed_raw);
    }
    else
    {
        /* Do nothing */
    }

    return engcon_engine_speed_rpm;
}

/*===========================================================================*
 * Function: ENGCON_GetEngineSpeedRaw
 *===========================================================================*/
uint32_t ENGCON_GetEngineSpeedRaw(void)
{
    return engcon_engine_speed_raw;
}

/*===========================================================================*
 * Function: ENGCON_UpdateEngineAngle
 *===========================================================================*/
void ENGCON_UpdateEngineAngle(float angle)
{
    engcon_engine_angle = angle;
}

/*===========================================================================*
 * Function: ENGCON_UpdateEngineSpeed
 *===========================================================================*/
void ENGCON_UpdateEngineSpeed(uint32_t speed)
{
    engcon_engine_speed_raw = speed;
    engcon_engine_speed_rpm = ENGINE_CONST_SPEED_UNKNOWN;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
