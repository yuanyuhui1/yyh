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
#include "Modbus/Source/Modbus_register_table_string_buffer.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_formatting.h"

TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink) ;

TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
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
TUSIGN16 GetTableLinkModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK ;

    // Get the starting array index and attribute of the first requested object
    //lint -e{838}
    result = GetStartingObjectArrayIndexModbus_Register_Table_String_Buffer(me,
                                                    pModbusRegisterTableRequestDetails,
                                                    pModbusRegisterTableRequestLink)  ;
    if (result == OK)
    {
        // Get the ending array index and attribute of the last requested object
        result = GetEndingObjectArrayIndexModbus_Register_Table_String_Buffer(me,
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
TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink)
{
    TUSIGN16 stringStartIndex = 0 ;
    TUSIGN16 stringEndIndex = 0 ;
    TUSIGN16 stringCount ;
    TBOOL stringFound = eFALSE ;
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 registerIndex ;

    // Check the requested index is in range
    if ((pRequestDetails->startRegisterAddress >= me->modbusRegisterTableRange.lowerRange) &&
        (pRequestDetails->startRegisterAddress <= me->modbusRegisterTableRange.upperRange))
    {
        stringStartIndex = me->modbusRegisterTableRange.lowerRange ;
        stringEndIndex = me->modbusRegisterTableRange.lowerRange ;
        // Work through the strings in the table until we reach the requested item
        for (stringCount = 0 ; (stringCount < me->objectCount) && (stringFound == eFALSE) ; )
        {
            // Update the end indexes of the current string being checked
            //lint -e{734} CNZUWAN
            stringEndIndex += me->pModbusRegisterTableItems[stringCount].itemDetails *
                    me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
            // Does the item fit within this string entry ?
            if ((pRequestDetails->startRegisterAddress >= stringStartIndex) &&
                (pRequestDetails->startRegisterAddress < stringEndIndex))
            {
                // Found our string
                stringFound = eTRUE ;
                // Now determine the start attribute
                registerIndex = pRequestDetails->startRegisterAddress - stringStartIndex ;
                pModbusRegisterTableRequestLink->startItemAttribute = registerIndex
                  / me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
                // And the start address
                    //lint -e{734} CNZUWAN
                pModbusRegisterTableRequestLink->startItemRegisterAddress = stringStartIndex +
                  (me->pModbusRegisterTableDataDescriptor->totalRegisterSize *
                   pModbusRegisterTableRequestLink->startItemAttribute) ;
            }
            else
            {
                // Check the next
                stringCount++ ;
                stringStartIndex = stringEndIndex ;
            }
        }
        // Did we find the requested string ?
        if (stringFound == eTRUE)
        {
            // We can set the initial array index now
            pModbusRegisterTableRequestLink->startItemArrayIndex = stringCount ;
            result = OK ;
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
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                TModbusRegisterTableRequestLink
                                                                  *pModbusRegisterTableRequestLink)
{
  TUSIGN16 result = OK ;
  TUSIGN16 stringCount ;
  TUSIGN16 registerCount = 0 ;
  TBOOL stringFound = eFALSE ;
  TUSIGN16 attributeCount ;
  TUSIGN16 registerAddress ;

  if (pRequestDetails->isPrivateRequest == eTRUE)
  {
    // Set the index of the last string that will be accessed
    pModbusRegisterTableRequestLink->endItemArrayIndex = (pModbusRegisterTableRequestLink->startItemArrayIndex +
    (pRequestDetails->itemCount - 1)) ;
    // The last attribute of the last string that will be accessed
    pModbusRegisterTableRequestLink->endItemAttribute = (me->pModbusRegisterTableItems
    [pModbusRegisterTableRequestLink->endItemArrayIndex].itemDetails - 1) ;
    // The count of strings that will be accessed
    pModbusRegisterTableRequestLink->itemArrayCount = pRequestDetails->itemCount ;
    // The register address of the last attribute
    pModbusRegisterTableRequestLink->endItemRegisterAddress = pModbusRegisterTableRequestLink->startItemRegisterAddress ;
    for (stringCount = pModbusRegisterTableRequestLink->startItemArrayIndex ;
    stringCount <= pModbusRegisterTableRequestLink->endItemArrayIndex ;
        stringCount++)
    {
      //lint -e{734} CNZUWAN      
      pModbusRegisterTableRequestLink->endItemRegisterAddress += (me->pModbusRegisterTableItems
      [stringCount].itemDetails * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
    }
  }
  else
  {
    // Get the requested count of registers
    registerCount = pRequestDetails->itemCount ;
    // Convert this to attributes
    registerCount /= me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
    // Starting with the first requested string, remove each attribute count until
    // we come to the last requested string
    // Hang on to the starting address of the initial string
    registerAddress = pModbusRegisterTableRequestLink->startItemRegisterAddress ;
    // More than one string requested ?
    attributeCount = me->pModbusRegisterTableItems[pModbusRegisterTableRequestLink->startItemArrayIndex].itemDetails -
    pModbusRegisterTableRequestLink->startItemAttribute ;
    if (attributeCount >= registerCount)
    {
      // No, so we can add the end array index and attribute here

      // Set the last requested table index
      pModbusRegisterTableRequestLink->endItemArrayIndex = pModbusRegisterTableRequestLink->startItemArrayIndex ;
      // The address of the last requested attribute
      //lint -e{734} CNZUWAN
      pModbusRegisterTableRequestLink->endItemRegisterAddress = pModbusRegisterTableRequestLink->startItemRegisterAddress +
      (registerCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
      registerCount-- ;
      // The last requested attribute
      pModbusRegisterTableRequestLink->endItemAttribute = pModbusRegisterTableRequestLink->startItemAttribute +
      registerCount ;
      // Only one array index requested
      pModbusRegisterTableRequestLink->itemArrayCount = 1 ;
    }
    else
    {
      // Keep updating the address of the last requested variable
      //lint -e{734} CNZUWAN
      registerAddress += (attributeCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
      // More than one string requested - first remove the number of attributes requested from the
      // first string
      #pragma diag_suppress=Pm033
      registerCount -= attributeCount ;
      // Now keep reducing the count of requested attributes until we find the requested string
      for (stringCount = pModbusRegisterTableRequestLink->startItemArrayIndex + 1 ;
          (stringCount < me->objectCount) && (stringFound == eFALSE) ;
              stringCount++)
      {
        if (me->pModbusRegisterTableItems[stringCount].itemDetails >= registerCount)
        {
            // Found our string
            pModbusRegisterTableRequestLink->endItemArrayIndex = stringCount ;
            // Set our end address - this is the end address of the attribute
            //lint -e{734} CNZUWAN
            registerAddress += (registerCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
            pModbusRegisterTableRequestLink->endItemRegisterAddress = registerAddress ;
            registerCount-- ;
            // Update our attribute
            pModbusRegisterTableRequestLink->endItemAttribute = registerCount ;
            // Update the count of array indexes involved in the request
            pModbusRegisterTableRequestLink->itemArrayCount = (pModbusRegisterTableRequestLink->endItemArrayIndex
                - pModbusRegisterTableRequestLink->startItemArrayIndex) + 1 ;
            // We can stop searching now
            stringFound = eTRUE ;
        }
        else
        {
            // If not remove the count of attributes from this string and check the next
            attributeCount = me->pModbusRegisterTableItems[stringCount].itemDetails ;
            registerCount -= attributeCount ;
            // Keep updating out rolling end register address
            //lint -e{734} CNZUWAN
            registerAddress += (attributeCount * me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
        }
      }
      #pragma diag_default=Pm033
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
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetRequestLengthModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                          *pModbusRegisterTableRequestLink,
                                                          TBOOL byteLength)
{
    // Length of the response in bytes
    TUSIGN16 requestLength = 0 ;
    // Count of whole objects
    TUSIGN16 wholeObjectCount = 0 ;
    // Set when a number of attributes has to be totalled for the first object
    TBOOL doFirstObject = eFALSE ;
    // Set when a number of attributes has to be totalled for the last object
    TBOOL doLastObject = eFALSE ;
    // Set when a number of attributes have been requested for only one object
    TBOOL doOneObject = eFALSE ;
    // Array index of the string length being added
    TUSIGN16 stringIndex ;
    // Depending on whether we're getting byte or register length, the following are initialised
    // accordingly
    TUSIGN16 attributeSize ;

    // Check if we're getting byte or register length
    if (byteLength == eTRUE)
    {
        attributeSize = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    }
    else
    {
        attributeSize = me->pModbusRegisterTableDataDescriptor->totalRegisterSize ;
    }

    // Are we reading more than two strings ? It has to be more than two
    // as if it's not, we may be straddling between the start and end of two strings
    if (pModbusRegisterTableRequestLink->itemArrayCount > 2)
    {
        // At least one whole string is being read
        wholeObjectCount = pModbusRegisterTableRequestLink->itemArrayCount - 2 ;
    }
    // Do we need to count a number of attributes from the first string ?
    if (pModbusRegisterTableRequestLink->endItemArrayIndex !=
            pModbusRegisterTableRequestLink->startItemArrayIndex)
    {
        if (pModbusRegisterTableRequestLink->startItemAttribute != 0)
        {
            // Yes, we're reading more than one object, and not starting with the first attribute
            doFirstObject = eTRUE ;
        }
        else
        {
            // We can read the whole initial string otherwise
            wholeObjectCount++ ;
        }
        if (pModbusRegisterTableRequestLink->endItemAttribute != (me->
            pModbusRegisterTableItems[pModbusRegisterTableRequestLink->
                endItemArrayIndex].itemDetails - 1))
        {
            // We'll need to count a number of attributes in the last string
            doLastObject = eTRUE ;
        }
        else
        {
            // We can count all of the attributes in the last string
            wholeObjectCount++ ;
        }
    }
    else
    {
        // In this case, we're reading only one string - can we read the whole object ?
        if ((pModbusRegisterTableRequestLink->startItemAttribute == 0) &&
            (pModbusRegisterTableRequestLink->endItemAttribute == me->
            pModbusRegisterTableItems[pModbusRegisterTableRequestLink->
            startItemArrayIndex].itemDetails - 1))
        {
            // Yes, the first and last attributes have been requested
            wholeObjectCount++ ;
        }
        else
        {
            // No, we need to count the number of attributes that have been requested
            doOneObject = eTRUE ;
        }
    }
    // Now we can get the length requested
    stringIndex = pModbusRegisterTableRequestLink->startItemArrayIndex ;
    // Only one item requested ?
    if (doOneObject == eTRUE)
    {
        //lint -e{734} CNZUWAN
        requestLength = (attributeSize *
            ((pModbusRegisterTableRequestLink->endItemAttribute -
                pModbusRegisterTableRequestLink->startItemAttribute) + 1)) ;
    }
    else
    {
        // Do we need to get the total for the first object ?
        if (doFirstObject == eTRUE)
        {
            // If so, starting with the first requested attribute, get the length
            //lint -e{734} CNZUWAN
            requestLength = (attributeSize *
                (me->pModbusRegisterTableItems[stringIndex].itemDetails -
                    pModbusRegisterTableRequestLink->startItemAttribute)) ;
            // Details of this string have been added
            stringIndex++ ;
        }
        // We now need to add the length of each whole string requested
        while(wholeObjectCount--)
        {
            //lint -e{734} CNZUWAN
            requestLength += (attributeSize *
                me->pModbusRegisterTableItems[stringIndex++].itemDetails) ;
        }
        // Do we need to add details of the last string ?
        if (doLastObject == eTRUE)
        {
            // Add the size of the number of attributes requested for the last string
            //lint -e{734} CNZUWAN
            requestLength += (attributeSize *
                (pModbusRegisterTableRequestLink->endItemAttribute + 1)) ;
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
TUSIGN16 CheckForObjectOverlapModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                           const  TModbusRegisterTableRequestDetails
                                                            *pModbusRegisterTableRequestDetails,
                                                            const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR ;
    TUSIGN16 stringItemCount = 0 ;
    TUSIGN16 stringItemSize = 0 ;

    // Private request ?
    if (pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE)
    {
        // Is the first requested register at the start of a string ?
        // Check that the first attribute has been requested
        if (pModbusRegisterTableRequestLink->startItemAttribute == 0)
        {
            // Yes, has the correct address been requested ?
            if (pModbusRegisterTableRequestLink->startItemRegisterAddress ==
                pModbusRegisterTableRequestDetails->startRegisterAddress)
            {
                // All checks OK
                result = OK ;
            }
        }
    }
    else
    {
        // Does the initial attribute address match that of the
        // address passed in the request ?
        if (pModbusRegisterTableRequestLink->startItemRegisterAddress ==
                pModbusRegisterTableRequestDetails->startRegisterAddress)
        {
            // Yes, does the end address match that of the last requested attribute ?
            if ((pModbusRegisterTableRequestDetails->startRegisterAddress +
                    pModbusRegisterTableRequestDetails->itemCount) ==
                pModbusRegisterTableRequestLink->endItemRegisterAddress)
            {
                // Ensure the last requested register is occupied. We need to 
                // get the size here manually as strings are allowed to change in size   
                for (stringItemCount = 0 ; stringItemCount < me->objectCount ; stringItemCount++)                           
                {
                    //lint -e{734} CNZUWAN
                    stringItemSize += (me->pModbusRegisterTableItems[stringItemCount].itemDetails *
                                        me->pModbusRegisterTableDataDescriptor->totalRegisterSize) ;
                }
                if (pModbusRegisterTableRequestLink->endItemRegisterAddress <=
                    (me->modbusRegisterTableRange.lowerRange + stringItemSize))
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
 \ndate: 2016-10-17
 \n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void FormatToInternalModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Count of items added to the buffer
    TUSIGN16 itemCount ;
    // Read position in the buffer
    TUSIGN16 readPosition = 0 ;
    // Write position in the buffer
    TUSIGN16 writePosition = 0 ;
    // Id of the string currently being accessed
    TUSIGN16 stringId ;
    // Maximum number of items to read from the current string
    TUSIGN16 itemsMax ;
    // Offset for the write index
    TUSIGN16 writePositionOffset ;
    // Offset for the read index
    TUSIGN16 readPositionOffset ;
    // Set when the message format is complete
    TBOOL messageFormatted = eFALSE ;

    // Set the read and write offsets
    writePositionOffset = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    readPositionOffset = writePositionOffset ;
    // Formatting a byte message ?
    if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
        (readPositionOffset == 1))
    {
        messageFormatted = eTRUE ;
    }
    // Do we actually need to format the message ?
    if (messageFormatted == eFALSE)
    {
        if (readPositionOffset == 1)
        {
            // If we're going to be reading bytes, read out two at a time
            // to cater for additional padding bytes
            readPositionOffset = 2 ;
        }
        // The outer loop counts the number of strings being converted in the buffer
        for (stringId = pModbusRegisterTableRequestLink->startItemArrayIndex ;
                stringId <= pModbusRegisterTableRequestLink->endItemArrayIndex ;
                    stringId++)
        {
            // Initial string ? If so, check if only one string has been requested
            if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
            {
                // Only one string - set the attribute count here
                itemsMax = (pModbusRegisterTableRequestLink->endItemAttribute -
                    pModbusRegisterTableRequestLink->startItemAttribute) + 1 ;
            }
            // Last string ? Check how many of its attributes have been requested
            else if (stringId == pModbusRegisterTableRequestLink->endItemArrayIndex)
            {
                // Limit the number of items that should be read here. Offset by one
                // as we're using a < for the loop and the last attribute is 0 based
                itemsMax = pModbusRegisterTableRequestLink->endItemAttribute + 1 ;
            }
            // First string not starting with the first attribute ?
            else if ((stringId == pModbusRegisterTableRequestLink->startItemArrayIndex) &&
                     (pModbusRegisterTableRequestLink->startItemAttribute != 0))
            {
                itemsMax = me->pModbusRegisterTableItems[stringId].itemDetails -
                  pModbusRegisterTableRequestLink->startItemAttribute ;
            }
            else
            {
                // If none of the above are true, read all of the string attributes
                itemsMax = me->pModbusRegisterTableItems[stringId].itemDetails ;
            }
            // Now convert the items to internal format
            for (itemCount = 0 ; itemCount < itemsMax ; itemCount++)
            {
                ConvertToInternalModbus_Formatting((void*)&pBuffer[readPosition],
                                                   &pBuffer[writePosition],
                                                    writePositionOffset) ;
                // Update the read and write positions
                writePosition += writePositionOffset ;
                readPosition += readPositionOffset ;
            }
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
void FormatToExternalModbus_Register_Table_String_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer)
{
    // Current read position in the buffer
    TUSIGN16 readPosition ;
    // Current write position in the buffer
    TUSIGN16 writePosition ;
    // Offset for the read position
    TUSIGN16 readPositionOffset ;
    // Offset for the write position
    TUSIGN16 writePositionOffset ;
    // Max number of items to be read from the current string
    TUSIGN16 itemsMax ;
    // Id of string being accessed
    TUSIGN16 stringId ;
    // Item being added to the buffer
    TUSIGN16 itemNumber ;
    // Set when the message is formatted
    TBOOL messageFormatted = eFALSE ;

    // The read size is the size of each item in bytes when converting to external
    readPositionOffset = me->pModbusRegisterTableDataDescriptor->totalByteSize ;
    writePositionOffset = readPositionOffset ;
    // Private command and a byte message ? If so, nothing needs to be done
    if ((pModbusRegisterTableRequestDetails->isPrivateRequest == eTRUE) &&
        (writePositionOffset == 1))
    {
        messageFormatted = eTRUE ;
    }
    // Do we need to format the message ?
    if (messageFormatted == eFALSE)
    {
        if (writePositionOffset == 1)
        {
            // If we're reading bytes from the buffer, we'll be adding two bytes back in place
            writePositionOffset = 2 ;
        }
        // The start positions for the read and write buffers are given by the lengths in bytes and registers
        readPosition = GetRequestLengthModbus_Register_Table_String_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eTRUE) ;
        writePosition = GetRequestLengthModbus_Register_Table_String_Buffer(me,
                                                                    pModbusRegisterTableRequestLink,
                                                                    eFALSE) ;
        // Double the write position here to give the count of registers in bytes
        writePosition *= 2 ;

        // Work through each requested string
        for (stringId = pModbusRegisterTableRequestLink->startItemArrayIndex ;
                stringId <= pModbusRegisterTableRequestLink->endItemArrayIndex ;
                    stringId++)
        {
            // Only one string requested ? This will limit the number of attributes we have to read from it
            if (pModbusRegisterTableRequestLink->itemArrayCount == 1)
            {
                // Yes, set the number of items to read here
                itemsMax = (pModbusRegisterTableRequestLink->endItemAttribute -
                    pModbusRegisterTableRequestLink->startItemAttribute) + 1 ;
            }
            // Reading the last string ? Again this changes how many attributes to read
            else if (stringId == pModbusRegisterTableRequestLink->endItemArrayIndex)
            {
                // Limit the number of items to read - offset by 1 here to account for the
                // < in the for loop and the 0 based index of the end attribute
                itemsMax = pModbusRegisterTableRequestLink->endItemAttribute + 1 ;
            }
            // First string and not starting with the first attribute ?
            else if ((stringId == pModbusRegisterTableRequestLink->startItemArrayIndex) &&
                     (pModbusRegisterTableRequestLink->startItemAttribute != 0))
            {
                itemsMax = me->pModbusRegisterTableItems[stringId].itemDetails -
                  pModbusRegisterTableRequestLink->startItemAttribute ;
            }
            else
            {
                // If neither of the above are true, read all of the attributes
                itemsMax = me->pModbusRegisterTableItems[stringId].itemDetails ;
            }
            for (itemNumber = 0 ; itemNumber < itemsMax ; itemNumber++)
            {
                // Offset the read and write indexes first
                readPosition -= readPositionOffset ;
                writePosition -= writePositionOffset ;
                // Convert the item
                ConvertToExternalModbus_Formatting((void *)&pBuffer[readPosition],
                                                    &pBuffer[writePosition],
                                                    readPositionOffset) ;
            }
        }
    }
}