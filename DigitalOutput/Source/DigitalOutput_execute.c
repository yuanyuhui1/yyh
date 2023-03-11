//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
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
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Coordinator/Interface/Coordinator.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
#include "DigitalOutput/Interface/DigitalOutput.h"
#include "DigitalOutput/Interface/DigitalOutput_idx.h"
#include "digitalOutput_hardware.h"
#include "digitalOutput_logic.h"
#include "digitalOutput_pulse.h"
#include "digitalOutput_types.h"
#include "digitalOutput_attributes.h"
#include "digitalOutput_execute.h"
#include "DigitalOutput_Frequency.h"


TUSIGN8 output_mode = DIGITALOUTPUT_DO1_NONE;


//-------------------------------------------------------------------------------------------------
/*!
 \brief Set alarm to diagnosis.
 \author Paul Li
 \date	2011-09-26
 \param attributeIdx
 \return 
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
void SetDiagnosis_DO(TINT16 attributeIdx)
{
    TUSIGN8 value=SET_ALARM;
    VIP_ASSERT( attributeIdx < (TINT16)DIGITALOUTPUT_DO_ALARM_SIZE );
    VIP_ASSERT( attributeIdx >= 0 );
    (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_digitaloutputAlarm, attributeIdx, &value);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Clear alarm to diagnosis.
 \author Paul Li
 \date	2011-09-26
 \param attributeIdx
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
void ClearDiagnosis_DO(TINT16 attributeIdx)
{
    TUSIGN8 value=CLEAR_ALARM;
    VIP_ASSERT( attributeIdx < (TINT16)DIGITALOUTPUT_DO_ALARM_SIZE );
    VIP_ASSERT( attributeIdx >= 0 );
    (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_digitaloutputAlarm, attributeIdx, &value);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Check if the Qmax is changed.
 \author Paul Li
 \date	2011-09-26
 \param -
 \return -
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
void CheckQmaxChange()
{
    static TFLOAT upperRange = 0.0f;
    static TUSIGN8 unitType=0;
    static TUSIGN8  unitCode= 0;

    if((digitalOutputDynamic.gMaxData >= (upperRange + 0.001f)) || (digitalOutputDynamic.gMaxData <= (upperRange - 0.001f)) || \
       (digitalOutputDynamic.unitType != unitType) || (digitalOutputDynamic.unitCode !=unitCode))
        {
            upperRange=digitalOutputDynamic.gMaxData;
            unitType=digitalOutputDynamic.unitType;
            unitCode= digitalOutputDynamic.unitCode;
            QmaxChange();
        }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Check if the Mode is changed.
 \author Paul Li
 \date	2011-09-26
 \param -
 \return -
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

void CheckMeasurementChange(TUSIGN8 measurementChange)
{
    if(measurementChange)
    {
      digitalOutputDynamic.oldOutputMode = DIGITALOUTPUT_DO1_NONE;
      QmaxChange();
      InitDigitalOutputTimers();
      pulseOutputAction = NO_PULSES;
    }
    else 
    {
      CheckQmaxChange();
    }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Check if the Mode is changed.
 \author Paul Li
 \date	2011-10-17
 \param -
 \return -
 \warning
 \test
 \n test-date: 2011-10-17
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------

void CheckSimulationMode(TUSIGN8 simulationMode)
{
    digitalOutputDynamic.outputMode = plantData.outputMode;
    digitalOutputDynamic.simuEnabled = DIGITALOUTPUT_SIMU_DISABLED;
    if(simulationMode == CDT_SIMULATION_CB_DO_LOGIC)      //simulation data mode
    {
        digitalOutputDynamic.outputMode = DIGITALOUTPUT_DO1_LOGIC;
        digitalOutputDynamic.simuEnabled = DIGITALOUTPUT_SIMU_ENABLED;
    }
    if(simulationMode == CDT_SIMULATION_CB_DO_PULSE)      //simulation data mode
    {
        digitalOutputDynamic.outputMode = DIGITALOUTPUT_DO1_PULSE;
        digitalOutputDynamic.simuEnabled = DIGITALOUTPUT_SIMU_ENABLED;
    }
    if(simulationMode == CDT_SIMULATION_CB_DO_FREQ)      //simulation data mode
    {
        digitalOutputDynamic.outputMode = DIGITALOUTPUT_DO1_FREQUENCY;
        digitalOutputDynamic.simuEnabled = DIGITALOUTPUT_SIMU_ENABLED;
    }
}

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  
\if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
\author
\date
\param -
\return -
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_DIGITALOUTPUT(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
    TUSIGN16 result;
    TUSIGN8 i;
    TUSIGN16 alarm = 0x0000;
    TUSIGN16 alarmSimulation = 0x0000;
    TUSIGN8 diagnosisSimulationStatus;
    static TUSIGN16 alarm_static = 0x00;

    alarm = digitalOutputDynamic.alarmState_DO;
    result = diagnosis_Get(((TUSIGN16)DIAGNOSIS_IDX_simulationStatus),(TINT16)WHOLE_OBJECT,&diagnosisSimulationStatus);

    if((diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED) && (result == (TUSIGN16)OK))
    {
        for(i=0; i<DIGITALOUTPUT_DO_ALARM_SIZE; i++)
        {
            TUSIGN8 alarmBitSimulation = 0x00;

            result = digitalOutput_Get((TUSIGN16)DIGITALOUTPUT_IDX_alarmSimulation_DO, i, &alarmBitSimulation);

            if ((result == (TUSIGN16)OK) && (alarmBitSimulation == SET_ALARM)) 
            {
                alarmSimulation |= (1<<i);
            }
        }
        alarm = alarmSimulation;
    }

    if((alarm & (1 << DIGITALOUTPUT_PULSENUMMAXALARM)) )
    {
        if (alarm_static !=  (1 << DIGITALOUTPUT_PULSENUMMAXALARM))
        {
            SetDiagnosis_DO(DIGITALOUTPUT_PULSENUMMAXALARM);
            alarm_static |= (1 << DIGITALOUTPUT_PULSENUMMAXALARM);
        }
    }
    else if(alarm_static ==  (1 << DIGITALOUTPUT_PULSENUMMAXALARM))
    {
        alarm_static &= ~(1 << DIGITALOUTPUT_PULSENUMMAXALARM);
        ClearDiagnosis_DO(DIGITALOUTPUT_PULSENUMMAXALARM);
    }
    else
    {}
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEEXE> \endif
 \brief  Called by coordinator function to update logic/pulse/frequency/OIML outputs
 \param  digitalInput 
 \if @SubGen end@ <METHODHEADER UPDATEEXE> \endif
 \author    Paul Li
 \date      2011-09-22
 \param doAlmStat
 \return returnCode
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
//@SubGen start@<METHOD UPDATEEXE>
TUSIGN16 UpdateEXE_DIGITALOUTPUT(const T_DIGTALINPUT_PARAMETER* digitalInput)
//@SubGen end@<METHOD UPDATEEXE>
{
    TUSIGN16 returnCode = (TUSIGN16) OK; 
    static TUSIGN8 functionstatus = DIGITALOUTPUT_FUNCTION_DISABLE;
    digitalOutputDynamic.elapsedTime = digitalInput->elapseTime;
    digitalOutputDynamic.unitCode = digitalInput->unitCode;
    digitalOutputDynamic.unitType = digitalInput->unitType;
    digitalOutputDynamic.gMaxData = digitalInput->upperRange;
    (void)unitsOfMeasure.ConvertFromBaseUnit(digitalOutputDynamic.unitType,digitalOutputDynamic.unitCode, &digitalOutputDynamic.gMaxData);
    digitalOutputDynamic.DoPercentage = (digitalInput->DoPercentage)/100;

    if(functionstatus == DIGITALOUTPUT_FUNCTION_DISABLE)
    {
#if 0
        LOGIC_OP1_PORT_DR = OUTPUT_PORT;    // OP1 port direction set for O/P
#endif
        functionstatus = DIGITALOUTPUT_FUNCTION_ENABLE;
    }

    CheckSimulationMode( digitalInput->simulationMode);

    switch (digitalOutputDynamic.outputMode)
    {
    case DIGITALOUTPUT_DO1_NONE:
        break;

    case DIGITALOUTPUT_DO1_LOGIC:
#if 0
        LOGIC_OP1_PORT = UpdateLogic_DIGITALOUTPUT(&(digitalOutputDynamic.logic_DO1),digitalInput->doAlarm);
#endif
        break;

    case DIGITALOUTPUT_DO1_PULSE:
        CheckMeasurementChange(digitalInput->measurementChange);
        if((digitalOutputDynamic.oldOutputMode != DIGITALOUTPUT_DO1_PULSE))
        {
            (void)SetPulseWidthTimer(plantData.pulseWidth.value);
            digitalOutputDynamic.oldOutputMode = DIGITALOUTPUT_DO1_PULSE;
            pulseOutputAction = NO_PULSES;
        }

        returnCode = UpdatePulse_DIGITALOUTPUT(digitalInput->flowValue);
        break;

    case DIGITALOUTPUT_DO1_FREQUENCY:
        returnCode = UpdateFrequency_DIGITALOUTPUT(digitalOutputDynamic.DoPercentage);
        break;

    default:
    break;
    }

    return returnCode;
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
\param -
\return -
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