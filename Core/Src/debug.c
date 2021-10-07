/*===========================================================================*
 * File:        debug.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        02.10.2021
 * Brief:       Module containing tools for debug
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "debug.h"

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
 * Function: Debug_SwoInit
 *===========================================================================*/
void Debug_SwoInit(void)
{
    /* Select the SWO interface */
    DBGMCU->CR &= DBGMCU_CR_TRACE_MODE_Msk;

    /* Enable the TRACE interface */
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
}

void SWO_Init(uint32_t portBits, uint32_t cpuCoreFreqHz)
{
    uint32_t SWOSpeed = 64000; /* default 64k baud rate */
    uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1; /* SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock */

    CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; /* enable trace in core debug */
    *((volatile unsigned *)(ITM_BASE + 0x400F0)) = 0x00000002; /* "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO NRZ, 1: SWO Manchester encoding) */
    *((volatile unsigned *)(ITM_BASE + 0x40010)) = SWOPrescaler; /* "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output */
    *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; /* ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC */
    ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk; /* ITM Trace Control Register */
    ITM->TPR = ITM_TPR_PRIVMASK_Msk; /* ITM Trace Privilege Register */
    ITM->TER = portBits; /* ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port. */
    *((volatile unsigned *)(ITM_BASE + 0x01000)) = 0x400003FE; /* DWT_CTRL */
    *((volatile unsigned *)(ITM_BASE + 0x40304)) = 0x00000100; /* Formatter and Flush Control Register */
}
/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
