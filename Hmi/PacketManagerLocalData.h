#ifndef _PACKETMANAGERLOCAL_DATA_H
#define _PACKETMANAGERLOCAL_DATA_H

/* ----------FUNCTION CODES from CB-------------------------- */
#define WRITE_STRING                1
#define WRITE_ARRAY                 2
#define SETUP_DISPLAY               3
#define BLANK_AREA                  4
#define READ_SWITCHES               5
#define SET_CONTRAST                6
//#define WRITE_PRE_DEFINED_STRING    7   // This function code is not supported
#define WRITE_SLIDER_BAR            8
#define WRITE_FRAME_TITLE           9
#define DRAW_AND_REPEAT_CUSTOM_CHAR             10
#define WRITE_ICON                  11
#define WRITE_STRING_ASIAN          12
#define WRITE_FRAME_TITLE_ASIAN     13
#define COMMS_V2_INIT               14
#define LOW_LEVEL_INFO              15
#define ROTATE_DISPLAY              16
//#define WRITE_LLRAM                 17   // This function code is not supported
//#define TRANSFER_RAM                18   // This function code is not supported
#define DRAW_WAVEFORM_GRID          19
#define DRAW_WAVEFORM_TRACE         20
#define UPDATE_WAVEFORM_DISPLAY     21

#define MIN_VALID_FUNCTION_CODE          1
#define MAX_VALID_FUNCTION_CODE          21
#define nVALID_FUNCTION_CODES            21


// define the frame type used in communication between CB and smart HMI
#define FRAME_TYPE_END                39
#define FRAME_OPERATOR_PAGE           40
#define FRAME_CONFIG_HEADER           41
#define FRAME_MENU_ACCESS_LEVEL       42
#define FRAME_MENU_SIGNAL             43
#define FRAME_MENU_OPERATOR           44
#define FRAME_MENU_DIAGNOSE3          45
#define FRAME_MENU_DIAGNOSE5          46
#define FRAME_MENU_DIAGNOSE6          47
#define FRAME_MENU_USER               48
#define FRAME_NUMERICAL_VIEW          49
#define FRAME_NUMERICAL_EDIT          50
#define FRAME_TIMEDATE_VIEW           51
#define FRAME_TIMEDATE_EDIT           52
#define FRAME_STRING_VIEW             53
#define FRAME_STRING_EDIT             54
#define FRAME_ENUM_VIEW               55
#define FRAME_ENUM_EDIT               56
#define FRAME_ADJUEST                 57
#define FRAME_DISPLAY_TEST            58
#define FRAME_LINEAR                  59
#define FRAME_WAVEFORM                60
#define FRAME_MANUAL_CALIBRATION      61
#define FRAME_GRAPH_OPERATOR          62


#define MIN_VALID_FRAME_CODE              39         
#define MAX_VALID_FRAME_CODE              62


//function code from HMI
#define INITIALISE_HMI                    50

#if defined(HMI_SUPPORTS_DISPLAY_MODE_1x6_BAR_A) || defined(HMI_SUPPORTS_DISPLAY_MODE_1x6_BAR_B) || defined(HMI_SUPPORTS_DISPLAY_MODE_1x9_BAR) || defined(HMI_SUPPORTS_DISPLAY_MODE_2x9_BAR)
#define HMI_SUPPORTS_OPERATOR_BARGRAPH
#endif

#define DISGUISE_PASSWORDS

//function code from HMI
#define INITIALISE_HMI                    50

#ifdef HMI_SUPPORTS_OPERATOR_FRAMES

typedef struct
{
    ADJUST_DATA dataValue;
    TUSIGN8     dataType;
    TBOOL       ruleViolation;
    TUSIGN8     fontSize_V;
    TUSIGN8     justifyForValues;
    TUSIGN8     minLengthForValues;
    TUSIGN8     significantDigitsForValues;
    TUSIGN8     lineNumber_V;
    TUSIGN8     lineNumber_ID;
} OPERATOR_PAGE_LINE_DATA;
#endif

