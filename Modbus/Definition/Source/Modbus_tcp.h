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
#ifndef __MODBUS_TCP_H__
#define __MODBUS_TCP_H__


#define MODBUS_TCP_TRANSACTION_ID_OFFSET  0
#define MODBUS_TCP_PROTOCOL_ID_OFFSET     2
#define MODBUS_TCP_LENGTH_OFFSET          4
#define MODBUS_TCP_UNIT_ID_OFFSET         6
#define MODBUS_TCP_PDU_OFFSET             7
#define MODBUS_TCP_FUNCTION_CODE_OFFSET   MODBUS_TCP_PDU_OFFSET
#define MODBUS_TCP_EXCEPTION_CODE_OFFSET  MODBUS_TCP_FUNCTION_CODE_OFFSET + 1

// Private Layer 7 definitions
#define MODBUS_PDU_FUNCTION_CODE_OFFSET     0
// Position in a layer 7 message where command data starts
#define MODBUS_PDU_DATA_START_OFFSET        1
// Position in a layer 7 message buffer for exception codes
#define MODBUS_PDU_EXCEPTION_CODE_OFFSET    1
// Bit Or'ed with command ids to generate an exception code
#define MODBUS_PDU_EXCEPTION_CODE           0x80

#define MODBUS_MBAP_HEADER_SIZE               7
#define MODBUS_MBAP_HEADER_SIZE_WITHOUT_UNIT  6
#define MODBUS_UNIT_ID_SIZE                   1
#define MODBUS_FUNCTION_CODE_SIZE             1
#define MODBUS_EXCEPTION_RESPONSE_SIZE        (MODBUS_MBAP_HEADER_SIZE + 2)

#define MODBUS_PROTOCOL_ID              0
#define MODBUS_EXCEPTION_FUNCTION_MASK  0x80
#define MODBUS_FUNCTION_CODE_MASK       0x7F
#define MODBUS_MAX_PDU_SIZE             253 
#define MODBUS_MAX_TCP_IP_ADU_SIZE      (MODBUS_MAX_PDU_SIZE + MODBUS_MBAP_HEADER_SIZE)

#define MODBUS_COMMAND_ID_READ_COILS 0x1
#define MODBUS_COMMAND_ID_READ_DISCRETE_INPUTS 0x2
#define MODBUS_COMMAND_ID_READ_HOLDING_REGISTERS 0x3
#define MODBUS_COMMAND_ID_READ_INPUT_REGISTERS 0x4
#define MODBUS_COMMAND_ID_WRITE_SINGLE_COIL 0x5
#define MODBUS_COMMAND_ID_WRITE_SINGLE_REGISTER 0x6
#define MODBUS_COMMAND_ID_WRITE_MULTIPLE_COILS 0xF
#define MODBUS_COMMAND_ID_WRITE_MULTIPLE_REGISTERS 0x10
#define MODBUS_COMMAND_ID_REPORT_SLAVE_ID 0x11

#define MODBUS_EXEP_ILLEGAL_FUNCTION 0x01 
#define MODBUS_EXEP_ILLEGAL_DATA_ADDRESS 0x02
#define MODBUS_EXEP_ILLEGAL_DATA_VALUE 0x03 
#define MODBUS_EXEP_SLAVE_DEVICE_FAILURE 0x04 
#define MODBUS_EXEP_ACKNOWLEDGE 0x05 
#define MODBUS_EXEP_SLAVE_DEVICE_BUSY 0x06 
#define MODBUS_EXEP_NEGATIVE_ACKNOWLEDGE 0x07 
#define MODBUS_EXEP_MEMORY_PARITY_ERROR 0x08 
#define MODBUS_EXEP_GATEWAY_PATH_UNAVAILABLE 0x0A 
#define MODBUS_EXEP_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND 0x0B 


typedef enum _E_MODBUS_TCP_TRANSACTION_STATE {
  TRANSACTION_NOT_USED = 1,
  TRANSACTION_EMPTY = 2,
  TRANSACTION_CONTAINS_REQUEST = 4,
} E_MODBUS_TCP_TRANSACTION_STATE;

struct _T_MODBUS_TCP_CONNECTION;

