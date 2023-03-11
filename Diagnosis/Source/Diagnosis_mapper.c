//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem diagnosis
 Module
 Description    Implementation of diagnosis mapper methods
                     for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"

#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "diagnosis_types.h"

#include "diagnosis_table_default.h"
#include "diagnosis_execute.h"

#include "Mapper/Interface/Mapper.h"
#include "Mapper/Source/Mapper_types.h"

#include "DigitalOutput/Interface/DigitalOutput.h"
#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"
#include "DigitalOutput/Source/digitalOutput_logic.h"
#include "DigitalOutput/Source/digitaloutput_pulse.h"
#include "DigitalOutput/Source/DigitalOutput_types.h"

#include "diagnosis_mapper.h"

extern const FAST __no_init T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefault;
extern FAST T_MAPPER_UNPROTECTED mapperUnprotected;
extern FAST T_DIAGNOSIS_DYNAMIC diagnosisDynamic;
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
TUSIGN16 Put_DiagnosisGroupMasking(T_DIAGNOSIS_CLASSES Classification,void FAST* ptrValue)
{
	TUSIGN8 counter;
	TUSIGN16 result = (TUSIGN16)OK;
	TUSIGN8 attributeIdx;

	VIP_ASSERT(ptrValue);
	VIP_ASSERT((Classification < DIAGNOSIS_CLASS_FAILURE) && (Classification > DIAGNOSIS_CLASS_NONE));

	for (counter = 0; counter < INPUT_TABLE_SIZE; counter++)
	{
		if(*inputTable[counter].classPtr == Classification)
		{
			attributeIdx = inputTable[counter].outputIdx;
			result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_alarmMasking,attributeIdx,ptrValue);
		}
	}
	return result;
}

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
TUSIGN16 Put_DiagnosisMasking(TUSIGN16 attributeIndex,void FAST* ptrValue)
{
	TUSIGN16 result = (TUSIGN16)OK;
	VIP_ASSERT(ptrValue);
	
	result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_alarmMasking,(TINT16)attributeIndex,ptrValue);
	return result;	
}



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
//lint -e{641} Spring 2013-05-23 Converting enum to int accepted
TUSIGN16 Put_DiagnosisAlarmSimulation(const void FAST* ptrValue)
{  //only support every time simulate one alarm
	TUSIGN8 counter,tabcount,almTemp;
	TUSIGN16 objIndex;
	TINT16 attributeIndex;
	TUSIGN16 result = (TUSIGN16)OK;

	VIP_ASSERT(ptrValue);
   
	//set the alarm simulation status
	almTemp = SET_ALARM;
	result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)0x0,&almTemp);
	if(result == (TUSIGN16)OK)
	{
		//clear all simulation alarm first
		almTemp = CLEAR_ALARM;
        //lint -e{656}
        for(counter = 0; counter < SIMULATION_OBJ_TABLE_SIZE; counter++)
		{
			objIndex = DiagnosisSimulationTable[counter].objIdx;
			result = diagnosis_Put((TUSIGN16)objIndex,(TINT16)WHOLE_OBJECT,&almTemp);
		}
		
		//find the first alarm input for the output alarm
		almTemp = SET_ALARM;
		for(counter = 0; counter <INPUT_TABLE_SIZE; counter ++)
		{
			if(inputTable[counter].outputIdx == *(TUSIGN8*)ptrValue)
			{
                //lint -e{656}
				for(tabcount = 0; tabcount < (TUSIGN8)SIMULATION_OBJ_TABLE_SIZE; tabcount++)
				{
					if(DiagnosisSimulationTable[tabcount].subIdx == inputTable[counter].subIdx)	
					{
						objIndex = DiagnosisSimulationTable[tabcount].objIdx;
						attributeIndex = (TINT16)inputTable[counter].alarm;
						//set corresponding alarm simulation object
						result |= diagnosis_Put((TUSIGN16)objIndex,(TINT16)attributeIndex,&almTemp);
						tabcount = (TUSIGN8)SIMULATION_OBJ_TABLE_SIZE;  	//break out
						counter = INPUT_TABLE_SIZE;
					}
				}
			}
		}
	}
  return result;
}

