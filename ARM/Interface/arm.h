// Suppress MISRA Diagnostics (requirement for ControlMaster)
// MISRA Rule 96
// In the definition of a function-like macro the whole definition,
// and each instance of a parameter, shall be enclosed in parentheses.
#pragma diag_suppress=Pm084

//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Class definition T_UNIT::ARM

                Subsystem interface
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Arm.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Arm.h
#endif

#if (T_UNIT_VERSION<0x020100)
  #error T_UNIT_VERSION must be greater than or equal to 2.1.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary
//#include "arm_bespoke.h"	// Include the custom-header info first, then the below

#ifdef __ARM_H__
  #error Arm.h included more than once
#endif

#define __ARM_H__

#define ARM_VERSION 0x010108   // Version 1.1.8

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

// Client Access Levels
#define NO_ACCESS                   (0)
#define READ_ONLY_ACCESS            (1)
#define WRITE_ONLY_ACCESS           (2)
#define READ_WRITE_ACCESS           (3)

#define LENGTH_OF_NULL          	(1)     // 1 chr ('\0')

//APP20180709
#define LENGTH_OF_HASH_PASSWORD     (64)    // 8 char per word, totally 8 word.
#define LENGTH_OF_INPUT_PASSWORD    (6)     // 6 chars are used with HMI input.

// The SIMPLE_PASSWORD_BITS is the number of bits representing the hashed version of a simple
// password. This also represents the bit-number of the "feature-bit", described in execute module
#define     SIMPLE_PASSWORD_BITS    (26)

// The following definition masks-in the required number of password bits relating to a hashed
// version of a simple password
#define     SIMPLE_LEVEL_MASK       (((TUSIGN32)1 << SIMPLE_PASSWORD_BITS)-1)

// The SERVICE_PASSWORD_BITS is the number of bits representing the high-security (hashed) code.
// This also represents the bit-number of the "feature-bit" which is described following this:
#define     SERVICE_PASSWORD_BITS	(19)

// The following definition masks-in the required number of password bits relating to a hashed
// service level password
#define     SERVICE_LEVEL_MASK      (((TUSIGN32)1 << SERVICE_PASSWORD_BITS)-1)

// Access Rights Table definitions

#define BASIC_USER_NO_ACCESS        (0x0000)    //
#define BASIC_USER_READ_ONLY        (0x0001)    //
#define BASIC_USER_WRITE_ONLY       (0x0002)    //
#define BASIC_USER_READ_WRITE       (0x0003)    //

#define STANDARD_USER_NO_ACCESS     (0x0000)    //
#define STANDARD_USER_READ_ONLY     (0x0004)    //
#define STANDARD_USER_WRITE_ONLY    (0x0008)    //
#define STANDARD_USER_READ_WRITE    (0x000C)    //

#define ADVANCED_USER_NO_ACCESS     (0x0000)    //
#define ADVANCED_USER_READ_ONLY     (0x0010)    //
#define ADVANCED_USER_WRITE_ONLY    (0x0020)    //
#define ADVANCED_USER_READ_WRITE    (0x0030)    //

#define SERVICE_USER_NO_ACCESS      (0x0000)    //
#define SERVICE_USER_READ_ONLY      (0x0040)    //
#define SERVICE_USER_WRITE_ONLY     (0x0080)    //
#define SERVICE_USER_READ_WRITE     (0x00C0)    //

#define CUSTODY_USER_NO_ACCESS      (0x0000)    //
#define CUSTODY_USER_READ_ONLY      (0x0100)    //
#define CUSTODY_USER_WRITE_ONLY     (0x0200)    //
#define CUSTODY_USER_READ_WRITE     (0x0300)    //

// The "Wildcard" subsystem index means "any subsystem" if referred to in an access-right table.
// A developer can specify this as the single subsystem index entry in his/her table. Then whatever
// "rights" are specified will apply globally, ie: the stated rights will apply to any subsystem
// that a caller needs to determine the access rights for:
#define WILDCARDSUBSYSTEM_IDX       (0xFFFF)    // NB: only use "Wildcard" in simple implementation
#define ANY_ATTRIBUTE	((TUSIGN16)WHOLE_OBJECT)// has the same meaning as "WHOLE_OBJECT" (= -1, so
                                                // in this context it is resolved to 0xFFFF;
                                                // similar concept to a "wildcard")
#define TABLE_TERMINATION           (0xFF)      // a means of identifying when a table-end reached

