//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Counters
 Module         
 Description    Performance counters
 Remarks
*/
//--------------------------------------------------------------------------------------------------

#ifndef __COUNTERS_H__
#define __COUNTERS_H__

#include <stddef.h> 

typedef enum _E_COUNTERS_VALUE_TYPES {
    TYPE_INTEGER,
    TYPE_UNSIGNED_INTEGER,
    TYPE_FLOAT,
    TYPE_DOUBLE
} E_COUNTERS_VALUE_TYPES;

typedef enum _E_COUNTER_TYPES {
    COUNTER_TYPE_CUMULATIVE,
    COUNTER_TYPE_MIN,
    COUNTER_TYPE_MAX,
    COUNTER_TYPE_LAST,
    COUNTER_TYPE_LAST_CARRY,
    COUNTER_TYPE_SAMPLED
} E_COUNTER_TYPES;

typedef struct _T_COUNTER {
    TUSIGN8 counterType;
    TUSIGN8 basketType;
    TUSIGN8 cumulativeValueType;
    TUSIGN8 currentBasketIndex;
    TUSIGN8 counterId;
    TUSIGN8 groupId;
    TUSIGN8 numberOfBaskets;
    TUSIGN16 basketPeriodInSeconds;
    TUSIGN32 currentBasketTime;
    void* baskets;
    void* cumulativeValue;
    TUSIGN32* timeStamps;
    size_t basketSize;
    size_t cumulativeValueSize;
    struct _T_COUNTER* nextCounter;
} T_COUNTER;

TBOOL CountersInitialise(T_COUNTER* ptrCounter);

void CountersStart(T_COUNTER* ptrCounter);

void CountersUpdate(T_COUNTER* ptrCounter, const void* ptrValue);

void CountersCurrentValue(T_COUNTER* ptrCounter, void* pValue);

char* CountersHistoryToJson(T_COUNTER* ptrCounter, char* jsonStringOutput, TBOOL addGroupId);

void CountersHistoryGroupToJson(TUSIGN8 groupId, char* jsonStringOutput);

char* CountersCurrentToJson(T_COUNTER* ptrCounter, char* jsonStringOutput, TBOOL addGroupId);

void CountersCurrentGroupToJson(TUSIGN8 groupId, char* jsonStringOutput);

char* CountersCurrentAllToJson(char* jsonStringOutput);

char* CountersHistoryAllToJson(char* jsonStringOutput);

#endif