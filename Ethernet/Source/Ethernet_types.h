//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Ethernet_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Ethernet_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ETHERNET_TYPES_H__
  #error Ethernet_types.h included more than once
#endif

#define __ETHERNET_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 selectedMainProtocol;
    TUSIGN8 enableModbusTcp;
    TUSIGN8 enableOpcUa;
    TUSIGN8 enableEthernetIp;
    TUSIGN8 enableProfinetIo;
    TUSIGN8 enableWebserver;
    TUSIGN8 port1AutoNegotiate;
    TUSIGN8 port1Speed;
    TUSIGN8 port1DuplexMode;
    TUSIGN8 port2AutoNegotiate;
    TUSIGN8 port2Speed;
    TUSIGN8 port2DuplexMode;
    TUSIGN8 useDhcp;
    TCHAR hostName[65];
    TUSIGN32 hostAddress;
    TUSIGN32 subnetMask;
    TUSIGN32 gatewayAddress;
    TUSIGN32 ntpServerAddress;
    TUSIGN32 dnsAddress[2];
    TCHAR domainName[65];
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_ETHERNET_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC> 
    TUSIGN8 chasisMAC[6];
    TUSIGN8 port1MAC[6];
    TUSIGN8 port2MAC[6];
    TUSIGN32 port1EthStatsHMI[4];
    TUSIGN32 port2EthStatsHMI[4];
    TUSIGN32 ipStats;
    TUSIGN32 ntpServerAddress2[2];
    TUSIGN8 portSpeedStatus[2];
    TUSIGN8 portDuplexStatus[2];
    TUSIGN8 hmiValidityTestResults[32];
     //@SubGen end@    <DATACLASS DYNAMIC> 
 } T_ETHERNET_DYNAMIC;


 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_ETHERNET_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TCHAR chasisMACstr[17];
    TCHAR port1MACstr[17];
    TCHAR port2MACstr[17];
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_ETHERNET_CONSTANT;












/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_ETHERNET_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_ETHERNET_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

