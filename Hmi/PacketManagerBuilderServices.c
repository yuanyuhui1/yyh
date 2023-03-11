#pragma diag_suppress=Go003

#include "IncludeList_Implementation.h"
#include <intrinsics.h>
#include <string.h>
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

#include "Hmi/Interface/HMI_idx.h"
#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "FrameTableDefinitions.h"
#include "FrameTable.h"
#include "keystateDefinitions.h"
#include "IncludeList_Frames_1.h"
#include "Adjuster.h"

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
#include "wavefornFrameDefinitions.h"
#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
#include "../../O2_Calibration/Interface/o2_calibration.h"
#include "../../O2_Calibration/Interface/o2_calibration_idx.h"
//#include "..\..\O2_Calibration\Source\O2_Calibration_PerformanceLog.h"
//#include "..\..\RelayOutput\Interface\RelayOutput.h"
//#include "..\..\RelayOutput\Interface\RelayOutput_idx.h"
//#include "..\..\nvinterface\Interface\NVinterface.h"
//#include "..\..\nvinterface\Interface\NVinterface_idx.h"
#include "../../Measurement/Interface/Measurement.h"
#include "../../Measurement/Interface/Measurement_idx.h"
//#include "..\..\OvenControl\Interface\OvenControl.h"
//#include "..\..\OvenControl\Interface\OvenControl_idx.h"
//#include "..\..\Digital_IO\Interface\Digital_IO.h"
//#include "..\..\Digital_IO\Interface\Digital_IO_idx.h"
//#include "..\..\Device\Interface\Device_idx.h"
//#include "nvInitIndeces.h"
#endif


#include "PacketManager.h"
#include "PacketManagerLocalData.h"
#include "PacketManagerBuilder.h"
#include "PacketManagerBuilderServices.h"       // PacketManagerBuilderServices

#include "writeProtect.h"

#pragma diag_suppress=Pm071
#include "low_level\core\Driver.h"
#pragma diag_default=Pm071

extern PACKETMANAGER_LOCAL_TYPE  *pPacketManagerLocal;

