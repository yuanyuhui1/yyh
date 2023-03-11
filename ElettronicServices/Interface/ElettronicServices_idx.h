//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices

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

#ifdef __ELETTRONIC_SERVICES_IDX_H__
  #error ElettronicServices_idx.h included more than once
#endif

#define __ELETTRONIC_SERVICES_IDX_H__

enum _T_ELETTRONIC_SERVICES_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_elettronicTemperature_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_softwareRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_hardwareRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_deviceSerialNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_feSerialNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_vSupplyReadBack,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_dipSwitchAndPushButtonsStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_swWriteProtection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_swRevisionHmi_transmitter,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_hwRevisionHmi_transmitter,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_deviceReset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_electronicServicesAlarmSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_localDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_cbSerialNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_resetOutputTrimming,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_elettronicTemperature,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_vSupplyReadBackDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_protectedDataCrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_localDiagnosisDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_initializeNv,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_I16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_resetToDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_I16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_saveAsDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_I16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_loadRomDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_I16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_classLoadRomDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_lowLevelHmiHwSwRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_vSupplyReadbackEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_vSupplyReadbackHighThreshold,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_vSupplyReadbackLowThreshold,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_vSupplyReadbackRetry,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_ramTestEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_romTestEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_cbNvWriteIsActive,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_feNvWriteIsActive,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_dynamicVariablesCheckEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_T_UnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_elettronicTemperatureDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : RANGEFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_alarmTRng,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_alarmTRng_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_protectedDataStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_hardwareCalibrationLoad,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_sensorID,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Sensor SAP/ERP No.
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_sensorSapErpNo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Sensor calibration date.
     \datatype          : SIMPLE_CHAR[12]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_sensorCalDate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Sensor calibration certificate No.
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_sensorCalCertNo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Sensor calibration location
     \datatype          : SIMPLE_CHAR[32]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_sensorCalLocation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_bootloaderRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[12]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranID,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Transmitter SAP/ERP No.
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranSapErpNo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Transmitter calibration date.
     \datatype          : SIMPLE_CHAR[12]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranCalDate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Transmitter calibration certificate No.
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranCalCertNo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Transmitter calibration location.
     \datatype          : SIMPLE_CHAR[32]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_tranCalLocation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_manufacturer,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_street,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_city,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Phone
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_phone,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_writeAutoZero,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_writeSelfDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_resetCB_Runhour,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[5]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_frontendRtc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_autozeroStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_autoSelfCheckTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_snrThreshold,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_sensorLocationTag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[20]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_sensoTag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_softwareMainRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_hardwareMainRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_swRevisionHmi_fe,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_CHAR[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_hwRevisionHmi_fe,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_formatFENV,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Record the maximum value of ambient temperature.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_ambientTemperatureMax,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_ambientTemperatureMax_obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_saveAllAsDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_resetAllToDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    ELETTRONICSERVICES_IDX_acttelettronicTemperature,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    ELETTRONICSERVICES_IDX_acttelettronicTemperature_obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_switchBLE,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ELETTRONICSERVICES_IDX_modeLCDLight,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    ELETTRONICSERVICES_IDX_configDataCheck
    //@SubGen end@    <ENUMERATIONS>
};