#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
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
extern const FAST __no_init T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefault;
T_BEHAVIOUR GetStatusForDOOutputSRV(T_SUBSYSTEM_IDX aSubsysIdx)
{
    T_BEHAVIOUR result;
    TUSIGN8 alarmenable = DIGITALOUTPUT_ALARM_DISABLE;
    TUSIGN8 start,end;
	TUSIGN8 counter = 0;
    
    start = FASTOutputTableAddressing[aSubsysIdx].start;
    end = FASTOutputTableAddressing[aSubsysIdx].end;
    result = DIGITALOUTPUT_ALARM_BEHAVIOUR_OK;
    
    // for each alarm defined in FASTOutputTable and belonging from aSubsysIdx
    for (counter = start; counter < end; counter++)
    {
        const T_DIAGNOSIS_INPUT_TABLE *inputTableTmp = FASTOutputTable[counter].inputTableAlarmPtr;
        TUSIGN8    dataByte, bitMask,outPutDataByte,outputBitMask,alarmData;

        // get from the T_DATA_OBJ the alarm value
        dataByte = inputTableTmp->alarm >> 3;               // equal to / 8
        bitMask = 1 << (inputTableTmp->alarm & 0x07);       // equal to % 8
        
        outPutDataByte = inputTableTmp->outputIdx >> 3;         // equal to / 8
        outputBitMask = 1 << (inputTableTmp->outputIdx & 0x07); // equal to % 8   
        alarmData = *((TUSIGN8 *)(inputTableTmp->objPtr) + dataByte);
		
		if ( (alarmData & bitMask) && 
            (!(diagnosisStaticFrequentConstantDefault.alarmMasking[outPutDataByte].byte & outputBitMask)) ) 
		{
			if(counter == (start + 0))   //6 T max  
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.maxTemperatureAlarm;
			}
			else if(counter == (start + 1))  //7=T min   
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.minTemperatureAlarm;
			}            
			else if(counter <= (start + 3))    //8,9 Qmax 
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.maxFlowrateAlarm;
			}
			else if(counter <= (start + 5))    //10 11 Qmin
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.minFlowrateAlarm;
			}
			else if(counter == (start + 6))    //12 Q cutoff
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.lowFlowCutOff;
			}
			else                               //13 14 flow over 103
			{
				alarmenable |= digitalOutputStaticFrequentConstantDefault.generalAlarm;
			}
            
			if(alarmenable == DIGITALOUTPUT_ALARM_ENABLE)
			{
				result = *FASTOutputTable[counter].behaviourPtr;   
				break;
			}
			else
			{
			}
		}
    }
    return result;
}

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
//lint -e{641} Spring 2013-05-23 Converting enum to int accepted
T_BEHAVIOUR GetOutputStatusSRV(T_SUBSYSTEM_IDX aSubsysIdx, T_DIAGNOSIS_CLASSES classification)
{
    T_BEHAVIOUR AlarmState = ALARM_BEHAVIOUR_OK;
    TUSIGN8 diagSimu = 0;
	TUSIGN8 alarmenable = 0;

    if(classification == DIAGNOSIS_CLASS_NONE)
    {
        return DIGITALOUTPUT_ALARM_BEHAVIOUR_OK;
    }
    else
    {
    	if(classification == DIAGNOSIS_CLASS_FAILURE)
        {
            alarmenable = digitalOutputStaticFrequentConstantDefault.generalAlarm;
            if(alarmenable == DIGITALOUTPUT_ALARM_ENABLE)
            {
                AlarmState = DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
            }
        }

        AlarmState |= GetStatusForDOOutputSRV((T_SUBSYSTEM_IDX)DIGITALOUTPUT_IDX);
    }


    diagSimu = diagnosisDynamic.simulationStatus;
    if(diagSimu == DIAGNOSIS_SIMULATION_ENABLED)
    {
        diagSimu = mapperUnprotected.diagSimulationType;

        if((aSubsysIdx == DIGITALOUTPUT_IDX) && (diagSimu == MAPPER_Simulate_Digital_Out))
        {
            AlarmState = DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
        }
    }
    else
    {
    }
  
    return AlarmState;
}

#endif