void PacketManagerBuilderServices_buildSignalText(TUSIGN8 *pSignalText, TUSIGN8 SignalNumber, TBOOL SignalsViewStyle)
{
    const SIGNAL_TABLE_TYPE *pSignal = FrameTableGetSignal(SignalNumber);

    ADJUST_DATA dataValue;
    TUSIGN8 dataType;
    TUSIGN8 *pStart;
    TUSIGN8 resolution;
    TUSIGN8 within_over_under, negNumber;
    TUSIGN8 i;
    TUSIGN8 lenVal;

    TUSIGN8 Text[21];

    TUSIGN8 MaxLen;

    #ifdef AZ200_SPECIAL_FEATURES_HMI
    TBOOL ValueIsValid = eTRUE;
    #endif

    // Signal Identifier
    if (SignalsViewStyle)
    {
        MaxLen = 3;
        LOCAL_stringtable_text_id.attribIdx = pSignal->short_identifier;
    }
    else
    {
        MaxLen = 11;
        LOCAL_stringtable_text_id.attribIdx = pSignal->long_identifier;
    }

    strcpy((char *)pSignalText,(const char *)FrameTableGetString(LOCAL_stringtable_text_id));
    LimitTextLength(pSignalText,MaxLen);

    for (i = strlen((char *)pSignalText); i < MaxLen; )
    {
        pSignalText[i] = ' ';
        i++;
        pSignalText[i] = 0x00;
    }

    // Signal Value
    if (SignalsViewStyle)   {MaxLen = 10;}
    else                    {MaxLen = 6;}

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


    AdjusterGetFloatValue((TUSIGN16)pSignal->value.ssIdx,
                          (TUSIGN16)pSignal->value.objIdx,
                          pSignal->value.attribIdx,
                          &dataValue,
                          &dataType);

    #ifdef AZ200_SPECIAL_FEATURES_HMI
    if (SignalsViewStyle)
    {
        ADJUST_DATA dataValidValue;
        TUSIGN8 dataValidType;

        if ((pSignal->value.ssIdx == MEAS_IDX) && (pSignal->value.objIdx == MEAS_IDX_Cell_Impedance) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(MEAS_IDX, MEAS_IDX_Cell_ImpedanceValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == MEAS_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else if ((pSignal->value.ssIdx == MEAS_IDX) && (pSignal->value.objIdx == MEAS_IDX_ImpedanceDrift) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(MEAS_IDX, MEAS_IDX_ImpedanceDriftValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == MEAS_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else if ((pSignal->value.ssIdx == CAL_IDX) && (pSignal->value.objIdx == CAL_IDX_ResponseTime) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(CAL_IDX, CAL_IDX_ResponseTimeValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == CAL_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else if ((pSignal->value.ssIdx == CAL_IDX) && (pSignal->value.objIdx == CAL_IDX_RecoveryRate) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(CAL_IDX, CAL_IDX_RecoveryRateValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == CAL_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else if ((pSignal->value.ssIdx == CAL_IDX) && (pSignal->value.objIdx == CAL_IDX_O2Deviation) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(CAL_IDX, CAL_IDX_o2DeviationValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == CAL_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else if ((pSignal->value.ssIdx == CAL_IDX) && (pSignal->value.objIdx == CAL_IDX_CellZeroOffset) && (pSignal->value.attribIdx == 0))
        {
            AdjusterGetFloatValue(CAL_IDX, CAL_IDX_CellZeroOffsetValid, 0, &dataValidValue, &dataValidType);

            if (dataValidValue.data_F == CAL_DATA_NOT_VALID)
            {
                ValueIsValid = eFALSE;
            }
        }
        else
        {
            ValueIsValid = eTRUE;
        }
    }
    else
    {
        ValueIsValid = eTRUE;
    }


    if (ValueIsValid == eFALSE)
    {
        pSignalText[3]  = ' ';
        pSignalText[4]  = ' ';
        pSignalText[5]  = ' ';
        pSignalText[6]  = ' ';
        pSignalText[7]  = ' ';
        pSignalText[8]  = '-';
        pSignalText[9]  = '-';
        pSignalText[10] = '-';
        pSignalText[11] = '-';
        pSignalText[12] = '-';
        pSignalText[13] = '-';
        pSignalText[14] = 0x00;
    }
    else
    #endif
    {
        #ifdef HMI_USES_DOUBLE
        if (dataType == ST_DOUBLE)
        {
            pStart = AdjusterBuildNumberStringFromDouble(dataValue.data_D,MaxLen, resolution, &negNumber, &within_over_under);
        }
        else
        #endif
        {
            pStart = AdjusterBuildNumberString(dataValue.data_F, MaxLen, resolution, &negNumber, &within_over_under);
        }

        if ((within_over_under == 1) || (within_over_under == 2))
        {
            if (SignalsViewStyle)
            {
                MaxLen = 8;
            }
            else
            {
                MaxLen = 6;
            }

            for (i = 0; i < MaxLen; i ++)
            {
                Text[i] = 0x05 + within_over_under;
            }

            Text[MaxLen] = 0x00;
        }
        else
        {
            while (*pStart == 0x20)
            {
                #pragma diag_suppress=Pm088
                pStart ++;
                #pragma diag_default=Pm088
            }

            Text[0] = negNumber ? '-' : ' ';
            Text[1] = 0x00;
            (void)strcat((char *)Text,(char *)pStart);
        }

        lenVal = strlen((char *)Text);

        // Add Spaces (if required) between ID and Value
        if (SignalsViewStyle)
        {
            if (lenVal <= 9)
            {
                for (i = 3; i < (13-lenVal); )
                {
                    pSignalText[i] = ' ';
                    i++;
                    pSignalText[i] = 0x00;
                }
            }
            else
            {
                pSignalText[3] = ' ';
                pSignalText[4] = 0x00;
            }
        }
        else
        {
            if (lenVal <= 7)
            {
                for (i = 11; i < (18-lenVal); )
                {
                    pSignalText[i] = ' ';
                    i++;
                    pSignalText[i] = 0x00;
                }
            }
            else
            {
                pSignalText[11] = ' ';
                pSignalText[12] = 0x00;
            }
        }

        (void)strcat((char *)pSignalText, (char *)Text);

        lenVal = strlen((char *)pSignalText);

        if (lenVal < FONT1_MAX_CHARS)	// MASH - check
        {
            // Engineering Units
            strcpy((char *)Text,
                   (const char *)PacketManagerGetEngUnits((TUSIGN16)pSignal->engUnits.ssIdx,
                                                          pSignal->engUnits.objIdx,
                                                          pSignal->engUnits.attribIdx,
                                                          pSignal->engUnitsType));

            #ifdef HMI_SUPPORTS_CUSTOM_UNITS
            PacketManagerRemoveCustomfromUnits(Text);
            #endif

            if (((strlen((char *)Text) < (FONT1_MAX_CHARS-lenVal) + 1)) && (SignalsViewStyle == eTRUE))
            {
                // There is room to add a space between value and eng. units
                (void)strcat((char *)pSignalText, " ");
                lenVal = strlen((char *)pSignalText);
            }

            LimitTextLength(Text,(TUSIGN8)(FONT1_MAX_CHARS-lenVal)); // MASH - check

            (void)strcat((char *)pSignalText, (char *)Text);
        }
    }
}

// No restriction on the optimization of this function
#ifdef HMI_USES_DOUBLE
void PacketManagerBuilderServices_GetBarRange(const SIGNAL_TABLE_TYPE *pSignal, TDOUBLE *pBarHigh, TDOUBLE *pBarLow)
#else
void PacketManagerBuilderServices_GetBarRange(const SIGNAL_TABLE_TYPE *pSignal, TFLOAT *pBarHigh, TFLOAT *pBarLow)
#endif
{
    ADJUST_DATA dataValue;
    TUSIGN8 dataType;

    if (AdjusterGetFloatValue((TUSIGN16)pSignal->valueLower.ssIdx,
                              (TUSIGN16)pSignal->valueLower.objIdx,
                              pSignal->valueLower.attribIdx,
                              &dataValue,
                              &dataType) == OK)
    {
        #ifdef HMI_USES_DOUBLE
        if (dataType == ST_DOUBLE)
        {
            *pBarLow = dataValue.data_D;
        }
        else
        {
            *pBarLow = (TDOUBLE)dataValue.data_F;
        }
        #else
        *pBarLow = dataValue.data_F;
        #endif
    }
    else
    {
        #ifdef HMI_USES_DOUBLE
        *pBarLow = 0.0;
        #else
        *pBarLow = 0.0F;
        #endif
    }

    if (AdjusterGetFloatValue((TUSIGN16)pSignal->valueUpper.ssIdx,
                              (TUSIGN16)pSignal->valueUpper.objIdx,
                              pSignal->valueUpper.attribIdx,
                              &dataValue,
                              &dataType) == OK)
    {
        #ifdef HMI_USES_DOUBLE
        if (dataType == ST_DOUBLE)
        {
            *pBarHigh = dataValue.data_D;
        }
        else
        {
            *pBarHigh = (TDOUBLE)dataValue.data_F;
        }
        #else
        *pBarHigh = dataValue.data_F;
        #endif
    }
    else
    {
        #ifdef HMI_USES_DOUBLE
        *pBarHigh = 100.0;
        #else
        *pBarHigh = 100.0F;
        #endif
    }
}

#if defined(HMI_SUPPORTS_MENU_FRAMES) && defined(HMI_USES_COMMONDIAGNOSTICS)

//TUSIGN8 PacketManagerBuilderServices_FindNextSignal(TUSIGN8 item, TUSIGN8 maxItems)
//{
//    TUSIGN8 returnVal = 0xFF;
//
//    #ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
//    TUSIGN8 SignalsViewEnable = ValidityTestCheck((TUSIGN16)FrameTableGetSignal(++item)->SignalsViewTest);
//    #else
//    TUSIGN8 SignalsViewEnable = FrameTableGetSignal(++item)->SignalsViewEnable;
//    #endif
//
//    while ((item <= maxItems) && (SignalsViewEnable == 0))
//    {
//        item ++;
//        #ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
//        SignalsViewEnable = ValidityTestCheck((TUSIGN16)FrameTableGetSignal(item)->SignalsViewTest);
//        #else
//        SignalsViewEnable = FrameTableGetSignal(item)->SignalsViewEnable;
//        #endif
//    }
//
//    if (item <= maxItems)
//    {
//        returnVal = item;
//    }
//    return returnVal;
//}

void PacketManagerBuilderServices_GetPartialDiagnosticDescription(const TUSIGN8* inStr, TUSIGN8 start, TUSIGN8 outLen, TUSIGN8* outStr)
{
    VIP_ASSERT(start > 0);
    TUSIGN8 inLen = (TUSIGN8)strlen((const char*)inStr);
    TUSIGN8 inIndex = start-1;
    TUSIGN8 outIndex;

    for (outIndex = 0; outIndex < outLen; outIndex ++)
    {
        if (inIndex > inLen)
        {
            outStr[outIndex] = 0x00;
        }
        else
        {
            outStr[outIndex] = inStr[inIndex];
        }
        inIndex++;
    }
    outStr[outLen] = 0x00;
}
#endif  // end of #if defined(HMI_SUPPORTS_MENU_FRAMES) && defined(HMI_USES_COMMONDIAGNOSTICS)


#if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
// No restriction on the optimization of this function
void PacketManagerBuilderServices_GetTrendRange(const SIGNAL_TABLE_TYPE *pSignal, TFLOAT *pBarHigh, TFLOAT *pBarLow)
{
    ADJUST_DATA dataValue, dataHigh, dataLow, dataInc;
    TBOOL   rulesAreActive;
    TUSIGN8 dataType;

    if (AdjusterGetFloatValueAndLimits((TUSIGN16)pSignal->value.ssIdx,
                                       (TUSIGN16)pSignal->value.objIdx,
                                       (TUSIGN8)pSignal->value.attribIdx,
                                       &dataValue,
                                       &dataHigh,
                                       &dataLow,
                                       &dataInc,
                                       &rulesAreActive,
                                       &dataType) == (TUSIGN16)OK)
    {
        #ifdef HMI_USES_DOUBLE
        if (dataType == ST_DOUBLE)
        {
            *pBarLow = (TFLOAT)dataLow.data_D;
            *pBarHigh = (TFLOAT)dataHigh.data_D;
        }
        else
        #endif
        {
            *pBarLow  = (TFLOAT)dataLow.data_F;
            *pBarHigh = (TFLOAT)dataHigh.data_F;
        }
    }
    else
    {
        *pBarLow  = 0.0F;
        *pBarHigh = 100.0F;
    }
}
#endif  // end of #if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)

#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)
TBOOL PacketManagerBuilderServices_GetValueAndMakeString_AutoAdjust(TUSIGN16 ssidx,TUSIGN16 objidx,TUSIGN8 attidx,TUSIGN8 maxChars,TUSIGN8 dpPosn,TUSIGN8 *str)
{
    TBOOL returnVal;
    ADJUST_DATA tVal;
    TUSIGN8 tType;

    TUSIGN8 *pNumber;
    TUSIGN8 negNumber;
    TUSIGN8 within_over_under;

    if (maxChars > 8)    {maxChars = 8;}

    switch (AdjusterGetFloatValue(ssidx,objidx,attidx,&tVal,&tType))
    {
    case OK:
        #ifdef HMI_USES_DOUBLE
        if (tType == ST_DOUBLE)
        {
            pNumber = AdjusterBuildNumberStringFromDouble(tVal.data_D,maxChars,dpPosn,&negNumber,&within_over_under);
        }
        else
        #endif
        {
            pNumber = AdjusterBuildNumberString(tVal.data_F,maxChars,dpPosn,&negNumber,&within_over_under);
        }
        if ((within_over_under == 1) || (within_over_under == 2))
        {
            // overrange or underrange
            TUSIGN8 i;
            str[0] = 0x20;
            for (i = 1; i <= maxChars; )
            {
                str[i] = 0x05 + within_over_under;
                i++;
                str[i] = 0x00;
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

            str[0] = negNumber ? '-' : ' ';
            str[1] = 0x00;
            (void)strcat((char *)str,(char *)pNumber);
        }
        returnVal = eTRUE;
        break;

    case ILLEGAL_ATTRIB_IDX:
        returnVal = eFALSE;
        break;

    default:
        returnVal = eTRUE;
        break;

    }
    return returnVal;
}
#endif // end of #if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)


#ifdef HMI_SUPPORTS_OPERATOR_FRAMES
TUSIGN8 PacketManagerBuilderServices_GetViewIcon(TUSIGN8 frameNo)
{
    TUSIGN8 returnVal = 0;

    #if (defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)) && !defined(DISABLE_OP_PAGE_ID)

    if (frameNo < nVIEW_ICONS)
    {
        returnVal = frameNo;
    }
    #endif

    return returnVal;
}

void PacketManagerBuilderServices_AdjustResolutionForRange(const SIGNAL_TABLE_TYPE *pSignalInTable, TUSIGN8 *pResolution)
{
    if (*pResolution == USE_RANGE)
    {
        if (pSignalInTable->FixedUpperLower == 0)
        {
            #ifdef HMI_USES_DOUBLE
            TDOUBLE High, Low;
            #else
            TFLOAT High, Low;
            #endif

            PacketManagerBuilderServices_GetBarRange(pSignalInTable, &High, &Low);

            if (Low < 0)    {Low *= -1;}
            if (High < 0)   {High *= -1;}
            if (Low > High) {High = Low;}

            if (High < 10)          {*pResolution = 4;}
            else if (High < 100)    {*pResolution = 3;}
            else if (High < 1000)   {*pResolution = 2;}
            else if (High < 10000)  {*pResolution = 1;}
            else                    {*pResolution = 0;}
        }
        else
        {
            *pResolution = 0;
        }
    }
}

// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerBuilderServices_PrepareOperatorPageLineData(TUSIGN8 LineNumber, TUSIGN8 signal_ID, OPERATOR_PAGE_LINE_DATA *LineData, DISPLAYSTYLE Style)
{
    ADJUST_DATA dataValue, HighLimit, LowLimit, Increment;
    TUSIGN8 dataType;
    //T_RANGE_E8 rangeE8;
    TBOOL rules;

    const SIGNAL_TABLE_TYPE *pSignalInTable = FrameTableGetSignal(signal_ID);

    (void)AdjusterGetFloatValueAndLimits((TUSIGN16)pSignalInTable->value.ssIdx,
                                         (TUSIGN16)pSignalInTable->value.objIdx,
                                         pSignalInTable->value.attribIdx,
                                         &dataValue,
                                         &HighLimit,
                                         &LowLimit,
                                         &Increment,
                                         &rules,
                                         &dataType);

    LineData->dataValue = dataValue;
    LineData->dataType = dataType;

    #ifdef DONT_FLASH_OVERRANGE_VALUES
    rules = eFALSE;
    #endif

    if (rules)
    {
        #ifdef HMI_USES_DOUBLE
        if (dataType == ST_DOUBLE)
        {
            LineData->ruleViolation = ((dataValue.data_D > HighLimit.data_D) || (dataValue.data_D < LowLimit.data_D)) ? eTRUE : eFALSE;
        }
        else
        #endif
        {
            LineData->ruleViolation = ((dataValue.data_F > HighLimit.data_F) || (dataValue.data_F < LowLimit.data_F)) ? eTRUE : eFALSE;
        }
    }
    else
    {
        LineData->ruleViolation = eFALSE;
    }

    if (Style >= EURO_1x9)
    {
        LineData->fontSize_V                 = FONT_1;
        LineData->justifyForValues           = JUSTIFY_RIGHT_WITH_INDENT_8;
        LineData->minLengthForValues         = 13;
        LineData->significantDigitsForValues = 12;
    }
    else if (Style >= EURO_1x6_A)
    {	
        LineData->fontSize_V                 = FONT_3;
        LineData->justifyForValues           = JUSTIFY_RIGHT;
        LineData->minLengthForValues         = 7;
        LineData->significantDigitsForValues = 6;
    }
    else if (Style > DISPLAYSTYLE_OFF_3)
    {
        LineData->fontSize_V                 = FONT_4;
        LineData->justifyForValues           = JUSTIFY_RIGHT;
        LineData->minLengthForValues         = 5;
        LineData->significantDigitsForValues = 4;
    }
    else
    {
        VIP_ASSERT(0);
    }

    #pragma diag_suppress=Pm057
    switch (Style)
    {
    case EURO_3x9:
    case EURO_3x9_DIAG:
    case ASIA_3x9:
    case ASIA_3x9_DIAG:
        switch (LineNumber)
        {
        case 3:
            LineData->lineNumber_V = 5;
            LineData->lineNumber_ID = 6;
            break;

        case 2:
            LineData->lineNumber_V = 3;
            LineData->lineNumber_ID = 4;
            break;

        case 1:
        default:
            LineData->lineNumber_V = 1;
            LineData->lineNumber_ID = 2;
            break;
        }

        #ifdef HMI_USES_COMMONDIAGNOSTICS
        if ((Style == EURO_3x9_DIAG) || (Style == ASIA_3x9_DIAG))
        {
            LineData->lineNumber_V --;
            LineData->lineNumber_ID --;
        }
        #endif
        break;

    case EURO_2x9:
    case EURO_2x9_DIAG:
    case ASIA_2x9:
    case ASIA_2x9_DIAG:
        if (LineNumber == 2)
        {
            LineData->lineNumber_V = 4;
            LineData->lineNumber_ID = 5;
        }
        else
        {
            LineData->lineNumber_V = 2;
            LineData->lineNumber_ID = 3;
        }
        break;

    case EURO_2x9_BAR:
    case EURO_2x9_BAR_DIAG:
    case ASIA_2x9_BAR:
    case ASIA_2x9_BAR_DIAG:
        if (LineNumber == 2)
        {
            LineData->lineNumber_V = 3;
            LineData->lineNumber_ID = 4;
        }
        else
        {
            LineData->lineNumber_V = 1;
            LineData->lineNumber_ID = 2;
        }
        break;

    case EURO_1x9:
    case EURO_1x9_DIAG:
    case ASIA_1x9:
    case ASIA_1x9_DIAG:
        LineData->lineNumber_V = 3;
        LineData->lineNumber_ID = 4;
        break;

    case EURO_1x9_BAR:
    case EURO_1x9_BAR_DIAG:
    case ASIA_1x9_BAR:
    case ASIA_1x9_BAR_DIAG:
        LineData->lineNumber_V = 2;
        LineData->lineNumber_ID = 3;
        break;



    case EURO_1x6_B:
    case EURO_1x6_B_DIAG:
    case ASIA_1x6_B:
    case ASIA_1x6_B_DIAG:
        LineData->lineNumber_V  = 2;
        LineData->lineNumber_ID = 5;

        #ifdef HMI_SUPPORTS_ASIAN
        if ((Style == ASIA_1x6_B) || (Style == ASIA_1x6_B_DIAG))
        {
            LineData->lineNumber_V --;
            LineData->lineNumber_ID --;
        }
        #endif
        break;

    case EURO_1x6_A:
    case EURO_1x6_A_DIAG:
    case ASIA_1x6_A:
    case ASIA_1x6_A_DIAG:
    case EURO_1x6_BAR_A:
    case EURO_1x6_BAR_A_DIAG:
    case ASIA_1x6_BAR_A:
    case ASIA_1x6_BAR_A_DIAG:
    case EURO_1x6_BAR_B:
    case EURO_1x6_BAR_B_DIAG:
    case ASIA_1x6_BAR_B:
    case ASIA_1x6_BAR_B_DIAG:
        LineData->lineNumber_V  = 1;
        LineData->lineNumber_ID = 4;

        #ifdef HMI_SUPPORTS_ASIAN
        if ((Style == ASIA_1x6_BAR_B) || (Style == ASIA_1x6_BAR_B_DIAG))
        {
            LineData->lineNumber_V --;
            LineData->lineNumber_ID --;
        }
        #endif
        break;

//    case EURO_1x3:
//    case EURO_1x3_DIAG:
//    case ASIA_1x3:
//    case ASIA_1x3_DIAG:
    case EURO_1x4:
    case EURO_1x4_DIAG:
    case ASIA_1x4:
    case ASIA_1x4_DIAG:
        LineData->lineNumber_V  = 1;
        LineData->lineNumber_ID = 5;
        break;

#ifdef COMMON_TOP_WORKS_NEW_FEATURES
    case EURO_4x9:
    case EURO_4x9_DIAG:
    case ASIA_4x9:
    case ASIA_4x9_DIAG:
        asm("nop");     // data is set up within Build4x9OperatorFrame()
        break;
#endif

    case DISPLAYSTYLE_OFF_0:
    case DISPLAYSTYLE_OFF_1:
    case DISPLAYSTYLE_OFF_2:
    case DISPLAYSTYLE_OFF_3:
    default:
        VIP_ASSERT(0);
   }
   #pragma diag_default=Pm057
}

#endif  // end of #ifdef HMI_SUPPORTS_OPERATOR_FRAMES

#ifdef COMMON_TOP_WORKS_NEW_FEATURES    //SHOW_LOGIN_LEVEL

#define nLOGINICONS    ARM_ACCESS_INVALID + 1
#define LOGIN_ICON_SIZE 16

static const TUSIGN8 LoginIcon_ReadOnly[LOGIN_ICON_SIZE]    = {0x00, 0x00, 0x00, 0x00, 0x70, 0x7E, 0x71, 0x51, 0x71, 0x7E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Standard[LOGIN_ICON_SIZE]    = {0x00, 0x00, 0x00, 0x00, 0x70, 0x72, 0x71, 0x51, 0x71, 0x7E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Advanced[LOGIN_ICON_SIZE]    = {0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x50, 0x70, 0x7E, 0x71, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Service[LOGIN_ICON_SIZE]     = {0x22, 0x63, 0x63, 0x3E, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x7C, 0xC6, 0xC6, 0x44, 0x00};
static const TUSIGN8 LoginIcon_Invalid[LOGIN_ICON_SIZE]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const TUSIGN8 LoginIcon_ReadOnly_T[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x30, 0x30, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Standard_T[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x30, 0x30, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Advanced_T[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Service_T[LOGIN_ICON_SIZE]   = {0x40, 0x60, 0x60, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0x80, 0x00};

static const TUSIGN8 LoginIcon_ReadOnly_B[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x00, 0x3E, 0x3F, 0x3E, 0x22, 0x3E, 0x3F, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Standard_B[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x00, 0x3E, 0x3E, 0x3E, 0x22, 0x3E, 0x3F, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Advanced_B[LOGIN_ICON_SIZE]  = {0x00, 0x00, 0x00, 0x3E, 0x3E, 0x3E, 0x22, 0x3E, 0x3F, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const TUSIGN8 LoginIcon_Service_B[LOGIN_ICON_SIZE]   = {0x04, 0x0C, 0x0C, 0x0F, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x1F, 0x18, 0x18, 0x08, 0x00};

static const TUSIGN8* LoginIcon[nLOGINICONS] =
{
    LoginIcon_Invalid,
    LoginIcon_ReadOnly,
    LoginIcon_Standard,
    LoginIcon_Advanced,
    LoginIcon_Service,
    LoginIcon_Invalid
};

static const TUSIGN8* LoginIcon_T[nLOGINICONS] =
{
    LoginIcon_Invalid,
    LoginIcon_ReadOnly_T,
    LoginIcon_Standard_T,
    LoginIcon_Advanced_T,
    LoginIcon_Service_T,
    LoginIcon_Invalid
};

static const TUSIGN8* LoginIcon_B[nLOGINICONS] =
{
    LoginIcon_Invalid,
    LoginIcon_ReadOnly_B,
    LoginIcon_Standard_B,
    LoginIcon_Advanced_B,
    LoginIcon_Service_B,
    LoginIcon_Invalid
};



TUSIGN8 PacketManagerBuilderServices_ShowLoginIcon(TUSIGN8 IconID, TUSIGN8 lineNumber, TUSIGN8 part)
{
    TUSIGN8 xPos = 1;

    if (IconID > ARM_ACCESS_INVALID)
    {
        IconID = ARM_ACCESS_INVALID;
    }

    if ((lineNumber == 7) || ((lineNumber == 6) && (part != LOGIN_ICON)))
    {
        // if the above test is true then displaying current login level at centre of bottom line

        // Amend xPos
        xPos = (MAX_COMMON_HMI_WIDTH - LOGIN_ICON_SIZE) / 2;

        // Amend Icon to Lock if user lock overrides ARM
        if (WriteProtect_GetUserLock(eFALSE,0,0,0))
        {
            IconID = ARM_ACCESS_READ_ONLY;
        }
    }


    const TUSIGN8 *pIcon;
    switch (part)
    {
    case LOGIN_ICON_B:  pIcon = LoginIcon_B[IconID];    break;
    case LOGIN_ICON_T:  pIcon = LoginIcon_T[IconID];    break;
    default:            pIcon = LoginIcon[IconID];      break;
    }

    return BuildWriteArray(pIcon, LOGIN_ICON_SIZE, xPos, lineNumber, 0);
}

#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES

const TUSIGN8 plotPattern[48][6] =
{
  //Row
  //  0     1     2     3     4     5       // PlotValue
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   //  <= 0
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x40},   //     1
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x20},   //     2
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x10},   //     3
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x08},   //     4
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x04},   //     5
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x02},   //     6
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},   //     7
    {0x00, 0x00, 0x00, 0x00, 0x80, 0x00},   //     8
    {0x00, 0x00, 0x00, 0x00, 0x40, 0x00},   //     9
    {0x00, 0x00, 0x00, 0x00, 0x20, 0x00},   //    10
    {0x00, 0x00, 0x00, 0x00, 0x10, 0x00},   //    11
    {0x00, 0x00, 0x00, 0x00, 0x08, 0x00},   //    12
    {0x00, 0x00, 0x00, 0x00, 0x04, 0x00},   //    13
    {0x00, 0x00, 0x00, 0x00, 0x02, 0x00},   //    14
    {0x00, 0x00, 0x00, 0x00, 0x01, 0x00},   //    15
    {0x00, 0x00, 0x00, 0x80, 0x00, 0x00},   //    16
    {0x00, 0x00, 0x00, 0x40, 0x00, 0x00},   //    17
    {0x00, 0x00, 0x00, 0x20, 0x00, 0x00},   //    18
    {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},   //    19
    {0x00, 0x00, 0x00, 0x08, 0x00, 0x00},   //    20
    {0x00, 0x00, 0x00, 0x04, 0x00, 0x00},   //    21
    {0x00, 0x00, 0x00, 0x02, 0x00, 0x00},   //    22
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00},   //    23
    {0x00, 0x00, 0x80, 0x00, 0x00, 0x00},   //    24
    {0x00, 0x00, 0x40, 0x00, 0x00, 0x00},   //    25
    {0x00, 0x00, 0x20, 0x00, 0x00, 0x00},   //    26
    {0x00, 0x00, 0x10, 0x00, 0x00, 0x00},   //    27
    {0x00, 0x00, 0x08, 0x00, 0x00, 0x00},   //    28
    {0x00, 0x00, 0x04, 0x00, 0x00, 0x00},   //    29
    {0x00, 0x00, 0x02, 0x00, 0x00, 0x00},   //    30
    {0x00, 0x00, 0x01, 0x00, 0x00, 0x00},   //    31
    {0x00, 0x80, 0x00, 0x00, 0x00, 0x00},   //    32
    {0x00, 0x40, 0x00, 0x00, 0x00, 0x00},   //    33
    {0x00, 0x20, 0x00, 0x00, 0x00, 0x00},   //    34
    {0x00, 0x10, 0x00, 0x00, 0x00, 0x00},   //    35
    {0x00, 0x08, 0x00, 0x00, 0x00, 0x00},   //    36
    {0x00, 0x04, 0x00, 0x00, 0x00, 0x00},   //    37
    {0x00, 0x02, 0x00, 0x00, 0x00, 0x00},   //    38
    {0x00, 0x01, 0x00, 0x00, 0x00, 0x00},   //    39
    {0x80, 0x00, 0x00, 0x00, 0x00, 0x00},   //    40
    {0x40, 0x00, 0x00, 0x00, 0x00, 0x00},   //    41
    {0x20, 0x00, 0x00, 0x00, 0x00, 0x00},   //    42
    {0x10, 0x00, 0x00, 0x00, 0x00, 0x00},   //    43
    {0x08, 0x00, 0x00, 0x00, 0x00, 0x00},   //    44
    {0x04, 0x00, 0x00, 0x00, 0x00, 0x00},   //    45
    {0x02, 0x00, 0x00, 0x00, 0x00, 0x00},   //    46
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // >= 47
};

void Set_NumericField(TFLOAT value, TUSIGN8 dps, TUSIGN8* field, TUSIGN8 fieldSize, TBOOL V_ScaleStyle)
{
    const TUSIGN8 WaveformFont_3Byte[15][4] =
    {
        {3, 0x7E, 0x42, 0x7E}, // 0        // WAVEFORMFONT_0
        {3, 0x44, 0x7E, 0x40}, // 1        // WAVEFORMFONT_1
        {3, 0x7A, 0x4A, 0x4E}, // 2        // WAVEFORMFONT_2
        {3, 0x42, 0x4A, 0x7E}, // 3        // WAVEFORMFONT_3
        {3, 0x1E, 0x10, 0x7E}, // 4        // WAVEFORMFONT_4
        {3, 0x4E, 0x4A, 0x7A}, // 5        // WAVEFORMFONT_5
        {3, 0x7E, 0x4A, 0x7A}, // 6        // WAVEFORMFONT_6
        {3, 0x02, 0x7A, 0x06}, // 7        // WAVEFORMFONT_7
        {3, 0x7E, 0x4A, 0x7E}, // 8        // WAVEFORMFONT_8
        {3, 0x5E, 0x52, 0x7E}, // 9        // WAVEFORMFONT_9
        {1, 0x10, 0x00, 0x00}, // .        // WAVEFORMFONT_DP
        {2, 0x08, 0x08, 0x00}, // -        // WAVEFORMFONT_MINUS
        {3, 0x06, 0x52, 0x0E}, // ?        // WAVEFORMFONT_QUESTION
        {3, 0x04, 0x7E, 0x04}, // ^        // WAVEFORMFONT_ARROW
        {3, 0x00, 0x00, 0x00}, //          // WAVEFORMFONT_SPACE
    };

    const TUSIGN8 WaveformFont_5Byte[15][6] =
    {
        {5, 0x3C, 0x42, 0x42, 0x42, 0x3C}, // 0        // WAVEFORMFONT_0
        {3, 0x44, 0x7E, 0x40, 0x00, 0x00}, // 1        // WAVEFORMFONT_1
        {5, 0x44, 0x62, 0x52, 0x4A, 0x44}, // 2        // WAVEFORMFONT_2
        {5, 0x24, 0x42, 0x4A, 0x4A, 0x34}, // 3        // WAVEFORMFONT_3
        {5, 0x18, 0x14, 0x12, 0x7E, 0x10}, // 4        // WAVEFORMFONT_4
        {5, 0x2E, 0x4A, 0x4A, 0x4A, 0x32}, // 5        // WAVEFORMFONT_5
        {5, 0x3C, 0x4A, 0x4A, 0x4A, 0x30}, // 6        // WAVEFORMFONT_6
        {5, 0x02, 0x62, 0x12, 0x0A, 0x06}, // 7        // WAVEFORMFONT_7
        {5, 0x34, 0x4A, 0x4A, 0x4A, 0x34}, // 8        // WAVEFORMFONT_8
        {5, 0x0C, 0x52, 0x52, 0x52, 0x3C}, // 9        // WAVEFORMFONT_9
        {2, 0x30, 0x30, 0x00, 0x00, 0x00}, // .        // WAVEFORMFONT_DP
        {4, 0x08, 0x08, 0x08, 0x08, 0x00}, // -        // WAVEFORMFONT_MINUS
        {5, 0x04, 0x02, 0x52, 0x0A, 0x04}, // ?        // WAVEFORMFONT_QUESTION
        {5, 0x08, 0x0C, 0x7E, 0x0C, 0x08}, // ^        // WAVEFORMFONT_ARROW
        {5, 0x00, 0x00, 0x00, 0x00, 0x00}, //          // WAVEFORMFONT_SPACE
    };


    typedef enum
    {
        WAVEFORMFONT_0,         //  0 = 0
        WAVEFORMFONT_1,         //  1 = 1
        WAVEFORMFONT_2,         //  2 = 2
        WAVEFORMFONT_3,         //  3 = 3
        WAVEFORMFONT_4,         //  4 = 4
        WAVEFORMFONT_5,         //  5 = 5
        WAVEFORMFONT_6,         //  6 = 6
        WAVEFORMFONT_7,         //  7 = 7
        WAVEFORMFONT_8,         //  8 = 8
        WAVEFORMFONT_9,         //  9 = 9
        WAVEFORMFONT_DP,        // 10 = .
        WAVEFORMFONT_MINUS,     // 11 = -
        WAVEFORMFONT_QUESTION,  // 12 = ?
        WAVEFORMFONT_ARROW,     // 13 = up arrow
        WAVEFORMFONT_SPACE      // 14 = space
    }WAVEFORMFONT_TYPE;

    TFLOAT multiplier;
    TUSIGN8 Range1000, Range100, Range10, Range1;
    TBOOL negVal = eFALSE;
    TUSIGN8 text[5];

    TINT16 PosLimit = dps ? 999 : 9999;
    TINT16 NegLimit = 999;

    switch (dps)
    {
    case 2:     multiplier = 100;   break;
    case 1:     multiplier = 10;    break;
    case 0:     multiplier = 1;     break;
    default:    multiplier = 1;     break;
    }

    if (value < 0.0f)
    {
        negVal = eTRUE;
        value *= -1;
    }

    TINT16 iValue = (TINT16)((value * multiplier) + 0.5F);

    TBOOL overrange;

    if (negVal) {overrange = (iValue > NegLimit) ? eTRUE : eFALSE;}
    else        {overrange = (iValue > PosLimit) ? eTRUE : eFALSE;}

    if (overrange)
    {
        memset((void *)text, V_ScaleStyle ? WAVEFORMFONT_SPACE : WAVEFORMFONT_ARROW, 5);
    }
    else
    {
        Range1000 = iValue / 1000;
        iValue = iValue % 1000;
        Range100 = iValue / 100;
        iValue = iValue % 100;
        Range10 = iValue / 10;
        Range1 = iValue % 10;

        switch (dps)
        {
        case 2:
            text[4] = Range1;
            text[3] = Range10;
            text[2] = WAVEFORMFONT_DP;
            text[1] = Range100;
            text[0] = ((Range1000 > 0)) ? Range1000 : WAVEFORMFONT_SPACE;
            break;

        case 1:
            text[4] = Range1;
            text[3] = WAVEFORMFONT_DP;
            text[2] = Range10;
            text[1] = ((Range1000 > 0) || (Range100 > 0)) ? Range100 : WAVEFORMFONT_SPACE;
            text[0] = ((Range1000 > 0)) ? Range1000 : WAVEFORMFONT_SPACE;
            break;

        case 0:
        default:
            text[4] = Range1;
            text[3] = ((Range1000 > 0) || (Range100 > 0) || (Range10 > 0)) ? Range10 : WAVEFORMFONT_SPACE;
            text[2] = ((Range1000 > 0) || (Range100 > 0)) ? Range100  : WAVEFORMFONT_SPACE;
            text[1] = ((Range1000 > 0)) ? Range1000 : WAVEFORMFONT_SPACE;
            text[0] = WAVEFORMFONT_SPACE;
            break;
        }
        if (negVal)
        {
            TUSIGN8 count = 0;

            while (text[count + 1] == WAVEFORMFONT_SPACE)
            {
                count ++;
            }

            if (text[count] == WAVEFORMFONT_SPACE)
            {
                text[count] = WAVEFORMFONT_MINUS;
            }
            else
            {
                memset((void *)text,WAVEFORMFONT_QUESTION,5);
            }
        }
    }

    TINT16  FieldIndex = (TINT16)fieldSize-1;
    TINT8  TextIndex;
    TUSIGN8 fontColumnIndex;


    for (TextIndex = 4; TextIndex >= 0; TextIndex --)
    {
        if (FieldIndex >= 0)
        {
            field[FieldIndex] = 0x00;
            FieldIndex --;
        }

        if (1) //(V_ScaleStyle)
        {
            for (fontColumnIndex = WaveformFont_3Byte[text[TextIndex]][0]; fontColumnIndex > 0; fontColumnIndex --)
            {
                if (FieldIndex >= 0)
                {
                    field[FieldIndex] = WaveformFont_3Byte[text[TextIndex]][fontColumnIndex];
                    FieldIndex --;
                }
            }
        }
        else
        {
            for (fontColumnIndex = WaveformFont_5Byte[text[TextIndex]][0]; fontColumnIndex > 0; fontColumnIndex --)
            {
                if (FieldIndex >= 0)
                {
                    field[FieldIndex] = WaveformFont_5Byte[text[TextIndex]][fontColumnIndex];
                    FieldIndex --;
                }
            }
        }
    }

    while (FieldIndex >= 0)
    {
        field[FieldIndex] = 0x00;
        FieldIndex --;
    }
}

void AddVerticalScale(TUSIGN8 *pDest, TUSIGN8 row, TUSIGN8 column)
{
    const TUSIGN8 VerticalScalePattern[6][2] =
    {
        // column
        //    1              // Row
        {0x02,0x6E}, // WAVEFORM_FRAME_ROW_0
        {0x00,0xDB}, // WAVEFORM_FRAME_ROW_1
        {0x00,0xB6}, // WAVEFORM_FRAME_ROW_2
        {0x00,0x6D}, // WAVEFORM_FRAME_ROW_3
        {0x00,0xDB}, // WAVEFORM_FRAME_ROW_4
        {0x40,0x76}  // WAVEFORM_FRAME_ROW_5
    };

    if (column == WAVEFORM_FRAME_COL_0)
    {
        pDest[14] |= VerticalScalePattern[row][0];
        pDest[15] |= VerticalScalePattern[row][1];
    }

    if (column == WAVEFORM_FRAME_COL_4)
    {
        pDest[15] |= VerticalScalePattern[row][1];
        pDest[16] |= VerticalScalePattern[row][0];
    }
}

void AddMajorDivisions(TUSIGN8 *pDest, TUSIGN8 nDivisions, TUSIGN8 zeroPosn, TUSIGN8 row, TUSIGN8 column)
{

    const TUSIGN8 majorDivisionValue[8][8] =
    {
      //Division Number
      //  0   1   2   3   4   5   6   7     // Number of Divisions
        { 0,  0,  0,  0,  0,  0,  0,  0},   // 0
        { 1, 46,  0,  0,  0,  0,  0,  0},   // 1
        { 1, 23, 46,  0,  0,  0,  0,  0},   // 2
        { 1, 16, 31, 46,  0,  0,  0,  0},   // 3
        { 1, 12, 23, 35, 46,  0,  0,  0},   // 4
        { 1, 10, 19, 28, 37, 46,  0,  0},   // 5
        { 1,  8, 16, 23, 31, 38, 46,  0},   // 6
        { 1,  7, 14, 20, 27, 33, 40, 46}    // 7
    };

    TUSIGN8 ZeroPosnStart;
    TUSIGN8 DivisionNumber;

    TUSIGN8 zeroPosnPattern = plotPattern[zeroPosn][row];

    TUSIGN8 majorDivPattern = 0;
    TUSIGN8 majorDivValue;

    for (DivisionNumber = 0; DivisionNumber <= nDivisions; DivisionNumber ++)
    {
        majorDivValue = majorDivisionValue[nDivisions][DivisionNumber];
        majorDivPattern |= plotPattern[majorDivValue][row];
    }

    switch (column)
    {
    case WAVEFORM_FRAME_COL_0:
        // zero position dots at 5,7,9,11,13,15,17,19,21,23
        ZeroPosnStart = ((row == WAVEFORM_FRAME_ROW_0) || (row == WAVEFORM_FRAME_ROW_5)) ? 15 : 5;
        break;

    case WAVEFORM_FRAME_COL_1:
        // Major division dots at 0, 10, 20
        pDest[0] |= majorDivPattern;
        pDest[10] |= majorDivPattern;
        pDest[20] |= majorDivPattern;
        // zero position dots at 0,2,4,6,8,10,12,14,16,18,20,22,24
        ZeroPosnStart = 0;
        break;

    case WAVEFORM_FRAME_COL_2:
        // Major division dots at 5, 15
        pDest[5] |= majorDivPattern;
        pDest[15] |= majorDivPattern;
        // zero position dots at 1,3,5,7,9,11,13,15,17,19,21,23
        ZeroPosnStart = 1;
        break;

    case WAVEFORM_FRAME_COL_3:
        // Major division dots at 0, 10, 20
        pDest[0] |= majorDivPattern;
        pDest[10] |= majorDivPattern;
        pDest[20] |= majorDivPattern;
        // zero position dots at 0,2,4,6,8,10,12,14,16,18,20,22,24
        ZeroPosnStart = 0;
        break;

    case WAVEFORM_FRAME_COL_4:
        // Major division dots at 5
        pDest[5] |= majorDivPattern;
        // zero position dots at 1,3,5,7,9,11,13,15,17,19,21,23
        ZeroPosnStart = 1;
        break;

    default:
        break;
    }

    TUSIGN8 i;

    for (i = ZeroPosnStart; i < WAVEFORM_PLOT_AREA_SIZE; i += 2)
    {
            pDest[i] |= zeroPosnPattern;
    }
}

void plotMerge(TUSIGN8 *pDest, TUSIGN8 *pSource, TUSIGN8 n)
{
    TUSIGN8 c;

    for (c = 0; c < n; c ++)
    {
        pDest[c] |= pSource[c];
    }
}

void AddPlots(TUSIGN8 *pDest, const TUSIGN8 *pData, TUSIGN8 row, TUSIGN8 column)
{
    TUSIGN8 DataIndexStart, DataIndexEnd, DestIndex, DataIndex;
    TUSIGN8 NewPlotPosition, OldPlotPosition;
    TUSIGN8 pattern;

    switch (column)
    {
    case WAVEFORM_FRAME_COL_4:   DataIndexStart = 95;   DataIndexEnd = 120;     DestIndex = 0;     break;
    case WAVEFORM_FRAME_COL_3:   DataIndexStart = 70;   DataIndexEnd = 95;      DestIndex = 0;     break;
    case WAVEFORM_FRAME_COL_2:   DataIndexStart = 45;   DataIndexEnd = 70;      DestIndex = 0;     break;
    case WAVEFORM_FRAME_COL_1:   DataIndexStart = 20;   DataIndexEnd = 45;      DestIndex = 0;     break;
    case WAVEFORM_FRAME_COL_0:   DataIndexStart = 0;    DataIndexEnd = 20;      DestIndex = 5;     break;
    default:                     DataIndexStart = 0;    DataIndexEnd = 20;      DestIndex = 5;     break;
    }

//    printf("Column = %d, Row = %d\r\nDataIndexStart = %d\r\nData = %d,%d,%d,%d,%d\r\n",
//    (int)column,//____|         |                      |            |  |  |  |  |
//    (int)row,//_________________|                      |            |  |  |  |  |
//    (int)DataIndexStart,//_____________________________|            |  |  |  |  |
//    (int) pData[DataIndexStart],//__________________________________|  |  |  |  |
//    (int) pData[DataIndexStart+1],//___________________________________|  |  |  |
//    (int) pData[DataIndexStart+2],//______________________________________|  |  |
//    (int) pData[DataIndexStart+3],//_________________________________________|  |
//    (int) pData[DataIndexStart+4] //____________________________________________|
//    );

    for (DataIndex = DataIndexStart; DataIndex < DataIndexEnd; DataIndex++)
    {
        pattern = 0;
        NewPlotPosition = pData[DataIndex];

        if (DataIndex == 0)
        {
            OldPlotPosition = NewPlotPosition;
        }
        else
        {
            OldPlotPosition = pData[DataIndex - 1];
        }

        if (NewPlotPosition > OldPlotPosition + 1)
        {
            // step change up - set every pixel from 1 above old position up to and including the new position
            TINT16 PlotPosition;
            for (PlotPosition = OldPlotPosition + 1; PlotPosition <= NewPlotPosition; PlotPosition ++)
            {
                pattern |= plotPattern[PlotPosition][row];
            }
        }
        else if (NewPlotPosition < OldPlotPosition - 1)
        {
            // step change down - set every pixel from 1 below old position down to and including the new position...
            TINT16 PlotPosition;
            for (PlotPosition = OldPlotPosition - 1; PlotPosition >= NewPlotPosition; PlotPosition --)
            {
                pattern |= plotPattern[PlotPosition][row];
            }
        }
        else
        {
            // slow ramp or steady signal - set pixel relating to new position
            pattern |= plotPattern[NewPlotPosition][row];
        }

        if (DestIndex < WAVEFORM_PLOT_AREA_SIZE)
        {
            pDest[DestIndex] |= pattern;
            DestIndex ++;
        }
    }
}

    const TUSIGN8 PlotData1[120] = {  13,12,12,12,13,14,16,20,23,26,29,33,39,40,41,40,38,35,31,27,
                                      23,20,19,20,22,25,26,26,25,22,19,16,15,14,14,14,15,15,15,15,
                                      15,16,16,16,16,16,17,17,17,17,17,17,17,18,18,19,19,19,20,20,
                                      20,20,20,20,19,19,19,19,18,18,18,17,17,17,17,16,16,16,15,15,
                                      15,16,18,19,20,20,20,20,20,19,18,18,17,17,17,17,17,18,18,19,
                                      20,21,21,22,22,23,26,27,29,32,34,35,35,34,32,30,26,23,20,19};

    const TUSIGN8 PlotData2[120] = {  47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,
                                      47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,26,26,26,26,
                                      26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
                                      26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
                                      26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
                                      26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26};

    const TUSIGN8 PlotData3[120] = {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 6, 7,
                                       7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

const TUSIGN8* GetPlotData(TUSIGN8 plotNumber)
{


    const TUSIGN8* returnPtr = NULL;
    switch (plotNumber)
    {
    case 1:     returnPtr = PlotData1;  break;
    case 2:     returnPtr = PlotData2;  break;
    case 3:     returnPtr = PlotData3;  break;
    default:    returnPtr = NULL;       break;
    }
    return returnPtr;
}
#endif // end of #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES


#ifdef COMMON_TOP_WORKS_NEW_FEATURES    //HMI_SUPPORTS_INDICATOR_ONLY
TBOOL PacketManagerBuilderServices_IndicatorOnly(void)
{
    TBOOL IndicatorOnly = eFALSE;

    #ifdef TYPE_1_IMPLEMENTATION
        #ifdef HMI_SUPPORTS_SERVICEPORT
        TUSIGN8 HardwareType = PacketManagerLocal_1.DisplayHardwareRevision;
        #else
        TUSIGN8 HardwareType = LOCAL_DisplayHardwareRevision;
        #endif

        if ((HardwareType == HMI_HW_NORMAL_A_INDICATOR_ONLY)    // Size A indicator without eeprom
         || (HardwareType == HMI_HW_EEPROM_A_INDICATOR_ONLY)    // Size A indicator with eeprom     or  Size A Indicator with MSP430F550x (HARDWARE_VERSION_SIZE_A_IND)
         || (HardwareType == HMI_HW_NORMAL_B_INDICATOR_ONLY)    // Size B indicator without eeprom
         || (HardwareType == HMI_HW_EEPROM_B_INDICATOR_ONLY))   // Size B indicator with eeprom     or  Size B Indicator with MSP430F550x (HARDWARE_VERSION_SIZE_B_IND)
        {
            IndicatorOnly = eTRUE;
        }
#endif

    return IndicatorOnly;
}
#endif


