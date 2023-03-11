//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_commands_helper.c
 Description    Helper functions used by the public and private command handlers

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_command_descriptor.h"
// Required for accessing command ids defined in the Modbus code generator
#include "Modbus/Source/Modbus_command_descriptors.h"
// For reading from and writing to Modbus message buffers
#include "Modbus/Source/Modbus_formatting.h"
// Module include
#include "Modbus/Source/Modbus_commands_helper.h"

// Private definitions - any buffer positions are given for a message buffer without the
// slave device id and the command id
#define MODBUS_COMMANDS_START_REGISTER_LOCATION                 0
#define MODBUS_COMMANDS_REGISTER_COUNT_LOCATION                 2
// Number of non-data bytes in a read request - start register, register count
#define MODBUS_COMMANDS_READ_REQUEST_SIZE                       4
// Number of non-data bytes in a write request - start register, register count, data byte count
#define MODBUS_COMMANDS_WRITE_REQUEST_SIZE                      5
// Location of data byte count in write requests
#define MODBUS_COMMANDS_WRITE_REQUEST_DATABYTE_COUNT_LOCATION   4

//lint --e{793} by CNZUWAN AS code review is accetable.

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Checks if the number of bytes in a message is correct
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pMessage - Reference to the message content
 \param   [in] length - The length of the received message
 \return  OK if the number of received bytes is correct
 \warning
 \test
 date: 2016-09-07
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckLengthModbus_Commands_Helper(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                            const TUSIGN8 *pMessage,
                                            TUSIGN16 length)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 dataByteCount = 0 ;

    // For any read/write command, we should have 1 byte for the command, 2 bytes for the starting register/coil,
    // and two bytes for the number of registers/coils
    // Read command / write single coil / write single register command ?
    // These should all have only 5 bytes in the request message
    if ((pModbusCommandDescriptor->functionType == MODBUS_FUNCTION_TYPE_READ) ||
        (pModbusCommandDescriptor->commandId == MODBUS_COMMAND_ID_WRITE_SINGLE_COIL) ||
        (pModbusCommandDescriptor->commandId == MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER))
    {
        if (length == MODBUS_COMMANDS_READ_REQUEST_SIZE)
        {
            // Correct number of bytes for read command
            result = OK ;
        }
    }
    else
    {
        // Get the number of data bytes in the message - we don't need to format this from the buffer
        // as it's a single data byte
        dataByteCount = pMessage[MODBUS_COMMANDS_WRITE_REQUEST_DATABYTE_COUNT_LOCATION] ;
        // If it's a write command, we need at least the command, starting index and count
        if (length > MODBUS_COMMANDS_WRITE_REQUEST_SIZE)
        {
            // Correct number of bytes ?
            if (MODBUS_COMMANDS_WRITE_REQUEST_SIZE + dataByteCount == length)
            {
                // Yes, OK to exit with OK
                result = OK ;
            }
        }
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Extracts the starting index and number of items requested from a message buffer
 \author  GL
 \date    13/08/08
 \param   [in] pMessage - Reference to the message to read the information from
 \param   [in] pModbusRegisterTableRequestDetails - The start register and item count
 \return  void
 \warning
 \test
 date: 2016-09-07
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void GetStartingIndexAndCountModbus_Commands_Helper(const TUSIGN8 *pMessage,
                                                    TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails)
{
    // Get the starting register index
    ConvertToInternalModbus_Formatting((void*)&pMessage[MODBUS_COMMANDS_START_REGISTER_LOCATION],
                                      (TUSIGN8*)&pModbusRegisterTableRequestDetails->startRegisterAddress,
                                      sizeof(TUSIGN16)) ;
    // And the item count
    ConvertToInternalModbus_Formatting((void*)&pMessage[MODBUS_COMMANDS_REGISTER_COUNT_LOCATION],
                                      (TUSIGN8*)&pModbusRegisterTableRequestDetails->itemCount,
                                      sizeof(TUSIGN16)) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Adds the starting index and number of items requested to a message buffer
 \author  GL
 \date    01/06/09
 \param   [in] pMessage - Reference to the message to read the information from
 \param   [in] pModbusRegisterTableRequestDetails - The start register and item count
 \return  void
 \warning
 \test
 date: 2016-09-07
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void AddStartingIndexAndCountModbus_Commands_Helper(TUSIGN8 *pMessage,
                                                    const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails)
{
    // Add the starting register index
    ConvertToExternalModbus_Formatting((void*)&pModbusRegisterTableRequestDetails->startRegisterAddress,
                                       (TUSIGN8*)&pMessage[MODBUS_COMMANDS_START_REGISTER_LOCATION],
                                       sizeof(TUSIGN16)) ;
    // Then the item count
    ConvertToExternalModbus_Formatting((void*)&pModbusRegisterTableRequestDetails->itemCount,
                                       (TUSIGN8*)&pMessage[MODBUS_COMMANDS_REGISTER_COUNT_LOCATION],
                                       sizeof(TUSIGN16)) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts Common Framework error information into a Modbus exception response code.
 \author  GL
 \date    29/07/08
 \param   [in] errorCode -error code
 \return  modbus Exception Type regarding the Error code.
 \warning
 \test
 date: 2016-09-07
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TModbusExceptionType TranslateErrorCodeToExceptionModbus_Commands_Helper(TUSIGN16 errorCode)
{
    TModbusExceptionType modbusExceptionType ;

    switch(errorCode)
    {
        case(OK):
            modbusExceptionType = MODBUS_EXCEPTION_TYPE_NONE ;
        break ;
        case(LESSTHAN_RANGEMIN):
        case(GREATERTHAN_RANGEMAX):
        case(RANGE_TOO_LOW):
        case(RANGE_TOO_HIGH):
        case(UPPER_RANGE_TOO_LOW):
        case(UPPER_RANGE_TOO_HIGH):
        case(LOWER_RANGE_TOO_LOW):
        case(LOWER_RANGE_TOO_HIGH):
        case(LESSTHAN_RANGEMIN_ERR):
        case(GREATERTHAN_RANGEMAX_ERR):
        case(RANGE_TOO_LOW_ERR):
        case(RANGE_TOO_HIGH_ERR):
        case(UPPER_RANGE_TOO_LOW_ERR):
        case(UPPER_RANGE_TOO_HIGH_ERR):
        case(LOWER_RANGE_TOO_LOW_ERR):
        case(LOWER_RANGE_TOO_HIGH_ERR):
        case(SPAN_TO_SMALL_ERR):
        case(SPAN_TO_SMALL):
        case(NOT_ON_GRID_ERR):
        case(RULE_VIOLATION):
        case(RULE_VIOLATION_ERR):
            modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_VALUE ;
        break ;
        case(ILLEGAL_OBJ_IDX):
        case(ILLEGAL_ATTRIB_IDX):
        case(READ_ONLY_ERR):
        case(READ_ONLY):
        case(STATE_DENIED):
        case(ILLEGAL_STATE):
        case(WRONG_STATE):
        case(WRONG_STATE_ERR): 
            modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS ;
        break ;
        case(NOT_ON_GRID):
        case(DEBUG_ERROR):
        case(METHOD_NOT_SUPPORTED):
        case(ILLEGAL_DATACLASS_INDEX):
        case(FATAL_ERROR):
        case(ERROR_RAM_STORAGE):
        case(ERROR_HW):
        default:
            modbusExceptionType = MODBUS_EXCEPTION_TYPE_SLAVE_DEVICE_FAILURE ;
        break ;
    }
    return(modbusExceptionType) ;
}