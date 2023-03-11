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
#include <stdint.h>
#include <stddef.h>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Modbus/Interface/Modbus.h"
#include "OSPL_memory.h"
#include "MODBUS_memory.h"

T_MODBUS_TCP_CONNECTION* modbusConnections = NULL;
T_MODBUS_TCP_TRANSACTION* modbusTransactions = NULL;
TCHAR* modubsPayloadBuffer = NULL;
TCHAR* modubsAuditLogBuffer = NULL;

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes memory pool for HModbus TCP handling
 \author    Michal Kochel
 \date      2020-04-10
 \return    TBOOL Is memory allocated correctly
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
TBOOL ModbusMemoryInit(void) {
    TBOOL result = eTRUE;

    modbusConnections = NULL;
    modbusTransactions = NULL;
    modubsPayloadBuffer = NULL;
    modubsAuditLogBuffer = NULL;

    //lint -e740 cast to (uint8_t**) is correct for this interface
    // Memory for Modbus TCP connections
    if (OSPL_MemAllocate(MODBUS_TCP_MAX_CLIENTS * sizeof(T_MODBUS_TCP_CONNECTION), (uint8_t**)&modbusConnections) != OSPL_SUCCESS) {
        result = eFALSE;
    }

    // Memory for Modbus TCP transactions
    if (result == eTRUE) {
        if (OSPL_MemAllocate(MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS * sizeof(T_MODBUS_TCP_TRANSACTION), (uint8_t**)&modbusTransactions) != OSPL_SUCCESS) {
            result = eFALSE;
        }
    }

    // Memory for store payload
    if (result == eTRUE) {
        if (OSPL_MemAllocate(MODBUS_MAX_PDU_SIZE * sizeof(TCHAR), (uint8_t**)&modubsPayloadBuffer) != OSPL_SUCCESS) {
            result = eFALSE;
        }
    }

    // Memory payload converton to string
    if (result == eTRUE) {
        // Needed to present pdu in hex format with NULL termination
        if (OSPL_MemAllocate((MODBUS_MAX_PDU_SIZE * 2 + 1) * sizeof(TCHAR), (uint8_t**)&modubsAuditLogBuffer) != OSPL_SUCCESS) {
            result = eFALSE;
        }
    }

    if (result == eFALSE) {
        if (modbusConnections != NULL) {
            (void)OSPL_MemFree((uint8_t*)modbusConnections);
            modbusConnections = NULL;
        }
        if (modbusTransactions != NULL) {
            (void)OSPL_MemFree((uint8_t*)modbusTransactions);
            modbusTransactions = NULL;
        }
        if (modubsPayloadBuffer != NULL) {
            (void)OSPL_MemFree((uint8_t*)modubsPayloadBuffer);
            modubsPayloadBuffer = NULL;
        }
        if (modubsAuditLogBuffer != NULL) {
            (void)OSPL_MemFree((uint8_t*)modubsAuditLogBuffer);
            modubsAuditLogBuffer = NULL;
        }
    }

    return result;
}
