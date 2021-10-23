/*===========================================================================*
 * File:        debug.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        02.10.2021
 * Brief:       Module containing tools for debug
 *===========================================================================*/
#ifndef _DEBUG_H_
#define _DEBUG_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "common_include.h"

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define Debug_DefineModuleTag(_TAG_)    static const char module_tag[] __attribute__((used)) = #_TAG_

#ifdef __GNUC__
    #define Debug_PrintLog(...)         Debug_PrintLogInternal(module_tag, __LINE__, __VA_ARGS__)
#endif

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * EXPORTED GLOBAL VARIABLES SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * EXPORTED FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Initialize SWO interface
 * param[in]:   None
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void Debug_SwoInit(void);

/*===========================================================================*
 * brief:       SWO printf alike implementation
 * param[in]:   format - a string to be written to output
 * param[in]:   ... - additional agruments
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void Debug_Print(char* format, ...);

/*===========================================================================*
 * brief:       SWO printf wrapper allowing logging the calling module name and line num
 * param[in]:   moduleTag - a string containing module name
 * param[in]:   codeLine - the calling module line number
 * param[in]:   format - a string to be written to output
 * param[in]:   ... - additional agruments
 * param[out]:  None
 * return:      None
 * details:     It is recommended to use this function only via macro Debug_PrintLog
 *===========================================================================*/
void Debug_PrintLogInternal(const char* moduleTag, const int codeLine, char* format, ...);


#endif
/* end of file */
