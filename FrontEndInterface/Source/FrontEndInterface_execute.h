//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before FrontEndInterface_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before FrontEndInterface_execute.h
#endif

#ifndef __FRONT_END_INTERFACE_H__
  #error  FrontEndInterface.h must be included before FrontEndInterface_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __FRONT_END_INTERFACE_EXECUTE_H__
  #error FrontEndInterface_execute.h included more than once
#endif

#define __FRONT_END_INTERFACE_EXECUTE_H__

#define FE_SET_DIAG(name)           SetDiagnosis_FE(FE_##name##)
#define FE_CLR_DIAG(name)           ClearDiagnosis_FE(FE_##name##)

#define AMPLITUDENUM   		    12
#define FERTCNUM   			    5

#define AMPYEAROFFSET		    0
#define AMPMONTHOFFSET	        1
#define AMPVALUEOFFSET		    2
#define AMPFREQOFFSET           6


#define FECOMOK				    		0
#define FECOMFAIL			    		1

#define MAXERRORCNT		    		25
#define MAX_BUFFER_CRCERR_CNT	8

#define WRITE_METER_TYPE_OK	    1
#define READ_FE_VERSION_OK		1
#define MAX_RETRY               2   // WHEN ERROR OR TIMEOUT ,We'll retry .

#define FE_BUFFER_OK				0
#define FE_BUFFER_FULL            1
#define FE_BUFFER_EMPTY            2
#define FE_BUFFER_CRCERR            3


#define FE_READ_CALCULATION_DATA  1
#define FE_CALCULATION_DATA_LENGTH 20


//#define FRONTEND_LAYER7_TEST



//@SubGen start@<METHODS>
void UpdateDiagnosis_FE(void);
void SetDiagnosis_FE(TUSIGN16 attributeIdx);
void ClearDiagnosis_FE(TUSIGN16 attributeIdx);
void InterpreterCmd_FE(const CMD_MSG* pMessage);
void ProcessComData_FE(const CMD_MSG* pMessage);
void ReadComCmd_FE(const CMD_MSG* pMessage);
void WriteComCmd_FE(const CMD_MSG* pMessage);
void ProcessCmd_FE(void);
TINT16 PutCommand_FE(TUSIGN8 cmdnum, TUSIGN8 datalen, void (*pFunc)(TUSIGN8 length,TUSIGN8* const pDataBuf), void (*pFuncFlag)(TUSIGN8 flag));
void SendCmd_FE(const CMD_MSG* pMessage);
void UpdateDiagData_FE(TUSIGN16 setDiag, TUSIGN16 clearDiag);
TBOOL CheckFEreadySRV_FE(void);
//@SubGen end@<METHODS>

TINT16 WriteFEBuffer(const CMD_MSG* cmdMsg);
TINT16 ReadFEBuffer(CMD_MSG* cmdMsg);
TINT16 ClearFEBuffer(void);
TUSIGN8 IsFEBuferEnpty(void);
void InitFEBuffer(void);

