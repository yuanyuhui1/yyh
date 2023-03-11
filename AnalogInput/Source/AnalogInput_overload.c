//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "analogInput_overload.h"
#include "AnalogInput_HW.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"

#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author Jax Yang
 \date   2012-10-15
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \test
      test-date:May 31 2013 10:41:22
 \n by: Jax Yang
 \n environment:IAR-CSPY with IO board
 \n intention:  Cover branch,Cover statement,Cover decision
 \n result module test: ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_ANALOG_INPUT(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    //lint -e{641} Jax Yang 2012-05-16 Converting enum to int accepted  
    TUSIGN16 result = FATAL_ERROR;
    
    //lint -e{746} 
    VIP_ASSERT(me==ANALOG_INPUT_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);
    
    if((typeOfStartUp & (TUSIGN8)INIT_DATA) != 0)
    {
        result |= analogInput.unit.LoadRomDefaults(me, (TINT16)ANALOGINPUT_DC_IDX_Unprotected);        
    }
    //lint -e{641} Jax Yang 2012-05-16 Converting enum to int accepted
    if( result == OK )
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
        	result = (TUSIGN16)HW_initial_AI();
        }

        if( (typeOfStartUp & INIT_TASKS) != 0)
        {
            
        }
        if( (typeOfStartUp & INIT_CALCULATION) != 0)
        {
        }
    }
    

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief	Put overload,mainly overload Trim and ai selection objects
 \author	Jax yang
 \date	16-11-2012
 \param  me = pointer to subsystem
 \param  objectIndex = object index.
 \param  attributeIndex = attribute index.
 \param  ptrValue = pointer to object value.
 \return T_UNIT error code.
 \warning none
 \test
      test-date:May 31 2013 10:41:22
 \n by: Jax Yang
 \n environment:IAR-CSPY with IO board
 \n intention:  Cover branch,Cover statement,Cover decision
 \n result module test: ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_T_AI(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    static TFLOAT aiInValueLowerPoint = 0.0,aiInValueUpperPoint = 0.0f;
    TFLOAT aiTrimLowerPoint = 0.0,aiTrimUpperPoint = 0.0f,current = 0.0f;
    TFLOAT calibration[2] = {1.0f,0.0f};
    static TUSIGN8 calibrationState = AI_CALIBRATION_OFF;
	TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 passwordEnable,aiSelection;
    
    /*lint -e613*/
	VIP_ASSERT(me);	
	VIP_ASSERT(ptrValue);
	VIP_ASSERT(*(me->ptrState)>=(TUSIGN16)INITIALIZED); // exception if not initialized
	VIP_ASSERT(me->ptrObjectList);


    // unknown object
    if( objectIndex>me->maxIdx )
    {
        result = (TUSIGN16)ILLEGAL_OBJ_IDX;
    }
    else
    {           
		switch (objectIndex)
		{            
        case AI_IDX_trimAiLowerEndpoint:
            current = *((TFLOAT *)ptrValue);   
            if(( calibrationState == AI_CALIBRATION_START) && ((current>3.6) && (current<4.5)) )
            {                   
                result  = analogInput_Get((TUSIGN16)AI_IDX_aiRawCurr,(TINT16)WHOLE_OBJECT, &aiInValueLowerPoint);  
                result |= Put_T_UNIT(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue);                 
            }
            else
            {
                result = (TUSIGN16)RULE_VIOLATION_ERR;
            }
            break;
            
        case AI_IDX_trimAiUpperEndpoint:
            current = *((TFLOAT *)ptrValue);
            if( (calibrationState == AI_CALIBRATION_START) && ((current>19.5) && (current<20.5)) )
            {
                result  = analogInput_Get((TUSIGN16)AI_IDX_aiRawCurr,(TINT16)WHOLE_OBJECT, &aiInValueUpperPoint); 
                result |= Put_T_UNIT(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue);             
            }
            else
            {
                result = (TUSIGN16)RULE_VIOLATION_ERR;            
            }
            break;

        case AI_IDX_calibrationState:
            calibrationState = *((TUSIGN8 *)ptrValue);
            if( calibrationState == AI_CALIBRATION_STOP )
            {
                calibrationState = AI_CALIBRATION_OFF;
                result  = analogInput_Get((TUSIGN16)AI_IDX_trimAiLowerEndpoint,(TINT16)WHOLE_OBJECT, &aiTrimLowerPoint);
                result |= analogInput_Get((TUSIGN16)AI_IDX_trimAiUpperEndpoint,(TINT16)WHOLE_OBJECT, &aiTrimUpperPoint);
                if( (aiInValueUpperPoint-aiInValueLowerPoint) >0 )
                {
                    calibration[0] = (aiTrimUpperPoint-aiTrimLowerPoint)/(aiInValueUpperPoint-aiInValueLowerPoint);
                    calibration[1] = aiTrimLowerPoint - (calibration[0]*aiInValueLowerPoint);
                    result |= analogInput_Put((TUSIGN16)AI_IDX_baseCalibration,(TINT16)WHOLE_OBJECT, calibration);                
                }
                else
                {
                    result = (TUSIGN16)RULE_VIOLATION_ERR; 
                }
            }
            result |= Put_T_UNIT(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue); 
            break;            
           
        case AI_IDX_aiSelection:
			result = coordinator_Get((TUSIGN16)CDT_IDX_passwordEnable,0,&passwordEnable);
			if( ((passwordEnable & CDT_ANALOGINPUT_ENABLE) > 0) && (result == (TUSIGN16)OK) )
			{
				aiSelection = *((TUSIGN8 *)ptrValue);
                result = Put_T_UNIT(me,objectIndex,attributeIndex,&aiSelection);
			}
			else
			{
				aiSelection = AI_WITHOUT;
                result = Put_T_UNIT(me,objectIndex,attributeIndex,&aiSelection);
                if( (*((TUSIGN8 *)ptrValue) == AI_WITHOUT) && (result == (TUSIGN16)OK) )
				{
					result = (TUSIGN16)OK;
				}
				else
				{
					result = (TUSIGN16)WRONG_STATE_ERR;
				}               
			}	
            
            break;            
            
        default: 
            result = Put_T_UNIT(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue);
            break;        
        }
    } 
    /*lint +e613*/

    return result;    
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief   Overload of LoadRomDefaults to update Calibration data,
          
 \author  Jax
 \date    2012-11-20
 \param   me = pointer to current out subsystem
 \param   dataClassIndex = index of the dataclass to be defaulted
 \return  error code
          OK                      operation was successful
          LESSTHAN_RANGEMIN       at least one value is less than its minimum value
          GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
          SPAN_TO_SMALL           then span between two values is to small
          NOT_ON_GRID             at least one value is not a multiple of its increment
          RULE_VIOLATION          this shows a violation of one or more businessrules
          DECLINE                 DEBUG, operation isn't permitted in the aktual unit-state
          ILLEGAL_OBJ_IDX         DEBUG, unknown Object
 \test
      test-date:May 31 2013 10:41:22
 \n by: Jax Yang
 \n environment:IAR-CSPY with IO board
 \n intention:  Cover branch,Cover statement,Cover decision
 \n result module test: ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 LoadRomDefaults_AI(const T_UNIT SLOW *me, TINT16 dataClassIndex)
{
    TUSIGN16    result = (TUSIGN16)OK;

    VIP_ASSERT(me==ANALOG_INPUT_ME);     // if invalid me pointer exit

    VIP_ASSERT(me->ptrDataSemaphore);   // if no semaphore available => exit

    if( dataClassIndex == (TINT16)ANALOGINPUT_DC_IDX_StaticFreqStaticDefalutCalibration )
    {         
        result = (TUSIGN16)OK;
    }
    else if(ALL_DATACLASSES == dataClassIndex)
    {
        //  load factory data of current out subsystem            
        result  = LoadRomDefaults_T_UNIT(me, (TINT16)ANALOGINPUT_DC_IDX_Unprotected);
        result  |= LoadRomDefaults_T_UNIT(me, (TINT16)ANALOGINPUT_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT);
    }
    else
    {
        //  load factory data of current out subsystem
        result = LoadRomDefaults_T_UNIT(me, dataClassIndex);
    }        

    return result;
}