//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Mapper Subsystem
 Module			Density.c

 Description    Manage Density device variable 
 <brief>
 	This file is used to configure the Device Variable Configuration
 </brief>
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Mapper_GasContent.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "Mapper/Source/Mapper_types.h"
#include "Mapper/Source/Mapper_attributes.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
//--------------------------------------------------------------------------------------------------
/*!
 \brief       Get the value of Gas content using fast path.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TFLOAT the value of density
 \warning     none
 \test
        test-date    :  May 14 2013 16:41:04
        by           :  Jax Yang
        environment  :  CSPY
        result       :  

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------
TFLOAT GetGasContent(void)
{	  
	return mapperDynamicDuplicated.dvContent; 
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief       Update the value of Gas content.
              This variable has three input channels:AnalogInput,Hart,preset,if the bit within 
              versionDependence of CDT is enabled,otherwise it only has preset and hart input channel.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TUSIGN16 frame work return codes
 \warning     none
 \test
 \test
        test-date    :  May 14 2013 16:41:04
        by           :  Jax Yang
        environment  :  CSPY
        result       :  

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 UpdateGasContent(void)
{
    TUSIGN16 errorCode = (TUSIGN16)OK;	
    TUSIGN8 Act_InputFail = CDT_PRESET;
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;   //HART input Content Selection.
	TUSIGN8 AISelection = AI_WITHOUT ;          //Analog input Content Selection.
	TFLOAT  GasContentInput = 0.0f;             //To store the Input Gas Content value.
    TUSIGN8 contentSimEnable = MAPPER_SIMU_DISABLE;
    TUSIGN8 source = 0;
    TUSIGN8  burstInStatus = 0xff;
    TFLOAT aiCurrent = 0.0f;
    static TFLOAT lastgoodGasC =0.0F;
    
	//Get Information from Hart Subsystem
	errorCode  = hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, 0, &HartSelection);
	//Get Information from AI Subsystem
	errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiSelection, 0, &AISelection);	
	//Get the dvContentSimEnable
	errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvContentSimEnable, 0, &contentSimEnable);      
	errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);
    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiTempCurr,0,&aiCurrent);
    
	if(errorCode == (TUSIGN16)OK)
	{
		if(contentSimEnable == MAPPER_SIMU_ENABLE)
        {
            errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvContentSim,(TINT16)WHOLE_OBJECT,&GasContentInput);         
        }
        else
        {
            if(AISelection == AI_GAS_CONTENT)
            { 
                if(aiCurrent>=3.8 && aiCurrent<=20.5)
                {
                    errorCode = analogInput_Get((TUSIGN16)AI_IDX_aiInValue, 0, &GasContentInput); 	
                    source = MAPPER_CONTENT_FORM_AI;
                    errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvContentSrc, 0, &source);	 
                }
                else
                {                  
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                          GasContentInput = lastgoodGasC;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvContentPreset,(TINT16)WHOLE_OBJECT,&GasContentInput);
                    }
                }               
            }
            else if(HartSelection == HART_SEC_GAS_CONTENT )
            {
                if(burstInStatus == HART_BURST_IN_OK)
                {
                    errorCode = hart_Get((TUSIGN16)HART_IDX_burstUpBaseValue, 0, &GasContentInput);
                    source = MAPPER_CONTENT_FORM_HART;
                    errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvContentSrc, 0, &source);	
                }
                else
                {
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                          GasContentInput = lastgoodGasC;
                    }
                    else
                    {
                        errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvContentPreset,(TINT16)WHOLE_OBJECT,&GasContentInput);
                    }
                }
                    
            }
            else
            {			
                errorCode = mapper_Get((TUSIGN16)MAPPER_IDX_dvContentPreset,(TINT16)WHOLE_OBJECT,&GasContentInput);
                source = MAPPER_CONTENT_FORM_PRESET;
                errorCode  = mapper_Put((TUSIGN16)MAPPER_IDX_dvContentSrc, 0, &source);	                 
            }        
        }
	}
	else
	{
	}	
                    
    lastgoodGasC = GasContentInput;                

    if(errorCode == (TUSIGN16)OK)
    {
        errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvContent,(TINT16)WHOLE_OBJECT,&GasContentInput);
        errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvContentDuplicated,(TINT16)WHOLE_OBJECT,&GasContentInput);    
    }
		
	return errorCode;	 
}