typedef enum
{
    powerUpState_InitialCondition,
    powerUpState_OffDelay,
    powerUpState_WaitingForHMI,
    powerUpState_HMIDetected
}POWERUP_STATE;

typedef enum
{
    DISPLAYSTYLE_OFF_0  = HMI_DISPLAY_MODE_OFF,
    DISPLAYSTYLE_OFF_1  = HMI_DISPLAY_MODE_OFF              + 1,
    DISPLAYSTYLE_OFF_2  = HMI_DISPLAY_MODE_OFF              + 2,
    DISPLAYSTYLE_OFF_3  = HMI_DISPLAY_MODE_OFF              + 3,
//    EURO_1x3            = (HMI_DISPLAY_MODE_1x3 * 4),
//    EURO_1x3_DIAG       = (HMI_DISPLAY_MODE_1x3 * 4)        + 1,
//    ASIA_1x3            = (HMI_DISPLAY_MODE_1x3 * 4)        + 2,
//    ASIA_1x3_DIAG       = (HMI_DISPLAY_MODE_1x3 * 4)        + 3,
    EURO_1x4            = (HMI_DISPLAY_MODE_1x4 * 4),
    EURO_1x4_DIAG       = (HMI_DISPLAY_MODE_1x4 * 4)        + 1,
    ASIA_1x4            = (HMI_DISPLAY_MODE_1x4 * 4)        + 2,
    ASIA_1x4_DIAG       = (HMI_DISPLAY_MODE_1x4 * 4)        + 3,
    EURO_1x6_A          = (HMI_DISPLAY_MODE_1x6_A * 4),
    EURO_1x6_A_DIAG     = (HMI_DISPLAY_MODE_1x6_A * 4)      + 1,
    ASIA_1x6_A          = (HMI_DISPLAY_MODE_1x6_A * 4)      + 2,
    ASIA_1x6_A_DIAG     = (HMI_DISPLAY_MODE_1x6_A * 4)      + 3,
    EURO_1x6_BAR_A      = (HMI_DISPLAY_MODE_1x6_BAR_A * 4),
    EURO_1x6_BAR_A_DIAG = (HMI_DISPLAY_MODE_1x6_BAR_A * 4)  + 1,
    ASIA_1x6_BAR_A      = (HMI_DISPLAY_MODE_1x6_BAR_A * 4)  + 2,
    ASIA_1x6_BAR_A_DIAG = (HMI_DISPLAY_MODE_1x6_BAR_A * 4)  + 3,
    EURO_1x6_B          = (HMI_DISPLAY_MODE_1x6_B * 4),
    EURO_1x6_B_DIAG     = (HMI_DISPLAY_MODE_1x6_B * 4)      + 1,
    ASIA_1x6_B          = (HMI_DISPLAY_MODE_1x6_B * 4)      + 2,
    ASIA_1x6_B_DIAG     = (HMI_DISPLAY_MODE_1x6_B * 4)      + 3,
    EURO_1x6_BAR_B      = (HMI_DISPLAY_MODE_1x6_BAR_B * 4),
    EURO_1x6_BAR_B_DIAG = (HMI_DISPLAY_MODE_1x6_BAR_B * 4)  + 1,
    ASIA_1x6_BAR_B      = (HMI_DISPLAY_MODE_1x6_BAR_B * 4)  + 2,
    ASIA_1x6_BAR_B_DIAG = (HMI_DISPLAY_MODE_1x6_BAR_B * 4)  + 3,
    EURO_1x9            = (HMI_DISPLAY_MODE_1x9 * 4),
    EURO_1x9_DIAG       = (HMI_DISPLAY_MODE_1x9 * 4)        + 1,
    ASIA_1x9            = (HMI_DISPLAY_MODE_1x9 * 4)        + 2,
    ASIA_1x9_DIAG       = (HMI_DISPLAY_MODE_1x9 * 4)        + 3,
    EURO_1x9_BAR        = (HMI_DISPLAY_MODE_1x9_BAR * 4),
    EURO_1x9_BAR_DIAG   = (HMI_DISPLAY_MODE_1x9_BAR * 4)    + 1,
    ASIA_1x9_BAR        = (HMI_DISPLAY_MODE_1x9_BAR * 4)    + 2,
    ASIA_1x9_BAR_DIAG   = (HMI_DISPLAY_MODE_1x9_BAR * 4)    + 3,
    EURO_2x9            = (HMI_DISPLAY_MODE_2x9 * 4),
    EURO_2x9_DIAG       = (HMI_DISPLAY_MODE_2x9 * 4)        + 1,
    ASIA_2x9            = (HMI_DISPLAY_MODE_2x9 * 4)        + 2,
    ASIA_2x9_DIAG       = (HMI_DISPLAY_MODE_2x9 * 4)        + 3,
    EURO_2x9_BAR        = (HMI_DISPLAY_MODE_2x9_BAR * 4),
    EURO_2x9_BAR_DIAG   = (HMI_DISPLAY_MODE_2x9_BAR * 4)    + 1,
    ASIA_2x9_BAR        = (HMI_DISPLAY_MODE_2x9_BAR * 4)    + 2,
    ASIA_2x9_BAR_DIAG   = (HMI_DISPLAY_MODE_2x9_BAR * 4)    + 3,
    EURO_3x9            = (HMI_DISPLAY_MODE_3x9 * 4),
    EURO_3x9_DIAG       = (HMI_DISPLAY_MODE_3x9 * 4)        + 1,
    ASIA_3x9            = (HMI_DISPLAY_MODE_3x9 * 4)        + 2,
    ASIA_3x9_DIAG       = (HMI_DISPLAY_MODE_3x9 * 4)        + 3,
    EURO_4x9            = (HMI_DISPLAY_MODE_4x9 * 4),
    EURO_4x9_DIAG       = (HMI_DISPLAY_MODE_4x9 * 4)        + 1,
    ASIA_4x9            = (HMI_DISPLAY_MODE_4x9 * 4)        + 2,
    ASIA_4x9_DIAG       = (HMI_DISPLAY_MODE_4x9 * 4)        + 3,
}DISPLAYSTYLE;

