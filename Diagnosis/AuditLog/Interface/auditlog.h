//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Audit Log
 Module         system_dbug.h
 Description    Audit logs interface
 Remarks
*/
//--------------------------------------------------------------------------------------------------

#ifndef _AUDIT_LOG_H
#define _AUDIT_LOG_H

#define AUDITLOG_TEXT_MAX_LEN 120
#define AUDITLOG_TEXT_HEADER_SIZE 60
#define AUDTILOG_TEXT_NEW_LINE 2
#define AUDITLOG_ENTRY_SIZE (AUDITLOG_TEXT_MAX_LEN + AUDITLOG_TEXT_HEADER_SIZE)
#define AUDIT_LOG_MAX_FILE_COUNT 5
#define AUDIT_LOG_FILE_PATH_MAX 50
#define AUDIT_LOG_FILE_EXT_LEN 5

typedef enum _E_AUDITLOG_SEVERITY {
    E_AUDITLOG_SEVERITY_INFO = 0,
    E_AUDITLOG_SEVERITY_WARNING,
    E_AUDITLOG_SEVERITY_ERROR,
    E_AUDITLOG_SEVERITY_CRITICAL
} E_AUDITLOG_SEVERITY;

typedef enum _E_AUDITLOG_SOURCE {
    E_AUDITLOG_SOURCE_ETHERNETIP = 0,
    E_AUDITLOG_SOURCE_PROFINETIO,
    E_AUDITLOG_SOURCE_OPCUA,
    E_AUDITLOG_SOURCE_MODBUSTCP,
    E_AUDITLOG_SOURCE_ETHERNET,
    E_AUDITLOG_SOURCE_SYSTEM,
    E_AUDITLOG_SOURCE_WEBSERVER
} E_AUDITLOG_SOURCE;

typedef struct _T_AUDITLOG_SETTINGS {
    TUSIGN16 startupCounter;
    TUSIGN32 maxBytesPerFile;
    TUSIGN8 fileCount;
    const TCHAR* filePathBase;
} T_AUDITLOG_SETTINGS;

TBOOL AuditLogInitialise(const T_AUDITLOG_SETTINGS* settings);

void AuditLogLog(E_AUDITLOG_SOURCE source, E_AUDITLOG_SEVERITY severity, const TCHAR* ptrText, ...);

TCHAR* AuditLogInfoToJson(TCHAR* ptrOutputBuffer);

void AuditLogClear(void);

#endif