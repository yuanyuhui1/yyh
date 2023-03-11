//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <stdio.h>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Coordinator/Interface/subsystem_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "diagnosis_types.h"
#include "diagnosis_attributes.h"
#include "diagnosis_overload.h"
#include "diagnosis_execute.h"
#include "AnalogInput/Interface/AnalogInput.h"

#if defined(DIAGNOSIS_MODULE_TEST) || defined(DIAGNOSIS_MODULE_GET_PUT_SPEED_TEST) || defined(DIAGNOSIS_UPDATE_SPEED_TEST)
#include "diagnosis_module_test.h"
#endif

//@CodeGen start@ <INCLUDE>
#include "DigitalOutput/Interface/DigitalOutput.h"
//@CodeGen end@ <INCLUDE>

//-------------------------------------------------------------------------------------------------
FAST TUSIGN16 diagnosisState;     // Subsystem state
FAST T_DATA_SEMAPHORE diagnosisDataSemaphore;

//lint -e{651} Giovanni Invernizzi 2007-06-25 initializer are OK
//lint -e{708} Giovanni Invernizzi 2007-06-25 union initialization is OK
//@CodeGen start@ <DATACLASS LIST>
const SLOW T_DATACLASS diagnosisDataClassList[]=
{
    CONSTRUCTOR_DC_DYNAMIC(diagnosisDynamic,diagnosisDynamicInitializer),
    CONSTRUCTOR_DC_STATIC_CONSTDEF(diagnosisStaticFrequentConstantDefault,diagnosisStaticFrequentConstantDefaultInitializer),
    CONSTRUCTOR_DC_STATIC_NODEF(diagnosisStaticCyclic, diagnosisStaticCyclicInitializer)
};
//@CodeGen end@ <DATACLASS LIST>

//@CodeGen start@ <CONSTANT INIT>
//@CodeGen end@ <CONSTANT INIT>

FAST T_DIAGNOSIS_DYNAMIC diagnosisDynamic;
//lint -e{651} Giovanni Invernizzi 2007-06-25 initializer are OK
//lint -e{708} Giovanni Invernizzi 2007-06-25 union initialization is OK
const SLOW T_DIAGNOSIS_DYNAMIC diagnosisDynamicInitializer=
{
//@CodeGen start@ <DYNAMIC INIT>
    DIAGNOSIS_SIMULATION_DISABLED,    //simulationStatus
    0,    //analoginputAlarm
    {0, 0, 0, 0},    //coordinatorAlarm
    0,    //digitaloutputAlarm
    0,    //elettronicservicesAlarm
    0,    //frontendinterfaceAlarm
    0,    //mapperAlarm
    0,    //mvmeasurementAlarm
    0,    //svmeasurementAlarm
    0,    //totalizerAlarm
    0,    //analoginputAlarmSimulation
    {0, 0, 0, 0},    //coordinatorAlarmSimulation
    0,    //digitaloutputAlarmSimulation
    0,    //elettronicservicesAlarmSimulation
    0,    //frontendinterfaceAlarmSimulation
    0,    //mapperAlarmSimulation
    0,    //mvmeasurementAlarmSimulation
    0,    //svmeasurementAlarmSimulation
    0,    //totalizerAlarmSimulation
    0,     //diagnosisConditionIdx
    {0,0,0,0,0,0},     //diagnosisOutputConditionSimulation
    0,     //diagnosisCounter
//@CodeGen end@ <DYNAMIC INIT>
};

#ifdef WIN32
  #pragma bss_seg(".AAA$DIAGNOSIS_DATA")
    const FAST T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const FAST __no_init T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefault;
