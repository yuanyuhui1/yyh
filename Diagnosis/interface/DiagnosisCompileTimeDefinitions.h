//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Compile Time Definitions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifdef __DIAGNOSIS_COMPILE_TIME_DEFINITIONS_H__
#error DiagnosisCompileTimeDefinitions.h included more than once
#endif

#define __DIAGNOSIS_COMPILE_TIME_DEFINITIONS_H__


//----------------------------------------------------------------------------------------------------------
//!  compile time definitions
//@CodeGen start@ <COMPILE TIME DEFINITIONS>
#define DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
#define DIAGNOSIS_USE_SYSTEM_TIME
#define DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
#define DIAGNOSIS_USE_CONDITION_DETAILS
//@CodeGen end@ <COMPILE TIME DEFINITIONS>

//@CodeGen start@ <MACROS>
#define DIAGNOSIS_UPDATE_RATE_MSEC                    500
#define DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER   4
#define DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER         15
#define DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER         15
//@CodeGen end@ <MACROS>
