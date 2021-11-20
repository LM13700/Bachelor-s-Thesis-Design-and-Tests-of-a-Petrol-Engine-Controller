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

#define IGNDRV_ENABLE_IGNITION_CHANNEL_1        (TIMER_IGNITION->CCMR1 |= TIM_CCMR1_OC2M)
#define IGNDRV_ENABLE_IGNITION_CHANNEL_2        (TIMER_IGNITION->CCMR2 |= TIM_CCMR2_OC3M)
#define IGNDRV_ENABLE_IGNITION_CHANNEL_3        (TIMER_IGNITION->CCMR2 |= TIM_CCMR2_OC4M)

#define IGNDRV_DISABLE_IGNITION_CHANNEL_1       (TIMER_IGNITION->CCMR1 &= ~TIM_CCMR1_OC2M)
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
    /* Ignition timer: TIM2 CH2(PB3)-CH3(PB10)-CH4(PB11) */

    /* Enable GPIOB clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    /* Set PB3 afternative function 1 (TIM2_CH2) */
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;
    /* Set PB10 afternative function 1 (TIM2_CH3) */
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL10_0;
    /* Set PB11 afternative function 1 (TIM2_CH4) */
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL11_0;

    /* Enable timer clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* Set timer clock prescaler to 0 (source clock divided by 0+1) */
    TIMER_IGNITION->PSC &= ~TIM_PSC_PSC;
    /* Set one-shot mode */
    TIMER_IGNITION->CR1 |= TIM_CR1_OPM;

    /* Enable interrupt requests */
    TIMER_IGNITION->DIER |= TIM_DIER_TIE;
    /* Enable update interrupt request */
    TIMER_IGNITION->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(TIM2_IRQn, 1U);
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/*===========================================================================*
 * Function: IgnDrv_PrepareIgnitionChannel
 *===========================================================================*/
void IgnDrv_PrepareIgnitionChannel(IgnDrv_IgnitionChannels_T channel, float fireAngle, float startAngle)
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

    /* tdelay = TIMx_CCR */
    /* tpulse = TIMx_ARR - TIMx_CCR + 1 */

    /* Set total time: tdelay + tpulse - 1 = TIMx_ARR */
    TIMER_IGNITION->ARR = Utils_FloatToUint32((angleDifference / ENCON_ONE_TRIGGER_PULSE_ANGLE) * engineSpeedRaw) - 1U;

    /* TIMx_CCR = TIMx_ARR + 1 - tpulse */
    tmpDelay = (TIMER_IGNITION->ARR + 1U) - TIMER_MS_TO_TIMER_REG_VALUE(ENCON_COILS_DWELL_TIME_MS);

    switch (channel)
    {
        case IGNDRV_CHANNEL_1:
            TIMER_IGNITION->CCR2 |= tmpDelay;
            break;

        case IGNDRV_CHANNEL_2:
            TIMER_IGNITION->CCR3 |= tmpDelay;
            break;

        case IGNDRV_CHANNEL_3:
            TIMER_IGNITION->CCR4 |= tmpDelay;
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
void IgnDrv_StartIgnitionModule(IgnDrv_IgnitionChannels_T channel)
{
    switch (channel)
    {
        case IGNDRV_CHANNEL_1:
            IGNDRV_ENABLE_IGNITION_CHANNEL_1;
            break;

        case IGNDRV_CHANNEL_2:
            IGNDRV_ENABLE_IGNITION_CHANNEL_2;
            break;

        case IGNDRV_CHANNEL_3:
            IGNDRV_ENABLE_IGNITION_CHANNEL_3;
            break;

        default:
            break;
    }

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
        IGNDRV_DISABLE_IGNITION_CHANNEL_1;
        IGNDRV_DISABLE_IGNITION_CHANNEL_2;
        IGNDRV_DISABLE_IGNITION_CHANNEL_3;
    }
    else
    {
        /* Do nothing */
    }
}


/* end of file */
