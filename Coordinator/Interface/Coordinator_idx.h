//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator

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
//@SubGen Version@<0x020101>

#ifdef __COORDINATOR_IDX_H__
  #error Coordinator_idx.h included more than once
#endif

#define __COORDINATOR_IDX_H__

enum _T_COORDINATOR_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM[4]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_coodinatorAlarmSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_operatingMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_workingTimeMSec,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_totalWorkingTimeHour,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_partialWorkingTimeHour,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Simulation mode
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_simulationMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Piezo sensor frequency
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frequency,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frequencySim_Enable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frequency_Sim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frequency_MaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frequency_MinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frequency_Span,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INPUT
    */
    CDT_IDX_frequency_Span_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_frequency_range,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_frequency_range_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frequency_Percentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frequency_Damped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_frequency_Damped_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_frequency_UnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_simFrequency_UnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : INOUT
    */
    CDT_IDX_frequency_UnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INOUT
    */
    CDT_IDX_frequency_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INOUT
    */
    CDT_IDX_frequency_SimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    CDT_IDX_frequency_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frequency_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_frequency_DampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperature,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperatureSim_Enable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperature_Sim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_temperature_DampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperature_Damped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : OUTPUT
    */
    CDT_IDX_temperature_Damped_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_temperature_UnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    CDT_IDX_simTemperature_UnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    CDT_IDX_temperature_UnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INOUT
    */
    CDT_IDX_temperature_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INOUT
    */
    CDT_IDX_temperatureSim_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperature_MaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_temperature_MinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_temperature_Span,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INPUT
    */
    CDT_IDX_temperature_Span_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_temperature_range,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_temperature_range_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    CDT_IDX_temperature_Percentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    CDT_IDX_temperature_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_temperature_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_temperature_Selection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_intRTDType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     CPU load
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_cpuLoad,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Front end configuration parameters
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frontEndConfParamWr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frontEndConfParamRd,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_frontEndTempTypeRd,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : INOUT
    */
    CDT_IDX_updateTimeFast,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    CDT_IDX_updateTimeSlow,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_forceReplace,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Transmitter maintenace cycle
     \datatype          : USIGN32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_tranMaintenanceCyc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Vortex family variable status. Hart  command 152.
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_vortexDeviceStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Additional vortex family status. Hart command 152.
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_additionalVortexDeviceStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hart command 153.
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    CDT_IDX_familyRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Piezo sensor calibartion status.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    CDT_IDX_calibrationStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[6]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_internalRTDPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[6]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_analogInputPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[6]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_energyFlowPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_frequency_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_frequency_Damped_Duplictated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_temperature_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_temperature_Damped_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_temperaturePreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_temperaturePreset_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     cdtDiagnosis byte 0 for cdt ,byte 1&2 from frontend diagnosis
     \datatype          : SIMPLE_U8[4]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_cdtDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[4]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_cdtDiagnosis_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_passwordEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    CDT_IDX_temperatureMaxRecord,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_temperature_Percentage_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : RANGEFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_temperatureAlmRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_converterType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_vortexmeterSize,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_swirlmeterSize,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_snr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frontendOperationHour,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_frontendMaintenance,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_vbrCorrectionOn,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_vbrFilStep,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_vbrFilNumCoef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_fcdOn,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_fcdShift,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_fcdDelay,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_fcdNumCoef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lockMinIntPeriod,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lockMaxIntPeriod,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lockEstimation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_accuracyHigh,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_accuracyMiddle,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_accuracyLow,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_analogGain,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_gainCtrl,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_gainLowThresh,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_gainHighThresh,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_gainMarginFreq,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_gainMaxShift,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lowFlowThreshOn,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lowFlowThresh,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_lowFlowThreshCoef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_noiseSampSize,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_staticDecimation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_middleBufDecimation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_maxFlowFreq,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_minFlowFreq,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_freqMinMarginPer,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_freqMaxMarginPer,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_readParameters,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_writeParameters,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_saveCustomerDef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_saveFactoryDef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_resetCustomerDef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_resetFactoryDef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_resetFE_Runhour,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_temperatureOffsetCorrect,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_temperatureOffsetCorrectDisp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    CDT_IDX_timeStamp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    CDT_IDX_actualDensitySelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    CDT_IDX_tPresetChangeFlag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_protectedDataStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[30]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_page1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_temperatureSensor,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureSensor_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureSensor_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_protectedDataCrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_autoselfcheck_Enable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_spanUpperRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureAlmRange_obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureMaxRecord_obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    CDT_IDX_sigAmplitude,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_vbrAmplitude,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_sigMagnitude,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_autoselfcheck_Enable_Duplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureOffsetCorrectUser,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_temperatureOffsetCorrectUserDisp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    CDT_IDX_reaction_InputFail,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Both FE and CB maintenance cycle
     \datatype          : USIGN32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_BothMaintenanceCycle,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the raw sensor temperature value from FE
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    CDT_IDX_rawTemperatureSensor,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_ext_algSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_int_algSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_config_alg_paras,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    CDT_IDX_read_alg_block,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     average_or_max + activate percentage + study over count + tolerance + one direction retrigger counter + two direction retrigger counter + remove high counter + remove low counter + delay counter
     \datatype          : SIMPLE_U16[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_ss_paras,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     overall counter + remove high counter + remove low counter + delay counter
     \datatype          : SIMPLE_U16[4]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    CDT_IDX_sf_paras
    //@SubGen end@    <ENUMERATIONS>
};




