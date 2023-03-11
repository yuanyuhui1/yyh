#ifndef _PACKETMANAGERBUILDER_H
#define _PACKETMANAGERBUILDER_H

#ifdef HMI_SUPPORTS_OPERATOR_FRAMES
TUSIGN8 BuildAdvancedOperatorFrame(void);
    #ifdef LOW_LEVEL_HMI_TEST
    TUSIGN8 BuildLowLevelHMITestFrame(void);
    #endif
    #ifdef HMI_SUPPORTS_GRAPH_VIEW
    TUSIGN8 BuildGraphicalOperatorFrame(TUSIGN8 frameNumber);
    #endif
    #ifdef HMI_SUPPORTS_DISPLAY_MODE_4x9
    TUSIGN8 Build4x9OperatorFrame(TUSIGN8 frameNumber);
    #endif
#endif

#ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
TUSIGN8 BuildGraphViewFrame(TUSIGN8 style);
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
TUSIGN8 BuildWaveformFrame(void);
#endif

#ifdef HMI_SUPPORTS_MATRIX_FRAMES
TUSIGN8 BuildMatrixFrame(TBOOL T_C_D);
#endif

#ifdef HMI_SUPPORTS_CONFIG_HEADER_FRAMES
TUSIGN8 BuildConfigHeader(void);
#endif

#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
TUSIGN8 BuildLinearizerFrame(void);
#endif

#ifdef HMI_SUPPORTS_MENU_FRAMES
TUSIGN8 BuildMenuFrame(void);
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    TUSIGN8 BuidDiagnosticsMenu(TUSIGN8 specialMenuFlag);
    #endif
#endif

#ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
TUSIGN8 BuildNumericalViewFrame(void);
TUSIGN8 BuildNumericalEditFrame(void);
#endif

#ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
TUSIGN8 BuildIPAddressViewFrame(void);
TUSIGN8 BuildIPAddressEditFrame(void);
#endif

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
TUSIGN8 BuildTimeDateViewFrame(void);
TUSIGN8 BuildTimeDateEditFrame(void);
#endif

#if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
TUSIGN8 BuildStringViewFrame(void);
TUSIGN8 BuildStringEditFrame(void);
#endif

#ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
TUSIGN8 BuildEnumEditFrame(void);
TUSIGN8 BuildEnumViewFrame(void);
#endif

#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)
TUSIGN8 BuildAdjustFrame(void);
    #ifdef HMI_SUPPORTS_DISPLAY_TEST
    TUSIGN8 BuildDisplayTest(void);
    #endif
#endif

#ifdef HMI_SUPPORTS_PROGRESS_FRAMES
TUSIGN8 BuildProgressFrame(void);
#endif

#ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
TUSIGN8 BuildTwoPointpHCalFrame(void);
#endif

#ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
TUSIGN8 BuildManualCalFrame(void);
#endif

#ifdef HMI_SUPPORTS_AUTO_CALIBRATION
TUSIGN8 BuildAutoCalFrame(void);
#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
TUSIGN8 BuildPerfomanceLogView(void);
#endif

#ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
TUSIGN8 BuildBitEnumerationFrame(TBOOL edit);
#endif

#ifdef EDP300_SPECIAL_FEATURES_HMI
TUSIGN8 BuildEDP300ModeFrame(void);
TUSIGN8 BuildEDP300QuickAdjustFrame(TBOOL fromConfig);
#endif

#ifdef HMI_SUPPORTS_MANUAL_VALVE_CAL
TUSIGN8 BuildEDP300ManualValveCalFrame(void);
#endif

#endif // #ifndef _PACKETMANAGERBUILDER_H
