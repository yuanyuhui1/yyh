//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "analogInput_execute.h"
#include "AnalogInput_HW.h"

#include "Coordinator/Interface/subsystem_idx.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h" 
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "analogInput_types.h"

extern FAST T_ANALOG_INPUT_UNPROTECTED analogInputUnprotected;

static TFLOAT   GetCurrentPercentage(void);
static TUSIGN16 RangeConvert(TUSIGN8 inType, TFLOAT* pPercentage);

//-------------------------------------------------------------------------------------------------
/*!
 \brief  get the percentage value of the AD convert data
 \author Spring-Kunli Zhou,Jax Yang,ZuoChen
 \date   2018-03-06
 \param void 
 \return percentage of the value
 \warning none
 \test
      test-date: 
     \n by:  
     \n environment: IAR CSPY
     \n intention: Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug NONE
*/
//-------------------------------------------------------------------------------------------------

static TFLOAT GetCurrentPercentage(void)
{
    static TFLOAT percentage = 0.0f;
    static TUSIGN8 outErrCnt = 0;
    static TUSIGN8 outRecoverCnt = 0;

    TFLOAT tempCur = 0.0f, rawCur = 0.0f;
    TFLOAT calibration[2] = {1.0f, 0.0f};
    TFLOAT AIValue =0.0f;
    TUSIGN8 tempSim = CDT_SIMULATION_CB_OFF;


    // Get current value from raw ADC value, unit mA
    rawCur = ((TFLOAT)(adConverter.rawValue) * ADC_RESOLUTION * 1000) / SAMPLE_R_VALUE;

    (void)analogInput_Get((TUSIGN16)AI_IDX_baseCalibration, (TINT16)WHOLE_OBJECT, calibration);
    tempCur = (rawCur * calibration[0]) + calibration[1];

    //lint -e{641,522} Jax Yang 2012-05-16 Converting enum to int accepted
    (void)analogInput_Put((TUSIGN16)AI_IDX_aiRawCurr, 0, &rawCur);

    (void)coordinator_Get((TUSIGN16)CDT_IDX_simulationMode, 0, &tempSim);
    if(tempSim == CDT_SIMULATION_CB_AI_VALUE)
    {
        (void)analogInput_Get((TUSIGN16)AI_IDX_aiSimulationData, 0, &AIValue);
        (void)analogInput_Put((TUSIGN16)AI_IDX_aiTempCurr, 0, &AIValue);
    }
    else
    {
        (void)analogInput_Put((TUSIGN16)AI_IDX_aiTempCurr, 0, &tempCur);
    }
    
    if(adConverter.state & ERR_SPICOM_FAIL)
    {
      //Clear Outof Range Alarm.
      outRecoverCnt = 0;
      outErrCnt = 0;
      adConverter.state &= ~ERR_OUT_OF_RANGE;
    }
    else
    {
        if((tempCur > HILIMCUR) || (tempCur < LOLIMCUR) )
        {
            outRecoverCnt = 0;
            if(outErrCnt >= MAX_OUT_OF_RANGE_ERR)
            {
              adConverter.state |= ERR_OUT_OF_RANGE;
            }
            else
            {
              outErrCnt++;
            }
        }
        else
        {
            outErrCnt = 0;
            if(adConverter.state & ERR_OUT_OF_RANGE)
            {
                if(outRecoverCnt >= OUT_ERR_RECOVER_TIMES)
                {
                    adConverter.state &= ~ERR_OUT_OF_RANGE;
                }
                else
                {
                    outRecoverCnt++;
                }
            }
            percentage = (tempCur - LORNGCUR) / (HIRNGCUR - LORNGCUR);
        }
    }
    return percentage;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  1.Mapping the percentage to variable accoring to the AI selection
         2.Implement the external cut off feature
 \param  InType 
 \param  TUSIGN8 inType, valid range 0~6
         TFLOAT* pPercentage pointer to percentage
 \author Spring-Kunli Zhou,Jax Yang
 \date   2011-05-10
 \return void
 \warning none
 \test
      test-date:Jun  8 2013 11:10:38
     \n by: Jax Yang
     \n environment: IAR CSPY
     \n intention: Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug NONE
*/
//-------------------------------------------------------------------------------------------------
static TUSIGN16 RangeConvert(TUSIGN8 inType, TFLOAT* pPercentage)
{
    TFLOAT AIValue;
    TUSIGN8 tempSim,alarm;
    T_DEV_RNGFLT value={0.0f, 0.0f, 0.0f};
    TUSIGN16 result=(TUSIGN16)FATAL_ERROR;
    TUSIGN8 cutoffOption = 0xff;
    TUSIGN8 cutoffState = 0xff;
    TFLOAT  cutoffThreshold = 0.0f;
    
    /*lint -e613*/
    VIP_ASSERT(pPercentage);
    
    result  = coordinator_Get((TUSIGN16)CDT_IDX_simulationMode, 0, &tempSim);
    result |= analogInput_Get((TUSIGN16)AI_IDX_aiExternalCutOffOption, (TINT16)WHOLE_OBJECT, &cutoffOption);
    
    if((tempSim == CDT_SIMULATION_CB_AI_VALUE) && (result == (TUSIGN16)OK))
    {
        result = analogInput_Get((TUSIGN16)AI_IDX_aiSimulationData, 0, &AIValue);
        *pPercentage = (AIValue - 4.0f) / 16.0f;
    }

    switch(inType)
    {
        case  AI_EXT_TEMPERATURE:
            result  = mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtRange, (TINT16)WHOLE_OBJECT, &value);
            break;

        case  AI_PRESSURE:
            result  = mapper_Get((TUSIGN16)MAPPER_IDX_dvPressureRangeRelative, (TINT16)WHOLE_OBJECT, &value);
            break;

        case  AI_PRESSURE_ABS:
            result  = mapper_Get((TUSIGN16)MAPPER_IDX_dvPressureRange, (TINT16)WHOLE_OBJECT, &value);
            break;

        case  AI_DENSITY:
            result  = mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityRange, (TINT16)WHOLE_OBJECT, &value);
            break;

      case  AI_GAS_CONTENT:
            result  = mapper_Get((TUSIGN16)MAPPER_IDX_dvContentRange, (TINT16)WHOLE_OBJECT, &value);
            break;

      case  AI_INTER_TEMPERATURE:
            result  =coordinator_Get((TUSIGN16)CDT_IDX_temperature_range, (TINT16)WHOLE_OBJECT, &value);
            break;
    default:
            break;
    }

    if((result == (TUSIGN16)OK) && (inType != AI_WITHOUT) && (inType != AI_EXTERNAL_OUTPUT_CUTOFF))
    {
        AIValue = ((*pPercentage) * (value.hiLim - value.loLim)) + value.loLim;
        if((inType == AI_PRESSURE_ABS) || (inType == AI_DENSITY) || (inType == AI_GAS_CONTENT))
        {
            if(AIValue < 0)
            {
                AIValue = 0;
            }
            
            if((AIValue > 100) && (inType == AI_GAS_CONTENT))
            {
                AIValue = 100;
            }
        }
        
        result  = analogInput_Put((TUSIGN16)AI_IDX_aiInValue, 0, &AIValue);
    }

    if((inType == AI_EXTERNAL_OUTPUT_CUTOFF) && (result == (TUSIGN16)OK))
    {
        switch(cutoffOption)
        {
        case AI_EXTERNAL_CUT_OFF_4MA:
            cutoffThreshold = 0.0f;
            break;
            
        case AI_EXTERNAL_CUT_OFF_8MA:
            cutoffThreshold = 0.25f;
            break;

        case AI_EXTERNAL_CUT_OFF_12MA:
            cutoffThreshold = 0.50f;
            break;

        default:
            break;
        }

        result = analogInput_Get((TUSIGN16)AI_IDX_aiDiagnosis, 0, &alarm);
        if((*pPercentage > cutoffThreshold) && (result == (TUSIGN16)OK))
        {
            alarm |= ERR_CUT_OFF;
            result = analogInput_Put((TUSIGN16)AI_IDX_aiDiagnosis, 0, &alarm);
            cutoffState = AI_EXTERNAL_CUT_OFF;
            result |= analogInput_Put((TUSIGN16)AI_IDX_aiExternalCutOffStatus, 0, &cutoffState);
        }
        else if((*pPercentage <= cutoffThreshold) && (result == (TUSIGN16)OK))
        {
            alarm &= ~ERR_CUT_OFF;
            result = analogInput_Put((TUSIGN16)AI_IDX_aiDiagnosis, 0, &alarm);
            cutoffState = AI_EXTERNAL_UNCUT_OFF;
            result |= analogInput_Put((TUSIGN16)AI_IDX_aiExternalCutOffStatus, 0, &cutoffState);
        }
        else
        {}
    }
    /*lint +e613*/
    
    return result;
 }


