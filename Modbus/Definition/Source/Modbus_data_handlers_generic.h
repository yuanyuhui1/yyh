//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_generic_handlers.h
 Description    Interface to the Modbus generic handler functions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Data_Handlers_Generic.h Error - Modbus_Register_Table.h must be included first."
#endif

#ifndef __COMMON_TYPE_H__
    #error "Modbus_Data_Handlers_Generic.h Error - Common_Type.h must be included first."
#endif

#ifdef __MODBUS_DATA_HANDLERS_GENERIC__
    #error "Modbus_Data_Handlers_Generic.h Error - Modbus_Data_Handlers_Generic.h is already included."
#endif

#define __MODBUS_DATA_HANDLERS_GENERIC__
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Reads objects in little endian format and loads the results into the passed message buffer
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusRegisterTable - Reference to the register table where the objects reside
 \param   [in] objectCount - Number of objects to read
 \param   [in[ pMessage - Reference to the buffer holding the coils to be written
 \param   [in] arraryIndex - Array index of the initial object
 \param   [out] pLength - Reference to a counter holding the number of bytes added to the buffer
 \return  OK if all of the objects are read successfully
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
TUSIGN16 ReadObjectsModbus_Data_Handlers_Generic(const TModbusRegisterTable *pModbusRegisterTable,
                                            TModbusHardwareLayer *pModbusHardwareLayer,
                                            const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                                            const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                            //lint -e{793} by CNZUWAN AS code review is accetable.
                                            TUSIGN8 *pMessage) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Writes objects in little endian format
 \author  GL
 \date    20/08/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusRegisterTable - Reference to the register table where the initial object resides
 \param   [in] objectCount - Number of objects to write
 \param   [in[ pMessage - Reference to the buffer holding the data to be written
 \param   [in] arrayIndex - Array index of initial object
 \param   [out] pLength - Reference to a counter holding the number of bytes added to the buffer
 \return  OK if all of the objects are written correctly
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
TUSIGN16 WriteObjectsModbus_Data_Handlers_Generic(const TModbusRegisterTable *pModbusRegisterTable,
                                                TModbusHardwareLayer *pModbusHardwareLayer,
                                                const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                                                const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                //lint -e{793} by CNZUWAN AS code review is accetable.
                                                TUSIGN8 *pMessage) ;
