//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description                    PacketManager of the HMI subsystem.
                                                Is responsible for managing the packetTable by adding command packets and retrieveing reply packets

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

/****** PacketManager.h ******/

#ifndef _PACKETMANAGER_H
#define _PACKETMANAGER_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _IMPLEMENTATIONDEFINITIONS
    #error implementation.h must be included
#endif

#define WRITE_STRING                            1
#define WRITE_ARRAY                             2
#define SETUP_DISPLAY                           3
#define BLANK_AREA                              4
#define READ_SWITCHES                           5
#define SET_CONTRAST                            6
#define WRITE_PRE_DEFINED_STRING                7
#define WRITE_SLIDER_BAR                        8
#define WRITE_FRAME_TITLE                       9
#define DRAW_AND_REPEAT_CUSTOM_CHAR             10
#define WRITE_ICON                              11
#define WRITE_STRING_ASIAN                      12
#define WRITE_FRAME_TITLE_ASIAN                 13
#define COMMS_V2_INIT                           14
#define LOW_LEVEL_INFO				            15
#define ROTATE_DISPLAY                          16

typedef enum _E_ADV_DEV_TYPE
{
    DTYPE_UNDEFINED,
    DTYPE_TTX300 = 1,
    DTYPE_TTX200,
    DTYPE_TSP341_N,
    DTYPE_TTX300_W,
    DTYPE_2600T_266xxx,
    DTYPE_Pxx100,
    DTYPE_2600T_266xxx_W,
    DTYPE_TZIDC_1xx,
    DTYPE_TZIDC_2xx,
    DTYPE_EDP300,
    DTYPE_FEX300 = 11,
    DTYPE_FEX100,
    DTYPE_FEW400,
    DTYPE_FEW500,
    DTYPE_FCXxxx,
    DTYPE_FCX4xx,
    DTYPE_FSV4x0,
    DTYPE_FAM3200,
    DTYPE_FAM540,
    DTYPE_FMTx00,
    DTYPE_FSS40 = 21,
    DTYPE_LMTxxx,
    DTYPE_LLTxxx,
    DTYPE_LWTxxx,
    DTYPE_LST300,
    DTYPE_AWT210,
    DTYPE_APA592,
    DTYPE_ACA592,
    DTYPE_AWT440,
    DTYPE_AZ20_30,
    DTYPE_AWT420 = 31,
    DTYPE_266_MV,
    DTYPE_JDF300,
    DTYPE_JDF200
        
}E_ADV_DEV_TYPE;


typedef enum _E_STRING_LIB_TYPE_CUS
{
    CUS_STRING_LIB_GENERAL = 1,
    CUS_STRING_LIB_HART_LMT = 2,
    CUS_STRING_LIB_HART_VT5,
    CUS_STRING_LIB_HART_PRESURE = 4,
    CUS_STRING_LIB_MAX = 4,
    CUS_STRING_LIB_NOT_USED = 0xFF
}E_STRING_LIB_TYPE_CUS;


typedef enum _E_SHMI_DEV_TYPE_CUS
{    
    CUS_DT_HART_LMT = 2,
    CUS_DT_HART_VT5 = 3,
    CUS_DT_HART_PRESURE = 4
}E_SHMI_DEV_TYPE_CUS;

#define SETUP_RESET_DISPLAY             0x80
#define SETUP_DEFAULT_CONTRAST          0x40
#define SETUP_TEST_DISPLAY              0x20
#define SETUP_ALL_PIXELS_ON             0x10
#define SETUP_BACKLIGHT_ON              0x08
#define SETUP_POWER_SAVE                0x04
#define SETUP_DISPLAY_ON                0x02
#define SETUP_INVERT_DISPLAY            0x01

#define PROCESSING_ERROR                0x08
#define HW_SWITCH_ERROR                 0x04
#define HW_DISPLAY_ERROR                0x02
#define COMMUNICATION_ERROR             0x01

