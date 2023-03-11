//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ElettronicServices_types.h
#endif

//............................................
//!  extend include list if neccessary
#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"


#ifdef __ELETTRONIC_SERVICES_TYPES_H__
  #error ElettronicServices_types.h included more than once
#endif

#define __ELETTRONIC_SERVICES_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of Configure block
 typedef struct _T_ELETTRONIC_SERVICES_CONFIGURE
 {
     //@SubGen start@    <DATACLASS Configure> 
    TUSIGN8 switchBLE;
    TUSIGN8 modeLCDLight;
    TUSIGN16 configDataCheck;
     //@SubGen end@    <DATACLASS Configure> 
 } T_ELETTRONIC_SERVICES_CONFIGURE;



 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_ELETTRONIC_SERVICES_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TUSIGN8 softwareRevision[3];
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_ELETTRONIC_SERVICES_CONSTANT;



 //-------------------------------------------------------------------------------------------------
 //! structure of Protected block
 typedef struct _T_ELETTRONIC_SERVICES_PROTECTED
 {
     //@SubGen start@    <DATACLASS Protected> 
    TUSIGN8 electronicServicesAlarmSimulation;
    TUSIGN8 initializeNv;
    TINT16 resetToDefault;
    TINT16 saveAsDefault;
    TINT16 loadRomDefault;
    TFLOAT elettronicTemperatureSimulation;
    TUSIGN8 protectedDataStatus;
     //@SubGen end@    <DATACLASS Protected> 
 } T_ELETTRONIC_SERVICES_PROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of ReplaceStaticRare block
 typedef struct _T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS ReplaceStaticRare> 
    TUSIGN8 deviceSerialNumber[8];
    TUSIGN8 feSerialNumber[3];
    TCHAR sensorID[8];
    TCHAR sensorSapErpNo[20];
    TCHAR sensorCalDate[12];
    TCHAR sensorCalCertNo[20];
    TCHAR sensorCalLocation[32];
    TCHAR tranSapErpNo[20];
     //@SubGen end@    <DATACLASS ReplaceStaticRare> 
 } T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE;



 //-------------------------------------------------------------------------------------------------
 //! structure of DynamicDuplicated block
 typedef struct _T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED
 {
     //@SubGen start@    <DATACLASS DynamicDuplicated>
    TUSIGN16 vSupplyReadBack;
    TUSIGN8 localDiagnosis;
    TFLOAT elettronicTemperature;
    TUSIGN16 vSupplyReadBackDuplicated;
    TUSIGN8 localDiagnosisDuplicated;
    TFLOAT elettronicTemperatureDuplicated;
     //@SubGen end@    <DATACLASS DynamicDuplicated>
 } T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED;


 //-------------------------------------------------------------------------------------------------
 //! structure of Unprotected block
 typedef struct _T_ELETTRONIC_SERVICES_UNPROTECTED
 {
     //@SubGen start@    <DATACLASS Unprotected> 
    TUSIGN8 hardwareRevision[3];
    TUSIGN16 dipSwitchAndPushButtonsStatus;
    TCHAR swRevisionHmi_transmitter[8];
    TCHAR hwRevisionHmi_transmitter[8];
    TUSIGN16 protectedDataCrc;
    TINT16 classLoadRomDefault;
    TCHAR lowLevelHmiHwSwRevision[8];
    TUSIGN8 cbNvWriteIsActive;
    TUSIGN8 feNvWriteIsActive;
    TUSIGN8 bootloaderRevision[8];
    TUSIGN8 frontendRtc[5];
    TUSIGN8 autozeroStatus;
    TINT16 snrThreshold;
    TUSIGN8 softwareMainRevision;
    TUSIGN8 hardwareMainRevision;
    TCHAR swRevisionHmi_fe[8];
    TCHAR hwRevisionHmi_fe[8];
    TUSIGN8 tranSapErpNoChangeFlag;
    TFLOAT acttelettronicTemperature;
     //@SubGen end@    <DATACLASS Unprotected> 
 } T_ELETTRONIC_SERVICES_UNPROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 hardwareCalibrationLoad;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT;







 //-------------------------------------------------------------------------------------------------
 //! structure of ReplaceStaticFrequentNoDefault block
 typedef struct _T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT
 {
     //@SubGen start@    <DATACLASS ReplaceStaticFrequentNoDefault> 
    TUSIGN8 swWriteProtection;
    TUSIGN8 vSupplyReadbackEnable;
    TUSIGN16 vSupplyReadbackHighThreshold;
    TUSIGN16 vSupplyReadbackLowThreshold;
    TUSIGN8 vSupplyReadbackRetry;
    TUSIGN8 ramTestEnable;
    TUSIGN8 romTestEnable;
    TUSIGN8 dynamicVariablesCheckEnable;
     //@SubGen end@    <DATACLASS ReplaceStaticFrequentNoDefault> 
 } T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_RARE block
 typedef struct _T_ELETTRONIC_SERVICES_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS STATIC_RARE> 
    TUSIGN8 cbSerialNumber[3];
    TUSIGN8 T_UnitCode;
    T_RANGE_FLT alarmTRng;
    TCHAR tranType[12];
    TCHAR tranID[8];
    TCHAR tranCalDate[12];
    TCHAR tranCalCertNo[20];
    TCHAR tranCalLocation[32];
    TCHAR manufacturer[20];
    TCHAR street[20];
    TCHAR city[20];
    TCHAR phone[20];
    TUSIGN32 autoSelfCheckTime;
    TCHAR sensorLocationTag[20];
    TCHAR sensoTag[20];
    TFLOAT ambientTemperatureMax;
     //@SubGen end@    <DATACLASS STATIC_RARE> 
 } T_ELETTRONIC_SERVICES_STATIC_RARE;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_ELETTRONIC_SERVICES_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_ELETTRONIC_SERVICES_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

