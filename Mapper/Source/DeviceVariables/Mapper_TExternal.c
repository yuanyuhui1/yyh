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
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Source/Mapper_types.h"
#include "Mapper/Source/Mapper_attributes.h"
#include "Mapper_TExternal.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
//--------------------------------------------------------------------------------------------------
/*!
 \brief       Fast path method:Get the value of T_External.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TFLOAT the value of external temperature
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

TFLOAT GetT_External(void)
{
	return mapperDynamicDuplicated.dvTExt;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief       Fast path method:Get the value of T_External.
              This variable has three input channel:AnalogInput,Hart,preset,if the bit within 
              versionDependence of CDT is enabled,otherwise it only has preset input channel.
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

TUSIGN16 UpdateT_External(void)
{
    TUSIGN8 Act_InputFail = CDT_PRESET;
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;//HART input Content Selection.
	TUSIGN8 AISelection = AI_WITHOUT ;//Analog input Content Selection.
	TUSIGN16 errorCode = (TUSIGN16)OK;	
    TUSIGN8 tExtSimEnable = MAPPER_SIMU_DISABLE; 
    TUSIGN8 source = 0;
    TUSIGN8  burstInStatus = 0xff;
    TFLOAT  TExtInput = 0.0F ;//To store the Input Temperature.
    TFLOAT aiCurrent = 0.0f;
    TFLOAT percentage = 0.0f;
    T_DEV_RNGFLT value;
    static TFLOAT  lastgoodTExt = 0.0f;
	//Get Information from Hart Subsystem
	errorCode  = hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, 0, &HartSelection);
	//Get Information from AI Subsystem
	errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiSelection, 0, &AISelection);  
	//Get the dvPressureSimEnable
	errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtSimEnable, 0, &tExtSimEnable); 	  
    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);
    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiTempCurr,0,&aiCurrent);
	
	if(errorCode == (TUSIGN16)OK)
	{
        if(tExtSimEnable == MAPPER_SIMU_ENABLE)
        {
            errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtSim,(TINT16)WHOLE_OBJECT,&TExtInput);
        }
        else
        {
            // AISelection is AI_TEMPERATURE
            if(AISelection == AI_EXT_TEMPERATURE) 
            {
                if(aiCurrent>=3.8 && aiCurrent<=20.5)
                {
                    errorCode = analogInput_Get((TUSIGN16)AI_IDX_aiInValue, 0, &TExtInput);
                }
                else
                {                   
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                           TExtInput = lastgoodTExt;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtPreset,(TINT16)WHOLE_OBJECT,&TExtInput);
                    }
                }
                
                source = MAPPER_TEXT_FROM_AI;
                errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtSrc, 0, &source);	                
            }
            //HART burst input enabled and HartSelection is HART_SEC_TEMPERATURE
            else if(HartSelection == HART_SEC_EXT_TEMPERATURE)
            {
                if (burstInStatus == HART_BURST_IN_OK)
                {
                    errorCode = hart_Get((TUSIGN16)HART_IDX_burstUpBaseValue, 0, &TExtInput);
                }
                else
                {
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                           TExtInput = lastgoodTExt;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtPreset,(TINT16)WHOLE_OBJECT,&TExtInput);
                    }
                }
                source = MAPPER_TEXT_FROM_HART;
                errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtSrc, 0, &source);	                 
            }
            //other cases except (AI_Enable&&AI input is temperature) and (HART_IN_Enable&&Hart input is temperature)
            else
            {			
                errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtPreset,(TINT16)WHOLE_OBJECT,&TExtInput);
                source = MAPPER_TEXT_FROM_PRESET;
                errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtSrc, 0, &source);	                 
            }	        
        }
        
        lastgoodTExt = TExtInput;

        if(errorCode == (TUSIGN16)OK)
        {
            errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvTExt,(TINT16)WHOLE_OBJECT,&TExtInput);
            errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtDuplicated,(TINT16)WHOLE_OBJECT,&TExtInput);        
        }
		
		//Calclulate the percentage
		errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtRange,(TINT16)WHOLE_OBJECT,&value);		
		if( errorCode == (TUSIGN16)OK && ((value.hiLim - value.loLim)>0) )
		{            
			percentage = (TFLOAT)100.0f*(TExtInput - value.loLim)/(value.hiLim - value.loLim);
			errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtPercentage,(TINT16)WHOLE_OBJECT,&percentage);
		}
	}
	else
	{
	}
    
	return errorCode;
}