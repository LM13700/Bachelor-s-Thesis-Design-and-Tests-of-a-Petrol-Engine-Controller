/*===========================================================================*
 * File:        swo.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        02.10.2021
 * Brief:       SWO module
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "swo.h"

#include "stdarg.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define SWO_PutChar(_CHAR_)         ITM_SendChar(_CHAR_)

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
 * brief:       SWO puts implementation
 * param[in]:   string - a pointer to the input string
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void SWO_PutS(const char* string) __attribute__((used));

/*===========================================================================*
 * brief:       Internal print function - vprintf alike
 * param[in]:   format - a string to be written to output
 * param[in]:   args - a variable arguments list
 * param[out]:  None
 * return:      None
 * details:     va_end is called at the end of the function
 *===========================================================================*/
static void SWO_InternalPrint(char* const format, va_list args) __attribute__((used));

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Swo_Init
 *===========================================================================*/
void Swo_Init(void)
{
#ifdef DEBUG

    /* Select the SWO interface */
    DBGMCU->CR &= DBGMCU_CR_TRACE_MODE_Msk;

    /* Enable the TRACE interface */
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;

#endif
}

/*===========================================================================*
 * Function: Swo_Print
 *===========================================================================*/
void Swo_Print(char* format, ...)
{
#ifdef DEBUG

    va_list args;
    va_start(args, format);

    SWO_InternalPrint(format, args);

#endif
}

/*===========================================================================*
 * Function: Swo_PrintLogInternal
 *===========================================================================*/
void Swo_PrintLogInternal(const char* moduleTag, const int codeLine, char* format, ...)
{
#ifdef DEBUG

    va_list args;
    va_start(args, format);

    Swo_Print("%s %d: ", moduleTag, codeLine);

    SWO_InternalPrint(format, args);

#endif
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: SWO_PutS
 *===========================================================================*/
static void SWO_PutS(const char* string)
{
#ifdef DEBUG
    unsigned int index = 0;

    while (string[index])
    {
        SWO_PutChar(string[index]);

        index++;
    }
#endif
}

/*===========================================================================*
 * Function: SWO_InternalPrint
 *===========================================================================*/
static void SWO_InternalPrint(char* const format, va_list args)
{
#ifdef DEBUG
    char* inputCh;
    int32_t value;

    inputCh = format;

    do
    {
        if (*inputCh != '%')
        {
            SWO_PutChar(*inputCh);
        }
        else
        {
            inputCh++;

            switch(*inputCh)
            {
                /* Decimal */
                case 'd':
                {
                    value = va_arg(args, int);
                    if (value < 0)
                    {
                        value *= -1;
                        SWO_PutChar('-');
                    }
                    SWO_PutS(ITOA(value, 10));
                    break;
                }
                case 'u':
                {
                    value = va_arg(args, uint32_t);

                    if (value < 0)
                    {
                        value *= -1;
                        SWO_PutChar('-');
                    }
                    SWO_PutS(ITOA(value, 10));
                    break;
                }

                /* Hexadecimal */
                case 'x':
                {
                    value = va_arg(args, uint32_t);
                    SWO_PutS(ITOA(value, 16));
                    break;
                }

                /* Char */
                case 'c':
                {
                    value = va_arg(args, int);
                    SWO_PutChar(value);
                    break;
                }

                /* String */
                case 's':
                {
                    SWO_PutS((char*)va_arg(args, int));
                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        inputCh++;

    } while (*inputCh != '\0');

    va_end(args);
#endif
}


/* end of file */
