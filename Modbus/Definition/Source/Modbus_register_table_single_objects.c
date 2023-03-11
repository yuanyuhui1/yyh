//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table.c
 Description    Type used to enacapsulate register table behaviour

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_register_table_single_objects.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Modbus/Interface/Modbus.h"
//--------------------------------------------------------------------------------------------------
/*!
 \brief   Checks if the values for a write request are allowed by the target objects. As the values are read from
 \brief   the message buffer, they are converted into internal format and stored in the layer 7 message buffer
 \author  GL        Klaus Pose
 \date    29/07/08, 2016-10-14
 \param   [in] pModbusCommandDescriptor - Reference to the request command structure
 \param   [in] pModbusRegisterTable - Reference to the register table where the objects reside
 \param   [in] objectCount - The number of objects being written
 \param   [in] arrayIndex - Array index where the initial object resides
 \param   [in] pMessage - Reference to the request message
 \return  OK if all of the values in the message can be written
 \warning Expects data to start at index 0 in the passed buffer
 \test
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckRangeModbus_Register_Table_Single_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    TUSIGN16 result = OK;
    TUSIGN16 objectsChecked;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0;
    // Subsystem being accessed for check function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL;
    TUSIGN16 stateSub = NOT_INITIALIZED;

    // Check the range for objects until an error occurs or all are checked
    for (objectsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex;
        (objectsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) && (result == OK);
        objectsChecked++)
    {
        // Check the next object
        // First get the details of the object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsChecked];
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId);
        if (pSub)
        {
            (void)pSub->GetState(pSub, &stateSub);
        }
        if (pSub && (stateSub >= INITIALIZED))
        {
            // Call the associated check function
            result = pSub->Check(pSub, pModbusRegisterTableItem->objectId,
                                 (TINT16)pModbusRegisterTableItem->itemDetails,
                                 (void*)&pBuffer[messageIndex]) ;
        }
        else
        { // fail
            result = WRONG_STATE_ERR;
        }
        // Update the buffer index
        messageIndex += me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    }
    return(result);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Writes an object with the value passed in the message buffer
 \author  GL        Klaus Pose
 \date    29/07/08, 2016-10-14
 \param   [in] pSub - Reference to the subsystem that owns the object being written
 \param   [in] pModbusRegisterTable - Register table where the object resides
 \param   [in] pModbusObjectTableItem - The register table item to write
 \param   [in] pMessage - Reference to the request message
 \return  OK if all of the values are written sucessfully
 \warning Expects data to be in internal (little endian) format. Register based buffers
 \warning have to be converted before writing will be successful
 \test
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 AccessObjectsModbus_Register_Table_Single_Objects(const TModbusRegisterTable *me,
                                                           const TModbusRegisterTableRequestDetails
                                                            *pModbusRequestDetails,
                                                           const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink,
                                                            TUSIGN8 *pBuffer,
                                                            TModbusRegisterTableAccessType
                                                            modbusRegisterTableAccessType,
                                                            TUSIGN16 *pLength)
{
    TUSIGN16 result = OK ;
    TUSIGN16 objectsWritten ;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0 ;
    // Subsystem being accessed for access function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL ;
    TUSIGN16 stateSub = NOT_INITIALIZED;
    // Pointer to the access function
    TUSIGN16 (*pAccessFunction)(const T_UNIT SLOW*,TUSIGN16, TINT16, void FAST*) ;
    // Count of registers added
    TUSIGN16 registersAdded = 0 ;

    // Write all of the objects until an error occurs
    for (objectsWritten = pModbusRegisterTableRequestLink->startItemArrayIndex ;
            (objectsWritten <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
            (result == OK) ;
        objectsWritten++)
    {
        // Get the details of the next item being written
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsWritten] ;
        // Get access to the subsystem where the object resides
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
        }
        if( pSub && (stateSub >= INITIALIZED) )
        {
            // Get the appropriate access function
            if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
            {
                pAccessFunction = pSub->Get ;
            }
            else
            {
                pAccessFunction = pSub->Put ;
            }
            // Access the data
            result = pAccessFunction(pSub,
                                    pModbusRegisterTableItem->objectId,
                                    (TINT16)pModbusRegisterTableItem->itemDetails,
                                    (void*)&pBuffer[messageIndex]) ;
        }
        else
        { // fail
            result = WRONG_STATE_ERR;
        }
        // Update the message position before checking the result
        messageIndex += me->pModbusRegisterTableDataDescriptor->totalByteSize ;
        // And the count of registers added
        registersAdded += me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
    }
    // Is this a read request ?
    if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
    {
        if (pModbusRequestDetails->isPrivateRequest == eTRUE)
        {
            // If it's a private request, set the number of bytes added to the buffer
            *pLength += messageIndex ;
        }
        else
        {
            // It it's a register based request, add the number of registers (in bytes) added
            //lint -e{734}  CNZUWAN
            *pLength += (registersAdded * MODBUS_BYTES_PER_REGISTER) ;
        }
    }
    return(result) ;
}