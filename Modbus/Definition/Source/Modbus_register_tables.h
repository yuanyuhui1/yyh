//-------------------------------------------------------------------------------------------------
/*                                                                                                 
<pre>                                                                                              
 Copyright              Copyright ABB, 2009.                                                       
                        All rights reserved. Reproduction, modification,                           
                        use or disclosure to third parties without express                         
                        authority is forbidden.                                                    
                                                                                                   
 System         ModbusGen                                                                          
 Module         ModbusRegisterTables.h                                                             
 Description    Interface to the register tables used by the Modbus subsystem.                     
                                                                                                   
 Remarks        This is a code generated file. It should not be modified.                          
                                                                                                   
</pre>                                                                                             
*/                                                                                                 
//-------------------------------------------------------------------------------------------------
#ifndef __MODBUS_REGISTER_TABLE__																	 
    #error "Modbus_Register_Tables.h Error - Modbus_Register_Table.h must be included first."	 
#endif                                                                                             
                                                                                                   
#ifdef __MODBUS_REGISTER_TABLES__                                                                  
    #error "Modbus_Register_Tables.h Error - Modbus_Register_Tables.h is already included."      
#endif                                                                                             
                                                                                                   
#define __MODBUS_REGISTER_TABLES__                                                                 
                                                                                                   
//-------------------------------------------------------------------------------------------------

#define MODBUS_REGISTER_TABLE_READONLY_1BIT_BOOL_B1RO1_START 0
#define MODBUS_REGISTER_TABLE_READONLY_1BIT_BOOL_B1RO1_END 1999
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_8BIT_READONLY_DYNAMIC_U8ROD_START 0
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_8BIT_READONLY_DYNAMIC_U8ROD_END 199
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READONLY_DYNAMIC_F32ROD_START 200
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READONLY_DYNAMIC_F32ROD_END 498
#define MODBUS_REGISTER_TABLE_DOUBLE_64BIT_READONLY_DYNAMIC_D64ROD_START 500
#define MODBUS_REGISTER_TABLE_DOUBLE_64BIT_READONLY_DYNAMIC_D64ROD_END 996
#define MODBUS_REGISTER_TABLE_CHARACTER_8BIT_READONLY_C8RO_START 1000
#define MODBUS_REGISTER_TABLE_CHARACTER_8BIT_READONLY_C8RO_END 1999
#define MODBUS_REGISTER_TABLE_USIGNED_INTEGER_8BIT_READONLY_U8RO_START 2000
#define MODBUS_REGISTER_TABLE_USIGNED_INTEGER_8BIT_READONLY_U8RO_END 2999
#define MODBUS_REGISTER_TABLE_USIGNED_INTEGER_16BIT_READONLY_U16RO_START 3000
#define MODBUS_REGISTER_TABLE_USIGNED_INTEGER_16BIT_READONLY_U16RO_END 3999
#define MODBUS_REGISTER_TABLE_SIGNED_INTEGER_16BIT_READONLY_S16RO_START 4000
#define MODBUS_REGISTER_TABLE_SIGNED_INTEGER_16BIT_READONLY_S16RO_END 4999
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_32BIT_READONLY_U32RO_START 5000
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_32BIT_READONLY_U32RO_END 5998
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READONLY_F32RO_START 6000
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READONLY_F32RO_END 6998
#define MODBUS_REGISTER_TABLE_VARIANT_16BIT_READONLY_SCANREGISTER1_V16RO1_START 9000
#define MODBUS_REGISTER_TABLE_VARIANT_16BIT_READONLY_SCANREGISTER1_V16RO1_END 9099
#define MODBUS_REGISTER_TABLE_VARIANT_16BIT_READONLY_SCANREGISTER2_V16RO2_START 9100
#define MODBUS_REGISTER_TABLE_VARIANT_16BIT_READONLY_SCANREGISTER2_V16RO2_END 9199
#define MODBUS_REGISTER_TABLE_CHARACTER_8BIT_READWRITE_C8RW_START 0
#define MODBUS_REGISTER_TABLE_CHARACTER_8BIT_READWRITE_C8RW_END 999
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_8BIT_READWRITE_U8RW_START 1000
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_8BIT_READWRITE_U8RW_END 1999
#define MODBUS_REGISTER_TABLE_SIGNED_INTEGER_16BIT_READWRITE_S16RW_START 3000
#define MODBUS_REGISTER_TABLE_SIGNED_INTEGER_16BIT_READWRITE_S16RW_END 3999
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTERGER_32BIT_READWRITE_U32RW_START 4000
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTERGER_32BIT_READWRITE_U32RW_END 4998
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READWRITE_F32RW_START 5000
#define MODBUS_REGISTER_TABLE_FLOAT_32BIT_READWRITE_F32RW_END 5998
#define MODBUS_REGISTER_TABLE_DOUBLE_64BIT_READWRITE_D64RW_START 6000
#define MODBUS_REGISTER_TABLE_DOUBLE_64BIT_READWRITE_D64RW_END 6996
#define MODBUS_REGISTER_TABLE_ACTION_8BIT_WRITEONLY_A8WO_START 7000
#define MODBUS_REGISTER_TABLE_ACTION_8BIT_WRITEONLY_A8WO_END 7999
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER1_U16RW1_START 9000
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER1_U16RW1_END 9099
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER2_U16RW2_START 9100
#define MODBUS_REGISTER_TABLE_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER2_U16RW2_END 9199

