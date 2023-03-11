//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Class definition T_UNIT::MAPPER

                Subsystem interface 
                for Subsystems based on T_UNIT.

Remarks        Version: 1.0.0                 
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Mapper.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Mapper.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __MAPPER_H__
  #error Mapper.h included more than once
#endif

#define __MAPPER_H__

#define MAPPER_VERSION 0x010000   // Version 1.0.0

//-------------------------------------------------------------------------------------------------
//!  define macros, enums
typedef struct _T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ
{
	TUSIGN8		index;
	TUSIGN16	subsystemIndex;
	TUSIGN16	objectIndex;
	TINT16		attributeIndex;
} T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ;

//@SubGen start@  <TYPES>
  // STATUS
  #define MAPPER_DISABLE 0 //
  #define MAPPER_ENABLE 1 //

  // ALARM_SIMULATION_TYPE
  #define MAPPER_OFF 0 //OFF
  #define MAPPER_Simulate_Digital_Out 2 //SIMULATE DIGITAL OUT
  #define MAPPER_VORTEX_SENSOR_FAILURE 4 //VORTEX SENSOR FAILURE
  #define MAPPER_TEMP_SENSOR_FAILURE 5 //TEMP SENSOR FAILURE
  #define MAPPER_VIBRAT_SENSOR_FAILURE 6 //VIBRAT SENSOR FAILURE
  #define MAPPER_ANALOGINPUT_OUT_OF_RANGE 7 //ANALOGINPUT OUT OF RANGE
  #define MAPPER_FLOWRATE_REACH_QMAX 8 //FLOWRATE REACH QMAX
  #define MAPPER_PROCESS_TEMP_REACH_TMAX 9 //PROCESS TEMP REACH TMAX
  #define MAPPER_ANALOGINPUT_CUT_OFF 10 //ANALOGINPUT CUT OFF
  #define MAPPER_PRESSURE_REACH_PMAX 11 //PRESSURE REACH PMAX
  #define MAPPER_FLOWRATE_REACH_QMIN 14 //FLOWRATE REACH QMIN
  #define MAPPER_PROCESS_TEMP_REACH_TMIN 15 //PROCESS TEMP REACH TMIN
  #define MAPPER_PRESSURE_REACH_PMIN 17 //PRESSURE REACH PMIN
  #define MAPPER_SNR_FAILURE 20 //SNR FAILURE
  #define MAPPER_FE_NV_FAILURE 21 //SENSOR NV FAILURE
  #define MAPPER_SENSOR_CAL_STATUS 22 //SENSOR CAL STATUS
  #define MAPPER_FE_SYNC_MISSING 23 //SENSOR SYNC MISSING
  #define MAPPER_FE_BOARD_COMM_ERROR 24 //SENSOR COMM ERROR
  #define MAPPER_CB_NV_FAILURE 27 //TRANSMITTER NV FAILURE
  #define MAPPER_AI_SPI_COM_FAIL 28 //AI SPI COM FAIL
  #define MAPPER_DIGITAL_OUT_CUTOFF 29 //PULSE OUT  CUTOFF
  #define MAPPER_RE_OUT_LINEAR_RANGE 30 //RE OUT LINEAR RANGE
  #define MAPPER_STEAM_TYPE_MISMATCH 34 //STEAM_TYPE_MISMATCH
  #define MAPPER_MAINTENANCE_WARNING 35 //MAINTENANCE WARNING
  #define MAPPER_CB_INSUF_INPUT_VOLTAGE 36 //TRANSMITTER INSUF INPUT VOLTAGE
  #define MAPPER_AMBIENT_TEMP_REACH_TMIN 37 //AMBIENT TEMP REACH TMIN
  #define MAPPER_AMBIENT_TEMP_REACH_TMAX 38 //AMBIENT TEMP REACH TMAX
  #define MAPPER_LOW_FLOWRATE_CUTOFF 39 //LOW FLOWRATE CUTOFF
  #define MAPPER_FLOWRATE_BG_103 40 //FLOWRATE BG 103
  #define MAPPER_DATA_SIMU_ALARM 41 //DATA SIMU ALARM
  #define MAPPER_DIAG_SIMU_ALARM 42 //DIAG SIMU ALARM
  #define MAPPER_NV_REPLACE_ERROR 47 //NV REPLACE ERROR
  #define MAPPER_FE_RAM_FAILURE 48 //SENSOR RAM FAILURE
  #define MAPPER_TOT_COUNTER_STOP 49 //TOT COUNTER STOP
  #define MAPPER_NO_HART_BURST_IN 51 //NO HART BURST IN

  // FLOATRANGE
  #define MAPPER_MAXVALUE 99999.9f //
  #define MAPPER_MINVALUE -99999.0f //

  // _UNIT_TYPES
  #define MAPPER_T_TEMPERATURE 9 //SI-Base-Quantities
  #define MAPPER_T_TIME 5 //
  #define MAPPER_T_VOLUME 17 //
  #define MAPPER_T_MASS_DENSITY 25 //Derivatives A
  #define MAPPER_T_SPECIFIC_VOLUME 27 //Derivatives A
  #define MAPPER_T_VOLUME_FLOW 39 //Derivatives A
  #define MAPPER_T_MASS_FLOW 41 //Derivatives A
  #define MAPPER_T_NORMAL_VOLUME_FLOW 42 //
  #define MAPPER_T_FREQUENCY 47 //
  #define MAPPER_T_PRESSURE 51 //Containing special Names
  #define MAPPER_T_POWER 55 //Containing special Names
  #define MAPPER_T_ANALYTICAL 139 //percent

  // _PERCENT_UNIT
  #define MAPPER_PERCENT 1 //

  // _T_VOLUME
  #define MAPPER_CUBIC_METER 1 //
  #define MAPPER_LITER 11 //

  // _T_TEMPERATURE_UNIT
  #define MAPPER_KELVIN 1 //Kevin
  #define MAPPER_CELSIUS 2 //Celsius
  #define MAPPER_FAHRENHEIT 3 //F

  // _T_MASS_DENSITY_UNIT
  #define MAPPER_KILOGRAM_PER_CUBIC_METER 1 //Kg/m3
  #define MAPPER_GRAM_PER_CUBIC_CENTIMETER 3 //g/cm3
  #define MAPPER_KILOGRAM_PER_LITER 6 //Kg/l
  #define MAPPER_GRAM_PER_MILLILITER 7 //g/ml
  #define MAPPER_GRAM_PER_LITER 8 //g/l
  #define MAPPER_POUND_PER_CUBIC_INCH 9 //lb/in
  #define MAPPER_POUND_PER_CUBIC_FOOT 10 //lb/ft3

  // _T_VOLUME_FLOW_UNIT
  #define MAPPER_CUBIC_METER_PER_SECOND 13 //
  #define MAPPER_CUBIC_METER_PER_MINUTE 14 //
  #define MAPPER_CUBIC_METER_PER_HOUR 15 //
  #define MAPPER_CUBIC_METER_PER_DAY 16 //
  #define MAPPER_CUBIC_FEET_PER_SECOND 29 //
  #define MAPPER_CUBIC_FEET_PER_MINUTE 30 //
  #define MAPPER_CUBIC_FEET_PER_HOUR 31 //
  #define MAPPER_CUBIC_FEET_PER_DAY 32 //
  #define MAPPER_LITER_PER_SECOND 48 //
  #define MAPPER_LITER_PER_MINUTE 49 //
  #define MAPPER_LITER_PER_HOUR 50 //
  #define MAPPER_LITER_PER_DAY 51 //
  #define MAPPER_KILOLITER_PER_SECOND 52 //
  #define MAPPER_KILOLITER_PER_MINUTE 60 //
  #define MAPPER_KILOLITER_PER_HOUR 61 //
  #define MAPPER_KILOLITER_PER_DAY 62 //
  #define MAPPER_US_GALLON_PER_SECOND 63 //
  #define MAPPER_US_GALLON_PER_MINUTE 64 //
  #define MAPPER_US_GALLON_PER_HOUR 65 //
  #define MAPPER_US_GALLON_PER_DAY 66 //
  #define MAPPER_IMPERIAL_GALLON_PER_SECOND 83 //
  #define MAPPER_IMPERIAL_GALLON_PER_MINUTE 84 //
  #define MAPPER_IMPERIAL_GALLON_PER_HOUR 85 //

  // _T_VOLUME_FLOW
  #define MAPPER_IMPERIAL_GALLON_PER_DAY 86 //
  #define MAPPER_BARREL_PER_SECOND 104 //
  #define MAPPER_BARREL_PER_MINUTE 105 //
  #define MAPPER_BARREL_PER_HOUR 106 //
  #define MAPPER_BARREL_PER_DAY 107 //

  // _T_NORM_VOLUME_FLOW
  #define MAPPER_NORMAL_CUBIC_METER_PER_SECOND_0C_1ATM 1 //
  #define MAPPER_NORMAL_CUBIC_METER_PER_MINUTE_0C_1ATM 2 //
  #define MAPPER_NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM 3 //
  #define MAPPER_NORMAL_CUBIC_METER_PER_DAY_0C_1ATM 4 //
  #define MAPPER_NORMAL_LITER_PER_SECOND_0C_1ATM 5 //
  #define MAPPER_NORMAL_LITER_PER_MINUTE_0C_1ATM 6 //
  #define MAPPER_NORMAL_LITER_PER_HOUR_0C_1ATM 7 //
  #define MAPPER_NORMAL_LITER_PER_DAY_0C_1ATM 8 //
  #define MAPPER_STANDARD_CUBIC_FEET_PER_MINUTE_60F_1ATM 9 //
  #define MAPPER_STANDARD_CUBIC_FEET_PER_HOUR_60F_1ATM 10 //
  #define MAPPER_STANDARD_CUBIC_METER_PER_SECOND_20C_1ATM 11 //
  #define MAPPER_STANDARD_CUBIC_METER_PER_MINUTE_20C_1ATM 12 //
  #define MAPPER_STANDARD_CUBIC_METER_PER_HOUR_20C_1ATM 13 //
  #define MAPPER_STANDARD_CUBIC_METER_PER_DAY_20C_1ATM 14 //
  #define MAPPER_STANDARD_LITER_PER_SECOND_20C_1ATM 15 //
  #define MAPPER_STANDARD_LITER_PER_MINUTE_20C_1ATM 16 //
  #define MAPPER_STANDARD_LITER_PER_HOUR_20C_1ATM 17 //
  #define MAPPER_STANDARD_LITER_PER_DAY_20C_1ATM 18 //

  // _T_MASS_FLOW
  #define MAPPER_GRAM_PER_SECOND 1 //g/s
  #define MAPPER_GRAM_PER_MINUTE 2 //g/min
  #define MAPPER_GRAM_PER_HOUR 3 //g/h
  #define MAPPER_KILOGRAM_PER_SECOND 5 //kg/s
  #define MAPPER_KILOGRAM_PER_MINUTE 6 //kg/min
  #define MAPPER_KILOGRAM_PER_HOUR 7 //kg/h
  #define MAPPER_KILOGRAM_PER_DAY 8 //kg/d

  // _T_PRESSURE_UNIT
  #define MAPPER_PASCAL_U 1 //Pa
  #define MAPPER_MEGAPASCAL 3 //MPa
  #define MAPPER_KILOPASCAL 4 //KPa
  #define MAPPER_HECTOPASCAL 7 //HPa
  #define MAPPER_BAR 8 //bar
  #define MAPPER_MILLIBAR 9 //mbar
  #define MAPPER_POUNDFORCE_PER_SQUARE_INCH 65 //psi
  #define MAPPER_KILOGRAMFORCE_PER_SQUARE_CENTIMETER 69 //kg/cm2
  #define MAPPER_MILLIMETER_H2O 53 //mm H2O

  // TF_CODE
  #define MAPPER_LINEAR 0 //
  #define MAPPER_SQUARE_ROOT 1 //
  #define MAPPER_SQUARE_ROOT_THIRD_POWER 2 //
  #define MAPPER_SQUARE_ROOT_FIFTH_POWER 3 //
  #define MAPPER_SPECIAL_CURVE 4 //
  #define MAPPER_SQUARE 5 //
  #define MAPPER_DISCRETE 230 //
  #define MAPPER_SQUARE_ROOT_PLUS_SPECIAL_CURVE 231 //
  #define MAPPER_SQUARE_ROOT_THIRD_POWER_PLUS_SPECIAL_CURVE 232 //
  #define MAPPER_SQUARE_ROOT_FIFTH_POWER_PLUS_SPECIAL_CURVE 233 //
  #define MAPPER_NOT_USED 250 //
  #define MAPPER_NONE 251 //
  #define MAPPER_UNKNOWN 252 //
  #define MAPPER_SPECIAL 253 //

  // AC_FLAG
  #define MAPPER_NOT_CONNECTED 0 //
  #define MAPPER_CONNECTED 1 //

  // DEVICE_VAR_CODE
  #define MAPPER_DEVICE_VAR_QV 0 //METERINGLABEL_Qv
  #define MAPPER_DEVICE_VAR_QN 1 //METERINGLABEL_Qn
  #define MAPPER_DEVICE_VAR_QM 2 //METERINGLABEL_Qm
  #define MAPPER_DEVICE_VAR_POWER 3 //METERINGLABEL_Temperature
  #define MAPPER_DEVICE_VAR_PARTIAL_QV 4 //Power - Heat Flow
  #define MAPPER_DEVICE_VAR_PARTIAL_QN 5 //
  #define MAPPER_DEVICE_VAR_T 7 //
  #define MAPPER_DEVICE_VAR_TOTQV 12 //
  #define MAPPER_DEVICE_VAR_TOTQN 13 //
  #define MAPPER_DEVICE_VAR_TOTQM 14 //
  #define MAPPER_DEVICE_VAR_TOTENERGY 15 //
  #define MAPPER_DEVICE_VAR_TOTQV_PARTIAL 16 //
  #define MAPPER_DEVICE_VAR_TOTQN_PARTIAL 17 //
  #define MAPPER_UNSUPPORTED_DEVICE_VAR 250 //

  // T_RANGE_CHOOSE
  #define MAPPER_UPPER_RANGE 0 //
  #define MAPPER_LOWERER_RANGE 1 //

  // _T_UNIT
  #define MAPPER_PERCENT_UNIT_CODE 57 //PERCENT_UNIT

  // FLOWOVER103_BHV
  #define MAPPER_FLOWOVER103_BHV_ALARM_OFF 0 //
  #define MAPPER_FLOWOVER103_BHV_ALARM_HI 3 //
  #define MAPPER_FLOWOVER103_BHV_ALARM_LO 4 //

  // _OPM_SEL_MASK
  #define MAPPER_SELECTION_LIQUIDQV 0x0001 //
  #define MAPPER_SELECTION_LIQUIDQN 0x0002 //
  #define MAPPER_SELECTION_LIQUIDQM 0x0004 //
  #define MAPPER_SELECTION_LIQUIDPOWER 0x0008 //
  #define MAPPER_SELECTION_GASQV 0x0010 //
  #define MAPPER_SELECTION_GASQN 0x0020 //
  #define MAPPER_SELECTION_GASQM 0x0040 //
  #define MAPPER_SELECTION_GASPOWER 0x0080 //
  #define MAPPER_SELECTION_GASPARTIALQV 0x0100 //
  #define MAPPER_SELECTION_GASPARTIALQN 0x0200 //
  #define MAPPER_SELECTION_STEAMQV 0x0400 //
  #define MAPPER_SELECTION_STEAMQM 0x0800 //
  #define MAPPER_SELECTION_STEAMPOWER 0x1000 //

  // 
  #define MAPPER_OPM_NUM 13 //

  // _SIMU_STATUS
  #define MAPPER_SIMU_DISABLE 0 //
  #define MAPPER_SIMU_ENABLE 1 //

  // MapperStatusTable
  #define MAPPER_UNPROTECTED 0 //
  #define MAPPER_CALCULATE 1 //
  #define MAPPER_PROTECTED 2 //

  // _TEXT_SRC
  #define MAPPER_TEXT_FROM_AI 0 //
  #define MAPPER_TEXT_FROM_HART 1 //
  #define MAPPER_TEXT_FROM_PRESET 2 //

  // _PRESSURE_SRC
  #define MAPPER_PRESSURE_FROM_AI 0 //
  #define MAPPER_PRESSURE_FROM_HART 1 //
  #define MAPPER_PRESSURE_FROM_PRESET 2 //

  // _DENSITY_SRC
  #define MAPPER_DENSITY_FROM_AI 0 //
  #define MAPPER_DENSITY_FROM_HART 1 //
  #define MAPPER_DENSITY_FROM_PRESET 2 //

  // _CONTENT_SRC
  #define MAPPER_CONTENT_FORM_AI 0 //
  #define MAPPER_CONTENT_FORM_HART 1 //
  #define MAPPER_CONTENT_FORM_PRESET 2 //

  // _MAPPER_DIAGNOSIS_ALARM_TABLE
  #define MAPPER_ALM_PRESSURE_REACH_MAX 0 //
  #define MAPPER_ALM_PRESSURE_REACH_MIN 1 //
  #define MAPPER_ALM_NO_HART_BURST_IN 2 //

  // _PRESET_STATE
  #define MAPPER_PRESET_ACCESSED 0 //
  #define MAPPER_PRESET_CHANGED 1 //

  // namurAlarmStatusTable
  #define MAPPER_NAMUR_NO_ALARM 0 //
  #define MAPPER_NAMUR_CHECKFUNCTION 1 //
  #define MAPPER_NAMUR_OFFSPECIFICATION 2 //
  #define MAPPER_NAMUR_MAINTENANCE 3 //
  #define MAPPER_NAMUR_FAILURE 4 //
