/*===========================================================================*
 * File:        engine_constants.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        09.11.2021
 * Brief:       Engine constatnts
 *===========================================================================*/
#ifndef _ENGINE_CONSTANTS_H_
#define _ENGINE_CONSTANTS_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "common_include.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define ENGINE_CONST_ANGLE_UNKNOWN                     (FLT_MAX)
#define ENGINE_CONST_SPEED_UNKNOWN                     (FLT_MAX)
#define ENGINE_CONST_SPEED_RAW_UNKNOWN                 (UINT32_MAX)

#define ENGINE_CONST_ENGINE_FULL_CYCLE_ANGLE           (720.0F)
#define ENGINE_CONST_ENGINE_ONE_ROTATION_ANGLE         (360.0F)
#define ENGINE_CONST_ENGINE_PISTONS_NO                 (3u)

/* Angle of engine used for synchronisation */
/* This angle is set at first speed signal after sync signal */
#define ENGINE_CONST_TRIGGER_ANGLE                     (180.0F)
/* This is number of trigger wheel teeth spinning with crank speed */
/* Wheel spins with cam speed (2 x crank speed), so real number is divided by 2 */
#define ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO            (30.0F)

#define ENGINE_CONST_ONE_TRIGGER_PULSE_ANGLE           (ENGINE_CONST_ENGINE_ONE_ROTATION_ANGLE /    \
                                                        ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO)

#define ENGINE_CONST_IGINITON_DWELL_TIME_MS            (7.0F)

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

/*===========================================================================*
 * brief:       Get current engine angle
 * param[in]:   None
 * param[out]:  None
 * return:      float - engine angle in degrees
 * details:     0 degrees means beginning of the first piston combustion stroke
 *===========================================================================*/
float ENGCON_GetEngineAngle(void);

/*===========================================================================*
 * brief:       Get current engine speed
 * param[in]:   None
 * param[out]:  None
 * return:      float - engine speed in RPM
 * details:     None
 *===========================================================================*/
float ENGCON_GetEngineSpeed(void);

/*===========================================================================*
 * brief:       Get current raw engine speed
 * param[in]:   None
 * param[out]:  None
 * return:      uint32_t - engine speed in raw register value
 * details:     None
 *===========================================================================*/
uint32_t ENGCON_GetEngineSpeedRaw(void);

/*===========================================================================*
 * brief:       Update engine angle
 * param[in]:   angle - engine speed in degrees
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void ENGCON_UpdateEngineAngle(float angle);

/*===========================================================================*
 * brief:       Get current engine speed
 * param[in]:   speed - engine speed in raw register value
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void ENGCON_UpdateEngineSpeed(uint32_t speed);


#endif
/* end of file */
