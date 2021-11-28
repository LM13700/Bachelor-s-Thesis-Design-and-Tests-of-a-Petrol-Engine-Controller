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

#include "ignition_driver.h"
#include "injection_driver.h"
#include "swo.h"
#include "speed_density.h"
#include "trigger_decoder.h"

SWO_DefineModuleTag(MAIN);

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

    while(1)
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

    SWO_Init();
    TrigD_Init(SpDen_TriggerCallback);
    IgnDrv_Init();
    InjDrv_Init();
    SpDen_Init();

    /* Debug pin PC13 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODE13_0;
    /* Toggle pin: GPIOC->ODR ^= GPIO_ODR_OD13; */

    main_is_speed_trigger_occured = false;

    EnableIRQ();

    // /* Enable GPIOA clock */
    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // /* Set PA0 afternative function 1 (TIM2_CH1) */
    // GPIOA->AFR[1] |= GPIO_AFRH_AFSEL15_0;
    // /* Set PA0 Alternative function mode */
    // GPIOA->MODER |= GPIO_MODER_MODE15_1;
    // /* Enable timer clock*/
    // RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // TIM2->PSC = 0U;
    // TIM2->ARR = 1000000U;
    // TIM2->CCR1 = 500000;
    // TIM2->CCMR1 |= TIM_CCMR1_OC1M;
    // TIM2->CCER |= TIM_CCER_CC1E;
    // // TIM2->CR1 |= TIM_CR1_OPM;
    // TIM2->CR1 |= TIM_CR1_CEN;
    // // TIM2->DIER |= TIM_DIER_UIE;
    // NVIC_EnableIRQ(TIM2_IRQn);
}


/* end of file */
