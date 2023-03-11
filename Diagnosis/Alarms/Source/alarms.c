
//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Audit Log
 Module         
 Description    Internal alarms
 Remarks
*/
//--------------------------------------------------------------------------------------------------

#include "RTOS/Interface/rtos.h"
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "Services/Interface/string_helper.h"
#include "Services/Interface/math_helper.h"

// #include "../../../PhysicalBlock/Interface/PhysicalBlock_idx.h"
// #include "../../../PhysicalBlock/Interface/PhysicalBlock.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Source/Timestamp.h"
#include "Diagnosis/AuditLog/Interface/auditlog.h"

#include <stddef.h>
//lint -e{309} built-in header-file
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "Diagnosis/Alarms/Interface/alarms.h"
#include "Diagnosis/Alarms/Interface/alarmsidx.h"

#define ALARM_TEXT_MAX_LEN 25
#define ALARM_MAX_ACTIVE 10
#define ALARM_HEADER_LEN 60

typedef struct _T_ALARM {
    TBOOL freeSlot;
    TUSIGN8 id;
    TUSIGN8 object;
    TUSIGN8 severity;
    T_TIME_STAMP time;
    TCHAR text[ALARM_TEXT_MAX_LEN];
} T_ALARM;

static T_ALARM alarmsList[ALARM_MAX_ACTIVE];

static E_AUDITLOG_SEVERITY AlarmSeverityToLogSeverity(E_ALARM_SEVERITY alarmSeverity) {
    switch (alarmSeverity) {
        case ALARM_SEVERITY_MINOR: return E_AUDITLOG_SEVERITY_INFO;
        case ALARM_SEVERITY_MAJOR: return E_AUDITLOG_SEVERITY_ERROR;
        case ALARM_SEVERITY_CRITICAL: return E_AUDITLOG_SEVERITY_CRITICAL;
    }

    return E_AUDITLOG_SEVERITY_INFO;
}

static char* AlarmSeverityToString(E_ALARM_SEVERITY alarmSeverity) {
    switch (alarmSeverity) {
        case ALARM_SEVERITY_MINOR: return "Minor";
        case ALARM_SEVERITY_MAJOR: return "Major";
        case ALARM_SEVERITY_CRITICAL: return "Critical";
    }

    return "Unknown";
}

static char* AlarmIdToString(E_ALARM_ID alarmId) {
    switch (alarmId) {
        case ALARM_CERTIFICATE_EXPIRING: return "Certificate expiring";
        case ALARM_CERTIFICATE_EXPIRED: return "Certificate expired";
        case ALARM_MEMORY_ERROR: return "Memory error";
    }

    return "Unknown";
}

static char* AlarmObjectToString(E_ALARM_OBJECT alarmObject) {
    switch (alarmObject) {
        case ALARM_OBJECT_MODULE: return "Module";
        case ALARM_OBJECT_WEBSERVER_CERTIFICATE: return "HTTPS certificate";
        case ALARM_OBJECT_INTERNAL_FLASH: return "Internal flash";
        case ALARM_OBJECT_NV_MEMORY: return "NV memory";
    }

    return "Unknown";
}

void AlarmsInit(void) {
    for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
        alarmsList[i].freeSlot = eTRUE;
    }
}

