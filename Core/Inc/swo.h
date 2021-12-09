/*===========================================================================*
 * File:        swo.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        02.10.2021
 * Brief:       SWO module
 *===========================================================================*/
#ifndef _SWO_H_
#define _SWO_H_

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

#define Swo_DefineModuleTag(_TAG_)    static const char module_tag[] __attribute__((used)) = #_TAG_

#ifdef __GNUC__
    #define Swo_PrintLog(...)         Swo_PrintLogInternal(module_tag, __LINE__, __VA_ARGS__)
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
void Swo_Init(void);

/*===========================================================================*
 * brief:       SWO printf alike implementation
 * param[in]:   format - a string to be written to output
 * param[in]:   ... - additional agruments
 * param[out]:  None
 * return:      None
 * details:     None
 *===========================================================================*/
void Swo_Print(char* format, ...) __attribute__ ((__format__(printf, 1, 2)));

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
void Swo_PrintLogInternal(const char* moduleTag, const int codeLine, char* format, ...)
                          __attribute__ ((__format__(printf, 3, 4)));


#endif
/* end of file */
