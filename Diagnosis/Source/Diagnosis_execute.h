//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Prototypes of execute methods
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

#ifdef __DIAGNOSIS_EXECUTE_H__
  #error Diagnosis_execute.h included more than once
#endif

#define __DIAGNOSIS_EXECUTE_H__

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Init module local variables

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitLocalVarables(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Compute the behaviour of the target subsystem
 \param      aSubsysIdx: the target subsystem

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
T_BEHAVIOUR GetStatusForOutputSRV(T_SUBSYSTEM_IDX aSubsysIdx);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Compute the device status

 \return     error-code
 <pre>
    OK                     operation was successful
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 UpdateDeviceStatusEXE(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Returns the worst classification of all the alarm active and the groups where an alarm
             is active.

 \param      aDiagnosisStatus: pointer to returned data
 \return     error-code
 <pre>
    OK                     operation was successful
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetNAMURStatusSRV(T_DIAGNOSIS_STATUS* aDiagnosisStatus);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Returns all the diagnostic active conditions regarding one group or the whole device
             in internal format sorted by the alarm priority

 \param      aGroup: the group of interest or DIAGNOSIS_GROUP_ALL
 \param      anActiveConditionList: pointer to returned data
 \return     error-code
 <pre>
    OK                     operation was successful
    RULE_VIOLATION_ERR     the specified group is unknown
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetActiveDiagnosticConditionsSRV(T_DIAGNOSIS_GROUPS aGroup,
                                          T_DIAGNOSIS_ACTIVE_CONDITION* anActiveConditionList);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Returns all the information about one active diagnostic condition

 \param      aConditionId: the condition of interest
 \param      aDetailedCondition: pointer to returned data
 \return     error-code
 <pre>
    OK                     operation was successful
    RULE_VIOLATION_ERR     the specified condition is unknown
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetDetailsOfDiagnosticConditionSRV(TUSIGN8 aConditionId,
                                            T_DIAGNOSIS_DETAILED_CONDITION* aDetailedCondition);
//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform ClearAlarmhistory Action
 \author	Spring Kunli.Zhou
 \date	20-06-2011
 \return T_UNIT error code.
 \warning
 \test
 \n by:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void diagClearHistory(void);


