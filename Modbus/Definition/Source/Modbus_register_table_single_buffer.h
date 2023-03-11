//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_single_buffer.h
 Description    Interface to Modbus message buffer handling functions for single type register tables

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Register_Table_Single_Buffer.h Error - Common_Type.h must be included first."
#endif

#ifndef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Register_Table_Single_Buffer.h Error - Modbus_Register_Table.h must be included first."
#endif

#ifdef __MODBUS_REGISTER_TABLE_SINGLE_BUFFER_H__
    #error "Modbus_Register_Table_Single_Buffer.h Error - Modbus_Register_Table_Single_Buffer.h is already included."
#endif

#define __MODBUS_REGISTER_TABLE_SINGLE_BUFFER_H__
//-------------------------------------------------------------------------------------------------

TUSIGN16 GetTableLinkModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink) ;

TUSIGN16 GetRequestLengthModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                          *pModbusRegisterTableRequestLink,
                                                          TBOOL byteLength) ;

TUSIGN16 CheckForObjectOverlapModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                            const TModbusRegisterTableRequestDetails
                                                            *pModbusRegisterTableRequestDetails,
                                                            const TModbusRegisterTableRequestLink
                                                            *pModbusRegisterTableRequestLink) ;

void FormatToInternalModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer) ;

void FormatToExternalModbus_Register_Table_Single_Buffer(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRegisterTableRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer) ;