//@SubGen start@  <TYPES>
  // THighSecurityEnableFormats
  #define ARM_HIGH_SECURITY_DISABLED 0 //Service level login via password
  #define ARM_HIGH_SECURITY_ENABLED 1 //Service level login via hasher/pin

  // 
  #define ARM_NUMBER_OF_HIGH_SECURITY_ENABLE_STATES 2 //THighSecurityEnableFormats numeric limit
  #define ARM_NUMBER_OF_CLIENT_ENABLE_STATES 3 //TClientEnableFormats numeric limit
  #define ARM_STANDARD_DEFAULT 5 //Standard default level
  #define ARM_ADVANCED_DEFAULT 6 //Advanced default level
  #define ARM_SERVICE_DEFAULT 7 //Service default level
  #define ARM_NUMBER_OF_USER_LOGGED_IN_STATES 8 //TClientLoginFormats numeric limit
  #define ARM_NUMBER_OF_CUSTODY_SWITCH_STATES 2 //TCustodySwitchActiveFormats numeric limit
  #define ARM_NUMBER_OF_READ_ONLY_SWITCH_STATES 2 //TReadOnlySwitchActiveFormats numeric limit
  #define ARM_STANDARD_DEFAULT_PASSWORD STANDARD_TARGET_PASSWORD //Meaning dependent on target build
  #define ARM_ADVANCED_DEFAULT_PASSWORD ADVANCED_TARGET_PASSWORD //Meaning dependent on target build
  #define ARM_SERVICE_DEFAULT_PASSWORD SERVICE_TARGET_PASSWORD //Meaning dependent on target build
  #define ARM_CLIENT_ENABLE_DEFAULT CLIENT_ENABLE_TARGET //Meaning dependent on target build
  #define ARM_STANDARD_DEFAULT_PASSWORD_1 HASH_PASSWORD_NULL_1 //Meaning dependent on target build
  #define ARM_STANDARD_DEFAULT_PASSWORD_2 HASH_PASSWORD_NULL_2 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_3 HASH_PASSWORD_NULL_3 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_4 HASH_PASSWORD_NULL_4 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_5 HASH_PASSWORD_NULL_5 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_6 HASH_PASSWORD_NULL_6 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_7 HASH_PASSWORD_NULL_7 //
  #define ARM_STANDARD_DEFAULT_PASSWORD_8 HASH_PASSWORD_NULL_8 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_1 HASH_PASSWORD_NULL_1 //Meaning dependent on target build
  #define ARM_ADVANCED_DEFAULT_PASSWORD_2 HASH_PASSWORD_NULL_2 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_3 HASH_PASSWORD_NULL_3 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_4 HASH_PASSWORD_NULL_4 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_5 HASH_PASSWORD_NULL_5 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_6 HASH_PASSWORD_NULL_6 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_7 HASH_PASSWORD_NULL_7 //
  #define ARM_ADVANCED_DEFAULT_PASSWORD_8 HASH_PASSWORD_NULL_8 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_1 HASH_PASSWORD_SERVICE_1 //Meaning dependent on target build
  #define ARM_SERVICE_DEFAULT_PASSWORD_2 HASH_PASSWORD_SERVICE_2 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_3 HASH_PASSWORD_SERVICE_3 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_4 HASH_PASSWORD_SERVICE_4 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_5 HASH_PASSWORD_SERVICE_5 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_6 HASH_PASSWORD_SERVICE_6 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_7 HASH_PASSWORD_SERVICE_7 //
  #define ARM_SERVICE_DEFAULT_PASSWORD_8 HASH_PASSWORD_SERVICE_8 //
  #define ARM_CALIBRATION_LOAD 0x55 //

  // TClientEnableFormats
  #define ARM_HART_ENABLED 0 //HART Client may login with write access
  #define ARM_HMI_ENABLED 1 //HMI Client may log in with write access
  #define ARM_HART_AND_HMI_ENABLED 2 //HART and HMI Client may log in with write access

  // TClientLoginFormats
  #define ARM_NO_USER 0 //No user logged in
  #define ARM_STANDARD_USER 1 //Standard user logged in
  #define ARM_ADVANCED_USER 2 //Advanced user logged in
  #define ARM_SERVICE_USER 3 //Service user logged in
  #define ARM_CUSTODY_USER 4 //Custody user logged in

  // TCustodySwitchActiveFormats
  #define ARM_CUSTODY_SWITCH_INACTIVE 0 //Custody (HW) switch disengaged
  #define ARM_CUSTODY_SWITCH_ACTIVE 1 //Custody (HW) switch engaged

  // TReadOnlySwitchActiveFormats
  #define ARM_READ_ONLY_SWITCH_INACTIVE 0 //Read-only (HW) switch disengaged
  #define ARM_READ_ONLY_SWITCH_ACTIVE 1 //Read-only (HW) switch engaged
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_ARM_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    ARM_DC_IDX_StaticFrequentConstantDefault_2,
    ARM_DC_IDX_StaticFrequentConstantDefault_Password,
    ARM_DC_IDX_DYNAMIC,
    ARM_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT,
    ARM_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_ARM
typedef struct _T_ARM
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    TUSIGN16 (* LogInEXE)(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* password);
    TUSIGN16 (* LogOffEXE)(T_SUBSYSTEM_IDX callerId);
    TUSIGN16 (* CheckAccessRightsSRV)(T_SUBSYSTEM_IDX callerId, T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId, TUSIGN16 attributeId, TUSIGN8 accessRequested);
    TUSIGN16 (* ResetPasswordsEXE)(TCHAR* password);
    TUSIGN16 (* ChangePasswordEXE)(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* newPassword);
    //@SubGen end@    <METHODS>
} T_ARM;


//-------------------------------------------------------------------------------------------------
//! define the T_ARM-object
extern SLOW const T_ARM arm;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_ARM
#define ARM_ME (&arm.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define arm_Get(objectIndex,attributeIndex,ptrValue) \
   arm.unit.Get(ARM_ME,objectIndex,attributeIndex,ptrValue)
#define arm_Put(objectIndex,attributeIndex,ptrValue) \
   arm.unit.Put(ARM_ME,objectIndex,attributeIndex,ptrValue)
#define arm_Check(objectIndex,attributeIndex,ptrValue) \
   arm.unit.Check(ARM_ME,objectIndex,attributeIndex,ptrValue)

#define arm_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   arm.unit.arm_GetAttributeDescription(ARM_ME,objectIndex,attributeIndex,ptrDescriptor)
#define arm_GetObjectDescription(objectIndex,ptrDescriptor) \
   arm.unit.arm_GetAttributeDescription(ARM_ME,objectIndex,ptrDescriptor)

// Enable MISRA Diagnostics (requirement for ControlMaster)
#pragma diag_default=Pm084
