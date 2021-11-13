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

typedef enum IgDrv_IgnitionChannels_Tag
{
    IGNDRV_CHANNEL_1,
    IGNDRV_CHANNEL_2,
    IGNDRV_CHANNEL_3,

    IGDRV_CHANNEL_COUNT
} IgDrv_IgnitionChannels_T;

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
void IGNDRV_Init(void);

/*===========================================================================*
 * brief:       Prepere ignition channel
 * param[in]:   channel - ignition channel to be prepared
 * param[in]:   fireAngle - engine angle at which ignition need to occure
 * param[in]:   startAngle - engine angle at which timer will be started
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void IGNDRV_PrepareIgnitionChannel(IgDrv_IgnitionChannels_T channel, float fireAngle, float startAngle);

/*===========================================================================*
 * brief:       Starts ignition timer
 * param[in]:   channel - channel which will be fired
 * param[out]:  None
 * return:      None
 * details:     It is necessary to previously prepare proper channel via prepare function
 *===========================================================================*/
void IGNDRV_StartIgnitionModule(IgDrv_IgnitionChannels_T channel);


#endif
/* end of file */
