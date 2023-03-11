//-------------------------------------------------------------------------------------------------
/*
<pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         ModbusGen
 Module         Modbus_Command_Descriptor.h
 Description    Definition of the Modbus Command Decriptor type.
 Remarks        Command descriptors are used to link command handler and (if necessary)
                data handler functions to a command id. For private commands, a reference to
                the register table on which the command acts is embedded in the command descriptor.
                For public commands, the range of registers/coils requested, and the location of
                the register table are used to determine what register table to access.

                Certain public commands, like the read slave id request have only a handler
                function as no data manipulation is required.
</pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Command_Descriptor.h error - Common_Type.h must be included first."
#endif

#ifndef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Command_Descriptor.h error - Modbus_Register_Table.h must be included first."
#endif

#ifndef __MODBUS_H__
    #error "Modbus_Command_Descriptor.h Error - Modbus.h must be included first."
#endif

#ifdef __MODBUS_COMMAND_DESCRIPTOR_H__
    #error "Modbus_Command_Descriptor.h error - Modbus_Command_Descriptor.h is already included."
#endif

#define __MODBUS_COMMAND_DESCRIPTOR_H__
//-------------------------------------------------------------------------------------------------

// Command types used for Modbus commands
typedef enum _TModbusCommandType
{
    // Register based command
    MODBUS_COMMAND_TYPE_REGISTER = 0,
    // Coil based command
    MODBUS_COMMAND_TYPE_COIL,
    // Private command
    MODBUS_COMMAND_TYPE_PRIVATE,
    // Special command handlers like comms loopback
    MODBUS_COMMAND_TYPE_SPECIAL,
    // Unrecognised command
    MODBUS_COMMAND_TYPE_UNKNOWN,
    // Count of available command types
    MODBUS_COMMAND_TYPE_COUNT
}TModbusCommandType;

// Function performed by Modbus command
typedef enum _TModbusFunctionType
{
    MODBUS_FUNCTION_TYPE_READ = 0,
    MODBUS_FUNCTION_TYPE_WRITE,
    MODBUS_FUNCTION_TYPE_SPECIAL,
    MODBUS_FUNCTION_TYPE_COUNT
}TModbusFunctionType;

// Exceptions returned for Modbus requests
typedef enum _TModbusExceptionType
{
    MODBUS_EXCEPTION_TYPE_NONE = 0,
    MODBUS_EXCEPTION_TYPE_ILLEGAL_FUNCTION = 1,
    MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS = 2,
    MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_VALUE = 3,
    MODBUS_EXCEPTION_TYPE_SLAVE_DEVICE_FAILURE = 4,
    MODBUS_EXCEPTION_TYPE_GATEWAY_PATH_UNAVAILABLE = 10,
    MODBUS_EXCEPTION_TYPE_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 11,
    MODBUS_EXCEPTION_TYPE_COUNT = 8
}TModbusExceptionType;

// The command id of the command descriptor
typedef TUSIGN8 TModbusCommandId;

// Type used to define the command table for use by layer 7
typedef struct _TModbusCommandDescriptor
{
    // Id of the command being described
    TModbusCommandId commandId;
    // The command type
    TModbusCommandType commandType;
    // Function performed by the command
    TModbusFunctionType functionType;
    // Register table accessed by the command
    const TModbusRegisterTable *pCommandRegisterTable;
    // Location of the table associated with the command
    TModbusRegisterTableLocation modbusRegisterTableLocation;
    // Command descriptor handler function
    TModbusExceptionType (*pModbusHandlerFunction)(const struct _TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                   TModbusHardwareLayer *pModbusHardwareLayer, TUSIGN8 *pMessage);
    // Command descriptor data management function
    TUSIGN16 (*pModbusDataManagementFunction)(const TModbusRegisterTable *pModbusRegisterTable,
                                              TModbusHardwareLayer *pModbusHardwareLayer,
                                              const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                                              const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                              TUSIGN8 *pMessage);
}TModbusCommandDescriptor;