#endif
//lint -e{651} Giovanni Invernizzi 2007-06-25 initializer are OK
//lint -e{708} Giovanni Invernizzi 2007-06-25 union initialization is OK
const SLOW T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefaultInitializer=
{
//@CodeGen start@ <STATIC_FREQUENT_CONSTANT_DEFAULT INIT>
    {0,0,0,0,0,0},   //alarmMasking[6]
    DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1,    //DigitalOutputAlarmBehavior
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_fe_board_comm_error
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_fe_board_not_detected
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_pv_sensor_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_sv_sensor_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_tv_sensor_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_fe_board_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_fe_nv_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_fe_ram_failure
    DIAGNOSIS_CLASS_FAILURE, //configurableClassification_cb_nv_failure
    DIAGNOSIS_CLASS_CHECK_FUNCTION, //configurableClassification_cb_nv_warning
    DIAGNOSIS_CLASS_CHECK_FUNCTION, //configurableClassification_tot_counter_stop
    DIAGNOSIS_CLASS_CHECK_FUNCTION, //configurableClassification_no_hart_burst_in
    DIAGNOSIS_CLASS_CHECK_FUNCTION, //configurableClassification_alarm_simu_warning
    DIAGNOSIS_CLASS_CHECK_FUNCTION, //configurableClassification_data_simu_warning
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_steam_type_mismatch
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_process_flowrate_more_than_103
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_pv_sensor_out_lims
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_pv_sensor_out_range
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_sv_sensor_out_lims
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_sv_sensor_out_range
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_low_flowrate_cutoff
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_nv_concistency_check
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_tv_sensor_out_lims
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_fv_sensor_failure
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_ambient_temp_reach_tmax
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_ambient_temp_reach_tmin
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_nv_storage_active
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_fv_sensor_out_lims
    DIAGNOSIS_CLASS_OFF_SPECIFICATION, //configurableClassification_fv_sensor_out_range
    DIAGNOSIS_CLASS_MAINTENANCE, //configurableClassification_nv_replace_warn
    DIAGNOSIS_CLASS_MAINTENANCE, //configurableClassification_cb_insuf_input_voltage
    DIAGNOSIS_CLASS_MAINTENANCE, //configurableClassification_maintenance_warning
    DIAGNOSIS_CLASS_MAINTENANCE, //configurableClassification_fe_nv_warning
//@CodeGen end@ <STATIC_FREQUENT_CONSTANT_DEFAULT INIT>
};

#ifdef WIN32
  #pragma bss_seg(".AAA$DIAGNOSIS_DATA")
    const FAST T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclic;
  #pragma bss_seg()
#else
  #pragma location="DIAGNOSIS_STATIC_CYCLIC"
    const FAST __no_init T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclic;
#endif
//lint -e{651} Giovanni Invernizzi 2007-06-25 initializer are OK
//lint -e{708} Giovanni Invernizzi 2007-06-25 union initialization is OK
const SLOW T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclicInitializer=
{
//@CodeGen start@ <STATIC_CYCLIC INIT>
    {0,0,0,0,0,0},    //alarmHistory[6]
//@CodeGen end@ <STATIC_CYCLIC INIT>

//@CodeGen start@ <DETAILS INIT>
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_board_comm_errorDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_board_not_detectedDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_pv_sensor_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_sv_sensor_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_tv_sensor_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_board_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_nv_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_ram_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_cb_nv_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_cb_nv_warningDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_tot_counter_stopDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_no_hart_burst_inDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_alarm_simu_warningDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_data_simu_warningDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_steam_type_mismatchDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_process_flowrate_more_than_103Details
    {0, 0, 0, 0, 0},     //diagnosisCondition_pv_sensor_out_limsDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_pv_sensor_out_rangeDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_sv_sensor_out_limsDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_sv_sensor_out_rangeDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_low_flowrate_cutoffDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_nv_concistency_checkDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_tv_sensor_out_limsDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fv_sensor_failureDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_ambient_temp_reach_tmaxDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_ambient_temp_reach_tminDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_nv_storage_activeDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fv_sensor_out_limsDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fv_sensor_out_rangeDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_nv_replace_warnDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_cb_insuf_input_voltageDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_maintenance_warningDetails
    {0, 0, 0, 0, 0},     //diagnosisCondition_fe_nv_warningDetails
//@CodeGen end@ <DETAILS INIT>

//@CodeGen start@ <INPUT_ALARM_HISTORY INIT>
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//@CodeGen end@ <INPUT_ALARM_HISTORY INIT>
};

