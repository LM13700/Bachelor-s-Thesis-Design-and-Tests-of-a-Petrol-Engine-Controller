/*===========================================================================*
 * File:        utils.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        10.10.2021
 * Brief:       Utils
 *===========================================================================*/
#ifndef _UTILS_H_
#define _UTILS_H_

#define ITOA(_NUMBER_, _BASE_)                  Utils_Itoa(_NUMBER_, _BASE_)


char* Utils_Itoa(unsigned int num, int base);

#endif
/* end of file */
