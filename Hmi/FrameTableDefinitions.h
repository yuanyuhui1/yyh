//FrameTableDefinitions.h
//
#ifndef _FRAMETABLEDEFINITIONS_H
#define _FRAMETABLEDEFINITIONS_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _COMPILETIMEDEFINITIONS
  #error compileTimeDefinitions.h must be included
#endif

#define SHMI_MAJOR_VER                         10    //xx, 10 <= x  <= 99, shall be equal to SHMI unit's 
#define SHMI_MINOR_VER                         3     //YY
#define SHMI_DEBUG_VER                         0     //ZZ


#define HIGH_LEVEL_VERSION_INFO                21100
#define HIGH_LEVEL_VERSION_INFO_TUSIGN16       (TUSIGN16)HIGH_LEVEL_VERSION_INFO

// this is in the format                    XYYZZ  which is derived from HMI/200X.YY.ZZ
//                                                                           |  |  |  |
//                    where:                                                 |  |  |  |
//                    (200) HMI software series _____________________________|  |  |  |
//                                                                              |  |  |
//                      (x) HMI Version ________________________________________|  |  |
//                                                                                 |  |
//                     (yy) Major Release Number___________________________________|  |
//                          incremented when a major new feature is introduced        |
//                                                                                    |
//                     (zz) Minor Release Number _____________________________________|
//                          incremented when (a) a minor new feature is introduced
//                                        or (b) a BugFix is added
//                          cleared when a major new feature is introduced
//
// NOTE: See also comment at top of HmiCoordinator.c
//

typedef struct
{
    TUSIGN16 year;
    TUSIGN8  month;
    TUSIGN8  date;
    TUSIGN8  hour;
    TUSIGN8  minute;
    TUSIGN8  second;
} TIMEANDDATE;

#define NUMBER_OF_FIELDS_IP_ADDRESS 4
typedef union
{
    TUSIGN32    networkAddress;
    TUSIGN8     dotted_IP[NUMBER_OF_FIELDS_IP_ADDRESS];
}IP_ADDRESS_HMI;

typedef union
{
    TUSIGN8                 data_8;
    TFLOAT                  data_F;
#ifdef HMI_USES_DOUBLE
    TDOUBLE                 data_D;
#endif
#ifdef HMI_SUPPORTS_16BIT_ENUMS
    TUSIGN16                data_16;
#endif
#ifdef HMI_SUPPORTS_INTEGER_FRAMES
    TUSIGN32                data_32;
#endif
}ADJUST_DATA;

typedef enum
{
/*  0 */  FRAME_TYPE_OPERATOR
/*  1 */ ,FRAME_TYPE_CONFIG_HEADER
/*  2 */ ,FRAME_TYPE_MENU
/*  3 */ ,FRAME_TYPE_NUMERICAL_PARAM
/*  4 */ ,FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT
/*  5 */ ,FRAME_TYPE_ENUMERATED_PARAM
/*  6 */ ,FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT
/*  7 */ ,FRAME_TYPE_MANUAL_ADJUST
/*  8 */ ,FRAME_TYPE_AUTO_ADJUST
/*  9 */ ,FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP
/* 10 */ ,FRAME_TYPE_STRING_PARAM
/* 11 */ ,FRAME_TYPE_STRING_PARAM_NO_EDIT
/* 12 */ ,FRAME_TYPE_STRING_PARAM_PACKED
/* 13 */ ,FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT
/* 14 */ ,FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT
/* 15 */ ,FRAME_TYPE_PROGRESS_BAR
/* 16 */ ,FRAME_TYPE_ALARM_HISTORY
/* 17 */ ,FRAME_TYPE_SECURE_ALARM_HISTORY
/* 18 */ ,FRAME_TYPE_TIME_AND_DATE_PARAM
/* 19 */ ,FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT
/* 20 */ ,FRAME_TYPE_MANUAL_CALIBRATION
/* 21 */ ,FRAME_TYPE_AUTO_CALIBRATION
/* 22 */ ,FRAME_TYPE_2_POINT_pH_CALIBRATION
/* 23 */ ,FRAME_TYPE_BIT_ENUMERATED_PARAM
/* 24 */ ,FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT
/* 25 */ ,FRAME_TYPE_INTEGER_PARAM
/* 26 */ ,FRAME_TYPE_INTEGER_PARAM_NO_EDIT
/* 27 */ ,FRAME_TYPE_IP_ADDRESS
/* 28 */ ,FRAME_TYPE_IP_ADDRESS_NO_EDIT
/* 29 */ ,FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT
/* 30 */ ,FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT
/* 31 */ ,FRAME_TYPE_WAVEFORM_FRAME
/* 32 */ ,FRAME_TYPE_MATRIX_T_C_D
/* 33 */ ,FRAME_TYPE_MATRIX_X_Y_XY
/* 34 */ ,FRAME_TYPE_LINEARIZER
}FRAME_TYPE_TYPE;

