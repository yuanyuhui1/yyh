//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2010.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 Pressure.c
 Description            implement the fast path method:GetPressure

 Remarks

*/
//--------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "Common/Interface/common_functions.h"

#include "Mapper_Pressure.h"
#include "Mapper/Source/Mapper_types.h"
#include "Mapper/Source/Mapper_attributes.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
//--------------------------------------------------------------------------------------------------
/*!
 \brief       Fast path method:Get the value of Pressure using.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TFLOAT the value of density
 \warning     none
 \test
        test-date    :  
        by           :  
        environment  :  CSPY
        result       :  

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------

TFLOAT GetPressure(void)
{
	return mapperDynamicDuplicated.dvPressure;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief       update the pressure.relative pressure shall be converted to absolute pressure
              This variable has three input channels:AnalogInput,Hart,preset,if the bit within 
              versionDependence of CDT is enabled,otherwise it only has preset and hart input channel.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TUSIGN16 frame work return codes
 \warning     none
 \test
        test-date    :  May 14 2013 16:41:05
        by           :  Jax Yang
        environment  :  CSPY
        result       :  OK

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 UpdatePressure(void)
{
	TUSIGN16 errorCode = (TUSIGN16)OK;	
    TUSIGN8 Act_InputFail = CDT_PRESET;
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;   // HART input Content Selection.
	TUSIGN8 AISelection = AI_WITHOUT ;          // Analog input Content Selection.
	TFLOAT  PressureInput=0.0f;                 // To store the Input density.	
    TUSIGN8 pressureSimEnable = MAPPER_SIMU_DISABLE;          
 
	static TUSIGN32 t_max_previoustime = 0x00000000;
	static TUSIGN32 t_min_previoustime = 0x00000000;        
	static TBOOL alarmMax = eFALSE;
	static TBOOL alarmMin = eFALSE;                
	TFLOAT Hysteresis = 0.5f;
	TFLOAT delayTime = 0.5f;    
    TUSIGN8 source = 0;
    TUSIGN8  burstInStatus = 0xff;
    TFLOAT aiCurrent = 0.0f;
    static TFLOAT lastgoodPress = 0.0f;
    
	//Get Information from Hart Subsystem
	errorCode  = hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, 0, &HartSelection);	
	errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiSelection, 0, &AISelection);  
	errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvPressureSimEnable, 0, &pressureSimEnable); 	
    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);
    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiTempCurr,0,&aiCurrent);
	
	// Get the input value from AI or hart or HMI.
	if(errorCode == (TUSIGN16)OK)
	{
        if(pressureSimEnable == MAPPER_SIMU_ENABLE)
        {
            errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvPressureSim,(TINT16)WHOLE_OBJECT,&PressureInput); 
        }
        else
        {
            if(( AISelection == AI_PRESSURE) || (AISelection == AI_PRESSURE_ABS ))
            { 
                if(aiCurrent>=3.8 && aiCurrent<=20.5)
                {
                    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiInValue, 0, &PressureInput);                 
                    source = MAPPER_PRESSURE_FROM_AI;
                    errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvPressureSrc, 0, &source);	
                }
                else
                {                    
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                          PressureInput = lastgoodPress;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&PressureInput);
						source = MAPPER_PRESSURE_FROM_PRESET;
                    }
                }
            }
            else if((HartSelection == HART_SEC_PRESSURE)||(HartSelection == HART_SEC_PRESSURE_ABS))
            {
                if(burstInStatus == HART_BURST_IN_OK)
                {
                    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstUpBaseValue, 0, &PressureInput);	
                    source = MAPPER_PRESSURE_FROM_HART;
                    errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvPressureSrc, 0, &source);	
                }
                else
                {
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                          PressureInput = lastgoodPress;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&PressureInput);
						source = MAPPER_PRESSURE_FROM_PRESET;
                    }               
                }
                
            }
            else
            {			
                errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&PressureInput);
                source = MAPPER_PRESSURE_FROM_PRESET;
                errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvPressureSrc, 0, &source);	                
            }                        
        }
	}
	else
	{}
    
    lastgoodPress = PressureInput; 
	
    if(errorCode == (TUSIGN16)OK)
    {
        if( (HartSelection != HART_SEC_PRESSURE_ABS) && (AISelection != AI_PRESSURE_ABS) && 
            (pressureSimEnable != MAPPER_SIMU_ENABLE)&& (source != MAPPER_PRESSURE_FROM_PRESET) )
        {
            if( PressureInput >= -101.325 )
            {
                PressureInput += 101.325;
            }
            else
            {
                PressureInput = 0;
            }
        }     
       
        //ALARM FOR Max
        CheckUpperRangeLimit_Alarm(mapperStaticRare.dvPressureAlmRange.hiLim,PressureInput, Hysteresis, delayTime,&t_max_previoustime, &alarmMax);            
        CheckLowerRangeLimit_Alarm(mapperStaticRare.dvPressureAlmRange.loLim,PressureInput, Hysteresis, delayTime,&t_min_previoustime, &alarmMin);

        errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvPressure,(TINT16)WHOLE_OBJECT,&PressureInput);
        errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvPressureDuplicated,(TINT16)WHOLE_OBJECT,&PressureInput);           
        
        //set local diagnosis   
        //lint -e{534} 
        ENABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
        if(alarmMax)
        {
            MAPPER_SETDIAG_((TUSIGN16)MAPPER_ALM_PRESSURE_REACH_MAX,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
        }
        else
        {
            MAPPER_CLRDIAG_((TUSIGN16)MAPPER_ALM_PRESSURE_REACH_MAX,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
        }
        
        if(alarmMin)
        {
            MAPPER_SETDIAG_((TUSIGN16)MAPPER_ALM_PRESSURE_REACH_MIN,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
        }
        else
        {
            MAPPER_CLRDIAG_((TUSIGN16)MAPPER_ALM_PRESSURE_REACH_MIN,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
        }
        DISABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
    }  
    
	return errorCode;
}