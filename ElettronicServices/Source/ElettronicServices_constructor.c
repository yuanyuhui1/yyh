//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <float.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_int16.h" 
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_const.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
//#include "..\..\t_data_obj\interface\t_data_obj_tabenum16.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "ElettronicServices/Source/ElettronicServices_types.h"
#include "ElettronicServices/Source/ElettronicServices_attributes.h"
#include "ElettronicServices/Source/ElettronicServices_overload.h"
#include "ElettronicServices/Source/ElettronicServices_execute.h"
#include "ElettronicServices/Source/ElettronicServices_action_fct.h"
#include "ElettronicServices/Source/versioncode.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uint32.h"

#include "Coordinator/Interface/Coordinator_idx.h"

extern const SLOW T_DATA_OBJ elettronicServicesObjectList[];
// Forward declaration ObjectList
extern const SLOW T_DATA_OBJ coordinatorObjectList[];

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 elettronicServicesState;     // Subsystem state
FAST T_DATA_SEMAPHORE elettronicServicesDataSemaphore;



const SLOW T_DATACLASS elettronicServicesDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(elettronicServicesReplaceStaticFrequentNoDefault,elettronicServicesReplaceStaticFrequentNoDefaultInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(elettronicServicesUnprotected,elettronicServicesUnprotectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(elettronicServicesDynamicDuplicated,elettronicServicesDynamicDuplicatedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(elettronicServicesReplaceStaticRare,elettronicServicesReplaceStaticRareInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(elettronicServicesProtected,elettronicServicesProtectedInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(elettronicServicesConfigure,elettronicServicesConfigureInitializer,elettronicServicesConfigureDefault),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(elettronicServicesConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(elettronicServicesStaticRare,elettronicServicesStaticRareInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(elettronicServicesStaticFrequentStaticDefault,elettronicServicesStaticFrequentStaticDefaultInitializer,elettronicServicesStaticFrequentStaticDefaultDefault)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Configure data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigure;
    const FAST T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_CONFIGURE"
    const FAST __no_init T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigure;
  #pragma location="ELETTRONIC_SERVICES_CONFIGURE_DEFAULT"
    const FAST __no_init T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureDefault;
#endif
const SLOW T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureInitializer=
{
    //@SubGen start@    <INITIALIZER CONFIGURE>
    ELETTRONICSERVICES_BLE_TOOTH_ON,    // TABENUM8 switchBLE
    ELETTRONICSERVICES_LIGHT_DYNAMIC,    // TABENUM8 modeLCDLight
    ELETTRONICSERVICES_ROM_CHECK_VALUE_CONFIG    // SIMPLE_U16 configDataCheck
    //@SubGen end@    <INITIALIZER CONFIGURE>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_ELETTRONIC_SERVICES_CONSTANT elettronicServicesConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    {SW_REV0,SW_REV1,SW_REV2}    // CONST_U8 softwareRevision[3]
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ELETTRONIC_SERVICES_PROTECTED elettronicServicesProtected;
const SLOW T_ELETTRONIC_SERVICES_PROTECTED elettronicServicesProtectedInitializer=
{
    //@SubGen start@    <INITIALIZER PROTECTED>
    0,    // T_DIAGNOSIS_ALARM electronicServicesAlarmSimulation
    ELETTRONICSERVICES_NV_NOT_INITIALIZE,    // TABENUM8 initializeNv
    0,    // SIMPLE_I16 resetToDefault
    0,    // SIMPLE_I16 saveAsDefault
    0,    // SIMPLE_I16 loadRomDefault
    0.0f,    // FLOAT elettronicTemperatureSimulation
    ELETTRONICSERVICES_UNPROTECTED    // TABENUM8 protectedDataStatus
    //@SubGen end@    <INITIALIZER PROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRare;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_REPLACE_STATIC_RARE"
    const FAST __no_init T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRare;
#endif
const SLOW T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_RARE>
    {51,75,64,0,0,0,0,0},    // SIMPLE_U8 deviceSerialNumber[8]
    {'A','B','B'},    // SIMPLE_U8 feSerialNumber[3]
    {'A','B','B',' ',' ',' ',' ',' '},    // SIMPLE_CHAR sensorID[8]
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},    // SIMPLE_CHAR sensorSapErpNo[20]
    {'2','0','1','2','-','0','1','-','0','1','0','0'},    // SIMPLE_CHAR sensorCalDate[12]
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR sensorCalCertNo[20]
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR sensorCalLocation[32]
    {'0','0','0','0','0','0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}    // SIMPLE_CHAR tranSapErpNo[20]
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ELETTRONIC_SERVICES_UNPROTECTED elettronicServicesUnprotected;
const SLOW T_ELETTRONIC_SERVICES_UNPROTECTED elettronicServicesUnprotectedInitializer=
{
    //@SubGen start@    <INITIALIZER UNPROTECTED>
    {1,0,0},    // SIMPLE_U8 hardwareRevision[3]
    0,    // SIMPLE_U16 dipSwitchAndPushButtonsStatus
    {'0','0','.','0','0','.','0','0'},    // SIMPLE_CHAR swRevisionHmi_transmitter[8]
    {'0','0','.','0','0','.','0','0'},    // SIMPLE_CHAR hwRevisionHmi_transmitter[8]
    0,    // SIMPLE_U16 protectedDataCrc
    0,    // SIMPLE_I16 classLoadRomDefault
    {32,32,32,32,32,32,32,32},    // SIMPLE_CHAR lowLevelHmiHwSwRevision[8]
    ELETTRONICSERVICES_WRITE_NOT_ACTIVE,    // TABENUM8 cbNvWriteIsActive
    ELETTRONICSERVICES_WRITE_NOT_ACTIVE,    // TABENUM8 feNvWriteIsActive
    {'0','0','.','0','0','.','0','4'},    // SIMPLE_U8 bootloaderRevision[8]
    {0,0,0,0,0},    // SIMPLE_U8 frontendRtc[5]
    ELETTRONICSERVICES_STATUS_NOT_START,    // TABENUM8 autozeroStatus
    -50,    // INT16 snrThreshold
    0,    // SIMPLE_U8 softwareMainRevision
    0,    // SIMPLE_U8 hardwareMainRevision
    {'0','0','.','0','0','.','0','0'},    // SIMPLE_CHAR swRevisionHmi_fe[8]
    {'0','0','.','0','0','.','0','0'},    // SIMPLE_CHAR hwRevisionHmi_fe[8]
    0,    // SIMPLE_U8 tranSapErpNoChangeFlag
    0.0f    // SIMPLE_FLOAT acttelettronicTemperature
    //@SubGen end@    <INITIALIZER UNPROTECTED>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED elettronicServicesDynamicDuplicated;
const SLOW T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED elettronicServicesDynamicDuplicatedInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC_DUPLICATED>
    0,    // SIMPLE_U16 vSupplyReadBack
    0,    // SIMPLE_U8 localDiagnosis
    0,    // SIMPLE_FLOAT elettronicTemperature
    0,    // SIMPLE_U16 vSupplyReadBackDuplicated
    0,    // SIMPLE_U8 localDiagnosisDuplicated
    0    // SIMPLE_FLOAT elettronicTemperatureDuplicated
    //@SubGen end@    <INITIALIZER DYNAMIC_DUPLICATED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefault;
    const FAST T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefault;
  #pragma location="ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultDefault;
#endif
    
 //lint -e{736} hao wu,  2013-01-15 loss of precision is Ok   
const SLOW T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultInitializer=
{ 
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
    ELETTRONICSERVICES_CALIBRATION_LOAD    // SIMPLE_U8 hardwareCalibrationLoad
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticFrequentNoDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT"
    const FAST __no_init T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefault;
#endif
const SLOW T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_FREQUENT_NO_DEFAULT>
    ELETTRONICSERVICES_WRITE_PROTECTION_OFF,    // TABENUM8 swWriteProtection
    ELETTRONICSERVICES_SIL_TEST_ON,    // TABENUM8 vSupplyReadbackEnable
    ELETTRONICSERVICES_V_SUPPLY_READBACK_HIGH_THRESHOLD,    // SIMPLE_U16 vSupplyReadbackHighThreshold
    ELETTRONICSERVICES_V_SUPPLY_READBACK_LOW_THRESHOLD,    // SIMPLE_U16 vSupplyReadbackLowThreshold
    ELETTRONICSERVICES_V_SUPPLY_READBACK_RETRY,    // SIMPLE_U8 vSupplyReadbackRetry
    ELETTRONICSERVICES_SIL_TEST_ON,    // TABENUM8 ramTestEnable
    ELETTRONICSERVICES_SIL_TEST_ON,    // TABENUM8 romTestEnable
    ELETTRONICSERVICES_SIL_TEST_ON    // TABENUM8 dynamicVariablesCheckEnable
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_FREQUENT_NO_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRare;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_STATIC_RARE"
    const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRare;
#endif
const SLOW T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_RARE>
    {0,0,0},    // SIMPLE_U8 cbSerialNumber[3]
    ELETTRONICSERVICES_CELSIUS,    // TABENUM8 T_UnitCode
    {85,-40},    // RANGEFLT alarmTRng {hiLim,loLim}
    {'0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR tranType[12]
    {'0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR tranID[8]
    {'2','0','1','2','-','0','1','-','0','1','0','0'},    // SIMPLE_CHAR tranCalDate[12]
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR tranCalCertNo[20]
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR tranCalLocation[32]
    {'A','B','B',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},    // SIMPLE_CHAR manufacturer[20]
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},    // SIMPLE_CHAR street[20]
    {'S','h','a','n','g','H','a','i',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},    // SIMPLE_CHAR city[20]
    {'+','8','6',' ','2','1',' ','8','8','8','8','8','8','8','8',' ',' ',' ',' ',' '},    // SIMPLE_CHAR phone[20]
    525600,    // USIGN32 autoSelfCheckTime
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR sensorLocationTag[20]
    {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},    // SIMPLE_CHAR sensoTag[20]
    -40    // FLOAT ambientTemperatureMax
    //@SubGen end@    <INITIALIZER STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 SwWriteProtectionTable[] =
// Table referenced by: swWriteProtection
{
  ELETTRONICSERVICES_WRITE_PROTECTION_OFF,
  ELETTRONICSERVICES_WRITE_PROTECTION_ON
};

const SLOW TUSIGN8 ElectronicServicesDiagnosisEnumTable[] =
// Table referenced by: electronicServicesAlarmSimulation
{
  ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW,
  ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH,
  ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH,
  ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW
};

const SLOW TUSIGN8 InitializeNvTable[] =
// Table referenced by: initializeNv
{
  ELETTRONICSERVICES_NV_NOT_INITIALIZE,
  ELETTRONICSERVICES_NV_INITIALIZE_START,
  ELETTRONICSERVICES_NV_INITIALIZE_END_OK,
  ELETTRONICSERVICES_NV_INITIALIZE_END_FAILED
};

const SLOW TUSIGN8 SilTestTable[] =
// Table referenced by: ramTestEnable, romTestEnable, vSupplyReadbackEnable, dynamicVariablesCheckEnable
{
  ELETTRONICSERVICES_SIL_TEST_OFF,
  ELETTRONICSERVICES_SIL_TEST_ON
};

const SLOW TUSIGN8 NvWriteIsActiveTable[] =
// Table referenced by: cbNvWriteIsActive, feNvWriteIsActive
{
  ELETTRONICSERVICES_WRITE_NOT_ACTIVE,
  ELETTRONICSERVICES_WRITE_ACTIVE
};

const SLOW TUSIGN8 TEMPERATURE[] =
// Table referenced by: T_UnitCode
{
  ELETTRONICSERVICES_KELVIN,
  ELETTRONICSERVICES_CELSIUS,
  ELETTRONICSERVICES_FAHRENHEIT
};

const SLOW TUSIGN8 PROTECT_TABLE[] =
// Table referenced by: protectedDataStatus
{
  ELETTRONICSERVICES_UNPROTECTED,
  ELETTRONICSERVICES_CALCULATE,
  ELETTRONICSERVICES_PROTECTED
};

const SLOW TUSIGN8 _AUTOZERO_STATUS[] =
// Table referenced by: autozeroStatus
{
  ELETTRONICSERVICES_STATUS_COMPLETED,
  ELETTRONICSERVICES_STATUS_UNCOMPLETED,
  ELETTRONICSERVICES_STATUS_FAIL,
  ELETTRONICSERVICES_STATUS_NOT_START
};

const SLOW TUSIGN8 TABLE_LCD_LIGHT_MODE[] =
// Table referenced by: modeLCDLight
{
  ELETTRONICSERVICES_LIGHT_OFF,
  ELETTRONICSERVICES_LIGHT_ON,
  ELETTRONICSERVICES_LIGHT_DYNAMIC
};

const SLOW TUSIGN8 TABLE_BLE_SWITCH_2[] =
// Table referenced by: switchBLE
{
  ELETTRONICSERVICES_BLE_TOOTH_OFF,
  ELETTRONICSERVICES_BLE_TOOTH_ON
};

static const SLOW T_RANGE_E8 swWriteProtectionDescriptor =
{
  SwWriteProtectionTable,  // TUSIGN8* ptrCodeTab
  sizeof(SwWriteProtectionTable)  // TUSIGN8 numberOfCodes
};

const SLOW T_RANGE_E8 electronicServicesAlarmSimulationDescriptor =
{
  ElectronicServicesDiagnosisEnumTable,  // TUSIGN8* ptrCodeTab
  ELETTRONICSERVICES_ALARM_CODES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 initializeNvDescriptor =
{
  InitializeNvTable,  // TUSIGN8* ptrCodeTab
  sizeof(InitializeNvTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: ramTestEnable, romTestEnable, vSupplyReadbackEnable, dynamicVariablesCheckEnable
{
  SilTestTable,  // TUSIGN8* ptrCodeTab
  sizeof(SilTestTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor1 =
// Descriptor referenced by: cbNvWriteIsActive, feNvWriteIsActive
{
  NvWriteIsActiveTable,  // TUSIGN8* ptrCodeTab
  sizeof(NvWriteIsActiveTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 autozeroStatusDescriptor =
{
  _AUTOZERO_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_AUTOZERO_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U32 autoSelfCheckTimeDescriptor =
{
  2,  // TUSIGN32 min
  525600,  // TUSIGN32 max
  1  // TUSIGN32 increment
};

static const SLOW T_UOM_PF_DESCRIPTOR elettronicTemperature_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  &elettronicServicesObjectList[ELETTRONICSERVICES_IDX_elettronicTemperature],  // void * pObject_value
  ELETTRONICSERVICES_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGEFLT_DESCRIPTOR alarmTRngDescriptor =
{
  500,  // TFLOAT maxHi
  -200  // TFLOAT maxLo
};

static const SLOW T_UOM_PF_DESCRIPTOR alarmTRng_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  &elettronicServicesObjectList[ELETTRONICSERVICES_IDX_alarmTRng],  // void * pObject_value
  ELETTRONICSERVICES_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 protectedDataStatusDescriptor =
{
  PROTECT_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(PROTECT_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR elettronicTemperatureSimulation_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(simTemperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  &elettronicServicesObjectList[ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation],  // void * pObject_value
  ELETTRONICSERVICES_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_I16 snrThresholdDescriptor =
{
  -80,  // TUSIGN16 min
  50,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_E8 T_UnitCodeDescriptor =
{
  TEMPERATURE,  // TUSIGN8* ptrCodeTab
  sizeof(TEMPERATURE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR ambientTemperatureMax_objDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  &elettronicServicesObjectList[ELETTRONICSERVICES_IDX_ambientTemperatureMax],  // void * pObject_value
  ELETTRONICSERVICES_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT ambientTemperatureMaxDescriptor =
{
  -80.0,  // TFLOAT min
  170.0,  // TFLOAT max
  1  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT elettronicTemperatureSimulationDescriptor =
{
  -60.0f,  // TFLOAT min
  210.0f,  // TFLOAT max
  1  // TFLOAT increment
};

static const SLOW T_UOM_PF_DESCRIPTOR acttelettronicTemperature_objDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  &elettronicServicesObjectList[ELETTRONICSERVICES_IDX_acttelettronicTemperature],  // void * pObject_value
  ELETTRONICSERVICES_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 switchBLEDescriptor =
{
  TABLE_BLE_SWITCH_2,  // TUSIGN8* ptrCodeTab
  sizeof(TABLE_BLE_SWITCH_2)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 modeLCDLightDescriptor =
{
  TABLE_LCD_LIGHT_MODE,  // TUSIGN8* ptrCodeTab
  sizeof(TABLE_LCD_LIGHT_MODE)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ elettronicServicesObjectList[] =
{
    //lint -e923 
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //elettronicTemperature_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &elettronicTemperature_ObjDescriptor),

    //elettronicTemperatureSimulation_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &elettronicTemperatureSimulation_ObjDescriptor),

    //softwareRevision
    CONSTRUCT_ARRAY_CONST_U8(
        elettronicServicesConstant.softwareRevision,
        3),

    //hardwareRevision
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesUnprotected.hardwareRevision,
        3,
        RAM),

    //deviceSerialNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesReplaceStaticRare.deviceSerialNumber,
        8,
        NON_VOLATILE),

    //feSerialNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesReplaceStaticRare.feSerialNumber,
        3,
        NON_VOLATILE),

    //vSupplyReadBack
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesDynamicDuplicated.vSupplyReadBack,
        RAM),

    //dipSwitchAndPushButtonsStatus
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesUnprotected.dipSwitchAndPushButtonsStatus,
        READONLY_RAM),

    //swWriteProtection
    CONSTRUCT_TABENUM8(
        &elettronicServicesReplaceStaticFrequentNoDefault.swWriteProtection,
        NON_VOLATILE,
        &swWriteProtectionDescriptor),

    //swRevisionHmi_transmitter
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesUnprotected.swRevisionHmi_transmitter,
        8,
        RAM),

    //hwRevisionHmi_transmitter
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesUnprotected.hwRevisionHmi_transmitter,
        8,
        RAM),

    //deviceReset
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        deviceReset_ELETTRONICSERVICES),

    //electronicServicesAlarmSimulation
    CONSTRUCT_ALARM(
        &elettronicServicesProtected.electronicServicesAlarmSimulation,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &electronicServicesAlarmSimulationDescriptor),

    //localDiagnosis
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesDynamicDuplicated.localDiagnosis,
        RAM),

    //cbSerialNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesStaticRare.cbSerialNumber,
        3,
        NON_VOLATILE),

    //resetOutputTrimming
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetOutputTrimming_ELETTRONICSERVICES),

    //elettronicTemperature
    CONSTRUCT_SIMPLE_FLOAT(
        &elettronicServicesDynamicDuplicated.elettronicTemperature,
        RAM),

    //vSupplyReadBackDuplicated
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesDynamicDuplicated.vSupplyReadBackDuplicated,
        RAM),

    //protectedDataCrc
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesUnprotected.protectedDataCrc,
        RAM),

    //localDiagnosisDuplicated
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesDynamicDuplicated.localDiagnosisDuplicated,
        RAM),

    //initializeNv
    CONSTRUCT_TABENUM8(
        &elettronicServicesProtected.initializeNv,
        RAM,
        &initializeNvDescriptor),

    //resetToDefault
    CONSTRUCT_SIMPLE_I16(
        &elettronicServicesProtected.resetToDefault,
        RAM),

    //saveAsDefault
    CONSTRUCT_SIMPLE_I16(
        &elettronicServicesProtected.saveAsDefault,
        RAM),

    //loadRomDefault
    CONSTRUCT_SIMPLE_I16(
        &elettronicServicesProtected.loadRomDefault,
        RAM),

    //classLoadRomDefault
    CONSTRUCT_SIMPLE_I16(
        &elettronicServicesUnprotected.classLoadRomDefault,
        RAM),

    //lowLevelHmiHwSwRevision
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesUnprotected.lowLevelHmiHwSwRevision,
        8,
        RAM),

    //vSupplyReadbackEnable
    CONSTRUCT_TABENUM8(
        &elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackEnable,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //vSupplyReadbackHighThreshold
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackHighThreshold,
        NON_VOLATILE),

    //vSupplyReadbackLowThreshold
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackLowThreshold,
        NON_VOLATILE),

    //vSupplyReadbackRetry
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackRetry,
        NON_VOLATILE),

    //ramTestEnable
    CONSTRUCT_TABENUM8(
        &elettronicServicesReplaceStaticFrequentNoDefault.ramTestEnable,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //romTestEnable
    CONSTRUCT_TABENUM8(
        &elettronicServicesReplaceStaticFrequentNoDefault.romTestEnable,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //cbNvWriteIsActive
    CONSTRUCT_TABENUM8(
        &elettronicServicesUnprotected.cbNvWriteIsActive,
        READONLY_RAM,
        &sharedTRangeE8Descriptor1),

    //feNvWriteIsActive
    CONSTRUCT_TABENUM8(
        &elettronicServicesUnprotected.feNvWriteIsActive,
        READONLY_RAM,
        &sharedTRangeE8Descriptor1),

    //dynamicVariablesCheckEnable
    CONSTRUCT_TABENUM8(
        &elettronicServicesReplaceStaticFrequentNoDefault.dynamicVariablesCheckEnable,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //T_UnitCode
    CONSTRUCT_TABENUM8(
        &elettronicServicesStaticRare.T_UnitCode,
        NON_VOLATILE,
        &T_UnitCodeDescriptor),

    //elettronicTemperatureDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &elettronicServicesDynamicDuplicated.elettronicTemperatureDuplicated,
        RAM),

    //elettronicTemperatureSimulation
    CONSTRUCT_FLOAT(
        &elettronicServicesProtected.elettronicTemperatureSimulation,
        RAM,
        &elettronicTemperatureSimulationDescriptor),

    //alarmTRng
    CONSTRUCT_STRUCT_RNGFLT(
        &elettronicServicesStaticRare.alarmTRng,
        NON_VOLATILE,
        &alarmTRngDescriptor),

    //alarmTRng_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &alarmTRng_ObjDescriptor),

    //protectedDataStatus
    CONSTRUCT_TABENUM8(
        &elettronicServicesProtected.protectedDataStatus,
        RAM,
        &protectedDataStatusDescriptor),

    //hardwareCalibrationLoad
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesStaticFrequentStaticDefault.hardwareCalibrationLoad,
        NON_VOLATILE),

    //sensorID
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.sensorID,
        8,
        NON_VOLATILE),

    //sensorSapErpNo
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.sensorSapErpNo,
        20,
        NON_VOLATILE),

    //sensorCalDate
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.sensorCalDate,
        12,
        NON_VOLATILE),

    //sensorCalCertNo
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.sensorCalCertNo,
        20,
        NON_VOLATILE),

    //sensorCalLocation
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.sensorCalLocation,
        32,
        NON_VOLATILE),

    //bootloaderRevision
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesUnprotected.bootloaderRevision,
        8,
        RAM),

    //tranType
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.tranType,
        12,
        NON_VOLATILE),

    //tranID
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.tranID,
        8,
        NON_VOLATILE),

    //tranSapErpNo
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesReplaceStaticRare.tranSapErpNo,
        20,
        NON_VOLATILE),

    //tranCalDate
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.tranCalDate,
        12,
        NON_VOLATILE),

    //tranCalCertNo
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.tranCalCertNo,
        20,
        NON_VOLATILE),

    //tranCalLocation
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.tranCalLocation,
        32,
        NON_VOLATILE),

    //manufacturer
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.manufacturer,
        20,
        NON_VOLATILE),

    //street
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.street,
        20,
        NON_VOLATILE),

    //city
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.city,
        20,
        NON_VOLATILE),

    //phone
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.phone,
        20,
        NON_VOLATILE),

    //writeAutoZero
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        writeAutoZero_ELETTRONICSERVICES),

    //writeSelfDiagnosis
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        writeSelfDiagnosis_ELETTRONICSERVICES),

    //resetCB_Runhour
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetCB_Runhour_ELETTRONICSERVICES),

    //frontendRtc
    CONSTRUCT_ARRAY_SIMPLE_U8(
        elettronicServicesUnprotected.frontendRtc,
        5,
        RAM),

    //autozeroStatus
    CONSTRUCT_TABENUM8(
        &elettronicServicesUnprotected.autozeroStatus,
        RAM,
        &autozeroStatusDescriptor),

    //autoSelfCheckTime
    CONSTRUCT_UINT32(
        &elettronicServicesStaticRare.autoSelfCheckTime,
        NON_VOLATILE,
        &autoSelfCheckTimeDescriptor),

    //snrThreshold
    CONSTRUCT_INT16(
        &elettronicServicesUnprotected.snrThreshold,
        RAM,
        &snrThresholdDescriptor),

    //sensorLocationTag
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.sensorLocationTag,
        20,
        NON_VOLATILE),

    //sensoTag
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesStaticRare.sensoTag,
        20,
        NON_VOLATILE),

    //softwareMainRevision
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesUnprotected.softwareMainRevision,
        RAM),

    //hardwareMainRevision
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesUnprotected.hardwareMainRevision,
        RAM),

    //swRevisionHmi_fe
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesUnprotected.swRevisionHmi_fe,
        8,
        RAM),

    //hwRevisionHmi_fe
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        elettronicServicesUnprotected.hwRevisionHmi_fe,
        8,
        RAM),

    //tranSapErpNoChangeFlag
    CONSTRUCT_SIMPLE_U8(
        &elettronicServicesUnprotected.tranSapErpNoChangeFlag,
        RAM),

    //formatFENV
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        formatFENV_ELETTRONICSERVICES),

    //ambientTemperatureMax
    CONSTRUCT_FLOAT(
        &elettronicServicesStaticRare.ambientTemperatureMax,
        NON_VOLATILE,
        &ambientTemperatureMaxDescriptor),

    //ambientTemperatureMax_obj
    CONSTRUCT_STRUCT_UOM_PF(
        &ambientTemperatureMax_objDescriptor),

    //saveAllAsDefault
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        saveAllAsDefault_ELETTRONICSERVICES),

    //resetAllToDefault
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetAllToDefault_ELETTRONICSERVICES),

    //acttelettronicTemperature
    CONSTRUCT_SIMPLE_FLOAT(
        &elettronicServicesUnprotected.acttelettronicTemperature,
        RAM),

    //acttelettronicTemperature_obj
    CONSTRUCT_STRUCT_UOM_PF(
        &acttelettronicTemperature_objDescriptor),

    //switchBLE
    CONSTRUCT_TABENUM8(
        &elettronicServicesConfigure.switchBLE,
        NON_VOLATILE,
        &switchBLEDescriptor),

    //modeLCDLight
    CONSTRUCT_TABENUM8(
        &elettronicServicesConfigure.modeLCDLight,
        NON_VOLATILE,
        &modeLCDLightDescriptor),

    //configDataCheck
    CONSTRUCT_SIMPLE_U16(
        &elettronicServicesConfigure.configDataCheck,
        NON_VOLATILE)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the ELETTRONIC_SERVICES-Class object