#if defined(TYPE_1_IMPLEMENTATION)
    #define EARLIEST_TYPE_1_LOW_LEVEL_SUPPORTING_240BYTE_BUFFER     41
    #define PACKETBUFFERSIZE_BEFORE_240                             60
#endif

#define BUFFERSIZE_SMART_HMI          80
#define PACKETSIZE_SMART_HMI          40   //33, the lowlevelinfo_s will exceed 33

#ifdef HMI_SUPPORTS_SERVICEPORT
#define PACKETBUFFERSIZE_SP                                         60

#if (PACKETBUFFERSIZE_SP > PACKETBUFFERSIZE)
#error PACKETBUFFERSIZE_SP cannot be greater than PACKETBUFFERSIZE
#endif

#endif

// define the frame type used in communication between CB and smart HMI
#define PACKET_HL_SIZE  40

#define FRAME_TYPE_END                39
#define FRAME_OPERATOR_PAGE           40
#define FRAME_CONFIG_HEADER           41
#define FRAME_MENU_ACCESS_LEVEL       42
#define FRAME_MENU_SIGNAL             43
#define FRAME_MENU_OPERATOR           44
#define FRAME_MENU_DIAGNOSE3          45
#define FRAME_MENU_DIAGNOSE5          46
#define FRAME_MENU_DIAGNOSE6          47
#define FRAME_MENU_USER               48
#define FRAME_NUMERICAL_VIEW          49
#define FRAME_NUMERICAL_EDIT          50
#define FRAME_TIMEDATE_VIEW           51
#define FRAME_TIMEDATE_EDIT           52
#define FRAME_STRING_VIEW             53
#define FRAME_STRING_EDIT             54
#define FRAME_ENUM_VIEW               55
#define FRAME_ENUM_EDIT               56
#define FRAME_ADJUEST                 57
#define FRAME_DISPLAY_TEST            58
#define FRAME_LINEAR                  59
#define FRAME_WAVEFORM                60
#define FRAME_MANUAL_CALIBRATION      61
#define FRAME_GRAPH_OPERATOR          62
#define FRAME_IPADDRESS_VIEW          63
#define FRAME_IPADDRESS_EDIT          64


