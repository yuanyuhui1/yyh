#ifndef _PACKETMANAGERBUILDERSERVICES_H
#define _PACKETMANAGERBUILDERSERVICES_H

//------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------

#ifdef HMI_SUPPORTS_OPERATOR_FRAMES
void PacketManagerBuilderServices_PrepareOperatorPageLineData(TUSIGN8 LineNumber, TUSIGN8 signal_ID, OPERATOR_PAGE_LINE_DATA *LineData, DISPLAYSTYLE Style);
void PacketManagerBuilderServices_AdjustResolutionForRange(const SIGNAL_TABLE_TYPE *pSignalInTable, TUSIGN8 *pResolution);
TUSIGN8 PacketManagerBuilderServices_GetViewIcon(TUSIGN8 frameNo);
#endif

#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)
TBOOL PacketManagerBuilderServices_GetValueAndMakeString_AutoAdjust(TUSIGN16 ssidx,TUSIGN16 objidx,TUSIGN8 attidx,TUSIGN8 maxChars,TUSIGN8 dpPosn,TUSIGN8 *str);
#endif

void PacketManagerBuilderServices_buildSignalText(TUSIGN8 *pSignalText, TUSIGN8 SignalNumber, TBOOL SignalsViewStyle);

#ifdef HMI_USES_DOUBLE
void PacketManagerBuilderServices_GetBarRange(const SIGNAL_TABLE_TYPE *pSignal, TDOUBLE *pBarHigh, TDOUBLE *pBarLow);
#else
void PacketManagerBuilderServices_GetBarRange(const SIGNAL_TABLE_TYPE *pSignal, TFLOAT *pBarHigh, TFLOAT *pBarLow);
#endif

#if defined(HMI_SUPPORTS_MENU_FRAMES) && defined(HMI_USES_COMMONDIAGNOSTICS)
// TUSIGN8 PacketManagerBuilderServices_FindNextSignal(TUSIGN8 item, TUSIGN8 maxItems);
void PacketManagerBuilderServices_GetPartialDiagnosticDescription(const TUSIGN8* inStr, TUSIGN8 start, TUSIGN8 outLen, TUSIGN8* outStr);
#endif


#if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
void PacketManagerBuilderServices_GetTrendRange(const SIGNAL_TABLE_TYPE *pSignal, TFLOAT *pBarHigh, TFLOAT *pBarLow);
#endif


#ifdef COMMON_TOP_WORKS_NEW_FEATURES    //SHOW_LOGIN_LEVEL

#define LOGIN_ICON      0
#define LOGIN_ICON_T    1
#define LOGIN_ICON_B    2

TUSIGN8 PacketManagerBuilderServices_ShowLoginIcon(TUSIGN8 IconID, TUSIGN8 lineNumber, TUSIGN8 part);
#endif

#ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
TBOOL PacketManagerBuilderServices_IndicatorOnly(void);
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES

void Set_NumericField(TFLOAT value, TUSIGN8 dps, TUSIGN8* field, TUSIGN8 fieldSize, TBOOL V_ScaleStyle);
void AddVerticalScale(TUSIGN8 *pDest, TUSIGN8 row, TUSIGN8 column);
void AddMajorDivisions(TUSIGN8 *pDest, TUSIGN8 nDivisions, TUSIGN8 zeroPosn, TUSIGN8 row, TUSIGN8 column);
void plotMerge(TUSIGN8 *pDest, TUSIGN8 *pSource, TUSIGN8 n);
void AddPlots(TUSIGN8 *pDest, const TUSIGN8 *pData, TUSIGN8 row, TUSIGN8 column);
const TUSIGN8* GetPlotData(TUSIGN8 plotNumber);
#endif // end of #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES

#endif  // end of  #ifndef _PACKETMANAGERBUILDERSERVICES_H
