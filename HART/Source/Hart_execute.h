//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Hart_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Hart_execute.h
#endif

#ifndef __HART_H__
  #error  Hart.h must be included before Hart_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_EXECUTE_H__
  #error Hart_execute.h included more than once
#endif

#define __HART_EXECUTE_H__

//@SubGen start@<METHODS>
void HARTCommandInterpreterEXE_HART(void);
void IncrementConfiChangeCounter_HART(void);
TUSIGN8 GetDeviceLockStatus_HART(void);
TBOOL HART_PutReceiveBuffer_SRV_HART(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength);
//@SubGen end@<METHODS>

