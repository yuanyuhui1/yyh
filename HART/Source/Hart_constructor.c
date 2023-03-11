//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
// include the data_obj-classes you need, e.g. T_DATA_OBJ::INT16
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_const.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"

#include "t_data_obj_hart_service_code.h"

#include "t_data_obj_addr.h"
#include "t_data_obj_conf.h"
#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "HART/Interface/Hart.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "hart_types.h"
#include "hart_attributes.h"
#include "hart_overload.h"
#include "hart_execute.h"
#include "hart_action_fct.h"
#include "HART/Layer7/Interface/layer7.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 hartState;     // Subsystem state
FAST T_DATA_SEMAPHORE hartDataSemaphore;



const SLOW T_DATACLASS hartDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(hartDyn,hartDynInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hartRare,hartRareInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(hartFreqCst,hartFreqCstInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hartFreqNo,hartFreqNoInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(hartCst),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(hartFreqStatic,hartFreqStaticInitializer,hartFreqStaticDefault),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(hartConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(hartDynamic,hartDynamicInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hartStaticRare,hartStaticRareInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of FreqStatic data
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_FREQ_STATIC hartFreqStatic;
    const FAST T_HART_FREQ_STATIC hartFreqStaticDefault;
  #pragma bss_seg()
#else
  #pragma location="HART_FREQ_STATIC"
    const FAST __no_init T_HART_FREQ_STATIC hartFreqStatic;
  #pragma location="HART_FREQ_STATIC_DEFAULT"
    const FAST __no_init T_HART_FREQ_STATIC hartFreqStaticDefault;
#endif
const SLOW T_HART_FREQ_STATIC hartFreqStaticInitializer=
{
    //@SubGen start@    <INITIALIZER FREQ_STATIC>
    HART_SEC_WITHOUT    // TABENUM8 burstUpTxSelection
    //@SubGen end@    <INITIALIZER FREQ_STATIC>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_HART_CONSTANT hartConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    HART_T_ELECTRIC_CURRENT    // CONST_U8 currentClassification
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_STATIC_RARE hartStaticRare;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_RARE"
    const FAST __no_init T_HART_STATIC_RARE hartStaticRare;
#endif
const SLOW T_HART_STATIC_RARE hartStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_RARE>
    {67,78},    // SIMPLE_U8 countryCode[2]
    0    // SIMPLE_U8 SiUnitOnly
    //@SubGen end@    <INITIALIZER STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_HART_DYNAMIC hartDynamic;
const SLOW T_HART_DYNAMIC hartDynamicInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC>
    {HART_CURRENT_MILLIAMPERES,HART_CURRENT_UNIT_TYPE},    // SIMPLE_U8 currentUnit[2]
    0,    // SIMPLE_U8 currentStatus
    HART_VORTEX_FLOW,    // TABENUM8 deviceVariableFamilyCodes
    0,    // SIMPLE_U8 writeDeviceVariableCommCode
    0X7FA00000,    // SIMPLE_U32 dumNAN
    0.0f,    // SIMPLE_FLOAT hartInSim
    0.0f,    // FLOAT fixedOutputValue
    {'w','r','i','t','e',' ','p','r','o','t','e','c','t','i','o','n',' ','o','f','f',0,0,0,0},    // SIMPLE_U8 ResetSoftWriteProtection[24]
    HART_BURST_IN_OK    // TABENUM8 burstInStatus
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of FreqNo data
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_FREQ_NO hartFreqNo;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_FREQUENT_NO_DEFAULT"
    const FAST __no_init T_HART_FREQ_NO hartFreqNo;
#endif
const SLOW T_HART_FREQ_NO hartFreqNoInitializer=
{
    //@SubGen start@    <INITIALIZER FREQ_NO>
    0,    // SIMPLE_U8 burstDefaultMsg
    {0,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_1,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,57,0.0,0,57,0,0},    // BURSTMSG_CONFVALUES burstMsg0ConfData {burstMsgNumber,modeControlCode,cmdExpansionFlag,commandNumber,burstSlot0,burstSlot1,burstSlot2,burstSlot3,burstSlot4,burstSlot5,burstSlot6,burstSlot7,updatePeriod,maximumUpdatePeriod,triggerModeSelectionCode,triggerDevVariableClassification,triggerUnitsCode,triggerValue,processValueClassification,processValueUnitsCode,processValueSubSysId,processValueObjId}
    {1,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_2,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,57,0.0,0,57,0,0},    // BURSTMSG_CONFVALUES burstMsg1ConfData {burstMsgNumber,modeControlCode,cmdExpansionFlag,commandNumber,burstSlot0,burstSlot1,burstSlot2,burstSlot3,burstSlot4,burstSlot5,burstSlot6,burstSlot7,updatePeriod,maximumUpdatePeriod,triggerModeSelectionCode,triggerDevVariableClassification,triggerUnitsCode,triggerValue,processValueClassification,processValueUnitsCode,processValueSubSysId,processValueObjId}
    {2,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_2,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,57,0.0,0,57,0,0},    // BURSTMSG_CONFVALUES burstMsg2ConfData {burstMsgNumber,modeControlCode,cmdExpansionFlag,commandNumber,burstSlot0,burstSlot1,burstSlot2,burstSlot3,burstSlot4,burstSlot5,burstSlot6,burstSlot7,updatePeriod,maximumUpdatePeriod,triggerModeSelectionCode,triggerDevVariableClassification,triggerUnitsCode,triggerValue,processValueClassification,processValueUnitsCode,processValueSubSysId,processValueObjId}
    0,    // SIMPLE_U16 confChangeCounterValue
    HART_LOCK_DEVICE_UNLOCKED,    // TABENUM8 lockDeviceSelection
    HART_LOCK_STATUS_NOT_LOCKED    // SIMPLE_U8 lockDeviceStatus
    //@SubGen end@    <INITIALIZER FREQ_NO>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_HART_DYN hartDyn;
const SLOW T_HART_DYN hartDynInitializer=
{
    //@SubGen start@    <INITIALIZER DYN>
    eFALSE,    // SIMPLE_U8 bolDeviceIsBusy
    -1234,    // SIMPLE_I16 dummys16
    123456789,    // SIMPLE_U32 dummy32
    224,    // SIMPLE_U8 dummyU8
    12345,    // SIMPLE_U16 dummyU16
    -10.0f,    // SIMPLE_FLOAT dummyFloat
    0.0,    // SIMPLE_DOUBLE dummyDouble
    {0,0,0.0,0},    // BURSTMSG_MANAGEVALUES burstMsg0ManageData {updatePeriodCounter,maximumUpdatePeriodCounter,lastSentProcessValue,lastDiagnosisCounter}
    {0,0,0.0,0},    // BURSTMSG_MANAGEVALUES burstMsg1ManageData {updatePeriodCounter,maximumUpdatePeriodCounter,lastSentProcessValue,lastDiagnosisCounter}
    {0,0,0.0,0},    // BURSTMSG_MANAGEVALUES burstMsg2ManageData {updatePeriodCounter,maximumUpdatePeriodCounter,lastSentProcessValue,lastDiagnosisCounter}
    SLOT_CODE_NONE,    // SIMPLE_U8 slot0
    SLOT_CODE_NONE,    // SIMPLE_U8 slot1
    SLOT_CODE_NONE,    // SIMPLE_U8 slot2
    SLOT_CODE_NONE,    // SIMPLE_U8 slot3
    SLOT_CODE_NONE,    // SIMPLE_U8 slot4
    SLOT_CODE_NONE,    // SIMPLE_U8 slot5
    SLOT_CODE_NONE,    // SIMPLE_U8 slot6
    SLOT_CODE_NONE,    // SIMPLE_U8 slot7
    0,    // SIMPLE_U16 subsystemIndex
    0,    // SIMPLE_U16 objectIndex
    0,    // SIMPLE_U8 objectDataLength
    0,    // SIMPLE_U16 attributeIndex
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U8 objectData[32]
    0,    // TABENUM8 board
    0,    // SIMPLE_U8 address
    0x0000,    // SIMPLE_U16 memoryAddress
    0,    // SIMPLE_U8 memoryLength
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 memoryData[32]
    HART_VALUE_USED_IN_HART7,    // TABENUM8 pvtLabelDistCode_Command15
    HART_PV_ALARMSEL_NONE,    // TABENUM8 PV_AlarmSelctionCode
    HART_PV_TRANSF_FC_SPECIAL,    // TABENUM8 PV_Transfer_FunctionCode
    0.0f,    // SIMPLE_FLOAT dampValue
    0,    // SIMPLE_U8 activeDeadbandCode
    0,    // SIMPLE_U8 counterReset
    HART_CONFIG_TIMER_STOP,    // TABENUM8 configExitTimer
    0,    // SIMPLE_U16 configExitElapsedTime
    0,    // SIMPLE_U16 diagCounterMoreStatusAvailablePrimMaster
    0,    // SIMPLE_U16 diagCounterMoreStatusAvailableSecMaster
    HART_EEP_CTRL_PASSIVE,    // TABENUM8 eepromControl
    HART_PERFORM_RESET_DEFAULT,    // SIMPLE_U8 performDeviceReset
    0x30,    // SIMPLE_BIT8 devVariableStatus
    45054,    // SIMPLE_U32 dataTimeStamp
    250,    // SIMPLE_U8 deviceVariableCode
    128000,    // SIMPLE_U32 updatePeriod
    HART_DEFAULT_EXTENDED_DEVICE_STATUS,    // TABENUM8 extendedDeviceStatus
    HART_DFEAULT_DEVICE_OPERATING_MODE,    // TABENUM8 deviceOperatingMode
    HART_DEFAULT_STANDARDIZED_STATUS_0,    // TABENUM8 standardizedStatus0
    HART_DEFAULT_ANALOG_CHANNEL_FIXED,    // TABENUM8 analogChannelFixed
    {0,0,0,0,0,0,0,0},    // SIMPLE_U8 softModemInfo[8]
    HART_FIND_DEVICE_DISABLED,    // TABENUM8 findDeviceSwitch
    {20.0f,4.0f,1.6f},    // SIMPLE_FLOAT transducerLimitsCmd14_54[3]
    {4,51},    // SIMPLE_U8 burstUpUnitCode[2]
    HART_BURST_IN_PRESSURE,    // TABENUM8 burstUpUnitType
    0.0f,    // SIMPLE_FLOAT burstUpDataValue
    50.0f,    // SIMPLE_FLOAT burstUpBaseValue
    {0x00,0x00,0x00},    // SIMPLE_U8 transducerSerialNumber[3]
    0,    // SIMPLE_U32 timeValueHART
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},    // HART_SERVICECODE ServiceCode[20]
    HART_SERVICE_service    // TABENUM8 HartAccessLevel
    //@SubGen end@    <INITIALIZER DYN>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of Rare data
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_RARE hartRare;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_RARE"
    const FAST __no_init T_HART_RARE hartRare;
#endif
const SLOW T_HART_RARE hartRareInitializer=
{
    //@SubGen start@    <INITIALIZER RARE>
    0,    // USIGN8 bitRateCode
    0,    // SIMPLE_U8 fskDisabled
    251,    // SIMPLE_U8 hwWriteProtection
    5,    // USIGN8 numberRquestPream
    5,    // USIGN8 numberRespPream
    0x001A,    // SIMPLE_U16 privateLabelDistributorCode
    {1,0,0}    // SIMPLE_U8 deviceIdNumber[3]
    //@SubGen end@    <INITIALIZER RARE>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of FreqCst data
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_FREQ_CST hartFreqCst;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_HART_FREQ_CST hartFreqCst;
#endif
const SLOW T_HART_FREQ_CST hartFreqCstInitializer=
{
    //@SubGen start@    <INITIALIZER FREQ_CST>
    HART_HART_REVISION_7,    // TABENUM8 hartCmdRev
    HART_HART_REVISION_7,    // TABENUM8 univCmdRev
    0,    // CONF configurationFlags
    0,    // ADDR pollAddr
    {0x82,0x08,0x20,0x82,0x08,0x20},    // SIMPLE_U8 tag[6]
    {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20},    // SIMPLE_U8 deviceMessage[24]
    {0,0,0},    // SIMPLE_U8 finalAssyNumber[3]
    {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20},    // SIMPLE_U8 descriptor[12]
    {0x01,0x01,0x00},    // SIMPLE_U8 deviceInstallDate[3]
    0,    // USIGN8 reserved
    {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20},    // SIMPLE_U8 measuringTask[24]
    0,    // SIMPLE_U8 minSwitchingTime
    1,    // SIMPLE_U8 loopCurrentMode
    {0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F},    // SIMPLE_U8 longTag[32]
    0xFE    // SIMPLE_U8 lastReceiveCommandNumber
    //@SubGen end@    <INITIALIZER FREQ_CST>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_HART_CST hartCst=
{
    //@SubGen start@    <INITIALIZER CST>
    0,    // CONST_U8 dummy_0
    250,    // CONST_U8 dummy250
    254,    // CONST_U8 dummy_254
    1,    // CONST_U8 transSpecRev
    HART_DEVICE_REV_MAPPED_TO_DIFF_VAL,    // CONST_U8 deviceRevisionLevel
    0x1A,    // CONST_U8 mfrId
    163,    // CONST_U8 mfrDeviceType
    18,    // CONST_U8 maxNoOfDevVariables
    {0x00,0x1A},    // CONST_U8 manufacturerIdCode[2]
    1,    // CONST_U8 deviceProfile
    2,    // CONST_U8 flags
    3,    // CONST_U8 bursttotalNumberOfMsg
    250,    // CONST_U8 notUsed
    22,    // CONST_U8 pvt_Label_Dist_Code
    0x00,    // CONST_U8 devVariableClassification
    {0,0,0},    // CONST_U8 devVariableTransducerSerialNo[3]
    250,    // CONST_U8 devVariableFamilyCode
    0x01,    // CONST_U8 pvAnalogChannelFlags
    0,    // CONST_U8 nonValidDeviceVariableClassification
    250,    // CONST_U8 nonValidDeviceVariableUnitCode
    {0x7F,0xA0,0x0,0x0},    // CONST_U8 nonValidDeviceVariableValue[4]
    0x30    // CONST_U8 nonValidDeviceVariableStatus
    //@SubGen end@    <INITIALIZER CST>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 ConfigExitTimerEnumTable[] =
// Table referenced by: configExitTimer
{
  HART_CONFIG_TIMER_STOP,
  HART_CONFIG_TIMER_START
};

const SLOW TUSIGN8 eepromControlTable[] =
// Table referenced by: eepromControl
{
  HART_EEP_CTRL_SAVE_DATA,
  HART_EEP_CTRL_LOAD_DATA,
  HART_EEP_CTRL_PASSIVE
};

const SLOW TUSIGN8 PV_AlarmSelctionCodeTable[] =
// Table referenced by: PV_AlarmSelctionCode
{
  HART_PV_ALARMSEL_HIGH,
  HART_PV_ALARMSEL_LOW,
  HART_PV_ALARMSEL_HOLDLASTOUTPVALUE,
  HART_PV_ALARMSEL_NOT_USED,
  HART_PV_ALARMSEL_NONE,
  HART_PV_ALARMSEL_UNKNOWN,
  HART_PV_ALARMSEL_SPECIAL
};

const SLOW TUSIGN8 PV_Transfer_FunctionCodeTable[] =
// Table referenced by: PV_Transfer_FunctionCode
{
  HART_PV_TRANSF_FC_LINEAR,
  HART_PV_TRANSF_FC_SQUAREROOT,
  HART_PV_TRANSF_FC_SQUAREROOT_THIRDPOW,
  HART_PV_TRANSF_FC_SQUAREROOT_FIFTHPOW,
  HART_PV_TRANSF_FC_SPECIALCURVE,
  HART_PV_TRANSF_FC_SQUARE,
  HART_PV_TRANSF_FC_NOT_USED,
  HART_PV_TRANSF_FC_NONE,
  HART_PV_TRANSF_FC_UNKOWN,
  HART_PV_TRANSF_FC_SPECIAL
};

const SLOW TUSIGN8 HartRevisionTable[] =
// Table referenced by: univCmdRev, hartCmdRev
{
  HART_HART_REVISION_5,
  HART_HART_REVISION_7
};

const SLOW TUSIGN8 FindDeviceSwitchTable[] =
// Table referenced by: findDeviceSwitch
{
  HART_FIND_DEVICE_DISABLED,
  HART_FIND_DEVICE_ONE_TIME,
  HART_FIND_DEVICE_CONTINUOUS
};

const SLOW TUSIGN8 ExtendedDeviceStatusTable[] =
// Table referenced by: extendedDeviceStatus
{
  HART_DEFAULT_EXTENDED_DEVICE_STATUS,
  HART_MAINTENANCE_REQUIRED,
  HART_DEVICE_VARIABLE_ALERT,
  HART_CRITICAL_POWER_FAILURE
};

const SLOW TUSIGN8 DeviceOperatingModeTable[] =
// Table referenced by: deviceOperatingMode
{
  HART_DFEAULT_DEVICE_OPERATING_MODE
};

const SLOW TUSIGN8 StandardizedStatus0Table[] =
// Table referenced by: standardizedStatus0
{
  HART_DEFAULT_STANDARDIZED_STATUS_0,
  HART_SIMULATION_ACTIVE,
  HART_NON_VOLATILE_MEMORY_DEFECT,
  HART_VOLATILE_MEMORY_DEFECT,
  HART_WATCHDOG_RESET_EXECUTED,
  HART_VOLTAGE_CONDITIONS_OUT_OF_RANGE,
  HART_ENVIRONMENTAL_CONDITIONS_OUT_OF_RANGE,
  HART_ELECTRONIC_DEFECT
};

const SLOW TUSIGN8 AnalogChannelFixedTable[] =
// Table referenced by: analogChannelFixed
{
  HART_DEFAULT_ANALOG_CHANNEL_FIXED,
  HART_ANALOG_CHANNEL_1_FIXED,
  HART_ANALOG_CHANNEL_2_FIXED,
  HART_ANALOG_CHANNEL_3_FIXED,
  HART_ANALOG_CHANNEL_4_FIXED
};

const SLOW TUSIGN8 lockDeviceSelectionTable[] =
// Table referenced by: lockDeviceSelection
{
  HART_LOCK_DEVICE_UNLOCKED,
  HART_LOCK_DEVICE_TEMPORARY,
  HART_LOCK_DEVICE_PERMANENT,
  HART_LOCK_DEVICE_ALL
};

const SLOW TUSIGN8 pvtLabelDistCode_Command15_Table[] =
// Table referenced by: pvtLabelDistCode_Command15
{
  HART_MFR_ID_HARTMANN_BRAUN,
  HART_MFR_ID_ABB,
  HART_VALUE_USED_IN_HART7
};

const SLOW TUSIGN8 T_HART_SELECTION[] =
// Table referenced by: burstUpTxSelection
{
  HART_SEC_EXT_TEMPERATURE,
  HART_SEC_PRESSURE,
  HART_SEC_PRESSURE_ABS,
  HART_SEC_GAS_CONTENT,
  HART_SEC_DENSITY,
  HART_SEC_WITHOUT,
  HART_SEC_INTER_TEMPERATURE
};

const SLOW TUSIGN8 T_HART_FAMILY_CODES[] =
// Table referenced by: deviceVariableFamilyCodes
{
  HART_TEMPERATURE,
  HART_PRESSRUE,
  HART_VALVE,
  HART_SIMPLE_PID_CONTROL,
  HART_PH,
  HART_CONDUCTIVITY,
  HART_TOTALIZER,
  HART_LEVEL,
  HART_VORTEX_FLOW,
  HART_MAG_FLOW,
  HART_CORIOLIS_FLOW
};

const SLOW TUSIGN8 T_BURST_IN_UNIT_TYPE[] =
// Table referenced by: burstUpUnitType
{
  HART_BURST_IN_TEMPERATURE,
  HART_BURST_IN_PRESSURE,
  HART_BURST_IN_DENSITY,
  HART_BURST_IN_CONTENT,
  HART_BURST_IN_NONE
};

const SLOW TUSIGN8 T_SERVICE_LEVEL[] =
// Table referenced by: HartAccessLevel
{
  HART_SERVICE_service,
  HART_SERVICE_advanced,
  HART_SERVICE_standard
};

const SLOW TUSIGN8 BoardsTable[] =
// Table referenced by: board
{
  HART_CB_BOARD
};

const SLOW TUSIGN8 BURST_IN_STATUS[] =
// Table referenced by: burstInStatus
{
  HART_BURST_IN_OK,
  HART_BURST_IN_FAILED
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor0 =
// Descriptor referenced by: bitRateCode, reserved
{
  0,  // TUSIGN8 min
  255,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 pollAddrDescriptor =
{
  0,  // TUSIGN8 min
  15,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 configExitTimerDescriptor =
{
  ConfigExitTimerEnumTable,  // TUSIGN8* ptrCodeTab
  sizeof(ConfigExitTimerEnumTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor1 =
// Descriptor referenced by: numberRquestPream, numberRespPream
{
  5,  // TUSIGN8 min
  20,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 PV_AlarmSelctionCodeDescriptor =
{
  PV_AlarmSelctionCodeTable,  // TUSIGN8* ptrCodeTab
  sizeof(PV_AlarmSelctionCodeTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 PV_Transfer_FunctionCodeDescriptor =
{
  PV_Transfer_FunctionCodeTable,  // TUSIGN8* ptrCodeTab
  sizeof(PV_Transfer_FunctionCodeTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor2 =
// Descriptor referenced by: univCmdRev, hartCmdRev
{
  HartRevisionTable,  // TUSIGN8* ptrCodeTab
  sizeof(HartRevisionTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 standardizedStatus0Descriptor =
{
  StandardizedStatus0Table,  // TUSIGN8* ptrCodeTab
  sizeof(StandardizedStatus0Table)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 extendedDeviceStatusDescriptor =
{
  ExtendedDeviceStatusTable,  // TUSIGN8* ptrCodeTab
  sizeof(ExtendedDeviceStatusTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 deviceOperatingModeDescriptor =
{
  DeviceOperatingModeTable,  // TUSIGN8* ptrCodeTab
  sizeof(DeviceOperatingModeTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 analogChannelFixedDescriptor =
{
  AnalogChannelFixedTable,  // TUSIGN8* ptrCodeTab
  sizeof(AnalogChannelFixedTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 findDeviceSwitchDescriptor =
{
  FindDeviceSwitchTable,  // TUSIGN8* ptrCodeTab
  sizeof(FindDeviceSwitchTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 lockDeviceSelectionDescriptor =
{
  lockDeviceSelectionTable,  // TUSIGN8* ptrCodeTab
  sizeof(lockDeviceSelectionTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 burstUpTxSelectionDescriptor =
{
  T_HART_SELECTION,  // TUSIGN8* ptrCodeTab
  sizeof(T_HART_SELECTION)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 pvtLabelDistCode_Command15Descriptor =
{
  pvtLabelDistCode_Command15_Table,  // TUSIGN8* ptrCodeTab
  sizeof(pvtLabelDistCode_Command15_Table)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 eepromControlDescriptor =
{
  eepromControlTable,  // TUSIGN8* ptrCodeTab
  sizeof(eepromControlTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 deviceVariableFamilyCodesDescriptor =
{
  T_HART_FAMILY_CODES,  // TUSIGN8* ptrCodeTab
  sizeof(T_HART_FAMILY_CODES)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 burstUpUnitTypeDescriptor =
{
  T_BURST_IN_UNIT_TYPE,  // TUSIGN8* ptrCodeTab
  sizeof(T_BURST_IN_UNIT_TYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT fixedOutputValueDescriptor =
{
  0.0f,  // TFLOAT min
  23.6f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 HartAccessLevelDescriptor =
{
  T_SERVICE_LEVEL,  // TUSIGN8* ptrCodeTab
  sizeof(T_SERVICE_LEVEL)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 boardDescriptor =
{
  BoardsTable,  // TUSIGN8* ptrCodeTab
  sizeof(BoardsTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 burstInStatusDescriptor =
{
  BURST_IN_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(BURST_IN_STATUS)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ hartObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //resetConfigFlag
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetConfigFlag_HART),

    //StartSelfTest
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        StartSelfTest_HART),

    //bolDeviceIsBusy
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.bolDeviceIsBusy,
        RAM),

    //dummy_0
    CONSTRUCT_CONST_U8(
        &hartCst.dummy_0),

    //dummys16
    CONSTRUCT_SIMPLE_I16(
        &hartDyn.dummys16,
        RAM),

    //dummy32
    CONSTRUCT_SIMPLE_U32(
        &hartDyn.dummy32,
        RAM),

    //dummy250
    CONSTRUCT_CONST_U8(
        &hartCst.dummy250),

    //dummy_254
    CONSTRUCT_CONST_U8(
        &hartCst.dummy_254),

    //dummyU8
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.dummyU8,
        RAM),

    //dummyU16
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.dummyU16,
        RAM),

    //dummyFloat
    CONSTRUCT_SIMPLE_FLOAT(
        &hartDyn.dummyFloat,
        RAM),

    //dummyDouble
    CONSTRUCT_SIMPLE_DOUBLE(
        &hartDyn.dummyDouble,
        RAM),

    //bitRateCode
    CONSTRUCT_USIGN8(
        &hartRare.bitRateCode,
        NON_VOLATILE,
        &sharedTRangeU8Descriptor0),

    //fskDisabled
    CONSTRUCT_SIMPLE_U8(
        &hartRare.fskDisabled,
        NON_VOLATILE),

    //hwWriteProtection
    CONSTRUCT_SIMPLE_U8(
        &hartRare.hwWriteProtection,
        NON_VOLATILE),

    //numberRquestPream
    CONSTRUCT_USIGN8(
        &hartRare.numberRquestPream,
        NON_VOLATILE,
        &sharedTRangeU8Descriptor1),

    //numberRespPream
    CONSTRUCT_USIGN8(
        &hartRare.numberRespPream,
        NON_VOLATILE,
        &sharedTRangeU8Descriptor1),

    //hartCmdRev
    CONSTRUCT_TABENUM8(
        &hartFreqCst.hartCmdRev,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor2),

    //univCmdRev
    CONSTRUCT_TABENUM8(
        &hartFreqCst.univCmdRev,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor2),

    //transSpecRev
    CONSTRUCT_CONST_U8(
        &hartCst.transSpecRev),

    //deviceRevisionLevel
    CONSTRUCT_CONST_U8(
        &hartCst.deviceRevisionLevel),

    //mfrId
    CONSTRUCT_CONST_U8(
        &hartCst.mfrId),

    //mfrDeviceType
    CONSTRUCT_CONST_U8(
        &hartCst.mfrDeviceType),

    //maxNoOfDevVariables
    CONSTRUCT_CONST_U8(
        &hartCst.maxNoOfDevVariables),

    //manufacturerIdCode
    CONSTRUCT_ARRAY_CONST_U8(
        hartCst.manufacturerIdCode,
        2),

    //privateLabelDistributorCode
    CONSTRUCT_SIMPLE_U16(
        &hartRare.privateLabelDistributorCode,
        NON_VOLATILE),

    //deviceProfile
    CONSTRUCT_CONST_U8(
        &hartCst.deviceProfile),

    //flags
    CONSTRUCT_CONST_U8(
        &hartCst.flags),

    //configurationFlags
    CONSTRUCT_CONF(
        &hartFreqCst.configurationFlags),

    //pollAddr
    CONSTRUCT_ADDR(
        &hartFreqCst.pollAddr,
        &pollAddrDescriptor),

    //bursttotalNumberOfMsg
    CONSTRUCT_CONST_U8(
        &hartCst.bursttotalNumberOfMsg),

    //burstDefaultMsg
    CONSTRUCT_SIMPLE_U8(
        &hartFreqNo.burstDefaultMsg,
        NON_VOLATILE),

    //burstMsg0ConfData
    CONSTRUCT_BURSTMSG_CONFVALUES(
        &hartFreqNo.burstMsg0ConfData),

    //burstMsg0ManageData
    CONSTRUCT_BURSTMSG_MANAGEVALUES(
        &hartDyn.burstMsg0ManageData),

    //burstMsg1ConfData
    CONSTRUCT_BURSTMSG_CONFVALUES(
        &hartFreqNo.burstMsg1ConfData),

    //burstMsg1ManageData
    CONSTRUCT_BURSTMSG_MANAGEVALUES(
        &hartDyn.burstMsg1ManageData),

    //burstMsg2ConfData
    CONSTRUCT_BURSTMSG_CONFVALUES(
        &hartFreqNo.burstMsg2ConfData),

    //burstMsg2ManageData
    CONSTRUCT_BURSTMSG_MANAGEVALUES(
        &hartDyn.burstMsg2ManageData),

    //tag
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.tag,
        6,
        NON_VOLATILE),

    //deviceMessage
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.deviceMessage,
        24,
        NON_VOLATILE),

    //finalAssyNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.finalAssyNumber,
        3,
        NON_VOLATILE),

    //descriptor
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.descriptor,
        12,
        NON_VOLATILE),

    //deviceIdNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartRare.deviceIdNumber,
        3,
        NON_VOLATILE),

    //deviceInstallDate
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.deviceInstallDate,
        3,
        NON_VOLATILE),

    //slot0
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot0,
        RAM),

    //slot1
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot1,
        RAM),

    //slot2
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot2,
        RAM),

    //slot3
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot3,
        RAM),

    //slot4
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot4,
        RAM),

    //slot5
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot5,
        RAM),

    //slot6
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot6,
        RAM),

    //slot7
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.slot7,
        RAM),

    //reserved
    CONSTRUCT_USIGN8(
        &hartFreqCst.reserved,
        NON_VOLATILE,
        &sharedTRangeU8Descriptor0),

    //subsystemIndex
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.subsystemIndex,
        RAM),

    //objectIndex
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.objectIndex,
        RAM),

    //objectDataLength
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.objectDataLength,
        RAM),

    //attributeIndex
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.attributeIndex,
        RAM),

    //objectData
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDyn.objectData,
        32,
        RAM),

    //board
    CONSTRUCT_TABENUM8(
        &hartDyn.board,
        RAM,
        &boardDescriptor),

    //address
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.address,
        RAM),

    //memoryAddress
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.memoryAddress,
        RAM),

    //memoryLength
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.memoryLength,
        RAM),

    //memoryData
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDyn.memoryData,
        32,
        RAM),

    //pvtLabelDistCode_Command15
    CONSTRUCT_TABENUM8(
        &hartDyn.pvtLabelDistCode_Command15,
        RAM,
        &pvtLabelDistCode_Command15Descriptor),

    //notUsed
    CONSTRUCT_CONST_U8(
        &hartCst.notUsed),

    //PV_AlarmSelctionCode
    CONSTRUCT_TABENUM8(
        &hartDyn.PV_AlarmSelctionCode,
        RAM,
        &PV_AlarmSelctionCodeDescriptor),

    //PV_Transfer_FunctionCode
    CONSTRUCT_TABENUM8(
        &hartDyn.PV_Transfer_FunctionCode,
        RAM,
        &PV_Transfer_FunctionCodeDescriptor),

    //dampValue
    CONSTRUCT_SIMPLE_FLOAT(
        &hartDyn.dampValue,
        RAM),

    //pvt_Label_Dist_Code
    CONSTRUCT_CONST_U8(
        &hartCst.pvt_Label_Dist_Code),

    //activeDeadbandCode
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.activeDeadbandCode,
        RAM),

    //measuringTask
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.measuringTask,
        24,
        NON_VOLATILE),

    //minSwitchingTime
    CONSTRUCT_SIMPLE_U8(
        &hartFreqCst.minSwitchingTime,
        NON_VOLATILE),

    //counterReset
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.counterReset,
        RAM),

    //configExitTimer
    CONSTRUCT_TABENUM8(
        &hartDyn.configExitTimer,
        RAM,
        &configExitTimerDescriptor),

    //configExitElapsedTime
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.configExitElapsedTime,
        RAM),

    //diagCounterMoreStatusAvailablePrimMaster
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.diagCounterMoreStatusAvailablePrimMaster,
        RAM),

    //diagCounterMoreStatusAvailableSecMaster
    CONSTRUCT_SIMPLE_U16(
        &hartDyn.diagCounterMoreStatusAvailableSecMaster,
        RAM),

    //eepromControl
    CONSTRUCT_TABENUM8(
        &hartDyn.eepromControl,
        RAM,
        &eepromControlDescriptor),

    //performDeviceReset
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.performDeviceReset,
        READONLY_RAM),

    //resetDevice
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetDevice_HART),

    //devVariableClassification
    CONSTRUCT_CONST_U8(
        &hartCst.devVariableClassification),

    //devVariableTransducerSerialNo
    CONSTRUCT_ARRAY_CONST_U8(
        hartCst.devVariableTransducerSerialNo,
        3),

    //devVariableFamilyCode
    CONSTRUCT_CONST_U8(
        &hartCst.devVariableFamilyCode),

    //devVariableStatus
    CONSTRUCT_SIMPLE_BIT8(
        &hartDyn.devVariableStatus,
        RAM),

    //dataTimeStamp
    CONSTRUCT_SIMPLE_U32(
        &hartDyn.dataTimeStamp,
        RAM),

    //loopCurrentMode
    CONSTRUCT_SIMPLE_U8(
        &hartFreqCst.loopCurrentMode,
        NON_VOLATILE),

    //longTag
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartFreqCst.longTag,
        32,
        NON_VOLATILE),

    //confChangeCounterValue
    CONSTRUCT_SIMPLE_U16(
        &hartFreqNo.confChangeCounterValue,
        NON_VOLATILE),

    //resetMoreStatusAvailable
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetMoreStatusAvailable_HART),

    //pvAnalogChannelFlags
    CONSTRUCT_CONST_U8(
        &hartCst.pvAnalogChannelFlags),

    //nonValidDeviceVariableClassification
    CONSTRUCT_CONST_U8(
        &hartCst.nonValidDeviceVariableClassification),

    //nonValidDeviceVariableUnitCode
    CONSTRUCT_CONST_U8(
        &hartCst.nonValidDeviceVariableUnitCode),

    //nonValidDeviceVariableValue
    CONSTRUCT_ARRAY_CONST_U8(
        hartCst.nonValidDeviceVariableValue,
        4),

    //nonValidDeviceVariableStatus
    CONSTRUCT_CONST_U8(
        &hartCst.nonValidDeviceVariableStatus),

    //deviceVariableCode
    CONSTRUCT_SIMPLE_U8(
        &hartDyn.deviceVariableCode,
        RAM),

    //updatePeriod
    CONSTRUCT_SIMPLE_U32(
        &hartDyn.updatePeriod,
        RAM),

    //extendedDeviceStatus
    CONSTRUCT_TABENUM8(
        &hartDyn.extendedDeviceStatus,
        RAM,
        &extendedDeviceStatusDescriptor),

    //deviceOperatingMode
    CONSTRUCT_TABENUM8(
        &hartDyn.deviceOperatingMode,
        RAM,
        &deviceOperatingModeDescriptor),

    //standardizedStatus0
    CONSTRUCT_TABENUM8(
        &hartDyn.standardizedStatus0,
        RAM,
        &standardizedStatus0Descriptor),

    //analogChannelFixed
    CONSTRUCT_TABENUM8(
        &hartDyn.analogChannelFixed,
        RAM,
        &analogChannelFixedDescriptor),

    //softModemInfo
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDyn.softModemInfo,
        8,
        RAM),

    //squawk
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        squawk_HART),

    //findDeviceSwitch
    CONSTRUCT_TABENUM8(
        &hartDyn.findDeviceSwitch,
        RAM,
        &findDeviceSwitchDescriptor),

    //lockDeviceSelection
    CONSTRUCT_TABENUM8(
        &hartFreqNo.lockDeviceSelection,
        NON_VOLATILE,
        &lockDeviceSelectionDescriptor),

    //lockDeviceStatus
    CONSTRUCT_SIMPLE_U8(
        &hartFreqNo.lockDeviceStatus,
        NON_VOLATILE),

    //transducerLimitsCmd14_54
    CONSTRUCT_ARRAY_SIMPLE_FLOAT(
        hartDyn.transducerLimitsCmd14_54,
        3,
        RAM),

    //burstUpUnitCode
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDyn.burstUpUnitCode,
        2,
        RAM),

    //burstUpUnitType
    CONSTRUCT_TABENUM8(
        &hartDyn.burstUpUnitType,
        RAM,
        &burstUpUnitTypeDescriptor),

    //burstUpDataValue
    CONSTRUCT_SIMPLE_FLOAT(
        &hartDyn.burstUpDataValue,
        RAM),

    //burstUpBaseValue
    CONSTRUCT_SIMPLE_FLOAT(
        &hartDyn.burstUpBaseValue,
        RAM),

    //burstUpTxSelection
    CONSTRUCT_TABENUM8(
        &hartFreqStatic.burstUpTxSelection,
        NON_VOLATILE,
        &burstUpTxSelectionDescriptor),

    //transducerSerialNumber
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDyn.transducerSerialNumber,
        3,
        RAM),

    //lastReceiveCommandNumber
    CONSTRUCT_SIMPLE_U8(
        &hartFreqCst.lastReceiveCommandNumber,
        NON_VOLATILE),

    //timeValueHART
    CONSTRUCT_SIMPLE_U32(
        &hartDyn.timeValueHART,
        RAM),

    //currentClassification
    CONSTRUCT_CONST_U8(
        &hartConstant.currentClassification),

    //currentUnit
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDynamic.currentUnit,
        2,
        RAM),

    //currentStatus
    CONSTRUCT_SIMPLE_U8(
        &hartDynamic.currentStatus,
        RAM),

    //setPVUpRangeValue
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        setPVUpRangeValue_HART),

    //setPVLowerRangeValue
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        setPVLowerRangeValue_HART),

    //setDeviceVariableNZero
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        setDeviceVariableNZero_HART),

    //resetDeviceVariableNTrim
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetDeviceVariableNTrim_HART),

    //deviceVariableFamilyCodes
    CONSTRUCT_TABENUM8(
        &hartDynamic.deviceVariableFamilyCodes,
        RAM,
        &deviceVariableFamilyCodesDescriptor),

    //writeDeviceVariableCommCode
    CONSTRUCT_SIMPLE_U8(
        &hartDynamic.writeDeviceVariableCommCode,
        RAM),

    //ServiceCode
    CONSTRUCT_HART_SERVICECODE(
        hartDyn.ServiceCode,
        RAM),

    //serviceCodeLogOut
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        serviceCodeLogOut_HART),

    //dumNAN
    CONSTRUCT_SIMPLE_U32(
        &hartDynamic.dumNAN,
        RAM),

    //countryCode
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartStaticRare.countryCode,
        2,
        NON_VOLATILE),

    //SiUnitOnly
    CONSTRUCT_SIMPLE_U8(
        &hartStaticRare.SiUnitOnly,
        NON_VOLATILE),

    //performActions
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        performActions_HART),

    //hartInSim
    CONSTRUCT_SIMPLE_FLOAT(
        &hartDynamic.hartInSim,
        RAM),

    //fixedOutputValue
    CONSTRUCT_FLOAT(
        &hartDynamic.fixedOutputValue,
        RAM,
        &fixedOutputValueDescriptor),

    //HartAccessLevel
    CONSTRUCT_TABENUM8(
        &hartDyn.HartAccessLevel,
        RAM,
        &HartAccessLevelDescriptor),

    //ResetSoftWriteProtection
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hartDynamic.ResetSoftWriteProtection,
        24,
        RAM),

    //burstInStatus
    CONSTRUCT_TABENUM8(
        &hartDynamic.burstInStatus,
        RAM,
        &burstInStatusDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the HART-Class object
SLOW const T_HART hart =
{
    {
        &hartState,
        &hartDataSemaphore,

        hartObjectList,
        //lint -e{778}
        sizeof(hartObjectList)/sizeof(hartObjectList[0])-1,

        hartDataClassList,
        sizeof(hartDataClassList)/sizeof(hartDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_HART, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_HART,
        Put_HART,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,HARTCommandInterpreterEXE_HART
    ,IncrementConfiChangeCounter_HART
    ,GetDeviceLockStatus_HART
    ,HART_PutReceiveBuffer_SRV_HART
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: hart_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_HART_TEMPLATE hartTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_HART_TEMPLATE hartTemplate;
const SLOW T_HART_TEMPLATE hartTemplateInitializer=
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
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_TEMPLATE hartTemplate;
    const FAST T_HART_TEMPLATE hartTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="HART_TEMPLATE"
    const FAST __no_init T_HART_TEMPLATE hartTemplate;
  #pragma location="HART_TEMPLATE_DEFAULT"
    const FAST __no_init T_HART_TEMPLATE hartTemplateDefault;
#endif
const SLOW T_HART_TEMPLATE hartTemplateInitializer=
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
  #pragma bss_seg(".AAA$HART_DATA")
    const FAST T_HART_TEMPLATE hartTemplate;
  #pragma bss_seg()
#else
  #pragma location="HART_TEMPLATE"
    const FAST __no_init T_HART_TEMPLATE hartTemplate;
#endif
const SLOW T_HART_TEMPLATE hartTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
hartTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(hartTemplate,hartTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(hartTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(hartTemplate,hartTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(hartTemplate,hartTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(hartTemplate,hartTemplateInitializer,hartTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

