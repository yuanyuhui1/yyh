//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         Subsystem MODBUS
 Module         MODBUS_generic_handlers.c
 Description    Generic data handlers used by both custom and public Modbus commands
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
//#include "..\..\modbus\source\modbus_command_descriptor.h"
//#include "..\..\modbus\source\modbus_command_descriptors.h"
#include "Modbus/Source/Modbus_data_handlers_generic.h"
//#include "..\..\coordinator\interface\subsystem_idx.h"
//lint --e{793} by CNZUWAN AS code review is accetable.

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Reads objects in little endian format and loads the results into the passed message buffer
 \author  GL
 \date    20/08/08
 \param   [in] pModbusRegisterTable - Reference to the modbus register table structure
 \param   [in] pModbusHardwareLayer - Reference to the hardware structure	 
 \param   [in] pModbusRequestDetails - Reference to the modbus request detailed structure
 \param   [in] pModbusRegisterTableRequestLink - Number of objects to read
 \param   [in[ pMessage - Reference to the buffer holding the coils to be written

 \return  OK if all of the objects are read successfully
 \warning
 \test
 date: 2016-09-01
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 ReadObjectsModbus_Data_Handlers_Generic(const TModbusRegisterTable *pModbusRegisterTable,
                                                 TModbusHardwareLayer *pModbusHardwareLayer,
                                                 const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                                                 const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                 TUSIGN8 *pMessage)
{
    // Operation result
    TUSIGN16 result = OK ;

    // Check the access rights for the requested objects if required for the hardware layer
    if (pModbusHardwareLayer->modbusServerResponseParameters.checkAccessRights == eTRUE)
    {
        result = pModbusRegisterTable->CheckAccessRights(pModbusRegisterTable,
                                                         pModbusRegisterTableRequestLink,
                                                         MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ);
    }
    // All allowed access ?
    if (result == OK)
    {
        // Read all of the requested objects
        result = pModbusRegisterTable->AccessObjects(pModbusRegisterTable,
                                                     pModbusRequestDetails,
                                                     pModbusRegisterTableRequestLink,
                                                     pMessage,
                                                     MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ,
                                                     &pModbusHardwareLayer->modbusCommonParameters.length);
        // All read OK ?
        if (result == OK)
        {
            // If so, we can now expand the buffer to Modbus format
            pModbusRegisterTable->FormatToExternal(pModbusRegisterTable,
                                                   pModbusRequestDetails,
                                                   pModbusRegisterTableRequestLink,
                                                   pMessage);
        }
    }
    return(result);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Writes objects in little endian format
 \author  GL
 \date    20/08/08
 \param   [in] pModbusRegisterTable - Reference to the modbus register table structure
 \param   [in] pModbusHardwareLayer - Reference to the hardware structure	 
 \param   [in] pModbusRequestDetails - Reference to the modbus request detailed structure
 \param   [in] pModbusRegisterTableRequestLink - Number of objects to read
 \param   [in[ pMessage - Reference to the buffer holding the coils to be written
 \return  OK if all of the objects are written correctly
 \warning
 \test
 date: 2016-09-05
 \n by: zuochen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 WriteObjectsModbus_Data_Handlers_Generic(const TModbusRegisterTable *pModbusRegisterTable,
                                                  TModbusHardwareLayer *pModbusHardwareLayer,
                                                  const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                                                  const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                  TUSIGN8 *pMessage)
{
    // Operation result
    TUSIGN16 result = OK ;

    // Check the access rights for the objects if required for the hardware layer
    if (pModbusHardwareLayer->modbusServerResponseParameters.checkAccessRights == eTRUE)
    {
        result = pModbusRegisterTable->CheckAccessRights(pModbusRegisterTable,
                                                         pModbusRegisterTableRequestLink,
                                                         MODBUS_REGISTER_TABLE_ACCESS_TYPE_WRITE);
    }
    // Allowed access to all objects ?
    if (result == OK)
    {
        // Yes - compress the buffer to internal format
        pModbusRegisterTable->FormatToInternal(pModbusRegisterTable,
                                               pModbusRequestDetails,
                                               pModbusRegisterTableRequestLink,
                                               pMessage);
        // Check if the values being written are within limits
        result = pModbusRegisterTable->CheckRange(pModbusRegisterTable,
                                                  pModbusRegisterTableRequestLink,
                                                  pMessage);
        // All values allowed ?
        if (result == OK)
        {
            // If so, write the values
            result = pModbusRegisterTable->AccessObjects(pModbusRegisterTable,
                                                         pModbusRequestDetails,
                                                         pModbusRegisterTableRequestLink,
                                                         pMessage,
                                                         MODBUS_REGISTER_TABLE_ACCESS_TYPE_WRITE,
                                                         &pModbusHardwareLayer->modbusCommonParameters.length);
        }
    }
    return(result);
}