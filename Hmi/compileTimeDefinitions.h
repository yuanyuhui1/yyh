#ifdef _COMPILETIMEDEFINITIONS
  #error CompileTimeDefinitions.h included more than once
#else
#define _COMPILETIMEDEFINITIONS

#define COMMON_TOP_WORKS_NEW_FEATURES

#define MULTIPLELANGUAGES
#define SECOND_OPERATOR_PAGE
#define THIRD_OPERATOR_PAGE
#define FOURTH_OPERATOR_PAGE
//#define FIFTH_OPERATOR_PAGE
//#define SIXTH_OPERATOR_PAGE
#define HMI_SUPPORTS_EASYSETUP
//#define HMI_USES_SPECIAL_OP_CODES
//#define HMI_USES_FAULT_FRAME
//#define HMI_SUPPORTS_FONT_2
#define HMI_SUPPORTS_FONT_3
//#define HMI_SUPPORTS_FONT_4
//#define HMI_SUPPORTS_FONT_5
//#define HMI_SUPPORTS_PACKEDASCII
#define HMI_SUPPORTS_STANDARDASCII
#define HMI_SUPPORTS_OPERATOR_FRAMES
#define HMI_SUPPORTS_CONFIG_HEADER_FRAMES
#define HMI_SUPPORTS_MENU_FRAMES
#define HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
#define HMI_SUPPORTS_ENUM_PARAM_FRAMES
//#define HMI_SUPPORTS_MANUAL_ADJUST
#define HMI_SUPPORTS_AUTO_ADJUST
//#define HMI_SUPPORTS_PROGRESS_FRAMES
#define HMI_SUPPORTS_ASIAN
#define HMI_SUPPORTS_CHINESE
//#define HMI_SUPPORTS_ASIAN_8
//#define EMBEDDED_ASIAN
#define NVINIT_STYLE       0
#define NVINIT_MAX_STATUS  0
#define HMI_SUPPORTS_ARM_READONLY
#define HMI_SUPPORTS_ARM_STANDARD
//#define HMI_ARM_STANDARD_RENAMED_CALIBRATE
//#define HMI_ARM_STANDARD_RENAMED_BASIC
#define HMI_SUPPORTS_ARM_ADVANCED
#define HMI_SUPPORTS_ARM_SERVICE
#define HMI_SUPPORTS_SECURITY_ACCESS
#define HMI_USES_COMMONDIAGNOSTICS
#define HMI_USES_DOUBLE
//#define HMI_SUPPORTS_SERVICEPORT
#define HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
#define HMI_SUPPORTS_DISPLAY_TEST
//#define HMI_SUPPORTS_POSITIONER_MODES
//#define HMI_SUPPORTS_MANUAL_CALIBRATION
//#define HMI_SUPPORTS_AUTO_CALIBRATION
//#define HMI_SUPPORTS_2PT_PH_CALIBRATION
//#define HMI_SUPPORTS_PARAMETER_DUMP
//#define HIGH_LEVEL_USES_EXTENDED_CHARACTER_SET
//#define HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
//#define HMI_SUPPORTS_CUSTOM_UNITS
//#define HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
//#define LEFT_RIGHT_LOCKOUT
//#define LAYER_2_OPTIMIZED_FOR_SPEED
//#define HMI_SUPPORTS_16BIT_ENUMS
#define HMI_SUPPORTS_INTEGER_FRAMES
#define HMI_SUPPORTS_IP_ADDRESS_FRAMES
//#define HMI_SUPPORTS_GRAPH_PARAM_FRAMES
#define HMI_SUPPORTS_STRINGS_OVER_19_CHARS
#define HMI_SUPPORTS_STRINGS_OVER_20_CHARS
//#define HMI_USES_INDIRECT_TEXT
//#define HMI_USES_CUSTOM_LANGUAGES
//#define HMI_SUPPORTS_WAVEFORM_FRAMES
//#define HMI_SUPPORTS_MATRIX_FRAMES
//#define HMI_SUPPORTS_LINEARIZER_FRAMES
//#define HMI_USES_4WCTW_DISPLAY
//#define HMI_USES_INDIRECT_STRINGS
//#define HMI_USES_NEWLINE
//#define HMI_SUPPORTS_DIRECT_ACCESS_FRAMES


