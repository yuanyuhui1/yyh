
//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2019.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         4WCTW
 Module         Ethernet
 Description    Counters for Ethernet
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//#include "../../System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
//#include "../../T_DATA_OBJ/Interface/simple_type.h"
//#include "../../T_DATA_OBJ/Interface/t_data_obj.h"
//#include "../../T_Unit/Interface/t_unit.h"
#include "Diagnosis/Counters/Interface/counters.h"
#include "Diagnosis/Counters/Interface/counters_idx.h"
#include "Ethernet_counters.h"
#include "OSPL_memory.h"
#include "CI_system_api.h"
#include "CI_system_types.h"
//#include "CI_codes.h"
#include <string.h>
//#include "system_dbug.h"

typedef uint32_t T_PORT_ETHSTATS[18];
typedef T_PORT_ETHSTATS T_ETHSTATS[2];

T_COUNTER* ethernetEthPort1Counters = NULL;
static TUSIGN32* ethernetEthPort1CountersValues = NULL;

T_COUNTER* ethernetEthPort2Counters = NULL;
static TUSIGN32* ethernetEthPort2CountersValues = NULL;

T_COUNTER* ethernetEthTcpIpCounters = NULL;
static TUSIGN32* ethernetEthTcpIpCountersValues = NULL;


static T_ETHSTATS* ciEthStats = NULL;
static CI_statistic_t* ciTcpIpStats = NULL;

static uint8_t oldSlotOfCiEthStats = 0;
static uint8_t activeSlotOfCiEthStats = 0;

#define CI_ETH_STATS_SLOTS_COUNT 2
#define ETHERNET_ETH_COUNTER_BASKETS 15
#define ETHERNET_ETH_COUNTER_BASASKETS_PERIOD 60
#define ETHERNET_ETH_COUNTER_ENTRY_SIZE (ETHERNET_ETH_COUNTER_BASKETS + 1)
#define ETHERNET_ETH_COUNTER_ENTRY_CUMULATIVEVALUE_OFFSET 0
#define ETHERNET_ETH_COUNTER_ENTRY_BASKETS_OFFSET 1

typedef enum _E_CI_ETH_STATS {
    ETHSTATS_BYTES_RECEIVED = 0,
    ETHSTATS_UNICAST_PACKETS_RECEIVED = 1,
    ETHSTATS_BROADCAST_PACKETS_RECEIVED = 2,
    ETHSTATS_MULTICAST_PACKETS_RECEIVED = 3,
    ETHSTATS_FRAMES_RECEIVED_WITH_ALIGNMENT_ERROR = 4,
    ETHSTATS_FRAMES_RECEIVED_WITH_CRC_FCS_ERROR = 5,
    ETHSTATS_FRAMES_RECEIVED_WITH_LARGE_FRAME_ERROR = 6,
//    ETHSTATS_FRAMES_WITH_RX_MAC_ERRORS = 7,
    ETHSTATS_BYTES_TRANSMITTED = 8,
    ETHSTATS_UNICAST_PACKETS_TRANSMITTED = 9,
    ETHSTATS_BROADCAST_PACKETS_TRANSMITTED = 10,
    ETHSTATS_MULTICAST_PACKETS_TRANSMITTED = 11,
    ETHSTATS_FRAMES_TRANSMITTED_AFTER_SINGLE_COLLISION = 12,
    ETHSTATS_FRAMES_TRANSMITTED_AFTER_MULTIPLE_COLLISIONS = 13,
    ETHSTATS_FRAMES_DROPPED_AFTER_EXCESSIVE_COLLISIONS = 14, 
//    ETHSTATS_FRAMES_WITH_A_COLLISION_AFTER_512_BITS = 15,
    ETHSTATS_FRAMES_DELAYED_BY_TRAFFIC = 16
//    ETHSTATS_FRAMES_WITH_TX_MAC_ERRORS = 17,
//    ETHSTATS_COUNT
} E_CI_ETH_STATS;

static inline uint32_t GetDifference(uint32_t oldValue, uint32_t newValue) {
    if (oldValue <= newValue) {
        return newValue - oldValue;
    }
    else {
        return ((uint32_t)0xFFFFFFFF - oldValue) + newValue;
    }
}

