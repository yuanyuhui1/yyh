//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Audit Log
 Module         
 Description    Counters interface
 Remarks
*/
//--------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "Services/Interface/string_helper.h"
#include "Services/Interface/math_helper.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Source/Timestamp.h"

#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>

//#include "system_dbug.h"

#include "Diagnosis/Counters/Interface/counters.h"

T_COUNTER* ptrRootCounter = NULL;

//-------------------------------------------------------------------------
/*! 
 \brief Converts value type to size in bytes
 \author Michal Kochel
 \date 03.06.2019
 \param valueType Value type
 \return size_t Size in bytes from value type
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static size_t CountersSizeFromValueType(E_COUNTERS_VALUE_TYPES valueType) {
    switch (valueType) {
        case TYPE_INTEGER: return sizeof(TINT32);
        case TYPE_UNSIGNED_INTEGER: return sizeof(TUSIGN32);
        case TYPE_FLOAT: return sizeof(TFLOAT);
        case TYPE_DOUBLE: return sizeof(TDOUBLE);
    }
    return sizeof(TINT32);
}

//-------------------------------------------------------------------------
/*! 
 \brief Checks if counters types are compatible
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be checked
 \return TBOOL Are counter types compatible
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static TBOOL CounterCompatibleTypes(const T_COUNTER* ptrCounter) {
    switch (ptrCounter->cumulativeValueType) {
        case TYPE_INTEGER: return (ptrCounter->basketType != TYPE_INTEGER) ? eFALSE : eTRUE;
        case TYPE_UNSIGNED_INTEGER: return (ptrCounter->basketType != TYPE_UNSIGNED_INTEGER) ? eFALSE : eTRUE;
        case TYPE_FLOAT: return (ptrCounter->basketType != TYPE_FLOAT) ? eFALSE : eTRUE;
        case TYPE_DOUBLE:
            return (ptrCounter->basketType != TYPE_FLOAT && ptrCounter->basketType != TYPE_DOUBLE) ? eFALSE : eTRUE;
        default: return eFALSE;
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts counter type as a string
 \author Michal Kochel
 \date 03.06.2019
 \param counterType Counter type
 \return const char* String of counter type
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static const char* CounterTypeToString(E_COUNTER_TYPES counterType) {
    switch (counterType) {
        case COUNTER_TYPE_CUMULATIVE: return "c";
        case COUNTER_TYPE_MIN: return "min";
        case COUNTER_TYPE_MAX: return "max";
        case COUNTER_TYPE_LAST: return "last";
        case COUNTER_TYPE_LAST_CARRY: return "lc";
        case COUNTER_TYPE_SAMPLED: return "samp";
        default: return "";
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Updates counter as Last type counter
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be updated
 \param ptrBasektValue Basket which will be updated
 \param ptrValue Value by which basket will be updated
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CounterLast(const T_COUNTER* ptrCounter, void* ptrBasektValue, const void* ptrValue) {
    switch (ptrCounter->basketType) {
        case TYPE_INTEGER: (*(TINT32*)ptrBasektValue) = (*(TINT32*)ptrValue); break;
        case TYPE_UNSIGNED_INTEGER: (*(TUSIGN32*)ptrBasektValue) = (*(TUSIGN32*)ptrValue); break;
        case TYPE_FLOAT:  (*(TFLOAT*)ptrBasektValue) = (*(TFLOAT*)ptrValue); break;
        case TYPE_DOUBLE: (*(TDOUBLE*)ptrBasektValue) = (*(TDOUBLE*)ptrValue); break;
        default: break;
    }    
}

//-------------------------------------------------------------------------
/*! 
 \brief Updates counter as MinMax type counter
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be updated
 \param ptrBasektValue Basket which will be updated
 \param ptrValue Value by which basket will be updated
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CounterMinMax(const T_COUNTER* ptrCounter, void* ptrBasektValue, const void* ptrValue) {
    switch (ptrCounter->basketType) {
        case TYPE_INTEGER: 
            if (ptrCounter->counterType == COUNTER_TYPE_MIN) {
                (*(TINT32*)ptrBasektValue) = min_tint32((*(TINT32*)ptrBasektValue), (*(TINT32*)ptrValue));
            } 
            else if (ptrCounter->counterType == COUNTER_TYPE_MAX) {
                (*(TINT32*)ptrBasektValue) = max_tint32((*(TINT32*)ptrBasektValue), (*(TINT32*)ptrValue));
            }
            break;
        case TYPE_UNSIGNED_INTEGER:
            if (ptrCounter->counterType == COUNTER_TYPE_MIN) {
                (*(TUSIGN32*)ptrBasektValue) = min_tusign32((*(TUSIGN32*)ptrBasektValue), (*(TUSIGN32*)ptrValue));
            } 
            else if (ptrCounter->counterType == COUNTER_TYPE_MAX) {
                (*(TUSIGN32*)ptrBasektValue) = max_tusign32((*(TUSIGN32*)ptrBasektValue), (*(TUSIGN32*)ptrValue));
            }
            break;
        case TYPE_FLOAT:            
            if (ptrCounter->counterType == COUNTER_TYPE_MIN) {
                (*(TFLOAT*)ptrBasektValue) = min_tfloat((*(TFLOAT*)ptrBasektValue), (*(TFLOAT*)ptrValue));
            } 
            else if (ptrCounter->counterType == COUNTER_TYPE_MAX) {
                (*(TFLOAT*)ptrBasektValue) = max_tfloat((*(TFLOAT*)ptrBasektValue), (*(TFLOAT*)ptrValue));
            }
            break;
        case TYPE_DOUBLE:
            if (ptrCounter->counterType == COUNTER_TYPE_MIN) {
                (*(TDOUBLE*)ptrBasektValue) = min_tdouble((*(TDOUBLE*)ptrBasektValue), (*(TDOUBLE*)ptrValue));
            } 
            else if (ptrCounter->counterType == COUNTER_TYPE_MAX) {
                (*(TDOUBLE*)ptrBasektValue) = max_tdouble((*(TDOUBLE*)ptrBasektValue), (*(TDOUBLE*)ptrValue));
            }
            break;
        default: break;
    }  
}

//-------------------------------------------------------------------------
/*! 
 \brief Updates counter as Cumulative type counter
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be updated
 \param ptrBasektValue Basket which will be updated
 \param ptrValue Value by which basket will be updated
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CounterCumulate(const T_COUNTER* ptrCounter, void* basektValue, const void* value) {
    switch (ptrCounter->cumulativeValueType) {
        case TYPE_INTEGER: 
            (*(TINT32*)ptrCounter->cumulativeValue) += (*(TINT32*)value);
            (*(TINT32*)basektValue) += (*(TINT32*)value);
            break;

        case TYPE_UNSIGNED_INTEGER: 
            (*(TUSIGN32*)ptrCounter->cumulativeValue) += (*(TUSIGN32*)value);
            (*(TUSIGN32*)basektValue) += (*(TUSIGN32*)value);
            break;

        case TYPE_FLOAT: 
            (*(TFLOAT*)ptrCounter->cumulativeValue) += (*(TFLOAT*)value);
            (*(TFLOAT*)basektValue) += (*(TFLOAT*)value);
            break;

        case TYPE_DOUBLE:
            if (ptrCounter->basketType == TYPE_FLOAT) {
                (*(TDOUBLE*)ptrCounter->cumulativeValue) += (*(TFLOAT*)value);
                (*(TFLOAT*)basektValue) += (*(TFLOAT*)value);
            }
            else if (ptrCounter->basketType == TYPE_DOUBLE) {
                (*(TDOUBLE*)ptrCounter->cumulativeValue) += (*(TDOUBLE*)value);
                (*(TDOUBLE*)basektValue) += (*(TDOUBLE*)value);
            }
            break;
        default: break;
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Sets basket with default value
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be updated
 \param ptrBasektValue Basket which will be updated
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CountersSetDefulatBasketValue(const T_COUNTER* ptrCounter, void* ptrBasektValue) {
    if (ptrCounter->counterType == COUNTER_TYPE_MIN) {
        switch (ptrCounter->basketType) {
            case TYPE_INTEGER: (*(TINT32*)ptrBasektValue) = INT_MAX; break;
            case TYPE_UNSIGNED_INTEGER: (*(TUSIGN32*)ptrBasektValue) = UINT_MAX; break;
            case TYPE_FLOAT:  (*(TFLOAT*)ptrBasektValue) = FLT_MAX; break;
            case TYPE_DOUBLE: (*(TDOUBLE*)ptrBasektValue) = FLT_MAX; break;
            default: break;
        }  
    }
    else if (ptrCounter->counterType == COUNTER_TYPE_MAX) {
        switch (ptrCounter->basketType) {
            case TYPE_INTEGER: (*(TINT32*)ptrBasektValue) = INT_MIN; break;
            case TYPE_UNSIGNED_INTEGER: (*(TUSIGN32*)ptrBasektValue) = 0; break;
            case TYPE_FLOAT:  (*(TFLOAT*)ptrBasektValue) = FLT_MIN; break;
            case TYPE_DOUBLE: (*(TDOUBLE*)ptrBasektValue) = FLT_MIN; break;
            default: break;
        } 
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Initializes counter structure based on initial settings
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be initialized
 \return TBOOL Initializastion status
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
TBOOL CountersInitialise(T_COUNTER* ptrCounter)
{
    ptrCounter->basketSize = CountersSizeFromValueType((E_COUNTERS_VALUE_TYPES)ptrCounter->basketType);
    (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &(ptrCounter->currentBasketTime));
    ptrCounter->currentBasketIndex = 0;

    memset(ptrCounter->baskets, 0, ptrCounter->basketSize * ptrCounter->numberOfBaskets);

    if (ptrCounter->counterType == COUNTER_TYPE_CUMULATIVE) {
        if (ptrCounter->cumulativeValue == NULL) {
            return eFALSE;
        }

        if (CounterCompatibleTypes(ptrCounter) != eTRUE) {
            return eFALSE;
        }

        ptrCounter->cumulativeValueSize = CountersSizeFromValueType((E_COUNTERS_VALUE_TYPES)ptrCounter->cumulativeValueType);
        memset(ptrCounter->cumulativeValue, 0, ptrCounter->cumulativeValueSize);
    }
    else {
        ptrCounter->cumulativeValueSize = 0;
        ptrCounter->cumulativeValue = NULL;

        CountersSetDefulatBasketValue(ptrCounter, ptrCounter->baskets);
    }

    if (ptrCounter->counterType == COUNTER_TYPE_SAMPLED) {
        ptrCounter->basketPeriodInSeconds = 0;
        if (ptrCounter->timeStamps != NULL) {
            memset(ptrCounter->timeStamps, 0, sizeof(TUSIGN32) * ptrCounter->numberOfBaskets);
        }
    }
    else {
        ptrCounter->timeStamps = NULL;
    }

    ptrCounter->nextCounter = NULL;

    // Check if counter is not on list already
    T_COUNTER* testCounter = ptrRootCounter;
    TBOOL counterInList = eFALSE;
    while (testCounter != NULL) {
        if (ptrCounter == testCounter) {
            counterInList = eTRUE;
            break;
        }
        else {
            testCounter = testCounter->nextCounter;
        }
    }

    // Push counter to tracked list
    if (counterInList == eFALSE) {
        T_COUNTER** targetCounter = &ptrRootCounter;

        while (*targetCounter != NULL) {
            targetCounter = &((*targetCounter)->nextCounter);
        }

        *targetCounter = ptrCounter;
    }

    return eTRUE;
}

//-------------------------------------------------------------------------
/*! 
 \brief Sets curent time as curent counter time
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be initialized
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
void CountersStart(T_COUNTER* ptrCounter)
{
    (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &(ptrCounter->currentBasketTime));
}
//-------------------------------------------------------------------------
/*! 
 \brief Updates baskets when counter was not updated in time diff
 \author Michal Kochel
 \date 22.07.2019
 \param ptrCounter Counter which will be updated
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CounterAdvanceBuckets(T_COUNTER* ptrCounter) {
    if (ptrCounter->counterType != COUNTER_TYPE_SAMPLED) {
        TUSIGN32 currentTime;
        TUSIGN32 basketDiff;
        
        (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &currentTime);

        basketDiff = (currentTime - ptrCounter->currentBasketTime) / ptrCounter->basketPeriodInSeconds;

        if (basketDiff > ptrCounter->numberOfBaskets) {
            basketDiff = ptrCounter->numberOfBaskets;
        }

        ptrCounter->currentBasketTime += basketDiff * ptrCounter->basketPeriodInSeconds;

        // Zero out all baskets since last update
        unsigned char* basketsBuffer = ptrCounter->baskets;

        for (size_t i = 0; i < basketDiff; ++i) {
            ptrCounter->currentBasketIndex = (ptrCounter->currentBasketIndex + 1) % ptrCounter->numberOfBaskets;

            if (ptrCounter->counterType == COUNTER_TYPE_LAST_CARRY) {
                TUSIGN8 previousBasket = (ptrCounter->currentBasketIndex + (ptrCounter->numberOfBaskets - 1)) % ptrCounter->numberOfBaskets;
                memcpy(
                    basketsBuffer + (ptrCounter->currentBasketIndex * ptrCounter->basketSize),
                    basketsBuffer + (previousBasket * ptrCounter->basketSize), 
                    ptrCounter->basketSize);
            }
            else {
                memset(basketsBuffer + (ptrCounter->currentBasketIndex * ptrCounter->basketSize), 0, ptrCounter->basketSize);
            }
        }

        basketsBuffer += ptrCounter->currentBasketIndex * ptrCounter->basketSize;

        // If we are in new basket
        if (basketDiff > 0) {
            CountersSetDefulatBasketValue(ptrCounter, basketsBuffer);
        }

        // Set timestamp for samples
        if (ptrCounter->counterType == COUNTER_TYPE_SAMPLED && ptrCounter->timeStamps != NULL) {
            ptrCounter->timeStamps[ptrCounter->currentBasketIndex] = currentTime;
        }
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Updates counter by given value
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be updated
 \param ptrValue Value by which to update counter
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
void CountersUpdate(T_COUNTER* ptrCounter, const void* ptrValue) {
    unsigned char* basketsBuffer;

    if (ptrCounter->counterType == COUNTER_TYPE_SAMPLED)
    {
        (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &(ptrCounter->currentBasketTime));
        ptrCounter->currentBasketIndex = (ptrCounter->currentBasketIndex + 1) % ptrCounter->numberOfBaskets;
    }
    else {
        CounterAdvanceBuckets(ptrCounter);
    }

    basketsBuffer = (unsigned char*)ptrCounter->baskets + ptrCounter->currentBasketIndex * ptrCounter->basketSize;

    // Update counter
    switch (ptrCounter->counterType) {
        case COUNTER_TYPE_CUMULATIVE: 
            CounterCumulate(ptrCounter, basketsBuffer, ptrValue); break;
        case COUNTER_TYPE_SAMPLED:
        case COUNTER_TYPE_LAST:
        case COUNTER_TYPE_LAST_CARRY:
            CounterLast(ptrCounter, basketsBuffer, ptrValue); break;
        case COUNTER_TYPE_MIN:
        case COUNTER_TYPE_MAX:
             CounterMinMax(ptrCounter, basketsBuffer, ptrValue); break;
        default: break;
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Prints given value to string
 \author Michal Kochel
 \date 03.06.2019
 \param type Type which should be printed
 \param ptrValue Value to print
 \param string String which will be updated
 \return int Number of characters printed to string
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static int CountersPrintValue(E_COUNTERS_VALUE_TYPES type, const void* ptrValue, char* string) {
    switch (type) {
        case TYPE_INTEGER: return sprintf(string, "%ld", (*(TINT32*)ptrValue));
        case TYPE_UNSIGNED_INTEGER: return sprintf(string, "%lu", (*(TUSIGN32*)ptrValue));
        case TYPE_FLOAT: return sprintf(string, "%f", (*(TFLOAT*)ptrValue));
        case TYPE_DOUBLE: return sprintf(string, "%f", (*(TDOUBLE*)ptrValue));
    }
    return 0;
}

//-------------------------------------------------------------------------
/*! 
 \brief Copies given value to buffer
 \author Michal Kochel
 \date 03.06.2019
 \param type Type which should be printed
 \param ptrValue Value to print
 \param string String which will be updated
 \return int Number of characters printed to string
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
static void CountersCopyValue(E_COUNTERS_VALUE_TYPES type, void* destBuffer, const void* srcBuffer) {
    switch (type) {
        case TYPE_INTEGER: memcpy(destBuffer, srcBuffer, sizeof (TINT32)); break;
        case TYPE_UNSIGNED_INTEGER:  memcpy(destBuffer, srcBuffer, sizeof (TUSIGN32)); break;
        case TYPE_FLOAT: memcpy(destBuffer, srcBuffer, sizeof (TFLOAT)); break;
        case TYPE_DOUBLE: memcpy(destBuffer, srcBuffer, sizeof (TDOUBLE)); break;
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Returns current value of counter
 \author Michal Kochel
 \date 03.06.2019
 \param type Type which should be printed
 \param ptrValue Value to print
 \param string String which will be updated
 \return int Number of characters printed to string
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
void CountersCurrentValue(T_COUNTER* ptrCounter, void* pValue) {
    TUSIGN8 basketIndex;

    CounterAdvanceBuckets(ptrCounter);

    if (ptrCounter->counterType == COUNTER_TYPE_CUMULATIVE) {
        CountersCopyValue((E_COUNTERS_VALUE_TYPES)ptrCounter->cumulativeValueType, pValue, ptrCounter->cumulativeValue);
    }
    else {
        basketIndex = (ptrCounter->currentBasketIndex) % ptrCounter->numberOfBaskets;

        CountersCopyValue((E_COUNTERS_VALUE_TYPES)ptrCounter->basketType, pValue, (const TUSIGN8*)(ptrCounter->baskets) + (basketIndex * ptrCounter->basketSize));
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Creates JSON with history counter
 \author Michal Kochel
 \date 03.06.2019
 \param ptrCounter Counter which will be converted to JSON
 \param jsonStringOutput String which will be filled with JSON
 \param addGroupId Add gorup id in JSON
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
char* CountersHistoryToJson(T_COUNTER* ptrCounter, char* jsonStringOutput, TBOOL addGroupId) {
    TUSIGN8 basketIndex;
    T_TIME_STAMP timestamp;

    CounterAdvanceBuckets(ptrCounter);

    WorkingSecondsToTimeStamp(ptrCounter->currentBasketTime, &timestamp);

    jsonStringOutput += sprintf(jsonStringOutput, "{\"t\":\"%s\",\"i\":%d,\"c\":%lu,\"p\":%d,", 
        CounterTypeToString((E_COUNTER_TYPES)ptrCounter->counterType), ptrCounter->counterId, timestamp.timestamp.seconds, ptrCounter->basketPeriodInSeconds);

    if (ptrCounter->counterType == COUNTER_TYPE_SAMPLED) {
        if (ptrCounter->timeStamps != NULL) {
            jsonStringOutput += sprintf(jsonStringOutput, "\"times\":[");
            for (TUSIGN8 i = 0; i < ptrCounter->numberOfBaskets; ++i) {
                basketIndex = (ptrCounter->currentBasketIndex + 1 + i) % ptrCounter->numberOfBaskets;
                
                WorkingSecondsToTimeStamp(ptrCounter->timeStamps[basketIndex], &timestamp);

                jsonStringOutput += sprintf(jsonStringOutput, "%lu", timestamp.timestamp.seconds);

                if (i < ptrCounter->numberOfBaskets - 1) {
                    jsonStringOutput += sprintf(jsonStringOutput, ",");
                }
            }
            jsonStringOutput += sprintf(jsonStringOutput, "],");
        }
    }

    if (addGroupId == eTRUE) {
        jsonStringOutput += sprintf(jsonStringOutput, "\"g\":%d,", ptrCounter->groupId);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "\"v\":[");

    unsigned char* basketsBuffer = ptrCounter->baskets;

    for (TUSIGN8 i = 0; i < ptrCounter->numberOfBaskets; ++i) {
        basketIndex = (ptrCounter->currentBasketIndex + 1 + i) % ptrCounter->numberOfBaskets;

        jsonStringOutput += CountersPrintValue((E_COUNTERS_VALUE_TYPES)ptrCounter->basketType, basketsBuffer + (basketIndex * ptrCounter->basketSize), jsonStringOutput);

        if (i < ptrCounter->numberOfBaskets - 1) {
            jsonStringOutput += sprintf(jsonStringOutput, ",");
        }
    }

    jsonStringOutput += sprintf(jsonStringOutput, "]}");

    return jsonStringOutput;
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with history counters group
\author Michal Kochel
\date 03.06.2019
\param groupId Group Id
\param jsonStringOutput String which will be filled with JSON
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
void CountersHistoryGroupToJson(TUSIGN8 groupId, char* jsonStringOutput) {
    T_COUNTER** ptrCounter;
    TUSIGN16 numberInGroup = 0;

    // Count number of counter in group
    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {
        if ((*ptrCounter)->groupId == groupId) {
            numberInGroup++;
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    // Go thorugh counters and print counters from group
    jsonStringOutput += sprintf(jsonStringOutput, "{\"i\":%d,\"counters\":[", groupId);

    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {
        if ((*ptrCounter)->groupId == groupId) {
            jsonStringOutput = CountersHistoryToJson((*ptrCounter), jsonStringOutput, eFALSE);
            numberInGroup--;
            if (numberInGroup > 0) {
                jsonStringOutput += sprintf(jsonStringOutput, ",");
            }
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "]}");
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with current counter
\author Michal Kochel
\date 03.06.2019
\param ptrCounter Counter which will be converted to JSON
\param jsonStringOutput String which will be filled with JSON
\param addGroupId Add group id in JSON
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
char* CountersCurrentToJson(T_COUNTER* ptrCounter, char* jsonStringOutput, TBOOL addGroupId) {
    TUSIGN8 basketIndex;

    CounterAdvanceBuckets(ptrCounter);

    jsonStringOutput += sprintf(jsonStringOutput, "{\"t\":\"%s\",\"i\":%d,\"v\":", CounterTypeToString((E_COUNTER_TYPES)ptrCounter->counterType), ptrCounter->counterId);

    unsigned char* basketsBuffer = ptrCounter->baskets;

    if (ptrCounter->counterType == COUNTER_TYPE_CUMULATIVE) {
        jsonStringOutput += CountersPrintValue((E_COUNTERS_VALUE_TYPES)ptrCounter->cumulativeValueType, ptrCounter->cumulativeValue, jsonStringOutput);
    }
    else {
        basketIndex = (ptrCounter->currentBasketIndex) % ptrCounter->numberOfBaskets;

        jsonStringOutput += CountersPrintValue((E_COUNTERS_VALUE_TYPES)ptrCounter->basketType, basketsBuffer + (basketIndex * ptrCounter->basketSize), jsonStringOutput);
    }

    if (addGroupId == eTRUE) {
        jsonStringOutput += sprintf(jsonStringOutput, ",\"g\":%d", ptrCounter->groupId);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "}");

    return jsonStringOutput;
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with current counters group
\author Michal Kochel
\date 03.06.2019
\param groupId Group Id
\param jsonStringOutput String which will be filled with JSON
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
void CountersCurrentGroupToJson(TUSIGN8 groupId, char* jsonStringOutput) {
    T_COUNTER** ptrCounter;
    TUSIGN16 numberInGroup = 0;

    // Count number of counter in group
    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {
        if ((*ptrCounter)->groupId == groupId) {
            numberInGroup++;
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    // Go thorugh counters and print counters from group
    jsonStringOutput += sprintf(jsonStringOutput, "{\"i\":%d,\"counters\":[", groupId);

    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {
        if ((*ptrCounter)->groupId == groupId) {
            jsonStringOutput = CountersCurrentToJson((*ptrCounter), jsonStringOutput, eFALSE);
           
            numberInGroup--;
            if (numberInGroup > 0) {
                jsonStringOutput += sprintf(jsonStringOutput, ",");
            }
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "]}");
}


//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with all current counters
\author Michal Kochel
\date 03.06.2019
\param jsonStringOutput String which will be filled with JSON
\return char* pointer to end of JSON string
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
char* CountersCurrentAllToJson(char* jsonStringOutput) {
    T_COUNTER** ptrCounter;

    jsonStringOutput += sprintf(jsonStringOutput, "\"countersCurrent\":[");

    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {

        jsonStringOutput = CountersCurrentToJson((*ptrCounter), jsonStringOutput, eTRUE);
           
        if ((*ptrCounter)->nextCounter != NULL) {
            jsonStringOutput += sprintf(jsonStringOutput, ",");
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "]");

    return jsonStringOutput;
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with all history counters
\author Michal Kochel
\date 03.06.2019
\param jsonStringOutput String which will be filled with JSON
\return char* pointer to end of json string
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
char* CountersHistoryAllToJson(char* jsonStringOutput) {
    T_COUNTER** ptrCounter;

    jsonStringOutput += sprintf(jsonStringOutput, "\"countersHistory\":[");

    ptrCounter = &ptrRootCounter;
    while (*ptrCounter != NULL) {

        jsonStringOutput = CountersHistoryToJson((*ptrCounter), jsonStringOutput, eTRUE);
           
        if ((*ptrCounter)->nextCounter != NULL) {
            jsonStringOutput += sprintf(jsonStringOutput, ",");
        }

        ptrCounter = &((*ptrCounter)->nextCounter);
    }

    jsonStringOutput += sprintf(jsonStringOutput, "]");

    return jsonStringOutput;
}
