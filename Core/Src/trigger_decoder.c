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

#include "swo.h"

SWO_DefineModuleTag(TRIGD);

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

    /* Current engine angle in degrees, 0 means beginning of the combustion stroke */
    static volatile uint32_t engine_angle;
    /* Current engine speed in RPM */
    static volatile float engine_speed;

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
 * brief:       EXTI0 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     Sync signal conditioning ISR
 *===========================================================================*/
extern void EXTI0_IRQHandler(void);

/*===========================================================================*
 * brief:       TIM2 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     Speed signal conditioning ISR
 *===========================================================================*/
extern void TIM2_IRQHandler(void);

/*===========================================================================*
 * brief:       Initialize sync input pin
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void TRIGD_SyncPinInit(void);

/*===========================================================================*
 * brief:       Initialize speed input pin
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void TRIGD_SpeedPinInit(void);

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
    TRIGD_SyncPinInit();
    TRIGD_SpeedPinInit();
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: EXTI0_IRQHandler
 *===========================================================================*/
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetPendingTrigger(EXTI_PR_PR0))
    {
        EXTI_ClearPendingTrigger(EXTI_PR_PR0);
    }

    NVIC_ClearPendingIRQ(EXTI0_IRQn);
}

/*===========================================================================*
 * Function: TIM2_IRQHandler
 *===========================================================================*/
void TIM2_IRQHandler(void)
{
    
    NVIC_ClearPendingIRQ(TIM2_IRQn);
}

/*===========================================================================*
 * Function: TRIGD_SyncPinInit
 *===========================================================================*/
static void TRIGD_SyncPinInit(void)
{
    /* Sync input pin: PA0 */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set GPIOA port 0 as input */
    GPIOA->MODER &= ~GPIO_MODER_MODER0;
    /* Disable pull-up and pull-down for GPIOA port 0 */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;

    /* Connect PA0 pin to the EXTI0 interrupt */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    /* Don't mask EXTI0 interrupt */
    EXTI->IMR |= EXTI_IMR_IM0;
    /* Disable rising edge trigger */
    EXTI->RTSR &= ~EXTI_RTSR_TR0;
    /* Enble falling edge trigger */
    EXTI->FTSR |= EXTI_FTSR_TR0;

    /* Enable interrupt request */
    EXTI_ClearPendingTrigger(EXTI_PR_PR0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/*===========================================================================*
 * Function: TRIGD_SpeedPinInit
 *===========================================================================*/
static void TRIGD_SpeedPinInit(void)
{
    /* Speed input pin: PA1 */
    /* Speed input timer: TIM2 CH2 */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA1 afternative function 1 (TIM2_CH2) */
    GPIOA->AFR[1] |= GPIO_AFRL_AFSEL0_0;
    /* Enable TIM2 clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Set TIM2 clock prescaler to 0 (source clock divided by 0+1) */
    TIM2->PSC |= TIM_PSC_PSC_Msk;
    /* Set TIM2 auto-reload register to its max value */
    TIM2->ARR |= TIM_ARR_ARR_Msk;

    /* Link TI2 input to the capture logic */
    TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;
    /* Enable rising edge trigger */
    TIM2->CCER &= ~TIM_CCER_CC2P;
    TIM2->CCER &= ~TIM_CCER_CC2NP;
    /* Enable capture */
    TIM2->CCER |= TIM_CCER_CC2E;

    /* Enable interrupt request */
    TIM2->DIER |= TIM_DIER_CC2IE;
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
}


/* end of file */
