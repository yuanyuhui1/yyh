//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_multi_buffer.c
 Description    Modbus message buffer handling functions for multi type register tables

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
#include "Modbus/Source/Modbus_register_table_multi_buffer.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_formatting.h"

TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                                       const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                        TModbusRegisterTableRequestLink
                                                                          *pModbusRegisterTableRequestLink) ;

TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetTableLinkModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK ;

    // Get the starting array index and attribute of the first requested object
    //lint -e{838}
    result = GetStartingObjectArrayIndexModbus_Register_Table_Multi_Buffer(me,
                                                    pModbusRegisterTableRequestDetails,
                                                    pModbusRegisterTableRequestLink)  ;
    if (result == OK)
    {
        // Get the ending array index and attribute of the last requested object
        result = GetEndingObjectArrayIndexModbus_Register_Table_Multi_Buffer(me,
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                                        const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                        TModbusRegisterTableRequestLink
                                                                          *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 attributeSizes ;
    TUSIGN16 arrayIndexCount = 0 ;
    TBOOL attributeFound = eFALSE ;
    TUSIGN16 attributeCount ;
    TUSIGN16 wholeObjectCount ;

    if ((pRequestDetails->startRegisterAddress >= me->modbusRegisterTableRange.lowerRange) &&
        (pRequestDetails->startRegisterAddress <= me->modbusRegisterTableRange.upperRange))
    {
        // Multi type tables are made up of attributes from (possibly) different objects.
        // Each attribute occupies one space in the register table
        // Determine what attribute has been requested first - first read ahead into the table
        // by the number of complete multiple requests before the first requested index
        wholeObjectCount = (pRequestDetails->startRegisterAddress - me->modbusRegisterTableRange.lowerRange)
                            /me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
        // The number of registers before the first requested item
        //lint -e{734}
        attributeSizes = (me->pModbusRegisterTableDataDescriptor->totalRegisterSize * wholeObjectCount) ;
        // Determine the first requested table item by firstly getting the count of table items in whole objects
        //lint -e{734}
        arrayIndexCount = (me->pModbusRegisterTableDataDescriptor->attributeCount * wholeObjectCount) ;
        attributeSizes += me->modbusRegisterTableRange.lowerRange ;
        // Now determine the first requested attribute
        for (attributeCount = 0 ; (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount)
             && (attributeFound == eFALSE) ; attributeCount++)
        {
            attributeSizes += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
            if (pRequestDetails->startRegisterAddress < attributeSizes)
            {
                attributeFound = eTRUE ;
                pModbusRegisterTableRequestLink->startItemArrayIndex = arrayIndexCount ;
                pModbusRegisterTableRequestLink->startItemAttribute = attributeCount ;
                pModbusRegisterTableRequestLink->startItemRegisterAddress = attributeSizes -
                    me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
                result = OK ;
            }
            else
            {
                arrayIndexCount++ ;
            }
        }
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 registerCount = 0 ;
    TUSIGN16 multiSize = 0 ;
    TUSIGN16 attributeCount ;
    TBOOL attributeFound = eFALSE ;
    TUSIGN16 arrayIndexCount ;
    TUSIGN16 wholeObjectCount ;
    TUSIGN16 totalRegisterCount ;

    // Private request ?
    if (pRequestDetails->isPrivateRequest == eTRUE)
    {
        // Multiple requests in private form request a number of a group of attributes
        // So we need the number of requests times the size of each request to give us the
        // last array index
        //lint -e{734}
        registerCount = pRequestDetails->itemCount * me->pModbusRegisterTableDataDescriptor->attributeCount ;
        // Reduce the register count by one to account for the zero based table
        registerCount-- ;
        pModbusRegisterTableRequestLink->endItemArrayIndex = pModbusRegisterTableRequestLink->
            startItemArrayIndex + registerCount ;
        // Set the count of items being accessed
        //lint -e{734}
        pModbusRegisterTableRequestLink->itemArrayCount = pRequestDetails->itemCount * me->
            pModbusRegisterTableDataDescriptor->attributeCount ;
        // The end register address
        //lint -e{734}
        pModbusRegisterTableRequestLink->endItemRegisterAddress = pModbusRegisterTableRequestLink->startItemRegisterAddress +
            (me->pModbusRegisterTableDataDescriptor->totalRegisterSize * pRequestDetails->itemCount) ;
        // The end attribute
        pModbusRegisterTableRequestLink->endItemAttribute = (me->pModbusRegisterTableDataDescriptor->
            attributeCount - 1) ;
        result = OK ;
    }
    else
    {
        // Preset the end array index here
        arrayIndexCount = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        // Was the first attribute of the initial table definition item requested ?
        if (pModbusRegisterTableRequestLink->startItemAttribute != 0)
        {
            // No, count up the sum of registers here - we may be able to complete the search here
            for (attributeCount = pModbusRegisterTableRequestLink->startItemAttribute ;
                    (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount) &&
                        (attributeFound == eFALSE) ; attributeCount++, arrayIndexCount++)
            {
                registerCount +=  me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
                // Do all requested registers fit ?
                if (pRequestDetails->itemCount <= registerCount)
                {
                    // Yes - stop searching here
                    attributeFound = eTRUE ;
                    // Attribute number remains the same
                    pModbusRegisterTableRequestLink->endItemAttribute = attributeCount ;
                    // As does the end array index
                    pModbusRegisterTableRequestLink->endItemArrayIndex = arrayIndexCount ;
                    // Only one register table item requested
                    pModbusRegisterTableRequestLink->itemArrayCount = (pModbusRegisterTableRequestLink->endItemArrayIndex -
                        pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
                    // We just need to add the size of the attribute to its start address
                    // to get the end address
                    pModbusRegisterTableRequestLink->endItemRegisterAddress =
                        pModbusRegisterTableRequestLink->startItemRegisterAddress + registerCount ;
                    result = OK ;
                }
            }
        }
        // Did we find the attribute ?
        if (attributeFound == eFALSE)
        {
            // For register based requests, we need to count up the size in registers of each attribute
            totalRegisterCount = pModbusRegisterTableRequestLink->startItemRegisterAddress ;
            // If so, hold on to the number of registers requested from the initial multiple request definition
            totalRegisterCount += registerCount ;
            // We still need to account for this number of requested registers
            registerCount = pRequestDetails->itemCount - registerCount ;
            // Determine how many complete multi requests - we don't want to have to read through each
            // table entry
            wholeObjectCount = registerCount / me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
            //lint -e{734}
            arrayIndexCount += me->pModbusRegisterTableDataDescriptor->attributeCount * wholeObjectCount ;
            //lint -e{734}
            totalRegisterCount += me->pModbusRegisterTableDataDescriptor->totalRegisterSize * wholeObjectCount ;
            // Offset the remaining count of requested registers by the number of whole multiple command definitions
            //lint -e{734}
            registerCount -= me->pModbusRegisterTableDataDescriptor->totalRegisterSize * wholeObjectCount ;
            // Check if we've landed on the correct attribute
            if (registerCount == 0)
            {
                // If so, we can set the attribute / address details here
                attributeFound = eTRUE ;
                pModbusRegisterTableRequestLink->endItemAttribute =
                    (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
                pModbusRegisterTableRequestLink->endItemRegisterAddress = totalRegisterCount ;
                pModbusRegisterTableRequestLink->endItemArrayIndex = (arrayIndexCount - 1) ;
                pModbusRegisterTableRequestLink->itemArrayCount = (pModbusRegisterTableRequestLink->endItemArrayIndex -
                    pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
                result = OK ;
            }
            // Check if we still need to search for the attribute
            if (attributeFound == eFALSE)
            {
                // Finally determine the end object / array index
                for (attributeCount = 0, multiSize = 0  ;
                 (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount) &&
                   (attributeFound == eFALSE) ; attributeCount++, arrayIndexCount++)
                {
                    multiSize += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
                    if (registerCount <= multiSize)
                    {
                        // Yes - find out what the last requested attribute was
                        attributeFound = eTRUE ;
                        pModbusRegisterTableRequestLink->endItemAttribute = attributeCount ;
                        pModbusRegisterTableRequestLink->endItemArrayIndex = arrayIndexCount ;
                        totalRegisterCount += multiSize ;
                        pModbusRegisterTableRequestLink->endItemRegisterAddress = totalRegisterCount ;
                        pModbusRegisterTableRequestLink->itemArrayCount =
                            (pModbusRegisterTableRequestLink->endItemArrayIndex -
                            pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
                        result = OK ;
                    }
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetRequestLengthModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                          *pModbusRegisterTableRequestLink,
                                                          TBOOL byteLength)
{
    // Length of the request in bytes
    TUSIGN16 requestLength = 0 ;
    // Count of table items
    TUSIGN16 itemCount ;
    TUSIGN16 startObject ;
    TUSIGN16 endObject ;
    TUSIGN16 currentAttribute ;
    TUSIGN16 lastAttribute ;
    TUSIGN16 startAttributeCount ;
    // The following are initialised depending on whether we're getting the byte or register length
    TUSIGN16 totalSize ;
    const TUSIGN16 *pAttributeSize ;

    if (byteLength == eTRUE)
    {
        totalSize = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
        pAttributeSize = me->pModbusRegisterTableDataDescriptor->pTableByteSizes ;
    }
    else
    {
        totalSize = me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
        pAttributeSize = me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes ;
    }
    // How many attributes have been requested ?
    itemCount = pModbusRegisterTableRequestLink->itemArrayCount ;
    // For the first object, what position in the multiple object is it in ?
    startObject = pModbusRegisterTableRequestLink->startItemArrayIndex %
      (me->pModbusRegisterTableDataDescriptor->attributeCount) ;
    // For the last object, what position in the multiple object is it in ?
    endObject = pModbusRegisterTableRequestLink->endItemArrayIndex %
      (me->pModbusRegisterTableDataDescriptor->attributeCount) ;

    // Just one attribute requested ?
    if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
    {
        // Yes - set the last attribute accordingly
        lastAttribute = pModbusRegisterTableRequestLink->startItemAttribute + 1 ;
        // Also set the count of attributes for the start definition
        startAttributeCount = 1 ;
    }
    else
    {
        // Are both attributes within the same table definition ?
        if ((startObject + pModbusRegisterTableRequestLink->itemArrayCount) <
            me->pModbusRegisterTableDataDescriptor->attributeCount)
        {
            // Yes, so again we need to set the last attribute and attribute count
            lastAttribute = endObject + 1 ;
            startAttributeCount = (endObject - startObject) + 1 ;
        }
        else
        {
            lastAttribute = me->pModbusRegisterTableDataDescriptor->attributeCount ;
            startAttributeCount = me->pModbusRegisterTableDataDescriptor->attributeCount - startObject ;
        }
    }
    // Offset for the first multiple command definition
    if (startObject != 0)
    {
        itemCount -= startAttributeCount ;
        for (currentAttribute = startObject ;
             currentAttribute < lastAttribute ;
             currentAttribute++)
        {
            requestLength += pAttributeSize[currentAttribute] ;
        }
    }
    // Then for the last multiple command definition
    if ((endObject != (me->pModbusRegisterTableDataDescriptor->attributeCount - 1))&&
        (itemCount > 0))
    {
        // At least one attribute has been requested
        currentAttribute = endObject ;
        do
        {
            if (itemCount != 0)
            {
                itemCount-- ;
            }
        }
        while(currentAttribute--) ;
        // Now get the length of the attributes at the end of the request
        for (currentAttribute = 0 ;
             currentAttribute <= endObject ;
             currentAttribute++)
        {
            requestLength += pAttributeSize[currentAttribute] ;
        }
    }
    itemCount /= me->pModbusRegisterTableDataDescriptor->attributeCount ;
    //lint -e{734}
    requestLength += itemCount * totalSize ;
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckForObjectOverlapModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                            const TModbusRegisterTableRequestDetails
                                                            *pModbusRegisterTableRequestDetails,
                                                            const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;

    // Private request ?
    if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
    {
        // Is the first requested register at the start of a multiple object definition ?
        if ((pModbusRegisterTableRequestDetails->startRegisterAddress %
            me->pModbusRegisterTableDataDescriptor->totalRegisterSize) == 0)
        {
            // Yes, are there enough whole objects for this request ?
            if (pModbusRegisterTableRequestDetails->itemCount <=
                (me->objectCount / me->pModbusRegisterTableDataDescriptor->attributeCount))
            {
                // Yes, so all checks are OK
                result = OK ;
            }
        }
    }
    else
    {
        // Does the first requested register land at the start of an object ?
        if (pModbusRegisterTableRequestLink->startItemRegisterAddress ==
            pModbusRegisterTableRequestDetails->startRegisterAddress)
        {
            // Yes, does the last requested register land at the end of an object ?
            if ((pModbusRegisterTableRequestLink->startItemRegisterAddress +
                pModbusRegisterTableRequestDetails->itemCount) ==
                pModbusRegisterTableRequestLink->endItemRegisterAddress)
            {
                // Does the end register reside within the table ?
                if (pModbusRegisterTableRequestLink->endItemRegisterAddress <=
                    (me->modbusRegisterTableRange.lowerRange +
                    ((me->objectCount / me->pModbusRegisterTableDataDescriptor->attributeCount)
                     * me->pModbusRegisterTableDataDescriptor->totalRegisterSize)))
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
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void FormatToInternalModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Index of item being added
    TUSIGN16 itemArrayId ;
    // Read position into the buffer
    TUSIGN16 readPosition = 0 ;
    // Write position into the buffer
    TUSIGN16 writePosition = 0 ;
    // Id of the attribute being accessed
    TUSIGN16 attributeNumber ;

    // Preset the attribute number - this gets updated (and possibly wraps around to 0)
    // each time an item is added to the buffer
    attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;

    for (itemArrayId = pModbusRegisterTableRequestLink->startItemArrayIndex ;
            itemArrayId <= pModbusRegisterTableRequestLink->endItemArrayIndex ;
                itemArrayId++)
    {
        // Convert the next item - before doing this, is this a private command and
        // a byte sized attribute ?
        if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
            (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1))
        {
            // If it is, skip the format, and just update the read position by one
            readPosition += 1 ;
        }
        else
        {
            ConvertToInternalModbus_Formatting((void*)&pBuffer[readPosition],
                                                &pBuffer[writePosition],
                                                me->pModbusRegisterTableDataDescriptor->
                                                pTableByteSizes[attributeNumber]) ;
            // Add the offsets to the read and write indexes
            if (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1)
            {
                readPosition += 2 ;
            }
            else
            {
                readPosition += me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
            }
        }
        // The write position is always offset by the byte size as any padding is removed
        writePosition += me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
        // Move on to the next attribute - check for overflow at the same time
        attributeNumber++ ;
        if (attributeNumber >= me->pModbusRegisterTableDataDescriptor->attributeCount)
        {
            attributeNumber = 0 ;
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
 \warning Will return 0 if any errors are encountered
 \test
 \ndate: 2016-10-13
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void FormatToExternalModbus_Register_Table_Multi_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Read position into the read buffer
    TUSIGN16 readPosition ;
    // Write position into the write buffer
    TUSIGN16 writePosition ;
    // Attribute being accessed
    TUSIGN16 attributeNumber ;
    // Count of items being formatted
    TUSIGN16 itemCount ;

    // Set the id of the attribute where to start reading data
    attributeNumber = pModbusRegisterTableRequestLink->endItemAttribute ;
    // Get the read and write start positions
    readPosition = GetRequestLengthModbus_Register_Table_Multi_Buffer(me,
                                                                pModbusRegisterTableRequestLink,
                                                                eTRUE) ;
    // Is this a private command ? If it is, there will be no padding bytes, so the
    // read position and write positions should match
    if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
    {
        writePosition = readPosition ;
    }
    else
    {
        writePosition = GetRequestLengthModbus_Register_Table_Multi_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eFALSE) ;
        // Double the write precision here to give the count of registers as a byte value
        writePosition *= 2 ;
    }

    for (itemCount = 0 ; itemCount < pModbusRegisterTableRequestLink->itemArrayCount ; itemCount++)
    {
        // Update the read and write positions
        readPosition -= me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
        // Check if we need to adjust the write position
        // Firstly - is this a private command and a byte attribute ?
        if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
            (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1))
        {
            // Yes - just move our write index by one (no padding for private commands)
            // And don't bother calling the conversion routine
            writePosition -= 1 ;
        }
        else
        {
            if (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1)
            {
                // Yes - reading bytes out of the buffer
                writePosition -= 2 ;
            }
            else
            {
                writePosition -= me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
            }
            // Convert the next item
            ConvertToExternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                &pBuffer[writePosition],
                                                me->pModbusRegisterTableDataDescriptor->
                                                pTableByteSizes[attributeNumber]) ;
        }
        // Now adjust the attribute
        if (attributeNumber == 0)
        {
            attributeNumber = ((me->pModbusRegisterTableDataDescriptor->attributeCount) - 1) ;
        }
        else
        {
            attributeNumber-- ;
        }
    }
}