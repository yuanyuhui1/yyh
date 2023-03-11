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
#include "Modbus/Source/Modbus_register_table_string_objects.h"
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
TUSIGN16 CheckRangeModbus_Register_Table_String_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    TUSIGN16 result = OK ;
    TUSIGN16 stringsChecked ;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0 ;
    // Subsystem being accessed for check function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL ;
    TUSIGN16 stateSub = NOT_INITIALIZED;

    // Check the range for objects until an error occurs or all are checked
    for (stringsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (stringsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
        stringsChecked++)
    {
        // Check the next string
        // First get the details of the object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[stringsChecked] ;
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
        }
        if( pSub && (stateSub >= INITIALIZED) )
        {
            // Call the associated check function
            result = pSub->Check(pSub,
                                pModbusRegisterTableItem->objectId,
                                (TINT16)0,
                                (void*)&pBuffer[messageIndex]) ;
        } else
        { // fail
            result = WRONG_STATE_ERR;
        }
        // Update the buffer index - is only one string being written ?
        if (pModbusRegisterTableRequestLink->itemArrayCount != 1)
        {
            // More than one. Is this the first string ?
            if (stringsChecked == pModbusRegisterTableRequestLink->startItemArrayIndex)
            {
                // Check if we started at some attribute past 0
                if (pModbusRegisterTableRequestLink->startItemAttribute > 0)
                {
                    // If so, jump into the buffer by the number of attributes from the start string
                    //lint -e{734} CNZUWAN
                    messageIndex = (me->pModbusRegisterTableDataDescriptor->totalByteSize *
                                    (me->pModbusRegisterTableItems[stringsChecked].itemDetails -
                                    pModbusRegisterTableRequestLink->startItemAttribute)) ;
                }
                else
                {
                    //lint -e{734} CNZUWAN
                    messageIndex = (me->pModbusRegisterTableDataDescriptor->totalByteSize *
                                    me->pModbusRegisterTableItems[stringsChecked].itemDetails) ;
                }
            }
            // Is it a string other than the last string ?
            else if (stringsChecked != pModbusRegisterTableRequestLink->endItemArrayIndex)
            {
                // Yes, so jump by the size of the string
                //lint -e{734} CNZUWAN
                messageIndex = (me->pModbusRegisterTableDataDescriptor->totalByteSize *
                                me->pModbusRegisterTableItems[stringsChecked].itemDetails) ;
            }
            else
            {
                // Last string, so nothing to do here.
            }
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
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 AccessObjectsModbus_Register_Table_String_Objects(const TModbusRegisterTable *me,
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
    TUSIGN16 stringsChecked ;
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
    // Set when can the whole object can be accessed
    TBOOL accessWholeObject ;
    // Function pointer to the access function
    TUSIGN16 (*pAccessFunction)(const T_UNIT SLOW*,TUSIGN16,TINT16,void FAST*) ;
    // Count of registers added
    TUSIGN16 registersAdded = 0 ;

    // Check the range for objects until an error occurs or all are checked
    for (stringsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (stringsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
        stringsChecked++)
    {
        // Check the next string
        // First get the details of the object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[stringsChecked] ;
        // Assume we're going to have to access a number of attributes
        accessWholeObject = eFALSE ;
        // How many attributes need to be written ? If all of the attributes can be written,
        // we can do a WHOLE_OBJECT put. If not, we need to Put each attribute individually
        // First string in the list ?
        if (pModbusRegisterTableRequestLink->startItemArrayIndex == stringsChecked)
        {
            // Can we access all attributes ?
            if ((pModbusRegisterTableRequestLink->startItemAttribute == 0) &&
                (pModbusRegisterTableRequestLink->startItemArrayIndex !=
                 pModbusRegisterTableRequestLink->endItemArrayIndex))
            {
                // Yes, set the access whole object flag so all attributes are accessed at once
                accessWholeObject = eTRUE ;
            }
            else
            {
                // Only one string being accessed ?
                if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
                {
                    // Yes, set the start and count of attributes
                    attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                    attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
                }
                else
                {
                    // No, at least two strings are being accessed, but we're not starting at the first attribute
                    // of the first item
                    attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
                    attributeCount = (pModbusRegisterTableItem->itemDetails - 1) ;
                }
            }
        }
        // String other than the last string ?
        else if (stringsChecked != pModbusRegisterTableRequestLink->endItemArrayIndex)
        {
            // Always access the whole object in this case
            accessWholeObject = eTRUE ;
        }
        else
        {
            // Last string, can we write all of it ?
            if (pModbusRegisterTableRequestLink->endItemAttribute ==
                (pModbusRegisterTableItem[stringsChecked].itemDetails - 1))
            {
                accessWholeObject = eTRUE ;
            }
            else
            {
                // No, so we need to write a number of attributes
                attributeNumber = 0 ;
                attributeCount = pModbusRegisterTableRequestLink->endItemAttribute ;
            }
        }
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
            // Get the appropriate access function
            if ( modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
            {
                pAccessFunction = pSub->Get ;
            }
            else
            {
                pAccessFunction = pSub->Put ;
            }
        } else
        {
            stateSub = NOT_INITIALIZED;
			//set pointer to zero to fulfill MISRA Rule 30 "pAccessFunction may not have been initialized" 
			pAccessFunction = 0;
        }
        // Writing whole object ?
        if (accessWholeObject == eTRUE)
        {
            // Yes, all attributes for the string have been requested
            if( stateSub >= INITIALIZED )
            {
				//check for zero pointer
				VIP_ASSERT(pAccessFunction);
                result = pAccessFunction(pSub,
                                        pModbusRegisterTableItem->objectId,
                                        WHOLE_OBJECT,
                                        (void*)&pBuffer[messageIndex]) ;
            }
            else
            { // fail
                result = WRONG_STATE_ERR;
            }
            // Update the message index with the size of the whole object
            //lint -e{734} CNZUWAN
            messageIndex += (me->pModbusRegisterTableDataDescriptor->totalByteSize *
                                pModbusRegisterTableItem->itemDetails) ;
            // Also the count of registers
            //lint -e{734} CNZUWAN
            registersAdded += (me->pModbusRegisterTableDataDescriptor->totalRegisterSize *
                                pModbusRegisterTableItem->itemDetails) ;
        }
        else
        {
            // No, write a number of attributes
            //lint -e{644} It has initialized.
            for (;(attributeNumber <= attributeCount) && (result == OK) ; attributeNumber++)
            {
                if( stateSub >= INITIALIZED )
                {
					//check for zero pointer
					VIP_ASSERT(pAccessFunction);
                    result = pAccessFunction(pSub,
                                            pModbusRegisterTableItem->objectId,
                                            (TINT16)attributeNumber,
                                            (void*)&pBuffer[messageIndex]) ;
                } else
                { // fail
                    result = WRONG_STATE_ERR;
                }
                // As we write the individual attributes, keep updating the position in the message
                messageIndex += me->pModbusRegisterTableDataDescriptor->totalByteSize ;
                registersAdded += me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
            }
        }
    }
    // Read request ?
    if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
    {
        // Private request ?
        if (pModbusRequestDetails->isPrivateRequest == eTRUE)
        {
            *pLength += messageIndex ;
        }
        else
        {
            // For public commands, add the number of registers converted to bytes
            //lint -e{734} CNZUWAN
            *pLength += (registersAdded * MODBUS_BYTES_PER_REGISTER) ;
        }
    }
    return(result) ;
}