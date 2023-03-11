#pragma diag_suppress=Go003

#include <intrinsics.h>
#include "IncludeList_Implementation.h"

#if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)


#include <math.h>
#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif

#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"

#include "TrendView.h"

#define PLOT_BUFFER_SIZE    20

TUSIGN8 GraphTimebase_1 = HMI_TRENDVIEW_TIMEBASE_1_MINUTE;
TUSIGN8 GraphTimebase_2 = HMI_TRENDVIEW_TIMEBASE_1_MINUTE;
TUSIGN8 GraphTimebase_3 = HMI_TRENDVIEW_TIMEBASE_1_MINUTE;
TUSIGN8 GraphTimebase_4 = HMI_TRENDVIEW_TIMEBASE_1_MINUTE;

const TUSIGN8 yAxisMax = 38;    // Top of yAxis
const TUSIGN8 yAxisMin = 0;     // Bottom of yAxis

typedef struct
{
    TUSIGN8 plotNumber;
    TUSIGN8 plotPosition_y;
    TUSIGN8 plot_flags;
} PLOT_BUFFER_ENTRY;

typedef struct
{
    // elements that must be initialised
    TUSIGN8             Status;                         // Current status of trend view system
    TFLOAT              yRangeMax;                      // Maximum value of y axis
    TFLOAT              yRangeMin;                      // Minimum value of Y axis
    TUSIGN16            sampleInterval;                 // Time between samples (mS)
    TUSIGN8             yAxis_x;                        // position of xAxis on yAxis

    TINT16              PlotBufferPutIndex;             // "points to" next Vacant plot buffer
    TINT16              PlotBufferGetIndex;             // "points to" plot buffer not yet sent to HMI
    TBOOL               waitForTimerWrap;               // flag to indicate that next sample will be taken after OS timer has overflowed

    // elements that need not be initialised
    TUSIGN8             TrendSubSystemIndex;            // subsystem index of signal being sampled
    TUSIGN16            TrendObjectIndex;               // object index of signal being sampled
    TINT16              TrendAttributeIndex;            // attribute index of signal being sampled
    PLOT_BUFFER_ENTRY   plotBuffer[PLOT_BUFFER_SIZE];   // buffer of plot positions
    TUSIGN8             yAxis_plotFlags;                // flags to indicate that y axis rows have been plotted on HMI
    TUSIGN8             xAxis_plotFlags;                // flags to indicate that x axis rows have been plotted on HMI
    TUSIGN8             HighAxisText[6];                // Text string - Maximum value of y axis
    TUSIGN8             LowAxisText[6];                 // Text string - Minimum value of y axis
    TUSIGN8             nSamplesTaken;                  // number of sample taken 0..99
    TUSIGN8             nSamplesPlotted;                // number of samples plotted 0n HMI 0..99
    TUSIGN16            lastExeTime;                    // Time when TrendViewExecuteSampling() was last executed
    TUSIGN16            nextSampleTime;                 // Time when next sample will be taken
    TUSIGN8             nArrayItems;                    // number of items in the array
    TUSIGN8             Array_dataType;                 // data type of the data in the array
}TRENDVIEW_LOCAL_TYPE;

#pragma diag_suppress=Pm023
TRENDVIEW_LOCAL_TYPE    TrendViewLocal_1 =
{
    HMI_TRENDVIEW_STATUS_INACTIVE,  // TUSIGN8 Status;              // Current status of trend view system
    0.0F,                           // TFLOAT  yRangeMax;           // Maximum value of y axis
    0.0F,                           // TFLOAT  yRangeMin;           // Minimum value of Y axis
    600,                            // TUSIGN16 sampleInterval;     // Time between samples (mS)
    0,                              // TUSIGN8 yAxis_x;             // position of xAxis on yAxis
    0,                              // TINT16 PlotBufferPutIndex;   // "points to" next Vacant plot buffer
    0,                              // TINT16 PlotBufferGetIndex;   // "points to" plot buffer not yet sent to HMI
    eFALSE,                         // TBOOL waitForTimerWrap       // flag to indicate that next sample will be taken after OS timer has overflowed
};
#pragma diag_default=Pm023

#ifdef HMI_SUPPORTS_SERVICEPORT
#pragma diag_suppress=Pm023
TRENDVIEW_LOCAL_TYPE    TrendViewLocal_2 =
{
    HMI_TRENDVIEW_STATUS_INACTIVE,  // TUSIGN8 Status;              // Current status of trend view system
    0.0F,                           // TFLOAT  yRangeMax;           // Maximum value of y axis
    0.0F,                           // TFLOAT  yRangeMin;           // Minimum value of Y axis
    600,                            // TUSIGN16 sampleInterval;     // Time between samples (mS)
    0,                              // TUSIGN8 yAxis_x;             // position of xAxis on yAxis
    0,                              // TINT16 PlotBufferPutIndex;   // "points to" next Vacant plot buffer
    0,                              // TINT16 PlotBufferGetIndex;   // "points to" plot buffer not yet sent to HMI
    eFALSE,                         // TBOOL waitForTimerWrap       // flag to indicate that next sample will be taken after OS timer has overflowed
};
#pragma diag_default=Pm023
#endif


TRENDVIEW_LOCAL_TYPE *pTrendViewLocal = &TrendViewLocal_1;

#ifdef HMI_SUPPORTS_SERVICEPORT
#define LOCAL_Status                pTrendViewLocal->Status                 // Current status of trend view system
#define LOCAL_yRangeMax             pTrendViewLocal->yRangeMax              // Maximum value of y axis
#define LOCAL_yRangeMin             pTrendViewLocal->yRangeMin              // Minimum value of Y axis
#define LOCAL_sampleInterval        pTrendViewLocal->sampleInterval         // Time between samples (mS)
#define LOCAL_yAxis_x               pTrendViewLocal->yAxis_x                // position of xAxis on yAxis
#define LOCAL_PlotBufferPutIndex    pTrendViewLocal->PlotBufferPutIndex     // "points to" next Vacant plot buffer
#define LOCAL_PlotBufferGetIndex    pTrendViewLocal->PlotBufferGetIndex     // "points to" plot buffer not yet sent to HMI
#define LOCAL_waitForTimerWrap      pTrendViewLocal->waitForTimerWrap       // flag to indicate that next sample will be taken after OS timer has overflowed
#define LOCAL_TrendSubSystemIndex   pTrendViewLocal->TrendSubSystemIndex    // subsystem index of signal being sampled
#define LOCAL_TrendObjectIndex      pTrendViewLocal->TrendObjectIndex       // object index of signal being sampled
#define LOCAL_TrendAttributeIndex   pTrendViewLocal->TrendAttributeIndex    // attribute index of signal being sampled
#define LOCAL_plotBuffer            pTrendViewLocal->plotBuffer             // buffer of plot positions
#define LOCAL_yAxis_plotFlags       pTrendViewLocal->yAxis_plotFlags        // flags to indicate that y axis rows have been plotted on HMI
#define LOCAL_xAxis_plotFlags       pTrendViewLocal->xAxis_plotFlags        // flags to indicate that x axis rows have been plotted on HMI
#define LOCAL_HighAxisText          pTrendViewLocal->HighAxisText           // Text string - Maximum value of y axis
#define LOCAL_LowAxisText           pTrendViewLocal->LowAxisText            // Text string - Minimum value of y axis
#define LOCAL_nSamplesTaken         pTrendViewLocal->nSamplesTaken          // number of sample taken 0..99
#define LOCAL_nSamplesPlotted       pTrendViewLocal->nSamplesPlotted        // number of samples plotted 0n HMI 0..99
#define LOCAL_lastExeTime           pTrendViewLocal->lastExeTime            // Time when TrendViewExecuteSampling() was last executed
#define LOCAL_nextSampleTime        pTrendViewLocal->nextSampleTime         // Time when next sample will be taken
#define LOCAL_nArrayItems           pTrendViewLocal->nArrayItems            // number of items in the array
#define LOCAL_Array_dataType        pTrendViewLocal->Array_dataType         // data type of the data in the array

