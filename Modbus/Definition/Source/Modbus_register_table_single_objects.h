//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_single_objects.h
 Description    Object handling functions for single type register tables

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Register_Table_Single_Objects.h Error - Common_Type.h must be included first."
#endif

#ifndef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Register_Table_Single_Objects.h Error - Modbus_Register_Table_Single_Objects.h must be included first."
#endif

#ifdef __MODBUS_REGISTER_TABLE_SINGLE_OBJECTS_H__
    #error "Modbus_Register_Table_Single_Objects.h Error - Modbus_Register_Table_Single_Objects.h is already included."
#endif

#define __MODBUS_REGISTER_TABLE_SINGLE_OBJECTS_H__
//-------------------------------------------------------------------------------------------------

TUSIGN16 CheckAccessRightsModbus_Register_Table_Single_Objects(const TModbusRegisterTable *me,
                                                                TModbusRegisterTableRequestLink
                                                                *pModbusRegisterTableRequestLink,
                                                                TModbusRegisterTableAccessType
                                                                modbusRegisterTableAccessType) ;

TUSIGN16 CheckRangeModbus_Register_Table_Single_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer) ;

TUSIGN16 AccessObjectsModbus_Register_Table_Single_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestDetails
                                                        *pModbusRequestDetails,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TUSIGN8 *pBuffer,
                                                        TModbusRegisterTableAccessType
                                                        modbusRegisterTableAccessType,
                                                        TUSIGN16 *pLength) ;