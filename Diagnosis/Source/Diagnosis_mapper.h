//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Prototypes of diagnosis mapper 
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before Diagnosis_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Diagnosis_execute.h
#endif

#ifndef __DIAGNOSIS_H__
  #error  Diagnosis.h must be included before Diagnosis_execute.h
#endif

#ifdef __DIAGNOSIS_MAPPER_H__
  #error Diagnosis_mapper.h included more than once
#endif

#define __DIAGNOSIS_MAPPER_H__


//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform Group masking writing
 \author	Spring Kunli.Zhou
 \date	20-06-2011
 \return T_UNIT error code.
 \warning
 \test
 \n by:						 Spring Zhou
 \n date:									 2013-05-23
 \n environment: 				CSpy
 \n result module test:OK	100% code coverage
 \n result Lint Level 3: Pass
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_DiagnosisGroupMasking(T_DIAGNOSIS_CLASSES Classification,void FAST* ptrValue);
//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform individual masking writing
 \author	Spring Kunli.Zhou
 \date	20-06-2011
 \return T_UNIT error code.
 \warning
 \test
 \n by:						    Spring Zhou
 \n date:							2013-05-23
 \n environment: 			CSpy
 \n result module test:OK 100% code coverage
 \n result Lint Level 3: Pass
 \bug
 */
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_DiagnosisMasking(TUSIGN16 attributeIndex,void FAST* ptrValue);
//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform Alarm Simulation Action
 \author	Spring Kunli.Zhou
 \date	20-06-2011
 \return T_UNIT error code.
 \warning
 \test
 \n by:						    Spring Zhou
 \n date:							2013-05-23
 \n environment: 			CSpy
 \n result module test:OK 100% code coverage
 \n result Lint Level 3: Pass
 \bug
 */
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_DiagnosisAlarmSimulation(const void FAST* ptrValue);
//--------------------------------------------------------------------------------------------------
/*!
 \brief	Get the DO fastoutput behavior
 \author	Spring Kunli.Zhou
 \date	20-09-2011
 \return T_BEHAVIOR.
 \warning
 \test
 \n by:						    Spring Zhou
 \n date:							2013-05-23
 \n environment: 			CSpy
 \n result module test:OK 100% code coverage
 \n result Lint Level 3: Pass
 \bug
 */
//--------------------------------------------------------------------------------------------------
#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
T_BEHAVIOUR GetStatusForDOOutputSRV(T_SUBSYSTEM_IDX aSubsysIdx);
//--------------------------------------------------------------------------------------------------
/*!
 \brief	Get the fastoutput behavior (including simu fastoutput)
 \author	Spring Kunli.Zhou
 \date	20-09-2011
 \return T_BEHAVIOR.
 \warning
 \test
 \n by:						    Spring Zhou
 \n date:							2013-05-23
 \n environment: 			CSpy
 \n result module test:OK 100% code coverage
 \n result Lint Level 3: Pass
 \bug
 */
//--------------------------------------------------------------------------------------------------
T_BEHAVIOUR GetOutputStatusSRV(T_SUBSYSTEM_IDX aSubsysIdx, T_DIAGNOSIS_CLASSES classification);
#endif


