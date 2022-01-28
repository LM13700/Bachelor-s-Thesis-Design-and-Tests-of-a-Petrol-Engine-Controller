/*===========================================================================*
 * File:        ignition_driver.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        09.11.2021
 * Brief:       Ignition colis driver module
 *===========================================================================*/
#ifndef _IGNITION_DRIVER_H_
#define _IGNITION_DRIVER_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "common_include.h"

#include "engine_constants.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

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
 * brief:       Initialize ignition driver module
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void IgnDrv_Init(void);

/*===========================================================================*
 * brief:       Prepere ignition channel
 * param[in]:   channel - ignition channel to be prepared
 * param[in]:   fireAngle - engine angle at which ignition need to occure
 * param[in]:   startAngle - engine angle at which module will be started
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void IgnDrv_PrepareIgnitionChannel(EnCon_CylinderChannels_T channel, float fireAngle, float startAngle);

/*===========================================================================*
 * brief:       Starts ignition module
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     It is necessary to previously prepare proper channel via prepare function
 *===========================================================================*/
void IgnDrv_StartIgnitionModule(void);


#endif
/* end of file */
