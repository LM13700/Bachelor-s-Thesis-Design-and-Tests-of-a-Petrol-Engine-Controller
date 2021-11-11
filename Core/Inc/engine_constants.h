/*===========================================================================*
 * File:        engine_constants.h
 * Project:     ECU
 * Author:      Mateusz Mroz
 * Date:        09.11.2021
 * Brief:       Engine constatnts
 *===========================================================================*/
#ifndef _ENGINE_CONSTANTS_H_
#define _ENGINE_CONSTANTS_H_

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

/* Angle of engine used for synchronisation */
/* This angle is set at first speed signal after sync signal */
#define ENGINE_CONST_TRIGGER_ANGLE                     (180.0F)
/* This is number of trigger wheel teeth spinning with crank speed */
/* Wheel spins with cam speed (2 x crank speed), so real number is divided by 2 */
#define ENGINE_CONST_TRIGGER_WHEEL_TEETH_NO            (30.0F)

#define ENGINE_CONST_ENGINE_FULL_CYCLE_ANGLE           (720.0F)
#define ENGINE_CONST_ENGINE_ONE_ROTATION_ANGLE         (360.0F)
#define ENGINE_CONST_ENGINE_PISTONS_NO                 (3u)

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


#endif
/* end of file */
