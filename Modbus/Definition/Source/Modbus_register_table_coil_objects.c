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
#include "Modbus/Source/Modbus_register_table_coil_objects.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Modbus/Interface/Modbus.h"
//lint --e{793} by CNZUWAN AS code review is accetable.

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Checks if the values for a write request are allowed by the target objects. As the values are read from
 \brief   the message buffer, they are converted into internal format and stored in the layer 7 message buffer
 \author  GL,       Klaus Pose
 \date    29/07/08, 2016-10-14
 \param   [in] pModbusCommandDescriptor - Reference to the request command structure
 \param   [in] pModbusRegisterTable - Reference to the register table where the objects reside
 \param   [in] objectCount - The number of objects being written
 \param   [in] arrayIndex - Array index where the initial object resides
 \param   [in] pMessage - Reference to the request message
 \return  OK if all of the values in the message can be written
 \warning Expects data to start at index 0 in the passed buffer
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
TUSIGN16 CheckRangeModbus_Register_Table_Coil_Objects(const TModbusRegisterTable *me,
                                                       const  TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        //lint -e{818}
                                                        TUSIGN8 *pBuffer)
{
    TUSIGN16 result = OK ;
    TUSIGN16 objectsChecked ;
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Current index of the message buffer
    TUSIGN16 messageIndex = 0 ;
    // Current bit mask into buffer
    TUSIGN8 coilMask = 0x1 ;
    // Coil value for 8 bit values
    TUSIGN8 u8CoilValue ;
    // Coil value for 16 bit values
    TUSIGN16 u16CoilValue ;
    // Subsystem being accessed for check function
    const T_UNIT SLOW *pSub = (const T_UNIT SLOW *)NULL ;
    TUSIGN16 stateSub = NOT_INITIALIZED;
    // Pointer to the value being written
    void* pWriteValue = (void* )NULL ;

    // Check if we need to set the 8 or 16 bit coil value
    if (me->pModbusRegisterTableDataDescriptor->totalByteSize == sizeof(TUSIGN16))
    {
        pWriteValue = &u16CoilValue ;
    }
    else
    {
        pWriteValue = &u8CoilValue ;
    }

    // Check the range for objects until an error occurs or all are checked
    for (objectsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (objectsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
        objectsChecked++)
    {
        // Check the next object
        // First get the details of the object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsChecked] ;
        // Get its owner subsystem reference
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
        }
        if( pSub && (stateSub >= INITIALIZED) )
        {
            // 16 or 8 bit value ?
            if (me->pModbusRegisterTableDataDescriptor->totalByteSize == sizeof(TUSIGN16))
            {
                if ((pBuffer[messageIndex] & coilMask) > 0)
                {
                	//lint -e{826}
                    *((TUSIGN16*)pWriteValue) = 0x1 ;
                }
                else
                {
					//lint -e{826}
                    *((TUSIGN16*)pWriteValue) = 0x0 ;
                }
            }
            else
            {
                if ((pBuffer[messageIndex] & coilMask) > 0)
                {
                	//lint -e{826}
                    *((TUSIGN8*)pWriteValue) = 0x1 ;
                }
                else
                {
                	//lint -e{826}
                    *((TUSIGN8*)pWriteValue) = 0x0 ;
                }
            }
            // Call the associated check function
            result = pSub->Check(pSub,
                                pModbusRegisterTableItem->objectId,
                                (TINT16)pModbusRegisterTableItem->itemDetails,
                                pWriteValue) ;
        } else
        { // fail
            result = WRONG_STATE_ERR;
        }
        // Update the coil mask
        coilMask <<= 1 ;
        // Do we need to check the next byte in the buffer ?
        if (coilMask == 0)
        {
            // Yes - update the buffer index - update by a byte each time here as the coil
            // data is packed 8 coil values per byte
            messageIndex++ ;
            // And reinitialise the coil mask
            coilMask = 0x1 ;
        }
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Writes an object with the value passed in the message buffer
 \author  GL,       Klaus Pose
 \date    29/07/08, 2016-10-14
 \param   [in] pSub - Reference to the subsystem that owns the object being written
 \param   [in] pModbusRegisterTable - Register table where the object resides
 \param   [in] pModbusObjectTableItem - The register table item to write
 \param   [in] pMessage - Reference to the request message
 \return  OK if all of the values are written sucessfully
 \warning Expects data to be in internal (little endian) format. Register based buffers
 \warning have to be converted before writing will be successful
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
TUSIGN16 AccessObjectsModbus_Register_Table_Coil_Objects(const TModbusRegisterTable *me,
                                                         //lint -e{715}
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
    // 8 bit value being accessed
    TUSIGN8 u8CoilValue ;
    // 16 bit value being accessed
    TUSIGN16 u16CoilValue ;
    // Mask for the byte currently being accessed
    TUSIGN8 coilMask ;
    // Pointer to the buffer being used to access the actual data object
    void *pAccessValue = (void *)NULL ;

    // Initialise the coil mask to point at the initial coil
    coilMask = 0x1 ;
    // Check if we're accessing 8 or 16 bit data
    if (me->pModbusRegisterTableDataDescriptor->totalByteSize == sizeof(TUSIGN16))
    {
        pAccessValue = &u16CoilValue ;
    }
    else
    {
        pAccessValue = &u8CoilValue ;
    }

    // Write all of the objects until an error occurs
    for (objectsWritten = pModbusRegisterTableRequestLink->startItemArrayIndex ;
            (objectsWritten <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
            (result == OK) ;
        objectsWritten++)
    {
        // Get the details of the next item being accessed
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsWritten] ;
        // Get access to the subsystem where the object resides
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId) ;
        if( pSub )
        {
            (void)pSub->GetState( pSub, &stateSub );
        }
        if( pSub && (stateSub >= INITIALIZED) )
        {
            // Writing the coil ?
            if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_WRITE)
            {
                // 16 bit data ?
                if (me->pModbusRegisterTableDataDescriptor->totalByteSize == sizeof(TUSIGN16))
                {
                    if ((pBuffer[messageIndex] & coilMask) > 0)
                    {
                    	//lint -e{826}
                        *((TUSIGN16 *)pAccessValue) = 0x1 ;
                    }
                    else
                    {
                    	//lint -e{826}
                        *((TUSIGN16 *)pAccessValue) = 0x0 ;
                    }
                }
                else
                {
                    if ((pBuffer[messageIndex] & coilMask) > 0)
                    {
                    	//lint -e{826}
                        *((TUSIGN8 *)pAccessValue) = 0x1 ;
                    }
                    else
                    {
                    	//lint -e{826}
                        *((TUSIGN8 *)pAccessValue) = 0x0 ;
                    }
                }
                // Now write the value
                result = pSub->Put(pSub,
                                    pModbusRegisterTableItem->objectId,
                                    (TINT16)pModbusRegisterTableItem->itemDetails,
                                    pAccessValue) ;
            }
            else
            {
                // First bit of this byte ?
                if (coilMask == 0x1)
                {
                    // Yes, ensure the byte in the buffer is cleared before OR'ing any bits with it
                    pBuffer[messageIndex] = 0x0 ;
                }
                // Read the value
                result = pSub->Get(pSub,
                                    pModbusRegisterTableItem->objectId,
                                    (TINT16)pModbusRegisterTableItem->itemDetails,
                                    pAccessValue) ;
                // Read OK ?
                if (result == OK)
                {
                    // 16 bit data ?
                    if (me->pModbusRegisterTableDataDescriptor->totalByteSize == sizeof(TUSIGN16))
                    {
                    	//lint -e{826}
                        if (*((TUSIGN16 *)pAccessValue) > 0)
                        {
                            pBuffer[messageIndex] |= coilMask ;
                        }
                    }
                    else
                    {
                        if (*((TUSIGN8 *)pAccessValue) > 0)
                        {
                            pBuffer[messageIndex] |= coilMask ;
                        }
                    }
                }
            }
        } else
        { // fail
            result = WRONG_STATE_ERR;
        }
        // Update the coil mask
        coilMask <<= 1 ;
        if (coilMask == 0)
        {
            coilMask = 0x1 ;
            messageIndex++ ;
        }
    }
    // If this is a read request, we need to update the message length
    if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ)
    {
        if ((pModbusRegisterTableRequestLink->itemArrayCount % MODBUS_COILS_PER_BYTE) > 0)
        {
            // Add a byte for the number of coils that won't fit into a multiple of 8
            (*pLength)++ ;
        }
        // Offset by one here for the initial eight coils
        (*pLength) += pModbusRegisterTableRequestLink->itemArrayCount / MODBUS_COILS_PER_BYTE ;
    }
    return(result) ;
}