//-------------------------------------------------------------------------------------------------
// Table referenced by: alarmHistoryDescriptor
static const SLOW TUSIGN8 outputAlarmCodes[DIAGNOSIS_CONDITIONS_NUM_ELEMENTS] =
{
//@CodeGen start@ <ALARM CONDITIONS>
  DIAGNOSIS_CONDITION_FE_BOARD_COMM_ERROR,
  DIAGNOSIS_CONDITION_FE_BOARD_NOT_DETECTED,
  DIAGNOSIS_CONDITION_PV_SENSOR_FAILURE,
  DIAGNOSIS_CONDITION_SV_SENSOR_FAILURE,
  DIAGNOSIS_CONDITION_TV_SENSOR_FAILURE,
  DIAGNOSIS_CONDITION_FE_BOARD_FAILURE,
  DIAGNOSIS_CONDITION_FE_NV_FAILURE,
  DIAGNOSIS_CONDITION_FE_RAM_FAILURE,
  DIAGNOSIS_CONDITION_CB_NV_FAILURE,
  DIAGNOSIS_CONDITION_CB_NV_WARNING,
  DIAGNOSIS_CONDITION_TOT_COUNTER_STOP,
  DIAGNOSIS_CONDITION_NO_HART_BURST_IN,
  DIAGNOSIS_CONDITION_ALARM_SIMU_WARNING,
  DIAGNOSIS_CONDITION_DATA_SIMU_WARNING,
  DIAGNOSIS_CONDITION_STEAM_TYPE_MISMATCH,
  DIAGNOSIS_CONDITION_PROCESS_FLOWRATE_MORE_THAN_103,
  DIAGNOSIS_CONDITION_PV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_PV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_SV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_SV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_LOW_FLOWRATE_CUTOFF,
  DIAGNOSIS_CONDITION_NV_CONCISTENCY_CHECK,
  DIAGNOSIS_CONDITION_TV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_FV_SENSOR_FAILURE,
  DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMAX,
  DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMIN,
  DIAGNOSIS_CONDITION_NV_STORAGE_ACTIVE,
  DIAGNOSIS_CONDITION_FV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_FV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_NV_REPLACE_WARN,
  DIAGNOSIS_CONDITION_CB_INSUF_INPUT_VOLTAGE,
  DIAGNOSIS_CONDITION_MAINTENANCE_WARNING,
  DIAGNOSIS_CONDITION_FE_NV_WARNING
//@CodeGen end@ <ALARM CONDITIONS>
};

// Descriptor referenced by: alarmHistory
static const SLOW T_RANGE_E8 alarmHistoryDescriptor =
{
  outputAlarmCodes,  // ptrCodeTab
  DIAGNOSIS_CONDITIONS_NUM_ELEMENTS  // numberOfCodes
};

// Table referenced by: simulationStatus
static const SLOW TUSIGN8 simulationStatusCodes[] =
{
  DIAGNOSIS_SIMULATION_DISABLED,
  DIAGNOSIS_SIMULATION_ENABLED
};

// Descriptor referenced by: simulationStatus
static const SLOW T_RANGE_E8 simulationStatusDescriptor =
{
  simulationStatusCodes,  // ptrCodeTab
  DIAGNOSIS_SIMULATION_NUM_ELEMENTS  // numberOfCodes
};

//@CodeGen start@ <MASKING DESCRIPTOR>
static const SLOW TUSIGN8 outputAlarmMaskingCodes[] =
{
  DIAGNOSIS_CONDITION_CB_NV_WARNING,
  DIAGNOSIS_CONDITION_TOT_COUNTER_STOP,
  DIAGNOSIS_CONDITION_NO_HART_BURST_IN,
  DIAGNOSIS_CONDITION_ALARM_SIMU_WARNING,
  DIAGNOSIS_CONDITION_DATA_SIMU_WARNING,
  DIAGNOSIS_CONDITION_STEAM_TYPE_MISMATCH,
  DIAGNOSIS_CONDITION_PROCESS_FLOWRATE_MORE_THAN_103,
  DIAGNOSIS_CONDITION_PV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_PV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_SV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_SV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_LOW_FLOWRATE_CUTOFF,
  DIAGNOSIS_CONDITION_NV_CONCISTENCY_CHECK,
  DIAGNOSIS_CONDITION_TV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_FV_SENSOR_FAILURE,
  DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMAX,
  DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMIN,
  DIAGNOSIS_CONDITION_NV_STORAGE_ACTIVE,
  DIAGNOSIS_CONDITION_FV_SENSOR_OUT_LIMS,
  DIAGNOSIS_CONDITION_FV_SENSOR_OUT_RANGE,
  DIAGNOSIS_CONDITION_NV_REPLACE_WARN,
  DIAGNOSIS_CONDITION_CB_INSUF_INPUT_VOLTAGE,
  DIAGNOSIS_CONDITION_MAINTENANCE_WARNING,
  DIAGNOSIS_CONDITION_FE_NV_WARNING
};