#define LANGUAGE_1_INCLUDED_AS_STANDARD
#define LANGUAGE_2_INCLUDED_AS_STANDARD
#define LANGUAGE_3_INCLUDED_AS_STANDARD
#define LANGUAGE_4_INCLUDED_AS_STANDARD
//#define LANGUAGE_5_INCLUDED_AS_STANDARD
//#define LANGUAGE_6_INCLUDED_AS_STANDARD
//#define LANGUAGE_7_INCLUDED_AS_STANDARD
//#define LANGUAGE_8_INCLUDED_AS_STANDARD
//#define LANGUAGE_9_INCLUDED_AS_STANDARD
//#define LANGUAGE_10_INCLUDED_AS_STANDARD
//#define LANGUAGE_11_INCLUDED_AS_STANDARD
//#define LANGUAGE_12_INCLUDED_AS_STANDARD
//#define LANGUAGE_13_INCLUDED_AS_STANDARD
//#define LANGUAGE_14_INCLUDED_AS_STANDARD
//#define LANGUAGE_15_INCLUDED_AS_STANDARD
//#define LANGUAGE_16_INCLUDED_AS_STANDARD
//#define LANGUAGE_17_INCLUDED_AS_STANDARD
//#define LANGUAGE_18_INCLUDED_AS_STANDARD
//#define LANGUAGE_19_INCLUDED_AS_STANDARD
//#define LANGUAGE_20_INCLUDED_AS_STANDARD

#define INCLUDE_EUROPEAN_PATTERNS
//#define INCLUDE_TURKISH_PATTERNS
//#define INCLUDE_KATAKANA_PATTERNS
//#define INCLUDE_RUSSIAN_PATTERNS
//#define INCLUDE_LATIN1_PATTERNS


#define TYPE_1_NVRAM_REQUIRED_VERSION 6

//#define GROUP_HW_STATUS_SUPPORTED
#define GROUP_HW_STATUS_ELECTRONICS_SUPPORTED
#define GROUP_HW_STATUS_SENSOR_SUPPORTED
//#define GROUP_HW_STATUS_ACTUATOR_SUPPORTED
#define GROUP_CONFIG_STATUS_SUPPORTED
//#define GROUP_CONFIG_STATUS_SETUP_SUPPORTED
#define GROUP_OPERATING_CONDITION_SUPPORTED
//#define GROUP_OPERATING_CONDITION_POWER_SUPPLY_SUPPORTED
//#define GROUP_OPERATING_CONDITION_PROCESS_SUPPORTED
//#define GROUP_INFORMATION_SUPPORTED
//#define GROUP_CALIBRATION_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_1_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_2_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_3_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_4_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_5_SUPPORTED
//#define GROUP_HW_STATUS_SENSOR_6_SUPPORTED
//#define GROUP_HW_STATUS_TRANSMITTER_SUPPORTED

#define INFORMATION_MESSAGES_OFFSET 201

#define HMI_SUPPORTS_DISPLAY_MODE_OFF
//#define HMI_SUPPORTS_DISPLAY_MODE_1x3
//#define HMI_SUPPORTS_DISPLAY_MODE_1x4
//#define HMI_SUPPORTS_DISPLAY_MODE_1x6_A
//#define HMI_SUPPORTS_DISPLAY_MODE_1x6_BAR_A
#define HMI_SUPPORTS_DISPLAY_MODE_1x6_B
#define HMI_SUPPORTS_DISPLAY_MODE_1x6_BAR_B
#define HMI_SUPPORTS_DISPLAY_MODE_1x9
#define HMI_SUPPORTS_DISPLAY_MODE_1x9_BAR
#define HMI_SUPPORTS_DISPLAY_MODE_2x9
#define HMI_SUPPORTS_DISPLAY_MODE_2x9_BAR
#define HMI_SUPPORTS_DISPLAY_MODE_3x9
//#define HMI_SUPPORTS_DISPLAY_MODE_1x3_ASIAN
#define HMI_SUPPORTS_DISPLAY_MODE_1x6_BAR_B_ASIAN
#define HMI_SUPPORTS_GRAPH_VIEW
//#define HMI_SUPPORTS_DISPLAY_MODE_4x9


