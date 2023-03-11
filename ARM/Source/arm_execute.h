//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Arm_execute.h
#endif

#ifndef __ARM_H__
  #error  Arm.h must be included before Arm_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_EXECUTE_H__
  #error Arm_execute.h included more than once
#endif

#define __ARM_EXECUTE_H__

/// externs declared in arm_execute.c, struct definitions in arm.h
#ifdef HMI_CLIENT_PRESENT
extern TUSIGN8 hmiAccessLevel;
#endif //HMI_CLIENT_PRESENT

#ifdef HART_CLIENT_PRESENT
extern TUSIGN8 hartAccessLevel;
#endif //HART_CLIENT_PRESENT

//-------------------------------------------------------------------------------------------------
// Definitions used in ARM execute methods
//-------------------------------------------------------------------------------------------------
// Used by/returned from ComparePasswords_ARM
#define NO_PASSWORD_MATCH           (0)     // used in declaring a password mismatch
#define PASSWORD_MATCH              (1)     // used in declaring a password match
#define MAX_NUMBER_OF_MISMATCHES    (3)     // used in determining a limit on mismatched passwords

// Definitions relating to the "feature bit" described in execute module:
#define     ACCESS_SPECIFIED_LEVEL  (1)     // hash-code used for normal access-level mode
#define     RESET_PASSWORDS         (0)     // hash-code used when resetting passwords

//-------------------------------------------------------------------------------------------------
// Function prototypes of ARM_execute
//-------------------------------------------------------------------------------------------------
TUSIGN16 ComparePasswords_ARM(const TCHAR * , TCHAR * , TUSIGN8 , TUSIGN16 , TUSIGN8 );

//@SubGen start@<METHODS>
TUSIGN16 LogInEXE_ARM(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* password);
TUSIGN16 LogOffEXE_ARM(T_SUBSYSTEM_IDX callerId);
TUSIGN16 CheckAccessRightsSRV_ARM(T_SUBSYSTEM_IDX callerId, T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId, TUSIGN16 attributeId, TUSIGN8 accessRequested);
TUSIGN16 ResetPasswordsEXE_ARM(TCHAR* password);
TUSIGN16 ChangePasswordEXE_ARM(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* newPassword);
//@SubGen end@<METHODS>
