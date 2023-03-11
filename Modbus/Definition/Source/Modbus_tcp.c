//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2019.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         4WCTW
 Module         Modbus TCP
 Description    
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <limits.h>
#include <stddef.h>
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
#include "Modbus_register_table.h"
#include "Modbus_command_descriptor.h"

#include "MODBUS_tcp.h"
#include "Modbus_counters.h"
#include "Modbus_memory.h"
//#include "system_dbug.h"
#include "CI_codes.h"
#include "CI_socket_api.h"

#include "Services/Interface/math_helper.h"


//#include <assert.h>

#define TEMP_BUFFER_SIZE 50
static RTOS_MUTEX_HANDLE modbusTaskState_Mutex;
static E_MODBUS_STATE modbusTaskState = MODBUS_STATE_NOT_STARTED;

static TUSIGN16 ExtracTUSIGN16(const TUSIGN8* buffer)
{
  TUSIGN16 value;
  value = ((TUSIGN16)buffer[0]) << 8;
  value |= buffer[1];

  return value;
}

static void SetTUSIGN16(TUSIGN8* buffer, TUSIGN16 value)
{
  buffer[0] = (value >> 8) & 0xFF;
  buffer[1] = value & 0xFF;
}

static void ZeroModbusConnection(T_MODBUS_TCP_CONNECTION* pConnection)
{
  pConnection->socket = -1;
  pConnection->isUsed = eFALSE;
  pConnection->recvTransaction = NULL;
}

