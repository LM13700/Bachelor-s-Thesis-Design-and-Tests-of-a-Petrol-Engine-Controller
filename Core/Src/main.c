/*===========================================================================*
 * File:        main.c
 * Project:     ECU
 * Author:      Mateusz Mróz
 * Date:        06.09.2021
 * Brief:       Main.c
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "main.h"

#include "engine_sensors.h"
#include "ignition_driver.h"
#include "injection_driver.h"
#include "speed_density.h"
#include "swo.h"
#include "trigger_decoder.h"

Swo_DefineModuleTag(MAIN);

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

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

volatile bool main_is_speed_trigger_occured;

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Function calling initializing functions
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void Main_CallInits(void);

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: main
 *===========================================================================*/
int main(void)
{
    Main_CallInits();

    while (1)
    {
        WaitForInterrupt();
        if (main_is_speed_trigger_occured)
        {
            SpDen_OnTriggerInterrupt();
            main_is_speed_trigger_occured = false;
        }
    }
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Main_CallInits
 *===========================================================================*/
void Main_CallInits(void)
{
    DisableIRQ();

    Swo_Init();
    TrigD_Init(SpDen_TriggerCallback);
    IgnDrv_Init();
    InjDrv_Init();
    EnSens_Init();
    SpDen_Init();

#if DEBUG
    /* Debug pin PC13 init */
    /* Toggle pin: GPIOC->ODR ^= GPIO_ODR_OD13; */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODE13_0;
#endif

    main_is_speed_trigger_occured = false;

    EnableIRQ();
}


/* end of file */