static const SLOW T_RANGE_E8 alarmMaskingDescriptor =
{
  outputAlarmMaskingCodes,
  DIAGNOSIS_MASKING_NUM_ELEMENTS
};
//@CodeGen end@ <MASKING DESCRIPTOR>

//@CodeGen start@ <DESCRIPTORS>
extern const SLOW T_RANGE_E8 aiAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 coodinatorAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 alarmSimulation_DODescriptor;
extern const SLOW T_RANGE_E8 electronicServicesAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 frontendAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 mapperAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 mvmAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 svmAlarmSimulationDescriptor;
extern const SLOW T_RANGE_E8 totDiagnosisSimulationDescriptor;
extern const SLOW T_RANGE_E8 alarmState_DODescriptor;
//@CodeGen end@ <DESCRIPTORS>


const SLOW T_DATA_OBJ diagnosisObjectList[] =
{
//@CodeGen start@ <OBJECTS>
    //alarmHistory
    CONSTRUCT_ALARM(
    diagnosisStaticCyclic.alarmHistory,
    6 * sizeof(T_DIAGNOSIS_ALARM),
    NON_VOLATILE,
    &alarmHistoryDescriptor),

    //alarmMasking
    CONSTRUCT_ALARM(
    diagnosisStaticFrequentConstantDefault.alarmMasking,
    6 * sizeof(T_DIAGNOSIS_ALARM),
    NON_VOLATILE,
    &alarmMaskingDescriptor),

    //simulationStatus
     CONSTRUCT_TABENUM8(
    &diagnosisDynamic.simulationStatus,
    RAM,
    &simulationStatusDescriptor),

    //analoginputAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.analoginputAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &aiAlarmSimulationDescriptor),

    //coordinatorAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.coordinatorAlarm,
    4 * sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &coodinatorAlarmSimulationDescriptor),

    //digitaloutputAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.digitaloutputAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &alarmSimulation_DODescriptor),

    //elettronicservicesAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.elettronicservicesAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &electronicServicesAlarmSimulationDescriptor),

    //frontendinterfaceAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.frontendinterfaceAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &frontendAlarmSimulationDescriptor),

    //mapperAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.mapperAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &mapperAlarmSimulationDescriptor),

    //mvmeasurementAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.mvmeasurementAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &mvmAlarmSimulationDescriptor),

    //svmeasurementAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.svmeasurementAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &svmAlarmSimulationDescriptor),

    //totalizerAlarm Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.totalizerAlarm,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &totDiagnosisSimulationDescriptor),

    //analoginputAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.analoginputAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &aiAlarmSimulationDescriptor),

    //coordinatorAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.coordinatorAlarmSimulation,
    4 * sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &coodinatorAlarmSimulationDescriptor),

    //digitaloutputAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.digitaloutputAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &alarmSimulation_DODescriptor),

    //elettronicservicesAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.elettronicservicesAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &electronicServicesAlarmSimulationDescriptor),

    //frontendinterfaceAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.frontendinterfaceAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &frontendAlarmSimulationDescriptor),

    //mapperAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.mapperAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &mapperAlarmSimulationDescriptor),

    //mvmeasurementAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.mvmeasurementAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &mvmAlarmSimulationDescriptor),

    //svmeasurementAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.svmeasurementAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &svmAlarmSimulationDescriptor),

    //totalizerAlarmSimulation Object
    CONSTRUCT_ALARM(
    diagnosisDynamic.totalizerAlarmSimulation,
    sizeof(T_DIAGNOSIS_ALARM),
    RAM,
    &totDiagnosisSimulationDescriptor),

    //diagnosisConditionIdx
    CONSTRUCT_TABENUM8(
    &diagnosisDynamic.diagnosisConditionIdx,
    RAM,
    &alarmHistoryDescriptor),

    //diagnosisOutputConditionSimulation
    CONSTRUCT_ARRAY_SIMPLE_U8(
    &diagnosisDynamic.diagnosisOutputConditionSimulation,
    6,
    RAM),

    //diagnosisCounter
    CONSTRUCT_SIMPLE_U16(
    &diagnosisDynamic.diagnosisCounter,
    RAM),
//@CodeGen end@ <OBJECTS>

