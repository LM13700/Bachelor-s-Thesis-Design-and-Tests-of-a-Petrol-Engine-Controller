/*===========================================================================*
 * File:        tables.c
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        17.11.2021
 * Brief:       Engine tables
 *===========================================================================*/

/*===========================================================================*
 *
 * INCLUDE SECTION
 *
 *===========================================================================*/

#include "tables.h"

/*===========================================================================*
 *
 * DEFINES AND MACRO SECTION
 *
 *===========================================================================*/

#define TABLES_SPEED_PRESSURE_ROWS                      (16u)
#define TABLES_SPEED_PRESSURE_COLUMNS                   (16u)

#define TABLES_SPEED_PRESSURE_LAST_INDEX                (TABLES_SPEED_PRESSURE_COLUMNS - 1U)
#define TABLES_FIRST_INDEX                              (0U)

/*===========================================================================*
 *
 * LOCAL TYPES AND ENUMERATION SECTION
 *
 *===========================================================================*/

typedef struct Tables_3dTable_Tag
{
    const float zTable[TABLES_SPEED_PRESSURE_ROWS][TABLES_SPEED_PRESSURE_COLUMNS];
    const float xTable[TABLES_SPEED_PRESSURE_COLUMNS];
    const float yTable[TABLES_SPEED_PRESSURE_COLUMNS];

} Tables_3dTable_T;

/*===========================================================================*
 *
 * GLOBAL VARIABLES AND CONSTANTS SECTION
 *
 *===========================================================================*/

/* VE in % */
static const Tables_3dTable_T tables_ve =
{
    .xTable = 
    {
        600.0F, 1100.0F, 1500.0F, 1900.0F, 2400.0F, 2800.0F, 3200.0F, 3600.0F, 4100.0F, 4500.0F, 4900.0F, 5300.0F,
        5700.0F, 6200.0F, 6600.0F, 7000.0F
    },
    .yTable =
    {
        25.0F, 30.0F, 36.0F, 40.0F, 46.0F, 50.0F, 56.0F, 60.0F, 66.0F, 70.0F, 76.0F, 80.0F, 86.0F, 92.0F, 96.0F, 101.0F
    },
    /* This table is written as it's natural for humans, point 0.0 is in the bottom left corner of the table */
    .zTable =
    {
        { 22.0F, 22.0F, 23.0F, 31.0F, 41.0F, 47.0F, 52.0F, 55.0F, 58.0F, 59.0F, 59.0F, 57.0F, 54.0F, 51.0F, 48.0F, 46.0F },
        { 22.0F, 22.0F, 23.0F, 33.0F, 42.0F, 48.0F, 53.0F, 57.0F, 60.0F, 61.0F, 61.0F, 59.0F, 56.0F, 53.0F, 50.0F, 48.0F },
        { 22.0F, 22.0F, 24.0F, 34.0F, 44.0F, 50.0F, 55.0F, 59.0F, 62.0F, 63.0F, 63.0F, 61.0F, 59.0F, 56.0F, 53.0F, 50.0F },
        { 22.0F, 22.0F, 25.0F, 35.0F, 45.0F, 52.0F, 57.0F, 61.0F, 64.0F, 65.0F, 65.0F, 63.0F, 60.0F, 57.0F, 54.0F, 51.0F },
        { 22.0F, 22.0F, 26.0F, 36.0F, 47.0F, 53.0F, 59.0F, 63.0F, 66.0F, 67.0F, 67.0F, 65.0F, 62.0F, 59.0F, 56.0F, 53.0F },
        { 22.0F, 22.0F, 27.0F, 37.0F, 48.0F, 55.0F, 60.0F, 65.0F, 68.0F, 69.0F, 69.0F, 67.0F, 64.0F, 61.0F, 58.0F, 55.0F },
        { 22.0F, 22.0F, 27.0F, 38.0F, 49.0F, 57.0F, 62.0F, 67.0F, 70.0F, 71.0F, 71.0F, 69.0F, 66.0F, 63.0F, 60.0F, 57.0F },
        { 22.0F, 22.0F, 28.0F, 39.0F, 50.0F, 58.0F, 64.0F, 68.0F, 72.0F, 73.0F, 73.0F, 71.0F, 67.0F, 64.0F, 61.0F, 58.0F },
        { 22.0F, 22.0F, 29.0F, 40.0F, 52.0F, 60.0F, 66.0F, 71.0F, 74.0F, 75.0F, 75.0F, 73.0F, 70.0F, 67.0F, 64.0F, 61.0F },
        { 22.0F, 22.0F, 30.0F, 41.0F, 53.0F, 61.0F, 67.0F, 72.0F, 76.0F, 77.0F, 77.0F, 75.0F, 71.0F, 67.0F, 64.0F, 61.0F },
        { 22.0F, 22.0F, 31.0F, 42.0F, 55.0F, 63.0F, 69.0F, 74.0F, 78.0F, 79.0F, 79.0F, 77.0F, 73.0F, 69.0F, 66.0F, 63.0F },
        { 22.0F, 22.0F, 31.0F, 43.0F, 56.0F, 64.0F, 71.0F, 76.0F, 80.0F, 81.0F, 81.0F, 79.0F, 75.0F, 71.0F, 67.0F, 64.0F },
        { 22.0F, 22.0F, 32.0F, 44.0F, 58.0F, 66.0F, 73.0F, 78.0F, 82.0F, 83.0F, 83.0F, 81.0F, 77.0F, 73.0F, 69.0F, 66.0F },
        { 22.0F, 22.0F, 33.0F, 46.0F, 59.0F, 68.0F, 75.0F, 80.0F, 85.0F, 86.0F, 85.0F, 83.0F, 79.0F, 75.0F, 71.0F, 67.0F },
        { 22.0F, 22.0F, 34.0F, 47.0F, 60.0F, 69.0F, 76.0F, 82.0F, 86.0F, 87.0F, 87.0F, 85.0F, 81.0F, 77.0F, 73.0F, 69.0F },
        { 22.0F, 22.0F, 34.0F, 48.0F, 62.0F, 71.0F, 78.0F, 84.0F, 88.0F, 89.0F, 89.0F, 87.0F, 83.0F, 79.0F, 75.0F, 71.0F }
    }
};

