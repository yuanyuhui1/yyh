//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis

 Module

 Description    This header file defines the complete list of public data-objects.
                Other subsystems need to use these macros instead of magic-numbers
                when accessing the public data-objects.

                The index of private data-object should not be available through
                the TUNIT interface.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifdef __DIAGNOSIS_IDX_H__
#error Diagnosis_idx.h included more than once
#endif

#define __DIAGNOSIS_IDX_H__

enum _T_DIAGNOSIS_IDX
{
//@CodeGen start@ <IDX>
    //alarmHistory
    //datatype          : T_DIAGNOSIS_ALARM[32]
    //typequalifier     : ARRAY
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_alarmHistory,

    //alarmMasking
    //datatype          : T_DIAGNOSIS_ALARM[32]
    //typequalifier     : ARRAY
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_alarmMasking,

    //simulationStatus
    //datatype          : TABENUM8
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_simulationStatus,

    //analoginputAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_analoginputAlarm,

    //coordinatorAlarm
    //datatype          : T_DIAGNOSIS_ALARM[4]
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_coordinatorAlarm,

    //digitaloutputAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_digitaloutputAlarm,

    //elettronicservicesAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_elettronicservicesAlarm,

    //frontendinterfaceAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_frontendinterfaceAlarm,

    //mapperAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_mapperAlarm,

    //mvmeasurementAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_mvmeasurementAlarm,

    //svmeasurementAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_svmeasurementAlarm,

    //totalizerAlarm
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_totalizerAlarm,

    //analoginputAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_analoginputAlarmSimulation,

    //coordinatorAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM[4]
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_coordinatorAlarmSimulation,

    //digitaloutputAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_digitaloutputAlarmSimulation,

    //elettronicservicesAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,

    //frontendinterfaceAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_frontendinterfaceAlarmSimulation,

    //mapperAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_mapperAlarmSimulation,

    //mvmeasurementAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,

    //svmeasurementAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_svmeasurementAlarmSimulation,

    //totalizerAlarmSimulation
    //datatype          : T_DIAGNOSIS_ALARM
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_totalizerAlarmSimulation,

    //diagnosisConditionIdx
    //datatype          : TABENUM8
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisConditionIdx,

    //diagnosisOutputConditionSimulation
    //datatype          : T_SIMPLE_U8[6]
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisOutputConditionSimulation,

    //diagnosisCounter
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : RAM
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCounter,

    //alarmState_DODescriptor_DigitalOutputAlarmBehavior
    //datatype          : TABENUM8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_DigitalOutputAlarmBehavior,

    //flowOver103AlarmStateDescriptor_FlowOver103Behavior
    //datatype          : TABENUM8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_FlowOver103Behavior,
    //diagnosisCondition_fe_board_comm_errorDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_board_comm_errorDetails,

    //diagnosisCondition_fe_board_not_detectedDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_board_not_detectedDetails,

    //diagnosisCondition_pv_sensor_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_pv_sensor_failureDetails,

    //diagnosisCondition_sv_sensor_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_sv_sensor_failureDetails,

    //diagnosisCondition_tv_sensor_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_tv_sensor_failureDetails,

    //diagnosisCondition_fe_board_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_board_failureDetails,

    //diagnosisCondition_fe_nv_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_nv_failureDetails,

    //diagnosisCondition_fe_ram_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_ram_failureDetails,

    //diagnosisCondition_cb_nv_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_cb_nv_failureDetails,

    //diagnosisCondition_cb_nv_warningDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_cb_nv_warningDetails,

    //diagnosisCondition_tot_counter_stopDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_tot_counter_stopDetails,

    //diagnosisCondition_no_hart_burst_inDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_no_hart_burst_inDetails,

    //diagnosisCondition_alarm_simu_warningDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_alarm_simu_warningDetails,

    //diagnosisCondition_data_simu_warningDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_data_simu_warningDetails,

    //diagnosisCondition_steam_type_mismatchDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_steam_type_mismatchDetails,

    //diagnosisCondition_process_flowrate_more_than_103Details
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_process_flowrate_more_than_103Details,

    //diagnosisCondition_pv_sensor_out_limsDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_pv_sensor_out_limsDetails,

    //diagnosisCondition_pv_sensor_out_rangeDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_pv_sensor_out_rangeDetails,

    //diagnosisCondition_sv_sensor_out_limsDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_sv_sensor_out_limsDetails,