#else // else condition for #ifdef HMI_SUPPORTS_SERVICEPORT

#define LOCAL_Status                TrendViewLocal_1.Status                 // Current status of trend view system
#define LOCAL_yRangeMax             TrendViewLocal_1.yRangeMax              // Maximum value of y axis
#define LOCAL_yRangeMin             TrendViewLocal_1.yRangeMin              // Minimum value of Y axis
#define LOCAL_sampleInterval        TrendViewLocal_1.sampleInterval         // Time between samples (mS)
#define LOCAL_yAxis_x               TrendViewLocal_1.yAxis_x                // position of xAxis on yAxis
#define LOCAL_PlotBufferPutIndex    TrendViewLocal_1.PlotBufferPutIndex     // "points to" next Vacant plot buffer
#define LOCAL_PlotBufferGetIndex    TrendViewLocal_1.PlotBufferGetIndex     // "points to" plot buffer not yet sent to HMI
#define LOCAL_waitForTimerWrap      TrendViewLocal_1.waitForTimerWrap       // flag to indicate that next sample will be taken after OS timer has overflowed
#define LOCAL_TrendSubSystemIndex   TrendViewLocal_1.TrendSubSystemIndex    // subsystem index of signal being sampled
#define LOCAL_TrendObjectIndex      TrendViewLocal_1.TrendObjectIndex       // object index of signal being sampled
#define LOCAL_TrendAttributeIndex   TrendViewLocal_1.TrendAttributeIndex    // attribute index of signal being sampled
#define LOCAL_plotBuffer            TrendViewLocal_1.plotBuffer             // buffer of plot positions
#define LOCAL_yAxis_plotFlags       TrendViewLocal_1.yAxis_plotFlags        // flags to indicate that y axis rows have been plotted on HMI
#define LOCAL_xAxis_plotFlags       TrendViewLocal_1.xAxis_plotFlags        // flags to indicate that x axis rows have been plotted on HMI
#define LOCAL_HighAxisText          TrendViewLocal_1.HighAxisText           // Text string - Maximum value of y axis
#define LOCAL_LowAxisText           TrendViewLocal_1.LowAxisText            // Text string - Minimum value of y axis
#define LOCAL_nSamplesTaken         TrendViewLocal_1.nSamplesTaken          // number of sample taken 0..99
#define LOCAL_nSamplesPlotted       TrendViewLocal_1.nSamplesPlotted        // number ofsamples plotted 0n HMI 0..99
#define LOCAL_lastExeTime           TrendViewLocal_1.lastExeTime            // Time when TrendViewExecuteSampling() was last executed
#define LOCAL_nextSampleTime        TrendViewLocal_1.nextSampleTime         // Time when next sample will be taken
#define LOCAL_nArrayItems           TrendViewLocal_1.nArrayItems            // number of items in the array
#define LOCAL_Array_dataType        TrendViewLocal_1.Array_dataType         // data type of the data in the array
#endif // end of else condition for #ifdef HMI_SUPPORTS_SERVICEPORT

#define NO_SAMPLE       0x80    //1xxxxxxx
#define ALL_PLOTTED     0x1F    //xxx11111
#define NONE_PLOTTED    0x00    //xxx00000

#define TIME_MARKER_PATTERN     0x55

const TUSIGN8 plot_flag_set[5] =
{
    0x01,   // xxx00001 - row 1
    0x02,   // xxx00010 - row 2
    0x04,   // xxx00100 - row 3
    0x08,   // xxx01000 - row 4
    0x10    // xxx10000 - row 5
};

const TUSIGN8 plot_flag_clear[5] =
{
    0xFE,   // xxx11110 - row 1
    0xFD,   // xxx11101 - row 2
    0xFB,   // xxx11011 - row 3
    0xF7,   // xxx10111 - row 4
    0xEF    // xxx01111 - row 5
};


const TUSIGN8 dotMask[40][5] =
{
// Row:    0     1     2     3     4
        { 0x40, 0x00, 0x00, 0x00, 0x00},    // 0
        { 0x20, 0x00, 0x00, 0x00, 0x00},    // 1
        { 0x10, 0x00, 0x00, 0x00, 0x00},    // 2
        { 0x08, 0x00, 0x00, 0x00, 0x00},    // 3
        { 0x04, 0x00, 0x00, 0x00, 0x00},    // 4
        { 0x02, 0x00, 0x00, 0x00, 0x00},    // 5
        { 0x01, 0x00, 0x00, 0x00, 0x00},    // 6
        { 0x00, 0x80, 0x00, 0x00, 0x00},    // 7
        { 0x00, 0x40, 0x00, 0x00, 0x00},    // 8
        { 0x00, 0x20, 0x00, 0x00, 0x00},    // 9
        { 0x00, 0x10, 0x00, 0x00, 0x00},    // 10
        { 0x00, 0x08, 0x00, 0x00, 0x00},    // 11
        { 0x00, 0x04, 0x00, 0x00, 0x00},    // 12
        { 0x00, 0x02, 0x00, 0x00, 0x00},    // 13
        { 0x00, 0x01, 0x00, 0x00, 0x00},    // 14
        { 0x00, 0x00, 0x80, 0x00, 0x00},    // 15
        { 0x00, 0x00, 0x40, 0x00, 0x00},    // 16
        { 0x00, 0x00, 0x20, 0x00, 0x00},    // 17
        { 0x00, 0x00, 0x10, 0x00, 0x00},    // 18
        { 0x00, 0x00, 0x08, 0x00, 0x00},    // 19
        { 0x00, 0x00, 0x04, 0x00, 0x00},    // 20
        { 0x00, 0x00, 0x02, 0x00, 0x00},    // 21
        { 0x00, 0x00, 0x01, 0x00, 0x00},    // 22
        { 0x00, 0x00, 0x00, 0x80, 0x00},    // 23
        { 0x00, 0x00, 0x00, 0x40, 0x00},    // 24
        { 0x00, 0x00, 0x00, 0x20, 0x00},    // 25
        { 0x00, 0x00, 0x00, 0x10, 0x00},    // 26
        { 0x00, 0x00, 0x00, 0x08, 0x00},    // 27
        { 0x00, 0x00, 0x00, 0x04, 0x00},    // 28
        { 0x00, 0x00, 0x00, 0x02, 0x00},    // 29
        { 0x00, 0x00, 0x00, 0x01, 0x00},    // 30
        { 0x00, 0x00, 0x00, 0x00, 0x80},    // 31
        { 0x00, 0x00, 0x00, 0x00, 0x40},    // 32
        { 0x00, 0x00, 0x00, 0x00, 0x20},    // 33
        { 0x00, 0x00, 0x00, 0x00, 0x10},    // 34
        { 0x00, 0x00, 0x00, 0x00, 0x08},    // 35
        { 0x00, 0x00, 0x00, 0x00, 0x04},    // 36
        { 0x00, 0x00, 0x00, 0x00, 0x02},    // 37
        { 0x00, 0x00, 0x00, 0x00, 0x01},    // 38
        { 0x00, 0x00, 0x00, 0x00, 0x00}     // Invalid
};

