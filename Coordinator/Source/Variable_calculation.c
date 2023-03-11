//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2010.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 Temperature.c
 Description            Temperature measurement
                          - Compute
                          - Unit conversion
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"

#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"

#include "coordinator_types.h"
#include "coordinator_attributes.h"
#include "Mapper/Source/Mapper_Measurement.h"

#include "Coordinator/Interface/Variable_calculatoin.h"
#include "Common/Interface/common_functions.h"

#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Source/AnalogInput_types.h"
#include "AnalogInput/Source/AnalogInput_attributes.h"

//lint -e641 Lawrence 2013-04-18

static TUSIGN8 freqDampInit=0; //Init damp
static TUSIGN32 freqPrevioustime=0; //Previous OS Time

//--------------------------------------------------------------------------------------------------
/*!
 \brief       Select the  Sensor  frequency accroding to Related Setting, and damping the frequency value 
 \author      zuochen wang
 \date        2010-07-02
 \param       none
 \return      frequency value
 \warning     none
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
TFLOAT Calculate_Frequency(void)
{
    if((coordinatorProtected.simulationMode == CDT_SIMULATION_FE_SENSOR_FREQ)||  \
        (coordinatorProtected.frequencySim_Enable == CDT_SIMULATION_ENABLE))       
    {		
        coordinatorDynamicDuplicated.frequency = coordinatorUnprotected.frequency_Sim;      
    }
    else if(analogInputStaticFrequentStaticDefault.aiSelection==AI_EXTERNAL_OUTPUT_CUTOFF)
    { 
        if(analogInput.IsExternalCutOff()==AI_EXTERNAL_CUT_OFF)
        {
            coordinatorDynamicDuplicated.frequency = 0.0f;
        }
    }
    else
    {
    }
    
    coordinatorDynamicDuplicated.frequency_Duplicated = coordinatorDynamicDuplicated.frequency;
    Damping(coordinatorDynamicDuplicated.frequency,&coordinatorDynamicDuplicated.frequency_Damped,&freqDampInit,&freqPrevioustime,coordinatorReplaceStaticRare.frequency_DampingTime);  
    coordinatorDynamicDuplicated.frequency_Damped_Duplictated = coordinatorDynamicDuplicated.frequency_Damped;
    
    return coordinatorDynamicDuplicated.frequency;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief       Select the  Sensor  Temperature T  accroding to the Sensor Related Setting. 
              and record the maximize process temperature.
 \author      zuochen wang
 \date        2010-07-02
 \param       none
 \return      temperature value
 \warning     none
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
TFLOAT Calculate_Temperature(void)
{
	static TUSIGN8 t_damp_Init=0; //Init damp
	static TUSIGN32 t_previoustime=0; //Previous OS Time
    
	static TUSIGN32 t_max_previoustime = 0x00000000;
	static TUSIGN32 t_min_previoustime = 0x00000000;        
    
	static TBOOL alarmMax = eFALSE;
	static TBOOL alarmMin = eFALSE;     
	
	TFLOAT temperature = 0.0f;
	TFLOAT Hysteresis = 0.5f;
	TFLOAT delayTime = 0.5f;
/*	
	if(coordinatorProtected.passwordEnable& CDT_INTERNALRTD_ENABLE)
	{	
		if(coordinatorReplaceStaticRare.intRTDType == CDT_TEMP_NONE)
		{
			temperature = coordinatorReplaceStaticFreqStaticDefault.temperaturePreset;
		}	
		else
		{
			temperature = coordinatorDynamicDuplicated.temperatureSensor + coordinatorReplaceStaticRare.temperatureOffsetCorrect + coordinatorStaticRare.temperatureOffsetCorrectUser;
		}			
	}
	else
	{
		temperature = coordinatorReplaceStaticFreqStaticDefault.temperaturePreset;
	}	
    
	if((coordinatorProtected.simulationMode == CDT_SIMULATION_CB_MEDIUM_TEMP)||  \
		(coordinatorProtected.temperatureSim_Enable == CDT_SIMULATION_ENABLE)) 
	{		
		temperature = coordinatorUnprotected.temperature_Sim;      
	}
*/ 
    
    
    
    temperature = coordinatorDynamicDuplicated.temperature; 
	//ALARM FOR Max
	CheckUpperRangeLimit_Alarm( coordinatorReplaceStaticRare.temperatureAlmRange.hiLim,temperature, Hysteresis, delayTime,&t_max_previoustime, &alarmMax);
    
	CheckLowerRangeLimit_Alarm(coordinatorReplaceStaticRare.temperatureAlmRange.loLim,temperature, Hysteresis, delayTime,&t_min_previoustime, &alarmMin);
    
	if(alarmMax)
	{
		//lint -e572 Lawrence 2013-04-18
        //lint -e778 Lawrence 2013-04-18
        CDT_SETDIAG_(CDT_ALRAM_T_MAX,coordinatorDynamicDuplicated.cdtDiagnosis);
	}
	else
	{
		
        CDT_CLRDIAG_(CDT_ALRAM_T_MAX,coordinatorDynamicDuplicated.cdtDiagnosis);
	}
	
	if(alarmMin)
	{
        
        CDT_SETDIAG_(CDT_ALRAM_T_MIN,coordinatorDynamicDuplicated.cdtDiagnosis);
	}
	else
	{
		
        CDT_CLRDIAG_(CDT_ALRAM_T_MIN,coordinatorDynamicDuplicated.cdtDiagnosis);
        //lint +e572 Lawrence 2013-04-18
        //lint +e778 Lawrence 2013-04-18
	}	
    
    if((coordinatorProtected.passwordEnable& CDT_INTERNALRTD_ENABLE)&&   \
        (coordinatorReplaceStaticRare.intRTDType != CDT_TEMP_NONE))
    {
    	//DAMPING Calculation
        Damping(coordinatorDynamicDuplicated.temperature, &coordinatorDynamicDuplicated.temperature_Damped, &t_damp_Init,&t_previoustime, coordinatorReplaceStaticFreqStaticDefault.temperature_DampingTime);  
        coordinatorDynamicDuplicated.temperature_Damped_Duplicated= coordinatorDynamicDuplicated.temperature_Damped;
        
    }
    else
    {
        coordinatorDynamicDuplicated.temperature_Damped = temperature;
        coordinatorDynamicDuplicated.temperature_Damped_Duplicated= coordinatorDynamicDuplicated.temperature_Damped;    
    }
	
	//coordinatorDynamicDuplicated.temperature = temperature;
	//coordinatorDynamicDuplicated.temperature_Duplicated = temperature;
	//DAMPING Calculation
	//Damping(coordinatorDynamicDuplicated.temperature, &coordinatorDynamicDuplicated.temperature_Damped, &t_damp_Init,&t_previoustime, coordinatorReplaceStaticFreqStaticDefault.temperature_DampingTime);  
	//coordinatorDynamicDuplicated.temperature_Damped_Duplicated= coordinatorDynamicDuplicated.temperature_Damped;
    
	//Calclulate the percentage
    if(analogInputStaticFrequentStaticDefault.aiSelection==AI_EXTERNAL_OUTPUT_CUTOFF)
    {
        if(analogInput.IsExternalCutOff()== AI_EXTERNAL_CUT_OFF)
        {
            coordinatorDynamicDuplicated.temperature_Percentage = 0.0f;
        }
    }
    else
    {
	    coordinatorDynamicDuplicated.temperature_Percentage= ((TFLOAT)100.0f * (coordinatorDynamicDuplicated.temperature_Damped - coordinatorReplaceStaticFreqStaticDefault.temperature_range.loLim))\
		    /(coordinatorReplaceStaticFreqStaticDefault.temperature_range.hiLim- coordinatorReplaceStaticFreqStaticDefault.temperature_range.loLim);// Calculation
	}
    coordinatorDynamicDuplicated.temperature_Percentage_Duplicated= coordinatorDynamicDuplicated.temperature_Percentage;
	return coordinatorDynamicDuplicated.temperature;
}

