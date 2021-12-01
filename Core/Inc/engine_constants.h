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

/* All given angles are relative to the 1st piston work stroke TDC */

#define ENCON_ANGLE_UNKNOWN                     (FLT_MAX)
#define ENCON_SPEED_UNKNOWN                     (FLT_MAX)
#define ENCON_SPEED_RAW_UNKNOWN                 (UINT32_MAX)

#define ENCON_ENGINE_CYCLES                     (4U)
#define ENCON_ENGINE_PISTONS_NO                 (3U)
#define ENCON_ENGINE_FULL_CYCLE_ANGLE           (720.0F)
#define ENCON_ENGINE_ONE_CYCLE_ANGLE            (ENCON_ENGINE_FULL_CYCLE_ANGLE / ENCON_ENGINE_CYCLES)
#define ENCON_ENGINE_ONE_ROTATION_ANGLE         (ENCON_ENGINE_FULL_CYCLE_ANGLE / 2.0F)

#define ENCON_ENGINE_WORK_ANGLE                 (ENCON_ENGINE_ONE_CYCLE_ANGLE * 0.0F)
#define ENCON_ENGINE_EXHAUST_ANGLE              (ENCON_ENGINE_ONE_CYCLE_ANGLE * 1.0F)
#define ENCON_ENGINE_INTAKE_ANGLE               (ENCON_ENGINE_ONE_CYCLE_ANGLE * 2.0F)
#define ENCON_ENGINE_COMPRESSION_ANGLE          (ENCON_ENGINE_ONE_CYCLE_ANGLE * 3.0F)

#define ENCON_ENGINE_PISTON_1_OFFSET            (ENCON_ENGINE_FULL_CYCLE_ANGLE / ENCON_ENGINE_PISTONS_NO * 0.0F)
#define ENCON_ENGINE_PISTON_2_OFFSET            (ENCON_ENGINE_FULL_CYCLE_ANGLE / ENCON_ENGINE_PISTONS_NO * 1.0F)
#define ENCON_ENGINE_PISTON_3_OFFSET            (ENCON_ENGINE_FULL_CYCLE_ANGLE / ENCON_ENGINE_PISTONS_NO * 2.0F)

#define ENCON_ENGINE_DISPLACEMENT_M3            (0.000796F)

/* Angle of engine used for synchronisation */
/* This angle is set at first speed signal after sync signal */
#define ENCON_TRIGGER_ANGLE                     (300.0F)
/* This is number of trigger wheel teeth spinning with crank speed */
/* Wheel spins with cam speed (2 x crank speed), so real number is divided by 2 */
#define ENCON_TRIGGER_WHEEL_TEETH_NO            (30.0F)

#define ENCON_ONE_TRIGGER_PULSE_ANGLE           (ENCON_ENGINE_ONE_ROTATION_ANGLE /    \
                                                 ENCON_TRIGGER_WHEEL_TEETH_NO)

#define ENCON_COILS_DWELL_TIME_MS               (5.0F)

/* Flow rate in cm^3/min */
#define ENCON_INJECTOR_FLOW_RATE_CC_MIN         (150.0F)
#define ENCON_INJECTOR_DEAD_TIME_MS             (0.0F)

#define ENCON_CRANKING_FLOOR_RPM                (250.0F)
#define ENCON_RUNNING_FLOOR_RPM                 (400.0F)

/* Angle offset after beggining of the intake stroke, at which injector will be opened */
#define ENCON_FUEL_DELIVERY_OFFSET_ANGLE        (10.0F)

/* Enrichment during cranking in % */
#define ENCON_CRANKING_ENRICHMENT               (15.0F)

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef enum EnCon_CylinderChannels_Tag
{
    ENCON_CHANNEL_1 = 0,
    ENCON_CHANNEL_2 = 1,
    ENCON_CHANNEL_3 = 2,

    ENCON_CHANNEL_COUNT
} EnCon_CylinderChannels_T;

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
float EnCon_GetEngineAngle(void);

/*===========================================================================*
 * brief:       Get current engine speed
 * param[in]:   None
 * param[out]:  None
 * return:      float - engine speed in RPM
 * details:     None
 *===========================================================================*/
float EnCon_GetEngineSpeed(void);

/*===========================================================================*
 * brief:       Get current raw engine speed
 * param[in]:   None
 * param[out]:  None
 * return:      uint32_t - engine speed in raw register value
 * details:     None
 *===========================================================================*/
uint32_t EnCon_GetEngineSpeedRaw(void);

/*===========================================================================*
 * brief:       Update engine angle
 * param[in]:   angle - engine speed in degrees
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void EnCon_UpdateEngineAngle(float angle);

/*===========================================================================*
 * brief:       Get current engine speed
 * param[in]:   speed - engine speed in raw register value
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void EnCon_UpdateEngineSpeed(uint32_t speed);


#endif
/* end of file */