typedef enum
{
/*  0 */  CFH_INVALID
/*  1 */ ,CFH_EASY_SETUP
/*  2 */ ,CFH_EASY_SETUP_I
/*  3 */ ,CFH_EASY_SETUP_C
/*  4 */ ,CFH_PRESS_DEVICE
/*  5 */ ,CFH_PRESS_DEVICE_I
/*  6 */ ,CFH_PRESS_DEVICE_C
/*  7 */ ,CFH_TEMP_DEVICE
/*  8 */ ,CFH_TEMP_DEVICE_I
/*  9 */ ,CFH_TEMP_DEVICE_C
/* 10 */ ,CFH_FLOW_DEVICE
/* 11 */ ,CFH_FLOW_DEVICE_I
/* 12 */ ,CFH_FLOW_DEVICE_C
/* 13 */ ,CFH_DISPLAY
/* 14 */ ,CFH_DISPLAY_I
/* 15 */ ,CFH_DISPLAY_C
/* 16 */ ,CFH_IO
/* 17 */ ,CFH_IO_I
/* 18 */ ,CFH_IO_C
/* 19 */ ,CFH_PROCESS_ALARM
/* 20 */ ,CFH_PROCESS_ALARM_I
/* 21 */ ,CFH_PROCESS_ALARM_C
/* 22 */ ,CFH_TOTALIZER
/* 23 */ ,CFH_TOTALIZER_I
/* 24 */ ,CFH_TOTALIZER_C
/* 25 */ ,CFH_COMMUNICATION
/* 26 */ ,CFH_COMMUNICATION_I
/* 27 */ ,CFH_COMMUNICATION_C
/* 28 */ ,CFH_CALIBRATE
/* 29 */ ,CFH_CALIBRATE_I
/* 30 */ ,CFH_CALIBRATE_C
/* 31 */ ,CFH_DIAGNOSTICS
/* 32 */ ,CFH_DIAGNOSTICS_I
/* 33 */ ,CFH_DIAGNOSTICS_C
/* 34 */ ,CFH_DEVICE_INFO
/* 35 */ ,CFH_DEVICE_INFO_I
/* 36 */ ,CFH_DEVICE_INFO_C
/* 37 */ ,CFH_SERVICE
/* 38 */ ,CFH_SERVICE_I
/* 39 */ ,CFH_SERVICE_C
/* 40 */ ,CFH_POSN_DEVICE
/* 41 */ ,CFH_POSN_DEVICE_I
/* 42 */ ,CFH_POSN_DEVICE_C
/* 43 */ ,CFH_CONDY_DEVICE
/* 44 */ ,CFH_CONDY_DEVICE_I
/* 45 */ ,CFH_CONDY_DEVICE_C
/* 46 */ ,CFH_PH_DEVICE
/* 47 */ ,CFH_PH_DEVICE_I
/* 48 */ ,CFH_PH_DEVICE_C
/* 49 */ ,CFH_O2_DEVICE
/* 50 */ ,CFH_O2_DEVICE_I
/* 51 */ ,CFH_O2_DEVICE_C
/* 52 */ ,CFH_COe_DEVICE
/* 53 */ ,CFH_COe_DEVICE_I
/* 54 */ ,CFH_COe_DEVICE_C
/* 55 */ ,CFH_FLOW_SENSOR_SETUP
/* 56 */ ,CFH_FLOW_SENSOR_SETUP_I
/* 57 */ ,CFH_FLOW_SENSOR_SETUP_C
/* 58 */ ,CFH_CONTROL
/* 59 */ ,CFH_CONTROL_I
/* 60 */ ,CFH_CONTROL_C
/* 61 */ ,CFH_PROFILE
/* 62 */ ,CFH_PROFILE_I
/* 63 */ ,CFH_PROFILE_C
/* 64 */ ,CFH_CONTROL_DEVICE
/* 65 */ ,CFH_CONTROL_DEVICE_I
/* 66 */ ,CFH_CONTROL_DEVICE_C
/* 67 */ ,CFH_INDICATOR_DEVICE
/* 68 */ ,CFH_INDICATOR_DEVICE_I
/* 69 */ ,CFH_INDICATOR_DEVICE_C
/* 70 */ ,CFH_FUNCTIONS
/* 71 */ ,CFH_FUNCTIONS_I
/* 72 */ ,CFH_FUNCTIONS_C
/* 73 */ ,CFH_TUNE
/* 74 */ ,CFH_TUNE_I
/* 75 */ ,CFH_TUNE_C
/* 76 */ ,CFH_BASIC_SETUP
/* 77 */ ,CFH_LEVEL_DEVICE
/* 78 */ ,CFH_ANALYSER_DEVICE
/* 79 */ ,CFH_ANALYSER_SENSOR
/* 80 */ ,CFH_MEDIA_CARD
}CONFIG_HEADER_TYPE;

#define UOM_NOT_APPLICABLE  0
#define FIXED_TEXT_UNITS    0
#define USE_UOM_1           1
#define USE_UOM_2           2
#define USE_UOM_3           3
#define USE_UOM_4           4
#define USE_UOM_5           5
#define USE_UOM_6           6
#define USE_UOM_7           7
#define USE_UOM_8           8
#define USE_UOM_9           9
#define USE_UOM_10          10
#define USE_UOM_11          11
#define USE_NEXT_ATTRIBUTE  12

