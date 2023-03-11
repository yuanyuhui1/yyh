//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"

#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "diagnosis_types.h"
#include "diagnosis_table_default.h"
#include "diagnosis_attributes.h"
#include "diagnosis_execute.h"

#ifdef DIAGNOSIS_USE_SYSTEM_TIME
/*#include "..\..\ff\stack\interface\common_type_stack.h"
#include "../../ff/stack/target/inc/keywords.h"
#include "../../ff/stack/target/inc/ff/vfd.h"
#define PACKED
#include "../../ff/stack/target/inc/ff/fbap.h"
#include "../../ff/stack/interface/stack_includes.h"*/
TUSIGN32 nowtime,oldtime;
TUSIGN32 diagnosisUpdateTime;
#endif

#define     DIAGNOSIS_LOCAL_HISTORY_SET         0x01
#define     DIAGNOSIS_LOCAL_HISTORY_UNSET       0x00

//--------------------------------------------------------------------------------------------------
// Update last occurrence time stamp, if system time stamp is not available
__no_init static T_DIAGNOSIS_STATUS             deviceStatus;

__no_init static TUSIGN8                        conditionGroupItr[DIAGNOSIS_GROUPS_NUM_ELEMENTS];
__no_init static T_DIAGNOSIS_ACTIVE_CONDITION   conditionGroupList[DIAGNOSIS_GROUPS_NUM_ELEMENTS][DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER];
__no_init static T_DIAGNOSIS_ACTIVE_CONDITION   tmpConditionGroupList[DIAGNOSIS_GROUPS_NUM_ELEMENTS][DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER];

__no_init static TUSIGN8                        activeConditionItr;
__no_init static T_DIAGNOSIS_ACTIVE_CONDITION   activeConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
__no_init static T_DIAGNOSIS_ACTIVE_CONDITION   tmpActiveConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];

__no_init static T_DIAGNOSIS_ALARM              outputDetailsHistory[6];
#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
__no_init static TUSIGN8                        inputLocalHistory[INPUT_TABLE_SIZE];
#endif

__no_init static const T_DATA_OBJ SLOW          *ptrAlarmHistoryObj;
__no_init static const T_DATA_OBJ SLOW          *ptrInputAlarmHistoryObj;