/* Spark angle before TDC */
static const Tables_3dTable_T tables_spark =
{
    .xTable = 
    {
        600.0F, 1100.0F, 1500.0F, 1900.0F, 2400.0F, 2800.0F, 3200.0F, 3600.0F, 4100.0F, 4500.0F, 4900.0F, 5300.0F,
        5700.0F, 6200.0F, 6600.0F, 7000.0F
    },
    .yTable =
    {
        25.0F, 30.0F, 36.0F, 40.0F, 46.0F, 50.0F, 56.0F, 60.0F, 66.0F, 70.0F, 76.0F, 80.0F, 86.0F, 92.0F, 96.0F, 101.0F
    },
    /* This table is written as it's natural for humans, point 0.0 is in the bottom left corner of the table */
    .zTable =
    {
        { 13.0F, 10.0F, 15.8F, 18.1F, 21.6F, 25.2F, 28.7F, 31.0F, 34.6F, 38.1F, 39.3F, 39.3F, 39.3F, 39.3F, 39.3F, 39.3F },
        { 13.0F, 10.0F, 15.6F, 17.9F, 21.4F, 24.9F, 28.3F, 30.7F, 34.1F, 37.6F, 38.8F, 38.8F, 38.8F, 38.8F, 38.8F, 38.8F },
        { 13.0F, 10.0F, 15.4F, 17.7F, 21.1F, 24.5F, 28.0F, 30.3F, 33.7F, 37.1F, 38.3F, 38.3F, 38.3F, 38.3F, 38.3F, 38.3F },
        { 13.0F, 10.0F, 15.2F, 17.5F, 20.8F, 24.2F, 27.6F, 29.9F, 33.3F, 36.7F, 37.8F, 37.8F, 37.8F, 37.8F, 37.8F, 37.8F },
        { 13.0F, 10.0F, 15.0F, 17.2F, 20.6F, 23.9F, 27.3F, 29.5F, 32.9F, 36.2F, 37.3F, 37.3F, 37.3F, 37.3F, 37.3F, 37.3F },
        { 13.0F, 10.0F, 14.8F, 17.0F, 20.3F, 23.6F, 26.9F, 29.1F, 32.4F, 35.7F, 36.9F, 36.9F, 36.9F, 36.9F, 36.9F, 36.9F },
        { 13.0F, 10.0F, 14.6F, 16.8F, 20.1F, 23.3F, 26.6F, 28.8F, 32.0F, 35.3F, 36.4F, 36.4F, 36.4F, 36.4F, 36.4F, 36.4F },
        { 13.0F, 10.0F, 14.4F, 16.6F, 19.8F, 23.0F, 26.2F, 28.4F, 31.6F, 34.8F, 35.9F, 35.9F, 35.9F, 35.9F, 35.9F, 35.9F },
        { 13.0F, 10.0F, 14.2F, 16.3F, 19.5F, 22.6F, 25.8F, 27.9F, 31.1F, 34.3F, 35.3F, 35.3F, 35.3F, 35.3F, 35.3F, 35.3F },
        { 13.0F, 10.0F, 14.0F, 16.1F, 19.2F, 22.3F, 25.5F, 27.5F, 30.7F, 33.8F, 34.8F, 34.8F, 34.8F, 34.8F, 34.8F, 34.8F },
        { 13.0F, 10.0F, 13.8F, 15.9F, 18.9F, 22.0F, 25.1F, 27.2F, 30.2F, 33.3F, 34.4F, 34.4F, 34.4F, 34.4F, 34.4F, 34.4F },
        { 13.0F, 10.0F, 13.6F, 15.6F, 18.7F, 21.7F, 24.8F, 26.8F, 29.8F, 32.9F, 33.9F, 33.9F, 33.9F, 33.9F, 33.9F, 33.9F },
        { 13.0F, 10.0F, 13.4F, 15.4F, 18.4F, 21.4F, 24.4F, 26.4F, 29.4F, 32.4F, 33.4F, 33.4F, 33.4F, 33.4F, 33.4F, 33.4F },
        { 5.0F, 10.0F, 13.2F, 15.2F, 18.1F, 21.1F, 24.1F, 26.0F, 29.0F, 31.9F, 32.9F, 32.9F, 32.9F, 32.9F, 32.9F, 32.9F  },
        { 5.0F, 10.0F, 13.0F, 15.0F, 17.9F, 20.8F, 23.7F, 25.6F, 28.5F, 31.5F, 32.4F, 32.4F, 32.4F, 32.4F, 32.4F, 32.4F  },
        { 5.0F, 10.0F, 12.8F, 14.7F, 17.6F, 20.5F, 23.3F, 25.3F, 28.1F, 31.0F, 31.9F, 31.9F, 31.9F, 31.9F, 31.9F, 31.9F  }
    }
};