typedef struct
{
        TUSIGN8     condition;
#ifdef COMMON_TOP_WORKS_NEW_FEATURES    //DIAGNOSTIC_SPLIT_DESC_AND_RECOVERY
        STR_REF     descriptionID;
        STR_REF     RecoveryActID;
#else
        STR_REF     stringID;
#endif
}DIAGNOSTICS_TABLE_ENTRY_TYPE;

typedef struct
{
        TUSIGN8                        ssIdx;                  // identifies the sub-system that "owns" the text
        TUSIGN8                        objIdx;                 // identifies the object that "owns" the text
        STR_REF                        attribIdx;              // identifies the attribute within the object that "owns" the text
}TEXT_IDENTIFIER_TYPE;


typedef struct
{
        TUSIGN8                        ssIdx;                  // identifies the sub-system that "owns" the data
        TUSIGN8                        objIdx;                 // identifies the object that "owns" the data
        TUSIGN8                        attribIdx;              // identifies the attribute within the object that "owns" the data
}ATTRIB_IDENTIFIER_TYPE;


#define MAX_RESOLUTION      0xFF
#define USE_RANGE           0xFE

#define AUTO_RANGING        MAX_RESOLUTION
#define USE_BARGRAPH_RANGE  USE_RANGE

#define CONFIGURABLE_DP_1   (TUSIGN8)201
#define CONFIGURABLE_DP_2   (TUSIGN8)202
#define CONFIGURABLE_DP_3   (TUSIGN8)203
#define CONFIGURABLE_DP_4   (TUSIGN8)204
#define CONFIGURABLE_DP_5   (TUSIGN8)205
#define CONFIGURABLE_DP_6   (TUSIGN8)206
#define CONFIGURABLE_DP_7   (TUSIGN8)207
#define CONFIGURABLE_DP_8   (TUSIGN8)208
#define CONFIGURABLE_DP_9   (TUSIGN8)209
#define CONFIGURABLE_DP_10  (TUSIGN8)210
#define CONFIGURABLE_DP_11  (TUSIGN8)211

#define MAX_STRING_PARAMETER_LENGTH 40

typedef struct
{
    TUSIGN8                 maxCharacters;                  // maximum no of characters allowed for number (including dec point)
    TUSIGN8                 characterSet_dpPosn;            // for string variables: defines character set, for numeric variables: defines display to max resolution(0xFF) or display number with fixed no of dp's (0 to 5)
}NUMBER_ATTRIBUTES_TYPE;

#define CHARACTER_SET_LATIN1                12
#define CHARACTER_SET_STANDARD_AND_TURKISH  11
#define CHARACTER_SET_TURKISH               10
#define CHARACTER_SET_STANDARD_AND_KATAKANA 9
#define CHARACTER_SET_KATAKANA              8
#define CHARACTER_SET_STANDARD_AND_RUSSIAN  7
#define CHARACTER_SET_RUSSIAN               6
#define CHARACTER_SET_PASSWORD              5
#define CHARACTER_SET_NUMERIC               4
#define CHARACTER_SET_FILENAME              3
#define CHARACTER_SET_EXTENDED              2
#define CHARACTER_SET_STANDARD              1
#define CHARACTER_SET_PACKED                0

// most significant 3 bits used for extended character Page
#define HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN      0x00  // 000xxxxx
#define HMI_EXTENDED_CHARACTER_PAGE_KATAKANA      0x20  // 001xxxxx
#define HMI_EXTENDED_CHARACTER_PAGE_TURKISH       0x40  // 010xxxxx
#define HMI_EXTENDED_CHARACTER_PAGE_RUSSIAN       0x60  // 011xxxxx
#define HMI_EXTENDED_CHARACTER_PAGE_LATIN1        0x80  // 100xxxxx
#define HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE  0xFF

typedef struct
{
        TUSIGN8                        ssIdx;
        TUSIGN8                        objIdx;
        TUSIGN8                        extendedCharacterPage;
        TUSIGN8                        reverse0;
        TUSIGN8                        reverse1;
        TUSIGN8                        reverse2;
        
}STRING_OBJECT_EXTENSION_TYPE;

typedef struct
{
        TUSIGN8                        ssIdx;
        TUSIGN8                        objIdx;
        TUSIGN8                        attIdx;
        TUSIGN16                       firstItem;
}INDIRECT_TEXT_IDENTIFICATION_TYPE;


typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pNextFrame;                    // address of the frame accessed following "Select" key operation (actually FRAME_FORMAT_TYPE*)
        STR_REF                                 unused_1;
        STR_REF                                 unused_2;
        TUSIGN8                                 frameNumber;
        ATTRIB_IDENTIFIER_TYPE                  Tag;
        TUSIGN8                                 tag_type;
        TUSIGN8                                 tag_maxCharacters;
        TUSIGN8                                 unused_3;
        TUSIGN8                                 unused_4;
        TUSIGN8                                 unused_5;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 unused_spare;
