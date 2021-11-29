/*===========================================================================*
 * File:        ignition_driver.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        09.11.2021
 * Brief:       Ignition coils driver module
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "ignition_driver.h"

#include "engine_constants.h"
#include "timers.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define IGNDRV_ENABLE_IGNITION_CHANNEL_1        (TIMER_IGNITION->CCMR1 |= TIM_CCMR1_OC1M)
#define IGNDRV_ENABLE_IGNITION_CHANNEL_2        (TIMER_IGNITION->CCMR2 |= TIM_CCMR2_OC3M)
#define IGNDRV_ENABLE_IGNITION_CHANNEL_3        (TIMER_IGNITION->CCMR2 |= TIM_CCMR2_OC4M)

#define IGNDRV_DISABLE_IGNITION_CHANNEL_1       (TIMER_IGNITION->CCMR1 &= ~TIM_CCMR1_OC1M)
#define IGNDRV_DISABLE_IGNITION_CHANNEL_2       (TIMER_IGNITION->CCMR2 &= ~TIM_CCMR2_OC3M)
#define IGNDRV_DISABLE_IGNITION_CHANNEL_3       (TIMER_IGNITION->CCMR2 &= ~TIM_CCMR2_OC4M)

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
 * brief:       TIM2 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     Ignition end event interrupt
 *===========================================================================*/
extern void TIM2_IRQHandler(void);

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: IgnDrv_Init
 *===========================================================================*/
void IgnDrv_Init(void)
{
    /* Ignition timer: TIM2 CH1(PA15)-CH3(PB10)-CH4(PA3) */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Enable GPIOB clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    /* Disable pull-up and pull-down for GPIOA port 15 */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;
    /* Set PA15 afternative function 1 (TIM2_CH1) */
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL15_0;
    /* Set PB10 afternative function 1 (TIM2_CH3) */
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL10_0;
    /* Set PA3 afternative function 1 (TIM2_CH4) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
    /* Set PA15 Alternative function mode */
    GPIOA->MODER |= GPIO_MODER_MODE15_1;
    /* Set PB10 Alternative function mode */
    GPIOB->MODER |= GPIO_MODER_MODE10_1;
    /* Set PA3 Alternative function mode */
    GPIOA->MODER |= GPIO_MODER_MODE3_1;

    /* Enable timer clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* Set timer clock prescaler to 0 (source clock divided by 0+1) */
    TIMER_IGNITION->PSC = (uint16_t)0U;
    /* Select the up counting mode */
    TIMER_IGNITION->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    /* Enable ignition channels outputs */
    TIMER_IGNITION->CCER |= TIM_CCER_CC1E;
    TIMER_IGNITION->CCER |= TIM_CCER_CC3E;
    TIMER_IGNITION->CCER |= TIM_CCER_CC4E;
    /* Set one-shot mode */
    TIMER_IGNITION->CR1 |= TIM_CR1_OPM;

    /* Enable interrupt requests */
    TIMER_IGNITION->DIER |= TIM_DIER_TIE;
    /* Enable update interrupt request */
    TIMER_IGNITION->DIER |= TIM_DIER_UIE;

#ifdef DEBUG
    /* Stop timer when core is halted in debug */
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
#endif

    NVIC_SetPriority(TIM2_IRQn, 1U);
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/*===========================================================================*
 * Function: IgnDrv_PrepareIgnitionChannel
 *===========================================================================*/
void IgnDrv_PrepareIgnitionChannel(EnCon_CylinderChannels_T channel, float fireAngle, float startAngle)
{
    float engineSpeedRaw;
    float angleDifference;
    uint32_t tmpDelay;

    if ((fireAngle > ENCON_ENGINE_FULL_CYCLE_ANGLE) || (startAngle > ENCON_ENGINE_FULL_CYCLE_ANGLE))
    {
        goto igndrv_prepare_ignition_channel_exit;
    }

    engineSpeedRaw = EnCon_GetEngineSpeedRaw();
    angleDifference = UTILS_CIRCULAR_DIFFERENCE(fireAngle, startAngle, ENCON_ENGINE_FULL_CYCLE_ANGLE);

    /* Make sure counter register is reset */
    TIMER_IGNITION->CNT = 0U;

    /* tdelay = TIMx_CCR */
    /* tpulse = TIMx_ARR - TIMx_CCR + 1 */

    /* Set total time: tdelay + tpulse - 1 = TIMx_ARR */
    TIMER_IGNITION->ARR = Utils_FloatToUint32((angleDifference / ENCON_ONE_TRIGGER_PULSE_ANGLE) *
                                              TIMER_SPEED_TIM_MULTIPLIER * engineSpeedRaw) - 1U;

    /* TIMx_CCR = TIMx_ARR + 1 - tpulse */
    tmpDelay = (TIMER_IGNITION->ARR + 1U) - TIMER_MS_TO_TIMER_REG_VALUE(ENCON_COILS_DWELL_TIME_MS);

    switch (channel)
    {
        case ENCON_CHANNEL_1:
            TIMER_IGNITION->CCR1 = tmpDelay;
            IGNDRV_ENABLE_IGNITION_CHANNEL_1;
            IGNDRV_DISABLE_IGNITION_CHANNEL_2;
            IGNDRV_DISABLE_IGNITION_CHANNEL_3;
            break;

        case ENCON_CHANNEL_2:
            TIMER_IGNITION->CCR3 = tmpDelay;
            IGNDRV_ENABLE_IGNITION_CHANNEL_2;
            IGNDRV_DISABLE_IGNITION_CHANNEL_1;
            IGNDRV_DISABLE_IGNITION_CHANNEL_3;
            break;

        case ENCON_CHANNEL_3:
            TIMER_IGNITION->CCR4 = tmpDelay;
            IGNDRV_ENABLE_IGNITION_CHANNEL_3;
            IGNDRV_DISABLE_IGNITION_CHANNEL_1;
            IGNDRV_DISABLE_IGNITION_CHANNEL_2;
            break;

        default:
            break;
    }

igndrv_prepare_ignition_channel_exit:

    return;
}

/*===========================================================================*
 * Function: IGNDRV_StartIgnitionModule
 *===========================================================================*/
void IgnDrv_StartIgnitionModule(void)
{
    /* Start the timer */
    TIMER_IGNITION->CR1 |= TIM_CR1_CEN;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: TIM2_IRQHandler
 *===========================================================================*/
extern void TIM2_IRQHandler(void)
{
    /* Overflow interrupt */
    if (TIMER_IGNITION->SR & TIM_SR_UIF)
    {
        /* Clear interrupt flag */
        TIMER_IGNITION->SR &= ~TIM_SR_UIF;
        /* Disable all ignition channels */
        // IGNDRV_DISABLE_IGNITION_CHANNEL_1;
        // IGNDRV_DISABLE_IGNITION_CHANNEL_2;
        // IGNDRV_DISABLE_IGNITION_CHANNEL_3;
    }
    else
    {
        /* Do nothing */
    }
}


/* end of file */