typedef struct _T_MODBUS_TCP_TRANSACTION {
  E_MODBUS_TCP_TRANSACTION_STATE state;
  TUSIGN8 buffer[MODBUS_MAX_TCP_IP_ADU_SIZE];
  TUSIGN16 bytesRead;
  TUSIGN16 msgLength;
  TINT32 socket;
  struct _T_MODBUS_TCP_CONNECTION* connection;
} T_MODBUS_TCP_TRANSACTION;

typedef struct _T_MODBUS_TCP_CONNECTION {
  TBOOL isUsed;
  TINT32 socket;
  TUSIGN32 lastRequestReceived;
  T_MODBUS_TCP_TRANSACTION* recvTransaction;
  TUSIGN32 clientIpAddress;
  TUSIGN16 clientPort;
} T_MODBUS_TCP_CONNECTION;

typedef enum _E_MODBUS_STATE
{
    MODBUS_STATE_NOT_STARTED,
    MODBUS_STATE_STARTING,
    MODBUS_STATE_STARTED,
    MODBUS_STATE_ENDING,
    MODBUS_STATE_RESET_ENDING,
    MODBUS_STATE_STARTING_FAILED
} E_MODBUS_STATE;

void InitModbusConnections(void);
T_MODBUS_TCP_CONNECTION* FindConnection(TINT32 socket);
TBOOL FreeModbusConnectionAvailable(void);
TBOOL PushModbusConnection(TINT32 socket, TUSIGN32 clientIpAddress, TUSIGN16 clientPort);
void RemoveModbusConnection(T_MODBUS_TCP_CONNECTION* pConnection);
void RemoveAllModbusConnections(void);
TUSIGN8 NumberOfActiveConnections(void);

void InitModbusTransactions(void);
T_MODBUS_TCP_TRANSACTION* GetModbusTransaction(void);
TUSIGN8 TransactionsCountOnSocket(TINT32 socket);
void BeginModbusTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction);
void SendModbusTransactionResponse(T_MODBUS_TCP_TRANSACTION* pTransaction);
void ReleaseModbusTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction);
T_MODBUS_TCP_TRANSACTION* FindModbusTransaction(TUSIGN16 state);
TBOOL ExtractMbapHeader(const TUSIGN8* pBuffer, TUSIGN16* msgLength);
TBOOL ExtractMbapHeaderInTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction);

TUSIGN16 GetTransactionId(const T_MODBUS_TCP_TRANSACTION* pTransaction);
TUSIGN16 GetMessageLength(const T_MODBUS_TCP_TRANSACTION* pTransaction);
TUSIGN8 GetUnitId(const T_MODBUS_TCP_TRANSACTION* pTransaction);
TUSIGN8 GetFunctionCode(const T_MODBUS_TCP_TRANSACTION* pTransaction);

void BuildExceptionInBuffer(TUSIGN8* pBuffer, TUSIGN8 exceptionCode);
void BuildExceptionInTransaction(T_MODBUS_TCP_TRANSACTION* pTransaction, TUSIGN8 exceptionCode);

TBOOL IsWriteFunction(const T_MODBUS_TCP_TRANSACTION* pTransaction);
TBOOL IsFunctionSupported(const T_MODBUS_TCP_TRANSACTION* pTransaction);
TBOOL ConnectionHasActiveTransaction(const T_MODBUS_TCP_CONNECTION* pConnection);
TBOOL StartActiveTransaction(T_MODBUS_TCP_CONNECTION* pConnection);

TBOOL UpdateConnectionsRecvTransaction(T_MODBUS_TCP_CONNECTION* pConnection);
void FlushConnectionRecvBuffer(const T_MODBUS_TCP_CONNECTION* pConnection, TUSIGN16 length);
void RejectNewTransaction(const T_MODBUS_TCP_CONNECTION* pConnection, TBOOL* pDisconnect);
void ReceiveTransaction(T_MODBUS_TCP_CONNECTION* pConnection, TBOOL* pDisconnect);
void HandleTransactionsTimeouts(void);

void InitModbusTaskState(void);
void ChangeModbusTaskState(E_MODBUS_STATE newState);
E_MODBUS_STATE GetModbusTaskState(void);

#endif