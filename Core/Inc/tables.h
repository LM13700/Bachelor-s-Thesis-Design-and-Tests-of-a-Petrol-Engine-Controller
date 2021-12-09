/*===========================================================================*
 * File:        tables.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        16.11.2021
 * Brief:       Engine tables
 *===========================================================================*/
#ifndef _TABLES_H_
#define _TABLES_H_

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

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef enum Tables_3D_Tag
{
    /* x-axis -> engine speed in [RPM], y-axis -> engine pressure in [kPa] */
    TABLES_3D_VE,
    /* x-axis -> engine speed in [RPM], y-axis -> engine pressure in [kPa] */
    TABLES_3D_SPARK,

    TABLES_3D_COUNT
} Tables_3D_T;

typedef enum Tables_2D_Tag
{
    /* x-axis -> voltage in [mV] */
    TABLES_2D_IAT,
    /* x-axis -> voltage in [mV] */
    TABLES_2D_CLT,
    /* x-axis -> temperature [oC] */
    TABLES_2D_CLT_ENRICHEMENT,

    TABLES_2S_COUNT
} Tables_2D_T;

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
 * brief:       Gets 3D table z-axis value for given x-axis and y-axis values
 * param[in]:   tableType - specifies which table will be read
 * param[in]:   xValue - x-axis value specific for given table type
 * param[in]:   yValue - y-axis value specific for given table type
 * param[out]:  None
 * return:      float - z-axis value
 * details:     Look in Tables_3D_T for table specific axis values units
 *===========================================================================*/
float Tables_Get3DTableValue(Tables_3D_T tableType, float xValue, float yValue);

/*===========================================================================*
 * brief:       Gets 2D table value for given x-axis value
 * param[in]:   tableType - specifies which table will be read
 * param[in]:   xValue - x-axis value specific for given table type
 * param[out]:  None
 * return:      float - y-axis value
 * details:     Look in Tables_2D_T for table specific axis values units
 *===========================================================================*/
float Tables_Get2DTableValue(Tables_2D_T tableType, float xValue);


#endif
/* end of file */
