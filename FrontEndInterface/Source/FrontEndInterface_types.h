//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before FrontEndInterface_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before FrontEndInterface_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __FRONT_END_INTERFACE_TYPES_H__
  #error FrontEndInterface_types.h included more than once
#endif

#define __FRONT_END_INTERFACE_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of DynamicDuplicated block
 typedef struct _T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED
 {
     //@SubGen start@    <DATACLASS DynamicDuplicated> 
    TUSIGN8 frontendDiagnosis;
    TUSIGN8 frontendAlarmSimulation;
    TUSIGN8 frontendDiagnosisDuplicated;
    TUSIGN8 frontendAlarmSimulationDuplicated;
     //@SubGen end@    <DATACLASS DynamicDuplicated> 
 } T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED;


/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_FRONT_END_INTERFACE_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_FRONT_END_INTERFACE_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

