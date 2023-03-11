//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2005.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem Modbus
Module
Description    Implementation of execute methods
for Subsystems based on T_UNIT.

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <limits.h>
#include <string.h>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/Modbus_idx.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Ethernet/Interface/Ethernet.h"
#include "Services/Interface/math_helper.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"
#include "modbus_types.h"
#include "modbus_attributes.h"
#include "modbus_scanregister.h"
#include "modbus_execute.h"
#include "modbus_tcp.h"
#include "modbus_counters.h"
#include "modbus_memory.h"
#include "Modbus_register_table.h"
#include "Modbus_command_descriptor.h"
#include "Modbus_command_descriptors.h"
#include "CI_codes.h"
#include "CI_socket_types.h"
#include "CI_socket_api.h"
#include "CI_system_api.h"

static int32_t mainListeningSocket;
static void* socketsSet = NULL;

static TUSIGN8 enableModbusTcp = ETH_UNSECURED_ACCESS_INVALID;
static TUSIGN8 ethLinksUp = 0;
static TUSIGN32 ipAddress = 0;
static TBOOL ipAddressValid = eFALSE;

static inline void HandleModbusTaskState(void);
static inline void BuildSocketSet(void** ppSocketsSet, int32_t* maxSocketDesc);
static inline TUSIGN16 StartListening(void);
static inline void AcceptNewClient(void);
static inline void HandleReceival(void);
static inline void HandleInterpretation(void);
static inline void StopListening(void);
static inline void ProcessModbusRequestMessage(TModbusHardwareLayer *pModbusHardwareLayer);
static inline void ReceiveModbusRequestMessage(T_MODBUS_TCP_TRANSACTION* pTransaction, TModbusHardwareLayer *pModbusHardwareLayer);
static inline void SentModbusResponseMessage(T_MODBUS_TCP_TRANSACTION* pTransaction, TModbusHardwareLayer *pModbusHardwareLayer);
static inline TUSIGN16 LoadExceptionMessageModbus(TUSIGN8 *pMessage, TUSIGN8 commandCode, TModbusExceptionType modbusExceptionType);

TUSIGN8 modbusPDUBuffer[MODBUS_MAX_PDU_MESSAGE_SIZE];

TModbusHardwareLayer hardwareLayerTCP_modbus =
{
  // Server response specifc parameters
  {
    // Set if access rights checks should be performed
    eTRUE,
  },
  // Common parameters
  {
    // Flag set whilst a message is being processed
    eFALSE,
    // The length of the message currently being processed
    0,
    // Mode the hardware layer is operating in
    MODBUS_MODE_SERVER,
    // Reference to the subsystem send message function
    //SendMessageTCPcallBack_MODBUS,
    // The buffer owned by the hardware layer used to store incoming/outgoing messages
    modbusPDUBuffer,
  },
};

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SINKEVENTEXE> \endif
 \brief  
 \param  pEvent Pointer to the event structure