const TUSIGN8 tinyFont[15][3] =
{
        { 0x3E, 0x22, 0x3E}, // 0        // TINYFONT_0
        { 0x24, 0x3E, 0x20}, // 1        // TINYFONT_1
        { 0x3A, 0x2A, 0x2E}, // 2        // TINYFONT_2
        { 0x22, 0x2A, 0x3E}, // 3        // TINYFONT_3
        { 0x0E, 0x08, 0x3E}, // 4        // TINYFONT_4
        { 0x2E, 0x2A, 0x3A}, // 5        // TINYFONT_5
        { 0x3C, 0x2A, 0x28}, // 6        // TINYFONT_6
        { 0x32, 0x0A, 0x06}, // 7        // TINYFONT_7
        { 0x3E, 0x2A, 0x3E}, // 8        // TINYFONT_8
        { 0x0E, 0x2A, 0x8E}, // 9        // TINYFONT_9
        { 0x00, 0x10, 0x00}, // .        // TINYFONT_DP
        { 0x08, 0x08, 0x08}, // -        // TINYFONT_MINUS
        { 0x04, 0x32, 0x0C}, // ?        // TINYFONT_QUESTION
        { 0x04, 0x3E, 0x04}, // up arrow // TINYFONT_ARROW
        { 0x00, 0x00, 0x00}  // space    // TINYFONT_SPACE
};

typedef enum
{
        TINYFONT_0,         //  0 = 0
        TINYFONT_1,         //  1 = 1
        TINYFONT_2,         //  2 = 2
        TINYFONT_3,         //  3 = 3
        TINYFONT_4,         //  4 = 4
        TINYFONT_5,         //  5 = 5
        TINYFONT_6,         //  6 = 6
        TINYFONT_7,         //  7 = 7
        TINYFONT_8,         //  8 = 8
        TINYFONT_9,         //  9 = 9
        TINYFONT_DP,        // 10 = .
        TINYFONT_MINUS,     // 11 = -
        TINYFONT_QUESTION,  // 12 = ?
        TINYFONT_ARROW,     // 13 = up arrow
        TINYFONT_SPACE      // 14 = space
}TINYFONT_TYPE;

#if defined(SIXTH_OPERATOR_PAGE)
#define MAX_FRAMENUMBER     6
#elif defined(FIFTH_OPERATOR_PAGE)
#define MAX_FRAMENUMBER     5
#elif defined(FOURTH_OPERATOR_PAGE)
#define MAX_FRAMENUMBER     4
#elif defined(THIRD_OPERATOR_PAGE)
#define MAX_FRAMENUMBER     3
#elif defined(SECOND_OPERATOR_PAGE)
#define MAX_FRAMENUMBER     2
#else
#define MAX_FRAMENUMBER     1
#endif

#define MINIMUM_TIMEBASE    HMI_TRENDVIEW_TIMEBASE_1_MINUTE
#define MAXIMUM_TIMEBASE    HMI_TRENDVIEW_TIMEBASE_30_MINUTE

#define MAX_PLOTS           100

// --- Local functions -----------------------------------------------------------------------------
TFLOAT CalculateAxis(TFLOAT AxisValue_f, TUSIGN8 *pAxisText, TBOOL HighNotLow);

#if defined(HMI_SUPPORTS_GRAPH_VIEW)
TFLOAT GetSample(void);
void addPlotToBuffer(TUSIGN8 SampleNumber, TUSIGN8 plotPosition);
#endif

#if defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
TFLOAT calculatePlotLevel(TUSIGN8 plotNumber, TUSIGN8 style);
#endif


TUSIGN8 calculatePlotPosition(TFLOAT Sample);
void addAxisTextToOutputPattern(TUSIGN8 *pAxisText,TUSIGN8 *pPattern);

//--------------------------------------------------------------------------------------------------
// brief: Sets Local data pointer to local data relavent to appropriate instance of HMI
//
// No restriction on the optimization of this function
void TrendViewSetLocalDataPointer(TUSIGN8 newInstance)
{
    #ifdef HMI_SUPPORTS_SERVICEPORT

    if      (newInstance == 1)  {pTrendViewLocal = &TrendViewLocal_1;}
    else if (newInstance == 2)  {pTrendViewLocal = &TrendViewLocal_2;}
    else                        {VIP_ASSERT(0);}

    #else // else condition for #ifdef HMI_SUPPORTS_SERVICEPORT

    pTrendViewLocal = &TrendViewLocal_1;

    #endif // end of else condition for #ifdef HMI_SUPPORTS_SERVICEPORT
}

//--------------------------------------------------------------------------------------------------
// brief: Returns Current Status of the Trend view system
//
// No restriction on the optimization of this function
TUSIGN8 TrendViewGetStatus(void)
{
    return LOCAL_Status;
}


