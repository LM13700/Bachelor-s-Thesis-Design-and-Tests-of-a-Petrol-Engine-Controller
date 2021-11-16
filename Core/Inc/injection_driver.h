/*===========================================================================*
 * File:        injection_driver.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        14.11.2021
 * Brief:       Fuel injectors driver module
 *===========================================================================*/
#ifndef _INJECTION_DRIVER_H_
#define _INJECTION_DRIVER_H_

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

typedef enum InjDrv_InjectionChannels_Tag
{
    INJDRV_CHANNEL_1,
    INJDRV_CHANNEL_2,
    INJDRV_CHANNEL_3,

    INJDRV_CHANNEL_COUNT
} InjDrv_InjectionChannels_T;

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
 * brief:       Initialize injection driver module
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void InjDrv_Init(void);

/*===========================================================================*
 * brief:       Prepere injection channel
 * param[in]:   channel - injection channel to be prepared
 * param[in]:   injAngle - engine angle at which injection need to start
 * param[in]:   startAngle - engine angle at which module will be started
 * param[in]:   injOpenTimeMs - required injector open time in ms
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void InjDrv_PrepareInjectionChannel(InjDrv_InjectionChannels_T channel, float injAngle, float startAngle,
                                    float injOpenTimeMs);

/*===========================================================================*
 * brief:       Starts injection module
 * param[in]:   channel - channel which will be started
 * param[out]:  None
 * return:      None
 * details:     It is necessary to previously prepare proper channel via prepare function
 *===========================================================================*/
void InjDrv_StartInjectionModule(InjDrv_InjectionChannels_T channel);


#endif
/* end of file */
