/*===========================================================================*
 * File:        engine_sensors.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        23.11.2021
 * Brief:       Module containing sensors support
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "engine_sensors.h"

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

/* Sync with "ensens_sensors_data" and ADC conversion sequence */
typedef enum EnSens_DataIndex_Tag
{
    ENSENS_DATA_INDEX_MAP = 0,
    ENSENS_DATA_INDEX_IAT = 1,
    ENSENS_DATA_INDEX_CLT = 2,

    ENSENS_DATA_INDEX_COUNT
} EnSens_DataIndex_T;

/*===========================================================================*
 *
 * GLOBAL VARIABLES AND CONSTANTS SECTION
 *
 *===========================================================================*/

static uint16_t ensens_sensors_data[ENSENS_DATA_INDEX_COUNT];

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: EnSens_Init
 *===========================================================================*/
void EnSens_Init(void)
{
    /* MAP pin: PA7 */
    /* IAT pin: PA8 */
    /* CLT pin: PA9 */

    /* Enable ADC clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set port PA7 in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER7;
    /* Set port PA8 in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER8;
    /* Set port PA9 in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER9;

    /* Set ADC clock to 25[MHz] (100MHz / 4) */
    ADC->CCR |= ADC_CCR_ADCPRE_0;

    /* Set ADC resolution to 8bits */
    ADC1->CR1 |= ADC_CR1_RES_1;
    /* Enable scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;

    // /* Set single conversion mode */
    // ADC1->CR2 &= ~ADC_CR2_CONT;
    /* Enable continous conversion mode */
    ADC1->CR2 |= ADC_CR2_CONT;
    /* Set End Of Conversion flag at the end of each conversion*/
    ADC1->CR2 |= ADC_CR2_EOCS;
    /* Enable DMA */
    ADC1->CR2 |= ADC_CR2_DMA;
    // /* Disable continuous DMA requests */
    // ADC1->CR2 &= ~ADC_CR2_DDS;
    /* Enable continuous DMA requests */
    ADC1->CR2 |= ADC_CR2_DDS;
    /* Set right data alignment */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* Set channel 7 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP7;
    /* Set channel 8 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP8;
    /* Set channel 9 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP9;

    /* Set total conversion number to 3 */
    ADC1->SQR1 |= ADC_SQR1_L_1;
    /* Select regular channels sequence */
    /* Set channel 7 as 1st */
    ADC1->SQR3 |= (0x07 << ADC_SQR3_SQ1_Pos);
    /* Set channel 8 as 2nd */
    ADC1->SQR3 |= (0x08 << ADC_SQR3_SQ2_Pos);
    /* Set channel 9 as 3rd */
    ADC1->SQR3 |= (0x09 << ADC_SQR3_SQ3_Pos);

    /* Enable AD converter */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Enable DMA2 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    /* Set direction to peripherial to memory */
    DMA2_Stream0->CR &= ~DMA_SxCR_DIR;
    /* Enable circular mode */
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;
    /* Enable memory address increment */
    DMA2_Stream0->CR |= DMA_SxCR_MINC;
    /* Set peripherial data size: 16bit */
    DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;
    /* Set memory data size: 16bit */
    DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
    /* Select channel 0 (ADC) */
    DMA2_Stream0->CR &= ~DMA_SxCR_CHSEL;

    /* Set number of data registers */
    DMA2_Stream0->NDTR = ENSENS_DATA_INDEX_COUNT;
    /* Set peripherial adress */
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    /* Set memory adress */
    DMA2_Stream0->M0AR = (uint32_t)&ensens_sensors_data[0];

    /* Enable DMA */
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}

/*===========================================================================*
 * Function: EnSens_StartMeas
 *===========================================================================*/
void EnSens_StartMeas(void)
{
    /* Clear status register */
    ADC1->SR = 0x00;
    /* Start regular channels conversion */
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/*===========================================================================*
 * Function: EnSens_GetMap
 *===========================================================================*/
float EnSens_GetMap(void)
{
    return 100.0F;
}

/*===========================================================================*
 * Function: EnSens_GetIat
 *===========================================================================*/
float EnSens_GetIat(void)
{
    return 300.0F;
}

/*===========================================================================*
 * Function: EnSens_GetClt
 *===========================================================================*/
float EnSens_GetClt(EnSens_CltResultTypes_T resultType)
{
    switch (resultType)
    {
        case ENSENS_CLT_RESULT_TYPE_TEMPERATURE:
            break;
        
        case ENSENS_CLT_RESULT_TYPE_ENRICHEMENT:
            break;

        default:
            break;
    }

    return 0.0F;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
