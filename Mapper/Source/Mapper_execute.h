//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Mapper_execute.h
#endif

#ifndef __MAPPER_H__
  #error  Mapper.h must be included before Mapper_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MAPPER_EXECUTE_H__
  #error Mapper_execute.h included more than once
#endif

#define __MAPPER_EXECUTE_H__


//@SubGen start@<METHODS>
TUSIGN16 GetPVPercentageForDOSRV_MAPPER(TFLOAT* ptrValue);
TUSIGN16 SetSelection_MAPPER(TUSIGN16 selection);
void CheckDataCrc_MAPPER(void);
TUSIGN16 UpdateDeviceVariablesEXE_MAPPER(void);
TUSIGN16 UpdatePVSVAssignment_MAPPER(void);
void UpdateDiagnosis_MAPPER(void);
TFLOAT GetDummyPercentageSRV_MAPPER(void);
//@SubGen end@<METHODS>