//function code from HMI
#define INITIALISE_HMI                    50

typedef struct
{
    // elements that must be initialised
    FRAME_FORMAT_TYPE const *pPrevDisplayFrame;
    TUSIGN8                 prevEditEnable;
    TUSIGN8                 packetBufferUsed;    
    TBOOL                   packetBufferPreparing;
    TUSIGN8                 lastFunctionCode;
    TUSIGN8                 SwitchErrorCount;
    TUSIGN8                 TransmitterBusy;
    TUSIGN8                 ReceiverWaiting;
    TUSIGN8                 RxByte[15];
    TUSIGN8                 LowLevelHMIFitted;
    TBOOL                   prvHMIcanAccessIO;
    TBOOL                   newCommsMode;
    TUSIGN8                 powerSwitchTimer;
    TUSIGN8                 RxCount;
    TUSIGN8                 DisplayDefaultContrast;
    TBOOL                   Asian;
    TBOOL                   PrvAsian;
    TUSIGN8                 AsianCharacterSet;
    #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
    TUSIGN8                 AsianBlockVersion;
    #endif

    TUSIGN8                 flashState;
    TUSIGN8                 last_contrast;
    TUSIGN8                 last_displayMode;
    TUSIGN8                 DisplayHardwareRevision;
    TUSIGN8                 DisplaySoftwareRevision;
    TBOOL                   overrangeValue_EditNotAllowed;
    POWERUP_STATE           powerUpState;
    TBOOL                   displayAutoscrollIcon;
    TUSIGN8                 Field_3_Icon;
    TUSIGN8                 Field_5_Icon;
    TUSIGN8                 Field_7_Icon;
    #ifdef EDP300_SPECIAL_FEATURES_HMI
    TUSIGN8                 Field_9_Icon;
    #endif

    TBOOL                   displayWriteProtectIcon;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    TBOOL                   prvBlankHistory;
    TUSIGN8                 infoMessageID;
    TUSIGN16                infoMessageTimer;
    #endif
    #ifdef AZ200_SPECIAL_FEATURES_HMI
    TBOOL                   autoExit;
    #endif
    TBOOL                   COMMS_V2_INIT_reqd;
    TBOOL                   LOW_LEVEL_INFO_reqd;

//    #ifdef HMI_SUPPORTS_GRAPH_VIEW
//    TUSIGN8                 *GraphPointer_Page1;
//    TUSIGN8                 *GraphPointer_Page2;
//    TUSIGN8                 *GraphPointer_Page3;
//    TUSIGN8                 *GraphPointer_Page4;
//    #endif

    // elements that need not be initialised
    FRAME_FORMAT_TYPE const *pCurrDisplayFrame;
    TUSIGN8                 CommunicationsErrorCode;
    TUSIGN8                 prevErrorAction;
    TUSIGN8                 iconStage;
    TUSIGN8                 barPosition;
    TINT16                  barPercent;
    TUSIGN8                 packetStage;
    TUSIGN8                 packet_HL[PACKET_HL_SIZE];
    TUSIGN8                 packetBuffer[PACKETBUFFERSIZE];
    TINT16                  sliderBarHigh;
    TINT16                  sliderBarActual;
    TUSIGN8                 highlightCursor;
    TUSIGN8                 TxLength;
    TUSIGN8                 TxCount;
    TEXT_IDENTIFIER_TYPE    stringtable_text_id;
    TUSIGN8                 updateActiveArea;
    TUSIGN8                 updateStaticArea;
    TUSIGN8                 keypressProcessed;
    TUSIGN8                 SwitchPressedCode;
    TUSIGN8                 SimpleIndicatorSwitchState;
    TUSIGN8                 prvBlankDisplay;
    DISPLAYSTYLE            last_displayStyle;
    TUSIGN8                 last_bargraphStyle;
    DISPLAYSTYLE            displayStyle;
    TUSIGN8                 bargraphStyle;
    TUSIGN8                 enumCursor;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    TUSIGN8                 prvAlarmActive;
    TUSIGN8                 prvAlarmClassification;
    TUSIGN8                 prvAlarmCategory;
    TUSIGN8                 prvAlarmPriority;
    TUSIGN8                 prvAlarmID;
    #endif
    #if defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_AUTO_CALIBRATION)
    TUSIGN8                 prvCalibrationStatus;
    #endif
}PACKETMANAGER_LOCAL_TYPE;