#endif
}OPERATOR_FRAME_TYPE;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pNextFrame;                    // address of the frame accessed following "Select" key operation (actually FRAME_FORMAT_TYPE*)
        STR_REF                                 unused_1;
        STR_REF                                 title_attribidx;
        TUSIGN8                                 validTest;                      // position in validityTestTable of a function that determines if the config level is available
        TUSIGN8                                 icon[6];                        // The (up to) 6 icons displayed in sequence to provide animation
        TUSIGN8                                 unused_2;
        TUSIGN8                                 iconID;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;
#endif
}CONFIG_HEADER_FRAME_TYPE;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pNextFrame;                    // address of the frame accessed following "Select" key operation (actually FRAME_FORMAT_TYPE*)
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 unused_1;
        TUSIGN8                                 firstItem_LSByte;
        TUSIGN8                                 firstItem_MSByte;               // position in menuTable of the first item in the menu
        TUSIGN8                                 lastItem_LSByte;
        TUSIGN8                                 lastItem_MSByte;                // position in menuTable of the last item in the menu
        TUSIGN8                                 unused_2;
        TUSIGN8                                 unused_3;
        TUSIGN8                                 unused_4;
        TUSIGN8                                 unused_5;
        TUSIGN8                                 specialMenuFlag;                // 0-std menu, 1-Operator Menu, 2-Signals view, 3-Diagnostics View, 4-ARM Access level Selection, 5-Alarm History, 6-Secure Alarm History
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 unused_spare;
#endif
}MENU_FRAME_TYPE;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  value;                          // The data to be displayed/adjusted
        ATTRIB_IDENTIFIER_TYPE                  engUnits;                       // engineering units assigned to the value to be displayed
        TUSIGN8                                 engUnitsType;                   //
        NUMBER_ATTRIBUTES_TYPE                  attributes;                     // Defines the value type, how it is stored and how it is to be displayed
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}NUMERICAL_PARAM_FRAME_TYPE;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  value;                          // The data to be displayed
        TUSIGN8                                 unused_1;                       //
        TUSIGN8                                 unused_2;                       //
        TUSIGN8                                 unused_3;                       //
        TUSIGN8                                 unused_4;                       //
        TUSIGN8                                 unused_5;                       //
        TUSIGN8                                 unused_6;                       //
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}GRAPH_PARAM_FRAME_TYPE;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        TUSIGN8                                 waveformSubsystemIdx;           // identifies the sub-system that "owns" the data
        TUSIGN8                                 unused_6;                       //
        TUSIGN8                                 unused_7;                       //
        TUSIGN8                                 waveformIndex;                  //
        TUSIGN8                                 unused_1;                       //
        TUSIGN8                                 unused_2;                       //
        TUSIGN8                                 unused_3;                       //
        TUSIGN8                                 unused_4;                       //
        TUSIGN8                                 unused_5;                       //
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}WAVEFORM_FRAME_TYPE;

typedef enum
{
    WAVEFORM_CURSOR_PRC_REL_VAL,
    WAVEFORM_CURSOR_CFG_REL_VAL,
    WAVEFORM_CURSOR_H_WIDTH,
    WAVEFORM_CURSOR_H_OFFSET,
    WAVEFORM_CURSOR_CFG_REL_VAL_EDIT

}WAVEFORM_CURSOR_POS;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  value;                          // The data to be displayed/adjusted
        TUSIGN8                                 unused_1;
        TUSIGN8                                 unused_2;
        TUSIGN8                                 unused_3;
        TUSIGN8                                 TimeAndOrDate;                  // 0 - Time and Date, 1 - Time Only, 2 - Date Only, 3 - Time Duration
        TUSIGN8                                 unused_4;
        TUSIGN8                                 unused_5;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}TIME_DATE_PARAM_FRAME;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  stringText;                     // The string to be displayed/adjusted
                                                                                // NOTE : stringText.ssIdx     - identifies the sub-system that "owns" the data
                                                                                //        stringText.objIdx    - identifies the object that "owns" the data
                                                                                //        stringText.attribIdx - is ignored (-1 = complete object is assumed)
        NUMBER_ATTRIBUTES_TYPE                  attributes;                     // Defines how the string is stored and how it is to be displayed
        TUSIGN8                                 unused_1;
        TUSIGN8                                 unused_2;
        TUSIGN8                                 unused_3;
        TUSIGN8                                 specialStringFlag;              // 0-standard string param, 1-Password/Hashcode Entry
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}STRING_PARAM_FRAME_TYPE;


typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  value;                          // The variable to be displayed/adjusted
        TUSIGN8                                 unused_1;
        TUSIGN8                                 engUnitsFlag;                   // 0 - use enumTable, 1 - use unitsTable
        TUSIGN8                                 unused_2;
        TUSIGN8                                 unused_3;
        TUSIGN8                                 firstItem_LSByte;               // position in enumTable of the text used when Value = 0
        TUSIGN8                                 firstItem_MSByte;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}ENUM_PARAM_FRAME_TYPE;

