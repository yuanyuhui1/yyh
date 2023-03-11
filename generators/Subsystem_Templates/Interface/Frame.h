//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Frame
 Module
 Description    Class definition T_UNIT::FRAME

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Frame.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Frame.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Frame.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __FRAME_H__
  #error Frame.h included more than once
#endif

#define __FRAME_H__

#define FRAME_VERSION 0x010000   // Version 1.0.0

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_FRAME_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_FRAME
typedef struct _T_FRAME
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    //@SubGen end@    <METHODS>
} T_FRAME;


//-------------------------------------------------------------------------------------------------
//! define the T_FRAME-object
extern SLOW const T_FRAME frame;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_FRAME
#define FRAME_ME (&frame.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define frame_Get(objectIndex,attributeIndex,ptrValue) \
   frame.unit.Get(FRAME_ME,objectIndex,attributeIndex,ptrValue)
#define frame_Put(objectIndex,attributeIndex,ptrValue) \
   frame.unit.Put(FRAME_ME,objectIndex,attributeIndex,ptrValue)
#define frame_Check(objectIndex,attributeIndex,ptrValue) \
   frame.unit.Check(FRAME_ME,objectIndex,attributeIndex,ptrValue)

#define frame_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   frame.unit.GetAttributeDescription(FRAME_ME,objectIndex,attributeIndex,ptrDescriptor)
#define frame_GetObjectDescription(objectIndex,ptrDescriptor) \
   frame.unit.GetObjectDescription(FRAME_ME,objectIndex,ptrDescriptor)
