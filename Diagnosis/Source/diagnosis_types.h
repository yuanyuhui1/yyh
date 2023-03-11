//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before Diagnosis_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Diagnosis_types.h
#endif

#ifndef __T_DATA_OBJ_ALARM_H__
  #error t_data_obj_alarm.h must be included before diagnosis_types.h
#endif

#ifdef __DIAGNOSIS_TYPES_H__
  #error Diagnosis_types.h included more than once
#endif

#define __DIAGNOSIS_TYPES_H__

// implement the required storage-classes
typedef struct _T_DIAGNOSIS_INPUT_TABLE {
#ifdef DIAGNOSIS_USE_CONDITION_DETAILS
    T_SUBSYSTEM_IDX                 subIdx;
#endif
    void FAST                       *objPtr;                  // pointer to the t_data_obj_alarm used to store the alarm
    TUSIGN8                         alarm;                    // enumeration of the alarm
    TUSIGN8                         priority;
    //T_DIAGNOSIS_CLASSES             classification;
    const TUSIGN8					*classPtr;
    T_DIAGNOSIS_GROUPS              group;
    TUSIGN8                         outputIdx;                // enumeration of the effect of the alarm on the diagnosis output
#ifdef DIAGNOSIS_USE_ALARM_DETAILED_HISTORY
    const T_DIAGNOSIS_ALARM_COUNTER *alarmRunTimeCounterPtr;  // detailed alarm history pointer
#endif
    const T_DIAGNOSIS_ALARM_COUNTER *outputRunTimeCounterPtr; // detailed output history pointer
} T_DIAGNOSIS_INPUT_TABLE;

typedef struct _T_DIAGNOSIS_FASTOUTPUT_TABLE {
    T_SUBSYSTEM_IDX                 subsysIdx;                // subsystem to drive
    const T_DIAGNOSIS_INPUT_TABLE   *inputTableAlarmPtr;      // pointer to the inputTable row containing the alarm
#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
    const T_BEHAVIOUR               *behaviourPtr;            // pointer to the behaviour object, leave 0 if no user behaviour is defined
#endif
} T_DIAGNOSIS_FASTOUTPUT_TABLE;

typedef struct _T_DIAGNOSIS_TABLE_ADDRESSING {
    TUSIGN8        start;
    TUSIGN8        end;
} T_DIAGNOSIS_TABLE_ADDRESSING;

typedef struct _T_DIAGNOSIS_SIMULATION_OBJ_ADDRESSING {
    TUSIGN16             objIdx;
    const T_UNIT SLOW    *pSub;
} T_DIAGNOSIS_SIMULATION_OBJ_ADDRESSING;

//========add for simulation HMI mapper start====================================
typedef struct _T_DIAGNOSIS_SIMULATION_MAPPER {
	 TUSIGN16			   subIdx;
	 TUSIGN16	           objIdx;
	 TUSIGN16	           subobjIdx;
} T_DIAGNOSIS_SIMULATION_MAPPER;
//========add for simulation HMI mapper end=====================================


typedef struct _T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
//@CodeGen start@ <STATIC_FREQUENT_CONSTANT_DEFAULT TYPES>
    T_DIAGNOSIS_ALARM alarmMasking[6];
    TUSIGN8 DigitalOutputAlarmBehavior;
    TUSIGN8 configurableClassification_fe_board_comm_error;
    TUSIGN8 configurableClassification_fe_board_not_detected;
    TUSIGN8 configurableClassification_pv_sensor_failure;
    TUSIGN8 configurableClassification_sv_sensor_failure;
    TUSIGN8 configurableClassification_tv_sensor_failure;
    TUSIGN8 configurableClassification_fe_board_failure;
    TUSIGN8 configurableClassification_fe_nv_failure;
    TUSIGN8 configurableClassification_fe_ram_failure;
    TUSIGN8 configurableClassification_cb_nv_failure;
    TUSIGN8 configurableClassification_cb_nv_warning;
    TUSIGN8 configurableClassification_tot_counter_stop;
    TUSIGN8 configurableClassification_no_hart_burst_in;
    TUSIGN8 configurableClassification_alarm_simu_warning;
    TUSIGN8 configurableClassification_data_simu_warning;
    TUSIGN8 configurableClassification_steam_type_mismatch;
    TUSIGN8 configurableClassification_process_flowrate_more_than_103;
    TUSIGN8 configurableClassification_pv_sensor_out_lims;
    TUSIGN8 configurableClassification_pv_sensor_out_range;
    TUSIGN8 configurableClassification_sv_sensor_out_lims;
    TUSIGN8 configurableClassification_sv_sensor_out_range;
    TUSIGN8 configurableClassification_low_flowrate_cutoff;
    TUSIGN8 configurableClassification_nv_concistency_check;
    TUSIGN8 configurableClassification_tv_sensor_out_lims;
    TUSIGN8 configurableClassification_fv_sensor_failure;
    TUSIGN8 configurableClassification_ambient_temp_reach_tmax;
    TUSIGN8 configurableClassification_ambient_temp_reach_tmin;
    TUSIGN8 configurableClassification_nv_storage_active;
    TUSIGN8 configurableClassification_fv_sensor_out_lims;
    TUSIGN8 configurableClassification_fv_sensor_out_range;
    TUSIGN8 configurableClassification_nv_replace_warn;
    TUSIGN8 configurableClassification_cb_insuf_input_voltage;
    TUSIGN8 configurableClassification_maintenance_warning;
    TUSIGN8 configurableClassification_fe_nv_warning;
