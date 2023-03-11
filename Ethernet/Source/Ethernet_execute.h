//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Ethernet_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Ethernet_execute.h
#endif

#ifndef __ETHERNET_H__
  #error  Ethernet.h must be included before Ethernet_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ETHERNET_EXECUTE_H__
  #error Ethernet_execute.h included more than once
#endif

#define __ETHERNET_EXECUTE_H__

const TCHAR* MainProtocolIdToName(TUSIGN8 protocolId);

//@SubGen start@<METHODS>
void SinkEventEXE_ETH(const void* pEvent);
void VerifyMainProtocolEXE_ETH(void);
void ControlFactoryDefaultsEXE_ETH(TUSIGN8 controlId);
void RefreshHmiValidityTestResultsEXE_ETH(void);
//@SubGen end@<METHODS>

