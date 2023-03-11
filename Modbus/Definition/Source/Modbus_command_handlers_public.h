//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_commands_public.h
 Description    Interface to the Modbus public command handlers

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __MODBUS_COMMAND_DESCRIPTORS__
    #error "Modbus_Commands_Public.h Error - Modbus_Command_Descriptors.h must be included first."
#endif

#ifndef __COMMON_TYPE_H__
    #error "Modbus_Commands_Public.h Error - Common_Type.h must be included first."
#endif

#ifdef __MODBUS_COMMANDS_PUBLIC__
    #error "Modbus_Commands_Public.h Error - Modbus_Commands_Public.h is already included."
#endif

#define __MODBUS_COMMANDS_PUBLIC__
//#define __MODULE_TEST_MODBUS_COMMANDS_PUBLIC__


//--------------------------------------------------------------------------------------------------
/*!
 \brief   Generic handler for register based operations.
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pMessage - Reference to the buffer holding the message
 \param   [in/out] pLength - Reference to a counter holding the number of bytes added to the buffer
 \return  The modbus exception after the message has been processed
 \warning
 \test
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType RegisterHandlerModbus_Command_Handlers_Public(const TModbusCommandDescriptor
                                                                    *pModbusCommandDescriptor,
                                                                    TModbusHardwareLayer
                                                                    *pModbusHardwareLayer,
                                                                    //lint -e{793} by CNZUWAN AS code review is accetable.
                                                                    TUSIGN8 *pMessage) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Command handler for coil based commands
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pMessage - Reference to the message buffer
 \param   [in/out] pLength - Reference to a counter holding the number of bytes in/added to the buffer
 \return  The exception response code, MODBUS_EXCEPTION_TYPE_NONE if the command is processed OK
 \warning
 \test
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType CoilHandlerModbus_Command_Handlers_Public(const TModbusCommandDescriptor
                                                                *pModbusCommandDescriptor,
                                                                TModbusHardwareLayer
                                                                *pModbusHardwareLayer,
                                                                TUSIGN8 *pMessage) ;