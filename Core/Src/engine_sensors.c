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

#include "tables.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define ENSENS_ADC_12_BIT_RESOLUTION            (4096U)
#define ENSENS_ADC_REFERENCE_VOLTAGE_V          (3.3F)
#define ENSENS_ADC_REFERENCE_VOLTAGE_MV         (3300.0F)

#define ENSENS_ADC_CALCULATE_VOLTAGE_MV(_REG_)  ((((float)(_REG_)) / ENSENS_ADC_12_BIT_RESOLUTION) *        \
                                                 ENSENS_ADC_REFERENCE_VOLTAGE_MV)

/* Pierburg 7.18222.01.0 MAP sensor */
#define ENSENS_CALCULATE_MAP_KPA(_MV_)          ((((float)(_MV_)) * 0.0280643351F + 10.1656376410F))

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

static volatile uint16_t ensens_sensors_data[ENSENS_DATA_INDEX_COUNT];

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       ADC Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
extern void ADC_IRQHandler(void);

/*===========================================================================*
 * brief:       DMA2 Stream0 Interrupt request handler
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
extern void DMA2_Stream0_IRQHandler(void);

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
    /* MAP pin: PA5 (ADC1_5) */
    /* IAT pin: PA7 (ADC1_7) */
    /* CLT pin: PB0 (ADC1_8) */

    /* Enable ADC clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Enable GPIOB clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    /* Set port PA5 in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER5;
    /* Set port PA7 in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER7;
    /* Set port PB0 in analog mode */
    GPIOB->MODER |= GPIO_MODER_MODER0;

    /* Set ADC clock to 25[MHz] (100MHz / 4) */
    ADC->CCR |= ADC_CCR_ADCPRE_0;

    /* Set ADC resolution to 12bits */
    ADC1->CR1 &= ~ADC_CR1_RES;
    /* Enable scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;

    /* Set single conversion mode */
    ADC1->CR2 &= ~ADC_CR2_CONT;
    /* Set End Of Conversion flag at the end of each conversion*/
    ADC1->CR2 |= ADC_CR2_EOCS;
    /* Set right data alignment */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    /* Enable DMA */
    ADC1->CR2 |= ADC_CR2_DMA;
    /* Disable continuous DMA requests */
    ADC1->CR2 &= ~ADC_CR2_DDS;
    /* Enable continuous DMA requests */
    ADC1->CR2 |= ADC_CR2_DDS;

    /* Set channel 5 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP5;
    /* Set channel 7 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP7;
    /* Set channel 8 sample time to 3 cycles */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP8;

    /* Set total conversion number to 3 */
    ADC1->SQR1 |= ADC_SQR1_L_1;

    /* Select regular channels sequence */
    /* Set channel 5 as 1st */
    ADC1->SQR3 |= (0x05 << ADC_SQR3_SQ1_Pos);
    /* Set channel 7 as 2nd */
    ADC1->SQR3 |= (0x07 << ADC_SQR3_SQ2_Pos);
    /* Set channel 8 as 3rd */
    ADC1->SQR3 |= (0x08 << ADC_SQR3_SQ3_Pos);

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
 * Function: EnSens_StartMeasurement
 *===========================================================================*/
void EnSens_StartMeasurement(void)
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
    return ENSENS_CALCULATE_MAP_KPA(ENSENS_ADC_CALCULATE_VOLTAGE_MV(ensens_sensors_data[ENSENS_DATA_INDEX_MAP]));
}

/*===========================================================================*
 * Function: EnSens_GetIat
 *===========================================================================*/
float EnSens_GetIat(void)
{
    float iatCelsius;

    iatCelsius = Tables_Get2DTableValue(TABLES_2D_IAT,
                                        ENSENS_ADC_CALCULATE_VOLTAGE_MV(ensens_sensors_data[ENSENS_DATA_INDEX_IAT]));

    return UTILS_CONVERT_C_TO_K(iatCelsius);
}

/*===========================================================================*
 * Function: EnSens_GetClt
 *===========================================================================*/
float EnSens_GetClt(EnSens_CltResultTypes_T resultType)
{
    float result;
    float cltCelsius;

    cltCelsius = Tables_Get2DTableValue(TABLES_2D_CLT,
                                        ENSENS_ADC_CALCULATE_VOLTAGE_MV(ensens_sensors_data[ENSENS_DATA_INDEX_CLT]));

    switch (resultType)
    {
        case ENSENS_CLT_RESULT_TYPE_TEMPERATURE:
            result = UTILS_CONVERT_C_TO_K(cltCelsius);
            break;
        
        case ENSENS_CLT_RESULT_TYPE_ENRICHEMENT:
            result = Tables_Get2DTableValue(TABLES_2D_CLT_ENRICHEMENT, cltCelsius);
            break;

        default:
            result = 0.0F;
            break;
    }

    return result;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: ADC_IRQHandler
 *===========================================================================*/
extern void ADC_IRQHandler(void)
{
    /* ADC overrun occured */
    if (ADC1->SR & ADC_SR_OVR)
    {
        /* Reinitialize number of data registers */
        DMA2_Stream0->NDTR = ENSENS_DATA_INDEX_COUNT;
        /* Reinitialize peripherial adress */
        DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
        /* Reinitialize memory adress */
        DMA2_Stream0->M0AR = (uint32_t)&ensens_sensors_data[0];
        /* Clean overrun bit */
        ADC1->SR &= ~ADC_SR_OVR;
    }
}

/*===========================================================================*
 * Function: DMA2_Stream0_IRQHandler
 *===========================================================================*/
extern void DMA2_Stream0_IRQHandler(void)
{
    
}


/* end of file */