#define LONGEST_NUMBER_IS_12_DIGITS
//#define LONGEST_NUMBER_IS_8_DIGITS
//#define LONGEST_NUMBER_IS_4_DIGITS


#define HMI_UPDATE_RATE_MILLISEC         100
#define HMI_DISPLAY_RATE_CYCLES          10
#define HMI_KEY_REPEAT_CYCLES            3
#define HMI_AUTOSCROLL_COUNT_LIMIT       100
#define HMI_DEFAULT_CONTRAST_COUNT_LIMIT 30

#define SS_HMI_OBJ_STRINGTABLE           100
#define SS_HMI_UNITS_TABLE_1             99
#define SS_HMI_UNITS_TABLE_2             98
#define SS_HMI_UNITS_TABLE_3             97
#define SS_HMI_UNITS_TABLE_4             96
#define SS_HMI_UNITS_TABLE_5             95
#define SS_HMI_UNITS_TABLE_6             94
#define SS_HMI_UNITS_TABLE_7             93
#define SS_HMI_UNITS_TABLE_8             92
#define SS_HMI_UNITS_TABLE_9             91
#define SS_HMI_UNITS_TABLE_10            90
#define SS_HMI_UNITS_TABLE_11            89

#define HMI_CONFIGMODE_TIMEOUT_SEC       300

#define PASSWORD_LENGTH                  6

//! HMI version information

#define VERSION_INFO   (TUSIGN16)20101

// this is in the format         XYYZZ
// which is derived from HMI/200X.YY.ZZ
// where:  X represents the compatible framework version and is changed only if new version incompatible with previous version
//        YY represents the HMI release version and is updated when new features are added
//        ZZ represents the HMI release version and is updated if bug fixes are introduced


#define HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
#define VALIDITY_TEST_CALLER
#define SIGNALS_TEST_NOT_APPLICABLE    0
#define SIGNALS_TEST_FOR_SIGNALS_VIEW  1
#define SIGNALS_TEST_FOR_OPERATOR_PAGE 2
#define SIGNALS_TEST_FOR_WAVEFORM      3

//#define HMI_SUPPORTS_DIAGNOSIS
//#define HMI_SUPPORTS_DIAGNOSIS_FIELD_1
//#define HMI_SUPPORTS_DIAGNOSIS_FIELD_2
#ifdef HMI_SUPPORTS_DIAGNOSIS
 #ifdef HMI_SUPPORTS_DIAGNOSIS_FIELD_1
#define FIRST_ITEM_DIAGNOSIS_FIELD_1  0
#endif
#ifdef HMI_SUPPORTS_DIAGNOSIS_FIELD_2
#define FIRST_ITEM_DIAGNOSIS_FIELD_2  0
#endif
#endif


#define STR_REF            TUSIGN16
#define MAX_SIGNALS        28
#define ITEMS_PER_MENU     4
#define PACKETBUFFERSIZE   240

//+@ HMIGen UserSpecificDefinitions @+
// The following definitions can be used to enable features to specific to individual projects
//
// COMPILER FEATURES
// -----------------
// #define OPTIMIZE_V3_4                              // Make this def'n to use #pragma optimize directives compatible with compiler version 3.40
// #define COMPILER_RX_FAMILY                         // Make this def'n to if HMI is for use on RX family of microcontrollers using "section = .." 
// #define COMPILER_RX_FAMILY_USING_LOCATION          // Make this def'n to if HMI is for use on RX family of microcontrollers using "location = .." 
//
// DATACLASSES
// -----------
// The following definitions can be used to change the HMI dataclasses
// #define SFCD                                      // Make this def'n to make the HMI use constant defaults for the static frequent data. This eliminates the requirement
                                                    // of additional RAM & EEPROM for default values for the static frequent data. The defaults anre set in ROM

