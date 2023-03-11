// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 96
// In the definition of a function-like macro the whole definition,
// and each instance of a parameter, shall be enclosed in parentheses.
#pragma diag_suppress=Pm084
#ifdef _R32C_COMPILER
#pragma diag_suppress=Pm154
#endif

//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    class definition T_UNIT::HMI

                subsystem interface
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hmi.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hmi.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be include before hmi.h
#endif

#if (T_UNIT_VERSION<0x020100)
  #error T_UNIT_VERSION must be greater or equal than 2.1.0
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HMI_H__
  #error hmi.h included more than once
#endif

#define __HMI_H__

//----------------------------------------------------------------------------------------------------------
//Define For Smart HMI
#define SUPPORT_OLD_HMI 1
#define SUPPORT_SHMI  1  
#define IP_ADDRESS_DATA_REVERSED 1
#define ASIAN_LANGUAGE_VERSION        10         //this shall be the same with lcd_patterns_Asian_550x.h
//this is for smartHMI
#define LANGUAGE_1_INCLUDED_AS_STANDARD_S
#define LANGUAGE_2_INCLUDED_AS_STANDARD_S
#define LANGUAGE_3_INCLUDED_AS_STANDARD_S
#define LANGUAGE_4_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_5_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_6_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_7_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_8_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_9_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_10_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_11_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_12_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_13_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_14_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_15_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_16_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_17_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_18_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_19_INCLUDED_AS_STANDARD_S
//#define LANGUAGE_20_INCLUDED_AS_STANDARD_S
//----------------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // _LANGUAGE
  #define HMI_LANGUAGE_ENGLISH 0 //English
  #define HMI_LANGUAGE_GERMAN 1 //German
  #define HMI_LANGUAGE_FRENCH 2 //French
  #define HMI_LANGUAGE_SPANISH 3 //Spanish
  #define HMI_LANGUAGE_ITALIAN 4 //Italian
  #define HMI_LANGUAGE_DUTCH 5 //Dutch
  #define HMI_LANGUAGE_DANISH 6 //Danish
  #define HMI_LANGUAGE_SWEDISH 7 //Swedish
  #define HMI_LANGUAGE_FINNISH 8 //Finnish
  #define HMI_LANGUAGE_POLISH 9 //Polish
  #define HMI_LANGUAGE_RUSSIAN 10 //Russian
  #define HMI_LANGUAGE_CHINESE 11 //Chinese
  #define HMI_LANGUAGE_JAPANESE 12 //Japanese
  #define HMI_LANGUAGE_TURKISH 13 //Turkish
  #define HMI_LANGUAGE_PORTUGUESE 14 //Portuguese
  #define HMI_LANGUAGE_NOT_DEFINED 20

  // _MAIN_OPERATOR_VIEW
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_1 0 //1st signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_2 1 //2nd signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_3 2 //3rd signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_4 3 //4th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_5 4 //5th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_6 5 //6th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_7 6 //7th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_8 7 //8th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_9 8 //9th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_10 9 //10th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_11 10 //11th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_12 11 //12th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_13 12 //13th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_14 13 //14th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_15 14 //15th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_16 15 //16th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_17 16 //17th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_18 17 //18th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_19 18 //19th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_20 19 //20th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_21 20 //21th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_22 21 //22th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_23 22 //23th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_24 23 //24th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_25 24 //25th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_26 25 //26th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_27 26 //271th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_28 27 //28th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_29 28 //29th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_30 29 //30th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_31 30 //31th signal defined in signals table
  #define HMI_MAIN_OPERATOR_VIEW_SIGNAL_32 31 //32th signal defined in signals table

  // _LOCAL_OP
  #define HMI_LOCAL_OP_DISABLED eTRUE //Local Operation Disabled
  #define HMI_LOCAL_OP_ENABLED eFALSE //Local Operation Enabled

  // _DISPLAY_MODE
  #define HMI_DISPLAY_MODE_OFF 0 //Operator page switched off
  #define HMI_DISPLAY_MODE_GRAPHIC 1 //Graph View
  #define HMI_DISPLAY_MODE_1x4 2 //1 row of 4 characters
  #define HMI_DISPLAY_MODE_1x6_A 3 //1 row of 6 characters (Rainbow Style)
  #define HMI_DISPLAY_MODE_1x6_BAR_A 4 //1 row of 6 characters + Bargraph (Rainbow Style)
  #define HMI_DISPLAY_MODE_1x6_B 5 //1 row of 6 characters (Standard Style)
  #define HMI_DISPLAY_MODE_1x6_BAR_B 6 //1 row of 6 characters + Bargraph (Standard Style)
  #define HMI_DISPLAY_MODE_1x9 7 //1 row of 9 characters
  #define HMI_DISPLAY_MODE_1x9_BAR 8 //1 row of 9 characters + Bargraph
  #define HMI_DISPLAY_MODE_2x9 9 //2 rows of 9 characters
  #define HMI_DISPLAY_MODE_2x9_BAR 10 //2 rows of 9 characters + Bargraph
  #define HMI_DISPLAY_MODE_3x9 11 //3 rows of 9 characters
  #define HMI_DISPLAY_MODE_4x9 12 //4 rows of 9 characters
  #define HMI_DISPLAY_MODE_LPV 13 //
  #define HMI_DISPLAY_MODE_LPV_SSV 14 //
  #define HMI_DISPLAY_MODE_LPV_BAR 15 //
  #define HMI_DISPLAY_MODE_MPV_MSV 16 //
  #define HMI_DISPLAY_MODE_MPV_MSV_BAR 17 //
  #define HMI_DISPLAY_MODE_SPV_SSV_STV 18 //

  // 
  #define HMI_DISPLAY_MODE_MAX 18 //
  #define HMI_VALVE_CAL_STATUS_INACTIVE 0 //Calibration status - for Manual Valve Calibration
  #define HMI_VALVE_CAL_STATUS_SUCCESSFUL 1 //Calibration status - for Manual Valve Calibration
  #define HMI_VALVE_CAL_STATUS_FAIL_SPAN_TOO_SMALL 2 //Calibration status - for Manual Valve Calibration
  #define HMI_VALVE_CAL_STATUS_FAIL_RANGE_ERROR 3 //Calibration status - for Manual Valve Calibration
  #define HMI_VALVE_CAL_STATUS_FAIL_SENSOR_ERROR 4 //Calibration status - for Manual Valve Calibration
  #define HMI_TRENDVIEW_TIMEBASE_OFF 0 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_1_MINUTE 1 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_2_MINUTE 2 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_5_MINUTE 3 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_10_MINUTE 4 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_20_MINUTE 5 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_TIMEBASE_30_MINUTE 6 //Timebase - for graphical(Trend) operator page
  #define HMI_TRENDVIEW_STATUS_INACTIVE 0 //Status of Trend view section
  #define HMI_TRENDVIEW_STATUS_SAMPLING 1 //Status of Trend view section
  #define HMI_TRENDVIEW_STATUS_ARRAY_VIEW 2 //Status of Trend view section
  #define HMI_ROTATION_N_CODES 2 //
  #define HMI_MENU_FORMAT_N_CODES 4 //
  #define HMI__LANGUAGE_N_OPTIONS LANGUAGE_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_LANGUAGE_N_OPTIONS CUSTOM_LANGUAGE_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_FORMAT_N_OPTIONS PAGE_1_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_LINE_1_N_OPTIONS PAGE_1_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_LINE_2_N_OPTIONS PAGE_1_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_LINE_3_N_OPTIONS PAGE_1_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_LINE_4_N_OPTIONS PAGE_1_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_1_LINE_B_N_OPTIONS PAGE_1_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_FORMAT_N_OPTIONS PAGE_2_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_LINE_1_N_OPTIONS PAGE_2_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_LINE_2_N_OPTIONS PAGE_2_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_LINE_3_N_OPTIONS PAGE_2_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_LINE_4_N_OPTIONS PAGE_2_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_2_LINE_B_N_OPTIONS PAGE_2_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_FORMAT_N_OPTIONS PAGE_3_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_LINE_1_N_OPTIONS PAGE_3_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_LINE_2_N_OPTIONS PAGE_3_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_LINE_3_N_OPTIONS PAGE_3_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_LINE_4_N_OPTIONS PAGE_3_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_3_LINE_B_N_OPTIONS PAGE_3_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_FORMAT_N_OPTIONS PAGE_4_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_LINE_1_N_OPTIONS PAGE_4_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_LINE_2_N_OPTIONS PAGE_4_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_LINE_3_N_OPTIONS PAGE_4_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_LINE_4_N_OPTIONS PAGE_4_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_4_LINE_B_N_OPTIONS PAGE_4_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_FORMAT_N_OPTIONS PAGE_5_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_LINE_1_N_OPTIONS PAGE_5_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_LINE_2_N_OPTIONS PAGE_5_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_LINE_3_N_OPTIONS PAGE_5_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_LINE_4_N_OPTIONS PAGE_5_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_5_LINE_B_N_OPTIONS PAGE_5_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_FORMAT_N_OPTIONS PAGE_6_FORMAT_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_LINE_1_N_OPTIONS PAGE_6_LINE_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_LINE_2_N_OPTIONS PAGE_6_LINE_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_LINE_3_N_OPTIONS PAGE_6_LINE_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_LINE_4_N_OPTIONS PAGE_6_LINE_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__PAGE_6_LINE_B_N_OPTIONS PAGE_6_LINE_B_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__DIAGNOSIS_VIEW_N_OPTIONS DIAGNOSIS_VIEW_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_1_N_OPTIONS CUSTOM_DP_1_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_2_N_OPTIONS CUSTOM_DP_2_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_3_N_OPTIONS CUSTOM_DP_3_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_4_N_OPTIONS CUSTOM_DP_4_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_5_N_OPTIONS CUSTOM_DP_5_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_6_N_OPTIONS CUSTOM_DP_6_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_7_N_OPTIONS CUSTOM_DP_7_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_8_N_OPTIONS CUSTOM_DP_8_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_9_N_OPTIONS CUSTOM_DP_9_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_10_N_OPTIONS CUSTOM_DP_10_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI__CUSTOM_DP_11_N_OPTIONS CUSTOM_DP_11_N_OPTIONS //the value according to ConstructorInclude.h, which is create by HMIGen
  #define HMI_WAVEFORM_PLOT_DATA_1_ADDRESS 1 //
  #define HMI_WAVEFORM_PLOT_DATA_2_ADDRESS 2 //
  #define HMI_WAVEFORM_PLOT_DATA_3_ADDRESS 3 //
  #define HMI_WAVEFORM_PLOT_ENABLE_1_U8 4 //
  #define HMI_WAVEFORM_PLOT_ENABLE_2_U8 5 //
  #define HMI_WAVEFORM_PLOT_ENABLE_3_U8 6 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_UNITS_U8 7 //
  #define HMI_WAVEFORM_H_SCALE_OFFSET_FLOAT 8 //
  #define HMI_WAVEFORM_H_SCALE_OFFSET_INC_OBJ 9 //
  #define HMI_WAVEFORM_H_SCALE_OFFSET_DEC_OBJ 10 //
  #define HMI_WAVEFORM_H_SCALE_RES_FLOAT 11 //
  #define HMI_WAVEFORM_H_SCALE_RES_INC_OBJ 12 //
  #define HMI_WAVEFORM_H_SCALE_RES_DEC_OBJ 13 //
  #define HMI_WAVEFORM_V_SCALE_RANGE_HIGH_FLOAT 14 //
  #define HMI_WAVEFORM_V_SCALE_RANGE_LOW_FLOAT 15 //
  #define HMI_WAVEFORM_V_SCALE_RANGE_DP_U8 16 //
  #define HMI_WAVEFORM_V_SCALE_RANGE_DIV_U8 17 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_NO_UNITS 0 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_FEET 1 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_INCHES 2 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_METRES 3 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_CENTIMETRES 4 //
  #define HMI_WAVEFORM_PLOT_DISTANCE_MILLIMETRES 5 //

  // _AUTOSCROLL
  #define HMI_AUTOSCROLL_DISABLED eFALSE //Autoscroll Disabled
  #define HMI_AUTOSCROLL_ENABLED eTRUE //Autoscroll Enabled

  // _CUSTOM_DP
  #define HMI_0_DP 0 //X
  #define HMI_1_DP 1 //X.X
  #define HMI_2_DP 2 //X.XX
  #define HMI_3_DP 3 //X.XXX
  #define HMI_4_DP 4 //X.XXXX
  #define HMI_5_DP 5 //X.XXXXX
  #define HMI_6_DP 6 //X.XXXXXX
  #define HMI_7_DP 7 //X.XXXXXXX
  #define HMI_8_DP 8 //X.XXXXXXXX
  #define HMI_9_DP 9 //X.XXXXXXXXX
  #define HMI_10_DP 10 //X.XXXXXXXXXX
  #define HMI_11_DP 11 //X.XXXXXXXXXXX
  #define HMI_12_DP 12 //X.XXXXXXXXXXXX

  // _KEYPRESS
  #define HMI_NO_KEYPRESS 0 //
  #define HMI_RIGHT_KEYPRESS 1 //
  #define HMI_LEFT_KEYPRESS 2 //
  #define HMI_UP_KEYPRESS 3 //
  #define HMI_DOWN_KEYPRESS 4 //
  #define HMI_FAST_UP_KEYPRESS 5 //
  #define HMI_FAST_DOWN_KEYPRESS 6 //
  #define HMI_EXIT_CONFIGURATION_KEYPRESS 7 //

  #define FORCED_KEYPRESS_OFFSET                    100
  #define HMI_RIGHT_KEYPRESS_FORCED                 (HMI_RIGHT_KEYPRESS + FORCED_KEYPRESS_OFFSET)
  #define HMI_LEFT_KEYPRESS_FORCED                  (HMI_LEFT_KEYPRESS + FORCED_KEYPRESS_OFFSET)
  #define HMI_UP_KEYPRESS_FORCED                    (HMI_UP_KEYPRESS + FORCED_KEYPRESS_OFFSET)
  #define HMI_DOWN_KEYPRESS_FORCED                  (HMI_DOWN_KEYPRESS + FORCED_KEYPRESS_OFFSET)

  // _DATE_FORMAT
  #define HMI_DATE_FORMAT_DD_MM_YYYY 0 //DD-MM-YYYY
  #define HMI_DATE_FORMAT_MM_DD_YYYY 1 //MM-DD-YYYY
  #define HMI_DATE_FORMAT_YYYY_MM_DD 2 //YYYY-MM-YYYY

  // _HMI_INSTANCE
  #define HMI_LOCAL_INSTANCE 1 //1st instance of HMI (o/p to standard display)
  #define HMI_REMOTE_INSTANCE 2 //2nd instance of HMI (o/p via service port)

  // _CALIBRATION_STATE
  #define HMI_CAL_STATUS_INACTIVE 0 //Auto / Manual calibration is inactive
  #define HMI_CAL_STATUS_WAITING_FOR_STABILITY 1 //Manual Calibration is waiting to be told that i/p signal is stable
  #define HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE 2 //Manual Calibration is waiting for a new process value
  #define HMI_CAL_STATUS_WAITING_FOR_STANDARD_1 3 //Auto Calibration is waiting for 1st standard (buffer solution/test gas) to be applied
  #define HMI_CAL_STATUS_CALCULATING_1 4 //Auto Calibration is caulculating factors for 1st standard (buffer solution/test gas)
  #define HMI_CAL_STATUS_WAITING_FOR_STANDARD_2 5 //Auto Calibration is waiting for 2nd standard (buffer solution/test gas) to be applied
  #define HMI_CAL_STATUS_CALCULATING_2 6 //Auto Calibration is caulculating factors for 2nd standard (buffer solution/test gas)
  #define HMI_CAL_STATUS_SUCCESS 7 //Auto / Manual calibration has completed and successfully calculated new factors (can be accepted/rejected)
  #define HMI_CAL_STATUS_FAILURE 8 //Auto / Manual calibration has completed but could not calculate new factors
  #define HMI_CAL_STATUS_REJECT 9 //Auto / Manual calibration is rejecting any new factors
  #define HMI_CAL_STATUS_ACCEPT 10 //Auto / Manual calibration is saving any new factors
  #define HMI_CAL_STATUS_SUCCESS_NO_REJECT 11 //Auto / Manual calibration has completed and successfully calculated new factors ("Exit" only - cannot  be rejected)
  #define HMI_CAL_STATUS_VIEW_BUFFER_TEMP 12 //2 point pH cal
  #define HMI_CAL_STATUS_EDIT_BUFFER_TEMP 13 //2 point pH cal
  #define HMI_CAL_STATUS_VIEW_BUFFER_1 14 //2 point pH cal
  #define HMI_CAL_STATUS_EDIT_BUFFER_1 15 //2 point pH cal
  #define HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_1 16 //2 point pH cal
  #define HMI_CAL_STATUS_VIEW_BUFFER_2 17 //2 point pH cal
  #define HMI_CAL_STATUS_EDIT_BUFFER_2 18 //2 point pH cal
  #define HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_2 19 //2 point pH cal
  #define HMI_CAL_STATUS_SET_OUTPUT_HOLD 20 //
  #define HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT 21 //
  #define HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT 22 //
  #define HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS 23 //
  #define HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE 24 //
  #define HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED 25 //

  // _CALIBRATION_FAILURE_REASON
  #define HMI_CAL_RESULT_NO_FAILURE 0 //Auto / Manual calibration has completed without errors
  #define HMI_CAL_RESULT_FAIL_UNSTABLE_INPUT 1 //Auto / Manual calibration has failed because i/p signal was not stable
  #define HMI_CAL_RESULT_FAIL_RESULT_OUT_OF_BOUNDS 2 //Auto / Manual calibration has failed because calculated factors were outside specified limits
  #define HMI_CAL_RESULT_FAIL_TEST_GAS_NOT_PRESENT 3 //Auto / Manual calibration has failed because Test Gas in not present
  #define HMI_CAL_RESULT_FAIL_NO_RESPONSE 4 //Auto / Manual calibration has failed because sensor in not responding
  #define HMI_CAL_RESULT_FAIL_NV_ERROR 5 //Auto / Manual calibration has failed because system has failed to save new calibration factors
  #define HMI_CAL_RESULT_FAIL_OFFSET_HIGH 6 //calibration has failed because calculated result was outside specified limits
  #define HMI_CAL_RESULT_FAIL_OFFSET_LOW 7 //calibration has failed because calculated result was outside specified limits
  #define HMI_CAL_RESULT_FAIL_SLOPE_HIGH 8 //calibration has failed because calculated result was outside specified limits
  #define HMI_CAL_RESULT_FAIL_SLOPE_LOW 9 //calibration has failed because calculated result was outside specified limits

  // _LL_HARDWARE_TYPE
  #define HMI_HW_NOT_RECOGNISED 0 //
  #define HMI_HW_NORMAL_X_X 1 //Original version without EEPROM (size, switches etc not defined)
  #define HMI_HW_EEPROM_X_X 2 //Original version with EEPROM (size, switches etc not defined)
  #define HMI_HW_REMOTE_HMI 3 //
  #define HMI_HW_NORMAL_A_STD_SWITCH 4 //HMI Size A
  #define HMI_HW_EEPROM_A_STD_SWITCH 5 //HMI Size A with EEPROM
  #define HMI_HW_NORMAL_A_CAP_SWITCH 6 //HMI Size A with capacitive switches
  #define HMI_HW_EEPROM_A_CAP_SWITCH 7 //HMI Size A with capacitive switches with EEPROM
  #define HMI_HW_NORMAL_A_INDICATOR_ONLY 8 //Size A indicator
  #define HMI_HW_EEPROM_A_INDICATOR_ONLY 9 //Size A indicator with EEPROM
  #define HMI_HW_NORMAL_B_STD_SWITCH 10 //HMI Size B
  #define HMI_HW_EEPROM_B_STD_SWITCH 11 //HMI Size B with EEPROM
  #define HMI_HW_NORMAL_B_HH_TERMINAL 12 //Hand held terminal Size B
  #define HMI_HW_EEPROM_B_HH_TERMINAL 13 //Hand held terminal Size B with EEPROM
  #define HMI_HW_NORMAL_B_CAP_SWITCH 14 //HMI Size B with capacitive switches
  #define HMI_HW_EEPROM_B_CAP_SWITCH 15 //HMI Size B with capacitive switches with EEPROM
  #define HMI_HW_NORMAL_B_INDICATOR_ONLY 16 //Size B indicator
  #define HMI_HW_EEPROM_B_INDICATOR_ONLY 17 //Size B indicator with EEPROM
  #define HMI_HW_EEPROM_F_STD_SWITCH 19 //Size F with standard switches
  #define HMI_HW_SMART 20 //Size B, smartHMI
  #define HMI_HW_NORMAL_B_SIMPLE_INDICATOR 32 //Size B simple indicator
  #define HMI_HW_NOT_AVAILABLE 0xFF //LL HMI not active - Type cannot be determined

  // _ROTATION
  #define HMI_ROTATION_0 0 //
  #define HMI_ROTATION_180 1 //

  // _MENU_FORMAT
  #define HMI_MENU_FORMAT_3_LINES 3 //
  #define HMI_MENU_FORMAT_4_LINES 4 //
  #define HMI_MENU_FORMAT_5_LINES 5 //
  #define HMI_MENU_FORMAT_6_LINES 6 //

  // LANGUAGE_OPTIONS
  #define HMI__LANGUAGE_OPTIONS_LIST LANGUAGE_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_LANGUAGE_OPTIONS
  #define HMI__CUSTOM_LANGUAGE_OPTIONS_LIST CUSTOM_LANGUAGE_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_FORMAT_OPTIONS
  #define HMI__PAGE_1_FORMAT_OPTIONS_LIST PAGE_1_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_LINE_1_OPTIONS
  #define HMI__PAGE_1_LINE_1_OPTIONS_LIST PAGE_1_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_LINE_2_OPTIONS
  #define HMI__PAGE_1_LINE_2_OPTIONS_LIST PAGE_1_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_LINE_3_OPTIONS
  #define HMI__PAGE_1_LINE_3_OPTIONS_LIST PAGE_1_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_LINE_4_OPTIONS
  #define HMI__PAGE_1_LINE_4_OPTIONS_LIST PAGE_1_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_1_LINE_B_OPTIONS
  #define HMI__PAGE_1_LINE_B_OPTIONS_LIST PAGE_1_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_FORMAT_OPTIONS
  #define HMI__PAGE_2_FORMAT_OPTIONS_LIST PAGE_2_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_LINE_1_OPTIONS
  #define HMI__PAGE_2_LINE_1_OPTIONS_LIST PAGE_2_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_LINE_2_OPTIONS
  #define HMI__PAGE_2_LINE_2_OPTIONS_LIST PAGE_2_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_LINE_3_OPTIONS
  #define HMI__PAGE_2_LINE_3_OPTIONS_LIST PAGE_2_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_LINE_4_OPTIONS
  #define HMI__PAGE_2_LINE_4_OPTIONS_LIST PAGE_2_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_2_LINE_B_OPTIONS
  #define HMI__PAGE_2_LINE_B_OPTIONS_LIST PAGE_2_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_FORMAT_OPTIONS
  #define HMI__PAGE_3_FORMAT_OPTIONS_LIST PAGE_3_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_LINE_1_OPTIONS
  #define HMI__PAGE_3_LINE_1_OPTIONS_LIST PAGE_3_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_LINE_2_OPTIONS
  #define HMI__PAGE_3_LINE_2_OPTIONS_LIST PAGE_3_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_LINE_3_OPTIONS
  #define HMI__PAGE_3_LINE_3_OPTIONS_LIST PAGE_3_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_LINE_4_OPTIONS
  #define HMI__PAGE_3_LINE_4_OPTIONS_LIST PAGE_3_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_3_LINE_B_OPTIONS
  #define HMI__PAGE_3_LINE_B_OPTIONS_LIST PAGE_3_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_FORMAT_OPTIONS
  #define HMI__PAGE_4_FORMAT_OPTIONS_LIST PAGE_4_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_LINE_1_OPTIONS
  #define HMI__PAGE_4_LINE_1_OPTIONS_LIST PAGE_4_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_LINE_2_OPTIONS
  #define HMI__PAGE_4_LINE_2_OPTIONS_LIST PAGE_4_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_LINE_3_OPTIONS
  #define HMI__PAGE_4_LINE_3_OPTIONS_LIST PAGE_4_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_LINE_4_OPTIONS
  #define HMI__PAGE_4_LINE_4_OPTIONS_LIST PAGE_4_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_4_LINE_B_OPTIONS
  #define HMI__PAGE_4_LINE_B_OPTIONS_LIST PAGE_4_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_FORMAT_OPTIONS
  #define HMI__PAGE_5_FORMAT_OPTIONS_LIST PAGE_5_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_LINE_1_OPTIONS
  #define HMI__PAGE_5_LINE_1_OPTIONS_LIST PAGE_5_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_LINE_2_OPTIONS
  #define HMI__PAGE_5_LINE_2_OPTIONS_LIST PAGE_5_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_LINE_3_OPTIONS
  #define HMI__PAGE_5_LINE_3_OPTIONS_LIST PAGE_5_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_LINE_4_OPTIONS
  #define HMI__PAGE_5_LINE_4_OPTIONS_LIST PAGE_5_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_5_LINE_B_OPTIONS
  #define HMI__PAGE_5_LINE_B_OPTIONS_LIST PAGE_5_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_FORMAT_OPTIONS
  #define HMI__PAGE_6_FORMAT_OPTIONS_LIST PAGE_6_FORMAT_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_LINE_1_OPTIONS
  #define HMI__PAGE_6_LINE_1_OPTIONS_LIST PAGE_6_LINE_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_LINE_2_OPTIONS
  #define HMI__PAGE_6_LINE_2_OPTIONS_LIST PAGE_6_LINE_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_LINE_3_OPTIONS
  #define HMI__PAGE_6_LINE_3_OPTIONS_LIST PAGE_6_LINE_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_LINE_4_OPTIONS
  #define HMI__PAGE_6_LINE_4_OPTIONS_LIST PAGE_6_LINE_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // PAGE_6_LINE_B_OPTIONS
  #define HMI__PAGE_6_LINE_B_OPTIONS_LIST PAGE_6_LINE_B_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // DIAGNOSIS_VIEW_OPTIONS
  #define HMI__DIAGNOSIS_VIEW_OPTIONS_LIST DIAGNOSIS_VIEW_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_1_OPTIONS
  #define HMI__CUSTOM_DP_1_OPTIONS_LIST CUSTOM_DP_1_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_2_OPTIONS
  #define HMI__CUSTOM_DP_2_OPTIONS_LIST CUSTOM_DP_2_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_3_OPTIONS
  #define HMI__CUSTOM_DP_3_OPTIONS_LIST CUSTOM_DP_3_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_4_OPTIONS
  #define HMI__CUSTOM_DP_4_OPTIONS_LIST CUSTOM_DP_4_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_5_OPTIONS
  #define HMI__CUSTOM_DP_5_OPTIONS_LIST CUSTOM_DP_5_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_6_OPTIONS
  #define HMI__CUSTOM_DP_6_OPTIONS_LIST CUSTOM_DP_6_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_7_OPTIONS
  #define HMI__CUSTOM_DP_7_OPTIONS_LIST CUSTOM_DP_7_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_8_OPTIONS
  #define HMI__CUSTOM_DP_8_OPTIONS_LIST CUSTOM_DP_8_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_9_OPTIONS
  #define HMI__CUSTOM_DP_9_OPTIONS_LIST CUSTOM_DP_9_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_10_OPTIONS
  #define HMI__CUSTOM_DP_10_OPTIONS_LIST CUSTOM_DP_10_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen

  // CUSTOM_DP_11_OPTIONS
  #define HMI__CUSTOM_DP_11_OPTIONS_LIST CUSTOM_DP_11_OPTIONS_LIST //the value according to ConstructorInclude.h, which is create by HMIGen
  //@SubGen end@ <TYPES>
  
