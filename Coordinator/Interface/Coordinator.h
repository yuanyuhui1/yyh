//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Class definition T_UNIT::COORDINATOR

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Coordinator.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Coordinator.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __COORDINATOR_H__
  #error Coordinator.h included more than once
#endif

#define __COORDINATOR_H__


//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // _CDT_DIAGNOSIS_ALARM_TABLE
  #define CDT_ALARM_CB_SIMUDATA 0 //simulation data alarm
  #define CDT_ALARM_CB_ALARM 1 //Alarm simulation.
  #define CDT_ALARM_CB_NV_ERROR 2 //EEPROM error
  #define CDT_ALARM_CB_MAINTENANCE 3 //Maintenance alarm
  #define CDT_ALARM_CB_UNCALIBRATED 4 //Piezo sensor uncalibrated alarm
  #define CDT_ALRAM_T_MAX 5 //
  #define CDT_ALRAM_T_MIN 6 //
  #define CDT_VORTEX_SENSOR_CIRCUIT_FAILURE 7 //
  #define CDT_VORTEX_SENSOR_OPEN 8 //
  #define CDT_VORTEX_SENSOR_SHORT 9 //
  #define CDT_VORTEX_SENSOR_DRIFT 10 //
  #define CDT_VIBRAT_SENSOR_CIRCUIT_FAILURE 11 //
  #define CDT_VIBRAT_SENSOR_OPEN 12 //
  #define CDT_VIBRAT_SENSOR_SHORT 13 //
  #define CDT_VIBRAT_SENSOR_DRIFT 14 //
  #define CDT_SYNC_MISSING 15 //
  #define CDT_SNR_FAILURE 16 //
  #define CDT_TEMP_SENSOR_UPPER_LIMIT 17 //
  #define CDT_TEMP_SENSOR_LOWER_LIMIT 18 //
  #define CDT_FE_RAM_FAILURE 19 //
  #define CDT_FE_NV_FAILURE 20 //
  #define CDT_FE_MAINTENANCE_MATURE 21 //
  #define CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED 22 //
  #define CDT_ALARM_REPLACE_REQUIRED_FE_TO_CB_BLOCKED 23 //
  #define CDT_ALARM_REPLACE_REQUIRED_BOTH_DIRECTION_ENABLED 24 //

  // 
  #define CDT_ALARM_NUM_CODES 25 //
  #define CDT_ALL_OPERATING_MODE 0X1FFF //all operating mode
  #define CDT_OPERATING_MODE_NUM 13 //Number of operating mode
  #define CDT_READ_METER_TYPE_INDEX 0 //
  #define CDT_READ_CALCULATION_DATA_INDEX 1 //
  #define CDT_READ_OPERATION_HOUR_INDEX 2 //
  #define CDT_READ_FE_MAINTAIN_CYCLE_INDEX 3 //
  #define CDT_READ_DSP_PARAM_CMD_INDEX 4 //
  #define CDT_READ_AUTOSELFCHECK_INDEX 5 //
  #define CDT_READ_SPAN_UPPRERRANGE_INDEX 6 //
  #define CDT_READ_OUTPUT_FILTER_ALG_INDEX 7 //
  #define CDT_READ_OUTPUT_FILTER_PARA_INDEX 8 //
  #define CDT_WRITE_METER_TYPE_INDEX 0 //
  #define CDT_WRITE_RESET_OP_HOUR_INDEX 1 //
  #define CDT_WRITE_FE_MAINTAIN_CYCLE_INDEX 2 //
  #define CDT_WRITE_DSP_PARAM_CMD_INDEX 3 //
  #define CDT_WRITE_DSP_DEFAULT_INDEX 4 //
  #define CDT_WRITE_DSP_FACTORY_INDEX 5 //
  #define CDT_WRITE_SAVE_DSP_DEFAULT_INDEX 6 //
  #define CDT_WRITE_SAVE_DSP_FACTORY_INDEX 7 //
  #define CDT_WRITE_SAVE_REPLACE_INDEX 8 //
  #define CDT_WRITE_AUTOSELFCHECK_INDEX 9 //
  #define CDT_WRITE_SPAN_UPPRERRANGE_INDEX 10 //
  #define CDT_WRITE_LOWFLOW_THRESHOLD_INDEX 11 //
  #define CDT_WRITE_OUTPUT_FILTER_ALG_INDEX 12 //
  #define CDT_WRITE_OUTPUT_FILTER_PARA_INDEX 13 //
  #define CDT_MAX_MAINTENANCE_CYCLE_HOURS 87600 //10 years
  #define CDT_MIN_MAINTENANCE_CYCLE_HOURS 0 //
  #define CDT_DEFAULT_MAINTENANCE_CYCLE_HOURS 17520 //2 years
  #define CDT_FE_SENSOR_TEMPERATURE_VALID_MIN (-200.0f) //below this value, the sensor temperature is not valid
  #define CDT_FE_SENSOR_TEMPERATURE_VALID_MAX (500.0f) //above this value, the senor temperature is not valid

  // _CDT_SIMULATION_MODE
  #define CDT_SIMULATION_CB_OFF 0 //Sim off
  #define CDT_SIMULATION_CB_QV_VALUE 1 //Qv Val
  #define CDT_SIMULATION_CB_QV_PERCENTAGE 2 //Qv Percentage
  #define CDT_SIMULATION_CB_QM_VALUE 3 //Qm Val
  #define CDT_SIMULATION_CB_QM_PERCENTAGE 4 //Qm Percentage
  #define CDT_SIMULATION_CB_MEDIUM_TEMP 5 //Medium Temp
  #define CDT_SIMULATION_CB_AMBIENT_TEMP 6 //Ambient Temp
  #define CDT_SIMULATION_CB_DO_FREQ 8 //DO Freq
  #define CDT_SIMULATION_CB_DO_LOGIC 9 //DO Logic
  #define CDT_SIMULATION_CB_DO_PULSE 10 //DO Puls
  #define CDT_SIMULATION_CB_AI_VALUE 12 //AI Val
  #define CDT_SIMULATION_FE_SENSOR_FREQ 13 //Sensor Freq

  // _CDT_FORCE_REPLACE
  #define CDT_FORCE_REPLACE_NONE 0 //NV replace is not enabled
  #define CDT_FORCE_REPLACE_FE_TO_CB_COMMON_ONLY 1 //NV replace from FE to CB for common data
  #define CDT_FORCE_REPLACE_FE_TO_CB 2 //NV replace from FE to CB for all data
  #define CDT_FORCE_REPLACE_CB_TO_FE 3 //NV replace from CB to FE for all data

  // _VORTEX_FAMILY_STATUS
  #define CDT_DEVICE_VARIABLE_SNR_FAILURE 0xD0 //Sensor bad signal quality
  #define CDT_DEVICE_VARIABLE_LIMIT_STATUS 0x30 //Device variables read only status
  #define CDT_MORE_DEVICE_STATUS_AVAILABLE 0x08 //More device family status available
  #define CDT_SENSOR_FAILURE 0x04 //Sensor failure(loss of shedder frequency signal)
  #define CDT_TRANSMITTER_FAILURE 0x02 //Transmitter failure
  #define CDT_FLOW_RATE_BELOW_LINEAR_RNG 0x01 //Flow rate is below linear range(too low Re number)

  // _SENSOR_CALIBRATION_STATUS
  #define CDT_SENSOR_IS_CALIBRATED 0 //Piezo sensor is calibarted.
  #define CDT_SENSOR_IS_UNCALIBRATED 1 //Piezo sensor is not calibarted.

  // _TEMP_SENSOR_TYPE
  #define CDT_TEMP_PT100 0 //PT100 temperature sensor
  #define CDT_TEMP_PT1000 1 //PT1000 temperature sensor
  #define CDT_TEMP_NONE 2 //

  // _SIMULATION_STATUS
  #define CDT_SIMULATION_DISABLE 0 //Disable device variable simulation.
  #define CDT_SIMULATION_ENABLE 1 //Enable device variable simulation.

  // _UNIT_TYPES
  #define CDT_T_TEMPERATURE 9 //Temperatire unit type
  #define CDT_T_FREQUENCY 47 //Frequency unit type
  #define CDT_T_MASS_DENSITY 25 //

  // _T_FREQUENCY
  #define CDT_HERTZ 1 //Hz

  // _T_TEMPERATURE
  #define CDT_KELVIN 1 //kelvin
  #define CDT_CELSIUS 2 //Celsius
  #define CDT_FAHRENHEIT 3 //Fahrenheit

  // _TSELECTION
  #define CDT_T_PRESET 0 //
  #define CDT_T_SENSOR 1 //
  #define CDT_T_AI 2 //
  #define CDT_T_HART 3 //

  // _OPERATING_MODE
  #define CDT_LIQUID_ACTUAL_VOLUME 0 //Liquid volume
  #define CDT_LIQUID_NORMAL_VOLUME 1 //Liquid normal volume
  #define CDT_LIQUID_MASS 2 //Liquid mass
  #define CDT_LIQUID_POWER 3 //Liquid power
  #define CDT_GAS_ACTUAL_VOLUME 4 //Gas actual volume
  #define CDT_GAS_STANDARD_VOLUME 5 //Gas standard volume
  #define CDT_GAS_MASS 6 //Gas mass
  #define CDT_GAS_POWER 7 //Gas power
  #define CDT_BIOGAS_ACTUAL_VOLUME 8 //Biogas actual volume
  #define CDT_BIOGAS_STANDARD_VOLUME 9 //Biogas standard volume
  #define CDT_STEAM_ACTUAL_VOLUME 10 //Steam actual volume
  #define CDT_STEAM_MASS 11 //Steam mass
  #define CDT_STEAM_POWER 12 //Steam power

  // _CDT_TASK
  #define CDT_MAIN_TASK 0x01 //
  #define CDT_SLOW_TASK 0x02 //
  #define CDT_CPULOAD_TASK 0x04 //
  #define CDT_SERVICE_TASK 0x08 //
  #define CDT_FE_TASK 0x10 //
  #define CDT_HART_TASK 0x20 //

  // _PASSWORD_CHECK_PASS
  #define CDT_INTERNALRTD_ENABLE 0x01 //
  #define CDT_ANALOGINPUT_ENABLE 0x02 //
  #define CDT_ENERGYFLOW_ENABLE 0x04 //

  // _CONVERTER_TYPE
  #define CDT_SWIREL 0 //
  #define CDT_VORTEX 1 //

  // _SWIRL_SIZE
  #define CDT_Swirl15mm 0 //
  #define CDT_Swirl20mm 1 //
  #define CDT_Swirl25mm 2 //
  #define CDT_Swirl32mm 3 //
  #define CDT_Swirl40mm 4 //
  #define CDT_Swirl50mm 5 //
  #define CDT_Swirl80mm 6 //
  #define CDT_Swirl100mm 7 //
  #define CDT_Swirl150mm 8 //
  #define CDT_Swirl200mm 9 //
  #define CDT_Swirl300mm 10 //
  #define CDT_Swirl400mm 11 //

  // _VORTEX_SIZE
  #define CDT_VotrexD15mm 0 //
  #define CDT_VotrexD25mm 1 //
  #define CDT_VotrexD40mm 2 //
  #define CDT_VotrexD50mm 3 //
  #define CDT_VotrexD80mm 4 //
  #define CDT_VotrexD100mm 5 //
  #define CDT_VotrexD150mm 6 //
  #define CDT_VotrexD200mm 7 //
  #define CDT_VotrexD250mm 8 //
  #define CDT_VotrexD300mm 9 //
  #define CDT_VotrexA15mm 10 //
  #define CDT_VotrexA25mm 11 //
  #define CDT_VotrexA40mm 12 //
  #define CDT_VotrexA50mm 13 //
  #define CDT_VotrexA80mm 14 //
  #define CDT_VotrexA100mm 15 //
  #define CDT_VotrexA150mm 16 //
  #define CDT_VotrexA200mm 17 //
  #define CDT_VotrexA250mm 18 //
  #define CDT_VotrexA300mm 19 //

  // _CMD_TYPE
  #define CDT_READ_DATA_CMD 0 //
  #define CDT_WRITE_DATA_CMD 1 //

  // _FE_READ_CMD
  #define CDT_READ_METER_TYPE 0 //
  #define CDT_READ_CALCULATION_DATA 1 //
  #define CDT_READ_OPERATION_HOUR 4 //
  #define CDT_READ_FE_MAINTAIN_CYCLE 6 //
  #define CDT_READ_DSP_PARAM_CMD 7 //
  #define CDT_READ_AUTOSELFDIAG_EN 0x0E //
  #define CDT_READ_SPAN_UPPRERRANGE 0x0A //
  #define CDT_READ_OUTPUT_FILTER_ALG 0x0F //
  #define CDT_READ_OUTPUT_FILTER_PARA 0x10 //

  // _FE_WRITE_CMD
  #define CDT_WRITE_METER_TYPE 0x80 //
  #define CDT_WRITE_RESET_OP_HOUR 0x84 //
  #define CDT_WRITE_FE_MAINTAIN_CYCLE 0x86 //
  #define CDT_WRITE_DSP_PARAM_CMD 0x87 //
  #define CDT_WRITE_DSP_DEFAULT 0x88 //
  #define CDT_WRITE_DSP_FACTORY 0x89 //
  #define CDT_WRITE_SAVE_DSP_DEFAULT 0x8A //
  #define CDT_WRITE_SAVE_DSP_FACTORY 0x8B //
  #define CDT_WRITE_SAVE_REPLACE 0x8D //
  #define CDT_WRITE_AUTOSELFDIAG_EN 0x8E //
  #define CDT_WRITE_SPAN_UPPRERRANGE 0x8F //
  #define CDT_WRITE_LOWFLOW_THRESHOLD 0x90 //
  #define CDT_WRITE_OUTPUT_FILTER_ALG 0x91 //
  #define CDT_WRITE_OUTPUT_FILTER_PARA 0x92 //

  // _STATUS
  #define CDT_STATUS_OFF 0 //
  #define CDT_STATUS_ON 1 //

  // CMD_DATA_LENGTH
  #define CDT_METER_TYPE_LENGTH 2 //
  #define CDT_CALCULATION_DATA_LENGTH 20 //
  #define CDT_OPERATION_HOUR_LENGTH 4 //
  #define CDT_FE_MAINTAIN_CYCLE_LENGTH 4 //
  #define CDT_DSP_PARAM_CMD_LENGTH 41 //
  #define CDT_SNR_LENGTH 2 //
  #define CDT_AUTOSELFDIAG_EN_LENGTH 2 //
  #define CDT_SPAN_UPPRERRANGE_LENGTH 2 //
  #define CDT_LOWFLOW_THRESHOLD_LENGTH 2 //
  #define CDT_DEFAULT_LENGTH 1 //
  #define CDT_OUTPUT_FILTER_ALG_LENGTH 2 //
  #define CDT_OUTPUT_FILTER_ALG_PARA_LENGTH 26 //

  // InitializeNv_Table
  #define CDT_NV_NOT_INITIALIZE 0 //
  #define CDT_NV_INITIALIZE_START 1 //
  #define CDT_NV_INITIALIZE_END_OK 2 //
  #define CDT_NV_INITIALIZE_END_FAILED 3 //

  // DENSITY_SELECTION_TABLE
  #define CDT_EXT_DENSITY 0 //
  #define CDT_CALCULATION_FROM_TP 1 //
  #define CDT_CALCULATION_FROM_T 2 //
  #define CDT_CALCULATION_FROM_P 3 //

  // GAIN_CONTROL_TAB
  #define CDT_GAIN_CONTROL_1TIMES 1 //
  #define CDT_GAIN_CONTROL_2TIMES 2 //
  #define CDT_GAIN_CONTROL_4TIMES 4 //
  #define CDT_GAIN_CONTROL_8TIMES 8 //

  // _CDT_STATUS_TABLE
  #define CDT_UNPROTECTED 0 //
  #define CDT_CALCULATE 1 //
  #define CDT_PROTECTED 2 //

  // REACTION_INPUT_FAIL
  #define CDT_PRESET 0 //
  #define CDT_LAST_GOOD_VALUE 1 //

  // _FE_OUT_FILTER_EXT_ALG_TABLE
  #define CDT_EXT_ALG_NO_FILTER 0 //
  #define CDT_EXT_ALG_PRESET_1 1 //
  #define CDT_EXT_ALG_PRESET_2 2 //
  #define CDT_EXT_ALG_SELF_CUSTOM 3 //

  // _FE_OUT_FILTER_INT_ALG_TABLE
  #define CDT_INT_ALG_USE_NONE 0x00 //
  #define CDT_INT_ALG_USE_SELF_STUDY 0x01 //
  #define CDT_INT_ALG_USE_SMOOTH_FILTER 0x02 //
  #define CDT_INT_ALG_USE_SS_AND_SF 0x03 //
