//-------------------------------------------------------------------------------------------------
/*
<pre>
 Copyright              Copyright ABB, 2009.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         ModbusGen
 Module         ModbusCommandDescriptors.c 
 Description    Descriptors of commands used by the Modbus subsystem
 Remarks        This is a code generated file. It should not be modified.
                Command descriptors are used to link command handler and (if necessary)
                data handler functions to a command id. For private commands, a reference to
                the register table on which the command acts is embedded in the command descriptor.
                For public commands, the range of registers/coils requested, and the location of
                the register table are used to determine what register table to access.
                Certain public commands, like the read slave id request have only a handler
                function as no data manipulation is required.
</pre>
*/
//-------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_tables.h"
#include "Modbus/Source/Modbus_command_descriptor.h" 
#include "Modbus/Source/Modbus_command_descriptors.h"
#include "Modbus/Source/Modbus_command_handlers_public.h"
#include "Modbus/Source/Modbus_command_handlers_private.h"
#include "Modbus/Source/Modbus_data_handlers_generic.h"

const TModbusCommandDescriptor modbusCommandDescriptorTable[] =
{                                                              
	// MODBUS_COMMAND_ID_READ_COILS = 0x1                        
	{                                                            
		MODBUS_COMMAND_ID_READ_COILS,							     
		MODBUS_COMMAND_TYPE_COIL,									 
		MODBUS_FUNCTION_TYPE_READ,	                             
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_OUTPUT_COILS,               
		CoilHandlerModbus_Command_Handlers_Public,                 
		ReadObjectsModbus_Data_Handlers_Generic,                   
	},															 
	// MODBUS_COMMAND_ID_READ_DISCRETE_INPUTS = 0x2				 
	{															 
		MODBUS_COMMAND_ID_READ_DISCRETE_INPUTS,					 
		MODBUS_COMMAND_TYPE_COIL,									 
		MODBUS_FUNCTION_TYPE_READ,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_INPUT_COILS,                
		CoilHandlerModbus_Command_Handlers_Public,                 
		ReadObjectsModbus_Data_Handlers_Generic,                   
	},															 
	// MODBUS_COMMAND_ID_READ_HOLDING_REGISTERS = 0x3			 
	{															 
		MODBUS_COMMAND_ID_READ_HOLDING_REGISTERS,					 
		MODBUS_COMMAND_TYPE_REGISTER,								 
		MODBUS_FUNCTION_TYPE_READ,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_HOLDING_REGISTERS,          
		RegisterHandlerModbus_Command_Handlers_Public,             
		ReadObjectsModbus_Data_Handlers_Generic,                   
	},															 
	// MODBUS_COMMAND_ID_READ_INPUT_REGISTERS = 0x4				 
	{															 
		MODBUS_COMMAND_ID_READ_INPUT_REGISTERS,					 
		MODBUS_COMMAND_TYPE_REGISTER,								 
		MODBUS_FUNCTION_TYPE_READ,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_INPUT_REGISTERS,            
		RegisterHandlerModbus_Command_Handlers_Public,             
		ReadObjectsModbus_Data_Handlers_Generic,                   
	},															 
	// MODBUS_COMMAND_ID_WRITE_SINGLE_COIL = 0x5				 
	{															 
		MODBUS_COMMAND_ID_WRITE_SINGLE_COIL,						 
		MODBUS_COMMAND_TYPE_COIL,									 
		MODBUS_FUNCTION_TYPE_WRITE,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_OUTPUT_COILS,               
		CoilHandlerModbus_Command_Handlers_Public,                 
		WriteObjectsModbus_Data_Handlers_Generic,                  
	},															 
	// MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER = 0x6			 
	{															 
		MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER,					 
		MODBUS_COMMAND_TYPE_REGISTER,								 
		MODBUS_FUNCTION_TYPE_WRITE,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_HOLDING_REGISTERS,          
		RegisterHandlerModbus_Command_Handlers_Public,             
		WriteObjectsModbus_Data_Handlers_Generic,                  
	},															 															 
	//  MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS = 0xF			 
	{															 
		MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS,					 
		MODBUS_COMMAND_TYPE_COIL,									 
		MODBUS_FUNCTION_TYPE_WRITE,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_OUTPUT_COILS,               
		CoilHandlerModbus_Command_Handlers_Public,                 
		WriteObjectsModbus_Data_Handlers_Generic,                  
	},															 
	// MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS = 0x10		 
	{															 
		MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS,				 
		MODBUS_COMMAND_TYPE_REGISTER,								 
		MODBUS_FUNCTION_TYPE_WRITE,								 
		(const TModbusRegisterTable *)NULL,                        
		MODBUS_REGISTER_TABLE_LOCATION_HOLDING_REGISTERS,          
		RegisterHandlerModbus_Command_Handlers_Public,             
		WriteObjectsModbus_Data_Handlers_Generic,                  
	},															 															 
} ;

//-------------------------------------------------------------------------------------------------
/*!																								 
 \brief   Returns the command descriptor for a passed command code                                
 \author  GL                                                                                      
 \date    23/08/05                                                                                
 \param   [in] commandCode - the command code for the required descriptor                         
 \return  A reference to the command descriptor structure										 
 \warning Will return NULL if the command code is unknown. NULL should be checked for before      
 \warning using the command descriptor reference													 
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
const TModbusCommandDescriptor *Get_Modbus_Command_Descriptors(TUSIGN8 commandCode)
{
    TUSIGN16 commandDescriptorCount;
    TBOOL commandDescriptorFound = eFALSE;
    const TModbusCommandDescriptor *pModbusCommandDescriptor = (const TModbusCommandDescriptor *)NULL;

    for (commandDescriptorCount = 0;
        (commandDescriptorCount < MODBUS_COMMAND_ID_COUNT) && (commandDescriptorFound == eFALSE);
        commandDescriptorCount++)
    {
        if ((TUSIGN8)modbusCommandDescriptorTable[commandDescriptorCount].commandId == commandCode)
        {
            pModbusCommandDescriptor = &modbusCommandDescriptorTable[commandDescriptorCount];
            commandDescriptorFound = eTRUE;
        }
    }
    return(pModbusCommandDescriptor);
}