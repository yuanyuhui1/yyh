//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MODBUS_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MODBUS_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MODBUS_TYPES_H__
  #error MODBUS_types.h included more than once
#endif

#define __MODBUS_TYPES_H__


// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_NO_DEFAULT block
 typedef struct _T_MODBUS_STATIC_FREQUENT_NO_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_NO_DEFAULT> 
    TUSIGN8 modbusIEEEFormat;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_NO_DEFAULT> 
 } T_MODBUS_STATIC_FREQUENT_NO_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CONSTANT_DEFAULT block
 typedef struct _T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
    TUSIGN16 configScanRegister1[32];
    TUSIGN16 configScanRegister2[32];
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
 } T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT;



 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_MODBUS_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TUSIGN16 networkPort;
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_MODBUS_CONSTANT;












 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_MODBUS_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC> 
    T_MODBUS_DIAG mbDiag;
    T_MODBUS_DIAG_HIS mbDiagHis;
    TUSIGN16 ScanRegister1[32];
    TUSIGN16 ScanRegister2[32];
    TUSIGN32 manufactuerDeviceID;
    TUSIGN32 sensorSerialID;
    TUSIGN32 slaveID;
    TUSIGN16 mDiagCondenseStatus[3];
    TUSIGN8 writeFunctionsEnabled;
     //@SubGen end@    <DATACLASS DYNAMIC> 
 } T_MODBUS_DYNAMIC;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_MODBUS_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_MODBUS_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

