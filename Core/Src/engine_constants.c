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

#define ENCON_SECONDS_IN_MINUTE                (60.0F)

#define ENCON_CALCULATE_RPM(_TIM_REG_VALUE_)   (ENCON_SECONDS_IN_MINUTE * (1.0F / (ENCON_TRIGGER_WHEEL_TEETH_NO * (((float)(_TIM_REG_VALUE_)) / ((float)TIMER_TIM_CLOCK)))))

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
static volatile float encon_engine_angle = ENCON_ANGLE_UNKNOWN;
/* Current engine raw speed value - obtained from input caputre timer registers difference */
/* Conversion to RPM is done in this module to decrease execution time of TIM3 ISR */
static uint32_t encon_engine_speed_raw = ENCON_SPEED_RAW_UNKNOWN;
/* Calculated engine speed value in RPM */
static float encon_engine_speed_rpm = ENCON_SPEED_UNKNOWN;

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
 * Function: EnCon_GetEngineAngle
 *===========================================================================*/
float EnCon_GetEngineAngle(void)
{
    return encon_engine_angle;
}

/*===========================================================================*
 * Function: EnCon_GetEngineSpeed
 *===========================================================================*/
float EnCon_GetEngineSpeed(void)
{
    if (ENCON_SPEED_RAW_UNKNOWN == encon_engine_speed_raw)
    {
        encon_engine_speed_rpm = ENCON_SPEED_UNKNOWN;
    }
    else if (ENCON_SPEED_UNKNOWN == encon_engine_speed_rpm)
    {
        encon_engine_speed_rpm = ENCON_CALCULATE_RPM(encon_engine_speed_raw);
    }
    else
    {
        /* Do nothing */
    }

    return encon_engine_speed_rpm;
}

/*===========================================================================*
 * Function: EnCon_GetEngineSpeedRaw
 *===========================================================================*/
uint32_t EnCon_GetEngineSpeedRaw(void)
{
    return encon_engine_speed_raw;
}

/*===========================================================================*
 * Function: EnCon_UpdateEngineAngle
 *===========================================================================*/
void EnCon_UpdateEngineAngle(float angle)
{
    encon_engine_angle = angle;
}

/*===========================================================================*
 * Function: EnCon_UpdateEngineSpeed
 *===========================================================================*/
void EnCon_UpdateEngineSpeed(uint32_t speed)
{
    encon_engine_speed_raw = speed;
    encon_engine_speed_rpm = ENCON_SPEED_UNKNOWN;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