SLOW const T_ELETTRONIC_SERVICES elettronicServices =
{
    {
        &elettronicServicesState,
        &elettronicServicesDataSemaphore,

        elettronicServicesObjectList,
        //lint -e{778}
        (sizeof(elettronicServicesObjectList)/sizeof(elettronicServicesObjectList[0]))-1,

        elettronicServicesDataClassList,
        sizeof(elettronicServicesDataClassList)/sizeof(elettronicServicesDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_ELETTRONIC_SERVICES, // will be overloaded
        LoadRomDefaults_ELETTRONIC_SERVICES,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_ELETTRONIC_SERVICES,
        Put_ELETTRONIC_SERVICES,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,ElettornicServicesEXE_ELETTRONICSERVICES
    ,CyclicAsyncSaveSRV_ELETTRONICSERVICES
    ,UpdateDiagnosis_ELETTRONICSERVICES
    ,ExitSRV_ELETTRONICSERVICES
    ,InizializeNvSRV_ELETTRONICSERVICES
    ,InitializeNvCheckSRV_ELETTRONICSERVICES
    ,ResetToDefaultSRV_ELETTRONICSERVICES
    ,LoadRomDefaultSRV_ELETTRONICSERVICES
    ,DeviceResetSRV_ELETTRONICSERVICES
    ,CheckCpuAsm_ELETTRONICSERVICES
    ,CheckDataCrc_ELETTRONICSERVICES
    ,CheckAutoZeroStatus_ELETTRONICSERVICES
    ,CheckDecreasingVoltage_ELETTRONICSERVICES
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: elettronicServices_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
    const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE"
    const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE_DEFAULT"
    const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateDefault;
#endif
const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE"
    const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
#endif
const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
elettronicServicesTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(elettronicServicesTemplate,elettronicServicesTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(elettronicServicesTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(elettronicServicesTemplate,elettronicServicesTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(elettronicServicesTemplate,elettronicServicesTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(elettronicServicesTemplate,elettronicServicesTemplateInitializer,elettronicServicesTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