CONSTRUCTOR_T_ATRLIST(coordinatorTemperature) =
{
	CDT_IDX_temperature_Damped_Obj,     ATTRIB_2,
	CDT_IDX_temperature_UnitCode,       WHOLE_OBJECT,
	CDT_IDX_temperature_UnitType,       WHOLE_OBJECT,
	CDT_IDX_temperature_Classification, WHOLE_OBJECT,
	CDT_IDX_temperature_Status,         WHOLE_OBJECT,
	CDT_IDX_temperature_DampingTime,    WHOLE_OBJECT,
	
	CDT_IDX_temperature_range_Obj,      ATTRIB_2,
	CDT_IDX_temperature_range_Obj,      ATTRIB_3,
	CDT_IDX_temperature_Percentage,     WHOLE_OBJECT,
	CDT_IDX_temperature_range_Obj,      ATTRIB_4,   //range span
	
    CDT_IDX_temperature_MaxDN,          WHOLE_OBJECT,  
    CDT_IDX_temperature_MinDN,          WHOLE_OBJECT,
    CDT_IDX_temperature_Span_Obj,       ATTRIB_4,       //dn span
	CDT_IDX_temperature_Obj,            ATTRIB_2,
};


const T_SELECTION coordinatorTemperatureSelection = ALL_SELECTIONS_ENABLE;