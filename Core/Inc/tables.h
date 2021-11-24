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

typedef enum Tables_SpeedPressure_Tag
{
    TABLES_SPEED_PRESSURE_VE,
    TABLES_SPEED_PRESSURE_SPARK,

    TABLES_SPEED_PRESSURE_COUNT
} Tables_SpeedPressure_T;

typedef enum Tables_TwoDimensions_Tag
{
    TABLES_TWO_DIMENSIONS_CLT_ENRICH,

    TABLES_TWO_DIMENSIONS_COUNT
} Tables_TwoDimensions_T;

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
 * brief:       Gets speed pressure table z-axis value for given engine speed and pressure
 * param[in]:   tableType - specifies which table will be read
 * param[in]:   speed - engine speed in [RPM]
 * param[in]:   pressure - engine pressure in [kPa]
 * param[out]:  None
 * return:      float - z-axis value
 * details:     None
 *===========================================================================*/
float Tables_GetSpeedPressureTableValue(Tables_SpeedPressure_T tableType, float speed, float pressure);

/*===========================================================================*
 * brief:       Gets 2D table value for given x-axis value
 * param[in]:   tableType - specifies which table will be read
 * param[in]:   xValue - x-axis value specific for given table type
 * param[out]:  None
 * return:      float - y-axis value
 * details:     None
 *===========================================================================*/
float Tables_GetTwoDimensionsTableValue(Tables_TwoDimensions_T tableType, float xValue);


#endif
/* end of file */