static TUSIGN16                                  tmpDiagnosisCounter = 0;

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Update the last alarm time stamp of a diagnosis condition:
             - If the system time is available get it;
             - If the system time is not available add every diagnosis update cycle the diagnosis
               update rate time.

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      detailedConditionPtr: pointer to the alarm counter to be updated
 \param      firstTime: TRUE if is the first time the diagnosis condition appears
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void UpdateTimeStampLastAlarm(T_DIAGNOSIS_ALARM_COUNTER *detailedConditionPtr, TBOOL firstTime)
{
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(detailedConditionPtr);

	//----------------------------------------------------------------------------------------------
	// it's the first time the alarm condition appears
	if (firstTime)
	{
		// update alarmCounter
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->alarmCounter++;
#ifdef DIAGNOSIS_USE_SYSTEM_TIME
		// update last occurrence time stamp, if system time stamp is available
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		//cif_get_system_time((T_SYS_TIME *)&detailedConditionPtr->timeStampLastAlarmHi);
		detailedConditionPtr->timeStampLastAlarmMsec = diagnosisUpdateTime;
#else
		// reset last occurrence time stamp, if system time stamp is not available
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->timeStampLastAlarmMsec = DIAGNOSIS_UPDATE_RATE_MSEC;
#endif
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->timeStampLastAlarmDay    = 0;

	}
	else
	{
		//------------------------------------------------------------------------------------------
		// Update last occurrence time stamp, if system time stamp is not available
#ifndef DIAGNOSIS_USE_SYSTEM_TIME
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->timeStampLastAlarmMsec += DIAGNOSIS_UPDATE_RATE_MSEC;
#else
		detailedConditionPtr->timeStampLastAlarmMsec += diagnosisUpdateTime;
#endif
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		if (detailedConditionPtr->timeStampLastAlarmMsec >= ALARM_COUNTER_MSEC_TO_DAY)
		{
			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			detailedConditionPtr->timeStampLastAlarmMsec -= ALARM_COUNTER_MSEC_TO_DAY;
			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			detailedConditionPtr->timeStampLastAlarmDay++;
		}
	}
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Update the alarm time counter of a diagnosis condition.

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      detailedConditionPtr: pointer to the alarm counter to be updated
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void UpdateAlarmTimeCounter(T_DIAGNOSIS_ALARM_COUNTER *detailedConditionPtr)
{
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(detailedConditionPtr);
#ifdef DIAGNOSIS_USE_SYSTEM_TIME
	detailedConditionPtr->alarmTimeCounterMsec += diagnosisUpdateTime;
#else
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	detailedConditionPtr->alarmTimeCounterMsec += DIAGNOSIS_UPDATE_RATE_MSEC;
#endif	
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	if (detailedConditionPtr->alarmTimeCounterMsec >= ALARM_COUNTER_MSEC_TO_DAY)
	{
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->alarmTimeCounterMsec -= ALARM_COUNTER_MSEC_TO_DAY;
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		detailedConditionPtr->alarmTimeCounterDay++;
	}
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Init module local variables

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitLocalVarables(void)
{
	TUSIGN8    counter;

	// set up input Table Local history, from Diagnosis_execute module
	for (counter = 0; counter < INPUT_TABLE_SIZE; counter++)
	{
#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
		inputLocalHistory[counter] = DIAGNOSIS_LOCAL_HISTORY_UNSET;
#endif
	}

	(void)memset(outputDetailsHistory, CLEAR_ALARM, 6 * sizeof(T_DIAGNOSIS_ALARM));

	// set up Alarm History Object Pointer
	ptrAlarmHistoryObj = &(DIAGNOSIS_ME->ptrObjectList[DIAGNOSIS_IDX_alarmHistory]);
	ptrInputAlarmHistoryObj = &(DIAGNOSIS_ME->ptrObjectList[DIAGNOSIS_IDX_NUM_ELEMENTS]);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Compute the behaviour of the target subsystem
 \param      aSubsysIdx: the target subsystem

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
T_BEHAVIOUR GetStatusForOutputSRV(T_SUBSYSTEM_IDX aSubsysIdx)
{
	T_BEHAVIOUR result;
	TUSIGN8 counter, priority = 0;
	const T_DIAGNOSIS_FASTOUTPUT_TABLE *FASTOutputTablePtr = 0;

	// for each alarm defined in FASTOutputTable and belonging from aSubsysIdx
	for (counter = FASTOutputTableAddressing[aSubsysIdx].start;
		counter < FASTOutputTableAddressing[aSubsysIdx].end;
		counter++)
	{
		const T_DIAGNOSIS_INPUT_TABLE *inputTableTmp = FASTOutputTable[counter].inputTableAlarmPtr;
		TUSIGN8    dataByte, bitMask;

		// get from the T_DATA_OBJ the alarm value
		dataByte = inputTableTmp->alarm >> 3;               // equal to / 8
		bitMask = 1 << (inputTableTmp->alarm & 0x07);       // equal to % 8

		// get the highest priority alarm
		if ((*((TUSIGN8 *)(inputTableTmp->objPtr) + dataByte) & bitMask) &&
			(inputTableTmp->priority > priority))
		{
			priority = inputTableTmp->priority;
			FASTOutputTablePtr = &(FASTOutputTable[counter]);
		}
	}

	// get the action correspondig to the alarm
	if (FASTOutputTablePtr != 0)
	{
		if (FASTOutputTablePtr->behaviourPtr)
		{
			result = *FASTOutputTablePtr->behaviourPtr;
		}
		else
		{
			result = ALARM_BEHAVIOUR_GENERAL_FAILURE;
		}
	}
	else
	{
		result = ALARM_BEHAVIOUR_OK;
	}

	return result;
}
#endif

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
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 UpdateDeviceStatusEXE(void)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16            result = OK;
	TUSIGN8             counter;
	T_DIAGNOSIS_ALARM   outputLocalAlarmHistory[6];
	T_DIAGNOSIS_ALARM   outputDetailsLocalHistory[6];
	T_DIAGNOSIS_STATUS  tmpDeviceStatus;
	TUSIGN8 active = 0;
	//---------------------------------------------------------------------------------------------
	// Initialize UpdateDeviceStatusEXE Variables
	// Output Local Alarm History and Output Details Local History
	(void)memset(outputLocalAlarmHistory, CLEAR_ALARM, 6 * sizeof(T_DIAGNOSIS_ALARM));
	(void)memset(outputDetailsLocalHistory, CLEAR_ALARM, 6 * sizeof(T_DIAGNOSIS_ALARM));

	// Device Status
	if (diagnosisDynamic.simulationStatus != DIAGNOSIS_SIMULATION_DISABLED)
	{
		tmpDeviceStatus.classification = DIAGNOSIS_CLASS_CHECK_FUNCTION;
	}
	else
	{
		tmpDeviceStatus.classification = DIAGNOSIS_CLASS_NONE;
	}

	(void)memset(tmpDeviceStatus.activeGroups, 0x00, sizeof(tmpDeviceStatus.activeGroups));

#ifdef DIAGNOSIS_USE_SYSTEM_TIME
	nowtime =GetTickCount_RTOS_TASK();
	diagnosisUpdateTime = nowtime-oldtime;
	oldtime = nowtime;
#endif	

	// Group Active Conditions Lists
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	for (counter = 0; counter < DIAGNOSIS_GROUPS_NUM_ELEMENTS; counter++)
	{
		TUSIGN8 listCounter;

		conditionGroupItr[counter] = 0x00;

		for (listCounter = 0; listCounter < DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER; listCounter++)
		{
			tmpConditionGroupList[counter][listCounter].activeConditionId = DIAGNOSIS_CONDITIONS_UNSET;
		}
	}

	// Active Conditions List
	activeConditionItr = 0x00;
	for (counter = 0; counter < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER; counter++)
	{
		tmpActiveConditionList[counter].activeConditionId = DIAGNOSIS_CONDITIONS_UNSET;
	}


	if (diagnosisDynamic.simulationStatus != DIAGNOSIS_SIMULATION_ENABLED_OUTPUT)
	{
		//---------------------------------------------------------------------------------------------
		// Compute UpdateDeviceStatusEXE Algorithm
		// for each alarm defined in the inputTable
		for (counter = 0; counter < INPUT_TABLE_SIZE; counter++)
		{
			TUSIGN8                     inputIdx, inputDataByte, inputBitMask;
			TUSIGN8                     outputIdx, outputDataByte, outputBitMask;
			T_DIAGNOSIS_ALARM_COUNTER   *detailedConditionPtr;

			// Compute data related to input condition to get from the T_DATA_OBJ the alarm value
			inputIdx = inputTable[counter].alarm;
			inputDataByte = inputIdx >> 3;                      // equal to / 8
			inputBitMask = 1 << (inputIdx & 0x07);              // equal to % 8

			// Compute data byte and bitmask related to the output condition
			outputIdx = inputTable[counter].outputIdx;
			outputDataByte = outputIdx >> 3;                    // equal to / 8
			outputBitMask = 1 << (outputIdx & 0x07);            // equal to % 8

			//-------------------------------------------------------------------------------------
			// Update outputRunTimeCounterPtr if the alarm is set in the alarm history
			if (diagnosisStaticCyclic.alarmHistory[outputDataByte].byte & outputBitMask)
			{
				if (!(outputDetailsLocalHistory[outputDataByte].byte & outputBitMask))
				{
					detailedConditionPtr = (T_DIAGNOSIS_ALARM_COUNTER *)inputTable[counter].outputRunTimeCounterPtr;
					if (detailedConditionPtr != NULL)
					{
						//lint -e{730} Giovanni Invernizzi 2008-12-17 Passed boolean value to TBOOL data type
						UpdateTimeStampLastAlarm(detailedConditionPtr, eFALSE);
						outputDetailsLocalHistory[outputDataByte].byte |= outputBitMask;
					}
				}
			}

#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
			//-------------------------------------------------------------------------------------
			// Update inputRunTimeCounterPtr if the alarm is set in the input alarm history
			if (diagnosisStaticCyclic.inputAlarmHistory[counter] == DIAGNOSIS_LOCAL_HISTORY_SET)
			{
				detailedConditionPtr = (T_DIAGNOSIS_ALARM_COUNTER *)inputTable[counter].alarmRunTimeCounterPtr;
				if (detailedConditionPtr != NULL)
				{
					//lint -e{730} Giovanni Invernizzi 2008-12-17 Passed boolean value to TBOOL data type
					UpdateTimeStampLastAlarm(detailedConditionPtr, eFALSE);
				}
			}
#endif

			if (*((TUSIGN8 *)(inputTable[counter].objPtr) + inputDataByte) & inputBitMask)
			{
				// if the alarm condition is set
				T_DIAGNOSIS_GROUPS      group = inputTable[counter].group;
				T_DIAGNOSIS_CLASSES     classification = (T_DIAGNOSIS_CLASSES)(*inputTable[counter].classPtr);
				TUSIGN8                 priority = inputTable[counter].priority;
				TUSIGN8                 localItr;
				TUSIGN8                 value;

#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
				//-------------------------------------------------------------------------------------
				// Update Input Alarm Detailed History
				detailedConditionPtr = (T_DIAGNOSIS_ALARM_COUNTER *)inputTable[counter].alarmRunTimeCounterPtr;
				if (detailedConditionPtr != NULL)
				{
					// it's the first time the diagnosis condition appears
					if (inputLocalHistory[counter] == DIAGNOSIS_LOCAL_HISTORY_UNSET)
					{
						inputLocalHistory[counter] = DIAGNOSIS_LOCAL_HISTORY_SET;
						result = ptrInputAlarmHistoryObj->pClass->Put(ptrInputAlarmHistoryObj, &inputLocalHistory[counter], counter, DIAGNOSIS_ME->ptrDataSemaphore);
						//lint -e{746} Giovanni Invernizzi 2007-06-26
						//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
						VIP_ASSERT(result == OK);

						//lint -e{730} Giovanni Invernizzi 2008-12-17 Passed boolean value to TBOOL data type
						UpdateTimeStampLastAlarm(detailedConditionPtr, eTRUE);
					}

					UpdateAlarmTimeCounter(detailedConditionPtr);
				}
#endif

				// Get the alarm object and check if the alarm condition is masked
				if (diagnosisStaticFrequentConstantDefault.alarmMasking[outputDataByte].byte & outputBitMask)
				{
					// Update local alarm history, it's necessary if the masking is done on an active alarm condition
					outputDetailsHistory[outputDataByte].byte &=~ outputBitMask;
				}
				else
				{

					// if the output condition was not already set by another alarm
					if (!(outputLocalAlarmHistory[outputDataByte].byte & outputBitMask))
					{
						//-------------------------------------------------------------------------------------
						// Update Local Alarm Counter
						outputLocalAlarmHistory[outputDataByte].byte |= outputBitMask;

						//-------------------------------------------------------------------------------------
						// Update Device Status
						// Get the worst classification
						if (classification > tmpDeviceStatus.classification)
						{
							tmpDeviceStatus.classification = classification;
						}
						else
						{
						}

						// set the group as active
						tmpDeviceStatus.activeGroups[group] = ACTIVE_GROUP;

						//-------------------------------------------------------------------------------------
						// Update Condition Lists
						// If space is enough build active conditions list
						if (activeConditionItr < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER)
						{
							// put the output enumeration index into the activeConditionList
							tmpActiveConditionList[activeConditionItr].activeConditionId = outputIdx;
							tmpActiveConditionList[activeConditionItr].classification = classification;
							tmpActiveConditionList[activeConditionItr].group = group;
							tmpActiveConditionList[activeConditionItr].priority = priority;
							activeConditionItr++;
						}

						// Put the output enumeration index into the ConditionList
						localItr = conditionGroupItr[group];
						if (localItr < DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER)
						{
							tmpConditionGroupList[group][localItr].activeConditionId = outputIdx;
							tmpConditionGroupList[group][localItr].classification = classification;
							tmpConditionGroupList[group][localItr].group = group;
							tmpConditionGroupList[group][localItr].priority = priority;
							conditionGroupItr[group]++;
						}

						//-------------------------------------------------------------------------------------
						// Update Alarm History
						value = SET_ALARM;
						result = ptrAlarmHistoryObj->pClass->Put(ptrAlarmHistoryObj, &value, outputIdx, DIAGNOSIS_ME->ptrDataSemaphore);
						//lint -e{746} Giovanni Invernizzi 2007-06-26
						//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
						VIP_ASSERT (result == OK);

						//-------------------------------------------------------------------------------------
						// Update Output Detailed History
						detailedConditionPtr = (T_DIAGNOSIS_ALARM_COUNTER *)inputTable[counter].outputRunTimeCounterPtr;
						if (detailedConditionPtr != NULL)
						{
							if (!(outputDetailsHistory[outputDataByte].byte & outputBitMask))
							{
								// the first time the diagnosis condition appears
								outputDetailsHistory[outputDataByte].byte |= outputBitMask;

								//lint -e{730} Giovanni Invernizzi 2008-12-17 Passed boolean value to TBOOL data type
								UpdateTimeStampLastAlarm(detailedConditionPtr, eTRUE);
							}

							UpdateAlarmTimeCounter(detailedConditionPtr);
							outputDetailsLocalHistory[outputDataByte].byte |= outputBitMask;
						}
					}
				}
			}
			else
			{
				// if the alarm condition is not set
				// update local alarm history, if the alarm was not already set by another condition
				/*if (!(outputDetailsLocalHistory[outputDataByte].byte & outputBitMask))
				{
				outputDetailsHistory[outputDataByte].byte &=~ outputBitMask;
				}*/

#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
				inputLocalHistory[counter] = DIAGNOSIS_LOCAL_HISTORY_UNSET;
#endif
			}
		}

		(void)memcpy(outputDetailsHistory,outputLocalAlarmHistory,6*sizeof(T_DIAGNOSIS_ALARM));//add by spring
	}

	// If Output Condition Simulation is enable
	else
	{
		TUSIGN8 outputIdx;

		// For each output simulation byte
		for(outputIdx = 0; outputIdx<6; outputIdx++)
		{
			TUSIGN8 bitMask = 0;
			// For each diagnosis condition bit
			for(bitMask = 0; bitMask<8; bitMask++)
			{
				// If the bit is set high
				if(diagnosisDynamic.diagnosisOutputConditionSimulation[outputIdx] & (1 << bitMask))
				{
					for(counter = 0; counter < INPUT_TABLE_SIZE; counter++)
					{
						// Check for an input row where input condition matchs
						// Get all information to updated global variables
						if(inputTable[counter].outputIdx == ((outputIdx * 8) + bitMask))
						{
							T_DIAGNOSIS_GROUPS      group = inputTable[counter].group;
							T_DIAGNOSIS_CLASSES     classification = (T_DIAGNOSIS_CLASSES) *inputTable[counter].classPtr;
							TUSIGN8                 priority = inputTable[counter].priority;
							TUSIGN8                 localItr;

							//-------------------------------------------------------------------------------------
							// Update Device Status
							// Get the worst classification
							if (classification > tmpDeviceStatus.classification)
							{
								tmpDeviceStatus.classification = classification;
							}

							// set the group as active
							tmpDeviceStatus.activeGroups[group] = ACTIVE_GROUP;

							//-------------------------------------------------------------------------------------
							// Update Condition Lists
							// If space is enough build active conditions list
							if (activeConditionItr < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER)
							{
								// put the output enumeration index into the activeConditionList
								tmpActiveConditionList[activeConditionItr].activeConditionId = (TUSIGN8)((outputIdx*8)+bitMask);
								tmpActiveConditionList[activeConditionItr].classification = classification;
								tmpActiveConditionList[activeConditionItr].group = group;
								tmpActiveConditionList[activeConditionItr].priority = priority;
								activeConditionItr++;
							}

							// Put the output enumeration index into the ConditionList
							localItr = conditionGroupItr[group];
							if (localItr < DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER)
							{
								tmpConditionGroupList[group][localItr].activeConditionId = (TUSIGN8)( (outputIdx*8)+bitMask);
								tmpConditionGroupList[group][localItr].classification = classification;
								tmpConditionGroupList[group][localItr].group = group;
								tmpConditionGroupList[group][localItr].priority = priority;
								conditionGroupItr[group]++;
							}
							break;
						}
					}
				}
			}
		}
	}

	// Update Diagnosis Counter
	// For each output condtion became active, diagnosis counter is incremented.
	for( active = 0; (active < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER) && (active < activeConditionItr); active++)
	{
		TUSIGN8 last = 0;
		TBOOL increment = eTRUE;

		for(; last < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER; last++)
		{
			if(activeConditionList[last].activeConditionId != DIAGNOSIS_CONDITIONS_UNSET)
			{
				if(tmpActiveConditionList[active].activeConditionId == activeConditionList[last].activeConditionId)
				{
					increment = eFALSE;
					break;
				}
			}
		}

		if(increment)
		{
			tmpDiagnosisCounter++;
		}
	}

	// Copy temporary device status to device status
	SuspendAll_RTOS_TASK();
	(void)memcpy(&diagnosisDynamic.diagnosisCounter, &tmpDiagnosisCounter, sizeof(diagnosisDynamic.diagnosisCounter));
	(void)memcpy(&deviceStatus, &tmpDeviceStatus, sizeof(deviceStatus));
	(void)memcpy(conditionGroupList, tmpConditionGroupList, sizeof(tmpConditionGroupList));
	(void)memcpy(activeConditionList, tmpActiveConditionList, sizeof(tmpActiveConditionList));
	ResumeAll_RTOS_TASK();

	return result;
}

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
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetNAMURStatusSRV(T_DIAGNOSIS_STATUS* aDiagnosisStatus)
{
	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(aDiagnosisStatus);
	SuspendAll_RTOS_TASK();
	//lint -e{668} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	(void)memcpy(aDiagnosisStatus, &deviceStatus, sizeof(T_DIAGNOSIS_STATUS));
	ResumeAll_RTOS_TASK();

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	return OK;
}

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
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetActiveDiagnosticConditionsSRV(T_DIAGNOSIS_GROUPS aGroup,
	T_DIAGNOSIS_ACTIVE_CONDITION* anActiveConditionList)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(anActiveConditionList);

	if (aGroup == DIAGNOSIS_GROUP_ALL)
	{
		//lint -e{668} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		(void)memcpy(anActiveConditionList,
			activeConditionList,
			DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER * sizeof(T_DIAGNOSIS_ACTIVE_CONDITION));
	}
	else if (aGroup < DIAGNOSIS_GROUPS_NUM_ELEMENTS)
	{
		//lint -e{668} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		(void)memcpy(anActiveConditionList,
			conditionGroupList[aGroup],
			DIAGNOSIS_ACTIVE_CONDITION_MAX_GROUP_NUMBER * sizeof(T_DIAGNOSIS_ACTIVE_CONDITION));
	}
	else
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = RULE_VIOLATION_ERR;
	}

	return result;
}

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
 \date       2008-12-17
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2007-06-18
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetDetailsOfDiagnosticConditionSRV(TUSIGN8 aConditionId,
                                            T_DIAGNOSIS_DETAILED_CONDITION* aDetailedCondition)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16    result = RULE_VIOLATION_ERR;
	TUSIGN8     counter;
