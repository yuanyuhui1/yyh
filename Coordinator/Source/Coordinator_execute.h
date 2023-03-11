//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Coordinator_execute.h
#endif

#ifndef __COORDINATOR_H__
  #error  Coordinator.h must be included before Coordinator_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __COORDINATOR_EXECUTE_H__
  #error Coordinator_execute.h included more than once
#endif

#define __COORDINATOR_EXECUTE_H__

#define VOTEXTYPENUM					0
#define SWIRLTYPENUM					72	
#define LIQUIDTYPENUM					0	
#define GASTYPENUM						24	
#define STEAMTYPENUM					48	


//@SubGen start@<METHODS>
void MainTaskEXE_CDT(void);
void FrontendTaskEXE_CDT(void);
void HartTaskEXE_CDT(void);
void SlowTaskEXE_CDT(void);
void ServicesTaskEXE_CDT(void);
void UpdateDiagnosis_CDT(void);
void UpdateWorkingTime_CDT(void);
void SendConfigToFE_CDT(void);
void StopTasksSRV_CDT(TUSIGN16 taskIdx);
TFLOAT GetTemperaturePercentageSRV_CDT(void);
void UpdateMaxTemperature_CDT(void);
TUSIGN16 UpdateSteamEnthalpy_CDT(TUSIGN16 opModeShift);
void CheckDataCrc_CDT(void);
void UpdateFunctionPassword_CDT(void);
TUSIGN16 UpdateInternalTemperature_CDT(void);
//@SubGen end@<METHODS>

enum FLAG_TASK_NUMBER
{
    FLAG_MAIN_TASK = 0,
    FLAG_SLOW_TASK,
    FLAG_WATCH_DOG_TASK,
    FLAG_TASK_SIZE
};

enum WATCH_DOG_TASK_FUNCTION_FLAG
{                        
    WATCH_DOG_TASK_MAIN_FLAG = 0,
    WATCH_DOG_TASK_HART_FLAG,
    WATCH_DOG_TASK_SERVICE_FLAG,
    WATCH_DOG_TASK_SLOW_FLAG,
    WATCH_DOG_TASK_FE_FLAG,
    SIZEOF_WATCH_DOG_TASK_FUNCTION_FLAG
};

enum MAIN_TASK_FUNCTION_FLAG
{
    MAIN_TASK_DATA_FLAG=0,
    MAIN_TASK_SVM_FLAG,
    MAIN_TASK_MVM_FLAG,
    MAIN_TASK_DO_FLAG,
    SIZEOF_MAIN_TASK_FUNCTION_FLAG
};


enum SLOW_TASK_FUNCTION_FLAG
{
    SLOW_TASK_MAPPER_FLAG=0,
    SLOW_TASK_OTHERDIA_FLAG,
    SLOW_TASK_DIADIA_FLAG,
    SIZEOF_SLOW_TASK_FUNCTION_FLAG
};


void CheckFlag(TUSIGN8 , TUSIGN8 );
void SetFlag(TUSIGN8 , TUSIGN8 );
void ClearFlag(TUSIGN16 );