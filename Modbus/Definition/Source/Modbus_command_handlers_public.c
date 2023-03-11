//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         Subsystem MODBUS
 Module         MODBUS_commands_public.c
 Description    Implementations of public Modbus commands
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_tables.h"
#include "Modbus/Source/Modbus_command_descriptor.h"
#include "Modbus/Source/Modbus_command_descriptors.h"
#include "Modbus/Source/Modbus_formatting.h"
#include "Modbus/Source/Modbus_commands_helper.h"
#include "Modbus/Source/Modbus_command_handlers_public.h"
//#include "..\..\modbus\source\modbus_slave_id.h"

// Note any of the following for message buffer locations are from the start of a message
// buffer with the slave and command ids removed.

// Buffer positions for register based commands
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_REGISTER_DATA_START     2
#define MODBUS_COMMAND_HANDLERS_PUBLIC_READ_DATA_START                      1
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_MULTIPLE_REGISTER_BYTE_START   4
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_MULTIPLE_REGISTER_DATA_START   5

// Definitions for the Diagnostics command
#define MODBUS_COMMANDS_PUBLIC_DIAGNOSTICS_SUB_COMMAND_LOOPBACK             0
#define MODBUS_COMMANDS_PUBLIC_DIAGNOSTICS_SUB_COMMAND_ID_LOCATION          0

// Defines common to coil and register commands
#define MODBUS_COMMANDS_READ_REQUEST_BYTE_COUNT_LOCATION                    0

// Read and write multiple coil based definitions
// Maximum number of coils that can be read at once
#define MODBUS_COMMAND_HANDLERS_PUBLIC_MAX_COILS_PER_READ_REQUEST           2000
// Maximum number of coils that can be written at once
#define MODBUS_COMMAND_HANDLERS_PUBLIC_MAX_COILS_PER_WRITE_REQUEST          1968

// Buffer positions for coil based commands
#define MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_READ_DATA_START                 1
#define MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_WRITE_DATA_START                5

// Defines for the write single coil command
// Value to set a coil for the write single coil command
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_SET                0xFF00
// Value to clear a coil for the single write coil command
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_CLEAR              0x0

// Value written to the buffer to set a single coil
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_SET_BUFFER_VALUE   0x1
// Value written to the buffer to clear a single coil
#define MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_CLEAR_BUFFER_VALUE 0x0

// Slave id buffer positions
#define MODBUS_COMMAND_HANDLERS_PUBLIC_SLAVE_ID_BYTE_COUNT_POSITION         0
#define MODBUS_COMMAND_HANDLERS_PUBLIC_SLAVE_ID_DATA_POSITION               1

// Private function declarations
//lint --e{793} by CNZUWAN AS code review is accetable.
void AddRegisterMessageParametersModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                                TUSIGN8 *pMessage, TUSIGN16 *pLength);

void AddCoilMessageParametersModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                            TUSIGN8 *pMessage, TUSIGN16 *pLength);

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Generic handler for register based operations.
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusHardwareLayer - Reference to the hardware Layer structure
 \param   [in] pMessage - Reference to the buffer holding the message
 \return  The modbus exception after the message has been processed
 \warning
 \test
 date: 2016-09-28
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType RegisterHandlerModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                                   TModbusHardwareLayer *pModbusHardwareLayer,
                                                                   TUSIGN8 *pMessage)
{
    // Exception type for the request result
    TModbusExceptionType modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS;
    // Local operation results
    TUSIGN16 result = OK;
    // Register table where the requested items reside
    const TModbusRegisterTable *pModbusRegisterTable = (const TModbusRegisterTable *)NULL;
    // Details of the register request
    TModbusRegisterTableRequestDetails registerHandlerRequestDetails;
    // Location in the message buffer to start data operations from
    TUSIGN8 messageIndex;
    // The table link for the request
    TModbusRegisterTableRequestLink registerHandlerRequestLink;
    // Size of the message in registers
    TUSIGN16 messageSize;
    // Buffer for single register writes
    // TUSIGN8 singleWriteBuffer[MODBUS_BYTES_PER_REGISTER] = {0};
    TUSIGN8 ResponseBuffer[MODBUS_MAX_DATA_SIZE] = {0};
    // Pointer to the message buffer - if it's a single register write, we use the single write buffer
    // so as to not interfere with the response (which is already in the receive message buffer)
    // TUSIGN8 *pMessageBuffer = (TUSIGN8 *)NULL;

    // Check the length of the command frame
    if (CheckLengthModbus_Commands_Helper(pModbusCommandDescriptor, pMessage,
                                          pModbusHardwareLayer->modbusCommonParameters.length) == OK)
    {
        // Clear the count of bytes in the buffer
        pModbusHardwareLayer->modbusCommonParameters.length = 0;
        // Get the start and end registers requested
        GetStartingIndexAndCountModbus_Commands_Helper(pMessage, &registerHandlerRequestDetails);

        // Check if it's a read or write command - the start point in the buffer varies depending on this
        if (pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_READ)
        {
            messageIndex = MODBUS_COMMAND_HANDLERS_PUBLIC_READ_DATA_START;
        }
        else
        {
            if (pModbusCommandDescriptor->commandId == MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER)
            {
                messageIndex = MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_REGISTER_DATA_START;
                // Add any request details specific to this request type
                registerHandlerRequestDetails.itemCount = 1;
                // Copy the data to be written to the single register
                (void)memcpy((void*)ResponseBuffer,
                            (void*)&pMessage[MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_REGISTER_DATA_START],
                            MODBUS_BYTES_PER_REGISTER);
            }
            else
            {
                messageIndex = MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_MULTIPLE_REGISTER_DATA_START;
                (void)memcpy((void*)ResponseBuffer,
                        (void*)&pMessage[MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_MULTIPLE_REGISTER_DATA_START],
                        pMessage[MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_MULTIPLE_REGISTER_BYTE_START]);
            }
        }

        // At least one item requested ?
        if (registerHandlerRequestDetails.itemCount > 0)
        {
            // Add the rest of the request details for use later
            registerHandlerRequestDetails.isPrivateRequest = eFALSE;
            // Get the register table where the requested objects reside
            pModbusRegisterTable = GetRegisterTableModbus_Register_Tables(registerHandlerRequestDetails.startRegisterAddress,
                                                                          pModbusCommandDescriptor->modbusRegisterTableLocation) ;
            // Unknown data address ?
            if (pModbusRegisterTable != NULL)
            {
                // Build the table link
                if (pModbusRegisterTable->GetTableLink(pModbusRegisterTable,
                                                       &registerHandlerRequestDetails,
                                                       &registerHandlerRequestLink) == OK)
                {
                    // Any overlapping items ?
                    if (pModbusRegisterTable->CheckForOverlap(pModbusRegisterTable,
                                                              &registerHandlerRequestDetails,
                                                              &registerHandlerRequestLink) == OK)
                    {
                        // Is this a write command ?
                        if (pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_WRITE)
                        {
                            // Get the size of the message in registers
                            messageSize = pModbusRegisterTable->GetLength(pModbusRegisterTable,
                                                                          &registerHandlerRequestLink,
                                                                          eFALSE);
                        }
                        else
                        {
                            messageSize = registerHandlerRequestDetails.itemCount;
                        }
                        // Has the maximum number of registers been exceeded ?
                        if (messageSize > (MODBUS_MAX_DATA_SIZE / 2))
                        {
                            result = FATAL_ERROR;
                        }
                        // Ok to proceed ?
                        if (result == OK)
                        {
                            // Then call the data handler
                            result = pModbusCommandDescriptor->pModbusDataManagementFunction(pModbusRegisterTable,
                                                                                             pModbusHardwareLayer,
                                                                                             &registerHandlerRequestDetails,
                                                                                             &registerHandlerRequestLink,
                                                                                             &ResponseBuffer[0]);
                            // Translate the Framework based result into an exception code
                            modbusExceptionType = TranslateErrorCodeToExceptionModbus_Commands_Helper(result);
                            // Any exception ?
                            if (modbusExceptionType == MODBUS_EXCEPTION_TYPE_NONE)
                            {
                                (void)memcpy((void*)&pMessage[messageIndex], (void*)ResponseBuffer,
                                             pModbusHardwareLayer->modbusCommonParameters.length);

                                // Add the appropriate message parameters
                                AddRegisterMessageParametersModbus_Command_Handlers_Public(pModbusCommandDescriptor,
                                                                                           pMessage,
                                                                                           &pModbusHardwareLayer->modbusCommonParameters.length);
                            }
                        }
                    }
                }
            }
        }
    }
    return(modbusExceptionType);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Command handler for coil based commands
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusHardwareLayer - Reference to the hardware Layer structure
 \param   [in] pMessage - Reference to the buffer holding the message
 \return  The exception response code, MODBUS_EXCEPTION_TYPE_NONE if the command is processed OK
 \warning
 \test
 date: 2016-09-28
 \n by: ZuoChen.wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType CoilHandlerModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                               TModbusHardwareLayer *pModbusHardwareLayer,
                                                               TUSIGN8 *pMessage)
{
    TModbusExceptionType modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS;
    // Register table where coils to be accessed reside
    const TModbusRegisterTable *pModbusRegisterTable = (const TModbusRegisterTable *)NULL;
    // Framework based return code from data management function
    TUSIGN16 result = OK;
    // Details of the request
    TModbusRegisterTableRequestDetails coilHandlerRequestDetails;
    // The link to the register table for the request
    TModbusRegisterTableRequestLink coilHandlerRequestLink;
    // Location where data will be accessed from
    TUSIGN16 messageIndex;

    // Check the command length
    if (CheckLengthModbus_Commands_Helper(pModbusCommandDescriptor, pMessage,
                                          pModbusHardwareLayer->modbusCommonParameters.length) == OK)
    {
        // Clear the count of bytes in the message buffer
        pModbusHardwareLayer->modbusCommonParameters.length = 0;
        // Get the start coil and count of coils
        GetStartingIndexAndCountModbus_Commands_Helper(pMessage, &coilHandlerRequestDetails);
        // Do we need to modify the request details ?
        if (pModbusCommandDescriptor->commandId == MODBUS_COMMAND_ID_WRITE_SINGLE_COIL)
        {
            // For the write single coil request, a value of 0xFF00 is sent to set the coil,
            // and 0x0 to clear it
            if (coilHandlerRequestDetails.itemCount == MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_SET)
            {
                // Set the value to be written in the appropriate buffer position. We'll still
                // use the generic handler for coil access for the single coil command
                pMessage[MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_WRITE_DATA_START] = 
                MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_SET_BUFFER_VALUE;
            }
            else if (coilHandlerRequestDetails.itemCount == MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_CLEAR)
            {
                pMessage[MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_WRITE_DATA_START] =
                MODBUS_COMMAND_HANDLERS_PUBLIC_WRITE_SINGLE_COIL_CLEAR_BUFFER_VALUE;
            }
            else
            {
                // Bad data value for write single coil command
                modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_VALUE;
                result = FATAL_ERROR;
            }
            // Only one coil being set in this case
            coilHandlerRequestDetails.itemCount = 1;
        }
        // At least one coil requested ?
        if (coilHandlerRequestDetails.itemCount > 0)
        {
            coilHandlerRequestDetails.isPrivateRequest = eFALSE;
            // Ok to proceed ?
            if (result == OK)
            {
                // Get the coil table for this request
                pModbusRegisterTable = GetRegisterTableModbus_Register_Tables(coilHandlerRequestDetails.
                                                                              startRegisterAddress,
                                                                              pModbusCommandDescriptor->modbusRegisterTableLocation);
                // Unknown coil address ?
                if (pModbusRegisterTable != NULL)
                {
                    // Build the table link
                    if (pModbusRegisterTable->GetTableLink(pModbusRegisterTable,
                                                           &coilHandlerRequestDetails,
                                                           &coilHandlerRequestLink) == OK)
                    {
                        // Any overlap ?
                        if (pModbusRegisterTable->CheckForOverlap(pModbusRegisterTable,
                                                                  &coilHandlerRequestDetails,
                                                                  &coilHandlerRequestLink) == OK)
                        {
                            // Is it a read command ?
                            if (pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_READ)
                            {
                                messageIndex = MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_READ_DATA_START;
                                // More than the possible number of coils requested ?
                                if (coilHandlerRequestDetails.itemCount >
                                    MODBUS_COMMAND_HANDLERS_PUBLIC_MAX_COILS_PER_READ_REQUEST)
                                {
                                    result = FATAL_ERROR;
                                }
                            }
                            else
                            {
                                messageIndex = MODBUS_COMMAND_HANDLERS_PUBLIC_COIL_WRITE_DATA_START;
                                // More than the allowable number of coils requested for writing ?
                                if (coilHandlerRequestDetails.itemCount >
                                    MODBUS_COMMAND_HANDLERS_PUBLIC_MAX_COILS_PER_WRITE_REQUEST)
                                {
                                    result = FATAL_ERROR;
                                }
                            }
                            // OK to proceed ?
                            if (result == OK)
                            {
                                // Call the data handling function
                                result = pModbusCommandDescriptor->pModbusDataManagementFunction(pModbusRegisterTable,
                                                                                                 pModbusHardwareLayer,
                                                                                                 &coilHandlerRequestDetails,
                                                                                                 &coilHandlerRequestLink,
                                                                                                 &pMessage[messageIndex]);
                                // Convert the framework based code to an exception code
                                modbusExceptionType = TranslateErrorCodeToExceptionModbus_Commands_Helper(result);
                                // Message processed OK ?
                                if (modbusExceptionType == MODBUS_EXCEPTION_TYPE_NONE)
                                {
                                    // Yes, so now we can add the rest of the response
                                    AddCoilMessageParametersModbus_Command_Handlers_Public(pModbusCommandDescriptor,
                                                                                           pMessage,
                                                                                           &pModbusHardwareLayer->
                                                                                           modbusCommonParameters.length);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return(modbusExceptionType);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Adds message parameters to register based Modbus messages
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the Modbus command descriptor
 \param   [in] pMessage - Reference to the message buffer
 \param   [in,Out] pLength - Reference to a counter holding the number of bytes added to the buffer
 \return  void
 \warning
 \test
 date: 2016-09-28
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void AddRegisterMessageParametersModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                                TUSIGN8 *pMessage, TUSIGN16 *pLength)
{
    switch(pModbusCommandDescriptor->commandId)
    {
        case(MODBUS_COMMAND_ID_READ_HOLDING_REGISTERS):
        case(MODBUS_COMMAND_ID_READ_INPUT_REGISTERS):

            // Also add the number of bytes that are in the application level response -
            // this consists of an echo of the requested start register and count
            //lint -e{734} by CNZUWAN AS code review is accetable.
            pMessage[MODBUS_COMMANDS_READ_REQUEST_BYTE_COUNT_LOCATION] = *pLength;
            // And another byte has been added to the message
            (*pLength)++;
        break;
        case(MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER):
        case(MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS):
            // For these commands, the reply is an echo of the request, so it's already in the buffer.
            // We just need to set the response length, which is the same for both requests
            *pLength = 4;
        break;
        default:
        break;
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Adds non-data object based parameters to a coil based response message
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the Modbus command descriptor
 \param   [in] pMessage - Reference to the message buffer
 \param   [in] pLength - Reference to the count of bytes in the response buffer
 \return  void
 \warning
 \test
 date: 2016-09-28
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void AddCoilMessageParametersModbus_Command_Handlers_Public(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                                            TUSIGN8 *pMessage, TUSIGN16 *pLength)
{
    switch(pModbusCommandDescriptor->commandId)
    {
        case(MODBUS_COMMAND_ID_READ_COILS):
        case(MODBUS_COMMAND_ID_READ_DISCRETE_INPUTS):
            // Add the number of bytes that are in the application level response
            //lint -e{734} by CNZUWAN AS code review is accetable.
            pMessage[MODBUS_COMMANDS_READ_REQUEST_BYTE_COUNT_LOCATION] = *pLength;
            // And another byte has been added to the message
            (*pLength)++;
        break;
        case(MODBUS_COMMAND_ID_WRITE_SINGLE_COIL):
        case(MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS):
            // For these commands, the reply is an echo of the request, so it's already in the buffer.
            // We just need to set the response length, which is the same for both requests
            *pLength = 4;
        break;
        default:
        break;
    }
}