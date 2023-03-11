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
#include "Modbus/Source/Modbus_register_table_whole_object_objects.h"
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
 \n date: 2016-10-18
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckRangeModbus_Register_Table_Whole_Object_Objects(const TModbusRegisterTable *me,
                                                            const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink,
                                                            TUSIGN8 *pBuffer)
{
    TUSIGN16 result = OK ;
    // Index of the whole object currently being checked
    TUSIGN16 objectsChecked ;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0 ;
    // Subsystem being accessed for check function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL ;
    TUSIGN16 stateSub = NOT_INITIALIZED;
    // Current attribute being accessed
    TUSIGN16 attributeNumber ;
    // Maximum number of attributes to access
    TUSIGN16 attributeCount ;
    // Check the range for attributes until an error occurs or all are checked
    for (objectsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (objectsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
        objectsChecked++)
    {
        // First object in the list ?
        if (objectsChecked == pModbusRegisterTableRequestLink->startItemArrayIndex)
        {
            // Only item in the list ?
            if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
            {
                // Yes, check what attributes need to be checked
                attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
            }
            else
            {
                // No, we know what the last attribute will be
                attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            }
        }
        // Last object in the list ?
        else if (objectsChecked == pModbusRegisterTableRequestLink->endItemArrayIndex)
        {
            // Can we check all of its attributes ?
            if (pModbusRegisterTableRequestLink->endItemAttribute ==
                (me->pModbusRegisterTableDataDescriptor->attributeCount - 1))
            {
                // Yes, set the attribute count accordingly
                attributeNumber = 0 ;
                attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            }
            else
            {
                // No, some at the end have been skipped
                attributeNumber = 0 ;
                attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
            }
        }
        else
        {
            // Read all of the object attributes
            attributeNumber = 0 ;
            attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
        }
        // Get the details of the whole object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsChecked] ;
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
        }
        // Call the associated check function for each attribute
        for (;(attributeNumber <= attributeCount) && (result == OK) ; attributeNumber++)
        {
            if( pSub && (stateSub >= INITIALIZED) )
            {
                // Call the associated check function
                result = pSub->Check(pSub,
                                pModbusRegisterTableItem->objectId,
                                (TINT16)attributeNumber,
                                (void*)&pBuffer[messageIndex]) ;
            } else
            { // fail
                result = WRONG_STATE_ERR;
            }
            // Update the index position for the message buffer as we check each item
            messageIndex += me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
        }
    }
    return(result) ;
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
 \n date: 2016-10-18
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 AccessObjectsModbus_Register_Table_Whole_Object_Objects(const TModbusRegisterTable *me,
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
    TUSIGN16 objectsChecked ;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0 ;
    // Subsystem being accessed for Put function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL ;
    TUSIGN16 stateSub = NOT_INITIALIZED;
    // Count of attributes to be written
    TUSIGN16 attributeCount ;
    // Id of attribute currently being written
    TUSIGN16 attributeNumber ;
    // Function pointer to the access function
    TUSIGN16 (*pAccessFunction)(const T_UNIT SLOW*,TUSIGN16,TINT16,void FAST*) ;
    // Count of registers added to the buffer
    TUSIGN16 registersAdded = 0 ;

    // Check the range for attributes until an error occurs or all are checked
    for (objectsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (objectsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
        objectsChecked++)
    {
        // First object in the list ?
        if (objectsChecked == pModbusRegisterTableRequestLink->startItemArrayIndex)
        {
            // Only item in the list ?
            if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
            {
                // Yes, check what attributes need to be checked
                attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
            }
            else
            {
                // No, we know what the last attribute will be
                attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            }
        }
        // Last object in the list ?
        else if (objectsChecked == pModbusRegisterTableRequestLink->endItemArrayIndex)
        {
            // Can we check all of its attributes ?
            if (pModbusRegisterTableRequestLink->endItemAttribute ==
                (me->pModbusRegisterTableDataDescriptor->attributeCount - 1))
            {
                // Yes, set the attribute count accordingly
                attributeNumber = 0 ;
                attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            }
            else
            {
                // No, some at the end have been skipped
                attributeNumber = 0 ;
                attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
            }
        }
        else
        {
            // Read all of the object attributes
            attributeNumber = 0 ;
            attributeCount = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
        }
        // Get the details of the object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsChecked] ;
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
            // And get the access function
            if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
            {
                pAccessFunction = pSub->Get ;
            }
            else
            {
                pAccessFunction = pSub->Put ;
            }
        } else
        {
			//set pointer to zero to fulfill MISRA Rule 30 "pAccessFunction may not have been initialized" 
			pAccessFunction = 0;
            stateSub = NOT_INITIALIZED;
        }
        // Call the access function for each attribute
        for (;(attributeNumber <= attributeCount) && (result == OK) ; attributeNumber++)
        {
            if( stateSub >= INITIALIZED )
            {
				//check for zero pointer
				VIP_ASSERT(pAccessFunction);
                // Call the access function
                result = pAccessFunction(pSub,
                                        pModbusRegisterTableItem->objectId,
                                        (TINT16)attributeNumber,
                                        (void*)&pBuffer[messageIndex]) ;
            } else
            { // fail
                result = WRONG_STATE_ERR;
            }
            // Update the index position for the message buffer as we access each item
            messageIndex += me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
            // And update the count of registers added for read requests
            registersAdded += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeNumber] ;
        }
    }
    // Read request ?
    if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
    {
        // Private command ?
        if (pModbusRequestDetails->isPrivateRequest == eTRUE)
        {
            // If it's a private request, add the number of bytes added to the buffer
            *pLength += messageIndex ;
        }
        else
        {
            // If it's a register based request, add the number of registers
            // (in bytes) added to the buffer
            //lint -e{734}
            *pLength += (registersAdded * MODBUS_BYTES_PER_REGISTER) ;
        }
    }
    return(result) ;
}