extern PACKETMANAGER_LOCAL_TYPE  *pPacketManagerLocal;

#define LOCAL_pPrevDisplayFrame             pPacketManagerLocal->pPrevDisplayFrame
#define LOCAL_prevEditEnable                pPacketManagerLocal->prevEditEnable
#define LOCAL_packetBufferUsed              pPacketManagerLocal->packetBufferUsed
#define LOCAL_packetBufferPreparing         pPacketManagerLocal->packetBufferPreparing
#define LOCAL_lastFunctionCode              pPacketManagerLocal->lastFunctionCode
#define LOCAL_SwitchErrorCount              pPacketManagerLocal->SwitchErrorCount
#define LOCAL_TransmitterBusy               pPacketManagerLocal->TransmitterBusy
#define LOCAL_ReceiverWaiting               pPacketManagerLocal->ReceiverWaiting
#define LOCAL_RxByte                        pPacketManagerLocal->RxByte
#define LOCAL_LowLevelHMIFitted             pPacketManagerLocal->LowLevelHMIFitted
#define LOCAL_prvHMIcanAccessIO             pPacketManagerLocal->prvHMIcanAccessIO
#define LOCAL_newCommsMode                  pPacketManagerLocal->newCommsMode
#define LOCAL_powerSwitchTimer              pPacketManagerLocal->powerSwitchTimer
#define LOCAL_RxCount                       pPacketManagerLocal->RxCount
#define LOCAL_DisplayDefaultContrast        pPacketManagerLocal->DisplayDefaultContrast
#define LOCAL_Asian                         pPacketManagerLocal->Asian
#define LOCAL_PrvAsian                      pPacketManagerLocal->PrvAsian
#define LOCAL_AsianCharacterSet             pPacketManagerLocal->AsianCharacterSet
#define LOCAL_flashState                    pPacketManagerLocal->flashState
#define LOCAL_last_contrast                 pPacketManagerLocal->last_contrast
#define LOCAL_last_displayMode              pPacketManagerLocal->last_displayMode
#define LOCAL_DisplayHardwareRevision       pPacketManagerLocal->DisplayHardwareRevision
#define LOCAL_DisplaySoftwareRevision       pPacketManagerLocal->DisplaySoftwareRevision
#define LOCAL_overrangeValue_EditNotAllowed pPacketManagerLocal->overrangeValue_EditNotAllowed
#define LOCAL_powerUpState                  pPacketManagerLocal->powerUpState
#define LOCAL_pCurrDisplayFrame             pPacketManagerLocal->pCurrDisplayFrame
#define LOCAL_CommunicationsErrorCode       pPacketManagerLocal->CommunicationsErrorCode
#define LOCAL_prevErrorAction               pPacketManagerLocal->prevErrorAction
#define LOCAL_iconStage                     pPacketManagerLocal->iconStage
#define LOCAL_barPosition                   pPacketManagerLocal->barPosition
#define LOCAL_barPercent                    pPacketManagerLocal->barPercent
#define LOCAL_packetStage                   pPacketManagerLocal->packetStage
#define LOCAL_packet_HL                     pPacketManagerLocal->packet_HL
#define LOCAL_packetBuffer                  pPacketManagerLocal->packetBuffer
#define LOCAL_sliderBarHigh                 pPacketManagerLocal->sliderBarHigh
#define LOCAL_sliderBarActual               pPacketManagerLocal->sliderBarActual
#define LOCAL_highlightCursor               pPacketManagerLocal->highlightCursor
#define LOCAL_TxLength                      pPacketManagerLocal->TxLength
#define LOCAL_TxCount                       pPacketManagerLocal->TxCount
#define LOCAL_stringtable_text_id           pPacketManagerLocal->stringtable_text_id
#define LOCAL_updateActiveArea              pPacketManagerLocal->updateActiveArea
#define LOCAL_updateStaticArea              pPacketManagerLocal->updateStaticArea
#define LOCAL_keypressProcessed             pPacketManagerLocal->keypressProcessed
#define LOCAL_SwitchPressedCode             pPacketManagerLocal->SwitchPressedCode
#define LOCAL_SimpleIndicatorSwitchState    pPacketManagerLocal->SimpleIndicatorSwitchState
#define LOCAL_prvBlankDisplay               pPacketManagerLocal->prvBlankDisplay
#define LOCAL_last_displayStyle             pPacketManagerLocal->last_displayStyle
#define LOCAL_last_bargraphStyle            pPacketManagerLocal->last_bargraphStyle
#define LOCAL_displayStyle                  pPacketManagerLocal->displayStyle
#define LOCAL_bargraphStyle                 pPacketManagerLocal->bargraphStyle
#define LOCAL_enumCursor                    pPacketManagerLocal->enumCursor
#define LOCAL_displayAutoscrollIcon         pPacketManagerLocal->displayAutoscrollIcon
#define LOCAL_Field_3_Icon                  pPacketManagerLocal->Field_3_Icon
#define LOCAL_Field_5_Icon                  pPacketManagerLocal->Field_5_Icon
#define LOCAL_Field_7_Icon                  pPacketManagerLocal->Field_7_Icon

