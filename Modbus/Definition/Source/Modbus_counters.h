//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2019.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         4WCTW
 Module         Modbus TCP
 Description    Counters for Modbus TCP
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef __MODBUS_COUNTERS_IDX_H__
#define __MODBUS_COUNTERS_IDX_H__

typedef enum _E_MODBUS_COUNTERS {
    /* 
    Modbus TCP connections currently active.
    */
    MODBUS_COUNTER_CONNECTIONS_ACTIVE = 0,

    /* Modbus TCP connections started. 
    When new connection is accepted.
    */
    MODBUS_COUNTER_CONNECTIONS_STARTED,

    /* 
    Modbus TCP connections rejected. 
    When all connections are active, all have active transactions and new wanted to be accepted and is rejected. 
    */
    MODBUS_COUNTER_CONNECTIONS_REJECTED,

    /* 
    Modbus TCP connections dropped. 
    When all connections are active, not all have active transactions and new connection was accepted.
    */
    MODBUS_COUNTER_CONNECTIONS_DROPPED,

    /* 
    Modbus TCP transaction received.
    When new transaction with correct MBAP header was received.
    */
    MODBUS_COUNTER_TRANSACTIONS_RECEIVED,
      
    /* 
    Modbus TCP transaction rejected.
    When there is no place in transactions buffer and it was not possible to read correct MBAP header.
    */
    MODBUS_COUNTER_TRANSACTIONS_REJECTED,  
    
    /* 
    Modbus TCP transaction corrupted.
    When it was not possible to correctly decode Modbus TCP MBAP header
    */
    MODBUS_COUNTER_TRANSACTIONS_CORRUPTED,

    /* 
    Modbus TCP transaction incomplete.
    When it was not possible to resegment whole Modbus TCP message wihin timeout.
    */
    MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE,

    /* 
    Number of supported Modbus TCP counters
    */
    MODBUS_COUNTER_COUNT
} E_MODBUS_COUNTERS;

TBOOL ModbusCounterInit(void);

void ModbusCounterUpdate(E_MODBUS_COUNTERS counter);

#endif