#define ASIAN_BLANK_MSBYTE              0x01
#define ASIAN_BLANK_LSBYTE              0x01

#if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
#define ASIAN_SOFTKEY_LINE              7
#else
#define ASIAN_SOFTKEY_LINE              6
#endif

// Pre-defined strings (definitions used for Write Pre-defined String commands)
#define PDS_NEXT                        0
#define PDS_EXIT                        1
#define PDS_SELECT                      2
#define PDS_BACK                        3
#define PDS_EDIT                        4
#define PDS_OK                          5
#define PDS_CANCEL                      6
#define PDS_CANCEL_HIGHLIGHT            7

#define INVALID_PDS                     255

// line definitions (definitions used for Write Pre-defined String commands)
#define BOTTOM_LINE                             0x00
#define TOP_LINE                                0x10

// text justification (definitions used for Write String command)  |European |  Asian  |
#define JUSTIFY_LEFT                            0x00            // |   Yes   |    Yes  |
#define JUSTIFY_CENTRE                          0x01            // |   Yes   |    Yes  |
#define JUSTIFY_RIGHT                           0x02            // |   Yes   |    Yes  |
#define JUSTIFY_RIGHT_WITH_INDENT_1             0x03            // |   Yes   |    Yes  |
#define JUSTIFY_RIGHT_WITH_INDENT_2             0x04            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_3             0x05            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_4             0x06            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_5             0x07            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_6             0x08            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_7             0x09            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_8             0x0A            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_9             0x0B            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_1              0x0C            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_HALF          0x0D            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_2              0x0E            // |   Yes   |    Yes  |
#define JUSTIFY_LEFT_WITH_INDENT_1_PIXEL        0x0F            // |   Yes   |    Yes  |
#define JUSTIFY_LEFT_WITH_INDENT_19_PIXEL       0x10            // |   Yes   |    Yes  |
#define JUSTIFY_LEFT_WITH_INDENT_30_PIXEL       0x11            // |   Yes   |    Yes  |
#define JUSTIFY_LEFT_WITH_INDENT_12             0x12            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_31_PIXEL       0x13            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_5              0x14            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_6              0x15            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_7              0x16            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_4              0x17            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_9              0x18            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_10             0x19            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_11             0x1A            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_13             0x1B            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_14             0x1C            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_3              0x1D            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_17             0x1E            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_18             0x1F            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_63_PIXEL       0x20            // |   Yes   |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_25_PIXEL       0x21            // |   Yes   |    No   |
#define JUSTIFY_RIGHT_WITH_INDENT_49_PIXEL      0x22            // |   No    |    Yes  |
#define JUSTIFY_LEFT_WITH_INDENT_78_PIXEL       0x23            // |   Yes   |    No   |

#define JUSTIFY_FOR_MANUAL_ADJ_0                0x24            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_1                0x25            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_2                0x26            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_3                0x27            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_4                0x28            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_5                0x29            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_6                0x2A            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_7                0x2B            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_8                0x2C            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_9                0x2D            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_10               0x2E            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_11               0x2F            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_12               0x30            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_13               0x31            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_14               0x32            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_15               0x33            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_16               0x34            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_17               0x35            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_18               0x36            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_19               0x37            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_20               0x38            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_21               0x39            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_22               0x3A            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_23               0x3B            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_24               0x3C            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_25               0x3D            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_26               0x3E            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_27               0x3F            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_28               0x40            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_29               0x41            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_30               0x42            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_31               0x43            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_32               0x44            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_33               0x45            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_34               0x46            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_35               0x47            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_36               0x48            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_37               0x49            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_38               0x4A            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_39               0x4B            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_40               0x4C            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_41               0x4D            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_42               0x4E            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_43               0x4F            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_44               0x50            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_45               0x51            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_46               0x52            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_47               0x53            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_48               0x54            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_49               0x55            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_50               0x56            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_51               0x57            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_52               0x58            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_53               0x59            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_54               0x5A            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_55               0x5B            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_56               0x5C            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_57               0x5D            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_58               0x5E            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_59               0x5F            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_60               0x60            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_61               0x61            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_62               0x62            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_63               0x63            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_64               0x64            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_65               0x65            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_66               0x66            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_67               0x67            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_68               0x68            // |   Yes   |    No   |
#define JUSTIFY_FOR_MANUAL_ADJ_69               0x69            // |   Yes   |    No   |

