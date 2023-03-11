//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ServicePort
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ServicePort_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ServicePort_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __SERVICE_PORT_TYPES_H__
  #error ServicePort_types.h included more than once
#endif

#define __SERVICE_PORT_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_SERVICE_PORT_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC> 
    TCHAR activationStatus;
    TUSIGN16 baudRate;
     //@SubGen end@    <DATACLASS DYNAMIC> 
 } T_SERVICE_PORT_DYNAMIC;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_SERVICE_PORT_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_SERVICE_PORT_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

