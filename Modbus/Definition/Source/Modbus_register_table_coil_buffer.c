//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         Subsystem MODBUS
 Module         MODBUS_register_table_coil_buffer.c
 Description    Modbus message buffer handling functions for coil type register tables
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
#include "Modbus/Source/Modbus_register_table_coil_buffer.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"

TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                                      const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                      TModbusRegisterTableRequestLink
                                                                      *pModbusRegisterTableRequestLink);

TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                                    const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                    TModbusRegisterTableRequestLink
                                                                    *pModbusRegisterTableRequestLink);

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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetTableLinkModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                       const TModbusRegisterTableRequestDetails
                                                       *pModbusRegisterTableRequestDetails,
                                                       TModbusRegisterTableRequestLink
                                                       *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK;

    // Get the starting array index and attribute of the first requested object
    //lint -e{838}
    result = GetStartingObjectArrayIndexModbus_Register_Table_Coil_Buffer(me,
                                                                          pModbusRegisterTableRequestDetails,
                                                                          pModbusRegisterTableRequestLink);
    if (result == OK)
    {
        // Get the ending array index and attribute of the last requested object
        result = GetEndingObjectArrayIndexModbus_Register_Table_Coil_Buffer(me,
                                                                            pModbusRegisterTableRequestDetails,
                                                                            pModbusRegisterTableRequestLink);
    }
    return(result);
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetStartingObjectArrayIndexModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                                      const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                      TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR;

    // We can check here if an out of range address has been requested because we know each
    // coil takes up one slot in the register table
    if ((pRequestDetails->startRegisterAddress >= me->modbusRegisterTableRange.lowerRange) &&
        ((pRequestDetails->startRegisterAddress + pRequestDetails->itemCount) <= me->modbusRegisterTableRange.upperRange))
    {
        // Coil numbers are always going to increase by 1 for sucessive coils. Just get the
        // offset for the table . . .
        pModbusRegisterTableRequestLink->startItemArrayIndex =
        pRequestDetails->startRegisterAddress - me->modbusRegisterTableRange.lowerRange;
        // The starting register/coil table index doesn't change
        pModbusRegisterTableRequestLink->startItemRegisterAddress = pRequestDetails->startRegisterAddress;
        // Attribute is always 0 for coil tables
        pModbusRegisterTableRequestLink->startItemAttribute = 0;
        // Start array index and register found OK
        result = OK;
    }
    return(result);
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715,818}
TUSIGN16 GetEndingObjectArrayIndexModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                                    const TModbusRegisterTableRequestDetails *pRequestDetails,
                                                                    TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = OK;

    // Coil commands always have one coil per register address / array index
    // Set the end item end address
    pModbusRegisterTableRequestLink->endItemRegisterAddress =
    (pModbusRegisterTableRequestLink->startItemRegisterAddress + pRequestDetails->itemCount);
    // The array index of the last requested item
    pModbusRegisterTableRequestLink->endItemArrayIndex =
    (pModbusRegisterTableRequestLink->startItemArrayIndex + pRequestDetails->itemCount) - 1;
    // The attribute of the last item (always 0 for coils)
    pModbusRegisterTableRequestLink->endItemAttribute = 0;
    // The count of requested items
    pModbusRegisterTableRequestLink->itemArrayCount =
    (pModbusRegisterTableRequestLink->endItemArrayIndex - pModbusRegisterTableRequestLink->startItemArrayIndex) + 1;
    return(result);
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715}
TUSIGN16 GetRequestLengthModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                           const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                           TBOOL byteLength)
{
    TUSIGN16 requestLength = 0;

    // For coils, we get 8 coils into every byte - we can ignore the bytelength
    // parameter here, because coils are packed into bytes
    if ((pModbusRegisterTableRequestLink->itemArrayCount % 8) > 0)
    {
        // If the coils won't fit into a multiple of eight, we know
        // we'll need at least one (possibly) extra byte
        requestLength = 1;
    }
    // Add the number of coils that will fit into a multiple of eight
    requestLength += (pModbusRegisterTableRequestLink->itemArrayCount / 8);
    return(requestLength);
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang,GL
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715}
TUSIGN16 CheckForObjectOverlapModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                                const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                                                                const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink)
{
    TUSIGN16 result = FATAL_ERROR;

    // We can only check here if there are enough coils in the table, the command
    // type doesn't matter
    if ((pModbusRegisterTableRequestLink->startItemArrayIndex + pModbusRegisterTableRequestLink->itemArrayCount) <= me->objectCount)
    {
        // Enough coils to access
        result = OK;
    }
    return(result);
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715,830}
void FormatToInternalModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                       const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                                                       const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                       TUSIGN8 *pBuffer)
{
    
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
 date: 2016-10-10
\n by: GL,ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715,830}
void FormatToExternalModbus_Register_Table_Coil_Buffer(const TModbusRegisterTable *me,
                                                       const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                                                       const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                       TUSIGN8 *pBuffer)
{
    
}