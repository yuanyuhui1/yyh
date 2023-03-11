//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_formatting.h
 Description    Interface to the Modbus formatting functions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Formatting.h Error - Common_Type.h must be included first."
#endif

#ifdef __MODBUS_FORMATTING__
    #error "Modbus_Formatting.h Error - Modbus_Formatting.h is already included."
#endif

#define __MODBUS_FORMATTING__
//-------------------------------------------------------------------------------------------------

// The character substituted for the upper byte of 16-bit values for 8-bit register values
#define MODBUS_FORMATTING_8_BIT_SUBSTITUTION_CHARACTER 0x00

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts the requested type into Modbus format data
 \author  GL
 \date    12/08/08
 \param   [in] pValue - Void reference to the value to format into the message buffer
 \param   [out] pBuffer - Reference to the message buffer to hold the Modbus formatted data
 \param   [in] byteCount - Number of bytes to format into the buffer
 \return  void
 \warning
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
//lint -e{793} by CNZUWAN 
void ConvertToExternalModbus_Formatting(const void *pValue, TUSIGN8 *pBuffer, TUSIGN16 byteCount) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts the requested type into Modbus format data
 \author  GL
 \date    12/08/08
 \param   [in] pValue - Void reference to the value to format into the message buffer
 \param   [out] pBuffer - Reference to the message buffer to hold the Modbus formatted data
 \param   [in] byteCount - Number of bytes to format into the buffer
 \return  void
 \warning
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
//lint -e{793} by CNZUWAN 
void ConvertToInternalModbus_Formatting(const void *pValue, TUSIGN8 *pBuffer, TUSIGN16 byteCount) ;