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

#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "MODBUS_counters.h"
#include "MODBUS_tcp.h"
#include "Diagnosis/Counters/Interface/counters.h"
#include "Diagnosis/Counters/Interface/counters_idx.h"

#define MODBUS_COUNTER_BASASKETS_PERIOD 60

#define MODBUS_COUNTER_CONNECTIONS_ACTIVE_BASKETS 15
#define MODBUS_COUNTER_CONNECTIONS_STARTED_BASKETS 15
#define MODBUS_COUNTER_CONNECTIONS_REJECTED_BASKETS 15
#define MODBUS_COUNTER_CONNECTIONS_DROPPED_BASKETS 15
#define MODBUS_COUNTER_TRANSACTIONS_RECEIVED_BASKETS 15
#define MODBUS_COUNTER_TRANSACTIONS_REJECTED_BASKETS 15
#define MODBUS_COUNTER_TRANSACTIONS_CORRUPTED_BASKETS 15
#define MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE_BASKETS 15

static T_COUNTER modbusCounters[MODBUS_COUNTER_COUNT];

static TUSIGN32 modbusConnectionsActive[MODBUS_COUNTER_CONNECTIONS_ACTIVE_BASKETS];

static TUSIGN32 modbusConnectionsStarted[MODBUS_COUNTER_CONNECTIONS_STARTED_BASKETS];
static TUSIGN32 modbusConnectionsStartedCum;

static TUSIGN32 modbusConnectionsRejected[MODBUS_COUNTER_CONNECTIONS_REJECTED_BASKETS];
static TUSIGN32 modbusConnectionsRejectedCum;

static TUSIGN32 modbusConnectionsDropped[MODBUS_COUNTER_CONNECTIONS_DROPPED_BASKETS];
static TUSIGN32 modbusConnectionsDroppedCum;

static TUSIGN32 modbusTransactionsReceived[MODBUS_COUNTER_CONNECTIONS_DROPPED_BASKETS];
static TUSIGN32 modbusTransactionsReceivedCum;

static TUSIGN32 modbusTransactionsRejected[MODBUS_COUNTER_TRANSACTIONS_REJECTED_BASKETS];
static TUSIGN32 modbusTransactionsRejectedCum;

static TUSIGN32 modbusTransactionsCorrupted[MODBUS_COUNTER_TRANSACTIONS_REJECTED_BASKETS];
static TUSIGN32 modbusTransactionsCorruptedCum;

static TUSIGN32 modbusTransactionsIncompleate[MODBUS_COUNTER_TRANSACTIONS_REJECTED_BASKETS];
static TUSIGN32 modbusTransactionsIncompleateCum;

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes counters 
 \author    Michal Kochel
 \date      2019-07-30
 \return    TBOOL If counters were initialised correctly
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
TBOOL ModbusCounterInit(void) {
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_ACTIVE].counterType = COUNTER_TYPE_LAST_CARRY;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_ACTIVE].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_ACTIVE].numberOfBaskets = MODBUS_COUNTER_CONNECTIONS_ACTIVE_BASKETS;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_ACTIVE].baskets = modbusConnectionsActive;

    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].numberOfBaskets = MODBUS_COUNTER_CONNECTIONS_STARTED_BASKETS;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].baskets = modbusConnectionsStarted;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_STARTED].cumulativeValue = &modbusConnectionsStartedCum;

    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].numberOfBaskets = MODBUS_COUNTER_CONNECTIONS_REJECTED_BASKETS;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].baskets = modbusConnectionsRejected;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_REJECTED].cumulativeValue = &modbusConnectionsRejectedCum;

    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].numberOfBaskets = MODBUS_COUNTER_CONNECTIONS_REJECTED_BASKETS;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].baskets = modbusConnectionsDropped;
    modbusCounters[MODBUS_COUNTER_CONNECTIONS_DROPPED].cumulativeValue = &modbusConnectionsDroppedCum;    

    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].numberOfBaskets = MODBUS_COUNTER_TRANSACTIONS_RECEIVED_BASKETS;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].baskets = modbusTransactionsReceived;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_RECEIVED].cumulativeValue = &modbusTransactionsReceivedCum;  

    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].numberOfBaskets = MODBUS_COUNTER_TRANSACTIONS_REJECTED_BASKETS;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].baskets = modbusTransactionsRejected;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_REJECTED].cumulativeValue = &modbusTransactionsRejectedCum;  

    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].numberOfBaskets = MODBUS_COUNTER_TRANSACTIONS_CORRUPTED_BASKETS;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].baskets = modbusTransactionsCorrupted;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_CORRUPTED].cumulativeValue = &modbusTransactionsCorruptedCum;

    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].counterType = COUNTER_TYPE_CUMULATIVE;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].basketType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].numberOfBaskets = MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE_BASKETS;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].baskets = modbusTransactionsIncompleate;
    modbusCounters[MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE].cumulativeValue = &modbusTransactionsIncompleateCum;

    TBOOL initCorrect = eTRUE;
    for (TUSIGN8 index = 0; index < MODBUS_COUNTER_COUNT; ++index) {
        modbusCounters[index].groupId = COUNTER_GROUP_MODBUSTCP;
        modbusCounters[index].counterId = index;
        modbusCounters[index].basketPeriodInSeconds = MODBUS_COUNTER_BASASKETS_PERIOD;

        if (CountersInitialise(modbusCounters + index) == eFALSE) {
            initCorrect = eFALSE;
        }
    }

    return initCorrect;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Updates counter
 \author    Michal Kochel
 \date      2019-07-30
 \param     counter Counter which should be updated
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
void ModbusCounterUpdate(E_MODBUS_COUNTERS counter) {
    switch (counter) {
        case MODBUS_COUNTER_CONNECTIONS_ACTIVE: {
            TUSIGN32 activeConnections = NumberOfActiveConnections();
            CountersUpdate(modbusCounters + MODBUS_COUNTER_CONNECTIONS_ACTIVE, &activeConnections);
        } break;

        case MODBUS_COUNTER_CONNECTIONS_STARTED:
        case MODBUS_COUNTER_CONNECTIONS_REJECTED:
        case MODBUS_COUNTER_CONNECTIONS_DROPPED:
        case MODBUS_COUNTER_TRANSACTIONS_RECEIVED:
        case MODBUS_COUNTER_TRANSACTIONS_REJECTED:
        case MODBUS_COUNTER_TRANSACTIONS_CORRUPTED:
        case MODBUS_COUNTER_TRANSACTIONS_INCOMPLETE:
        case MODBUS_COUNTER_COUNT:
        default: {
            TUSIGN32 newValue = 1;
            CountersUpdate(modbusCounters + counter, &newValue);
        } break;
    }
}
