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
//#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
//#include "..\..\system\interface\assert.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_table_whole_object_buffer.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_formatting.h"

TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                                       const  TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                        TModbusRegisterTableRequestLink
                                                                          *pModbusRegisterTableRequestLink) ;

TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
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
 \n date: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetTableLinkModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails
                                                                *pModbusRegisterTableRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK ;

    // Get the starting array index and attribute of the first requested object
    //lint -e{838}
    result = GetStartingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(me,
                                                    pModbusRegisterTableRequestDetails,
                                                    pModbusRegisterTableRequestLink)  ;
    if (result == OK)
    {
        // Get the ending array index and attribute of the last requested object
        result = GetEndingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(me,
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
 \n date: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                                       const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                        TModbusRegisterTableRequestLink
                                                                          *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 attributeCount ;
    TBOOL attributeFound = eFALSE ;
    TUSIGN16 registerCount ;
    TUSIGN16 arrayIndexCount = 0 ;
    TUSIGN16 wholeObjectCount ;


    if ((pRequestDetails->startRegisterAddress >= me->modbusRegisterTableRange.lowerRange) &&
        (pRequestDetails->startRegisterAddress <= me->modbusRegisterTableRange.upperRange))
    {
        registerCount = me->modbusRegisterTableRange.lowerRange ;
        // Determine how many whole objects reside before the requested register start
        wholeObjectCount = (pRequestDetails->startRegisterAddress - me->modbusRegisterTableRange.lowerRange) /
            me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
        // Add the size to the total count of registers
        //lint -e{734}
        registerCount += (wholeObjectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
        // This is also the number of array indexes in the register table we have to jump
        arrayIndexCount = wholeObjectCount ;
        // Found our array index, now find the attribute
        pModbusRegisterTableRequestLink->startItemArrayIndex = arrayIndexCount ;
        for (attributeCount = 0 ;
             (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount) &&
               (attributeFound == eFALSE) ;
              attributeCount++)
        {
            registerCount += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
            if (pRequestDetails->startRegisterAddress < registerCount)
            {
                attributeFound = eTRUE ;
                pModbusRegisterTableRequestLink->startItemAttribute = attributeCount ;
                // Also store the start address of the attribute at this point
                pModbusRegisterTableRequestLink->startItemRegisterAddress = (registerCount -
                    me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount]) ;
                result = OK ;
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
 \n date: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 registerCount ;
    TUSIGN16 attributeCount ;
    TBOOL attributeFound = eFALSE ;
    TUSIGN16 attributeSizes = 0 ;
    TUSIGN16 arrayIndexes = 0 ;
    TUSIGN16 wholeObjectCount ;
    TUSIGN16 firstObjectAttributeSizes = 0 ;

    if (pRequestDetails->isPrivateRequest == eTRUE)
    {
        // If it's a private request, the end array index is just the count
        // of whole objects requested and the start index
        pModbusRegisterTableRequestLink->endItemArrayIndex =
          pModbusRegisterTableRequestLink->startItemArrayIndex +
                (pRequestDetails->itemCount - 1) ;
        // End item register index
        //lint -e{734}
        pModbusRegisterTableRequestLink->endItemRegisterAddress =
            pModbusRegisterTableRequestLink->startItemRegisterAddress +
                (pRequestDetails->itemCount *
                    me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
        // Item count
        pModbusRegisterTableRequestLink->itemArrayCount = pRequestDetails->itemCount ;
        result = OK ;
        // End item attribute is always the last attribute
        pModbusRegisterTableRequestLink->endItemAttribute = (me->
            pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
    }
    else
    {
        // The count of registers we need to allocate
        registerCount = pRequestDetails->itemCount ;
        // Preset the array index being accessed to the start array index of the request
        arrayIndexes = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        // First determine if it's just one or several attributes from the initial item
        for (attributeCount = pModbusRegisterTableRequestLink->startItemAttribute ;
             (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount) &&
                (attributeFound == eFALSE) ;
                    attributeCount++)
        {
            firstObjectAttributeSizes += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
            if (registerCount <= firstObjectAttributeSizes)
            {
                // Found our attribute - the end array index will match the start
                pModbusRegisterTableRequestLink->endItemArrayIndex =
                    pModbusRegisterTableRequestLink->startItemArrayIndex ;
                // Set the attribute
                pModbusRegisterTableRequestLink->endItemAttribute = attributeCount ;
                // The end register address of the last attribute
                pModbusRegisterTableRequestLink->endItemRegisterAddress =
                    pModbusRegisterTableRequestLink->startItemRegisterAddress + firstObjectAttributeSizes ;
                // The count of array items is one in this case
                pModbusRegisterTableRequestLink->itemArrayCount = 1 ;
                attributeFound = eTRUE ;
                result = OK ;
            }
        }
        // Was only one whole object set of attributes requested ?
        if (attributeFound == eFALSE)
        {
            arrayIndexes++ ;
            // No, so first offset by the number of registers from the whole object
            registerCount -= firstObjectAttributeSizes ;
            // First offset by the total number of whole objects requested

            wholeObjectCount = registerCount/me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
            //lint -e{734}   
            registerCount -= (wholeObjectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
            // However many complete whole object requests determines the additional count
            // of register table array indexes
            arrayIndexes += wholeObjectCount ;
            // All registers accounted for ?
            if (registerCount == 0)
            {
                // Yes, so we can stop searching now
                // The end attribute is the last attribite of the last table item
                pModbusRegisterTableRequestLink->endItemAttribute =
                    (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
                // The end register index can be found by adding the count of registers
                // from the first table item and all of the registers from the whole object calculation
                //lint -e{734}
                pModbusRegisterTableRequestLink->endItemRegisterAddress =
                    (pModbusRegisterTableRequestLink->startItemRegisterAddress + firstObjectAttributeSizes +
                    (wholeObjectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize)) ;
                // We already know the end array index
                pModbusRegisterTableRequestLink->endItemArrayIndex = (arrayIndexes - 1) ;
                // Finally set the count of array indexes
                if (pModbusRegisterTableRequestLink->startItemArrayIndex == 0)
                {
                    pModbusRegisterTableRequestLink->itemArrayCount =
                        (pModbusRegisterTableRequestLink->endItemArrayIndex + 1) ;
                }
                else
                {
                    pModbusRegisterTableRequestLink->itemArrayCount =
                        (pModbusRegisterTableRequestLink->endItemArrayIndex
                            - pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
                }
                result = OK ;
            }
            else
            {
                // Now determine the end requested attribute
                //arrayIndexes++ ;
                attributeSizes = 0 ;
                // First determine if it's just one or several attributes from the initial item
                for (attributeCount = 0 ;
                     (attributeCount < me->pModbusRegisterTableDataDescriptor->attributeCount) &&
                        (attributeFound == eFALSE) ;
                            attributeCount++)
                {
                    attributeSizes += me->pModbusRegisterTableDataDescriptor->pTableRegisterSizes[attributeCount] ;
                    if (registerCount <= attributeSizes)
                    {
                        // Found our attribute
                        pModbusRegisterTableRequestLink->endItemArrayIndex = arrayIndexes ;
                        pModbusRegisterTableRequestLink->endItemAttribute = attributeCount ;
                        // Set the end register address
                        //lint -e{734}
                        pModbusRegisterTableRequestLink->endItemRegisterAddress =
                            (pModbusRegisterTableRequestLink->startItemRegisterAddress + firstObjectAttributeSizes +
                            (wholeObjectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) + attributeSizes) ;
                        // And the count of array indexes
                        pModbusRegisterTableRequestLink->itemArrayCount =
                            (pModbusRegisterTableRequestLink->endItemArrayIndex -
                                pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
                        attributeFound = eTRUE ;
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
 \brief   Returns the size of a message in registers. Either we're at the start, in case we may
 \brief   start midway through an object at the end, in which case we may end midway though an object
 \brief   or in the middle, which may or may not be a whole object . . . .
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
 \test
 \n date: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetRequestLengthModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                          *pModbusRegisterTableRequestLink,
                                                          TBOOL byteLength)
{
    TUSIGN16 currentAttribute ;
    TUSIGN16 requestLength = 0 ;
    TUSIGN16 wholeObjectCount = 0 ;
    TBOOL doFirstItem = eTRUE ;
    TBOOL doLastItem = eTRUE ;
    TBOOL singleItem = eFALSE ;
    // Depending on whether we're getting the byte or register size, the following
    // are initialised accordingly
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

    // Get the count of whole objects to read - first check that we're reading from more than
    // one object
    if (pModbusRegisterTableRequestLink->itemArrayCount > 1)
    {
        // And also that at least three objects are being read from
        if (pModbusRegisterTableRequestLink->itemArrayCount >= 3)
        {
            // Offset the calculated count by 2 (first and last objects) - we decide what to do with these next
            wholeObjectCount = pModbusRegisterTableRequestLink->itemArrayCount - 2 ;
        }
        // Can we read the whole first object ?
        if (pModbusRegisterTableRequestLink->startItemAttribute == 0)
        {
            // Yes - add it here
            wholeObjectCount++ ;
            doFirstItem = eFALSE ;
        }
        // Can we read the whole last object ?
        if (pModbusRegisterTableRequestLink->endItemAttribute ==
            (me->pModbusRegisterTableDataDescriptor->attributeCount - 1))
        {
            // Yes, add it here
            wholeObjectCount++ ;
            doLastItem = eFALSE ;
        }
    }
    else
    {
        // Only one object being read - can we read the whole object ?
        if ((pModbusRegisterTableRequestLink->startItemAttribute == 0) &&
            (pModbusRegisterTableRequestLink->endItemAttribute ==
            (me->pModbusRegisterTableDataDescriptor->attributeCount - 1)))
        {
            // Yes, add it here
            wholeObjectCount++ ;
            doFirstItem = eFALSE ;
            doLastItem = eFALSE ;
        }
        else
        {
            // No - some attributes have been requested, but we'll have to get the size
            // for the individual elements
            singleItem = eTRUE ;
            doFirstItem = eFALSE ;
            doLastItem = eFALSE ;
        }
    }
    // Add the count of whole objects
    //lint -e{734,838}
    requestLength = totalSize * wholeObjectCount ;
    // Do we need to add a number of attributes from the initial item ?
    if (doFirstItem == eTRUE)
    {
        for (currentAttribute = pModbusRegisterTableRequestLink->startItemAttribute ;
                currentAttribute < me->pModbusRegisterTableDataDescriptor->attributeCount ;
                    currentAttribute++)
        {
            requestLength += pAttributeSize[currentAttribute] ;
        }
    }
    // Do we need to add a number of attributes from the last item ?
    if (doLastItem == eTRUE)
    {
        for (currentAttribute = 0 ;
                currentAttribute <= pModbusRegisterTableRequestLink->endItemAttribute ;
                    currentAttribute++)
        {
            requestLength += pAttributeSize[currentAttribute] ;
        }
    }
    // Is it a single item ?
    if (singleItem == eTRUE)
    {
        for (currentAttribute = pModbusRegisterTableRequestLink->startItemAttribute ;
                currentAttribute <= pModbusRegisterTableRequestLink->endItemAttribute ;
                    currentAttribute++)
        {
            requestLength += pAttributeSize[currentAttribute] ;
        }
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
TUSIGN16 CheckForObjectOverlapModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails
                                                                *pModbusRegisterTableRequestDetails,
                                                                const TModbusRegisterTableRequestLink
                                                                *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;

    // Private or public command ?
    if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
    {
        // Does the first item start at the start of a whole object ?
        if (pModbusRegisterTableRequestDetails->startRegisterAddress ==
            (me->modbusRegisterTableRange.lowerRange +
            (me->pModbusRegisterTableDataDescriptor->totalRegisterSize *
                pModbusRegisterTableRequestLink->startItemArrayIndex)))
        {
            // Yes, are there enough objects for the request ?
            if ((pModbusRegisterTableRequestLink->startItemArrayIndex +
                    pModbusRegisterTableRequestLink->itemArrayCount) <=
                me->objectCount)
            {
                // Yes, so this request is OK
                result = OK ;
            }
        }
    }
    else
    {
        // Does the first requested register match that of the first attribute ?
        if (pModbusRegisterTableRequestDetails->startRegisterAddress ==
            pModbusRegisterTableRequestLink->startItemRegisterAddress)
        {
            // Yes, does the last register address match that of
            // the last requested attribute ?
            if ((pModbusRegisterTableRequestDetails->startRegisterAddress +
                    pModbusRegisterTableRequestDetails->itemCount) ==
                        pModbusRegisterTableRequestLink->endItemRegisterAddress)
            {
                // Does the end register reside within the table ?
                if (pModbusRegisterTableRequestLink->endItemRegisterAddress <=                        
                        (me->modbusRegisterTableRange.lowerRange +
                            (me->objectCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize)))
                {
                    // Yes, request details are OK
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
 \n date: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void FormatToInternalModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Read index into the buffer
    TUSIGN16 readPosition = 0 ;
    // Write index into the buffer
    TUSIGN16 writePosition = 0 ;
    // Current array index being accessed
    TUSIGN16 arrayIndexId ;
    // Current attribute being accessed
    TUSIGN16 attributeNumber ;
    TUSIGN16 attributeNumberMax ;

    for (arrayIndexId = 0 ; arrayIndexId < pModbusRegisterTableRequestLink->itemArrayCount ; arrayIndexId++)
    {
        // Only one whole object selected ?
        if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
        {
            // If so, set the start and end attributes accordingly
            attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
            attributeNumberMax = pModbusRegisterTableRequestLink->endItemAttribute + 1 ;
        }
        // Last requested whole object ?
        else if (arrayIndexId == pModbusRegisterTableRequestLink->endItemArrayIndex)
        {
            // If we're on the last whole object, set the count of attributes accordingly
            attributeNumber = 0 ;
            attributeNumberMax = pModbusRegisterTableRequestLink->endItemAttribute + 1 ;
        }
        // Start object and not starting with the first attribute ?
        else if ((arrayIndexId == 0) && (pModbusRegisterTableRequestLink->startItemAttribute != 0))
        {
            attributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
            attributeNumberMax = me->pModbusRegisterTableDataDescriptor->attributeCount ;
        }
        else
        {
            // If none of the above are true, read all attributes
            attributeNumber = 0 ;
            attributeNumberMax = me->pModbusRegisterTableDataDescriptor->attributeCount ;
        }
        // Read the attributes
        for (;attributeNumber < attributeNumberMax ; attributeNumber++)
        {
            // Is this a private command reading a byte attribute ?
            if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
                (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1))
            {
                // Just update the read position - no conversion is necessary
                readPosition++ ;
            }
            else
            {
                // First read in the item
                ConvertToInternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                    &pBuffer[writePosition],
                                                    me->pModbusRegisterTableDataDescriptor->
                                                    pTableByteSizes[attributeNumber]) ;

                if (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1)
                {
                    readPosition += 2 ;
                }
                else
                {
                    readPosition += me->pModbusRegisterTableDataDescriptor->
                        pTableByteSizes[attributeNumber] ;
                }
            }
            // Update the write position
            writePosition += me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
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
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void FormatToExternalModbus_Register_Table_Whole_Object_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Read position into the buffer
    TUSIGN16 readPosition ;
    // Write position into the buffer
    TUSIGN16 writePosition ;
    // Array index being accessed
    TUSIGN16 arrayIndex ;
    // Attribute being accessed
    TUSIGN16 attributeNumber ;
    // Last attribute to access
    TUSIGN16 lastAttributeNumber ;
    // Count of attributes to convert for the current whole object
    TUSIGN16 attributeCount ;

    // Get the length of the message in bytes and registers. This gives the start read and write positions
    readPosition = GetRequestLengthModbus_Register_Table_Whole_Object_Buffer(me,
                                                                pModbusRegisterTableRequestLink,
                                                                eTRUE) ;
    // Is this a private command ? If it is, the read and write positions should match
    if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
    {
        writePosition = readPosition ;
    }
    else
    {
        writePosition = GetRequestLengthModbus_Register_Table_Whole_Object_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eFALSE) ;
        // Double the write precision so the count of registers is in bytes
        writePosition *= 2 ;
    }

    // Now convert the values into the buffer
    for (arrayIndex = 0 ; arrayIndex < pModbusRegisterTableRequestLink->itemArrayCount ; arrayIndex++)
    {
        // Only one item ? This will limit the number of attributes we need to access
        if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
        {
            lastAttributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
            attributeNumber = pModbusRegisterTableRequestLink->endItemAttribute ;
        }
        // Is this the first item in the list (reading backwards when converting to external) ?
        else if (arrayIndex == (pModbusRegisterTableRequestLink->itemArrayCount - 1))
        {
            // Yes, so set the first attribute accoringly
            attributeNumber = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            lastAttributeNumber = pModbusRegisterTableRequestLink->startItemAttribute ;
        }
        // Is it the first item and we're not starting with the initial attribute ?
        else if ((arrayIndex == 0) &&
                    (pModbusRegisterTableRequestLink->endItemAttribute !=
                    (me->pModbusRegisterTableDataDescriptor->attributeCount - 1)))
        {
            // No, so set the attribute numbers accordingly
            attributeNumber = pModbusRegisterTableRequestLink->endItemAttribute ;
            lastAttributeNumber = 0 ;
        }
        else
        {
            // Read all of the attributes
            attributeNumber = (me->pModbusRegisterTableDataDescriptor->attributeCount - 1) ;
            lastAttributeNumber = 0 ;
        }
        // Convert the attributes into the buffer
        attributeCount = (attributeNumber - lastAttributeNumber) + 1 ;
        while(attributeCount--)
        {
            // First offset the read and write positions
            readPosition -= me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
            // Is this a private command reading a byte variable ?
            if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
                (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1))
            {
                // Yes, so just reduce the write position by 1 and don't do a conversion
                writePosition-- ;
            }
            else
            {
                // Reading bytes ?
                if (me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] == 1)
                {
                    writePosition -= 2 ;
                }
                else
                {
                    writePosition -= me->pModbusRegisterTableDataDescriptor->pTableByteSizes[attributeNumber] ;
                }
                // Convert the value
                ConvertToExternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                    &pBuffer[writePosition],
                                                    me->pModbusRegisterTableDataDescriptor->
                                                    pTableByteSizes[attributeNumber]) ;
            }
            // Update the attribute number
            attributeNumber-- ;
        }
    }
}