typedef enum _TModbusRegisterTableId
{
	MODBUS_REGISTER_TABLE_ID_READONLY_1BIT_BOOL_B1RO1 = 0 ,
	MODBUS_REGISTER_TABLE_ID_READWRITE_1BIT_BOOL_B1RW1 ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_8BIT_READONLY_DYNAMIC_U8ROD ,
	MODBUS_REGISTER_TABLE_ID_FLOAT_32BIT_READONLY_DYNAMIC_F32ROD ,
	MODBUS_REGISTER_TABLE_ID_DOUBLE_64BIT_READONLY_DYNAMIC_D64ROD ,
	MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READONLY_C8RO ,
	MODBUS_REGISTER_TABLE_ID_USIGNED_INTEGER_8BIT_READONLY_U8RO ,
	MODBUS_REGISTER_TABLE_ID_USIGNED_INTEGER_16BIT_READONLY_U16RO ,
	MODBUS_REGISTER_TABLE_ID_SIGNED_INTEGER_16BIT_READONLY_S16RO ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_32BIT_READONLY_U32RO ,
	MODBUS_REGISTER_TABLE_ID_FLOAT_32BIT_READONLY_F32RO ,
	MODBUS_REGISTER_TABLE_ID_DOUBLE_64BIT_READONLY_D64RO ,
	MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER1_V16RO1 ,
	MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER2_V16RO2 ,
	MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READWRITE_C8RW ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_8BIT_READWRITE_U8RW ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_U16RW ,
	MODBUS_REGISTER_TABLE_ID_SIGNED_INTEGER_16BIT_READWRITE_S16RW ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTERGER_32BIT_READWRITE_U32RW ,
	MODBUS_REGISTER_TABLE_ID_FLOAT_32BIT_READWRITE_F32RW ,
	MODBUS_REGISTER_TABLE_ID_DOUBLE_64BIT_READWRITE_D64RW ,
	MODBUS_REGISTER_TABLE_ID_ACTION_8BIT_WRITEONLY_A8WO ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER1_U16RW1 ,
	MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER2_U16RW2 ,
	MODBUS_REGISTER_TABLE_ID_COUNT
}TModbusRegisterTableId ;
																									 
// Extern declaration to the register tables so they can be accessed in the command descriptors    
// file																							 
extern const TModbusRegisterTable modbusRegisterTables[] ;										 
																									 
//-------------------------------------------------------------------------------------------------
/*!                                                                                                
 \brief   Gets the register table in which a register resides                                     
 \author  GL                                                                                      
 \date    24/07/08                                                                                
 \param   [in] registerIndex - index of the register to get the table id for                      
 \param   [in] modbusRegisterTableLocation - location (eg, Input Coils) of the register table     
 \return  pModbusRegisterTable - Reference to a register table                                    
 \warning Will return NULL if the register cannot be found                                        
 \test                                                                                            
 date: 2005-08-23                                                                                  
 \n by: GL                                                                                        
 \n environment: IAR Embedded Workbench V3.10A                                                    
 \n intention: 100% code coverage                                                                 
 \n result module test: Pass, 100% code coverage                                                  
 \n result Lint Level 3:                                                                          
 \bug                                                                                             
*/                                                                                                 
//-------------------------------------------------------------------------------------------------
const TModbusRegisterTable *GetRegisterTableModbus_Register_Tables(TUSIGN16 registerIndex,         
                                   TModbusRegisterTableLocation modbusRegisterTableLocation) ;     
                                                                                                   