#define SP_HMI_PROCESS_HMIFRAME 0
#define SP_HMI_PROCESS_HMIKEY   1

//----------------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//----------------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_HMI_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    HMI_DC_IDX__SFD_SIMPLE,
    HMI_DC_IDX__SFD_SIMPLE_CTW,
    HMI_DC_IDX__SFD_MULTI_OP_CTW,
    HMI_DC_IDX_DYNAMIC,
    HMI_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//----------------------------------------------------------------------------------------------------------
//!  definition of class T_HMI
typedef struct _T_HMI
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    TBOOL (* HMI_EXE)(TUSIGN8 instance);
    void (* HMI_ForceDisplayTimeoutSRV)(TUSIGN8 instance);
    void (* HMI_PowerOnSRV)(TUSIGN8 instance);
    void (* HMI_AllowIOaccessSRV)(TUSIGN8 instance);
    void (* HMI_DenyIOaccessSRV)(TUSIGN8 instance);
    void (* HMI_TransmitInterruptSRV)(void);
    void (* HMI_ReceiveInterruptSRV)(void);
    TBOOL (* HMI_IsIOActiveSRV)(TUSIGN8 instance);
    TBOOL (* HMI_NV_InitCompleteSRV)(void);
    TUSIGN8 (* HMI_GetKeypressSRV)(TUSIGN8 instance);
    TBOOL (* HMI_IsOperatorViewActive)(TUSIGN8 instance);
    TUSIGN8 (* HMI_Get_LLHardwareType_SRV)(TUSIGN8 instance);
    TBOOL (* HMI_GetAutoscrollState_SRV)(TUSIGN8 instance);
    TUSIGN8 (* HMI_GetOperatorPageNumber_SRV)(TUSIGN8 instance);
    void (* HMI_resetConfigurationTimeout)(void);
    TBOOL (* HMI_SetStartupFrame_SRV)(TBOOL autoscrollState, TUSIGN8 operatorPage, TUSIGN8 instance);
    void (* HMI_SetOperatorIcon_SRV)(TUSIGN8 field, TUSIGN8 iconID);
    void (* HMI_SetGraphPointer_SRV)(TUSIGN8 frameNumber, TUSIGN8* pGraph);
    void (* HMI_ToggleBacklight_SRV)(void);
    void (* HMI_SetFunctionKeyLock_SRV)(TBOOL lockState);
    void (* HMI_SetParameterDumpFont_SRV)(TUSIGN8 font);
    void (* HMI_SetOperatorInfo_SRV)(TUSIGN8 infoID);
    TBOOL (* HMI_SetGraphTimebase_SRV)(TUSIGN8 frameNumber, TUSIGN8 timebase);
    void (* HMI_AllowReInit_SRV)(TUSIGN8 InitFlags);
    TUSIGN8 (* HMI_GetLanguageIndex_SRV)(TUSIGN8 language);
    void (* HMI_InitialiseGlobalConfigChangeCounter_SRV)(TUSIGN16 ss_Idx, TUSIGN16 ob_Idx, TINT16 at_Idx);
    void (* HMI_ReInitialiseLowLevelDetection_SRV)(void);
    TBOOL (* HMI_PutPacketBuffer_SRV)(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength);
    //@SubGen end@    <METHODS>
} T_HMI;

//! define the T_HMI-object
extern SLOW const T_HMI hmi;

//! the this-pointer for the T_UNIT-methods in T_HMI
#define HMI_ME (&hmi.unit)

//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define hmi_Get(objectIndex,attributeIndex,ptrValue)                            hmi.unit.Get(HMI_ME,objectIndex,attributeIndex,ptrValue)
#define hmi_Put(objectIndex,attributeIndex,ptrValue)                            hmi.unit.Put(HMI_ME,objectIndex,attributeIndex,ptrValue)
#define hmi_Check(objectIndex,attributeIndex,ptrValue)                          hmi.unit.Check(HMI_ME,objectIndex,attributeIndex,ptrValue)
#define hmi_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor)   hmi.unit.GetAttributeDescription(HMI_ME,objectIndex,attributeIndex,ptrDescriptor)
#define hmi_GetObjectDescription(objectIndex,ptrDescriptor)                     hmi.unit.GetObjectDescription(HMI_ME,objectIndex,ptrDescriptor)

// M. Ashford - Enable MISRA Diagnostics
#pragma diag_default=Pm084
#ifdef _R32C_COMPILER
#pragma diag_default=Pm154
#endif