//@SubGen end@ <TYPES>


#define MAIN_UPDATE_TIME       100
#define SLOW_UPDATE_TIME       250
#define HART_UPDATE_TIME       500
#define FE_UPDATE_TIME         70
#define SERVICE_UPDATE_TIME    100
#define MODBUS_UPDATE_TIME     500

#define LENGTH_OF_FUNCTION_PASSWORD 6
#define LENGTH_OF_ORDER_NUMBER      20

extern const SLOW TUSIGN8 _CDT_DIAGNOSIS_ALARM_TABLE[];

 #define CDT_DIAG_SIZE	25
//how many bits per word
#define CDT_DIAG_WORD_SIZE   8

#if( CDT_DIAG_WORD_SIZE == 8 )
    #define CDT_DIAG_BIT_(index)   ((index) &  7)
    #define CDT_DIAG_WRD_(index)   ((index) >> 3)
	  // words = (CDT_DIAG_SIZE) >> 3
#elif( CDT_DIAG_WORD_SIZE == 16 )
    #define CDT_DIAG_BIT_(index)   ((index) & 15)
    #define CDT_DIAG_WRD_(index)   ((index) >> 4)
	  // words = (CDT_DIAG_SIZE) >> 4
#elif( CDT_DIAG_WORD_SIZE == 32 )
    #define CDT_DIAG_BIT_(index)   ((index) & 31)
    #define CDT_DIAG_WRD_(index)   ((index) >> 5)
	  // words = (CDT_DIAG_SIZE) >> 5
