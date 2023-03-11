//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Class definition T_UNIT::ELETTRONIC_SERVICES

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ElettronicServices.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before ElettronicServices.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __ELETTRONIC_SERVICES_H__
  #error ElettronicServices.h included more than once
#endif

#define __ELETTRONIC_SERVICES_H__

#define ELETTRONIC_SERVICES_VERSION (0x010302)   // Version 1.3.2

#define CYCLIC      	0
#define START_UP    	1


//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // dipSwitchAndPushButtonsStatusTable
  #define ELETTRONICSERVICES_PUSH_BUTTONS_ZERO_STATUS 0x0001 //
  #define ELETTRONICSERVICES_PUSH_BUTTONS_SPAN_STATUS 0x0002 //
  #define ELETTRONICSERVICES_PUSH_BUTTONS_WRITE_PROTECT_STATUS 0x0004 //
  #define ELETTRONICSERVICES_DIP_SWITCH_1_STATUS 0x0008 //
  #define ELETTRONICSERVICES_DIP_SWITCH_2_STATUS 0x0010 //
  #define ELETTRONICSERVICES_DIP_SWITCH_3_STATUS 0x0020 //
  #define ELETTRONICSERVICES_DIP_SWITCH_4_STATUS 0x0040 //
  #define ELETTRONICSERVICES_DIP_SWITCH_5_STATUS 0x0080 //
  #define ELETTRONICSERVICES_DIP_SWITCH_6_STATUS 0x0100 //

  // SwWriteProtectionTable
  #define ELETTRONICSERVICES_WRITE_PROTECTION_OFF 0 //
  #define ELETTRONICSERVICES_WRITE_PROTECTION_ON 1 //

  // SwWriteProtectionDefault
  #define ELETTRONICSERVICES_WRITE_PROTECTION_NONE 251 //

  // ElectronicServicesDiagnosisEnumTable
  #define ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW 0 //
  #define ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH 1 //
  #define ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH 4 //
  #define ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW 5 //

  // ElectronicServicesDiagnosisEnumCodes
  #define ELETTRONICSERVICES_ALARM_CODES 6 //

  // 
  #define ELETTRONICSERVICES_ALARM_CODES_DUPLICATED ELETTRONICSERVICES_ALARM_CODES //
  #define ELETTRONICSERVICES_V_SUPPLY_READBACK_HIGH_THRESHOLD 40000 //
  #define ELETTRONICSERVICES_V_SUPPLY_READBACK_LOW_THRESHOLD 10450 //
  #define ELETTRONICSERVICES_V_SUPPLY_READBACK_RETRY 5 //
  #define ELETTRONICSERVICES_TEMPERATURE_READBACK_RETRY 5 //
  #define ELETTRONICSERVICES_CAL_LOCATION_CODES 6 //
  #define ELETTRONICSERVICES_CALIBRATION_LOAD 0x35 //
  #define ELETTRONICSERVICES_READ_FE_RTC_INDEX 0 //
  #define ELETTRONICSERVICES_READ_FE_VERSION_INDEX 1 //
  #define ELETTRONICSERVICES_READ_SNR_THRESHOLD_INDEX 3 //
  #define ELETTRONICSERVICES_READ_AUTOZERO_STATUS_INDEX 2 //
  #define ELETTRONICSERVICES_WRITE_AUTO_ZERO_INDEX 0 //
  #define ELETTRONICSERVICES_WRITE_SELF_DIAGNOSIS_INDEX 1 //
  #define ELETTRONICSERVICES_WRITE_HW_VERSION_INDEX 3 //
  #define ELETTRONICSERVICES_WRITE_FE_RTC_INDEX 2 //
  #define ELETTRONICSERVICES_WRITE_SNR_THRESHOLD_INDEX 4 //
  #define ELETTRONICSERVICES_WRITE_FORMAT_FENV_INDEX 5 //
  #define ELETTRONICSERVICES_BOOT_VER_NUM_ADDR 0x12F00 //Boot loader version number stored address.
  #define ELETTRONICSERVICES_BOOT_VER_NUM_LEN 3 //Boot loader version number length. High version number is the first byte.
  #define ELETTRONICSERVICES_CB_HW_VER_NUM_ADDR 0xE000 //
  #define ELETTRONICSERVICES_CB_HW_VER_NUM_LEN 3 //High version number is the first byte.
  #define ELETTRONICSERVICES_ROM_CHECK_VALUE_CONFIG 0x3456 //it is use to check the configure data

  // InitializeNvTable
  #define ELETTRONICSERVICES_NV_NOT_INITIALIZE 0 //
  #define ELETTRONICSERVICES_NV_INITIALIZE_START 1 //
  #define ELETTRONICSERVICES_NV_INITIALIZE_END_OK 2 //
  #define ELETTRONICSERVICES_NV_INITIALIZE_END_FAILED 3 //

  // SilTestTable
  #define ELETTRONICSERVICES_SIL_TEST_OFF 0 //
  #define ELETTRONICSERVICES_SIL_TEST_ON 1 //

  // NvWriteIsActiveTable
  #define ELETTRONICSERVICES_WRITE_NOT_ACTIVE 0 //
  #define ELETTRONICSERVICES_WRITE_ACTIVE 1 //

  // CalibrationLocationTable
  #define ELETTRONICSERVICES_CAL_IN_SHANGHAI 0 //Shanghai
  #define ELETTRONICSERVICES_CAL_IN_STONEHOUSE 1 //Stonehouse
  #define ELETTRONICSERVICES_CAL_IN_GOETTINGEN 2 //Goettingen
  #define ELETTRONICSERVICES_CAL_IN_WARMINSTER 3 //Warminster
  #define ELETTRONICSERVICES_CAL_IN_SYDNEY 4 //Sydney
  #define ELETTRONICSERVICES_CAL_IN_BEIJING 5 //Beijing

  // _UNIT_TYPES
  #define ELETTRONICSERVICES_T_TEMPERATURE 9 //SI-Base-Quantities

  // TEMPERATURE
  #define ELETTRONICSERVICES_KELVIN 1 //Kevin
  #define ELETTRONICSERVICES_CELSIUS 2 //Celsius
  #define ELETTRONICSERVICES_FAHRENHEIT 3 //F

  // PROTECT_TABLE
  #define ELETTRONICSERVICES_UNPROTECTED 0 //
  #define ELETTRONICSERVICES_CALCULATE 1 //
  #define ELETTRONICSERVICES_PROTECTED 2 //

  // _FE_READ_CMD
  #define ELETTRONICSERVICES_READ_FE_RTC 5 //
  #define ELETTRONICSERVICES_READ_FE_VERSION 8 //
  #define ELETTRONICSERVICES_READ_SNR_THRESHOLD 12 //
  #define ELETTRONICSERVICES_READ_AUTOZERO_STATUS 9 //

  // _FE_WRITE_CMD
  #define ELETTRONICSERVICES_WRITE_AUTO_ZERO 0x81 //
  #define ELETTRONICSERVICES_WRITE_SELF_DIAGNOSIS 0x82 //
  #define ELETTRONICSERVICES_WRITE_FE_HW_VERSION 0x83 //
  #define ELETTRONICSERVICES_WRITE_FE_RTC 0x85 //
  #define ELETTRONICSERVICES_WRITE_SNR_THRESHOLD 0x8C //
  #define ELETTRONICSERVICES_WRITE_FORMAT_FENV 0xFE //

  // CMD_DATA_LENGTH
  #define ELETTRONICSERVICES_SIGNAL_AMPLITUDE_LENGTH 96 //
  #define ELETTRONICSERVICES_VIBRATION_AMPLITUDE_LENGTH 96 //
  #define ELETTRONICSERVICES_FE_RTC_LENGTH 5 //
  #define ELETTRONICSERVICES_FE_HD_VERSION_LENGTH 3 //
  #define ELETTRONICSERVICES_FE_VERSION_LENGTH 6 //
  #define ELETTRONICSERVICES_AUTOZERO_STAUS_LENGTH 3 //
  #define ELETTRONICSERVICES_SNR_THRESHOLD_LENGTH 2 //
  #define ELETTRONICSERVICES_DEFUALT_LENGTH 1 //

  // _AUTOZERO_STATUS
  #define ELETTRONICSERVICES_STATUS_COMPLETED 0 //
  #define ELETTRONICSERVICES_STATUS_UNCOMPLETED 1 //
  #define ELETTRONICSERVICES_STATUS_FAIL 2 //
  #define ELETTRONICSERVICES_STATUS_NOT_START 3 //

  // TABLE_LCD_LIGHT_MODE
  #define ELETTRONICSERVICES_LIGHT_OFF 0 //HMI LCD light turn off
  #define ELETTRONICSERVICES_LIGHT_ON 1 //HMI LCD light turn on
  #define ELETTRONICSERVICES_LIGHT_DYNAMIC 2 //HMI LCD light auto adjust

  // TABLE_BLE_SWITCH_2
  #define ELETTRONICSERVICES_BLE_TOOTH_OFF 0 //HMI bluetooth off
  #define ELETTRONICSERVICES_BLE_TOOTH_ON 5 //HMI bluetooth on
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_ELETTRONIC_SERVICES_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    ELETTRONICSERVICES_DC_IDX_ReplaceStaticFrequentNoDefault,
    ELETTRONICSERVICES_DC_IDX_Unprotected,
    ELETTRONICSERVICES_DC_IDX_DynamicDuplicated,
    ELETTRONICSERVICES_DC_IDX_ReplaceStaticRare,
    ELETTRONICSERVICES_DC_IDX_Protected,
    ELETTRONICSERVICES_DC_IDX_Configure,
    ELETTRONICSERVICES_DC_IDX_CONSTANT,
    ELETTRONICSERVICES_DC_IDX_STATIC_RARE,
    ELETTRONICSERVICES_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT,
    ELETTRONICSERVICES_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_ELETTRONIC_SERVICES
