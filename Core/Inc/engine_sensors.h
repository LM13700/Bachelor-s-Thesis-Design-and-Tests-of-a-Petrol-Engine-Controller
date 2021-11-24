/*===========================================================================*
 * File:        engine_sensors.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        23.11.2021
 * Brief:       Module containing sensors support
 *===========================================================================*/
#ifndef _ENGINE_SENSORS_H_
#define _ENGINE_SENSORS_H_

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

/*===========================================================================*
 *
 * EXPORTED TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef enum EnSens_CltResultType_Tag
{
    /* Temperature in Kelvin */
    ENSENS_CLT_RESULT_TYPE_TEMPERATURE,
    /* Enrichement percentage */
    ENSENS_CLT_RESULT_TYPE_ENRICHEMENT,

    ENSENS_CLT_RESULT_TYPE_COUNT
} EnSens_CltResultTypes_T;

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
 * brief:       Gets MAP value
 * param[in]:   None
 * param[out]:  None
 * return:      float - absolute MAP value in kPa
 * details:     MAP - Manifold Absolute Pressure
 *===========================================================================*/
float EnSens_GetMap(void);

/*===========================================================================*
 * brief:       Gets IAT value
 * param[in]:   None
 * param[out]:  None
 * return:      float - IAT value in kelvins
 * details:     IAT - Intake Air Temperature
 *===========================================================================*/
float EnSens_GetIat(void);

/*===========================================================================*
 * brief:       Gets CLT specified value
 * param[in]:   resultType - what type of result will be returned
 * param[out]:  None
 * return:      float - value described in "EnSens_CltResultTypes_T" declaration
 * details:     CLT - Coolant Temperature
 *===========================================================================*/
float EnSens_GetClt(EnSens_CltResultTypes_T resultType);

/*===========================================================================*
 * brief:       Gets oil pressure
 * param[in]:   None
 * param[out]:  None
 * return:      float - oil pressure in kPa
 * details:     None
 *===========================================================================*/
// float EnSens_GetOilPressure(void);


#endif
/* end of file */
