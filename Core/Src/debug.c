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

#include "stdarg.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define Debug_PutChar(_CHAR_)         ITM_SendChar(_CHAR_)

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
static void Debug_PutS(const char* string);

/*===========================================================================*
 * brief:       Internal print function - vprintf alike
 * param[in]:   format - a string to be written to output
 * param[in]:   args - a variable arguments list
 * param[out]:  None
 * return:      None
 * details:     va_end is called at the end of the function
 *===========================================================================*/
static void _InternalPrint(char* const format, va_list args);

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
#ifdef DEBUG

    /* Select the SWO interface */
    DBGMCU->CR &= DBGMCU_CR_TRACE_MODE_Msk;

    /* Enable the TRACE interface */
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;

#endif
}

/*===========================================================================*
 * Function: Debug_Print
 *===========================================================================*/
void Debug_Print(char* format, ...)
{
#ifdef DEBUG

    va_list args;
    va_start(args, format);

    _InternalPrint(format, args);

#endif
}

/*===========================================================================*
 * Function: Debug_PrintLogInternal
 *===========================================================================*/
void Debug_PrintLogInternal(const char* moduleTag, const int codeLine, char* format, ...)
{
#ifdef DEBUG

    va_list args;
    va_start(args, format);

    Debug_Print("%s %d: ", moduleTag, codeLine);

    _InternalPrint(format, args);

#endif
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Debug_PutS
 *===========================================================================*/
static void Debug_PutS(const char* string)
{
    unsigned int index = 0;

    while (string[index])
    {
        Debug_PutChar(string[index]);

        index++;
    }
}

/*===========================================================================*
 * Function: _InternalPrint
 *===========================================================================*/
static void _InternalPrint(char* const format, va_list args)
{
    char* inputCh;
    int32_t value;

    inputCh = format;

    do
    {
        if (*inputCh != '%')
        {
            Debug_PutChar(*inputCh);
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
                        Debug_PutChar('-');
                    }
                    Debug_PutS(ITOA(value, 10));
                    break;
                }

                /* Hexadecimal */
                case 'x':
                {
                    value = va_arg(args, unsigned int);
                    Debug_PutS(ITOA(value, 16));
                    break;
                }

                /* Char */
                case 'c':
                {
                    value = va_arg(args, int);
                    Debug_PutChar(value);
                    break;
                }

                /* String */
                case 's':
                {
                    Debug_PutS((char*)va_arg(args, int));
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
}


/* end of file */