//@SubGen end@ <TYPES>
  
  #define MAPPER_DIAG_SIZE        3

  #define MAPPER_DIAG_WORD_SIZE   8   

#if( MAPPER_DIAG_WORD_SIZE == 8 )
    #define MAPPER_DIAG_BIT_(index)   ((index) &  7)
    #define MAPPER_DIAG_WRD_(index)   ((index) >> 3)
	  // words = (AI_DIAG_SIZE) >> 3
#elif( MAPPER_DIAG_WORD_SIZE == 16 )
    #define MAPPER_DIAG_BIT_(index)   ((index) & 15)
    #define MAPPER_DIAG_WRD_(index)   ((index) >> 4)
	  // words = (AI_DIAG_SIZE) >> 4
#elif( MAPPER_DIAG_WORD_SIZE == 32 )
    #define MAPPER_DIAG_BIT_(index)   ((index) & 31)
    #define MAPPER_DIAG_WRD_(index)   ((index) >> 5)
	  // words = (AI_DIAG_SIZE) >> 5
#else
    #error invalid definition for MAPPER_DIAG_WORD_SIZE
#endif

#if(MAPPER_DIAG_SIZE > MAPPER_DIAG_WORD_SIZE)
  #define MAPPER_SETDIAG_(index,var)           (var[MAPPER_DIAG_WRD_(index)] |=  (1U << MAPPER_DIAG_BIT_(index)))
  #define MAPPER_CLRDIAG_(index,var)           (var[MAPPER_DIAG_WRD_(index)] &= ~(1U << MAPPER_DIAG_BIT_(index)))
  #define MAPPER_GETDIAG_(index,var)    (TBOOL)(var[MAPPER_DIAG_WRD_(index)] &   (1U << MAPPER_DIAG_BIT_(index)))
