#ifndef SU_INTERFACE_H
#define SU_INTERFACE_H

#include <stddef.h>

#define BYTES_PER_CHAR 1 
#define ARTIFACT_NAME_SIZE 30
#define ARCH_PATH_SIZE 128
#define FS_PATH_SIZE 128

#define SUP_FLAG_FLASH_ADDRESS 0x181ffff0

#define EXTRACT_DIRECTORY "/sandbox/"
#define SUP_LOCATION "/newSw/Update.7z"
#define TEMP_MANIFEST_LOCATION "/sandbox/manifest.csv.temp"
#define MANIFEST_LOCATION "/sandbox/manifest.csv"
#define UPDATE_PACKAGE_ETHERNETIP_LOCATION "/sandbox/firmware/OC_ETH/EthernetIP/update.pkg"
#define UPDATE_PACKAGE_PROFINET_LOCATION "/sandbox/firmware/OC_ETH/Profinet/update.pkg"
#define ETHERNET_IP_APP_DIR "/firmware/OC_ETH/EthernetIP/"
#define PROFINET_APP_DIR "/firmware/OC_ETH/Profinet/"
#define ETHERNET_IP_APP_LOCATION "/firmware/OC_ETH/EthernetIP/update.pkg"
#define PROFINET_APP_LOCATION "/firmware/OC_ETH/Profinet/update.pkg"
#define ADI_SOFTWARE_UPGRADE_STATUS_FILE "/status.txt"
#define ADI_SOFTWARE_UPGRADE_ERROR_LOG_FILE "/log.txt"
#define ADI_SOFTWARE_UPGRADE_ERROR_LOG_MAX_SIZE 100
#define ARTIFACT_IOAPP_NAME "ioapp"
#define ARTIFACT_OTHER_NAME "other"

#define UPDATE_PATH_LEN 60 // Typical path is "/firmware/OC_ETH/EthernetIP/update.pkg"

#define ARTIFACT_COMMAND_ADD "add"
#define ARTIFACT_COMMAND_DELETE "del"

typedef enum _E_SUP_RET
{
	E_SUP_OK = 0,
	E_SUP_CANNOT_OPEN_FILE = 1,
	E_SUP_INVALID_PARAM = 2,
	E_SUP_OUT_OF_MEMORY = 3,
	E_SUP_INTERNAL_ERROR = 4,
	E_SUP_FILESIZE_ERROR = 5,
    E_SUP_WRONG_MANIFEST = 6,
    E_SUP_FS_ERROR = 7,
    E_SUP_DECOMPRESSION_ERROR = 8,
    E_SUP_DECRYPTION_ERROR = 9,
    E_SUP_VERIFICATION_ERROR = 10,
    E_SUP_FLASH_ERROR = 11,
    E_SUP_COPYING_ERROR = 12,
    E_SUP_FLAG_ERROR = 13,
	E_SUP_ADI_UPDATE_ERROR = 14,
	E_SUP_NO_ACTION = 0xff
} E_SUP_RET;

typedef enum _E_SUP_ARTIFACT_TYPE
{
	E_SUP_ARTIFACT_ERROR = 0,
	E_SUP_ARTIFACT_OTHER = 1 << 0,
	E_SUP_ARTIFACT_IOAPP = 1 << 1,
	E_SUP_ARTIFACT_NONE = 0,
	E_SUP_ARTIFACT_ALL = 0xFF
} E_SUP_ARTIFACT_TYPE;

typedef enum _E_SUP_VERIFICATION_RESULT
{
	E_SUP_VER_RES_OK = 0,
	E_SUP_VER_RES_INVALID_ENTITYID = 1 << 0,
	E_SUP_VER_RES_INVALID_PLATFORMID = 1 << 1,
	E_SUP_VER_RES_INVALID_COMPONENTID = 1 << 2,
	E_SUP_VER_RES_INVALID_HARDWARE_REVISION = 1 << 3
} E_SUP_VERIFICATION_RESULT;

typedef enum _E_SUP_MANIFEST_COMMAND
{
	E_SUP_ADD = 0,
	E_SUP_DELETE = 1,
	E_SUP_COMMAND_ERROR = 2
}E_SUP_MANIFEST_COMMAND;

typedef struct _T_SUP_FW_VERSION
{
	unsigned char major;
	unsigned char minor;
	unsigned char bugfix;
}T_SUP_FW_VERSION;

typedef struct _T_SUP_MANIFEST_HEADER
{
    T_SUP_FW_VERSION updatePackageRevision;
    char entityID[8];
    char platformID[8];
    char componentID[8];
    char hardwareCompabilityWhiteList[64];
    char hardwareCompabilityBlackList[64];
}T_SUP_MANIFEST_HEADER;

typedef struct _T_SUP_MANIFEST_RECORD
{
	E_SUP_ARTIFACT_TYPE artifactType;
	char artifactName[ARTIFACT_NAME_SIZE];
	char archPath[ARCH_PATH_SIZE];
	char fsPath[FS_PATH_SIZE];
	E_SUP_MANIFEST_COMMAND command;
	long filesize;
	unsigned char crc32[32];
}T_SUP_MANIFEST_RECORD;

typedef enum _E_SUP_STATE
{
    E_SUP_STATE_INVALID = 0,
	E_SUP_STATE_PREVERIFICATION = 1,
	E_SUP_STATE_PREVERIFICATION_OK = 2, 
	E_SUP_STATE_FIRST_REBOOT = 3,
	E_SUP_STATE_FIRST_REBOOT_DECOMPRESSION = 4,
	E_SUP_STATE_FIRST_REBOOT_DECOMPRESSION_OK = 5,
    E_SUP_STATE_FIRST_REBOOT_VERIFICATION = 6,
    E_SUP_STATE_FIRST_REBOOT_UPDATE_COPY = 8,
    E_SUP_STATE_FIRST_REBOOT_UPDATE_COPY_OK = 9,
    E_SUP_STATE_SECOND_REBOOT = 10,
	E_SUP_STATE_UPDATE_STARTED = 11,
	E_SUP_STATE_UPDATE_OK = 12,
	E_SUP_STATE_UPDATE_FAIL = 13
}E_SUP_STATE;

//! SU interface
E_SUP_RET   softwareUpdate(TUSIGN8 selectedMainProtocol);
E_SUP_RET   prevalidateSoftwarePackage(const char* uploadedFile, T_SUP_MANIFEST_HEADER *manifestHeader, E_SUP_VERIFICATION_RESULT* pVerificationResult);
E_SUP_RET   triggerSoftwareUpdate(void);
E_SUP_STATE getSoftwareUpdateState(void);
E_SUP_RET parseSuStatusFileOutput(void);
E_SUP_RET parseSuLogFileOutput(void);
E_SUP_RET copyFilesFromSandboxToFs(void);
TBOOL getBootloaderErrorText(TCHAR* errorLogBuffer, size_t errorLogBufferSize);
E_SUP_RET setBootloaderFirmwareSource(TUSIGN8 mainProtocol, TBOOL fromSandbox);
void LogSoftwareUpdateStatus(E_SUP_RET result);

#endif