#define JUSTIFY_LEFT_WITH_INDENT_6_PIXEL        0x6A            // |    No   |    Yes* | *Only on Type 4 & Type 5 implementations
#define JUSTIFY_LEFT_WITH_INDENT_12_PIXEL       0x6B            // |    No   |    Yes* | *Only on Type 4 & Type 5 implementations
#define JUSTIFY_LEFT_WITH_INDENT_35_PIXEL       0x6C            // |    Yes* |    No   | *Only on Type 4 & Type 5 implementations

#define JUSTIFY_LEFT_WITH_INDENT_20_PIXEL       0x6D            // |    Yes  |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_79_PIXEL       0x6E            // |    Yes  |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_98_PIXEL       0x6F            // |    Yes  |    No   |
#define JUSTIFY_LEFT_WITH_INDENT_14_PIXEL       0x70            // |    Yes  |    No   |

#define JUSTIFY_AS_TITLE                        0x03            // |   Yes   |    No   |


#ifdef HMI_SUPPORTS_ASIAN
#define JUSTIFY_AS_SECONDARY_IDENTIFER          LOCAL_Asian ? JUSTIFY_LEFT_WITH_INDENT_63_PIXEL : JUSTIFY_CENTRE
#else
#define JUSTIFY_AS_SECONDARY_IDENTIFER          JUSTIFY_CENTRE
#endif

#ifdef HMI_SUPPORTS_ASIAN
#ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER
#define MAX_ASIAN_TITLE_LEN                     SecondaryIdentifierActive ? 5 : 8
#else
#define MAX_ASIAN_TITLE_LEN                     8
#endif
#endif

#define SIGNAL_ID_BUFFER_SIZE                   11
#define DIAG_CATEGORY_BUFFER_SIZE               19 //15
#define FULLWIDTH_TEXT_BUFFER_SIZE              21

#define ADJUST_BAR_MAX  70

//font sizes (definitions used for Write String command)
#define FONT_0  0x00            // 0 =  5x7      6x8
#define FONT_1  0x08            // 1 =  5x14     6x16
#define FONT_2  0x10            // 2 = 10x14    12x16
#define FONT_3  0x18            // 3 = 15x21    18x24
#define FONT_4  0x20            // 4 = 20x28    24x32
#define FONT_5  0x28            // 5 = 25x35    30x40

//line/string inversion (definitions used for Write String command)
#define NO_INVERT                       0x00
#define STRING_INVERT                   0x40
//#define LINE_INVERT                     0x80
#define CHAR_INVERT                     0xC0


#define PRE_BLANK                       0x80

#define WRITE_STRING_STRING_START_POSITION          4
#define WRITE_STRING_N_STRING_FORMAT_CHARS          2

#define WRITE_STRING_EXTENDED_STRING_START_POSITION 5
#define WRITE_STRING_EXTENDED_N_STRING_FORMAT_CHARS 3

#define WRITE_TITLE_STRING_START_POSITION           2
#define WRITE_TITLE_N_STRING_FORMAT_CHARS           0

#define WRITE_TITLE_EXTENDED_STRING_START_POSITION  3
#define WRITE_TITLE_EXTENDED_N_STRING_FORMAT_CHARS  1

//#ifdef LOW_LEVEL_USES_EXTENDED_CHARACTER_SET
//#define STRING_START_POSITION_LOCAL_HMI     5
//#define N_STRING_FORMAT_CHARS_LOCAL_HMI     3
//#else
//#define STRING_START_POSITION_LOCAL_HMI     4
//#define N_STRING_FORMAT_CHARS_LOCAL_HMI     2
//#endif