//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Sets Timebase used on operator pages of type HMI_DISPLAY_MODE_GRAPHIC
//
// No restriction on the optimization of this function
TBOOL TrendViewSetGraphTimebase(TUSIGN8 frameNumber, TUSIGN8 timebase)
{
    TBOOL ReturnVal = eFALSE;

    if ((timebase >= MINIMUM_TIMEBASE)
     && (timebase <= MAXIMUM_TIMEBASE))
    {
        ReturnVal = eTRUE;
        switch (frameNumber)
        {
        case 1:     GraphTimebase_1 = timebase;     break;
        case 2:     GraphTimebase_2 = timebase;     break;
        case 3:     GraphTimebase_3 = timebase;     break;
        case 4:     GraphTimebase_4 = timebase;     break;
        default:    ReturnVal = eFALSE;             break;
        }
    }
    return ReturnVal;
}
#endif

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Starts Sampling
//
// No restriction on the optimization of this function
TBOOL TrendViewStartSampling(TUSIGN8 frameNumber, TFLOAT TrendHigh, TFLOAT TrendLow, TUSIGN8 TrendSignal_ss, TUSIGN16 TrendSignal_ob, TINT16 TrendSignal_at)
{
    TBOOL returnVal;

    TFLOAT TrendValue;
    T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)TrendSignal_ss);

    if ((pSub->Get(pSub,TrendSignal_ob,TrendSignal_at,(void FAST *)&TrendValue) != (TUSIGN16)OK)    // cannot start sampling if TrendSignal is not valid
     || (LOCAL_Status != HMI_TRENDVIEW_STATUS_INACTIVE)                                             // cannot start sampling if sampling is not inactive
     || (frameNumber < 1) || (frameNumber > MAX_FRAMENUMBER))                                       // cannot start sampling if frame number is not valid
    {
        returnVal = eFALSE;
    }
    else if (((frameNumber == 1) && ((GraphTimebase_1 > MAXIMUM_TIMEBASE) || (GraphTimebase_1 < MINIMUM_TIMEBASE) || (SFD_SIMPLE.displayMode_1   != HMI_DISPLAY_MODE_GRAPHIC)))
          || ((frameNumber == 2) && ((GraphTimebase_2 > MAXIMUM_TIMEBASE) || (GraphTimebase_2 < MINIMUM_TIMEBASE) || (SFD_MULTI_OP.displayMode_2 != HMI_DISPLAY_MODE_GRAPHIC)))
          || ((frameNumber == 3) && ((GraphTimebase_3 > MAXIMUM_TIMEBASE) || (GraphTimebase_3 < MINIMUM_TIMEBASE) || (SFD_MULTI_OP.displayMode_3 != HMI_DISPLAY_MODE_GRAPHIC)))
          || ((frameNumber == 4) && ((GraphTimebase_4 > MAXIMUM_TIMEBASE) || (GraphTimebase_4 < MINIMUM_TIMEBASE) || (SFD_MULTI_OP.displayMode_4 != HMI_DISPLAY_MODE_GRAPHIC))))
    {
    // cannot start sampling if timebase is invalid or display mode incorrect
        returnVal = eFALSE;
    }
    else
    {
        // Calculate Y axis range
        if (TrendLow > TrendHigh)
        {
            TFLOAT swap = TrendLow;
            TrendLow = TrendHigh;
            TrendHigh = swap;
        }

        LOCAL_yRangeMax = CalculateAxis(TrendHigh, LOCAL_HighAxisText, eTRUE);
        LOCAL_yRangeMin = CalculateAxis(TrendLow,  LOCAL_LowAxisText,  eFALSE);

        LOCAL_yAxis_plotFlags = NONE_PLOTTED;

        // Calculate zero position
        LOCAL_yAxis_x = calculatePlotPosition(0.0F);
        LOCAL_xAxis_plotFlags = NONE_PLOTTED;

        // calculate sample interval
        TUSIGN8 timebase;

        switch (frameNumber)
        {
        case 1:     timebase = GraphTimebase_1;     break;
        case 2:     timebase = GraphTimebase_2;     break;
        case 3:     timebase = GraphTimebase_3;     break;
        case 4:     timebase = GraphTimebase_4;     break;
        default:    VIP_ASSERT(0);                  break;
        }

        switch (timebase)
        {

        //                      timebase(mS)         timebase(min) * 60 * 1000
        // sample interval = -------------------  = ----------------------------
        //                    number of samples               100

        case HMI_TRENDVIEW_TIMEBASE_30_MINUTE:          LOCAL_sampleInterval = 18000;   break;
        case HMI_TRENDVIEW_TIMEBASE_20_MINUTE:          LOCAL_sampleInterval = 12000;   break;
        case HMI_TRENDVIEW_TIMEBASE_10_MINUTE:          LOCAL_sampleInterval = 6000;    break;
        case HMI_TRENDVIEW_TIMEBASE_5_MINUTE:           LOCAL_sampleInterval = 3000;    break;
        case HMI_TRENDVIEW_TIMEBASE_2_MINUTE:           LOCAL_sampleInterval = 1200;    break;
        default: /* HMI_TRENDVIEW_TIMEBASE_1_MINUTE */  LOCAL_sampleInterval = 600;     break;
        }

        // Adjust sample interval for OS Tick not equal to 1mS
        #ifdef OS_TICK_DIV
        LOCAL_sampleInterval /= OS_TICK_DIV;
        #endif

        // Set Trend signal indeces
        LOCAL_TrendSubSystemIndex = TrendSignal_ss;
        LOCAL_TrendObjectIndex    = TrendSignal_ob;
        LOCAL_TrendAttributeIndex = TrendSignal_at;

        // SetSampleNumber
        //hMI_TrendViewDynamic.SampleNumber = 0;
        LOCAL_nSamplesTaken = 0;
        LOCAL_nSamplesPlotted = 0;

        // Reset Plot position buffer
        TUSIGN8 i;
        for (i = 0; i < PLOT_BUFFER_SIZE; i ++)
        {
            //LOCAL_plotBuffer[i].plotNumber = NO_SAMPLE;
            LOCAL_plotBuffer[i].plot_flags = NO_SAMPLE;
        }
        LOCAL_PlotBufferPutIndex = 0;
        LOCAL_PlotBufferGetIndex = 0;

        // Sampling system is now active
        LOCAL_Status = HMI_TRENDVIEW_STATUS_SAMPLING;

        returnVal = eTRUE;
    }
    return returnVal;
}
#endif

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Stops Sampling
//
// No restriction on the optimization of this function
TBOOL TrendViewStopSampling(void)
{
    TBOOL returnVal = eFALSE;

    if (LOCAL_Status == HMI_TRENDVIEW_STATUS_SAMPLING)
    {
        LOCAL_Status = HMI_TRENDVIEW_STATUS_INACTIVE;
        returnVal = eTRUE;
    }
    return returnVal;
}
#endif

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Performs the sampling operation
//
// No restriction on the optimization of this function
void TrendViewExecuteSampling(void)
{
    if (LOCAL_Status == HMI_TRENDVIEW_STATUS_SAMPLING)
    {
        TUSIGN16 OSTime = GetTickCount_RTOS_TASK();

        if (LOCAL_nSamplesTaken == 0)
        {
            addPlotToBuffer(LOCAL_nSamplesTaken,calculatePlotPosition(GetSample()));
            LOCAL_nSamplesTaken ++;

            TUSIGN16 thisSampleTime = OSTime;
            LOCAL_nextSampleTime = thisSampleTime + LOCAL_sampleInterval;
            LOCAL_waitForTimerWrap = LOCAL_nextSampleTime > thisSampleTime ? eFALSE : eTRUE;
        }

        if (OSTime < LOCAL_lastExeTime)
        {
            LOCAL_waitForTimerWrap = eFALSE;
        }

        if ((OSTime >= LOCAL_nextSampleTime) && (LOCAL_waitForTimerWrap == eFALSE))
        {
            addPlotToBuffer(LOCAL_nSamplesTaken,calculatePlotPosition(GetSample()));
            LOCAL_nSamplesTaken ++;

            TUSIGN16 thisSampleTime = LOCAL_nextSampleTime;
            LOCAL_nextSampleTime += LOCAL_sampleInterval;
            LOCAL_waitForTimerWrap = LOCAL_nextSampleTime > thisSampleTime ? eFALSE : eTRUE;
        }

        LOCAL_nSamplesTaken = LOCAL_nSamplesTaken % 100;

        LOCAL_lastExeTime = OSTime;

    }
}
#endif

