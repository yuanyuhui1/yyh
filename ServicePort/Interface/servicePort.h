//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ServicePort
 Module
 Description    Class definition T_UNIT::SERVICE_PORT

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ServicePort.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ServicePort.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before ServicePort.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __SERVICE_PORT_H__
  #error ServicePort.h included more than once
#endif

#define __SERVICE_PORT_H__

#define SERVICE_PORT_VERSION (0x010203)   // Version 1.2.2  change to 1.2.3 don't support changing baud rate now.  wuhao 2012-12-03

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // Service Port HMI Status
  #define SERVICEPORT_HMI_STATUS 3 //For Object ActiveStatus. Service Port is disbale, HMI is enable.

  // Service Port SmartHMI Status
  #define SERVICEPORT_SHMI_STATUS 2 //For Object ActiveStatus. Service port is disbale, Smart HMI is enbale.

  // Service Port Service Status
  #define SERVICEPORT_SERVICE_STATUS 1 //For Object ActiveStatus.Service port is enabled. HMI/SmartHMI is disable

  // Service Port Uninitialized Status
  #define SERVICEPORT_UNINITIALIZED_STATUS 0 //For Object ActiveStatus.Service port is disabled.HMI/SmartHMI is disable.

  // Service Port Tx Interrupt Priority level
  #define SERVICEPORT_TX_INT_PRIORITY 3 //

  // Service Port Rx Interrupt Priority level
  #define SERVICEPORT_RX_INT_PRIORITY 3 //

  // BoardsTable
  #define SERVICEPORT_CB_BOARD 0 //
  #define SERVICEPORT_FE_BOARD 1 //
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_SERVICE_PORT_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    SERVICEPORT_DC_IDX_DYNAMIC,
    SERVICEPORT_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_SERVICE_PORT
typedef struct _T_SERVICE_PORT
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* ServPortCommandInterpreterEXE)(void);
    void (* ServPortUpdateActivationStatusSRV)(void);
    TUSIGN8 (* ServPortGetActivationStatusSRV)(void);
    void (* ServPortRedetectHMISRV)(TBOOL isSmartHMI);
    //@SubGen end@    <METHODS>
} T_SERVICE_PORT;


//-------------------------------------------------------------------------------------------------
//! define the T_SERVICE_PORT-object
extern SLOW const T_SERVICE_PORT servicePort;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_SERVICE_PORT
#define SERVICE_PORT_ME (&servicePort.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define servicePort_Get(objectIndex,attributeIndex,ptrValue) \
   servicePort.unit.Get(SERVICE_PORT_ME,objectIndex,attributeIndex,ptrValue)
#define servicePort_Put(objectIndex,attributeIndex,ptrValue) \
   servicePort.unit.Put(SERVICE_PORT_ME,objectIndex,attributeIndex,ptrValue)
#define servicePort_Check(objectIndex,attributeIndex,ptrValue) \
   servicePort.unit.Check(SERVICE_PORT_ME,objectIndex,attributeIndex,ptrValue)

#define servicePort_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   servicePort.unit.GetAttributeDescription(SERVICE_PORT_ME,objectIndex,attributeIndex,ptrDescriptor)
#define servicePort_GetObjectDescription(objectIndex,ptrDescriptor) \
   servicePort.unit.GetObjectDescription(SERVICE_PORT_ME,objectIndex,ptrDescriptor)

