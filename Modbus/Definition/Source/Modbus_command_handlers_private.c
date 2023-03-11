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
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_command_descriptor.h"
#include "Modbus/Source/Modbus_command_handlers_private.h"
//#include "..\..\modbus\source\modbus_command_descriptors.h"
#include "Modbus/Source/Modbus_commands_helper.h"
//#include "..\..\modbus\source\modbus_register_tables.h"
//#include <string.h>

// Defines for private command handlers - any buffer positions are given for a buffer
// without the slave and command id bytes
#define MODBUS_COMMAND_HANDLERS_PRIVATE_READ_REQUEST_BYTE_COUNT_LOCATION    0
#define MODBUS_COMMAND_HANDLERS_PRIVATE_READ_DATA_START_LOCATION            1
#define MODBUS_COMMAND_HANDLERS_PRIVATE_WRITE_DATA_START_LOCATION           5

// Private function declarations
//lint --e{793} by CNZUWAN AS code review is accetable.
TUSIGN16 AddCustomMessageParametersModbus_Command_Handlers_Private(const TModbusCommandDescriptor
                                                                    *pModbusCommandDescriptor,
                                                                    TUSIGN8 *pMessage,
                                                                    TUSIGN16 *pLength) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Generic handler for register based operations.
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusHardwareLayer - Reference to the message command structure
 \param   [in] pMessage - Reference to the buffer holding the message
 \return  The modbus exception after the message has been processed
 \warning
 \test
 date: 2016-10-09
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType PrivateHandlerModbus_Command_Handlers_Private(const TModbusCommandDescriptor
                                                                    *pModbusCommandDescriptor,
                                                                    TModbusHardwareLayer
                                                                    *pModbusHardwareLayer,
                                                                    TUSIGN8 *pMessage)
{
    // Details of the request
    TModbusRegisterTableRequestDetails privateHandlerRequestDetails ;
    // Pointer to the register table to be accessed
    const TModbusRegisterTable *pModbusRegisterTable = (TModbusRegisterTable *)NULL ;
    // Link to the register table via the request
    TModbusRegisterTableRequestLink privateHandlerRequestLink ;
    // Eventual exception returned by the request
    TModbusExceptionType modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS ;
    // Local operation results
    TUSIGN16 result = OK ;
    // Length of the message in bytes
    TUSIGN16 messageLength ;
    // Position where the data handler function will start to access the message buffer
    TUSIGN16 messagePosition ;

    // Check the length of the message
    if (CheckLengthModbus_Commands_Helper(pModbusCommandDescriptor,
                                          pMessage,
                                          pModbusHardwareLayer->modbusCommonParameters.length) == OK)
    {
        // Now clear the count of bytes in the message buffer
        pModbusHardwareLayer->modbusCommonParameters.length = 0 ;
        // Get the start register and requested item count from the command
        GetStartingIndexAndCountModbus_Commands_Helper(pMessage, &privateHandlerRequestDetails) ;
        // At least one item requested ?
        if (privateHandlerRequestDetails.itemCount > 0)
        {
            // It's a private request, so set the request type accordingly
            privateHandlerRequestDetails.isPrivateRequest = eTRUE ;
            // Get the register table associated with the command
            pModbusRegisterTable = pModbusCommandDescriptor->pCommandRegisterTable ;

            // Build the table link
            if (pModbusRegisterTable->GetTableLink(pModbusRegisterTable,
                                                    &privateHandlerRequestDetails,
                                                    &privateHandlerRequestLink) == OK)
            {
                // Are any object addresses overlapping ?
                if (pModbusRegisterTable->CheckForOverlap(pModbusRegisterTable,
                                                            &privateHandlerRequestDetails,
                                                            &privateHandlerRequestLink) == OK)
                {
                    // Will the message fit in the reply buffer ?
                    messageLength = pModbusRegisterTable->GetLength(pModbusRegisterTable,
                                                            &privateHandlerRequestLink,
                                                            eTRUE) ;
                    if (messageLength >= MODBUS_MAX_DATA_SIZE)
                    {
                        result = FATAL_ERROR ;
                    }
                    if (result == OK)
                    {
                        // Read command ?
                        if (pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_READ)
                        {
                            messagePosition = MODBUS_COMMAND_HANDLERS_PRIVATE_READ_DATA_START_LOCATION ;
                        }
                        else
                        {
                            messagePosition = MODBUS_COMMAND_HANDLERS_PRIVATE_WRITE_DATA_START_LOCATION ;
                        }

                        // Call the data handler for the command
                        result = pModbusCommandDescriptor->pModbusDataManagementFunction(pModbusRegisterTable,
                                                                                        pModbusHardwareLayer,
                                                                                        &privateHandlerRequestDetails,
                                                                                        &privateHandlerRequestLink,
                                                                                        &pMessage[messagePosition]) ;
                        // Check if any exceptions occured
                        modbusExceptionType = TranslateErrorCodeToExceptionModbus_Commands_Helper(result) ;
                        // No exception ?
                        if (modbusExceptionType == MODBUS_EXCEPTION_TYPE_NONE)
                        {
                            // No, so we can add the remaining items to the message buffer
                            (void)AddCustomMessageParametersModbus_Command_Handlers_Private(pModbusCommandDescriptor,
                                                                                      pMessage,
                                                                                      &pModbusHardwareLayer->
                                                                                      modbusCommonParameters.
                                                                                      length) ;
                        }
                    }
                }
            }
        }
    }
    return(modbusExceptionType) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   String data type read handler function - string data differs from other object types
 \brief   in that the size of each string object in the register table can be different. For this
 \brief   reason, the initial string object index has to be calculated by summing the length of
 \brief   all of the previous strings. Only if this is valid will the string objects be read
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pMessage - Reference to the buffer to read data into
 \param   [out] pLength - Reference to a counter holding the number of bytes added to the buffer
 \return  OK if the message is processed OK
 \warning
 \test
 date: 2016-10-09
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 AddCustomMessageParametersModbus_Command_Handlers_Private(const TModbusCommandDescriptor
                                                                    *pModbusCommandDescriptor,
                                                                    TUSIGN8 *pMessage,
                                                                    TUSIGN16 *pLength)
{
    TUSIGN16 result = OK ;

    // Read command ?
    if (pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_READ)
    {
        // Read commands add the number of data bytes being sent in addition to the read data
        //lint -e{734} by CNZUWAN . Accetable.
        pMessage[MODBUS_COMMAND_HANDLERS_PRIVATE_READ_REQUEST_BYTE_COUNT_LOCATION] = *pLength ;
        // Another byte added
        (*pLength)++ ;
    }
    else
    {
        // Write commands always follow the same response format. They echo back the write locations
        // and variable count. This is already in the buffer, so just the size is needed
        *pLength = 4 ;
    }
    return(result) ;
}