//--------------------------------------------------------------------------------------------------
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Returns the array data for one row of the y axis
//
// No restriction on the optimization of this function
TUSIGN8 TrendViewGetYaxis(TUSIGN8 row, TUSIGN8* pPattern)
{
    TUSIGN8 arrayLength = 0;
    if ((LOCAL_yAxis_plotFlags & plot_flag_set[row]) == 0)  // Only send data if it has not been acknowledged by HMI
    {
        switch(row)
        {
        case 4:    // Build Graph Row 4 (Top Row of graph)
            // y axis value ( pPattern[0] - pPattern[23]
            addAxisTextToOutputPattern(LOCAL_HighAxisText,pPattern);
            // "grid line" markers (pPattern[23]
            pPattern[23] = dotMask[yAxisMax][row]       // yRangeMax marker &
                         | dotMask[LOCAL_yAxis_x][row]; // x axis marker

            // y axis (pPattern[24])
            pPattern[24] = 0xFF;                        // y axis

            // length (return value)
            arrayLength = 25;
            break;

        case 3:
        case 2:
        case 1:
            // grid line markers (pPattern[0]
            pPattern[0] = dotMask[LOCAL_yAxis_x][row];  // x axis marker

            // y axis (pPattern[1])
            pPattern[1] = 0xFF;                         // y axis

            // length (return value)
            arrayLength = 2;
            break;

        case 0:    // Build Graph Row 0 (Lowest Row of graph)
            // y axis value ( pPattern[0] - pPattern[23]
            addAxisTextToOutputPattern(LOCAL_LowAxisText,pPattern);

            // "grid line" markers (pPattern[23]
            pPattern[23] = dotMask[yAxisMin][row]       // yRangeMin marker &
                         | dotMask[LOCAL_yAxis_x][row]; // x axis marker

            // y axis (pPattern[24])
            pPattern[24] = 0x7F;                        // y axis

            // length (return value)
            arrayLength = 25;
            break;

        default:
            arrayLength = 0;
            break;
        }
    }
    return arrayLength;
}

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Acknowledges that the array data for this row of the y axis has been plotted
//
// No restriction on the optimization of this function
void TrendViewAcknowledgeYaxis(TUSIGN8 row)
{
    if (row <= 4)
    {
        // set "axis plotted" flag for this row
        LOCAL_yAxis_plotFlags |= plot_flag_set[row];
    }
}
#endif

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Returns the array data for one row of the x axis
//
// No restriction on the optimization of this function
TUSIGN8 TrendViewGetXaxis(TUSIGN8 row, TUSIGN8* pPattern)
{
    TUSIGN8 arrayLength = 0;

    if ((row <= 4)                                              // cannot return x axis if row number is invalid
     && (dotMask[LOCAL_yAxis_x][row] != 0x00)                   // need not return x axis if x axis is not on this row
     && ((LOCAL_xAxis_plotFlags & plot_flag_set[row]) == 0))    // need not return x axis if it has already been acknowledged by HMI
    {
        *pPattern = dotMask[LOCAL_yAxis_x][row];                // x axis marker
        arrayLength = 1;
    }
    return arrayLength;
}
#endif

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Acknowledges that the array data for this row of the x axis has been plotted
//
// No restriction on the optimization of this function
void TrendViewAcknowledgeXaxis(TUSIGN8 row)
{
    if (row <= 4)
    {
        // set "axis plotted" flag for this row
        LOCAL_xAxis_plotFlags |= plot_flag_set[row];
    }
}
#endif

//-------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Returns the array data for one row of plot data
//
// No restriction on the optimization of this function
TUSIGN8 TrendViewGetPlot(TUSIGN8 row, TUSIGN8* pPattern, TUSIGN8* pPlotNumber)
{
    TUSIGN8 arrayLength = 0;
    TUSIGN8 pattern = 0x00;

    if (row > 4)                                                                                    // cannot return data if row number is invalid
    {
        arrayLength = 0;
    }
    else if (LOCAL_PlotBufferGetIndex == LOCAL_PlotBufferPutIndex)                                  // cannot return data in sample not yet taken
    {
        arrayLength = 0;
    }
    else if ((LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plot_flags & plot_flag_set[row]) != 0)     // cannot return data if data already acknowledged
    {
        arrayLength = 0;
    }
    else if (LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plot_flags == NO_SAMPLE)
    {
        arrayLength = 0;
    }
    else
    {
        // first plot
        if (LOCAL_nSamplesPlotted == 0)
        {
            TUSIGN8 FirstPlotPosition = LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plotPosition_y;
            pattern = dotMask[FirstPlotPosition][row];
        }
        else
        {
            // get previous position
            TINT16 PlotBufferGetIndex_previousPlot = LOCAL_PlotBufferGetIndex - 1;
            if (PlotBufferGetIndex_previousPlot < 0)
            {
                PlotBufferGetIndex_previousPlot = PLOT_BUFFER_SIZE - 1;
            }

            TINT16 OldPlotPosition = LOCAL_plotBuffer[PlotBufferGetIndex_previousPlot].plotPosition_y;
            TINT16 NewPlotPosition = LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plotPosition_y;

            if (NewPlotPosition > OldPlotPosition + 1)
            {
                // step change up - set every pixel from 1 above old position up to and including the new position
                TINT16 PlotPosition;
                for (PlotPosition = OldPlotPosition + 1; PlotPosition <= NewPlotPosition; PlotPosition ++)
                {
                    pattern |= dotMask[PlotPosition][row];
                }
            }
            else if (NewPlotPosition < OldPlotPosition - 1)
            {
                // step change down - set every pixel from 1 below old position down to and including the new position...
                TINT16 PlotPosition;
                for (PlotPosition = OldPlotPosition - 1; PlotPosition >= NewPlotPosition; PlotPosition --)
                {
                    pattern |= dotMask[PlotPosition][row];
                }
            }
            else
            {
                // slow ramp or steady signal - set pixel relating to new position
                pattern = dotMask[NewPlotPosition][row];
            }

            // set data for HMI
            *pPlotNumber = LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plotNumber;

            pPattern[0] = dotMask[LOCAL_yAxis_x][row] | pattern;
            pPattern[1] = dotMask[LOCAL_yAxis_x][row] | TIME_MARKER_PATTERN;
            pPattern[2] = dotMask[LOCAL_yAxis_x][row] | 0;

            if (LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plotNumber == 99)
            {
                arrayLength = 1;
            }
            else if (LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plotNumber == 98)
            {
                arrayLength = 2;
            }
            else
            {
                arrayLength = 3;
            }
        }
    }
    return arrayLength;
}
#endif