//@CodeGen end@ <STATIC_FREQUENT_CONSTANT_DEFAULT TYPES>
 } T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_DIAGNOSIS_DYNAMIC
 {
    TUSIGN8 simulationStatus;

//@CodeGen start@ <DYNAMIC TYPES>
    T_DIAGNOSIS_ALARM analoginputAlarm[1];
    T_DIAGNOSIS_ALARM coordinatorAlarm[4];
    T_DIAGNOSIS_ALARM digitaloutputAlarm[1];
    T_DIAGNOSIS_ALARM elettronicservicesAlarm[1];
    T_DIAGNOSIS_ALARM frontendinterfaceAlarm[1];
    T_DIAGNOSIS_ALARM mapperAlarm[1];
    T_DIAGNOSIS_ALARM mvmeasurementAlarm[1];
    T_DIAGNOSIS_ALARM svmeasurementAlarm[1];
    T_DIAGNOSIS_ALARM totalizerAlarm[1];
    T_DIAGNOSIS_ALARM analoginputAlarmSimulation[1];
    T_DIAGNOSIS_ALARM coordinatorAlarmSimulation[4];
    T_DIAGNOSIS_ALARM digitaloutputAlarmSimulation[1];
    T_DIAGNOSIS_ALARM elettronicservicesAlarmSimulation[1];
    T_DIAGNOSIS_ALARM frontendinterfaceAlarmSimulation[1];
    T_DIAGNOSIS_ALARM mapperAlarmSimulation[1];
    T_DIAGNOSIS_ALARM mvmeasurementAlarmSimulation[1];
    T_DIAGNOSIS_ALARM svmeasurementAlarmSimulation[1];
    T_DIAGNOSIS_ALARM totalizerAlarmSimulation[1];
    TUSIGN8 diagnosisConditionIdx;
    TUSIGN8 diagnosisOutputConditionSimulation[6];
    TUSIGN16 diagnosisCounter;
//@CodeGen end@ <DYNAMIC TYPES>
 } T_DIAGNOSIS_DYNAMIC;


//@CodeGen start@ <CONSTANT TYPES>
//@CodeGen end@ <CONSTANT TYPES>

 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_CYCLIC block
 typedef struct _T_DIAGNOSIS_STATIC_CYCLIC
 {
 //@CodeGen start@ <STATIC_CYCLIC TYPES>
    T_DIAGNOSIS_ALARM alarmHistory[6];
 //@CodeGen start@ <STATIC_CYCLIC TYPES>

//@CodeGen start@ <DETAILS TYPES>
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_board_comm_errorDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_board_not_detectedDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_pv_sensor_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_sv_sensor_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_tv_sensor_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_board_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_nv_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_ram_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_cb_nv_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_cb_nv_warningDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_tot_counter_stopDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_no_hart_burst_inDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_alarm_simu_warningDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_data_simu_warningDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_steam_type_mismatchDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_process_flowrate_more_than_103Details;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_pv_sensor_out_limsDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_pv_sensor_out_rangeDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_sv_sensor_out_limsDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_sv_sensor_out_rangeDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_low_flowrate_cutoffDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_nv_concistency_checkDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_tv_sensor_out_limsDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fv_sensor_failureDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_ambient_temp_reach_tmaxDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_ambient_temp_reach_tminDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_nv_storage_activeDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fv_sensor_out_limsDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fv_sensor_out_rangeDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_nv_replace_warnDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_cb_insuf_input_voltageDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_maintenance_warningDetails;
    T_DIAGNOSIS_ALARM_COUNTER diagnosisCondition_fe_nv_warningDetails;
//@CodeGen end@ <DETAILS TYPES>

//@CodeGen start@ <INPUT_ALARM_HISTORY TYPES>
    TUSIGN8 inputAlarmHistory[48];
//@CodeGen end@ <INPUT_ALARM_HISTORY TYPES>
 } T_DIAGNOSIS_STATIC_CYCLIC;

