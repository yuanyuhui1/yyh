//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Modbus TCP
 Module         Memory 
 Description    Defines memory used Modbus TCP
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __MODBUS_MEMORY_H__
#define __MODBUS_MEMORY_H__

#include "MODBUS_tcp.h"

extern T_MODBUS_TCP_CONNECTION* modbusConnections;
extern T_MODBUS_TCP_TRANSACTION* modbusTransactions;
extern TCHAR* modubsPayloadBuffer;
extern TCHAR* modubsAuditLogBuffer;

TBOOL ModbusMemoryInit(void);

#endif