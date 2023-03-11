//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_objects.h
 Description    Interface to the generic Modbus object manipulation functions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Register_Table_Objects.h Error - Common_Type.h must be included first."
#endif

#ifndef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Register_Table_Objects.h Error - Modbus_Register_Table.h must be included first."
#endif

#ifdef __MODBUS_REGISTER_TABLE_OBJECTS__
    #error "Modbus_Register_Table_Objects.h Error - Modbus_Register_Table_Objects.h is already included."
#endif

#define __MODBUS_REGISTER_TABLE_OBJECTS__
//-------------------------------------------------------------------------------------------------

TUSIGN16 CheckAccessRightsModbus_Register_Table_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TModbusRegisterTableAccessType
                                                        modbusRegisterTableAccessType) ;
