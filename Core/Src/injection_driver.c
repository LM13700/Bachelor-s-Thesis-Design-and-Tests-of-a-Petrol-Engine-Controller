/*===========================================================================*
 * File:        injection_driver.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        14.11.2021
 * Brief:       Fuel injectors driver module
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "injection_driver.h"

#include "engine_constants.h"
#include "timers.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define INJDRV_ENABLE_INJECTION_CHANNEL_1        (TIMER_INJECTOR->CCMR1 |= TIM_CCMR1_OC1M)
#define INJDRV_ENABLE_INJECTION_CHANNEL_2        (TIMER_INJECTOR->CCMR1 |= TIM_CCMR1_OC2M)
#define INJDRV_ENABLE_INJECTION_CHANNEL_3        (TIMER_INJECTOR->CCMR2 |= TIM_CCMR2_OC3M)

#define INJDRV_DISABLE_INJECTION_CHANNEL_1       (TIMER_INJECTOR->CCMR1 &= ~TIM_CCMR1_OC1M)
#define INJDRV_DISABLE_INJECTION_CHANNEL_2       (TIMER_INJECTOR->CCMR1 &= ~TIM_CCMR1_OC2M)
#define INJDRV_DISABLE_INJECTION_CHANNEL_3       (TIMER_INJECTOR->CCMR2 &= ~TIM_CCMR2_OC3M)

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
 * brief:       TIM5 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     Injection end event interrupt
 *===========================================================================*/
extern void TIM5_IRQHandler(void);

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: InjDrv_Init
 *===========================================================================*/
void InjDrv_Init(void)
{
    /* Injection timer: TIM5 CH1(PA0)-CH2(PA1)-CH3(PA2) */

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA0 afternative function 2 (TIM5_CH1) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;
    /* Set PA1 afternative function 2 (TIM5_CH2) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL1_1;
    /* Set PA2 afternative function 2 (TIM5_CH3) */
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;

    /* Enable timer clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    /* Set timer clock prescaler to 0 (source clock divided by 0+1) */
    TIMER_INJECTOR->PSC &= ~TIM_PSC_PSC;
    /* Set one-shot mode */
    TIMER_INJECTOR->CR1 |= TIM_CR1_OPM;

    /* Enable interrupt requests */
    TIMER_INJECTOR->DIER |= TIM_DIER_TIE;
    /* Enable update interrupt request */
    TIMER_INJECTOR->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(TIM5_IRQn, 1U);
    NVIC_ClearPendingIRQ(TIM5_IRQn);
    NVIC_EnableIRQ(TIM5_IRQn);
}

/*===========================================================================*
 * Function: InjDrv_PrepareInjectionChannel
 *===========================================================================*/
void InjDrv_PrepareInjectionChannel(InjDrv_InjectionChannels_T channel, float injAngle, float startAngle,
                                    float injOpenTimeMs)
{
    float engineSpeedRaw;
    float angleDifference;
    uint32_t tmpDelay;

    if ((injAngle > ENCON_ENGINE_FULL_CYCLE_ANGLE) || (startAngle > ENCON_ENGINE_FULL_CYCLE_ANGLE) ||
        (injAngle < 0.0F) || (startAngle < 0.0F) || (injOpenTimeMs < 0.0F))
    {
        goto injdrv_prepare_injection_channel_exit;
    }

    engineSpeedRaw = EnCon_GetEngineSpeedRaw();
    angleDifference = UTILS_CIRCULAR_DIFFERENCE(injAngle, startAngle, ENCON_ENGINE_FULL_CYCLE_ANGLE);

    /* tdelay = TIMx_CCR */
    /* injOpenTimeMs <=> tpulse = TIMx_ARR - TIMx_CCR + 1 */

    /* Set total time: tdelay + tpulse - 1 = TIMx_ARR */
    TIMER_INJECTOR->ARR = Utils_FloatToUint32((angleDifference / ENCON_ONE_TRIGGER_PULSE_ANGLE) * engineSpeedRaw) +
                          TIMER_MS_TO_TIMER_REG_VALUE(injOpenTimeMs) - 1U;

    /* TIMx_CCR = TIMx_ARR + 1 - tpulse */
    tmpDelay = (TIMER_INJECTOR->ARR + 1U) - TIMER_MS_TO_TIMER_REG_VALUE(injOpenTimeMs);

    switch (channel)
    {
        case INJDRV_CHANNEL_1:
            TIMER_INJECTOR->CCR2 |= tmpDelay;
            break;

        case INJDRV_CHANNEL_2:
            TIMER_INJECTOR->CCR3 |= tmpDelay;
            break;

        case INJDRV_CHANNEL_3:
            TIMER_INJECTOR->CCR4 |= tmpDelay;
            break;

        default:
            break;
    }

injdrv_prepare_injection_channel_exit:

    return;
}

/*===========================================================================*
 * Function: InjDrv_StartInjectionModule
 *===========================================================================*/
void InjDrv_StartInjectionModule(InjDrv_InjectionChannels_T channel)
{
    switch (channel)
    {
        case INJDRV_CHANNEL_1:
            INJDRV_ENABLE_INJECTION_CHANNEL_1;
            break;

        case INJDRV_CHANNEL_2:
            INJDRV_ENABLE_INJECTION_CHANNEL_2;
            break;

        case INJDRV_CHANNEL_3:
            INJDRV_ENABLE_INJECTION_CHANNEL_3;
            break;

        default:
            break;
    }

    /* Start the timer */
    TIMER_INJECTOR->CR1 |= TIM_CR1_CEN;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: TIM5_IRQHandler
 *===========================================================================*/
extern void TIM5_IRQHandler(void)
{
    /* Overflow interrupt */
    if (TIMER_INJECTOR->SR & TIM_SR_UIF)
    {
        /* Clear interrupt flag */
        TIMER_INJECTOR->SR &= ~TIM_SR_UIF;
        /* Disable all injection channels */
        INJDRV_DISABLE_INJECTION_CHANNEL_1;
        INJDRV_DISABLE_INJECTION_CHANNEL_2;
        INJDRV_DISABLE_INJECTION_CHANNEL_3;
    }
    else
    {
        /* Do nothing */
    }
}


/* end of file */
