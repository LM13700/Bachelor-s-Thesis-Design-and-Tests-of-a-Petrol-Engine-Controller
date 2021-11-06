/*===========================================================================*
 * File:        trigger_decoder.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        24.10.2021
 * Brief:       Trigger decoder
 *===========================================================================*/
#ifndef _TRIGGER_DECODER_H_
#define _TRIGGER_DECODER_H_

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

#define TRIGD_ANGLE_UNKNOWN             FLT_MAX

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
 * brief:       Initialize trigger decode module
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void TRIGD_Init(void);

/*===========================================================================*
 * brief:       Get current engine speed
 * param[in]:   None
 * param[out]:  None
 * return:      float - engine speed in RPM
 * details:     None
 *===========================================================================*/
float TRIGD_GetEngineSpeed(void);

/*===========================================================================*
 * brief:       Get current engine angle
 * param[in]:   None
 * param[out]:  None
 * return:      float - engine angle in degrees
 * details:     0 degrees means beginning of the first piston combustion stroke
 *===========================================================================*/
float TRIGD_GetEngineAngle(void);


#endif
/* end of file */