typedef struct _T_ELETTRONIC_SERVICES
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* ElettornicServicesEXE)(void);
    void (* CyclicAsyncSaveSRV)(void);
    void (* UpdateDiagnosis)(void);
    void (* ExitSRV)(void);
    void (* InizializeNvSRV)(void);
    TUSIGN8 (* InitializeNvCheckSRV)(void);
    void (* ResetToDefaultSRV)(void);
    void (* LoadRomDefaultSRV)(void);
    void (* DeviceResetSRV)(void);
    void (* CheckCpuAsm)(void);
    void (* CheckDataCrc)(void);
    void (* CheckAutoZeroStatus)(void);
    TBOOL (* CheckDecreasingVoltage)(void);
    //@SubGen end@    <METHODS>
} T_ELETTRONIC_SERVICES;


//-------------------------------------------------------------------------------------------------
//! define the T_ELETTRONIC_SERVICES-object
extern SLOW const T_ELETTRONIC_SERVICES elettronicServices;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_ELETTRONIC_SERVICES
#define ELETTRONIC_SERVICES_ME (&elettronicServices.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define elettronicServices_Get(objectIndex,attributeIndex,ptrValue) \
   elettronicServices.unit.Get(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue)
#define elettronicServices_Put(objectIndex,attributeIndex,ptrValue) \
   elettronicServices.unit.Put(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue)
#define elettronicServices_Check(objectIndex,attributeIndex,ptrValue) \
   elettronicServices.unit.Check(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue)

#define elettronicServices_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   elettronicServices.unit.GetAttributeDescription(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrDescriptor)
#define elettronicServices_GetObjectDescription(objectIndex,ptrDescriptor) \
   elettronicServices.unit.GetObjectDescription(ELETTRONIC_SERVICES_ME,objectIndex,ptrDescriptor)
