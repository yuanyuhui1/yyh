//-------------------------------------------------------------------------------------------------
/*                                                                                                 
<pre>                                                                                              
 Copyright              Copyright ABB, 2009.                                                       
                        All rights reserved. Reproduction, modification,                           
                        use or disclosure to third parties without express                         
                        authority is forbidden.                                                    
                                                                                                   
 System         ModbusGen                                                                          
 Module         ModbusRegisterTables.c                                                             
 Description    Register tables used by the Modbus subsystem                                       
                                                                                                   
 Remarks        This is a code generated file. It should only be modified within the tags          
                <MODBUS_GEN_HEADERS_START> and <MODBUS_GEN_HEADERS_END> and then only when the     
                ModbusGen option 'Add Include Paths' is unchecked. This is used to disable         
                the addition of header file includes for subsystems and data objects when          
                nested includes are required that cannot be detected.                              
                                                                                                   
</pre>                                                                                             
*/                                                                                                 
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__																			 
	#error "Modbus_Command_Descriptors.h error - Common_Type.h must be included first."		     
#endif																							 
																									 
#ifndef __MODBUS_COMMAND_DESCRIPTOR_H__                                                            
	#error "Modbus_Command_Descriptors.h error - Modbus_Command_Descriptor.h must be included first."
#endif                                                                                             
																									 
#ifdef __MODBUS_COMMAND_DESCRIPTORS__																 
	#error "Modbus_Command_Descriptors.h error - Modbus_Command_Descriptors.h is already included."
#endif																							 
																									 
#define __MODBUS_COMMAND_DESCRIPTORS__															 
//-------------------------------------------------------------------------------------------------
																									 
// Ids of commands implemented by the Modbus subsystem
#define MODBUS_COMMAND_ID_READ_COILS 0x1
#define MODBUS_COMMAND_ID_READ_DISCRETE_INPUTS 0x2
#define MODBUS_COMMAND_ID_READ_HOLDING_REGISTERS 0x3
#define MODBUS_COMMAND_ID_READ_INPUT_REGISTERS 0x4
#define MODBUS_COMMAND_ID_WRITE_SINGLE_COIL 0x5
#define MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER 0x6
#define MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS 0xF
#define MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS 0x10

// Commands implemented by the Modbus subsystem
typedef enum _TModbusCommand                   
{                                              
	MODBUS_COMMAND_READ_COILS = 0 ,
	MODBUS_COMMAND_READ_DISCRETE_INPUTS ,
	MODBUS_COMMAND_READ_HOLDING_REGISTERS ,
	MODBUS_COMMAND_READ_INPUT_REGISTERS ,
	MODBUS_COMMAND_WRITE_SINGLE_COIL ,
	MODBUS_COMMAND_WRITE_SINGLE_REGISTER ,
	MODBUS_COMMAND_WRITE_MULTIPLE_COILS ,
	MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS ,
	MODBUS_COMMAND_ID_COUNT
}TModbusCommand ;

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
const TModbusCommandDescriptor *Get_Modbus_Command_Descriptors(TUSIGN8 commandCode) ;			     
																									 