/*===========================================================================*
 *
 * LOCAL FUNCTION DECLARATION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * brief:       Bilinear interpolation for 3D table
 * param[in]:   x - an x-value of searched point
 * param[in]:   y - an y-value of searched point
 * param[in]:   x0 - an x-value index of the table which is first before "x"
 * param[in]:   y0 - an y-value index of the table which is first before "y"
 * param[in]:   table - a pointer to the structure containing the table
 * param[out]:  None
 * return:      float - interpolated z-value for given "x" and "y"
 * details:     None
 *===========================================================================*/
float Tables_BilinearInterpolation(float x, float y, uint8_t x0, uint8_t y0, const Tables_3dTable_T* table);

/*===========================================================================*
 * brief:       Gets the closest table value index (rounded down) to the "value"
 * param[in]:   value - the value to be searched for
 * param[in]:   table - a pointer to the speed pressure table
 * param[out]:  None
 * return:      uint8_t - table index
 * details:     None
 *===========================================================================*/
uint8_t Tables_GetIndexFromSpeedPressureTable(float value, const float* table);

/*===========================================================================*
 *
 * FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Tables_GetSpeedPressureTableValue
 *===========================================================================*/
float Tables_GetSpeedPressureTableValue(Tables_SpeedPressure_T tableType, float speed, float pressure)
{
    uint8_t xIndex;
    uint8_t yIndex;
    const Tables_3dTable_T* table;

    switch (tableType)
    {
        case TABLES_SPEED_PRESSURE_VE:
            table = &tables_ve;
            break;

        case TABLES_SPEED_PRESSURE_SPARK:
            table = &tables_spark;
            break;

        default:
            return 0.0F;
            break;
    }

    xIndex = Tables_GetIndexFromSpeedPressureTable(speed, table->xTable);
    yIndex = Tables_GetIndexFromSpeedPressureTable(pressure, table->yTable);

    return Tables_BilinearInterpolation(speed, pressure, xIndex, yIndex, table);
}