//#ifdef HMI_SUPPORTS_SERVICEPORT
//#define STRING_START_POSITION_REMOTE_HMI    4
//#define N_STRING_FORMAT_CHARS_REMOTE_HMI    2
//#endif

// Error action - following a reply from low level HMI
#define LOW_LEVEL_ERROR_NOT_PRESENT             0
#define LOW_LEVEL_ERROR_IGNORED                 1
#define LOW_LEVEL_ERROR_RESPONSE_BUILT          2
#define LOW_LEVEL_ERROR_REBUILD_FRAME           3

#define _1ST_LINE        0
#define _2ND_LINE        1
#define _3RD_LINE        2
#define _BAR_LINE        3

#define DISPLAY_ERROR_WARNING   0
#define DISPLAY_WRITE_PROTECT   1
#define DISPLAY_BLANK           2
#define DISPLAY_AUTOSCROLL      3
#define DISPLAY_MANUALSCROLL    4

#ifdef DISPLAY_SIZE_E

// Icon array for controller display in portrait mode
//
//    |   ICONFIELD_1      |    ICONFIELD_2     |   ICONFIELD_3      |    ICONFIELD_4     |    ICONFIELD_5     |    ICONFIELD_6     |    ICONFIELD_7     |    ICONFIELD_8     |
//    |  Autoscroll Icon   |       Blank        |   Control Icon     |       Blank        | write protect icon |       Blank        |     Mode Icon      |      Blank         |
//    |<---- 7 pixels ---->|<---  7 pixels ---->|<--- 24 pixels ---->|<---- 5 pixels ---->|<--- 10 pixels ---->|<---- 2 pixels ---->|<--- 24 pixels ---->|<--- 15 pixels ---->|
//    |                    |                    |                    |                    |                    |                    |                    |                    |
//    |[12]            [18]|[19]            [25]|[26]            [49]|[50]            [54]|[55]            [64]|[65]            [66]|[67]            [90]|[91]           [105]|
//    |                    |                    |                    |                    |                    |                    |                    |                    |
//    |                                                                                                                                                                       |
//    |<------------------------------------------------------------------------------ 94 pixels ---------------------------------------------------------------------------->|
//    |                                                                                                                                                                       |

#define ICONFIELD_1_START   12
#define ICONFIELD_2_START   19
#define ICONFIELD_3_START   26
#define ICONFIELD_4_START   50
#define ICONFIELD_5_START   55
#define ICONFIELD_6_START   65
#define ICONFIELD_7_START   67
#define ICONFIELD_8_START   90
#define ICONFIELD_END       106


#else

// Icon array for all displays apart from controller display in portrait mode
//
//    |   ICONFIELD_1      |    ICONFIELD_2     |   ICONFIELD_3      |    ICONFIELD_4     |    ICONFIELD_5     |    ICONFIELD_6     |    ICONFIELD_7     |    ICONFIELD_8     |
//    |  Autoscroll Icon   |       Blank        |   Control Icon     |       Blank        | write protect icon |       Blank        |     Mode Icon      |      Blank         |
//    |<---- 7 pixels ---->|<--- 11 pixels ---->|<--- 24 pixels ---->|<---- 5 pixels ---->|<--- 10 pixels ---->|<---- 2 pixels ---->|<--- 24 pixels ---->|<--- 19 pixels ---->|
//    |                    |                    |                    |                    |                    |                    |                    |                    |
//    |[12]            [18]|[19]            [29]|[30]            [53]|[54]            [58]|[59]            [68]|[69]            [70]|[71]            [94]|[95]           [113]|
//    |                    |                    |                    |                    |                    |                    |                    |                    |
//    |                                                                                                                                                                       |
//    |<----------------------------------------------------------------------------- 102 pixels ---------------------------------------------------------------------------->|
//    |                                                                                                                                                                       |