//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER ISEXTERNALCUTOFF> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER ISEXTERNALCUTOFF> \endif
 \author Jax Yang 
 \date  2013-05-15
 \param void
 \return 0 uncut off,1 cut off
 \warning 
 \test
      test-date:Jun  8 2013 11:10:38
     \n by: Jax Yang
     \n environment: IAR CSPY
     \n intention: Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug NONE
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD ISEXTERNALCUTOFF>
TUSIGN8 IsExternalCutOff_AI(void)
//@SubGen end@<METHOD ISEXTERNALCUTOFF>
{
    return ((TUSIGN8)analogInputUnprotected.aiExternalCutOffStatus);
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEANALOGINPUT> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEANALOGINPUT> \endif
 \author Jax Yang
 \date 2013-05-15
 \param void
 \return void
 \warning none
 \test
      test-date:Jun  8 2013 11:10:38
     \n by: Jax Yang
     \n environment: IAR CSPY
     \n intention: Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug NONE
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEANALOGINPUT>
void UpdateAnalogInput_AI(void)
//@SubGen end@<METHOD UPDATEANALOGINPUT>
{
    TUSIGN8 AISelection = AI_WITHOUT;
    static TUSIGN8 oldAISelection = AI_WITHOUT;
    TFLOAT PerValue;
    
    (void)analogInput_Get((TUSIGN16)AI_IDX_aiSelection, 0, (void FAST*)&AISelection); 
    
    if(AISelection != AI_WITHOUT)
    {
        GetRawValue();                          // get the raw AD value
        PerValue = GetCurrentPercentage();      // get the percentage value
        
        (void)analogInput_Put((TUSIGN16)AI_IDX_aiDiagnosis, 0, &adConverter.state);
        (void)RangeConvert(AISelection,&PerValue);  // get the variable value in base unit
    }
    
    if((oldAISelection != AI_WITHOUT) && (AISelection == AI_WITHOUT) )
    {
        adConverter.state = 0;
        (void)analogInput_Put((TUSIGN16)AI_IDX_aiDiagnosis, 0, &adConverter.state);
    }

    oldAISelection = AISelection;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \author Jax Yang
 \date 2013-05-15
 \param void
 \return void
 \warning none
 \test
      test-date:Jun  8 2013 11:10:38
     \n by: Jax Yang
     \n environment: IAR CSPY
     \n intention: Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug NONE
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_AI(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
    static TUSIGN8 aiDiagnosisSimulationStatus = DIAGNOSIS_SIMULATION_DISABLED;
    static TUSIGN8 aiOldDiagnosis = 0;
    static TUSIGN8 aiOldDiagnosisSimulation = 0;
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 i = 0;
    TUSIGN8 inputDiangosis = 0x00, setDiagnosis, clearDiagnosis=0x00;
    TUSIGN8 alarmSimulation = 0x00;
    TUSIGN8 diagnosisSimulationStatus = 0;
    TUSIGN8 setAlarm = SET_ALARM;
    TUSIGN8 clearAlarm = CLEAR_ALARM;

    result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus, (TINT16)WHOLE_OBJECT, &diagnosisSimulationStatus);
    if(diagnosisSimulationStatus != aiDiagnosisSimulationStatus)
    {
        if(diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED)
        {
            aiOldDiagnosisSimulation = aiOldDiagnosis;
            aiOldDiagnosis = 0;
        }
        else
        {
            aiOldDiagnosis = aiOldDiagnosisSimulation;
            aiOldDiagnosisSimulation = 0;
        }

        aiDiagnosisSimulationStatus = diagnosisSimulationStatus;
    }

    if((result == (TUSIGN16)OK) && (diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED))
    {
        for(i=0; i<AI_DIAGNOSIS_ALARM_TABLE_SIZE; i++)
        {
            TUSIGN8 alarmBitSimulation = 0x00;
            //lint -e{641} Jax Yang 2012-05-16 Converting enum to int accepted
            result = analogInput_Get((TUSIGN16)AI_IDX_aiAlarmSimulation, i, &alarmBitSimulation);

            if((result == (TUSIGN16)OK) && (alarmBitSimulation == SET_ALARM))
            {
              //lint -e{734} ZuoChen 2018-03-09 .Only three bits in use
                alarmSimulation |= (1<<i);
            }
        }
        setDiagnosis = alarmSimulation & ~aiOldDiagnosisSimulation;
        clearDiagnosis = ~alarmSimulation & aiOldDiagnosisSimulation;
        aiOldDiagnosisSimulation = alarmSimulation;
    }
    else
    {
        result = analogInput_Get((TUSIGN16)AI_IDX_aiDiagnosis, (TINT16)WHOLE_OBJECT, &inputDiangosis);
        setDiagnosis = inputDiangosis & ~aiOldDiagnosis;
        clearDiagnosis = ~inputDiangosis & aiOldDiagnosis;
        aiOldDiagnosis = inputDiangosis;
    }    
    
    for(i=0; i<AI_DIAGNOSIS_ALARM_TABLE_SIZE; i++)
    {
        if(setDiagnosis & (1 << i))
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_analoginputAlarm, i, &setAlarm);
        }
        else if(clearDiagnosis & (1 << i))
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_analoginputAlarm, i, &clearAlarm);
        }
        else
        {}
    }
}



/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author 
 \date 
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/