// #define SFD_SPLIT                                 // Make this def'n to split the static frequent data into separate segments for
                                                    // (1) minimum requirements for a simple HMI with only 1 operator page
					                                // (2) additional data required for multiple operator pages
					                                // (3) additional data required for the custom decimal point implementation
//
// Startup Logo (These definitions have no effect on type 1 implementations
// ------------------------------------------------------------------------
// #define OEM_BLANK                                 // Make this def'n to replace the ABB logo with a blank display
// #define OEM_HMI_BUTTONS                           // Make this def'n to replace the ABB logo with a representation of the HMI buttons

// Appearance
// ----------
// #define DISABLE_SMOOTH_FONT                       // Make this def'n to remove the code that provides smoothed appearance of the process values in the operator pages
// #define DONT_FLASH_OVERRANGE_VALUES               // Make this def'n to stop process values flashing if they are outside the limits defined in their descriptor
// #define VALUE_1_FAST                              // Make this def'n to cause the primary process value to be updated at the fastest possible rate

// Hardware Specific
// -----------------
// #define DISPLAY_FLIP_HORIZONTAL                   // Make this def'n to cause the displayed image to be flipped (mirrored) horizontally
// #define DISPLAY_FLIP_VERTICAL                     // Make this def'n to cause the displayed image to be flipped (mirrored) vertically
// #define DISPLAY_ROTATE_180                        // Make this def'n to cause the displayed image to be rotated by 180 degrees

// Switch Operation
// ----------------
// #define STRING_ACCELERATOR                        // Make this def'n to provide an accelerator on string edit frames
// #define ENUM_ACCELERATOR                          // Make this def'n to provide an accelerator on enum edit frames
// #define IPADDR_ACCELERATOR                        // Make this def'n to provide an accelerator on IP address edit frames
// #define MANUAL_ADJUST_ACCELERATOR                 // Make this def'n to provide an accelerator on Manual Adjust frames
// #define DATE_TIME_ACCELERATOR                     // Make this def'n to provide an accelerator on date/time edit frames (accelerator only acts on time fields)
// #define DISABLE_BACK_MOVE_SELECT                  // Make this def'n to disable the ability to navigate between parameter view frames immediately below a sub-menu
// #define HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE      // Make this def'n to allow "Hold Output" & "Release Output" stages to be added to calibration sequences
// #define HMI_CONFIG_MODE_TIMEOUT_DISABLE           // Make this def'n to disable the mechanism that causes the HMI to return to the operator page after 5 minutes of inactivity.
// #define DISABLE_QUICK_EXIT                        // Make this def'n to disable the mechanism that allows the user to quickly return to the operator by pressing & holding the left (Back) function key.
// #define DISABLE_OP_PAGE_ID                        // Make this def'n to replace the "numbered" view icons on the operator page with a generic view icon
 #define ENABLE_LINE_SOURCE_VALIDITY_TESTS         // Make this def'n to enable the mechanism that applies the signals validity tests to the operator page line signal sources
// #define OVERRIDE_LR_LOCKOUT_WHILST_LOGGED_IN      // Make this def'n to enable Left & Right keys whilst user is logged in. NOTE: This defn has is only relevant if LEFT_RIGHT_LOCKOUT & HMI_SUPPORTS_SECURITY_ACCESS are both defined
// #define LEFT_KEY_SELECTS_STANDARD_LEVEL           // Make this def'n to override left key operation - Left key enters configuration at Standard level (ARM)
// #define LEFT_KEY_SELECTS_ADVANCED_LEVEL           // Make this def'n to override left key operation - Left key enters configuration at Advanced level (ARM)
// #define LEFT_KEY_SELECTS_SERVICE_LEVEL            // Make this def'n to override left key operation - Left key enters configuration at Service level (ARM)
// #define DISPLAY_DIAGNOSIS_CONDITION_DETAILS       // Make this def'n to enable the diagnostics details frame - available from diagnostics view & diagnostics history
// #define HMI_SUPPORTS_KEY_RENAMING					// Make this def'n to enable the renaming of function keys
// #define UP_DOWN_KEYS_RESPOND_ONCE                 // Make this def'n to make the up & down keys respond only once - key must be released & pressed again to perform repeated action

