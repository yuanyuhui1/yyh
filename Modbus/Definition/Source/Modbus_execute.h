//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MODBUS_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MODBUS_execute.h
#endif

#ifndef __MODBUS_H__
  #error  MODBUS.h must be included before MODBUS_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MODBUS_EXECUTE_H__
  #error MODBUS_execute.h included more than once
#endif

#define __MODBUS_EXECUTE_H__


//@SubGen start@<METHODS>
void UpdateScanRegister_MODBUS(void );
void SinkEventEXE_MODBUS(const void* pEvent);
void UpdateEnableStateEXE_MODBUS(TUSIGN8 enableState);
void TaskEXE_MODBUS(void);
//@SubGen end@<METHODS>