//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before Diagnosis_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Diagnosis_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before Diagnosis_overload.h
#endif

#ifdef __DIAGNOSIS_OVERLOAD_H__
  #error Diagnosis_overload.h included more than once
#endif

#define __DIAGNOSIS_OVERLOAD_H__

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Initialize the subsystem; (hardware, tasks, data, calculation)

 \param      typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return     error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_DIAGNOSIS(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);

//--------------------------------------------------------------------------------------------------
/*!
\brief      put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
\author     Giovanni Invernizzi
\date       2006-01-09
\param      objectIndex    object index
\param      attributeIndex attribute index; <0 --> put whole object
\param      ptrValue  pointer to attribute-value and return-Value
\return     error-code
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
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_T_UNIT_DIAGNOSIS(const T_UNIT SLOW*me,
                              TUSIGN16 objectIndex,
                              TINT16 attributeIndex,
                              void FAST* ptrValue);