//-------------------------------------------------------------------------
/*! 
\brief Start alarm on given object
\author Michal Kochel
\date 13.06.2019
\param id Alarm id
\param object Object
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
void AlarmsStart(TUSIGN8 id, TUSIGN8 object, E_ALARM_SEVERITY severity, TBOOL transient, const TCHAR* ptrText, ...) {
    T_ALARM* freeAlarm = NULL;

    for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
        if (alarmsList[i].freeSlot == eTRUE) {
            freeAlarm = alarmsList + i;
            break;
        }
    }

    if (freeAlarm != NULL) {
        // Build alarms text
        TCHAR logText[ALARM_TEXT_MAX_LEN + ALARM_HEADER_LEN];
        va_list args;

        //lint -e{10,40} Errors comming from std interface
        va_start(args, ptrText);
        (void)vsnprintf((char*)freeAlarm->text, ALARM_TEXT_MAX_LEN, (const char*)ptrText, args);
        va_end(args);

        (void)snprintf((char*)logText, ALARM_TEXT_MAX_LEN + ALARM_HEADER_LEN, "ALARM [start,%s,%s] %s", 
            AlarmIdToString((E_ALARM_ID)id), 
            AlarmObjectToString((E_ALARM_OBJECT)object), 
            (char*)freeAlarm->text
        );

        TBOOL registerAlarm = transient == eFALSE ? eTRUE : eFALSE;

        // Check if is not started already
        for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
            if (alarmsList[i].freeSlot == eFALSE && alarmsList[i].id == id && alarmsList[i].object == object) {
                // If text is the same then end
                if (strcmp((const char*)alarmsList[i].text, (const char*)freeAlarm->text) == 0) { 
                    return;
                }
                else {
                    strcpy((char*)alarmsList[i].text, (const char*)freeAlarm->text);
                    registerAlarm = eFALSE;
                }
            }
        }

        // Register alarm
        if (registerAlarm) {
            freeAlarm->freeSlot = eFALSE;
            freeAlarm->id = id;
            freeAlarm->object = object;
            freeAlarm->severity = severity;
            (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &(freeAlarm->time));
        }

        // Register in audit log
#ifdef COMPILE_WITH_AUDITLOG
        AuditLogLog(E_AUDITLOG_SOURCE_SYSTEM, AlarmSeverityToLogSeverity(severity), logText);
#endif
    }
}

//-------------------------------------------------------------------------
/*! 
\brief Cancel given alarm on given object
\author Michal Kochel
\date 13.06.2019
\param id Alarm id
\param object Object
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
void AlarmsCancel(TUSIGN8 id, TUSIGN8 object) {
    for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
        if (alarmsList[i].freeSlot == eFALSE && alarmsList[i].id == id && alarmsList[i].object == object) {
            TCHAR logText[ALARM_TEXT_MAX_LEN + ALARM_HEADER_LEN];

            (void)snprintf((char*)logText, ALARM_TEXT_MAX_LEN + ALARM_HEADER_LEN, "ALARM [cancel,%s,%s] %s", 
                AlarmIdToString((E_ALARM_ID)alarmsList[i].id), 
                AlarmObjectToString((E_ALARM_OBJECT)alarmsList[i].object), 
                alarmsList[i].text
            );
#ifdef COMPILE_WITH_AUDITLOG
            AuditLogLog(E_AUDITLOG_SOURCE_SYSTEM, AlarmSeverityToLogSeverity((E_ALARM_SEVERITY)alarmsList[i].severity), logText);
#endif
            alarmsList[i].freeSlot = eTRUE;
            break;
        }
    }
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with all active alarms
\author Michal Kochel
\date 13.06.2019
\param jsonStringOutput String which will be filled with JSON
\return Pointer to end of JSON string
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
char* AlarmsToJson(char* jsonStringOutput) {
    int activeAlarms = 0;
    TCHAR timeStringBuffer[TIMESTMAP_FORMAT_ISO8601_LENGTH + 1];

    for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
        if (alarmsList[i].freeSlot == eFALSE) {
            activeAlarms++;
        }
    }

    jsonStringOutput += sprintf(jsonStringOutput, "\"alarms\":[");
    for (int i = 0; i < ALARM_MAX_ACTIVE; ++i) {
        if (alarmsList[i].freeSlot == eFALSE) {
            (void)TimeStampInISO8601(&(alarmsList[i].time), timeStringBuffer);

            jsonStringOutput += sprintf(jsonStringOutput, "{\"id\":\"%s\",\"object\":\"%s\",\"severity\":\"%s\",\"time\":\"%s\",\"text\":\"%s\"}",
                AlarmIdToString((E_ALARM_ID)alarmsList[i].id),
                AlarmObjectToString((E_ALARM_OBJECT)alarmsList[i].object),
                AlarmSeverityToString((E_ALARM_SEVERITY)alarmsList[i].severity),
                timeStringBuffer,
                alarmsList[i].text
                );
            activeAlarms--;

            if (activeAlarms > 0) {
                jsonStringOutput += sprintf(jsonStringOutput, ",");
            }
        }
    }
    jsonStringOutput += sprintf(jsonStringOutput, "]");

    return jsonStringOutput;
}