// Special Operation Functions
// ---------------------------
// #define HMI_SUPPORTS_SECONDARY_IDENTIFIER         // Make this def'n to allow additional text to be added to a view or edit frame
// #define HMI_SUPPORTS_SEQUENCES                    // Make this def'n to allow sub-menus to be processed as a sequence
// #define HMI_SUPPORTS_MANUAL_VALVE_CAL             // Make this def'n to allow Auto Adjust frames to act as manual valve calibration frames
// #define HMI_SUPPORTS_CONFIG_CHANGE_COUNTER        // Make this def'n to allow a (normally disabled) configuration change counter to be incremented when parameter values are edited and accepted
// #define HMI_SUPPORTS_GLOBAL_CONFIG_CHANGE_COUNTER // Make this def'n to allow a (normally enabled) configuration change counter to be incremented when parameter values are edited and accepted
// #define HMI_SUPPORTS_MANUAL_ADJUST_MODIFIER       // Make this def'n to allow a Manual Adjust frames to support Manual valve, relay and digital o/p 

// Special Builds
// --------------
// #define LOW_LEVEL_HMI_TEST                        // Make this def'n to build a version of HMI specifically to test low level HMI's
// #define AZ200_SPECIAL_FEATURES_HMI                // Make this def'n to introduce features specific to the Endura series of Oxygen analysers
// #define EDP300_SPECIAL_FEATURES_HMI               // Make this def'n to introduce features specific to the Tornado series of valve positioners
// #define APA592_SPECIAL_FEATURES_HMI               // Make this def'n to introduce features specific to the Starlight series of analysers
// #define MILE2_266_SPECIAL_FEATURES_HMI            // Make this def'n to introduce features specific to the 266 series of pressure transmitters
// #define NAVIGATOR_SPECIAL_FEATURES_HMI            // Make this def'n to introduce features specific to the Navigator series of devices

// Miscellaneous
// -------------
// #define OVERRIDE_POWERUP_CHECK                    // Make this def'n to remove the requirement to call HMI_PowerOnSRV() before HMI can apply power to the low level HMI
// #define LOCAL_DATA_CHECK                          // Make this def'n to introduce a feature that checks the values of all HMI configuration objects when the HMI subsystem is initialized
// #define LOCAL_INTERRUPT_HANDLERS                  // Make this def'n to use interrupt handlers for HMI defined locally in HMI software
// #define DISABLE_NATIVE_LANGUAGE                   // Make this def'n to disable the mechanism that forces the language enumeration text to be displayed in its native language
// #define LIMIT_EXECUTION_TIME                      // Make this def'n to force HMI EXE to stop building packets after a defined period of time

//-@ HMIGen UserSpecificDefinitions @-



//+@ HMIGen ARM_PIN_OBJECT_Definition @+
// The object index of the ARM PIN (�rolling?PIN) must be defined here.
// eg:
//      #define ARM_PIN_OBJECT    ARM_IDX_pin
        #define ARM_PIN_OBJECT    ARM_IDX_adpin
//
//#error ARM_PIN_OBJECT not defined


//-@ HMIGen ARM_PIN_OBJECT_Definition @-


//+@ HMIGen EXECUTION_TIME_LIMIT_mS_Definition @+
#ifdef LIMIT_EXECUTION_TIME
// If LIMIT_EXECUTION_TIME is defined then the time limit must be defined here.
//
// EXECUTION_TIME_LIMIT_mS cannot be less than 10mS
// eg:
//       #define EXECUTION_TIME_LIMIT_mS 50

    #error EXECUTION_TIME_LIMIT_mS not defined
//
#endif
//-@ HMIGen EXECUTION_TIME_LIMIT_mS_Definition @-


#endif
