/*===========================================================================*
 * File:        engine_sensors.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        23.11.2021
 * Brief:       Module containing sensors support
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "engine_sensors.h"

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
 * Function: EnSens_GetMap
 *===========================================================================*/
float EnSens_GetMap(void)
{
    return 100.0F;
}

/*===========================================================================*
 * Function: EnSens_GetIat
 *===========================================================================*/
float EnSens_GetIat(void)
{
    return 300.0F;
}

/*===========================================================================*
 * Function: EnSens_GetClt
 *===========================================================================*/
float EnSens_GetClt(EnSens_CltResultTypes_T resultType)
{
    switch (resultType)
    {
        case ENSENS_CLT_RESULT_TYPE_TEMPERATURE:
            break;
        
        case ENSENS_CLT_RESULT_TYPE_ENRICHEMENT:
            break;

        default:
            break;
    }

    return 1.0F;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/


/* end of file */
