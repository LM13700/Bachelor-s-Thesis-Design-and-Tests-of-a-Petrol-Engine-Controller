/*===========================================================================*
 * File:        utils.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        10.10.2021
 * Brief:       Utils
 *===========================================================================*/
#ifndef _UTILS_H_
#define _UTILS_H_

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 *
 * EXPORTED DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define ITOA(_NUMBER_, _BASE_)                  Utils_Itoa(_NUMBER_, _BASE_)

/*===========================================================================*
 *
 * EXPORTED FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Converts number to string with corresponding number base
 * param[in]:   number - number to convert
 * param[in]:   base - number base
 * param[out]:  None
 * return:      char* - pointer to the output string
 * details:     None
 *===========================================================================*/
char* Utils_Itoa(unsigned int number, unsigned int base);


#endif
/* end of file */
