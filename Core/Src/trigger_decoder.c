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

#include "engine_constants.h"
#include "swo.h"

SWO_DefineModuleTag(TRIGD);

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define TRIGD_SPEED_TIMER_MAX_VAL               (UINT32_MAX)
#define TRIGD_SPEED_TIMER_REGISTER              (TIM3->CCR2)

#define TRIGD_SECONDS_IN_MINUTE                 (60.0F)
#define TRIGD_CALCULATE_RPM(_TIM_REG_VALUE_)    (TRIGD_SECONDS_IN_MINUTE * ((_TIM_REG_VALUE_) / SystemCoreClock) *    \
                                                 ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO)

/*===========================================================================*
 *
 * LOCAL TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

/* Current engine angle in degrees, 0 means beginning of the first piston combustion stroke */
static volatile float trigd_engine_angle;
/* Current engine speed in RPM */
static volatile float trigd_engine_speed;
/* Flag set when sync signal was received */
static volatile bool trigd_is_sync_pending;

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
 * brief:       TIM3 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     Speed signal conditioning ISR
 *===========================================================================*/
extern void TIM3_IRQHandler(void);

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
    trigd_engine_angle = TRIGD_ANGLE_UNKNOWN;
    trigd_engine_speed = 0.0F;
    trigd_is_sync_pending = false;

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
        trigd_is_sync_pending= true;
        EXTI_ClearPendingTrigger(EXTI_PR_PR0);
    }
}

/*===========================================================================*
 * Function: TIM3_IRQHandler
 *===========================================================================*/
void TIM3_IRQHandler(void)
{
    static bool isLastValueCaptured = false;
    static uint32_t lastCapturedValue;

    /* Disable interrupts */
    __disable_irq();

    /* Capture interrupt */
    if (TIM3->SR & TIM_SR_CC2IF)
    {
        /* Clear interrupt flag */
        TIM3->SR &= ~TIM_SR_CC2IF;

        if (isLastValueCaptured)
        {
            if (TRIGD_SPEED_TIMER_REGISTER >= lastCapturedValue)
            {
                trigd_engine_speed = TRIGD_CALCULATE_RPM(TRIGD_SPEED_TIMER_REGISTER - lastCapturedValue);
            }
            else
            {
                trigd_engine_speed = TRIGD_CALCULATE_RPM((TRIGD_SPEED_TIMER_MAX_VAL - lastCapturedValue) +
                                                         TRIGD_SPEED_TIMER_REGISTER);
            }
        }

        if (trigd_is_sync_pending)
        {
            trigd_engine_angle = ENGINE_CONST_TRIGGER_ANGLE;
            trigd_is_sync_pending = false;
        }
        else
        {
            if (trigd_engine_angle != TRIGD_ANGLE_UNKNOWN)
            {
                trigd_engine_angle += (ENGINE_CONST_ENGINE_ONE_ROTATION_ANGLE / ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO);
                if (trigd_engine_angle >= ENGINE_CONST_ENGINE_FULL_CYCLE_ANGLE)
                {
                    trigd_engine_angle -= ENGINE_CONST_ENGINE_FULL_CYCLE_ANGLE;
                }
            }
        }

        lastCapturedValue = TRIGD_SPEED_TIMER_REGISTER;
        isLastValueCaptured = true;
    }
    /* Overflow interrupt */
    else if (TIM3->SR & TIM_SR_UIF)
    {
        /* Clear interrupt flag */
        TIM3->SR &= ~TIM_SR_UIF;
        isLastValueCaptured = false;
        trigd_engine_angle = TRIGD_ANGLE_UNKNOWN;
    }
    else
    {
        /* Do nothing*/
    }

    /* Enable interrupts */
    __enable_irq();
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
    /* Speed input pin: PA6 */
    /* Speed input timer: TIM3 CH1 */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA6 afternative function 2 (TIM3_CH1) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
    /* Enable TIM3 clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    /* Set TIM3 clock prescaler to 0 (source clock divided by 0+1) */
    TIM3->PSC |= TIM_PSC_PSC_Msk;
    /* Set TIM3 auto-reload register to its max value */
    TIM3->ARR |= TIM_ARR_ARR_Msk;

    /* Link TIM3 input to the capture logic */
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;
    /* Enable rising edge trigger */
    TIM3->CCER &= ~TIM_CCER_CC2P;
    TIM3->CCER &= ~TIM_CCER_CC2NP;
    /* Enable capture */
    TIM3->CCER |= TIM_CCER_CC2E;

    /* Enable interrupt request */
    TIM3->DIER |= TIM_DIER_CC2IE;
    NVIC_ClearPendingIRQ(TIM3_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
}


/* end of file */
