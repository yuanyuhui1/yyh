//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_formatting.c
 Description    Routines for reading from and writing to Modbus message buffers

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "Modbus/Source/Modbus_formatting.h"

// For accessing the object used to determine the IEEE format
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"
#include "Modbus/Source/Modbus_types.h"
#include "Modbus/Source/Modbus_attributes.h"


//lint --e{793} by CNZUWAN AS code review is accetable.
//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts the requested type into Modbus format data
 \author  GL
 \date    12/08/08
 \param   [in] pValue - Void reference to the value to format into the message buffer
 \param   [out] pBuffer - Reference to the message buffer to hold the Modbus formatted data
 \param   [in] byteCount - Number of bytes to format into the buffer
 \return  void
 \warning Will only format up to the largest system data type (TDOUBLE)
 \test
 date: 2016-09-01
 \n by: ZuoChen Wang.
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void ConvertToExternalModbus_Formatting(const void *pValue,TUSIGN8 *pBuffer, TUSIGN16 byteCount)
{
    // Set our local buffer to the size of the largest variable that can be loaded into the message buffer
    TUSIGN8 tempBuffer[sizeof(TDOUBLE)] ;
    // Number of bytes to be added to the message buffer
    TUSIGN16 bytesDone = byteCount ;
    // Number of the byte currently being added to the message buffer
    TUSIGN16 byteNumber = 0 ;
    // Word currently being added to the message buffer
    TUSIGN16 wordNumber = 0 ;
    // Value of word being added to the message buffer
    TUSIGN8 wordBuffer[2] ;

    // Do we need to add a padding byte ?
    if (byteCount == 1)
    {
        // Protect here from overwriting a byte from the same buffer
        // (pValue and pBuffer might be the same . . . .)
        tempBuffer[0] = *((TUSIGN8*)pValue) ;
        pBuffer[0] = MODBUS_FORMATTING_8_BIT_SUBSTITUTION_CHARACTER ;
        pBuffer[1] = tempBuffer[0] ;
    }
    else
    {
        (void)memcpy((void*)tempBuffer,pValue,(size_t)byteCount) ;
        // What byte order is required for the data ? Different IEEE formats only apply to variables
        // greater than 2 bytes in size
        if ((modbusStaticFrequentNoDefault.modbusIEEEFormat == MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED) ||
            (byteCount <= 2))
        {
            // Standard Modbus big endian format
            // IEEE format (MSW) MSB - LSB (LSW) MSB - LSB
            // Just read ahead into the value and reverse the byte order
            for (byteNumber = 0 ; byteNumber < byteCount ; byteNumber++)
            {
                bytesDone-- ;
                pBuffer[byteNumber] = tempBuffer[bytesDone] ;
            }
        }
        else
        {
            // IEEE format (LSW) MSB - LSB (MSW) MSB - LSB
            byteNumber = 0 ;
            // Starting at the end of the variable, reverse each word into the buffer
            for (wordNumber = 0 ; wordNumber < byteCount / 2 ; wordNumber++)
            {
                // Copy the next word
                //lint -e{545}
                (void)memcpy((void*)&wordBuffer, (void*)&tempBuffer[wordNumber * 2], sizeof(TUSIGN16)) ;
                // And into the target buffer
                pBuffer[byteNumber++] = wordBuffer[1] ;
                pBuffer[byteNumber++] = wordBuffer[0] ;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts the requested type into internal format data
 \author  GL
 \date    12/08/08
 \param   [in] pValue - Void reference to the value to format into the message buffer
 \param   [out] pBuffer - Reference to the message buffer to hold the Modbus formatted data
 \param   [in] byteCount - Number of bytes to format into the buffer
 \return  void
 \warning The maximum size that can be formatted is that of a double (largest system data type)
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
void ConvertToInternalModbus_Formatting(const void *pValue, TUSIGN8 *pBuffer, TUSIGN16 byteCount)
{
    TUSIGN8 tempBuffer[sizeof(TDOUBLE)] ;
    TUSIGN16 bytesDone = byteCount ;
    TUSIGN16 byteNumber = 0 ;
    // Word being read from the buffer
    TUSIGN8 wordBuffer[2] ;
    // Index of the word being read from the buffer
    TUSIGN16 wordNumber ;

    // Is it an eight bit value ?
    if (byteCount == 1)
    {
        // Yes, so skip over the first byte - this can be ignored as it's just a padding byte
        pBuffer[0] = ((TUSIGN8*)pValue)[1] ;
    }
    else
    {
        (void)memcpy((void*)tempBuffer,pValue,(size_t)byteCount) ;
        // Standard Modbus format, data is converted to little endian format
        if ((modbusStaticFrequentNoDefault.modbusIEEEFormat == MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED) ||
            (byteCount <= 2))
        {
            // Non-eight bit value in default IEEE format -
            // read ahead into the value and reverse the byte order
            for (byteNumber = 0 ; byteNumber < byteCount ; byteNumber++)
            {
                bytesDone-- ;
                pBuffer[byteNumber] = tempBuffer[bytesDone] ;
            }
        }
        else
        {
            // IEEE format where data in the message buffer is arranged as LSW (MSB - LSB) MSW (MSB - LSB)
            for (wordNumber = 0 ; wordNumber < byteCount / 2 ; wordNumber++)
            {
                // Get the next word value
                (void)memcpy((void*)wordBuffer, (void *)&tempBuffer[wordNumber * 2], sizeof(TUSIGN16)) ;
                // Reverse the bytes into position
                pBuffer[byteNumber++] = wordBuffer[1] ;
                pBuffer[byteNumber++] = wordBuffer[0] ;
            }
        }
    }
}