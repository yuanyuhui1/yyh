//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before DigitalOutput_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before DigitalOutput_execute.h
#endif

#ifndef __DIGITAL_OUTPUT_H__
  #error  DigitalOutput.h must be included before DigitalOutput_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __DIGITAL_OUTPUT_EXECUTE_H__
  #error DigitalOutput_execute.h included more than once
#endif

#define __DIGITAL_OUTPUT_EXECUTE_H__




#define FSCALEFREQUENCY (10000)	// Hz @ Qmax




extern TUSIGN8 output_mode;

extern void QmaxChange(void);

void SetDiagnosis_DO(TINT16 attributeIdx);
void ClearDiagnosis_DO(TINT16 attributeIdx);

extern TUSIGN16 SetPulseWidthTimer(TFLOAT pulseWidth);
void UpdateDialogsis(void);

//@SubGen start@<METHODS>
TUSIGN16 UpdateEXE_DIGITALOUTPUT(const T_DIGTALINPUT_PARAMETER* digitalInput);
void UpdateDiagnosis_DIGITALOUTPUT(void);
//@SubGen end@<METHODS>