typedef struct
{
        TUSIGN8                                 titleBar;               // Frame title
        TUSIGN8                                 titleBar_HighByte;
        TUSIGN8                                 title;                  // parameter title
        TUSIGN8                                 title_HighByte;
        STR_REF                                 value_ssIdx;            // Subsystem index for both displayed and adjusted values
        STR_REF                                 adjvalue_objidx;        // Object index of adjusted value
        TUSIGN8                                 adjvalue_attribidx;     // Attribute index of adjusted value
        TUSIGN8                                 dispvalue_objidx;       // Object index of displayed value
        TUSIGN8                                 dispvalue_attribidx;    // Attribute index of displayed value
        ATTRIB_IDENTIFIER_TYPE                  engUnits;               // engineering units assigned to the value to be displayed
        TUSIGN8                                 engUnitsType;           //
        NUMBER_ATTRIBUTES_TYPE                  attributes;             // Defines the value type, how it is stored and how it is to be displayed
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;          // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}ADJUST_FRAME_TYPE;

typedef struct
{
        TUSIGN8                                 firstItem_LSB_Left;     // position in enumTable of the Left Key text used when status = 0
        TUSIGN8                                 firstItem_MSB_Left;
        TUSIGN8                                 firstItem_LSB_Right;    // position in enumTable of the Right Key text used when status = 0
        TUSIGN8                                 firstItem_MSB_Right;
        STR_REF                                 titleBar_attribidx;     // Frame title "NV Initialization"
        STR_REF                                 attribIdx_status;       // Attribue Index of status enum
        TUSIGN8                                 firstItem_LSB_Status1;  // position in enumTable of the upper line text used when status = 0
        TUSIGN8                                 firstItem_MSB_Status1;
        TUSIGN8                                 firstItem_LSB_Status2;  // position in enumTable of the lower line text used when status = 0
        TUSIGN8                                 firstItem_MSB_Status2;
        TUSIGN8                                 ssidx;                  // Subsystem index for both status enum and progress timer
        TUSIGN8                                 objidx_status;          // Object Index of status enum (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_progress;        // Object Index of progress timer value (attribue index of 0 is assumed)
        TUSIGN8                                 nVinitFlag;             // Defines that this frame is used for nV init (if so then all switches disabled)
        TUSIGN8                                 attribIdx_progress;     // Attribue Index of progress timer value
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;          // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}PROGRESS_BAR_FRAME_TYPE;

typedef struct
{
        TUSIGN8                                 matrix_object_index;    // matrix object index
        TUSIGN8                                 dpPosn_Col;             // defines the dp position for column data
        TUSIGN8                                 dpPosn_Row;             // defines the dp position for row data
        TUSIGN8                                 dpPosn_Elm;             // defines the dp position for element data
        STR_REF                                 title_attribidx;        // Frame title
        STR_REF                                 matrix_subsystem_index; // matrix subsystem index
        TUSIGN8                                 UOM_type_col;           // Defines the type of units applied to column data
        TUSIGN8                                 UOM_data_col_H;         // UOM_ss_col / UOM_IDS_col(MSByte)
        TUSIGN8                                 UOM_data_col_L;         // UOM_obj_col / UOM_IDS_col(LSByte)
        TUSIGN8                                 UOM_type_row;           // Defines the type of units applied to row data
        TUSIGN8                                 UOM_data_row_H;         // UOM_ss_row / UOM_IDS_row(MSByte)
        TUSIGN8                                 UOM_data_row_L;         // UOM_obj_row / UOM_IDS_row(LSByte)
        TUSIGN8                                 UOM_type_elm;           // Defines the type of units applied to element data
        TUSIGN8                                 UOM_data_elm_H;         // UOM_ss_elm / UOM_IDS_elm(MSByte)
        TUSIGN8                                 UOM_data_elm_L;         // UOM_obj_elm / UOM_IDS_elm(LSByte)
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;          // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}MATRIX_FRAME_TYPE;

typedef enum
{
    MATRIX_CURSOR_EXIT,
    MATRIX_CURSOR_COLUMN,
    MATRIX_CURSOR_COLUMN_EDIT,                  // do not change order "MATRIX_CURSOR_COLUMN_EDIT" must follow "MATRIX_CURSOR_COLUMN"
    MATRIX_CURSOR_ROW,
    MATRIX_CURSOR_ROW_EDIT,                     // do not change order "MATRIX_CURSOR_ROW_EDIT" must follow "MATRIX_CURSOR_ROW"
    MATRIX_CURSOR_ELEMENT,
    MATRIX_CURSOR_ELEMENT_EDIT,                 // do not change order "MATRIX_CURSOR_ELEMENT_EDIT" must follow "MATRIX_CURSOR_ELEMENT"
    MATRIX_CURSOR_ELEMENT_RESET
}MATRIX_CURSOR_POS;


typedef struct
{
        TUSIGN8                                 linearizer_subsystem_index;     // Linearizer Subsystem index
        TUSIGN8                                 inpoint_object_index;           // Input Point Object Index
        TUSIGN8                                 outpoint_object_index;          // Output Point Object Index
        TUSIGN8                                 pointEnable_objectIndex;        // Output Point Enable Object Index
        STR_REF                                 title_attribidx;                // Frame title
        STR_REF                                 unused;                         //
        TUSIGN8                                 dpPosn_inpoint;                 // defines the dp position for input points
        TUSIGN8                                 UOM_type_inpoint;               // Defines the type of units applied to input points
        TUSIGN8                                 UOM_data_inpoint_H;             // UOM_ss_inpoint / UOM_IDS_inpoint(MSByte)
        TUSIGN8                                 UOM_data_inpoint_L;             // UOM_obj_inpoint / UOM_IDS_inpoint(LSByte)
        TUSIGN8                                 dpPosn_outpoint;                // defines the dp position for output points
        TUSIGN8                                 UOM_type_outpoint;              // Defines the type of units applied to output points
        TUSIGN8                                 UOM_data_outpoint_H;            // UOM_ss_outpoint / UOM_IDS_outpoint(MSByte)
        TUSIGN8                                 UOM_data_outpoint_L;            // UOM_obj_outpoint / UOM_IDS_outpoint(LSByte)
        TUSIGN8                                 ProcessSignal;                  // ProcessSignal
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;          // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}LINEARIZER_FRAME_TYPE;

typedef enum
{
    LINEARIZER_CURSOR_EXIT,
//    LINEARIZER_CURSOR_INPOINT_CAPTURE,
    LINEARIZER_CURSOR_INPOINT_VIEW,
    LINEARIZER_CURSOR_INPOINT_EDIT,                 // do not change order "LINEARIZER_CURSOR_INPOINT_EDIT" must follow "LINEARIZER_CURSOR_INPOINT_VIEW"
//    LINEARIZER_CURSOR_OUTPOINT_ENABLE_DISABLE,
    LINEARIZER_CURSOR_OUTPOINT_VIEW,
    LINEARIZER_CURSOR_OUTPOINT_EDIT,                // do not change order "LINEARIZER_CURSOR_OUTPOINT_EDIT" must follow "LINEARIZER_CURSOR_OUTPOINT_VIEW"
}LINEARIZER_CURSOR_POS;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pNextFrame;            // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;     // ID of "Device Information"
        STR_REF                                 title_attribidx;        // ID of "Device ID"
        TUSIGN8                                 unused_TUSIGN8_1;
        TUSIGN8                                 unused_TUSIGN8_2;
        TUSIGN8                                 unused_TUSIGN8_3;
        TUSIGN8                                 unused_TUSIGN8_4;
        TUSIGN8                                 unused_TUSIGN8_5;
        TUSIGN8                                 unused_TUSIGN8_6;
        TUSIGN8                                 unused_TUSIGN8_7;
        TUSIGN8                                 unused_TUSIGN8_8;
        TUSIGN8                                 unused_TUSIGN8_9;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 unused_spare;
#endif
}SERVICE_LEVEL_INFO_FRAME_TYPE;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pNextFrame;            // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;     // Frame title
        STR_REF                                 unused_STR_REF_1;
        TUSIGN8                                 ProcessSignal_1;        // Signal table ID of process value being calibrated
        TUSIGN8                                 unused_TUSIGN8_1;
        TUSIGN8                                 ResultSignal_1;         // Signal table ID of 1st result
        TUSIGN8                                 ResultSignal_2;         // Signal table ID of 2nd result
        TUSIGN8                                 ssidx;                  // Subsystem index for status enum, error Result and target value
        TUSIGN8                                 objidx_status;          // Object Index of calibration status enum (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_errorResult;     // Object Index of calibration error Result (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_targetValue;     // Object Index of "new" target value (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_settlingTimer;   // Object Index of calibration settling Timer (attribue index of 0 is assumed)
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;
#endif
}MANUAL_CALIBRATION_FRAME_TYPE;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *pSecondaryInfo;        // address of the associated autoCalFrame_2 frame where additional info is stored
        STR_REF                                 titleBar_attribidx;     // Frame title
        STR_REF                                 Prompt_1;               // Instruction to user at 1st stage of calibration
        TUSIGN8                                 ProcessSignal_1;        // Signal table ID of process value being calibrated
        TUSIGN8                                 ProcessSignal_2;        // Signal table ID of secondary process value
        TUSIGN8                                 ResultSignal_1;         // Signal table ID of 1st result
        TUSIGN8                                 ResultSignal_2;         // Signal table ID of 2nd result
        TUSIGN8                                 ssidx;                  // Subsystem index for status enum, error Result and target value
        TUSIGN8                                 objidx_status;          // Object Index of calibration status enum (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_errorResult;     // Object Index of calibration error Result (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_targetValue;     // Object Index of calibration standard value (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_settlingTimer;   // Object Index of calibration settling Timer (attribue index of 0 is assumed)
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;
#endif
}AUTO_CALIBRATION_FRAME_TYPE_PRIMARY;

typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *unused_Pointer;
        STR_REF                                 unused_STR_REF_1;
        STR_REF                                 Prompt_2;               // Instruction to user at 2nd stage of calibration
        TUSIGN8                                 ssidx;                  // Subsystem index for Calibration Standard 1 type & Calibration Standard 2 type
        TUSIGN8                                 objidx_Std_1;           // Object Index for Calibration Standard 1 type
        TUSIGN8                                 attribidx_Std_1;        // Attribute Index for Calibration Standard 1 type
        TUSIGN8                                 firstItem_LSB_Status1;  // position in enumTable of the upper line text used when Calibration Standard 1 = 0
        TUSIGN8                                 firstItem_MSB_Status1;
        TUSIGN8                                 objidx_Std_2;           // Object Index for Calibration Standard 1 type
        TUSIGN8                                 attribidx_Std_2;        // Attribute Index for Calibration Standard 1 type
        TUSIGN8                                 firstItem_LSB_Status2;  // position in enumTable of the lower line text used when Calibration Standard 2 = 0
        TUSIGN8                                 firstItem_MSB_Status2;
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 unused_spare;
#endif
}AUTO_CALIBRATION_FRAME_TYPE_SECONDARY;


