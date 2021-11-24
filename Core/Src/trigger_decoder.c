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
#include "timers.h"

SWO_DefineModuleTag(TRIGD);

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define TRIGD_SPEED_TIMER_REGISTER              (TIMER_SPEED->CCR2)

/*===========================================================================*
 *
 * LOCAL TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

/* Current engine angle in degrees, 0 means beginning of the first piston combustion stroke */
static volatile float trigd_engine_angle;
/* Flag set when sync signal was received */
static volatile bool trigd_is_sync_pending;

static volatile Trigd_IsrCallback trigd_trigger_callback;

extern bool main_is_speed_trigger_occured;

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
static void TrigD_SyncPinInit(void);

/*===========================================================================*
 * brief:       Initialize speed input pin
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void TrigD_SpeedPinInit(void);

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: TrigD_Init
 *===========================================================================*/
void TrigD_Init(Trigd_IsrCallback callback)
{
    trigd_engine_angle = ENCON_ANGLE_UNKNOWN;
    trigd_is_sync_pending = false;

    if (callback != NULL)
    {
        trigd_trigger_callback = callback;
    }

    TrigD_SyncPinInit();
    TrigD_SpeedPinInit();
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

    main_is_speed_trigger_occured = true;

    /* Capture interrupt */
    if (TIMER_SPEED->SR & TIM_SR_CC2IF)
    {
        /* Start scheduled actions */
        trigd_trigger_callback();

        /* Clear interrupt flag */
        TIMER_SPEED->SR &= ~TIM_SR_CC2IF;

        if (isLastValueCaptured)
        {
            EnCon_UpdateEngineSpeed(UTILS_CIRCULAR_DIFFERENCE(TRIGD_SPEED_TIMER_REGISTER, lastCapturedValue,
                                    TIMER_SPEED_TIMER_MAX_VAL));
        }

        if (trigd_is_sync_pending)
        {
            trigd_engine_angle = ENCON_TRIGGER_ANGLE;
            trigd_is_sync_pending = false;
        }
        else
        {
            if (trigd_engine_angle != ENCON_ANGLE_UNKNOWN)
            {
                trigd_engine_angle += (ENCON_ONE_TRIGGER_PULSE_ANGLE);
                if (trigd_engine_angle >= ENCON_ENGINE_FULL_CYCLE_ANGLE)
                {
                    trigd_engine_angle -= ENCON_ENGINE_FULL_CYCLE_ANGLE;
                }
            }
        }

        EnCon_UpdateEngineAngle(trigd_engine_angle);
        lastCapturedValue = TRIGD_SPEED_TIMER_REGISTER;
        isLastValueCaptured = true;
    }
    /* Overflow interrupt */
    else if (TIMER_SPEED->SR & TIM_SR_UIF)
    {
        /* Clear interrupt flag */
        TIMER_SPEED->SR &= ~TIM_SR_UIF;
        isLastValueCaptured = false;
        trigd_engine_angle = ENCON_ANGLE_UNKNOWN;
        EnCon_UpdateEngineAngle(trigd_engine_angle);
        EnCon_UpdateEngineSpeed(ENCON_SPEED_RAW_UNKNOWN);
    }
    else
    {
        /* Do nothing*/
    }
}

/*===========================================================================*
 * Function: TrigD_SyncPinInit
 *===========================================================================*/
static void TrigD_SyncPinInit(void)
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
    NVIC_SetPriority(EXTI0_IRQn, 1U);
    EXTI_ClearPendingTrigger(EXTI_PR_PR0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/*===========================================================================*
 * Function: TrigD_SpeedPinInit
 *===========================================================================*/
static void TrigD_SpeedPinInit(void)
{
    /* Speed input pin: PA6 */
    /* Speed input timer: TIM3 CH1 */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA6 afternative function 2 (TIM3_CH1) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
    /* Enable timer clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    /* Set timer clock prescaler to 0 (source clock divided by 0+1) */
    TIMER_SPEED->PSC &= ~TIM_PSC_PSC;
    /* Set timer auto-reload register to its max value */
    TIMER_SPEED->ARR |= TIM_ARR_ARR;

    /* Link timer input to the capture logic */
    TIMER_SPEED->CCMR1 |= TIM_CCMR1_CC2S_0;
    /* Enable rising edge trigger */
    TIMER_SPEED->CCER &= ~TIM_CCER_CC2P;
    TIMER_SPEED->CCER &= ~TIM_CCER_CC2NP;
    /* Enable capture */
    TIMER_SPEED->CCER |= TIM_CCER_CC2E;

    /* Enable interrupt requests */
    TIMER_SPEED->DIER |= TIM_DIER_TIE;
    /* Enable capture interrupt request */
    TIMER_SPEED->DIER |= TIM_DIER_CC2IE;
    /* Enable update interrupt request */
    TIMER_SPEED->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(TIM3_IRQn, 0U);
    NVIC_ClearPendingIRQ(TIM3_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);

    /* Start the timer */
    TIMER_SPEED->CR1 |= TIM_CR1_CEN;
}


/* end of file */
