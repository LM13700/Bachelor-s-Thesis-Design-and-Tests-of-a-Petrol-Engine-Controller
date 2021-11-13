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

#define IGNDRV_MS_IN_S                          (1000.0F)
#define IGNDRV_MS_TO_TIMER_RESOLUTION(_MS_)     (((_MS_) / IGNDRV_MS_IN_S) * (float)TIMER_TIM_CLOCK)

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
 * Function: IGNDRV_Init
 *===========================================================================*/
void IGNDRV_Init(void)
{
    /* Ignition timer: TIM2 CH2(PA1)-CH3(PA2)-CH4(PA3) */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA1 afternative function 1 (TIM2_CH2) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL1_0;
    /* Set PA2 afternative function 1 (TIM2_CH3) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
    /* Set PA3 afternative function 1 (TIM2_CH4) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;

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

    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/*===========================================================================*
 * Function: IGNDRV_PrepareIgnitionChannel
 *===========================================================================*/
void IGNDRV_PrepareIgnitionChannel(IgDrv_IgnitionChannels_T channel, float fireAngle, float startAngle)
{
    float engineSpeedRaw;
    float angleDifference;

    engineSpeedRaw = ENGCON_GetEngineSpeedRaw();
    angleDifference = UTILS_CIRCULAR_DIFFERENCE(fireAngle, startAngle, ENGINE_CONST_ENGINE_FULL_CYCLE_ANGLE);

    /* tdelay = TIMx_CCR */
    /* tpulse = TIMx_ARR - TIMx_CCR + 1 */

    /* Set total time: tdelay + tpulse - 1 = TIMx_ARR */
    TIMER_IGNITION->ARR = Utils_FloatToUint32(((angleDifference / ENGINE_CONST_ONE_TRIGGER_PULSE_ANGLE) *
                                               engineSpeedRaw) - 1.0F);

    /* Set Compare register for proper channel */
    switch (channel)
    {
        case IGNDRV_CHANNEL_1:
            TIMER_IGNITION->CCR2 |= (TIMER_IGNITION->ARR + 1U) - Utils_FloatToUint32(IGNDRV_MS_TO_TIMER_RESOLUTION
                                     (ENGINE_CONST_IGINITON_DWELL_TIME_MS));
            break;

        case IGNDRV_CHANNEL_2:
            TIMER_IGNITION->CCR3 |= (TIMER_IGNITION->ARR + 1U) - Utils_FloatToUint32(IGNDRV_MS_TO_TIMER_RESOLUTION
                                     (ENGINE_CONST_IGINITON_DWELL_TIME_MS));
            break;

        case IGNDRV_CHANNEL_3:
            TIMER_IGNITION->CCR4 |= (TIMER_IGNITION->ARR + 1U) - Utils_FloatToUint32(IGNDRV_MS_TO_TIMER_RESOLUTION
                                     (ENGINE_CONST_IGINITON_DWELL_TIME_MS));
            break;

        default:
            break;
    }
}

/*===========================================================================*
 * Function: IGNDRV_StartIgnitionModule
 *===========================================================================*/
void IGNDRV_StartIgnitionModule(IgDrv_IgnitionChannels_T channel)
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

    /* Start timer */
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
