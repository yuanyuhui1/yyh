//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_Layer7.c
 Description    Application layer for the Modbus subsystem

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Command_Handlers_Private.h Error - Common_Type.h must be included first."
#endif

#ifndef __MODBUS_COMMAND_DESCRIPTOR_H__
    #error "Modbus_Command_Handlers_Private.h Error - Modbus_Command_Descriptor.h must be included first."
#endif

#ifdef __MODBUS_COMMAND_HANDLERS_PRIVATE_H__
    #error "Modbus_Command_Handlers_Private.h Error - Modbus_Command_Handlers_Private.h is already included."
#endif

#define __MODBUS_COMMAND_HANDLERS_PRIVATE_H__
//-------------------------------------------------------------------------------------------------

TModbusExceptionType PrivateHandlerModbus_Command_Handlers_Private(const TModbusCommandDescriptor
                                                                    *pModbusCommandDescriptor,
                                                                    TModbusHardwareLayer
                                                                    *pModbusHardwareLayer,
                                                                    //lint -e{793} by CNZUWAN AS code review is accetable.
                                                                    TUSIGN8 *pMessage) ;