//-------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
// brief: Called by PacketManager::BuildGraphicalOperatorFrame() - Acknowledges that the array data for this row of plot data has been plotted
//
// No restriction on the optimization of this function
void TrendViewAcknowledgePlot(TUSIGN8 row)
{
    if (row <= 4)
    {
        if (LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plot_flags != NO_SAMPLE)
        {
            // set "data plotted" flag for this row
            LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plot_flags |= plot_flag_set[row];

            if (LOCAL_plotBuffer[LOCAL_PlotBufferGetIndex].plot_flags == ALL_PLOTTED)
            {
                // all rows rows have been plotted
                LOCAL_nSamplesPlotted ++;
                LOCAL_nSamplesPlotted = LOCAL_nSamplesPlotted % 100;

                LOCAL_PlotBufferGetIndex ++;
                if (LOCAL_PlotBufferGetIndex >= PLOT_BUFFER_SIZE)
                {
                    LOCAL_PlotBufferGetIndex = 0;
                }
            }
        }
    }
}
#endif

//--------------------------------------------------------------------------------------------------
TFLOAT CalculateAxis(TFLOAT AxisValue_f, TUSIGN8 *pAxisText, TBOOL HighNotLow)
{
    TUSIGN32 AxisValue_u32, digitValue;
    TUSIGN8 i, dp;

    TBOOL    negVal = AxisValue_f < 0 ? eTRUE : eFALSE;
    TFLOAT AxisValue_abs = (TFLOAT)fabs((TDOUBLE)AxisValue_f);

    if (AxisValue_abs > 99999)
    {
        AxisValue_abs = 99999;
    }

    if      (AxisValue_abs < 10)    { dp = 3;     AxisValue_abs *= 1000; }
    else if (AxisValue_abs < 100)   { dp = 2;     AxisValue_abs *= 100;  }
    else if (AxisValue_abs < 1000)  { dp = 1;     AxisValue_abs *= 10;   }
//  else if (AxisValue_abs < 10000) { dp = 0;                            }
    else                            { dp = 0;                            }

    if ((( HighNotLow) && ( negVal)) || ((!HighNotLow) && (!negVal)))
    {
        // upper range value is -ve so round down - round towards zero
        // lower range value is +ve so round down - round towards zero
        AxisValue_u32 = (TUSIGN32)AxisValue_abs;
    }
    else
    {
        // upper range value is +ve so round up - round away from zero
        // lower range value is -ve so round up - round away from zero
        AxisValue_u32 = (TUSIGN32)(AxisValue_abs + 0.5);
    }

    TFLOAT returnVal = AxisValue_u32;
    switch (dp)
    {
    case 3:     returnVal = (TFLOAT)AxisValue_u32 / 1000;   break;
    case 2:     returnVal = (TFLOAT)AxisValue_u32 / 100;    break;
    case 1:     returnVal = (TFLOAT)AxisValue_u32 / 10;     break;
    default:    returnVal = (TFLOAT)AxisValue_u32;          break;
    }

    if (negVal)
    {
        returnVal *= -1.0F;
    }

    i = 0;

    digitValue = AxisValue_u32 % 10;        //   0   1   2   3
    AxisValue_u32 -= digitValue;            //   |   |   |   |
    pAxisText[5-i] = digitValue;            //  [5] [5] [5] [5]
    i++;                                    //   |   |   |   |
                                            //   |   |   |   |
    if ((i == dp) && (dp != 0))             //   |   |   |   |
    {                                       //   |   |   |   |
        pAxisText[5-i]  = TINYFONT_DP;      //   -  [4]  -   -
        i ++;                               //   |   |   |   |
    }                                       //   |   |   |   |
                                            //   |   |   |   |
    digitValue = AxisValue_u32 % 100;       //   |   |   |   |
    AxisValue_u32 -= digitValue;            //   |   |   |   |
    pAxisText[5-i] = digitValue / 10;       //  [4] [3] [4] [4]
    i++;                                    //   |   |   |   |
                                            //   |   |   |   |
    if ((i == dp) && (dp != 0))             //   |   |   |   |
    {                                       //   |   |   |   |
        pAxisText[5-i]  = TINYFONT_DP;      //   -   -  [3]  -
        i ++;                               //   |   |   |   |
    }                                       //   |   |   |   |
                                            //   |   |   |   |
    digitValue = AxisValue_u32 % 1000;      //   |   |   |   |
    AxisValue_u32 -= digitValue;            //   |   |   |   |
    pAxisText[5-i] = digitValue / 100;      //  [3] [2] [2] [3]
    i++;                                    //   |   |   |   |
                                            //   |   |   |   |
    if ((i == dp) && (dp != 0))             //   |   |   |   |
    {                                       //   |   |   |   |
        pAxisText[5-i]  = TINYFONT_DP;      //   -   -   -  [2]
        i ++;                               //   |   |   |   |
    }                                       //   |   |   |   |
                                            //   |   |   |   |
    digitValue = AxisValue_u32 % 10000;     //   |   |   |   |
    AxisValue_u32 -= digitValue;            //   |   |   |   |
    pAxisText[5-i] = digitValue / 1000;     //  [2] [1] [1] [1]
    i++;                                    //   |   |   |   |
                                            //   |   |   |   |
    if ((i == dp) && (dp != 0))             //   |   |   |   |
    {                                       //   |   |   |   |
        pAxisText[5-i]  = TINYFONT_DP;      //   |   |   |   |
        i ++;                               //   |   |   |   |
    }                                       //   |   |   |   |
                                            //   |   |   |   |
    digitValue = AxisValue_u32 % 100000;    //   |   |   |   |
    AxisValue_u32 -= digitValue;            //   |   |   |   |
    pAxisText[5-i] = digitValue / 10000;    //  [1] [0] [0] [0]
    i++;                                    //   |   |   |   |

    pAxisText[0] = TINYFONT_0;


    if (dp != 0)
    {
        while (pAxisText[5] == TINYFONT_0)
        {
            pAxisText[5] = pAxisText[4];
            pAxisText[4] = pAxisText[3];
            pAxisText[3] = pAxisText[2];
            pAxisText[2] = pAxisText[1];
            pAxisText[1] = pAxisText[0];
            pAxisText[0] = TINYFONT_0;
        }
        if (pAxisText[5] == TINYFONT_DP)
        {
            pAxisText[5] = pAxisText[4];
            pAxisText[4] = pAxisText[3];
            pAxisText[3] = pAxisText[2];
            pAxisText[2] = pAxisText[1];
            pAxisText[1] = pAxisText[0];
            pAxisText[0] = TINYFONT_0;
        }
    }

    i = 0;
    while ((pAxisText[i] == TINYFONT_0) && (pAxisText[i + 1] != TINYFONT_DP) && (i < 5))
    {
        pAxisText[i] = TINYFONT_SPACE;
        i ++;
    }

    if (negVal)
    {
        i = 0;
        while ((pAxisText[i + 1] == TINYFONT_SPACE) && (i < 4))
        {
            i ++;
        }
        pAxisText[i] = TINYFONT_MINUS;
    }
    return returnVal;
}


