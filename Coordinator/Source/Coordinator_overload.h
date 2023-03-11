//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Coordinator_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before Coordinator_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __COORDINATOR_OVERLOAD_H__
  #error Coordinator_overload.h included more than once
#endif

#define __COORDINATOR_OVERLOAD_H__

#define LENGTH_OF_FUNCTION_NAME    10
#define MAX_LENGTH_OF_INPUT_STR        (LENGTH_OF_ORDER_NUMBER + LENGTH_OF_FUNCTION_NAME + 1)
extern const TCHAR strIntRTDName[LENGTH_OF_FUNCTION_NAME] ;
extern const TCHAR strAIName[LENGTH_OF_FUNCTION_NAME];
extern const TCHAR strEnergyName[LENGTH_OF_FUNCTION_NAME];

extern TCHAR strIntPassword[LENGTH_OF_FUNCTION_PASSWORD+1];
extern TCHAR strAiPassword[LENGTH_OF_FUNCTION_PASSWORD+1];
extern TCHAR strEnergyPassword[LENGTH_OF_FUNCTION_PASSWORD+1];
//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to COORDINATOR-Class in Coordinator_constructor.c 
TUSIGN16 Initialize_COORDINATOR(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
TUSIGN16 Put_CDT(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 Get_CDT(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 LoadRomDefaults_COORDINATOR(const T_UNIT SLOW *me, TINT16 dataClassIndex);
TUSIGN16 GetAttributeDescription_CDT(const struct _T_UNIT SLOW*me,TUSIGN16 objectIndex,TUSIGN16 attributeIndex,T_DO_ATTRIB_RANGE FAST* ptrDescriptor);

void GetFunctionPasswd(TCHAR const *strOrderNum, TCHAR const *strFunctionName, TCHAR *strPassword); 