\if @SubGen end@ <METHODHEADER SINKEVENTEXE> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SINKEVENTEXE>
void SinkEventEXE_MODBUS(const void* pEvent)
//@SubGen end@<METHOD SINKEVENTEXE>
{
  if (*(modbus.unit.ptrState) < INITIALIZED)
  {
    return;
  }

  const CI_eventInfo_t eventInfo = *((const CI_eventInfo_t *)pEvent);

  switch (eventInfo.event)
  {
    case CI_eventLinkChange:
    {
      int32_t result;
      uint32_t linkState;
      int32_t szLinkState = sizeof(linkState);
      ethLinksUp = 0;
      
      result = CI_GetSystemParam(CI_sysParamPortLinkState, &linkState, &szLinkState);
      if (result == szLinkState)
      {
        ethLinksUp = linkState;

        if (ethLinksUp == 0)
        {
          ipAddressValid = eFALSE;
          ipAddress = 0;
        }
      }
      else
      {
      }
    } break;

    case CI_eventIpConfigChange:
    {
      if (eventInfo.info == 0)
      {
        ipAddressValid = eFALSE;
        ipAddress = 0;
        
        ChangeModbusTaskState(MODBUS_STATE_ENDING);
      }
      else
      {
        int32_t result;
        uint32_t newIpAddress = 0;
        int32_t szIpAddress = sizeof(newIpAddress);
        
        result = CI_GetSystemParam(CI_sysParamIpAddress, &newIpAddress, &szIpAddress);
        //if (result == szIpAddress)
        {
          if (ipAddress != newIpAddress)
          {
            ipAddress = newIpAddress;
            ipAddressValid = eTRUE;
            if (enableModbusTcp == ETH_UNSECURED_ACCESS_UNSECURED_READ_ONLY
             || enableModbusTcp == ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE)
            {
              ChangeModbusTaskState(MODBUS_STATE_RESET_ENDING);
            }
          }
        }
      }
    } break;

    case CI_eventIpConflict:
    case CI_eventPlcConnectionChange:
    case CI_eventReadItem:
    case CI_eventWriteItem:
    case CI_eventRiFlagChange:
    case CI_eventDeviceBlinkInd:
    case CI_eventNetworkResetRequest:
    case CI_eventWebServerStatus:
    case CI_eventLast:
    default:
      break;
  }
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEENABLESTATEEXE> \endif
 \brief  
 \param  enableState Enable state
\if @SubGen end@ <METHODHEADER UPDATEENABLESTATEEXE> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEENABLESTATEEXE>
void UpdateEnableStateEXE_MODBUS(TUSIGN8 enableState)
//@SubGen end@<METHOD UPDATEENABLESTATEEXE>
{
  enableModbusTcp = enableState;
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER TASKEXE> \endif
 \brief  
\if @SubGen end@ <METHODHEADER TASKEXE> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TASKEXE>
void TaskEXE_MODBUS(void)
//@SubGen end@<METHOD TASKEXE>
{
  HandleModbusTaskState();

  switch (GetModbusTaskState())
  {
    case MODBUS_STATE_STARTING:
    {
        TUSIGN16 result;
        result = StartListening();
        if (result == OK)
        {
            ChangeModbusTaskState(MODBUS_STATE_STARTED);
        }
        else
        {
            ChangeModbusTaskState(MODBUS_STATE_STARTING_FAILED);
        }
    } break;

    case MODBUS_STATE_STARTED:
    {
        HandleReceival();
        HandleInterpretation();
    } break;
        
    case MODBUS_STATE_ENDING:
    case MODBUS_STATE_RESET_ENDING:
    {
        StopListening();

        if (GetModbusTaskState() == MODBUS_STATE_ENDING)
        {
            ChangeModbusTaskState(MODBUS_STATE_NOT_STARTED);
        }
        else if(GetModbusTaskState() == MODBUS_STATE_RESET_ENDING)
        {
            ChangeModbusTaskState(MODBUS_STATE_STARTING);
        }
        else
        {
        }
    } break;

    case MODBUS_STATE_NOT_STARTED:
    case MODBUS_STATE_STARTING_FAILED:
    default:
      break;
  }
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATESCANREGISTER> \endif
 \brief  Updates the Modbus Scan Registers
 \param   Updates the Modbus Scan Registers
\if @SubGen end@ <METHODHEADER UPDATESCANREGISTER> \endif
\author Jax-Ynag
\date 2015-6-24
\param void
\return void
\warning
\test
test-date: 2016-08-24
\n by: ZuoChen Wang
\n environment:IAR Embedded Workbench Renesas RX V2.41.3
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATESCANREGISTER>
void UpdateScanRegister_MODBUS(void )
//@SubGen end@<METHOD UPDATESCANREGISTER>
{
    UpdateScanRegisterViaTable(configScanRegisterIntern1, modbusDynamic.ScanRegister1);
    UpdateScanRegisterViaTable(configScanRegisterIntern2, modbusDynamic.ScanRegister2);
}


/*
This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
\if @SubGen start@ <METHODHEADER TEMPLATE> \endif
\if @SubGen end@ <METHODHEADER TEMPLATE> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Handles state changes of Modbus TCP task
 \author    Michal Kochel
 \date      2019-08-19
 \warning 
 \test 
 \n test-date:          2019-MM-DD
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: TODO
 \n result Lint Level3: TODO
*/
//--------------------------------------------------------------------------------------------------
static inline void HandleModbusTaskState(void)
{
  if (ethLinksUp > 0 && ipAddressValid == eTRUE &&
     (enableModbusTcp == ETH_UNSECURED_ACCESS_UNSECURED_READ_ONLY
     || enableModbusTcp == ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE))
  {
      if (GetModbusTaskState() == MODBUS_STATE_NOT_STARTED)
      {
          ChangeModbusTaskState(MODBUS_STATE_STARTING);
      }
  }
  else
  {
      if (GetModbusTaskState() != MODBUS_STATE_NOT_STARTED)
      {
          ChangeModbusTaskState(MODBUS_STATE_ENDING);
      }
  }

  TUSIGN8 currentWriteFunctionsEnabled;
  if (modbus_Get(MODBUS_IDX_writeFunctionsEnabled, WHOLE_OBJECT, &currentWriteFunctionsEnabled) == OK)
  {
    TUSIGN8 newWriteFunctionsEnabled = (enableModbusTcp == ETH_UNSECURED_ACCESS_UNSECURED_READ_WRITE ? \
                                        MODBUS_WRITE_FUNCTIONS_ENABLED : MODBUS_WRITE_FUNCTIONS_DISABLED);
    
    if (newWriteFunctionsEnabled != currentWriteFunctionsEnabled)
    {
      (void)modbus_Put(MODBUS_IDX_writeFunctionsEnabled, WHOLE_OBJECT, &newWriteFunctionsEnabled);

      if (GetModbusTaskState() == MODBUS_STATE_STARTED)
      {
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Builds socket set from main listening socket and all active connections socket
 \author    Michal Kochel
 \date      2019-07-17
 \param     void** Pointer tosocket set which will be updated
 \param     int32_t* Output with maximal value of descriptor
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline void BuildSocketSet(void** ppSocketsSet, int32_t* maxSocketDesc)
{
  if (*ppSocketsSet == NULL)
  {
    *ppSocketsSet = CI_GetFdSet();
  }

  CI_FdZero(*ppSocketsSet);

  // Add main socket
  CI_FdSet(mainListeningSocket, *ppSocketsSet);
  *maxSocketDesc = mainListeningSocket;

  // Add all client sockets
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eTRUE)
    {
      CI_FdSet(modbusConnections[index].socket, *ppSocketsSet);
      *maxSocketDesc = max_tint32(*maxSocketDesc, modbusConnections[index].socket);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Starts listening on main Modbus TCP socket
 \author    Michal Kochel
 \date      2019-07-17
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline TUSIGN16 StartListening(void)
{
  int32_t socketErr;
  int32_t returnValue;
  int32_t optionOn = 1;
  TUSIGN16 cfResult;
  TUSIGN16 portNumber;
  struct sockaddr_in mainListeningSocketAddr;

  InitModbusConnections();

  InitModbusTransactions();
  
  // Create socket
  mainListeningSocket = CI_Socket(AF_INET, CI_SOCK_STREAM, CI_IPPROTO_TCP, &socketErr);
  if (mainListeningSocket == -1)
  {
    return (TUSIGN16)FATAL_ERROR;
  }

  // Set options
  returnValue = CI_OK;
  returnValue |= CI_SetSockOpt(mainListeningSocket, CI_SOL_SOCKET, CI_SO_REUSEADDR, (const void *)&optionOn, sizeof(int32_t));
  returnValue |= CI_SetSockOpt(mainListeningSocket, CI_SOL_SOCKET, CI_SO_KEEPALIVE, (const void *)&optionOn, sizeof(int32_t));
  returnValue |= CI_SetSockOpt(mainListeningSocket, CI_SOL_SOCKET, CI_SO_NBIO, NULL, 0);
  returnValue |= CI_SetSockOpt(mainListeningSocket, CI_IPPROTO_TCP, CI_TCP_NODELAY, (const void *)&optionOn, sizeof(int32_t));

  if (returnValue != CI_OK)
  {
    return (TUSIGN16)FATAL_ERROR;
  }

  // Get port number
  cfResult = (TUSIGN16)modbus_Get((TUSIGN16)MODBUS_IDX_networkPort, (TINT16)WHOLE_OBJECT, &portNumber);
  if (cfResult != (TUSIGN16)OK)
  {
    return (TUSIGN16)FATAL_ERROR;
  }

  // Set address
  memset(&mainListeningSocketAddr, 0, sizeof(struct sockaddr_in));
  mainListeningSocketAddr.sin_family = AF_INET;
  //lint -e{835,845,831,572} CI_HTONL is from 3rd party
  mainListeningSocketAddr.sin_addr.s_addr = CI_HTONL(INADDR_ANY);
  mainListeningSocketAddr.sin_port = CI_HTONS((unsigned short)portNumber);

  // Bind and start listening
  //lint -e{740,826} This is how sockaddrs is passed
  returnValue = CI_Bind(mainListeningSocket, (struct sockaddr*)&mainListeningSocketAddr, sizeof(struct sockaddr_in));
  if (returnValue != CI_OK)
  {
    return (TUSIGN16)FATAL_ERROR;
  }

  returnValue = CI_Listen(mainListeningSocket, MODBUS_TCP_MAX_CLIENTS);
  if (returnValue != CI_OK)
  {
    return (TUSIGN16)FATAL_ERROR;
  }

  return (TUSIGN16)OK;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Accepts new client connection
 \author    Michal Kochel
 \date      2019-07-17
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline void AcceptNewClient(void)
{
  int32_t newClientSocket;
  struct sockaddr_in newClientSocketAddr;
  socklen_t addresLen;
  int32_t returnValue;

  memset(&newClientSocketAddr, 0, sizeof(struct sockaddr_in));
  addresLen = sizeof(struct sockaddr_in);

  //lint -e{740,826} This is how sockaddrs is passed
  newClientSocket = CI_Accept(mainListeningSocket, (struct sockaddr*)&newClientSocketAddr,  &addresLen);
  if (newClientSocket > CI_OK)
  {
    int32_t tcpKeepaliveCnt = 1;
    int32_t tcpKeepaliveIdle = 60;
    int32_t tcpKeepaliveIntvl = 5;

    returnValue = CI_OK;
    returnValue |= CI_SetSockOpt(newClientSocket, CI_IPPROTO_TCP, CI_TCP_KEEPALIVE_CNT, (void *)&tcpKeepaliveCnt, sizeof(int32_t));
    returnValue |= CI_SetSockOpt(newClientSocket, CI_IPPROTO_TCP, CI_TCP_KEEPALIVE_IDLE, (void *)&tcpKeepaliveIdle, sizeof(int32_t));
    returnValue |= CI_SetSockOpt(newClientSocket, CI_IPPROTO_TCP, CI_TCP_KEEPALIVE_INTVL, (void *)&tcpKeepaliveIntvl, sizeof(int32_t));

    if (PushModbusConnection(newClientSocket, CI_NTOHL(newClientSocketAddr.sin_addr.s_addr), CI_NTOHS(newClientSocketAddr.sin_port)) == eFALSE)
    {
      if (CI_CloseSocket(newClientSocket, &returnValue) != CI_OK)
      {
      }
    }
  }
  else
  {
    int32_t socketError = -1;
    (void)CI_GetLastSocketError(mainListeningSocket, &socketError);
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Handles receival on sockets
 \author    Michal Kochel
 \date      2019-07-17
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline void HandleReceival(void)
{
  int32_t returnValue;
  int32_t maxSocketDesc;
  int32_t selectTimeout = 100000;

  BuildSocketSet(&socketsSet, &maxSocketDesc);

  returnValue = CI_Select(maxSocketDesc + 1, socketsSet, NULL, NULL, selectTimeout);
  if (returnValue > 0 && mainListeningSocket != -1)
  {
    // Test activity on main socket
    if (CI_FdIsSet(mainListeningSocket, socketsSet))
    {
      AcceptNewClient();
    }
    // Test activity on all connected sockets
    for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
    {
      T_MODBUS_TCP_CONNECTION* connection; 

      connection = modbusConnections + index;

      if (connection->isUsed == eTRUE)
      {
        if (CI_FdIsSet(connection->socket, socketsSet))
        {
          TBOOL disconnect = eFALSE;
          if (ConnectionHasActiveTransaction(connection))
          {
            ReceiveTransaction(connection, &disconnect);
          }
          else
          {
            if (StartActiveTransaction(connection) == eTRUE)
            {
              ReceiveTransaction(connection, &disconnect);
            }
            else
            {
              RejectNewTransaction(connection, &disconnect);
            }
          }

          if (disconnect == eTRUE)
          {
            RemoveModbusConnection(connection);
            ModbusCounterUpdate(MODBUS_COUNTER_CONNECTIONS_ACTIVE);
          }
        }
      }
    }
  }
  HandleTransactionsTimeouts();
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Handle Modbus Interpretation
 \author    Michal Kochel
 \date      2019-08-23
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline void HandleInterpretation(void)
{
  T_MODBUS_TCP_TRANSACTION* pTransaction;

  pTransaction = FindModbusTransaction((TUSIGN16)TRANSACTION_CONTAINS_REQUEST);
  if (pTransaction != NULL)
  {
    ReceiveModbusRequestMessage(pTransaction, &hardwareLayerTCP_modbus);
    ProcessModbusRequestMessage(&hardwareLayerTCP_modbus);
    SentModbusResponseMessage(pTransaction, &hardwareLayerTCP_modbus);
    SendModbusTransactionResponse(pTransaction);
    ReleaseModbusTransaction(pTransaction);
  }
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief     Stops listening on sockets
 \author    Michal Kochel
 \date      2019-08-23
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static inline void StopListening(void)
{
  int32_t returnValue;

  RemoveAllModbusConnections();

  if (CI_CloseSocket(mainListeningSocket, &returnValue) != CI_OK)
  {
  }
  mainListeningSocket = -1;
  
  if (socketsSet != NULL)
  {
    CI_ReturnFdSet(socketsSet);
    socketsSet = NULL;
  }
}

static inline void ProcessModbusRequestMessage(TModbusHardwareLayer *pModbusHardwareLayer)
{
    VIP_ASSERT(pModbusHardwareLayer);

    if (pModbusHardwareLayer->modbusCommonParameters.modbusMode == MODBUS_MODE_SERVER)
    {
        // Reference to the command descriptor of the received command
        const TModbusCommandDescriptor *pModbusCommandDescriptor = (TModbusCommandDescriptor *)NULL;
        // Id of the command of the received message
        TUSIGN8 modbusCommand;
        // Result of message processing - start with an exception code of unknown command
        TModbusExceptionType modbusExceptionType = MODBUS_EXCEPTION_TYPE_ILLEGAL_FUNCTION;

        // Get the command type from the message buffer
        modbusCommand = pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer[MODBUS_PDU_FUNCTION_CODE_OFFSET];
        // Now try to get a command descriptor for it
        pModbusCommandDescriptor = Get_Modbus_Command_Descriptors(modbusCommand);
        // Was a valid command descriptor returned ?
        if (pModbusCommandDescriptor != (TModbusCommandDescriptor *)NULL)
        {
            // Remove a byte from the length - we only care about the application level data
            pModbusHardwareLayer->modbusCommonParameters.length--;
            // Now call the command descriptor's command handler
            modbusExceptionType = pModbusCommandDescriptor->pModbusHandlerFunction(pModbusCommandDescriptor,
                pModbusHardwareLayer, &pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer[MODBUS_PDU_DATA_START_OFFSET]);
        }
        // Do we need to load an exception response ?
        if (modbusExceptionType != MODBUS_EXCEPTION_TYPE_NONE)
        {
            // If so, load the appropriate response
            (void)LoadExceptionMessageModbus(pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer,
                                        modbusCommand, modbusExceptionType);
            // Only two bytes in an exception code
            pModbusHardwareLayer->modbusCommonParameters.length = 2;
        }
        else
        {
            // If not, we can send the response
            // Add the command id to the initial location in the response buffer
            pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer[MODBUS_PDU_FUNCTION_CODE_OFFSET] = modbusCommand;
            // Another byte added to the length
            pModbusHardwareLayer->modbusCommonParameters.length++;
        }
    }
}

static inline void ReceiveModbusRequestMessage(T_MODBUS_TCP_TRANSACTION* pTransaction, TModbusHardwareLayer *pModbusHardwareLayer)
{
    pModbusHardwareLayer->modbusCommonParameters.length = pTransaction->msgLength - MODBUS_UNIT_ID_SIZE;

    (void)memcpy(pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer, 
                 pTransaction->buffer + MODBUS_TCP_PDU_OFFSET, pTransaction->msgLength - MODBUS_UNIT_ID_SIZE);

    if (!pModbusHardwareLayer->modbusCommonParameters.messageProcessing)
    {
        pModbusHardwareLayer->modbusCommonParameters.messageProcessing = eTRUE ;
    }
}

static inline void SentModbusResponseMessage(T_MODBUS_TCP_TRANSACTION* pTransaction, TModbusHardwareLayer *pModbusHardwareLayer)
{
    (void)memcpy(pTransaction->buffer + MODBUS_TCP_PDU_OFFSET,
                 pModbusHardwareLayer->modbusCommonParameters.pMessageBuffer,
                 pModbusHardwareLayer->modbusCommonParameters.length);

    pTransaction->msgLength = pModbusHardwareLayer->modbusCommonParameters.length + MODBUS_UNIT_ID_SIZE;
    // We can accept messages for this layer again
    pModbusHardwareLayer->modbusCommonParameters.messageProcessing = eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Loads the appropriate exception message for a passed exception code to the Layer 7 message buffer
 \author  GL
 \date    29/07/08
 \param   [in] commandCode - Code of the command to prepare the exception response for
 \param   [in] modbusExceptionType - The exception code for the response
 \return  OK if the exception message was loaded OK
 \warning
 \test
 \n date: 2016-10-19
 \n by: ZuoChen Wang
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: 
 \bug
*/
//--------------------------------------------------------------------------------------------------
static inline TUSIGN16 LoadExceptionMessageModbus(TUSIGN8 *pMessage, TUSIGN8 commandCode, TModbusExceptionType modbusExceptionType)
{
    TUSIGN16 result = OK ;

    // Load the id of the command first
    pMessage[MODBUS_PDU_FUNCTION_CODE_OFFSET] = commandCode | MODBUS_PDU_EXCEPTION_CODE ;
    // Then add the exception code
    switch(modbusExceptionType)
    {
        case(MODBUS_EXCEPTION_TYPE_NONE):
            // No action is necessary if there's no exception code, and our result remains at OK
            break;
        case(MODBUS_EXCEPTION_TYPE_ILLEGAL_FUNCTION):
        case(MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_ADDRESS):
        case(MODBUS_EXCEPTION_TYPE_ILLEGAL_DATA_VALUE):
        case(MODBUS_EXCEPTION_TYPE_SLAVE_DEVICE_FAILURE):
            // Valid exception code, so load it into the appropriate location
            pMessage[MODBUS_PDU_EXCEPTION_CODE_OFFSET] = modbusExceptionType ;
            break ;
        case (MODBUS_EXCEPTION_TYPE_GATEWAY_PATH_UNAVAILABLE):
        case (MODBUS_EXCEPTION_TYPE_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND):
        case (MODBUS_EXCEPTION_TYPE_COUNT):  
        default:
            result = FATAL_ERROR ;
            break ;
    }
    return(result);
}