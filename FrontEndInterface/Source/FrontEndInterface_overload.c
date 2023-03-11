//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
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
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "FrontEndInterface/Interface/FrontEndInterface_idx.h"
#include "frontEndInterface_types.h"
#include "frontEndInterface_attributes.h"
#include "frontEndInterface_overload.h"
#include "frontEndInterface_execute.h"
#include "FrontEndInterface/Layer2/FED_layer2.h"

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author 
 \date   
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
 test:
 test-date: 2013-4-27
 \n environment: IAR C_SPY
 \n intention:Code coverage 100%
 \n result module test: OK  
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_FRONT_END_INTERFACE(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = (TUSIGN16)FATAL_ERROR;
	
    //lint -e{746} 
    VIP_ASSERT(me==FRONT_END_INTERFACE_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);

    if( result == (TUSIGN16)OK )
    {
        if( (typeOfStartUp & (TUSIGN8)INIT_HARDWARE) != 0)
        {
            Initialize_HW();
        }

        if( (typeOfStartUp & (TUSIGN8)INIT_TASKS) != 0)
        {
           InitFEBuffer();
        }

        if( (typeOfStartUp & (TUSIGN8)INIT_DATA) != 0)
        {
            result = frontEndInterface.unit.LoadRomDefaults(me, (TINT16)FRONTENDINTERFACE_DC_IDX_DynamicDuplicated);
        }

        if( (typeOfStartUp & (TUSIGN8)INIT_CALCULATION) != 0)
        {
            Initialize_FED();
        }
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Overload of Put for pre and post Put_T_UNIT actions
 \author  Spring
 \date    2011-09-01, 
 \param   me =        pointer to current out subsystem
 \param   objIdx =    object index
 \param   attribIdx = attribute index
 \param   ptrValue =  pointer to object value and return-Value
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
 test-date: 2013-4-27
 \n environment: IAR C_SPY
 \n intention:Code coverage 100%
 \n result module test: OK  
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_FRONT_END_INTERFACE(const T_UNIT SLOW *me, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue)
{
    TUSIGN16    result = (TUSIGN16)OK;

    VIP_ASSERT(me==FRONT_END_INTERFACE_ME);     // if invalid me pointer exit

    // check for zeropointer
    VIP_ASSERT(ptrValue);

    result = Put_T_UNIT(me,objIdx,attribIdx,ptrValue);

    VIP_ASSERT(result < (TUSIGN16)DEBUG_ERROR);     // if result is not permitted, exit

    //  if no error, ...
    if (result == (TUSIGN16)OK)
    {
        if (objIdx == (TUSIGN16)FE_IDX_frontendAlarmSimulation)
	    {
		    //lint -e{641} 
		    result = Put_T_UNIT(me,FE_IDX_frontendAlarmSimulationDuplicated,attribIdx,ptrValue);
	    }
        //  else if fixed current value written, ...
        else if (objIdx == (TUSIGN16)FE_IDX_frontendDiagnosis)
        {
			//lint -e{641} 
            result = Put_T_UNIT(me,FE_IDX_frontendDiagnosisDuplicated,attribIdx,ptrValue);
        }
        else
        {
            result = (TUSIGN16)OK;
        }
    }
    return result;
}