typedef struct
{
        struct _FRAME_FORMAT_TYPE const         *unused_Pointer;
        STR_REF                                 titleBar_attribidx;     // Frame title
        STR_REF                                 ProcessSignal_1;        // Signal table ID of process value being calibrated
        TUSIGN8                                 ResultSignal_1;         // Signal table ID of 1st result
        TUSIGN8                                 ResultSignal_2;         // Signal table ID of 2nd result
        TUSIGN8                                 ssidx;                  // Subsystem index for status enum, error Result, Buffer values & Temperature
        TUSIGN8                                 objidx_status;          // Object Index of calibration status enum (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_errorResult;     // Object Index of calibration error Result (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_buffer_1;        // Object Index of buffer 1 value (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_buffer_2;        // Object Index of buffer 2 value (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_temperature;     // Object Index of buffer temperature value (attribue index of 0 is assumed)
        TUSIGN8                                 objidx_tempUnits;       // Object Index of buffer temperature units (attribue index of 0 is assumed)
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;
#endif
}TWO_POINT_pH_CALIBRATION_FRAME_TYPE;

typedef struct
{
        void const                              *pFaultFrame;                   // address of the frame to be displayed if there is a non-standard reply attempting to save the adjusted value
        STR_REF                                 titleBar_attribidx;
        STR_REF                                 title_attribidx;                // Parameter title
        ATTRIB_IDENTIFIER_TYPE                  value;                          // The variable to be displayed/adjusted
        TUSIGN8                                 numberOfBits;                   // The number of bits (up to 8) that are relevant (if less than 8 the bits with lowest significance are used)
        TUSIGN8                                 unused_1;                       //
        TUSIGN8                                 unused_2;                       //
        TUSIGN8                                 unused_3;                       //
        TUSIGN8                                 unused_4;                       //
        TUSIGN8                                 displayStyle;                   // Example..
                                                                                //  8 bits (Data = 10101010)
                                                                                //          1 =  "10101010"  2 =  "8-6-4-2-"  3 =  "-2-4-6-8"
                                                                                //          bit7__|      |   bit7__|      |   bit0__|      |
                                                                                //          blt0_________|   bit0_________|   bit7_________|
                                                                                //
                                                                                //  4 bits (Data = xxxx1010)
                                                                                //          1 =  "1010"      2 =  "4-2-"      3 =  "-2-4"
                                                                                //          bit3__|  |       bit3__|  |       bit0__|  |
                                                                                //          bit0_____|       bit0_____|       bit3_____|
#ifdef HMI_SUPPORTS_EASYSETUP
        TUSIGN8                                 easySetupFlag;                  // Defines that the frame is: (0)not in easysetup sequence (1)in easysetup sequence (2)last frame in easysetup sequence
#endif
}BIT_ENUMERATION_FRAME_TYPE;