#define ICONFIELD_1_START   12
#define ICONFIELD_2_START   19
#define ICONFIELD_3_START   30
#define ICONFIELD_4_START   54
#define ICONFIELD_5_START   59
#define ICONFIELD_6_START   69
#define ICONFIELD_7_START   71
#define ICONFIELD_8_START   95
#define ICONFIELD_END       114

#endif

#if (defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)) && !defined(DISABLE_OP_PAGE_ID)

    #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #define nVIEW_ICONS     7
    #else
        #define nVIEW_ICONS     5
    #endif
#else
        #define nVIEW_ICONS     1
#endif


#if defined(COMMON_TOP_WORKS_NEW_FEATURES) && defined(TYPE_1_IMPLEMENTATION)

    #define ROTATION_0_COLUMN_MIRROR            0xA0    // SET_COLUMN_MIRROR_OFF
    #define ROTATION_0_COMMMON_OP_MODE_SEL      0xC0    // COMMMON_OP_MODE_SEL_LOW

    #define ROTATION_180_COLUMN_MIRROR          0xA1    // SET_COLUMN_MIRROR_ON
    #define ROTATION_180_COMMMON_OP_MODE_SEL    0xC8    // COMMMON_OP_MODE_SEL_HIGH_COPY

    #define ROTATION_0_COLUMN_OFFSET            0
    #define ROTATION_180_COLUMN_OFFSET          4

#endif // end of #if defined(COMMON_TOP_WORKS_NEW_FEATURES) && defined(TYPE_1_IMPLEMENTATION)


#ifdef HMI_SUPPORTS_SERVICEPORT
extern T_DATA_SEMAPHORE  Sema_HMI_REMOTE_INSTANCE;
#endif

//#define SUPPORT_OLD_HMI 1
//#define SUPPORT_SHMI  1  
//For Smart HMI
extern TBOOL gHotHMI_ReinitializeWaveformFlag;
extern const TUSIGN8* operatorPageData[6][5];
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
extern const TUSIGN8 SecurityIcon[12];
#endif
extern TUSIGN8 PacketManagerBuildImage_SHMI(TUSIGN8 t_updateActiveArea, TUSIGN8 t_keypressProcessed);
extern TUSIGN8 BuildReadSwitches_SHMI(TUSIGN8 lastPacket);
extern TUSIGN8 BuildV2Init_s(TUSIGN8 contrast, TUSIGN8 lastPacket);
extern TUSIGN8 BuildLowLevelInfo_s(TUSIGN8 lastPacket);

/*!
 \brief         Builds Packets required to display an image
 \author        Roger Arnold
 \param         updateActiveArea - identifies that a display timeout has occurred
                        keypressProcessed - identifies that a key has been processed
 \return        the total number of bytes in packet buffer (built and ready to send to the low level HMI)
 \test          No details available
*/
TUSIGN8 PacketManagerBuildImage_HMI(TUSIGN8 t_updateActiveArea, TUSIGN8 t_keypressProcessed);

/*!
 \brief         Informs that the currently displayed numeric value is overrange or underrange and therefore not being displayed properly
 \author        Roger Arnold
 \param
 \return        eFALSE - value OK , eTRUE value is overrange or underrange
 \test          No details available
*/
TBOOL PacketManagerIsNumericValueOverrange(void);

/*!
 \brief         Gets Engineering units
 \author        Roger Arnold
 \param         T_SUBSYSTEM_IDX subsysIdx - Subsystem index of the engineering unit code
                TUSIGN8 objIdx            - Object index of the engineering unit code
                TUSIGN8 attribIdx         - Attribute index of the engineering unit code
                TUSIGN8 type              - Defines the type of engineering unit
 \return        TUSIGN8 const *           - Address of the engineering unit string
 \test          No details available
*/
TUSIGN8 const *PacketManagerGetEngUnits(T_SUBSYSTEM_IDX subsysIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx, TUSIGN8 type);

