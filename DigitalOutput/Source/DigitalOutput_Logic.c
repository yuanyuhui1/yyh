//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"


#include "DigitalOutput/Interface/DigitalOutput.h"
#include "digitaloutput_hardware.h"
#include "digitalOutput_logic.h"
#include "digitaloutput_pulse.h"
#include "digitalOutput_types.h"
#include "digitalOutput_attributes.h"
#include "digitalOutput_execute.h"  //extern TFlowData gSystemFlowData;


//-------------------------------------------------------------------------------------------------
/*!
 \brief Update Logic Output.
 \author Paul Li
 \date	2011-09-26
 \param *me, alarmState
 \return port value.
 \warning
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 UpdateLogic_DIGITALOUTPUT(const TLogic *me, TUSIGN8 alarmState)
{
    // Declare local variables
    // pre-set "OFF", to take account of the "non-function" configurations
    TUSIGN8 logicOutputStatus = (TUSIGN8) DIGITALOUTPUT_LOGIC_LOW;
    InitDigitalOutputTimers();
    output_mode = DIGITALOUTPUT_DO1_LOGIC;
    // sort-out the logic behaviour required:
    // Handle Simulation logics:
    if (*(me->pSimuEnabled) == (TUSIGN8) DIGITALOUTPUT_SIMU_ENABLED)
    {
        // Simulation logic outputs: get the command and apply the NO/NC configuration requirement
        //logicOutputStatus = logicSimulationCommand ^ logicActiveState;
        logicOutputStatus = *(me->pLgcSimCmd);
    }            
    else// Handle Normal logics:
    {
        // The former list of "Alarms" {GENERAL_ALARM, MAX_MIN_ALARM, MIN_ALARM, MAX_ALARM, and
        // EMPTY_PIPE} has been replaced by a general Digital Output Alarm {GENERAL_ALARM} which
        // takes the Diagnosis-derived "alarmState" as its source.
            // Get General logic signal status from Diagnostics subsystem
            if (alarmState == (TUSIGN8)DIGITALOUTPUT_ALARM_BEHAVIOUR_OK)
            {
              logicOutputStatus = (TUSIGN8) DIGITALOUTPUT_LOGIC_LOW;
            }
            else
            {
              logicOutputStatus = (TUSIGN8) DIGITALOUTPUT_LOGIC_HIGH;
            }
       
    }
    
    
    *(me->pOutputLogic) = logicOutputStatus;
    
    // apply the NO/NC configuration requirement
    logicOutputStatus ^= *(me->pLgcActStte);	  // "NO" = as-is, "NC" inverts the signal	
    digitalOutputDynamic.logicValue = logicOutputStatus;
    
    return (logicOutputStatus);

}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Update None Output.
 \author Paul Li
 \date	2011-09-26
 \param *me
 \return port value.
 \warning
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 UpdateNone_DIGITALOUTPUT(const TLogic *me)
{
    TUSIGN8 logicOutputStatus = (TUSIGN8) DIGITALOUTPUT_LOGIC_LOW;
    
    InitDigitalOutputTimers();
  	output_mode = DIGITALOUTPUT_DO1_NONE;
    *(me->pOutputLogic) = DIGITALOUTPUT_LOGIC_LOW;
    
    //apply the NO/NC configuration requirement
    logicOutputStatus ^= *(me->pLgcActStte);	  // "NO" = as-is, "NC" inverts the signal	
    
    return (logicOutputStatus);

}