typedef union
{
        OPERATOR_FRAME_TYPE                     operatorFrame;          // data required to define an operator level frame
        CONFIG_HEADER_FRAME_TYPE                configHeaderFrame;      // data required to define a configuration level header frame
        MENU_FRAME_TYPE                         menuFrame;              // data required to define a menu frame
        NUMERICAL_PARAM_FRAME_TYPE              numericalParamFrame;    // data required to define a frame for display of a numerical value or a manual adjustment
        STRING_PARAM_FRAME_TYPE                 stringParamFrame;       // data required to define a frame for display of an alphanumeric string
        ENUM_PARAM_FRAME_TYPE                   enumParamFrame;         // data required to define a frame for display of an enumerated list
        ADJUST_FRAME_TYPE                       adjParamFrame;          // data required to define a frame for auto/manual adjustment of a value
        PROGRESS_BAR_FRAME_TYPE                 progressFrame;          // data required to define a progress bar and status frame
        SERVICE_LEVEL_INFO_FRAME_TYPE           serviceLevelInfoFrame;  // data required to define a frame for service level information
        TIME_DATE_PARAM_FRAME                   timeDateFrame;          // data required to define a frame for display of a time/date value
        MANUAL_CALIBRATION_FRAME_TYPE           manualCalFrame;         // data required to define a frame for display of a manual calibration procedure
        AUTO_CALIBRATION_FRAME_TYPE_PRIMARY     autoCalFrame;           // data required to define a frame for display of a automatic calibration procedure (1st part)
        AUTO_CALIBRATION_FRAME_TYPE_SECONDARY   autoCalFrame_2;         // data required to define a frame for display of a automatic calibration procedure (2nd part)
        TWO_POINT_pH_CALIBRATION_FRAME_TYPE     pHCalFrame;             // data required to define a frame for display of a 2 point pH calibration procedure
        BIT_ENUMERATION_FRAME_TYPE              bitEnumParamFrame;      // data required to define a frame for display of a bit enumeration
        GRAPH_PARAM_FRAME_TYPE                  graphParamFrame;        // data required to define a frame for display of a graph of values from an array
        WAVEFORM_FRAME_TYPE                     waveformFrame;          // data required to define a frame for display of a graph of values from an array (waveform style)
        MATRIX_FRAME_TYPE                       matrixFrame;            // data required to define a frame for display of values from a matrix
        LINEARIZER_FRAME_TYPE                   linearizerFrame;        // data required to define a frame for display of values from a linearizer
}FRAME_DEFINITION_TYPE;

