//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_single_buffer.c
 Description    Modbus message handling functions for single type register tables

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
//#include "..\..\system\interface\assert.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_table_single_buffer.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_formatting.h"

TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink) ;

TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
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
TUSIGN16 GetTableLinkModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK ;

    // Get the starting array index and attribute of the first requested object
    //lint -e{838} CNZUWAN
    result = GetStartingObjectArrayIndexModbus_Register_Table_Single_Buffer(me,
                                                    pModbusRegisterTableRequestDetails,
                                                    pModbusRegisterTableRequestLink)  ;
    if (result == OK)
    {
        // Get the ending array index and attribute of the last requested object
        result = GetEndingObjectArrayIndexModbus_Register_Table_Single_Buffer(me,
                                                    pModbusRegisterTableRequestDetails,
                                                    pModbusRegisterTableRequestLink)  ;
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Gets the array index of an object when passed its register index.
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to find the object in
 \param   [in] pModbusCommandDescriptor - reference to the command request
 \param   [in] startIndex - The starting index for the data
 \param   [out] pArrayIndex - Reference to the value used to hold the value of the array index
 \return  The actual start register of the object. This may not be the same as a register in the middle
 \return  of the object may be passed
 \warning
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
TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                                        const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                        TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink)
{
    TUSIGN16 tempRegisterIndex = 0 ;
    TUSIGN16 result = FATAL_ERROR ;

    if ((pRequestDetails->startRegisterAddress >= me->modbusRegisterTableRange.lowerRange) &&
        (pRequestDetails->startRegisterAddress <= me->modbusRegisterTableRange.upperRange))
    {
        // Single type tables are made up of types that are always the same size
        // First we need to get the table offset
        tempRegisterIndex = pRequestDetails->startRegisterAddress - me->modbusRegisterTableRange.lowerRange ;
        // Now we divide the register index by the table type size in registers - this will give us the
        // array index
        pModbusRegisterTableRequestLink->startItemArrayIndex = tempRegisterIndex / me->
            pModbusRegisterTableDataDescriptor->totalRegisterSize ;
        // Set the start register address from this
        //lint -e{734} CNZUWAN
        pModbusRegisterTableRequestLink->startItemRegisterAddress =
            (pModbusRegisterTableRequestLink->startItemArrayIndex *
                me->pModbusRegisterTableDataDescriptor->totalRegisterSize) +
                    me->modbusRegisterTableRange.lowerRange ;
        // Always set the attribute to 0
        pModbusRegisterTableRequestLink->startItemAttribute = 0 ;
        // All details extracted OK
        result = OK ;
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Gets the array index of an object when passed its register index.
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to find the object in
 \param   [in] pModbusCommandDescriptor - reference to the command request
 \param   [in] startIndex - The starting index for the data
 \param   [out] pArrayIndex - Reference to the value used to hold the value of the array index
 \return  The actual start register of the object. This may not be the same as a register in the middle
 \return  of the object may be passed
 \warning
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
TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK ;
    TUSIGN16 objectCount ;

    if (pRequestDetails->isPrivateRequest == eTRUE)
    {
        objectCount = pRequestDetails->itemCount ;
    }
    else
    {
        objectCount = pRequestDetails->itemCount /
          me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
    }
    // Now set the link details
    // The count of requested items
    pModbusRegisterTableRequestLink->itemArrayCount = objectCount ;
    // The end item end register address
    //lint -e{734} CNZUWAN
    pModbusRegisterTableRequestLink->endItemRegisterAddress =
        (pModbusRegisterTableRequestLink->startItemRegisterAddress +
            (objectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize)) ;
    // The end attribute (always 0 for single tables)
    pModbusRegisterTableRequestLink->endItemAttribute = 0 ;
    // The end array index
    pModbusRegisterTableRequestLink->endItemArrayIndex =
        pModbusRegisterTableRequestLink->startItemArrayIndex + (objectCount - 1) ;
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
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
TUSIGN16 GetRequestLengthModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                          *pModbusRegisterTableRequestLink,
                                                          TBOOL byteLength)
{
    TUSIGN16 requestLength ;

    // In the case of single tables, each item is the same size - check whether we need to
    // return the byte or register size
    if (byteLength == eTRUE)
    {
         //lint -e{734} CNZUWAN
        requestLength = (me->pModbusRegisterTableDataDescriptor->totalByteSize *
                            pModbusRegisterTableRequestLink->itemArrayCount) ;
    }
    else
    {
        //lint -e{734} CNZUWAN
        requestLength = (me->pModbusRegisterTableDataDescriptor->totalRegisterSize *
                            pModbusRegisterTableRequestLink->itemArrayCount) ;
    }
    return(requestLength) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
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
TUSIGN16 CheckForObjectOverlapModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                            const TModbusRegisterTableRequestDetails
                                                            *pModbusRegisterTableRequestDetails,
                                                            const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;

    // Does the initial register requested match that of the initial object ?
    if (pModbusRegisterTableRequestDetails->startRegisterAddress ==
        pModbusRegisterTableRequestLink->startItemRegisterAddress)
    {
        // Private request ?
        if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
        {
            // Are there enough objects for the request ?
            if ((pModbusRegisterTableRequestLink->startItemArrayIndex +
                pModbusRegisterTableRequestLink->itemArrayCount)
                <= me->objectCount)
            {
                // Yes, request is OK
                result = OK ;
            }
        }
        else
        {
            // Does the last register address requested match that of the last object ?
            if (pModbusRegisterTableRequestLink->endItemRegisterAddress ==
                    (pModbusRegisterTableRequestLink->startItemRegisterAddress +
                        pModbusRegisterTableRequestDetails->itemCount))
            {
                // Is the last register address within range of the end of the table ?
                if (pModbusRegisterTableRequestLink->endItemRegisterAddress <=
                    ((me->objectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) +
                    me->modbusRegisterTableRange.lowerRange))
                {
                    // Yes, this request is OK
                    result = OK ;
                }
            }
        }
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
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
void FormatToInternalModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    TUSIGN16 itemsAdded ;
    // Read position in the message buffer
    TUSIGN16 readPosition = 0 ;
    // Write position in the message buffer
    TUSIGN16 writePosition = 0 ;
    // Size to offset the read position
    TUSIGN16 readPositionOffset ;
    // Size to offset the write position
    TUSIGN16 writePositionOffset ;
    // Set when the message doesn't need to be formatted
    TBOOL messageFormatted = eFALSE ;

    // For internal formatting, the write offset is the size in bytes of the variable being added
    writePositionOffset = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    // The read posision offset depends on whether we're reading 8 bit data out of the buffer
    // If we are, the upper padding byte has to be removed
    readPositionOffset = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    // Before continuing, check if this is a private command. If it is, and we're converting 8 bit data,
    // nothing needs to be done
    if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
        (writePositionOffset == 1))
    {
        // We can exit right away
        messageFormatted = eTRUE ;
    }
    if (messageFormatted == eFALSE)
    {
        // Reading 8 bits ?
        if (writePositionOffset == 1)
        {
            readPositionOffset = 2 ;
        }
        for (itemsAdded = 0 ; itemsAdded < pModbusRegisterTableRequestLink->itemArrayCount ; itemsAdded++)
        {
            ConvertToInternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                &pBuffer[writePosition],
                                                writePositionOffset) ;
            // Update the read and write positions
            readPosition += readPositionOffset ;
            writePosition += writePositionOffset ;
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning When formatting to external, we need to work backwards through the buffer. We do this
 \warning so we don't overwrite anything if padding bytes need to be added
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
void FormatToExternalModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Current read position in the buffer
    TUSIGN16 readPosition = 0 ;
    // Current write position in the buffer
    TUSIGN16 writePosition ;
    // Offset for the read position - always the size of the item in bytes
    TUSIGN16 readPositionOffset ;
    // Offset for the write position
    TUSIGN16 writePositionOffset ;
    // Count of items added to the buffer
    TUSIGN16 itemsAdded ;
    // Set when message formatting is complete
    TBOOL messageFormatted = eFALSE ;

    // Set the read offset size
    readPositionOffset = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    writePositionOffset = readPositionOffset ;
    // Check if we're formatting a byte message
    if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
        (writePositionOffset == 1))
    {
        // Yes, so we can exit
        messageFormatted = eTRUE ;
    }
    if (messageFormatted == eFALSE)
    {
        // Reading bytes ?
        if (writePositionOffset == 1)
        {
            writePositionOffset = 2 ;
        }

        // Get the size of the message in registers - this will give us the write offset into
        // the buffer
        writePosition = GetRequestLengthModbus_Register_Table_Single_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eFALSE) ;
        // Multiply by 2 here to get the regsiter quantity in bytes
        writePosition *= 2 ;
        // The size in bytes gives us the read offset
        readPosition = GetRequestLengthModbus_Register_Table_Single_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eTRUE) ;
        // Each time we add an item into the buffer, we reduce the write position by the total register
        // size of the added item
        for (itemsAdded = 0 ; itemsAdded < pModbusRegisterTableRequestLink->itemArrayCount ; itemsAdded++)
        {
            // Offset the read and write positions
            writePosition -= writePositionOffset ;
            readPosition -= readPositionOffset ;
            // Convert the next item
            ConvertToExternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                &pBuffer[writePosition],
                                                readPositionOffset) ;
        }
    }
}