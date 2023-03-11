//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
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
// include the data_obj-classes you need, e.g. T_DATA_OBJ::INT16
//#include "..\..\t_data_obj\interface\t_data_obj_int16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Ethernet/Interface/Ethernet.h"
#include "ethernet_types.h"
#include "ethernet_attributes.h"
#include "ethernet_overload.h"
#include "ethernet_execute.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
TUSIGN16 ethernetState;     // Subsystem state
T_DATA_SEMAPHORE ethernetDataSemaphore;
TUSIGN8 ethernet_ControlFactoryDefaults = 0;

const T_DATACLASS ethernetDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(ethernetConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(ethernetDynamic,ethernetDynamicInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(ethernetStaticFrequentStaticDefault,ethernetStaticFrequentStaticDefaultInitializer,ethernetStaticFrequentStaticDefaultDefault)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const T_ETHERNET_CONSTANT ethernetConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    {'?','?','-','?','?','-','?','?','-','?','?','-','?','?','-','?','?'},    // SIMPLE_CHAR chasisMACstr[17]
    {'?','?','-','?','?','-','?','?','-','?','?','-','?','?','-','?','?'},    // SIMPLE_CHAR port1MACstr[17]
    {'?','?','-','?','?','-','?','?','-','?','?','-','?','?','-','?','?'}    // SIMPLE_CHAR port2MACstr[17]
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
T_ETHERNET_DYNAMIC ethernetDynamic;
const T_ETHERNET_DYNAMIC ethernetDynamicInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC>
    {0,0,0,0,0,0},    // SIMPLE_U8 chasisMAC[6]
    {0,0,0,0,0,0},    // SIMPLE_U8 port1MAC[6]
    {0,0,0,0,0,0},    // SIMPLE_U8 port2MAC[6]
    {0,0,0,0},    // SIMPLE_U32 port1EthStatsHMI[4]
    {0,0,0,0},    // SIMPLE_U32 port2EthStatsHMI[4]
    0,    // SIMPLE_U32 ipStats
    {0,0},    // SIMPLE_U32 ntpServerAddress2[2]
    {ETH_PORT_SPEED_100,ETH_PORT_SPEED_100},    // TABENUM8 portSpeedStatus[2]
    {ETH_FULL_DUPLEX,ETH_FULL_DUPLEX},    // TABENUM8 portDuplexStatus[2]
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    // SIMPLE_U8 hmiValidityTestResults[32]
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefault;
    const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT"
    const __no_init T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefault;
  #pragma location="ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const __no_init T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultDefault;
#endif
const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
    ETH_INVALID_PROTOCOL,    // TABENUM8 selectedMainProtocol
    ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE,    // TABENUM8 enableModbusTcp
    ETH_FULL_ACCESS_INVALID,    // TABENUM8 enableOpcUa
    ETH_UNSECURED_ACCESS_INVALID,    // TABENUM8 enableEthernetIp
    ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE,    // TABENUM8 enableProfinetIo
    ETH_SECURED_ACCESS_SECURED_READ_WRITE,    // TABENUM8 enableWebserver
    ETH_AUTO_NEGOTIATE_ENABLED,    // TABENUM8 port1AutoNegotiate
    ETH_PORT_SPEED_100,    // TABENUM8 port1Speed
    ETH_FULL_DUPLEX,    // TABENUM8 port1DuplexMode
    ETH_AUTO_NEGOTIATE_ENABLED,    // TABENUM8 port2AutoNegotiate
    ETH_PORT_SPEED_100,    // TABENUM8 port2Speed
    ETH_FULL_DUPLEX,    // TABENUM8 port2DuplexMode
    ETH_DHCP_STATE_DISABLED,    // TABENUM8 useDhcp
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_CHAR hostName[65]
    0xC0A80506,    // SIMPLE_U32 hostAddress
    0xFFFFFF00,    // SIMPLE_U32 subnetMask
    0xC0A80501,    // SIMPLE_U32 gatewayAddress
    0,    // SIMPLE_U32 ntpServerAddress
    {0,0},    // SIMPLE_U32 dnsAddress[2]
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    // SIMPLE_CHAR domainName[65]
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 E_SelectedMainProtocol[] =
// Table referenced by: selectedMainProtocol
{
  ETH_INVALID_PROTOCOL,
  ETH_ETHERNETIP_PROTOCOL,
  ETH_PROFINETIO_PROTOCOL
};

const SLOW TUSIGN8 E_AutoNegotiate[] =
// Table referenced by: port1AutoNegotiate, port2AutoNegotiate
{
  ETH_AUTO_NEGOTIATE_UNKNOWN,
  ETH_AUTO_NEGOTIATE_DISABLED,
  ETH_AUTO_NEGOTIATE_ENABLED
};

const SLOW TUSIGN8 E_PortSpeed[] =
// Table referenced by: port1Speed, port2Speed, portSpeedStatus
{
  ETH_PORT_SPEED_UNKNOWN,
  ETH_PORT_SPEED_10,
  ETH_PORT_SPEED_100,
  ETH_PORT_SPEED_1000
};

const SLOW TUSIGN8 E_DuplexMode[] =
// Table referenced by: port1DuplexMode, port2DuplexMode, portDuplexStatus
{
  ETH_UNKNOWN_DUPLEX,
  ETH_HALF_DUPLEX,
  ETH_FULL_DUPLEX
};

const SLOW TUSIGN8 E_UseDhcp[] =
// Table referenced by: useDhcp
{
  ETH_DHCP_STATE_DISABLED,
  ETH_DHCP_STATE_ENABLED
};

const SLOW TUSIGN8 E_AccessStateFull[] =
// Table referenced by: enableOpcUa
{
  ETH_FULL_ACCESS_INVALID,
  ETH_FULL_ACCESS_DISABLED,
  ETH_FULL_ACCESS_UNSECURED_READ_ONLY,
  ETH_FULL_ACCESS_UNSECURED_READ_WRITE,
  ETH_FULL_ACCESS_SECURED_READ_ONLY,
  ETH_FULL_ACCESS_SECURED_READ_WRITE
};

const SLOW TUSIGN8 E_AccessStateUnsecured[] =
// Table referenced by: enableModbusTcp, enableEthernetIp, enableProfinetIo
{
  ETH_UNSECURED_ACCESS_INVALID,
  ETH_UNSECURED_ACCESS_DISABLED,
  ETH_UNSECURED_ACCESS_UNSECURED_READ_ONLY,
  ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE
};

const SLOW TUSIGN8 E_AccessStateSecured[] =
// Table referenced by: enableWebserver
{
  ETH_SECURED_ACCESS_INVALID,
  ETH_SECURED_ACCESS_DISABLED,
  ETH_SECURED_ACCESS_SECURED_READ_ONLY,
  ETH_SECURED_ACCESS_SECURED_READ_WRITE
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: enableModbusTcp, enableEthernetIp, enableProfinetIo
{
  E_AccessStateUnsecured,  // TUSIGN8* ptrCodeTab
  sizeof(E_AccessStateUnsecured)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 enableOpcUaDescriptor =
{
  E_AccessStateFull,  // TUSIGN8* ptrCodeTab
  sizeof(E_AccessStateFull)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 enableWebserverDescriptor =
{
  E_AccessStateSecured,  // TUSIGN8* ptrCodeTab
  sizeof(E_AccessStateSecured)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 selectedMainProtocolDescriptor =
{
  E_SelectedMainProtocol,  // TUSIGN8* ptrCodeTab
  sizeof(E_SelectedMainProtocol)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor1 =
// Descriptor referenced by: port1AutoNegotiate, port2AutoNegotiate
{
  E_AutoNegotiate,  // TUSIGN8* ptrCodeTab
  sizeof(E_AutoNegotiate)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor2 =
// Descriptor referenced by: port1Speed, port2Speed, portSpeedStatus
{
  E_PortSpeed,  // TUSIGN8* ptrCodeTab
  sizeof(E_PortSpeed)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor3 =
// Descriptor referenced by: port1DuplexMode, port2DuplexMode, portDuplexStatus
{
  E_DuplexMode,  // TUSIGN8* ptrCodeTab
  sizeof(E_DuplexMode)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 useDhcpDescriptor =
{
  E_UseDhcp,  // TUSIGN8* ptrCodeTab
  sizeof(E_UseDhcp)  // TUSIGN8 numberOfCodes
};


//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const T_DATA_OBJ ethernetObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //selectedMainProtocol
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.selectedMainProtocol,
        NON_VOLATILE,
        &selectedMainProtocolDescriptor),

    //enableModbusTcp
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.enableModbusTcp,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //enableOpcUa
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.enableOpcUa,
        NON_VOLATILE,
        &enableOpcUaDescriptor),

    //enableEthernetIp
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.enableEthernetIp,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //enableProfinetIo
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.enableProfinetIo,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //enableWebserver
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.enableWebserver,
        NON_VOLATILE,
        &enableWebserverDescriptor),

    //chasisMAC
    CONSTRUCT_ARRAY_SIMPLE_U8(
        ethernetDynamic.chasisMAC,
        6,
        READONLY_RAM),

    //port1MAC
    CONSTRUCT_ARRAY_SIMPLE_U8(
        ethernetDynamic.port1MAC,
        6,
        READONLY_RAM),

    //port1AutoNegotiate
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port1AutoNegotiate,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor1),

    //port1Speed
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port1Speed,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor2),

    //port1DuplexMode
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port1DuplexMode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor3),

    //port2MAC
    CONSTRUCT_ARRAY_SIMPLE_U8(
        ethernetDynamic.port2MAC,
        6,
        READONLY_RAM),

    //port2AutoNegotiate
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port2AutoNegotiate,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor1),

    //port2Speed
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port2Speed,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor2),

    //port2DuplexMode
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.port2DuplexMode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor3),

    //useDhcp
    CONSTRUCT_TABENUM8(
        &ethernetStaticFrequentStaticDefault.useDhcp,
        NON_VOLATILE,
        &useDhcpDescriptor),

    //hostName
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        ethernetStaticFrequentStaticDefault.hostName,
        65,
        NON_VOLATILE),

    //hostAddress
    CONSTRUCT_SIMPLE_U32(
        &ethernetStaticFrequentStaticDefault.hostAddress,
        NON_VOLATILE),

    //subnetMask
    CONSTRUCT_SIMPLE_U32(
        &ethernetStaticFrequentStaticDefault.subnetMask,
        NON_VOLATILE),

    //gatewayAddress
    CONSTRUCT_SIMPLE_U32(
        &ethernetStaticFrequentStaticDefault.gatewayAddress,
        NON_VOLATILE),

    //ntpServerAddress
    CONSTRUCT_SIMPLE_U32(
        &ethernetStaticFrequentStaticDefault.ntpServerAddress,
        NON_VOLATILE),

    //port1EthStatsHMI
    CONSTRUCT_ARRAY_SIMPLE_U32(
        ethernetDynamic.port1EthStatsHMI,
        4,
        READONLY_RAM),

    //port2EthStatsHMI
    CONSTRUCT_ARRAY_SIMPLE_U32(
        ethernetDynamic.port2EthStatsHMI,
        4,
        READONLY_RAM),

    //ipStats
    CONSTRUCT_SIMPLE_U32(
        &ethernetDynamic.ipStats,
        READONLY_RAM),

    //dnsAddress
    CONSTRUCT_ARRAY_SIMPLE_U32(
        ethernetStaticFrequentStaticDefault.dnsAddress,
        2,
        NON_VOLATILE),

    //domainName
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        ethernetStaticFrequentStaticDefault.domainName,
        65,
        NON_VOLATILE),

    //chasisMACstr
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        ethernetConstant.chasisMACstr,
        17,
        ROM),

    //port1MACstr
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        ethernetConstant.port1MACstr,
        17,
        ROM),

    //port2MACstr
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        ethernetConstant.port2MACstr,
        17,
        ROM),

    //ntpServerAddress2
    CONSTRUCT_ARRAY_SIMPLE_U32(
        ethernetDynamic.ntpServerAddress2,
        2,
        RAM),

    //portSpeedStatus
    CONSTRUCT_ARRAY_TABENUM8(
        ethernetDynamic.portSpeedStatus,
        2,
        READONLY_RAM,
        &sharedTRangeE8Descriptor2),

    //portDuplexStatus
    CONSTRUCT_ARRAY_TABENUM8(
        ethernetDynamic.portDuplexStatus,
        2,
        READONLY_RAM,
        &sharedTRangeE8Descriptor3),

    //hmiValidityTestResults
    CONSTRUCT_ARRAY_SIMPLE_U8(
        ethernetDynamic.hmiValidityTestResults,
        32,
        RAM)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the ETHERNET-Class object