static void ZeroModbusTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  pTransaction->socket = -1;
  pTransaction->bytesRead = 0;
  pTransaction->msgLength = 0;
  pTransaction->state = TRANSACTION_NOT_USED;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Sends data in non-blocking socket
 \author    Michal Kochel
 \date      2020-06-18
 \param     socket Socket on which to send
 \param     buf_p Data to send
 \param     length Data size in buf_p
 \param     flags Flags used on send
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
static int32_t CI_SendNonBlock(int32_t socket, const void *buf_p, int32_t length, int32_t flags)
{
  void* sendSocketSet;
  int32_t bytesSent;
  int32_t bytesToSend = length;
  int32_t selectResult;

  sendSocketSet = CI_GetFdSet();
  CI_FdZero(sendSocketSet);
  CI_FdSet(socket, sendSocketSet);

  TUSIGN8* dataBuffer = (TUSIGN8*)buf_p;
  do {
    bytesSent = 0;
    selectResult = CI_Select(socket + 1, NULL, sendSocketSet, NULL, 10000);
    if (selectResult > 0)
    {
      if (CI_FdIsSet(socket, sendSocketSet))
      {
        bytesSent = CI_Send(socket, dataBuffer, bytesToSend, flags);
        if (bytesSent > 0)
        {
          bytesToSend -= bytesSent;
          dataBuffer += bytesSent;
        }
      }
    }
  } while (bytesToSend > 0 && bytesSent != CI_ERROR);

  CI_ReturnFdSet(sendSocketSet);

  return bytesSent != CI_ERROR ? length : bytesSent;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes Modbus connections
 \author    Michal Kochel
 \date      2019-07-16
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
void InitModbusConnections(void)
{
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    ZeroModbusConnection(modbusConnections + index);
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Finds connection associated with given socket
 \author    Michal Kochel
 \date      2019-07-16
 \param    socket Socket descriptor
 \return T_MODBUS_TCP_CONNECTION* Connection or NULL if connection was not found
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
T_MODBUS_TCP_CONNECTION* FindConnection(TINT32 socket)
{
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eTRUE && modbusConnections[index].socket == socket)
    {
      return modbusConnections + index;
    }
  }
  return NULL;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Pushes connection to active connections list
 \author    Michal Kochel
 \date      2019-07-16
 \param     socket Socket descriptor
 \param     clientIpAddress Connecting client IP addres
 \param     clientPort Connecting client port 
 \return TBOOL State if connection was pusshed to the list
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
TBOOL PushModbusConnection(TINT32 socket, TUSIGN32 clientIpAddress, TUSIGN16 clientPort)
{
  T_MODBUS_TCP_CONNECTION* pFreeConnection = NULL;
  TUSIGN32 minLastRequestReceived = UINT_MAX;
  TBOOL needsRemoval = eTRUE;

  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eFALSE)
    {
      pFreeConnection = modbusConnections + index;
      needsRemoval = eFALSE;
      break;
    }
    else
    {
      if (TransactionsCountOnSocket(modbusConnections[index].socket) == 0)
      {
        if (modbusConnections[index].lastRequestReceived < minLastRequestReceived)
        {
          minLastRequestReceived = modbusConnections[index].lastRequestReceived;
          pFreeConnection = modbusConnections + index;
        }
      }
    }
  }

  if (pFreeConnection != NULL)
  {
    if (needsRemoval == eTRUE)
    {
      ModbusCounterUpdate(MODBUS_COUNTER_CONNECTIONS_DROPPED);
      RemoveModbusConnection(pFreeConnection);
    }

    pFreeConnection->socket = socket;
    pFreeConnection->isUsed = eTRUE;
    pFreeConnection->recvTransaction = NULL;
    pFreeConnection->lastRequestReceived = GetTickCount_RTOS_TASK();
    pFreeConnection->clientIpAddress = clientIpAddress;
    pFreeConnection->clientPort = clientPort;


    ModbusCounterUpdate(MODBUS_COUNTER_CONNECTIONS_ACTIVE);
    ModbusCounterUpdate(MODBUS_COUNTER_CONNECTIONS_STARTED);
    
    return eTRUE;
  }


  ModbusCounterUpdate(MODBUS_COUNTER_CONNECTIONS_REJECTED);

  return eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Removes connection from active list
 \author    Michal Kochel
 \date      2019-07-16
 \param     pConnection Connection
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
void RemoveModbusConnection(T_MODBUS_TCP_CONNECTION* pConnection)
{
  VIP_ASSERT(pConnection != NULL);

  int32_t errorCode;

  // Stop receving on socket
  (void)CI_Shutdown(pConnection->socket, CI_SHUT_RDWR, &errorCode);

  // Release transaction which is beeing received
  if (pConnection->recvTransaction != NULL)
  {
    ReleaseModbusTransaction(pConnection->recvTransaction);
  }

  // Release all transaction that might be ongoing
  for (size_t index = 0; index < MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS; ++index)
  {
    if (modbusTransactions[index].socket == pConnection->socket)
    {
      ReleaseModbusTransaction(modbusTransactions + index);
    }
  }

  // Close socket
  if (CI_CloseSocket(pConnection->socket, &errorCode) != CI_OK)
  {
  }


  ZeroModbusConnection(pConnection);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Removes all connections from active list
 \author    Michal Kochel
 \date      2019-11-14
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
void RemoveAllModbusConnections(void)
{
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eTRUE)
    {
      RemoveModbusConnection(modbusConnections + index);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Number of active connections
 \author    Michal Kochel
 \date      2019-07-19
 \return    TUSIGN8 number of active connections
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
TUSIGN8 NumberOfActiveConnections(void)
{
  TUSIGN8 count = 0;
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eTRUE)
    {
      count++;
    }
  }
  return count;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes Modbus transactions set
 \author    Michal Kochel
 \date      2019-07-16
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
void InitModbusTransactions(void)
{
  for (size_t index = 0; index < MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS; ++index)
  {
    ZeroModbusTransaction(modbusTransactions + index);
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Returns free transacation slot
 \author    Michal Kochel
 \date      2019-07-16
 \return    T_MODBUS_TCP_TRANSACTION* Pointer to free transaction or NULL if there is no free transaction
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
T_MODBUS_TCP_TRANSACTION* GetModbusTransaction(void)
{
  for (size_t index = 0; index < MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS; ++index)
  {
    if (modbusTransactions[index].state == TRANSACTION_NOT_USED)
    {
      ZeroModbusTransaction(modbusTransactions + index);
      modbusTransactions[index].state = TRANSACTION_EMPTY;
      return modbusTransactions + index;
    }
  }
  return NULL;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Number of active transactions on given socket
 \author    Michal Kochel
 \date      2019-07-16
 \param     socket Socket descriptor
 \return    TUSIGN8 Number of active transactions
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
TUSIGN8 TransactionsCountOnSocket(TINT32 socket)
{
  TUSIGN8 transactionsCount = 0;

  for (size_t index = 0; index < MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS; ++index)
  {
    if (modbusTransactions[index].state != TRANSACTION_NOT_USED && modbusTransactions[index].socket == socket)
    {
      transactionsCount++;
    }
  }

  return transactionsCount;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes transaction on given socket
 \author    Michal Kochel
 \date      2019-07-16
 \param     pTransaction Transaction which will be initialized
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
void BeginModbusTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  pTransaction->state = TRANSACTION_CONTAINS_REQUEST;
}




//--------------------------------------------------------------------------------------------------
/*!
 \brief     Transmits transaction to Modbus master
 \author    Michal Kochel
 \date      2019-07-16
 \param     transaction Transaction which will be sent
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
void SendModbusTransactionResponse(T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  // Fill length
  SetTUSIGN16(pTransaction->buffer + MODBUS_TCP_LENGTH_OFFSET, pTransaction->msgLength);

  // Send data on socket
  int32_t returnValue = CI_SendNonBlock(pTransaction->socket, pTransaction->buffer, pTransaction->msgLength + MODBUS_MBAP_HEADER_SIZE_WITHOUT_UNIT, 0);
  if (returnValue >= 0)
  {
  }
  else
  {
    int32_t socketError = -1;
    (void)CI_GetLastSocketError(pTransaction->socket, &socketError);
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Release transaction slot
 \author    Michal Kochel
 \date      2019-07-16
 \param     pTransaction Transaction which will be released
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
void ReleaseModbusTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  ZeroModbusTransaction(pTransaction);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Find transaction
 \author    Michal Kochel
 \date      2019-07-16
 \param     state State in which transaction has to be
 \param     notInSet State/s in which no other transaction can be
 \return    T_MODBUS_TCP_TRANSACTION* Transaction which meets given conditions or NULL if such transaction was not found
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
T_MODBUS_TCP_TRANSACTION* FindModbusTransaction(TUSIGN16 state)
{
  T_MODBUS_TCP_TRANSACTION* pTransaction = NULL;

  for (size_t index = 0; index < MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS; ++index)
  {
    if (modbusTransactions[index].state == (E_MODBUS_TCP_TRANSACTION_STATE)state)
    {
      pTransaction = modbusTransactions + index;
    }
  }

  return pTransaction;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Extracts MBAP header information from buffer
 \author    Michal Kochel
 \date      2019-07-16
 \param     pBuffer Buffer containing header
 \param     msgLength Extracted message length
 \return    TBOOL If is correct MBAP header
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
TBOOL ExtractMbapHeader(const TUSIGN8* pBuffer, TUSIGN16* msgLength)
{
  TUSIGN16 protocol;

  protocol = ExtracTUSIGN16(pBuffer + MODBUS_TCP_PROTOCOL_ID_OFFSET);
  *msgLength = ExtracTUSIGN16(pBuffer + MODBUS_TCP_LENGTH_OFFSET);
  return (protocol == MODBUS_PROTOCOL_ID && (*msgLength > 0 && *msgLength <= MODBUS_MAX_PDU_SIZE)) ? eTRUE : eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Extracts MBAP header information from buffer
 \author    Michal Kochel
 \date      2019-07-16
 \param     pTransaction Transaction with data from whihc to get header
 \return    TBOOL If is correct MBAP header
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
TBOOL ExtractMbapHeaderInTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  return ExtractMbapHeader(pTransaction->buffer, &(pTransaction->msgLength));
}

TUSIGN16 GetTransactionId(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  return ExtracTUSIGN16(pTransaction->buffer + MODBUS_TCP_TRANSACTION_ID_OFFSET); 
}

TUSIGN16 GetMessageLength(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  return ExtracTUSIGN16(pTransaction->buffer + MODBUS_TCP_LENGTH_OFFSET); 
}

TUSIGN8 GetUnitId(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  return *(pTransaction->buffer + MODBUS_TCP_UNIT_ID_OFFSET); 
}

TUSIGN8 GetFunctionCode(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  return *(pTransaction->buffer + MODBUS_TCP_FUNCTION_CODE_OFFSET); 
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief     Builds exception message in buffer
 \author    Michal Kochel
 \date      2019-07-17
 \param     pBuffer Buffer with request header
 \param     exceptionCode Exception code which will be set
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
void BuildExceptionInBuffer(TUSIGN8* pBuffer, TUSIGN8 exceptionCode)
{
  // Set exeption on function code
  pBuffer[MODBUS_TCP_FUNCTION_CODE_OFFSET] |= MODBUS_EXCEPTION_FUNCTION_MASK;

  // Set exeption code
  pBuffer[MODBUS_TCP_EXCEPTION_CODE_OFFSET] = exceptionCode;

  // Set length
  SetTUSIGN16(pBuffer + MODBUS_TCP_LENGTH_OFFSET, 3);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Builds exception message in transaction
 \author    Michal Kochel
 \date      2019-07-17
 \param     pTransaction Transaction with request header
 \param     exceptionCode Exception code which will be set
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
void BuildExceptionInTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction, TUSIGN8 exceptionCode)
{
  BuildExceptionInBuffer(pTransaction->buffer, exceptionCode);
  pTransaction->msgLength = 3;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Returns if Modbus function in transaction is write fucntion
 \author    Michal Kochel
 \date      2020-04-10
 \param     pTransaction Transaction which will be cheked
 \return    TBOOL If transaction contains write function
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
TBOOL IsWriteFunction(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  if ((pTransaction->buffer[MODBUS_TCP_FUNCTION_CODE_OFFSET] & MODBUS_FUNCTION_CODE_MASK) == MODBUS_COMMAND_ID_WRITE_SINGLE_COIL || 
      (pTransaction->buffer[MODBUS_TCP_FUNCTION_CODE_OFFSET] & MODBUS_FUNCTION_CODE_MASK) == MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER || 
      (pTransaction->buffer[MODBUS_TCP_FUNCTION_CODE_OFFSET] & MODBUS_FUNCTION_CODE_MASK) == MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS || 
      (pTransaction->buffer[MODBUS_TCP_FUNCTION_CODE_OFFSET] & MODBUS_FUNCTION_CODE_MASK) == MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS)
  {
    return eTRUE;
  }
  else
  {
    return eFALSE;
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Returns if Modbus function in transaction is supported
 \author    Michal Kochel
 \date      2019-07-17
 \param     pTransaction Transaction which will be cheked
 \return    TBOOL If transaction contains supported function
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
TBOOL IsFunctionSupported(const T_MODBUS_TCP_TRANSACTION* pTransaction)
{
  TUSIGN8 writeFunctionsEnabled = MODBUS_WRITE_FUNCTIONS_DISABLED;
  TBOOL supported = eTRUE;
  
  if (modbus_Get((TUSIGN16)MODBUS_IDX_writeFunctionsEnabled, (TINT16)WHOLE_OBJECT, &writeFunctionsEnabled) == (TUSIGN16)OK)
  {
    if (writeFunctionsEnabled == MODBUS_WRITE_FUNCTIONS_DISABLED)
    {
      supported = IsWriteFunction(pTransaction) == eTRUE ? eFALSE : eTRUE;
    }
  }
  return supported;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Checks is there an ongoing transaction on connection
 \author    Michal Kochel
 \date      2019-07-17
 \param     pConnection Connection which will be checked
 \return    TBOOL If free transaction was found
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
TBOOL ConnectionHasActiveTransaction(const T_MODBUS_TCP_CONNECTION* pConnection)
{
    return (pConnection->recvTransaction != NULL) ? eTRUE : eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Tries to find free modbus transaction and sets it in connection
 \author    Michal Kochel
 \date      2019-07-17
 \param     pConnection Connection for which new transaction will be fetched
 \return    TBOOL If free transaction was found
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
TBOOL StartActiveTransaction(T_MODBUS_TCP_CONNECTION* pConnection)
{
  pConnection->recvTransaction = GetModbusTransaction();
  
  if (pConnection->recvTransaction != NULL)
  {
    pConnection->recvTransaction->socket = pConnection->socket;
    pConnection->recvTransaction->connection = pConnection;
    pConnection->lastRequestReceived = GetTickCount_RTOS_TASK();
    return eTRUE;
  }
  return eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Flushes recv buffer of connection
 \author    Michal Kochel
 \date      2019-07-17
 \param     pConnection Connection whos buffer will be flushed
 \param     length Max length to flush
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
void FlushConnectionRecvBuffer(const T_MODBUS_TCP_CONNECTION* pConnection, TUSIGN16 length)
{
  void* recvSocketSet;
  int32_t selectResult;
  TUSIGN8 tempBuffer[TEMP_BUFFER_SIZE];
  int32_t bytesRead;
  int32_t bytesToRead = length;

  tempBuffer[0] = 0;

  recvSocketSet = CI_GetFdSet();
  CI_FdZero(recvSocketSet);
  CI_FdSet(pConnection->socket, recvSocketSet);

  do {
    selectResult = CI_Select(pConnection->socket + 1, recvSocketSet, NULL, NULL, 10000);
    if (selectResult > 0)
    {
      if (CI_FdIsSet(pConnection->socket, recvSocketSet))
      {
        bytesRead = CI_Recv(pConnection->socket, tempBuffer, min_tint32(bytesToRead, TEMP_BUFFER_SIZE), 0);
        if (bytesRead > 0)
        {
          bytesToRead -= bytesRead;
        }
        else
        {
          break;
        }
      }
    }
    else
    {
      break;
    }
  } while (bytesToRead > 0);

  CI_ReturnFdSet(recvSocketSet);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Rejects new transaction
 \author    Michal Kochel
 \date      2019-07-17
 \param     pConnection Connection whos buffer will be flushed
 \param     pDisconnect Output if connection should be disconnected 
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
void RejectNewTransaction(const T_MODBUS_TCP_CONNECTION* pConnection, TBOOL* pDisconnect)
{
  TUSIGN8 exceptionResponse[MODBUS_EXCEPTION_RESPONSE_SIZE];
  int32_t bytesRead;
  E_MODBUS_COUNTERS counterToUpdate = MODBUS_COUNTER_TRANSACTIONS_REJECTED;

  exceptionResponse[0] = 0;

  bytesRead = CI_Recv(pConnection->socket, exceptionResponse, MODBUS_EXCEPTION_RESPONSE_SIZE, 0);
  if (bytesRead == MODBUS_EXCEPTION_RESPONSE_SIZE)
  {
    TUSIGN16 length;
    
    if (ExtractMbapHeader(exceptionResponse, &length) == eTRUE)
    {
      FlushConnectionRecvBuffer(pConnection, length);
      BuildExceptionInBuffer(exceptionResponse, MODBUS_EXEP_SLAVE_DEVICE_BUSY);

      if (CI_SendNonBlock(pConnection->socket, exceptionResponse, MODBUS_EXCEPTION_RESPONSE_SIZE, 0) >= 0)
      {
        counterToUpdate = MODBUS_COUNTER_TRANSACTIONS_REJECTED;
        *pDisconnect = eFALSE;
      }
    }
  }
  ModbusCounterUpdate(counterToUpdate);
  *pDisconnect = eTRUE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Receive new transaction
 \author    Michal Kochel
 \date      2019-07-17
 \param     pConnection Connection on which reception will occur
 \param     pDisconnect Output if connection should be disconnected 
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
void ReceiveTransaction(T_MODBUS_TCP_CONNECTION* pConnection, TBOOL* pDisconnect)
{
  int32_t bytesRead;
  int bytesToRead;
  T_MODBUS_TCP_TRANSACTION* pTransaction = pConnection->recvTransaction;

  *pDisconnect = eFALSE;

  // Firstly read header only
  if (pTransaction->bytesRead < MODBUS_MBAP_HEADER_SIZE)
  {
    bytesToRead = MODBUS_MBAP_HEADER_SIZE - pTransaction->bytesRead;
  }
  // And then rest of message
  else
  {
    bytesToRead = (pTransaction->msgLength + MODBUS_MBAP_HEADER_SIZE_WITHOUT_UNIT) - pTransaction->bytesRead;
  }

  bytesRead = CI_Recv(pConnection->socket, pTransaction->buffer + pTransaction->bytesRead, bytesToRead, 0);

  if (bytesRead > 0)
  {
    pTransaction->bytesRead += (TUSIGN16)bytesRead;
    // If we have read full header
    if (pTransaction->bytesRead == MODBUS_MBAP_HEADER_SIZE)
    {
      if (ExtractMbapHeaderInTransaction(pTransaction) == eFALSE)
      {
        ModbusCounterUpdate(MODBUS_COUNTER_TRANSACTIONS_CORRUPTED);
        *pDisconnect = eTRUE;
      }
    }
    // If expected size was read
    else
    {
      if (bytesRead == bytesToRead)
      {
        // Check if 
        if (IsFunctionSupported(pTransaction) == eTRUE)
        {
          ModbusCounterUpdate(MODBUS_COUNTER_TRANSACTIONS_RECEIVED);
          BeginModbusTransaction(pTransaction);
          pConnection->recvTransaction = NULL;
        }
        else
        {
          ModbusCounterUpdate(MODBUS_COUNTER_TRANSACTIONS_REJECTED);
          BuildExceptionInTransaction(pTransaction, MODBUS_EXCEPTION_TYPE_ILLEGAL_FUNCTION);
          SendModbusTransactionResponse(pTransaction);
          ReleaseModbusTransaction(pTransaction);
        }
      }
    }
  }
  else
  {
    if (pTransaction->bytesRead > 0)
    {
      ModbusCounterUpdate(MODBUS_COUNTER_TRANSACTIONS_CORRUPTED);
    }

    if (bytesRead == 0)
    {
    }
    else
    {
    }
    *pDisconnect = eTRUE;
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Handles if transaction buffers under receival should be freed
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
void HandleTransactionsTimeouts()
{
  for (size_t index = 0; index < MODBUS_TCP_MAX_CLIENTS; ++index)
  {
    if (modbusConnections[index].isUsed == eTRUE && modbusConnections[index].recvTransaction != NULL)
    {
      if ((GetTickCount_RTOS_TASK() - modbusConnections[index].lastRequestReceived) > MODBUS_TCP_ADU_RESEGMENTATION_TIMEOUT_IN_MS)
      {
        ReleaseModbusTransaction(modbusConnections[index].recvTransaction);
        ModbusCounterUpdate(MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE);
        modbusConnections[index].recvTransaction = NULL;
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes state of Modbus TCP task
 \author    Michal Kochel
 \date      2019-08-26
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
void InitModbusTaskState(void)
{
  CreateRecursive_RTOS_MUTEX(modbusTaskState_Mutex);
  modbusTaskState = MODBUS_STATE_NOT_STARTED;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Changes state of Modbus TCP task
 \author    Michal Kochel
 \date      2019-07-24
 \param     newState Target state
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
void ChangeModbusTaskState(E_MODBUS_STATE newState)
{
    TBOOL allow = eFALSE;

    TakeRecursive_RTOS_MUTEX(modbusTaskState_Mutex, RTOS_WAIT_FOREVER);
    switch (newState)
    {
        case MODBUS_STATE_NOT_STARTED:
            if (modbusTaskState == MODBUS_STATE_ENDING ||    // Modbus TCP was disabled
                modbusTaskState == MODBUS_STATE_STARTING)    // Failed to start Modbus TCP
            {
                allow = eTRUE;
            }
            break;
        case MODBUS_STATE_STARTING:
            if (modbusTaskState == MODBUS_STATE_NOT_STARTED ||    // Modbus TCP is starting
                modbusTaskState == MODBUS_STATE_ENDING ||         // Modbus TCP was enabled during previous Modbus TCP ending
                modbusTaskState == MODBUS_STATE_RESET_ENDING)     // Modbus TCP was restarted
            {    
                allow = eTRUE;
            }
            break;
        case MODBUS_STATE_STARTED:
            if (modbusTaskState == MODBUS_STATE_STARTING)       // Modbus TCP was correctly started
            {
                allow = eTRUE;
            }
            break;
        case MODBUS_STATE_RESET_ENDING:
        case MODBUS_STATE_ENDING:
            if (modbusTaskState == MODBUS_STATE_STARTING ||    // Modbus TCP is ended while it was starting
                modbusTaskState == MODBUS_STATE_STARTED)     // Modbus TCP is ended while it was running
            {
                allow = eTRUE;
            }
            break;
        case MODBUS_STATE_STARTING_FAILED:
            allow = eTRUE;
            break;
        default:
            break;
    }
    if (allow == eTRUE)
    {
        modbusTaskState = newState;
    }
    GiveRecursive_RTOS_MUTEX(modbusTaskState_Mutex);
}

E_MODBUS_STATE GetModbusTaskState(void)
{
  return modbusTaskState;
}
