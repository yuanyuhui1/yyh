//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hmi_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hmi_execute.h
#endif

#ifndef __HMI_H__
  #error  hmi.h must be included before hmi_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HMI_EXECUTE_H__
  #error hmi_EXECUTE.h included more than once
#endif

#define __HMI_EXECUTE_H__

//@SubGen start@<METHODS>
TBOOL HMI_EXE_HMI(TUSIGN8 instance);
void HMI_ForceDisplayTimeoutSRV_HMI(TUSIGN8 instance);
void HMI_PowerOnSRV_HMI(TUSIGN8 instance);
void HMI_AllowIOaccessSRV_HMI(TUSIGN8 instance);
void HMI_DenyIOaccessSRV_HMI(TUSIGN8 instance);
void HMI_TransmitInterruptSRV_HMI(void);
void HMI_ReceiveInterruptSRV_HMI(void);
TBOOL HMI_IsIOActiveSRV_HMI(TUSIGN8 instance);
TBOOL HMI_NV_InitCompleteSRV_HMI(void);
TUSIGN8 HMI_GetKeypressSRV_HMI(TUSIGN8 instance);
TBOOL HMI_IsOperatorViewActive_HMI(TUSIGN8 instance);
TUSIGN8 HMI_Get_LLHardwareType_SRV_HMI(TUSIGN8 instance);
TBOOL HMI_GetAutoscrollState_SRV_HMI(TUSIGN8 instance);
TUSIGN8 HMI_GetOperatorPageNumber_SRV_HMI(TUSIGN8 instance);
void HMI_resetConfigurationTimeout_HMI(void);
TBOOL HMI_SetStartupFrame_SRV_HMI(TBOOL autoscrollState, TUSIGN8 operatorPage, TUSIGN8 instance);
void HMI_SetOperatorIcon_SRV_HMI(TUSIGN8 field, TUSIGN8 iconID);
void HMI_SetGraphPointer_SRV_HMI(TUSIGN8 frameNumber, TUSIGN8* pGraph);
void HMI_ToggleBacklight_SRV_HMI(void);
void HMI_SetFunctionKeyLock_SRV_HMI(TBOOL lockState);
void HMI_SetParameterDumpFont_SRV_HMI(TUSIGN8 font);
void HMI_SetOperatorInfo_SRV_HMI(TUSIGN8 infoID);
TBOOL HMI_SetGraphTimebase_SRV_HMI(TUSIGN8 frameNumber, TUSIGN8 timebase);
void HMI_AllowReInit_SRV_HMI(TUSIGN8 InitFlags);
TUSIGN8 HMI_GetLanguageIndex_SRV_HMI(TUSIGN8 language);
void HMI_InitialiseGlobalConfigChangeCounter_SRV_HMI(TUSIGN16 ss_Idx, TUSIGN16 ob_Idx, TINT16 at_Idx);
void HMI_ReInitialiseLowLevelDetection_SRV_HMI(void);
TBOOL HMI_PutPacketBuffer_SRV_HMI(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength);
//@SubGen end@<METHODS>
