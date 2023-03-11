

#pragma diag_suppress=Go003


#include <intrinsics.h>
#include <math.h>
#include <string.h>

#include "IncludeList_Implementation.h"

#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../embos/interface/rtos.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#endif

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "ServicePort/Layer7/servicePort_layer7.h"
#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"

#include "Hmi/Interface/HMI_idx.h"
#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "FrameTableDefinitions.h"
#include "FrameTable.h"
#include "keystateDefinitions.h"

#include "IncludeList_Frames_1.h"

#include "FixedIconDefinitions.h"

#include "Adjuster.h"
#include "writeProtect.h"


#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
#include "wavefornFrameDefinitions.h"
#include "../../waveform/interface/waveform.h"
#endif
#include "TrendView.h"

#include "PacketManager.h"
#include "PacketManagerLocalData.h"
#include "PacketManagerBuilder.h"
#include "PacketManagerBuilder_s.h"
#include "PacketManagerBuilderServices.h"       // PacketManagerBuilderServices


#pragma diag_suppress=Pm071
#include "low_level\core\Driver.h"
#pragma diag_default=Pm071


#ifdef SUPPORT_SHMI


#define SmartHMI_WAVEFORM_LENGTH    84
#define WAVEFORM_DO_UPDATE  0x00
#define WAVEFORM_NOT_UPDATE 0xFF


static TUSIGN8 testLanguage = HMI_LANGUAGE_ENGLISH;
static TUSIGN8 preDisplayLang = HMI_LANGUAGE_NOT_DEFINED;
static TUSIGN8 preCursorSel = 0xFF; 
static TUSIGN8 packetStage = 0;



extern TUSIGN8 valid_if_service_account_is_allowed(void);

static TBOOL IsSamePacket(TUSIGN8* pData1, TUSIGN8* pData2, TUSIGN8 len)
{
    TUSIGN16 i;

    for(i=0; i<len; i++)
    {
        if(pData1[i] != pData2[i])
        {
            return eFALSE;
        }
    }

    return eTRUE;    
}


TUSIGN8 BuildErrorResponse_s(void)
{
    //TODO: by Grace 2020/12/4
    //refer to BuildErrorResponse.
    return LOW_LEVEL_ERROR_NOT_PRESENT;
}


//------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------
#ifdef HMI_SUPPORTS_OPERATOR_FRAMES
#ifdef HMI_SUPPORTS_GRAPH_VIEW
TUSIGN8 BuildSFrame_GraphicOperator(TUSIGN8 frameNumber)
{
    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_BLANK_AREA,        // Blank Area                              | - Start of Static Area
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_TAG,               // Tag                                     | - Start of Active Area
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_1,       // Autoscroll Icon              [12]..[18] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_2,       // Blank                        [19]..[29] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_3,       // Control Icon                 [30]..[53] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_4,       // Blank                        [54]..[58] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_5,       // Write Protect icon           [59]..[65] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_6,       // Blank                        [66]..[70] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_7,       // Mode icon                    [71]..[94] |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_ICONFIELD_8,       // Blank                        [95]..[113]|
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_REMOVE_DIAGNOSTICS,// Remove Diagnostics                      |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_DIAG_ICON_T,       // Diagnostics Classification Icon (Top)   |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_DIAG_ICON_B,       // Diagnostics Classification Icon (Bottom)|
                                                    //-----------------------------------------|
    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_DIAG_MESSAGE_1_16, //                                         |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_DIAG_MESSAGE_17_32,//                                         |
                                                    //-----------------------------------------|
    #endif
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_DIAG_CATEGORY,     // Diagnostics Category Text               |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_BLANK_GRAPH,       // Blank the graph area                    |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_Y_AXIS_ROW_0,      // y axis - bottom                         |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_Y_AXIS_ROW_1,      // y axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_Y_AXIS_ROW_2,      // y axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_Y_AXIS_ROW_3,      // y axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_Y_AXIS_ROW_4,      // y axis - top                            |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_SIGNAL_ID,         // Signal Identifier                       |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_X_AXIS_ROW_0,      // x axis - bottom                         |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_X_AXIS_ROW_1,      // x axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_X_AXIS_ROW_2,      // x axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_X_AXIS_ROW_3,      // x axis                                  |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_X_AXIS_ROW_4,      // x axis - top                            |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_1_ROW_0,      // Trace data - bottom                     |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_1_ROW_1,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_1_ROW_2,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_1_ROW_3,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_1_ROW_4,      // Trace data - top                        |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_2_ROW_0,      // Trace data - bottom                     |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_2_ROW_1,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_2_ROW_2,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_2_ROW_3,      // Trace data                              |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_PLOT_2_ROW_4,      // Trace data - top                        |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_VIEW_ICON,         // View Icon                               |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_MENU_ICON,         // Menu Icon                               |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_LAST_PACKET,       //                                         |
                                                    //-----------------------------------------|
        GRAPHICAL_OPERATOR_FRAME_READ_SWITCHES      //                                         |
                                                    //-----------------------------------------| - There is no keypress area
    }GRAPHICAL_OPERATOR_FRAME_PACKET;
    
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION_0,
        FRAME_STAGE_OPERATION_1,
        FRAME_STAGE_OPERATION_2,
        FRAME_STAGE_OPERATION_3,
        FRAME_STAGE_OPERATION_4,
        FRAME_STAGE_OPERATION_5,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
    
    TUSIGN8 TraceDataPolot1_1[6] = {0};
    TUSIGN8 TraceDataPolot1_2[6] = {0};
    TUSIGN8 TraceDataPolot1_3[6] = {0};
    TUSIGN8 TraceDataPolot1_4[6] = {0};
    TUSIGN8 TraceDataPolot1_5[6] = {0};
    
    TUSIGN8 TraceDataPolot2_1[6] = {0};
    TUSIGN8 TraceDataPolot2_2[6] = {0};
    TUSIGN8 TraceDataPolot2_3[6] = {0};
    TUSIGN8 TraceDataPolot2_4[6] = {0};
    TUSIGN8 TraceDataPolot2_5[6] = {0};
    
    TUSIGN8 NamurClass = 0;
    TUSIGN8 BlankFlag = 0x00;
    
    static TUSIGN8 operation0_flag = 0x00;
    static TUSIGN8 operation1_flag = 0x00;
    static TUSIGN8 operation2_flag = 0x00;
    static TUSIGN8 operation3_flag = 0x00;
    static TUSIGN8 operation4_flag = 0x00;
    static TUSIGN8 operation5_flag = 0x00;

    #ifdef HMI_USES_COMMONDIAGNOSTICS
    static TUSIGN8 NAMURClassification[2];
    static TUSIGN8 NAMURCategory[2];
    static TUSIGN8 infoMessageID[2];
    #endif

    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
    //static TUSIGN8 NAMUR_ActiveAlarm[2];
    #endif

    //static TBOOL   alarmActive[2];

#ifdef HMI_SUPPORTS_SERVICEPORT
    TUSIGN8        OpPageInstance = PacketManagerIsRemoteInstance() ? 1 : 0;
#else
    TUSIGN8        OpPageInstance = 0;
#endif


    SIGNAL_TABLE_TYPE const *pSignal = NULL;

        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    FrameTableDisableAutoscroll();
    #endif

    #if defined(SIXTH_OPERATOR_PAGE)
    if (frameNumber == 6)
    {
        pSignal = FrameTableGetSignal(SFD_MULTI_OP.mainOperatorView_6_bargraph);
    }
    else
    #endif
    {
        #if defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        if (frameNumber == 5)
        {
            pSignal = FrameTableGetSignal(SFD_MULTI_OP.mainOperatorView_5_bargraph);
        }
        else
        #endif
        {
            #if defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
            if (frameNumber == 4)
            {
                pSignal = FrameTableGetSignal(SFD_MULTI_OP.mainOperatorView_4_bargraph);
            }
            else
            #endif
            {
                #if defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
                if (frameNumber == 3)
                {
                    pSignal = FrameTableGetSignal(SFD_MULTI_OP.mainOperatorView_3_bargraph);
                }
                else
                #endif
                {
                    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
                    if (frameNumber == 2)
                    {
                        pSignal = FrameTableGetSignal(SFD_MULTI_OP.mainOperatorView_2_bargraph);
                    }
                    else
                    #endif
                    {
                        pSignal = FrameTableGetSignal(SFD_SIMPLE.mainOperatorView_1_bargraph);
                    }
                }
            }
        }
    }
    if (TrendViewGetStatus() == HMI_TRENDVIEW_STATUS_INACTIVE)
    {
        #ifdef HMI_USES_DOUBLE
        TDOUBLE tempTrendHigh, tempTrendLow;
        #else
        TFLOAT tempTrendHigh, tempTrendLow;
        #endif

        TFLOAT TrendHigh, TrendLow;

        switch (pSignal->FixedUpperLower)
        {
        case 0:
            PacketManagerBuilderServices_GetBarRange(pSignal,&tempTrendHigh,&tempTrendLow);
            TrendHigh = (TFLOAT)tempTrendHigh;
            TrendLow  = (TFLOAT)tempTrendLow;
            break;

        case 1:
            TrendHigh = 100.0F;
            TrendLow = 0.0F;
            break;

        case 2:
            TrendHigh = 20.0F;
            TrendLow = 4.0F;
            break;

        default:
            PacketManagerBuilderServices_GetTrendRange(pSignal,&TrendHigh,&TrendLow);
            break;

        }


        TrendViewStartSampling((TUSIGN8)frameNumber,                // frameNumber
                               (TFLOAT)TrendHigh,                   // TrendHigh
                               (TFLOAT)TrendLow,                    // TrendLow
                               (TUSIGN8)pSignal->value.ssIdx,       // TrendSignal_ss
                               (TUSIGN16)pSignal->value.objIdx,     // TrendSignal_ob
                               (TINT16)pSignal->value.attribIdx);   // TrendSignal_at
    }

    // Get Alarm info
    //alarmActive[OpPageInstance] = eFALSE;

    #ifdef HMI_USES_COMMONDIAGNOSTICS

    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
        AdjusterGetNAMURStatus(&NAMURClassification[OpPageInstance], &NAMURCategory[OpPageInstance]);
    #else
        AdjusterGetNAMURStatus(&NAMURClassification[OpPageInstance], &NAMURCategory[OpPageInstance]);
    #endif

    #ifndef INFORMATION_MESSAGES_OFFSET
    infoMessageID[OpPageInstance] = 0;
    #else
    infoMessageID[OpPageInstance] = GetInfoMessageID((TBOOL)(NAMURClassification[OpPageInstance] == CLASSIFICATION_NONE));
    #endif

    //if ((NAMURClassification[OpPageInstance] != CLASSIFICATION_NONE) || (infoMessageID[OpPageInstance] != 0))
    //{
    //    alarmActive[OpPageInstance] = eTRUE;
    //}
    #endif

    // Autoscroll Icon
    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    LOCAL_displayAutoscrollIcon = FrameTableIsAutoscrollEnabled();
    #else
    LOCAL_displayAutoscrollIcon = eFALSE;
    #endif

    // Write Protect Icon
    #ifdef COMMON_TOP_WORKS_NEW_FEATURES
    TBOOL User_lock = WriteProtect_GetUserLock(eFALSE,0,0,0);
    TBOOL ARM_lock = eFALSE;

    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
        if (AdjusterGetARMLevel() == ARM_ACCESS_READ_ONLY)
        {
            ARM_lock = eTRUE;
        }
        #endif

        if ((User_lock) || (ARM_lock))
        {
            LOCAL_displayWriteProtectIcon = eTRUE;
        }
        else
        {
            LOCAL_displayWriteProtectIcon = eFALSE;
        }
    #else
        TUSIGN8 HART_lock = WriteProtect_GetHARTLock();
        TUSIGN8 HMI_lock  = WriteProtect_GetHMILock();

        if (
            #ifdef HMI_SUPPORTS_SECURITY_ACCESS
            (AdjusterGetARMLevel() == ARM_ACCESS_READ_ONLY) ||
            #endif
            (HART_lock != HMI_LOCAL_OP_ENABLED) ||
            (HMI_lock  != HMI_LOCAL_OP_ENABLED))
        {
            LOCAL_displayWriteProtectIcon = eTRUE;
        }
        else
        {
            LOCAL_displayWriteProtectIcon = eFALSE;
        }
    #endif
  
    TUSIGN8 isBuild = 0;
    
#if (!TEST_FRAME_GRAPH_OPERATOR)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif

    
    if(operation0_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_0;
    }
    else if(operation1_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_1;
    }
    else if(operation2_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_2;
    }
    
    else if(operation3_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_3;
    }
    
    else if(operation4_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_4;
    }
    
    else if(operation5_flag)
    {
        packetStage = FRAME_STAGE_OPERATION_5;
    }
    
    else if ((LOCAL_updateStaticArea) || (testLanguage != preDisplayLang))
    {
        LOCAL_updateStaticArea = eFALSE;
        packetStage = FRAME_STAGE_STATIC;
        BlankFlag = 0x01;
    }
    
    else if(LOCAL_updateActiveArea)
    {
        packetStage = FRAME_STAGE_STATIC;
        BlankFlag = 0x00;
        LOCAL_updateActiveArea = eFALSE;
    }
    
    if(packetStage==FRAME_STAGE_STATIC)
    { 
        TUSIGN8 arrayData[4];
        TUSIGN8 row = 0;
        TUSIGN8 arrayLength;
        TUSIGN8 plotNumber;
        TUSIGN8 language_bank_idx;
        
        for(row=0;row<5;row++)
        {
            arrayLength = TrendViewGetPlot(row, arrayData, &plotNumber);
            
            if(row == 0)
            {
                TraceDataPolot1_1[0] = arrayLength;
                TraceDataPolot1_1[1] = plotNumber;
                memcpy(&TraceDataPolot1_1[2],arrayData,arrayLength);
            }
            
            if(row == 1)
            {
                TraceDataPolot1_2[0] = arrayLength;
                TraceDataPolot1_2[1] = plotNumber;
                memcpy(&TraceDataPolot1_2[2],arrayData,arrayLength);
            }
            
            if(row == 2)
            {
                TraceDataPolot1_3[0] = arrayLength;
                TraceDataPolot1_3[1] = plotNumber;
                memcpy(&TraceDataPolot1_3[2],arrayData,arrayLength);
            }
            
            if(row == 3)
            {
                TraceDataPolot1_4[0] = arrayLength;
                TraceDataPolot1_4[1] = plotNumber;
                memcpy(&TraceDataPolot1_4[2],arrayData,arrayLength);
            }
            
            if(row == 4)
            {
                TraceDataPolot1_5[0] = arrayLength;
                TraceDataPolot1_5[1] = plotNumber;
                memcpy(&TraceDataPolot1_5[2],arrayData,arrayLength);
            }

            TrendViewAcknowledgePlot(row);
        }
      
      
        preDisplayLang = testLanguage;
        if(!BlankFlag)
        {
            language_bank_idx = testLanguage;
        }
        else
        {
            language_bank_idx = testLanguage | 0x80;
        }
        
        LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5)+31;                
        LOCAL_packet_HL[2] = language_bank_idx;
        memcpy(&LOCAL_packet_HL[3],TraceDataPolot1_1,6);
        memcpy(&LOCAL_packet_HL[9],TraceDataPolot1_2,6);
        memcpy(&LOCAL_packet_HL[15],TraceDataPolot1_3,6);
        memcpy(&LOCAL_packet_HL[21],TraceDataPolot1_4,6);
        memcpy(&LOCAL_packet_HL[27],TraceDataPolot1_5,6);
        if(AddPacketToBuffer(33, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_0;
            isBuild += 1;
        } 
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_0)
    {                    
        TUSIGN8 arrayData[4];
        TUSIGN8 row = 0;
        TUSIGN8 arrayLength;
        TUSIGN8 plotNumber;
            
        for(row=0;row<5;row++)
        {
            arrayLength = TrendViewGetPlot(row, arrayData, &plotNumber);
            
            if(row == 0)
            {
                TraceDataPolot2_1[0] = arrayLength;
                TraceDataPolot2_1[1] = plotNumber;
                memcpy(&TraceDataPolot2_1[2],arrayData,arrayLength);
            }
            
            if(row == 1)
            {
                TraceDataPolot2_2[0] = arrayLength;
                TraceDataPolot2_2[1] = plotNumber;
                memcpy(&TraceDataPolot2_2[2],arrayData,arrayLength);
            }
            
            if(row == 2)
            {
                TraceDataPolot2_3[0] = arrayLength;
                TraceDataPolot2_3[1] = plotNumber;
                memcpy(&TraceDataPolot2_3[2],arrayData,arrayLength);
            }
            
            if(row == 3)
            {
                TraceDataPolot2_4[0] = arrayLength;
                TraceDataPolot2_4[1] = plotNumber;
                memcpy(&TraceDataPolot2_4[2],arrayData,arrayLength);
            }
            
            if(row == 4)
            {
                TraceDataPolot2_5[0] = arrayLength;
                TraceDataPolot2_5[1] = plotNumber;
                memcpy(&TraceDataPolot2_5[2],arrayData,arrayLength);
            }

            TrendViewAcknowledgePlot(row);
        }

        LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5) + 30;                //length
        memcpy(&LOCAL_packet_HL[2],TraceDataPolot2_1,6);
        memcpy(&LOCAL_packet_HL[8],TraceDataPolot2_2,6);
        memcpy(&LOCAL_packet_HL[14],TraceDataPolot2_3,6);
        memcpy(&LOCAL_packet_HL[20],TraceDataPolot2_4,6);
        memcpy(&LOCAL_packet_HL[26],TraceDataPolot2_5,6);
        if(AddPacketToBuffer(32, 0))
        {            
            packetStage = FRAME_STAGE_OPERATION_1;
            isBuild += 1;
            operation0_flag = 0;
        }
        else
        {
            operation0_flag = 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_1)
    {
        NamurClass = NAMURClassification[OpPageInstance];
        
        E_STRING_TYPE stringType;
        TUSIGN8 diagnoseLength;
        TUSIGN8 signalIDlength;
        const TUSIGN8* psignalID;
        const TUSIGN8* pdiagnose;
        

        #ifdef HMI_USES_COMMONDIAGNOSTICS
            #ifdef DISPLAY_SIZE_E
            TUSIGN8 CategoryStringLen = 13;
            #else
            TUSIGN8 CategoryStringLen = 14;
            #endif

            #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
            if (PacketManagerBuilderServices_IndicatorOnly())
            {
                CategoryStringLen = 18;
            }
            #endif

            TUSIGN8 CategoryString[DIAG_CATEGORY_BUFFER_SIZE+1];
            memset(CategoryString,0x00,DIAG_CATEGORY_BUFFER_SIZE+1);

            if (CategoryStringLen > DIAG_CATEGORY_BUFFER_SIZE)
            {
                CategoryStringLen = DIAG_CATEGORY_BUFFER_SIZE;
            }

            if (infoMessageID[OpPageInstance] != 0)
            {
                #ifndef INFORMATION_MESSAGES_OFFSET
                    LOCAL_stringtable_text_id.attribIdx = IDS_BLANK;
                #else
                    #ifdef COMMON_TOP_WORKS_NEW_FEATURES
                        LOCAL_stringtable_text_id.attribIdx = FrameTableGetDiagnosticTableEntry((TUSIGN8)(infoMessageID[OpPageInstance] + INFORMATION_MESSAGES_OFFSET)).descriptionID;
                    #else
                        LOCAL_stringtable_text_id.attribIdx = FrameTableGetDiagnosticDescriptionStringID((TUSIGN8)(infoMessageID[OpPageInstance] + INFORMATION_MESSAGES_OFFSET));
                    #endif
                #endif
            }
            else
            {
                #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
                if ((PacketManagerBuilderServices_IndicatorOnly()) && (LOCAL_flashState))
                {
                    LOCAL_packetStage ++;
                }
                else
                #endif
                {
                    LOCAL_stringtable_text_id.attribIdx = FrameTableGetNAMURCategoryString(NAMURCategory[OpPageInstance]);
                }
            }

            pdiagnose = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);            
            if(stringType==STRING_TYPE_LIBIDX)
            {
                diagnoseLength = 2;
            }
            else
            {
                for(diagnoseLength=0; diagnoseLength<MAX_LENGTH_FRAME_TITLE; diagnoseLength++)
                {
                    if(pdiagnose[diagnoseLength] == '\0')
                    {
                        break;
                    }
                }
             }  
            
        #endif
        
        LOCAL_stringtable_text_id.attribIdx = pSignal->short_identifier;
        psignalID = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);            
        if(stringType==STRING_TYPE_LIBIDX)
        {
            signalIDlength = 2;
        }
        
        else
        {
            for(signalIDlength=0; signalIDlength<MAX_LENGTH_FRAME_TITLE; signalIDlength++)
            {
                if(psignalID[signalIDlength] == '\0')
                {
                    break;
                }
            }
         } 
            
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5) + diagnoseLength + signalIDlength+4;
        LOCAL_packet_HL[2] = testLanguage;
        LOCAL_packet_HL[3] = NamurClass;        
        LOCAL_packet_HL[4] = (stringType<<5)|diagnoseLength; 
        LOCAL_packet_HL[5] = (stringType<<5)|signalIDlength; 
        memcpy(&LOCAL_packet_HL[6],pdiagnose,diagnoseLength);
        memcpy(&LOCAL_packet_HL[6+diagnoseLength],psignalID,signalIDlength);
        
        if(AddPacketToBuffer(diagnoseLength + signalIDlength + 6, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild += 1;
            operation1_flag = 0;
        }  
        else
        {
            operation1_flag = 1;
        }
        
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_2)
    {
        TUSIGN8 arrayData4[26];
        TUSIGN8 arrayData4_temp[26];
        TUSIGN8 arrayLength4 = 0;
        TUSIGN8 row4 = 4;
        TUSIGN8 arrayData4_change = 0;
        
        arrayLength4 = TrendViewGetYaxis(row4, arrayData4_temp);
        
        /*
        TrendViewAcknowledgeYaxis(row4);
        
        if(!memcmp(arrayData4,arrayData4_temp,26))
        {
            arrayData4_change = 0;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+1;
            LOCAL_packet_HL[2] = arrayData4_change;
            
            if(AddPacketToBuffer(3, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_3;
                isBuild += 1;
                operation2_flag = 0;
            } 
            else
            {
                operation2_flag = 1;
            }
        }
        
        
        else
        */
        {
            memcpy(arrayData4,arrayData4_temp,26);
            arrayData4_change = 1;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+arrayLength4+2;
            LOCAL_packet_HL[2] = arrayData4_change;
            LOCAL_packet_HL[3] = arrayLength4;
            memcpy(&LOCAL_packet_HL[4],arrayData4_temp,arrayLength4);
            
            if(AddPacketToBuffer(arrayLength4+4, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_3;
                isBuild += 1;
                operation2_flag = 0;
            } 
            else
            {
                operation2_flag = 1;
            }
        }
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_3)
    {
        TUSIGN8 arrayData1[26];
        TUSIGN8 arrayData1_temp[26];
        TUSIGN8 arrayLength1 = 0;
        TUSIGN8 row1 = 1;
        TUSIGN8 arrayData1_change = 0;
        
        arrayLength1 = TrendViewGetYaxis(row1, arrayData1_temp);
        /*
        TrendViewAcknowledgeYaxis(row1);
        TrendViewAcknowledgeYaxis(row1+1);
        TrendViewAcknowledgeYaxis(row1+2);
        
        if(!memcmp(arrayData1,arrayData1_temp,26))
        {
            arrayData1_change = 0;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+1;
            LOCAL_packet_HL[2] = arrayData1_change;
            
            if(AddPacketToBuffer(3, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_4;
                isBuild += 1;
                operation3_flag = 0;
            } 
            else
            {
                operation3_flag = 1;
            }
        }
        
        else
        */
        {
            memcpy(arrayData1,arrayData1_temp,26);
            arrayData1_change = 1;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+arrayLength1+2;
            LOCAL_packet_HL[2] = arrayData1_change;
            LOCAL_packet_HL[3] = arrayLength1;
            memcpy(&LOCAL_packet_HL[4],arrayData1_temp,arrayLength1);
            
            if(AddPacketToBuffer(arrayLength1+4, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_4;
                isBuild += 1;
                operation3_flag = 0;
            } 
            else
            {
                operation3_flag = 1;
            }
        }
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_4)
    {
        TUSIGN8 arrayData0[26];
        TUSIGN8 arrayData0_temp[26];
        TUSIGN8 arrayLength0 = 0;
        TUSIGN8 row0 = 0;
        TUSIGN8 arrayData0_change = 0;
        
        arrayLength0 = TrendViewGetYaxis(row0, arrayData0_temp);
        /*
        TrendViewAcknowledgeYaxis(row0);
        
        if(!memcmp(arrayData0,arrayData0_temp,26))
        {
            arrayData0_change = 0;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+1;
            LOCAL_packet_HL[2] = arrayData0_change;
            
            if(AddPacketToBuffer(3, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_5;
                isBuild += 1;
                operation4_flag = 0;
            } 
            else
            {
                operation4_flag = 1;
            }
        }
        
        else
        */
        {
            memcpy(arrayData0,arrayData0_temp,26);
            arrayData0_change = 1;
            LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
            LOCAL_packet_HL[1] = (packetStage<<5)+arrayLength0+2;
            LOCAL_packet_HL[2] = arrayData0_change;
            LOCAL_packet_HL[3] = arrayLength0;
            memcpy(&LOCAL_packet_HL[4],arrayData0_temp,arrayLength0);
            
            if(AddPacketToBuffer(arrayLength0+4, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_5;
                isBuild += 1;
                operation4_flag = 0;
            } 
            else
            {
                operation4_flag = 1;
            }
        }
    }
    
    if(packetStage==FRAME_STAGE_OPERATION_5)
    {
        TUSIGN8 arrayDatax[2];
        TUSIGN8 arrayLengthx = 0;
        TUSIGN8 rowx;
        
        for(rowx = 0;rowx<4;rowx++)
        {
            arrayLengthx = TrendViewGetXaxis(rowx, arrayDatax);
            //TrendViewAcknowledgeXaxis(rowx);
            if(arrayLengthx)
            {
                break;
            }
        }
        
        LOCAL_packet_HL[0] = FRAME_GRAPH_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5)+arrayLengthx+2;
        LOCAL_packet_HL[2] = rowx;
        LOCAL_packet_HL[3] = arrayLengthx;        
        memcpy(&LOCAL_packet_HL[4],arrayDatax,arrayLengthx); 
        
        if(AddPacketToBuffer(arrayLengthx+4, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
            operation5_flag = 0;
        } 
        else
        {
            operation5_flag = 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;
}
#endif

// this function not size optimized beyond medium
static TUSIGN8 PackLineData_barValue(TUSIGN8* pPackedData, TUSIGN8 signal)
{
    //TUSIGN8 maxLen = 8;
    //TUSIGN8* pString;
    ADJUST_DATA bargraphDataValue;
    TUSIGN8 bargraphDatatype;
    TINT8  barPercent;
    //TBOOL rules;    
    const SIGNAL_TABLE_TYPE *pSignalInTable;
    //TUSIGN8 within_over_under, negVal;
    //TUSIGN16 i;
    //TUSIGN32 varValue;
    //NUMBER_ATTRIBUTES_TYPE varAttri;

    #ifdef HMI_USES_DOUBLE
    TDOUBLE barHigh_d, barLow_d;
    #endif
    TFLOAT barHigh, barLow, barValue;

    if(signal==0xFF)
    {
        pPackedData[0] = 0;
        return 1;
    }
    
    pSignalInTable = FrameTableGetSignal(signal);
    (void)AdjusterGetFloatValue((TUSIGN16)pSignalInTable->value.ssIdx,
                                            (TUSIGN16)pSignalInTable->value.objIdx,
                                            pSignalInTable->value.attribIdx,
                                            &bargraphDataValue,
                                            &bargraphDatatype);

    switch (pSignalInTable->FixedUpperLower)
    {
        case 0:
            #ifdef HMI_USES_DOUBLE
            PacketManagerBuilderServices_GetBarRange(pSignalInTable,&barHigh_d,&barLow_d);
            barHigh = (TFLOAT)barHigh_d;
            barLow  = (TFLOAT)barLow_d;
            #else
            PacketManagerBuilderServices_GetBarRange(pSignalInTable,&barHigh,&barLow);
            #endif
            break;

        case 1:
            barHigh = 100.0F;
            barLow = 0.0F;
            break;

        default:
            barHigh = 20.0F;
            barLow = 4.0F;
            break;
    }

    #ifdef HMI_USES_DOUBLE
    if (bargraphDatatype == ST_DOUBLE)
    {
        barValue = (TFLOAT)bargraphDataValue.data_D;
    }
    else
    #endif
    {
        barValue = bargraphDataValue.data_F;
    }
    
    if ((((*((TUSIGN32 *) &barValue) & 0x7f800000) == 0x7f800000) && ((*((TUSIGN32 *) &barValue) & 0x007fffff) != 0))
     || (((*((TUSIGN32 *) &barHigh ) & 0x7f800000) == 0x7f800000) && ((*((TUSIGN32 *) &barHigh ) & 0x007fffff) != 0))
     || (((*((TUSIGN32 *) &barLow  ) & 0x7f800000) == 0x7f800000) && ((*((TUSIGN32 *) &barLow  ) & 0x007fffff) != 0)))
    {
        barPercent = -1;
    }
    else
    {
        if (fabs((TDOUBLE)(barLow - barHigh)) == 0.0)
        {
            if (barValue >= barHigh)
            {
                barPercent = 101;
            }
            else
            {
                barPercent = -1;
            }
        }
        else
        {
            TFLOAT calc = ((barValue - barLow)/(barHigh - barLow)) * 100;
            if (calc > 100)
            {
                barPercent = 101;
            }
            else if (calc < 0)
            {
                barPercent = -1;
            }
            else
            {
                barPercent = (TUSIGN8)(calc + 0.5F);
            }
        }
    }

    pPackedData[0] = 1;
    pPackedData[1] = barPercent;
    return 2;
    
}

TUSIGN8 text[30];
ADJUST_DATA dataValue;



// this function not size optimized beyond medium
static TUSIGN8 PackLineData_value(TUSIGN8* pPackedData, TUSIGN8 signal, OPERATOR_PAGE_LINE_DATA* pLineDataParam)
{
    TUSIGN8 maxLen = 9;
    TUSIGN8* pString = NULL;
    ADJUST_DATA  HighLimit, LowLimit, Increment;
    TUSIGN8 dataType;
    TBOOL rules;    
    const SIGNAL_TABLE_TYPE *pSignalInTable;
    TUSIGN8 within_over_under=0, negVal=0;
    TUSIGN16 i;
    TUSIGN8 len =0;
    TBOOL clearZero;
    TUSIGN32 varValue;

    if(signal==0xFF)
    {
        pPackedData[0] = 0;
        return 1;
    }
    
    pSignalInTable = FrameTableGetSignal(signal);
    (void)AdjusterGetFloatValueAndLimits((TUSIGN16)pSignalInTable->value.ssIdx,
                                            (TUSIGN16)pSignalInTable->value.objIdx,
                                             pSignalInTable->value.attribIdx,
                                             &dataValue,
                                             &HighLimit,
                                             &LowLimit,
                                             &Increment,
                                             &rules,
                                             &dataType);
      
    if(dataType == ST_FLOAT)
    {         
        pString = AdjusterBuildNumberString(dataValue.data_F, maxLen, pSignalInTable->dpPosn, &negVal, &within_over_under);
        
    }
#ifdef HMI_USES_DOUBLE
    else if(dataType == ST_DOUBLE)
    {
        pString = AdjusterBuildNumberStringFromDouble(dataValue.data_D, maxLen, pSignalInTable->dpPosn, &negVal, &within_over_under);
    }
#endif
    
#ifdef HMI_SUPPORTS_INTEGER_FRAMES
    else if(dataType == ST_U8)
    {
        varValue = dataValue.data_8;        
        pString = AdjusterBuildNumberStringFromUSIGN32(varValue, LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes, &within_over_under);
    }
#endif
    
#ifdef HMI_SUPPORTS_16BIT_ENUMS
    else if(dataType == ST_U16)
    {
        varValue = dataValue.data_16;
        pString = AdjusterBuildNumberStringFromUSIGN32(varValue, LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes, &within_over_under);
    }
#endif
    
#ifdef HMI_SUPPORTS_INTEGER_FRAMES
    else if(dataType == ST_U32)
    {
        varValue = dataValue.data_32;
        pString = AdjusterBuildNumberStringFromUSIGN32(varValue, LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes, &within_over_under);
    }
#endif

    // Number too large to convert to string. Conversion routine builds "999999"
    // Flash between "999999" and up/down arrows by building a string of arrows if flash state not set
    if (((within_over_under == 1) || (within_over_under == 2)) && (!LOCAL_flashState))
    {
        for (i = 0; i < 9; i ++)
        {
            text[i] = 0x05 + within_over_under;
        }
        text[9] = 0x00;
        text[13] = 0x00;
    }
    // Number converted to string but is outside business rules)
    // Flash between "999999" and "      " by building a string of spaces if flash state not set
    else if ((pLineDataParam!=NULL) && (pLineDataParam->ruleViolation) && (!LOCAL_flashState))
    {
        for (i = 0; i < 9; i ++)
        {
            text[i] = 0x20;
        }
        text[9] = 0x00;
        text[13] = 0x00;
    }
    else
    {
        while (*pString == 0x20)
        {
            #pragma diag_suppress=Pm088
            pString ++;
            #pragma diag_default=Pm088
        }

        text[0] = negVal ? '-' : 0;
        text[1] = 0x00;
        (void)strcat((char *)text,(char *)pString);
    }
    

    len = 0;
    clearZero = eTRUE;
    for(i=0; i<30; i++)
    {
        if(text[i]==0)
        {
            break;
        }
                
        if(len==0)
        {
            if(text[i]==' ')
            {
                continue;
            }
            else
            {
                if(text[i]!='0')
                {
                    clearZero = eFALSE;
                }
                pPackedData[len+1] = text[i];
                len++;
            }
        }
        else
        {
            if(text[i]=='0')
            {
                if(clearZero)
                {
                    continue;
                }
                else
                {                    
                    pPackedData[len+1] = text[i];
                    len++;
                }
            }
            else
            {                
                clearZero = eFALSE;
                pPackedData[len+1] = text[i];
                len++;
            }
        } 
        
    }

    pPackedData[0] = len;
    
    return (len+1);
    
}

static TUSIGN8 PackLineData_tag(TUSIGN8* pPackedData, TUSIGN8 signal)
{
    const SIGNAL_TABLE_TYPE *pSignalInTable;
    E_STRING_TYPE stringType;
    const unsigned char* pString;
    TUSIGN16 i;

    if(signal==0xFF)
    {
        pPackedData[0] = 0;
        return 1;
    }
   
    pSignalInTable = FrameTableGetSignal(signal);                                                             
    LOCAL_stringtable_text_id.attribIdx = pSignalInTable->short_identifier;
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, HMI_LANGUAGE_ENGLISH);
    }
    else
    {
        pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, SFD_SIMPLE.language);
    }    
    
    

    if(stringType==STRING_TYPE_LIBIDX)
    {
        pPackedData[0] = (stringType<<5)|2;
        pPackedData[1] = pString[0];
        pPackedData[2] = pString[1];
        return 3;
    }
    else
    {
        
        for(i=0; i<3; i++)   //max tag size = 6
        {
            if(pString[i]==0)
            {
                break;
            }
            pPackedData[i+1] = pString[i];
        }        
        
        pPackedData[0] = (stringType<<5)|i;
        return (i+1);
    }  
        
}

static TUSIGN8 PackLineData_unit(TUSIGN8* pPackedData, TUSIGN8 signal)
{
    const SIGNAL_TABLE_TYPE *pSignalInTable;
    const unsigned char* pString;   
    E_STRING_TYPE unitStrType;
    TUSIGN16 i;

    if(signal==0xFF)
    {
        pPackedData[0] = 0;
        return 1;
    }
   
    pSignalInTable = FrameTableGetSignal(signal);  
    pString = PacketManagerGetEngUnits_s((TUSIGN16)pSignalInTable->engUnits.ssIdx,
                                                   pSignalInTable->engUnits.objIdx,
                                                   pSignalInTable->engUnits.attribIdx,
                                                   pSignalInTable->engUnitsType,
                                                   &unitStrType);
         
    if(unitStrType==STRING_TYPE_LIBIDX)
    {
        pPackedData[1] = pString[0];
        pPackedData[2] = pString[1];    
        pPackedData[0] = ((TUSIGN8)STRING_TYPE_LIBIDX<<5) + 2;
        return 3;
    }
    else
    {
        for(i=0; i<5; i++)
        {
            if(pString[i]==0)
            {
                break;
            }
            pPackedData[i+1] = pString[i];
        }  
        pPackedData[0] = (unitStrType<<5) + i;
        return (i+1); 
    }    

}


TUSIGN8 BuildSFrame_OperatorPage(void)
{
    TUSIGN8 frameNumber;
    TUSIGN8 displayMode;
    TUSIGN8 signal[4];
    OPERATOR_PAGE_LINE_DATA lineData[3];
    TUSIGN8* pData;
    TUSIGN8  len, totalLen;
    TUSIGN8 numBuild = 0;
    TUSIGN8 nValue = 0;

    static TUSIGN8 s_prevDisplayMode = HMI_DISPLAY_MODE_OFF;
    static TUSIGN8 s_prevDisplayLang = HMI_LANGUAGE_NOT_DEFINED;
    static TUSIGN8 s_prevFrame = 0;
    static TUSIGN8 s_prevDynamicData2[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //first byte = length
 
#ifdef HMI_USES_COMMONDIAGNOSTICS    
    static TUSIGN8 s_infoMessageID;
    //static TUSIGN8 s_NAMUR_ActiveAlarm;
    static TUSIGN8 s_preNAMURClassification;
    static TUSIGN8 s_preNAMURCategory;
    TUSIGN8 NAMURClassification;
    TUSIGN8 NAMURCategory;
#endif
    
    static TBOOL   s_alarmActive;
    //static TBOOL   s_alarmStateChanged;

    static TUSIGN8 numberLength = 8;

#if (!TEST_FRAME_OPERATOR_PAGE)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    

    frameNumber = LOCAL_pCurrDisplayFrame->frameDefinition.operatorFrame.frameNumber;    
    if ((frameNumber < 1) || (frameNumber > 6))
    {
        frameNumber = 1;
    }

    if(frameNumber!=1)
    {
        frameNumber = frameNumber;
    }
    
    displayMode = *(operatorPageData[frameNumber-1][0]);
    if (displayMode == HMI_DISPLAY_MODE_OFF)
    {
        frameNumber = 1;            
        displayMode = *(operatorPageData[frameNumber][0]);
    } 

    signal[_1ST_LINE] = *(operatorPageData[frameNumber-1][1]);
    signal[_2ND_LINE] = *(operatorPageData[frameNumber-1][2]);
    signal[_3RD_LINE] = *(operatorPageData[frameNumber-1][3]);
    signal[_BAR_LINE] = *(operatorPageData[frameNumber-1][4]);
    
    if (displayMode == HMI_DISPLAY_MODE_GRAPHIC)
    {// 2. DISPLAY_MODE_GRAPHIC not processed here
        displayMode = HMI_DISPLAY_MODE_1x4;
    }    
    else if (displayMode == HMI_DISPLAY_MODE_1x6_BAR_A)
    {// 3. DISPLAY_MODE_1x6_BAR_A uses the same signal for bargraph as that for line 1
        signal[_BAR_LINE] = signal[_1ST_LINE];
    }
    else if ((displayMode == HMI_DISPLAY_MODE_1x6_BAR_A) 
          || (displayMode == HMI_DISPLAY_MODE_1x6_BAR_B) 
          || (displayMode == HMI_DISPLAY_MODE_1x9_BAR) 
          || (displayMode == HMI_DISPLAY_MODE_2x9_BAR))
    {// 4. If "displayMode" selects a bargraph and the signal assigned to bargraph has no bargraph limits applied (fixedUpperLower > = 3)

        if(FrameTableGetSignal(signal[_BAR_LINE])->FixedUpperLower >= 3)
        {
            displayMode --;  //    Then adjust display mode to remove bargraph.
        }        
    }


#ifdef HMI_USES_COMMONDIAGNOSTICS
    
    //s_NAMUR_ActiveAlarm = AdjusterGetNAMURStatus(&NAMURClassification, &NAMURCategory);
    AdjusterGetNAMURStatus(&NAMURClassification, &NAMURCategory);
    
#ifdef INFORMATION_MESSAGES_OFFSET
    s_infoMessageID = GetInfoMessageID((TBOOL)(NAMURClassification == CLASSIFICATION_NONE));
#else
    s_infoMessageID = 0;
#endif

    if ((NAMURClassification != CLASSIFICATION_NONE) || (s_infoMessageID != 0))
    {
        //if (!s_alarmActive)
        //{
        //    s_alarmStateChanged = eTRUE;
        //}
        s_alarmActive = eTRUE;
    }
    else
    {
        //if (s_alarmActive)
        //{
        //    s_alarmStateChanged = eTRUE;
        //}
        s_alarmActive = eFALSE;
    }
#else
    //s_alarmStateChanged = eFALSE;
    s_alarmActive = eFALSE;
#endif


    // From this point onwards DisplayStyle is used  rather than displayMode   
    #pragma diag_suppress=Pm020
    DISPLAYSTYLE displayStyle = (DISPLAYSTYLE)((displayMode * 4) + (LOCAL_Asian ? 2 : 0) + (s_alarmActive ? 1 : 0));
    #pragma diag_default=Pm020


    #pragma diag_suppress=Pm057
    switch (displayMode)
    {
        case HMI_DISPLAY_MODE_3x9:  
            nValue ++;
            (void)PacketManagerBuilderServices_PrepareOperatorPageLineData(3, signal[_3RD_LINE], &lineData[_3RD_LINE], displayStyle);

        case HMI_DISPLAY_MODE_2x9:
        case HMI_DISPLAY_MODE_2x9_BAR:
            nValue ++;
            (void)PacketManagerBuilderServices_PrepareOperatorPageLineData(2, signal[_2ND_LINE], &lineData[_2ND_LINE], displayStyle);
            
        case HMI_DISPLAY_MODE_1x9:
        case HMI_DISPLAY_MODE_1x9_BAR:
            nValue ++;
            (void)PacketManagerBuilderServices_PrepareOperatorPageLineData(1, signal[_1ST_LINE], &lineData[_1ST_LINE], displayStyle);
            break;
            
        default:
            nValue = 0;
            (void)PacketManagerBuilderServices_PrepareOperatorPageLineData(1, signal[_1ST_LINE], &lineData[_1ST_LINE], displayStyle);
            break;
    }
    #pragma diag_default=Pm057

    for ( ; nValue >0; nValue --)
    {
        TUSIGN8 j = nValue - 1;
        const SIGNAL_TABLE_TYPE *pSignalInTable = FrameTableGetSignal(signal[j]);
        TUSIGN8 res = pSignalInTable->dpPosn;
        TUSIGN8 tempNumberLength = 8;

        TFLOAT  tVal, limit_8chars, limit_9chars;
        TBOOL   tVal_NAN = eFALSE;

        // if resolution is USE_RANGE then recalculate
        PacketManagerBuilderServices_AdjustResolutionForRange(pSignalInTable, &res);

        // if resolution is a configuarable DP position then get the actual DP position
        AdjusterAdjustResolutionForCustomDP(&res);

        #ifdef HMI_USES_DOUBLE
        if (lineData[j].dataType == ST_DOUBLE)
        {
            tVal = (TFLOAT)lineData[j].dataValue.data_D;
        }
        else
        #endif
        {
            tVal = (TFLOAT)lineData[j].dataValue.data_F;

            TFLOAT testNAN = tVal;

            if (((*((TUSIGN32 *) &testNAN) & 0x7f800000) == 0x7f800000)
             && ((*((TUSIGN32 *) &testNAN) & 0x007fffff) != 0))
            {
                tVal_NAN = eTRUE;
            }                   
        }

        if (tVal_NAN)
        {
            tempNumberLength = 8;
        }
        else
        {
            if (tVal < 0)
            {
                tVal *= -1.0F;
            }

            if (res == MAX_RESOLUTION)
            {
                res = 0;
            }

            switch (res)
            {
            case 0:     limit_9chars = 100000000;  limit_8chars = 10000000; break;
            case 1:     limit_9chars = 10000000;   limit_8chars = 1000000;  break;
            case 2:     limit_9chars = 1000000;    limit_8chars = 100000;   break;
            case 3:     limit_9chars = 100000;     limit_8chars = 10000;    break;
            case 4:     limit_9chars = 10000;      limit_8chars = 1000;     break;
            case 5:     limit_9chars = 1000;       limit_8chars = 100;      break;
            case 6:     limit_9chars = 100;        limit_8chars = 10;       break;
            case 7:     limit_9chars = 10;         limit_8chars = -1;       break;
            default:    limit_9chars = -1;         limit_8chars = -1;       break;
            }

            if (tVal < limit_8chars)
            {
                tempNumberLength = 8;
            }
            else if (tVal < limit_9chars)
            {
                tempNumberLength = 9;
            }
            else
            {
                tempNumberLength = 13;
            }
        }

        if (tempNumberLength > numberLength)
        {
            numberLength = tempNumberLength;
        }
    }


    typedef enum
    {   
        FRAME_STAGE_STATIC_1 = 0,  //tag&other
        FRAME_STAGE_STATIC_2,      //unit
        FRAME_STAGE_DYNAMIC_1,     //alarm
        FRAME_STAGE_DYNAMIC_2,     //vars 
        FRAME_STAGE_END = 7,
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

   

    if ((displayMode         != s_prevDisplayMode)
     || (testLanguage != s_prevDisplayLang)
     || (frameNumber         != s_prevFrame)
     || LOCAL_updateStaticArea)             //update all
    {
        LOCAL_packetStage =  FRAME_STAGE_STATIC_1;  
        packetStage = FRAME_STAGE_STATIC_1; 

        //for the first time, all frames will be packed.
        s_preNAMURCategory = 0xFF;
        s_preNAMURClassification = 0xFF;
        s_prevDynamicData2[0] = 0xFF;        
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if(LOCAL_updateActiveArea)
        {  
            LOCAL_packetStage =  FRAME_STAGE_DYNAMIC_1;
            packetStage = FRAME_STAGE_DYNAMIC_1;            
            
            LOCAL_updateActiveArea = eFALSE;
        }
        else
        {
            return 0;
        }
    }
       
    if(packetStage==FRAME_STAGE_STATIC_1)
    {
        LOCAL_packet_HL[0] = FRAME_OPERATOR_PAGE;
        //LOCAL_packet_HL[1] = ?;                //length

        

        LOCAL_packet_HL[2] = testLanguage;
        s_prevDisplayLang = testLanguage;
        
        LOCAL_packet_HL[3] = frameNumber;
        LOCAL_packet_HL[4] = displayMode;        //Number of variables
        LOCAL_packet_HL[5] = FrameTableIsAutoscrollEnabled();
        totalLen = 4;
        pData = &LOCAL_packet_HL[6];
        switch(displayMode)
        {        
            case HMI_DISPLAY_MODE_1x4:
            case HMI_DISPLAY_MODE_1x6_A:
            case HMI_DISPLAY_MODE_1x6_B:
            case HMI_DISPLAY_MODE_1x9:
                totalLen += PackLineData_tag(pData, signal[_1ST_LINE]);
                break;
            
            case HMI_DISPLAY_MODE_1x6_BAR_A:
            case HMI_DISPLAY_MODE_1x6_BAR_B:
            case HMI_DISPLAY_MODE_1x9_BAR:
                len = PackLineData_tag(pData, signal[_1ST_LINE]);
                totalLen += len;
                pData += len;
                totalLen += PackLineData_tag(pData, signal[_BAR_LINE]);
                break;
            
            
            case HMI_DISPLAY_MODE_2x9:
                len = PackLineData_tag(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_tag(pData, signal[_2ND_LINE]);
                break;                

            case HMI_DISPLAY_MODE_2x9_BAR:
                len = PackLineData_tag(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_tag(pData, signal[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_tag(pData, signal[_BAR_LINE]);
                break;
                
            //case HMI_DISPLAY_MODE_3x9:     
            default:
                len = PackLineData_tag(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_tag(pData, signal[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_tag(pData, signal[_3RD_LINE]);
                break;   
                
        }
        LOCAL_packet_HL[1] = (packetStage<<5) | totalLen; 
        if(AddPacketToBuffer(2+totalLen, 0))
        {
            s_prevDisplayMode = displayMode;
            s_prevFrame = frameNumber;   
            packetStage = FRAME_STAGE_STATIC_2;
            numBuild += 1;
        }
        
    }

    if(packetStage==FRAME_STAGE_STATIC_2)
    {
        LOCAL_packet_HL[0] = FRAME_OPERATOR_PAGE;
        //LOCAL_packet_HL[1] = ?;                //length        
        totalLen = 0;
        pData = &LOCAL_packet_HL[2];
        switch(displayMode)
        {        
            case HMI_DISPLAY_MODE_1x4:
            case HMI_DISPLAY_MODE_1x6_A:
            case HMI_DISPLAY_MODE_1x6_B:
            case HMI_DISPLAY_MODE_1x9:
                totalLen += PackLineData_unit(pData, signal[_1ST_LINE]);
                break;
            
            case HMI_DISPLAY_MODE_1x6_BAR_A:
            case HMI_DISPLAY_MODE_1x6_BAR_B:
            case HMI_DISPLAY_MODE_1x9_BAR:
                len = PackLineData_unit(pData, signal[_1ST_LINE]);
                totalLen += len;
                pData += len;
                totalLen += PackLineData_unit(pData, signal[_BAR_LINE]);
                break;
            
            
            case HMI_DISPLAY_MODE_2x9:
                len = PackLineData_unit(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_unit(pData, signal[_2ND_LINE]);
                break;                

            case HMI_DISPLAY_MODE_2x9_BAR:
                len = PackLineData_unit(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_unit(pData, signal[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_unit(pData, signal[_BAR_LINE]);
                break;
                
            //case HMI_DISPLAY_MODE_3x9:     
            default:
                len = PackLineData_unit(pData, signal[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_unit(pData, signal[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_unit(pData, signal[_3RD_LINE]);
                break;   
                
        }
        LOCAL_packet_HL[1] = (packetStage<<5) + totalLen;  
        if(AddPacketToBuffer(2+totalLen, 0))
        {
            packetStage = FRAME_STAGE_DYNAMIC_1;
            numBuild += 1;
        }
    }

    if(packetStage==FRAME_STAGE_DYNAMIC_1)
    {//ALARM string

        if((NAMURClassification!=s_preNAMURClassification)
            ||(NAMURCategory!=s_preNAMURCategory))
        {
            LOCAL_packet_HL[0] = FRAME_OPERATOR_PAGE;        
#ifdef HMI_USES_COMMONDIAGNOSTICS 
            LOCAL_packet_HL[2] = NAMURClassification;
            LOCAL_packet_HL[3] = NAMURCategory;     
            totalLen = 2;
#else
            totalLen = 0;
#endif
        
            LOCAL_packet_HL[1] = (packetStage<<5) + totalLen; 
            if(AddPacketToBuffer(2+totalLen, 0))
            {
                s_preNAMURCategory = NAMURCategory;
                s_preNAMURClassification = NAMURClassification;
            
                packetStage = FRAME_STAGE_DYNAMIC_2;
                numBuild += 1;
            }

            
        }
        else
        {
            packetStage = FRAME_STAGE_DYNAMIC_2;
        }
        
    }

    if(packetStage==FRAME_STAGE_DYNAMIC_2)
    {
        LOCAL_packet_HL[0] = FRAME_OPERATOR_PAGE;
        //LOCAL_packet_HL[1] = len;
        pData = &LOCAL_packet_HL[2];
        totalLen = 0;
        
        switch(displayMode)
        {        
            case HMI_DISPLAY_MODE_1x4:
            case HMI_DISPLAY_MODE_1x6_A:
            case HMI_DISPLAY_MODE_1x6_B:
            case HMI_DISPLAY_MODE_1x9:
                totalLen += PackLineData_value(pData, signal[_1ST_LINE], &lineData[_1ST_LINE]);
                break;
            
            case HMI_DISPLAY_MODE_1x6_BAR_A:
            case HMI_DISPLAY_MODE_1x6_BAR_B:
            case HMI_DISPLAY_MODE_1x9_BAR:
                len = PackLineData_value(pData, signal[_1ST_LINE], &lineData[_1ST_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_barValue(pData, signal[_BAR_LINE]);
                break;
            
            
            case HMI_DISPLAY_MODE_2x9:
                len = PackLineData_value(pData, signal[_1ST_LINE], &lineData[_1ST_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_value(pData, signal[_2ND_LINE], &lineData[_2ND_LINE]);
                break;                

            case HMI_DISPLAY_MODE_2x9_BAR:
                len = PackLineData_value(pData, signal[_1ST_LINE], &lineData[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_value(pData, signal[_2ND_LINE], &lineData[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_barValue(pData, signal[_BAR_LINE]);
                break;
                
            //case HMI_DISPLAY_MODE_3x9:     
            default:
                len = PackLineData_value(pData, signal[_1ST_LINE], &lineData[_1ST_LINE]);
                pData += len;
                totalLen += len;
                len = PackLineData_value(pData, signal[_2ND_LINE], &lineData[_2ND_LINE]);
                pData += len;
                totalLen += len;
                totalLen += PackLineData_value(pData, signal[_3RD_LINE], &lineData[_3RD_LINE]);
                break;   
                
        }

        if((totalLen!=s_prevDynamicData2[0])
         ||(!IsSamePacket(&LOCAL_packet_HL[2], &s_prevDynamicData2[1], totalLen)))
        {
            LOCAL_packet_HL[1] = (packetStage<<5) + totalLen; 
            if(AddPacketToBuffer(2+totalLen, 0))
            { 
                if(totalLen<=31)
                {
                    memcpy(&s_prevDynamicData2[1], &LOCAL_packet_HL[2], totalLen);
                    s_prevDynamicData2[0] = totalLen;
                }  

                packetStage = FRAME_STAGE_END;
                numBuild += 1;
            }             
        }
        else
        {
            packetStage = FRAME_STAGE_END;
        }
         
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            numBuild += 1;
        }        
    }    
    
    return numBuild;    
    
}

#endif // end of "#if HMI_SUPPORTS_OPERATOR_FRAMES"


#ifdef HMI_SUPPORTS_CONFIG_HEADER_FRAMES
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_ConfigHeader(void)
{
    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        CFH_FRAME_PACKET_TITLE_BAR,                 // Title Bar                               | - Start of Static Area - Start of keypress area
                                                    //-----------------------------------------|
        CFH_FRAME_PACKET_TITLE,                     // Config header name eg "Device"          |
                                                    //-----------------------------------------|
        CFH_FRAME_PACKET_SLIDER_BAR,                // Slider bar                              |
                                                    //-----------------------------------------|
        CFH_FRAME_RIGHT_KEY,                        // right function key "Select"             |
                                                    //-----------------------------------------|

    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //SHOW_LOGIN_LEVEL
      #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|
        CFH_FRAME_LOGIN_LEVEL_F,                    // Login level Icon (for SIZE_F)           |
                                                    //-----------------------------------------|
      #else // else condition for #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)


        #if defined(HMI_SUPPORTS_ASIAN) && !defined(TYPE_4_IMPLEMENTATION) && !defined(TYPE_5_IMPLEMENTATION)
                                                    //-----------------------------------------|
        CFH_FRAME_LOGIN_LEVEL_6,                    // Login level Icon (on line 6)            |
                                                    //-----------------------------------------|
        #endif
                                                    //-----------------------------------------|
        CFH_FRAME_LOGIN_LEVEL_7,                    // Login level Icon (on Line 7)            |
                                                    //-----------------------------------------|
      #endif   // end of else condition for #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
    #endif  // end of #ifdef COMMON_TOP_WORKS_NEW_FEATURES

    #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|
        CFH_FRAME_ICON,                             // Animated Icon                           | - Start of Active Area
                                                    //-----------------------------------------|
    #else // else condition for #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_TL,                          // Icon top left                           | - Start of Active Area
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_TR,                          // Icon top right                          |
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_CL,                          // Icon middle left                        |
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_CR,                          // Icon middle right                       |
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_BL,                          // Icon bottom left                        |
                                                    //-----------------------------------------|
        CFH_FRAME_ICON_BR,                          // Icon bottom right                       |
                                                    //-----------------------------------------|
    #endif   // end of else condition for #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|                                                
        CFH_FRAME_LEFT_KEY,                         //left function key  "Exit"                |
                                                    //-----------------------------------------|
        CFH_FRAME_READ_SWITCHES                     //                                         |
                                                    //-----------------------------------------|
    }CFH_FRAME_PACKET;
  
  #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
        const TUSIGN8 START_OF_ANIMATION = CFH_FRAME_ICON;
#else
        //const TUSIGN8 START_OF_ANIMATION = CFH_FRAME_ICON_TL;
        TUSIGN8 startPacket;
#endif
  
   TUSIGN8 isBuild = 0;
   TUSIGN8 IconID1 = 0; 
   TUSIGN8 IconID2 = 0; 
   TUSIGN8 IconID3 = 0; 
   TUSIGN8 IconID4 = 0; 
   TUSIGN8 IconID5 = 0; 
   TUSIGN8 IconID6 = 0; 
      
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   				
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

#if (!TEST_FRAME_CONFIG_HEADER)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    if (LOCAL_updateStaticArea || LOCAL_keypressProcessed || (SFD_SIMPLE.language != preDisplayLang))
    {       
        preDisplayLang = SFD_SIMPLE.language;
        packetStage = FRAME_STAGE_STATIC;
        LOCAL_iconStage = 0;
        LOCAL_updateStaticArea = eFALSE;
    }

    if(packetStage == FRAME_STAGE_STATIC)
    {    
        TUSIGN8 sliderbar_num = 0;
        TUSIGN8 sliderbar_high = 0;
        TUSIGN8 loop_count = 0;
        E_STRING_TYPE stringType;
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.configHeaderFrame.title_attribidx;
        

        //TUSIGN8 const * headerName = FrameTableGetString(LOCAL_stringtable_text_id);
        TUSIGN8 const * headerName = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);

        TUSIGN16 actual = (TINT16)(LOCAL_pCurrDisplayFrame - FrameTableGetConfigHeaderAddress(0));
        TUSIGN16 high   = (TINT16)(FrameTableGetConfigHeaderAddress(1) - FrameTableGetConfigHeaderAddress(0));
        sliderbar_num  = (TUSIGN8)actual;
        sliderbar_high = (TUSIGN8)high;
        
        if(stringType == STRING_TYPE_LIBIDX)
        {
            loop_count = 2;
        }
        else
        {
            for(loop_count=0; loop_count<28;loop_count++)
            {
                if(headerName[loop_count] == '\0')
                {
                    break;
                }
            }           
        }
        
        
        TUSIGN8 IconID = ARM_ACCESS_INVALID;

        #ifdef HMI_SUPPORTS_SECURITY_ACCESS
        IconID = AdjusterGetARMLevel();
        #endif
        LOCAL_packet_HL[0] = FRAME_CONFIG_HEADER;
        LOCAL_packet_HL[1] = (packetStage<<5)+loop_count+5;   
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = sliderbar_num;
        LOCAL_packet_HL[4] = sliderbar_high;
        LOCAL_packet_HL[5] = IconID;
        LOCAL_packet_HL[6] = (stringType<<5)|loop_count;
        memcpy(&LOCAL_packet_HL[7], headerName, loop_count);
        if(AddPacketToBuffer(loop_count+7, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild +=1;
        }          
    }
	  
    if(packetStage == FRAME_STAGE_OPERATION)
    {
        LOCAL_iconStage ++;
        if ((LOCAL_iconStage >= 6) || (LOCAL_pCurrDisplayFrame->frameDefinition.configHeaderFrame.icon[LOCAL_iconStage] == 0))          
        {
            LOCAL_iconStage = 0;
        }
        
        LOCAL_packetStage = CFH_FRAME_ICON_TL;
        #if !defined (TYPE_4_IMPLEMENTATION) && !defined (TYPE_5_IMPLEMENTATION)
        TUSIGN8 iconNum = LOCAL_pCurrDisplayFrame->frameDefinition.configHeaderFrame.icon[LOCAL_iconStage];
        TUSIGN8 iconPart;
        #endif

        //TUSIGN8 iconLineOffset = LOCAL_Asian? 0 : 1;
        do
        {
            startPacket = LOCAL_packetStage;
            iconPart = FrameTableGetConfigIconPart(iconNum,(TUSIGN8)(LOCAL_packetStage - CFH_FRAME_ICON_TL));

            if(LOCAL_packetStage==CFH_FRAME_ICON_TL)
            {
                IconID1 = iconPart;
            }
            if(LOCAL_packetStage==CFH_FRAME_ICON_TR)
            {
                IconID2 = iconPart;
            }
            if(LOCAL_packetStage==CFH_FRAME_ICON_CL)
            {
                IconID3 = iconPart;
            }
            if(LOCAL_packetStage==CFH_FRAME_ICON_CR)
            {
                IconID4 = iconPart;
            }
            if(LOCAL_packetStage==CFH_FRAME_ICON_BL)
            {
                IconID5 = iconPart;
            }
            
            if(LOCAL_packetStage==CFH_FRAME_ICON_BR)
            {
                IconID6 = iconPart;
            }

                    
            /*                 
            else
            {
                // configurable icon
                packetsBuilt += BuildWriteArray(iconAddr,
                LARGE_ICON_SPLIT_WIDTH,
                icon_xPos[LOCAL_packetStage - CFH_FRAME_ICON_TL],
                (TUSIGN8)(icon_yPos[LOCAL_packetStage - CFH_FRAME_ICON_TL] + iconLineOffset),
                0);
            } 
            */
            LOCAL_packetStage++;

        } while ((startPacket != LOCAL_packetStage) && (LOCAL_packetStage <= CFH_FRAME_ICON_BR));
        
        LOCAL_packet_HL[0] = FRAME_CONFIG_HEADER;							   
	LOCAL_packet_HL[1] = (packetStage<<5)+6; 
	LOCAL_packet_HL[2] = IconID1;
        LOCAL_packet_HL[3] = IconID2;
        LOCAL_packet_HL[4] = IconID3;
        LOCAL_packet_HL[5] = IconID4;
        LOCAL_packet_HL[6] = IconID5;
        LOCAL_packet_HL[7] = IconID6;

	if(AddPacketToBuffer(8, 0))
	{
            packetStage = FRAME_STAGE_END;
            isBuild +=1;
	}
    }
	
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END; 
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
		 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1;
        }        
    }
    
    return isBuild;

}


#endif    // end of "#if HMI_SUPPORTS_CONFIG_HEADER_FRAMES"


#ifdef HMI_SUPPORTS_MENU_FRAMES

TUSIGN8 BuildSFrame_StdMenu(void)
{
    TUSIGN8 isBuild = 0;
    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_TITLE_BAR,                // Title Bar                               | - Start of Static Area
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_LEFT_KEY,                 // left function key  "Back"               |
                                                    //-----------------------------------------|
    #ifdef COMMON_TOP_WORKS_NEW_FEATURES            //SHOW_LOGIN_LEVEL
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_LOGIN_LEVEL_7,            // Login level Icon (on line 7)            |
                                                    //-----------------------------------------|
    #endif  // end of #ifdef COMMON_TOP_WORKS_NEW_FEATURES
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_1,              // Menu Item (1/3) (1/4) (1/5) (1/6)       | - Start of keypress area - Start of active area for Signals view menu
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_2,              // Menu Item (2/3) (2/4) (2/5) (2/6)       |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_3,              // Menu Item (3/3) (3/4) (3/5) (3/6)       |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_4,              // Menu Item       (4/4) (4/5) (4/6)       |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_5,              // Menu Item             (5/5) (5/6)       |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_MENU_ITEM_6,              // Menu Item                   (6/6)       |
                                                    //-----------------------------------------|

    #if !defined (TYPE_1_IMPLEMENTATION) || !defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|                                                
        MENU_FRAME_PACKET_HL_ON_1,                  // LHS of highlighted Item (ypos = 1)      |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_HL_ON_2,                  // LHS of highlighted Item (ypos = 2)      |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_HL_ON_3,                  // LHS of highlighted Item (ypos = 3)      |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_HL_ON_4,                  // LHS of highlighted Item (ypos = 4)      |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_HL_ON_5,                  // LHS of highlighted Item (ypos = 5)      |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_HL_ON_6,                  // LHS of highlighted Item (ypos = 6)      |
                                                    //-----------------------------------------|
    #endif
                                                    
    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //SHOW_LOGIN_LEVEL
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_LOGOUT_ICON_T,            // Logout Icon                             |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_LOGOUT_ICON_B,            // Logout Icon                             |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_READONLY_ICON_T,          // Read Only Icon                          |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_READONLY_ICON_B,          // Read Only Icon                          |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_STANDARD_ICON_T,          // Standard Icon                           |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_STANDARD_ICON_B,          // Standard Icon                           |
                                                    //-----------------------------------------|                                   
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_ADVANCED_ICON_T,          // Advanced Icon                           |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_ADVANCED_ICON_B,          // Advanced Icon                           |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_SERVICE_ICON_T,           // Service Icon                            |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_SERVICE_ICON_B,           // Service Icon                            |
                                                    //-----------------------------------------|
    #endif
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_SLIDER_OR_SECURITY,       // Slider bar or Security Icon             |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_RIGHT_KEY,                // Right function key  "Select"            |
                                                    //-----------------------------------------|
        MENU_FRAME_PACKET_READ_SWITCHES,            //                                         |
                                                    //-----------------------------------------| - There is no active area for standard menu frames
    }MENU_FRAME_PACKET;

    

    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_KEYPRESS,
        FRAME_STAGE_KEYPRESS_2,
        FRAME_STAGE_KEYPRESS_3,
        FRAME_STAGE_KEYPRESS_4,
        FRAME_STAGE_KEYPRESS_5,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
    
    static TUSIGN8 ismenuitemadd1 = 0x01;
    static TUSIGN8 ismenuitemadd2 = 0x01;
    static TUSIGN8 ismenuitemadd3 = 0x01;
    static TUSIGN8 ismenuitemadd4 = 0x01;
    static TUSIGN8 ismenuitemadd5 = 0x01;
     
    TUSIGN8 menuiteminvert_1 = 0;
    TUSIGN8 menuiteminvert_2 = 0;
    TUSIGN8 menuiteminvert_3 = 0;
    TUSIGN8 menuiteminvert_4 = 0;

    TUSIGN8 menuItem1[25] = {0};
    TUSIGN8 menuItem2[25] = {0};
    TUSIGN8 menuItem3[25] = {0};
    TUSIGN8 menuItem4[25] = {0};
    
    TUSIGN8 titlebar[19] = {0};
    
    TUSIGN8 menuItemLenth1 = 0;
    TUSIGN8 menuItemLenth2 = 0;
    TUSIGN8 menuItemLenth3 = 0;
    TUSIGN8 menuItemLenth4 = 0;
    
    E_STRING_TYPE stringType;
    
    TUSIGN8 const * pMenuText;
    
    TUSIGN8 rightKey;
    TUSIGN8 sliderbar_num;
    TUSIGN8 sliderbar_high = 0;

    //cursorSel = FrameTableGetCursorPosition();

    #pragma diag_suppress=Pm080
    #define MAX_MENU_ITEMS 24
    #pragma diag_default=Pm080

    MENU_ITEM_TYPE const *pMenuItem;

    TINT16 *pDisplayedSelections;
    TUSIGN8 DisplayedSelectionsIndex;

    #ifdef HMI_USES_NEWLINE
    static TBOOL   NewlineState[2] = {eFALSE,eFALSE};
    static TUSIGN8 NewlineCount[2] = {0,0};
    #endif

    static TINT16 DisplayedSelections_LOCAL[6];
    #ifdef HMI_SUPPORTS_SERVICEPORT
    static TINT16 DisplayedSelections_REMOTE[6];

#if (!TEST_FRAME_MENU_USER)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif

    if (PacketManagerIsRemoteInstance())
    {
        pDisplayedSelections = DisplayedSelections_REMOTE;
        instance = 1;
    }
    else
    #endif
    {
        pDisplayedSelections = DisplayedSelections_LOCAL;
    }

    #ifdef HMI_USES_NEWLINE
    NewlineCount[instance] ++;
    if (NewlineCount[instance] >= HMI_DISPLAY_RATE_CYCLES)
    {
        NewlineCount[instance] = 0;
        if (NewlineState[instance])
        {
            NewlineState[instance] = eFALSE;
        }
        else
        {
            NewlineState[instance] = eTRUE;
        }
    }

    if ((LOCAL_updateStaticArea) || (LOCAL_keypressProcessed) || LOCAL_updateActiveArea)
    {
        NewlineState[instance] = eFALSE;
        NewlineCount[instance] = 0;        
    }
    #endif
    
    if ((SFD_SIMPLE.language != preDisplayLang) ||LOCAL_updateStaticArea || LOCAL_keypressProcessed)
    {
        LOCAL_updateStaticArea = eTRUE;
        packetStage = FRAME_STAGE_STATIC;
        
        for (DisplayedSelectionsIndex = 0; DisplayedSelectionsIndex < 6; DisplayedSelectionsIndex ++)
        {
            pDisplayedSelections[DisplayedSelectionsIndex] = -1;
        }
    }
    
    if(!ismenuitemadd1)
    {
        packetStage=FRAME_STAGE_KEYPRESS;  
    }
    if(ismenuitemadd1 && !ismenuitemadd2)
    {
        packetStage=FRAME_STAGE_KEYPRESS_2;
    }
    if(ismenuitemadd1 && ismenuitemadd2 && !ismenuitemadd3)
    {
        packetStage=FRAME_STAGE_KEYPRESS_3;
    }
    
    if(ismenuitemadd1 && ismenuitemadd2 && ismenuitemadd3 && (!ismenuitemadd4))
    {
        packetStage=FRAME_STAGE_KEYPRESS_4;
    }
    
    if(ismenuitemadd1 &&ismenuitemadd2 && ismenuitemadd3 && ismenuitemadd4 && (!ismenuitemadd5))
    {
        packetStage=FRAME_STAGE_KEYPRESS_5;
    }
    else
    {
        asm("nop");
    } 
   
    if(packetStage==FRAME_STAGE_STATIC)
    {       
        TUSIGN8 const *ptitlebar;
        TUSIGN8 titlebarLength;
        
        TUSIGN8 LOGIN_ICON_LEVEL7;
        
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.titleBar_attribidx;
        
        ptitlebar = FrameTableGetStringSpecificLanguage_s(&stringType,LOCAL_stringtable_text_id, testLanguage);
  
        if(stringType == STRING_TYPE_LIBIDX)
        {
            titlebarLength = 2;
        }
        else
        {
            for(titlebarLength=0; titlebarLength<MAX_LENGTH_FRAME_TITLE;titlebarLength++)
            {
                if(ptitlebar[titlebarLength] == '\0')
                {
                    break;
                }
            }
        }
        

        titlebar[0] = (stringType<<5)|titlebarLength;
        memcpy(&titlebar[1],ptitlebar,titlebarLength);
            
    #ifdef COMMON_TOP_WORKS_NEW_FEATURES    //SHOW_LOGIN_LEVEL

        /*
        specialMenuFlagID       |   Menu Name
                                |
        0-                      |   std menu
        1                       |   Operator Menu
        2                       |   Signals view
        3                       |   Diagnostics View
        4                       |   ARM Access level Selection
        5                       |   Alarm History
        6                       |   Secure Alarm History    
        */
            
        /*---------------Login level Icon (on line 7)-----------------*/
        
        if ((LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 0)   
         || (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 4))  
        {
            TUSIGN8 IconID = ARM_ACCESS_INVALID;
            #ifdef HMI_SUPPORTS_SECURITY_ACCESS
            IconID = AdjusterGetARMLevel();            /* Get access level icon ID to display*/
            #endif

            LOGIN_ICON_LEVEL7= IconID;  
        }
        else
        {
            LOGIN_ICON_LEVEL7 = 0xFF;                   /* Not std menu or access level selection menu, icon is unnecessary */
        }
        
    #endif  // end of #ifdef COMMON_TOP_WORKS_NEW_FEATURES

        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+titlebarLength+3;
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = LOGIN_ICON_LEVEL7;
        memcpy(&LOCAL_packet_HL[4],titlebar,titlebarLength+1);
        if(AddPacketToBuffer(titlebarLength+5, 0))
        {
            packetStage = FRAME_STAGE_KEYPRESS;
            isBuild += 1;
        }          
    }
    
    do
    {   
        LOCAL_packetStage = MENU_FRAME_PACKET_MENU_ITEM_1;
        
        //---------------------------------------------//-----------------------------------------|
        // MENU_FRAME_PACKET_MENU_ITEM_1,              // Menu Item (1/3) (1/4) (1/5) (1/6)       |
        // MENU_FRAME_PACKET_MENU_ITEM_2,              // Menu Item (2/3) (2/4) (2/5) (2/6)       |
        // MENU_FRAME_PACKET_MENU_ITEM_3,              // Menu Item (3/3) (3/4) (3/5) (3/6)       |
        // MENU_FRAME_PACKET_MENU_ITEM_4,              // Menu Item       (4/4) (4/5) (4/6)       |
        // MENU_FRAME_PACKET_MENU_ITEM_5,              // Menu Item             (5/5) (5/6)       |
        // MENU_FRAME_PACKET_MENU_ITEM_6,              // Menu Item                   (6/6)       |
        //---------------------------------------------//-----------------------------------------|
        if ((LOCAL_packetStage >= MENU_FRAME_PACKET_MENU_ITEM_1) && (LOCAL_packetStage <= MENU_FRAME_PACKET_MENU_ITEM_6))
        {
            TUSIGN8 menuSelection = FrameTableGetCursorPosition();
            TBOOL   menuSelectionShown;
            TBOOL   ShowCursor = eFALSE;
            TUSIGN8 startPacket;

            TUSIGN8  itemsPerMenu;
            if (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 2)
            {
                itemsPerMenu = 3;
            }
            if((testLanguage==HMI_LANGUAGE_CHINESE))
            {
                itemsPerMenu = 2;
            }
            else
            {
                itemsPerMenu = 4;
            }
            
            TUSIGN16 firstItem = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.firstItem_MSByte << 8) + (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.firstItem_LSByte;
            TUSIGN16 lastItem = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.lastItem_MSByte << 8) + (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.lastItem_LSByte;

            //Check if the current menu selection is being displayed
            menuSelectionShown = eFALSE;
            DisplayedSelectionsIndex = 0;

            while ((menuSelectionShown == eFALSE) && (DisplayedSelectionsIndex < itemsPerMenu))
            {
                if (pDisplayedSelections[DisplayedSelectionsIndex] == menuSelection)
                {
                    menuSelectionShown = eTRUE;
                }
                else
                {
                    DisplayedSelectionsIndex ++;
                }
            }

            if (menuSelectionShown)
            {
                // current menu selection is being displayed..just set the cursor position
                LOCAL_highlightCursor = DisplayedSelectionsIndex;
                ShowCursor = eTRUE;
            }
            else
            {
                // current menu selection is not being displayed
                // Get Details of what to display (Create an array of valid cursor values)

                TUSIGN8  numberOfItems = 0;
                TUSIGN8  rangeIndex = 0;
                TUSIGN16 menuItem;

                #if (MAX_SIGNALS > MAX_MENU_ITEMS)
                TUSIGN8  menuItemTab[MAX_SIGNALS] = {0};

                #else
                TUSIGN8  menuItemTab[MAX_MENU_ITEMS] = {0};
                #endif

                // Get Details of what to display (Create an array of valid cursor values)
                for(menuItem = firstItem, rangeIndex = 0; menuItem <= lastItem; menuItem++, rangeIndex ++)
                {
                    
                    MENU_ITEM_TYPE const *pStdMenuItem = FrameTableGetMenuItem(menuItem);

                    TUSIGN8 itemValid = 0;

                    if (pStdMenuItem != NULL)
                    {
                        itemValid = ValidityTestCheck(
                                                        #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
                                                        (FRAME_FORMAT_TYPE const*)NULL,
                                                        #endif
                                                        (TUSIGN16)pStdMenuItem->validTest
                                                        #ifdef VALIDITY_TEST_CALLER
                                                        ,SIGNALS_TEST_NOT_APPLICABLE
                                                        #endif
                                                        );
                    }
                    
                    if (itemValid)
                    {
                        menuItemTab[numberOfItems] = rangeIndex;
                        numberOfItems++;
                    }
                    
                }

                VIP_ASSERT(numberOfItems);      // at least one item in the menu must be valid
                rangeIndex = 0;
                while ((menuSelection != menuItemTab[rangeIndex]) && (rangeIndex < numberOfItems))
                {
                    rangeIndex ++;
                }

                if (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag != 2)
                {
                    LOCAL_sliderBarHigh = numberOfItems-1;
                    LOCAL_sliderBarActual = rangeIndex;
                }

                if (LOCAL_updateStaticArea)
                {
                     LOCAL_updateStaticArea = eFALSE;
                    // update static area flag is set - build a new display from scratch
                    if (rangeIndex >= numberOfItems)
                    {
                        // (rangeIndex >= numberOfItems) - menu selection is not valid
                        LOCAL_highlightCursor = 0;                      // set the cursor to the first line
                        ShowCursor = eFALSE;                            // do not allow the cursor to be shown
                        rangeIndex = 0;                                 // set the first item to be displayed as the first menu item
                    }
                    else if (rangeIndex < itemsPerMenu)
                    {
                        // (rangeIndex < itemsPerMenu) - menu selection is within the first few items
                        LOCAL_highlightCursor = rangeIndex;             // set the cursor to the selected value
                        ShowCursor = eTRUE;                             // allow the cursor to be shown
                        rangeIndex = 0;                                 // set the first item to be displayed as the first menu item
                    }
                    else
                    {
                        // (rangeIndex >= itemsPerMenu) - current value is beyond the end of display (if displaying the first few enum values)
                        LOCAL_highlightCursor = (itemsPerMenu - 1);     // set the cursor to the last line
                        ShowCursor = eTRUE;                             // allow the cursor to be shown
                        rangeIndex = (rangeIndex - itemsPerMenu) + 1;   // set the first item to be displayed so that the last item to be displayed is the selected value
                    }
                }
                else
                {
                    // update static area flag is not set - build a new display based on what was previously displayed
                    if (menuSelection < pDisplayedSelections[0])
                    {
                        // new value is less than previous lowest displayed value - build a new display with selected value at top
                        LOCAL_highlightCursor = 0;                      // set the cursor to the first line
                        ShowCursor = eTRUE;                             // allow the cursor to be shown
                        rangeIndex = rangeIndex;                        // do not change range index so that the first item to be displayed is the selected value
                    }
                    else
                    {
                        // new value must be more than previous highest displayed value - build a new display with selected value at bottom
                        LOCAL_highlightCursor = (itemsPerMenu - 1);     // set the cursor to the last line
                        ShowCursor = eTRUE;                             // allow the cursor to be shown
                        rangeIndex = (rangeIndex - itemsPerMenu) + 1;   // set the first item to be displayed so that the last item to be displayed is the selected value
                    }
                }
                // Fill the displayed values array
                for (DisplayedSelectionsIndex = 0; DisplayedSelectionsIndex < itemsPerMenu; DisplayedSelectionsIndex++)
                {
                    if (rangeIndex < numberOfItems)
                    {
                        pDisplayedSelections[DisplayedSelectionsIndex] = menuItemTab[rangeIndex];
                    }
                    rangeIndex ++;
                }
            }

            do
            {
                startPacket = LOCAL_packetStage;
                TUSIGN8 lineNumber = LOCAL_packetStage - MENU_FRAME_PACKET_MENU_ITEM_1;

                if ((pDisplayedSelections[lineNumber] < 0) || (lineNumber >= itemsPerMenu))
                {
                    LOCAL_packetStage ++;
                }
                else
                {        
                    pMenuItem = FrameTableGetMenuItem((TUSIGN16)(pDisplayedSelections[lineNumber] + firstItem));
                    LOCAL_stringtable_text_id.attribIdx = pMenuItem == NULL ? IDS_BLANK : pMenuItem->text_attribIdx;
                    LOCAL_stringtable_text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
                    LOCAL_stringtable_text_id.ssIdx = HMI_IDX;

                    pMenuText = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);

                        
                    TUSIGN8 invert = (TUSIGN8)(ShowCursor && (lineNumber == LOCAL_highlightCursor) ? STRING_INVERT : NO_INVERT);
                    
                    if(lineNumber == 0)
                    {
                        menuiteminvert_1 = invert;
                        
                        if(stringType == STRING_TYPE_LIBIDX)
                        {
                            menuItemLenth1 = 2;
                        }
                        else
                        {
                            for(menuItemLenth1=0; menuItemLenth1<28;menuItemLenth1++)
                            {
                                if(pMenuText[menuItemLenth1] == '\0')
                                {
                                    break;
                                }
                            }
                        }

                        menuItem1[0] = (stringType<<5)|menuItemLenth1;                        
                        memcpy(&menuItem1[1],pMenuText,menuItemLenth1);
                    }
                    
                    if(lineNumber == 1)
                    {
                        menuiteminvert_2 = invert;
                        if(stringType == STRING_TYPE_LIBIDX)
                        {
                            menuItemLenth2 = 2;
                        }
                        else
                        {
                            for(menuItemLenth2=0; menuItemLenth2<28;menuItemLenth2++)
                            {
                                if(pMenuText[menuItemLenth2] == '\0')
                                {
                                    break;
                                }
                            }
                        }
                        menuItem2[0] = (stringType<<5)|menuItemLenth2;                        
                        memcpy(&menuItem2[1],pMenuText,menuItemLenth2);
                        
                    }
                    
                    if(lineNumber == 2)
                    {
                        menuiteminvert_3 = invert;
                        if(stringType == STRING_TYPE_LIBIDX)
                        {
                            menuItemLenth3 = 2;
                        }
                        else
                        {
                            for(menuItemLenth3=0; menuItemLenth3<28;menuItemLenth3++)
                            {
                                if(pMenuText[menuItemLenth3] == '\0')
                                {
                                    break;
                                }
                            }                           
                        }
                        menuItem3[0] = (stringType<<5)|menuItemLenth3;                        
                        memcpy(&menuItem3[1],pMenuText,menuItemLenth3);
                        
                    }
                                       
                    if(lineNumber == 3)
                    {
                        menuiteminvert_4 = invert;
                        if(stringType == STRING_TYPE_LIBIDX)
                        {
                            menuItemLenth4= 2;
                        }
                        else
                        {
                            for(menuItemLenth4=0; menuItemLenth4<28;menuItemLenth4++)
                            {
                                if(pMenuText[menuItemLenth4] == '\0')
                                {
                                    break;
                                }
                            }
                        }
                        menuItem4[0] = (stringType<<5)|menuItemLenth4;                        
                        memcpy(&menuItem4[1],pMenuText,menuItemLenth4);
                    }
                      
                    LOCAL_packetStage++;
                }
            }
            while ((LOCAL_packetStage != startPacket) && (LOCAL_packetStage <= MENU_FRAME_PACKET_MENU_ITEM_6));
        }
       
        
        //---------------------------------------------//-----------------------------------------|
        // MENU_FRAME_PACKET_SLIDER_OR_SECURITY        // Slider bar or Security Icon             |
        //---------------------------------------------//-----------------------------------------|

        #ifdef HMI_SUPPORTS_SECURITY_ACCESS
        if (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 4)
        {
            sliderbar_num = SecurityIcon[0];
        }
        #endif
        else
        {
            TUSIGN8 high,actual;
            FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
            sliderbar_high = high;
            sliderbar_num  = actual; 
        }
        
        //---------------------------------------------//-----------------------------------------|
        // MENU_FRAME_PACKET_RIGHT_KEY                 // Right function key  "Select"            |
        //---------------------------------------------//-----------------------------------------|

        if (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 2)
        {
            rightKey = 'E';   //  exit
        }
        else
        {
            rightKey = 'S';  //  select
        }
 
    }while(0);
    
    if(packetStage==FRAME_STAGE_KEYPRESS)
    {              
        /*  ------ menu item 1 ------ */
        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+menuItemLenth1+2;
        LOCAL_packet_HL[2] = menuiteminvert_1;
        memcpy(&LOCAL_packet_HL[3],menuItem1,menuItemLenth1+1);
        if(AddPacketToBuffer(menuItemLenth1+4, 0))
        {                        
            isBuild += 1;
            packetStage = FRAME_STAGE_KEYPRESS_2;
            ismenuitemadd1 = 0x01;
        }
        else
        {
            ismenuitemadd1 = 0x00;
        }
    }
    
    if(packetStage==FRAME_STAGE_KEYPRESS_2)
    {  
        /*  ------ menu item 2 ------ */
        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+menuItemLenth2+2;
        LOCAL_packet_HL[2] = menuiteminvert_2;
        memcpy(&LOCAL_packet_HL[3],menuItem2,menuItemLenth2+1);
        if(AddPacketToBuffer(menuItemLenth2+4, 0))
        {                        
            isBuild += 1;
            if(testLanguage==11)
            {
                packetStage = FRAME_STAGE_KEYPRESS_5;
            }
            else
            {
                packetStage = FRAME_STAGE_KEYPRESS_3;
            }
            
            ismenuitemadd2 = 0x01;
        }
        else
        {
            ismenuitemadd2 = 0x00;
        }
    }
    
    if(packetStage==FRAME_STAGE_KEYPRESS_3)
    {  
        /*  ------ menu item 3 ------ */
        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+menuItemLenth3+2;
        LOCAL_packet_HL[2] = menuiteminvert_3;
        memcpy(&LOCAL_packet_HL[3],menuItem3,menuItemLenth3+1);
        if(AddPacketToBuffer(menuItemLenth3+4, 0))
        {                        
            isBuild += 1;
            packetStage = FRAME_STAGE_KEYPRESS_4;
            ismenuitemadd3 = 0x01;
        }
        else
        {
            ismenuitemadd3 = 0x00;
        }
    }
    
    if(packetStage==FRAME_STAGE_KEYPRESS_4)
    {  
        /*  ------ menu item 4 ------ */
        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+menuItemLenth4+2;
        LOCAL_packet_HL[2] = menuiteminvert_4;
        memcpy(&LOCAL_packet_HL[3],menuItem4,menuItemLenth4+1);
        if(AddPacketToBuffer(menuItemLenth4+4, 0))
        {                        
            isBuild += 1;
            packetStage = FRAME_STAGE_KEYPRESS_5;
            ismenuitemadd4 = 0x01;
        }
        else
        {
            ismenuitemadd4 = 0x00;
        }
    }
    
    if(packetStage==FRAME_STAGE_KEYPRESS_5)
    {
         /*  ------ other ------ */
        LOCAL_packet_HL[0] = FRAME_MENU_USER;
        LOCAL_packet_HL[1] = (packetStage<<5)+3;
        LOCAL_packet_HL[2] = sliderbar_num;
        LOCAL_packet_HL[3] = sliderbar_high;
        LOCAL_packet_HL[4] = rightKey;
        if(AddPacketToBuffer(5, 0))
        {                        
            isBuild += 1;
            packetStage = FRAME_STAGE_END;
            ismenuitemadd5 = 0x01;
        }
        else
        {
            ismenuitemadd5 = 0x00;
        }
    }
    
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            isBuild += 1;
        }        
    }
    
    return isBuild;
}

#endif

TUSIGN8 BuildSFrame_AccessLevelMenu(void)
{
    TUSIGN8  cursorSel;
    TUSIGN16 serviceSwitchState, loginState;
    TUSIGN8 buildNum = 0;
    TUSIGN8 actual, high;
    static TUSIGN16 s_preAccessState = 0xFFFF;
    static TUSIGN16 s_preServiceState = 0xFFFF;
    
#if (!TEST_FRAME_MENU_ACCESS_LEVEL)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    
    cursorSel = FrameTableGetCursorPosition();
    FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
    arm_Get((TUSIGN16)ARM_IDX_serviceAccountSwitch, (TINT16)WHOLE_OBJECT, &serviceSwitchState);
    arm_Get((TUSIGN16)ARM_IDX_hmiLoginState, (TINT16)WHOLE_OBJECT, &loginState);
    
    typedef enum
    {   
        FRAME_STAGE_STATIC = 0,   
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,   //max = 7
        FRAME_STAGE_NULL,
    }FRAME_STAGE;


    if ((testLanguage != preDisplayLang) 
      || LOCAL_updateStaticArea)
    {
        LOCAL_packetStage =  FRAME_STAGE_STATIC;  
        packetStage = FRAME_STAGE_STATIC;
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if((serviceSwitchState != s_preServiceState)
        || (loginState != s_preAccessState)
        || (cursorSel != preCursorSel))
        {
            packetStage =  FRAME_STAGE_OPERATION;            
            LOCAL_packetStage = FRAME_STAGE_OPERATION;
        }
        else
        {
            return 0;
        }
    }  

    if(packetStage == FRAME_STAGE_STATIC)
    {
        LOCAL_packet_HL[0] = FRAME_MENU_ACCESS_LEVEL;
        LOCAL_packet_HL[1] = (packetStage<<5) + 1;                     //length
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        if(AddPacketToBuffer(3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            buildNum += 1;
        }
    }  

    if(packetStage == FRAME_STAGE_OPERATION)
    {
        LOCAL_packet_HL[0] = FRAME_MENU_ACCESS_LEVEL;
        LOCAL_packet_HL[1] = (packetStage<<5) + 4;                     //length
        LOCAL_packet_HL[2] = cursorSel;
        LOCAL_packet_HL[3] = high;
        LOCAL_packet_HL[4] = serviceSwitchState;                   
        LOCAL_packet_HL[5] = loginState;
        if(AddPacketToBuffer(6, 0))
        {
            preCursorSel = cursorSel;
            s_preServiceState = serviceSwitchState;
            s_preAccessState = loginState;
            
            packetStage = FRAME_STAGE_END;
            buildNum += 1;
        }
    }
    

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            buildNum += 1;
        }        
    }

    return buildNum;
    
}

TUSIGN8 BuildSFrame_OperatorMenu(void)
{
    TUSIGN8  cursorSel;
    TUSIGN8 isScrollShown, isPage2Displayed, isPage3Displayed, isPage4Displayed;
    TUSIGN8 isBuild = 0;
    static TUSIGN8 s_preScrolState = 0xFF;
    static TUSIGN8 s_prePage2Mode = 0xFF;
    static TUSIGN8 s_prePage3Mode = 0xFF;
    static TUSIGN8 s_prePage4Mode = 0xFF;
    

#if (!TEST_FRAME_MENU_OPERATOR)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    
    
    cursorSel = FrameTableGetCursorPosition();
    hmi_Get((TUSIGN16)HMI_IDX_autoscrollEnable, (TINT16)WHOLE_OBJECT, &isScrollShown);
    hmi_Get((TUSIGN16)HMI_IDX_displayMode_2, (TINT16)WHOLE_OBJECT, &isPage2Displayed);
    hmi_Get((TUSIGN16)HMI_IDX_displayMode_3, (TINT16)WHOLE_OBJECT, &isPage3Displayed);
    hmi_Get((TUSIGN16)HMI_IDX_displayMode_4, (TINT16)WHOLE_OBJECT, &isPage4Displayed);

    typedef enum
    {   
        FRAME_STAGE_STATIC = 0,
        FRAME_STAGE_OPERATION,    
        FRAME_STAGE_END = 7,
        FRAME_STAGE_NULL
    }FRAME_STAGE;

    
    if((testLanguage != preDisplayLang) 
      || LOCAL_updateStaticArea)
    {
        packetStage = FRAME_STAGE_STATIC;
        LOCAL_packetStage =  FRAME_STAGE_STATIC;        
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if((cursorSel != preCursorSel)
        || (isScrollShown != s_preScrolState)
        || (isPage2Displayed != s_prePage2Mode)
        || (isPage3Displayed != s_prePage3Mode)
        || (isPage4Displayed != s_prePage4Mode))
        {
            packetStage =  FRAME_STAGE_OPERATION;            
            LOCAL_packetStage = FRAME_STAGE_OPERATION;
        }
        else
        {
            return eFALSE;
        }
    }  

    if(packetStage == FRAME_STAGE_STATIC)
    {
        LOCAL_packet_HL[0] = FRAME_MENU_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5) + 1;                     //length

        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;

        if(AddPacketToBuffer(3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild +=1;
        }
    }

    if(packetStage == FRAME_STAGE_OPERATION)
    {
        TUSIGN8 actual, high;
        
        FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
        
        LOCAL_packet_HL[0] = FRAME_MENU_OPERATOR;
        LOCAL_packet_HL[1] = (packetStage<<5) + 6;                     //length
        LOCAL_packet_HL[2] = actual;
        LOCAL_packet_HL[3] = high;
        LOCAL_packet_HL[4] = isScrollShown;                             //Number of variables
        LOCAL_packet_HL[5] = isPage2Displayed;  
        LOCAL_packet_HL[6] = isPage3Displayed;  
        LOCAL_packet_HL[7] = isPage4Displayed;  
        if(AddPacketToBuffer(8, 0))
        {
            preCursorSel = actual;
            s_preScrolState = isScrollShown;  
            s_prePage2Mode = isPage2Displayed;
            s_prePage3Mode = isPage3Displayed;
            s_prePage4Mode = isPage4Displayed;
            packetStage = FRAME_STAGE_END;
            isBuild +=1;
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1;
        }        
    }

    return isBuild;
    
}


TUSIGN8 BuildSFrame_SignalMenu(void)
{
    //OPERATOR_PAGE_LINE_DATA lineData[3];
    static TUSIGN8 s_preSigSel = 0xFF; 
    static TUSIGN8 s_prevDisplayItems[3] = {0xFF, 0xFF, 0xFF};
    static TUSIGN8 s_menuItemTab[MAX_SIGNALS] = {0};
    static TUSIGN8 s_maxMenuNum;
    static TUSIGN8 s_idxMenuSel;
    static TUSIGN8 s_prevDynamicData3[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //first byte = length
    TBOOL needRefreshMenus = eTRUE;
    TUSIGN8 isBuild = 0;
    TUSIGN8 firstItem, lastItem, cursorSel; 
    
    TUSIGN16 idxSig, i,j;       
    TUSIGN8* pData;
    TUSIGN8 totalLen, len;
  
    
    TUSIGN8 lineNum;
    //TUSIGN8 actual, high;

    
#if (!TEST_FRAME_MENU_SIGNAL)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
#else
        testLanguage = SFD_SIMPLE.language;
#endif
       
    
  
    typedef enum
    {  
        FRAME_STAGE_STATIC = 0,  //not changed part. language/menu numbers
        FRAME_STAGE_DYNAMIC_1,   //tag
        FRAME_STAGE_DYNAMIC_2,   //unit
        FRAME_STAGE_DYNAMIC_3,   //vars
        FRAME_STAGE_END = 7,
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    //FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
    cursorSel = FrameTableGetCursorPosition();     
    if ((testLanguage != preDisplayLang)
     ||  LOCAL_updateStaticArea) //update all
    {
        LOCAL_packetStage =  FRAME_STAGE_STATIC; 
        packetStage = FRAME_STAGE_STATIC;
        s_prevDynamicData3[0] = 0;
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if(s_preSigSel!=cursorSel)
        {
            LOCAL_packetStage =  FRAME_STAGE_DYNAMIC_1;
            packetStage = FRAME_STAGE_DYNAMIC_1;
        }
        else if(LOCAL_updateActiveArea)
        {  
            LOCAL_packetStage =  FRAME_STAGE_DYNAMIC_3;
            packetStage = FRAME_STAGE_DYNAMIC_3;            
            
            LOCAL_updateActiveArea = eFALSE;
        }
        else
        {
            return 0;
        }
    }

    if(packetStage==FRAME_STAGE_STATIC)
    {
        firstItem =   (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.firstItem_MSByte << 8) 
                    + (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.firstItem_LSByte;
    
        lastItem =    (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.lastItem_MSByte << 8) 
                    + (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.lastItem_LSByte;

        j = 0xFF;
        for(idxSig=firstItem, i=0; idxSig<=lastItem; idxSig++)
        {        
#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
            #ifdef VALIDITY_TEST_CALLER
            if(ValidityTestCheck((TUSIGN16)FrameTableGetSignal(idxSig)->SignalsViewTest, SIGNALS_TEST_FOR_SIGNALS_VIEW))
            #else
            if(ValidityTestCheck((TUSIGN16)FrameTableGetSignal(idxSig)->SignalsViewTest))
            #endif
#else
            if(FrameTableGetSignal(idxSig)->SignalsViewEnable)
#endif
            {
                s_menuItemTab[i] = idxSig;
                i++;
            }
        }
        s_maxMenuNum = i;

        LOCAL_packet_HL[0] = FRAME_MENU_SIGNAL;
        LOCAL_packet_HL[1] = (packetStage<<5) + 2;                //length

        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;

        LOCAL_packet_HL[3] = s_maxMenuNum;   //total signal number displayed
        if(AddPacketToBuffer(4, 0))
        {
            packetStage = FRAME_STAGE_DYNAMIC_1;
            isBuild += 1;
        }  
        
    }

    if((testLanguage==HMI_LANGUAGE_CHINESE))
    {
        lineNum = 2;
    }
    else
    {
        lineNum = 3;
    }

    if(packetStage==FRAME_STAGE_DYNAMIC_1)
    {        
        
        for(i=0; i<lineNum; i++)
        {
            if(cursorSel==s_prevDisplayItems[i])
            {
                needRefreshMenus = eFALSE;
                break;
            }
        }

        if(needRefreshMenus)
        {
            j=0;
            for(i=0; i<s_maxMenuNum; i++)
            {
                if(cursorSel==s_menuItemTab[i])
                {
                    s_idxMenuSel = i;
                    break;
                }
            }
            
            //record the signals to be displayed.
            s_prevDisplayItems[0] = 0xFF;
            s_prevDisplayItems[1] = 0xFF;
            s_prevDisplayItems[2] = 0xFF;
            i=s_idxMenuSel;
            j=0;
            do
            {
                s_prevDisplayItems[j] = s_menuItemTab[i];
                i++;
                j++;
                if(i>=s_maxMenuNum)
                {
                    break;
                }                
            }
            while(j<lineNum);  
        }

        LOCAL_packet_HL[0] = FRAME_MENU_SIGNAL;
        //LOCAL_packet_HL[1] = ?;                //length 
        LOCAL_packet_HL[2] = s_prevDisplayItems[0];      //current displayed signal
        LOCAL_packet_HL[3] = s_prevDisplayItems[1];      //current displayed signal
        LOCAL_packet_HL[4] = s_prevDisplayItems[2];      //current displayed signal
        LOCAL_packet_HL[5] = s_idxMenuSel;               //first signali menu index
        LOCAL_packet_HL[6] = cursorSel;
        totalLen = 5;
        pData = &LOCAL_packet_HL[7];      
        for(i=0; i<lineNum; i++)
        {            
            len = PackLineData_tag(pData, s_prevDisplayItems[i]);
            pData += len;
            totalLen += len;
        }        
        
        LOCAL_packet_HL[1] = (packetStage<<5) + totalLen;  
        if(AddPacketToBuffer(2+totalLen, 0))
        {
            s_preSigSel = cursorSel;
            packetStage = FRAME_STAGE_DYNAMIC_2;
            isBuild += 1;
        }
        
    }
    

    if(packetStage==FRAME_STAGE_DYNAMIC_2)
    {
        
        LOCAL_packet_HL[0] = FRAME_MENU_SIGNAL;
        //LOCAL_packet_HL[1] = ?;                //length        
        totalLen = 0;
        pData = &LOCAL_packet_HL[2];  

        for(i=0; i<lineNum; i++)
        {            
            len = PackLineData_unit(pData, s_prevDisplayItems[i]);
            pData += len;
            totalLen += len;
        } 
        
        LOCAL_packet_HL[1] = (packetStage<<5) + totalLen;  
        if(AddPacketToBuffer(2+totalLen, 0))
        {
            packetStage = FRAME_STAGE_DYNAMIC_3;
            isBuild += 1;
        }
    }
    

    if(packetStage==FRAME_STAGE_DYNAMIC_3)
    {
        LOCAL_packet_HL[0] = FRAME_MENU_SIGNAL;
        //LOCAL_packet_HL[1] = len;
        totalLen = 0;
        pData = &LOCAL_packet_HL[2];

        for(i=0; i<lineNum; i++)
        {
            len = PackLineData_value(pData, s_prevDisplayItems[i], NULL);
            pData += len;
            totalLen += len;
        } 

        if(totalLen>31)
        {
            totalLen = 31;
        }

        if(totalLen==0)
        {
            packetStage = FRAME_STAGE_END;
        }
        else if((totalLen!=s_prevDynamicData3[0])
         ||(!IsSamePacket(&LOCAL_packet_HL[2], &s_prevDynamicData3[1], totalLen)))
        {
            LOCAL_packet_HL[1] = (packetStage<<5) + totalLen; 
            if(AddPacketToBuffer(2+totalLen, 0))
            { 
                memcpy(&s_prevDynamicData3[1], &LOCAL_packet_HL[2], totalLen);
                s_prevDynamicData3[0] = totalLen;
                packetStage = FRAME_STAGE_END;
                isBuild += 1;
            }             
        }
        else
        {
            packetStage = FRAME_STAGE_END;
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;
    
}


#ifdef HMI_USES_COMMONDIAGNOSTICS
// No restriction on the optimization of this function

TUSIGN8 BuildSFrame_DiagnoseMenu3(void)
{
    static TUSIGN8 s_preAlarmID = 0xFF;
    static TUSIGN8 s_preMaxAlarmNum = 0xFF;
#ifdef COMMON_TOP_WORKS_NEW_FEATURES
    static const TUSIGN8* s_pFullRecoveryAct;
    static TUSIGN8 s_alarmActionLen = 0;
    static E_STRING_TYPE s_alarmActionCharacter = STRING_TYPE_EUROPEAN;
#endif 
    TUSIGN8 cursorSel, nActiveAlarms;      
    TUSIGN8 isBuild = 0;
    TUSIGN8* pData;
    TUSIGN16 i;
    
    
#if (!TEST_FRAME_MENU_DIAGNOSE3)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
#else
        testLanguage = SFD_SIMPLE.language;
#endif
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION_0,  //changed part, if user operated with up/down button pressed.
        FRAME_STAGE_OPERATION_1,  //diagnose description
        FRAME_STAGE_OPERATION_2,  //action-0, 31
        FRAME_STAGE_OPERATION_3,  //action-1, 17
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
    
    
    cursorSel = FrameTableGetCursorPosition();   
    nActiveAlarms = AdjusterGetNumberOfAlarmConditions(eTRUE);     //get the alarm list
    if(nActiveAlarms!= s_preMaxAlarmNum)
    {
        cursorSel = 0;
        FrameTableSetCursorPosition(cursorSel);
    }
    
    else if (cursorSel >= nActiveAlarms)
    {
        cursorSel = 0;
        FrameTableSetCursorPosition(cursorSel);
    }
   
    
    if ((testLanguage != preDisplayLang)
     ||  LOCAL_updateStaticArea) //update all
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LOCAL_packetStage = FRAME_STAGE_STATIC;       
        
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if((cursorSel!= preCursorSel)
            || (nActiveAlarms!=s_preMaxAlarmNum))
        {
            packetStage =  FRAME_STAGE_OPERATION_0;            
            LOCAL_packetStage = FRAME_STAGE_OPERATION_0;
        }
        else
        {
            return 0;
        }
    }

    if(packetStage==FRAME_STAGE_OPERATION_2)
    {
        packetStage = FRAME_STAGE_OPERATION_2;
    }

    if(packetStage==FRAME_STAGE_STATIC)
    {         
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE3;
        LOCAL_packet_HL[1] = (packetStage<<5) | 2;                
        
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = nActiveAlarms;
        
        if(AddPacketToBuffer(4, 0))
        {            
            s_preMaxAlarmNum = nActiveAlarms;
            if(nActiveAlarms>0)
            {
                packetStage = FRAME_STAGE_OPERATION_0;
            }
            else
            {
                packetStage = FRAME_STAGE_END;
            }            
            isBuild += 1;
        }          
    }

    if(packetStage==FRAME_STAGE_OPERATION_0)
    {   
        TBOOL   bAlarmActive = eFALSE;
        TUSIGN8 AlarmClassification = CLASSIFICATION_NONE;
        TUSIGN8 AlarmCategory = CATEGORY_UNSPECIFIED;
        TUSIGN8 AlarmPriority = 0;        
        TUSIGN8 NAMURClassification;
        TUSIGN8 NAMURCategory;
        
        bAlarmActive = AdjusterGetAlarmConditionDetails(cursorSel, &AlarmClassification, &AlarmCategory, &AlarmPriority, &s_preAlarmID);
        AdjusterGetNAMURStatus(&NAMURClassification, &NAMURCategory);
        
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE3;
        LOCAL_packet_HL[1] = (packetStage<<5) + 8;                //length
        LOCAL_packet_HL[2] = bAlarmActive;
        LOCAL_packet_HL[3] = cursorSel;   
        LOCAL_packet_HL[4] = AlarmClassification;
        LOCAL_packet_HL[5] = AlarmCategory;
        LOCAL_packet_HL[6] = AlarmPriority;
        LOCAL_packet_HL[7] = s_preAlarmID;
        LOCAL_packet_HL[8] = NAMURClassification;
        LOCAL_packet_HL[9] = NAMURCategory; 
        if(AddPacketToBuffer(10, 0))
        {
            preCursorSel = cursorSel;    
            packetStage = FRAME_STAGE_OPERATION_1;
            isBuild += 1;
        }
    }
    

    if(packetStage==FRAME_STAGE_OPERATION_1)
    {
        const TUSIGN8* pFullDescription;
        E_STRING_TYPE stringType;

        LOCAL_stringtable_text_id.attribIdx = FrameTableGetDiagnosticTableEntry(s_preAlarmID).descriptionID;
        pFullDescription = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);             
        
        if(stringType==STRING_TYPE_LIBIDX)
        {   
            LOCAL_packet_HL[3] = pFullDescription[0];
            LOCAL_packet_HL[4] = pFullDescription[1];
            i = 2;
        }
        else
            {
            pData = &LOCAL_packet_HL[3];  
            for(i=0; i<30; i++)
            {            
                if(pFullDescription[i]==0)
                {
                    break;
                }
                else
                {
                        pData[i] = pFullDescription[i];
                }
            }         
                    
        }       
        
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE3; 
        LOCAL_packet_HL[1] = (packetStage<<5) + 1 + i; 
        LOCAL_packet_HL[2] = (stringType<<5)|i;   
        if(AddPacketToBuffer(3+i, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild += 1;
        }
        
    }
    

    if(packetStage==FRAME_STAGE_OPERATION_2)
    {
            if((testLanguage==HMI_LANGUAGE_CHINESE))
            {
                LOCAL_packet_HL[3] = 0;
                i = 0;
                s_alarmActionLen = 0;
            }
            else
            {
                LOCAL_stringtable_text_id.attribIdx = FrameTableGetDiagnosticTableEntry(s_preAlarmID).RecoveryActID;
                s_pFullRecoveryAct = FrameTableGetStringSpecificLanguage_s(&s_alarmActionCharacter, LOCAL_stringtable_text_id, testLanguage); 
                if(s_alarmActionCharacter==STRING_TYPE_LIBIDX)
                {
                    LOCAL_packet_HL[4] = s_pFullRecoveryAct[0];
                    LOCAL_packet_HL[5] = s_pFullRecoveryAct[1];
                    i = 2;
                    s_alarmActionLen = 2;
                }
                else
                {
                    for(i=0; i<48; i++)
                    {            
                        if(s_pFullRecoveryAct[i]==0)
                        {
                            break;
                        }
                    } 
                    s_alarmActionLen = i;
            
                    if(s_alarmActionLen>29)
                    {
                        i = 29;
                    }

                    memcpy(&LOCAL_packet_HL[4], s_pFullRecoveryAct, i);
                }

                LOCAL_packet_HL[3] = (s_alarmActionCharacter<<5)|i;
                
            }
                
            LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE3;
            LOCAL_packet_HL[1] = (packetStage<<5) + (i+2);   //max size = 31
            LOCAL_packet_HL[2] = 0;    //reserved
            if(AddPacketToBuffer(4+i, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_3;
                isBuild += 1;
            }   
        
        }            

    if(packetStage==FRAME_STAGE_OPERATION_3)
    {
        //LOCAL_packet_HL[1] = ?;                       //length  
        if(s_alarmActionLen<=29)
        {
            i = 0;
        }
        else
        {       
            i = s_alarmActionLen-29;
            if(i>18)
            {
                i = 18;
            }
            memcpy(&LOCAL_packet_HL[3], &s_pFullRecoveryAct[29], i);
        }        
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE3;
        LOCAL_packet_HL[1] = (packetStage<<5) + i+1;   
        LOCAL_packet_HL[2] = (s_alarmActionCharacter<<5)|i;
        if(AddPacketToBuffer(3+i, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }   
        
    }


    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;
    
}

/*
5---alarm history
6---secure alarm history
*/
TUSIGN8 BuildSFrame_DiagnoseMenu5(TUSIGN8 specialMenuFlag)
{
    static TUSIGN8 s_preAlarmID = 0xFF;
    //static TUSIGN8 s_preMaxAlarmNum = 0xFF;    
    static TBOOL   s_historyEmpty = eTRUE;  
    static TUSIGN8 s_alarmActive[24];      //max display 24 alarms max, if more , the display number of other will not display correctly.
    //static TUSIGN8 s_displayNum;
    TUSIGN8  maxAlarmNum;      
    TUSIGN8 isBuild = 0;
    TUSIGN8* pData;
    TUSIGN16 i;
    
#if (!TEST_FRAME_MENU_DIAGNOSE5)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
#else
        testLanguage = SFD_SIMPLE.language;
#endif
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION_1,  //diagnose description
        FRAME_STAGE_OPERATION_2,  //statistics
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;



    if (LOCAL_updateStaticArea)
    {
        s_historyEmpty = eTRUE;
        memset(s_alarmActive, 0xFF, 24);
    }
   
    if (LOCAL_updateStaticArea
    ||  LOCAL_keypressProcessed
    ||  (testLanguage != preDisplayLang)) //update all
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LOCAL_packetStage = FRAME_STAGE_STATIC;        
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        /*
        if(LOCAL_updateActiveArea)
        {
            packetStage =  FRAME_STAGE_OPERATION_2;    //only statistics data are updated
        }
        else
        {
            return 0;
        }
        */
        return 0;
    }
    
    
    if(packetStage==FRAME_STAGE_STATIC)
    { 
        TUSIGN8 packetLen = 0;
        
        // Diagnostic History
        TINT16 firstAlarm = AdjusterFindFirstDiagHistoryEntry((TUSIGN8)(specialMenuFlag - 4));
        if (firstAlarm < 0)
        {
            s_historyEmpty = eTRUE;
            memset(s_alarmActive, 0xFF, 24);
            packetLen = 0;
        }
        else
        {
            TUSIGN8 AlarmClassification;      // Yes      | Yes       |
            TUSIGN8 AlarmCategory;            // Yes      | Yes       |
            TUSIGN8 AlarmPriority;            // Yes      | Yes       |
            //TUSIGN8 AlarmID;                // Yes      | Yes       |
            TUSIGN16 AlarmCount;              //          | Yes       |
            TUSIGN32 AlarmTime_msec;          //          | Yes       |
            TUSIGN16 AlarmTime_day;           //          | Yes       |
            TUSIGN32 AlarmMostRecent_msec;    //          | Yes       |
            TUSIGN16 AlarmMostRecent_day;     //          | Yes       |
            TUSIGN8 cursorSel;

            
            maxAlarmNum = AdjusterGetNumberOfAlarmConditions(eTRUE);     //get the alarm list  
            preCursorSel = FrameTableGetCursorPosition(); 
            
            if (s_historyEmpty)
            {// previously displaying empty history                
                
                s_alarmActive[0] = preCursorSel;
                cursorSel = 0;
                s_historyEmpty = eFALSE;
            }
            else
            {                
                
                for(i=0; i<24; i++)
                {
                    if(preCursorSel==s_alarmActive[i])
                    {                        
                        break;
                    }
                }
                
                if(i<24)
                {//already registered
                    cursorSel = i;
                }
                else
                {
                    for(i=0; i<24; i++)
                    {
                        if(0xFF==s_alarmActive[i])
                        {
                            break;
                        }
                    }
                    
                    if(i<24)
                    {
                        s_alarmActive[i] = preCursorSel;
                        cursorSel = i;
                    }
                    else
                    {
                        preCursorSel = s_alarmActive[23];
                        cursorSel = 23;
                    }
                    
                }
                
                            
            }
          
            (void)AdjusterGetAlarmHistoryDetails((TUSIGN8)(specialMenuFlag - 4),        // TUSIGN8 HistoryNumber
                                                     preCursorSel,                      // TUSIGN8 SelectedCondition
                                                     &AlarmClassification,   // TUSIGN8 *pAlarmClassification
                                                     &AlarmCategory,         // TUSIGN8 *pAlarmCategory
                                                     &AlarmPriority,         // TUSIGN8 *pAlarmPriority
                                                     &s_preAlarmID,          // TUSIGN8 *pAlarmID
                                                     &AlarmCount,            // TUSIGN16 *pAlarmCount
                                                     &AlarmTime_msec,        // TUSIGN32 *pAlarmTime_msec
                                                     &AlarmTime_day,         // TUSIGN16 *pAlarmTime_day
                                                     &AlarmMostRecent_msec,  // TUSIGN32 *pAlarmMostRecent_msec
                                                     &AlarmMostRecent_day);  // TUSIGN16 *pAlarmMostRecent_day           

            
            LOCAL_packet_HL[4] = maxAlarmNum;   
            LOCAL_packet_HL[5] = cursorSel;              
            LOCAL_packet_HL[6] = s_preAlarmID;
            LOCAL_packet_HL[7] = AlarmClassification;
            LOCAL_packet_HL[8] = AlarmCategory;
            LOCAL_packet_HL[9] = AlarmPriority;            
            packetLen = 6;            
            
        }
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE5;
        LOCAL_packet_HL[1] = (packetStage<<5) | packetLen+2;  
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = s_historyEmpty;
        if(AddPacketToBuffer(4+packetLen, eFALSE))
        {
            packetStage = FRAME_STAGE_OPERATION_1;
            isBuild += 1;
        }          
    }

       

    if(packetStage==FRAME_STAGE_OPERATION_1)
    {
        const TUSIGN8* pFullDescription;
        E_STRING_TYPE stringType;

        if((testLanguage!=HMI_LANGUAGE_CHINESE)
        && (!s_historyEmpty))
        {      
    
            LOCAL_stringtable_text_id.attribIdx = FrameTableGetDiagnosticTableEntry(s_preAlarmID).descriptionID;
            pFullDescription = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);             
            
            if(stringType==STRING_TYPE_LIBIDX)
            {
                LOCAL_packet_HL[3] = pFullDescription[0];
                LOCAL_packet_HL[4] = pFullDescription[1];
                i = 2;
            }
            else
            {
                pData = &LOCAL_packet_HL[3];  
                for(i=0; i<30; i++)
                {
                    if(pFullDescription[i]==0)
                    {
                        break;
                    }
                    else
                    {
                        pData[i] = pFullDescription[i];
                    }
                } 
                
            }
            LOCAL_packet_HL[2] = (stringType<<5)|i; 
            
        }
        else
        {         
            i = 0;
            LOCAL_packet_HL[2] = 0;
        }        

        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE5; 
        LOCAL_packet_HL[1] = (packetStage<<5) + 1 + i; 
        if(AddPacketToBuffer(3+i, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild += 1;
        }          
        
    }


    if(packetStage==FRAME_STAGE_OPERATION_2)
    {                   
        TUSIGN8 len;
                                                    //----------|-----------|
        TUSIGN8 AlarmClassification;      // Yes      | Yes       |
        TUSIGN8 AlarmCategory;            // Yes      | Yes       |
        TUSIGN8 AlarmPriority;            // Yes      | Yes       |
        TUSIGN8 AlarmID;                  // Yes      | Yes       |
        TUSIGN16 AlarmCount;              //          | Yes       |
        TUSIGN32 AlarmTime_msec;          //          | Yes       |
        TUSIGN16 AlarmTime_day;           //          | Yes       |
        TUSIGN32 AlarmMostRecent_msec;    //          | Yes       |
        TUSIGN16 AlarmMostRecent_day;     //          | Yes       |
                   
           
        if (s_historyEmpty)
        {
            len = 0;
        }
        else
        {           
            // Get data required for the alarm to be displayed
            (void)AdjusterGetAlarmHistoryDetails((TUSIGN8)(specialMenuFlag - 4),        // TUSIGN8 HistoryNumber
                                                 preCursorSel,                        // TUSIGN8 SelectedCondition
                                                 &AlarmClassification,   // TUSIGN8 *pAlarmClassification
                                                 &AlarmCategory,         // TUSIGN8 *pAlarmCategory
                                                 &AlarmPriority,         // TUSIGN8 *pAlarmPriority
                                                 &AlarmID,               // TUSIGN8 *pAlarmID
                                                 &AlarmCount,            // TUSIGN16 *pAlarmCount
                                                 &AlarmTime_msec,        // TUSIGN32 *pAlarmTime_msec
                                                 &AlarmTime_day,         // TUSIGN16 *pAlarmTime_day
                                                 &AlarmMostRecent_msec,  // TUSIGN32 *pAlarmMostRecent_msec
                                                 &AlarmMostRecent_day);  // TUSIGN16 *pAlarmMostRecent_day
            
            TUSIGN8 AlarmTime_sec = (TUSIGN8)((AlarmTime_msec / 1000) % 60);
            TUSIGN8 AlarmTime_min = (TUSIGN8)((AlarmTime_msec / 60000) % 60);
            TUSIGN8 AlarmTime_hour   = (TUSIGN8)(AlarmTime_msec / 3600000);   
            TUSIGN8 AlarmMostRecent_sec = (TUSIGN8)((AlarmMostRecent_msec / 1000) % 60);
            TUSIGN8 AlarmMostRecent_min = (TUSIGN8)((AlarmMostRecent_msec / 60000) % 60);
            TUSIGN8 AlarmMostRecent_hour   = (TUSIGN8)(AlarmMostRecent_msec / 3600000);

            LOCAL_packet_HL[2] = AlarmCount>>8;
            LOCAL_packet_HL[3] = AlarmCount;
            LOCAL_packet_HL[4] = AlarmTime_day>>8;
            LOCAL_packet_HL[5] = AlarmTime_day;
            LOCAL_packet_HL[6] = AlarmTime_hour;
            LOCAL_packet_HL[7] = AlarmTime_min;
            LOCAL_packet_HL[8] = AlarmTime_sec;
            LOCAL_packet_HL[9] = AlarmMostRecent_day>>8;
            LOCAL_packet_HL[10] = AlarmMostRecent_day;
            LOCAL_packet_HL[11] = AlarmMostRecent_hour;
            LOCAL_packet_HL[12] = AlarmMostRecent_min;
            LOCAL_packet_HL[13] = AlarmMostRecent_sec;
            len = 12;
        }       
        
        LOCAL_packet_HL[0] = FRAME_MENU_DIAGNOSE5;
        LOCAL_packet_HL[1] = (packetStage<<5)+len;        
        if(AddPacketToBuffer(len+2, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }

    }


    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;

}

// end of "#ifdef HMI_USES_COMMONDIAGNOSTICS"
#endif


#ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_NumericalView(void)
{
    static TUSIGN8 s_valueAndUnit[23] = {0};   //first byte is the ident
    
#if (!TEST_FRAME_NUMERICAL_VIEW)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
#else
        testLanguage = SFD_SIMPLE.language;
#endif

        typedef enum
	{                                               //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_TITLE_BAR,      // Title Bar "-------Device--------"       | - Start of Static Area
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_ID_2,           // Secondary Identifier                    |
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_TITLE,          // Parameter title                         |
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_LEFT_KEY,       // left function key  "Back"/"Next"        |
                                                        //-----------------------------------------|
    #ifndef DISABLE_BACK_MOVE_SELECT
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_BMS_ICON_6,     // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_BMS_ICON_7,     // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                        //-----------------------------------------|
    #endif
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_VALUE,          // Parameter value & units                 | - Start of active area
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_SLIDER_BAR,     // Slider bar                              |
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_RIGHT_KEY,      // right function key "Edit"               |
                                                        //-----------------------------------------|
            NUMERICAL_VIEW_FRAME_PACKET_READ_SWITCHES   //                                         |
                                                        //-----------------------------------------| - There is no keypress area
	}NUMERICAL_VIEW_FRAME_PACKET;
  
    
    TUSIGN8 isBuild = 0;
    //TUSIGN8 cursorSel = FrameTableGetCursorPosition();
   
	/*
	
	Define differnt stage of frame, update date from static or dymatic.
	
	you can find out in the original code which elememt is staic or dymatic.
	
	*/
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_STATIC_2,
        FRAME_STAGE_OPERATION = 2,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
	
	/*
	
	Code from here decided which stage to use. You can find in the original code .
	
	add 'packetStage' to adapt out stage.
	
	*/

    #ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER
    TUSIGN8 SecondaryIdentifierText[SECONDARY_IDENTIFIER_LEN];
    TBOOL SecondaryIdentifierActive = FrameTableGetSecondaryIdentifier(SecondaryIdentifierText);
    #endif

    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
    TBOOL   IntegerFrame = eFALSE;
    if ((LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_INTEGER_PARAM) || (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_INTEGER_PARAM_NO_EDIT))
    {
        IntegerFrame = eTRUE;
    }
    #endif
    if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
    {
        packetStage =  FRAME_STAGE_STATIC;  			/*  If system language changed, we have to chenge the whole frame */
        LOCAL_updateStaticArea = eFALSE;
        s_valueAndUnit[0] = 0xFF;
    }
    else if(LOCAL_updateActiveArea)
    {
        packetStage =  FRAME_STAGE_OPERATION; 
        LOCAL_updateActiveArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        return eFALSE;
    }

    if(packetStage==FRAME_STAGE_STATIC)
    {        
        TUSIGN8 titleLen = 0;
        E_STRING_TYPE stringType;
        TBOOL hasUpdownButton;

        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.titleBar_attribidx;
        
        TUSIGN8 const *pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            titleLen = 2;
        }
        else
        {
            for(titleLen=0; titleLen<MAX_LENGTH_FRAME_TITLE; titleLen++)
            {
                if(pTitle[titleLen] == '\0')
                {
                    break;
                }
            }
        }
        LOCAL_packet_HL[4] = (stringType<<5)|titleLen;

        if (!FrameTableIsBackMoveSelectAvailable(LOCAL_pCurrDisplayFrame))
        {
            hasUpdownButton = eFALSE;
        }
        else
        {
            hasUpdownButton = eTRUE;
        }
        
        
        LOCAL_packet_HL[0] = FRAME_NUMERICAL_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + titleLen + 3;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = hasUpdownButton;
        memcpy(&LOCAL_packet_HL[5], pTitle, titleLen);
        if(AddPacketToBuffer(titleLen+5, 0))
        {
            packetStage = FRAME_STAGE_STATIC_2;
            isBuild += 1;
        }
    }

    if(packetStage==FRAME_STAGE_STATIC_2)
    {
        TUSIGN8 stringLen = 0;
        E_STRING_TYPE stringType;
        TUSIGN8 leftButton;
        
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.title_attribidx;
        
        
        TUSIGN8 const *pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<MAX_LENGTH_PARAM_NAME; stringLen++)
            {
                if(pString[stringLen] == '\0')
                {
                    break;
                }
            }
        }

		
        #ifdef HMI_SUPPORTS_SEQUENCES
        if (FrameTableIsSequenceEnabled())
        {
                LOCAL_stringtable_text_id.attribIdx = IDS_NEXT;
                leftButton = 'N';    
        }
        else
        #endif
        {
                #ifdef HMI_SUPPORTS_EASYSETUP
                if (LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.easySetupFlag != 0)
                {
                    LOCAL_stringtable_text_id.attribIdx = IDS_NEXT;
                    leftButton = 'N'; 
                }
                else
                #endif
                {
                    LOCAL_stringtable_text_id.attribIdx = IDS_BACK;
                    leftButton = 'B'; 
                }
        }

        LOCAL_packet_HL[0] = FRAME_NUMERICAL_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + 2 + stringLen; 
        LOCAL_packet_HL[2] = leftButton;
        LOCAL_packet_HL[3] = (stringType<<5)|stringLen;
        memcpy(&LOCAL_packet_HL[4], pString, stringLen);
        if(AddPacketToBuffer(stringLen+4, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild += 1;
        } 	
    }

    if(packetStage==FRAME_STAGE_OPERATION)
    {            
        
        E_STRING_TYPE unitStrType;
        TUSIGN8 ValueLength  = 0;
        TUSIGN8 unitLen = 0;
        TUSIGN8 SliderBarNum = 0;
        TUSIGN8 sliderbar_high = 0;
        TUSIGN8 RightButton = 0;
		
        ADJUST_DATA tVal, tHigh, tLow, tInc;
        TUSIGN8 tType;
        TUSIGN8 units[18] = "";
        TUSIGN8 value_and_units[FONT1_MAX_CHARS+1] = "";
        TUSIGN8 *pNumber;
        TUSIGN8 negNumber;
        TUSIGN8 within_over_under;

        ATTRIB_IDENTIFIER_TYPE valueID;
//        T_RANGE_E8 enumRange;
        TBOOL rulesAreActive;

        #ifdef HMI_SUPPORTS_INTEGER_FRAMES
        TUSIGN32 Value32 = 0;
        #endif

        TUSIGN16 GetValueResult;

        valueID = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value;

        #ifdef HMI_SUPPORTS_INTEGER_FRAMES
        if (IntegerFrame)
        {
            // Get Integer Value
            T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)valueID.ssIdx);
            GetValueResult = pSub->Get(pSub,(TUSIGN16)valueID.objIdx,(TINT16)valueID.attribIdx,(void FAST *)&Value32);
        }
        else
        #endif
        {
            GetValueResult = AdjusterGetFloatValueAndLimits((TUSIGN16)valueID.ssIdx,
                                                            (TUSIGN16)valueID.objIdx,
                                                            valueID.attribIdx,
                                                            &tVal,
                                                            &tHigh,
                                                            &tLow,
                                                            &tInc,
                                                            &rulesAreActive,
                                                            &tType);
        }

        if (GetValueResult == (TUSIGN16)OK)
        {
            (void)strncpy((char *)units,
                          (const char *)PacketManagerGetEngUnits_s((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.ssIdx,
                                                                 LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.objIdx,
                                                                 LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.attribIdx,
                                                                 LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnitsType,
                                                                 &unitStrType),
                          8
                         );

            #ifdef HMI_SUPPORTS_CUSTOM_UNITS
            PacketManagerRemoveCustomfromUnits(units);
            #endif

            LimitTextLength(units,8);

            #ifdef HMI_SUPPORTS_INTEGER_FRAMES
            if (IntegerFrame)
            {
                pNumber = AdjusterBuildNumberStringFromUSIGN32(Value32,LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes,&within_over_under);
                negNumber = 0;
            }
            else
            #endif
            {
                #ifdef HMI_USES_DOUBLE
                if (tType == ST_DOUBLE)
                {
                    pNumber = AdjusterBuildNumberStringFromDouble(tVal.data_D,
                                                                  LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                  LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                  &negNumber,
                                                                  &within_over_under);
                }
                else
                #endif
                {
                    pNumber = AdjusterBuildNumberString(tVal.data_F,
                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                        &negNumber,
                                                        &within_over_under);
                }
            }

            if ((within_over_under == 1) || (within_over_under == 2))
            {
                // overrange or underrange
                TUSIGN8 i;
                value_and_units[0] = 0x20;
                LOCAL_overrangeValue_EditNotAllowed = eTRUE;

                for (i = 1; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; )
                {
                    value_and_units[i] = 0x05 + within_over_under;
                    i++;
                    value_and_units[i] = 0x00;
                }
            }
            else
            {
                LOCAL_overrangeValue_EditNotAllowed = eFALSE;

                // value within range or a normal numerical frame with an edit
                while (*pNumber == 0x20)
                {
                    #pragma diag_suppress=Pm088
                    pNumber ++;
                    #pragma diag_default=Pm088
                }

                if (negNumber)
                {
                    value_and_units[0] = '-';
                    value_and_units[1] = 0x00;
                }
                (void)strcat((char *)value_and_units,(char *)pNumber);
            }
            if(unitStrType!=STRING_TYPE_LIBIDX)
            {                
                (void)strcat((char *)value_and_units," ");
                (void)strcat((char *)value_and_units,(char *)units);
            }
        }
        else
        {
            value_and_units[0] = 0x00;
        }
		
        TUSIGN8 *pValue = value_and_units;
        for(ValueLength=0;ValueLength<20;ValueLength++)
        {
            if(pValue[ValueLength] == '\0')
            {
                    break;
            }
        }

        for(unitLen=0;unitLen<5;unitLen++)
        {
            if(units[unitLen] == '\0')
            {
                break;
            }
        }
       
        #ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.easySetupFlag != 0)
        {
            TUSIGN8 lastStage, currentStage;
            FrameTableGetEasySetupStages(&lastStage, &currentStage);
            sliderbar_high = lastStage;
            SliderBarNum = currentStage; 
        }
        else
        #endif
        {
            TUSIGN8 high,actual;

            FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
            sliderbar_high = high;
            SliderBarNum = actual; 
        }

        valueID = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value;

        if ((WriteProtect_HMICanAccess(valueID, (TUSIGN8)0)) && (LOCAL_pCurrDisplayFrame->frameType != FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT))
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_EDIT;
            RightButton = 'E';     //Edit
        }
        else
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_BLANK;
            RightButton = 'B';     //Blank
        }

        if((ValueLength==s_valueAndUnit[0]) 
        && (IsSamePacket(&s_valueAndUnit[1], (TUSIGN8*)pValue, ValueLength)))
        {//if the same packet, not send again
            packetStage = FRAME_STAGE_END;
        }
        else
        {
            TUSIGN8* pData = NULL;
            
            LOCAL_packet_HL[0] = FRAME_NUMERICAL_VIEW;
            LOCAL_packet_HL[1] = (packetStage<<5) +ValueLength +unitLen + 5;                //length
            LOCAL_packet_HL[2] = RightButton;
            LOCAL_packet_HL[3] = SliderBarNum;
            LOCAL_packet_HL[4] = sliderbar_high;
            LOCAL_packet_HL[5] = ValueLength;
            memcpy(&LOCAL_packet_HL[6],pValue,ValueLength);

            pData = &LOCAL_packet_HL[6];
            pData += ValueLength;
            *pData++ = (unitStrType<<5)+unitLen;
            memcpy(pData, units, unitLen);
            
            if(AddPacketToBuffer(ValueLength+unitLen+7, 0))
            {
                s_valueAndUnit[0] = ValueLength;
                memcpy(&s_valueAndUnit[1], pValue, ValueLength);
                packetStage = FRAME_STAGE_END;
                isBuild += 1;
            }
        }        
        
    }
    

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;
    
}


// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN8 BuildSFrame_NumericalEdit(void)
{

#if (!TEST_FRAME_NUMERICAL_EDIT)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE) )
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
#else
        testLanguage = SFD_SIMPLE.language;
#endif

    typedef enum
    {                                               // Packet                                  |
                                                                                               //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_TITLE_BAR,      // Parameter title                         | - Start of Static Area
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_ICON,           // Title Bar Icon                          |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_ID_2,           // Secondary Identifier                    |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_LEFT_KEY,       // left function key  "Next"               |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_UNITS,          // Engineering Units                       | - Start of active area
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_MAX,            // High Limit                              | - Start of keypress area
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_MIN,            // Low Limit                               |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_RIGHT_KEY,      // right function key "OK" / "Cancel"      |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_MAX_ICON,       // High Limit icon or text                 |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_MIN_ICON,       // Low Limit icon or text                  |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_VALUE,          // Parameter value & units                 |
                                                                                                //-----------------------------------------|
        NUMERICAL_EDIT_FRAME_PACKET_READ_SWITCHES   //                                         |
                                                                                                    //-----------------------------------------|
    }NUMERICAL_EDIT_FRAME_PACKET;

    TUSIGN8 isBuild = 0;
    
        //TUSIGN8 packetsBuilt = 0;
    TUSIGN8 text[22];

    ADJUST_DATA tVal, tHigh, tLow, tInc;
    TUSIGN8 tType = ST_NIL;

    TBOOL rulesAreActive;
    TUSIGN8 LimitError = 0;
    TUSIGN16 errorCode;
    
    TUSIGN8 EngUnitsLength=0;
    E_STRING_TYPE unitStrType;
    
    typedef enum
    {
        FRAME_STAGE_STATIC    = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_KEYPRESS,
        FRAME_STAGE_END       = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
	
    tHigh.data_F = 0.0F;
    tLow.data_F = 0.0F;
    #ifdef HMI_USES_DOUBLE
    tHigh.data_D = 0.0;
    tLow.data_D = 0.0;
    #endif
	
#define FINISHED
	#ifdef FINISHED
	
    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
    TBOOL   IntegerFrame = eFALSE;
    if ((LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_INTEGER_PARAM) || (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_INTEGER_PARAM_NO_EDIT))
    {
        IntegerFrame = eTRUE;
    }
    #endif


    if (LOCAL_updateStaticArea || LOCAL_keypressProcessed)
    {
        packetStage = FRAME_STAGE_STATIC;
        LOCAL_updateStaticArea = eFALSE;
    }
    /*
    else if (LOCAL_updateActiveArea)
    {
        packetStage  = FRAME_STAGE_STATIC;
        LOCAL_updateActiveArea = eFALSE;
    }
    */
    else
    {
        asm("nop");
    }

    if(packetStage==FRAME_STAGE_STATIC)
    {        
        TUSIGN8 stringLen;
        E_STRING_TYPE stringType;

        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.title_attribidx;
        
        
        TUSIGN8 const* pString =  FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<MAX_LENGTH_PARAM_NAME; stringLen++)
            {
                if(pString[stringLen] == '\0')  
                {
                    break;
                }
            }
        }
        
 
        LOCAL_packet_HL[3] = (stringType<<5)| stringLen;
        LOCAL_packet_HL[0] = FRAME_NUMERICAL_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) + stringLen + 2;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        memcpy(&LOCAL_packet_HL[4], pString, stringLen);

        if(AddPacketToBuffer(stringLen+4, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild += 1;
        }   
    }

    if(packetStage==FRAME_STAGE_OPERATION)
    {   
  
        strcpy((char *)text,
               (const char *)PacketManagerGetEngUnits_s((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.ssIdx,
                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.objIdx,
                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnits.attribIdx,
                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.engUnitsType,
                                                        &unitStrType)
              );

        LimitTextLength(text,MAX_LENGTH_UNIT);

        for(EngUnitsLength=0;EngUnitsLength<MAX_LENGTH_UNIT;EngUnitsLength++)
        {
            if(text[EngUnitsLength] == '\0')
            {
                break;
            }
        }

        LOCAL_packet_HL[0] = FRAME_NUMERICAL_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) + 1 + EngUnitsLength;                //length        
        LOCAL_packet_HL[2] = (unitStrType<<5)+EngUnitsLength;
        memcpy(&LOCAL_packet_HL[3], text, EngUnitsLength);

        if(AddPacketToBuffer(EngUnitsLength+3, 0))
        {
            packetStage = FRAME_STAGE_KEYPRESS;
            isBuild += 1;
        }
    }
	
    if(packetStage==FRAME_STAGE_KEYPRESS)
    {
        TUSIGN8 Right_button;

        TUSIGN8 *HighLimited;
        TUSIGN8 HighLimitedLength = 0;

        TUSIGN8 *LowLimited;
        TUSIGN8 LowLimitTextLength = 0;

        TUSIGN8 *Value;
        TUSIGN8 ValueLength = 0;
		
        // Prepare data for stages NUMERICAL_EDIT_FRAME_PACKET_MAX..NUMERICAL_EDIT_FRAME_PACKET_MIN
        //---------------------------------------------//-----------------------------------------|
        // NUMERICAL_EDIT_FRAME_PACKET_MAX             // High Limit                              |
        // NUMERICAL_EDIT_FRAME_PACKET_MIN             // Low Limit                               |
        //---------------------------------------------//-----------------------------------------|
        
        TUSIGN8 Stage = 0;

        
        {
            Stage = 0;
            #ifdef HMI_SUPPORTS_INTEGER_FRAMES
            if (IntegerFrame)
            {
                    T_DO_ATTRIB_RANGE Range;

                    T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.ssIdx);
                    (void)pSub->GetAttributeDescription(pSub,(TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.objIdx,(TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range);
                    tHigh.data_32 = Range.range.u32.max;
                    tLow.data_32  = Range.range.u32.min;
                    rulesAreActive = Range.rulesAreActive;
            }
            else
            #endif
            {
                    (void)AdjusterGetFloatValueAndLimits((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.ssIdx,
                                                                                             (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.objIdx,
                                                                                             LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value.attribIdx,
                                                                                             &tVal,
                                                                                             &tHigh,
                                                                                             &tLow,
                                                                                             &tInc,
                                                                                             &rulesAreActive,
                                                                                             &tType);
            }
            LimitError = AdjusterGetLimitError(&errorCode);
            
        }
        
        //---------------------------------------------//-----------------------------------------|
        // NUMERICAL_EDIT_FRAME_PACKET_MAX             // High Limit                              |
        //---------------------------------------------//-----------------------------------------|

        
        {
            Stage = 1;
            TUSIGN8 *pNumber;
            TUSIGN8 negNumber = 0;
            TUSIGN8 within_over_under = 0;
            TUSIGN8 i;

            text[0] = ' ';
            text[1] = 0x00;

            if (rulesAreActive)
            {
                #ifdef HMI_SUPPORTS_INTEGER_FRAMES
                if (IntegerFrame)
                {
                    pNumber = AdjusterBuildNumberStringFromUSIGN32(tHigh.data_32,LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes,&within_over_under);
                    negNumber = 0;
                }
                else
                #endif
                {
                        #ifdef HMI_USES_DOUBLE
                        if (tType == ST_DOUBLE)
                        {
                            pNumber = AdjusterBuildNumberStringFromDouble(tHigh.data_D,
                                                                          LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                          LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                          &negNumber,
                                                                          &within_over_under);
                        }
                        else
                        #endif
                        {
                            pNumber = AdjusterBuildNumberString(tHigh.data_F,
                                                                LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                &negNumber,
                                                                &within_over_under);
                        }
                }

                switch (within_over_under)
                {
                case 1: // overrange
                        for (i = 0; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                        {
                                (void)strcat((char *)text,"\x06");
                        }
                        break;

                case 2: // underrange
                        for (i = 0; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                        {
                                (void)strcat((char *)text,"\x07");
                        }
                        break;

                default:
                        text[0] = negNumber? '-' : ' ';
                        text[1] = 0x00;
                        while (*pNumber == ' ')
                        {
                                #pragma diag_suppress=Pm088
                                pNumber ++;
                                #pragma diag_default=Pm088
                        }
                        (void)strcat((char *)text,(char *)pNumber);
                        break;
                }
            }
            else
            {
                for (i = 0; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                {
                        (void)strcat((char *)text,"9");
                }
            }
            HighLimited = text;
            
            for(HighLimitedLength=0;HighLimitedLength<MAX_LENGTH_PARAM_NAME;HighLimitedLength++)
            {
                if(HighLimited[HighLimitedLength]=='\0')
                {
                    break;
                }
            }
                
            LOCAL_packet_HL[0] = FRAME_NUMERICAL_EDIT;        
            LOCAL_packet_HL[1] = ((packetStage+Stage)<<5) + HighLimitedLength + 1;
            LOCAL_packet_HL[2] = HighLimitedLength;
            memcpy(&LOCAL_packet_HL[3],HighLimited,HighLimitedLength);
            if(AddPacketToBuffer(HighLimitedLength+3, 0))
            {
                isBuild += 1;
            }
            
            
        }
        //---------------------------------------------//-----------------------------------------|
        // NUMERICAL_EDIT_FRAME_PACKET_MIN             // Low Limit                               |
        //---------------------------------------------//-----------------------------------------|
        
        
        {                       		
            /* ------------------------------------------------Get Low Limit Value---------------------------------------*/
            Stage = 2;
            TUSIGN8 *pNumber;
            TUSIGN8 negNumber = 0;
            TUSIGN8 within_over_under = 0;
            TUSIGN8 i;

            text[0] = ' ';
            text[1] = 0x00;

             if(rulesAreActive)
             {
                    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
                    if (IntegerFrame)
                    {
                            pNumber = AdjusterBuildNumberStringFromUSIGN32(tLow.data_32,LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes,&within_over_under);
                            negNumber = 0;
                    }
                    else
                    #endif
                    {
                            #ifdef HMI_USES_DOUBLE
                            if (tType == ST_DOUBLE)
                            {
                                    pNumber = AdjusterBuildNumberStringFromDouble(tLow.data_D,
                                                                                  LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                                  LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                                  &negNumber,
                                                                                  &within_over_under);
                            }
                            else
                            #endif
                            {
                                    pNumber = AdjusterBuildNumberString(tLow.data_F,
                                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                        LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                        &negNumber,
                                                                        &within_over_under);
                            }
                    }
                    switch (within_over_under)
                    {
                    case 1: // overrange
                            for (i = 0; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                            {
                                    (void)strcat((char *)text,"\x06");
                            }
                            break;

                    case 2: // underrange
                            for (i = 0; i <= LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                            {
                                    (void)strcat((char *)text,"\x07");
                            }
                            break;

                    default:
                            text[0] = negNumber? '-' : ' ';
                            text[1] = 0x00;
                            while (*pNumber == ' ')
                            {
                                    #pragma diag_suppress=Pm088
                                    pNumber ++;
                                    #pragma diag_default=Pm088
                            }
                            (void)strcat((char *)text,(char *)pNumber);
                            break;
                    }
            }

            else
            {
                    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
                    if ((tType == ST_U8) || (tType == ST_U16) || (tType == ST_U32) || (tType == ST_CHAR) || (tType == ST_WIDECHAR) || (IntegerFrame))
                    #else
                    if ((tType == ST_U8) || (tType == ST_U16) || (tType == ST_U32) || (tType == ST_CHAR) || (tType == ST_WIDECHAR))
                    #endif
                    {
                            strcat((char *)text,"  0");
                    }
                    else
                    {
                            text[0] = negNumber? '-' : ' ';
                            text[1] = 0x00;

                            for (i = 0; i < LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters; i ++)
                            {
                                    (void)strcat((char *)text,"9");
                            }
                    }
            }
            
            LowLimited  = text;
            
            for(LowLimitTextLength=0;LowLimitTextLength<MAX_LENGTH_PARAM_NAME;LowLimitTextLength++)
            {
                if(LowLimited[LowLimitTextLength] == '\0')
                {
                        break;
                }
            }
            
            LOCAL_packet_HL[0] = FRAME_NUMERICAL_EDIT;        
            LOCAL_packet_HL[1] = ((packetStage+Stage)<<5) + LowLimitTextLength + 1;
            LOCAL_packet_HL[2] = LowLimitTextLength;
            memcpy(&LOCAL_packet_HL[3],LowLimited,LowLimitTextLength);
            if(AddPacketToBuffer(LowLimitTextLength+3, 0))
            {
                isBuild += 1;
            }
            
           
        }
        
        //---------------------------------------------//-----------------------------------------|
        // NUMERICAL_EDIT_FRAME_PACKET_RIGHT_KEY       // right function key "OK" / "Cancel"      |
        //---------------------------------------------//-----------------------------------------|
        
        
        {                      			
            Stage = 3;
            TUSIGN8 negValue;
            TUSIGN8 *pValue;	

            ATTRIB_IDENTIFIER_TYPE valueID = LOCAL_pCurrDisplayFrame->frameDefinition.numericalParamFrame.value;
            TBOOL cursorAtLimit = FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit() ? eTRUE : eFALSE;

            if ((WriteProtect_HMICanAccess(valueID, (TUSIGN8)0)) && (cursorAtLimit == eFALSE))
            {
                Right_button = 'O';  //OK
            }
            else
            {
                Right_button = 'C';   //cancel
            }

            // get parameter value from Adjuster
            pValue = AdjusterGetVariableAsString(&negValue);

            text[0] = negValue ? '-' : ' ';
            text[1] = 0x00;
            (void)strcat((char *)text,(char*)pValue);

            if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
            {
                Value = text;
            }
            else
            {
                Value = text;
            }

            for(ValueLength=0;ValueLength<20;ValueLength++)
            {
                if(Value[ValueLength] == '\0')
                {
                    break;
                }
            }

            LOCAL_packet_HL[0] = FRAME_NUMERICAL_EDIT;        
            LOCAL_packet_HL[1] = ((packetStage+Stage)<<5) + ValueLength + 5;
            LOCAL_packet_HL[2] = Right_button;
            LOCAL_packet_HL[3] = FrameTableGetCursorPosition();
            LOCAL_packet_HL[4] = (TUSIGN8)((LimitError & 0x02) != 0 ? STRING_INVERT:NO_INVERT);
            LOCAL_packet_HL[5] = (TUSIGN8)((LimitError & 0x01) != 0 ? STRING_INVERT:NO_INVERT);
            LOCAL_packet_HL[6] = ValueLength;
            memcpy(&LOCAL_packet_HL[7],Value,ValueLength);
            if(AddPacketToBuffer(ValueLength+7, 0))
            {
                packetStage = FRAME_STAGE_END;
                isBuild += 1;
            }
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    #endif
    return isBuild;
}

// end of "#if HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES"
#endif


#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_TimeDateView(void)
{
    TUSIGN8 isBuild = 0;
    TUSIGN8 cursorSel;
    
#if (!TEST_FRAME_TIMEDATE_VIEW)
            if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
            {
                testLanguage = HMI_LANGUAGE_ENGLISH;
            }
            else
            {
                testLanguage = SFD_SIMPLE.language;
            }    
#else
            testLanguage = SFD_SIMPLE.language;
#endif

    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    cursorSel = FrameTableGetCursorPosition();  
    if ((testLanguage != preDisplayLang)   
     ||  LOCAL_updateStaticArea) //update all
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LOCAL_packetStage = FRAME_STAGE_STATIC;
        
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if(LOCAL_updateActiveArea
            || (cursorSel != preCursorSel))
        {
            packetStage =  FRAME_STAGE_OPERATION;            
            LOCAL_packetStage = FRAME_STAGE_OPERATION;
            
            LOCAL_updateActiveArea = eFALSE;
        }
        else
        {
            return 0;
        }
    }
    
    
    if(packetStage==FRAME_STAGE_STATIC)
    {         
        LOCAL_packet_HL[0] = FRAME_TIMEDATE_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) | 1;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
     
        if(AddPacketToBuffer(3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild += 1;
        }          
    }

    if(packetStage==FRAME_STAGE_OPERATION)
    {                   
        LOCAL_packet_HL[0] = FRAME_TIMEDATE_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + 1;                //length
        LOCAL_packet_HL[2] = cursorSel;
        if(AddPacketToBuffer(3, 0))
        {            
            preCursorSel = cursorSel;
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;

}

// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_TimeDateEdit(void)
{
    TUSIGN8 isBuild = 0;
    TUSIGN8 cursorSel;
    
#if (!TEST_FRAME_TIMEDATE_EDIT)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    cursorSel = FrameTableGetCursorPosition();  
    if ((testLanguage != preDisplayLang)
     ||  LOCAL_updateStaticArea) //update all
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LOCAL_packetStage = FRAME_STAGE_STATIC;
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        if(LOCAL_updateActiveArea
            || (cursorSel != preCursorSel))
        {
            packetStage =  FRAME_STAGE_OPERATION;            
            LOCAL_packetStage = FRAME_STAGE_OPERATION;
            
            LOCAL_updateActiveArea = eFALSE;
        }
        else
        {
            return 0;
        }
    }
    
    
    if(packetStage==FRAME_STAGE_STATIC)
    {         
        LOCAL_packet_HL[0] = FRAME_TIMEDATE_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) | 1;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        
        if(AddPacketToBuffer(3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild += 1;
        }          
    }

    if(packetStage==FRAME_STAGE_OPERATION)
    {                   
        LOCAL_packet_HL[0] = FRAME_TIMEDATE_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) + 1;                //length
        LOCAL_packet_HL[2] = cursorSel;
        if(AddPacketToBuffer(3, 0))
        {
            
            preCursorSel = cursorSel;
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;

}
#endif


#if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_StringView(void)
{
#if (!TEST_FRAME_STRING_VIEW)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif

    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_TITLE_BAR,         // Title Bar "-------Device--------"       | - Start of Static Area
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_ID_2,              // Secondary Identifier                    |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_TITLE,             // Parameter title                         |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_LEFT_KEY,          // left function key  "Back"/"Next"        |
                                                    //-----------------------------------------|
    #ifndef DISABLE_BACK_MOVE_SELECT
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_BMS_ICON_6,        // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_BMS_ICON_7,        // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                    //-----------------------------------------|
    #endif
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_SLIDER_BAR,        // Slider bar                              | - Start of active area
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_RIGHT_KEY,         // right function key "Edit"               |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_VALUE_2,           // Parameter value & units                 |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_VALUE_1,           // Parameter value & units                 |
                                                    //-----------------------------------------|
       #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_RESET_COMPLETE_1,         //                "Reset"                  |
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_RESET_COMPLETE_2,         //                  "OK"                   |
                                                    //-----------------------------------------|
       #endif
                                                    //-----------------------------------------|
        STRING_VIEW_FRAME_PACKET_READ_SWITCHES      //                                         |
                                                    //-----------------------------------------| - There is no keypress area
    }STRING_VIEW_FRAME_PACKET;

  
  
  
    TUSIGN8 isBuild = 0;
    TEXT_IDENTIFIER_TYPE text_id;
    static TUSIGN8 s_text[44] = {0};
    static TUSIGN8 s_textLen = 0;

									
	/*
			This string is used to indicate the frame stage(static, active or end).
			
			Static means that the whole frame will be updated from static elememt just like title and left / right button , slider bar.
			
			Active means that not the whole frame will be changed , the static will not be change and only some necessary elememt 
			will be changed just like value and the unit of value.\
			
	*/								
    typedef enum
    {//-----------------------------------------|
        FRAME_STAGE_STATIC = 0,                 //   Static from here (Nothing to update)  |    
        FRAME_STAGE_OPERATION_TITLE,            //  |   
        FRAME_STAGE_OPERATION_1,                  //  Active from here  (Start to update active area)                 //
        FRAME_STAGE_OPERATION_2,
        FRAME_STAGE_END=7,                      //   Frame update end       |                           //-----------------------------------------|
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

	
	/* This part code used to chose which frame to choose */
	if (LOCAL_updateStaticArea)
	{
		packetStage = FRAME_STAGE_STATIC;      /* If the whole frame need to update, we will start from slider bar, please refer to original code in the PackManagerBuilder.c*/
	}
	//else if ((LOCAL_updateActiveArea) && (packetStage > FRAME_STAGE_OPERATION))
	//{
	//	packetStage = FRAME_STAGE_OPERATION;		  /* If only active area need to update, we will start from button */
	//}
	else
	{
		asm("nop");
	}



        if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
	{
            packetStage =  FRAME_STAGE_STATIC;  			/*  If system language changed, we have to chenge the whole frame */
            LOCAL_updateStaticArea = eFALSE;
	}
	else if(packetStage>=FRAME_STAGE_NULL)
	{
            return eFALSE;
	}
									
	if(packetStage == FRAME_STAGE_STATIC)         	              
	{         
            TUSIGN8 left_button;
            TUSIGN8 sliderbar_num;
            TUSIGN8 sliderbar_high;
            TUSIGN8 titleBarLength = 0;
            TUSIGN8 const * titleBar;
            TBOOL hasUpdownButton;
		
            /*---------------------------        left button       ------------------------*/
            {
                #ifdef HMI_SUPPORTS_EASYSETUP
                if (LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.easySetupFlag != 0)
                {
                   left_button = 'N';
                }
                else
                #endif
                {
                   left_button = 'B';   //  back
                }
            }
            /*-----------------------------------  slider bar ---------------------------------*/
            
            #ifdef HMI_SUPPORTS_EASYSETUP
            if (LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.easySetupFlag != 0)
            {
                TUSIGN8 lastStage, currentStage;
                FrameTableGetEasySetupStages(&lastStage, &currentStage);
                sliderbar_high  = lastStage;
                sliderbar_num = currentStage;
            }
            else
            #endif
            {
                    TUSIGN8 high,actual;
                    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
                    FrameTableGetMenuPositionDetailsForTFT(&actual, &high);
                    #else
                    FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
                    #endif
                    sliderbar_high  = high;
                    sliderbar_num   = actual;
            }

            /* Add the title bar to packet*/
            E_STRING_TYPE stringType;

            LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.titleBar_attribidx;
            titleBar = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);   

            if(stringType==STRING_TYPE_LIBIDX)
            {
                titleBarLength = 2;
            }
            else
            {
                for(titleBarLength=0; titleBarLength<MAX_LENGTH_FRAME_TITLE; titleBarLength++)
                {
                    if(titleBar[titleBarLength] == '\0')
                    {
                        break;
                    }
                }
            }        

            LOCAL_packet_HL[7] = (stringType<<5)| titleBarLength;

            if (!FrameTableIsBackMoveSelectAvailable(LOCAL_pCurrDisplayFrame))
            {
                hasUpdownButton = eFALSE;
            }
            else
            {
                hasUpdownButton = eTRUE;
            }
        
            LOCAL_packet_HL[0] = FRAME_STRING_VIEW;
            LOCAL_packet_HL[1] = (packetStage<<5) + titleBarLength + 6;
            LOCAL_packet_HL[2] = testLanguage;
            preDisplayLang = testLanguage;

            LOCAL_packet_HL[3] = left_button;  
            LOCAL_packet_HL[4] = sliderbar_num; 
            LOCAL_packet_HL[5] = sliderbar_high;
            LOCAL_packet_HL[6] = hasUpdownButton; 
            
            memcpy(&LOCAL_packet_HL[8], titleBar, titleBarLength);
            if(AddPacketToBuffer(titleBarLength+8, 0))
            {
                isBuild +=1;
                packetStage = FRAME_STAGE_OPERATION_TITLE;
            }                 
	}				
							
/* ==================================  Active  Part ==================================================*/
	
	if(packetStage==FRAME_STAGE_OPERATION_TITLE)
	{
            TUSIGN8 titleLength = 0;
            E_STRING_TYPE stringType;
            LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.title_attribidx;

            TUSIGN8 const * title = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);

            if(stringType==STRING_TYPE_LIBIDX)
            {
                titleLength = 2;
            }
            else
            {
                for(titleLength=0; titleLength<MAX_LENGTH_FRAME_TITLE; titleLength++)
                {
                    if(title[titleLength] == '\0')
                    {
                        break;
                    }
                }
            }        
            
            LOCAL_packet_HL[2] = (stringType<<5)| titleLength;
            LOCAL_packet_HL[0] = FRAME_STRING_VIEW;
            LOCAL_packet_HL[1] = (packetStage<<5) + titleLength + 1 ;
            memcpy(&LOCAL_packet_HL[3], title, titleLength);

            if(AddPacketToBuffer(titleLength+3, 0))
            {
                    packetStage = FRAME_STAGE_OPERATION_1;   /* Jump to next stage*/
                    isBuild +=1;
            } 
	}


    if(packetStage==FRAME_STAGE_OPERATION_1)
    { 

        TUSIGN8 right_button = 0;
        TUSIGN8 ValueLength = 0;

        /*---------------------------        right button       ------------------------*/
        if ((LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM_NO_EDIT) || (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT))
        {
            right_button = 'B';     //right button == blank
        }
        else
        {
            if (WriteProtect_HMICanAccess(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText, (TUSIGN8)0/*write*/))
            {
                right_button = 'E';    //right button == edit
            }
            else
            {
                right_button = 'B';  
            }
        }

    /*-----------------------------VALUE---------------------------*/        
        
        TUSIGN8 i;
        s_text[0] = 0x00;
        text_id.ssIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText.ssIdx;
        text_id.objIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText.objIdx;
        text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText.attribIdx;

        s_textLen = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.maxCharacters;
        //TUSIGN8 extendedCharacterPage = FrameTableGetExtendedCharacterPage(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn);

#ifdef HMI_SUPPORTS_PACKEDASCII
        if ((LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM_PACKED) || (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT))
        {
            // 40 unpacked characters = 30 packed characters
            TUSIGN8 packedtext[31] = {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,
            0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,
            0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,
            0x00};
            TUSIGN8 packedLen;

            // len (unpacked) cannot be more than 40 therefore len * 6 cannot be more than 240
            //lint -e{734} Roger Arnold 2005-1-31
            i =  s_textLen * 6;
            packedLen = i / 8;
            if (i % 8)
            {
                packedLen ++;
            }

            for (i = 0; i < packedLen; i++)
            {
                // Return value of "AdjusterGetAttribute()" ignored
                //lint -e{534} Roger Arnold 2005-1-31
                AdjusterGetAttribute((TUSIGN16)text_id.ssIdx, (TUSIGN16)text_id.objIdx, i, (void FAST *)&packedtext[i]);
            }

            AdjusterUnpackPASCII(packedtext, packedLen, s_text);

            for(i=0; i<s_textLen; i++)
            {
                if(s_text[i]==0)
                {
                    break;
                }
            }

            s_textLen = i;
            
        }
#endif

#ifdef HMI_SUPPORTS_STANDARDASCII
        if ((LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM) || (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_STRING_PARAM_NO_EDIT))
        {
#if defined(HMI_SUPPORTS_SECURITY_ACCESS) && defined(DISGUISE_PASSWORDS)
            if ((text_id.ssIdx == HMI_IDX) && ((text_id.objIdx == HMI_IDX_SetPassword_StandardLevel)
                                               || (text_id.objIdx == HMI_IDX_SetPassword_AdvancedLevel)
                                                   || (text_id.objIdx == HMI_IDX_SetPassword_ServiceLevel)
                                                       || (text_id.objIdx == HMI_IDX_SetPassword_StandardLevelDefault)
                                                           || (text_id.objIdx == HMI_IDX_SetPassword_AdvancedLevelDefault)
                                                               || (text_id.objIdx == HMI_IDX_SetPassword_ServiceLevelDefault)))
            {
                for (i = 0; i < PASSWORD_LENGTH; i++)
                {
                    s_text[i] = 0x2A;
                }
                s_text[PASSWORD_LENGTH] = 0x00;
            }
            else
#endif
            {
                for (i = 0; i < s_textLen; i++)
                {
                    AdjusterGetAttribute((TUSIGN16)text_id.ssIdx, (TUSIGN16)text_id.objIdx, i, (void FAST *)&s_text[i]);
                }
                s_text[s_textLen] = 0x00;
            }
        }
#endif

/*
#if defined(HMI_SUPPORTS_STRINGS_OVER_20_CHARS) || (defined(HMI_SUPPORTS_STRINGS_OVER_19_CHARS) && defined(DISPLAY_SIZE_E))
        if (strlen((char *)text) > FONT1_MAX_CHARS-1)
        {
            text[FONT1_MAX_CHARS-1] = 0x00;
            //packetsBuilt += BuildWriteString(text,extendedCharacterPage,4,JUSTIFY_RIGHT_WITH_INDENT_1,FONT_0,NO_INVERT,0,0,FONT1_MAX_CHARS-1,1);
        }
        else
#endif
        {
            i = strlen((char *)s_text) - 1;
            while ((i > 0) && (s_text[i] == ' '))
            {
                s_text[i] = 0x00;
                i --;
            }

            //packetsBuilt += BuildWriteString(text,extendedCharacterPage,4,JUSTIFY_RIGHT_WITH_INDENT_1,FONT_1,NO_INVERT,0,0,FONT1_MAX_CHARS-1,1);
        }
*/

        if(s_textLen>20)
        {
            ValueLength = 20;
        }
        else
        {
            ValueLength = s_textLen;
        }

        LOCAL_packet_HL[0] = FRAME_STRING_VIEW;        
        LOCAL_packet_HL[1] = (packetStage<<5) + ValueLength + 2; 
        LOCAL_packet_HL[2] = right_button;
        LOCAL_packet_HL[3] = ValueLength; //valuelength
        memcpy(&LOCAL_packet_HL[4],s_text,ValueLength);

        if(AddPacketToBuffer(ValueLength+4, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild +=1;
        }
        
    }

    if(packetStage==FRAME_STAGE_OPERATION_2)
    {
        TUSIGN8 varLen_2 = 0;

        if(s_textLen>20)
        {
            varLen_2 = s_textLen - 20;
        }
        
        LOCAL_packet_HL[0] = FRAME_STRING_VIEW;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 1 + varLen_2; 
        LOCAL_packet_HL[2] = varLen_2;
        if(varLen_2>0)
        {
            if(varLen_2>12)
            {
                varLen_2 = 12;
            }

            memcpy(&LOCAL_packet_HL[3], &s_text[20], varLen_2);
        }
        
        if(AddPacketToBuffer(3+varLen_2, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild +=1;
        }  
    }
        /*------------------------------------end-------------------------------*/
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1;
        }  
    }


    return isBuild;

}

TUSIGN8         BuildSFrame_StringEdit(void)
{
#if (!TEST_FRAME_STRING_EDIT)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif


typedef enum
{                                               // Packet                                  |
    STRING_EDIT_FRAME_PACKET_TITLE_BAR,         // Parameter title                         | - Start of Static Area
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_ID_2,              // Secondary Identifier                    |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_ICON,              // Title Bar Icon                          |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_T,          // Character Set Top Border                |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_B,          // Character Set Bottom Border             |
                                                //-----------------------------------------|
  #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_L_T,        // Character Set LHS Border (Top Half)     |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_L_B,        // Character Set LHS Border (Bottom Half)  |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_R_T,        // Character Set RHS Border (Top Half)     |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_R_B,        // Character Set RHS Border (Bottom Half)  |
                                                //-----------------------------------------|
  #else
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_T_C,        // Character Set Top Border at cursor      |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_BORDER_B_C,        // Character Set Bottom Border at cursor   |
                                                //-----------------------------------------|
  #endif
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_LEFT_KEY,          // left function key  "Next"               |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_ID_PROMPT,         // Prompt "ID"            (for #code Entry)|
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_PIN_PROMPT,        // Prompt "Pin"           (for #code Entry)|
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_PASSWORD_PROMPT,   // Prompt "Password"      (for #code Entry)|
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_ID,                // ID                     (for #code Entry)|
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_PIN,               // PIN                    (for #code Entry)|
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_VALUE,             // String value                            | - Start of keypress Area
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_CHARSET,           // Character Set                           |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_HIGHLIGHT,         // LHS of highlight (selected char)        |
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_RIGHT_KEY,         // right function key                      | - Start of Active Area
                                                //-----------------------------------------|
    STRING_EDIT_FRAME_PACKET_READ_SWITCHES      //                                         |
                                                //-----------------------------------------|
}STRING_EDIT_FRAME_PACKET;


    //TUSIGN8 packetsBuilt = 0;
    TUSIGN8 isBuild =0;
    TUSIGN8 characterSetline = 5;


 typedef enum
    {
      FRAME_STAGE_STATIC,   
      FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE,
      FRAME_STAGE_OPERATION_STRING_ACTIVE,
      FRAME_STAGE_END = 7,                 
      FRAME_STAGE_NULL,
    }FRAME_STAGE;
	
	
    #ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER
    TUSIGN8 SecondaryIdentifierText[SECONDARY_IDENTIFIER_LEN];
    TBOOL SecondaryIdentifierActive = FrameTableGetSecondaryIdentifier(SecondaryIdentifierText);
    #endif

    #ifdef HMI_SUPPORTS_SECURITY_ACCESS

typedef enum
    {
    STRING_MODE_STANDARD,
    STRING_MODE_PASSWORD,
    STRING_MODE_SERVICE,
    STRING_MODE_SET_PASSWORD,
    }_STRING_MODE;

    _STRING_MODE StringMode = STRING_MODE_STANDARD;

    if (LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.specialStringFlag == 2)
    {
        StringMode = STRING_MODE_SERVICE;
    }
    else if (LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.specialStringFlag != 0)
    {
        TBOOL   ARM_highSecurityState = AdjusterGetARMhighSecurityState();
        TBOOL   ARM_serviceRequested  = FrameTableGetRequestedARMLevel() == ARM_ACCESS_SERVICE ? eTRUE : eFALSE;

        StringMode = STRING_MODE_PASSWORD;

        if ((ARM_serviceRequested) && (ARM_highSecurityState))
        {
            StringMode = STRING_MODE_SERVICE;
        }
    }
    else
    {
        TUSIGN8 stringSI = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText.ssIdx;
        TUSIGN8 stringOI = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText.objIdx;
        if ((stringSI == HMI_IDX) && ((stringOI == HMI_IDX_SetPassword_StandardLevel)
                                   || (stringOI == HMI_IDX_SetPassword_AdvancedLevel)
                                   || (stringOI == HMI_IDX_SetPassword_ServiceLevel)
                                   || (stringOI == HMI_IDX_SetPassword_StandardLevelDefault)
                                   || (stringOI == HMI_IDX_SetPassword_AdvancedLevelDefault)
                                   || (stringOI == HMI_IDX_SetPassword_ServiceLevelDefault)))
        {
            StringMode = STRING_MODE_SET_PASSWORD;
        }
    }
    #endif

    #if defined(HMI_SUPPORTS_ASIAN) && !defined(TYPE_4_IMPLEMENTATION) && !defined(TYPE_5_IMPLEMENTATION)
    if (LOCAL_Asian)
    {
        characterSetline --;
    }
    #endif

    if (LOCAL_updateStaticArea || (testLanguage != preDisplayLang))
    {
        LOCAL_packetStage = FRAME_STAGE_STATIC;
		packetStage = FRAME_STAGE_STATIC;
                LOCAL_updateStaticArea = eFALSE;
    }
    else if ((LOCAL_keypressProcessed) && (LOCAL_packetStage > FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE))
    {
        LOCAL_packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;
	packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;

    }
//    else if ((LOCAL_updateActiveArea) && (LOCAL_packetStage > FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE))
//    {
//        LOCAL_packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;
//	packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;
//        LOCAL_updateActiveArea = eFALSE;
//    }
    else
    {
        asm("nop");
    }

    	if (packetStage == FRAME_STAGE_END)
	{
		packetStage = FRAME_STAGE_END;
	}
    if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
	{
		packetStage =  FRAME_STAGE_STATIC;  			/*  If system language changed, we have to chenge the whole frame */
		LOCAL_updateStaticArea = eFALSE;
	}
	else if(packetStage>=FRAME_STAGE_NULL)
	{
		return eFALSE;
	}
    // if (LOCAL_packetStage == STRING_EDIT_FRAME_PACKET_READ_SWITCHES)
    // {
        // packetsBuilt += BuildReadSwitches(1);
        // LOCAL_packetStage = STRING_EDIT_FRAME_PACKET_READ_SWITCHES;
    // }
    TUSIGN8 TitleBarLength = 0;
    TUSIGN8 StringValue_len = 0;

    if(packetStage == FRAME_STAGE_STATIC)
    {
        E_STRING_TYPE stringType;
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.title_attribidx;
        
        TUSIGN8 const *TitleBar = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);

        if(stringType==STRING_TYPE_LIBIDX)
        {
            TitleBarLength = 2;
        }
        else
        {
            for(TitleBarLength=0; TitleBarLength<MAX_LENGTH_PARAM_NAME; TitleBarLength++)
            {
                if(TitleBar[TitleBarLength] == '\0')
                {
                    break;
                }
            }
        }  
        LOCAL_packet_HL[3] = (stringType<<5)| TitleBarLength;

//  Prepare data for stages STRING_EDIT_FRAME_PACKET_ID_PROMPT..STRING_EDIT_FRAME_PACKET_PIN (Additional info for service level hashcode entry)
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_ID_PROMPT          // Prompt "ID"            (for #code Entry)|
// STRING_EDIT_FRAME_PACKET_PIN_PROMPT         // Prompt "Pin"           (for #code Entry)|
// STRING_EDIT_FRAME_PACKET_PASSWORD_PROMPT    // Prompt "Password"      (for #code Entry)|
// STRING_EDIT_FRAME_PACKET_ID                 // ID                     (for #code Entry)|
// STRING_EDIT_FRAME_PACKET_PIN                // PIN                    (for #code Entry)|
//---------------------------------------------//-----------------------------------------|
        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
        if (StringMode == STRING_MODE_SERVICE)
        #endif
        {  
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_ID_PROMPT          // Prompt "ID"            (for #code Entry)|
//---------------------------------------------//-----------------------------------------|
        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
            LOCAL_packet_HL [6+TitleBarLength] = IDS_ID_PROMPT -  IDS_PASSWORD_PROMPT; // 1index -> 526 +1, refer stringDefinition.h
        #endif
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_PIN_PROMPT         // Prompt "Pin"           (for #code Entry)|
//---------------------------------------------//-----------------------------------------|

        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
            LOCAL_packet_HL [7+TitleBarLength] = IDS_PIN_PROMPT - IDS_PASSWORD_PROMPT; // 2 index -> 526 +2, refer stringDefinition.h
        #endif


//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_PASSWORD_PROMPT    // Prompt "Password"      (for #code Entry)|
//---------------------------------------------//-----------------------------------------|

        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
            LOCAL_packet_HL [8+TitleBarLength] = IDS_PASSWORD_PROMPT - IDS_PASSWORD_PROMPT; //0 index -> 526+0 , refer stringDefinition.h
        #endif
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_ID                 // ID                     (for #code Entry)|
//---------------------------------------------//-----------------------------------------|

        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
            TUSIGN8 IDText[9];
            AdjusterGetArmID(IDText, 8);
            IDText[8] = 0x00;
            memcpy(&LOCAL_packet_HL [9+TitleBarLength],IDText,9);
        #endif
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_PIN                // PIN                    (for #code Entry)|
//---------------------------------------------//-----------------------------------------|
 
        #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
            //TUSIGN8     pinText[6];
            TUSIGN16    pin = (TUSIGN16)AdjusterGetARMpin();
            LOCAL_packet_HL [9+TitleBarLength] = pin;
            LOCAL_packet_HL [10+TitleBarLength] = (pin >> 8);


            //pinText[0] = ((TUSIGN8)(pin / 10000)) + 0x30;
            pin = pin % 10000;
            //pinText[1] = ((TUSIGN8)(pin / 1000)) + 0x30;
            pin = pin % 1000;
            //pinText[2] = ((TUSIGN8)(pin / 100)) + 0x30;
            pin = pin % 100;
            //pinText[3] = ((TUSIGN8)(pin / 10)) + 0x30;
            pin = pin % 10;
            //pinText[4] = ((TUSIGN8)(pin)) + 0x30;
            //pinText[5] = 0x00;
        #endif
        
        }

        
        LOCAL_packet_HL[0] = FRAME_STRING_EDIT;        
        LOCAL_packet_HL[1] = (packetStage<<5) + TitleBarLength + 2; 
        LOCAL_packet_HL[2] = testLanguage | (StringMode << 5);
        preDisplayLang = testLanguage;
        
        memcpy(&LOCAL_packet_HL[4],TitleBar,TitleBarLength);
        if(StringMode != STRING_MODE_SERVICE)
        {
            if(AddPacketToBuffer(TitleBarLength+4, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;
                isBuild +=1;
            }  
        }
        else
        {
            LOCAL_packet_HL[1] = (packetStage<<5) + TitleBarLength + 9;
            if(AddPacketToBuffer(TitleBarLength+11, 0))
            {
                packetStage = FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE;
                isBuild +=1;
            } 
        }
}

if(packetStage == FRAME_STAGE_OPERATION_KEYPRESS_ACTIVE)
{
// FRAME_STRING_EDIT								LOCAL_packet_HL [0]
// Length											LOCAL_packet_HL [1]
// CharSetline  1 byte								
// Right KEY    1 byte								LOCAL_packet_HL [2]
// CharSet text and value							LOCAL_packet_HL [3+21]
  

        TUSIGN8 cursor = FrameTableGetCursorPosition();
        if(cursor>=18)
        {
            cursor++;
            cursor--;
        }
        //TUSIGN8 extendedCharacterPage = FrameTableGetExtendedCharacterPage(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn);
        
		TUSIGN8 LimitedNumericCharacterSet[13];
//        TBOOL   LimitedNumericCharacterSetInUse = eFALSE;

        TUSIGN8 const *characterSet = FrameTableGetCharacterSet(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn);
        
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_CHARSET            // Character Set                           |
//---------------------------------------------//-----------------------------------------|


        if (LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn == CHARACTER_SET_NUMERIC)
        {

            TBOOL   AllowSpace;
            TUSIGN8 MinValid, MaxValid;
            FrameTableGetNumericCharacterSetRestrictions(&AllowSpace, &MinValid, &MaxValid);

            if ((!AllowSpace) || (MinValid != 0) || (MaxValid != 9))
            {
                memset((void *)LimitedNumericCharacterSet, 0x00, 13);
                TUSIGN8 ii,jj;

                ii = 0;
                LimitedNumericCharacterSet[ii ++] = 0x08;   //Terminator
                if (AllowSpace)
                {
                    LimitedNumericCharacterSet[ii ++] = 0x20;   //Space
                }

                if((MaxValid-MinValid)>9)
                {
                    return isBuild;
                }
                
                for (jj = MinValid; jj <= MaxValid; jj++)
                {
                    LimitedNumericCharacterSet[ii ++] = jj + 0x30;
                }
                characterSet = LimitedNumericCharacterSet;
//                LimitedNumericCharacterSetInUse = eTRUE;
            }
        }

        if (!characterSet)
        {
            // Do not display character set if no character set found
            LOCAL_packetStage ++;
        }
        else
        {
            TUSIGN8 len = (TUSIGN8)strlen((const char *)characterSet);
            TUSIGN8 selectedChar,j;
            #if defined(HMI_SUPPORTS_STRINGS_OVER_19_CHARS) || defined(HMI_SUPPORTS_STRINGS_OVER_20_CHARS)
            TUSIGN8 text1[41];
            #else
            TUSIGN8 text1[23];
            #endif
            TINT16 cursor1 = (TINT16)FrameTableGetCursorPosition();

            // Return value of "strcpy()" ignored
            //lint -e{534} Roger Arnold 2005-1-31
            strcpy((char *)text1,(char *)AdjusterGetVariableAsString(&j));

            // find selected character
            if (cursor1 > strlen((char *)text1))
            {
                selectedChar = characterSet[0];
            }
            else
            {
                selectedChar = text1[cursor1];
            }

            // set cursor so that it defines the offset to the selected character in character set
            cursor1 = 0;
            while ((characterSet[cursor1] != selectedChar) && (cursor1 < len))
            {
                cursor1++;
            }
            if (cursor1 >= len)
            {
                cursor1 = 0;
            }

            // move cursor 10 characters back
            cursor1 -= 10;

            if (len > 10)
            {
                while (cursor1 < 0)
                {
                   cursor1 += len;
                }
            }

            // copy maximum number of characters from character set to "text"
            for (j = 0;j < (TUSIGN8)(FONT1_MAX_CHARS); j++)
            {
                if ((cursor1 < 0) || (cursor1 >= len))
                {
                    text1[j] = ' ';
                }
                else
                {
                    text1[j] = characterSet[cursor1];
                }

                cursor1 ++;


                if (len > 10)
                {
                    if (cursor1 > (len-1))
                    {
                        cursor1 = 0;
                    }
                }
            }
            text1[(TUSIGN8)(FONT1_MAX_CHARS)] = 0;
            
            #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                text1[(TUSIGN8)(FONT1_MAX_CHARS - 1)] = 0;
				memcpy(&LOCAL_packet_HL[3],text1,FONT1_MAX_CHARS);
                //packetsBuilt += BuildWriteString(text,extendedCharacterPage,characterSetline,JUSTIFY_LEFT_WITH_INDENT_5_PIXEL,FONT_0,CHAR_INVERT,10,0,1,0);
            #else
				
				memcpy(&LOCAL_packet_HL[3],text1,FONT1_MAX_CHARS);
                //packetsBuilt += BuildWriteString(text,extendedCharacterPage,characterSetline,JUSTIFY_LEFT,FONT_0,CHAR_INVERT,10,0,1,0);
            #endif
        }
    
//---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_HIGHLIGHT          // LHS of highlight (selected char)        |
//---------------------------------------------//-----------------------------------------|
//CharSetline    
        #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
            //packetsBuilt += BuildWriteIcon(ICON_1P_BLACK, 74, 10, 0);
        #else
            #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
                //LOCAL_packetStage ++;   // Additional Highlight not required on TFT displays
            #else
                // const TUSIGN8 SelectedCharacterHighlight = 0xFF;
                // packetsBuilt += BuildCustomCharacterString(&SelectedCharacterHighlight,
                                                           // 1,
                                                           // 59,
                                                           // characterSetline,
                                                           // 0,
                                                           // 0);
            #endif
        #endif
                
	
//---------------------------------------------//-----------------------------------------|
// STRING_VIEW_FRAME_PACKET_RIGHT_KEY          // right function key                      |
//---------------------------------------------//-----------------------------------------|

        TBOOL cursorAtLimit = FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit() ? eTRUE : eFALSE;

        if ((WriteProtect_HMICanAccess(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.stringText, (TUSIGN8)0)) && (cursorAtLimit == eFALSE))
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_OK;
            LOCAL_packet_HL [2] = cursorAtLimit;
        }
        else
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_CANCEL;
	    LOCAL_packet_HL [2] = cursorAtLimit;
        }
		
		LOCAL_packet_HL[0] = FRAME_STRING_EDIT;
		LOCAL_packet_HL[1] = (packetStage<<5) + FONT1_MAX_CHARS + 1;
		//LOCAL_packet_HL[2] = characterSetline;
		if(AddPacketToBuffer(FONT1_MAX_CHARS+3, 0))
		{
			isBuild +=1;
			packetStage = FRAME_STAGE_OPERATION_STRING_ACTIVE;
		}
}   


if(packetStage == FRAME_STAGE_OPERATION_STRING_ACTIVE)
{
  // FRAME_STRING_EDIT								LOCAL_packet_HL [0]
  // Length									LOCAL_packet_HL [1]
  // Cursor                                                                     LOCAL_packet_HL [2]
  // String value [0] ,								
  // string value[cursor] 							
  // string value and Length							LOCAL_packet_HL [3+len]
  //---------------------------------------------//-----------------------------------------|
// STRING_EDIT_FRAME_PACKET_VALUE              // String value                            |
//---------------------------------------------//-----------------------------------------|
        TUSIGN8 maxChars;
        #if defined(HMI_SUPPORTS_STRINGS_OVER_19_CHARS) || defined(HMI_SUPPORTS_STRINGS_OVER_20_CHARS)
        TUSIGN8 text[41];
        #else
        TUSIGN8 text[22];
        #endif
        TUSIGN8 i;
        TUSIGN8 cursor = FrameTableGetCursorPosition();
        //TUSIGN8 extendedCharacterPage = FrameTableGetExtendedCharacterPage(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn);
        
    //TUSIGN8 LimitedNumericCharacterSet[13];
    //        TBOOL   LimitedNumericCharacterSetInUse = eFALSE;

        //TUSIGN8 const *characterSet = FrameTableGetCharacterSet(LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.characterSet_dpPosn);
        
        // Return value of "strcpy()" ignored
        //lint -e{534} Roger Arnold 2005-1-31
        strcpy((char *)text,(char *)AdjusterGetVariableAsString(&i));
        // get max available length from frame table
        maxChars = LOCAL_pCurrDisplayFrame->frameDefinition.stringParamFrame.attributes.maxCharacters;

        // make string as long as it can be by adding special character (0x09)
        for (i = (TUSIGN8)strlen((char *)text); i < maxChars; i++)
        {
            text[i] = 0x09;
        }

        #if defined (HMI_SUPPORTS_SECURITY_ACCESS) && defined(DISGUISE_PASSWORDS)
        if (StringMode != STRING_MODE_STANDARD)
        {
            for (i = 0; i < maxChars; i++)
            {
                if (i != cursor)
                {
                    text[i] = 0x2A;
                }
            }
        }
        #endif

        text[maxChars] = 0x00;
        //This will limit BuildSFrame_StringEdit() to only send 18 bytes for string value, due lack of buffer size and packet size
        // if SHMI
        
        if(maxChars > 20)
        {
          StringValue_len = 20;
        }
        else
        {
        StringValue_len = maxChars;
        }

        if (cursor >= sizeof(text) - 1)
        {
            // corrupt cursor - skip stage
            LOCAL_packetStage ++;
        }
        else
        {
            if (cursor != FrameTableGetCursorPositionLimit())
            {
                if (text[cursor] == 0x09)           // if the cursor is beyond the end of the valid string then
                {
                    text[cursor] = 0x08;            // put the special character (0x08) at the cursor position
                }

                if (cursor <= 19)
                {
                    text[20] = 0x00;
                    #if defined(HMI_SUPPORTS_SECURITY_ACCESS)
                    if (StringMode == STRING_MODE_SERVICE)
                    {
                         LOCAL_packet_HL [2] = FrameTableGetCursorPosition();
                         memcpy(&LOCAL_packet_HL[3],&text[0],StringValue_len);
                        // packetsBuilt += BuildWriteString(&text[0],                              // const TUSIGN8 *suppliedText
                                                         // HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,  // TUSIGN8 extendedCharacterPage
                                                         // 3,                                     // TUSIGN8 line
                                                         // JUSTIFY_LEFT_WITH_INDENT_12,           // TUSIGN8 justify
                                                         // FONT_0,                                // TUSIGN8 font
                                                         // CHAR_INVERT,                           // TUSIGN8 invert
                                                         // cursor,                                // TUSIGN8 invchar
                                                         // 0,                                     // TUSIGN8 blank
                                                         // 1,                                     // TUSIGN8 minLength
                                                         // 0);                                    // TUSIGN8 lastPacket
                    }
                    else
                    #endif
                    {
                      LOCAL_packet_HL [2] = FrameTableGetCursorPosition();
                      memcpy(&LOCAL_packet_HL[3],&text[0],StringValue_len);
                        // packetsBuilt += BuildWriteString(&text[0],
                                                         // extendedCharacterPage,
                                                         // 2,
                                                         // #ifdef DISPLAY_SIZE_F
                                                         // JUSTIFY_LEFT_WITH_INDENT_1,
                                                         // #else
                                                         // JUSTIFY_LEFT,
                                                         // #endif
                                                         // FONT_0,
                                                         // CHAR_INVERT,
                                                         // cursor,
                                                         // 0,
                                                         // 1,
                                                         // 0);
                    }
                }
                else
                {
                    cursor +=1;
                    text[cursor] = 0x00;
                    cursor -= 20;
                    
                     LOCAL_packet_HL [2] = 19;

                    #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && defined(HMI_SUPPORTS_ARM_SERVICE)
                    if (StringMode == STRING_MODE_SERVICE)
                    {
                       memcpy(&LOCAL_packet_HL[3],&text[cursor],StringValue_len);

                        //packetsBuilt += BuildWriteString(&text[cursor],HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,3,JUSTIFY_LEFT_WITH_INDENT_12,FONT_0,CHAR_INVERT,19,0,1,0);
                    }
                    else
                    #endif
                    {
                           
                           memcpy(&LOCAL_packet_HL[3],&text[cursor],StringValue_len);
                        // packetsBuilt += BuildWriteString(&text[cursor],
                                                         // extendedCharacterPage,
                                                         // 2,
                                                         // #ifdef DISPLAY_SIZE_F
                                                         // JUSTIFY_LEFT_WITH_INDENT_1,
                                                         // #else
                                                         // JUSTIFY_LEFT,
                                                         // #endif
                                                         // FONT_0,
                                                         // CHAR_INVERT,
                                                         // 19,
                                                         // 0,
                                                         // 1,
                                                         // 0);
                    }
                }
            }
            else
            {
                LOCAL_packet_HL [2] = FrameTableGetCursorPosition();
                text[20] = 0x00;
                #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && defined(HMI_SUPPORTS_ARM_SERVICE)
                if (StringMode == STRING_MODE_SERVICE)
                {
					memcpy(&LOCAL_packet_HL[3],&text,StringValue_len);
                    //packetsBuilt += BuildWriteString(text,HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,3,JUSTIFY_LEFT_WITH_INDENT_12,FONT_0,NO_INVERT,0,0,1,0);
                }
                else
                #endif
                {
					
					memcpy(&LOCAL_packet_HL[3],&text,StringValue_len);
                    // packetsBuilt += BuildWriteString(text,
                                                     // extendedCharacterPage,
                                                     // 2,
                                                     // #ifdef DISPLAY_SIZE_F
                                                     // JUSTIFY_LEFT_WITH_INDENT_1,
                                                     // #else
                                                     // JUSTIFY_LEFT,
                                                     // #endif
                                                     // FONT_0,
                                                     // NO_INVERT,
                                                     // 0,
                                                     // 0,
                                                     // 1,
                                                     // 0);
                }
            }
        }

  
  		LOCAL_packet_HL[0] = FRAME_STRING_EDIT;
		LOCAL_packet_HL[1] = (packetStage<<5) + StringValue_len + 1;
		//LOCAL_packet_HL[2] = characterSetline;
		if(AddPacketToBuffer(StringValue_len+3, 0))
		{
			isBuild +=1;
			packetStage = FRAME_STAGE_END;
		}
}

	/*------------------------------------end-------------------------------*/
		if(packetStage==FRAME_STAGE_END)
		{
			LOCAL_packet_HL[0] = FRAME_TYPE_END;        
			LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
			if(AddPacketToBuffer(2, 1))
			{
				packetStage = FRAME_STAGE_NULL;
				isBuild +=1;
			}  
		}																			
	

    return isBuild;

}



#endif

#ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_EnumView(void)
{
    TUSIGN8 isBuild = 0;
    
    
    static TUSIGN8 s_valueAndUnit[23] = {0};   //first byte is the ident
    

#if (!TEST_FRAME_ENUM_VIEW)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
									
    typedef enum
    {                                                   //-----------------------------------------|
        FRAME_STAGE_STATIC = 0,   		        //   Static from here (Nothing to update)  |
        FRAME_STAGE_OPERATION_TITLE,            
        FRAME_STAGE_OPERATION,          		//	 Active from here  (Start to update active area)
        FRAME_STAGE_END=7,                 		//   Frame update end 					   |
        FRAME_STAGE_NULL,							
    }FRAME_STAGE;									


    if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
    {
        packetStage =  FRAME_STAGE_STATIC;              /*  If system language changed, we have to chenge the whole frame */
        LOCAL_updateStaticArea = eFALSE;
        s_valueAndUnit[0] = 0xFF;
    }
    else if(LOCAL_updateActiveArea)
    {
        packetStage =  FRAME_STAGE_OPERATION; 
        LOCAL_updateActiveArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        return eFALSE;
    }
   
    if(packetStage == FRAME_STAGE_STATIC)              
    {
        TUSIGN8 left_button;
        TUSIGN8 titleLen = 0;        
        TUSIGN8 const * pTitle;
        E_STRING_TYPE stringType;
        TBOOL hasUpdownButton;

        /*---------------------------        left button       ------------------------*/        
#ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.easySetupFlag != 0)
        {
            left_button = 'N';
	}
	else
#endif
	{
            left_button = 'B';   //  back
	}

        /* Add the title bar to packet*/
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.titleBar_attribidx; 
        
        pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            titleLen = 2;
        }
        else
        {
            for(titleLen=0; titleLen<MAX_LENGTH_FRAME_TITLE; titleLen++)
            {
                if(pTitle[titleLen] == '\0')
                {
                    break;
                }
            }
        }   
                
        LOCAL_packet_HL[5] = (stringType<<5)| titleLen;

        if (!FrameTableIsBackMoveSelectAvailable(LOCAL_pCurrDisplayFrame))
        {
            hasUpdownButton = eFALSE;
        }
        else
        {
            hasUpdownButton = eTRUE;
        }
        
        LOCAL_packet_HL[0] = FRAME_ENUM_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + titleLen + 4;
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = left_button;  
        LOCAL_packet_HL[4] = hasUpdownButton;
        memcpy(&LOCAL_packet_HL[6], pTitle, titleLen);
        if(AddPacketToBuffer(titleLen+6, 0))
        {
            isBuild +=1;
            packetStage = FRAME_STAGE_OPERATION_TITLE;
        }                 
    }

    //enum param name and value
    if(packetStage==FRAME_STAGE_OPERATION_TITLE)
    {         
        TUSIGN8 sliderbar_num;
        TUSIGN8 sliderbar_high;
        TUSIGN8 titleLen = 0;
        E_STRING_TYPE stringType;
        
        /*-----------------------------------  slider bar ---------------------------------*/
        #ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.easySetupFlag != 0)
        {
            TUSIGN8 lastStage, currentStage;
            FrameTableGetEasySetupStages(&lastStage, &currentStage);
            sliderbar_high = lastStage;
            sliderbar_num  = currentStage;
        }
        else
        #endif
        {
            TUSIGN8 high,actual;
            FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
            sliderbar_high = high;
            sliderbar_num  = actual;
        }

        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.title_attribidx;
        
        TUSIGN8 const * pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);

        if(stringType==STRING_TYPE_LIBIDX)
        {
            titleLen = 2;
        }
        else
        {
            for(titleLen=0; titleLen<MAX_LENGTH_PARAM_NAME; titleLen++)
            {
                if(pTitle[titleLen] == '\0')
                {
                      break;
                }
            }
        }
        LOCAL_packet_HL[4] = (stringType<<5)| titleLen;

        LOCAL_packet_HL[0] = FRAME_ENUM_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + titleLen + 3;        
        LOCAL_packet_HL[2] = sliderbar_num; 
        LOCAL_packet_HL[3] = sliderbar_high;
        memcpy(&LOCAL_packet_HL[5], pTitle, titleLen);

        if(AddPacketToBuffer(titleLen+5, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;   /* Jump to next stage*/
            isBuild +=1;
        } 
    }

	
    if(packetStage==FRAME_STAGE_OPERATION)
    {   
        TUSIGN16 OK_val;
        ADJUST_DATA tVal;
        TUSIGN8 tType;
        T_RANGE_E8 enum8Range;
        #ifdef HMI_SUPPORTS_16BIT_ENUMS
        T_RANGE_E16 enum16Range;
        #endif

        TBOOL   rulesAreActive;
        TUSIGN8 right_button = 0;
        
        //E_STRING_TYPE unitStrType; 
        //TUSIGN8 units[10];
        //TUSIGN8 unitLen = 0;
        
        const TUSIGN8* pString;        
        TUSIGN8 strLen = 0;          
        E_STRING_TYPE strType;

        
        /*---------------------------        right button       ------------------------*/
        if (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT)
        {
            right_button = 'B';     //right button == blank
        }
        else
        {
            if (WriteProtect_HMICanAccess(LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value, 0))
            {
                    right_button = 'E';    //right button == exit
            }
            else
            {
                    right_button = 'B';  
            }
        }
		     
	/*-----------------------------VALUE---------------------------*/
        if (LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.engUnitsFlag != UOM_NOT_APPLICABLE)
        {
            pString = PacketManagerGetEngUnits_s((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.ssIdx,
                                                                   LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.objIdx,
                                                                   LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.attribIdx,
                                                                   LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.engUnitsFlag,
                                                                   &strType);
                
            for(strLen=0; strLen<MAX_LENGTH_UNIT ; strLen++)
            {
                if(pString[strLen] == 0)
                {
                    break;
                }
            } 

        }
        else
        {                
            OK_val = AdjusterGetEnumValueAndRange((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.ssIdx,
                                                                                      (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.objIdx,
                                                                                      LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.attribIdx,
                                                                                      &tVal,
                                                                                      &enum8Range,
                                                                                      #ifdef HMI_SUPPORTS_16BIT_ENUMS
                                                                                      &enum16Range,
                                                                                      #endif
                                                                                      &rulesAreActive,
                                                                                      &tType
                                                                                      );

            if (OK_val == OK)
            {
                TUSIGN16 tableOffset = ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.firstItem_MSByte << 8)
                                                         + LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.firstItem_LSByte
                                                         + tVal.data_8;

                LOCAL_stringtable_text_id.attribIdx = FrameTableGetEnumItem(tableOffset);
            }
            else
            {
                LOCAL_stringtable_text_id.attribIdx = 0;
            }

            pString = FrameTableGetStringSpecificLanguage_s(&strType, LOCAL_stringtable_text_id, testLanguage);
            if(strType==STRING_TYPE_LIBIDX)
            {
                strLen = 2;
            }
            else
            {
                for(strLen=0; strLen<MAX_LENGTH_UNIT; strLen++)
                {
                    if(pString[strLen] == '\0')
                    {
                        break;
                    }
                }
            }  
            
        }        
        
        LOCAL_packet_HL[3] = (strType<<5)|strLen;
        
        if((LOCAL_packet_HL[3]==s_valueAndUnit[0]) 
        && (IsSamePacket(&s_valueAndUnit[1], (TUSIGN8*)pString, strLen)))
        {//if the same packet, not send again
            packetStage = FRAME_STAGE_END;
        }
        else
        {
            LOCAL_packet_HL[0] = FRAME_ENUM_VIEW;        
            LOCAL_packet_HL[1] = (packetStage<<5) + strLen + 2; 
            LOCAL_packet_HL[2] = right_button;
            memcpy(&LOCAL_packet_HL[4], pString, strLen);
            if(AddPacketToBuffer(strLen+4, 0))
            {
                s_valueAndUnit[0] = LOCAL_packet_HL[3];
                memcpy(&s_valueAndUnit[1], pString, strLen);
                packetStage = FRAME_STAGE_END;
                isBuild +=1;
            }        
        }
        
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1;
        }  
    }
    
    return isBuild;
}

#define LANGUAGE_ENGLISH_ATTRIBUTE    29

// No restriction on the optimization of this function

TUSIGN8 BuildSFrame_EnumEdit(void)
{
    TUSIGN8 isBuild = 0;
    static TINT16 DisplayedValues[6];
    TUSIGN8 i;

    typedef enum
    {
        FRAME_STAGE_STATIC,   
        FRAME_STAGE_TEXT_1,
        FRAME_STAGE_TEXT_2,
        FRAME_STAGE_TEXT_3,
        FRAME_STAGE_TEXT_4,
        FRAME_STAGE_TEXT_5,
        FRAME_STAGE_TEXT_6,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    
    if ((testLanguage != preDisplayLang) 
    ||   LOCAL_updateStaticArea)
    {
        packetStage = FRAME_STAGE_STATIC;
        LOCAL_updateStaticArea = eFALSE; 
        for(i=0; i<6; i++)
        {
            DisplayedValues[i] = -1;
        }
    }
    else if ((LOCAL_keypressProcessed) && (packetStage > FRAME_STAGE_TEXT_1))
    {
        packetStage = FRAME_STAGE_TEXT_1;
    }
    #ifdef HMI_USES_NEWLINE    
    else if ((LOCAL_updateActiveArea) && (packetStage > FRAME_STAGE_TEXT_1))
    {
        packetStage = FRAME_STAGE_TEXT_1;
        LOCAL_updateActiveArea = eFALSE;
    }
    #endif
    else if(packetStage>=FRAME_STAGE_NULL)
    {
      return eFALSE;
    }


#if (!TEST_FRAME_ENUM_EDIT)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
            {
            testLanguage = HMI_LANGUAGE_ENGLISH;
            }
            else
            {
            testLanguage = SFD_SIMPLE.language;
            }
#else
        testLanguage = SFD_SIMPLE.language;
#endif

    TUSIGN8  nBits;
    TUSIGN16 eVal = AdjusterGetEnumVariable(&nBits);

    TBOOL eValOffsetShown;
    TUSIGN8 startPacket;

    T_UNIT const SLOW *pSub; // Pointer to Subsystem
    T_DO_ATTRIB_RANGE Range; // range information
    TUSIGN16 eValOffset;

    TUSIGN16 numberOfCodes;
    TUSIGN8  itemsPerMenu;
    TBOOL    isLanguage = eFALSE;
    //TBOOL    isUOM = eFALSE;
        
    TUSIGN8 EngUnitsFlag = LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.engUnitsFlag;
    if (EngUnitsFlag == USE_NEXT_ATTRIBUTE)
    {
        TUSIGN8     unitTable = EngUnitsFlag;

        TUSIGN16 getResult = AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.ssIdx,
                                                  (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.objIdx,
                                                  (TUSIGN8)(LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.attribIdx + 1),
                                                  (void FAST *)&unitTable);

        if ((unitTable < USE_UOM_1) || (unitTable >= USE_NEXT_ATTRIBUTE) || (getResult != (TUSIGN16)OK))
        {
            EngUnitsFlag = UOM_NOT_APPLICABLE;
        }
        else
        {
            EngUnitsFlag = unitTable;
        }
    }
    
    if ((LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.ssIdx==HMI_IDX)
     && (LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.objIdx==HMI_IDX_language))
    {
        isLanguage = eTRUE;
    }    

    switch (SFD_SIMPLE_CTW.MenuFormat)
    {
        case HMI_MENU_FORMAT_3_LINES:   itemsPerMenu = 3;   break;
        case HMI_MENU_FORMAT_4_LINES:   itemsPerMenu = 4;   break;
        case HMI_MENU_FORMAT_5_LINES:   itemsPerMenu = 5;   break;
        case HMI_MENU_FORMAT_6_LINES:   itemsPerMenu = 6;   break;
        default:                    itemsPerMenu = 4;   break;
    }
    
    //----define the itemsPerMenu
    if((testLanguage==HMI_LANGUAGE_CHINESE))
    {
        if (EngUnitsFlag == UOM_NOT_APPLICABLE)
        {
            itemsPerMenu = 2;
        }
        else
        {
            if (itemsPerMenu==3)  
            { 
                itemsPerMenu = 2;
            }
            else
            { 
                itemsPerMenu = 4; 
            }
        }
    }
    else
    {        
        if (isLanguage)
        {
            itemsPerMenu = 3;
        }  
    }

    //2.----Get Details of what is to be displayed
    pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.ssIdx);
    (void)pSub->GetAttributeDescription(pSub,
                                        (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.objIdx,
                                        (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.value.attribIdx,
                                        &Range);


    eValOffset = 0;

#ifdef HMI_SUPPORTS_16BIT_ENUMS
    if (nBits == 16)
    {
        numberOfCodes = Range.range.e16.numberOfCodes;
        while ((eVal != Range.range.e16.ptrCodeTab[eValOffset]) && (eValOffset < numberOfCodes))
        {
            eValOffset ++;
        }
    }
    else
#endif
    {
        numberOfCodes = Range.range.e8.numberOfCodes;
        while ((eVal != Range.range.e8.ptrCodeTab[eValOffset]) && (eValOffset < numberOfCodes))
        {
                eValOffset ++;
        }
    }
        
    if (packetStage == FRAME_STAGE_STATIC)
    {
        // update static area flag is set - current enum value in not being displayed
        eValOffsetShown = eFALSE;
    }
    else
    {
        // update static area flag is not set - Check if the current enum value is being displayed
        eValOffsetShown = eFALSE;
        i = 0;
        while ((eValOffsetShown == eFALSE) && (i < itemsPerMenu))
        {
            if (DisplayedValues[i] == eValOffset)
            {
                eValOffsetShown = eTRUE;
                LOCAL_enumCursor = i;
                break;
            }
            else
            {
                i++;
            }
        }
    }

    
    if (!eValOffsetShown)
    {
        // current enum value is not being displayed
        // Get Details of what to display
        TUSIGN16 rangeIndex = eValOffset;

		if ((packetStage == FRAME_STAGE_STATIC) || (rangeIndex >= numberOfCodes))
		{
			// update static area flag is set, or current enum value is not valid (not found in ptrCodeTab) ....
			// .... build a new display from scratch
			if (rangeIndex >= numberOfCodes)
			{
				// (rangeIndex >= Range.range.e8.numberOfCodes) - current value is not valid
				LOCAL_enumCursor = 0;                           // set the cursor to the first line
				rangeIndex = 0;                                 // set the first item to be displayed as the first enum value
			}
			else if (rangeIndex < itemsPerMenu)
			{
				// (rangeIndex < itemsPerMenu) - current value is within the first few items
				LOCAL_enumCursor = rangeIndex;                  // set the cursor to the selected value
				rangeIndex = 0;                                 // set the first item to be displayed as the first enum value
			}
			else
			{
				// (rangeIndex >= itemsPerMenu) - current value is beyond the end of display (if displaying the first few enum values)
				LOCAL_enumCursor = (itemsPerMenu - 1);          // set the cursor to the last line
				rangeIndex = (rangeIndex - itemsPerMenu) + 1;   // set the first item to be displayed so that the last item to be displayed is the selected value
			}
		}
		else
		{
			// update static area flag is not set and current enum value is valid ....
			// .... build a new display based on what was previously displayed
        if (eValOffset < DisplayedValues[0])
			{
				// new value is less than previous lowest displayed value - build a new display with selected value at top
				LOCAL_enumCursor = 0;                           // set the cursor to the first line
				rangeIndex = rangeIndex;                        // do not change range index so that the first item to be displayed is the selected value
			}
			else
			{
				// new value must be more than previous highest displayed value - build a new display with selected value at bottom
				LOCAL_enumCursor = (itemsPerMenu - 1);          // set the cursor to the last line
				rangeIndex = (rangeIndex - itemsPerMenu) + 1;   // set the first item to be displayed so that the last item to be displayed is the selected value
			}
		}

		// Fill the displayed values array
        for (i=0; i<itemsPerMenu; i++)
        {
            if (rangeIndex < numberOfCodes)
            {
                DisplayedValues[i] = rangeIndex;
            }
            rangeIndex ++;
        }
    }


    if(packetStage == FRAME_STAGE_STATIC)
    {
        TUSIGN8 titleLength = 0;
        E_STRING_TYPE stringType;        
        TUSIGN8 const *pTitle;

        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.title_attribidx;
        
        pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {  
            titleLength = 2;
        }
        else
        {
            for(titleLength=0; titleLength<MAX_LENGTH_FRAME_TITLE; titleLength++)
            {
                if(pTitle[titleLength] == '\0')
                {
                    break;
                }
            }           
        }        
        LOCAL_packet_HL[5] = (stringType<<5)| titleLength;

        LOCAL_packet_HL[0] = FRAME_ENUM_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) + titleLength + 4;
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = itemsPerMenu;
        LOCAL_packet_HL[4] = isLanguage;
        memcpy(&LOCAL_packet_HL[6], pTitle, titleLength);
        if(AddPacketToBuffer(titleLength+6, 0))
        {
            packetStage = FRAME_STAGE_TEXT_1;
            isBuild +=1;
        }          
    }

    if((packetStage>=FRAME_STAGE_TEXT_1)&& (packetStage<=FRAME_STAGE_TEXT_6))
    {         
        do
        {
            TUSIGN8 lineNumber = packetStage - FRAME_STAGE_TEXT_1;
            startPacket = packetStage;
            if ((DisplayedValues[lineNumber] < 0) || (lineNumber >= itemsPerMenu))
            {
                packetStage++;
            }
            else
            {
                TUSIGN16 enumItem;
                const TUSIGN8* pString;
                E_STRING_TYPE stringType;
                TUSIGN8 stringLen;
                TBOOL isInvert = eFALSE;                
                TUSIGN8 disLanguage;

                if(lineNumber==LOCAL_enumCursor)
                {
                    isInvert = eTRUE;
                }
                

                if (EngUnitsFlag == UOM_NOT_APPLICABLE)
                {
                    enumItem =   Range.range.e8.ptrCodeTab[DisplayedValues[lineNumber]]
                             + ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.firstItem_MSByte << 8)
                             + ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.firstItem_LSByte);
                    
                    LOCAL_stringtable_text_id.attribIdx = FrameTableGetEnumItem(enumItem);

                }
                else
                {
                    #ifdef HMI_SUPPORTS_16BIT_ENUMS
                    if (nBits == 16)
                    {
                        enumItem = Range.range.e16.ptrCodeTab[DisplayedValues[lineNumber]];
                    }
                    else
                    #endif
                    {
                        enumItem = Range.range.e8.ptrCodeTab[DisplayedValues[lineNumber]];
                    }
                    LOCAL_stringtable_text_id.attribIdx = enumItem;
                }

                LOCAL_stringtable_text_id.objIdx = SS_HMI_OBJ_STRINGTABLE - EngUnitsFlag;
                LOCAL_stringtable_text_id.ssIdx = HMI_IDX;
                
                disLanguage = testLanguage; 
                if(isLanguage)
                {                    
                    pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, HMI_LANGUAGE_ENGLISH);
                    if(stringType!=STRING_TYPE_LIBIDX)
                    {
                        if(memcmp(pString, "English", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_ENGLISH;
                        }
                        else if(memcmp(pString, "German", 6)==0)
                        {
                            disLanguage = HMI_LANGUAGE_GERMAN;
                        }
                        else if(memcmp(pString, "French", 6)==0)
                        {
                            disLanguage = HMI_LANGUAGE_FRENCH;
                        }
                        else if(memcmp(pString, "Spanish", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_SPANISH;
                        }
                        else if(memcmp(pString, "Italian", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_ITALIAN;
                        }
                        else if(memcmp(pString, "Dutch", 5)==0)
                        {
                            disLanguage = HMI_LANGUAGE_DUTCH;
                        }
                        else if(memcmp(pString, "Danish", 6)==0)
                        {
                            disLanguage = HMI_LANGUAGE_DANISH;
                        }
                        else if(memcmp(pString, "Swedish", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_SWEDISH;
                        }
                        else if(memcmp(pString, "Finnish", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_FINNISH;
                        }
                        else if(memcmp(pString, "Polish", 6)==0)
                        {
                            disLanguage = HMI_LANGUAGE_POLISH;
                        }
                        else if(memcmp(pString, "Russian", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_RUSSIAN;
                        }
                        else if(memcmp(pString, "Chinese", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_CHINESE;
                        }
                        else if(memcmp(pString, "Japanese", 8)==0)
                        {
                            disLanguage = HMI_LANGUAGE_JAPANESE;
                        }
                        else if(memcmp(pString, "Turkish", 7)==0)
                        {
                            disLanguage = HMI_LANGUAGE_TURKISH;
                        }
                        else if(memcmp(pString, "Portuguese", 10)==0)
                        {
                            disLanguage = HMI_LANGUAGE_PORTUGUESE;
                        }
                        else
                        {
                            disLanguage = HMI_LANGUAGE_ENGLISH;
                        }
                    }

                }
                
                pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, disLanguage);
                if(stringType==STRING_TYPE_LIBIDX)
                {
                    stringLen = 2;
                }
                else
                {
                    for(i=0; i<MAX_LENGTH_VALUE; i++)
                    {
                        if(pString[i]==0)
                        {
                            break;
                        }
                    }
                    stringLen = i;                    
                }
                LOCAL_packet_HL[3] = (stringType<<5)| stringLen;

                LOCAL_packet_HL[0] = FRAME_ENUM_EDIT;
                LOCAL_packet_HL[1] = (packetStage<<5) + 2 + stringLen;
                LOCAL_packet_HL[2] = isInvert;
                memcpy(&LOCAL_packet_HL[4], pString, stringLen);
                if(AddPacketToBuffer(4+stringLen, 0))
                {
                    packetStage++;
                    isBuild +=1;
                }
                else
                {
                    break;
                }
            }
        }while ((packetStage != startPacket) && (packetStage <= FRAME_STAGE_TEXT_6));
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1;
        }        
    }
    return isBuild;
}

#endif//end of "#if HMI_SUPPORTS_ENUM_PARAM_FRAMES"

//static TUSIGN8 lastBarPosition = 0;
#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)
// No restriction on the optimization of this function

TUSIGN8 BuildSFrame_Adjust(void)
{
    //static TUSIGN8 s_preDisplayLang = HMI_LANGUAGE_NOT_DEFINED;
    TUSIGN8 isBuild =0;
    //TUSIGN8* pData;
    //TUSIGN16 i;
    
    TBOOL isAutoAdjustment = eFALSE; //manual_adjust
    static TUSIGN8 lastBarPosition = 0;


#if (!TEST_FRAME_ADJUST)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif


    if(LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_AUTO_ADJUST)
    {
        isAutoAdjustment = eTRUE;// auto adjust
    }
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //Icon IDs , Access privilage, Frmae title
        FRAME_STAGE_OPERATION_1,  //Parameter Title 
        FRAME_STAGE_OPERATION_2,  //Engineering Units - active//,Failure message,BAR Graph Value, and  param value - keypress
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
    
    /* This part code used to chose which frame to choose */
	if (LOCAL_updateStaticArea)
	{
		packetStage = FRAME_STAGE_STATIC;      /* If the whole frame need to update, we will start from slider bar, please refer to original code in the PackManagerBuilder.c*/
	}
//	else if ((LOCAL_updateActiveArea) && (packetStage >= FRAME_STAGE_OPERATION_2))
//	{
//        if (AdjusterIsDataAdjusted() != 0)
//        {
//            packetStage = FRAME_STAGE_OPERATION_2;  /* If only active area need to update, we will start from button */
//        }
//	}
    else if ((LOCAL_keypressProcessed) && (LOCAL_packetStage > FRAME_STAGE_OPERATION_2))
    {
            packetStage = FRAME_STAGE_OPERATION_2;
    }
    else if ((isAutoAdjustment == eTRUE) && (LOCAL_packetStage > FRAME_STAGE_OPERATION_2) && (AdjusterIsDataAdjusted() != 0))
    {
            packetStage = FRAME_STAGE_OPERATION_2;
    }
	else
	{
		asm("nop");
	}


    if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
	{
		packetStage =  FRAME_STAGE_STATIC;  			/*  If system language changed, we have to chenge the whole frame */
		LOCAL_updateStaticArea = eFALSE;
	}
	else if(packetStage>=FRAME_STAGE_NULL)
	{
		return eFALSE;
	}
    
    LOCAL_packetBufferUsed = 0;
    ATTRIB_IDENTIFIER_TYPE valueID;
    valueID.ssIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx;
    valueID.objIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx;
    valueID.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx;

    if(packetStage==FRAME_STAGE_STATIC)
    {         
        TUSIGN8 stringLen= 0;
        TUSIGN8 left_button;
        const TUSIGN8* pTitle;
        E_STRING_TYPE stringType;

       
#ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_pCurrDisplayFrame->frameDefinition.enumParamFrame.easySetupFlag != 0)
        {
            left_button = 'N'; //Next
        }
        else
#endif
        {
            if(isAutoAdjustment == eTRUE)
            {
                left_button = 'B';   //  back
            }
            else
            {
                left_button = 'C';   //  Cancel
            }
        }
        
                
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.titleBar;
        #if MAX_STRINGS > 255
        LOCAL_stringtable_text_id.attribIdx += ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.titleBar_HighByte * 256);
        #endif
        pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {

            for(stringLen=0; stringLen<MAX_LENGTH_FRAME_TITLE; stringLen++)
            {
                if(pTitle[stringLen] == '\0')
                {
                    break;
                }
            }
        }
        LOCAL_packet_HL[7] = (stringType<<5)|stringLen;
        LOCAL_packet_HL[0] = FRAME_ADJUEST;
        LOCAL_packet_HL[1] = (packetStage<<5) + stringLen + 6;
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = WriteProtect_HMICanAccess(valueID, 0);  
        LOCAL_packet_HL[4] = isAutoAdjustment;
        LOCAL_packet_HL[5] = 0xFF;
        LOCAL_packet_HL[6] = left_button;

        memcpy(&LOCAL_packet_HL[8], pTitle, stringLen);
        if(AddPacketToBuffer(stringLen+8, 0))
        {
            isBuild +=1;
            packetStage = FRAME_STAGE_OPERATION_1;
        }            
    }


    // Parameter Title
    if(packetStage==FRAME_STAGE_OPERATION_1)
    {
        const TUSIGN8* pString;
        TUSIGN8 stringLen = 0;
        E_STRING_TYPE stringType;


        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.title;
        #if MAX_STRINGS > 255
        LOCAL_stringtable_text_id.attribIdx += ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.title_HighByte * 256);
        #endif
        pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<MAX_LENGTH_PARAM_NAME; stringLen++)
            {
                if(pString[stringLen] == '\0')
                {
                    break;
                }
            }
        }
        LOCAL_packet_HL[2] = (stringType<<5)|stringLen;
        

        LOCAL_packet_HL[0] = FRAME_ADJUEST;
        LOCAL_packet_HL[1] = (packetStage<<5) + stringLen + 1 ;
        memcpy(&LOCAL_packet_HL[3], pString, stringLen);

        if(AddPacketToBuffer(stringLen+3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild +=1;
        }   
        
        
    }
    
    //Engineering Units - active
    // Eng Units, Bar Value and Param Value - key press
    if(packetStage==FRAME_STAGE_OPERATION_2)
    {
        
        // Get ENG units and Add to frame
        TUSIGN16 i;
        TUSIGN8 units[15] = {0};
        TUSIGN8* pUnit = NULL;
        TUSIGN8 text[15] = {0x00};
        TUSIGN8 ValueLength;
        TUSIGN8 *pValue = NULL;
        TUSIGN16 errorCode = 0;
        E_STRING_TYPE unitStrType;
        // Return value of "strcpy()" ignored
        //lint -e{534} Roger Arnold 2005-2-01
        pUnit = (TUSIGN8*)PacketManagerGetEngUnits_s((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.engUnits.ssIdx,
                                                     LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.engUnits.objIdx,
                                                     LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.engUnits.attribIdx,
                                                     LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.engUnitsType,
                                                     &unitStrType);
        for(i=0; i<8; i++)
        {
            if(pUnit[i]=='\0')
            {
                break;
            }
        }
        if(i>0)
        {          
            memcpy((char *)units, pUnit, i);
        }

#ifdef HMI_SUPPORTS_CUSTOM_UNITS
        PacketManagerRemoveCustomfromUnits(units);
#endif
        LimitTextLength(units,7);
             
       // Get Bar Graph/Contrast Value and Add to frame
        
        if ((LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx == HMI_IDX)
         && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx == HMI_IDX_contrast)
         && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx == 0))
        {
            LOCAL_packet_HL[3] = SFD_SIMPLE.contrast;
        }
        else
        {
            //failure message
            LOCAL_packet_HL[3] = IDS_OK;  //IDS_OK  8
            if ((AdjusterGetLimitError(&errorCode)) && (LOCAL_barPosition == 0))
            {
                 LOCAL_packet_HL[3] = IDS_ERROR; //IDS_ERROR 9
            }
            else
            { 
                LOCAL_packet_HL[3] = IDS_CLEAR_ERROR; //IDS_CLEAR_OK 10            
            }
          
        }
        
            // Set Local bar position and ParamValue
        //manual adjust
        if(!isAutoAdjustment)

          {
                TUSIGN8 *pNumber;
                TUSIGN8 negNumber;
                
                TUSIGN8 within_over_under;
         
                ADJUST_DATA tVal, tHigh, tLow, tInc;
                TUSIGN8 tType;
//                T_RANGE_E8 enumRange;
                TBOOL rulesAreActive;

                if (AdjusterGetFloatValueAndLimits((TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx), //
                                                   (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx),//  
                                                   LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx,
                                                   &tVal,
                                                   &tHigh,
                                                   &tLow,
                                                   &tInc,
                                                   &rulesAreActive,
                                                   &tType) == OK)
                {

                    #ifdef HMI_USES_DOUBLE
                    if (tType == ST_DOUBLE)
                    {
                        pNumber = AdjusterBuildNumberStringFromDouble(tVal.data_D,
                                                                      LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.maxCharacters,
                                                                      LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.characterSet_dpPosn,
                                                                      &negNumber,
                                                                      &within_over_under);
                        if (fabs(tLow.data_D - tHigh.data_D) == 0.0)
                        {
                            if (tVal.data_D > tHigh.data_D)
                            {
                                LOCAL_barPosition = (ADJUST_BAR_MAX -1);
                            }
                            else
                            {
                                LOCAL_barPosition = 0;
                            }
                        }
                        else
                        {
                            LOCAL_barPosition = (TUSIGN8)((((tVal.data_D - tLow.data_D)/(tHigh.data_D - tLow.data_D)) * (TDOUBLE)(ADJUST_BAR_MAX -1)) + 0.5);
                        }
                    }
                    else
                    #endif
                    {
                         pNumber = AdjusterBuildNumberString(tVal.data_F,
                                                            LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.maxCharacters,
                                                            LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.characterSet_dpPosn,
                                                            &negNumber,
                                                            &within_over_under);
                         
                        if ((TFLOAT)(fabs((TDOUBLE)(tLow.data_F - tHigh.data_F))) == 0.0f)
                        {
                            if (tVal.data_F > tHigh.data_F)
                            {
                                LOCAL_barPosition = (ADJUST_BAR_MAX -1);
                            }
                            else
                            {
                                LOCAL_barPosition = 0;
                            }
                        }
                        else
                        {
                            LOCAL_barPosition = (TUSIGN8)((((tVal.data_F - tLow.data_F)/(tHigh.data_F - tLow.data_F)) * (TFLOAT)(ADJUST_BAR_MAX -1)) + 0.5F);
                        }
                    }
                    

                    if ((within_over_under == 1) || (within_over_under == 2))
                    {
                        // overrange or underrange
                        TUSIGN8 i;

                        if(tVal.data_F==100)
                        {
                            text[0] = '1';
                            text[1] = '0';
                            text[2] = '0';
                            text[3] = 0;
                        }
                        else
                        {
                            text[0] = 0x20;
                            for (i = 1; i <= LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.maxCharacters; )
                            {
                                text[i] = 0x05 + within_over_under;
                                i++;
                                text[i] = 0x00;
                            }
                        }
                    }
                    else
                    {
                        while (*pNumber == 0x20)
                        {
                            #pragma diag_suppress=Pm088
                            pNumber ++;
                            #pragma diag_default=Pm088
                        }

                        text[0] = negNumber ? '-' : ' ';
                        text[1] = 0x00;
                        for(i=1; i<10; i++)
                        {
                            if(pNumber[i-1]==0)
                            {
                                break;                                
                            }
                            text[i] = pNumber[i-1];                            
                        }
                    }
                    
                }
                else
                {
                    // failed to read value - display "..."
                    text[0] = 0x20;
                    for (i = 1; i <= LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.attributes.maxCharacters; )
                    {
                        text[i] = 0x09;
                        i ++;
                        text[i] = 0x00;
                    }
                    LOCAL_barPosition = 0;
                } 
                
            pValue = text;
            for(ValueLength=0;ValueLength<8;ValueLength++)
            {
                if(pValue[ValueLength] == '\0')
                {
                    break;
                }
            }
                
        }
        //auto adjust
        else
        {
          if (AdjusterIsDataAdjusted() == 0)
            {
                LOCAL_barPosition = 0;
            }
            else
            {
                
                    LOCAL_barPosition += (ADJUST_BAR_MAX / 35);
                    if (LOCAL_barPosition > ADJUST_BAR_MAX)
                    {
                        LOCAL_barPosition = 0;
                        AdjusterResetDataAdjusted();
                    }
                
            }
            ValueLength =0;
        }

        if(((LOCAL_barPosition-lastBarPosition) >= 6)
        || (LOCAL_barPosition>=ADJUST_BAR_MAX)
        || (LOCAL_barPosition==0)
        || (!isAutoAdjustment))
        {
            lastBarPosition = LOCAL_barPosition;
            LOCAL_packet_HL[0] = FRAME_ADJUEST;
            LOCAL_packet_HL[1] = (packetStage<<5) +  11 + ValueLength;
            LOCAL_packet_HL[2] = LOCAL_barPosition;
            LOCAL_packet_HL[4] = (unitStrType<<5)+7;
            memcpy(&LOCAL_packet_HL[5], units, 7);
            LOCAL_packet_HL[12] = ValueLength;
            memcpy(&LOCAL_packet_HL[13], text, ValueLength);         
            if(AddPacketToBuffer(13+ValueLength, 0))
            {
                packetStage = FRAME_STAGE_END;
                isBuild +=1;
            } 
        }
        
    }
    

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild +=1 ;
        }        
    }
    
    return isBuild;

}

#endif //end of "#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)"


#ifdef HMI_SUPPORTS_DISPLAY_TEST
// No restriction on the optimization of this function
TUSIGN8 BuildSFrame_DisplayTest(void)
{
    static TUSIGN8 s_statusBar = 0;
    static TUSIGN16 s_counter = 0;
    TUSIGN8 isBuild = 0;
    
#if (!TEST_FRAME_DISPLAY_TEST)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        //FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
        
       
    if ((testLanguage != preDisplayLang)
     ||  LOCAL_updateStaticArea) //update all
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LOCAL_packetStage = FRAME_STAGE_STATIC;
        LOCAL_updateStaticArea = 0;
        s_statusBar = 0;
        s_counter = 0;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        s_counter++;
        if(s_counter>=10)
        {
            s_counter = 0;
        }
        if((AdjusterIsDataAdjusted() != 0)
            && (s_counter==1))
        {//go to test stage
            packetStage =  FRAME_STAGE_STATIC;  
            LOCAL_packetStage = FRAME_STAGE_STATIC;
            s_statusBar++;
            if(s_statusBar>=12)
            {
                s_statusBar = 0;                
                AdjusterResetDataAdjusted();
            }
        }
        else
        {
            return 0;
        }
    }
    
    
    if(packetStage==FRAME_STAGE_STATIC)
    {       
        E_STRING_TYPE stringType;
        const TUSIGN8* pTitleBar;
        TUSIGN8 titleBarLength;
        ATTRIB_IDENTIFIER_TYPE valueID;

        valueID.ssIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx;
        valueID.objIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx;
        valueID.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx;

        if (WriteProtect_HMICanAccess(valueID, 0))
        {
            LOCAL_packet_HL[4] = eTRUE;
        }
        else
        {
            LOCAL_packet_HL[4] = 0;
        }

        if(s_statusBar==0)
        {
            LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.titleBar;
            #if MAX_STRINGS > 255
            LOCAL_stringtable_text_id.attribIdx += ((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.titleBar_HighByte * 256);
            #endif
            
            pTitleBar = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);   
            if(stringType==STRING_TYPE_LIBIDX)
            {
                titleBarLength = 2;
            }
            else
            {
                for(titleBarLength=0; titleBarLength<MAX_LENGTH_FRAME_TITLE; titleBarLength++)
                {
                    if(pTitleBar[titleBarLength] == '\0')
                    {
                        break;
                    }
                }
            } 
            LOCAL_packet_HL[5] = (stringType<<5)|titleBarLength;

            if(titleBarLength>0)
            {
                memcpy(&LOCAL_packet_HL[6], pTitleBar, titleBarLength);
            }
        }
        else
        {
            LOCAL_packet_HL[5] = 0;
            titleBarLength = 0;
        }
        
        
        LOCAL_packet_HL[0] = FRAME_DISPLAY_TEST;
        LOCAL_packet_HL[1] = (packetStage<<5) | (4+titleBarLength);                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = s_statusBar;
        
        
        if(AddPacketToBuffer(6+titleBarLength, 0))
        {            
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }          
    }


    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;

}
#endif

#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES

#define LIN_CURRENTLY_EDITING   ((currLinearizerCursorPos == LINEARIZER_CURSOR_INPOINT_EDIT) || (currLinearizerCursorPos == LINEARIZER_CURSOR_OUTPOINT_EDIT))
#define LIN_PREVIOUSLY_EDITING  ((prvLinearizerCursorPos[LinearizerInstance] == LINEARIZER_CURSOR_INPOINT_EDIT) || (prvLinearizerCursorPos[LinearizerInstance] == LINEARIZER_CURSOR_OUTPOINT_EDIT))

#define LIN_CURRENTLY_VIEWING   ((currLinearizerCursorPos != LINEARIZER_CURSOR_INPOINT_EDIT) && (currLinearizerCursorPos != LINEARIZER_CURSOR_OUTPOINT_EDIT))
#define LIN_PREVIOUSLY_VIEWING  ((prvLinearizerCursorPos[LinearizerInstance] != LINEARIZER_CURSOR_INPOINT_EDIT) && (prvLinearizerCursorPos[LinearizerInstance] != LINEARIZER_CURSOR_OUTPOINT_EDIT))

#define LIN_ACKNOWLEDGE_CURSOR_CHANGE   prvLinearizerCursorPos[LinearizerInstance] = currLinearizerCursorPos


#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
TUSIGN8 BuildSFrame_Linearizer(void)
{
#if (!TEST_FRAME_LINEAR)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif

    typedef enum
    {                                               // Packet                                  |
                                                    //=========================================|
        LINEARIZER_FRAME_TITLE_BAR,                 // Title Bar (View or Edit)                | 0 - Start of Static Area (LINEARIZER EDIT) or (LINEARIZER VIEW)
                                                    //-----------------------------------------|    Start from here if LINEARIZER EDIT / LINEARIZER VIEW changed
        LINEARIZER_FRAME_TITLE_ICON,                // Title Bar Icon (View or Edit)           | 1
                                                    //-----------------------------------------|
        LINEARIZER_FRAME_LEFT_KEY,                  //                                         | 2
                                                    //=========================================|
        LINEARIZER_VIEW_INPOINT_ID,                 // ID of Input point data                  | 3 - Start of Keypress Area (LINEARIZER VIEW)
                                                    //-----------------------------------------|     Start from here if cursor changed (LINEARIZER VIEW)
        LINEARIZER_VIEW_INPOINT_ID_HIGHLIGHT,       //                                         |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_OUTPOINT_ID,                // ID of Output point                      |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_OUTPOINT_ID_HIGHLIGHT,      //                                         |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_SELECTED_POINT,             //                                         |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_SELECTED_POINT_HIGHLIGHT_4, // Add'l highlight on selected point       |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_SELECTED_POINT_HIGHLIGHT_5, // Add'l highlight on selected point       |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_PV_ID,                      // ID of Process value                     |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_PV_VALUE,                   // value of Process value                  | 8 - Start of active Area (LINEARIZER VIEW)
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_PV_UNITS,                   // units for Process value                 |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_INPOINT_VALUE,              // value of I/P point data                 | 9
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_OUPOINT_VALUE,              // value of O/P point data                 | 10
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_INPOINT_UNITS,              // units for I/P point data                |
                                                    //-----------------------------------------|
        LINEARIZER_VIEW_OUPOINT_UNITS,              // units for O/P point data                |
                                                    //-----------------------------------------|
        LINEARIZER_FRAME_CENTRE_KEY_ICON,           //                                         | 18
                                                    //-----------------------------------------|
        LINEARIZER_FRAME_CENTRE_KEY_TEXT,           //                                         | 17
                                                    //=========================================|
        LINEARIZER_EDIT_UNITS,                      // Engineering Units                       | 11 --Start of (LINEARIZER EDIT)
                                                    //-----------------------------------------|
        LINEARIZER_EDIT_MAX,                        // High Limit                              | 12
                                                    //-----------------------------------------|
        LINEARIZER_EDIT_MIN,                        // Low Limit                               | 13
                                                    //-----------------------------------------|
        LINEARIZER_EDIT_MAX_ICON,                   // High Limit icon or text                 | 14
                                                    //-----------------------------------------|
        LINEARIZER_EDIT_MIN_ICON,                   // Low Limit icon or text                  | 15
                                                    //-----------------------------------------|
        LINEARIZER_EDIT_VALUE,                      // Parameter value                         | 16 - Start of Keypress Area (LINEARIZER EDIT)
                                                    //=========================================|
        LINEARIZER_FRAME_RIGHT_KEY,                 //                                         | 19
                                                    //=========================================|
        LINEARIZER_FRAME_READ_SWITCHES              //                                         | 20
                                                    //-----------------------------------------| - There is no active area (LINEARIZER EDIT) or (LINEARIZER VIEW)
    }LINEARIZER_FRAME_PACKET;

    TUSIGN8 isBuild = 0;
    TUSIGN8 cursorSel;

    // General data
    TUSIGN8 text[23];
    TUSIGN8 LinearizerInstance = 0;
    //E_STRING_TYPE unitStrType;
    //TUSIGN8 unitLen;
    //TUSIGN8 units[10] = {0};

    #ifdef HMI_SUPPORTS_SERVICEPORT
    LinearizerInstance = PacketManagerIsRemoteInstance() ? 1 : 0;
    #endif

    // LINEARIZER SUBSYSTEM DATA
    TUSIGN16 LinearizerSubsystem = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.linearizer_subsystem_index);
    TUSIGN16 InPointObject       = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.inpoint_object_index);
    TUSIGN16 OutPointObject      = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.outpoint_object_index);
    TUSIGN16 PointEnableObject   = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.pointEnable_objectIndex);
    const T_UNIT *pLinearizer    = GetSubsystemPtr(LinearizerSubsystem);


    // CURSOR DATA
    static LINEARIZER_CURSOR_POS prvLinearizerCursorPos[2] = {LINEARIZER_CURSOR_EXIT,LINEARIZER_CURSOR_EXIT};
    LINEARIZER_CURSOR_POS currLinearizerCursorPos = FrameTable_GetLinearizerCursor();
    TBOOL EditView_changed = ((LIN_CURRENTLY_EDITING && LIN_PREVIOUSLY_VIEWING) || (LIN_CURRENTLY_VIEWING && LIN_PREVIOUSLY_EDITING)) ? eTRUE : eFALSE;
    TBOOL Cursor_changed   = (currLinearizerCursorPos != prvLinearizerCursorPos[LinearizerInstance]) ? eTRUE : eFALSE;

    // LINEARIZER POINT
    TUSIGN8 LinPointSelected = FrameTable_GetLinearizerPoint();
    TUSIGN8 s_Point_selected[3] = {0x3F,0x3F,0x00};
    if (LinPointSelected <= 99)
    {
        s_Point_selected[0] = (LinPointSelected / 10) + 0x30;
        s_Point_selected[1] = (LinPointSelected % 10) + 0x30;
    }

    // DATA SPECIFIC TO EDIT OPERATION
    TFLOAT  maxValue = 0;
    TFLOAT  minValue = 0;
    TUSIGN8 min_max_DP = 0;
    TBOOL   min_max_rules = 0;
    TUSIGN8 LimitError = 0;
    TUSIGN16 errorCode = 0;

    TUSIGN8 inpointLine = 4;
    TUSIGN8 outpointLine = 5;
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_VIEW_KEYPRESS,
        FRAME_STAGE_OPERATION_1=3,
        FRAME_STAGE_OPERATION_2=4,
        FRAME_STAGE_OPERATION_3=5,
        FRAME_STAGE_END=7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;
    
    
    typedef enum
    {
        FRAME_STAGE_EDIT = 9,
        FRAME_STAGE_EDIT_KEYPRESS=12,
    }FRAME_STAGE2;
    
    TUSIGN8 VIEW_PV_VALUE[15] = {0};
    TUSIGN8 VIEW_PV_VALUE_LENGTH = 0;

    TUSIGN8 VIEW_PV_UNITS[15] = {0};
    TUSIGN8 VIEW_PV_UNITS_LENGTH = 0;
    E_STRING_TYPE VIEW_PV_UNITS_TYPE;

    TUSIGN8 VIEW_INPOINT_VALUE[15] = {0};
    TUSIGN8 VIEW_INPOINT_VALUE_LENGTH = 0;

    TUSIGN8 VIEW_INPOINT_UNITS[15] = {0};
    TUSIGN8 VIEW_INPOINT_UNITS_LENGTH = 0;
    E_STRING_TYPE VIEW_INPOINT_UNITS_TYPE;

    TUSIGN8 VIEW_OUPOINT_VALUE[15] = {0};
    TUSIGN8 VIEW_OUPOINT_VALUE_LENGTH = 0;

    TUSIGN8 VIEW_OUPOINT_UNITS[15] = {0};
    TUSIGN8 VIEW_OUPOINT_UNITS_LENGTH = 0;
    //E_STRING_TYPE VIEW_OUPOINT_UNITS_TYPE;
    
    TUSIGN8 Defaultarray[2] = {0x00,0x00};
    TUSIGN8 const *FRAME_CENTRE_KEY_TEXT = Defaultarray;
    TUSIGN8 FRAME_CENTRE_KEY_ICON = 0;
    TUSIGN8 FRAME_CENTRE_KEY_LENGTH = 0;    
    TUSIGN8 FRAME_CENTRE_KEY_IDENT = 0;
    
    if ((FrameTable_isLinearizerProcessValueDisplayEnabled() == eFALSE)
     || (LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.ProcessSignal == 0xFF))
    {
        inpointLine  --;
        outpointLine --;
    }
  
    static TUSIGN8 isOperation1 = 0x01;
    static TUSIGN8 isOperation2 = 0x01;
    static TUSIGN8 isEndframe   = 0x01;
    
    
     /*---------Grace defined frame stage choose ------------*/
    
    cursorSel = FrameTableGetCursorPosition();  
    
    
    if ((SFD_SIMPLE.language != preDisplayLang) ||  LOCAL_updateStaticArea || (EditView_changed) || (Cursor_changed) || (LOCAL_keypressProcessed) || LOCAL_updateActiveArea) 
    {
        packetStage =  FRAME_STAGE_STATIC;  
        LIN_ACKNOWLEDGE_CURSOR_CHANGE;

        if (LIN_CURRENTLY_EDITING)
        {
            if ((LOCAL_keypressProcessed) && (LOCAL_packetStage > LINEARIZER_EDIT_VALUE) || LOCAL_updateActiveArea)
            {
                packetStage =  FRAME_STAGE_STATIC;  
                LIN_ACKNOWLEDGE_CURSOR_CHANGE;
            }
        } 
        LOCAL_updateStaticArea =eFALSE;
    }
    
    if(!isOperation1)
    {
        packetStage = FRAME_STAGE_OPERATION_1;
    }
    if(isOperation1 && (!isOperation2))
    {
        packetStage = FRAME_STAGE_OPERATION_2;
    }
    if(isOperation1 && isOperation2 && (!isEndframe))
    {
        packetStage = FRAME_STAGE_END;
    }
    
    if(packetStage==FRAME_STAGE_STATIC)
    {
        TUSIGN8 titlebarLength;
        const TUSIGN8* pTitle;
        
        TUSIGN8 titlebarICON;
        TUSIGN8 leftkey;
        
        /*Title Bar (View or Edit)*/
        if (LIN_CURRENTLY_EDITING)
        {
            if (currLinearizerCursorPos == LINEARIZER_CURSOR_INPOINT_EDIT)
            {
                (void)strcpy((char *)text, "In ");
            }
            else
            {
                (void)strcpy((char *)text, "Out ");
            }
            (void)strcat((char *)text, (char *)s_Point_selected);
            
            
            for(titlebarLength=0;titlebarLength<15;titlebarLength++)
            {
                if(text[titlebarLength] == '\0')
                {
                    break;
                }
            }
            
            LOCAL_packet_HL[5] = (STRING_TYPE_EUROPEAN<<5)|titlebarLength;

            
        }
        else
        {
            E_STRING_TYPE stringType;
            LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.title_attribidx;
            
            pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);            
            if(stringType==STRING_TYPE_LIBIDX)
            {
                titlebarLength = 2;
            }
            else
            {
                for(titlebarLength=0; titlebarLength<14; titlebarLength++)
                {
                    if(pTitle[titlebarLength] == '\0')
                    {
                        break;
                    }
                }
            }
            LOCAL_packet_HL[5] = (stringType<<5)|titlebarLength;
        }
        
        /*Title Bar Icon (View or Edit)*/
        
        if (LIN_CURRENTLY_EDITING)
        {
            titlebarICON = ICON_NUMBER_EDIT;            /*BuildWriteIcon(ICON_NUMBER_EDIT, MAX_COMMON_HMI_WIDTH-SMALL_ICON_WIDTH-1, 0, 0)*/
        }
        else
        {
            /*
            TUSIGN8 linearizerIcon[13] = {0x00, 0xFF, 0xC0, 0xA0, 0x90, 0x88, 0x88, 0x88, 0x88, 0x88, 0x84, 0x82, 0x81};
            packetsBuilt += BuildWriteArray(linearizerIcon, 13, MAX_COMMON_HMI_WIDTH-SMALL_ICON_WIDTH-1, 0, 0);
            */
            titlebarICON = 0x81;
        }
        
        /*LINEARIZER_FRAME_LEFT_KEY*/
        
        if (LIN_CURRENTLY_EDITING)
        {
            /*
            LOCAL_stringtable_text_id.attribIdx = IDS_NEXT;
            packetsBuilt += BuildWriteString(FrameTableGetString(LOCAL_stringtable_text_id),HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE,7,JUSTIFY_LEFT,FONT_0,NO_INVERT,0,0,9,0);
            */
            leftkey = 'N';      /*   'N' means "NEXT" leftbutton   */
        }
        else
        {
            /*
            packetsBuilt += BuildWriteArray(LeftKey_Next, KEY_FIELD_WIDTH, 0, 7, 0);
            */
            leftkey = 'M';      /*     'M' means write LeftKey_Next array */
            
        }
        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5) + titlebarLength + 4;                
        //LOCAL_packet_HL[2] = SFD_SIMPLE.language;
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = titlebarICON; 
        LOCAL_packet_HL[4] = leftkey;
        if(LIN_CURRENTLY_EDITING)
        {
            memcpy(&LOCAL_packet_HL[6],text,titlebarLength);
        }
        else
        {
            memcpy(&LOCAL_packet_HL[6],pTitle,titlebarLength);
        }
        
        if(AddPacketToBuffer(titlebarLength+6, 0))
        {
            preDisplayLang = SFD_SIMPLE.language;
            if(leftkey == 'N')
            {
                packetStage = FRAME_STAGE_EDIT;
            }
            else
            {
                packetStage = FRAME_STAGE_VIEW_KEYPRESS;
            }
            
            isBuild += 1;
        }          
    }
    
    if(packetStage==FRAME_STAGE_VIEW_KEYPRESS && (LIN_CURRENTLY_EDITING == eFALSE))
    {        
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_VIEW_INPOINT_ID,                 // ID of Input point data                  |
        //    LINEARIZER_VIEW_INPOINT_ID_HIGHLIGHT        //                                         |
        //    LINEARIZER_VIEW_OUTPOINT_ID,                // ID of Output point                      |
        //    LINEARIZER_VIEW_OUTPOINT_ID_HIGHLIGHT       //                                         |
        //    LINEARIZER_VIEW_SELECTED_POINT,             //                                         |
        //    LINEARIZER_VIEW_SELECTED_POINT_HIGHLIGHT_4, // Add'l highlight on selected point       |
        //    LINEARIZER_VIEW_SELECTED_POINT_HIGHLIGHT_5, // Add'l highlight on selected point       |
        //    LINEARIZER_VIEW_PV_ID,                      // ID of Process value                     |
        //    LINEARIZER_VIEW_PV_VALUE,                   // value of Process value                  |
        //    LINEARIZER_VIEW_PV_UNITS,                   // units for Process value                 |
        //    LINEARIZER_VIEW_INPOINT_VALUE,              // value of I/P point data                 |
        //    LINEARIZER_VIEW_OUPOINT_VALUE,              // value of O/P point data                 |
        //    LINEARIZER_VIEW_INPOINT_UNITS,              // units for I/P point data                |
        //    LINEARIZER_VIEW_OUPOINT_UNITS,              // units for O/P point data                |
        //    LINEARIZER_FRAME_CENTRE_KEY_ICON,           //                                         |
        //    LINEARIZER_FRAME_CENTRE_KEY_TEXT,           //                                         |
        //------------------------------------------------//-----------------------------------------|

        TUSIGN8 INPUTPIONT_DATA_ID_INVERT;
        TUSIGN8 OUTPUTPIONT_DATA_ID_INVERT;

        
        TUSIGN8 SELECT_POINT[20];
        TUSIGN8 SELECT_POINT_LENGTH;
        TUSIGN8 SELECT_POINT_INVERT;
        
        //TUSIGN8 VIEW_PV_ID[20];
        //TUSIGN8 VIEW_PV_ID_LENGTH;
        
        /*ID of Input point data  */
        (void)strcpy((char *)text, "In ");    
        INPUTPIONT_DATA_ID_INVERT = ((currLinearizerCursorPos == LINEARIZER_CURSOR_INPOINT_VIEW)?STRING_INVERT:NO_INVERT);
        
        /*ID of Output point invert*/
        OUTPUTPIONT_DATA_ID_INVERT = (currLinearizerCursorPos == LINEARIZER_CURSOR_OUTPOINT_VIEW ? STRING_INVERT : NO_INVERT);
        
        /*LINEARIZER_VIEW_SELECTED_POINT*/
        TUSIGN8 invert = currLinearizerCursorPos == LINEARIZER_CURSOR_EXIT ? STRING_INVERT : NO_INVERT;
        (void)strcpy((char *)text, (char *)s_Point_selected);
        
        SELECT_POINT_INVERT = invert;    /*LINEARIZER_VIEW_SELECTED_POINT INVERT*/
        
        for(SELECT_POINT_LENGTH=0;SELECT_POINT_LENGTH<15;SELECT_POINT_LENGTH++)
        {
            if(text[SELECT_POINT_LENGTH] == '\0')
            {
                break;
            }
        }
        
        memcpy(SELECT_POINT,text,SELECT_POINT_LENGTH);

        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5) + SELECT_POINT_LENGTH + 3;                //length
        LOCAL_packet_HL[2] = INPUTPIONT_DATA_ID_INVERT; 
        LOCAL_packet_HL[3] = OUTPUTPIONT_DATA_ID_INVERT;
        LOCAL_packet_HL[4] = SELECT_POINT_INVERT;
        
        memcpy(&LOCAL_packet_HL[5],SELECT_POINT, SELECT_POINT_LENGTH);
        
        if(AddPacketToBuffer(SELECT_POINT_LENGTH+5, 0))
        {
            isBuild += 1;
        }
        

        TUSIGN8 SpecificLanguage;
        const SIGNAL_TABLE_TYPE *pSignal = FrameTableGetSignal(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.ProcessSignal);
              
        LOCAL_stringtable_text_id.attribIdx = pSignal->short_identifier;
        if((testLanguage==HMI_LANGUAGE_CHINESE))
        {
            SpecificLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            SpecificLanguage = testLanguage;
        }        
  
        E_STRING_TYPE stringType;
        TUSIGN8 stringLen;
        TUSIGN8 const *pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, SpecificLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {    
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<3; stringLen++)
            {
                if(pString[stringLen] == '\0')
                {
                    break;
                }
            }
        }
        LOCAL_packet_HL[2] = (stringType<<5)|stringLen;
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5+1) + stringLen + 1;                //length 
        memcpy(&LOCAL_packet_HL[3],pString, stringLen);
   
        if(AddPacketToBuffer(stringLen+3, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_1;
            isBuild += 1;
        }
    }
     
     
    if(packetStage==FRAME_STAGE_OPERATION_1 || packetStage==FRAME_STAGE_OPERATION_2)
    {                   
        /*LINEARIZER_VIEW_PV_VALUE*/
        do
        {
            const SIGNAL_TABLE_TYPE *pSignal = FrameTableGetSignal(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.ProcessSignal);

            ADJUST_DATA dataValue;
            TUSIGN8 dataType;
            TUSIGN8 *pStart;
            TUSIGN8 resolution;
            TUSIGN8 within_over_under, negNumber;

            // -----------------------------------------------------------------------------------------
            // Get the resolution (dec point position) and recalculate resolution if resolution is based upon range
            
            resolution = pSignal->dpPosn;

            if (resolution == USE_RANGE)
            {
                if (pSignal->FixedUpperLower == 0)
                {
                    #ifdef HMI_USES_DOUBLE
                    TDOUBLE High, Low;
                    #else
                    TFLOAT High, Low;
                    #endif
                    PacketManagerBuilderServices_GetBarRange(pSignal, &High, &Low);

                    if (Low < 0)    {Low *= -1;}
                    if (High < 0)   {High *= -1;}
                    if (Low > High) {High = Low;}

                    if (High < 10)          {resolution = 4;}
                    else if (High < 100)    {resolution = 3;}
                    else if (High < 1000)   {resolution = 2;}
                    else if (High < 10000)  {resolution = 1;}
                    else                    {resolution = 0;}
                }
                else
                {
                    resolution = 0;
                }
            }
            
            // -----------------------------------------------------------------------------------------
            // Get the value
            AdjusterGetFloatValue((TUSIGN16)pSignal->value.ssIdx,
                                  (TUSIGN16)pSignal->value.objIdx,
                                  pSignal->value.attribIdx,
                                  &dataValue,
                                  &dataType);

            // -----------------------------------------------------------------------------------------
            // Convert value to string
            #ifdef HMI_USES_DOUBLE
            if (dataType == ST_DOUBLE)
            {
                pStart = AdjusterBuildNumberStringFromDouble(dataValue.data_D,6, resolution, &negNumber, &within_over_under);
            }
            else
            #endif
            {
                pStart = AdjusterBuildNumberString(dataValue.data_F, 7, resolution, &negNumber, &within_over_under);
            }

            if ((within_over_under == 1) || (within_over_under == 2))
            {
                // overrange or underrange
                TUSIGN8 i;
                text[0] = 0x20;

                for (i = 1; i <= 6; )
                {
                    text[i] = 0x05 + within_over_under;
                    i++;
                    text[i] = 0x00;
                }
            }
            else
            {
                text[0] = 0x00;

                // value within range or a normal numerical frame with an edit
                while (*pStart == 0x20)
                {
                    #pragma diag_suppress=Pm088
                    pStart ++;
                    #pragma diag_default=Pm088
                }
                if (negNumber)
                {
                    text[0] = '-';
                    text[1] = 0x00;
                }
                (void)strcat((char *)text,(char *)pStart);
            }
            
            for(VIEW_PV_VALUE_LENGTH=0;VIEW_PV_VALUE_LENGTH<15;VIEW_PV_VALUE_LENGTH++)
            {
                if(text[VIEW_PV_VALUE_LENGTH] == '\0')
                {
                    break;
                }
            }
            
            memcpy(VIEW_PV_VALUE,text,VIEW_PV_VALUE_LENGTH);
       
        }while(0);
        
        /*units for Process value */
        do
        {
            const SIGNAL_TABLE_TYPE *pSignal = FrameTableGetSignal(LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.ProcessSignal);

            strcpy((char *)text,
                       (const char *)PacketManagerGetEngUnits_s((TUSIGN16)pSignal->engUnits.ssIdx,
                                                              pSignal->engUnits.objIdx,
                                                              pSignal->engUnits.attribIdx,
                                                              pSignal->engUnitsType,
                                                              &VIEW_PV_UNITS_TYPE));
            #ifdef HMI_SUPPORTS_CUSTOM_UNITS
            PacketManagerRemoveCustomfromUnits(text);
            #endif

            LimitTextLength(text,8);
            
            for(VIEW_PV_UNITS_LENGTH=0;VIEW_PV_UNITS_LENGTH<8;VIEW_PV_UNITS_LENGTH++)
            {
                if(text[VIEW_PV_UNITS_LENGTH] == '\0')
                {
                    break;
                }
            }
            memcpy(VIEW_PV_UNITS,text,VIEW_PV_UNITS_LENGTH);
            
        }while(0);
        
        
        /*
            value of I/P point data  
            value of O/P point data 
        */
        LOCAL_packetStage = LINEARIZER_VIEW_INPOINT_VALUE;
        if((LOCAL_packetStage >= LINEARIZER_VIEW_INPOINT_VALUE) && (LOCAL_packetStage <= LINEARIZER_VIEW_OUPOINT_VALUE))
        {
            TBOOL pointValid;
            TUSIGN8 startPacket,DataDecPt,lineNumber,valueObjectIndex;

            do
            {
                startPacket = LOCAL_packetStage;

                if (LOCAL_packetStage == LINEARIZER_VIEW_INPOINT_VALUE)
                {
                    lineNumber = inpointLine;

                    valueObjectIndex = InPointObject;
                    DataDecPt = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.dpPosn_inpoint;
                    pointValid = eTRUE;
                }
                else
                {
                    lineNumber = outpointLine;

                    if (FrameTable_GetLinearizerPointEnable(LinearizerSubsystem,PointEnableObject,LinPointSelected))
                    {
                        valueObjectIndex = OutPointObject;
                        DataDecPt = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.dpPosn_outpoint;
                        pointValid = eTRUE;
                    }
                    else
                    {
                        pointValid = eFALSE;
                    }
                }

                if (!pointValid)
                {
                    strcpy((char*)text, " ------");
                }
                
                else
                {
                    TFLOAT  DataValue = 0;

                    if (pLinearizer->Get(pLinearizer,valueObjectIndex,LinPointSelected,(void FAST *)&DataValue) < (TUSIGN16)ERROR_CODES)
                    {
                        TUSIGN8 *pNumber;
                        TUSIGN8 negNumber;
                        TUSIGN8 within_over_under;

                        pNumber = AdjusterBuildNumberString(DataValue, 7, DataDecPt, &negNumber, &within_over_under);

                        if ((within_over_under == 1) || (within_over_under == 2))
                        {
                            // overrange or underrange
                            TUSIGN8 i;
                            text[0] = 0x20;

                            for (i = 1; i <= 6; )
                            {
                                text[i] = 0x05 + within_over_under;
                                i++;
                                text[i] = 0x00;
                            }
                        }
                        else
                        {
                            text[0] = 0x00;

                            // value within range or a normal numerical frame with an edit
                            while (*pNumber == 0x20)
                            {
                                #pragma diag_suppress=Pm088
                                pNumber ++;
                                #pragma diag_default=Pm088
                            }
                            if (negNumber)
                            {
                                text[0] = '-';
                                text[1] = 0x00;
                            }
                            (void)strcat((char *)text,(char *)pNumber);
                        }
                    }
                    else
                    {
                        strcpy((char*)text, " ");
                    }
                }
                
                if(lineNumber == inpointLine)
                {
                    for(VIEW_INPOINT_VALUE_LENGTH=0;VIEW_INPOINT_VALUE_LENGTH<15;VIEW_INPOINT_VALUE_LENGTH++)
                    {
                        if(text[VIEW_INPOINT_VALUE_LENGTH] == '\0')
                        {
                            break;
                        }
                    }
                    memcpy(VIEW_INPOINT_VALUE,text,VIEW_INPOINT_VALUE_LENGTH);
                }
                
                if(lineNumber == outpointLine)
                {
                    for(VIEW_OUPOINT_VALUE_LENGTH=0;VIEW_OUPOINT_VALUE_LENGTH<15;VIEW_OUPOINT_VALUE_LENGTH++)
                    {
                        if(text[VIEW_OUPOINT_VALUE_LENGTH] == '\0')
                        {
                            break;
                        }
                    } 
                    
                    memcpy(VIEW_OUPOINT_VALUE,text,VIEW_OUPOINT_VALUE_LENGTH);
                }
                LOCAL_packetStage++;
            }
            while ((LOCAL_packetStage != startPacket) && (LOCAL_packetStage <= LINEARIZER_VIEW_OUPOINT_VALUE));
        }
        
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_VIEW_INPOINT_UNITS,              // units for I/P point data                |
        //    LINEARIZER_VIEW_OUPOINT_UNITS,              // units for O/P point data                |
        //------------------------------------------------//-----------------------------------------|
        LOCAL_packetStage = LINEARIZER_VIEW_INPOINT_UNITS;
        if ((LOCAL_packetStage >= LINEARIZER_VIEW_INPOINT_UNITS) && (LOCAL_packetStage <= LINEARIZER_VIEW_OUPOINT_UNITS))
        {
            TBOOL pointValid;
            TUSIGN8 startPacket;
            TUSIGN8 unitsType;
            TUSIGN8 UOM_data_L;
            TUSIGN8 UOM_data_H;
            TUSIGN8 lineNumber;

            do
            {
                startPacket = LOCAL_packetStage;

                if (LOCAL_packetStage == LINEARIZER_VIEW_INPOINT_UNITS)
                {
                    lineNumber = inpointLine;
                    unitsType = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_type_inpoint;
                    UOM_data_L = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_inpoint_L;
                    UOM_data_H = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_inpoint_H;
                    pointValid = eTRUE;
                }
                else
                {
                    lineNumber = outpointLine;

                    if (FrameTable_GetLinearizerPointEnable(LinearizerSubsystem,PointEnableObject,LinPointSelected) == eFALSE)
                    {
                        pointValid = eFALSE;
                    }
                    else
                    {
                        unitsType = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_type_outpoint;
                        UOM_data_L = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_outpoint_L;
                        UOM_data_H = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_outpoint_H;
                        pointValid = eTRUE;
                    }
                }

                if (!pointValid)
                {
                    strcpy((char*)text, " ");
                }
                else
                {
                    // add units to "value_and_units"
                    if (unitsType == FIXED_TEXT_UNITS)
                    {
                        (void)strcpy((char*)text,(const char*)PacketManagerGetEngUnits_s(0, UOM_data_L, UOM_data_H, unitsType, &VIEW_INPOINT_UNITS_TYPE));
                    }
                    else if (unitsType <= USE_UOM_11)
                    {
                        (void)strcpy((char*)text,(const char*)PacketManagerGetEngUnits_s((T_SUBSYSTEM_IDX)UOM_data_H, UOM_data_L, 0, unitsType, &VIEW_INPOINT_UNITS_TYPE));
                    }
                    else
                    {
                        (void)strcpy((char*)text,(const char*)PacketManagerGetEngUnits_s(0, IDS_BLANK, 0, FIXED_TEXT_UNITS, &VIEW_INPOINT_UNITS_TYPE));
                    }

                    #ifdef HMI_SUPPORTS_CUSTOM_UNITS
                    PacketManagerRemoveCustomfromUnits(text);
                    #endif

                    LimitTextLength(text,8);
                }
                LOCAL_packetStage++;
                
                if(lineNumber == inpointLine)
                {
                    for(VIEW_INPOINT_UNITS_LENGTH=0;VIEW_INPOINT_UNITS_LENGTH<8;VIEW_INPOINT_UNITS_LENGTH++)
                    {
                        if(text[VIEW_INPOINT_UNITS_LENGTH] == '\0')
                        {
                            break;
                        }
                    }
                    memcpy(VIEW_INPOINT_UNITS,text,VIEW_INPOINT_UNITS_LENGTH);
                }                
                else
                {
                    for(VIEW_OUPOINT_UNITS_LENGTH=0;VIEW_OUPOINT_UNITS_LENGTH<8;VIEW_OUPOINT_UNITS_LENGTH++)
                    {
                        if(text[VIEW_OUPOINT_UNITS_LENGTH] == '\0')
                        {
                            break;
                        }
                    }
                    memcpy(VIEW_OUPOINT_UNITS,text,VIEW_OUPOINT_UNITS_LENGTH);
                    //VIEW_OUPOINT_UNITS_TYPE = VIEW_INPOINT_UNITS_TYPE;
                }
            }
            while ((LOCAL_packetStage != startPacket) && (LOCAL_packetStage <= LINEARIZER_VIEW_OUPOINT_UNITS));
        }
        
        /*
            LINEARIZER_FRAME_CENTRE_KEY_ICON,
            LINEARIZER_FRAME_CENTRE_KEY_TEXT,         
        */
        
        FRAME_CENTRE_KEY_ICON = 'B';    //set default
        //if ((LOCAL_packetStage >= LINEARIZER_FRAME_CENTRE_KEY_ICON) && (LOCAL_packetStage <= LINEARIZER_FRAME_CENTRE_KEY_TEXT))
        do
        {
            //TUSIGN8 SpecificLanguage = testLanguage;
            E_STRING_TYPE stringType;
            
            //const TUSIGN8 *pArray = Blank_Icon;
    
            switch (currLinearizerCursorPos)
            {
                case LINEARIZER_CURSOR_INPOINT_VIEW:
                    if (FrameTable_isLinearizerInpointCapture_Enabled())
                    {
                        if ((FrameTable_isLinearizerProcessValueDisplayEnabled())
                         && (LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.ProcessSignal != 0xFF))
                        {
                            ATTRIB_IDENTIFIER_TYPE ARM_check_object = {LinearizerSubsystem,InPointObject,LinPointSelected};
                            if (WriteProtect_HMICanAccess(ARM_check_object, (TUSIGN8)0))
                            {
                                LOCAL_stringtable_text_id.attribIdx = IDS_CAPTURE;

                                FRAME_CENTRE_KEY_ICON = 'U';
                            }
                        }
                    }
                    break;

                case LINEARIZER_CURSOR_OUTPOINT_VIEW:
                    if (FrameTable_isLinearizerPointEnableDisable_Enabled())
                    {
                        ATTRIB_IDENTIFIER_TYPE ARM_check_object = {LinearizerSubsystem,PointEnableObject,LinPointSelected};
                        if (WriteProtect_HMICanAccess(ARM_check_object, (TUSIGN8)0))
                        {
                            if (FrameTable_GetLinearizerPointEnable(LinearizerSubsystem, PointEnableObject, LinPointSelected))
                            {
                                LOCAL_stringtable_text_id.attribIdx = IDS_DEACTIVATE;

                                FRAME_CENTRE_KEY_ICON = 'D';
                            }
                            else
                            {
                                LOCAL_stringtable_text_id.attribIdx = IDS_ACTIVATE;


                                FRAME_CENTRE_KEY_ICON = 'U';
                            }
                        }
                    }
                    break;

                case LINEARIZER_CURSOR_EXIT:
                default:
                    LOCAL_stringtable_text_id.attribIdx = IDS_BLANK;

                    FRAME_CENTRE_KEY_ICON = 'B';
                
                break;

            }

            
            TUSIGN8 specLang = testLanguage;
            if(testLanguage==HMI_LANGUAGE_CHINESE)
            {
                specLang = HMI_LANGUAGE_ENGLISH;
            }
            
            FRAME_CENTRE_KEY_TEXT = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, specLang);
            if(stringType==STRING_TYPE_LIBIDX)
            {            
                FRAME_CENTRE_KEY_LENGTH = 2;
                FRAME_CENTRE_KEY_IDENT = 0xE2;
            }
            else
            {
                TUSIGN16 i;
                
                for (i=0; i<10; i++)
                {
                    if(FRAME_CENTRE_KEY_TEXT[i]==0)
                    {
                        break;
                    }
                }
                FRAME_CENTRE_KEY_LENGTH = i;
                FRAME_CENTRE_KEY_IDENT = (stringType<<5)| i;
            }
            
            LOCAL_stringtable_text_id.attribIdx = IDS_BLANK;
            
        }while(0);
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_1)
    {
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5) + VIEW_PV_VALUE_LENGTH + VIEW_PV_UNITS_LENGTH + 2;              
        LOCAL_packet_HL[2] = VIEW_PV_VALUE_LENGTH;
        LOCAL_packet_HL[3] = (VIEW_PV_UNITS_TYPE<<5)+ VIEW_PV_UNITS_LENGTH;
        
        memcpy(&LOCAL_packet_HL[4],VIEW_PV_VALUE,VIEW_PV_VALUE_LENGTH);
        memcpy(&LOCAL_packet_HL[4+VIEW_PV_VALUE_LENGTH],VIEW_PV_UNITS,VIEW_PV_UNITS_LENGTH);
        if(AddPacketToBuffer(VIEW_PV_VALUE_LENGTH+VIEW_PV_UNITS_LENGTH+4, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_2;
            isBuild += 1;
            isOperation1 = 0x01;
            isOperation2 = 0x00;
            isEndframe   = 0x00;
        }
        else
        {
            isOperation1 = 0x00;
            isOperation2 = 0x00;
            isEndframe   = 0x00;
        }
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_2)
    {
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5)+VIEW_INPOINT_VALUE_LENGTH+VIEW_INPOINT_UNITS_LENGTH+VIEW_OUPOINT_VALUE_LENGTH+VIEW_OUPOINT_UNITS_LENGTH+4;  
        LOCAL_packet_HL[2] = VIEW_INPOINT_VALUE_LENGTH;
        LOCAL_packet_HL[3] = VIEW_INPOINT_UNITS_LENGTH;
        LOCAL_packet_HL[4] = VIEW_OUPOINT_VALUE_LENGTH;
        LOCAL_packet_HL[5] = VIEW_OUPOINT_UNITS_LENGTH;
        
        memcpy(&LOCAL_packet_HL[6],VIEW_INPOINT_VALUE,VIEW_INPOINT_VALUE_LENGTH);
        memcpy(&LOCAL_packet_HL[6+VIEW_INPOINT_VALUE_LENGTH],VIEW_INPOINT_UNITS,VIEW_INPOINT_UNITS_LENGTH);
        memcpy(&LOCAL_packet_HL[6+VIEW_INPOINT_VALUE_LENGTH+VIEW_INPOINT_UNITS_LENGTH],VIEW_OUPOINT_VALUE,VIEW_OUPOINT_VALUE_LENGTH);
        memcpy(&LOCAL_packet_HL[6+VIEW_INPOINT_VALUE_LENGTH+VIEW_INPOINT_UNITS_LENGTH+VIEW_OUPOINT_VALUE_LENGTH],VIEW_OUPOINT_UNITS,VIEW_OUPOINT_UNITS_LENGTH);
       
        if(AddPacketToBuffer(VIEW_INPOINT_VALUE_LENGTH+VIEW_INPOINT_UNITS_LENGTH+VIEW_OUPOINT_VALUE_LENGTH+VIEW_OUPOINT_UNITS_LENGTH+6, 0))
        {
            packetStage = FRAME_STAGE_OPERATION_3;
            isBuild += 1;
            isOperation2 = 0x01;
            isEndframe   = 0x00;
        }
        else
        {
            isOperation2 = 0x00;
            isEndframe   = 0x00;
        }
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_3)
    {        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = (packetStage<<5)+2+FRAME_CENTRE_KEY_LENGTH;  
        LOCAL_packet_HL[2] = FRAME_CENTRE_KEY_ICON;
        LOCAL_packet_HL[3] = FRAME_CENTRE_KEY_IDENT;
        memcpy(&LOCAL_packet_HL[4],FRAME_CENTRE_KEY_TEXT,FRAME_CENTRE_KEY_LENGTH);
       
        if(AddPacketToBuffer(4+FRAME_CENTRE_KEY_LENGTH, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        } 
    }
    
    if((packetStage == FRAME_STAGE_EDIT))
    {

        TUSIGN8 EDIT_UNITS[7] = {0};                      // Engineering Units
        TUSIGN8 EDIT_UNITS_LENGTH = 0;
        E_STRING_TYPE EDIT_UNITS_TYPE;
        
        TUSIGN8 EDIT_MAX[13] = {0};                        // High Limit 
        TUSIGN8 EDIT_MAX_LENGTH = 0;
        
        TUSIGN8 EDIT_MIN[13] = {0};                        // Low Limit
        TUSIGN8 EDIT_MIN_LENGTH = 0;        

        
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_EDIT_UNITS,                      // Engineering Units                       |
        //    LINEARIZER_EDIT_MAX,                        // High Limit                              |
        //    LINEARIZER_EDIT_MIN,                        // Low Limit                               |
        //------------------------------------------------//-----------------------------------------|
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_EDIT_UNITS,                      // Engineering Units                       |
        //------------------------------------------------//-----------------------------------------|
        //if (LOCAL_packetStage == LINEARIZER_EDIT_UNITS)
        do
        {
            TUSIGN8 unitsType, UOM_data_L, UOM_data_H;
            TUSIGN8 const *pUnits;
            //E_STRING_TYPE unitStrType;

            switch (currLinearizerCursorPos)
            {
                case LINEARIZER_CURSOR_INPOINT_EDIT:
                    unitsType = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_type_inpoint;
                    UOM_data_L = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_inpoint_L;
                    UOM_data_H = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_inpoint_H;
                    break;
                case LINEARIZER_CURSOR_OUTPOINT_EDIT:
                    unitsType = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_type_outpoint;
                    UOM_data_L = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_outpoint_L;
                    UOM_data_H = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.UOM_data_outpoint_H;
                    break;
                default:
                    unitsType  = FIXED_TEXT_UNITS;
                    UOM_data_L = IDS_BLANK;
                    UOM_data_H = 0;
                    break;
            }

            if (unitsType == FIXED_TEXT_UNITS)
            {
                //pUnits = PacketManagerGetEngUnits(0, UOM_data_L, UOM_data_H, unitsType);
                pUnits = PacketManagerGetEngUnits_s(0, UOM_data_L, UOM_data_H, unitsType, &EDIT_UNITS_TYPE);
            }
            else if (unitsType <= USE_UOM_11)
            {
                //pUnits = PacketManagerGetEngUnits((T_SUBSYSTEM_IDX)UOM_data_H, UOM_data_L, 0, unitsType);
                pUnits = PacketManagerGetEngUnits_s((T_SUBSYSTEM_IDX)UOM_data_H, UOM_data_L, 0, unitsType, &EDIT_UNITS_TYPE);
            }
            else
            {
                //pUnits = PacketManagerGetEngUnits(0, IDS_BLANK, 0, FIXED_TEXT_UNITS);
                pUnits = PacketManagerGetEngUnits_s(0, IDS_BLANK, 0, FIXED_TEXT_UNITS, &EDIT_UNITS_TYPE);
            }

            (void)strcpy((char *)text,(const char *)pUnits);

            #ifdef HMI_SUPPORTS_CUSTOM_UNITS
            PacketManagerRemoveCustomfromUnits(text);
            #endif

            LimitTextLength(text,8);
            
            for(EDIT_UNITS_LENGTH=0;EDIT_UNITS_LENGTH<15;EDIT_UNITS_LENGTH++)
            {
                if(text[EDIT_UNITS_LENGTH] == '\0')
                {
                    break;
                }
            }
            memcpy(EDIT_UNITS,text,EDIT_UNITS_LENGTH);
            //packetsBuilt += BuildWriteString(text,HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,2,JUSTIFY_RIGHT,FONT_0,NO_INVERT,0,0,8,0);
        }while(0);
        
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_EDIT_MAX,                        // High Limit                              |
        //    LINEARIZER_EDIT_MIN,                        // Low Limit                               |
        //------------------------------------------------//-----------------------------------------|
        //if ((LOCAL_packetStage >= LINEARIZER_EDIT_MAX) && (LOCAL_packetStage <= LINEARIZER_EDIT_MIN))
        do
        {
            //T_SUBSYSTEM_IDX DataSS = LinearizerSubsystem;
            TUSIGN16        DataObj;
            TINT16          DataAtt = LinPointSelected;

            TBOOL           DataValid;
            TUSIGN8         DataDecPt;

            switch (currLinearizerCursorPos)
            {
                case LINEARIZER_CURSOR_INPOINT_EDIT:
                    DataObj   = InPointObject;
                    DataDecPt = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.dpPosn_inpoint;
                    DataValid = eTRUE;
                    break;
                case LINEARIZER_CURSOR_OUTPOINT_EDIT:
                    DataObj   = OutPointObject;
                    DataDecPt = LOCAL_pCurrDisplayFrame->frameDefinition.linearizerFrame.dpPosn_outpoint;
                    DataValid = eTRUE;
                    break;
                default:
                    DataValid = eFALSE;
                    break;
            }
            if (DataValid)
            {
                T_DO_ATTRIB_RANGE Range;
                //const T_UNIT SLOW *pSub = GetSubsystemPtr(DataSS);

                if (pLinearizer->GetAttributeDescription(pLinearizer,DataObj,DataAtt,(T_DO_ATTRIB_RANGE FAST*)&Range) < (TUSIGN16)ERROR_CODES)
                {
                    maxValue = Range.range.flt.max;
                    minValue = Range.range.flt.min;
                    min_max_rules = Range.rulesAreActive;
                    min_max_DP = DataDecPt;

                    LimitError = AdjusterGetLimitError(&errorCode);
                }
                else
                {
                    DataValid = eFALSE;
                }
            }

            if (!DataValid)
            {
                LOCAL_packetStage = LINEARIZER_EDIT_MIN + 1;
            }
        }while(0);
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_EDIT_MAX,                        // High Limit                              |
        //------------------------------------------------//-----------------------------------------|

        //if (LOCAL_packetStage == LINEARIZER_EDIT_MAX)
            
        if(LOCAL_packetStage != (LINEARIZER_EDIT_MIN + 1))
        {
            do
            {

                text[0] = ' ';
                text[1] = 0x00;

                if (min_max_rules)
                {
                    TUSIGN8 *pNumber;
                    TUSIGN8 negNumber = 0;
                    TUSIGN8 within_over_under = 0;
                    TUSIGN8 i;

                    pNumber = AdjusterBuildNumberString(maxValue, 7, min_max_DP, &negNumber, &within_over_under);

                    switch (within_over_under)
                    {
                    case 1: // overrange

                        for (i = 0; i <= 6; i ++)
                        {
                            (void)strcat((char *)text,"\x06");
                        }
                        break;

                    case 2: // underrange
                        for (i = 0; i <= 6; i ++)
                        {
                            (void)strcat((char *)text,"\x07");
                        }
                        break;

                    default:
                        text[0] = negNumber? '-' : ' ';
                        text[1] = 0x00;
                        while (*pNumber == ' ')
                        {
                            #pragma diag_suppress=Pm088
                            pNumber ++;
                            #pragma diag_default=Pm088
                        }
                        (void)strcat((char *)text,(char *)pNumber);
                        break;
                    }
                }
                else
                {
                   (void)strcpy((char *)text, " 999999");
                }

                for(EDIT_MAX_LENGTH=0;EDIT_MAX_LENGTH<15;EDIT_MAX_LENGTH++)
                {
                    if(text[EDIT_MAX_LENGTH] == '\0')
                    {
                        break;
                    }
                }
                memcpy(EDIT_MAX,text,EDIT_MAX_LENGTH);
            }while(0);

            //------------------------------------------------//-----------------------------------------|
            //    LINEARIZER_EDIT_MIN,                        // Low Limit                               |
            //------------------------------------------------//-----------------------------------------|
            
            //if (LOCAL_packetStage == LINEARIZER_EDIT_MIN)
            do
            {

                text[0] = ' ';
                text[1] = 0x00;

                if (min_max_rules)
                {
                    TUSIGN8 *pNumber;
                    TUSIGN8 negNumber = 0;
                    TUSIGN8 within_over_under = 0;
                    TUSIGN8 i;

                    pNumber = AdjusterBuildNumberString(minValue, 7, min_max_DP, &negNumber, &within_over_under);

                    switch (within_over_under)
                    {
                    case 1: // overrange
                        for (i = 0; i <= 6; i ++)
                        {
                            (void)strcat((char *)text,"\x06");
                        }
                        break;

                    case 2: // underrange
                        for (i = 0; i <= 6; i ++)
                        {
                            (void)strcat((char *)text,"\x07");
                        }
                        break;

                    default:
                        text[0] = negNumber? '-' : ' ';
                        text[1] = 0x00;
                        while (*pNumber == ' ')
                        {
                            #pragma diag_suppress=Pm088
                            pNumber ++;
                            #pragma diag_default=Pm088
                        }
                        (void)strcat((char *)text,(char *)pNumber);
                        break;
                    }
                }

                else
                {
                   (void)strcpy((char *)text, "-999999");
                }

                for(EDIT_MIN_LENGTH=0;EDIT_MIN_LENGTH<15;EDIT_MIN_LENGTH++)
                {
                    if(text[EDIT_MIN_LENGTH] == '\0')
                    {
                        break;
                    }
                }
                memcpy(EDIT_MIN,text,EDIT_MIN_LENGTH);
            }while(0);
        }
        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+EDIT_UNITS_LENGTH+1;         
        LOCAL_packet_HL[2] = EDIT_UNITS_LENGTH;
        memcpy(&LOCAL_packet_HL[3],EDIT_UNITS,EDIT_UNITS_LENGTH);
        if(AddPacketToBuffer(EDIT_UNITS_LENGTH+3, 0))
        {
            preCursorSel = cursorSel;
            isBuild += 1;
        }
        
        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = ((packetStage-7)<<5)+EDIT_MAX_LENGTH+1;                //length
        LOCAL_packet_HL[2] = EDIT_MAX_LENGTH;
        memcpy(&LOCAL_packet_HL[3],EDIT_MAX,EDIT_MAX_LENGTH);
        
        if(AddPacketToBuffer(EDIT_MAX_LENGTH+3,0))
        {
            preCursorSel = cursorSel;
            isBuild += 1;
        }
        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = ((packetStage-6)<<5)+EDIT_MIN_LENGTH+1;                //length
        LOCAL_packet_HL[2] = EDIT_MIN_LENGTH;
        memcpy(&LOCAL_packet_HL[3],EDIT_MIN,EDIT_MIN_LENGTH);
        
        if(AddPacketToBuffer(EDIT_MIN_LENGTH+3,0))
        {
            preCursorSel = cursorSel;
            packetStage = FRAME_STAGE_EDIT_KEYPRESS;
            isBuild += 1;
        }
        
    }
    
    if(packetStage == FRAME_STAGE_EDIT_KEYPRESS)
    {
        TUSIGN8 paramValue[23];
        TUSIGN8 paramValueLength;
        TUSIGN8 rightkey;
        TUSIGN8 invertChar = 0;

        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_EDIT_VALUE,                      // Parameter value                         |
        //------------------------------------------------//-----------------------------------------|
        //if (LOCAL_packetStage == LINEARIZER_EDIT_VALUE)
        do
        {
            TUSIGN8 negValue;
            TUSIGN8 *pValue;
            // get parameter value from Adjuster
            pValue = AdjusterGetVariableAsString(&negValue);

            text[0] = negValue ? '-' : ' ';
            text[1] = 0x00;
            (void)strcat((char *)text,(char*)pValue);
            
            for(paramValueLength=0;paramValueLength<21;paramValueLength++)
            {
                if(text[paramValueLength] == '\0')
                {
                    break;
                }
            }
            memcpy(paramValue,text,paramValueLength);
            
            if(FrameTableGetCursorPosition() != FrameTableGetCursorPositionLimit())
            {
                invertChar = FrameTableGetCursorPosition();
            }
            
        }while(0);
        
        //------------------------------------------------//-----------------------------------------|
        //    LINEARIZER_FRAME_RIGHT_KEY,                 //                                         |
        //------------------------------------------------//-----------------------------------------|
        //if (LOCAL_packetStage == LINEARIZER_FRAME_RIGHT_KEY)
        do
        {
            if ((currLinearizerCursorPos == LINEARIZER_CURSOR_INPOINT_EDIT) || (currLinearizerCursorPos == LINEARIZER_CURSOR_OUTPOINT_EDIT))
            {
                LOCAL_stringtable_text_id.attribIdx = IDS_OK;
                //TUSIGN8 invert = NO_INVERT;
                //TUSIGN8 minlen = 9;
                
                rightkey = 'O';     /*  ----  OK ---- */

                if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                {
                    LOCAL_stringtable_text_id.attribIdx = IDS_CANCEL;
                    rightkey = 'C'; /*  ---- Cancel ----*/
                }
            }
            else
            {

                //const TUSIGN8 *pArray = Blank_Icon;

                switch (currLinearizerCursorPos)
                {
                    case LINEARIZER_CURSOR_INPOINT_VIEW:
                        if (FrameTable_isLinearizerInpointEdit_Enabled())
                        {
                            ATTRIB_IDENTIFIER_TYPE ARM_check_object = {LinearizerSubsystem,InPointObject,LinPointSelected};
                            if (WriteProtect_HMICanAccess(ARM_check_object, (TUSIGN8)0))
                            {

                                //pArray = RightKey_Edit;
                                rightkey = 'E';

                            }
                        }
                        break;

                    case LINEARIZER_CURSOR_OUTPOINT_VIEW:
                        if (FrameTable_isLinearizerOutpointEdit_Enabled())
                        {
                            if (FrameTable_GetLinearizerPointEnable(LinearizerSubsystem, PointEnableObject, LinPointSelected))
                            {
                                ATTRIB_IDENTIFIER_TYPE ARM_check_object = {LinearizerSubsystem,OutPointObject,LinPointSelected};
                                if (WriteProtect_HMICanAccess(ARM_check_object, (TUSIGN8)0))
                                {
                                    #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                        IconID = SMALL_ICON_EDIT_KEY;
                                    #else
                                        //pArray = RightKey_Edit;
                                        rightkey = 'E';
                                    #endif
                                }
                            }
                        }
                        break;

                    case LINEARIZER_CURSOR_EXIT:
                    default:
                        #ifdef HMI_SUPPORTS_SEQUENCES
                        if (FrameTableIsSequenceEnabled())
                        {
                            //pArray = RightKey_Next;
                            rightkey = 'N';
                        }
                        else
                        #endif
                        {
                            //pArray = RightKey_Back;
                            rightkey = 'B';
                        }
                        #endif
                        break;
                }
                
                rightkey = 0xFF;
            }
        }while(0);
        
        LOCAL_packet_HL[0] = FRAME_LINEAR;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+paramValueLength+5;
        LOCAL_packet_HL[2] = rightkey;
        LOCAL_packet_HL[3] = (TUSIGN8)((LimitError & 0x02) != 0 ? STRING_INVERT:NO_INVERT);
        LOCAL_packet_HL[4] = (TUSIGN8)((LimitError & 0x01) != 0 ? STRING_INVERT:NO_INVERT);
        LOCAL_packet_HL[5] = invertChar;
        LOCAL_packet_HL[6] = paramValueLength;
        memcpy(&LOCAL_packet_HL[7],paramValue,paramValueLength);
        if(AddPacketToBuffer(paramValueLength+7, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
            isEndframe = 0x01;
        }        
        else
        {
            isEndframe = 0x00;
        }
    }
    
    return isBuild;
}
#endif



#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES

TUSIGN8 BuildSFrame_Waveform(void)
{
#if (!TEST_FRAME_WAVEFORM)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif

    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_BLANK_AREA,                  // Blank entire display                    | - Start of Static Area
                                                    //=========================================|
        WAVEFORM_FRAME_CRV_EDIT_ICON,               // Title Bar Icon                          | - Stert of Active Area (CRV Edit display)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_UNITS,              // Engineering Units                       |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_MAX,                // High Limit                              | - Start of Keypress area (CRV Edit display)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_MIN,                // Low Limit                               |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_MAX_ICON,           // High Limit icon or text                 |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_MIN_ICON,           // Low Limit icon or text                  |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_VALUE,              // Parameter value                         |
                                                    //=========================================|
        WAVEFORM_FRAME_CRV_EDIT_RIGHT_KEY,          //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_EDIT_LEFT_KEY,           //                                         |
                                                    //=========================================|
        WAVEFORM_FRAME_PRV_ID_LEFT_HIGHLIGHT,       // Highlight to left of PRV_ID             | - Start of continuous repeat area (Waveform display - WAVEFORM_DATA_UPDATE_PRV)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_PRV_ID,                      // Process Related Value Identifier        |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_PRV,                         // Process Related Value                   |
                                                    //=========================================|
        WAVEFORM_FRAME_CRV_ID_LEFT_HIGHLIGHT,       // Highlight to left of CRV_ID             | - Start of continuous repeat area (Waveform display - WAVEFORM_DATA_UPDATE_CRV)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV_ID,                      // Configuration Related Value Identifier  |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_CRV,                         // Configuration Related Value             |
                                                    //=========================================|
        WAVEFORM_FRAME_H_WIDTH_LEFT_HIGHLIGHT,      // Highlight to left of H_WIDTH            | - Start of continuous repeat area (Waveform display - WAVEFORM_DATA_UPDATE_H_WIDTH)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_H_WIDTH,                     // Horizontal Width Value                  |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_H_WIDTH_UNITS_1,             // Horizontal Width units (mm, cm etc)     |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_H_WIDTH_UNITS_2,             // Horizontal Width units (/DIV)           |
                                                    //=========================================|
        WAVEFORM_FRAME_H_OFFSET_LEFT_HIGHLIGHT,     // Highlight to left of H_OFFSET           | - Start of continuous repeat area (Waveform display - WAVEFORM_DATA_UPDATE_H_OFFSET)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_H_OFFSET,                    // Horizontal offset Value                 |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_H_OFFSET_UNITS,              // Horizontal offset units                 |
                                                    //=========================================|
        WAVEFORM_FRAME_WAVEFORM_RIGHT_KEY,          //                                         | - Start of continuous repeat area (Waveform display - WAVEFORM_DATA_UPDATE_SWITCHES)
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_WAVEFORM_LEFT_KEY,           //                                         |
                                                    //=========================================|
        WAVEFORM_FRAME_WAVEFORM_NOT_SUPPORTED,      //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_DRAW_GRID,                   //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_DRAW_TRACE_1,                //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_DRAW_TRACE_2,                //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_DRAW_TRACE_3,                //                                         |
                                                    //-----------------------------------------|
        WAVEFORM_FRAME_UPDATE_WAVEFORM,             //                                         |
                                                    //=========================================|
        WAVEFORM_FRAME_READ_SWITCHES                //                                         |
                                                    //-----------------------------------------|
    }WAVEFORM_FRAME_PACKET;

    typedef enum
    {
        WAVEFORM_DATA_UPDATE_PRV,
        WAVEFORM_DATA_UPDATE_CRV,
        WAVEFORM_DATA_UPDATE_H_WIDTH,
        WAVEFORM_DATA_UPDATE_H_OFFSET,
        WAVEFORM_DATA_UPDATE_SWITCHES,
        WAVEFORM_DATA_UPDATE_ALL,
    }WAVEFORM_DATA_UPDATE;

    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_OPERATION_CFG_1,
        FRAME_STAGE_OPERATION_CFG_2,
        FRAME_STAGE_OPERATION_CFG_3,
        FRAME_STAGE_OPERATION_CFG_4,
        FRAME_STAGE_OPERATION_CFG_5,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    typedef enum
    {
        FRAME_STAGE_UPDATE_PRV = 9,
        FRAME_STAGE_UPDATE_CRV,
        FRAME_STAGE_UPDATE_H_WIDTH,
        FRAME_STAGE_UPDATE_H_OFFSET,
        FRAME_STAGE_UPDATE_SWITCHES,
        FRAME_STAGE_UPDATETRACE1 = 14 ,
        FRAME_STAGE_UPDATETRACE2,
        FRAME_STAGE_UPDATETRACE3,
        FRAME_STAGE_UPDATETRACE4,
        FRAME_STAGE_UPDATETRACE5
    }WAVEFORM_FRAME_STAGE;
    
    static TUSIGN8 TracePage    = 0xFF;
    static TUSIGN8 EditPage     = 0x02;
    static TUSIGN8 ButtonPage   = 0x01;
    static TUSIGN8 PageType     = 0x01;
    
    static TUSIGN8 UpdateSwitch_flag = 0x00;
    static TUSIGN8 UpdateTrace2_flag = 0x00;
    static TUSIGN8 UpdateTrace3_flag = 0x00;
    static TUSIGN8 UpdateTrace4_flag = 0x00;
    //static TUSIGN8 UpdateTrace5_flag = 0x00;
    static TUSIGN16 TraceTriggrCount = 0;
    
    //TUSIGN8 WaveformCursorPosition;

    TUSIGN8 PRV_ID[25] = {0};
    TUSIGN8 PRV_Value[18] = {0};
    TUSIGN8 PRV_ID_Length;
    
    TUSIGN8 CRV_ID[25] = {0};
    TUSIGN8 CRV_Value[18] = {0};
    TUSIGN8 CRV_ID_Length;
    
    //TUSIGN8 CRV_Highlight;

    TUSIGN8 OFFSET_UNITS;
    TUSIGN8 H_WIDTH_UNITS;   

    TUSIGN8 S_H_ScaleResn[2] = {0};
    TUSIGN8 S_WIDTHDP;
    //TUSIGN8 H_WIDTH_Highlight;

    TUSIGN8 S_H_ScaleOffset[4] = {0};
    TUSIGN8 S_OFFSETDP;
    //TUSIGN8 H_OFFSET_Highlight;

    TUSIGN8 rightkey;
    TUSIGN8 blank_index = 0;

    static TUSIGN8 WAVEFORM_GRID_DATA[DRAW_GRID_DATABLOCK_SIZE];
    static TUSIGN8 WAVEFORM_SHMI[SmartHMI_WAVEFORM_LENGTH];

 
    TUSIGN8 isBuild = 0;
        
#ifdef HMI_SUPPORTS_SERVICEPORT
    TUSIGN8        HMI_Instance = PacketManagerIsRemoteInstance() ? 1 : 0;
#else
    TUSIGN8        HMI_Instance = 0;
#endif

    const SIGNAL_TABLE_TYPE *pPRVsignal;
    const SIGNAL_TABLE_TYPE *pCRVsignal;
    E_STRING_TYPE unitStrType = STRING_TYPE_EUROPEAN;
    TUSIGN8 units[10]={0};
    TUSIGN8 unitLen = 0;
    
    E_STRING_TYPE stringType;


//    static TBOOL ActiveAreaUpdatePending[2] = {eFALSE,eFALSE};
//    static TBOOL KeyPressAreaUpdatePending[2] = {eFALSE,eFALSE};

    static TUSIGN8 DRAW_GRID_datablock_localHMI[DRAW_GRID_DATABLOCK_SIZE]      = {0};
    static TUSIGN8 DRAW_GRID_datablock_remoteHMI[DRAW_GRID_DATABLOCK_SIZE]     = {0};
    static TUSIGN8 DRAW_TRACE_1_datablock_localHMI[DRAW_TRACE_DATABLOCK_SIZE]  = {0};
    static TUSIGN8 DRAW_TRACE_1_datablock_remoteHMI[DRAW_TRACE_DATABLOCK_SIZE] = {0};
    static TUSIGN8 DRAW_TRACE_2_datablock_localHMI[DRAW_TRACE_DATABLOCK_SIZE]  = {0};
    static TUSIGN8 DRAW_TRACE_2_datablock_remoteHMI[DRAW_TRACE_DATABLOCK_SIZE] = {0};
    static TUSIGN8 DRAW_TRACE_3_datablock_localHMI[DRAW_TRACE_DATABLOCK_SIZE]  = {0};
    static TUSIGN8 DRAW_TRACE_3_datablock_remoteHMI[DRAW_TRACE_DATABLOCK_SIZE] = {0};
    //static TUSIGN8 DRAW_TRACE_4_datablock_localHMI[SmartHMI_WAVEFORM_LENGTH]   = {0};
    static TUSIGN8 plotData[DRAW_TRACE_DATABLOCK_SIZE];
    static TUSIGN8 plotData_2;

    
    TUSIGN8 waveformIndex = LOCAL_pCurrDisplayFrame->frameDefinition.waveformFrame.waveformIndex;

    TUSIGN16 waveformSubsystem = (TUSIGN16)(LOCAL_pCurrDisplayFrame->frameDefinition.waveformFrame.waveformSubsystemIdx);
    const T_WAVEFORM *pWav = (T_WAVEFORM*)(GetSubsystemPtr(waveformSubsystem));
    TBOOL DataValid = eFALSE;

    TUSIGN8 PlotDistanceUnits = pWav->GetU8Data(HMI_WAVEFORM_PLOT_DISTANCE_UNITS_U8, &DataValid);
    if (!DataValid) {PlotDistanceUnits = HMI_WAVEFORM_PLOT_DISTANCE_NO_UNITS;}

    ADJUST_DATA tVal, tHigh, tLow, tInc;
    TUSIGN8 tType;
    TUSIGN8 LimitError = 0;
    TUSIGN16 errorCode;

    TBOOL rulesAreActive;

    tHigh.data_F = 0.0F;
    tLow.data_F = 0.0F;
    #ifdef HMI_USES_DOUBLE
    tHigh.data_D = 0.0;
    tLow.data_D = 0.0;
    #endif
    
    
    TUSIGN8 CRV_EDIT_ICON = ICON_NUMBER_EDIT;
    TUSIGN8 EngUnit[10] = {0};
    TUSIGN8 EngUnitLength;
    TUSIGN8 HighLimitValue[18] = {0};
    TUSIGN8 HighLimitLength;
    TUSIGN8 LowLimitValue[18] = {0};
    TUSIGN8 LowLimitLength;

    TUSIGN8 ParameterValue[18] = {0};
    TUSIGN8 ValueLength;
    TUSIGN8 RightButton;

    pCRVsignal = FrameTableGetSignal(FrameTable_GetWaveformCRVSignal(waveformIndex));
    
    pPRVsignal = FrameTableGetSignal(FrameTable_GetWaveformPRVSignal(waveformIndex));

    
    if(gHotHMI_ReinitializeWaveformFlag)
    {
        gHotHMI_ReinitializeWaveformFlag = eFALSE;
        memset(DRAW_GRID_datablock_localHMI,    0, sizeof(DRAW_GRID_datablock_localHMI));
        memset(DRAW_GRID_datablock_remoteHMI,   0, sizeof(DRAW_GRID_datablock_remoteHMI));
        memset(DRAW_TRACE_1_datablock_localHMI, 0, sizeof(DRAW_TRACE_1_datablock_localHMI));
        memset(DRAW_TRACE_1_datablock_remoteHMI,0, sizeof(DRAW_TRACE_1_datablock_remoteHMI));
        memset(DRAW_TRACE_2_datablock_localHMI, 0, sizeof(DRAW_TRACE_2_datablock_localHMI));
        memset(DRAW_TRACE_2_datablock_remoteHMI,0, sizeof(DRAW_TRACE_2_datablock_remoteHMI));
        memset(DRAW_TRACE_3_datablock_localHMI, 0, sizeof(DRAW_TRACE_3_datablock_localHMI));
        memset(DRAW_TRACE_3_datablock_remoteHMI,0, sizeof(DRAW_TRACE_3_datablock_remoteHMI));
    }

    WAVEFORM_CURSOR_POS WaveformCursorPos = FrameTable_GetWaveformCursor(waveformIndex);
    //TBOOL WaveformCursorChanged = FrameTable_IsWaveformCursorChanged(waveformIndex);
    
   
    if (FrameTable_IsWaveformEditStateChanged(waveformIndex))
    {
        LOCAL_updateStaticArea = eTRUE;
        FrameTable_AcknowledgeWaveformEditStateChange(waveformIndex);
    }
    
    /* -------- FRAME STAGE TO OPERATION --------*/
    
    if(WaveformCursorPos == WAVEFORM_CURSOR_CFG_REL_VAL_EDIT)
    {
        //if (LOCAL_updateActiveArea || LOCAL_keypressProcessed)
        //{
              PageType = EditPage;
              packetStage = FRAME_STAGE_STATIC;
        //}
    }
    
    else if (LOCAL_updateStaticArea || (testLanguage != preDisplayLang))
    {
        PageType = ButtonPage;
        preDisplayLang = SFD_SIMPLE.language;;
        packetStage = FRAME_STAGE_STATIC;
        
        if(LOCAL_updateStaticArea)
        {
            LOCAL_updateStaticArea = eFALSE;
            lastButtonPage_head[0] = 0;     //clear all the record
            lastButtonPage_trace1[0] = 0;
            lastButtonPage_trace2[0] = 0;
            lastButtonPage_trace3[0] = 0;
            lastButtonPage_trace4[0] = 0;
            lastButtonPage_trace5[0] = 0;
        }
        
        DRAW_GRID_datablock_remoteHMI[HMI_Instance]    ^= 0xFF;
        DRAW_TRACE_1_datablock_remoteHMI[HMI_Instance] ^= 0xFF;
        DRAW_TRACE_2_datablock_remoteHMI[HMI_Instance] ^= 0xFF;
        DRAW_TRACE_3_datablock_remoteHMI[HMI_Instance] ^= 0xFF;
        LOCAL_updateStaticArea = eFALSE;
    }
    
    else if(UpdateSwitch_flag)
    {
        PageType = ButtonPage;
        packetStage = FRAME_STAGE_UPDATE_SWITCHES;
    }
    
    else if(UpdateTrace2_flag)
    {
        PageType = TracePage;
        packetStage = FRAME_STAGE_UPDATETRACE2;
    }
    
    else if((!UpdateTrace2_flag) && UpdateTrace3_flag)
    {
        PageType = TracePage;
        packetStage = FRAME_STAGE_UPDATETRACE3;
    }
    
    else if((!UpdateTrace2_flag) && (!UpdateTrace3_flag) && UpdateTrace4_flag)
    {
        PageType = TracePage;
        packetStage = FRAME_STAGE_UPDATETRACE4;
    }
    
#define nFULL
#ifdef FULL
    else if((!UpdateTrace2_flag) && (!UpdateTrace3_flag) && (!UpdateTrace4_flag) && UpdateTrace5_flag)
    {
        PageType = TracePage;
        packetStage = FRAME_STAGE_UPDATETRACE5;
    }
#endif    
    
    else
    {   
        if(WaveformCursorPos != WAVEFORM_CURSOR_CFG_REL_VAL_EDIT)
        {
            TraceTriggrCount++;            
            if(TraceTriggrCount>=10000)
            {
                TraceTriggrCount = 0;
            }
            
            if(TraceTriggrCount%2 == 0)
            {
                PageType = TracePage;
            }            
            else
            {
                PageType = ButtonPage;
            }
            packetStage = FRAME_STAGE_STATIC;
        }
    }
   
    if(packetStage == FRAME_STAGE_STATIC)
    {
        TUSIGN8 titlebarLength;
        if (WaveformCursorPos == WAVEFORM_CURSOR_CFG_REL_VAL_EDIT)
        {            
            
            LOCAL_stringtable_text_id.attribIdx = pCRVsignal->long_identifier;

            TUSIGN8 const *ptitlebar = FrameTableGetStringSpecificLanguage_s(&stringType,LOCAL_stringtable_text_id, testLanguage);
            
            if(stringType==STRING_TYPE_LIBIDX)
            {
                titlebarLength = 2;
            }
            else
            {
                for(titlebarLength=0; titlebarLength<MAX_LENGTH_FRAME_TITLE; titlebarLength++)
                {
                    if(ptitlebar[titlebarLength] == '\0')
                    {
                        break;
                    }
                }                
            } 

            LOCAL_packet_HL[5] = (stringType<<5)|titlebarLength;
            memcpy(&LOCAL_packet_HL[6],ptitlebar,titlebarLength);
        }
        
        else
        { 
            titlebarLength = 3;
            LOCAL_packet_HL[5] =  (STRING_TYPE_EUROPEAN<<5)|titlebarLength;
            LOCAL_packet_HL[6] = 'B';
            LOCAL_packet_HL[7] = 'L';
            LOCAL_packet_HL[8] = 'K';
            
        }
        
        
        /*...... ADD FRAME_STAGE_STATIC DATA ......*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+titlebarLength+4;                
        //TUSIGN8 SpecificLanguage      = testLanguage;
        LOCAL_packet_HL[2] = testLanguage; 
        
        /* .... NEXT FRAME TO OPERATION ....*/
        if (PageType == EditPage)
        {
            PageType = EditPage;
            packetStage = FRAME_STAGE_OPERATION_CFG_1;
        }
        else if(PageType == TracePage)
        {
            PageType = TracePage;
            packetStage = FRAME_STAGE_UPDATETRACE1;
        }
        else
        {
            PageType = ButtonPage;
            packetStage = FRAME_STAGE_UPDATE_PRV;
        }

        LOCAL_packet_HL[3] = WaveformCursorPos;
        LOCAL_packet_HL[4] = PageType;
        
        if(AddPacketToBuffer(titlebarLength+6, 0))
        {
            isBuild += 1;
        } 
    }
    
    //------------------------------------------------//-----------------------------------------|
    //    WAVEFORM_FRAME_CRV_EDIT_ICON,               // Title Bar Icon                          |
    //    WAVEFORM_FRAME_CRV_EDIT_UNITS,              // Engineering Units                       |
    //    WAVEFORM_FRAME_CRV_EDIT_MAX,                // High Limit                              |
    //    WAVEFORM_FRAME_CRV_EDIT_MIN,                // Low Limit                               |
    //    WAVEFORM_FRAME_CRV_EDIT_MAX_ICON,           // High Limit icon or text                 |
    //    WAVEFORM_FRAME_CRV_EDIT_MIN_ICON,           // Low Limit icon or text                  |
    //    WAVEFORM_FRAME_CRV_EDIT_VALUE,              // Parameter value                         |
    //    WAVEFORM_FRAME_CRV_EDIT_RIGHT_KEY,          //                                         |
    //    WAVEFORM_FRAME_CRV_EDIT_LEFT_KEY,           //                                         |
    //------------------------------------------------//-----------------------------------------|

    if(PageType == EditPage)
    {        
        do
        {
            //TUSIGN8 text[FONT1_MAX_CHARS+1] = {0};

            strcpy((char *)EngUnit,
                   (const char *)PacketManagerGetEngUnits_s((TUSIGN16)pCRVsignal->engUnits.ssIdx,
                                                                      pCRVsignal->engUnits.objIdx,
                                                                      pCRVsignal->engUnits.attribIdx,
                                                                      pCRVsignal->engUnitsType,
                                                                      &unitStrType)
                  );

            #ifdef HMI_SUPPORTS_CUSTOM_UNITS
            PacketManagerRemoveCustomfromUnits(EngUnit);
            #endif

            LimitTextLength(EngUnit,8);
           
            for(EngUnitLength=0; EngUnitLength<8; EngUnitLength++)
            {
                if(EngUnit[EngUnitLength] == '\0')
                {
                    break;
                }
            }            
            
        }while(0);
        //------------------------------------------------//-----------------------------------------|
        //    WAVEFORM_FRAME_CRV_EDIT_MAX,                // High Limit                              |
        //    WAVEFORM_FRAME_CRV_EDIT_MIN,                // Low Limit                               |
        //------------------------------------------------//-----------------------------------------|

        (void)AdjusterGetFloatValueAndLimits((TUSIGN16)pCRVsignal->value.ssIdx,
                                             (TUSIGN16)pCRVsignal->value.objIdx,
                                             pCRVsignal->value.attribIdx,
                                             &tVal,
                                             &tHigh,
                                             &tLow,
                                             &tInc,
                                             &rulesAreActive,
                                             &tType);

        LimitError = AdjusterGetLimitError(&errorCode);
        
        //------------------------------------------------//-----------------------------------------|
        //    WAVEFORM_FRAME_CRV_EDIT_MAX,                // High Limit                              |
        //------------------------------------------------//-----------------------------------------|
        do
        {

            TUSIGN8 *pNumber;
            TUSIGN8 negNumber = 0;
            TUSIGN8 within_over_under = 0;
            TUSIGN8 i;
            TUSIGN8 text[FONT1_MAX_CHARS+1];

            text[0] = ' ';
            text[1] = 0x00;

            TUSIGN8 crv_edit_max_chars = FrameTableGetCursorPositionLimit() - 1;    // RJA_CRV_maxCharacters

            if (rulesAreActive)
            {
                #ifdef HMI_USES_DOUBLE
                if (tType == ST_DOUBLE)
                {
                    pNumber = AdjusterBuildNumberStringFromDouble(tHigh.data_D,
                                                                  crv_edit_max_chars,   /* RJA_CRV_maxCharacters
                                                                  CRV_EDIT_MAX_CHARS,  */
                                                                  pCRVsignal->dpPosn,
                                                                  &negNumber,
                                                                  &within_over_under);
                }
                else
                #endif
                {
                    pNumber = AdjusterBuildNumberString(tHigh.data_F,
                                                        crv_edit_max_chars,  /* RJA_CRV_maxCharacters
                                                        CRV_EDIT_MAX_CHARS,  */
                                                        pCRVsignal->dpPosn,
                                                        &negNumber,
                                                        &within_over_under);
                }

                switch (within_over_under)
                {
                case 1: // overrange
                    for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                    for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                    {
                        (void)strcat((char *)text,"\x06");
                    }
                    break;

                case 2: // underrange
                    for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                    for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                    {
                        (void)strcat((char *)text,"\x07");
                    }
                    break;

                default:
                    text[0] = negNumber? '-' : ' ';
                    text[1] = 0x00;
                    while (*pNumber == ' ')
                    {
                        #pragma diag_suppress=Pm088
                        pNumber ++;
                        #pragma diag_default=Pm088
                    }
                    (void)strcat((char *)text,(char *)pNumber);
                    break;
                }
            }
            else
            {
                for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                {
                    (void)strcat((char *)text,"9");
                }
            }

            //TUSIGN8 justify = JUSTIFY_LEFT_WITH_INDENT_6;
            
            for(HighLimitLength=0;HighLimitLength<15;HighLimitLength++)
            {
                if(text[HighLimitLength] == '\0')
                {
                    break;
                }
                else
                {
                    HighLimitValue[HighLimitLength+1] = text[HighLimitLength];
                }
            }
            
            HighLimitValue[0] = (TUSIGN8)((LimitError & 0x02) != 0 ? STRING_INVERT:NO_INVERT);

        }while(0);

        /* WAVEFORM_FRAME_CRV_EDIT_MIN */
        do
        {

            TUSIGN8 *pNumber;
            TUSIGN8 negNumber = 0;
            TUSIGN8 within_over_under = 0;
            TUSIGN8 i;

            TUSIGN8 text[FONT1_MAX_CHARS+1];
            text[0] = ' ';
            text[1] = 0x00;

            TUSIGN8 crv_edit_max_chars = FrameTableGetCursorPositionLimit() - 1;    // RJA_CRV_maxCharacters

            if (rulesAreActive)
            {
                #ifdef HMI_USES_DOUBLE
                if (tType == ST_DOUBLE)
                {
                    pNumber = AdjusterBuildNumberStringFromDouble(tLow.data_D,
                                                                  crv_edit_max_chars,   /* RJA_CRV_maxCharacters
                                                                  CRV_EDIT_MAX_CHARS,  */
                                                                  pCRVsignal->dpPosn,
                                                                  &negNumber,
                                                                  &within_over_under);
                }
                else
                #endif
                {
                    pNumber = AdjusterBuildNumberString(tLow.data_F,
                                                        crv_edit_max_chars,   /* RJA_CRV_maxCharacters
                                                        CRV_EDIT_MAX_CHARS,  */
                                                        pCRVsignal->dpPosn,
                                                        &negNumber,
                                                        &within_over_under);
                }

                switch (within_over_under)
                {
                case 1: // overrange
                    for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                    for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                    {
                        (void)strcat((char *)text,"\x06");
                    }
                    break;

                case 2: // underrange
                    for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                    for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                    {
                        (void)strcat((char *)text,"\x07");
                    }
                    break;

                default:
                    text[0] = negNumber? '-' : ' ';
                    text[1] = 0x00;
                    while (*pNumber == ' ')
                    {
                        #pragma diag_suppress=Pm088
                        pNumber ++;
                        #pragma diag_default=Pm088
                    }
                    (void)strcat((char *)text,(char *)pNumber);
                    break;
                }
            }

            else
            {
                if ((tType == ST_U8) || (tType == ST_U16) || (tType == ST_U32) || (tType == ST_CHAR) || (tType == ST_WIDECHAR))
                {
                    strcat((char *)text,"  0");
                }
                else
                {
                    text[0] = negNumber? '-' : ' ';
                    text[1] = 0x00;

                    for (i = 0; i <= crv_edit_max_chars; i ++)      /* RJA_CRV_maxCharacters
                    for (i = 0; i <= CRV_EDIT_MAX_CHARS; i ++)      */
                    {
                        (void)strcat((char *)text,"9");
                    }
                }
            }

            //TUSIGN8 justify = JUSTIFY_LEFT_WITH_INDENT_6;
            
            for(LowLimitLength=0;LowLimitLength<15;LowLimitLength++)
            {
                if(text[LowLimitLength] == '\0')
                {
                    break;
                }
                else
                {
                    LowLimitValue[LowLimitLength+1] = text[LowLimitLength];
                }
            }
            
            LowLimitValue[0] = (TUSIGN8)((LimitError & 0x01) != 0 ? STRING_INVERT : NO_INVERT);
            
        }while(0);
    

        //WAVEFORM_FRAME_CRV_EDIT_VALUE,              // Parameter value                         |
        do
        {
            TUSIGN8 text[FONT1_MAX_CHARS+1];
            //TUSIGN8 lineNumber = 1;
            
            
            TUSIGN8 negValue;
            TUSIGN8 *pValue;
            // get parameter value from Adjuster
            pValue = AdjusterGetVariableAsString(&negValue);

            text[0] = negValue ? '-' : ' ';
            text[1] = 0x00;
            (void)strcat((char *)text,(char*)pValue);
            
            
            for(ValueLength=0;ValueLength<15;ValueLength++)
            {
                if(text[ValueLength] == '\0')
                {
                    break;
                }
            }
            memcpy(&ParameterValue[1],text,ValueLength);

            if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
            {
                ParameterValue[0] = NO_INVERT;
            }
            else
            {
                ParameterValue[0] = FrameTableGetCursorPosition();
            }
        }while(0);

        do
        {
 
            RightButton = 'O';  //OK

            if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
            {
                RightButton = 'C';//CANCEL
            }
        }while(0);
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_CFG_1)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+2;                
        LOCAL_packet_HL[2] = RightButton;
        LOCAL_packet_HL[3] = CRV_EDIT_ICON;
        if(AddPacketToBuffer(4, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_OPERATION_CFG_2;
        } 	
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_CFG_2)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+HighLimitLength+2; 
        LOCAL_packet_HL[2] = HighLimitLength;
        memcpy(&LOCAL_packet_HL[3],HighLimitValue, HighLimitLength+1);

        if(AddPacketToBuffer(HighLimitLength+4, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_OPERATION_CFG_3;
        }
    }
    
    if(packetStage == FRAME_STAGE_OPERATION_CFG_3)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+LowLimitLength+2;    
        LOCAL_packet_HL[2] = LowLimitLength;    
        memcpy(&LOCAL_packet_HL[3],LowLimitValue, LowLimitLength+1);

        if(AddPacketToBuffer(LowLimitLength+4, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_OPERATION_CFG_4;
        } 
    }
        
    if(packetStage == FRAME_STAGE_OPERATION_CFG_4)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+EngUnitLength+1;
        LOCAL_packet_HL[2] = (unitStrType<<5)+EngUnitLength;    
        memcpy(&LOCAL_packet_HL[3],EngUnit, EngUnitLength);

        if(AddPacketToBuffer(EngUnitLength+3, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_OPERATION_CFG_5;
        } 
    }
 
        
    if(packetStage == FRAME_STAGE_OPERATION_CFG_5)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = (packetStage<<5)+ValueLength+2;
        LOCAL_packet_HL[2] = ValueLength;
        memcpy(&LOCAL_packet_HL[3],ParameterValue, ValueLength+1);

        if(AddPacketToBuffer(ValueLength+4, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }
    }
   
    if(PageType == ButtonPage)
    {         
        //------------------------------------------------//-----------------------------------------|
        //    WAVEFORM_FRAME_PRV_ID_LEFT_HIGHLIGHT,       // Highlight to left of PRV_ID             |
        //    WAVEFORM_FRAME_PRV_ID,                      // Process Related Value Identifier        |
        //    WAVEFORM_FRAME_PRV,                         // Process Related Value                   |
        //    WAVEFORM_FRAME_CRV_ID_LEFT_HIGHLIGHT,       // Highlight to left of CRV_ID             |
        //    WAVEFORM_FRAME_CRV_ID,                      // Configuration Related Value Identifier  |
        //    WAVEFORM_FRAME_CRV,                         // Configuration Related Value             |
        //    WAVEFORM_FRAME_H_WIDTH_LEFT_HIGHLIGHT,      // Highlight to left of H_WIDTH            |
        //    WAVEFORM_FRAME_H_WIDTH,                     // Horizontal Width Value                  |
        //    WAVEFORM_FRAME_H_WIDTH_UNITS_1,             // Horizontal Width units (mm, cm etc)     |
        //    WAVEFORM_FRAME_H_WIDTH_UNITS_2,             // Horizontal Width units (/DIV)           |
        //    WAVEFORM_FRAME_H_OFFSET_LEFT_HIGHLIGHT,     // Highlight to left of H_OFFSET           |
        //    WAVEFORM_FRAME_H_OFFSET,                    // Horizontal offset Value                 |
        //    WAVEFORM_FRAME_H_OFFSET_UNITS,              // Horizontal offset units                 |
        //    WAVEFORM_FRAME_WAVEFORM_RIGHT_KEY,          //                                         |
        //    WAVEFORM_FRAME_WAVEFORM_LEFT_KEY,           //                                         |
        //    WAVEFORM_FRAME_WAVEFORM_NOT_SUPPORTED       //                                         |
        //    WAVEFORM_FRAME_DRAW_GRID,                   //                                         |
        //    WAVEFORM_FRAME_DRAW_TRACE_1,                //                                         |
        //    WAVEFORM_FRAME_DRAW_TRACE_2,                //                                         |
        //    WAVEFORM_FRAME_DRAW_TRACE_3,                //                                         |
        //    WAVEFORM_FRAME_UPDATE_WAVEFORM,             //                                         |
        //------------------------------------------------//-----------------------------------------|
        
        /*
            WAVEFORM_FRAME_PRV_ID_LEFT_HIGHLIGHT  (Highlight to left of PRV_ID)            
            WAVEFORM_FRAME_PRV_ID,                (Process Related Value Identifier)      
            WAVEFORM_FRAME_PRV,                   (Process Related Value)                  
        */
        
        do
        {
            
            PRV_ID[0] = (WaveformCursorPos == WAVEFORM_CURSOR_PRC_REL_VAL ? STRING_INVERT : NO_INVERT);

            /* ....Process Related Value Identifier.... */
            do
            {
                LOCAL_stringtable_text_id.attribIdx = pPRVsignal->short_identifier;

                TUSIGN8 const *Prv_id = FrameTableGetStringSpecificLanguage_s(&stringType,LOCAL_stringtable_text_id, testLanguage);

                if(stringType==STRING_TYPE_LIBIDX)
                {
                    PRV_ID_Length = 2;
                }
                else
                {
                    for(PRV_ID_Length=0; PRV_ID_Length<MAX_LENGTH_PARAM_NAME; PRV_ID_Length++)
                    {
                        if(Prv_id[PRV_ID_Length] == '\0')
                        {
                          break;
                        }
                    }

                   
                } 

                PRV_ID[1] = (stringType<<5)|PRV_ID_Length;  
                memcpy(&PRV_ID[2],Prv_id,PRV_ID_Length);
                
                
            }while(0);
           
        }while(0);
         
         /*
            WAVEFORM_FRAME_CRV_ID_LEFT_HIGHLIGHT,        (Highlight to left of CRV_ID)             
            WAVEFORM_FRAME_CRV_ID,                       (Configuration Related Value Identifier)  
            WAVEFORM_FRAME_CRV,                          (Configuration Related Value)             
         */
        do
        {
            //pCRVsignal = FrameTableGetSignal(FrameTable_GetWaveformCRVSignal(waveformIndex));
            
            do
            {  
                CRV_ID[0] = (WaveformCursorPos == WAVEFORM_CURSOR_CFG_REL_VAL ? STRING_INVERT : NO_INVERT);
      
                /* .... Configuration Related Value Identifier ....*/ 
                do
                {
                    LOCAL_stringtable_text_id.attribIdx = pCRVsignal->short_identifier;

                    TUSIGN8 const *Crv_id = FrameTableGetStringSpecificLanguage_s(&stringType,LOCAL_stringtable_text_id, testLanguage);

                    if(stringType==STRING_TYPE_LIBIDX)
                    {
                        CRV_ID_Length = 2;
                    }
                    else
                    {
                        for(CRV_ID_Length=0; CRV_ID_Length<MAX_LENGTH_PARAM_NAME; CRV_ID_Length++)
                        {
                            if(Crv_id[CRV_ID_Length] == '\0')
                            {
                              break;
                            }
                        }

                    } 

                    CRV_ID[1] = (stringType<<5)|CRV_ID_Length;                    
                    memcpy(&CRV_ID[2],Crv_id,CRV_ID_Length);
                    
                }while(0);
                
            }while(0);
            
        }while(0);
        
        
        /*
            WAVEFORM_FRAME_H_WIDTH_LEFT_HIGHLIGHT,      // Highlight to left of H_WIDTH            
            WAVEFORM_FRAME_H_WIDTH,                     // Horizontal Width Value                  
            WAVEFORM_FRAME_H_WIDTH_UNITS_1,             // Horizontal Width units (mm, cm etc)     
            WAVEFORM_FRAME_H_WIDTH_UNITS_2,             // Horizontal Width units (/DIV)           
        */
        do
        {
            /*  Highlight to left of H_WIDTH  */
            //H_WIDTH_Highlight = (WaveformCursorPos == WAVEFORM_CURSOR_H_WIDTH ? 0xFF : 0x00);
              
            /*Horizontal Width Value*/
            do
            {
                //TUSIGN8 i;
                TUSIGN8 NumericField[OFFSET_VALUE_FIELD_WIDTH];
                TFLOAT H_ScaleResn = pWav->GetFloatData(HMI_WAVEFORM_H_SCALE_RES_FLOAT, &DataValid);
                TUSIGN8 widthDP;
                if (!DataValid)
                {
                    memset((void*)NumericField, 0x00, OFFSET_VALUE_FIELD_WIDTH);
                }
                else
                {
                    widthDP = 0;
                    switch (PlotDistanceUnits)
                    {
                        case HMI_WAVEFORM_PLOT_DISTANCE_FEET:           widthDP = 1;    break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_INCHES:         widthDP = 0;    break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_METRES:         widthDP = 2;    break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_CENTIMETRES:    widthDP = 0;    break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_MILLIMETRES:    widthDP = 0;    break;
                        default:                                        widthDP = 0;    break;
                    }
                    
                    S_H_ScaleResn[0] = (TUSIGN8)((TUSIGN16)H_ScaleResn>>8);
                    S_H_ScaleResn[1] = (TUSIGN8)((TUSIGN16)H_ScaleResn & 0xff);
                    S_WIDTHDP = widthDP;
                }
                //WaveformCursorPosition = WaveformCursorPos;                
            }while(0);
            
            /*  Horizontal Width units (mm, cm etc) */ 
            do
            {
                H_WIDTH_UNITS = PlotDistanceUnits;
                //WaveformCursorPosition = WaveformCursorPos;

            }while(0);
            
        }while(0);
        
        /*
            WAVEFORM_FRAME_H_OFFSET_LEFT_HIGHLIGHT,     // Highlight to left of H_OFFSET           |
            WAVEFORM_FRAME_H_OFFSET,                    // Horizontal offset Value                 |
            WAVEFORM_FRAME_H_OFFSET_UNITS,              // Horizontal offset units                 |
        */
        do
        {     
            /*Highlight to left of H_OFFSET*/            
            //H_OFFSET_Highlight = (WaveformCursorPos == WAVEFORM_CURSOR_H_OFFSET ? 0xFF : 0x00);

            /*Horizontal offset Value */
            do
            {
                union 
                {
                  TFLOAT  ScaleOffset;
                  TUSIGN8 Offset[4];
                }floatdata;
                
                
                //TUSIGN8 i;
                TUSIGN8 NumericField[OFFSET_VALUE_FIELD_WIDTH];
                //TFLOAT H_ScaleOffset = pWav->GetFloatData(HMI_WAVEFORM_H_SCALE_OFFSET_FLOAT, &DataValid);
                floatdata.ScaleOffset = pWav->GetFloatData(HMI_WAVEFORM_H_SCALE_OFFSET_FLOAT, &DataValid);
                TUSIGN8 offsetDP;
                if (!DataValid)
                {
                    memset((void*)NumericField, 0x00, OFFSET_VALUE_FIELD_WIDTH);
                }
                else
                {
                    offsetDP = 0;
                    switch (PlotDistanceUnits)
                    {
                        case HMI_WAVEFORM_PLOT_DISTANCE_FEET:           offsetDP = 1;   break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_INCHES:         offsetDP = 0;   break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_METRES:         offsetDP = 2;   break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_CENTIMETRES:    offsetDP = 0;   break;
                        case HMI_WAVEFORM_PLOT_DISTANCE_MILLIMETRES:    offsetDP = 0;   break;
                        default:                                        offsetDP = 0;   break;
                    }
                    //TUSIGN8 textField[5];

                    //Set_NumericFieldText(H_ScaleOffset,offsetDP, textField, 5, eFALSE);
                    //Set_NumericFieldPattern(textField, 5, NumericField, OFFSET_VALUE_FIELD_WIDTH);
                }
                
                //WaveformCursorPosition = WaveformCursorPos;
                
                S_H_ScaleOffset[0] = floatdata.Offset[0];
                S_H_ScaleOffset[1] = floatdata.Offset[1];
                S_H_ScaleOffset[2] = floatdata.Offset[2];
                S_H_ScaleOffset[3] = floatdata.Offset[3];
                
                S_OFFSETDP = offsetDP;
            }while(0);
            
            /* .... Horizontal offset units .... */
            OFFSET_UNITS = PlotDistanceUnits;
            //WaveformCursorPosition = WaveformCursorPos;
        }while(0);
        /*
            WAVEFORM_FRAME_WAVEFORM_RIGHT_KEY,                                                
            WAVEFORM_FRAME_WAVEFORM_LEFT_KEY,                                                  
        */
        do
        {   
            /*WAVEFORM_FRAME_WAVEFORM_RIGHT_KEY, */
          
            if ((WriteProtect_HMICanAccess(FrameTableGetSignal(FrameTable_GetWaveformCRVSignal(waveformIndex))->value, (TUSIGN8)0)) && (WaveformCursorPos == WAVEFORM_CURSOR_CFG_REL_VAL))
            {
                rightkey = 'E';
            }
            else
            {
                rightkey = 'B';
            }
        }while(0);
        
        /* .... Waveform grid ....*/      
    }while(0);
        
    if(packetStage == FRAME_STAGE_UPDATE_PRV)
    {
         /* ------ Add PRV related date to buffer.  1 --------*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+PRV_ID_Length+2; 
        
        memcpy(&LOCAL_packet_HL[2],PRV_ID,PRV_ID_Length+2);
        //memcpy(&LOCAL_packet_HL[4+PRV_ID_Length],PRV_Value,9);
        
        if(AddPacketToBuffer(PRV_ID_Length+4, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_UPDATE_CRV;
        } 	
    }
    
   if(packetStage == FRAME_STAGE_UPDATE_CRV)
    {
        /* ------ Add CRV related date to buffer. --------*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+CRV_ID_Length+2;
        
        memcpy(&LOCAL_packet_HL[2],CRV_ID,CRV_ID_Length+2);
        //memcpy(&LOCAL_packet_HL[4+CRV_ID_Length],CRV_Value,6);
        
        if(AddPacketToBuffer(CRV_ID_Length+4, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_UPDATE_H_WIDTH;
        } 
    }
    
    if(packetStage == FRAME_STAGE_UPDATE_H_WIDTH)
    {
        /* ------ Add H_WIDTH related date to buffer. --------*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+4;  
        LOCAL_packet_HL[2] = S_H_ScaleResn[0];
        LOCAL_packet_HL[3] = S_H_ScaleResn[1];
        LOCAL_packet_HL[4] = S_WIDTHDP;
        LOCAL_packet_HL[5] = H_WIDTH_UNITS;
        
        if(AddPacketToBuffer(6, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_UPDATE_H_OFFSET;
        } 
    }
    
    if(packetStage == FRAME_STAGE_UPDATE_H_OFFSET)
    {
        /* ------ Add H_OFFSET related date to buffer. --------*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+6;                
        LOCAL_packet_HL[2] = S_H_ScaleOffset[0];                
        LOCAL_packet_HL[3] = S_H_ScaleOffset[1];  
        LOCAL_packet_HL[4] = S_H_ScaleOffset[2];
        LOCAL_packet_HL[5] = S_H_ScaleOffset[3];
        LOCAL_packet_HL[6] = S_OFFSETDP;
        LOCAL_packet_HL[7] = OFFSET_UNITS;
        
        if(AddPacketToBuffer(8, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_UPDATE_SWITCHES;
        } 
    }
    
    if(packetStage == FRAME_STAGE_UPDATE_SWITCHES)
    {
        /* ------ Add WAVEFORM_KEY related date to buffer. --------*/
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-8)<<5)+1;                
        LOCAL_packet_HL[2] = rightkey;
        if(AddPacketToBuffer(3, 0))
        {
            isBuild += 1;
            packetStage = FRAME_STAGE_END;
            UpdateSwitch_flag = 0x00;
        }
        else
        {
            UpdateSwitch_flag = 0x01;
        }
    }
    
    if(PageType == TracePage)
    {
        /* .... Process Related Value .... */
        do
        {
            TUSIGN16 subsysIdx = (TUSIGN16)pPRVsignal->value.ssIdx;
            TUSIGN16 objIdx    = (TUSIGN16)pPRVsignal->value.objIdx;
            TUSIGN8 attribIdx  = pPRVsignal->value.attribIdx;

            ADJUST_DATA value;
            TUSIGN8 type;

            TUSIGN8 PRV_str[26];

            // Get Process Related Value
            if (AdjusterGetFloatValue(subsysIdx, objIdx, attribIdx, &value, &type) == (TUSIGN16)OK)
            {
                TUSIGN8 *pNumber;
                TUSIGN8 negNumber;
                TUSIGN8 within_over_under;

                #ifdef HMI_USES_DOUBLE
                if (type == ST_DOUBLE)
                {
                    pNumber = AdjusterBuildNumberStringFromDouble(value.data_D, 6, pPRVsignal->dpPosn, &negNumber, &within_over_under);
                }
                else
                #endif
                {
                    pNumber = AdjusterBuildNumberString(value.data_F, 6, pPRVsignal->dpPosn, &negNumber, &within_over_under);
                }

                if ((within_over_under == 1) || (within_over_under == 2))
                {
                    // overrange or underrange
                    PRV_str[0] = 0x20;
                    PRV_str[1] = 0x05 + within_over_under;
                    PRV_str[2] = 0x05 + within_over_under;
                    PRV_str[3] = 0x05 + within_over_under;
                    PRV_str[4] = 0x05 + within_over_under;
                    PRV_str[5] = 0x05 + within_over_under;
                    PRV_str[6] = 0x05 + within_over_under;
                    PRV_str[7] = 0x00;
                }
                else
                {
                    while (*pNumber == 0x20)
                    {
                        #pragma diag_suppress=Pm088
                        pNumber ++;
                        #pragma diag_default=Pm088
                    }

                    PRV_str[0] = negNumber ? '-' : ' ';
                    PRV_str[1] = 0x00;

                    (void)strcat((char *)PRV_str,(char *)pNumber);
                }
            }
            else
            {
                PRV_str[0] = '?';
                PRV_str[1] = 0x00;
            }
            
            TUSIGN8 const *pPRVU_str = PacketManagerGetEngUnits_s((TUSIGN16)pPRVsignal->engUnits.ssIdx,
                                                                          pPRVsignal->engUnits.objIdx,
                                                                          pPRVsignal->engUnits.attribIdx,
                                                                          pPRVsignal->engUnitsType,
                                                                          &unitStrType);
                        
            memcpy(units, pPRVU_str, 8);

            if(unitStrType==STRING_TYPE_EUROPEAN)
            {
                (void)strcat((char *)PRV_str,(const char *)units);                
            }
            
            memcpy(PRV_Value,PRV_str,9);
            
        }while(0);
        
        
         /* .... Configuration Related Value ....*/
        
        do
        {
            TUSIGN16 subsysIdx = (TUSIGN16)pCRVsignal->value.ssIdx;
            TUSIGN16 objIdx    = (TUSIGN16)pCRVsignal->value.objIdx;
            TUSIGN8 attribIdx  = pCRVsignal->value.attribIdx;

            ADJUST_DATA value;
            TUSIGN8 type;

            TUSIGN8 CRV_str[26];

            // Get Process Related Value
            if (AdjusterGetFloatValue(subsysIdx, objIdx, attribIdx, &value, &type) == (TUSIGN16)OK)
            {
                TUSIGN8 *pNumber;
                TUSIGN8 negNumber;
                TUSIGN8 within_over_under;

                #ifdef HMI_USES_DOUBLE
                if (type == ST_DOUBLE)
                {
                    pNumber = AdjusterBuildNumberStringFromDouble(value.data_D, 4, pCRVsignal->dpPosn, &negNumber, &within_over_under);
                }
                else
                #endif
                {
                    pNumber = AdjusterBuildNumberString(value.data_F, 4, pCRVsignal->dpPosn, &negNumber, &within_over_under);
                }

                if ((within_over_under == 1) || (within_over_under == 2))
                {
                    // overrange or underrange
                    CRV_str[0] = 0x20;
                    CRV_str[1] = 0x05 + within_over_under;
                    CRV_str[2] = 0x05 + within_over_under;
                    CRV_str[3] = 0x05 + within_over_under;
                    CRV_str[4] = 0x05 + within_over_under;
                    CRV_str[5] = 0x00;
                }
                else
                {
                    while (*pNumber == 0x20)
                    {
                        #pragma diag_suppress=Pm088
                        pNumber ++;
                        #pragma diag_default=Pm088
                    }


                    CRV_str[0] = negNumber ? '-' : ' ';
                    CRV_str[1] = 0x00;

                    (void)strcat((char *)CRV_str,(char *)pNumber);
                }
            }
            else
            {
                CRV_str[0] = '?';
                CRV_str[1] = 0x00;
            }
            memcpy(CRV_Value,CRV_str,6);
        }while(0);
            

        do
        {
            TUSIGN8 datablock[DRAW_GRID_DATABLOCK_SIZE] =         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                    //                      [x][x][x][x][x][x][x][x][x][x][x][x][x][x][x]
                                    // Function Code ________|  |  |  |  |  |  |  |  |  |  |  |  |  |  |
                                    // Length __________________|  |  |  |  |  |  |  |  |  |  |  |  |  |
                                    // Data: Zero Position ________|  |  |  |  |  |  |  |  |  |  |  |  |
                                    //       Number of Divisions _____|  |  |  |  |  |  |  |  |  |  |  |
                                    //       V Scale high [0] ___________|  |  |  |  |  |  |  |  |  |  |
                                    //       V Scale high [1] ______________|  |  |  |  |  |  |  |  |  |
                                    //       V Scale high [2] _________________|  |  |  |  |  |  |  |  |
                                    //       V Scale high [3] ____________________|  |  |  |  |  |  |  |
                                    //       V Scale high [4] _______________________|  |  |  |  |  |  |
                                    //       V Scale low [0] ___________________________|  |  |  |  |  |
                                    //       V Scale low [1] ______________________________|  |  |  |  |
                                    //       V Scale low [2] _________________________________|  |  |  |
                                    //       V Scale low [3] ____________________________________|  |  |
                                    //       V Scale low [4] _______________________________________|  |
                                    // Checksum _______________________________________________________|

            // Vertical Scale Values
            TBOOL HighValid, LowValid, DivValid, DPValid;

            TUSIGN8 V_ScaleRangeDP = pWav->GetU8Data(HMI_WAVEFORM_V_SCALE_RANGE_DP_U8, &DPValid);
            TFLOAT  V_ScaleRangeHigh = pWav->GetFloatData(HMI_WAVEFORM_V_SCALE_RANGE_HIGH_FLOAT, &HighValid);
            TFLOAT  V_ScaleRangeLow = pWav->GetFloatData(HMI_WAVEFORM_V_SCALE_RANGE_LOW_FLOAT, &LowValid);

            if ((HighValid) && (LowValid) && (DPValid))
            {
                Set_NumericFieldText(V_ScaleRangeHigh, V_ScaleRangeDP, &datablock[DATA_INDEX_V_SCALE_HIGH], 5, eTRUE);
                Set_NumericFieldText(V_ScaleRangeLow,  V_ScaleRangeDP, &datablock[DATA_INDEX_V_SCALE_LOW],  5, eTRUE);
                datablock[DATA_INDEX_ZERO_POSITION] = (TUSIGN8)((((0 - V_ScaleRangeLow) * 45) / (V_ScaleRangeHigh - V_ScaleRangeLow)) + 0.49f) + 1;
            }

            // Number of vertical scale divisions
            datablock[DATA_INDEX_N_DIVISIONS] = pWav->GetU8Data(HMI_WAVEFORM_V_SCALE_RANGE_DIV_U8, &DivValid);
            if (!DivValid)
            {
                datablock[DATA_INDEX_N_DIVISIONS] = 0;
            }
            
            memcpy(WAVEFORM_GRID_DATA,datablock,DRAW_GRID_DATABLOCK_SIZE);

        }while(0); 
        
        do
        {       
            TUSIGN8 *pPlotData;
            TBOOL DataValid;

            TUSIGN8 PlotEnable = pWav->GetU8Data(HMI_WAVEFORM_PLOT_ENABLE_1_U8, &DataValid);
            if (!DataValid)
            {
                PlotEnable = 0;
            }

            if (PlotEnable)
            {
                pPlotData = pWav->GetAddressData(HMI_WAVEFORM_PLOT_DATA_3_ADDRESS, &DataValid);
                pPlotData += 10;    // dont display first 10 bytes

                if (!DataValid)
                {
                    PlotEnable = 0;
                }
            }

            if (!PlotEnable)
            {
                isBuild = 0;   
            }
            else
            {
                OS_EnterRegion();
                copyPlotData((TUSIGN16*)plotData, (const TUSIGN16*)pPlotData);
                OS_LeaveRegion();
                
#ifndef FULL
                for(TUSIGN8 loop=0;loop<14;loop++)
                {
                    if(loop<13)
                    {
                        plotData[loop*8]   |= ((plotData[loop*8+6] << 2) &~ 0x7F); 
                        plotData[loop*8+1] |= ((plotData[loop*8+6] << 3) &~ 0x7F);
                        plotData[loop*8+2] |= ((plotData[loop*8+6] << 4) &~ 0x7F);
                        plotData[loop*8+3] |= ((plotData[loop*8+6] << 5) &~ 0x7F);
                        plotData[loop*8+4] |= ((plotData[loop*8+6] << 6) &~ 0x7F);
                        plotData[loop*8+5] |= ((plotData[loop*8+6] << 7) &~ 0x7F); 
                        
                        plotData[loop*8]   |= ((plotData[loop*8+7] << 1) &~ 0xBF);  
                        plotData[loop*8+1] |= ((plotData[loop*8+7] << 2) &~ 0xBF);
                        plotData[loop*8+2] |= ((plotData[loop*8+7] << 3) &~ 0xBF);
                        plotData[loop*8+3] |= ((plotData[loop*8+7] << 4) &~ 0xBF);
                        plotData[loop*8+4] |= ((plotData[loop*8+7] << 5) &~ 0xBF);
                        plotData[loop*8+5] |= ((plotData[loop*8+7] << 6) &~ 0xBF);
                        
                        memcpy(&WAVEFORM_SHMI[loop*6],&plotData[loop*8],6);
                    }
                    
                    else
                    {                     
                        memcpy(&WAVEFORM_SHMI[loop*6],&plotData[loop*8],6);
                    }
                }
#endif
            }
        }while(0);
        
        do
        {
            TUSIGN8 *pPlotData;
            TBOOL DataValid;

            TUSIGN8 PlotEnable = pWav->GetU8Data(HMI_WAVEFORM_PLOT_ENABLE_2_U8, &DataValid);
            if (!DataValid)
            {
                PlotEnable = 0;
            }

            if (PlotEnable)
            {
                pPlotData = pWav->GetAddressData(HMI_WAVEFORM_PLOT_DATA_2_ADDRESS, &DataValid);
                pPlotData += 10;    // dont display first 10 bytes
                if (!DataValid)
                {
                    PlotEnable = 0;
                }
            }

            if (!PlotEnable)
            {
                isBuild = 0;
            }
            else
            {
                OS_EnterRegion();
                copyPlotData((TUSIGN16*)plotData, (const TUSIGN16*)pPlotData);
                for(blank_index = 0; blank_index<109;blank_index++)
                {
                    TUSIGN8 startNum = plotData[0];
                    TUSIGN8 endNum   = plotData[109];
                    
                    if(startNum == endNum)
                    {
                        blank_index = startNum;
                        plotData_2  = blank_index;
                        break;
                    }
                    else
                    {
                          if(plotData[blank_index] != plotData[blank_index+1])
                          {
                              plotData_2 = endNum;
                              break;
                          }
                    }
                }
                
                OS_LeaveRegion();
            }
        }while(0);

    }
#ifdef FULL
    if(packetStage==FRAME_STAGE_UPDATETRACE1)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+28;                
        LOCAL_packet_HL[2] = SFD_SIMPLE.language;
        LOCAL_packet_HL[3] = plotData_2;  
        LOCAL_packet_HL[4] = PageType;
        
        memcpy(&LOCAL_packet_HL[5],plotData,25);
        if(AddPacketToBuffer(30, 0))
        {
            packetStage = FRAME_STAGE_UPDATETRACE2;
            isBuild += 1;
        }          
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE2)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+30; 
        memcpy(&LOCAL_packet_HL[2],&plotData[25],30);
        if(AddPacketToBuffer(32, 0))
        {
            packetStage = FRAME_STAGE_UPDATETRACE3;
            isBuild += 1;
            UpdateTrace2_flag = 0;
            UpdateTrace3_flag = 1;
        } 
        else
        {
            UpdateTrace2_flag = 1;
            UpdateTrace3_flag = 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE3)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+30; 
        memcpy(&LOCAL_packet_HL[2],&plotData[55],30);
        if(AddPacketToBuffer(32, 0))
        {
            packetStage = FRAME_STAGE_UPDATETRACE4;
            isBuild += 1;
            UpdateTrace3_flag = 0;
            UpdateTrace4_flag = 1;
            UpdateTrace5_flag = 1;
        } 
        else
        {
            UpdateTrace3_flag = 1;
            UpdateTrace4_flag = 1;
            UpdateTrace5_flag = 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE4)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+30; 
        memcpy(&LOCAL_packet_HL[2],&plotData[85],30);
        if(AddPacketToBuffer(32, 0))
        {
            packetStage = FRAME_STAGE_UPDATETRACE5;
            isBuild += 1;
            UpdateTrace4_flag = 0;
        } 
        else
        {
            UpdateTrace4_flag = 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE5)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+12; 
        memcpy(&LOCAL_packet_HL[2],WAVEFORM_GRID_DATA,12);
        if(AddPacketToBuffer(14, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        } 
        else
        {
        }
    }
#else
    if(packetStage==FRAME_STAGE_UPDATETRACE1)
    {
        
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+29;                
        LOCAL_packet_HL[2] = SFD_SIMPLE.language;
        LOCAL_packet_HL[3] = plotData_2;  
        LOCAL_packet_HL[4] = PageType;
        LOCAL_packet_HL[30] = blank_index;
        memcpy(&LOCAL_packet_HL[5],WAVEFORM_SHMI,25);

        if((lastButtonPage_trace1[0]==31)
        && (0==memcmp(&lastButtonPage_trace1[1], &LOCAL_packet_HL[0], 31)))
        {
            packetStage = FRAME_STAGE_UPDATETRACE2;
        }
        else
        {            
            LOCAL_packetBufferUsed = 0;   //made this as the first segment.
            if(AddPacketToBuffer(31, 0))
            {
                memcpy(&lastButtonPage_trace1[1], &LOCAL_packet_HL[0], 31);
                lastButtonPage_trace1[0] = 31;
                
                packetStage = FRAME_STAGE_UPDATETRACE2;
                isBuild += 1;
            }  
        }                
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE2)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+30; 
        memcpy(&LOCAL_packet_HL[2],&WAVEFORM_SHMI[25],30);

        if((lastButtonPage_trace2[0]==32)
        && (0==memcmp(&lastButtonPage_trace2[1], &LOCAL_packet_HL[0], 32)))
        {
            packetStage = FRAME_STAGE_UPDATETRACE3;
            UpdateTrace2_flag = 0;
            UpdateTrace3_flag = 1;
        }
        else
        {
            if(AddPacketToBuffer(32, 0))
            {
                memcpy(&lastButtonPage_trace2[1], &LOCAL_packet_HL[0], 32);
                lastButtonPage_trace2[0] = 32;
                
                packetStage = FRAME_STAGE_UPDATETRACE3;
                isBuild += 1;
                UpdateTrace2_flag = 0;
                UpdateTrace3_flag = 1;
            }  
            else
            {
                UpdateTrace2_flag = 1;
                UpdateTrace3_flag = 1;
            }
        } 
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE3)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+30; 
        memcpy(&LOCAL_packet_HL[2],&WAVEFORM_SHMI[55],30);

        if((lastButtonPage_trace3[0]==32)
        && (0==memcmp(&lastButtonPage_trace3[1], &LOCAL_packet_HL[0], 32)))
        {
            packetStage = FRAME_STAGE_UPDATETRACE4;            
            UpdateTrace3_flag = 0;
        }
        else
        {
            if(AddPacketToBuffer(32, 0))
            {
                memcpy(&lastButtonPage_trace3[1], &LOCAL_packet_HL[0], 32);
                lastButtonPage_trace3[0] = 32;
                
                packetStage = FRAME_STAGE_UPDATETRACE4;
                isBuild += 1;
                UpdateTrace3_flag = 0;
            }  
            else
            {
                UpdateTrace3_flag = 1;
            }
        } 

    }

    if(packetStage==FRAME_STAGE_UPDATETRACE4)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+DRAW_GRID_DATABLOCK_SIZE;         
        memcpy(&LOCAL_packet_HL[2], WAVEFORM_GRID_DATA, DRAW_GRID_DATABLOCK_SIZE);
        
        if((lastButtonPage_trace4[0]==(DRAW_GRID_DATABLOCK_SIZE+2))
        && (0==memcmp(&lastButtonPage_trace4[1], &LOCAL_packet_HL[0], (DRAW_GRID_DATABLOCK_SIZE+2))))
        {
            packetStage = FRAME_STAGE_UPDATETRACE5;            
            UpdateTrace4_flag = 0;
        }
        else
        {
            if(AddPacketToBuffer((DRAW_GRID_DATABLOCK_SIZE+2), 0))
            {
                memcpy(&lastButtonPage_trace4[1], &LOCAL_packet_HL[0], (DRAW_GRID_DATABLOCK_SIZE+2));
                lastButtonPage_trace4[0] = (DRAW_GRID_DATABLOCK_SIZE+2);
                
                packetStage = FRAME_STAGE_UPDATETRACE5;
                isBuild += 1;
                UpdateTrace4_flag = 0;
            }  
            else
            {
                UpdateTrace4_flag = 1;
            }
        } 
    }
    
    if(packetStage==FRAME_STAGE_UPDATETRACE5)
    {
        LOCAL_packet_HL[0] = FRAME_WAVEFORM;        
        LOCAL_packet_HL[1] = ((packetStage-14)<<5)+17; 
        memcpy(&LOCAL_packet_HL[2],PRV_Value,9);
        memcpy(&LOCAL_packet_HL[11],CRV_Value,5);  
        LOCAL_packet_HL[16] = (unitStrType<<5)+unitLen;  //only index unit
        memcpy(&LOCAL_packet_HL[17],units,2);        
        if((lastButtonPage_trace5[0]==19)
        && (0==memcmp(&lastButtonPage_trace5[1], &LOCAL_packet_HL[0], 19)))
        {
            packetStage = FRAME_STAGE_END;            
            //UpdateTrace5_flag = 0;
        }
        else
        {
            if(AddPacketToBuffer(19, 0))
            {
                memcpy(&lastButtonPage_trace5[1], &LOCAL_packet_HL[0], 19);
                lastButtonPage_trace5[0] = 19;
                
                packetStage = FRAME_STAGE_END;
                isBuild += 1;
                //UpdateTrace5_flag = 0;
            }  
            else
            {
                //UpdateTrace5_flag = 1;
            }
        } 

    }
#endif
    
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }
    }

    if(LOCAL_packetBuffer[4]==1)  //button page header
    {
        if((lastButtonPage_head[0]==LOCAL_packetBufferUsed)
        && (0==memcmp(&lastButtonPage_head[1], &LOCAL_packetBuffer[0], LOCAL_packetBufferUsed)))
        {// the same header......
            LOCAL_packetBufferUsed = 0;  
            isBuild = 0;            
        }
        else
        {            
            PageType = TracePage;
            TraceTriggrCount = 0;  //next pageType is NULL mesage, then trace
            memcpy(&lastButtonPage_head[1], &LOCAL_packetBuffer[0], LOCAL_packetBufferUsed);
            lastButtonPage_head[0] = LOCAL_packetBufferUsed;
        }
    }
    else if(LOCAL_packetBuffer[4]==0xFF)
    {
        if(LOCAL_packetBufferUsed<=11)
        {            
            LOCAL_packetBufferUsed = 0;  
            isBuild = 0; 
        }
    }    
    
    return isBuild;  
    
}

#endif



#ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
#define MAN_CAL_FRAME_PROGRESS_BAR_WIDTH    50
#define MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH  15
static TUSIGN8 lastManulCalData[120] = {0};   //first is the dataLen

TUSIGN8 BuildSFrame_ManulCal(void)
{       
  
    #if (!TEST_FRAME_MANUAL_CALIBRATION)
        if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
        {
            testLanguage = HMI_LANGUAGE_ENGLISH;
        }
        else
        {
            testLanguage = SFD_SIMPLE.language;
        }    
    #else
        testLanguage = SFD_SIMPLE.language;
    #endif

    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_TITLE_BAR,             // Title Bar                               | - Start of Static Area
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_ICON_T,                // Icon (Top)                              |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_ICON_B,                // Icon (Bottom                            |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_BLANK_AREA,            // Blank Area (lines 2-6)                  | - Start of status changed area
                                                    //-----------------------------------------|
    #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BAR_LEFT,     // LHS border of progress bar              |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BAR_RIGHT,    // RHS border of progress bar              |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BAR_MIDDLE_T, // Active area of progress bar (top)       |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BAR_MIDDLE_B, // Active area of progress bar (bottom)    |
                                                    //-----------------------------------------|
    #else
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_L_4,   // Left side of progress bar               |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_M_4,   // Middle of Progress bar                  |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_R_4,   // Right Side of progress bar              |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_L_5,   // Left side of progress bar               |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_M_5,   // Middle of Progress bar                  |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PROGRESS_BORDER_R_5,   // Right Side of progress bar              |
                                                    //-----------------------------------------|
    #endif                                                
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_CONTINUE_PROMPT,       // "Continue When Stable"                  |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_NEW_VALUE_PROMPT,      // "New"                                   |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_NEW_VALUE_UNITS,       //                                         |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_NEW_VALUE,             // New Value                               | - Start of keypress area (Status = HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE)
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_RESULT_1,              // Result 1                                | - Start of Active Area (Status = HMI_CAL_STATUS_SUCCESS or HMI_CAL_STATUS_SUCCESS_NO_REJECT)
                                                    //-----------------------------------------|
    #ifdef HMI_SUPPORTS_ASIAN
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_RESULT_1_ID,           // Identifier for 1st Result               |
                                                    //-----------------------------------------|
    #endif
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_RESULT_2,              // Result 2                                |
                                                    //-----------------------------------------|
    #ifdef HMI_SUPPORTS_ASIAN
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_RESULT_2_ID,           // Identifier for 2nd Result               |
                                                    //-----------------------------------------|
    #endif
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_CAL_PASS,              // "Calibration Complete"                  |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_CAL_FAIL,              // "Calibration Failed"                    | - Start of Active Area (Status = HMI_CAL_STATUS_FAILURE)
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_FAIL_REASON,           // Reason for calibration failure          |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PV,                    // Process Value                           | - Start of Active Area (Status = HMI_CAL_STATUS_WAITING_FOR_STABILITY or HMI_CAL_STATUS_CALCULATING_1)
                                                    //-----------------------------------------|
    
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_PV_ID,                 // Identifier for Process Value            |
                                                    //-----------------------------------------|
    
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_OUTPUT_HOLD_PROMPT,           //                                         |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_CALCULATION_PROGRESS,  //                                         |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_4,   //                                         |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5,   //                                         |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_RIGHT_KEY,             // Right Key -  Continue/Abort/Accept      |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_LEFT_KEY,              // Left Key -  Abort/Next/Reject           |
                                                    //-----------------------------------------|
        MAN_CAL_FRAME_PACKET_READ_SWITCHES          //                                         |
                                                    //-----------------------------------------|
    }MAN_CAL_FRAME_PACKET;

    typedef enum
      {
        FRAME_STAGE_STATIC,
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_OPERATION_BUTTON=3,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
      }FRAME_STAGE;

    //TUSIGN8 packetsBuilt = 0;
    TUSIGN8 isBuild = 0;
    //TUSIGN8 SamplingProgressRepeats;
    
    E_STRING_TYPE stringType;
    TUSIGN8 stringLen;
    TUSIGN8 const *pTitle;
    TUSIGN8 Continue[2] = {0};
    TUSIGN8 Defaultarray[2] = {0x00,0x00};
    TUSIGN8 const *Failed_Reason = Defaultarray;
    TUSIGN8 const *ProcessValueID = Defaultarray;
    //TUSIGN8 const *Hold_OutPut;

    
    TUSIGN8 RightKey = 0;
    TUSIGN8 LeftKey = 0;
    TUSIGN8 resultLength1 = 0;
    TUSIGN8 resultLength2 = 0;
    TUSIGN8 pvValueLen = 0;
    TUSIGN8 Result1[25] = {0};
    TUSIGN8 Result2[25] = {0};
    TUSIGN8 pvValue[25] = {0};
    TUSIGN8 NewValue[10] = {0};
    TUSIGN8 NewValueLength = 0;
    TUSIGN8 NewValueInvert = 0;
    TUSIGN8 NewValueInvertChar = 0;
    TUSIGN8 Calibration_Failed = 0;
    TUSIGN8 Calibration_Compelete = 0;
    TUSIGN8 New_ID[5]   = {0}; 


    ADJUST_DATA value;
    TUSIGN8     type;
    
    #if defined (TYPE_1_IMPLEMENTATION) && defined (DISPLAY_SIZE_F)
    static TUSIGN8 progressPosition = TIMERLEFTXPOS + 1;
    static TBOOL   progressCountUp  = eTRUE;
    #else
    static TUSIGN8 progressPosition = (MAX_COMMON_HMI_WIDTH / 2) - (MAN_CAL_FRAME_PROGRESS_BAR_WIDTH / 2);
    static TBOOL   progressCountUp  = eTRUE;
    #endif
    
    TUSIGN8     calibrationStatus = 0;
    //TUSIGN8     ActiveAreaStartPacket = MAN_CAL_FRAME_PACKET_PV;
    //TUSIGN8     KeypressAreaStartPacket = MAN_CAL_FRAME_PACKET_READ_SWITCHES;

    TUSIGN16 subsystem = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ssidx;
    TUSIGN16 object    = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.objidx_status;
    TUSIGN8  attribute = 0;

    if (AdjusterGetFloatValue(subsystem, object, attribute, &value, &type) == (TUSIGN16)OK)
    {
        VIP_ASSERT(type == ST_U8);
        calibrationStatus = (TUSIGN8)value.data_F;
    }

    if (LOCAL_updateStaticArea || LOCAL_keypressProcessed)
    {
        packetStage = FRAME_STAGE_STATIC; 
        LOCAL_prvCalibrationStatus = calibrationStatus;
    }
    
    else
    {
        asm("nop");
    }

    if(packetStage == FRAME_STAGE_STATIC)
    {
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_TITLE_BAR              // Title Bar                               |
        //---------------------------------------------//-----------------------------------------|

        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.titleBar_attribidx;

        pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<MAX_LENGTH_FRAME_TITLE; stringLen++)
            {
                if(pTitle[stringLen] == '\0')
                {
                    break;
                }
            }
        }        
        LOCAL_packet_HL[4] = (stringType<<5)|stringLen;
        
        LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;
        LOCAL_packet_HL[1] = (packetStage<<5) + stringLen + 3;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = calibrationStatus;
        if(calibrationStatus==0x0B)
        {
            calibrationStatus = 0x0B;
        }
        memcpy(&LOCAL_packet_HL[5], pTitle, stringLen);
        if(AddPacketToBuffer(stringLen+5, 0))
        {    
            packetStage = FRAME_STAGE_OPERATION;
            LOCAL_packetStage = MAN_CAL_FRAME_PACKET_CONTINUE_PROMPT;
            isBuild += 1;
        }
    }

    if(packetStage == FRAME_STAGE_OPERATION)
    {
        pvValueLen = PacketManagerBuilderServices_buildSignalText_s(&pvValue[0],LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ProcessSignal_1,eTRUE);
        pvValueLen = 22;  //SHMI using a const value
        
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_CONTINUE_PROMPT        // "Continue When Stable"                  |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_CONTINUE_PROMPT)
        {
            //TBOOL SkipThisStage = eFALSE;

            //TUSIGN8 yPos      = 6;
            //TUSIGN8 Justify   = JUSTIFY_CENTRE;
            //TUSIGN8 TextLimit = FONT1_MAX_CHARS;
            TUSIGN8 const *Continue_While_Stable;

            switch (calibrationStatus)
            {
                case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
                    if (!FrameTableIsSampleCollectionEnabled())
                    {
                        LOCAL_stringtable_text_id.attribIdx = IDS_CONTINUE_WHEN_STABLE;
                        //yPos = 4;
                    }
                    else
                    {
                        LOCAL_stringtable_text_id.attribIdx = IDS_SAMPLE_COLLECTION;
                        //yPos = 5;
                    }
                    break;

                case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
                    LOCAL_stringtable_text_id.attribIdx = IDS_SAMPLE_COLLECTION;
                    break;

                case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
                case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
                    LOCAL_stringtable_text_id.attribIdx = IDS_SAMPLING_COMPLETE;

                    break;

                default:
                    break;
            }
            
            Continue_While_Stable = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
            memcpy(Continue,Continue_While_Stable,2);
            
            LOCAL_packetStage ++;
     
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_NEW_VALUE_PROMPT       // "New"                                   |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_NEW_VALUE_PROMPT)
        {
            if (calibrationStatus == HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE)
            {
                TUSIGN8* pModifiedIdentifier = GetModifiedIdentifier();
                
                if (pModifiedIdentifier)
                {
                    TUSIGN8 IdentifierString[4] = {0};
                    memcpy((void *)IdentifierString,(void const *)pModifiedIdentifier,3);
                    memcpy(&New_ID[1],IdentifierString,4);
                    New_ID[0] = 0x00;
                }
                else
                {
                    New_ID[0] = 0xFF;
                    New_ID[1] = 0x00;
                    New_ID[2] = 0x00;
                    New_ID[3] = 0x00;
                    New_ID[4] = 0x00;
                    
                }
            }
            
            LOCAL_packetStage++;
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_NEW_VALUE_UNITS,       //                                         |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_NEW_VALUE_UNITS)
        {
            if (calibrationStatus == HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE)
            {
                TUSIGN8* pModifiedUnits = GetModifiedUnits();
                
                if (pModifiedUnits)
                {
                    TUSIGN8 UnitsString[9] = {0};
                    TUSIGN8 i;
                    
                    memcpy((void *)UnitsString,(void const *)pModifiedUnits,8);
                    
                    for (i =  strlen((char const *)UnitsString); i < 7; i ++)
                    {
                        UnitsString[i] = ' ';
                    }
                    
                }
            }
            
            LOCAL_packetStage++;
        } 
    
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_NEW_VALUE              // New Value                               |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_NEW_VALUE)
        {
            if (calibrationStatus == HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE)
            {
                TUSIGN8 negValue;
                TUSIGN8 *pValue;
                TUSIGN8 text[15];

                // get parameter value from Adjuster
                pValue = AdjusterGetVariableAsString(&negValue);
                text[0] = negValue ? '-' : ' ';
                text[1] = 0x00;

                (void)strcat((char *)text,(char*)pValue);
                
                for(NewValueLength=0;NewValueLength<10;NewValueLength++)
                {
                    if(text[NewValueLength]=='\0')
                    {
                        break;
                    }
                }
                memcpy(NewValue,text,NewValueLength);
               
                if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                {
                    NewValueInvert = 0x00;
                }
                else
                {
                    NewValueInvert = CHAR_INVERT;
                    NewValueInvertChar = FrameTableGetCursorPosition();
                }
                
            }
            
            LOCAL_packetStage++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_RESULT_1               // Result 1                                |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_RESULT_1)
        {
            TUSIGN8 ResultSignal = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ResultSignal_1;

            if (((calibrationStatus == HMI_CAL_STATUS_SUCCESS) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS_NO_REJECT))
             && (ResultSignal != 0xFF))
            {
                //TUSIGN8 ResultText[FONT1_MAX_CHARS + 1];
                resultLength1 = PacketManagerBuilderServices_buildSignalText_s(&Result1[0],ResultSignal,eFALSE);
                //memcpy(Result1,ResultText,22);
            }
            
            LOCAL_packetStage++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_RESULT_2               // Result 2                                |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_RESULT_2)
        {
            TUSIGN8 ResultSignal = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ResultSignal_2;

            if (((calibrationStatus == HMI_CAL_STATUS_SUCCESS) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS_NO_REJECT))
             && (ResultSignal != 0xFF))
            {
                //TUSIGN8 ResultText[FONT1_MAX_CHARS + 1];
                resultLength2 = PacketManagerBuilderServices_buildSignalText_s(&Result2[0],ResultSignal,eFALSE);
            }
                
            LOCAL_packetStage ++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_CAL_PASS               // "Calibration Complete"                  |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_CAL_PASS)
        {
            if (((calibrationStatus == HMI_CAL_STATUS_SUCCESS) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS_NO_REJECT))
             && (LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ResultSignal_1 == 0xFF)
             && (LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ResultSignal_2 == 0xFF))
            {
                Calibration_Compelete = 1;
            }
            else
            {
                Calibration_Compelete = 0;
            }
            
            LOCAL_packetStage ++;
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_CAL_FAIL               // "Calibration Failed"                    |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_CAL_FAIL)
        {
            if (calibrationStatus == HMI_CAL_STATUS_FAILURE)
            {
                Calibration_Failed = 1;
            }
            else
            {
                Calibration_Failed = 0;
            }
            
            LOCAL_packetStage ++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_FAIL_REASON            // Reason for calibration failure          |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_FAIL_REASON)
        {
            if ((calibrationStatus == HMI_CAL_STATUS_FAILURE) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS_NO_REJECT))
            {
                TUSIGN8 ErrorReason = HMI_CAL_RESULT_NO_FAILURE;
                subsystem = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ssidx;
                object    = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.objidx_errorResult;
                attribute = 0;

                if (AdjusterGetFloatValue(subsystem, object, attribute, &value, &type) == (TUSIGN16)OK)
                {
                    VIP_ASSERT(type == ST_U8);
                    ErrorReason = (TUSIGN8)value.data_F;
                }

                switch (ErrorReason)
                {
                    case HMI_CAL_RESULT_FAIL_RESULT_OUT_OF_BOUNDS:  LOCAL_stringtable_text_id.attribIdx = IDS_RESULT_OUT_OF_BOUNDS; break;
                    case HMI_CAL_RESULT_FAIL_UNSTABLE_INPUT:        LOCAL_stringtable_text_id.attribIdx = IDS_UNSTABLE_INPUT;       break;
                    case HMI_CAL_RESULT_FAIL_TEST_GAS_NOT_PRESENT:  LOCAL_stringtable_text_id.attribIdx = IDS_TEST_GAS_NOT_PRESENT; break;
                    case HMI_CAL_RESULT_FAIL_NO_RESPONSE:           LOCAL_stringtable_text_id.attribIdx = IDS_NO_RESPONSE;          break;
                    case HMI_CAL_RESULT_FAIL_NV_ERROR:              LOCAL_stringtable_text_id.attribIdx = IDS_NV_ERROR;             break;
                    case HMI_CAL_RESULT_FAIL_OFFSET_HIGH:           LOCAL_stringtable_text_id.attribIdx = IDS_OFFSET_HIGH;          break;
                    case HMI_CAL_RESULT_FAIL_OFFSET_LOW:            LOCAL_stringtable_text_id.attribIdx = IDS_OFFSET_LOW;           break;
                    case HMI_CAL_RESULT_FAIL_SLOPE_HIGH:            LOCAL_stringtable_text_id.attribIdx = IDS_SLOPE_HIGH;           break;
                    case HMI_CAL_RESULT_FAIL_SLOPE_LOW:             LOCAL_stringtable_text_id.attribIdx = IDS_SLOPE_LOW;            break;
                    default:                                        LOCAL_stringtable_text_id.attribIdx = IDS_BLANK;                break;
                }

                if ((calibrationStatus == HMI_CAL_STATUS_SUCCESS) || (calibrationStatus == HMI_CAL_STATUS_SUCCESS_NO_REJECT))
                {

                }

                Failed_Reason = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
                
            }
            
            LOCAL_packetStage++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_PV                     // Process Value                           |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_PV)
        {
            //TUSIGN8 PVText[FONT1_MAX_CHARS+1];

            pvValueLen = PacketManagerBuilderServices_buildSignalText_s(&pvValue[0],LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ProcessSignal_1,eTRUE);
            pvValueLen = 22;  //SHMI using a const value
            //memcpy(pvValue,PVText,22);
            
            LOCAL_packetStage++;
        }
        
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_PV_ID,                 // Identifier for Process Value            |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_PV_ID)
        {
            if (testLanguage == HMI_LANGUAGE_CHINESE)
            {
                 LOCAL_stringtable_text_id.attribIdx = FrameTableGetSignal(LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ProcessSignal_1)->short_identifier;
                 ProcessValueID = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
            }
            
            LOCAL_packetStage ++;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_OUTPUT_HOLD_PROMPT            //                                         |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_OUTPUT_HOLD_PROMPT)
        {
            TBOOL skipThisStage = eTRUE;

            #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
            if (calibrationStatus == HMI_CAL_STATUS_SET_OUTPUT_HOLD)
            {
                LOCAL_stringtable_text_id.attribIdx = IDS_HOLD_OUTPUT;
                skipThisStage = eFALSE;
            }
            else if ((calibrationStatus == HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT)
                  || (calibrationStatus == HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT))
            {
                LOCAL_stringtable_text_id.attribIdx = IDS_RELEASE_OUTPUT;
                skipThisStage = eFALSE;
            }
            else
            {
                skipThisStage = eTRUE;
            }
            #endif

            if (skipThisStage)
            {
                LOCAL_packetStage ++;
            }
            
            else
            {
                FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
            }
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_CALCULATION_PROGRESS,  //                                         |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_CALCULATION_PROGRESS)
        {
            if (calibrationStatus == HMI_CAL_STATUS_CALCULATING_1)
            {
                //TUSIGN8 customChar[MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH];  //{0x81,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0x81}
                //TUSIGN8 r;

                //customChar[0] = 0x81;
                //for (r = 1; r < MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH; r ++)
                //{
                //    customChar[r] = 0xBD;
                //}
                //customChar[MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH - 1] = 0x81;

                //packetsBuilt += BuildWriteArray(customChar, MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH, progressPosition, 5, 0);

                if (progressCountUp)
                {
                    if (progressPosition >= ((MAX_COMMON_HMI_WIDTH / 2) + (MAN_CAL_FRAME_PROGRESS_BAR_WIDTH / 2)) - MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH)
                    {
                        progressCountUp  = eFALSE;
                        progressPosition = (((MAX_COMMON_HMI_WIDTH / 2) + (MAN_CAL_FRAME_PROGRESS_BAR_WIDTH / 2)) - MAN_CAL_FRAME_PROGRESS_BLOCK_WIDTH) - 1;
                    }
                    else
                    {
                        progressPosition ++;
                    }
                }
                else
                {
                    if (progressPosition <= ((MAX_COMMON_HMI_WIDTH / 2) - (MAN_CAL_FRAME_PROGRESS_BAR_WIDTH / 2)))
                    {
                        progressCountUp  = eTRUE;
                        progressPosition = ((MAX_COMMON_HMI_WIDTH / 2) - (MAN_CAL_FRAME_PROGRESS_BAR_WIDTH / 2)) + 1;
                    }
                    else
                    {
                        progressPosition --;
                    }
                }
            }
            else
            {
                LOCAL_packetStage ++;
            }
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_4,   //                                         |
        // MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5,   //                                         |
        //---------------------------------------------//-----------------------------------------|
        if ((LOCAL_packetStage >= MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_4)
         && (LOCAL_packetStage <= MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5))
        {
            //TINT16  SamplingProgress = 0;
            //TBOOL   SamplingActive   = eTRUE;

            if (calibrationStatus == HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS)
            {
                subsystem = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ssidx;
                object    = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.objidx_settlingTimer;
                attribute = 0;

                if (AdjusterGetFloatValue(subsystem, object, attribute, &value, &type) == (TUSIGN16)OK)
                {
                    VIP_ASSERT(type == ST_FLOAT);
                    //SamplingProgress = (TUSIGN8)(value.data_F + 0.5);
                }
            }
            else if ((calibrationStatus == HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE) || (calibrationStatus == HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED))
            {
                //SamplingProgress = 100;
            }
            else
            {
                //SamplingActive    = eFALSE;
                LOCAL_packetStage = MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5 + 1;
            }

            //if (SamplingActive)
            //{
            //    SamplingProgressRepeats = (TUSIGN8)((SamplingProgress * 78) / 100);
            //}
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_4,   //                                         |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_4)
        {
            //TUSIGN8 customChar = 0xF0;
            /*
            packetsBuilt += BuildCustomCharacterString(&customChar,             // const TUSIGN8 *pCustomCharacter
                                                       1,                       // TUSIGN8 nChars
                                                       0,                       // TUSIGN8 x_pos
                                                       4,                       // TUSIGN8 y_pos
                                                       SamplingProgressRepeats, // TUSIGN8 nRepeats
                                                       0);                      // TUSIGN8 lastPacket
            
            */
        }
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5,   //                                         |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_SAMPLING_PROGRESS_5)
        {
            //TUSIGN8 customChar = 0x7F;
            /*
            packetsBuilt += BuildCustomCharacterString(&customChar,             // const TUSIGN8 *pCustomCharacter
                                                       1,                       // TUSIGN8 nChars
                                                       0,                       // TUSIGN8 x_pos
                                                       5,                       // TUSIGN8 y_pos
                                                       SamplingProgressRepeats, // TUSIGN8 nRepeats
                                                       0);                      // TUSIGN8 lastPacket
            
            */
        }
        
        switch (calibrationStatus)
        {
            
            /*=========================================================================*/
              
            case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                
                /*
                    1. Pv value.
                    2. New value.
                */
                
                LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                LOCAL_packet_HL[1] = (packetStage<<5) + NewValueLength+3; 
                LOCAL_packet_HL[2] = NewValueInvert;
                LOCAL_packet_HL[3] = NewValueInvertChar;
                LOCAL_packet_HL[4] = NewValueLength;
                memcpy(&LOCAL_packet_HL[5],NewValue,NewValueLength);
                if(AddPacketToBuffer(NewValueLength+5, 0))
                {
                    isBuild += 1;
                }
                
                LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                LOCAL_packet_HL[1] = ((packetStage+1)<<5)+pvValueLen+2;
                memcpy(&LOCAL_packet_HL[2],pvValue,pvValueLen);
                LOCAL_packet_HL[24] = ProcessValueID[0];
                LOCAL_packet_HL[25] = ProcessValueID[1];
                if(AddPacketToBuffer(pvValueLen+4, 0))
                {
                    packetStage = FRAME_STAGE_OPERATION_BUTTON;
                    isBuild += 1;
                }
                
                break;
                
            /*===========================================================================*/    

            case HMI_CAL_STATUS_SUCCESS_NO_REJECT:
            case HMI_CAL_STATUS_SUCCESS:
              
                /*  
                    1. Pv value.
                    2. Result 1.
                    3. Result 2.
                */
                
                if(Calibration_Compelete)
                {
                    LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                    LOCAL_packet_HL[1] = (packetStage<<5)+pvValueLen+3; 
                    LOCAL_packet_HL[2] = Calibration_Compelete;
                    memcpy(&LOCAL_packet_HL[3],pvValue,pvValueLen);
                    LOCAL_packet_HL[25] = ProcessValueID[0];
                    LOCAL_packet_HL[26] = ProcessValueID[1];
                    if(AddPacketToBuffer(pvValueLen+5, 0))
                    {
                        packetStage = FRAME_STAGE_OPERATION_BUTTON;
                        isBuild += 1;
                    } 
                }
                
                else
                {
                    TUSIGN8 PVLen = 0;
                    
                    LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                    LOCAL_packet_HL[1] = (packetStage<<5)+resultLength1+2; 
                    LOCAL_packet_HL[2] = Calibration_Compelete;
                    LOCAL_packet_HL[3] = resultLength1;
                    if(resultLength1>0)
                    {                        
                        memcpy(&LOCAL_packet_HL[4],Result1,resultLength1);
                    }
                    if(AddPacketToBuffer(resultLength1+4, 0))
                    {                        
                        isBuild += 1;
                    } 
                    
                    LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                    LOCAL_packet_HL[1] = ((packetStage+1)<<5)+resultLength2+1;
                    LOCAL_packet_HL[2] = resultLength2;
                    if(resultLength2>0)
                    {                        
                        memcpy(&LOCAL_packet_HL[3],Result2,resultLength2);
                    }
                    else
                    {
                        if(resultLength1==0)
                        {//both = 0, then copy PV Value

                            memcpy(&LOCAL_packet_HL[3],pvValue,pvValueLen);
                            PVLen = pvValueLen;
                            LOCAL_packet_HL[1] += pvValueLen;
                        }
                    }
                    if(AddPacketToBuffer(resultLength2+PVLen+3, 0))
                    {
                        packetStage = FRAME_STAGE_OPERATION_BUTTON;
                        isBuild += 1;
                    }  
                }

                
                break;

            /*===========================================================================*/
                
            case HMI_CAL_STATUS_FAILURE:
              
                /*
                    1. PV value.
                    2. Calibration Failed
                    3. Failed Reason( etc. signal Not Stable)
                */  
              
                LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                LOCAL_packet_HL[1] = (packetStage<<5)+pvValueLen+5; 
                LOCAL_packet_HL[2] = Calibration_Failed;
                memcpy(&LOCAL_packet_HL[3],Failed_Reason,2);
                memcpy(&LOCAL_packet_HL[5],pvValue,pvValueLen);
                LOCAL_packet_HL[27] = ProcessValueID[0];
                LOCAL_packet_HL[28] = ProcessValueID[1];
                if(AddPacketToBuffer(pvValueLen+7, 0))
                {
                    packetStage = FRAME_STAGE_OPERATION_BUTTON;
                    isBuild += 1;
                } 
                
                break;
                
           /*==============================================================================*/

            case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
            case HMI_CAL_STATUS_CALCULATING_1:
            case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
            case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
            case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
              
                /*
                      1.PV value.
                      2.continue when stable.
                */
              
                LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
                LOCAL_packet_HL[1] = (packetStage<<5)+pvValueLen+4; 
                LOCAL_packet_HL[2] = Continue[0];
                LOCAL_packet_HL[3] = Continue[1];
                memcpy(&LOCAL_packet_HL[4],pvValue,pvValueLen);
                LOCAL_packet_HL[26] = ProcessValueID[0];
                LOCAL_packet_HL[27] = ProcessValueID[1];
                if(AddPacketToBuffer(pvValueLen+6, 0))
                {
                    packetStage = FRAME_STAGE_OPERATION_BUTTON;
                    isBuild += 1;
                } 
                break;
                
           /*===============================================================================*/     

    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
            case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
              
                /*
                    1.Hold :         Hold Output.
                    2.Hold ACCEPT:   Release Output
                    3.Hold REJECT:   Release OutPut    
                */
                
                break;
           /*================================================================================*/
    #endif

            default:
                break;
        }  
    }

    if(packetStage==FRAME_STAGE_OPERATION_BUTTON)
    {
        
        LOCAL_packetStage = MAN_CAL_FRAME_PACKET_RIGHT_KEY;
        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_RIGHT_KEY              // Right Key -  Continue/Abort/Accept      |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_RIGHT_KEY)
        {
            ATTRIB_IDENTIFIER_TYPE StatusID;

            switch (calibrationStatus)
            {
                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                    RightKey = 16;
                    break;
                #endif

                case HMI_CAL_STATUS_WAITING_FOR_STABILITY:

                    StatusID.ssIdx = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.ssidx;
                    StatusID.objIdx = LOCAL_pCurrDisplayFrame->frameDefinition.manualCalFrame.objidx_status;
                    StatusID.attribIdx = 0;

                    if (WriteProtect_HMICanAccess(StatusID, (TUSIGN8)0))
                    {
                        if (FrameTableIsSampleCollectionEnabled())
                        {
                           RightKey = 55;
                        }
                        else
                        {
                            RightKey = 43;
                        }
                    }
                    else
                    {
                        RightKey = 0;
                    }
                    break;

                case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                    if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                    {
                        RightKey = 40;
                    }
                    else
                    {
                        RightKey = 43;
                    }
                    break;

                case HMI_CAL_STATUS_SUCCESS:
                    RightKey = 44;
                    break;

                case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
                     RightKey = 43;
                    break;


                case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
                     RightKey = 37;
                    break;

                case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
                default:
                     RightKey = 0;
                    break;
            }
            LOCAL_packetStage = MAN_CAL_FRAME_PACKET_LEFT_KEY;
        }

        //---------------------------------------------//-----------------------------------------|
        // MAN_CAL_FRAME_PACKET_LEFT_KEY               // Left Key -  Abort/Next/Reject           |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == MAN_CAL_FRAME_PACKET_LEFT_KEY)
        {
            switch (calibrationStatus)
            {
                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                    LeftKey = 17;
                    break;
                #endif

                case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
                case HMI_CAL_STATUS_CALCULATING_1:
                case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
                case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
                    LeftKey = 40;
                    break;

                case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                    LeftKey = 3;
                    break;

                case HMI_CAL_STATUS_SUCCESS:
                    LeftKey = 45;
                    break;

                case HMI_CAL_STATUS_SUCCESS_NO_REJECT:
                case HMI_CAL_STATUS_FAILURE:
                    LeftKey = 2;
                    break;

                case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
                default:
                    LeftKey = 0;
                    break;
            }
        }

        LOCAL_packet_HL[0] = FRAME_MANUAL_CALIBRATION;        
        LOCAL_packet_HL[1] = (packetStage<<5)+2;
        LOCAL_packet_HL[2] = LeftKey;
        LOCAL_packet_HL[3] = RightKey;
        if(AddPacketToBuffer(4, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }        
    }

    if(packetStage==FRAME_STAGE_END)
    {
        if((lastManulCalData[0]>0)
        && (0==memcmp(&lastManulCalData[1], LOCAL_packetBuffer, LOCAL_packetBufferUsed)))
        {//it is different packet            
            LOCAL_packetBufferUsed = 0;
            packetStage = FRAME_STAGE_NULL;
            isBuild = 0;          
        }
        else
        {
            memcpy(&lastManulCalData[1], LOCAL_packetBuffer, LOCAL_packetBufferUsed);
            lastManulCalData[0] = LOCAL_packetBufferUsed;
            
            LOCAL_packet_HL[0] = FRAME_TYPE_END;        
            LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
            if(AddPacketToBuffer(2, 1))
            {
                packetStage = FRAME_STAGE_NULL;
                isBuild += 1;
            }
        }
    }

    return isBuild;
}

#endif


#ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
// No restriction on the optimization of this function
TUSIGN8 BuildFrame_IPAddressView(void)
{
    /*

    IP_ADDRESS_VIEW_FRAME_PACKET:

        IP_ADDRESS_VIEW_FRAME_PACKET_TITLE_BAR,     // Title Bar "-------Device--------"       | - Start of Static Area
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_ID_2,          // Secondary Identifier                    |
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_TITLE,         // Parameter title                         |
                                                    //-----------------------------------------|
        #ifndef DISABLE_BACK_MOVE_SELECT
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_BMS_ICON_6,    // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_BMS_ICON_7,    // BACK_MOVE_SELECT (UP/Down) Icon         |
                                                    //-----------------------------------------|
        #endif
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_RIGHT_KEY,     // right function key "Edit"               | - Start of active area
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_VALUE,         // Parameter Value                         |
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_SLIDER_BAR,    // Slider bar                              |
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_LEFT_KEY,      // left function key  "Back"/"Next"        |
                                                    //-----------------------------------------|
        IP_ADDRESS_VIEW_FRAME_PACKET_READ_SWITCHES  //                                         |
                                                    //-----------------------------------------| - There is no keypress area
    */
    
#if (!TEST_FRAME_IPADDRESS_VIEW)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif  
    
    TUSIGN8 isBuild = 0;
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_STATIC_2,
        FRAME_STAGE_OPERATION,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    #ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER
    TUSIGN8 SecondaryIdentifierText[SECONDARY_IDENTIFIER_LEN];
    TBOOL SecondaryIdentifierActive = FrameTableGetSecondaryIdentifier(SecondaryIdentifierText);
    #endif

    if ((testLanguage != preDisplayLang) || LOCAL_updateStaticArea)
    {
        packetStage =  FRAME_STAGE_STATIC;  			/*  change the whole frame if language change  */
        LOCAL_updateStaticArea = eFALSE;
    }
    else if(LOCAL_updateActiveArea)
    {
        packetStage =  FRAME_STAGE_OPERATION; 
        LOCAL_updateActiveArea = eFALSE;
    }
    else if(packetStage>=FRAME_STAGE_NULL)
    {
        return eFALSE;
    }
    else
    {
        asm("nop");
    }
    
    
    if(packetStage==FRAME_STAGE_STATIC)
    {
        TUSIGN8 TitleLen = 0;
        E_STRING_TYPE stringType;
        
        /* Get the string or index of title */
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.titleBar_attribidx;
         
        TUSIGN8 const *pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            TitleLen = 2;
        }
        else
        {
            for(TitleLen=0; TitleLen<MAX_LENGTH_FRAME_TITLE; TitleLen++)
            {
                if(pTitle[TitleLen] == '\0')
                {
                    break;
                }
            }
        }
        
        LOCAL_packet_HL[0] = FRAME_IPADDRESS_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + TitleLen + 2;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = (stringType<<5)|TitleLen;
        memcpy(&LOCAL_packet_HL[4], pTitle, TitleLen);
        if(AddPacketToBuffer(TitleLen+4, 0))
        {
            packetStage = FRAME_STAGE_STATIC_2;
            isBuild += 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_STATIC_2)
    {
        TUSIGN8 stringLen = 0;
        TUSIGN8 Rightkey = IDS_BLANK;
        E_STRING_TYPE stringType;
        TBOOL hasUpdownButton;
        
        /* BACK_MOVE_SELECT (UP/Down) Icon */
        if (!FrameTableIsBackMoveSelectAvailable(LOCAL_pCurrDisplayFrame))
        {
            hasUpdownButton = eFALSE;
        }
        else
        {
            hasUpdownButton = eTRUE;
        }
        
        /* IP_ADDRESS_VIEW_FRAME_PACKET_TITLE */
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.title_attribidx;
        
        TUSIGN8 const *pString = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        
        if(stringType==STRING_TYPE_LIBIDX)
        {
            stringLen = 2;
        }
        else
        {
            for(stringLen=0; stringLen<MAX_LENGTH_PARAM_NAME; stringLen++)
            {
                if(pString[stringLen] == '\0')
                {
                    break;
                }
            }
        }
        
        /* IP_ADDRESS_VIEW_FRAME_PACKET_RIGHT_KEY */

        if (LOCAL_pCurrDisplayFrame->frameType == FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT)
        {
            Rightkey = IDS_BLANK;
        }
        else if (WriteProtect_HMICanAccess(LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.value, (TUSIGN8)0/*write*/))
        {
            Rightkey = IDS_EDIT;
        }
        else
        {
            Rightkey = IDS_BLANK;
        }
        
        LOCAL_packet_HL[0] = FRAME_IPADDRESS_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + 3 + stringLen; 
        LOCAL_packet_HL[2] = Rightkey;
        LOCAL_packet_HL[3] = hasUpdownButton;
        LOCAL_packet_HL[4] = (stringType<<5)|stringLen;
        memcpy(&LOCAL_packet_HL[5], pString, stringLen);
        
        if(AddPacketToBuffer(stringLen+5, 0))
        {
            packetStage = FRAME_STAGE_OPERATION;
            isBuild += 1;
        } 
    }
    
 
    if(packetStage==FRAME_STAGE_OPERATION)
    { 
      
        /* IP_ADDRESS_VIEW_FRAME_PACKET_VALUE */
        const TUSIGN8 TextLine_len = 16;
        TUSIGN8 TextLine[16];
        TUSIGN8 SliderBarNum = 0;
        TUSIGN8 SliderBarHigh = 0;
        TUSIGN8 Leftkey = IDS_BLANK;

        TUSIGN32 IPAddress;
        IP_ADDRESS_HMI IP_Addr_union;

        (void)AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                   (TUSIGN16)LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.value.objIdx,
                                   LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                   (void FAST *)&IPAddress);
    #if defined(IP_ADDRESS_DATA_REVERSED) && (IP_ADDRESS_DATA_REVERSED == 1)
        IP_ADDRESS_HMI *pIPValue32 = (IP_ADDRESS_HMI*)&IPAddress;
        IP_ADDRESS_HMI *pReverseIPValue32 = (IP_ADDRESS_HMI*)&IP_Addr_union.networkAddress;
        pReverseIPValue32->dotted_IP[0] = pIPValue32->dotted_IP[3];
        pReverseIPValue32->dotted_IP[1] = pIPValue32->dotted_IP[2];
        pReverseIPValue32->dotted_IP[2] = pIPValue32->dotted_IP[1];
        pReverseIPValue32->dotted_IP[3] = pIPValue32->dotted_IP[0];
    #else
        IP_Addr_union.networkAddress = IPAddress;
    #endif

        TextLine[0] = '0';
        IPAddress = IP_Addr_union.dotted_IP[0];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            TextLine[0] ++;
        }
        TextLine[1] = (IPAddress / 10) + 0x30;
        TextLine[2] = (IPAddress % 10) + 0x30;
        TextLine[3] = '.';

        TextLine[4] = '0';
        IPAddress = IP_Addr_union.dotted_IP[1];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            TextLine[4] ++;
        }
        TextLine[5] = (IPAddress / 10) + 0x30;
        TextLine[6] = (IPAddress % 10) + 0x30;
        TextLine[7] = '.';

        TextLine[8] = '0';
        IPAddress = IP_Addr_union.dotted_IP[2];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            TextLine[8] ++;
        }
        TextLine[9] = (IPAddress / 10) + 0x30;
        TextLine[10] = (IPAddress % 10) + 0x30;
        TextLine[11] = '.';

        TextLine[12] = '0';
        IPAddress = IP_Addr_union.dotted_IP[3];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            TextLine[12] ++;
        }
        TextLine[13] = (IPAddress / 10) + 0x30;
        TextLine[14] = (IPAddress % 10) + 0x30;
        TextLine[15] = 0x00;

        /* IP_ADDRESS_VIEW_FRAME_PACKET_SLIDER_BAR */
        TUSIGN8 high,actual;

        #ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.easySetupFlag != 0)
        {
            FrameTableGetEasySetupStages(&high, &actual);
        }
        else
        #endif
        {
            #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
            FrameTableGetMenuPositionDetailsForTFT(&actual, &high);
            #else
            FrameTableAccessMenuPositionDetails(&actual, &high, eTRUE);
            #endif
        }
        
        SliderBarHigh = high;
        SliderBarNum  = actual;
        
        /* IP_ADDRESS_VIEW_FRAME_PACKET_LEFT_KEY */
        #ifdef HMI_SUPPORTS_SEQUENCES
        if (FrameTableIsSequenceEnabled())
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_NEXT;
        }
        else
        #endif
        {
            #ifdef HMI_SUPPORTS_EASYSETUP
            if (LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.easySetupFlag != 0)
            {
                LOCAL_stringtable_text_id.attribIdx = IDS_NEXT;
            }
            else
            #endif
            {
                LOCAL_stringtable_text_id.attribIdx = IDS_BACK;
            }
        }
        
        Leftkey = LOCAL_stringtable_text_id.attribIdx;
        
        LOCAL_packet_HL[0] = FRAME_IPADDRESS_VIEW;
        LOCAL_packet_HL[1] = (packetStage<<5) + 3 + TextLine_len; 
        LOCAL_packet_HL[2] = Leftkey;
        LOCAL_packet_HL[3] = SliderBarNum;
        LOCAL_packet_HL[4] = SliderBarHigh;
        memcpy(&LOCAL_packet_HL[5], TextLine, TextLine_len);
        
        if(AddPacketToBuffer(TextLine_len+5, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        } 
        
    }

    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }
    
    return isBuild;

}

// No restriction on the optimization of this function
TUSIGN8 BuildFrame_IPAddressEdit(void)
{
    /*
    typedef enum
    {                                               // Packet                                  |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_TITLE_BAR,     // Title Bar                               |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_ID_2,          // Secondary Identifier                    |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_LEFT_KEY,      // left function key  "Next"               |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_ICON,          // Icon (Top)                              |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_3,       // Parameter Value (3/3)                   | - Start of Keypress area
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_2,       // Parameter Value (2/3)                   |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_1,       // Parameter Value (1/3)                   |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_ALL,     // Parameter Value (All)                   |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_HL_U,          // Highlight border on edit field (Top)    |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_HL_L,          // Highlight border on edit field (Bottom) |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_RIGHT_KEY,     // right function key "OK"/"Cancel"        |
                                                    //-----------------------------------------|
        IP_ADDRESS_EDIT_FRAME_PACKET_READ_SWITCHES  //                                         |
                                                    //-----------------------------------------| - There is no active area
    }IP_ADDRESS_EDIT_FRAME_PACKET;

    */
  
  
  #if (!TEST_FRAME_IPADDRESS_EDIT)
    if((SFD_SIMPLE.language==HMI_LANGUAGE_CHINESE))
    {
        testLanguage = HMI_LANGUAGE_ENGLISH;
    }
    else
    {
        testLanguage = SFD_SIMPLE.language;
    }    
#else
    testLanguage = SFD_SIMPLE.language;
#endif  
    
    TUSIGN8 isBuild = 0;
    
    typedef enum
    {
        FRAME_STAGE_STATIC = 0,   //not changed part if no user operation, including language
        FRAME_STAGE_KEYPRESS,
        FRAME_STAGE_END = 7,                 
        FRAME_STAGE_NULL,
    }FRAME_STAGE;

    const TUSIGN8 IPAddressLen = 18;
    TUSIGN8 IPAddressText[18]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    #ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER
    TUSIGN8 SecondaryIdentifierText[SECONDARY_IDENTIFIER_LEN];
    TBOOL SecondaryIdentifierActive = FrameTableGetSecondaryIdentifier(SecondaryIdentifierText);
    #endif


    TUSIGN8 cursor = FrameTableGetCursorPosition();
    
    if (cursor > 4)
    {
        cursor = 4;
    }
    
    if (LOCAL_updateStaticArea || LOCAL_keypressProcessed)
    {
        packetStage = FRAME_STAGE_STATIC;
        LOCAL_updateStaticArea = eFALSE;
    }

    else
    {
        asm("nop");
    }
    
    if(packetStage==FRAME_STAGE_STATIC)
    {
        TUSIGN8 TitleLen = 0;
        E_STRING_TYPE stringType;
        
        /* Get the string or index of title */
        LOCAL_stringtable_text_id.attribIdx = LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.titleBar_attribidx;
         
        TUSIGN8 const *pTitle = FrameTableGetStringSpecificLanguage_s(&stringType, LOCAL_stringtable_text_id, testLanguage);
        if(stringType==STRING_TYPE_LIBIDX)
        {
            TitleLen = 2;
        }
        else
        {
            for(TitleLen=0; TitleLen<MAX_LENGTH_FRAME_TITLE; TitleLen++)
            {
                if(pTitle[TitleLen] == '\0')
                {
                    break;
                }
            }
        }
        
        LOCAL_packet_HL[0] = FRAME_IPADDRESS_EDIT;
        LOCAL_packet_HL[1] = (packetStage<<5) + TitleLen + 2;                
        LOCAL_packet_HL[2] = testLanguage;
        preDisplayLang = testLanguage;
        LOCAL_packet_HL[3] = (stringType<<5)|TitleLen;
        memcpy(&LOCAL_packet_HL[4], pTitle, TitleLen);
        if(AddPacketToBuffer(TitleLen+4, 0))
        {
            packetStage = FRAME_STAGE_KEYPRESS;
            isBuild += 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_KEYPRESS)
    {
      
        TUSIGN8 Rightkey = IDS_BLANK;
        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_3,       // Parameter Value (3/3)                   |
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_2,       // Parameter Value (2/3)                   |
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_1,       // Parameter Value (1/3)                   |
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_ALL,     // Parameter Value (All)                   |
        //---------------------------------------------//-----------------------------------------|

        TUSIGN32 IPAddress = AdjusterGetTimeDateVariable();
        IP_ADDRESS_HMI IP_Addr_union;

        IP_Addr_union.networkAddress = IPAddress;

        IPAddressText[0] = ' ';

        IPAddressText[1] = '0';
        IPAddress = IP_Addr_union.dotted_IP[0];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            IPAddressText[1] ++;
        }
        IPAddressText[2] = (IPAddress / 10) + 0x30;
        IPAddressText[3] = (IPAddress % 10) + 0x30;
        IPAddressText[4] = '.';

        IPAddressText[5] = '0';
        IPAddress = IP_Addr_union.dotted_IP[1];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            IPAddressText[5] ++;
        }
        IPAddressText[6] = (IPAddress / 10) + 0x30;
        IPAddressText[7] = (IPAddress % 10) + 0x30;
        IPAddressText[8] = '.';

        IPAddressText[9] = '0';
        IPAddress = IP_Addr_union.dotted_IP[2];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            IPAddressText[9] ++;
        }
        IPAddressText[10] = (IPAddress / 10) + 0x30;
        IPAddressText[11] = (IPAddress % 10) + 0x30;
        IPAddressText[12] = '.';

        IPAddressText[13] = '0';
        IPAddress = IP_Addr_union.dotted_IP[3];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            IPAddressText[13] ++;
        }
        IPAddressText[14] = (IPAddress / 10) + 0x30;
        IPAddressText[15] = (IPAddress % 10) + 0x30;
        IPAddressText[16] = ' ';
        IPAddressText[17] = 0x00;
        

        /*
        if (cursor > 3)
        {
            LOCAL_packetStage = IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_ALL;
        }
        */
#ifdef APL
        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_3,       // Parameter Value (3/3)                   |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_3)
        {
            const IP_ADDRESS_EDIT_FIELD_DATA* pEditDataField = &IPAddressEditFieldData[2][cursor];

            IPAddressText[pEditDataField->lastChar] = 0x00;

            packetsBuilt += BuildWriteString(&IPAddressText[pEditDataField->firstChar],
                                             HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE,
                                             3,
                                             pEditDataField->justify,
                                             FONT_1,
                                             pEditDataField->invert,
                                             pEditDataField->invertChar,
                                             0,
                                             0,
                                             0);
        }

        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_2,       // Parameter Value (2/3)                   |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_2)
        {
            const IP_ADDRESS_EDIT_FIELD_DATA* pEditDataField = &IPAddressEditFieldData[1][cursor];

            IPAddressText[pEditDataField->lastChar] = 0x00;

            packetsBuilt += BuildWriteString(&IPAddressText[pEditDataField->firstChar],
                                             HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE,
                                             3,
                                             pEditDataField->justify,
                                             FONT_1,
                                             pEditDataField->invert,
                                             pEditDataField->invertChar,
                                             0,
                                             0,
                                             0);
        }
        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_1,       // Parameter Value (1/3)                   |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_1)
        {
            const IP_ADDRESS_EDIT_FIELD_DATA* pEditDataField = &IPAddressEditFieldData[0][cursor];

            IPAddressText[pEditDataField->lastChar] = 0x00;

            packetsBuilt += BuildWriteString(&IPAddressText[pEditDataField->firstChar],
                                             HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE,
                                             3,
                                             pEditDataField->justify,
                                             FONT_1,
                                             pEditDataField->invert,
                                             pEditDataField->invertChar,
                                             0,
                                             0,
                                             0);
        }
        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_ALL,     // Parameter Value (All)                   |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_VALUE_ALL)
        {
            if (cursor <= 3)
            {
                LOCAL_packetStage ++;
            }
            else
            {
                packetsBuilt += BuildWriteString(IPAddressText, HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE, 3, JUSTIFY_LEFT_WITH_INDENT_3, FONT_1, NO_INVERT, 0, 0, 0, 0);
            }
        }

        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_HL_U,          // Highlight border on edit field (Top)    |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_HL_U)
        {
            #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
                LOCAL_packetStage = IP_ADDRESS_EDIT_FRAME_PACKET_HL_L + 1;  // additional highlight not required on TFT displays
            #else
                if (cursor <= 3)
                {
                    TUSIGN8 CustomChar = 0xFF;
                    packetsBuilt += BuildCustomCharacterString(&CustomChar, 1, (TUSIGN8)(23 + (cursor * 24)), 3, 0, 0);
                }
                else
                {
                    LOCAL_packetStage = IP_ADDRESS_EDIT_FRAME_PACKET_HL_L + 1;
                }
            #endif
        }
        //---------------------------------------------//-----------------------------------------|
        // IP_ADDRESS_EDIT_FRAME_PACKET_HL_L,          // Highlight border on edit field (Bottom) |
        //---------------------------------------------//-----------------------------------------|
        if (LOCAL_packetStage == IP_ADDRESS_EDIT_FRAME_PACKET_HL_L)
        {
            #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
                LOCAL_packetStage = IP_ADDRESS_EDIT_FRAME_PACKET_HL_L + 1;  // additional highlight not required on TFT displays
            #else
                if (cursor <= 3)
                {
                    TUSIGN8 CustomChar = 0xFF;
                    packetsBuilt += BuildCustomCharacterString(&CustomChar, 1, (TUSIGN8)(23 + (cursor * 24)), 4, 0, 0);
                }
                else
                {
                    LOCAL_packetStage ++;
                }
            #endif
        }
#endif
        /*IP_ADDRESS_EDIT_FRAME_PACKET_RIGHT_KEY */
        TBOOL cursorAtLimit = FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit() ? eTRUE : eFALSE;

        if ((WriteProtect_HMICanAccess(LOCAL_pCurrDisplayFrame->frameDefinition.timeDateFrame.value, (TUSIGN8)0)) && (cursorAtLimit == eFALSE))
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_OK;
        }
        else
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_CANCEL;
        }
        
        Rightkey = LOCAL_stringtable_text_id.attribIdx;
            
        LOCAL_packet_HL[0] = FRAME_IPADDRESS_EDIT;        
        LOCAL_packet_HL[1] = ((packetStage)<<5) + IPAddressLen + 2;
        LOCAL_packet_HL[2] = Rightkey;
        LOCAL_packet_HL[3] = cursor;
        memcpy(&LOCAL_packet_HL[4],IPAddressText,IPAddressLen);
        if(AddPacketToBuffer(IPAddressLen+4, 0))
        {
            packetStage = FRAME_STAGE_END;
            isBuild += 1;
        }
    }
    
    if(packetStage==FRAME_STAGE_END)
    {
        LOCAL_packet_HL[0] = FRAME_TYPE_END;        
        LOCAL_packet_HL[1] = (packetStage<<5) + 0; 
        if(AddPacketToBuffer(2, 1))
        {
            packetStage = FRAME_STAGE_NULL;
            isBuild += 1;
        }        
    }

    return isBuild;




    
}

#endif  // end of "#ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES"


#endif   //SUPPORT_SHMI
