//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
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
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"
#include "modbus_types.h"
#include "modbus_attributes.h"
#include "modbus_overload.h"
#include "modbus_execute.h"


//-------------------------------------------------------------------------------------------------
//! unit global attributes
TUSIGN16 modbusState;     // Subsystem state
T_DATA_SEMAPHORE modbusDataSemaphore;


const T_DATACLASS modbusDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(modbusConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(modbusDynamic,modbusDynamicInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(modbusStaticFrequentNoDefault,modbusStaticFrequentNoDefaultInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(modbusStaticFrequentConstantDefault,modbusStaticFrequentConstantDefaultInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentNoDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    const T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_STATIC_FREQUENT_NO_DEFAULT"
    const __no_init T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefault;
#endif
const T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_NO_DEFAULT>
    MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED    // TABENUM8 modbusIEEEFormat
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_NO_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    const T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const __no_init T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefault;
#endif
const T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U16 configScanRegister1[32]
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    // SIMPLE_U16 configScanRegister2[32]
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of constant block
const T_MODBUS_CONSTANT modbusConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    MODBUS_TCP_PORT    // SIMPLE_U16 networkPort
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
T_MODBUS_DYNAMIC modbusDynamic;
const T_MODBUS_DYNAMIC modbusDynamicInitializer=
{
  //@SubGen start@    <INITIALIZER DYNAMIC>
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // MODBUS_DIAG mbDiag {diagByte0Bit0,diagByte0Bit1,diagByte0Bit2,diagByte0Bit3,diagByte0Bit4,diagByte0Bit5,diagByte0Bit6,diagByte0Bit7,diagByte1Bit0,diagByte1Bit1,diagByte1Bit2,diagByte1Bit3,diagByte1Bit4,diagByte1Bit5,diagByte1Bit6,diagByte1Bit7,diagByte2Bit0,diagByte2Bit1,diagByte2Bit2,diagByte2Bit3,diagByte2Bit4,diagByte2Bit5,diagByte2Bit6,diagByte2Bit7,diagByte3Bit0,diagByte3Bit1,diagByte3Bit2,diagByte3Bit3,diagByte3Bit4,diagByte3Bit5,diagByte3Bit6,diagByte3Bit7,diagByte4Bit0,diagByte4Bit1,diagByte4Bit2,diagByte4Bit3,diagByte4Bit4,diagByte4Bit5,diagByte4Bit6,diagByte4Bit7,diagByte5Bit0,diagByte5Bit1,diagByte5Bit2,diagByte5Bit3,diagByte5Bit4,diagByte5Bit5,diagByte5Bit6,diagByte5Bit7}
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // MODBUS_DIAG_HIS mbDiagHis {diagHisByte0Bit0,diagHisByte0Bit1,diagHisByte0Bit2,diagHisByte0Bit3,diagHisByte0Bit4,diagHisByte0Bit5,diagHisByte0Bit6,diagHisByte0Bit7,diagHisByte1Bit0,diagHisByte1Bit1,diagHisByte1Bit2,diagHisByte1Bit3,diagHisByte1Bit4,diagHisByte1Bit5,diagHisByte1Bit6,diagHisByte1Bit7,diagHisByte2Bit0,diagHisByte2Bit1,diagHisByte2Bit2,diagHisByte2Bit3,diagHisByte2Bit4,diagHisByte2Bit5,diagHisByte2Bit6,diagHisByte2Bit7,diagHisByte3Bit0,diagHisByte3Bit1,diagHisByte3Bit2,diagHisByte3Bit3,diagHisByte3Bit4,diagHisByte3Bit5,diagHisByte3Bit6,diagHisByte3Bit7,diagHisByte4Bit0,diagHisByte4Bit1,diagHisByte4Bit2,diagHisByte4Bit3,diagHisByte4Bit4,diagHisByte4Bit5,diagHisByte4Bit6,diagHisByte4Bit7,diagHisByte5Bit0,diagHisByte5Bit1,diagHisByte5Bit2,diagHisByte5Bit3,diagHisByte5Bit4,diagHisByte5Bit5,diagHisByte5Bit6,diagHisByte5Bit7}
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U16 ScanRegister1[32]
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U16 ScanRegister2[32]
    0,    // SIMPLE_U32 manufactuerDeviceID
    0,    // SIMPLE_U32 sensorSerialID
    0,    // SIMPLE_U32 slaveID
    {0,0,0},    // SIMPLE_U16 mDiagCondenseStatus[3]
    MODBUS_WRITE_FUNCTIONS_ENABLED    // TABENUM8 writeFunctionsEnabled
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 TModbusConfigurationIEEEEnableState[] =
// Table referenced by: modbusIEEEFormat
{
  MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED,
  MODBUS_CONFIGURATION_IEEE_FORMAT_DISABLED
};

const SLOW TUSIGN8 E_WriteFunctionsEnabling[] =
// Table referenced by: writeFunctionsEnabled
{
  MODBUS_WRITE_FUNCTIONS_DISABLED,
  MODBUS_WRITE_FUNCTIONS_ENABLED
};

static const SLOW T_RANGE_E8 modbusIEEEFormatDescriptor =
{
  TModbusConfigurationIEEEEnableState,  // TUSIGN8* ptrCodeTab
  MODBUS_IEEE_FORMAT_COUNT  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 writeFunctionsEnabledDescriptor =
{
  E_WriteFunctionsEnabling,  // TUSIGN8* ptrCodeTab
  sizeof(E_WriteFunctionsEnabling)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const T_DATA_OBJ modbusObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //mbDiag
    CONSTRUCT_MODBUS_DIAG(
        &modbusDynamic.mbDiag,
        RAM),

    //mbDiagHis
    CONSTRUCT_MODBUS_DIAG_HIS(
        &modbusDynamic.mbDiagHis,
        RAM),

    //modbusIEEEFormat
    CONSTRUCT_TABENUM8(
        &modbusStaticFrequentNoDefault.modbusIEEEFormat,
        NON_VOLATILE,
        &modbusIEEEFormatDescriptor),

    //configScanRegister1
    CONSTRUCT_ARRAY_SIMPLE_U16(
        modbusStaticFrequentConstantDefault.configScanRegister1,
        32,
        NON_VOLATILE),

    //configScanRegister2
    CONSTRUCT_ARRAY_SIMPLE_U16(
        modbusStaticFrequentConstantDefault.configScanRegister2,
        32,
        NON_VOLATILE),

    //ScanRegister1
    CONSTRUCT_ARRAY_SIMPLE_U16(
        modbusDynamic.ScanRegister1,
        32,
        RAM),

    //ScanRegister2
    CONSTRUCT_ARRAY_SIMPLE_U16(
        modbusDynamic.ScanRegister2,
        32,
        RAM),

    //manufactuerDeviceID
    CONSTRUCT_SIMPLE_U32(
        &modbusDynamic.manufactuerDeviceID,
        RAM),

    //sensorSerialID
    CONSTRUCT_SIMPLE_U32(
        &modbusDynamic.sensorSerialID,
        RAM),

    //slaveID
    CONSTRUCT_SIMPLE_U32(
        &modbusDynamic.slaveID,
        RAM),

    //mDiagCondenseStatus
    CONSTRUCT_ARRAY_SIMPLE_U16(
        modbusDynamic.mDiagCondenseStatus,
        3,
        RAM),

    //networkPort
    CONSTRUCT_SIMPLE_U16(
        &modbusConstant.networkPort,
        READONLY_RAM),

    //writeFunctionsEnabled
    CONSTRUCT_TABENUM8(
        &modbusDynamic.writeFunctionsEnabled,
        RAM,
        &writeFunctionsEnabledDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the MODBUS-Class object
SLOW const T_MODBUS modbus =
{
    {
        &modbusState,
        &modbusDataSemaphore,

        modbusObjectList,
        //lint -e{778}
        sizeof(modbusObjectList)/sizeof(modbusObjectList[0])-1,

        modbusDataClassList,
        sizeof(modbusDataClassList)/sizeof(modbusDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_MODBUS, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,

        Get_MODBUS,  // will be overloaded
        Put_MODBUS,  // will be overloaded
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,UpdateScanRegister_MODBUS
    ,SinkEventEXE_MODBUS
    ,UpdateEnableStateEXE_MODBUS
    ,TaskEXE_MODBUS
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: Modbus_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const T_MODBUS_TEMPLATE modbusTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
T_MODBUS_TEMPLATE modbusTemplate;
const T_MODBUS_TEMPLATE modbusTemplateInitializer=
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
  #pragma bss_seg(".AAA$MODBUS_DATA")
    const T_MODBUS_TEMPLATE modbusTemplate;
    const T_MODBUS_TEMPLATE modbusTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_TEMPLATE"
    const __no_init T_MODBUS_TEMPLATE modbusTemplate;
  #pragma location="MODBUS_TEMPLATE_DEFAULT"
    const __no_init T_MODBUS_TEMPLATE modbusTemplateDefault;
#endif
const T_MODBUS_TEMPLATE modbusTemplateInitializer=
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
  #pragma bss_seg(".AAA$MODBUS_DATA")
    const T_MODBUS_TEMPLATE modbusTemplate;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_TEMPLATE"
    const __no_init T_MODBUS_TEMPLATE modbusTemplate;
#endif
const T_MODBUS_TEMPLATE modbusTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
modbusTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(modbusTemplate,modbusTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(modbusTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(modbusTemplate,modbusTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(modbusTemplate,modbusTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(modbusTemplate,modbusTemplateInitializer,modbusTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/
