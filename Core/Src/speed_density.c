/*===========================================================================*
 * File:        speed_density.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        20.11.2021
 * Brief:       Module containing speed density method calculations
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "speed_density.h"

#include "ignition_driver.h"
#include "injection_driver.h"
#include "tables.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define SPEED_DENSITY_IGNITION_NO_PENDING       (IGNDRV_CHANNEL_COUNT)
#define SPEED_DENSITY_INJECTION_NO_PENDING      (INJDRV_CHANNEL_COUNT)

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

static volatile IgnDrv_IgnitionChannels_T speed_density_pending_ignition_channel;
static volatile InjDrv_InjectionChannels_T speed_density_pending_injection_channel;

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
 * Function: SpeedDensity_Init
 *===========================================================================*/
void SpeedDensity_Init(void)
{
    speed_density_pending_ignition_channel = SPEED_DENSITY_IGNITION_NO_PENDING;
    speed_density_pending_injection_channel = SPEED_DENSITY_INJECTION_NO_PENDING;
}

/*===========================================================================*
 * Function: SpeedDensity_OnTriggerInterrupt
 *===========================================================================*/
void SpeedDensity_OnTriggerInterrupt(void)
{
    
}

/*===========================================================================*
 * Function: SpeedDensity_TriggerCallback
 *===========================================================================*/
void SpeedDensity_TriggerCallback(void)
{
    if (speed_density_pending_ignition_channel != SPEED_DENSITY_IGNITION_NO_PENDING)
    {
        IgnDrv_StartIgnitionModule(speed_density_pending_ignition_channel);
        speed_density_pending_ignition_channel = SPEED_DENSITY_IGNITION_NO_PENDING;
    }

    if (speed_density_pending_injection_channel != SPEED_DENSITY_INJECTION_NO_PENDING)
    {
        InjDrv_StartInjectionModule(speed_density_pending_injection_channel);
        speed_density_pending_injection_channel = SPEED_DENSITY_INJECTION_NO_PENDING;
    }
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