typedef struct _FRAME_FORMAT_TYPE
{
        FRAME_TYPE_TYPE                         frameType;              // defines the type of frame?operator level frame, configuration level header, menu frame etc
#ifdef HMI_USES_SPECIAL_OP_CODES
        TUSIGN8                                 specialOpCode[3];       // identifies any "special" operations that may be required before,during and after the frame is displayed
#endif
        FRAME_DEFINITION_TYPE                   frameDefinition;        // data required to display frame data (depends upon frame type)
}FRAME_FORMAT_TYPE;


typedef struct
{
        STR_REF                                 text_attribIdx;
        TUSIGN8                                 validTest;              // position in validityTestTable of a function that determines if the menu item is available
}MENU_ITEM_TYPE;


typedef struct
{
    ATTRIB_IDENTIFIER_TYPE  value;
    TUSIGN8                 dpPosn;
    TUSIGN8                 engUnitsType;
    ATTRIB_IDENTIFIER_TYPE  engUnits;
    STR_REF                 short_identifier;
    STR_REF                 long_identifier;
    ATTRIB_IDENTIFIER_TYPE  valueLower;
    ATTRIB_IDENTIFIER_TYPE  valueUpper;
    TUSIGN8                 FixedUpperLower;        // 0 - uses "valueLower" & "valueUpper"
                                                    // 1 - uses 4..20
                                                    // 2 - uses 0..100
    #ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
    TUSIGN8                 SignalsViewTest;        // position in validityTestTable of a function that determines if the signal is included in the signals view
    #else
    TUSIGN8                 SignalsViewEnable;      // 0 - not included in signals view, 1 - included in signals view
    #endif

}SIGNAL_TABLE_TYPE;


typedef struct
{
    FRAME_FORMAT_TYPE const *pFrame;
    TUSIGN8                 cursor;
}FRAME_MEMORY_ENTRY;

#define FRAME_MEMORY_SIZE       7

#define NO_DISPLAY_ON_ADJUST    0xFF

#ifdef HMI_SUPPORTS_EASYSETUP
typedef struct
{
    const TUSIGN8*  tableAddress;
    TUSIGN8         tableSize;
}ESVT_LIST_OF_TABLES_ENTRY;
#endif

#define CLASSIFICATION_MAX              5
#define CLASSIFICATION_INFORMATIONONLY  5
#define CLASSIFICATION_FAILURE          4
#define CLASSIFICATION_MAINTENANCEREQD  3
#define CLASSIFICATION_OFFSPEC          2
#define CLASSIFICATION_CHECKFUNCTION    1
#define CLASSIFICATION_NONE             0

#define CLASSIFICATION_NOT_NAMUR        CLASSIFICATION_MAX + 1

#define CATEGORY_TRANSMITTER            18
#define CATEGORY_SENSOR_6               17
#define CATEGORY_SENSOR_5               16
#define CATEGORY_SENSOR_4               15
#define CATEGORY_SENSOR_3               14
#define CATEGORY_SENSOR_2               13
#define CATEGORY_SENSOR_1               12

#define CATEGORY_CALIBRATION            11
#define CATEGORY_INFORMATION            10
#define CATEGORY_POWERSUPPLY            9
#define CATEGORY_PROCESS                8
#define CATEGORY_ACTUATOR               7
#define CATEGORY_HARDWARE               6
#define CATEGORY_SENSOR                 5
#define CATEGORY_ELECTRONICS            4
#define CATEGORY_INSTALLATION           3
#define CATEGORY_CONFIGURATION          2
#define CATEGORY_OPERATION              1
#define CATEGORY_UNSPECIFIED            0

#define ARM_ACCESS_LOGOUT               0
#define ARM_ACCESS_READ_ONLY            1
#define ARM_ACCESS_STANDARD             2
#define ARM_ACCESS_ADVANCED             3
#define ARM_ACCESS_SERVICE              4
#define ARM_ACCESS_INVALID              5

#ifdef LEFT_RIGHT_LOCKOUT

#define NOT_OPERATOR_PAGE               0
#define WRONG_KEY_PRESSED               1
#define NO_KEY_PRESSED                  2
#define LOOKING_FOR_UP                  3
#define LOOKING_FOR_UP_AND_DOWN         4
#define UP_KEY_ACTIVE                   5
#define LEFT_RIGHT_ENABLED              6

#endif


#endif