#else
    #error invalid definition for CDT_DIAG_WORD_SIZE
#endif


#if(CDT_DIAG_SIZE > CDT_DIAG_WORD_SIZE)
  #define CDT_SETDIAG_(index,var)           (var[CDT_DIAG_WRD_(index)] |=  (1U << CDT_DIAG_BIT_(index)));\
	(var##_Duplicated[CDT_DIAG_WRD_(index)] |=  (1U << CDT_DIAG_BIT_(index)))

  #define CDT_GETDIAG_(index,var)    (TBOOL)(var[CDT_DIAG_WRD_(index)] &   (1U << CDT_DIAG_BIT_(index)))

  #define CDT_CLRDIAG_(index,var)           (var[CDT_DIAG_WRD_(index)] &= ~(1U << CDT_DIAG_BIT_(index)));\
    (var##_Duplicated[CDT_DIAG_WRD_(index)] &= ~(1U << CDT_DIAG_BIT_(index)))
#else
  #define CDT_SETDIAG_(index,var)           (var |=  (1U << CDT_DIAG_BIT_(index)));(var##_Duplicated |=  (1U << CDT_DIAG_BIT_(index)))
  #define CDT_CLRDIAG_(index,var)           (var &= ~(1U << CDT_DIAG_BIT_(index)));(var##_Duplicated &= ~(1U << CDT_DIAG_BIT_(index)))
  #define CDT_GETDIAG_(index,var)    (TBOOL)(var &   (1U << CDT_DIAG_BIT_(index)))
#endif


// _CDT_LOCAL_DIAGNOSIS_ALARM_TABLE
#define CDT_LOCAL_ALARM_CB_SIMUDATA 0 //simulation data alarm
#define CDT_LOCAL_ALARM_CB_ALARM 1 //Alarm simulation.
#define CDT_LOCAL_ALARM_CB_NV_NOT_INIT 2 //EEPROM is not initialized.
#define CDT_LOCAL_ALARM_CB_NV_ERROR 3 //EEPROM data error and could not be repairedmain could not be repaired
#define CDT_LOCAL_ALARM_CB_NV_HWDEFECT 4 //EEPROM hardware defect.

//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_COORDINATOR_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    COORDINATOR_DC_IDX_ReplaceStaticRare,
    COORDINATOR_DC_IDX_DynamicDuplicated,
    COORDINATOR_DC_IDX_Protected,
    COORDINATOR_DC_IDX_Unprotected,
    COORDINATOR_DC_IDX_ReplaceStaticRare_Calibration,
    COORDINATOR_DC_IDX_ReplaceStaticFreqStaticDefault,
    COORDINATOR_DC_IDX_FE_OUTPUT_FILTER_PARA,
    COORDINATOR_DC_IDX_CONSTANT,
    COORDINATOR_DC_IDX_STATIC_RARE,
    COORDINATOR_DC_IDX_STATIC_CYCLIC,
    COORDINATOR_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT,
    COORDINATOR_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_COORDINATOR
typedef struct _T_COORDINATOR
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* MainTaskEXE)(void);
    void (* FrontendTaskEXE)(void);
    void (* HartTaskEXE)(void);
    void (* SlowTaskEXE)(void);
    void (* ServicesTaskEXE)(void);
    void (* UpdateDiagnosis)(void);
    void (* UpdateWorkingTime)(void);
    void (* SendConfigToFE)(void);
    void (* StopTasksSRV)(TUSIGN16 taskIdx);
    TFLOAT (* GetTemperaturePercentageSRV)(void);
    void (* UpdateMaxTemperature)(void);
    TUSIGN16 (* UpdateSteamEnthalpy)(TUSIGN16 opModeShift);
    void (* CheckDataCrc)(void);
    void (* UpdateFunctionPassword)(void);
    TUSIGN16 (* UpdateInternalTemperature)(void);
    //@SubGen end@    <METHODS>
} T_COORDINATOR;