    //diagnosisCondition_sv_sensor_out_rangeDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_sv_sensor_out_rangeDetails,

    //diagnosisCondition_low_flowrate_cutoffDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_low_flowrate_cutoffDetails,

    //diagnosisCondition_nv_concistency_checkDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_nv_concistency_checkDetails,

    //diagnosisCondition_tv_sensor_out_limsDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_tv_sensor_out_limsDetails,

    //diagnosisCondition_fv_sensor_failureDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fv_sensor_failureDetails,

    //diagnosisCondition_ambient_temp_reach_tmaxDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_ambient_temp_reach_tmaxDetails,

    //diagnosisCondition_ambient_temp_reach_tminDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_ambient_temp_reach_tminDetails,

    //diagnosisCondition_nv_storage_activeDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_nv_storage_activeDetails,

    //diagnosisCondition_fv_sensor_out_limsDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fv_sensor_out_limsDetails,

    //diagnosisCondition_fv_sensor_out_rangeDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fv_sensor_out_rangeDetails,

    //diagnosisCondition_nv_replace_warnDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_nv_replace_warnDetails,

    //diagnosisCondition_cb_insuf_input_voltageDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_cb_insuf_input_voltageDetails,

    //diagnosisCondition_maintenance_warningDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_maintenance_warningDetails,

    //diagnosisCondition_fe_nv_warningDetails
    //datatype          : T_DIAGNOSIS_ALARM_COUNTER
    //typequalifier     : STRUCT
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_diagnosisCondition_fe_nv_warningDetails,


    //configurableClassification_fe_board_comm_error
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_board_comm_error,

    //configurableClassification_fe_board_not_detected
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_board_not_detected,

    //configurableClassification_pv_sensor_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_pv_sensor_failure,

    //configurableClassification_sv_sensor_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_sv_sensor_failure,

    //configurableClassification_tv_sensor_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_tv_sensor_failure,

    //configurableClassification_fe_board_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_board_failure,

    //configurableClassification_fe_nv_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_nv_failure,

    //configurableClassification_fe_ram_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_ram_failure,

    //configurableClassification_cb_nv_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_cb_nv_failure,

    //configurableClassification_cb_nv_warning
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_cb_nv_warning,

    //configurableClassification_tot_counter_stop
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_tot_counter_stop,

    //configurableClassification_no_hart_burst_in
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_no_hart_burst_in,

    //configurableClassification_alarm_simu_warning
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_alarm_simu_warning,

    //configurableClassification_data_simu_warning
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_data_simu_warning,

    //configurableClassification_steam_type_mismatch
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_steam_type_mismatch,

    //configurableClassification_process_flowrate_more_than_103
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_process_flowrate_more_than_103,

    //configurableClassification_pv_sensor_out_lims
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_pv_sensor_out_lims,

    //configurableClassification_pv_sensor_out_range
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_pv_sensor_out_range,

    //configurableClassification_sv_sensor_out_lims
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_sv_sensor_out_lims,

    //configurableClassification_sv_sensor_out_range
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_sv_sensor_out_range,

    //configurableClassification_low_flowrate_cutoff
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_low_flowrate_cutoff,

    //configurableClassification_nv_concistency_check
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_nv_concistency_check,

    //configurableClassification_tv_sensor_out_lims
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_tv_sensor_out_lims,

    //configurableClassification_fv_sensor_failure
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fv_sensor_failure,

    //configurableClassification_ambient_temp_reach_tmax
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_ambient_temp_reach_tmax,

    //configurableClassification_ambient_temp_reach_tmin
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_ambient_temp_reach_tmin,

    //configurableClassification_nv_storage_active
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_nv_storage_active,

    //configurableClassification_fv_sensor_out_lims
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fv_sensor_out_lims,

    //configurableClassification_fv_sensor_out_range
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fv_sensor_out_range,

    //configurableClassification_nv_replace_warn
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_nv_replace_warn,

    //configurableClassification_cb_insuf_input_voltage
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_cb_insuf_input_voltage,

    //configurableClassification_maintenance_warning
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_maintenance_warning,

    //configurableClassification_fe_nv_warning
    //datatype          : T_SIMPLE_U8
    //typequalifier     : SIMPLE
    //storage qualifier : NON_VOLATILE
    //direction         : INOUT
    DIAGNOSIS_IDX_configurableClassification_fe_nv_warning,

    DIAGNOSIS_IDX_NUM_ELEMENTS
//@CodeGen end@ <IDX>
};