#ifdef DIAGNOSIS_USE_CONDITION_DETAILS
	TUSIGN8     detailsCounter = 0;
#endif

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(aDetailedCondition);

#ifdef DIAGNOSIS_USE_CONDITION_DETAILS
	for (counter = 0; counter < DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER; counter++)
	{
		// if the alarm condition is not set
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		aDetailedCondition->details[counter].alarm = DIAGNOSIS_CONDITIONS_UNSET;
	}
#endif

	for (counter = 0; counter < INPUT_TABLE_SIZE; counter++)
	{
		if (inputTable[counter].outputIdx == aConditionId)
		{
			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			aDetailedCondition->classification = (T_DIAGNOSIS_CLASSES) *inputTable[counter].classPtr;
			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			aDetailedCondition->group = inputTable[counter].group;
			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			aDetailedCondition->priority = inputTable[counter].priority;

			//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
			if (inputTable[counter].outputRunTimeCounterPtr != NULL) {
				(void)memcpy(&(aDetailedCondition->runtimeCounter),
					inputTable[counter].outputRunTimeCounterPtr,
					sizeof(T_DIAGNOSIS_ALARM_COUNTER));
			} else {
				(void)memset(&(aDetailedCondition->runtimeCounter),
					0,
					sizeof(T_DIAGNOSIS_ALARM_COUNTER));
			}

#ifdef DIAGNOSIS_USE_CONDITION_DETAILS
			if (detailsCounter < DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER)
			{
				TUSIGN8 inputIdx, inputDataByte, inputBitMask;

				// Compute data related to input condition to get from the T_DATA_OBJ the alarm value
				inputIdx = inputTable[counter].alarm;
				inputDataByte = inputIdx >> 3;                      // equal to / 8
				inputBitMask = 1 << (inputIdx & 0x07);              // equal to % 8

				if (*((TUSIGN8 *)(inputTable[counter].objPtr) + inputDataByte) & inputBitMask)
				{
					// if the alarm condition is set
					//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
					aDetailedCondition->details[detailsCounter].subIdx = inputTable[counter].subIdx;
					//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
					aDetailedCondition->details[detailsCounter].alarm = inputTable[counter].alarm;
					detailsCounter++;
				}
			}
#endif

			//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
			result = OK;
		}
	}
	return result;
}


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
void diagClearHistory(void)
{
	TUSIGN8 counter;
	TUSIGN8 tabcount;
	TUSIGN8 TempAlm;
	TUSIGN8 result = OK;
	T_DIAGNOSIS_ALARM_COUNTER* pHistoryDetailPtr;
	T_RANGE_E8 descriptor;
	TUSIGN8 attribindex;

    descriptor = *((const T_RANGE_E8 SLOW *)(diagnosis.unit.ptrObjectList[DIAGNOSIS_IDX_alarmHistory].ptrDescription));
	
	//clear history details
	for(counter = 0; counter < DIAGNOSIS_CONDITIONS_NUM_ELEMENTS; counter++)
	{		
		attribindex = descriptor.ptrCodeTab[counter];
		VIP_ASSERT(diagnosis_Get(DIAGNOSIS_IDX_alarmHistory,attribindex,&TempAlm)== OK);
		if((result == OK)&&(TempAlm == SET_ALARM))
		{
			for(tabcount = 0; tabcount < INPUT_TABLE_SIZE; tabcount ++)
			{
				if(inputTable[tabcount].outputIdx == attribindex)
				{
					SuspendAll_RTOS_TASK();
#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
					pHistoryDetailPtr = (T_DIAGNOSIS_ALARM_COUNTER*)(inputTable[tabcount].alarmRunTimeCounterPtr);
					if(pHistoryDetailPtr != NULL)
					{
						(void)memset(pHistoryDetailPtr, CLEAR_ALARM, sizeof(T_DIAGNOSIS_ALARM_COUNTER));
					}
#endif
					pHistoryDetailPtr = (T_DIAGNOSIS_ALARM_COUNTER*)(inputTable[tabcount].outputRunTimeCounterPtr);
					if(pHistoryDetailPtr != NULL)
					{
						(void)memset(pHistoryDetailPtr, CLEAR_ALARM, sizeof(T_DIAGNOSIS_ALARM_COUNTER));
					}
					ResumeAll_RTOS_TASK();
				}
			}
			TempAlm = CLEAR_ALARM;
			VIP_ASSERT(diagnosis_Put(DIAGNOSIS_IDX_alarmHistory,attribindex,&TempAlm) == OK);	
		}
	}
	SuspendAll_RTOS_TASK();
#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
	(void)memset(inputLocalHistory, CLEAR_ALARM, INPUT_TABLE_SIZE);
#endif
	(void)memset(outputDetailsHistory, CLEAR_ALARM, 6 * sizeof(T_DIAGNOSIS_ALARM));
	ResumeAll_RTOS_TASK();
}



