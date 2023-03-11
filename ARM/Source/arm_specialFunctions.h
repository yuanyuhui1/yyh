//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module			arm_specialFunctions.h
 Description    Project-dependent functions header for ARM component

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __ARM_H__
  #error  Arm.h must be included before Arm_specialFunctions.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_SPECIALFUNCTIONS_H__
  #error Arm_specialFunctions.h included more than once
#endif

#define __ARM_SPECIALFUNCTIONS_H__
#include "ARM/Interface/arm_bespoke.h"
/// externs declared in Arm_specialFunctions.c

void AllClientAccessLevelInit_ARM(void);
TBOOL GetSpecial_ARM(void);
TUSIGN16 GetClientLoginLevel(TUSIGN8* );
TUSIGN32 GetDeviceId_ARM(TUSIGN32* );
#ifdef HART_CLIENT_PRESENT
TUSIGN8 HartAccessLevelInit_ARM(void);
TUSIGN8 HartAccessLevel_ARM(T_SUBSYSTEM_IDX , TUSIGN16 , TUSIGN16 );
#endif //HART_CLIENT_PRESENT
#ifdef HMI_CLIENT_PRESENT
TUSIGN8 HmiAccessLevelInit_ARM(void);
#endif //HMI_CLIENT_PRESENT
TUSIGN16 NvMemSave_ARM(TUSIGN16 , void SLOW* , TUSIGN16 );
TBOOL PreLoginCheck_ARM(TUSIGN8 , TUSIGN8 , TCHAR* );
void PostLoginCheck_ARM(TUSIGN16 , TCHAR* );
void PostLogoffCheck_ARM(T_SUBSYSTEM_IDX );
TBOOL ResetPasswordsCheck_ARM(TCHAR* );
TBOOL VariableWriteCheck_ARM(T_SUBSYSTEM_IDX , TUSIGN16 );
void MIDOverrideCheck_ARM(T_SUBSYSTEM_IDX , TUSIGN16 , TUSIGN16 , TUSIGN8 , TUSIGN8* , TUSIGN16* );

#ifndef DEVICE_SUBSYSTEM_PRESENT	// If no Device subsystem, these functions will be needed
TUSIGN8 StringFormat32BitDeviceStringFormatting(TUSIGN32 *, TUSIGN8 *, TBOOL ) ;
#endif //!DEVICE_SUBSYSTEM_PRESENT
TUSIGN16 StringFormat32BitHexStringFormatting(TUSIGN32 *, TCHAR *, TUSIGN16, TBOOL );