//-------------------------------------------------------------------------------------------------
//! define the T_COORDINATOR-object
extern SLOW const T_COORDINATOR coordinator;

extern void ( * comWrite_cdt[])(TUSIGN8 , TUSIGN8* const);
extern void ( * comRead_cdt[])(TUSIGN8 , TUSIGN8* const);

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_COORDINATOR
#define COORDINATOR_ME (&coordinator.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define coordinator_Get(objectIndex,attributeIndex,ptrValue) \
   coordinator.unit.Get(COORDINATOR_ME,objectIndex,attributeIndex,ptrValue)
#define coordinator_Put(objectIndex,attributeIndex,ptrValue) \
   coordinator.unit.Put(COORDINATOR_ME,objectIndex,attributeIndex,ptrValue)
#define coordinator_Check(objectIndex,attributeIndex,ptrValue) \
   coordinator.unit.Check(COORDINATOR_ME,objectIndex,attributeIndex,ptrValue)

#define coordinator_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   coordinator.unit.GetAttributeDescription(COORDINATOR_ME,objectIndex,attributeIndex,ptrDescriptor)
#define coordinator_GetObjectDescription(objectIndex,ptrDescriptor) \
   coordinator.unit.GetObjectDescription(COORDINATOR_ME,objectIndex,ptrDescriptor)

