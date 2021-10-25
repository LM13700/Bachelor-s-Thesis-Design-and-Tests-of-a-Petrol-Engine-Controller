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

#include "swo.h"

SWO_DefineModuleTag(MAIN);

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define LEDPORT                 (GPIOC)
#define LED1                    (13u)
#define ENABLE_GPIO_CLOCK       (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)

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

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Function stopping MCU for given time
 * param[in]:   ms - time in ms
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void Main_MsDelay(uint32_t ms);

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
    SWO_Init();

    while(1)
    {
        SWO_Print("Hello world from SWO %d\n", 1u);
        SWO_PrintLog("Hello world from SWO %d\n", 2u);
        Main_MsDelay(1000u);
    }
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Main_MsDelay
 *===========================================================================*/
static void Main_MsDelay(uint32_t ms)
{
    volatile uint32_t x;

    while (ms-- > 0)
    {
        x = 500;

        while (x-- > 0)
        {
            __NOP();
        }
    }
}


/* end of file */