static inline uint32_t GetEthStatDiff(uint8_t portIndex, uint8_t oldSlot, uint8_t newSlot, E_CI_ETH_STATS ethStat) {
    return GetDifference(ciEthStats[oldSlot][portIndex][ethStat], ciEthStats[newSlot][portIndex][ethStat]);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes  ethernet counters for given port
 \author    Michal Kochel
 \date      2019-10-25
 \param     ethernetEthCounters Counters which will be initilaized
 \param     ethernetEthCountersValues Buffer with values for counters
 \param     groupId Counters group id
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
static TBOOL InitEthernetCounters(T_COUNTER* ethernetEthCounters, TUSIGN32* ethernetEthCountersValues, TUSIGN8 groupId) {
    TBOOL initCorrect = eTRUE;

    for (TUSIGN8 index = 0 ; index < ETHERNET_ETH_COUNTER_COUNT && initCorrect == eTRUE; ++index) {
        ethernetEthCounters[index].counterId = index;
        ethernetEthCounters[index].groupId = groupId;
        ethernetEthCounters[index].counterType = COUNTER_TYPE_CUMULATIVE;
        ethernetEthCounters[index].basketType = TYPE_UNSIGNED_INTEGER;
        ethernetEthCounters[index].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
        ethernetEthCounters[index].numberOfBaskets = ETHERNET_ETH_COUNTER_BASKETS;
        ethernetEthCounters[index].basketPeriodInSeconds = ETHERNET_ETH_COUNTER_BASASKETS_PERIOD;
        ethernetEthCounters[index].baskets = (void *)(((TUSIGN8*)ethernetEthCountersValues) + (((ETHERNET_ETH_COUNTER_ENTRY_SIZE * index) + ETHERNET_ETH_COUNTER_ENTRY_BASKETS_OFFSET) * sizeof(TUSIGN32)));
        //lint -e{835} Zero to operator + is ok
        ethernetEthCounters[index].cumulativeValue = ((TUSIGN8*)ethernetEthCountersValues) + (((ETHERNET_ETH_COUNTER_ENTRY_SIZE * index) + ETHERNET_ETH_COUNTER_ENTRY_CUMULATIVEVALUE_OFFSET) * sizeof(TUSIGN32));

        if (CountersInitialise(ethernetEthCounters + index) == eFALSE) {
            initCorrect = eFALSE;
        }
    }

    return initCorrect;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Initializes counters 
 \author    Michal Kochel
 \date      2019-10-25
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
TBOOL EthernetCounterInit(void) {
    TBOOL allocationCorrect = eTRUE;

    //lint -e740 cast to (uint8_t**) is ok for this interface
    if (OSPL_MemAllocate(ETHERNET_ETH_COUNTER_COUNT * sizeof(T_COUNTER), (uint8_t**)&ethernetEthPort1Counters) != OSPL_SUCCESS) {
        allocationCorrect = eFALSE;
    }
    if (allocationCorrect == eTRUE) {
        if (OSPL_MemAllocate(ETHERNET_ETH_COUNTER_COUNT * sizeof(T_COUNTER), (uint8_t**)&ethernetEthPort2Counters) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
    }
    if (allocationCorrect == eTRUE) {
        if (OSPL_MemAllocate(ETHERNET_TCP_IP_COUNTER_COUNT * sizeof(T_COUNTER), (uint8_t**)&ethernetEthTcpIpCounters) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
    }
    if (allocationCorrect == eTRUE) {
        if (OSPL_MemAllocate(CI_ETH_STATS_SLOTS_COUNT * sizeof(T_ETHSTATS), (uint8_t**)&ciEthStats) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
    }
    if (allocationCorrect == eTRUE) {
        if (OSPL_MemAllocate(CI_ETH_STATS_SLOTS_COUNT * sizeof(CI_statistic_t), (uint8_t**)&ciTcpIpStats) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
    }
    if (allocationCorrect == eTRUE) {
        size_t sizeToAllocate = (ETHERNET_ETH_COUNTER_COUNT * (ETHERNET_ETH_COUNTER_BASKETS + 1) * sizeof(TUSIGN32));
        if (OSPL_MemAllocate(sizeToAllocate, (uint8_t**)&ethernetEthPort1CountersValues) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
        else {
            memset(ethernetEthPort1CountersValues, 0, sizeToAllocate);
        }

        if (OSPL_MemAllocate(sizeToAllocate, (uint8_t**)&ethernetEthPort2CountersValues) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
        else {
            memset(ethernetEthPort2CountersValues, 0, sizeToAllocate);
        }
    }
    if (allocationCorrect == eTRUE) {
        if (OSPL_MemAllocate((ETHERNET_TCP_IP_COUNTER_COUNT * (ETHERNET_ETH_COUNTER_BASKETS + 1) * sizeof(TUSIGN32)), (uint8_t**)&ethernetEthTcpIpCountersValues) != OSPL_SUCCESS) {
            allocationCorrect = eFALSE;
        }
        else {
            memset(ethernetEthTcpIpCountersValues, 0, (ETHERNET_TCP_IP_COUNTER_COUNT * (ETHERNET_ETH_COUNTER_BASKETS + 1) * sizeof(TUSIGN32)));
        }
    }

    if (allocationCorrect == eTRUE) {

        memset(ciEthStats, 0, sizeof(T_ETHSTATS) * CI_ETH_STATS_SLOTS_COUNT);
        memset(ciTcpIpStats, 0, sizeof(CI_statistic_t) * CI_ETH_STATS_SLOTS_COUNT);

        if (InitEthernetCounters(ethernetEthPort1Counters, ethernetEthPort1CountersValues, COUNTER_GROUP_ETHERNET_PORT1) == eFALSE) {
            allocationCorrect = eFALSE;
        }

        if (InitEthernetCounters(ethernetEthPort2Counters, ethernetEthPort2CountersValues, COUNTER_GROUP_ETHERNET_PORT2) == eFALSE) {
            allocationCorrect = eFALSE;
        }

        if (allocationCorrect == eTRUE) {
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].counterId = 0;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].groupId = COUNTER_GROUP_TCPIP;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].counterType = COUNTER_TYPE_LAST_CARRY;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].basketType = TYPE_UNSIGNED_INTEGER;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].numberOfBaskets = ETHERNET_ETH_COUNTER_BASKETS;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].basketPeriodInSeconds = ETHERNET_ETH_COUNTER_BASASKETS_PERIOD;
            ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS].baskets = ((TUSIGN8*)ethernetEthTcpIpCountersValues) + ((ETHERNET_ETH_COUNTER_ENTRY_BASKETS_OFFSET) * sizeof(TUSIGN32));

            if (CountersInitialise(ethernetEthTcpIpCounters + 0) == eFALSE) {
                allocationCorrect = eFALSE;
            }


            for (TUSIGN8 index = ETHERNET_TCP_IP_COUNTER_TCP_CONNECTIONS_ESTABLISHED ; index < ETHERNET_TCP_IP_COUNTER_COUNT && allocationCorrect == eTRUE; ++index) {
                ethernetEthTcpIpCounters[index].counterId = index;
                ethernetEthTcpIpCounters[index].groupId = COUNTER_GROUP_TCPIP;
                ethernetEthTcpIpCounters[index].counterType = COUNTER_TYPE_CUMULATIVE;
                ethernetEthTcpIpCounters[index].basketType = TYPE_UNSIGNED_INTEGER;
                ethernetEthTcpIpCounters[index].cumulativeValueType = TYPE_UNSIGNED_INTEGER;
                ethernetEthTcpIpCounters[index].numberOfBaskets = ETHERNET_ETH_COUNTER_BASKETS;
                ethernetEthTcpIpCounters[index].basketPeriodInSeconds = ETHERNET_ETH_COUNTER_BASASKETS_PERIOD;
                ethernetEthTcpIpCounters[index].baskets = ((TUSIGN8*)ethernetEthTcpIpCountersValues) + (((ETHERNET_ETH_COUNTER_ENTRY_SIZE * index) + ETHERNET_ETH_COUNTER_ENTRY_BASKETS_OFFSET) * sizeof(TUSIGN32));
                //lint -e{835} Zero to operator + is ok
                ethernetEthTcpIpCounters[index].cumulativeValue = ((TUSIGN8*)ethernetEthTcpIpCountersValues) + (((ETHERNET_ETH_COUNTER_ENTRY_SIZE * index) + ETHERNET_ETH_COUNTER_ENTRY_CUMULATIVEVALUE_OFFSET) * sizeof(TUSIGN32));

                if (CountersInitialise(ethernetEthTcpIpCounters + index) == eFALSE) {
                    allocationCorrect = eFALSE;
                }
            }
        }
    }

    if (allocationCorrect == eFALSE) {
        if (ethernetEthPort1Counters != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthPort1Counters);
        }
        if (ethernetEthPort2Counters != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthPort2Counters);
        }
        if (ethernetEthTcpIpCounters != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthTcpIpCounters);
        }
        if (ciEthStats != NULL) {
            (void)OSPL_MemFree((uint8_t*)ciEthStats);
        }
        if (ciTcpIpStats != NULL) {
            (void)OSPL_MemFree((uint8_t*)ciTcpIpStats);
        }
        if (ethernetEthPort1CountersValues != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthPort1CountersValues);
        }
        if (ethernetEthPort2CountersValues != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthPort2CountersValues);
        }
        if (ethernetEthTcpIpCountersValues != NULL) {
            (void)OSPL_MemFree((uint8_t*)ethernetEthTcpIpCountersValues);
        }
    }
    
    return allocationCorrect;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Peg ethenet counters 
 \author    Michal Kochel
 \date      2019-10-25
 \param     portIndex Index of port to use
 \param     ethernetEthCounters Ethernet counters for given port
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
void UpdateEthCounters(TUSIGN8 portIndex, T_COUNTER* ethernetEthCounters) {
    uint32_t value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_BYTES_RECEIVED);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_BYTES_RX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_UNICAST_PACKETS_RECEIVED);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_BROADCAST_PACKETS_RECEIVED);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_MULTICAST_PACKETS_RECEIVED);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_PACKETS_RX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_BYTES_TRANSMITTED);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_BYTES_TX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_UNICAST_PACKETS_TRANSMITTED);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_BROADCAST_PACKETS_TRANSMITTED);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_MULTICAST_PACKETS_TRANSMITTED);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_PACKETS_TX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_TRANSMITTED_AFTER_SINGLE_COLLISION);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_TRANSMITTED_AFTER_MULTIPLE_COLLISIONS);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_FRAMES_COLISIONS_TX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_DROPPED_AFTER_EXCESSIVE_COLLISIONS);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_FRAMES_DROPPED_TX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_RECEIVED_WITH_ALIGNMENT_ERROR);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_RECEIVED_WITH_CRC_FCS_ERROR);
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_RECEIVED_WITH_LARGE_FRAME_ERROR);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_FRAMES_ERRORS_RX]), &value);

    value = 0;
    value += GetEthStatDiff(portIndex, oldSlotOfCiEthStats, activeSlotOfCiEthStats, ETHSTATS_FRAMES_DELAYED_BY_TRAFFIC);
    CountersUpdate(&(ethernetEthCounters[ETHERNET_ETH_COUNTER_FRAMES_DELAYED_TX]), &value);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Peg counters 
 \author    Michal Kochel, Klaus Pose
 \date      2019-10-25   , 2019-11-22
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
void EthernetCounterUpdate(void) {
    int32_t szParam;
    int32_t i32status;
    uint32_t value;

    oldSlotOfCiEthStats = activeSlotOfCiEthStats;
    activeSlotOfCiEthStats = (activeSlotOfCiEthStats + 1) % CI_ETH_STATS_SLOTS_COUNT;

    szParam = sizeof(T_ETHSTATS);
    i32status = CI_GetSystemParam(CI_sysParamPortEthStats, ciEthStats[activeSlotOfCiEthStats], &szParam);
    if (i32status == (int32_t)sizeof(T_ETHSTATS)) {
        UpdateEthCounters(0, ethernetEthPort1Counters);
        UpdateEthCounters(1, ethernetEthPort2Counters);
    }

    szParam = sizeof(CI_statistic_t);
    i32status = CI_GetSystemParam(CI_sysParamIpStats, &ciTcpIpStats[activeSlotOfCiEthStats], &szParam);
    if (i32status == (int32_t)sizeof(CI_statistic_t)) {
        value = ciTcpIpStats[activeSlotOfCiEthStats].openSockets;
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpEstablished, ciTcpIpStats[activeSlotOfCiEthStats].tcpEstablished);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_CONNECTIONS_ESTABLISHED]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].udpInDatagrams, ciTcpIpStats[activeSlotOfCiEthStats].udpInDatagrams);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_UDP_RX_DATAGRAMS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].udpInErrors, ciTcpIpStats[activeSlotOfCiEthStats].udpInErrors);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_UDP_RX_ERRORS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].udpOutDatagrams, ciTcpIpStats[activeSlotOfCiEthStats].udpOutDatagrams);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_UDP_TX_DATAGRAMS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpInSegs, ciTcpIpStats[activeSlotOfCiEthStats].tcpInSegs);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_RX_SEGMENTS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpInErrs, ciTcpIpStats[activeSlotOfCiEthStats].tcpInErrs);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_RX_ERRORS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpOutSegs, ciTcpIpStats[activeSlotOfCiEthStats].tcpOutSegs);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_TX_SEGMENTS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpRetransSegs, ciTcpIpStats[activeSlotOfCiEthStats].tcpRetransSegs);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_TX_RETRANS_SEGMENTS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpOutRsts, ciTcpIpStats[activeSlotOfCiEthStats].tcpOutRsts);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_OUT_RESETS]), &value);

        value = GetDifference(ciTcpIpStats[oldSlotOfCiEthStats].tcpEstabResets, ciTcpIpStats[activeSlotOfCiEthStats].tcpEstabResets);
        CountersUpdate(&(ethernetEthTcpIpCounters[ETHERNET_TCP_IP_COUNTER_TCP_ESTABLISHED_RESETS]), &value);
    }
}