#else
  #define MAPPER_SETDIAG_(index,var)           (var |=  (1U << MAPPER_DIAG_BIT_(index)))
  #define MAPPER_CLRDIAG_(index,var)           (var &= ~(1U << MAPPER_DIAG_BIT_(index)))
  #define MAPPER_GETDIAG_(index,var)    (TBOOL)(var &   (1U << MAPPER_DIAG_BIT_(index)))
#endif


#define MAPPER_OBJECT_LOCAL_DIAGNOSIS    mapperUnprotected.mapperDiagnosis

#define PRESSURE_MAX_ALARM    	0x01
#define PRESSURE_MIN_ALARM      0x02

//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_MAPPER_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    MAPPER_DC_IDX_Unprotected,
    MAPPER_DC_IDX_Protected,
    MAPPER_DC_IDX_DynamicDuplicated,
    MAPPER_DC_IDX_CONSTANT,
    MAPPER_DC_IDX_STATIC_RARE,
    MAPPER_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT,
    MAPPER_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT,
    MAPPER_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_MAPPER
typedef struct _T_MAPPER
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    TUSIGN16 (* GetPVPercentageForDOSRV)(TFLOAT* ptrValue);
    TUSIGN16 (* SetSelection)(TUSIGN16 selection);
    void (* CheckDataCrc)(void);
    TUSIGN16 (* UpdateDeviceVariablesEXE)(void);
    TUSIGN16 (* UpdatePVSVAssignment)(void);
    void (* UpdateDiagnosis)(void);
    TFLOAT (* GetDummyPercentageSRV)(void);
    //@SubGen end@    <METHODS>
} T_MAPPER;


//-------------------------------------------------------------------------------------------------
//! define the T_MAPPER-object
extern SLOW const T_MAPPER mapper;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_MAPPER
#define MAPPER_ME (&mapper.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define mapper_Get(objectIndex,attributeIndex,ptrValue) \
   mapper.unit.Get(MAPPER_ME,objectIndex,attributeIndex,ptrValue)
#define mapper_Put(objectIndex,attributeIndex,ptrValue) \
   mapper.unit.Put(MAPPER_ME,objectIndex,attributeIndex,ptrValue)
#define mapper_Check(objectIndex,attributeIndex,ptrValue) \
   mapper.unit.Check(MAPPER_ME,objectIndex,attributeIndex,ptrValue)

#define mapper_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   mapper.unit.GetAttributeDescription(MAPPER_ME,objectIndex,attributeIndex,ptrDescriptor)
#define mapper_GetObjectDescription(objectIndex,ptrDescriptor) \
   mapper.unit.GetObjectDescription(MAPPER_ME,objectIndex,ptrDescriptor)

