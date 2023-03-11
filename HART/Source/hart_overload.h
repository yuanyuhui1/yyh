//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    prototypes of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hart_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hart_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before hart_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_OVERLOAD_H__
  #error hart_overload.h included more than once
#endif

#define __HART_OVERLOAD_H__

// Bit value defines to decode the masters to be locked
// Used for command 71 Lock Device and GetDeviceLockState_HART()
#define LOCK_STATE_BIT_SINGLE               0x01
#define LOCK_STATE_BIT_ALL                  0x02
#define LOCK_STATE_BIT_SECONDARY_MASTER     0x40
#define LOCK_STATE_BIT_PRIMARY_MASTER       0x80

typedef struct
{
    TUSIGN8     day;
    TUSIGN8     month;
    TUSIGN8     year;
}HART_DATE;


//--------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author Heiko Kresse
 \date   2004-11-09
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  if a subsystem needs onle once a preparation for calculating, then
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inseid the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Initialize_HART(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Put_HART: HART put overload function.
 \author    Horst Seele
 \date      2005-11-23
 \param     commandNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Put_HART(const T_UNIT SLOW *me, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Get_HART: HART put overload function.
 \author    Martin Dahl
 \date      2006-11-23
 \param     objIdx    object index
 \n         ptrValue  pointer to object value and return-Value
 \return    error-code
 \n         OK                       operation was successful
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Get_HART(const T_UNIT SLOW *me, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue);