/*===========================================================================*
 * Function: Tables_GetTwoDimensionsTableValue
 *===========================================================================*/
float Tables_GetTwoDimensionsTableValue(Tables_TwoDimensions_T tableType, float xValue)
{
    return 1.0F;
}

/*===========================================================================*
 *
 * LOCAL FUNCTION DEFINITION SECTION
 *
 *===========================================================================*/

/*===========================================================================*
 * Function: Tables_BilinearInterpolation
 *===========================================================================*/
float Tables_BilinearInterpolation(float x, float y, uint8_t x0, uint8_t y0, const Tables_3dTable_T* table)
{
    float u;
    float v;

    /* zTable[yIndex][xIndex] - becouse of the way it's stored in C */

    if (NULL == table)
    {
        return 0.0F;
    }

    u = (x - table->xTable[x0]) / (table->xTable[x0 + 1U] - table->xTable[x0]);
    v = (y - table->yTable[y0]) / (table->yTable[y0 + 1U] - table->yTable[y0]);

    /* Z table is written in cartesian coordinate (0,0 is in bottom left corner) */
    /* In C (0,0) is top left corner, so y index need to be flipped */
    y0 = TABLES_SPEED_PRESSURE_ROWS - y0;

    return (1.0F - u) * (((1.0F - v) * table->zTable[y0][x0]) + (v * table->zTable[y0 + 1U][x0])) +
           (u * (((1.0F - v) * table->zTable[y0][x0 + 1U]) + (v * table->zTable[y0 + 1U][x0 + 1U])));
}

/*===========================================================================*
 * Function: Tables_GetIndexFromSpeedPressureTable
 *===========================================================================*/
uint8_t Tables_GetIndexFromSpeedPressureTable(float value, const float* table)
{
    uint8_t index;
    uint8_t result;

    if (NULL == table)
    {
        result = 0U;
        goto tables_get_index_from_speed_pressure_table_exit;
    }

    if (value <= table[TABLES_FIRST_INDEX])
    {
        result = TABLES_FIRST_INDEX;
    }
    else if (value >= table[TABLES_SPEED_PRESSURE_LAST_INDEX])
    {
        result = TABLES_SPEED_PRESSURE_LAST_INDEX;
    }
    else
    {
        for (index = 0; index < (TABLES_SPEED_PRESSURE_COLUMNS - 1U); index++)
        {
            if ((value >= table[index]) && (value < table[index + 1U]))
            {
                result = index;
            }
        }
    }

tables_get_index_from_speed_pressure_table_exit:

    return result;
}



/* end of file */