#ifdef HMI_SUPPORTS_CUSTOM_UNITS
void PacketManagerRemoveCustomfromUnits(TUSIGN8 *txt);
#endif

/*!
 \brief         Reads most recent error codes from HMI Interface class and prepares PacketManagerBuildImage() to react to those errors
 \author        Roger Arnold
 \param
 \return        0 no errors were present , 1 errors were present
 \test          No details available
*/
TUSIGN8 PacketManagerHandleErrors(TBOOL isSmartHMI);

/*!
 \brief         Reads and returns the most recent switch codes from HMI Interface class
 \author        Roger Arnold
 \param
 \return        most recent switch codes
 \test          No details available
*/
TUSIGN8 PacketManagerGetKeystate(TUSIGN8 instance);

/*!
 \brief
 \author        Roger Arnold
 \param
 \return
 \test          No details available
*/
TUSIGN8 PacketManagerIsWaitingForReply(void);

/*!
 \brief
 \author        Roger Arnold
 \param
 \return
 \test          No details available
*/
TBOOL PacketManagerProcessWaitForReply(TUSIGN8 instance, TUSIGN8 *pTimer, TUSIGN8 *pTimeoutCount);

/*!
 \brief
 \author        Roger Arnold
 \param
 \return
 \test          No details available
*/
void PacketManagerForceReply(void);

/*!
 \brief
 \author        Roger Arnold
 \param
 \return
 \test          No details available
*/
TBOOL PacketManagerIsLowLevelActive(TUSIGN8 instance);

/*!
 \brief
 \author        Roger Arnold
 \param
 \return
 \test          No details available
*/
void PacketManagerManageLowLevelPower(TBOOL, TUSIGN8, TBOOL);


TUSIGN8 PacketManagerGetDefaultContrast(void);

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)

void PacketManagerSendDataToLowLevel(TUSIGN8 bytesToSend);
void PacketManagerReceiveDataFromLowLevel(void);
void PacketManagerExecuteLowLevel(void);

#else
/*!
 \brief         Ititiates SCI transmission of display packet data to the low level HMI
 \author        Roger Arnold
 \param         the number of bytes to send
 \return
 \test          No details available
*/
void PacketManagerStartTransmission(TUSIGN8 bytesToSend);

#endif

void PacketManagerTransmitInterrupt(void);
void PacketManagerReceiveInterupt(void);


#ifdef HMI_SUPPORTS_SERVICEPORT

void PacketManagerCopyServicePortData(TBOOL toLocal);
TBOOL PacketManagerIsRemoteInstance(void);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the Packet Manager local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void PacketManagerSetLocalDataPointer(TUSIGN8 newInstance);


/*----------------------------------------------------------------------------------------------------------!
 \brief         Registers the HMI with the service port
 \author        Roger Arnold
 \param         none
 \return        TBOOL - eTRUE = registration completed OK, eFALSE = registration failed
 \test          No details available
*/
TBOOL PacketManager_SP_RegisterClientEXE(void);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Starts transmission of HMI packets via the service port
 \author        Roger Arnold
 \param         TUSIGN8 bytesToSend -
 \return        none
 \test          No details available
*/
void PacketManagerStartTransmission_SP(TUSIGN8 bytesToSend);

#endif


#if defined (HMI_MODULE_TEST) && (defined(HMI_MODULE_TEST_PACKETMANAGER_1) || defined(HMI_MODULE_TEST_PACKETMANAGER_2) || defined(HMI_MODULE_TEST_PACKETMANAGER_3))
TUSIGN8 PacketManager_ModuleTest(void);
#endif


#if defined(HMI_SUPPORTS_MENU_FRAMES) && ( defined(HMI_USES_COMMONDIAGNOSTICS) || defined(AZ200_SPECIAL_FEATURES))
TUSIGN8 PacketManagerConvertValue(TUSIGN16 value, TUSIGN8 *pText, TBOOL min2);
#endif

