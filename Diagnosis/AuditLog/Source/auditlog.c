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

#include "RTOS/Interface/rtos.h"
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Diagnosis/AuditLog/Interface/auditlog.h"
#include "Services/Interface/string_helper.h"
// #include "../../../PhysicalBlock/Interface/PhysicalBlock_idx.h"
// #include "../../../PhysicalBlock/Interface/PhysicalBlock.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Source/Timestamp.h"
#include "Services/Interface/math_helper.h"
#include "FS_interface.h"

// #include "system_dbug.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h> 

static T_AUDITLOG_SETTINGS logSettings;

static RTOS_MUTEX_HANDLE auditLogAccessMutex;

static struct _AUDIT_LOG_INTERNALS {
    TUSIGN32 currentFileSize;
    TCHAR lastLoggedText[AUDITLOG_TEXT_MAX_LEN];
} auditLogInternals = /*lint -e{840} */ {
    0,
    "\0"
};

static const TCHAR* const webServerPath = (const TCHAR* const)"/public/web-server/";

TCHAR buffer[AUDITLOG_ENTRY_SIZE];

//-------------------------------------------------------------------------
/*! 
\brief Builds path for file with given index
\author  
\date  
\param filePath Output file path
\param fileId Id if file
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
void AuditLogBuildFilePath(TCHAR* filePath, TUSIGN8 fileId) {
    const size_t fileBaseLength = strlen((const char*)logSettings.filePathBase);
    strcpy((char*)filePath, (const char*)logSettings.filePathBase);
    filePath += fileBaseLength;

    (void)sprintf((char*)filePath, "%d.csv", fileId);
}

//-------------------------------------------------------------------------
/*! 
\brief Create file 
\author  
\date  
\param mainFs File system in which to create file
\param filePath Path to file 
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
static TBOOL CreateFile(FS_fileSystem_t* mainFs, const TCHAR* filePath) {
    FS_file_t* logFile;
    if (FS_FileOpen(mainFs, (const char*)filePath, FS_modeWrite, &logFile) == FS_OK) {
        uint8_t endLine = (uint8_t)'\n';
        (void)FS_FileWrite(mainFs, logFile, &endLine, 1);
        (void)FS_FileClose(mainFs, &logFile);
        
        return eTRUE;
    }
    return eFALSE;
}

//-------------------------------------------------------------------------
/*! 
\brief Initializes internals based on settings
\author  
\date  
\param ptrSettings Audit log service settings
\return Status of initialization 
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
TBOOL AuditLogInitialise(const T_AUDITLOG_SETTINGS* ptrSettings) {
    FS_fileSystem_t* mainFs;
    FS_file_t* logFile;
    FS_dir_t* dir;
    int32_t fsResult;
    TCHAR filePath[AUDIT_LOG_FILE_PATH_MAX];

    (void)memcpy(&logSettings, ptrSettings, sizeof(T_AUDITLOG_SETTINGS));

    Create_RTOS_MUTEX(auditLogAccessMutex);

    // Check if seetings are correct
    if (logSettings.fileCount < 1 || logSettings.fileCount > AUDIT_LOG_MAX_FILE_COUNT) {
        // DBUG_LOG_FATAL("[DIAG] File count error [is: %u, should: %d]", logSettings.fileCount, AUDIT_LOG_MAX_FILE_COUNT);
        return eFALSE;
    }

    if (strlen((const char*)logSettings.filePathBase) + AUDIT_LOG_FILE_EXT_LEN > AUDIT_LOG_FILE_PATH_MAX) {
        // DBUG_LOG_FATAL("[DIAG] File base to long");
        return eFALSE;
    }

    // Initialize file system
    fsResult = FS_GetInstance(FS_instanceMain, &mainFs);
    if (fsResult != FS_OK) {
        return eFALSE;
    }

    (void)Take_RTOS_MUTEX(auditLogAccessMutex, RTOS_WAIT_FOREVER);

    // Initialize directory
    TCHAR directoryFromBase[70];

    const TCHAR* lastSlashPos = (const TCHAR*)strrchr((const char*)logSettings.filePathBase, '/');

    if (lastSlashPos == NULL) {
        return eFALSE;
    }

    (void)strcpy_safe(directoryFromBase, logSettings.filePathBase, (size_t)(lastSlashPos - logSettings.filePathBase) + 1);

    int32_t auditLogDirOpen = FS_DirOpen(mainFs, (const char*)directoryFromBase, &dir);
    if (auditLogDirOpen == FS_NOT_EXIST) {
        if (FS_DirCreate(mainFs, (const char*)directoryFromBase) != FS_OK) {
            // DBUG_LOG_ERROR("[DIAG] Failed dir [path:%s]", directoryFromBase);
            return eFALSE;
        }
    }
    else if (auditLogDirOpen == FS_OK) {
        (void)FS_DirClose(mainFs, &dir);
    }

    // Initilize files
    for (TUSIGN8 fileIndex = 0; fileIndex < logSettings.fileCount; ++fileIndex) {
        AuditLogBuildFilePath(filePath, fileIndex);

        if (FS_FileOpen(mainFs, (const char*)filePath, FS_modeRead, &logFile) != FS_OK) {
          if (CreateFile(mainFs, filePath) == eFALSE) {
            // DBUG_LOG_ERROR("[DIAG] Failed file [path:%s]", filePath);
            return eFALSE;
          }
        }
        else {
            if (fileIndex == 0) {
                (void)FS_FileSeek(mainFs, logFile, 0, FS_seekEnd);
                int32_t fileTellPos = FS_FileTell(mainFs, logFile);
                if (fileTellPos < 0) {
                    fileTellPos = 0;
                }
                auditLogInternals.currentFileSize = (TUSIGN32)fileTellPos;
            }
            (void)FS_FileClose(mainFs, &logFile);
        }
    }

    (void)Give_RTOS_MUTEX(auditLogAccessMutex);

    return eTRUE;
}

//-------------------------------------------------------------------------
/*! 
\brief Returns severity string
\author  
\date  
\param severity Logging severity
\return String containing severity
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
static const TCHAR* AuditLogSeverityToText(E_AUDITLOG_SEVERITY severity) {
    switch (severity) {
        case E_AUDITLOG_SEVERITY_INFO: return (const TCHAR*)"INF";
        case E_AUDITLOG_SEVERITY_WARNING: return (const TCHAR*)"WRN";
        case E_AUDITLOG_SEVERITY_ERROR: return (const TCHAR*)"ERR";
        case E_AUDITLOG_SEVERITY_CRITICAL: return (const TCHAR*)"CRI";
        default: return (const TCHAR*)"UNK";
    }
}

//-------------------------------------------------------------------------
/*! 
\brief Returns source string
\author  
\date  
\param source Logging source
\return String containing source
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
static const TCHAR* AuditLogSourceToText(E_AUDITLOG_SOURCE severity) {
    switch (severity) {
        case E_AUDITLOG_SOURCE_ETHERNETIP: return (const TCHAR*)"EtherNet/IP";
        case E_AUDITLOG_SOURCE_PROFINETIO: return (const TCHAR*)"Profinet/IO";
        case E_AUDITLOG_SOURCE_OPCUA: return (const TCHAR*)"OPCUA";
        case E_AUDITLOG_SOURCE_MODBUSTCP: return (const TCHAR*)"Modbus";
        case E_AUDITLOG_SOURCE_ETHERNET: return (const TCHAR*)"Ethernet";
        case E_AUDITLOG_SOURCE_SYSTEM: return (const TCHAR*)"System";
        case E_AUDITLOG_SOURCE_WEBSERVER: return (const TCHAR*)"WebSrv";
        default: return (const TCHAR*)"UNK";
    }
}

//-------------------------------------------------------------------------
/*! 
\brief Save log to file
\author  
\date  
\param data Data to save to file
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
static void AuditLogSaveToFile(const TCHAR* data) {
    const size_t dataSize = strlen((const char*)data);
    FS_fileSystem_t* mainFs;
    FS_file_t* logFile;
    int32_t fsResult;
    TCHAR filePath[AUDIT_LOG_FILE_PATH_MAX];

    fsResult = FS_GetInstance(FS_instanceMain, &mainFs);
    if (fsResult != FS_OK) {
        return;
    }

    if (auditLogInternals.currentFileSize + dataSize > logSettings.maxBytesPerFile) {
        // Swap files
        TCHAR newFilePath[AUDIT_LOG_FILE_PATH_MAX];

        // Remove last file
        AuditLogBuildFilePath(filePath, logSettings.fileCount - 1);
        (void)FS_Remove(mainFs, (const char*)filePath);

        for (TUSIGN8 fileIndex = logSettings.fileCount - 1; fileIndex >= 1 ; --fileIndex) {
            AuditLogBuildFilePath(filePath, fileIndex - 1);
            AuditLogBuildFilePath(newFilePath, fileIndex);

            (void)FS_Rename(mainFs, (const char*)filePath, (const char*)newFilePath);
        }

        // Create first file
        AuditLogBuildFilePath(filePath, 0);
        (void)CreateFile(mainFs, filePath);

        auditLogInternals.currentFileSize = 0;
    }

    AuditLogBuildFilePath(filePath, 0);
    if (FS_FileOpen(mainFs, (const char*)filePath, FS_modeAppend, &logFile) == FS_OK) {
        (void)FS_FileWrite(mainFs, logFile, data, dataSize);
        (void)FS_FileClose(mainFs, &logFile);
    }

    auditLogInternals.currentFileSize += dataSize;
}

//-------------------------------------------------------------------------
/*! 
\brief Logs information to log
\author  
\date  
\param source Logging source
\param severity Logging severity
\param ptrText Logging text
\return Status of initialization 
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
void AuditLogLog(E_AUDITLOG_SOURCE source, E_AUDITLOG_SEVERITY severity, const TCHAR* ptrText, ...) {
    TCHAR* outputBuffer = buffer;
    TCHAR* uniqueText;
    va_list args;
    T_TIME_STAMP currentTime;

    (void)Take_RTOS_MUTEX(auditLogAccessMutex, RTOS_WAIT_FOREVER);

    // Startup counter
    outputBuffer += sprintf((char*)outputBuffer, "%u;", logSettings.startupCounter);

    // Print timestamp
    (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &currentTime);
    outputBuffer += TimeStampInISO8601(&currentTime, outputBuffer);
    outputBuffer = strappendch(outputBuffer, ';');

    // Print source
    uniqueText = outputBuffer;
    outputBuffer = strappendstr(outputBuffer, AuditLogSourceToText(source));
    outputBuffer = strappendch(outputBuffer, ';');
 
    // Print severity
    outputBuffer = strappendstr(outputBuffer, AuditLogSeverityToText(severity));
    outputBuffer = strappendch(outputBuffer, ';');
 
    size_t bytesLeft = (size_t)((AUDITLOG_ENTRY_SIZE - (outputBuffer - buffer)) - AUDTILOG_TEXT_NEW_LINE); // 2 is for endline and NULL char

    // Print text
    int maxOutput;

    //lint -e{10,40} Errors comming from std interface
    va_start(args, ptrText);
    maxOutput = vsnprintf((char*)outputBuffer, bytesLeft, (const char*)ptrText, args);
    va_end(args);

    if (maxOutput < 0) {
        maxOutput = 0;
    }

    outputBuffer += min_size_t((size_t)maxOutput, bytesLeft);

    if ((size_t)maxOutput > bytesLeft) {
        (void)strcpy((char*)outputBuffer - 3, "..."); 
    }

    // Print new line
    (void)strappendchnull(outputBuffer, '\n');

    // Check if log was not used previosly
    if (strncmp((const char*)uniqueText, (const char*)auditLogInternals.lastLoggedText, AUDITLOG_TEXT_MAX_LEN) != 0) {
        AuditLogSaveToFile(buffer);
        (void)strcpy_safe(auditLogInternals.lastLoggedText, uniqueText, AUDITLOG_TEXT_MAX_LEN);
    }

    (void)Give_RTOS_MUTEX(auditLogAccessMutex);
}

//-------------------------------------------------------------------------
/*! 
\brief Creates JSON with audit log information
\author  
\date  
\param ptrOutputBuffer Outbut buffer where JSON object will be written to
\return TCHAR*  End of string with JSON object
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
TCHAR* AuditLogInfoToJson(TCHAR* ptrOutputBuffer) {
    const TCHAR* filePath = logSettings.filePathBase;

    if (strstr((const char*)filePath, (const char*)webServerPath) != NULL) {
        filePath += strlen((const char*)webServerPath);
    }

    ptrOutputBuffer = strappendstr(ptrOutputBuffer, "\"audit\":{\"path\":\"");
    ptrOutputBuffer = strappendstr(ptrOutputBuffer, filePath);
    ptrOutputBuffer = strappendstr(ptrOutputBuffer, "\",\"count\":");
    ptrOutputBuffer += sprintf((char*)ptrOutputBuffer, "%u", logSettings.fileCount);
    ptrOutputBuffer = strappendstr(ptrOutputBuffer, "},");

    return ptrOutputBuffer;
}

//-------------------------------------------------------------------------
/*! 
 \brief Clears audit log
 \author  
 \date  
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
void AuditLogClear(void) {
    FS_fileSystem_t* mainFs;
    FS_file_t* logFile;
    int32_t fsResult;
    TCHAR filePath[AUDIT_LOG_FILE_PATH_MAX];

    // Initialize file system
    fsResult = FS_GetInstance(FS_instanceMain, &mainFs);
    if (fsResult == FS_OK) {
        (void)Take_RTOS_MUTEX(auditLogAccessMutex, RTOS_WAIT_FOREVER);

        // Initilize files
        for (TUSIGN8 fileIndex = 0; fileIndex < logSettings.fileCount; ++fileIndex) {
            AuditLogBuildFilePath(filePath, fileIndex);

            if (FS_FileOpen(mainFs, (const char*)filePath, FS_modeWrite, &logFile) == FS_OK) {
                (void)FS_FileClose(mainFs, &logFile);
            }
        }

       (void)Give_RTOS_MUTEX(auditLogAccessMutex);
    }
}