//@CodeGen start@ <BEHAVIOUR>
    //DigitalOutputAlarmBehavior Behavior Object
    CONSTRUCT_TABENUM8(
    &diagnosisStaticFrequentConstantDefault.DigitalOutputAlarmBehavior,
    NON_VOLATILE,
    &alarmState_DODescriptor),

//@CodeGen end@ <BEHAVIOUR>

//@CodeGen start@ <DETAILS>
    //diagnosisCondition_fe_board_comm_errorDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_board_comm_errorDetails),

    //diagnosisCondition_fe_board_not_detectedDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_board_not_detectedDetails),

    //diagnosisCondition_pv_sensor_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_pv_sensor_failureDetails),

    //diagnosisCondition_sv_sensor_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_sv_sensor_failureDetails),

    //diagnosisCondition_tv_sensor_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_tv_sensor_failureDetails),

    //diagnosisCondition_fe_board_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_board_failureDetails),

    //diagnosisCondition_fe_nv_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_nv_failureDetails),

    //diagnosisCondition_fe_ram_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_ram_failureDetails),

    //diagnosisCondition_cb_nv_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_cb_nv_failureDetails),

    //diagnosisCondition_cb_nv_warningDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_cb_nv_warningDetails),

    //diagnosisCondition_tot_counter_stopDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_tot_counter_stopDetails),

    //diagnosisCondition_no_hart_burst_inDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_no_hart_burst_inDetails),

    //diagnosisCondition_alarm_simu_warningDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_alarm_simu_warningDetails),

    //diagnosisCondition_data_simu_warningDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_data_simu_warningDetails),

    //diagnosisCondition_steam_type_mismatchDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_steam_type_mismatchDetails),

    //diagnosisCondition_process_flowrate_more_than_103Details Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_process_flowrate_more_than_103Details),

    //diagnosisCondition_pv_sensor_out_limsDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_pv_sensor_out_limsDetails),

    //diagnosisCondition_pv_sensor_out_rangeDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_pv_sensor_out_rangeDetails),

    //diagnosisCondition_sv_sensor_out_limsDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_sv_sensor_out_limsDetails),

    //diagnosisCondition_sv_sensor_out_rangeDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_sv_sensor_out_rangeDetails),

    //diagnosisCondition_low_flowrate_cutoffDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_low_flowrate_cutoffDetails),

    //diagnosisCondition_nv_concistency_checkDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_nv_concistency_checkDetails),

    //diagnosisCondition_tv_sensor_out_limsDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_tv_sensor_out_limsDetails),

    //diagnosisCondition_fv_sensor_failureDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fv_sensor_failureDetails),

    //diagnosisCondition_ambient_temp_reach_tmaxDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_ambient_temp_reach_tmaxDetails),

    //diagnosisCondition_ambient_temp_reach_tminDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_ambient_temp_reach_tminDetails),

    //diagnosisCondition_nv_storage_activeDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_nv_storage_activeDetails),

    //diagnosisCondition_fv_sensor_out_limsDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fv_sensor_out_limsDetails),

    //diagnosisCondition_fv_sensor_out_rangeDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fv_sensor_out_rangeDetails),

    //diagnosisCondition_nv_replace_warnDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_nv_replace_warnDetails),

    //diagnosisCondition_cb_insuf_input_voltageDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_cb_insuf_input_voltageDetails),

    //diagnosisCondition_maintenance_warningDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_maintenance_warningDetails),

    //diagnosisCondition_fe_nv_warningDetails Details Object
    CONSTRUCT_ALARM_COUNTER(&diagnosisStaticCyclic.diagnosisCondition_fe_nv_warningDetails),
//@CodeGen end@ <DETAILS>