#ifdef HMI_USES_COMMONDIAGNOSTICS
TUSIGN8 GetInfoMessageID(TBOOL OverrideTimer);
void UpdateInfoMessageTimer(void);
#endif

TUSIGN8 PacketManagerGetLLHardwareType(TUSIGN8 instance);

void PacketManagerSetOperatorIcon(TUSIGN8 field, TUSIGN8 iconID);
void PacketManagerSetOperatorInfo(TUSIGN8 infoID);

TUSIGN8 BuildReadSwitches(TUSIGN8 lastPacket);

#ifdef HMI_SUPPORTS_ASIAN
TUSIGN8 BuildFrameTitle_w(const TUSIGN8 *titleText, TUSIGN8 maxLength, TUSIGN8 lastPacket);
TUSIGN8 BuildWriteString_w(const TUSIGN8 *text, TUSIGN8 line, TUSIGN8 justify, TUSIGN8 invert, TUSIGN8 blank, TUSIGN8 minLength, TUSIGN8 maxLength, TUSIGN8 lastPacket);
#endif

TUSIGN8 BuildBlankArea(TUSIGN8 start, TUSIGN8 end, TUSIGN8 lastPacket);
TUSIGN8 BuildFrameTitle(const TUSIGN8 *titleText, TUSIGN8 lastPacket);
TUSIGN8 BuildSliderBar(TINT16 high, TINT16 actual, TUSIGN8 lastPacket);
TUSIGN8 BuildWriteIcon(TUSIGN8 iconID, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 lastPacket);
TUSIGN8 BuildWriteArray(const TUSIGN8 *array, TUSIGN8 arraySize, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 lastPacket);
TUSIGN8 BuildCustomCharacterString(const TUSIGN8 *pCustomCharacter, TUSIGN8 nChars, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 nRepeats, TUSIGN8 lastPacket);
TUSIGN8 BuildSetContrast(TUSIGN8 contrast, TUSIGN8 lastPacket);
TUSIGN8 BuildSetupDisplay(TUSIGN8 f, TUSIGN8 lastPacket);
TUSIGN8 BuildWriteString(const TUSIGN8 *suppliedText, TUSIGN8 extendedCharacterPage, TUSIGN8 line, TUSIGN8 justify, TUSIGN8 font, TUSIGN8 invert, TUSIGN8 invchar, TUSIGN8 blank, TUSIGN8 minLength, TUSIGN8 lastPacket);

#if defined(HMI_SUPPORTS_SERVICEPORT) || defined(TYPE_1_IMPLEMENTATION)
void convertExtendedCharacterSet(TUSIGN8 *text, TUSIGN8 extendedCharacterPage, TUSIGN8 len);
#endif

void LimitTextLength(TUSIGN8 *txt, TUSIGN8 maxlen);

//#ifdef HMI_SUPPORTS_GRAPH_VIEW
//void PacketManagerSetGraphPointer(TUSIGN8 frameNumber, TUSIGN8* pGraph);
//#endif

void PacketManagerToggleBacklight(void);
TBOOL PacketManagerOtherSubsystemPutPacketBuffer(T_SUBSYSTEM_IDX systemIdx, TUSIGN8 processCmd, TUSIGN8 *ptrArray, TUSIGN8 arrayLength);
void PacketManagerReInitialiseLowLevelDetection(void);

#ifdef TYPE_1_IMPLEMENTATION
TBOOL Type_1_HardwareSupportsExtendedCharacterSets(void);
TBOOL Type_1_SoftwareSupportsExtendedWriteString(void);
#endif

#if defined(SUPPORT_SHMI)
extern TUSIGN8 AddPacketToBuffer(TUSIGN8 bytesToAdd, TUSIGN8 lastPacket);
extern void PacketManagerReadyToTransmission(void);
#endif

#endif
