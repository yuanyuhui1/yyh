#ifndef _TRENDVIEW_H
#define _TRENDVIEW_H

#if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)

void TrendViewSetLocalDataPointer(TUSIGN8 newInstance);
TUSIGN8 TrendViewGetStatus(void);
TUSIGN8 TrendViewGetYaxis(TUSIGN8 row, TUSIGN8* pPattern);

#if defined(HMI_SUPPORTS_GRAPH_VIEW)
TBOOL TrendViewSetGraphTimebase(TUSIGN8 frameNumber,TUSIGN8 timebase);
TBOOL TrendViewStartSampling(TUSIGN8 frameNumber, TFLOAT TrendHigh, TFLOAT TrendLow, TUSIGN8 TrendSignal_ss, TUSIGN16 TrendSignal_ob, TINT16 TrendSignal_at);
void TrendViewExecuteSampling(void);
TBOOL TrendViewStopSampling(void);
TUSIGN8 TrendViewGetYaxis(TUSIGN8 row, TUSIGN8* pPattern);
void TrendViewAcknowledgeYaxis(TUSIGN8 row);
TUSIGN8 TrendViewGetXaxis(TUSIGN8 row, TUSIGN8* pPattern);
void TrendViewAcknowledgeXaxis(TUSIGN8 row);
TUSIGN8 TrendViewGetPlot(TUSIGN8 row, TUSIGN8* pPattern, TUSIGN8* pPlotNumber);
void TrendViewAcknowledgePlot(TUSIGN8 row);
#endif

#if defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
TBOOL TrendViewStartArrayCalculation(TUSIGN8 ArraySignal_ss,TUSIGN16 ArraySignal_ob);
TBOOL TrendViewStopArrayCalculation(void);
TUSIGN8 TrendViewGetYaxis(TUSIGN8 row, TUSIGN8* pPattern);
TBOOL TrendViewGet25plots(TUSIGN8 firstPlot, TUSIGN8 row, TUSIGN8* pPattern, TUSIGN8 style);

#endif // end of #if defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)

#endif  // end of #if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)

#endif  // end of #ifndef _TRENDVIEW_H
