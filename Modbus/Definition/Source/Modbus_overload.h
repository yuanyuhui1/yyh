//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MODBUS_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MODBUS_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before MODBUS_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MODBUS_OVERLOAD_H__
  #error MODBUS_overload.h included more than once
#endif

#define __MODBUS_OVERLOAD_H__

//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to MODBUS-Class in MODBUS_constructor.c
//-------------------------------------------------------------------------------------------------
/*!
\brief  initialize the subsystem; (hardware, tasks, data, calculation)
\author
\date
\param  typeOfStartUp
<pre>
INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
INIT_DATA       =0x20  initialize data and data structures
INIT_CALCULATION=0x10  initialize calculations used by the subsystem
</pre>
\return error-code
<pre>
OK                      operation was successful
ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
ERROR_RAM_STORAGE       attributes inside the ram are corrupted
ERROR_HW                hardware used by the subsystem doesn't work
</pre>
\test
\test-date: 2013-3-28
\n by: Jax Yang
\n environment:IAR Embedded Workbench Version 2.30.6
\n intention:
\n result module test:OK
\n result Lint Level 3:OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_MODBUS(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
//--------------------------------------------------------------------------------------------------
/*!
\brief  put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
\author Heiko Kresse
\date   2004-04-18
\param  objectIndex    object index
\param  attributeIndex attribute index; <0 --> put whole object
\param  ptrValue  pointer to attribute-value and return-Value
\return error-code
<pre>
OK                      operation was successful
LESSTHAN_RANGEMIN       at least one value is less than its minimum value
GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
SPAN_TO_SMALL           then span between two values is to small
NOT_ON_GRID             at least one value is not a multiple of its increment
RULE_VIOLATION          this shows a violation of one or more businessrules
DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
ILLEGAL_OBJ_IDX         DEBUG, unknown Object
ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
</pre>
\test
\test-date: 2013-3-28
\n by: Jax Yang
\n environment:IAR Embedded Workbench Version 2.30.6
\n intention:
\n result module test:OK
\n result Lint Level 3:OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_MODBUS(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
 \author 
 \date   
 \param  objectIndex    object index
 \param  attributeIndex attribute index; attributeIndex<0 --> get whole object
 \param  ptrValue  pointer for return-Value
 \return error-code
 <pre>
   OK                      operation was successful
   DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
   ILLEGAL_OBJ_IDX         DEBUG, unknown Object
   ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
 </pre>
 \test
 \n by:
 \n intention:
 \n result module test:
 \n result Lint Level 3:  // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_MODBUS(const T_UNIT SLOW*me, 
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);