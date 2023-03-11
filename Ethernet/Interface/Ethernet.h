//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Class definition T_UNIT::ETHERNET

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Ethernet.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Ethernet.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Ethernet.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __ETHERNET_H__
  #error Ethernet.h included more than once
#endif

#define __ETHERNET_H__

#define ETHERNET_VERSION 0x010000   // Version 1.0.0

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // E_SelectedMainProtocol
  #define ETH_INVALID_PROTOCOL 0 //Invalid protocol
  #define ETH_ETHERNETIP_PROTOCOL 1 //EtherNet/IP is selected
  #define ETH_PROFINETIO_PROTOCOL 2 //PROFINET IO is selected

  // E_AutoNegotiate
  #define ETH_AUTO_NEGOTIATE_UNKNOWN 0 //Link Auto-Negotiation is unknown
  #define ETH_AUTO_NEGOTIATE_DISABLED 1 //Link Auto-Negotiation is disabled
  #define ETH_AUTO_NEGOTIATE_ENABLED 2 //Link Auto-Negotiation is enabled

  // E_PortSpeed
  #define ETH_PORT_SPEED_UNKNOWN 0 //Speed on posrt is unknown
  #define ETH_PORT_SPEED_10 1 //Speed on port of up to 10 Mbits/s
  #define ETH_PORT_SPEED_100 2 //Speed on port of up to 100 Mbits/s
  #define ETH_PORT_SPEED_1000 3 //Speed on port of up to 1000 Mbits/s

  // E_DuplexMode
  #define ETH_UNKNOWN_DUPLEX 0 //Unknown duplex
  #define ETH_HALF_DUPLEX 1 //Half duplex
  #define ETH_FULL_DUPLEX 2 //Full duplex

  // E_UseDhcp
  #define ETH_DHCP_STATE_DISABLED 0 //DHCP is disabled and static IP settings are used
  #define ETH_DHCP_STATE_ENABLED 1 //DHCP is enabled

  // E_AccessStateFull
  #define ETH_FULL_ACCESS_INVALID 0 //Access is undefined
  #define ETH_FULL_ACCESS_DISABLED 1 //Access is disabled
  #define ETH_FULL_ACCESS_UNSECURED_READ_ONLY 2 //Access is unsecured with read only permission
  #define ETH_FULL_ACCESS_UNSECURED_READ_WRITE 3 //Access is unsecured with read and write permission
  #define ETH_FULL_ACCESS_SECURED_READ_ONLY 4 //Access is secured with read only permission
  #define ETH_FULL_ACCESS_SECURED_READ_WRITE 5 //Access is secured with read and write permission

  // E_AccessStateUnsecured
  #define ETH_UNSECURED_ACCESS_INVALID 0 //Access is undefined
  #define ETH_UNSECURED_ACCESS_DISABLED 1 //Access is disabled
  #define ETH_UNSECURED_ACCESS_UNSECURED_READ_ONLY 2 //Access is unsecured with read only permission
  #define ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE 3 //Access is unsecured with read and write permission

  // E_AccessStateSecured
  #define ETH_SECURED_ACCESS_INVALID 0 //Access is undefined
  #define ETH_SECURED_ACCESS_DISABLED 1 //Access is disabled
  #define ETH_SECURED_ACCESS_SECURED_READ_ONLY 4 //Access is secured with read only permission
  #define ETH_SECURED_ACCESS_SECURED_READ_WRITE 5 //Access is secured with read and write permission
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_ETHERNET_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    ETHERNET_DC_IDX_CONSTANT,
    ETHERNET_DC_IDX_DYNAMIC,
    ETHERNET_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT,
    ETHERNET_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_ETHERNET
typedef struct _T_ETHERNET
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* SinkEventEXE)(const void* pEvent);
    void (* VerifyMainProtocolEXE)(void);
    void (* ControlFactoryDefaultsEXE)(TUSIGN8 controlId);
    void (* RefreshHmiValidityTestResultsEXE)(void);
    //@SubGen end@    <METHODS>
} T_ETHERNET;


//-------------------------------------------------------------------------------------------------
//! define the T_ETHERNET-object
extern SLOW const T_ETHERNET ethernet;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_ETHERNET
#define ETHERNET_ME (&ethernet.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define ethernet_Get(objectIndex,attributeIndex,ptrValue) \
   ethernet.unit.Get(ETHERNET_ME,objectIndex,attributeIndex,ptrValue)
#define ethernet_Put(objectIndex,attributeIndex,ptrValue) \
   ethernet.unit.Put(ETHERNET_ME,objectIndex,attributeIndex,ptrValue)
#define ethernet_Check(objectIndex,attributeIndex,ptrValue) \
   ethernet.unit.Check(ETHERNET_ME,objectIndex,attributeIndex,ptrValue)

#define ethernet_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   ethernet.unit.GetAttributeDescription(ETHERNET_ME,objectIndex,attributeIndex,ptrDescriptor)
#define ethernet_GetObjectDescription(objectIndex,ptrDescriptor) \
   ethernet.unit.GetObjectDescription(ETHERNET_ME,objectIndex,ptrDescriptor)

