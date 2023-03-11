#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <OSPL_memory.h>
// #include "system_dbug.h"
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Services/SoftwareUpdate/Interface/SU_Interface.h"
#include "LinkedList.h"
#include "FS_interface.h"
#include "FS_utility.h"
#include "Services/SoftwareUpdate/Decompression/Interface/DecService.h"
#include "Diagnosis/AuditLog/Interface/auditlog.h"
#include "SEM_crypto.h"
#include "Services/Interface/string_helper.h"
#include "Services/Interface/checksum.h"
#include "FS_utility.h"
#include "Coordinator/Interface/Coordinator.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "Services/Interface/math_helper.h"
#include "Ethernet/Interface/Ethernet.h"
#include "CI_system_api.h"
#include "HWPL_sys_interface.h"

#define SUP_FLAG_FLASH_FILE "/supFlag"

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Sets Software Update State in state file
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     state Value to set
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET setSoftwareUpdateState(E_SUP_STATE state) {
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    FS_fileSystem_t* mainFs;
    FS_file_t* flagFile;
    
    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        if (FS_FileOpen(mainFs, SUP_FLAG_FLASH_FILE, FS_modeWrite, &flagFile) == FS_OK) {
            if (FS_FileWrite(mainFs, flagFile, (uint8_t*)&state, 1) == 1) {
                (void)FS_FileFlush(mainFs, flagFile);

                eRetVal = E_SUP_OK;
            }

            (void)FS_FileClose(mainFs, &flagFile);
        }  
    }
    
    // DBUG_LOG_INFO("[SW] Seting update state to %d result %d\n", state, eRetVal);

    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Gets Software Update State from state file
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    E_SUP_STATE 
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_STATE getSoftwareUpdateState() {
    FS_fileSystem_t* mainFs;
    FS_file_t* flagFile;
    uint8_t stateInFile; 
    E_SUP_STATE retVal = E_SUP_STATE_INVALID;
    
    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        if (FS_FileOpen(mainFs, SUP_FLAG_FLASH_FILE, FS_modeRead, &flagFile) == FS_OK) {
            if (FS_FileRead(mainFs, flagFile, &stateInFile, 1) == 1) {
                retVal = (E_SUP_STATE)stateInFile;
            }

            (void)FS_FileClose(mainFs, &flagFile);
        }
    }
  
    return retVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Parse type of artifact string to enum
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     artifactStr String from manifest
 \return    E_SUP_ARTIFACT_TYPE
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_ARTIFACT_TYPE parseArtifactType(const T_STRING_VIEW *artifactStr) {
	if (artifactStr->length > 0) {
        if (!strncmp((const char*)artifactStr->buffer, ARTIFACT_IOAPP_NAME, artifactStr->length)) {
            return E_SUP_ARTIFACT_IOAPP;
        }
        if (!strncmp((const char*)artifactStr->buffer, ARTIFACT_OTHER_NAME, artifactStr->length)) {
            return E_SUP_ARTIFACT_OTHER;
        }
    }

	return E_SUP_ARTIFACT_ERROR;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates sandbox folder temporary files
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET createSandboxFolder() {
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    FS_fileSystem_t* mainFs;
    FS_dir_t* dir;

    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        int32_t dirOpenResult = FS_DirOpen(mainFs, EXTRACT_DIRECTORY, &dir);
        if (dirOpenResult == FS_OK) {
            eRetVal = E_SUP_OK;
            (void)FS_DirClose(mainFs, &dir);
        }
        else if (dirOpenResult == FS_NOT_EXIST){
            if (FS_DirCreate(mainFs, EXTRACT_DIRECTORY) == FS_OK) {
                eRetVal = E_SUP_OK;
            }
        }
    }

    return eRetVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Check manifestHeader for compability, provide issues
 \author    Klaus Pose
 \date      2020-12-15
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  
 \n environment:         
 \n intention:           
 \n result module test:  
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET CheckManifestHeader(const T_SUP_MANIFEST_HEADER *manifestHeader, E_SUP_VERIFICATION_RESULT* pResult)
{
    char const * pString;
    size_t szString;
    int vComp;
    E_SUP_RET retVal = E_SUP_OK;

    if( !manifestHeader ) {
        return E_SUP_WRONG_MANIFEST;
    }

    if( pResult ) {
        *pResult = E_SUP_VER_RES_OK;
    }

    pString = "ABB";
    szString = strlen(pString);
    if( szString == strlen(manifestHeader->entityID) ) {
        vComp = memcmp( manifestHeader->entityID, pString, szString);
        if( vComp != 0 ) {
            if( pResult ) {
                *pResult |= E_SUP_VER_RES_INVALID_ENTITYID;
            }
            retVal = E_SUP_WRONG_MANIFEST;
        }
    } else {
        if( pResult ) {
            *pResult |= E_SUP_VER_RES_INVALID_ENTITYID;
        }
        retVal = E_SUP_WRONG_MANIFEST;
    }
    
    pString = "2WCTW";
    szString = strlen(pString);
    if( szString == strlen(manifestHeader->platformID) ) {
        vComp = memcmp( manifestHeader->platformID, pString, szString);
        if( vComp != 0 ) {
            if( pResult ) {
                *pResult |= E_SUP_VER_RES_INVALID_PLATFORMID;
            }
            retVal = E_SUP_WRONG_MANIFEST;
        }
    } else {
        if( pResult ) {
            *pResult |= E_SUP_VER_RES_INVALID_PLATFORMID;
        }
        retVal = E_SUP_WRONG_MANIFEST;
    }
    
    pString = "APL";
    szString = strlen(pString);
    if( szString == strlen(manifestHeader->componentID) ) {
        vComp = memcmp( manifestHeader->componentID, pString, szString);
        if( vComp != 0 ) {
            if( pResult ) {
                *pResult |= E_SUP_VER_RES_INVALID_COMPONENTID;
            }
            retVal = E_SUP_WRONG_MANIFEST;
        }
    } else {
        if( pResult ) {
            *pResult |= E_SUP_VER_RES_INVALID_COMPONENTID;
        }
        retVal = E_SUP_WRONG_MANIFEST;
    }

    return retVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates sandbox folder temporary files
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     list pointer to list with parsed manifest entries
 \param     location pointer to string with location of manifest
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static E_SUP_RET OpenManifest(update_list *list, const char* location, T_SUP_MANIFEST_HEADER *manifestHeader, E_SUP_VERIFICATION_RESULT* pVerificationResult) {
	E_SUP_RET eRetVal = E_SUP_OK;
    FS_fileSystem_t* mainFs;
    FS_file_t* file;
    long filesize;
    uint8_t* manifestBuffer;
    size_t manifestBufferSize;
    long manifestBytesRead;

    if( FS_GetInstance(FS_instanceMain, &mainFs) != FS_OK) {
        return E_SUP_CANNOT_OPEN_FILE;  
    }
        
    if (FS_FileOpen(mainFs, location, FS_modeRead, &file) !=  FS_OK) {
        return E_SUP_CANNOT_OPEN_FILE;
    }

    //! getting file size and rewinding to begginning
    filesize = FS_FileSize(mainFs, file);

    //! buffer for manifest allocated on slow heap
    manifestBufferSize = sizeof(char) * (unsigned)filesize;
    manifestBuffer = (uint8_t*)pvPortMallocSlow(manifestBufferSize);

    //! oom check
    if (manifestBuffer == NULL) {
        (void)FS_FileClose(mainFs, &file);
        return E_SUP_OUT_OF_MEMORY;
    }

    manifestBytesRead = FS_FileRead(mainFs, file, manifestBuffer, (uint32_t)filesize);

    //! check against reading    
    if (manifestBytesRead == filesize) {
        T_SUP_MANIFEST_RECORD man;
        TINT32 tempInt;

        T_STRING_VIEW fileContents;
        fileContents.buffer = manifestBuffer;
        fileContents.length = (TUSIGN32)filesize;

        (void)string_view_tok_int(&fileContents, ',', &tempInt, 10);
        manifestHeader->updatePackageRevision.major = tempInt & 0xFF;
        (void)string_view_tok_int(&fileContents, ',', &tempInt, 10);
        manifestHeader->updatePackageRevision.minor = tempInt & 0xFF;
        (void)string_view_tok_int(&fileContents, ',', &tempInt, 10);
        manifestHeader->updatePackageRevision.bugfix = tempInt & 0xFF;

        (void)string_view_tok_string(&fileContents, ',', (TCHAR*)manifestHeader->entityID);       // e.g. ABB-IAMA
        (void)string_view_tok_string(&fileContents, ',', (TCHAR*)manifestHeader->platformID);     // e.g. 4WCTW, 2WCTW ....
        (void)string_view_tok_string(&fileContents, ',', (TCHAR*)manifestHeader->componentID);    // OC_ETH 1 (2Ch 100BaseT, Type4), OC_ETH 2 (1Ch APL)
        (void)string_view_tok_string(&fileContents, ',', (TCHAR*)manifestHeader->hardwareCompabilityBlackList);
        (void)string_view_tok_string(&fileContents, '\n', (TCHAR*)manifestHeader->hardwareCompabilityWhiteList);

        eRetVal = CheckManifestHeader( manifestHeader, pVerificationResult );

        if( eRetVal == E_SUP_OK ) 
        {
            TBOOL endReached = eFALSE;

            while (endReached == eFALSE) {
                T_STRING_VIEW artifactType;

                endReached = strntokloc_view(&fileContents, &artifactType, ',');
                if (endReached == eTRUE) {
                    break;
                }

                //! 1. Parsing artifact type
                man.artifactType = parseArtifactType(&artifactType);
                if (man.artifactType == E_SUP_ARTIFACT_ERROR) {
                    eRetVal = E_SUP_WRONG_MANIFEST;
                    break;
                }

                //! 2. Getting filename
                endReached |= string_view_tok_string(&fileContents, ',', (TCHAR*)man.artifactName);

                //! 3. Getting path from archive
                endReached |= string_view_tok_string(&fileContents, ',', (TCHAR*)man.archPath);

                //! 4. Getting path from archive
                endReached |= string_view_tok_string(&fileContents, ',', (TCHAR*)man.fsPath);
                        
                //! 5. File size
                endReached |= string_view_tok_int(&fileContents, ',', (TINT32*)&(man.filesize), 10);

                if (endReached == eTRUE) {
                    eRetVal = E_SUP_WRONG_MANIFEST;
                }
                //! 6. CRC 
                endReached |= string_view_tok_strhex(&fileContents, '\n', man.crc32, sizeof(man.crc32));

                LinkedList_Append(man, list);
            }
        }
    } else
    {   // error at FS_FileRead(...)
        eRetVal = E_SUP_FS_ERROR;
    }

    //! Free memory from buffer
    vPortFreeSlow(manifestBuffer);

    (void)FS_FileClose(mainFs, &file);
    
    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Validates briefly SUP after upload
 \author    Piotr Kmiecinski, Klaus Pose
 \date      2020-02-24      , 2020-12-15
 \param     uploadedFile Name of SUP to verify
 \param     fwVersion Version of software taken from package
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET prevalidateSoftwarePackage(const char* uploadedFile, T_SUP_MANIFEST_HEADER *manifestHeader, E_SUP_VERIFICATION_RESULT* pVerificationResult) {
    E_SUP_RET eRetVal;    
    
    // DBUG_LOG_INFO("[SW] Prevalidation");
    
    eRetVal = createSandboxFolder();
    if (eRetVal != E_SUP_OK) {
        // DBUG_LOG_ERROR("[SW] Cannot create sandbox folder [error: %d]", eRetVal);
        return eRetVal;
    }

    eRetVal = setSoftwareUpdateState(E_SUP_STATE_PREVERIFICATION);
    if (eRetVal != E_SUP_OK) {
        return eRetVal;
    }
    
    eRetVal = decompressManifest(uploadedFile);
    if (eRetVal != E_SUP_OK) {
        // DBUG_LOG_ERROR("[SW] Failed to decompress [error: %d]", eRetVal);
        return eRetVal;
    }

    //! Create list for manifest entries    
    update_list *list = LinkedList_Make();

    eRetVal = OpenManifest(list, TEMP_MANIFEST_LOCATION, manifestHeader, pVerificationResult);

    if (eRetVal == E_SUP_OK) {
        eRetVal = setSoftwareUpdateState(E_SUP_STATE_PREVERIFICATION_OK);
    }
    else {
        // DBUG_LOG_ERROR("[SW] Open manifest failed [error: %d]", eRetVal);
    }

    LinkedList_Delete(list);

    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Validates SUP
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     uploadedFile Name of SUP to verify
 \param     fwVersion Version of software taken from package
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET ValidateSUPackage(const update_list* filesList) {
    E_SUP_RET eRetVal = E_SUP_INTERNAL_ERROR;
    char filePath[ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE+1];
    int32_t fileSize = 0;
    TUSIGN8 digest[32]; 
    FS_fileSystem_t* fileSystem;

    if (FS_GetInstance(FS_instanceMain, &fileSystem) == FS_OK) {
        if (filesList->head != NULL) {
            eRetVal = E_SUP_OK;

            const list_node* currentFile = filesList->head;
            for (; ((currentFile != NULL) && (eRetVal == E_SUP_OK)); currentFile = currentFile->next) {
                // Build file path
                memset(filePath, 0, ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE + 1);
                strcat(filePath, currentFile->record.archPath);
                strcat(filePath, (const char*)"/");
                strcat(filePath, currentFile->record.artifactName);

                // Verify file size
                fileSize = FS_FileSizeFromPath(fileSystem, filePath);
                if (fileSize != currentFile->record.filesize) {
                    eRetVal = E_SUP_FILESIZE_ERROR;
                    break;
                }
                
                // Verify checksum
                if (GetFileChecksum((const TCHAR*)filePath, digest, sizeof(digest)) != eTRUE) {
                    eRetVal = E_SUP_INTERNAL_ERROR;
                    break;
                }
                if (memcmp(currentFile->record.crc32, digest, sizeof(digest)) != 0) {
                    eRetVal = E_SUP_WRONG_MANIFEST;
                    break;
                }
            }
        }
    }

	return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Starts software update process 
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET triggerSoftwareUpdate() {
    E_SUP_RET eRetVal;
      
    // DBUG_LOG_INFO("[SW] Trigger software update");
      
    eRetVal = setSoftwareUpdateState(E_SUP_STATE_FIRST_REBOOT);
    if (eRetVal != E_SUP_OK) {
        return eRetVal;
    }

// #ifndef COMPILE_CONFIG_FACTORY1
//     (void)mBInterface.SetTimeBusyEXE(60*10);
// #endif

    // coordinator.Exit(CORD_STARTUP_REASON_SOFTWARE_RESET);
    elettronicServices.ExitSRV();

    //! Actually, never gets to this point because of board reset
    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Writes given string into ADI bootloader status file
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     path path to status file
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET writeToAdiUpgradeStatusFile(const char* value) {
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    FS_fileSystem_t* mainFs;
    FS_file_t* flagFile;

    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        if (FS_FileOpen(mainFs, ADI_SOFTWARE_UPGRADE_STATUS_FILE, FS_modeWrite, &flagFile) == FS_OK) {
            size_t bufferSize = strlen(value); 
            if (FS_FileWrite(mainFs, flagFile, (const uint8_t*)value, bufferSize) == (int32_t)bufferSize) {
                eRetVal = E_SUP_OK;
            }

            (void)FS_FileClose(mainFs, &flagFile);
        }  
    }
    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates status.txt file for ADI-side SU process
 \author    Piotr Kmiecinski, Klaus Pose
 \date      2020-02-24      , 2021-01-11
 \param     path path to status file
 \param     fromSandbox eTRUE, if the file shall be referenced directly from sandbox
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
#define ADI_SOFTWARE_UPGRADE_STATUS_CONTENT_OVERHEAD 5
E_SUP_RET setBootloaderFirmwareSource(TUSIGN8 mainProtocol, TBOOL fromSandbox)
{
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    char updatePath[UPDATE_PATH_LEN];
    const char* path;
    FS_fileSystem_t* mainFs;

	// Set firmware path based on protocol
    if( fromSandbox == eTRUE ) {
        switch (mainProtocol) {
            case ETH_ETHERNETIP_PROTOCOL: path = UPDATE_PACKAGE_ETHERNETIP_LOCATION; break;
            case ETH_PROFINETIO_PROTOCOL: path = UPDATE_PACKAGE_PROFINET_LOCATION; break;

            case ETH_INVALID_PROTOCOL:
            default:
                path = UPDATE_PACKAGE_ETHERNETIP_LOCATION; 
                break;
        }
    } else
    {
        switch (mainProtocol) {
            case ETH_ETHERNETIP_PROTOCOL: path = ETHERNET_IP_APP_LOCATION; break;
            case ETH_PROFINETIO_PROTOCOL: path = PROFINET_APP_LOCATION; break;

            case ETH_INVALID_PROTOCOL:
            default:
                path = ETHERNET_IP_APP_LOCATION; 
                break;
        }
    }

    if (strlen(path) <= (UPDATE_PATH_LEN - ADI_SOFTWARE_UPGRADE_STATUS_CONTENT_OVERHEAD)) {
        // Check if file to change to exists
        if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
            if (FS_FileExists(mainFs, path) == FS_ERROR) {
                eRetVal = E_SUP_CANNOT_OPEN_FILE;
            }
            else {
                sprintf(updatePath, "1:%s:", path);

                eRetVal = writeToAdiUpgradeStatusFile(updatePath);
            }
        }
    }

    return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Process status.txt file for ADI-side SU process
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET parseSuStatusFileOutput()
{
	E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    FS_fileSystem_t *mainFs;
	FS_file_t *flagFile;
	char suStatus = 0;
    int32_t readLen = 0;
    
	if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        if (FS_FileOpen(mainFs, ADI_SOFTWARE_UPGRADE_STATUS_FILE, FS_modeRead, &flagFile) == FS_OK) {
            readLen = FS_FileSize(mainFs, flagFile);
            
            if (readLen > 0) {
                // Read first character with status
                if (FS_FileRead(mainFs, flagFile, (uint8_t*)&suStatus, 1) == 1) {
                    eRetVal = (suStatus == '0') ? E_SUP_OK : E_SUP_ADI_UPDATE_ERROR;
                }
            }

            (void)FS_FileClose(mainFs, &flagFile);
        }
    }

	return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Removes manifest used for prevalidation of SUP
 \author    Piotr Kmiecinski, Klaus Pose
 \date      2020-02-24      , 2021-01-18
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET removeTemporaryManifest()
{
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;
    FS_fileSystem_t* mainFs;

    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        (void)FS_Remove(mainFs, TEMP_MANIFEST_LOCATION);    // failure if not present => OK
        eRetVal = E_SUP_OK;
    }

	return eRetVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Copies fles based on manifest file and type of artifact
 \author    Michal Kochel
 \date      2020-05-25
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET CopyFilesBasedOnManifest(const update_list* list, TUSIGN32 artifactTypesOk, TUSIGN32 artifactTypesNok) {
    FS_fileSystem_t *fs_p;
    E_SUP_RET eRetVal = E_SUP_FS_ERROR;

    if (FS_GetInstance(FS_instanceMain, &fs_p) == FS_OK) {
        char sourcePath[ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE + 1];
        char destPath[ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE + 1];
        
        const list_node * current = list->head;
        if (list->head != NULL) {
            eRetVal = E_SUP_OK;
            for (; current != NULL; current = current->next) {
                if ((current->record.artifactType & artifactTypesOk) > 0 && (current->record.artifactType & artifactTypesNok) == 0) {
                    // Make sure directory exists
                    if (FS_CreateDirRec(fs_p, current->record.fsPath, destPath) != FS_OK) {
                        eRetVal = E_SUP_INTERNAL_ERROR;
                    }

                    memset(sourcePath, 0, ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE + 1);
                    strcat(sourcePath, current->record.archPath);
                    strcat(sourcePath, (const char*)"/");
                    strcat(sourcePath, current->record.artifactName);
                    
                    memset(destPath, 0, ARCH_PATH_SIZE + ARTIFACT_NAME_SIZE + 1);
                    strcat(destPath, current->record.fsPath);
                    strcat(destPath, (const char*)"/");
                    strcat(destPath, current->record.artifactName);

                    // DBUG_LOG_INFO("[SW] CP %s -> %s", sourcePath, destPath );
            
                    if (FS_FileCopy(fs_p, sourcePath, destPath) != FS_OK) {
                        // DBUG_LOG_ERROR("[SW] Cannot copy artifact");
                        eRetVal = E_SUP_INTERNAL_ERROR;
                    }
                }
            }
        }
    }
    return eRetVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Copies files from sandbox to proper locations on FS
 \author    Piotr Kmiecinski, Klaus Pose
 \date      2020-02-24      , 2021-01-12
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET copyFilesFromSandboxToFs()
{
    E_SUP_RET eRetVal;
    T_SUP_MANIFEST_HEADER manifestHeader;
    FS_fileSystem_t* mainFs;

    update_list* list = LinkedList_Make();
    
    eRetVal = OpenManifest(list, MANIFEST_LOCATION, &manifestHeader, NULL);
    if (eRetVal == E_SUP_OK) {
        if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
            // clean folder first
            (void)FS_RemoveDirRecursively(mainFs, "public/web-server/downloads");
        }

        eRetVal = CopyFilesBasedOnManifest(list, E_SUP_ARTIFACT_ALL, E_SUP_ARTIFACT_NONE);    
    }

    LinkedList_Delete(list);

    return eRetVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Pre decompression action: e.g. purge sandbox
 \author    Klaus Pose
 \date      2021-01-13
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static E_SUP_RET mPreDecompression(void)
{
    E_SUP_RET eRetVal = E_SUP_OK;
    int32_t i32;
    FS_fileSystem_t* mainFs;
//    char pathStorage[FS_NAME_MAX] = "/sandbox/";

    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        // purge sandbox folder
        i32 = FS_RemoveDirRecursively(mainFs, "sandbox" );
        if( i32 != FS_OK ) {
            eRetVal = E_SUP_CANNOT_OPEN_FILE;
        }
        // DBUG_LOG_DEBUG("[SW] purge sandbox result = %d", i32);
    }
    return eRetVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Reads error log from bootloader
 \author    Michal Kochel
 \date      2020-08-28
 \param     errorLogBuffer Buffer for error text
 \param     errorLogBufferSize Size of buffer for error text
 \return    TBOOL If data was read from file
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
TBOOL getBootloaderErrorText(TCHAR* errorLogBuffer, size_t errorLogBufferSize) {
    FS_fileSystem_t* mainFs;
    FS_file_t* logFile;
    size_t bytesRead = 0;

    if (FS_GetInstance(FS_instanceMain, &mainFs) == FS_OK) {
        if (FS_FileOpen(mainFs, ADI_SOFTWARE_UPGRADE_ERROR_LOG_FILE, FS_modeRead, &logFile) == FS_OK) {
            
            bytesRead = (size_t)FS_FileRead(mainFs, logFile, errorLogBuffer, errorLogBufferSize);
            errorLogBuffer[min_size_t(bytesRead, errorLogBufferSize - 1)] = '\0';
            
            ReplaceChar(errorLogBuffer, errorLogBufferSize, '\r', '\0');
            ReplaceChar(errorLogBuffer, errorLogBufferSize, '\n', '\0');
            
            (void)FS_FileClose(mainFs, &logFile);
        }
    }
    return bytesRead > 0 ? eTRUE : eFALSE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Add entry to audit log with status of software update
 \author    Michał Kochel
 \date      2020-04-23
 \param     result Sw update result
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
void LogSoftwareUpdateStatus(E_SUP_RET result) {
    if (result != E_SUP_NO_ACTION) {
        E_SUP_STATE updateState = getSoftwareUpdateState();

        if (result == E_SUP_OK) {
#ifdef COMPILE_WITH_AUDITLOG
            AuditLogLog(
                E_AUDITLOG_SOURCE_SYSTEM, 
                E_AUDITLOG_SEVERITY_INFO,
                "SW update succeeded"
                );
#endif
        }
        else {

            TCHAR errorLogBuffer[ADI_SOFTWARE_UPGRADE_ERROR_LOG_MAX_SIZE];
            errorLogBuffer[0] = '\0';

            if (updateState >= E_SUP_STATE_SECOND_REBOOT) {
                (void)getBootloaderErrorText(errorLogBuffer, ADI_SOFTWARE_UPGRADE_ERROR_LOG_MAX_SIZE);
            }
#ifdef COMPILE_WITH_AUDITLOG
            AuditLogLog(
                E_AUDITLOG_SOURCE_SYSTEM,
                E_AUDITLOG_SEVERITY_ERROR,
                "SW update failed [st: %u, e: %d t: %s]",
                updateState,
                result,
                errorLogBuffer
            );
#endif
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Software update
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET softwareUpdate(TUSIGN8 selectedMainProtocol)
{	
	E_SUP_RET eRetVal = E_SUP_NO_ACTION;

    if (getSoftwareUpdateState() == E_SUP_STATE_FIRST_REBOOT) {
        // DBUG_LOG_INFO("[SW] First reboot");

        eRetVal = setSoftwareUpdateState(E_SUP_STATE_FIRST_REBOOT_DECOMPRESSION);
        if (eRetVal != E_SUP_OK) {
            return eRetVal;
        }

        (void)mPreDecompression();

        eRetVal = decompressSUP(SUP_LOCATION);
        if (eRetVal != E_SUP_OK) {
            // DBUG_LOG_ERROR("[SW] Decompression failed [error:%d]", eRetVal);
            eRetVal = E_SUP_DECOMPRESSION_ERROR;
            return eRetVal;
        }

        (void)setSoftwareUpdateState(E_SUP_STATE_FIRST_REBOOT_VERIFICATION);

        T_SUP_MANIFEST_HEADER manifestHeader;
        update_list* filesList = LinkedList_Make();
    
        if (OpenManifest(filesList, MANIFEST_LOCATION, &manifestHeader, NULL) == E_SUP_OK) {
            eRetVal = ValidateSUPackage(filesList);
            if (eRetVal == E_SUP_OK) {
                (void)setSoftwareUpdateState(E_SUP_STATE_FIRST_REBOOT_UPDATE_COPY_OK);
            }
            else {
                // DBUG_LOG_ERROR("[SW] Verification failed [error:%d]", eRetVal);
                eRetVal = E_SUP_VERIFICATION_ERROR;
            }

            LinkedList_Delete(filesList);
        }

        if (eRetVal != E_SUP_OK) {
            return eRetVal;
        }
        
        eRetVal = setBootloaderFirmwareSource(selectedMainProtocol, eTRUE);
        if (eRetVal != E_SUP_OK) {
            // DBUG_LOG_ERROR("[SW] Failed to set IO-APP location for bootloader [error:%d, activeProtocol:%d]", eRetVal, selectedMainProtocol);
            eRetVal = E_SUP_FLAG_ERROR;
            return eRetVal;
        }
       
        eRetVal = removeTemporaryManifest();
        if (eRetVal != E_SUP_OK) {
            // DBUG_LOG_ERROR("[SW] FS error [error:%d]", eRetVal);
            return eRetVal;
        }

        eRetVal = setSoftwareUpdateState(E_SUP_STATE_SECOND_REBOOT);
        if (eRetVal != E_SUP_OK) {
            return eRetVal;
        }

        // DBUG_LOG_INFO("[SW] Trigger second reboot");

        // force software reset
        HWPL_SysReset();

        // This code will only be reached when testing localy
        return E_SUP_INTERNAL_ERROR;
    }

    if (getSoftwareUpdateState() == E_SUP_STATE_SECOND_REBOOT) {
        // DBUG_LOG_INFO("[SW] Second reboot");

        eRetVal = parseSuStatusFileOutput();
        if (eRetVal != E_SUP_OK) {   
            // DBUG_LOG_ERROR("[SW] Bootloader update failed");

            (void)writeToAdiUpgradeStatusFile("0::");

            (void)setSoftwareUpdateState(E_SUP_STATE_UPDATE_FAIL);
            eRetVal = E_SUP_ADI_UPDATE_ERROR;
        }
        else {
            eRetVal = copyFilesFromSandboxToFs();
            if (eRetVal != E_SUP_OK) {
                // DBUG_LOG_ERROR("[SW] Copy from sandbox failed [error:%d]", eRetVal);  
                (void)setSoftwareUpdateState(E_SUP_STATE_UPDATE_FAIL);
                eRetVal = E_SUP_COPYING_ERROR;
            }
            else {
                // DBUG_LOG_INFO("[SW] Update OK");
                (void)setSoftwareUpdateState(E_SUP_STATE_UPDATE_OK);
                eRetVal = E_SUP_OK;
            }
        }
    }
    return eRetVal;
}
