//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_commands_helper.h
 Description    Interface to the helper functions used by the custom and public command handlers

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Commands_Helper.h Error - Common_Type.h must be included first."
#endif

#ifdef __MODBUS_COMMANDS_HELPER__
    #error "Modbus_Commands_Helper.h Error - Modbus_Commands_Helper.h is already included."
#endif

#define __MODBUS_COMMANDS_HELPER__
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Checks if the number of bytes in a message is correct
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pMessage - Reference to the message content
 \param   [in] length - The length of the received message
 \return  OK if the number of received bytes is correct
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
TUSIGN16 CheckLengthModbus_Commands_Helper(const TModbusCommandDescriptor *pModbusCommandDescriptor,
                                            const TUSIGN8 *pMessage,
                                            //lint -e{793} by CNZUWAN AS code review is accetable.
                                            TUSIGN16 length) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Extracts the starting index and number of items requested from a message buffer
 \author  GL
 \date    13/08/08
 \param   [in] pMessage - Reference to the message to read the information from
 \param   [in] pModbusRegisterTableRequestDetails - The start register and item count
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
void GetStartingIndexAndCountModbus_Commands_Helper(const TUSIGN8 *pMessage,
                                                    //lint -e{793} by CNZUWAN AS code review is accetable
                                                    TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Adds the starting index and number of items requested to a message buffer
 \author  GL
 \date    01/06/09
 \param   [in] pMessage - Reference to the message to read the information from
 \param   [in] pModbusRegisterTableRequestDetails - The start register and item count
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
void AddStartingIndexAndCountModbus_Commands_Helper(TUSIGN8 *pMessage,
                                                    //lint -e{793} by CNZUWAN AS code review is accetable
                                                    const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails) ;

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts Common Framework error information into a Modbus exception response code.
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the request command structure
 \param   [in] startingIndex - Start index of initial register being written
 \param   [in] objectCount - The number of objects being written
 \param   [in] pMessage - Reference to the request message
 \return  OK if all of the values in the message can be written
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
//lint -e{793} by CNZUWAN AS code review is accetable
TModbusExceptionType TranslateErrorCodeToExceptionModbus_Commands_Helper(TUSIGN16 errorCode) ;