//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
TFLOAT GetSample(void)
{
    TFLOAT ReturnSample = LOCAL_yRangeMin;
    TFLOAT MeasuredSample;

    T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_TrendSubSystemIndex);
    if (pSub->Get(pSub,LOCAL_TrendObjectIndex,LOCAL_TrendAttributeIndex,(void FAST *)&MeasuredSample) == (TUSIGN16)OK)
    {
        ReturnSample = MeasuredSample;
    }
    return ReturnSample;
}
#endif

//--------------------------------------------------------------------------------------------------
TUSIGN8 calculatePlotPosition(TFLOAT Sample)
{
    TUSIGN8 PlotPosition;

    if (Sample <= LOCAL_yRangeMin)
    {
        PlotPosition = yAxisMin;
    }
    else if (Sample >= LOCAL_yRangeMax)
    {
        PlotPosition = yAxisMax;
    }
    else
    {
        PlotPosition = (TUSIGN8)((((Sample - LOCAL_yRangeMin) * (yAxisMax - yAxisMin)) / (LOCAL_yRangeMax - LOCAL_yRangeMin)) + 0.5);
    }
    return PlotPosition;
}

//--------------------------------------------------------------------------------------------------
#if defined(HMI_SUPPORTS_GRAPH_VIEW)
void addPlotToBuffer(TUSIGN8 SampleNumber, TUSIGN8 plotPosition)
{
    PLOT_BUFFER_ENTRY newEntry;
    newEntry.plotNumber     = SampleNumber % 100;
    newEntry.plotPosition_y = plotPosition;
    newEntry.plot_flags     = NONE_PLOTTED;

    LOCAL_plotBuffer[LOCAL_PlotBufferPutIndex] = newEntry;

    LOCAL_PlotBufferPutIndex ++;

    if (LOCAL_PlotBufferPutIndex >= PLOT_BUFFER_SIZE)
    {
        LOCAL_PlotBufferPutIndex = 0;
    }

    LOCAL_plotBuffer[LOCAL_PlotBufferPutIndex].plot_flags = NO_SAMPLE;
}
#endif

//--------------------------------------------------------------------------------------------------
void addAxisTextToOutputPattern(TUSIGN8 *pAxisText, TUSIGN8 *pPattern)
{
    TUSIGN8 textIndex,patternIndex;

    patternIndex = 0;

    for (textIndex = 0; textIndex < 6; textIndex ++)
    {
        pPattern[patternIndex++] = tinyFont[pAxisText[textIndex]][0];
        pPattern[patternIndex++] = tinyFont[pAxisText[textIndex]][1];
        pPattern[patternIndex++] = tinyFont[pAxisText[textIndex]][2];
        pPattern[patternIndex++] = 0x00;
    }
}

//--------------------------------------------------------------------------------------------------
#ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
// brief:
//
// No restriction on the optimization of this function
TBOOL TrendViewStartArrayCalculation(TUSIGN8 ArraySignal_ss,TUSIGN16 ArraySignal_ob)
{
    TBOOL returnVal;

    if (LOCAL_Status != HMI_TRENDVIEW_STATUS_INACTIVE)          // cannot Array Calculation if sampling is not inactive
    {
        returnVal = eFALSE;
    }
    else
    {
        // Set Array Object indeces
        LOCAL_TrendSubSystemIndex = ArraySignal_ss;
        LOCAL_TrendObjectIndex    = ArraySignal_ob;

        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_TrendSubSystemIndex);

        // Calculate Y axis range
        TFLOAT TrendHigh = 100.0F;
        TFLOAT TrendLow  =   0.0F;
        T_DO_ATTRIB_RANGE rangeDescriptor;

        if (pSub->GetAttributeDescription(pSub,LOCAL_TrendObjectIndex,0,(T_DO_ATTRIB_RANGE FAST *)&rangeDescriptor) == (TUSIGN16)OK)
        {
            switch (rangeDescriptor.dataType)
            {
            case ST_FLOAT:  LOCAL_Array_dataType = ST_FLOAT;        TrendHigh = rangeDescriptor.range.flt.max;          TrendLow  = rangeDescriptor.range.flt.min;          break;
            case ST_U16:    LOCAL_Array_dataType = ST_U16;          TrendHigh = (TFLOAT)rangeDescriptor.range.u16.max;  TrendLow  = (TFLOAT)rangeDescriptor.range.u16.min;  break;
            case ST_I16:    LOCAL_Array_dataType = ST_I16;          TrendHigh = (TFLOAT)rangeDescriptor.range.i16.max;  TrendLow  = (TFLOAT)rangeDescriptor.range.i16.min;  break;
            case ST_U8:     LOCAL_Array_dataType = ST_U8;           TrendHigh = (TFLOAT)rangeDescriptor.range.u8.max;   TrendLow  = (TFLOAT)rangeDescriptor.range.u8.min;   break;
            default:        LOCAL_Array_dataType = ST_NIL;          TrendHigh = 100.0f;                                 TrendLow  = 0.0f;                                   break;
            }
        }

        if (TrendLow > TrendHigh)
        {
            TFLOAT swap = TrendLow;
            TrendLow = TrendHigh;
            TrendHigh = swap;
        }

        LOCAL_yRangeMax = CalculateAxis(TrendHigh, LOCAL_HighAxisText, eTRUE);
        LOCAL_yRangeMin = CalculateAxis(TrendLow,  LOCAL_LowAxisText,  eFALSE);

        LOCAL_yAxis_plotFlags = NONE_PLOTTED;


        // Calculate number of items in array
        LOCAL_nArrayItems = 1;
        T_DO_OBJECT_DESCRIPTION objectDescriptor;
        if (pSub->GetObjectDescription(pSub,LOCAL_TrendObjectIndex,(T_DO_OBJECT_DESCRIPTION FAST *)&objectDescriptor) == (TUSIGN16)OK)
        {
            if (objectDescriptor.typeQualifier == ARRAY)
            {
                LOCAL_nArrayItems = objectDescriptor.numberOfAttributes;

                if (LOCAL_nArrayItems > 100)
                {
                    LOCAL_nArrayItems = 100;
                }
            }
        }

        // Calculate zero position
        LOCAL_yAxis_x = calculatePlotPosition(0.0F);

        // Array Calculation is now active
        LOCAL_Status = HMI_TRENDVIEW_STATUS_ARRAY_VIEW;
        returnVal = eTRUE;
    }
    return returnVal;
}
#endif

