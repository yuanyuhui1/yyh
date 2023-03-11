//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ServicePort
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ServicePort_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ServicePort_execute.h
#endif

#ifndef __SERVICE_PORT_H__
  #error  ServicePort.h must be included before ServicePort_execute.h
#endif

//............................................
//!  extend include list if neccessary



#ifdef __SERVICE_PORT_EXECUTE_H__
  #error ServicePort_execute.h included more than once
#endif

#define __SERVICE_PORT_EXECUTE_H__

//@SubGen start@<METHODS>
void ServPortCommandInterpreterEXE_SERVICEPORT(void);
void ServPortUpdateActivationStatusSRV_SERVICEPORT(void);
TUSIGN8 ServPortGetActivationStatusSRV_SERVICEPORT(void);
void ServPortRedetectHMISRV_SERVICEPORT(TBOOL isSmartHMI);
//@SubGen end@<METHODS>


//test function
#ifdef SERVICEPORT_SUBSYSTEM_TEST

extern volatile TUSIGN8 Auto_Test_ServicePort;
void Transmit_String(TUSIGN8* str, TINT16 len);
void Test_Initialize_LAYER2_2(void);
#endif

