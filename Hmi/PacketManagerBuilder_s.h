#if defined(SUPPORT_SHMI)
#ifndef _PACKET_MANAGER_BUILDER_S_H
#define _PACKET_MANAGER_BUILDER_S_H


#define MAX_LENGTH_FRAME_TITLE       17
#define MAX_LENGTH_PARAM_NAME        21
#define MAX_LENGTH_VALUE             20
#define MAX_LENGTH_UNIT              8


//#define HMI_SUPPORTS_ASIAN          1

//==========for test======
#define TEST_FRAME_GRAPH_OPERATOR          1 //=-?
#define TEST_FRAME_OPERATOR_PAGE           1 //=--
#define TEST_FRAME_CONFIG_HEADER           1 //=--
#define TEST_FRAME_MENU_ACCESS_LEVEL       1 //=--
#define TEST_FRAME_MENU_SIGNAL             1 //=--
#define TEST_FRAME_MENU_OPERATOR           1 //=--
#define TEST_FRAME_MENU_DIAGNOSE3          1 //=-- 
#define TEST_FRAME_MENU_DIAGNOSE5          1 //=--
#define TEST_FRAME_MENU_DIAGNOSE6          1 //=--   
#define TEST_FRAME_MENU_USER               1 //=-x     --jonathan
#define TEST_FRAME_NUMERICAL_VIEW          1 //=--
#define TEST_FRAME_NUMERICAL_EDIT          1 //=--
//#define TEST_FRAME_TIMEDATE_VIEW           0     // not support by LMT, while in LWT
//#define TEST_FRAME_TIMEDATE_EDIT           0     // not support by LMT, while in LWT
#define TEST_FRAME_STRING_VIEW             1  //=--      //--adithya       could use the dynamic of enumview
#define TEST_FRAME_STRING_EDIT             1  //=--     //--adithya
#define TEST_FRAME_ENUM_VIEW               1  //=--
#define TEST_FRAME_ENUM_EDIT               1  //=--
#define TEST_FRAME_ADJUST                  1  //=-x 
#define TEST_FRAME_LINEAR                  1  //=-x 
#define TEST_FRAME_WAVEFORM                1  //=-x     --jonathan
#define TEST_FRAME_MANUAL_CALIBRATION      1  //=-x
#define TEST_FRAME_DISPLAY_TEST            1  //=--
#define TEST_FRAME_IPADDRESS_VIEW          1
#define TEST_FRAME_IPADDRESS_EDIT          1

TUSIGN8 BuildErrorResponse_s(void);


#ifdef HMI_SUPPORTS_OPERATOR_FRAMES
TUSIGN8 BuildSFrame_OperatorPage(void);
    #ifdef HMI_SUPPORTS_GRAPH_VIEW
    TUSIGN8 BuildSFrame_GraphicOperator(TUSIGN8 frameNum);
    #endif
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
TUSIGN8 BuildSFrame_Waveform(void);
#endif

#ifdef HMI_SUPPORTS_CONFIG_HEADER_FRAMES
TUSIGN8 BuildSFrame_ConfigHeader(void);
#endif

#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
TUSIGN8 BuildSFrame_Linearizer(void);
#endif

#ifdef HMI_SUPPORTS_MENU_FRAMES
TUSIGN8 BuildSFrame_StdMenu(void);
TUSIGN8 BuildSFrame_OperatorMenu(void);
TUSIGN8 BuildSFrame_AccessLevelMenu(void);
TUSIGN8 BuildSFrame_SignalMenu(void);
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    TUSIGN8 BuildSFrame_DiagnoseMenu3(void);
    TUSIGN8 BuildSFrame_DiagnoseMenu5(TUSIGN8 specialMenuFlag);
    //TUSIGN8 BuildSFrame_DiagnoseMenu6(TUSIGN8 specialMenuFlag);
    #endif
#endif

#ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
TUSIGN8 BuildSFrame_NumericalView(void);
TUSIGN8 BuildSFrame_NumericalEdit(void);
#endif

#ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
TUSIGN8 BuildFrame_IPAddressView(void);
TUSIGN8 BuildFrame_IPAddressEdit(void);
#endif

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
TUSIGN8 BuildSFrame_TimeDateView(void);
TUSIGN8 BuildSFrame_TimeDateEdit(void);
#endif

#if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
TUSIGN8 BuildSFrame_StringView(void);
TUSIGN8 BuildSFrame_StringEdit(void);
#endif

#ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
TUSIGN8 BuildSFrame_EnumEdit(void);
TUSIGN8 BuildSFrame_EnumView(void);
#endif

#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)
TUSIGN8 BuildSFrame_Adjust(void);
    #ifdef HMI_SUPPORTS_DISPLAY_TEST
    TUSIGN8 BuildSFrame_DisplayTest(void);
    #endif
#endif


#ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
TUSIGN8 BuildSFrame_ManulCal(void);
#endif



#endif // #ifndef _PACKETMANAGERBUILDER_H
#endif
