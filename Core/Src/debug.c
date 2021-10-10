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

#include <stdio.h>
#include <stdarg.h>

char* convert(unsigned int, int);       //Convert integer number into octal, hex, etc.

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
 * param[in]:   const char* - a pointer to the string
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
static void Debug_PutS(const char* string);

/*===========================================================================*
 * brief:       
 * param[in]:   
 * param[out]:  
 * return:      
 * details:     
 *===========================================================================*/
char* convert(unsigned int num, int base);

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

    char *traverse;
    int32_t index;
    char *s; 

    va_list arg;
    va_start(arg, format);

    for (traverse = format; *traverse != '\0'; traverse++)
    {
        while (*traverse != '%')
        { 
            Debug_PutChar(*traverse);
            traverse++; 
        } 

        traverse++; 

        //Module 2: Fetching and executing arguments
        switch(*traverse) 
        {
            case 'c' : index = va_arg(arg,int);     //Fetch char argument
                        Debug_PutChar(index);
                        break;

            case 'd' : index = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(index < 0)
                        {
                            index = -index;
                            Debug_PutChar('-');
                        }
                        Debug_PutS(ITOA(index, 10));
                        break;

            case 'o': index = va_arg(arg,unsigned int); //Fetch Octal representation
                        Debug_PutS(convert(index, 8));
                        break;

            case 's': s = va_arg(arg,char *);       //Fetch string
                        Debug_PutS(s);
                        break;

            case 'x': index = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        Debug_PutS(convert(index, 16));
                        break;
        }
    }

    //Module 3: Closing argument list to necessary clean-up
    va_end(arg);

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
    int index = 0;

    while (string[index])
    {
        Debug_PutChar(string[index]);

        index++;
    }

    Debug_PutChar('\n');
}

char* convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    } while(num != 0);

    return(ptr);
}


/* end of file */