const T_ETHERNET ethernet =
{
    {
        &ethernetState,
        &ethernetDataSemaphore,

        ethernetObjectList,
        //lint -e{778} 
        sizeof(ethernetObjectList)/sizeof(ethernetObjectList[0])-1,

        ethernetDataClassList,
        sizeof(ethernetDataClassList)/sizeof(ethernetDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_ETHERNET, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_ETHERNET, // will be overloaded
        SaveAsDefault_ETHERNET, // will be overloaded
        Get_ETHERNET, // will be overloaded
        Put_ETHERNET, // will be overloaded
        Check_T_UNIT,
        GetAttributeDescription_ETHERNET, // will be overloaded
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,SinkEventEXE_ETH
    ,VerifyMainProtocolEXE_ETH
    ,ControlFactoryDefaultsEXE_ETH
    ,RefreshHmiValidityTestResultsEXE_ETH
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: ethernet_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const T_ETHERNET_TEMPLATE ethernetTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
T_ETHERNET_TEMPLATE ethernetTemplate;
const T_ETHERNET_TEMPLATE ethernetTemplateInitializer=
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
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    const T_ETHERNET_TEMPLATE ethernetTemplate;
    const T_ETHERNET_TEMPLATE etnernetTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_TEMPLATE"
    const __no_init T_ETHERNET_TEMPLATE ethernetTemplate;
  #pragma location="ETHERNET_TEMPLATE_DEFAULT"
    const __no_init T_ETHERNET_TEMPLATE ethernetTemplateDefault;
#endif
const T_ETHERNET_TEMPLATE ethernetTemplateInitializer=
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
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    const T_ETHERNET_TEMPLATE ethernetTemplate;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_TEMPLATE"
    const __no_init T_ETHERNET_TEMPLATE ethernetTemplate;
#endif
const T_ETHERNET_TEMPLATE ethernetTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
ethernetTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(ethernetTemplate,ethernetTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(ethernetTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(ethernetTemplate,ethernetTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(ethernetTemplate,ethernetTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(ethernetTemplate,ethernetTemplateInitializer,ethernetTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