//--------------------------------------------------------------------------------------------------
#ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
// brief: Stops Array calculation
//
// No restriction on the optimization of this function
TBOOL TrendViewStopArrayCalculation(void)
{
    TBOOL returnVal = eFALSE;

    if (LOCAL_Status == HMI_TRENDVIEW_STATUS_ARRAY_VIEW)
    {
        LOCAL_Status = HMI_TRENDVIEW_STATUS_INACTIVE;
        returnVal = eTRUE;
    }
    return returnVal;
}
#endif

//--------------------------------------------------------------------------------------------------
#ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
// brief:
//
// No restriction on the optimization of this function
TBOOL TrendViewGet25plots(TUSIGN8 firstPlot, TUSIGN8 row, TUSIGN8* pPattern, TUSIGN8 style)
{
    TBOOL returnVal = eFALSE;   // assume that there is no data to send
    TINT16 OldPlotPosition, NewPlotPosition;
    TUSIGN8 i;
    TUSIGN8 pattern = 0x00;



    for (i = 0; i < 25; i ++)
    {
        NewPlotPosition = calculatePlotPosition( calculatePlotLevel((TUSIGN8)(firstPlot + i + 1),style));

        pattern = 0x00;

        if (i == 0)
        {
            if (firstPlot == 0)
            {
                OldPlotPosition = NewPlotPosition;
            }
            else
            {
                OldPlotPosition = calculatePlotPosition(calculatePlotLevel(firstPlot,style));
            }
        }

        if (NewPlotPosition > OldPlotPosition + 1)
        {
            // step change up - set every pixel from 1 above old position up to and including the new position
            TINT16 PlotPosition;
            for (PlotPosition = OldPlotPosition + 1; PlotPosition <= NewPlotPosition; PlotPosition ++)
            {
                pattern |= dotMask[PlotPosition][row];
            }
        }
        else if (NewPlotPosition < OldPlotPosition - 1)
        {
            // step change down - set every pixel from 1 below old position down to and including the new position...
            TINT16 PlotPosition;
            for (PlotPosition = OldPlotPosition - 1; PlotPosition >= NewPlotPosition; PlotPosition --)
            {
                pattern |= dotMask[PlotPosition][row];
            }
        }
        else
        {
            // slow ramp or steady signal - set pixel relating to new position
            pattern = dotMask[NewPlotPosition][row];
        }

        pattern |= dotMask[LOCAL_yAxis_x][row];

        if (pattern)
        {
            // if there is any data to send then set return val
            returnVal = eTRUE;
        }

        pPattern[i] = dotMask[LOCAL_yAxis_x][row] | pattern;

        OldPlotPosition = NewPlotPosition; // set previous plot position ready for next time around
    }

    return returnVal;
}

#endif

//--------------------------------------------------------------------------------------------------
#ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
// brief:
//
// No restriction on the optimization of this function
TFLOAT calculatePlotLevel(TUSIGN8 plotNumber, TUSIGN8 style)
{
    TFLOAT plotLevel;

    if (style == 2)
    {
        TFLOAT      arrayData_flt;
        TUSIGN16    arrayData_u16;
        TINT16      arrayData_i16;
        TUSIGN8     arrayData_u8;

        TFLOAT      singlePoint;

        TFLOAT  tempCalc = ((TFLOAT)(plotNumber - 1) / (TFLOAT)(MAX_PLOTS)) * (LOCAL_nArrayItems);
        TUSIGN8 arrayOffset = (TUSIGN8)tempCalc;
        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_TrendSubSystemIndex);

        switch (LOCAL_Array_dataType)
        {
        case ST_FLOAT:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset, (void FAST *)&arrayData_flt);
            singlePoint = arrayData_flt;
            break;

        case ST_U16:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset, (void FAST *)&arrayData_u16);
            singlePoint = (TFLOAT)arrayData_u16;
            break;

        case ST_I16:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset, (void FAST *)&arrayData_i16);
            singlePoint = (TFLOAT)arrayData_i16;
            break;

        case ST_U8:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset, (void FAST *)&arrayData_u8);
            singlePoint = (TFLOAT)arrayData_u8;
            break;

        default:
            singlePoint = 0.0f;
            break;
            }

        plotLevel = singlePoint;
    }
    else
    {
        TFLOAT      arrayData_flt;
        TUSIGN16    arrayData_u16;
        TINT16      arrayData_i16;
        TUSIGN8     arrayData_u8;

        TFLOAT      lowPoint;
        TFLOAT      highPoint;

        TFLOAT  tempCalc = ((TFLOAT)(plotNumber - 1) / (TFLOAT)(MAX_PLOTS - 1)) * (LOCAL_nArrayItems - 1);
        TUSIGN8 arrayOffset_start = (TUSIGN8)floor((TDOUBLE)tempCalc);
        TUSIGN8 arrayOffset_end   = (TUSIGN8)ceil((TDOUBLE)tempCalc);
        TFLOAT  arrayOffset_frac  = tempCalc - arrayOffset_start;
        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_TrendSubSystemIndex);

        switch (LOCAL_Array_dataType)
        {
        case ST_FLOAT:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_start, (void FAST *)&arrayData_flt);
            lowPoint = arrayData_flt;
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_end,   (void FAST *)&arrayData_flt);
            highPoint = arrayData_flt;
            break;

        case ST_U16:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_start, (void FAST *)&arrayData_u16);
            lowPoint = (TFLOAT)arrayData_u16;
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_end,   (void FAST *)&arrayData_u16);
            highPoint = (TFLOAT)arrayData_u16;
            break;

        case ST_I16:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_start, (void FAST *)&arrayData_i16);
            lowPoint = (TFLOAT)arrayData_i16;
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_end,   (void FAST *)&arrayData_i16);
            highPoint = (TFLOAT)arrayData_i16;
            break;

        case ST_U8:
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_start, (void FAST *)&arrayData_u8);
            lowPoint = (TFLOAT)arrayData_u8;
            pSub->Get(pSub, LOCAL_TrendObjectIndex, (TINT16)arrayOffset_end,   (void FAST *)&arrayData_u8);
            highPoint = (TFLOAT)arrayData_u8;
            break;

        default:
            lowPoint = 0.0f;
            highPoint = 0.0f;
            break;
        }

        plotLevel = ((highPoint - lowPoint) * arrayOffset_frac) + lowPoint;
    }

    return plotLevel;
}
#endif

//--------------------------------------------------------------------------------------------------

#endif  // end of #if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)