#ifdef EDP300_SPECIAL_FEATURES_HMI
#define LOCAL_Field_9_Icon                  pPacketManagerLocal->Field_9_Icon
#endif

#define LOCAL_displayWriteProtectIcon       pPacketManagerLocal->displayWriteProtectIcon

#ifdef HMI_USES_COMMONDIAGNOSTICS
#define LOCAL_prvAlarmActive                pPacketManagerLocal->prvAlarmActive
#define LOCAL_prvAlarmClassification        pPacketManagerLocal->prvAlarmClassification
#define LOCAL_prvAlarmCategory              pPacketManagerLocal->prvAlarmCategory
#define LOCAL_prvAlarmPriority              pPacketManagerLocal->prvAlarmPriority
#define LOCAL_prvAlarmID                    pPacketManagerLocal->prvAlarmID
#define LOCAL_prvBlankHistory               pPacketManagerLocal->prvBlankHistory
#define LOCAL_infoMessageID                 pPacketManagerLocal->infoMessageID
#define LOCAL_infoMessageTimer              pPacketManagerLocal->infoMessageTimer
#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
#define LOCAL_autoExit                      pPacketManagerLocal->autoExit
#endif
#define LOCAL_COMMS_V2_INIT_reqd            pPacketManagerLocal->COMMS_V2_INIT_reqd
#define LOCAL_LOW_LEVEL_INFO_reqd           pPacketManagerLocal->LOW_LEVEL_INFO_reqd

#if defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_AUTO_CALIBRATION) || defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)
#define LOCAL_prvCalibrationStatus          pPacketManagerLocal->prvCalibrationStatus
#endif

#if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
#define LOCAL_AsianBlockVersion             pPacketManagerLocal->AsianBlockVersion
#endif


//    #ifdef HMI_SUPPORTS_GRAPH_VIEW
//#define LOCAL_GraphPointer_Page1            pPacketManagerLocal->GraphPointer_Page1
//#define LOCAL_GraphPointer_Page2            pPacketManagerLocal->GraphPointer_Page2
//#define LOCAL_GraphPointer_Page3            pPacketManagerLocal->GraphPointer_Page3
//#define LOCAL_GraphPointer_Page4            pPacketManagerLocal->GraphPointer_Page4
//    #endif

#endif // #ifndef _PACKETMANAGERLOCAL_DATA_H
