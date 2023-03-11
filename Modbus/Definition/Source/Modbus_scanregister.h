//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module
 Description    Prototypes of overloaded methods of _MODBUS_scanregister
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MODBUS_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MODBUS_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before MODBUS_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MODBUS_SCANREGISTER_H__
  #error MODBUS_scanregister.h included more than once
#endif

#define __MODBUS_SCANREGISTER_H__



typedef struct 
{
   T_UNIT const SLOW*  subsystemPointer;
   TUSIGN16 objectId;
   TUSIGN16 itemDetails;
   TUSIGN8 dataType;
} TconfigScanRegisterIntern;

extern TconfigScanRegisterIntern  configScanRegisterIntern1[32];
extern TconfigScanRegisterIntern  configScanRegisterIntern2[32];
extern TUSIGN16 CheckModbusAddress(TUSIGN16 modbusRegister);
extern void TranslateConfigScanReg2Intern(const TUSIGN16* pConfigScanRegister, TconfigScanRegisterIntern* pConfigScanRegisterIntern);
extern void UpdateScanRegisterViaTable(const TconfigScanRegisterIntern*  pConfReg, TUSIGN16* pScanRegister);
extern void UpdateScanRegister(TUSIGN16* pConfigScanRegister, TUSIGN16* pScanRegister);