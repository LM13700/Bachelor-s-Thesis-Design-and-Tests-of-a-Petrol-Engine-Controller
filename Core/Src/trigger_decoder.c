/*===========================================================================*
 * File:        trigger_decoder.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        24.10.2021
 * Brief:       Trigger decoder
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "trigger_decoder.h"

#include "pins_assignment.h"

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

static volatile uint32_t trigd_count;

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       EXTI0
 * param[in]:   
 * param[out]:  
 * return:      
 * details:     
 *===========================================================================*/
extern void EXTI0_IRQHandler(void);
void EXTI0_IRQHandler(void)
{
    trigd_count++;
}

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: TRIGD_Init
 *===========================================================================*/
void TRIGD_Init()
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set GPIOA port 0 as input */
    GPIOA->MODER |= GPIO_MODER_MODER0;
    /* Enable pull-up for GPIOA port 0 */
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

    /* Connect PA0 pin to the EXTI0 interrupt */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    /* Don't mask EXTI0 interrupt */
    EXTI->IMR |= EXTI_IMR_IM0;
    /* Disable rising edge trigger */
    EXTI->RTSR &= ~EXTI_RTSR_TR0;
    /* Enble falling edge trigger */
    EXTI->FTSR |= EXTI_FTSR_TR0;

    EXTI_ClearPendingTrigger(EXTI_PR_PR0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
