//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Arm_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_TYPES_H__
  #error Arm_types.h included more than once
#endif

#define __ARM_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of StaticFrequentConstantDefault_Password block
 typedef struct _T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD
 {
     //@SubGen start@    <DATACLASS StaticFrequentConstantDefault_Password>
    TUSIGN8 hardwareCalibrationLoad;
    TUSIGN8 serviceAccountSwitch;
    TUSIGN32 stdDefaultPassword[8];
    TUSIGN32 advDefaultPassword[8];
    TUSIGN32 svcDefaultPassword[8];
    TUSIGN32 stdUserPassword[8];
    TUSIGN32 advUserPassword[8];
    TUSIGN32 svcUserPassword[8];
     //@SubGen end@    <DATACLASS StaticFrequentConstantDefault_Password>
 } T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CONSTANT_DEFAULT block
 typedef struct _T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
    TUSIGN32 stdDefaultPassword_dummy;
    TUSIGN32 advDefaultPassword_dummy;
    TUSIGN32 svcDefaultPassword_dummy;
    TUSIGN32 stdUserPassword_dummy;
    TUSIGN32 advUserPassword_dummy;
    TUSIGN32 svcUserPassword_dummy;
    TUSIGN16 adpin;
    TUSIGN8 clientEnableState;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
 } T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT;



 //-------------------------------------------------------------------------------------------------
 //! structure of StaticFrequentConstantDefault_2 block
 typedef struct _T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2
 {
     //@SubGen start@    <DATACLASS StaticFrequentConstantDefault_2>
    TUSIGN8 hiSecAccessState;
     //@SubGen end@    <DATACLASS StaticFrequentConstantDefault_2>
 } T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2;



 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_ARM_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC>
    TUSIGN8 hmiLoginState;
    TUSIGN8 hartLoginState;
    TUSIGN8 transmitterIdStringForm[8];
    TUSIGN8 readOnlySwitchStatus;
    TUSIGN8 custodySwitchStatus;
     //@SubGen end@    <DATACLASS DYNAMIC>
 } T_ARM_DYNAMIC;




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_ARM_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template>
    //@SubGen end@    <DATACLASS Template>
} T_ARM_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