//@CodeGen start@ <INPUT_ALARM_HISTORY>
    //configurableClassification_fe_board_comm_error
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_board_comm_error,
    NON_VOLATILE),

    //configurableClassification_fe_board_not_detected
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_board_not_detected,
    NON_VOLATILE),

    //configurableClassification_pv_sensor_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_pv_sensor_failure,
    NON_VOLATILE),

    //configurableClassification_sv_sensor_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_sv_sensor_failure,
    NON_VOLATILE),

    //configurableClassification_tv_sensor_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_tv_sensor_failure,
    NON_VOLATILE),

    //configurableClassification_fe_board_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_board_failure,
    NON_VOLATILE),

    //configurableClassification_fe_nv_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_nv_failure,
    NON_VOLATILE),

    //configurableClassification_fe_ram_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_ram_failure,
    NON_VOLATILE),

    //configurableClassification_cb_nv_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_cb_nv_failure,
    NON_VOLATILE),

    //configurableClassification_cb_nv_warning
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_cb_nv_warning,
    NON_VOLATILE),

    //configurableClassification_tot_counter_stop
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_tot_counter_stop,
    NON_VOLATILE),

    //configurableClassification_no_hart_burst_in
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_no_hart_burst_in,
    NON_VOLATILE),

    //configurableClassification_alarm_simu_warning
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_alarm_simu_warning,
    NON_VOLATILE),

    //configurableClassification_data_simu_warning
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_data_simu_warning,
    NON_VOLATILE),

    //configurableClassification_steam_type_mismatch
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_steam_type_mismatch,
    NON_VOLATILE),

    //configurableClassification_process_flowrate_more_than_103
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_process_flowrate_more_than_103,
    NON_VOLATILE),

    //configurableClassification_pv_sensor_out_lims
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_pv_sensor_out_lims,
    NON_VOLATILE),

    //configurableClassification_pv_sensor_out_range
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_pv_sensor_out_range,
    NON_VOLATILE),

    //configurableClassification_sv_sensor_out_lims
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_sv_sensor_out_lims,
    NON_VOLATILE),

    //configurableClassification_sv_sensor_out_range
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_sv_sensor_out_range,
    NON_VOLATILE),

    //configurableClassification_low_flowrate_cutoff
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_low_flowrate_cutoff,
    NON_VOLATILE),

    //configurableClassification_nv_concistency_check
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_nv_concistency_check,
    NON_VOLATILE),

    //configurableClassification_tv_sensor_out_lims
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_tv_sensor_out_lims,
    NON_VOLATILE),

    //configurableClassification_fv_sensor_failure
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fv_sensor_failure,
    NON_VOLATILE),

    //configurableClassification_ambient_temp_reach_tmax
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_ambient_temp_reach_tmax,
    NON_VOLATILE),

    //configurableClassification_ambient_temp_reach_tmin
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_ambient_temp_reach_tmin,
    NON_VOLATILE),

    //configurableClassification_nv_storage_active
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_nv_storage_active,
    NON_VOLATILE),

    //configurableClassification_fv_sensor_out_lims
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fv_sensor_out_lims,
    NON_VOLATILE),

    //configurableClassification_fv_sensor_out_range
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fv_sensor_out_range,
    NON_VOLATILE),

    //configurableClassification_nv_replace_warn
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_nv_replace_warn,
    NON_VOLATILE),

    //configurableClassification_cb_insuf_input_voltage
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_cb_insuf_input_voltage,
    NON_VOLATILE),

    //configurableClassification_maintenance_warning
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_maintenance_warning,
    NON_VOLATILE),

    //configurableClassification_fe_nv_warning
    CONSTRUCT_SIMPLE_U8(
    &diagnosisStaticFrequentConstantDefault.configurableClassification_fe_nv_warning,
    NON_VOLATILE),


    CONSTRUCT_ARRAY_SIMPLE(
    diagnosisStaticCyclic.inputAlarmHistory,
    ST_U8,
    48,
    NON_VOLATILE)
//@CodeGen end@ <INPUT_ALARM_HISTORY>
};


SLOW const T_DIAGNOSIS diagnosis =
{
    {
        &diagnosisState,
        &diagnosisDataSemaphore,

        diagnosisObjectList,
        //lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
        DIAGNOSIS_IDX_NUM_ELEMENTS - 1,

        diagnosisDataClassList,
        sizeof(diagnosisDataClassList)/sizeof(diagnosisDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_DIAGNOSIS, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        Put_T_UNIT_DIAGNOSIS,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }
#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
    ,GetStatusForOutputSRV
#endif
    ,UpdateDeviceStatusEXE
    ,GetNAMURStatusSRV
    ,GetActiveDiagnosticConditionsSRV
    ,GetDetailsOfDiagnosticConditionSRV

/*#ifdef DIAGNOSIS_MODULE_TEST
    ,ModuleTest
#endif*/
#ifdef DIAGNOSIS_MODULE_GET_PUT_SPEED_TEST
   ,GetPutSpeedTest
#endif
#ifdef DIAGNOSIS_UPDATE_SPEED_TEST
    ,UpdateSpeedTest
#endif
};

