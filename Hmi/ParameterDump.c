
#pragma diag_suppress=Go003

#include "IncludeList_Implementation.h"

#include "ParameterDump.h"

#ifdef HMI_SUPPORTS_PARAMETER_DUMP

#include <intrinsics.h>
#include <math.h>
#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif

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

#include "FixedIconDefinitions.h"


#include "Adjuster.h"
#include "PacketManager.h"

//------------------------------------------------------------------
// ParameterDump.c

// ------------------- Service port interface -----------------

#include "../../ServicePort/interface/SpClientInfo.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Interface/ServicePort_idx.h"

// Local Data
#define LINELENGTH 128
TUSIGN8 parameterDumpOutputLine[LINELENGTH];
TBOOL readyForNextLine = eFALSE;
TBOOL RegisteredWithServicePort = eFALSE;

// Functions
TBOOL ParameterDump_SP_RegisterClientEXE(void);

void ParameterDump_OutputLine(void);
void SP_CallbackParameterDumpDataReceived(void);
void SP_CallbackParameterDumpTransmissionComplete(TUSIGN8 *pMsg);
void SP_CallbackParameterDumpCommsError(TSpCommsError err);
void SP_CallbackParameterDumpConnected(void);
void SP_CallbackParameterDumpDisconnected(void);

#pragma diag_suppress=Pm022
#pragma diag_suppress=Pm023

TSpCallbacks SP_CallbacksParameterDump =
{
    SP_CallbackParameterDumpDataReceived,           // evtDataReceived;            typedef void (*TCallbackDataReceived)(void)
    SP_CallbackParameterDumpTransmissionComplete,   // evtTransmissionComplete;    typedef void (*TCallbackTransmissionComplete)(TUSIGN8 *pMsg)
    SP_CallbackParameterDumpCommsError,             // evtCommsError;              typedef void (*TCallbackCommsError)(TSpCommsError err)
    NULL,                                           // evtTxDataFlowHalted;        typedef void (*TCallbackTxDataFlowHalted)(void)
    NULL,                                           // evtTxDataFlowResumed;       typedef void (*TCallbackTxDataFlowResumed)(void)
    NULL,                                           // evtRxDataFlowHalted;        typedef void (*TCallbackRxDataFlowHalted)(void)
    NULL,                                           // evtRxDataFlowResumed;       typedef void (*TCallbackRxDataFlowResumed)(void)
    SP_CallbackParameterDumpConnected,              // evtConnected;               typedef void (*TCallbackConnected)(void)
    SP_CallbackParameterDumpDisconnected,           // evtDisconnected;            typedef void (*TCallbackDisconnected)(void)
    NULL,                                           // evtPoweredUp;               typedef void (*TCallbackPoweredUp)(void)
    NULL,                                           // evtPoweredDown;             typedef void (*TCallbackPoweredDown)(void)
};

#define PARAMETER_DUMP_CLIENT_IDENTITY 7 //
TCHAR ParameterDump_ClientProtocolName[] = {"Parameter Dump SP Client"} ;
TSpClientInfo SP_ClientInfoParameterDump = CONSTRUCT_SP_CLIENTINFO(PARAMETER_DUMP_CLIENT_IDENTITY, (TSpCallbacks*)&SP_CallbacksParameterDump, ParameterDump_ClientProtocolName);

#pragma diag_default=Pm022
#pragma diag_default=Pm023


// -------------------- Configuration Dump --------------------
// Local Data
TBOOL ConfigDumpActive;
FRAME_FORMAT_TYPE const *ConfigDumpCurrentFrame;
TUSIGN8 ConfigDumpCurrentCursor;
TUSIGN8 ConfigDumpCursorLimit;

FRAME_MEMORY_ENTRY  ConfigDumpFrameMemory[FRAME_MEMORY_SIZE];
TUSIGN8 ConfigDumpFrameMemoryPointer;
TBOOL ConfigDumpPulledMenu;
TBOOL ConfigDumpPulledCFH;

// Functions
void ConfigDumpStart(void);
void ConfigDumpExecute(void);
void ConfigDumpNextFrame(void);
void ConfigDumpFrameMemory_Push(void);
void ConfigDumpFrameMemory_Pull(void);
void ConfigDumpProcessFrame(void);


// -------------------- Information Dump --------------------
void InfoDumpStart(void);
void InfoDumpExecute(void);

// --- Status Dump ---
// Local Data

TBOOL StatusDumpActive;

// Functions
void InfoDumpProcessStatus(void);

TUSIGN8 numberText[24];

// --- Signals Dump ---
// Local Data
TBOOL   SignalsDumpActive;
TUSIGN8 SignalsDumpCurrentSignal;

// Functions
void InfoDumpNextSignal(void);
void InfoDumpProcessSignal(void);

// --- Diagnostic History Dump ---
// Local Data

TBOOL HistoryDumpActive;

#ifdef HMI_USES_COMMONDIAGNOSTICS
TUSIGN8 HistoryDumpCurrentAlarm;
TUSIGN8 HistoryDumpMaxAlarms;
TUSIGN8 HistoryDumpProcessStage;
#endif // HMI_USES_COMMONDIAGNOSTICS

// Functions
#ifdef HMI_USES_COMMONDIAGNOSTICS
void HistoryDumpProcessAlarm(void);
void HistoryDumpNextAlarm(void);
#endif // HMI_USES_COMMONDIAGNOSTICS

// -------------------- Invalid Command Reply Dump --------------------
// Local Data
TBOOL InvalidCommandActive;

// Functions
void InvalidCommandDumpStart(void);
void InvalidCommandDumpExecute(void);

// -------------------- General --------------------
const TUSIGN8 space[2]     = {0x20, 0x00};        // " "
const TUSIGN8 delimiter[2] = {0x3B, 0x00};        // ";"
const TUSIGN8 newline[3]   = {0x0D, 0x0A, 0x00};  // "\r\n"

TUSIGN8 *ConfigDumpBuildNumberString(TFLOAT value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under);
#ifdef HMI_USES_DOUBLE
TUSIGN8 *ConfigDumpBuildNumberStringFromDouble(TDOUBLE value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under);
#endif //HMI_USES_DOUBLE

// -------------------- Configuration Dump --------------------

void ConfigDumpStart(void)
{
    ConfigDumpCurrentFrame = FrameTableGetConfigHeaderAddress(0);
    ConfigDumpCurrentCursor = 0;

    for (ConfigDumpFrameMemoryPointer = 0; ConfigDumpFrameMemoryPointer < FRAME_MEMORY_SIZE; ConfigDumpFrameMemoryPointer++)
    {
        ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].pFrame = NULL;
        ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].cursor = 0xFF;
    }
    ConfigDumpFrameMemoryPointer = 0;

    ConfigDumpActive = eTRUE;
    ConfigDumpPulledMenu = eFALSE;
    ConfigDumpPulledCFH = eFALSE;

    readyForNextLine = eTRUE;
}

void ConfigDumpExecute(void)
{
    if (ConfigDumpActive && readyForNextLine)
    {
        ConfigDumpProcessFrame();

        do
        {
            ConfigDumpNextFrame();
        } while (ConfigDumpActive && (ConfigDumpPulledMenu || ConfigDumpPulledCFH));
    }
}

void ConfigDumpProcessFrame(void)
{
    TUSIGN8 Field[33];
//  TUSIGN8 Line[60];
    TUSIGN8 i,len;

//    ATTRIB_IDENTIFIER_TYPE valueID;
    ADJUST_DATA tVal, tHigh, tLow, tInc;
    TUSIGN8 tType;
//    T_RANGE_E8      enumRange;
    TBOOL           rulesAreActive;


    TUSIGN8 negNumber;
    TUSIGN8 within_over_under;
    TUSIGN8 *pNumber;

    #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
    TUSIGN32 TimeDateValue;
    TIMEANDDATE convertedTimeDate;
    #endif // HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES

    #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
    TUSIGN32       IPAddress;
    IP_ADDRESS_HMI IP_Addr_union;
    #endif // HMI_SUPPORTS_IP_ADDRESS_FRAMES


    TEXT_IDENTIFIER_TYPE stringtable_text_id = {HMI_IDX, SS_HMI_OBJ_STRINGTABLE, IDS_BLANK};


    // Blank output line
    for (i = 0; i < LINELENGTH; i++)
    {
        parameterDumpOutputLine[i] = 0x00;
    }

    // Add Indent(s)
    for (i = 0; i < ConfigDumpFrameMemoryPointer; i ++)
    {
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
    }

    switch (ConfigDumpCurrentFrame->frameType)
    {
    case FRAME_TYPE_CONFIG_HEADER:
        // <Frame Title><New Line>
        // <Frame Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.configHeaderFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        break;

    #ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
    case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
    case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:
        // "<Parameter Title><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.graphParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        break;
    #endif

    case FRAME_TYPE_NUMERICAL_PARAM:
    case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
        #ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
        // "<Parameter Title><delimiter><value><space><units><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        if (AdjusterGetFloatValue((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.ssIdx,
                                  (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.objIdx,
                                  ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.attribIdx,
                                  &tVal,
                                  &tType) == OK)
        {
            #ifdef HMI_USES_DOUBLE
            if (tType == ST_DOUBLE)
            {
                pNumber = ConfigDumpBuildNumberStringFromDouble(tVal.data_D,
                                                                ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                                ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                                &negNumber,
                                                                &within_over_under);
            }
            else
            #endif // HMI_USES_DOUBLE
            {
                pNumber = ConfigDumpBuildNumberString(tVal.data_F,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.attributes.maxCharacters,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn,
                                                      &negNumber,
                                                      &within_over_under);
            }
            if (within_over_under == 1)
            {
                Field[0] = '>';
                Field[1] = 0x00;
                (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
            }
            else
            {
                if (within_over_under == 2)
                {
                    Field[0] = '<';
                    Field[1] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
            }
            if (negNumber)
            {
                Field[0] = '-';
                Field[1] = 0x00;
                (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
            }
            (void)strcat((char *)parameterDumpOutputLine,(const char *)pNumber);
        }
        else
        {
            Field[0] = '?';
            Field[1] = 0x00;
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }

        // <space>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)space);

        // <units>
        strcpy((char *)Field,
               (const char *)PacketManagerGetEngUnits((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.ssIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.objIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.attribIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnitsType)
               );
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
        break;


    case FRAME_TYPE_INTEGER_PARAM:
    case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
        #ifdef HMI_SUPPORTS_INTEGER_FRAMES
        // "<Parameter Title><delimiter><value><space><units><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        {
            TUSIGN32 Value32 = 0;
            T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.ssIdx);

            if (pSub->Get(pSub,
                          (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.objIdx,
                          (TINT16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.value.attribIdx,
                          (void FAST *)&Value32) == (TUSIGN16)OK)
            {
                pNumber = AdjusterBuildNumberStringFromUSIGN32(Value32, ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.attributes, &within_over_under);

                if (within_over_under == 1)
                {
                    Field[0] = '>';
                    Field[1] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                else
                {
                    if (within_over_under == 2)
                    {
                        Field[0] = '<';
                        Field[1] = 0x00;
                        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                    }
                }
                (void)strcat((char *)parameterDumpOutputLine,(const char *)pNumber);
            }
            else
            {
                Field[0] = '?';
                Field[1] = 0x00;
                (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
            }
        }
        // <space>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)space);

        // <units>
        strcpy((char *)Field,
               (const char *)PacketManagerGetEngUnits((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.ssIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.objIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnits.attribIdx,
                                                      ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.engUnitsType));

        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();

        #endif // HMI_SUPPORTS_INTEGER_FRAMES
        break;

    case FRAME_TYPE_BIT_ENUMERATED_PARAM:
    case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
        #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        // "<Parameter Title><delimiter><value><New Line>"
        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        if (AdjusterGetFloatValue((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.value.ssIdx,
                                  (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.value.objIdx,
                                  ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.value.attribIdx,
                                  &tVal,
                                  &tType) == OK)
        {
            TUSIGN8 b;
            TUSIGN8 bitEnumValue = tVal.data_8;

            TUSIGN8 mask_LSB_First = 0x01;
            TUSIGN8 mask_MSB_First = 0x01 << (ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.numberOfBits - 1);

            for (b = 0; b <= 8; b++)
            {
                if (b >= ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.numberOfBits)
                {
                    Field[b] = 0x00;
                }
                else
                {
                    switch (ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.displayStyle)
                    {
                    case 3:
                        if ((bitEnumValue & mask_LSB_First) == 0)
                        {
                            Field[b] = '-';
                        }
                        else
                        {
                            Field[b] = '1' + i;
                        }
                        break;

                    case 2:
                        if ((bitEnumValue & mask_MSB_First) == 0)
                        {
                            Field[b] = '-';
                        }
                        else
                        {
                            Field[b] = (ConfigDumpCurrentFrame->frameDefinition.bitEnumParamFrame.numberOfBits - b) + '0';
                        }
                        break;

                    case 1:
                        if ((bitEnumValue & mask_MSB_First) == 0)
                        {
                            Field[b] = '0';
                        }
                        else
                        {
                            Field[b] = '1';
                        }
                        break;

                    default:
                        Field[b] = ' ';
                        break;
                    }
                }
                mask_LSB_First = mask_LSB_First << 1;
                mask_MSB_First = mask_MSB_First >> 1;
            }
        }
        else
        {
            Field[0] = '?';
            Field[1] = 0x00;
        }
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();

        #endif // HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        break;


    case FRAME_TYPE_ENUMERATED_PARAM:
    case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
    case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
        #ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
        // "<Parameter Title><delimiter><value><New Line>"
        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        if (ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.engUnitsFlag != UOM_NOT_APPLICABLE)
        {
            strcpy((char *)Field,
                   (const char *)PacketManagerGetEngUnits((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.ssIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.objIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.attribIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.engUnitsFlag)
               );
        }
        else
        {
            if (AdjusterGetEnumValue((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.ssIdx,
                                 (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.objIdx,
                                 ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.value.attribIdx,
                                 &tVal,
                                 &tType) == OK)
            {
                TUSIGN16 tableOffset = ((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.firstItem_MSByte << 8)
                                     + ConfigDumpCurrentFrame->frameDefinition.enumParamFrame.firstItem_LSByte
                                     + tVal.data_8;

                stringtable_text_id.attribIdx = FrameTableGetEnumItem(tableOffset);
                (void)strcpy((char *)Field,(const char *)FrameTableGetString(stringtable_text_id));
            }
            else
            {
                Field[0] = '?';
                Field[1] = 0x00;
            }
        }
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_ENUM_PARAM_FRAMES
        break;

    case FRAME_TYPE_STRING_PARAM_PACKED:
    case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
    case FRAME_TYPE_STRING_PARAM:
    case FRAME_TYPE_STRING_PARAM_NO_EDIT:
        #if defined(HMI_SUPPORTS_PACKEDASCII) || defined(HMI_SUPPORTS_STANDARDASCII)
        // "<Parameter Title><delimiter><value><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        Field[0] = 0x00;

        #ifdef HMI_SUPPORTS_PACKEDASCII
        if ( ( ConfigDumpCurrentFrame->frameType == FRAME_TYPE_STRING_PARAM_PACKED )
            || ( ConfigDumpCurrentFrame->frameType == FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT ) )
        {
            // 40 unpacked characters = 30 packed characters
            TUSIGN8 packedtext[31] = {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,
                                      0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,
                                      0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,
                                      0x00};

            len = ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.attributes.maxCharacters;
            // len (unpacked) cannot be more than 40 therefore len * 6 cannot be more than 240
            //lint -e{734} Roger Arnold 2005-1-31
            i =  len * 6;
            len = i / 8;
            if (i % 8)
            {
                len ++;
            }
            for (i = 0; i < len; i++)
            {
                // Return value of "AdjusterGetAttribute()" ignored
                //lint -e{534} Roger Arnold 2005-1-31
                AdjusterGetAttribute((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                     (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                     i,
                                     (void FAST*)&packedtext[i]);
            }
            AdjusterUnpackPASCII(packedtext,len,Field);
        }
        #endif // HMI_SUPPORTS_PACKEDASCII

        #ifdef HMI_SUPPORTS_STANDARDASCII
        if ((ConfigDumpCurrentFrame->frameType == FRAME_TYPE_STRING_PARAM) || (ConfigDumpCurrentFrame->frameType == FRAME_TYPE_STRING_PARAM_NO_EDIT))
        {
            #ifdef HMI_SUPPORTS_SECURITY_ACCESS
            if ((ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX) &&
                    ((ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevel)
                  || (ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevel)
                  || (ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevel)
                  || (ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevelDefault)
                  || (ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevelDefault)
                  || (ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevelDefault)))
            {
                for (i = 0; i < PASSWORD_LENGTH; i++)
                {
                    Field[i] = 0x2A;
                }
                Field[PASSWORD_LENGTH] = 0x00;
            }
            else
            #endif // HMI_SUPPORTS_SECURITY_ACCESS
            {
                len = ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.attributes.maxCharacters;

                for (i = 0; i < len; i++)
                {
                    AdjusterGetAttribute((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                         (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                         i,
                                         (void FAST *)&Field[i]);
                }
                Field[len] = 0x00;
            }
        }
        #endif // HMI_SUPPORTS_STANDARDASCII
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // defined(HMI_SUPPORTS_PACKEDASCII) || defined(HMI_SUPPORTS_STANDARDASCII)
        break;

    case FRAME_TYPE_MANUAL_ADJUST:
        #ifdef HMI_SUPPORTS_MANUAL_ADJUST
        // "<Parameter Title><delimiter><Bargaph level><space><%><delimiter><value><space><units><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.title;
        #if MAX_STRINGS > 255
        stringtable_text_id.attribIdx += ((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.title_HighByte * 256);
        #endif // MAX_STRINGS > 255
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);


        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Bargaph level>
        if (AdjusterGetFloatValueAndLimits(ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.value_ssIdx,
                                           ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.adjvalue_objidx,
                                           ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.adjvalue_attribidx,
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
                tVal.data_F = (TFLOAT)tVal.data_D;
                tLow.data_F = (TFLOAT)tLow.data_D;
                tHigh.data_F = (TFLOAT)tHigh.data_D;
            }
            #endif // HMI_USES_DOUBLE

            if ((TFLOAT)(fabs((TDOUBLE)(tLow.data_F - tHigh.data_F))) == 0.0f)
            {
                if (tVal.data_F > tHigh.data_F)
                {
                    Field[0] = '>';
                    Field[1] = '1';
                    Field[2] = '0';
                    Field[3] = '0';
                    Field[4] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                else
                {
                    Field[0] = '<';
                    Field[1] = '0';
                    Field[2] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
            }
            else
            {
                TFLOAT percent = (100.0F * (tVal.data_F - tLow.data_F)) / (tHigh.data_F - tLow.data_F);
                if (percent > 100.0F)
                {
                    Field[0] = '>';
                    Field[1] = '1';
                    Field[2] = '0';
                    Field[3] = '0';
                    Field[4] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                else if (percent < 0.0F)
                {
                    Field[0] = '<';
                    Field[1] = '0';
                    Field[2] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                else
                {
                    pNumber = ConfigDumpBuildNumberString(percent,3,0,&negNumber,&within_over_under);
                   (void)strcat((char *)parameterDumpOutputLine,(const char *)pNumber);
                }
            }
        }
        else
        {
            Field[0] = '?';
            Field[1] = 0x00;
           (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }

        // <space>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)space);

        // <%>
        Field[0] = '%';
        Field[1] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <value>
        if (ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.dispvalue_objidx != NO_DISPLAY_ON_ADJUST)
        {
            if (AdjusterGetFloatValue((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.value_ssIdx,
                                      (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.dispvalue_objidx,
                                      ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.dispvalue_attribidx,
                                      &tVal,
                                      &tType) == OK)
            {
                #ifdef HMI_USES_DOUBLE
                if (tType == ST_DOUBLE)
                {
                pNumber = ConfigDumpBuildNumberStringFromDouble(tVal.data_D,
                                                                ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.attributes.maxCharacters,
                                                                ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.attributes.characterSet_dpPosn,
                                                                &negNumber,
                                                                &within_over_under);
                }
                else
                #endif // HMI_USES_DOUBLE
                {
                    pNumber = ConfigDumpBuildNumberString(tVal.data_F,
                                                          ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.attributes.maxCharacters,
                                                          ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.attributes.characterSet_dpPosn,
                                                          &negNumber,
                                                          &within_over_under);
                }
                if (within_over_under == 1)
                {
                    Field[0] = '>';
                    Field[1] = 0x00;
                   (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                else
                {
                    if (within_over_under == 2)
                    {
                        Field[0] = '<';
                        Field[1] = 0x00;
                        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                    }
                }
                if (negNumber)
                {
                    Field[0] = '-';
                    Field[1] = 0x00;
                    (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
                }
                (void)strcat((char *)parameterDumpOutputLine,(const char *)pNumber);
            }
            else
            {
                Field[0] = '?';
                Field[1] = 0x00;
                (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
            }
        }

        // <space>
        if (ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.dispvalue_objidx != NO_DISPLAY_ON_ADJUST)
        {
            (void)strcat((char *)parameterDumpOutputLine,(const char *)space);
        }

        // <units>
        if (ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.dispvalue_objidx != NO_DISPLAY_ON_ADJUST)
        {
            strcpy((char *)Field,
                   (const char *)PacketManagerGetEngUnits((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.engUnits.ssIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.engUnits.objIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.engUnits.attribIdx,
                                                          ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.engUnitsType)
                   );
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_MANUAL_ADJUST
        break;


    case FRAME_TYPE_AUTO_ADJUST:
    case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
        #ifdef HMI_SUPPORTS_AUTO_ADJUST
        // "<Parameter Title><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.title;
        #if MAX_STRINGS > 255
        stringtable_text_id.attribIdx += ((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.adjParamFrame.title_HighByte * 256);
        #endif // MAX_STRINGS > 255
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_AUTO_ADJUST
        break;

    case FRAME_TYPE_PROGRESS_BAR:
        #ifdef HMI_SUPPORTS_PROGRESS_FRAMES
        // "<Parameter Title><New Line>"

        // <Parameter Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.progressFrame.titleBar_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_PROGRESS_FRAMES
        break;

    case FRAME_TYPE_IP_ADDRESS:
    case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
        #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
        // "<Parameter Title><delimiter><IP Address><New line>"
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // Read the IP Address and convert to string.
        (void)AdjusterGetAttribute((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                   (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.objIdx,
                                   ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                   (void FAST *)&IPAddress);

        IP_Addr_union.networkAddress = IPAddress;

        Field[0] = '0';
        IPAddress = IP_Addr_union.dotted_IP[0];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            Field[0] ++;
        }
        Field[1] = (IPAddress / 10) + 0x30;
        Field[2] = (IPAddress % 10) + 0x30;
        Field[3] = '.';

        Field[4] = '0';
        IPAddress = IP_Addr_union.dotted_IP[1];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            Field[4] ++;
        }
        Field[5] = (IPAddress / 10) + 0x30;
        Field[6] = (IPAddress % 10) + 0x30;
        Field[7] = '.';

        Field[8] = '0';
        IPAddress = IP_Addr_union.dotted_IP[2];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            Field[8] ++;
        }
        Field[9] = (IPAddress / 10) + 0x30;
        Field[10] = (IPAddress % 10) + 0x30;
        Field[11] = '.';

        Field[12] = '0';
        IPAddress = IP_Addr_union.dotted_IP[3];
        while (IPAddress >= 100)
        {
            IPAddress -= 100;
            Field[12] ++;
        }
        Field[13] = (IPAddress / 10) + 0x30;
        Field[14] = (IPAddress % 10) + 0x30;
        Field[15] = 0x00;

        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();

        #endif  // HMI_SUPPORTS_IP_ADDRESS_FRAMES
        break;

    case FRAME_TYPE_TIME_AND_DATE_PARAM:
    case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
        #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
        // "<Parameter Title><delimiter><Time><delimiter><Date><New line>"
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.title_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // Read the time/date and convert to string.
        (void)AdjusterGetAttribute((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                   (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.objIdx,
                                   ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                   (void FAST *)&TimeDateValue);
        (void)AdjusterConvertSecondsToDate(TimeDateValue, &convertedTimeDate);

        // <Time>
        if ((ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.TimeAndOrDate == 0)
         || (ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.TimeAndOrDate == 1))
        {
            Field[0] = (convertedTimeDate.hour / 10) + 0x30;
            Field[1] = (convertedTimeDate.hour % 10) + 0x30;
            Field[2] = ':';
            Field[3] = (convertedTimeDate.minute / 10) + 0x30;
            Field[4] = (convertedTimeDate.minute % 10) + 0x30;
            Field[5] = ':';
            Field[6] = (convertedTimeDate.second / 10) + 0x30;
            Field[7] = (convertedTimeDate.second % 10) + 0x30;
            Field[8] = 0x00;

            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
        }

        // <Date>
        if ((ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.TimeAndOrDate == 0)
         || (ConfigDumpCurrentFrame->frameDefinition.timeDateFrame.TimeAndOrDate == 2))
        {
            TUSIGN8 y_th = ((TUSIGN8)(convertedTimeDate.year / 1000)) + 0x30;
            TUSIGN8 y_h  = ((TUSIGN8)((convertedTimeDate.year / 100) % 10)) + 0x30;
            TUSIGN8 y_t  = ((TUSIGN8)((convertedTimeDate.year / 10) % 10)) + 0x30;
            TUSIGN8 y_u  = ((TUSIGN8)(convertedTimeDate.year % 10)) + 0x30;

            TUSIGN8 m_t  = (convertedTimeDate.month / 10) + 0x30;
            TUSIGN8 m_u  = (convertedTimeDate.month % 10) + 0x30;

            TUSIGN8 d_t  = (convertedTimeDate.date / 10) + 0x30;
            TUSIGN8 d_u  = (convertedTimeDate.date % 10) + 0x30;

            switch (SFD_SIMPLE.DateFormat)
            {
            case HMI_DATE_FORMAT_DD_MM_YYYY:
                Field[0] = d_t;
                Field[1] = d_u;
                Field[2] = '-';
                Field[3] = m_t;
                Field[4] = m_u;
                Field[5] = '-';
                Field[6] = y_th;
                Field[7] = y_h;
                Field[8] = y_t;
                Field[9] = y_u;
                Field[10] = 0x00;
                break;

            case HMI_DATE_FORMAT_MM_DD_YYYY:
                Field[0] = m_t;
                Field[1] = m_u;
                Field[2] = '-';
                Field[3] = d_t;
                Field[4] = d_u;
                Field[5] = '-';
                Field[6] = y_th;
                Field[7] = y_h;
                Field[8] = y_t;
                Field[9] = y_u;
                Field[10] = 0x00;
                break;

            default: // HMI_DATE_FORMAT_YYYY_MM_DD
                Field[0] = y_th;
                Field[1] = y_h;
                Field[2] = y_t;
                Field[3] = y_u;
                Field[4] = '-';
                Field[5] = m_t;
                Field[6] = m_u;
                Field[7] = '-';
                Field[8] = d_t;
                Field[9] = d_u;
                Field[10] = 0x00;
                break;
            }
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
        break;

    case FRAME_TYPE_MENU:
        #ifdef HMI_SUPPORTS_MENU_FRAMES
        // NOTE This caters for all of the following frame types
        // specialMenuFlag = 0  Standard Menu
        // specialMenuFlag = 1  Operator Menu
        // specialMenuFlag = 2  Signals view
        // specialMenuFlag = 3  Diagnostics View
        // specialMenuFlag = 4  ARM Access level Selection
        // specialMenuFlag = 5  Alarm History
        // specialMenuFlag = 6  Secure Alarm History

        // <Frame Title><New Line>
        // <Frame Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.menuFrame.titleBar_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_MENU_FRAMES
        break;

    case FRAME_TYPE_MANUAL_CALIBRATION:
        #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
        // <Frame Title><New Line>
        // <Frame Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.manualCalFrame.titleBar_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_MANUAL_CALIBRATION
        break;

    case FRAME_TYPE_2_POINT_pH_CALIBRATION:
        #ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
        // <Frame Title><New Line>
        // <Frame Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.pHCalFrame.titleBar_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_2PT_PH_CALIBRATION
        break;


    case FRAME_TYPE_AUTO_CALIBRATION:
        #ifdef HMI_SUPPORTS_AUTO_CALIBRATION
        // <Frame Title><New Line>
        // <Frame Title>
        stringtable_text_id.attribIdx = ConfigDumpCurrentFrame->frameDefinition.autoCalFrame.titleBar_attribidx;
        (void)strncat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id),21);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        ParameterDump_OutputLine();
        #endif // HMI_SUPPORTS_AUTO_CALIBRATION
        break;

    case FRAME_TYPE_OPERATOR:
    default:
        break;
    }
}

void ConfigDumpNextFrame(void)
{
    switch (ConfigDumpCurrentFrame->frameType)
    {
    case FRAME_TYPE_OPERATOR:
        ConfigDumpActive = eFALSE;
        break;

    case FRAME_TYPE_CONFIG_HEADER:
        if ((ConfigDumpCurrentFrame->frameDefinition.configHeaderFrame.pNextFrame == NULL) || (ConfigDumpPulledCFH == eTRUE))
        {
            do
            {
                #pragma diag_suppress=Pm088
                ConfigDumpCurrentFrame ++;
                #pragma diag_default=Pm088
                ConfigDumpCurrentCursor = 0;

                if (ConfigDumpCurrentFrame->frameType != FRAME_TYPE_CONFIG_HEADER)
                {
                    ConfigDumpActive = eFALSE;
                }

            } while ((ValidityTestCheck((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.configHeaderFrame.validTest) == 0) && (ConfigDumpActive));

            ConfigDumpPulledCFH = eFALSE;
            ConfigDumpPulledMenu = eFALSE;
        }
        else
        {
            // ...(1) push current frame (header) onto stack
            ConfigDumpFrameMemory_Push();
            // ...(2) go to child
            ConfigDumpCurrentFrame = ConfigDumpCurrentFrame->frameDefinition.configHeaderFrame.pNextFrame;
            ConfigDumpPulledCFH = eFALSE;
            ConfigDumpPulledMenu = eFALSE;
            if (ConfigDumpCurrentFrame->frameType == FRAME_TYPE_MENU)
            {
                ConfigDumpCurrentCursor = 0xFF;
            }
            else
            {
                ConfigDumpCurrentCursor = 0;
            }
        }
        break;

    case FRAME_TYPE_NUMERICAL_PARAM:
    case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
    case FRAME_TYPE_ENUMERATED_PARAM:
    case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
    case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
    case FRAME_TYPE_MANUAL_ADJUST:
    case FRAME_TYPE_AUTO_ADJUST:
    case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
    case FRAME_TYPE_STRING_PARAM:
    case FRAME_TYPE_STRING_PARAM_NO_EDIT:
    case FRAME_TYPE_STRING_PARAM_PACKED:
    case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
    case FRAME_TYPE_PROGRESS_BAR:
    case FRAME_TYPE_ALARM_HISTORY:
    case FRAME_TYPE_SECURE_ALARM_HISTORY:
    case FRAME_TYPE_TIME_AND_DATE_PARAM:
    case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
    case FRAME_TYPE_MANUAL_CALIBRATION:
    case FRAME_TYPE_AUTO_CALIBRATION:
    case FRAME_TYPE_2_POINT_pH_CALIBRATION:
    case FRAME_TYPE_BIT_ENUMERATED_PARAM:
    case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
    case FRAME_TYPE_INTEGER_PARAM:
    case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
    case FRAME_TYPE_IP_ADDRESS:
    case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
    case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
    case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:
        #ifdef HMI_SUPPORTS_EASYSETUP
            if (ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.easySetupFlag == 1)
            {
                #pragma diag_suppress=Pm088
                ConfigDumpCurrentFrame ++;
                #pragma diag_default=Pm088
            }
            else
        #endif // HMI_SUPPORTS_EASYSETUP
            {
                ConfigDumpFrameMemory_Pull();
            }
        break;

    case FRAME_TYPE_MENU:
        if (ConfigDumpCurrentFrame->frameDefinition.menuFrame.specialMenuFlag != 0)
        {
            #ifdef HMI_SUPPORTS_EASYSETUP
            if (ConfigDumpCurrentFrame->frameDefinition.numericalParamFrame.easySetupFlag == 1)
            {
                #pragma diag_suppress=Pm088
                ConfigDumpCurrentFrame ++;
                #pragma diag_default=Pm088
            }
            else
            #endif // HMI_SUPPORTS_EASYSETUP
            {
                ConfigDumpFrameMemory_Pull();
            }
        }
        else
        {
           TUSIGN8 cursorValid;

           do
            {
                TUSIGN16 firstItem;
                MENU_ITEM_TYPE const *pMenuItem;

                cursorValid = 0;

                // (1) Set the Cursor
                if (ConfigDumpCurrentCursor == 0xFF)
                {
                    ConfigDumpCurrentCursor = 0;    // set cursor to zero  first entry into menu - menu title has been processed
                }
                else
                {
                    ConfigDumpCurrentCursor ++;     // increment cursor
                }

                if (ConfigDumpCurrentCursor <= ConfigDumpCursorLimit)
                {
                    // (2) Get pointer to the menu item
                    firstItem = ((TUSIGN16)(ConfigDumpCurrentFrame->frameDefinition.menuFrame.firstItem_MSByte) << 8) + (TUSIGN16)(ConfigDumpCurrentFrame->frameDefinition.menuFrame.firstItem_LSByte);
                    pMenuItem = FrameTableGetMenuItem((TUSIGN16)(firstItem +  ConfigDumpCurrentCursor));


                    // (3) Check that the menu item is valid
                    if (pMenuItem != NULL)
                    {
                        cursorValid = ValidityTestCheck((TUSIGN16)pMenuItem->validTest);
                    }
                }
            } while ((cursorValid == 0) && (ConfigDumpCurrentCursor <= ConfigDumpCursorLimit));

            if (cursorValid)
            {
                // A valid child frame has been found ...
                // ...(1) push menu onto stack
                ConfigDumpFrameMemory_Push();
                // ...(2) go to child
                #pragma diag_suppress=Pm088
                ConfigDumpCurrentFrame = ConfigDumpCurrentFrame->frameDefinition.menuFrame.pNextFrame + ConfigDumpCurrentCursor;
                ConfigDumpPulledCFH = eFALSE;
                ConfigDumpPulledMenu = eFALSE;
                #pragma diag_default=Pm088

                if (ConfigDumpCurrentFrame->frameType == FRAME_TYPE_MENU)
                {
                    ConfigDumpCurrentCursor = 0xFF;
                }
                else
                {
                    ConfigDumpCurrentCursor = 0;
                }
            }
            else
            {
                // No more valid child frames ...
                // ...(1) go back
                ConfigDumpFrameMemory_Pull();
            }
        }
        break;

    default:
        asm("nop");
        break;
    }
    // End of "switch (ConfigDumpCurrentFrame->frameType)"
}

void ConfigDumpFrameMemory_Push(void)
{
    ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].pFrame = ConfigDumpCurrentFrame;
    ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].cursor = ConfigDumpCurrentCursor;
    ConfigDumpFrameMemoryPointer ++;
}

void ConfigDumpFrameMemory_Pull(void)
{
    ConfigDumpFrameMemoryPointer --;
    ConfigDumpCurrentFrame = ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].pFrame;
    ConfigDumpCurrentCursor = ConfigDumpFrameMemory[ConfigDumpFrameMemoryPointer].cursor;

    if (ConfigDumpCurrentFrame->frameType == FRAME_TYPE_MENU)
    {
        TUSIGN16 cursorL = ((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.menuFrame.firstItem_MSByte << 8) + (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.menuFrame.firstItem_LSByte;
        TUSIGN16 cursorH = ((TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.menuFrame.lastItem_MSByte << 8) + (TUSIGN16)ConfigDumpCurrentFrame->frameDefinition.menuFrame.lastItem_LSByte;
        ConfigDumpCursorLimit = (TUSIGN8)(cursorH - cursorL);
        ConfigDumpPulledMenu = eTRUE;
    }
    else
    {
        ConfigDumpPulledMenu = eFALSE;
    }

    if (ConfigDumpCurrentFrame->frameType == FRAME_TYPE_CONFIG_HEADER)
    {
        ConfigDumpPulledCFH = eTRUE;
    }
    else
    {
        ConfigDumpPulledCFH = eFALSE;
    }
}

// -------------------- Information Dump --------------------
void InfoDumpStart(void)
{
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    StatusDumpActive = eTRUE;
    #endif // HMI_USES_COMMONDIAGNOSTICS

    SignalsDumpCurrentSignal = 0;
    SignalsDumpActive = eTRUE;

    #ifdef HMI_USES_COMMONDIAGNOSTICS
    HistoryDumpMaxAlarms = AdjusterGetNumberOfAlarmConditions(eFALSE) - 1;
    HistoryDumpCurrentAlarm = 0;
    HistoryDumpProcessStage = 0;
    HistoryDumpActive = eTRUE;
    #endif // HMI_USES_COMMONDIAGNOSTICS

    readyForNextLine = eTRUE;
}


void InfoDumpExecute(void)
{
    if (readyForNextLine)
    {
        if (StatusDumpActive)
        {
            #ifdef HMI_USES_COMMONDIAGNOSTICS
            InfoDumpProcessStatus();
            StatusDumpActive = eFALSE;
            #else
            StatusDumpActive = eFALSE;
            #endif // HMI_USES_COMMONDIAGNOSTICS
        }
        else
        {
            if (SignalsDumpActive)
            {
                InfoDumpProcessSignal();    // Build o/p for this signal
                InfoDumpNextSignal();       // Select next signal (or switch off if all signals processed)
            }
            else
            {
                if (HistoryDumpActive)
                {
                    #ifdef HMI_USES_COMMONDIAGNOSTICS
                    HistoryDumpProcessAlarm();  // Build o/p for this alarm
                    HistoryDumpNextAlarm();     // Select next alarm (or switch off if all alarms processed)
                    #else
                    HistoryDumpActive = eFALSE;
                    #endif // HMI_USES_COMMONDIAGNOSTICS
                }
            }
        }
    }
}


void InfoDumpProcessStatus(void)
{
    #ifdef HMI_USES_COMMONDIAGNOSTICS
//    TUSIGN8 Line[20];
    TEXT_IDENTIFIER_TYPE stringtable_text_id = {HMI_IDX, SS_HMI_OBJ_STRINGTABLE, IDS_BLANK};

    // <AlarmClassification - AlarmCategory>
    TUSIGN8 NAMURClassification, NAMURCategory;
    AdjusterGetNAMURStatus(&NAMURClassification, &NAMURCategory);

    if (NAMURClassification != CLASSIFICATION_NONE)
    {
        switch (NAMURClassification)
        {
        case CLASSIFICATION_INFORMATIONONLY: parameterDumpOutputLine[0] = 'I';    break;
        case CLASSIFICATION_FAILURE:         parameterDumpOutputLine[0] = 'F';    break;
        case CLASSIFICATION_MAINTENANCEREQD: parameterDumpOutputLine[0] = 'M';    break;
        case CLASSIFICATION_OFFSPEC:         parameterDumpOutputLine[0] = 'S';    break;
        case CLASSIFICATION_CHECKFUNCTION:   parameterDumpOutputLine[0] = 'C';    break;
        default:                                                     break;
        }

        parameterDumpOutputLine[1] = '-';
        parameterDumpOutputLine[2] = 0x00;

        stringtable_text_id.attribIdx = FrameTableGetNAMURCategoryString(NAMURCategory);
        (void)strcat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id));

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
    }
    #endif // HMI_USES_COMMONDIAGNOSTICS
}

void InfoDumpProcessSignal(void)
{
    TEXT_IDENTIFIER_TYPE stringtable_text_id = {HMI_IDX, SS_HMI_OBJ_STRINGTABLE, IDS_BLANK};
    const SIGNAL_TABLE_TYPE *pSignal = FrameTableGetSignal(SignalsDumpCurrentSignal);

    TUSIGN8 Field[5];
    ADJUST_DATA tVal;
    TUSIGN8 tType;

    TUSIGN8 negNumber;
    TUSIGN8 within_over_under;
    TUSIGN8 *pNumber;

    // <Short tag><delimiter><Long Tag><delimiter><value><space><units><new line>

    // <Short tag>
    stringtable_text_id.attribIdx = pSignal->short_identifier;
    strcpy((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id));

    // <delimiter>
    (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

    // <Long Tag>
    stringtable_text_id.attribIdx = pSignal->long_identifier;
    strcat((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id));

    // <delimiter>
    (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

    // <value>
    if (AdjusterGetFloatValue((TUSIGN16)pSignal->value.ssIdx,
                              (TUSIGN16)pSignal->value.objIdx,
                              pSignal->value.attribIdx,
                              &tVal,
                              &tType) == OK)
    {
        #ifdef HMI_USES_DOUBLE
        if (tType == ST_DOUBLE)
        {
            pNumber = ConfigDumpBuildNumberStringFromDouble(tVal.data_D,
                                                            12,
                                                            pSignal->dpPosn,
                                                            &negNumber,
                                                            &within_over_under);
        }
        else
        #endif // HMI_USES_DOUBLE
        {
            pNumber = ConfigDumpBuildNumberString(tVal.data_F,
                                                  8,
                                                  pSignal->dpPosn,
                                                  &negNumber,
                                                  &within_over_under);
        }
        if (within_over_under == 1)
        {
            Field[0] = '>';
            Field[1] = 0x00;
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }
        else
        {
            if (within_over_under == 2)
            {
                Field[0] = '<';
                Field[1] = 0x00;
                (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
            }
        }
        if (negNumber)
        {
            Field[0] = '-';
            Field[1] = 0x00;
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }
        (void)strcat((char *)parameterDumpOutputLine,(const char *)pNumber);
    }
    else
    {
        Field[0] = '?';
        Field[1] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
    }

    // <space>
    (void)strcat((char *)parameterDumpOutputLine,(const char *)space);

    // <units>
    (void)strcat((char *)parameterDumpOutputLine,(const char *)PacketManagerGetEngUnits((TUSIGN16)pSignal->engUnits.ssIdx,
                                                                           pSignal->engUnits.objIdx,
                                                                           pSignal->engUnits.attribIdx,
                                                                           pSignal->engUnitsType));

    // <New Line>
    (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

    ParameterDump_OutputLine();
}

void InfoDumpNextSignal(void)
{
    TBOOL CurrentSignalValid = eFALSE;

    while ((CurrentSignalValid == eFALSE) && (SignalsDumpActive == eTRUE))
    {
        SignalsDumpCurrentSignal ++;

        if (SignalsDumpCurrentSignal >= MAX_SIGNALS)
        {
            SignalsDumpActive = eFALSE;
        }
        else
        {
            #ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
            if (ValidityTestCheck((TUSIGN16)FrameTableGetSignal(SignalsDumpCurrentSignal)->SignalsViewTest))
            #else
            if (FrameTableGetSignal(SignalsDumpCurrentSignal)->SignalsViewEnable == 1)
            #endif
            {
                CurrentSignalValid = eTRUE;
            }
        }
    }
}

#ifdef HMI_USES_COMMONDIAGNOSTICS
void HistoryDumpProcessAlarm(void)
{
//    TUSIGN8 Field[10];
//    TUSIGN8 Line[20];

    TUSIGN8 AlarmClassification;
    TUSIGN8 AlarmCategory;
    TUSIGN8 AlarmPriority;
    TUSIGN8 AlarmID;
    TUSIGN16 AlarmCount;
    TUSIGN32 AlarmTime_msec;
    TUSIGN16 AlarmTime_day;
    TUSIGN32 AlarmMostRecent_msec;
    TUSIGN16 AlarmMostRecent_day;

    TBOOL AlarmActive = AdjusterGetAlarmHistoryDetails(0,
                                                       HistoryDumpCurrentAlarm,
                                                       &AlarmClassification,
                                                       &AlarmCategory,
                                                       &AlarmPriority,
                                                       &AlarmID,
                                                       &AlarmCount,
                                                       &AlarmTime_msec,
                                                       &AlarmTime_day,
                                                       &AlarmMostRecent_msec,
                                                       &AlarmMostRecent_day);

    // HistoryDumpProcessStage == 0     <Alarm Category><delimiter><Alarm Code><delimiter><Status><new line>
    // HistoryDumpProcessStage == 1     <delimiter><Alarm Text (line 1)<new line>
    //                                  <delimiter><Alarm Text (line 2)<new line>
    //                                  <delimiter><Alarm Text (line 3)<new line>
    //                                  <delimiter><Alarm Text (line 4)<new line>
    //                                  <delimiter><Alarm Text (line 5)<new line>
    // 5 * (1 + 16 + 2) = 95 characters max

    // HistoryDumpProcessStage == 2     <delimiter><Alarm Count><new line>
    //                                  <delimiter><Total time active><new line>
    //                                  <delimiter><Most Recent activation><new line>

    if (HistoryDumpProcessStage == 0)
    {
        // <Alarm Category><delimiter><Alarm Code><delimiter><Status><new line>
        // 14 + 1 + 8 + 1 + 8 + 2 = 34 characters max

        TUSIGN8 Field[10];
        TEXT_IDENTIFIER_TYPE stringtable_text_id = {HMI_IDX, SS_HMI_OBJ_STRINGTABLE, IDS_BLANK};

        // <Alarm Category>
        stringtable_text_id.attribIdx = FrameTableGetNAMURCategoryString(AlarmCategory);
        (void)strcpy((char *)parameterDumpOutputLine,(const char *)FrameTableGetString(stringtable_text_id));

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Alarm Code>
        switch (AlarmClassification)
        {
        case CLASSIFICATION_INFORMATIONONLY:    Field[0] = 'I';   break;
        case CLASSIFICATION_FAILURE:            Field[0] = 'F';   break;
        case CLASSIFICATION_MAINTENANCEREQD:    Field[0] = 'M';   break;
        case CLASSIFICATION_OFFSPEC:            Field[0] = 'S';   break;
        case CLASSIFICATION_CHECKFUNCTION:      Field[0] = 'C';   break;
        default:                                Field[0] = '_';   break;
        }

        Field[1] = 0x30 + (AlarmPriority / 100);
        Field[2] = 0x30 + ((AlarmPriority % 100) / 10);
        Field[3] = 0x30 + (AlarmPriority % 10);
        Field[4] = '.';
        Field[5] = 0x30 + (AlarmID / 100);
        Field[6] = 0x30 + ((AlarmID % 100) / 10);
        Field[7] = 0x30 + (AlarmID % 10);
        Field[8] = 0x00;

        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Status>
        if (!AlarmActive)
        {
            Field[0] = 'I';
            Field[1] = 'N';
            Field[2] = 0x00;
            (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);
        }

        Field[0] = 'A';
        Field[1] = 'C';
        Field[2] = 'T';
        Field[3] = 'I';
        Field[4] = 'V';
        Field[5] = 'E';
        Field[6] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        //<new line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
    }
    else if (HistoryDumpProcessStage == 1)
    {
        // <delimiter><Alarm Text><new line>
        // 1 + 16 + 2 = 19 characters max
        // 5 lines = 95 characters max

        TEXT_IDENTIFIER_TYPE stringtable_text_id = {HMI_IDX, SS_HMI_OBJ_STRINGTABLE, IDS_BLANK};

        TUSIGN8 len,i;

        #ifdef COMMON_TOP_WORKS_NEW_FEATURES    // DIAGNOSTIC_SPLIT_DESC_AND_RECOVERY

        DIAGNOSTICS_TABLE_ENTRY_TYPE diagnosisTableEntry = FrameTableGetDiagnosticTableEntry(AlarmID);
        const TUSIGN8 *descriptionAddress;
        const TUSIGN8 *recoveryActAddress;

        stringtable_text_id.attribIdx = diagnosisTableEntry.descriptionID;
        descriptionAddress = FrameTableGetString(stringtable_text_id);
        stringtable_text_id.attribIdx = diagnosisTableEntry.RecoveryActID;
        recoveryActAddress = FrameTableGetString(stringtable_text_id);

        #else

        const TUSIGN8 *stringAddress;
        TUSIGN8  startChar;

        stringtable_text_id.attribIdx = FrameTableGetDiagnosticDescriptionStringID(AlarmID);
        stringAddress = FrameTableGetString(stringtable_text_id);
        len = strlen((const char *)stringAddress);

        #endif

        // Blank output line
        for (i = 0; i < LINELENGTH; i++)
        {
            parameterDumpOutputLine[i] = 0x00;
        }

        #ifdef COMMON_TOP_WORKS_NEW_FEATURES    // DIAGNOSTIC_SPLIT_DESC_AND_RECOVERY

        len = strlen((const char *)descriptionAddress);

        if (len)
        {
            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
            // <Description (1st line)>
            (void)strncat((char *)parameterDumpOutputLine,(const char *)&descriptionAddress[0],16);
            // <New Line>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        }

        if (len > 16)
        {
            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
            // <Description (2nd line)>
            (void)strncat((char *)parameterDumpOutputLine,(const char *)&descriptionAddress[16],16);
            // <New Line>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        }

        len = strlen((const char *)recoveryActAddress);
        if (len)
        {
            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
            // <Recovery text (1st line)>
            (void)strncat((char *)parameterDumpOutputLine,(const char *)&recoveryActAddress[0],16);
            // <New Line>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        }

        if (len > 16)
        {
            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
            // <Recovery text (2nd line)>
            (void)strncat((char *)parameterDumpOutputLine,(const char *)&recoveryActAddress[16],16);
            // <New Line>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        }

        if (len > 32)
        {
            // <delimiter>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
            // <Recovery text (3rd line)>
            (void)strncat((char *)parameterDumpOutputLine,(const char *)&recoveryActAddress[32],16);
            // <New Line>
            (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
        }

        #else

        for (i = 0; i <= 4; i++)
        {
            startChar = (i * 16);

            if (startChar < len)
            {
                // <delimiter>
                (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);
                // <Alarm Text>
                (void)strncat((char *)parameterDumpOutputLine,(const char *)&stringAddress[startChar],16);
                // <New Line>
                (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);
            }
        }

        #endif

        ParameterDump_OutputLine();
    }
    else if (HistoryDumpProcessStage == 2)
    {
        // <delimiter><"n:"><delimiter><Alarm Count><new line> (11 characters)
        // 1 + 2 + 1 + 5 + 2 = 11 characters max

        // <delimiter><"t:"><delimiter><Total time active><new line> (24 characters)
        // 1 + 2 + 1 + 18 + 2 = 24 characters max

        // <delimiter><"tn":><delimiter><Most Recent activation><new line> (25 characters)
        // 1 + 3 + 1 + 18 + 2 = 25 characters max

        // 11 + 24 + 25 = 60 characters max

        TUSIGN8 Field[10];

        // <delimiter><"n:"><delimiter><Alarm Count><new line> (11 characters)

        // <delimiter>
        (void)strcpy((char *)parameterDumpOutputLine,(const char *)delimiter);

        //  <"n:">
        Field[0] = 'n';
        Field[1] = ':';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Alarm Count>
        (void)PacketManagerConvertValue(AlarmCount, Field, eFALSE);
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);


        // <delimiter><"t:"><delimiter><Total time active><new line> (24 characters)

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <"t:">
        Field[0] = 't';
        Field[1] = ':';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Total time active>
        // days
        (void)PacketManagerConvertValue(AlarmTime_day, Field, eFALSE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (days)
        Field[0] = 'd';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // hours
        (void)PacketManagerConvertValue((TUSIGN16)(AlarmTime_msec / 3600000) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (hours)
        Field[0] = 'h';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // minutes
        (void)PacketManagerConvertValue((TUSIGN16)((AlarmTime_msec / 60000) % 60) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (minutes)
        Field[0] = 'm';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // seconds
        (void)PacketManagerConvertValue((TUSIGN16)((AlarmTime_msec / 1000) % 60) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (seconds)
        Field[0] = 's';
        Field[1] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);


        // <delimiter><"tn":><delimiter><Most Recent activation><new line> (25 characters)
        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <"tn:">
        Field[0] = 't';
        Field[1] = 'n';
        Field[2] = ':';
        Field[3] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(const char *)Field);

        // <delimiter>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)delimiter);

        // <Total time active>
        // days
        (void)PacketManagerConvertValue(AlarmMostRecent_day, Field, eFALSE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (days)
        Field[0] = 'd';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // hours
        (void)PacketManagerConvertValue((TUSIGN16)(AlarmMostRecent_msec / 3600000) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (hours)
        Field[0] = 'h';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // minutes
        (void)PacketManagerConvertValue((TUSIGN16)((AlarmMostRecent_msec / 60000) % 60) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (minutes)
        Field[0] = 'm';
        Field[1] = ' ';
        Field[2] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // seconds
        (void)PacketManagerConvertValue((TUSIGN16)((AlarmMostRecent_msec / 1000) % 60) , Field, eTRUE);
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // units (minutes)
        Field[0] = 's';
        Field[1] = 0x00;
        (void)strcat((char *)parameterDumpOutputLine,(char *)Field);

        // <New Line>
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);

        ParameterDump_OutputLine();
    }
    else
    {
    }
}

void HistoryDumpNextAlarm(void)
{
    if (HistoryDumpProcessStage < 2)
    {
        HistoryDumpProcessStage ++;
    }
    else
    {
        HistoryDumpProcessStage = 0;
        HistoryDumpCurrentAlarm ++;

        if (HistoryDumpCurrentAlarm > HistoryDumpMaxAlarms)
        {
            HistoryDumpActive = eFALSE;
        }
    }
}

#endif // HMI_USES_COMMONDIAGNOSTICS

// -------------------- Invalid Command Reply Dump --------------------
void InvalidCommandDumpStart(void)
{
    InvalidCommandActive = eTRUE;
    readyForNextLine = eTRUE;
}

void InvalidCommandDumpExecute(void)
{
    if (InvalidCommandActive && readyForNextLine)
    {
        /* Unrecognised command received.
           Valid commands are:
           C Configuration Dump
           I Signals and Alarm Information Dump
        */

        (void)strcpy((char *)parameterDumpOutputLine,"Unrecognised command received.");          //  30
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);                     //   2
        (void)strcat((char *)parameterDumpOutputLine,"Valid commands are:");                     //  19
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);                     //   2
        (void)strcat((char *)parameterDumpOutputLine,"C Configuration Dump");                    //  20
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);                     //   2
        (void)strcat((char *)parameterDumpOutputLine,"I Signals and Alarm Information Dump");    //  36
        (void)strcat((char *)parameterDumpOutputLine,(const char *)newline);                     //   2
                                                                                    // ---
        ParameterDump_OutputLine();                                                 // 113 Characters
        InvalidCommandActive = eFALSE;
    }
}

TUSIGN8 *ConfigDumpBuildNumberString(TFLOAT value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under)
{
    TUSIGN16 x3, x2, x1, x_1, x_2, x_3, x_4;
    TUSIGN8 i,returnOffset;
    TFLOAT limitL,limitH;

    // --------------------------------------------------------------------------------------------
    // 1. Limit length
    #if defined(LONGEST_NUMBER_IS_4_DIGITS)
    if (len > 4)    {len = 4;}

    #elif defined(LONGEST_NUMBER_IS_8_DIGITS)
    if (len > 8)    {len = 8;}

    #else
    if (len > 12)   {len = 12;}

    #endif

    // --------------------------------------------------------------------------------------------
    // 2. If a configuarable DP position has been selected then get the actual DP position
    switch (resolution)
    {
    case CONFIGURABLE_DP_1:     resolution = SFD_CUSTOM_DP.customDP_1;      break;
    case CONFIGURABLE_DP_2:     resolution = SFD_CUSTOM_DP.customDP_2;      break;
    case CONFIGURABLE_DP_3:     resolution = SFD_CUSTOM_DP.customDP_3;      break;
    case CONFIGURABLE_DP_4:     resolution = SFD_CUSTOM_DP.customDP_4;      break;
    case CONFIGURABLE_DP_5:     resolution = SFD_CUSTOM_DP.customDP_5;      break;
    case CONFIGURABLE_DP_6:     resolution = SFD_CUSTOM_DP.customDP_6;      break;
    case CONFIGURABLE_DP_7:     resolution = SFD_CUSTOM_DP.customDP_7;      break;
    case CONFIGURABLE_DP_8:     resolution = SFD_CUSTOM_DP.customDP_8;      break;
    case CONFIGURABLE_DP_9:     resolution = SFD_CUSTOM_DP.customDP_9;      break;
    case CONFIGURABLE_DP_10:    resolution = SFD_CUSTOM_DP.customDP_10;     break;
    case CONFIGURABLE_DP_11:    resolution = SFD_CUSTOM_DP.customDP_11;     break;
    default:                                                                break;
    }

    // --------------------------------------------------------------------------------------------
    // 3. If value is less than zero then set "negative value flag" and make number positive
    if (value < 0)
    {
        value = value * -1;
        *pNegVal = 1;
    }
    else
    {
        *pNegVal = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 4. If maximum resolution has been selected then calculate the get the actual DP position
    //    according to length and absolute value
    if (resolution == 0xFF)
    {
        if      (value < 10.0f)            /*   1 digit before dp  */  {i = 1;}
        else if (value < 100.0f)           /*   2 digits before dp */  {i = 2;}
        else if (value < 1000.0f)          /*   3 digits before dp */  {i = 3;}
        else if (value < 10000.0f)         /*   4 digits before dp */  {i = 4;}
        else if (value < 100000.0f)        /*   5 digits before dp */  {i = 5;}
        else if (value < 1000000.0f)       /*   6 digits before dp */  {i = 6;}
        else if (value < 10000000.0f)      /*   7 digits before dp */  {i = 7;}
        else if (value < 100000000.0f)     /*   8 digits before dp */  {i = 8;}
        else if (value < 1000000000.0f)    /*   9 digits before dp */  {i = 9;}
        else if (value < 10000000000.0f)   /*  10 digits before dp */  {i = 10;}
        else if (value < 100000000000.0f)  /*  11 digits before dp */  {i = 11;}
        else if (value < 1000000000000.0f) /*  12 digits before dp */  {i = 12;}
        else                               /* >12 digits before dp */  {i = 13;}

        if (len <= i)   {resolution = 0;}
        else            {resolution = len - (i + 1);}

    }

    // --------------------------------------------------------------------------------------------
    // 5. Limit resolution
    #if defined(LONGEST_NUMBER_IS_4_DIGITS)
    if (resolution > 2)     {resolution = 2;}

    #elif defined(LONGEST_NUMBER_IS_8_DIGITS)
    if (resolution > 6)     {resolution = 6;}

    #else
    if (resolution > 10)    {resolution = 10;}
    #endif

    while ((resolution > (len - 2)) && (resolution != 0))
    {
        resolution --;
    }

    // --------------------------------------------------------------------------------------------
    // 6. Calculate high limit
    limitH = 0.0f;

    if (resolution == 0)
    {
        switch (len)
        {
        case 1:     limitH = 9.0f;     break;
        case 2:     limitH = 99.0f;     break;
        case 3:     limitH = 999.0f;     break;
        case 4:     limitH = 9999.0f;     break;
        case 5:     limitH = 99999.0f;     break;
        case 6:     limitH = 999999.0f;     break;
        case 7:     limitH = 9999999.0f;     break;
        case 8:     limitH = 99999999.0f;     break;
        case 9:     limitH = 999999999.0f;     break;
        case 10:    limitH = 9999999999.0f;     break;
        case 11:    limitH = 99999999999.0f;     break;
        case 12:    limitH = 999999999999.0f;     break;
        default:    assert(0);                     break;
        }
    }
    else
    {
        switch (len)
        {
        // set limit assuming resolution = 1
        case 12:    limitH = 9999999999.9f;     break;
        case 11:    limitH = 999999999.9f;     break;
        case 10:    limitH = 99999999.9f;     break;
        case 9:     limitH = 9999999.9f;     break;
        case 8:     limitH = 999999.9f;     break;
        case 7:     limitH = 99999.9f;     break;
        case 6:     limitH = 9999.9f;     break;
        case 5:     limitH = 999.9f;     break;
        case 4:     limitH = 99.9f;     break;
        case 3:     limitH = 9.9f;     break;
        default:    limitH = 0.0f;     break;
        }

        switch (resolution)
        {
        // adjust limit for resolution > 1
        case 10:    limitH /= 1000000000.0f;     break;
        case 9:     limitH /= 100000000.0f;     break;
        case 8:     limitH /= 10000000.0f;     break;
        case 7:     limitH /= 1000000.0f;     break;
        case 6:     limitH /= 100000.0f;     break;
        case 5:     limitH /= 10000.0f;     break;
        case 4:     limitH /= 1000.0f;     break;
        case 3:     limitH /= 100.0f;     break;
        case 2:     limitH /= 10.0f;     break;
        default:    asm("nop");         break;
        }
    }

    // --------------------------------------------------------------------------------------------
    // 7. Calculate low limit
    limitL = limitH * -1;

    // --------------------------------------------------------------------------------------------
    // 8. Apply limits
    if (value > limitH)
    {
        value = limitH;
        *pWithin_over_under = 1;
    }
    else if (value < limitL)
    {
        value = limitL;
        *pWithin_over_under = 2;
    }
    else
    {
        *pWithin_over_under = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 9. Separate number

    // XXXXxxxxxxxx.xxxxxxxxxx
    x3 = (int)(value / 100000000.0f);
    value -= x3 * 100000000.0f;

    // xxxxXXXXxxxx.xxxxxxxxxx
    x2 = (int)(value / 10000.0f);
    value -= x2 * 10000.0f;

    // xxxxxxxxXXXX.xxxxxxxxxx
    x1 = (int)value;
    value -= (float)x1;

    // xxxxxxxxxxxx.XXxxxxxxxx
    value *= 100.0f;
    x_1 = (int)value;
    value -= (float)x_1;

    // xxxxxxxxxxxx.xxXXXXxxxx
    value *= 10000.0f;
    x_2 = (int)value;
    value -= (float)x_2;

     // xxxxxxxxxxxx.xxxxxxXXXX
    value *= 10000.0f;
    x_3 = (int)value;
    value -= (float)x_3;

     // xxxxxxxxxxxx.xxxxxxxxxxXXXX
    value *= 10000.0f;
    x_4 = (int)value;

    // --------------------------------------------------------------------------------------------
    // 9. Round value according to resolution

	switch (resolution)
	{
	case 0:		x_1 += 50;		break;
	case 1:		x_1 += 5;		break;
	case 2:		x_2 += 5000;	break;
	case 3:		x_2 += 500;		break;
	case 4:		x_2	+= 50;		break;
	case 5:		x_2 += 5;		break;
	case 6:		x_3 += 5000;	break;
	case 7:		x_3 += 500;		break;
	case 8:		x_3	+= 50;		break;
	case 9:		x_3 += 5;		break;
	case 10:	x_4 += 5000;	break;
    default:    asm("nop");     break;
	}

    if (x_4 > 9999)
    {
        x_4 = 0;    x_3 ++;
    }
    if (x_3 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 ++;
    }
    if (x_2 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 ++;
    }
    if (x_1 > 99)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 ++;
    }
    if (x1 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 = 0;     x2 ++;
    }
    if (x2 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 = 0;     x2 = 0;     x3 ++;
    }
    if (x3 > 9999)
    {
        x_4 = 9999;
        x_3 = 9999;
        x_2 = 9999;
        x_1 = 9999;
        x1  = 9999;
        x2  = 9999;
        x3  = 9999;
        *pWithin_over_under = 1;
        resolution = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 10. Convert value blocks to text


    numberText[0] = ' ';

#if defined(LONGEST_NUMBER_IS_12_DIGITS)
    // Xxxxxxxxxxxx.xxxxxxxxxx
    numberText[1] = ((TUSIGN8)(x3 / 1000)) + 0x30;
    x3 %= 1000;

    // xXxxxxxxxxxx.xxxxxxxxxx
    numberText[2] = ((TUSIGN8)(x3 / 100)) + 0x30;
    x3 %= 100;

    // xxXxxxxxxxxx.xxxxxxxxxx
    numberText[3] = ((TUSIGN8)(x3 / 10)) + 0x30;
    x3 %= 10;

    // xxxXxxxxxxxx.xxxxxxxxxx
    numberText[4] = ((TUSIGN8)(x3)) + 0x30;
#endif // defined(LONGEST_NUMBER_IS_12_DIGITS)

#if defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)
    // xxxxXxxxxxxx.xxxxxxxxxx
    numberText[5] = ((TUSIGN8)(x2 / 1000)) + 0x30;
    x2 %= 1000;

    // xxxxxXxxxxxx.xxxxxxxxxx
    numberText[6] = ((TUSIGN8)(x2 / 100)) + 0x30;
    x2 %= 100;

    // xxxxxxXxxxxx.xxxxxxxxxx
    numberText[7] = ((TUSIGN8)(x2 / 10)) + 0x30;
    x2 %= 10;

    // xxxxxxxXxxxx.xxxxxxxxxx
    numberText[8] = ((TUSIGN8)(x2)) + 0x30;
#endif // defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)

    // xxxxxxxxXxxx.xxxxxxxxxx
    numberText[9] = ((TUSIGN8)(x1 / 1000)) + 0x30;
    x1 %= 1000;

    // xxxxxxxxxXxx.xxxxxxxxxx
    numberText[10] = ((TUSIGN8)(x1 / 100)) + 0x30;
    x1 %= 100;

    // xxxxxxxxxxXx.xxxxxxxxxx
    numberText[11] = ((TUSIGN8)(x1 / 10)) + 0x30;
    x1 %= 10;

    // xxxxxxxxxxxX.xxxxxxxxxx
    numberText[12] = ((TUSIGN8)(x1)) + 0x30;

    numberText[13] = '.';

    // xxxxxxxxxxxx.Xxxxxxxxxx
    numberText[14] = ((TUSIGN8)(x_1 / 10)) + 0x30;
    x_1 %= 10;

    // xxxxxxxxxxxx.xXxxxxxxxx
    numberText[15] = ((TUSIGN8)(x_1)) + 0x30;

#if defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)
    // xxxxxxxxxxxx.xxXxxxxxxx
    numberText[16] = ((TUSIGN8)(x_2 / 1000)) + 0x30;
    x_2 %= 1000;

    // xxxxxxxxxxxx.xxxXxxxxxx
    numberText[17] = ((TUSIGN8)(x_2 / 100)) + 0x30;
    x_2 %= 100;

    // xxxxxxxxxxxx.xxxxXxxxxx
    numberText[18] = ((TUSIGN8)(x_2 / 10)) + 0x30;
    x_2 %= 10;

    // xxxxxxxxxxxx.xxxxxXxxxx
    numberText[19] = ((TUSIGN8)(x_2)) + 0x30;
#endif // defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)

#if defined(LONGEST_NUMBER_IS_12_DIGITS)
    // xxxxxxxxxxxx.xxxxxxXxxx
    numberText[20] = ((TUSIGN8)(x_3 / 1000)) + 0x30;
    x_3 %= 1000;

    // xxxxxxxxxxxx.xxxxxxxXxx
    numberText[21] = ((TUSIGN8)(x_3 / 100)) + 0x30;
    x_3 %= 100;

    // xxxxxxxxxxxx.xxxxxxxxXx
    numberText[22] = ((TUSIGN8)(x_3 / 10)) + 0x30;
    x_3 %= 10;

    // xxxxxxxxxxxx.xxxxxxxxxX
    numberText[23] = ((TUSIGN8)(x_3)) + 0x30;
#endif // defined(LONGEST_NUMBER_IS_12_DIGITS)

    // --------------------------------------------------------------------------------------------
    // Blank leading zeros


    #if defined(LONGEST_NUMBER_IS_4_DIGITS)
    i = 9;
    #elif defined(LONGEST_NUMBER_IS_8_DIGITS)
    i = 5;
    #else
    i = 1;
    #endif

    while ((numberText[i] == 0x30) && (i < 12))
    {
        numberText[i] = 0x20;
        i ++;
    }

    // --------------------------------------------------------------------------------------------
    // 11. Terminate string and return the address of the most significant digit
    if (resolution == 0)
    {
        numberText[13] = 0;                       // terminate string at dp position
        returnOffset = 13 - len;                // start of displayed string is "dp position" - "length"
    }
    else
    {
        i = resolution + 14;
        numberText[i] = 0;                        // terminate string after "least significant digit"
        returnOffset = i - len;                 // start of displayed string is "least significant digit" - "length"
    }
    return &numberText[returnOffset];
}

#ifdef HMI_USES_DOUBLE
TUSIGN8 *ConfigDumpBuildNumberStringFromDouble(TDOUBLE value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under)
{
    int x3, x2, x1, x_1, x_2, x_3, x_4;
    TUSIGN8 i,returnOffset;
    TDOUBLE limitL,limitH;

    if (len > 12)   {len = 12;}

    // --------------------------------------------------------------------------------------------
    // 2. If a configuarable DP position has been selected then get the actual DP position
    switch (resolution)
    {
    case CONFIGURABLE_DP_1:     resolution = SFD_CUSTOM_DP.customDP_1;    break;
    case CONFIGURABLE_DP_2:     resolution = SFD_CUSTOM_DP.customDP_2;    break;
    case CONFIGURABLE_DP_3:     resolution = SFD_CUSTOM_DP.customDP_3;    break;
    case CONFIGURABLE_DP_4:     resolution = SFD_CUSTOM_DP.customDP_4;    break;
    case CONFIGURABLE_DP_5:     resolution = SFD_CUSTOM_DP.customDP_5;    break;
    case CONFIGURABLE_DP_6:     resolution = SFD_CUSTOM_DP.customDP_6;    break;
    case CONFIGURABLE_DP_7:     resolution = SFD_CUSTOM_DP.customDP_7;    break;
    case CONFIGURABLE_DP_8:     resolution = SFD_CUSTOM_DP.customDP_8;    break;
    case CONFIGURABLE_DP_9:     resolution = SFD_CUSTOM_DP.customDP_9;    break;
    case CONFIGURABLE_DP_10:    resolution = SFD_CUSTOM_DP.customDP_10;   break;
    case CONFIGURABLE_DP_11:    resolution = SFD_CUSTOM_DP.customDP_11;   break;
    default:                                                                                        break;
    }

    // --------------------------------------------------------------------------------------------
    // 3. If value is less than zero then set "negative value flag" and make number positive
    if (value < 0)
    {
        value = value * -1;
        *pNegVal = 1;
    }
    else
    {
        *pNegVal = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 4. If maximum resolution has been selected then calculate the get the actual DP position
    //    according to length and absolute value
    if (resolution == 0xFF)
    {
        if      (value < 10.0)            /*   1 digit before dp  */  {i = 1;}
        else if (value < 100.0)           /*   2 digits before dp */  {i = 2;}
        else if (value < 1000.0)          /*   3 digits before dp */  {i = 3;}
        else if (value < 10000.0)         /*   4 digits before dp */  {i = 4;}
        else if (value < 100000.0)        /*   5 digits before dp */  {i = 5;}
        else if (value < 1000000.0)       /*   6 digits before dp */  {i = 6;}
        else if (value < 10000000.0)      /*   7 digits before dp */  {i = 7;}
        else if (value < 100000000.0)     /*   8 digits before dp */  {i = 8;}
        else if (value < 1000000000.0)    /*   9 digits before dp */  {i = 9;}
        else if (value < 10000000000.0)   /*  10 digits before dp */  {i = 10;}
        else if (value < 100000000000.0)  /*  11 digits before dp */  {i = 11;}
        else if (value < 1000000000000.0) /*  12 digits before dp */  {i = 12;}
        else                              /* >12 digits before dp */  {i = 13;}

        if (len <= i)   {resolution = 0;}
        else            {resolution = len - (i + 1);}
    }

    // --------------------------------------------------------------------------------------------
    // 5. Limit resolution
    if (resolution > 10)    {resolution = 10;}

    while (resolution > (len - 2) && resolution != 0)
    {
        resolution --;
    }

    // --------------------------------------------------------------------------------------------
    // 6. Calculate high limit

    limitH = 0.0f;

    if (resolution == 0)
    {
        switch (len)
        {
        case 1:     limitH = 9.0;     break;
        case 2:     limitH = 99.0;     break;
        case 3:     limitH = 999.0;     break;
        case 4:     limitH = 9999.0;     break;
        case 5:     limitH = 99999.0;     break;
        case 6:     limitH = 999999.0;     break;
        case 7:     limitH = 9999999.0;     break;
        case 8:     limitH = 99999999.0;     break;
        case 9:     limitH = 999999999.0;     break;
        case 10:    limitH = 9999999999.0;     break;
        case 11:    limitH = 99999999999.0;     break;
        case 12:    limitH = 999999999999.0;     break;
        default:    assert(0);                    break;
        }
    }
    else
    {
        switch (len)
        {
        // set limit assuming resolution = 1
        case 12:    limitH = 9999999999.9;      break;
        case 11:    limitH = 999999999.9;      break;
        case 10:    limitH = 99999999.9;      break;
        case 9:     limitH = 9999999.9;      break;
        case 8:     limitH = 999999.9;      break;
        case 7:     limitH = 99999.9;      break;
        case 6:     limitH = 9999.9;      break;
        case 5:     limitH = 999.9;      break;
        case 4:     limitH = 99.9;      break;
        case 3:     limitH = 9.9;      break;
        default:    limitH = 0.0;     break;
        }

        switch (resolution)
        {
        // adjust limit for resolution > 1
        case 10:    limitH /= 1000000000.0;      break;
        case 9:     limitH /= 100000000.0;      break;
        case 8:     limitH /= 10000000.0;      break;
        case 7:     limitH /= 1000000.0;      break;
        case 6:     limitH /= 100000.0;      break;
        case 5:     limitH /= 10000.0;      break;
        case 4:     limitH /= 1000.0;      break;
        case 3:     limitH /= 100.0;      break;
        case 2:     limitH /= 10.0;      break;
        default:    asm("nop");         break;
        }
    }

    // --------------------------------------------------------------------------------------------
    // 7. Calculate low limit
    limitL = limitH * -1;

    // --------------------------------------------------------------------------------------------
    // 8. Apply limits
    if (value > limitH)
    {
        value = limitH;
        *pWithin_over_under = 1;
    }
    else if (value < limitL)
    {
        value = limitL;
        *pWithin_over_under = 2;
    }
    else
    {
        *pWithin_over_under = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 9. Separate number

    // XXXXxxxxxxxx.xxxxxxxxxx
    x3 = (int)(value / 100000000.0);
    value -= x3 * 100000000.0;

    // xxxxXXXXxxxx.xxxxxxxxxx
    x2 = (int)(value / 10000.0);
    value -= x2 * 10000.0;

    // xxxxxxxxXXXX.xxxxxxxxxx
    x1 = (int)value;
    value -= (double)x1;

    // xxxxxxxxxxxx.XXxxxxxxxx
    value *= 100.0;
    x_1 = (int)value;
    value -= (double)x_1;

    // xxxxxxxxxxxx.xxXXXXxxxx
    value *= 10000.0;
    x_2 = (int)value;
    value -= (double)x_2;

     // xxxxxxxxxxxx.xxxxxxXXXX
    value *= 10000.0;
    x_3 = (int)value;
    value -= (double)x_3;

     // xxxxxxxxxxxx.xxxxxxxxxxXXXX
    value *= 10000.0f;
    x_4 = (int)value;

    // --------------------------------------------------------------------------------------------
    // 9. Round value according to resolution

	switch (resolution)
	{
	case 0:		x_1 += 50;		break;
	case 1:		x_1 += 5;		break;
	case 2:		x_2 += 5000;	break;
	case 3:		x_2 += 500;		break;
	case 4:		x_2	+= 50;		break;
	case 5:		x_2 += 5;		break;
	case 6:		x_3 += 5000;	break;
	case 7:		x_3 += 500;		break;
	case 8:		x_3	+= 50;		break;
	case 9:		x_3 += 5;		break;
	case 10:	x_4 += 5000;	break;
    default:    asm("nop");     break;
	}

    if (x_4 > 9999)
    {
        x_4 = 0;    x_3 ++;
    }
    if (x_3 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 ++;
    }
    if (x_2 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 ++;
    }
    if (x_1 > 99)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 ++;
    }
    if (x1 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 = 0;     x2 ++;
    }
    if (x2 > 9999)
    {
        x_4 = 0;   x_3 = 0;    x_2 = 0;    x_1 = 0;    x1 = 0;     x2 = 0;     x3 ++;
    }
    if (x3 > 9999)
    {
        x_4 = 9999;
        x_3 = 9999;
        x_2 = 9999;
        x_1 = 9999;
        x1  = 9999;
        x2  = 9999;
        x3  = 9999;
        *pWithin_over_under = 1;
        resolution = 0;
    }

    // --------------------------------------------------------------------------------------------
    // 10. Convert value blocks to text

    numberText[0] = ' ';

    // Xxxxxxxxxxxx.xxxxxxxxxx
    numberText[1] = ((TUSIGN8)(x3 / 1000)) + 0x30;
    x3 %= 1000;

    // xXxxxxxxxxxx.xxxxxxxxxx
    numberText[2] = ((TUSIGN8)(x3 / 100)) + 0x30;
    x3 %= 100;

    // xxXxxxxxxxxx.xxxxxxxxxx
    numberText[3] = ((TUSIGN8)(x3 / 10)) + 0x30;
    x3 %= 10;

    // xxxXxxxxxxxx.xxxxxxxxxx
    numberText[4] = ((TUSIGN8)(x3)) + 0x30;

    // xxxxXxxxxxxx.xxxxxxxxxx
    numberText[5] = ((TUSIGN8)(x2 / 1000)) + 0x30;
    x2 %= 1000;

    // xxxxxXxxxxxx.xxxxxxxxxx
    numberText[6] = ((TUSIGN8)(x2 / 100)) + 0x30;
    x2 %= 100;

    // xxxxxxXxxxxx.xxxxxxxxxx
    numberText[7] = ((TUSIGN8)(x2 / 10)) + 0x30;
    x2 %= 10;

    // xxxxxxxXxxxx.xxxxxxxxxx
    numberText[8] = ((TUSIGN8)(x2)) + 0x30;

    // xxxxxxxxXxxx.xxxxxxxxxx
    numberText[9] = ((TUSIGN8)(x1 / 1000)) + 0x30;
    x1 %= 1000;

    // xxxxxxxxxXxx.xxxxxxxxxx
    numberText[10] = ((TUSIGN8)(x1 / 100)) + 0x30;
    x1 %= 100;

    // xxxxxxxxxxXx.xxxxxxxxxx
    numberText[11] = ((TUSIGN8)(x1 / 10)) + 0x30;
    x1 %= 10;

    // xxxxxxxxxxxX.xxxxxxxxxx
    numberText[12] = ((TUSIGN8)(x1)) + 0x30;

    numberText[13] = '.';

    // xxxxxxxxxxxx.Xxxxxxxxxx
    numberText[14] = ((TUSIGN8)(x_1 / 10)) + 0x30;
    x_1 %= 10;

    // xxxxxxxxxxxx.xXxxxxxxxx
    numberText[15] = ((TUSIGN8)(x_1)) + 0x30;

    // xxxxxxxxxxxx.xxXxxxxxxx
    numberText[16] = ((TUSIGN8)(x_2 / 1000)) + 0x30;
    x_2 %= 1000;

    // xxxxxxxxxxxx.xxxXxxxxxx
    numberText[17] = ((TUSIGN8)(x_2 / 100)) + 0x30;
    x_2 %= 100;

    // xxxxxxxxxxxx.xxxxXxxxxx
    numberText[18] = ((TUSIGN8)(x_2 / 10)) + 0x30;
    x_2 %= 10;

    // xxxxxxxxxxxx.xxxxxXxxxx
    numberText[19] = ((TUSIGN8)(x_2)) + 0x30;

    // xxxxxxxxxxxx.xxxxxxXxxx
    numberText[20] = ((TUSIGN8)(x_3 / 1000)) + 0x30;
    x_3 %= 1000;

    // xxxxxxxxxxxx.xxxxxxxXxx
    numberText[21] = ((TUSIGN8)(x_3 / 100)) + 0x30;
    x_3 %= 100;

    // xxxxxxxxxxxx.xxxxxxxxXx
    numberText[22] = ((TUSIGN8)(x_3 / 10)) + 0x30;
    x_3 %= 10;

    // xxxxxxxxxxxx.xxxxxxxxxX
    numberText[23] = ((TUSIGN8)(x_3)) + 0x30;


    i = 1;
    while (numberText[i] == 0x30 && i < 12)
    {
        numberText[i] = 0x20;
        i ++;
    }

    // --------------------------------------------------------------------------------------------
    // 11. Terminate string and return the address of the most significant digit
    if (resolution == 0)
    {
        numberText[13] = 0;                       // terminate string at dp position
        returnOffset = 13 - len;                // start of displayed string is "dp position" - "length"
    }
    else
    {
        i = resolution + 14;
        numberText[i] = 0;                        // terminate string after "least significant digit"
        returnOffset = i - len;                 // start of displayed string is "least significant digit" - "length"
    }
    return &numberText[returnOffset];
}
#endif // end of #ifdef HMI_USES_DOUBLE


// --------------------------------------- Service port interface ---------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TBOOL ParameterDump_SP_RegisterClientEXE(void)
{
    T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;
    return pServicePort->RegisterClientEXE(&SP_ClientInfoParameterDump);
}

#pragma diag_suppress=Pm011
const char * const nonStdConversionString_Terminal[]=
{
                //              Converted From            To (Terminal)     (System)            (Standard Ascii)

    0,          // \00 (0x00)   TERMINATOR              not changed         not changed         not changed
    "\242",     // \01 (0x01)   "ó"                     "ó" \242 (0xA2)     "ó" \363 (0xF3)     "o"
    "\241",     // \02 (0x02)   "í"                     "í" \241 (0xA1)     "í" \241 (0xED)     "i"
    "\243",     // \03 (0x03)   "ú"                     "ú" \243 (0xA3)     "ú" \372 (0xFA)     "u"
    "\240",     // \04 (0x04)   "á"                     "á" \240 (0xA0)     "á" \341 (0xE1)     "a"
    "\203",     // \05 (0x05)   "å"                     "å" \203 (0x83)     "å" \345 (0xE5)     "a"
    "^",        // \06 (0x06)   up arrow                "^"                 "^"                 "^"
    "v",        // \07 (0x07)   down arrow              "v"                 "v"                 "v"
    "_",        // \10 (0x08)   three bars              "_"                 "_"                 "_"
    "_",        // \11 (0x09)   three dots              "_"                 "_"                 "_"
    "\222",     // \12 (0x0A)   "Æ"                     "Æ" \222 (0x92)     "Æ" \306 (0xC6)     "AE"
    "\370",     // \13 (0x0B)   "º"                     "º" \370 (0xF8)     "º" \272 (0xBA)     "deg"
    "\216",     // \14 (0x0C)   "Ä"                     "Ä" \216 (0x8E)     "Ä" \304 (0xC4)     "A"
    "\231",     // \15 (0x0D)   "Ö"                     "Ö" \231 (0x99)     "Ö" \326 (0xD6)     "O"
    "\232",     // \16 (0x0E)   "Ü"                     "Ü" \232 (0x9A)     "Ü" \334 (0xDC)     "U"
    "\204",     // \17 (0x0F)   "ä"                     "ä" \204 (0x84)     "ä" \344 (0xE4)     "a"

    "\224",     // \20 (0x10)   "ö"                     "ö" \224 (0x94)     "ö" \366 (0xF6)     "o"
    "\201",     // \21 (0x11)   "ü"                     "ü" \201 (0x81)     "ü" \374 (0xFC)     "u"
    "\202",     // \22 (0x12)   "é"                     "é" \202 (0x82)     "é" \351 (0xE9)     "e"
    "\212",     // \23 (0x13)   "è"                     "è" \212 (0x8A)     "è" \350 (0xE8)     "e"
    "\210",     // \24 (0x14)   "ê"                     "ê" \210 (0x88)     "ê" \352 (0xEA)     "e"
    "\207",     // \25 (0x15)   "ç"                     "ç" \207 (0x87)     "ç" \347 (0xE7)     "c"
    "OHM",      // \26 (0x16)   omega                   "OHM"               "Ohm"               "Ohm"
    "SIGMA",    // \27 (0x17)   sigma                   "SIGMA"             "S"                 "S"
    "\346",     // \30 (0x18)   "µ"                     "µ" \346 (0xE6)     "µ" \265 (0xB5)     "u"
    "\375",     // \31 (0x19)   "²"                     "²" \375 (0xFD)     "²" \262 (0xB2)     "2"
    "\374",     // \32 (0x1A)   "³"                     "³" \374 (0xFC)     "³" \263 (0xB3)     "3"
    "\221",     // \33 (0x1B)   "æ"                     "æ" \221 (0x91)     "æ" \346 (0xE6)     "ae"
    "\341",     // \34 (0x1C)   "ß"                     "ß" \341 (0xE1)     "ß" \337 (0xDF)     "ss"
    "\245",     // \35 (0x1D)   "Ñ"                     "Ñ" \245 (0xA5)     "Ñ" \321 (0xD1)     "N"
    "\244",     // \36 (0x1E)   "ñ"                     "ñ" \244 (0xA4)     "ñ" \361 (0xF1)     "n"
    "\233",     // \37 (0x1F)   "ø"                     "ø" \233 (0x9B)     "ø" \370 (0xF8)     "o"

    " ",        // \40  (0x20)  " "                     not changed         not changed         not changed
    "!",        // \41  (0x21)  "!"                     not changed         not changed         not changed
    "\"",       // \42  (0x22)  """                     not changed         not changed         not changed
    "#",        // \43  (0x23)  "#"                     not changed         not changed         not changed
    "$",        // \44  (0x24)  "$"                     not changed         not changed         not changed
    "%",        // \45  (0x25)  "%"                     not changed         not changed         not changed
    "&",        // \46  (0x26)  "&"                     not changed         not changed         not changed
    "'",        // \47  (0x27)  "'"                     not changed         not changed         not changed
    "(",        // \50  (0x28)  "("                     not changed         not changed         not changed
    ")",        // \51  (0x29)  ")"                     not changed         not changed         not changed
    "*",        // \52  (0x2A)  "*"                     not changed         not changed         not changed
    "+",        // \53  (0x2B)  "+"                     not changed         not changed         not changed
    ",",        // \54  (0x2C)  ","                     not changed         not changed         not changed
    "-",        // \55  (0x2D)  "-"                     not changed         not changed         not changed
    ".",        // \56  (0x2E)  "."                     not changed         not changed         not changed
    "/",        // \57  (0x2F)  "/"                     not changed         not changed         not changed

    "0",        // \60  (0x30)  "0"                     not changed         not changed         not changed
    "1",        // \61  (0x31)  "1"                     not changed         not changed         not changed
    "2",        // \62  (0x32)  "2"                     not changed         not changed         not changed
    "3",        // \63  (0x33)  "3"                     not changed         not changed         not changed
    "4",        // \64  (0x34)  "4"                     not changed         not changed         not changed
    "5",        // \65  (0x35)  "5"                     not changed         not changed         not changed
    "6",        // \66  (0x36)  "6"                     not changed         not changed         not changed
    "7",        // \67  (0x37)  "7"                     not changed         not changed         not changed
    "8",        // \70  (0x38)  "8"                     not changed         not changed         not changed
    "9",        // \71  (0x39)  "9"                     not changed         not changed         not changed
    ":",        // \72  (0x3A)  ":"                     not changed         not changed         not changed
    ";",        // \73  (0x3B)  ";"                     not changed         not changed         not changed
    "<",        // \74  (0x3C)  "<"                     not changed         not changed         not changed
    "=",        // \75  (0x3D)  "="                     not changed         not changed         not changed
    ">",        // \76  (0x3E)  ">"                     not changed         not changed         not changed
    "?",        // \77  (0x3F)  "?"                     not changed         not changed         not changed

    "@",        // \100 (0x40)  "@"                     not changed         not changed         not changed
    "A",        // \101 (0x41)  "A"                     not changed         not changed         not changed
    "B",        // \102 (0x42)  "B"                     not changed         not changed         not changed
    "C",        // \103 (0x43)  "C"                     not changed         not changed         not changed
    "D",        // \104 (0x44)  "D"                     not changed         not changed         not changed
    "E",        // \105 (0x45)  "E"                     not changed         not changed         not changed
    "F",        // \106 (0x46)  "F"                     not changed         not changed         not changed
    "G",        // \107 (0x47)  "G"                     not changed         not changed         not changed
    "H",        // \110 (0x48)  "H"                     not changed         not changed         not changed
    "I",        // \111 (0x49)  "I"                     not changed         not changed         not changed
    "J",        // \112 (0x4A)  "J"                     not changed         not changed         not changed
    "K",        // \113 (0x4B)  "K"                     not changed         not changed         not changed
    "L",        // \114 (0x4C)  "L"                     not changed         not changed         not changed
    "M",        // \115 (0x4D)  "M"                     not changed         not changed         not changed
    "N",        // \116 (0x4E)  "N"                     not changed         not changed         not changed
    "O",        // \117 (0x4F)  "O"                     not changed         not changed         not changed

    "P",        // \120 (0x50)  "P"                     not changed         not changed         not changed
    "Q",        // \121 (0x51)  "Q"                     not changed         not changed         not changed
    "R",        // \122 (0x52)  "R"                     not changed         not changed         not changed
    "S",        // \123 (0x53)  "S"                     not changed         not changed         not changed
    "T",        // \124 (0x54)  "T"                     not changed         not changed         not changed
    "U",        // \125 (0x55)  "U"                     not changed         not changed         not changed
    "V",        // \126 (0x56)  "V"                     not changed         not changed         not changed
    "W",        // \127 (0x57)  "W"                     not changed         not changed         not changed
    "X",        // \130 (0x58)  "X"                     not changed         not changed         not changed
    "Y",        // \131 (0x59)  "Y"                     not changed         not changed         not changed
    "Z",        // \132 (0x5A)  "Z"                     not changed         not changed         not changed
    "[",        // \133 (0x5B)  "["                     not changed         not changed         not changed
    "\\",       // \134 (0x5C)  "\"                     not changed         not changed         not changed
    "]",        // \135 (0x5D)  "]"                     not changed         not changed         not changed
    "^",        // \136 (0x5E)  "^"                     not changed         not changed         not changed
    "_",        // \137 (0x5F)  "_"                     not changed         not changed         not changed

    "`",        // \140 (0x60)  "`"                     not changed         not changed         not changed
    "a",        // \141 (0x61)  "a"                     not changed         not changed         not changed
    "b",        // \142 (0x62)  "b"                     not changed         not changed         not changed
    "c",        // \143 (0x63)  "c"                     not changed         not changed         not changed
    "d",        // \144 (0x64)  "d"                     not changed         not changed         not changed
    "e",        // \145 (0x65)  "e"                     not changed         not changed         not changed
    "f",        // \146 (0x66)  "f"                     not changed         not changed         not changed
    "g",        // \147 (0x67)  "g"                     not changed         not changed         not changed
    "h",        // \150 (0x68)  "h"                     not changed         not changed         not changed
    "i",        // \151 (0x69)  "i"                     not changed         not changed         not changed
    "j",        // \152 (0x6A)  "j"                     not changed         not changed         not changed
    "k",        // \153 (0x6B)  "k"                     not changed         not changed         not changed
    "l",        // \154 (0x6C)  "l"                     not changed         not changed         not changed
    "m",        // \155 (0x6D)  "m"                     not changed         not changed         not changed
    "n",        // \156 (0x6E)  "n"                     not changed         not changed         not changed
    "o",        // \157 (0x6F)  "o"                     not changed         not changed         not changed

    "p",        // \160 (0x70)  "p"                     not changed         not changed         not changed
    "q",        // \161 (0x71)  "q"                     not changed         not changed         not changed
    "r",        // \162 (0x72)  "r"                     not changed         not changed         not changed
    "s",        // \163 (0x73)  "s"                     not changed         not changed         not changed
    "t",        // \164 (0x74)  "t"                     not changed         not changed         not changed
    "u",        // \165 (0x75)  "u"                     not changed         not changed         not changed
    "v",        // \166 (0x76)  "v"                     not changed         not changed         not changed
    "w",        // \167 (0x77)  "w"                     not changed         not changed         not changed
    "x",        // \170 (0x78)  "x"                     not changed         not changed         not changed
    "y",        // \171 (0x79)  "y"                     not changed         not changed         not changed
    "z",        // \172 (0x7A)  "z"                     not changed         not changed         not changed
    "{",        // \173 (0x7B)  "{"                     not changed         not changed         not changed
    "|",        // \174 (0x7C)  "|"                     not changed         not changed         not changed
    "}",        // \175 (0x7D)  "}"                     not changed         not changed         not changed
    "~",        // \176 (0x7E)  "~"                     not changed         not changed         not changed
    "\137",     // \177 (0x7F)  <del>                   "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\200",     // \200 (0x80)  "Ç"                     not changed         "Ç" \307 (0xC7)     "C"
    "\201",     // \201 (0x81)  "ü"                     not changed         "ü" \374 (0xFC)     "u"
    "\202",     // \202 (0x82)  "é"                     not changed         "é" \351 (0xE9)     "e"
    "\203",     // \203 (0x83)  "â"                     not changed         "â" \342 (0xE2)     "a"
    "\204",     // \204 (0x84)  "ä"                     not changed         "ä" \344 (0xE4)     "a"
    "\205",     // \205 (0x85)  "à"                     not changed         "à" \340 (0xE0)     "a"
    "\206",     // \206 (0x86)  "å"                     not changed         "å" \345 (0xE5)     "a"
    "\207",     // \207 (0x87)  "ç"                     not changed         "ç" \347 (0xE7)     "c"
    "\210",     // \210 (0x88)  "ê"                     not changed         "ê" \352 (0xEA)     "e"
    "\211",     // \211 (0x89)  "ë"                     not changed         "ë" \353 (0xEB)     "e"
    "\212",     // \212 (0x8A)  "è"                     not changed         "è" \350 (0xE8)     "e"
    "\213",     // \213 (0x8B)  "ï"                     not changed         "ï" \357 (0xEF)     "i"
    "\214",     // \214 (0x8C)  "î"                     not changed         "î" \356 (0xEE)     "i"
    "\215",     // \215 (0x8D)  "ì"                     not changed         "ì" \354 (0xEC)     "i"
    "\216",     // \216 (0x8E)  "Ä"                     not changed         "Ä" \304 (0xC4)     "A"
    "\217",     // \217 (0x8F)  "Å"                     not changed         "Å" \305 (0xC5)     "A"

    "\220",     // \220 (0x90)  "É"                     not changed         "É" \311 (0xC9)     "E"
    "\221",     // \221 (0x91)  "æ"                     not changed         "æ" \346 (0xE6)     "ae"
    "\222",     // \222 (0x92)  "Æ"                     not changed         "Æ" \306 (0xC6)     "AE"
    "\223",     // \223 (0x93)  "ô"                     not changed         "ô" \364 (0xF4)     "o"
    "\224",     // \224 (0x94)  "ö"                     not changed         "ö" \366 (0xF6)     "o"
    "\225",     // \225 (0x95)  "ò"                     not changed         "ò" \362 (0xF2)     "o"
    "\226",     // \226 (0x96)  "û"                     not changed         "û" \373 (0xFB)     "u"
    "\227",     // \227 (0x97)  "ù"                     not changed         "ù" \371 (0xF9)     "u"
    "\230",     // \230 (0x98)  "ÿ"                     not changed         "ÿ" \377 (0xFF)     "y"
    "\231",     // \231 (0x99)  "Ö"                     not changed         "Ö" \326 (0xD6)     "O"
    "\232",     // \232 (0x9A)  "Ü"                     not changed         "Ü" \334 (0xDC)     "U"
    "\233",     // \233 (0x9B)  "ø"                     not changed         "ø" \370 (0xF8)     "o"
    "\137",     // \234 (0x9C)  Ka (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\235",     // \235 (0x9D)  "Ø"                     not changed         "Ø" \330 (0xD8)     "O"
    "\137",     // \236 (0x9E)  Be (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \237 (0x9F)  Be (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\240",     // \240 (0xA0)  "á"                     not changed         "á" \341 (0xE1)     "a"
    "\241",     // \241 (0xA1)  "í"                     not changed         "í" \355 (0xED)     "i"
    "\242",     // \242 (0xA2)  "ó"                     not changed         "ó" \363 (0xF3)     "o"
    "\243",     // \243 (0xA3)  "ú"                     not changed         "ú" \372 (0xFA)     "u"
    "\244",     // \244 (0xA4)  "ñ"                     not changed         "ñ" \361 (0xF1)     "n"
    "\245",     // \245 (0xA5)  "Ñ"                     not changed         "Ñ" \321 (0xD1)     "N"
    "\137",     // \246 (0xA6)   Ge (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \247 (0xA7)   Ge (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \250 (0xA8)   De (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \251 (0xA9)   De (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \252 (0xAA)   Zhe (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \253 (0xAB)   Zhe (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \254 (0xAC)   Ze (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \255 (0xAD)   Ze (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \256 (0xAE)   I (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \257 (0xAF)   I (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \260 (0xB0)   Short I (Cyrillic) uc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \261 (0xB1)   Short I (Cyrillic) lc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \262 (0xB2)   El (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \263 (0xB3)   El (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \264 (0xB4)   Em (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\265",     // \265 (0xB5)   "Á"                    not changed         "Á" \301 (0xC1)     "A"
    "\266",     // \266 (0xB6)   "Â"                    not changed         "Â" \302 (0xC2)     "A"
    "\267",     // \267 (0xB7)   "À"                    not changed         "À" \300 (0xC0)     "A"
    "\137",     // \270 (0xB8)   Pe (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \271 (0xB9)   Pe (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \272 (0xBA)   U (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \273 (0xBB)   U (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \274 (0xBC)   Ef (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \275 (0xBD)   Ef (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \276 (0xBE)   Tse (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \277 (0xBF)   Tse (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \300 (0xC0)   Che (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \301 (0xC1)   Che (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \302 (0xC2)   Sha (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \303 (0xC3)   Sha (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \304 (0xC4)   Shcha (Cyrillic) uc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \305 (0xC5)   Shcha (Cyrillic) lc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\306",     // \306 (0xC6)   "ã"                    not changed         "ã" \343 (0xE3)     "a"
    "\307",     // \307 (0xC7)   "Ã"                    not changed         "Ã" \303 (0xC3)     "A"
    "\137",     // \310 (0xC8)   Yer (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \311 (0xC9)   Yer (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \312 (0xCA)   Yery (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \313 (0xCB)   Yery (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \314 (0xCC)   Soft (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \315 (0xCD)   Soft (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \316 (0xCE)   E (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \317 (0xCF)   E (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \320 (0xD0)   Yu (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \321 (0xD1)   Yu (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\322",     // \322 (0xD2)   "Ê"                    not changed         "Ê" \312 (0xCA)     "E"
    "\323",     // \323 (0xD3)   "Ë"                    not changed         "Ë" \313 (0xCB)     "E"
    "\324",     // \324 (0xD4)   "È"                    not changed         "È" \310 (0xC8)     "E"
    "\137",     // \325 (0xD5)   En (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\326",     // \326 (0xD6)   "Í"                    not changed         "Í" \315 (0xCD)     "I"
    "\327",     // \327 (0xD7)   "Î"                    not changed         "Î" \316 (0xCE)     "I"
    "\330",     // \330 (0xD8)   "Ï"                    not changed         "Ï" \317 (0xCF)     "I"
    "\137",     // \331 (0xD9)   Ya (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \332 (0xDA)   Ya (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "A",        // \333 (0xDB)   "A" + Ogonek           "A"                 "A"                 "A"
    "a",        // \334 (0xDC)   "a" + Ogonek           "a"                 "a"                 "a"
    "\137",     // \335 (0xDD)   Ve (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\336",     // \336 (0xD2)   "Ì"                    not changed         "Ì" \314 (0xCC)     "I"
    "\137",     // \337 (0xDF)   Te (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\340",     // \340 (0xE0)   "ó"                    not changed         "ó" \363 (0xF3)     "o"
    "\341",     // \341 (0xE1)   "ß"                    not changed         "ß" \337 (0xDF)     "ss"
    "\342",     // \342 (0xE2)   "Ô"                    not changed         "Ô" \324 (0xD4)     "O"
    "\343",     // \343 (0xE3)   "Ò"                    not changed         "Ò" \322 (0xD2)     "O"
    "\344",     // \344 (0xE4)   "õ"                    not changed         "õ" \365 (0xF5)     "o"
    "\345",     // \345 (0xE5)   "Õ"                    not changed         "Õ" \325 (0xD5)     "O"
    "\346",     // \346 (0xE6)   "µ"                    not changed         "µ" \265 (0xB5)     "u"
    "C",        // \347 (0xE7)   "C" + acute            "C"                 "C"                 "C"
    "c",        // \350 (0xE8)   "c" + acute            "c"                 "c"                 "c"
    "\351",     // \351 (0xE9)   "Ú"                    not changed         "Ú" \332 (0xDA)     "U"
    "\352",     // \352 (0xEA)   "Û"                    not changed         "Û" \333 (0xDB)     "U"
    "\353",     // \353 (0xEB)   "Ù"                    not changed         "Ù" \331 (0xD9)     "U"
    "\354",     // \354 (0xEC)   "ý"                    not changed         "ý" \375 (0xFD)     "y"
    "\355",     // \355 (0xED)   "Ý"                    not changed         "Ý" \335 (0xDD)     "Y"
    "E",        // \356 (0xEE)   "E" + Ogonek           "E"                 "E"                 "E"
    "e",        // \357 (0xEF)   "e" + Ogonek           "e"                 "e"                 "e"

    "L",        // \360 (0xF0)   "L" + stroke           "L"                 "L"                 "L"
    "l",        // \361 (0xF1)   "l" + stroke           "l"                 "l"                 "l"
    "N",        // \362 (0xF2)   "N" + acute            "N"                 "N"                 "N"
    "n",        // \363 (0xF3)   "n" + acute            "n"                 "n"                 "n"
    "S",        // \364 (0xF4)   "S" + acute            "S"                 "S"                 "S"
    "s",        // \365 (0xF5)   "s" + acute            "s"                 "s"                 "s"
    "Z",        // \366 (0xF6)   "Z" + acute            "Z"                 "Z"                 "Z"
    "z",        // \367 (0xF7)   "z" + acute            "z"                 "z"                 "z"
    "\370",     // \370 (0xF8)   "°"                    not changed         "º" \272 (0xBA)     "deg"
    "Z",        // \371 (0xF9)   "Z" + single dot above "Z"                 "Z"                 "Z"
    "z",        // \372 (0xFA)   "z" + single dot above "z"                 "z"                 "z"
    "Y",        // \373 (0xFB)   "Ÿ"                    "Y"                 "Ÿ" \237 (0x9F)     "Y"
    "\374",     // \374 (0xFC)   "³"                    not changed         "³" \263 (0xB3)     "3"
    "\375",     // \375 (0xFD)   "²"                    not changed         "²" \262 (0xB2)     "2"
    "\137",     // \376 (0xFE)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137"      // \377 (0xFF)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
};

const char * const nonStdConversionString_System[]=
{
                //              Converted From            To (Terminal)     (System)            (Standard Ascii)

    0,          // \00 (0x00)   TERMINATOR              not changed         not changed         not changed
    "\363",     // \01 (0x01)   "ó"                     "ó" \242 (0xA2)     "ó" \363 (0xF3)     "o"
    "\241",     // \02 (0x02)   "í"                     "í" \241 (0xA1)     "í" \241 (0xED)     "i"
    "\372",     // \03 (0x03)   "ú"                     "ú" \243 (0xA3)     "ú" \372 (0xFA)     "u"
    "\341",     // \04 (0x04)   "á"                     "á" \240 (0xA0)     "á" \341 (0xE1)     "a"
    "\345",     // \05 (0x05)   "å"                     "å" \203 (0x83)     "å" \345 (0xE5)     "a"
    "^",        // \06 (0x06)   up arrow                "^"                 "^"                 "^"
    "v",        // \07 (0x07)   down arrow              "v"                 "v"                 "v"
    "_",        // \10 (0x08)   three bars              "_"                 "_"                 "_"
    "_",        // \11 (0x09)   three dots              "_"                 "_"                 "_"
    "\306",     // \12 (0x0A)   "Æ"                     "Æ" \222 (0x92)     "Æ" \306 (0xC6)     "AE"
    "\272",     // \13 (0x0B)   "º"                     "º" \370 (0xF8)     "º" \272 (0xBA)     "deg"
    "\304",     // \14 (0x0C)   "Ä"                     "Ä" \216 (0x8E)     "Ä" \304 (0xC4)     "A"
    "\326",     // \15 (0x0D)   "Ö"                     "Ö" \231 (0x99)     "Ö" \326 (0xD6)     "O"
    "\334",     // \16 (0x0E)   "Ü"                     "Ü" \232 (0x9A)     "Ü" \334 (0xDC)     "U"
    "\344",     // \17 (0x0F)   "ä"                     "ä" \204 (0x84)     "ä" \344 (0xE4)     "a"

    "\366",     // \20 (0x10)   "ö"                     "ö" \224 (0x94)     "ö" \366 (0xF6)     "o"
    "\374",     // \21 (0x11)   "ü"                     "ü" \201 (0x81)     "ü" \374 (0xFC)     "u"
    "\351",     // \22 (0x12)   "é"                     "é" \202 (0x82)     "é" \351 (0xE9)     "e"
    "\350",     // \23 (0x13)   "è"                     "è" \212 (0x8A)     "è" \350 (0xE8)     "e"
    "\352",     // \24 (0x14)   "ê"                     "ê" \210 (0x88)     "ê" \352 (0xEA)     "e"
    "\347",     // \25 (0x15)   "ç"                     "ç" \207 (0x87)     "ç" \347 (0xE7)     "c"
    "Ohm",      // \26 (0x16)   omega                   "OHM"               "Ohm"               "Ohm"
    "S",        // \27 (0x17)   sigma                   "SIGMA"             "S"                 "S"
    "\265",     // \30 (0x18)   "µ"                     "µ" \346 (0xE6)     "µ" \265 (0xB5)     "u"
    "\262",     // \31 (0x19)   "²"                     "²" \375 (0xFD)     "²" \262 (0xB2)     "2"
    "\263",     // \32 (0x1A)   "³"                     "³" \374 (0xFC)     "³" \263 (0xB3)     "3"
    "\346",     // \33 (0x1B)   "æ"                     "æ" \221 (0x91)     "æ" \346 (0xE6)     "ae"
    "\337",     // \34 (0x1C)   "ß"                     "ß" \341 (0xE1)     "ß" \337 (0xDF)     "ss"
    "\321",     // \35 (0x1D)   "Ñ"                     "Ñ" \245 (0xA5)     "Ñ" \321 (0xD1)     "N"
    "\361",     // \36 (0x1E)   "ñ"                     "ñ" \244 (0xA4)     "ñ" \361 (0xF1)     "n"
    "\370",     // \37 (0x1F)   "ø"                     "ø" \233 (0x9B)     "ø" \370 (0xF8)     "o"

    " ",        // \40  (0x20)  " "                     not changed         not changed         not changed
    "!",        // \41  (0x21)  "!"                     not changed         not changed         not changed
    "\"",       // \42  (0x22)  """                     not changed         not changed         not changed
    "#",        // \43  (0x23)  "#"                     not changed         not changed         not changed
    "$",        // \44  (0x24)  "$"                     not changed         not changed         not changed
    "%",        // \45  (0x25)  "%"                     not changed         not changed         not changed
    "&",        // \46  (0x26)  "&"                     not changed         not changed         not changed
    "'",        // \47  (0x27)  "'"                     not changed         not changed         not changed
    "(",        // \50  (0x28)  "("                     not changed         not changed         not changed
    ")",        // \51  (0x29)  ")"                     not changed         not changed         not changed
    "*",        // \52  (0x2A)  "*"                     not changed         not changed         not changed
    "+",        // \53  (0x2B)  "+"                     not changed         not changed         not changed
    ",",        // \54  (0x2C)  ","                     not changed         not changed         not changed
    "-",        // \55  (0x2D)  "-"                     not changed         not changed         not changed
    ".",        // \56  (0x2E)  "."                     not changed         not changed         not changed
    "/",        // \57  (0x2F)  "/"                     not changed         not changed         not changed

    "0",        // \60  (0x30)  "0"                     not changed         not changed         not changed
    "1",        // \61  (0x31)  "1"                     not changed         not changed         not changed
    "2",        // \62  (0x32)  "2"                     not changed         not changed         not changed
    "3",        // \63  (0x33)  "3"                     not changed         not changed         not changed
    "4",        // \64  (0x34)  "4"                     not changed         not changed         not changed
    "5",        // \65  (0x35)  "5"                     not changed         not changed         not changed
    "6",        // \66  (0x36)  "6"                     not changed         not changed         not changed
    "7",        // \67  (0x37)  "7"                     not changed         not changed         not changed
    "8",        // \70  (0x38)  "8"                     not changed         not changed         not changed
    "9",        // \71  (0x39)  "9"                     not changed         not changed         not changed
    ":",        // \72  (0x3A)  ":"                     not changed         not changed         not changed
    ";",        // \73  (0x3B)  ";"                     not changed         not changed         not changed
    "<",        // \74  (0x3C)  "<"                     not changed         not changed         not changed
    "=",        // \75  (0x3D)  "="                     not changed         not changed         not changed
    ">",        // \76  (0x3E)  ">"                     not changed         not changed         not changed
    "?",        // \77  (0x3F)  "?"                     not changed         not changed         not changed

    "@",        // \100 (0x40)  "@"                     not changed         not changed         not changed
    "A",        // \101 (0x41)  "A"                     not changed         not changed         not changed
    "B",        // \102 (0x42)  "B"                     not changed         not changed         not changed
    "C",        // \103 (0x43)  "C"                     not changed         not changed         not changed
    "D",        // \104 (0x44)  "D"                     not changed         not changed         not changed
    "E",        // \105 (0x45)  "E"                     not changed         not changed         not changed
    "F",        // \106 (0x46)  "F"                     not changed         not changed         not changed
    "G",        // \107 (0x47)  "G"                     not changed         not changed         not changed
    "H",        // \110 (0x48)  "H"                     not changed         not changed         not changed
    "I",        // \111 (0x49)  "I"                     not changed         not changed         not changed
    "J",        // \112 (0x4A)  "J"                     not changed         not changed         not changed
    "K",        // \113 (0x4B)  "K"                     not changed         not changed         not changed
    "L",        // \114 (0x4C)  "L"                     not changed         not changed         not changed
    "M",        // \115 (0x4D)  "M"                     not changed         not changed         not changed
    "N",        // \116 (0x4E)  "N"                     not changed         not changed         not changed
    "O",        // \117 (0x4F)  "O"                     not changed         not changed         not changed

    "P",        // \120 (0x50)  "P"                     not changed         not changed         not changed
    "Q",        // \121 (0x51)  "Q"                     not changed         not changed         not changed
    "R",        // \122 (0x52)  "R"                     not changed         not changed         not changed
    "S",        // \123 (0x53)  "S"                     not changed         not changed         not changed
    "T",        // \124 (0x54)  "T"                     not changed         not changed         not changed
    "U",        // \125 (0x55)  "U"                     not changed         not changed         not changed
    "V",        // \126 (0x56)  "V"                     not changed         not changed         not changed
    "W",        // \127 (0x57)  "W"                     not changed         not changed         not changed
    "X",        // \130 (0x58)  "X"                     not changed         not changed         not changed
    "Y",        // \131 (0x59)  "Y"                     not changed         not changed         not changed
    "Z",        // \132 (0x5A)  "Z"                     not changed         not changed         not changed
    "[",        // \133 (0x5B)  "["                     not changed         not changed         not changed
    "\\",       // \134 (0x5C)  "\"                     not changed         not changed         not changed
    "]",        // \135 (0x5D)  "]"                     not changed         not changed         not changed
    "^",        // \136 (0x5E)  "^"                     not changed         not changed         not changed
    "_",        // \137 (0x5F)  "_"                     not changed         not changed         not changed

    "`",        // \140 (0x60)  "`"                     not changed         not changed         not changed
    "a",        // \141 (0x61)  "a"                     not changed         not changed         not changed
    "b",        // \142 (0x62)  "b"                     not changed         not changed         not changed
    "c",        // \143 (0x63)  "c"                     not changed         not changed         not changed
    "d",        // \144 (0x64)  "d"                     not changed         not changed         not changed
    "e",        // \145 (0x65)  "e"                     not changed         not changed         not changed
    "f",        // \146 (0x66)  "f"                     not changed         not changed         not changed
    "g",        // \147 (0x67)  "g"                     not changed         not changed         not changed
    "h",        // \150 (0x68)  "h"                     not changed         not changed         not changed
    "i",        // \151 (0x69)  "i"                     not changed         not changed         not changed
    "j",        // \152 (0x6A)  "j"                     not changed         not changed         not changed
    "k",        // \153 (0x6B)  "k"                     not changed         not changed         not changed
    "l",        // \154 (0x6C)  "l"                     not changed         not changed         not changed
    "m",        // \155 (0x6D)  "m"                     not changed         not changed         not changed
    "n",        // \156 (0x6E)  "n"                     not changed         not changed         not changed
    "o",        // \157 (0x6F)  "o"                     not changed         not changed         not changed

    "p",        // \160 (0x70)  "p"                     not changed         not changed         not changed
    "q",        // \161 (0x71)  "q"                     not changed         not changed         not changed
    "r",        // \162 (0x72)  "r"                     not changed         not changed         not changed
    "s",        // \163 (0x73)  "s"                     not changed         not changed         not changed
    "t",        // \164 (0x74)  "t"                     not changed         not changed         not changed
    "u",        // \165 (0x75)  "u"                     not changed         not changed         not changed
    "v",        // \166 (0x76)  "v"                     not changed         not changed         not changed
    "w",        // \167 (0x77)  "w"                     not changed         not changed         not changed
    "x",        // \170 (0x78)  "x"                     not changed         not changed         not changed
    "y",        // \171 (0x79)  "y"                     not changed         not changed         not changed
    "z",        // \172 (0x7A)  "z"                     not changed         not changed         not changed
    "{",        // \173 (0x7B)  "{"                     not changed         not changed         not changed
    "|",        // \174 (0x7C)  "|"                     not changed         not changed         not changed
    "}",        // \175 (0x7D)  "}"                     not changed         not changed         not changed
    "~",        // \176 (0x7E)  "~"                     not changed         not changed         not changed
    "\137",     // \177 (0x7F)  <del>                   "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\307",     // \200 (0x80)  "Ç"                     not changed         "Ç" \307 (0xC7)     "C"
    "\374",     // \201 (0x81)  "ü"                     not changed         "ü" \374 (0xFC)     "u"
    "\351",     // \202 (0x82)  "é"                     not changed         "é" \351 (0xE9)     "e"
    "\342",     // \203 (0x83)  "â"                     not changed         "â" \342 (0xE2)     "a"
    "\344",     // \204 (0x84)  "ä"                     not changed         "ä" \344 (0xE4)     "a"
    "\340",     // \205 (0x85)  "à"                     not changed         "à" \340 (0xE0)     "a"
    "\345",     // \206 (0x86)  "å"                     not changed         "å" \345 (0xE5)     "a"
    "\347",     // \207 (0x87)  "ç"                     not changed         "ç" \347 (0xE7)     "c"
    "\352",     // \210 (0x88)  "ê"                     not changed         "ê" \352 (0xEA)     "e"
    "\353",     // \211 (0x89)  "ë"                     not changed         "ë" \353 (0xEB)     "e"
    "\350",     // \212 (0x8A)  "è"                     not changed         "è" \350 (0xE8)     "e"
    "\357",     // \213 (0x8B)  "ï"                     not changed         "ï" \357 (0xEF)     "i"
    "\356",     // \214 (0x8C)  "î"                     not changed         "î" \356 (0xEE)     "i"
    "\354",     // \215 (0x8D)  "ì"                     not changed         "ì" \354 (0xEC)     "i"
    "\304",     // \216 (0x8E)  "Ä"                     not changed         "Ä" \304 (0xC4)     "A"
    "\305",     // \217 (0x8F)  "Å"                     not changed         "Å" \305 (0xC5)     "A"

    "\311",     // \220 (0x90)  "É"                     not changed         "É" \311 (0xC9)     "E"
    "\346",     // \221 (0x91)  "æ"                     not changed         "æ" \346 (0xE6)     "ae"
    "\306",     // \222 (0x92)  "Æ"                     not changed         "Æ" \306 (0xC6)     "AE"
    "\364",     // \223 (0x93)  "ô"                     not changed         "ô" \364 (0xF4)     "o"
    "\366",     // \224 (0x94)  "ö"                     not changed         "ö" \366 (0xF6)     "o"
    "\362",     // \225 (0x95)  "ò"                     not changed         "ò" \362 (0xF2)     "o"
    "\373",     // \226 (0x96)  "û"                     not changed         "û" \373 (0xFB)     "u"
    "\371",     // \227 (0x97)  "ù"                     not changed         "ù" \371 (0xF9)     "u"
    "\377",     // \230 (0x98)  "ÿ"                     not changed         "ÿ" \377 (0xFF)     "y"
    "\326",     // \231 (0x99)  "Ö"                     not changed         "Ö" \326 (0xD6)     "O"
    "\334",     // \232 (0x9A)  "Ü"                     not changed         "Ü" \334 (0xDC)     "U"
    "\370",     // \233 (0x9B)  "ø"                     not changed         "ø" \370 (0xF8)     "o"
    "\137",     // \234 (0x9C)  Ka (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\330",     // \235 (0x9D)  "Ø"                     not changed         "Ø" \330 (0xD8)     "O"
    "\137",     // \236 (0x9E)  Be (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \237 (0x9F)  Be (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\341",     // \240 (0xA0)  "á"                     not changed         "á" \341 (0xE1)     "a"
    "\355",     // \241 (0xA1)  "í"                     not changed         "í" \355 (0xED)     "i"
    "\363",     // \242 (0xA2)  "ó"                     not changed         "ó" \363 (0xF3)     "o"
    "\372",     // \243 (0xA3)  "ú"                     not changed         "ú" \372 (0xFA)     "u"
    "\361",     // \244 (0xA4)  "ñ"                     not changed         "ñ" \361 (0xF1)     "n"
    "\321",     // \245 (0xA5)  "Ñ"                     not changed         "Ñ" \321 (0xD1)     "N"
    "\137",     // \246 (0xA6)   Ge (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \247 (0xA7)   Ge (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \250 (0xA8)   De (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \251 (0xA9)   De (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \252 (0xAA)   Zhe (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \253 (0xAB)   Zhe (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \254 (0xAC)   Ze (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \255 (0xAD)   Ze (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \256 (0xAE)   I (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \257 (0xAF)   I (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \260 (0xB0)   Short I (Cyrillic) uc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \261 (0xB1)   Short I (Cyrillic) lc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \262 (0xB2)   El (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \263 (0xB3)   El (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \264 (0xB4)   Em (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\301",     // \265 (0xB5)   "Á"                    not changed         "Á" \301 (0xC1)     "A"
    "\302",     // \266 (0xB6)   "Â"                    not changed         "Â" \302 (0xC2)     "A"
    "\300",     // \267 (0xB7)   "À"                    not changed         "À" \300 (0xC0)     "A"
    "\137",     // \270 (0xB8)   Pe (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \271 (0xB9)   Pe (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \272 (0xBA)   U (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \273 (0xBB)   U (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \274 (0xBC)   Ef (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \275 (0xBD)   Ef (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \276 (0xBE)   Tse (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \277 (0xBF)   Tse (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \300 (0xC0)   Che (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \301 (0xC1)   Che (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \302 (0xC2)   Sha (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \303 (0xC3)   Sha (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \304 (0xC4)   Shcha (Cyrillic) uc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \305 (0xC5)   Shcha (Cyrillic) lc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\343",     // \306 (0xC6)   "ã"                    not changed         "ã" \343 (0xE3)     "a"
    "\303",     // \307 (0xC7)   "Ã"                    not changed         "Ã" \303 (0xC3)     "A"
    "\137",     // \310 (0xC8)   Yer (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \311 (0xC9)   Yer (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \312 (0xCA)   Yery (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \313 (0xCB)   Yery (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \314 (0xCC)   Soft (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \315 (0xCD)   Soft (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \316 (0xCE)   E (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \317 (0xCF)   E (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \320 (0xD0)   Yu (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \321 (0xD1)   Yu (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\312",     // \322 (0xD2)   "Ê"                    not changed         "Ê" \312 (0xCA)     "E"
    "\313",     // \323 (0xD3)   "Ë"                    not changed         "Ë" \313 (0xCB)     "E"
    "\310",     // \324 (0xD4)   "È"                    not changed         "È" \310 (0xC8)     "E"
    "\137",     // \325 (0xD5)   En (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\315",     // \326 (0xD6)   "Í"                    not changed         "Í" \315 (0xCD)     "I"
    "\316",     // \327 (0xD7)   "Î"                    not changed         "Î" \316 (0xCE)     "I"
    "\317",     // \330 (0xD8)   "Ï"                    not changed         "Ï" \317 (0xCF)     "I"
    "\137",     // \331 (0xD9)   Ya (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \332 (0xDA)   Ya (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "A",        // \333 (0xDB)   "A" + Ogonek           "A"                 "A"                 "A"
    "a",        // \334 (0xDC)   "a" + Ogonek           "a"                 "a"                 "a"
    "\137",     // \335 (0xDD)   Ve (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\314",     // \336 (0xD2)   "Ì"                    not changed         "Ì" \314 (0xCC)     "I"
    "\137",     // \337 (0xDF)   Te (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\363",     // \340 (0xE0)   "ó"                    not changed         "ó" \363 (0xF3)     "o"
    "\337",     // \341 (0xE1)   "ß"                    not changed         "ß" \337 (0xDF)     "ss"
    "\324",     // \342 (0xE2)   "Ô"                    not changed         "Ô" \324 (0xD4)     "O"
    "\322",     // \343 (0xE3)   "Ò"                    not changed         "Ò" \322 (0xD2)     "O"
    "\365",     // \344 (0xE4)   "õ"                    not changed         "õ" \365 (0xF5)     "o"
    "\325",     // \345 (0xE5)   "Õ"                    not changed         "Õ" \325 (0xD5)     "O"
    "\265",     // \346 (0xE6)   "µ"                    not changed         "µ" \265 (0xB5)     "u"
    "C",        // \347 (0xE7)   "C" + acute            "C"                 "C"                 "C"
    "c",        // \350 (0xE8)   "c" + acute            "c"                 "c"                 "c"
    "\332",     // \351 (0xE9)   "Ú"                    not changed         "Ú" \332 (0xDA)     "U"
    "\333",     // \352 (0xEA)   "Û"                    not changed         "Û" \333 (0xDB)     "U"
    "\331",     // \353 (0xEB)   "Ù"                    not changed         "Ù" \331 (0xD9)     "U"
    "\375",     // \354 (0xEC)   "ý"                    not changed         "ý" \375 (0xFD)     "y"
    "\335",     // \355 (0xED)   "Ý"                    not changed         "Ý" \335 (0xDD)     "Y"
    "E",        // \356 (0xEE)   "E" + Ogonek           "E"                 "E"                 "E"
    "e",        // \357 (0xEF)   "e" + Ogonek           "e"                 "e"                 "e"

    "L",        // \360 (0xF0)   "L" + stroke           "L"                 "L"                 "L"
    "l",        // \361 (0xF1)   "l" + stroke           "l"                 "l"                 "l"
    "N",        // \362 (0xF2)   "N" + acute            "N"                 "N"                 "N"
    "n",        // \363 (0xF3)   "n" + acute            "n"                 "n"                 "n"
    "S",        // \364 (0xF4)   "S" + acute            "S"                 "S"                 "S"
    "s",        // \365 (0xF5)   "s" + acute            "s"                 "s"                 "s"
    "Z",        // \366 (0xF6)   "Z" + acute            "Z"                 "Z"                 "Z"
    "z",        // \367 (0xF7)   "z" + acute            "z"                 "z"                 "z"
    "\272",     // \370 (0xF8)   "°"                    not changed         "º" \272 (0xBA)     "deg"
    "Z",        // \371 (0xF9)   "Z" + single dot above "Z"                 "Z"                 "Z"
    "z",        // \372 (0xFA)   "z" + single dot above "z"                 "z"                 "z"
    "\237",     // \373 (0xFB)   "Ÿ"                    "Y"                 "Ÿ" \237 (0x9F)     "Y"
    "\263",     // \374 (0xFC)   "³"                    not changed         "³" \263 (0xB3)     "3"
    "\262",     // \375 (0xFD)   "²"                    not changed         "²" \262 (0xB2)     "2"
    "\137",     // \376 (0xFE)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137"      // \377 (0xFF)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
};

const char * const nonStdConversionString_Standard[]=
{
                //              Converted From            To (Terminal)     (System)            (Standard Ascii)

    0,          // \00 (0x00)   TERMINATOR              not changed         not changed         not changed
    "o",        // \01 (0x01)   "ó"                     "ó" \242 (0xA2)     "ó" \363 (0xF3)     "o"
    "i",        // \02 (0x02)   "í"                     "í" \241 (0xA1)     "í" \241 (0xED)     "i"
    "u",        // \03 (0x03)   "ú"                     "ú" \243 (0xA3)     "ú" \372 (0xFA)     "u"
    "a",        // \04 (0x04)   "á"                     "á" \240 (0xA0)     "á" \341 (0xE1)     "a"
    "a",        // \05 (0x05)   "å"                     "å" \203 (0x83)     "å" \345 (0xE5)     "a"
    "^",        // \06 (0x06)   up arrow                "^"                 "^"                 "^"
    "v",        // \07 (0x07)   down arrow              "v"                 "v"                 "v"
    "_",        // \10 (0x08)   three bars              "_"                 "_"                 "_"
    "_",        // \11 (0x09)   three dots              "_"                 "_"                 "_"
    "AE",       // \12 (0x0A)   "Æ"                     "Æ" \222 (0x92)     "Æ" \306 (0xC6)     "AE"
    "deg",      // \13 (0x0B)   "º"                     "º" \370 (0xF8)     "º" \272 (0xBA)     "deg"
    "A",        // \14 (0x0C)   "Ä"                     "Ä" \216 (0x8E)     "Ä" \304 (0xC4)     "A"
    "O",        // \15 (0x0D)   "Ö"                     "Ö" \231 (0x99)     "Ö" \326 (0xD6)     "O"
    "U",        // \16 (0x0E)   "Ü"                     "Ü" \232 (0x9A)     "Ü" \334 (0xDC)     "U"
    "A",        // \17 (0x0F)   "ä"                     "ä" \204 (0x84)     "ä" \344 (0xE4)     "a"

    "o",        // \20 (0x10)   "ö"                     "ö" \224 (0x94)     "ö" \366 (0xF6)     "o"
    "u",        // \21 (0x11)   "ü"                     "ü" \201 (0x81)     "ü" \374 (0xFC)     "u"
    "e",        // \22 (0x12)   "é"                     "é" \202 (0x82)     "é" \351 (0xE9)     "e"
    "e",        // \23 (0x13)   "è"                     "è" \212 (0x8A)     "è" \350 (0xE8)     "e"
    "e",        // \24 (0x14)   "ê"                     "ê" \210 (0x88)     "ê" \352 (0xEA)     "e"
    "c",        // \25 (0x15)   "ç"                     "ç" \207 (0x87)     "ç" \347 (0xE7)     "c"
    "Ohm",      // \26 (0x16)   omega                   "OHM"               "Ohm"               "Ohm"
    "S",        // \27 (0x17)   sigma                   "SIGMA"             "S"                 "S"
    "u",        // \30 (0x18)   "µ"                     "µ" \346 (0xE6)     "µ" \265 (0xB5)     "u"
    "2",        // \31 (0x19)   "²"                     "²" \375 (0xFD)     "²" \262 (0xB2)     "2"
    "3",        // \32 (0x1A)   "³"                     "³" \374 (0xFC)     "³" \263 (0xB3)     "3"
    "ae",       // \33 (0x1B)   "æ"                     "æ" \221 (0x91)     "æ" \346 (0xE6)     "ae"
    "ss",       // \34 (0x1C)   "ß"                     "ß" \341 (0xE1)     "ß" \337 (0xDF)     "ss"
    "N",        // \35 (0x1D)   "Ñ"                     "Ñ" \245 (0xA5)     "Ñ" \321 (0xD1)     "N"
    "n",        // \36 (0x1E)   "ñ"                     "ñ" \244 (0xA4)     "ñ" \361 (0xF1)     "n"
    "o",        // \37 (0x1F)   "ø"                     "ø" \233 (0x9B)     "ø" \370 (0xF8)     "o"

    " ",        // \40  (0x20)  " "                     not changed         not changed         not changed
    "!",        // \41  (0x21)  "!"                     not changed         not changed         not changed
    "\"",       // \42  (0x22)  """                     not changed         not changed         not changed
    "#",        // \43  (0x23)  "#"                     not changed         not changed         not changed
    "$",        // \44  (0x24)  "$"                     not changed         not changed         not changed
    "%",        // \45  (0x25)  "%"                     not changed         not changed         not changed
    "&",        // \46  (0x26)  "&"                     not changed         not changed         not changed
    "'",        // \47  (0x27)  "'"                     not changed         not changed         not changed
    "(",        // \50  (0x28)  "("                     not changed         not changed         not changed
    ")",        // \51  (0x29)  ")"                     not changed         not changed         not changed
    "*",        // \52  (0x2A)  "*"                     not changed         not changed         not changed
    "+",        // \53  (0x2B)  "+"                     not changed         not changed         not changed
    ",",        // \54  (0x2C)  ","                     not changed         not changed         not changed
    "-",        // \55  (0x2D)  "-"                     not changed         not changed         not changed
    ".",        // \56  (0x2E)  "."                     not changed         not changed         not changed
    "/",        // \57  (0x2F)  "/"                     not changed         not changed         not changed

    "0",        // \60  (0x30)  "0"                     not changed         not changed         not changed
    "1",        // \61  (0x31)  "1"                     not changed         not changed         not changed
    "2",        // \62  (0x32)  "2"                     not changed         not changed         not changed
    "3",        // \63  (0x33)  "3"                     not changed         not changed         not changed
    "4",        // \64  (0x34)  "4"                     not changed         not changed         not changed
    "5",        // \65  (0x35)  "5"                     not changed         not changed         not changed
    "6",        // \66  (0x36)  "6"                     not changed         not changed         not changed
    "7",        // \67  (0x37)  "7"                     not changed         not changed         not changed
    "8",        // \70  (0x38)  "8"                     not changed         not changed         not changed
    "9",        // \71  (0x39)  "9"                     not changed         not changed         not changed
    ":",        // \72  (0x3A)  ":"                     not changed         not changed         not changed
    ";",        // \73  (0x3B)  ";"                     not changed         not changed         not changed
    "<",        // \74  (0x3C)  "<"                     not changed         not changed         not changed
    "=",        // \75  (0x3D)  "="                     not changed         not changed         not changed
    ">",        // \76  (0x3E)  ">"                     not changed         not changed         not changed
    "?",        // \77  (0x3F)  "?"                     not changed         not changed         not changed

    "@",        // \100 (0x40)  "@"                     not changed         not changed         not changed
    "A",        // \101 (0x41)  "A"                     not changed         not changed         not changed
    "B",        // \102 (0x42)  "B"                     not changed         not changed         not changed
    "C",        // \103 (0x43)  "C"                     not changed         not changed         not changed
    "D",        // \104 (0x44)  "D"                     not changed         not changed         not changed
    "E",        // \105 (0x45)  "E"                     not changed         not changed         not changed
    "F",        // \106 (0x46)  "F"                     not changed         not changed         not changed
    "G",        // \107 (0x47)  "G"                     not changed         not changed         not changed
    "H",        // \110 (0x48)  "H"                     not changed         not changed         not changed
    "I",        // \111 (0x49)  "I"                     not changed         not changed         not changed
    "J",        // \112 (0x4A)  "J"                     not changed         not changed         not changed
    "K",        // \113 (0x4B)  "K"                     not changed         not changed         not changed
    "L",        // \114 (0x4C)  "L"                     not changed         not changed         not changed
    "M",        // \115 (0x4D)  "M"                     not changed         not changed         not changed
    "N",        // \116 (0x4E)  "N"                     not changed         not changed         not changed
    "O",        // \117 (0x4F)  "O"                     not changed         not changed         not changed

    "P",        // \120 (0x50)  "P"                     not changed         not changed         not changed
    "Q",        // \121 (0x51)  "Q"                     not changed         not changed         not changed
    "R",        // \122 (0x52)  "R"                     not changed         not changed         not changed
    "S",        // \123 (0x53)  "S"                     not changed         not changed         not changed
    "T",        // \124 (0x54)  "T"                     not changed         not changed         not changed
    "U",        // \125 (0x55)  "U"                     not changed         not changed         not changed
    "V",        // \126 (0x56)  "V"                     not changed         not changed         not changed
    "W",        // \127 (0x57)  "W"                     not changed         not changed         not changed
    "X",        // \130 (0x58)  "X"                     not changed         not changed         not changed
    "Y",        // \131 (0x59)  "Y"                     not changed         not changed         not changed
    "Z",        // \132 (0x5A)  "Z"                     not changed         not changed         not changed
    "[",        // \133 (0x5B)  "["                     not changed         not changed         not changed
    "\\",       // \134 (0x5C)  "\"                     not changed         not changed         not changed
    "]",        // \135 (0x5D)  "]"                     not changed         not changed         not changed
    "^",        // \136 (0x5E)  "^"                     not changed         not changed         not changed
    "_",        // \137 (0x5F)  "_"                     not changed         not changed         not changed

    "`",        // \140 (0x60)  "`"                     not changed         not changed         not changed
    "a",        // \141 (0x61)  "a"                     not changed         not changed         not changed
    "b",        // \142 (0x62)  "b"                     not changed         not changed         not changed
    "c",        // \143 (0x63)  "c"                     not changed         not changed         not changed
    "d",        // \144 (0x64)  "d"                     not changed         not changed         not changed
    "e",        // \145 (0x65)  "e"                     not changed         not changed         not changed
    "f",        // \146 (0x66)  "f"                     not changed         not changed         not changed
    "g",        // \147 (0x67)  "g"                     not changed         not changed         not changed
    "h",        // \150 (0x68)  "h"                     not changed         not changed         not changed
    "i",        // \151 (0x69)  "i"                     not changed         not changed         not changed
    "j",        // \152 (0x6A)  "j"                     not changed         not changed         not changed
    "k",        // \153 (0x6B)  "k"                     not changed         not changed         not changed
    "l",        // \154 (0x6C)  "l"                     not changed         not changed         not changed
    "m",        // \155 (0x6D)  "m"                     not changed         not changed         not changed
    "n",        // \156 (0x6E)  "n"                     not changed         not changed         not changed
    "o",        // \157 (0x6F)  "o"                     not changed         not changed         not changed

    "p",        // \160 (0x70)  "p"                     not changed         not changed         not changed
    "q",        // \161 (0x71)  "q"                     not changed         not changed         not changed
    "r",        // \162 (0x72)  "r"                     not changed         not changed         not changed
    "s",        // \163 (0x73)  "s"                     not changed         not changed         not changed
    "t",        // \164 (0x74)  "t"                     not changed         not changed         not changed
    "u",        // \165 (0x75)  "u"                     not changed         not changed         not changed
    "v",        // \166 (0x76)  "v"                     not changed         not changed         not changed
    "w",        // \167 (0x77)  "w"                     not changed         not changed         not changed
    "x",        // \170 (0x78)  "x"                     not changed         not changed         not changed
    "y",        // \171 (0x79)  "y"                     not changed         not changed         not changed
    "z",        // \172 (0x7A)  "z"                     not changed         not changed         not changed
    "{",        // \173 (0x7B)  "{"                     not changed         not changed         not changed
    "|",        // \174 (0x7C)  "|"                     not changed         not changed         not changed
    "}",        // \175 (0x7D)  "}"                     not changed         not changed         not changed
    "~",        // \176 (0x7E)  "~"                     not changed         not changed         not changed
    "\137",     // \177 (0x7F)  <del>                   "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "C",        // \200 (0x80)  "Ç"                     not changed         "Ç" \307 (0xC7)     "C"
    "u",        // \201 (0x81)  "ü"                     not changed         "ü" \374 (0xFC)     "u"
    "e",        // \202 (0x82)  "é"                     not changed         "é" \351 (0xE9)     "e"
    "a",        // \203 (0x83)  "â"                     not changed         "â" \342 (0xE2)     "a"
    "a",        // \204 (0x84)  "ä"                     not changed         "ä" \344 (0xE4)     "a"
    "a",        // \205 (0x85)  "à"                     not changed         "à" \340 (0xE0)     "a"
    "a",        // \206 (0x86)  "å"                     not changed         "å" \345 (0xE5)     "a"
    "c",        // \207 (0x87)  "ç"                     not changed         "ç" \347 (0xE7)     "c"
    "e",        // \210 (0x88)  "ê"                     not changed         "ê" \352 (0xEA)     "e"
    "e",        // \211 (0x89)  "ë"                     not changed         "ë" \353 (0xEB)     "e"
    "e",        // \212 (0x8A)  "è"                     not changed         "è" \350 (0xE8)     "e"
    "i",        // \213 (0x8B)  "ï"                     not changed         "ï" \357 (0xEF)     "i"
    "i",        // \214 (0x8C)  "î"                     not changed         "î" \356 (0xEE)     "i"
    "i",        // \215 (0x8D)  "ì"                     not changed         "ì" \354 (0xEC)     "i"
    "A",        // \216 (0x8E)  "Ä"                     not changed         "Ä" \304 (0xC4)     "A"
    "A",        // \217 (0x8F)  "Å"                     not changed         "Å" \305 (0xC5)     "A"

    "E",        // \220 (0x90)  "É"                     not changed         "É" \311 (0xC9)     "E"
    "ae",       // \221 (0x91)  "æ"                     not changed         "æ" \346 (0xE6)     "ae"
    "AE",       // \222 (0x92)  "Æ"                     not changed         "Æ" \306 (0xC6)     "AE"
    "o",        // \223 (0x93)  "ô"                     not changed         "ô" \364 (0xF4)     "o"
    "o",        // \224 (0x94)  "ö"                     not changed         "ö" \366 (0xF6)     "o"
    "o",        // \225 (0x95)  "ò"                     not changed         "ò" \362 (0xF2)     "o"
    "u",        // \226 (0x96)  "û"                     not changed         "û" \373 (0xFB)     "u"
    "u",        // \227 (0x97)  "ù"                     not changed         "ù" \371 (0xF9)     "u"
    "y",        // \230 (0x98)  "ÿ"                     not changed         "ÿ" \377 (0xFF)     "y"
    "O",        // \231 (0x99)  "Ö"                     not changed         "Ö" \326 (0xD6)     "O"
    "U",        // \232 (0x9A)  "Ü"                     not changed         "Ü" \334 (0xDC)     "U"
    "o",        // \233 (0x9B)  "ø"                     not changed         "ø" \370 (0xF8)     "o"
    "\137",     // \234 (0x9C)  Ka (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "O",        // \235 (0x9D)  "Ø"                     not changed         "Ø" \330 (0xD8)     "O"
    "\137",     // \236 (0x9E)  Be (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \237 (0x9F)  Be (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "a",        // \240 (0xA0)  "á"                     not changed         "á" \341 (0xE1)     "a"
    "i",        // \241 (0xA1)  "í"                     not changed         "í" \355 (0xED)     "i"
    "o",        // \242 (0xA2)  "ó"                     not changed         "ó" \363 (0xF3)     "o"
    "u",        // \243 (0xA3)  "ú"                     not changed         "ú" \372 (0xFA)     "u"
    "n",        // \244 (0xA4)  "ñ"                     not changed         "ñ" \361 (0xF1)     "n"
    "N",        // \245 (0xA5)  "Ñ"                     not changed         "Ñ" \321 (0xD1)     "N"
    "\137",     // \246 (0xA6)   Ge (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \247 (0xA7)   Ge (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \250 (0xA8)   De (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \251 (0xA9)   De (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \252 (0xAA)   Zhe (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \253 (0xAB)   Zhe (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \254 (0xAC)   Ze (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \255 (0xAD)   Ze (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \256 (0xAE)   I (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \257 (0xAF)   I (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \260 (0xB0)   Short I (Cyrillic) uc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \261 (0xB1)   Short I (Cyrillic) lc  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \262 (0xB2)   El (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \263 (0xB3)   El (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \264 (0xB4)   Em (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "A",        // \265 (0xB5)   "Á"                    not changed         "Á" \301 (0xC1)     "A"
    "A",        // \266 (0xB6)   "Â"                    not changed         "Â" \302 (0xC2)     "A"
    "A",        // \267 (0xB7)   "À"                    not changed         "À" \300 (0xC0)     "A"
    "\137",     // \270 (0xB8)   Pe (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \271 (0xB9)   Pe (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \272 (0xBA)   U (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \273 (0xBB)   U (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \274 (0xBC)   Ef (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \275 (0xBD)   Ef (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \276 (0xBE)   Tse (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \277 (0xBF)   Tse (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \300 (0xC0)   Che (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \301 (0xC1)   Che (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \302 (0xC2)   Sha (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \303 (0xC3)   Sha (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \304 (0xC4)   Shcha (Cyrillic) uc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \305 (0xC5)   Shcha (Cyrillic) lc    "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "a",        // \306 (0xC6)   "ã"                    not changed         "ã" \343 (0xE3)     "a"
    "A",        // \307 (0xC7)   "Ã"                    not changed         "Ã" \303 (0xC3)     "A"
    "\137",     // \310 (0xC8)   Yer (Cyrillic) uc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \311 (0xC9)   Yer (Cyrillic) lc      "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \312 (0xCA)   Yery (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \313 (0xCB)   Yery (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \314 (0xCC)   Soft (Cyrillic) uc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \315 (0xCD)   Soft (Cyrillic) lc     "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \316 (0xCE)   E (Cyrillic) uc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \317 (0xCF)   E (Cyrillic) lc        "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "\137",     // \320 (0xD0)   Yu (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \321 (0xD1)   Yu (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "E",        // \322 (0xD2)   "Ê"                    not changed         "Ê" \312 (0xCA)     "E"
    "E",        // \323 (0xD3)   "Ë"                    not changed         "Ë" \313 (0xCB)     "E"
    "E",        // \324 (0xD4)   "È"                    not changed         "È" \310 (0xC8)     "E"
    "\137",     // \325 (0xD5)   En (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "I",        // \326 (0xD6)   "Í"                    not changed         "Í" \315 (0xCD)     "I"
    "I",        // \327 (0xD7)   "Î"                    not changed         "Î" \316 (0xCE)     "I"
    "I",        // \330 (0xD8)   "Ï"                    not changed         "Ï" \317 (0xCF)     "I"
    "\137",     // \331 (0xD9)   Ya (Cyrillic) uc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137",     // \332 (0xDA)   Ya (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "A",        // \333 (0xDB)   "A" + Ogonek           "A"                 "A"                 "A"
    "a",        // \334 (0xDC)   "a" + Ogonek           "a"                 "a"                 "a"
    "\137",     // \335 (0xDD)   Ve (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "I",        // \336 (0xD2)   "Ì"                    not changed         "Ì" \314 (0xCC)     "I"
    "\137",     // \337 (0xDF)   Te (Cyrillic) lc       "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"

    "o",        // \340 (0xE0)   "ó"                    not changed         "ó" \363 (0xF3)     "o"
    "ss",       // \341 (0xE1)   "ß"                    not changed         "ß" \337 (0xDF)     "ss"
    "O",        // \342 (0xE2)   "Ô"                    not changed         "Ô" \324 (0xD4)     "O"
    "O",        // \343 (0xE3)   "Ò"                    not changed         "Ò" \322 (0xD2)     "O"
    "o",        // \344 (0xE4)   "õ"                    not changed         "õ" \365 (0xF5)     "o"
    "O",        // \345 (0xE5)   "Õ"                    not changed         "Õ" \325 (0xD5)     "O"
    "u",        // \346 (0xE6)   "µ"                    not changed         "µ" \265 (0xB5)     "u"
    "C",        // \347 (0xE7)   "C" + acute            "C"                 "C"                 "C"
    "c",        // \350 (0xE8)   "c" + acute            "c"                 "c"                 "c"
    "U",        // \351 (0xE9)   "Ú"                    not changed         "Ú" \332 (0xDA)     "U"
    "U",        // \352 (0xEA)   "Û"                    not changed         "Û" \333 (0xDB)     "U"
    "U",        // \353 (0xEB)   "Ù"                    not changed         "Ù" \331 (0xD9)     "U"
    "y",        // \354 (0xEC)   "ý"                    not changed         "ý" \375 (0xFD)     "y"
    "Y",        // \355 (0xED)   "Ý"                    not changed         "Ý" \335 (0xDD)     "Y"
    "E",        // \356 (0xEE)   "E" + Ogonek           "E"                 "E"                 "E"
    "e",        // \357 (0xEF)   "e" + Ogonek           "e"                 "e"                 "e"

    "L",        // \360 (0xF0)   "L" + stroke           "L"                 "L"                 "L"
    "l",        // \361 (0xF1)   "l" + stroke           "l"                 "l"                 "l"
    "N",        // \362 (0xF2)   "N" + acute            "N"                 "N"                 "N"
    "n",        // \363 (0xF3)   "n" + acute            "n"                 "n"                 "n"
    "S",        // \364 (0xF4)   "S" + acute            "S"                 "S"                 "S"
    "s",        // \365 (0xF5)   "s" + acute            "s"                 "s"                 "s"
    "Z",        // \366 (0xF6)   "Z" + acute            "Z"                 "Z"                 "Z"
    "z",        // \367 (0xF7)   "z" + acute            "z"                 "z"                 "z"
    "deg",      // \370 (0xF8)   "°"                    not changed         "º" \272 (0xBA)     "deg"
    "Z",        // \371 (0xF9)   "Z" + single dot above "Z"                 "Z"                 "Z"
    "z",        // \372 (0xFA)   "z" + single dot above "z"                 "z"                 "z"
    "Y",        // \373 (0xFB)   "Ÿ"                    "Y"                 "Ÿ" \237 (0x9F)     "Y"
    "3",        // \374 (0xFC)   "³"                    not changed         "³" \263 (0xB3)     "3"
    "2",        // \375 (0xFD)   "²"                    not changed         "²" \262 (0xB2)     "2"
    "\137",     // \376 (0xFE)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
    "\137"      // \377 (0xFF)   SPARE                  "NOT RECOGNISED"    "NOT RECOGNISED"    "NOT RECOGNISED"
};

#pragma diag_default=Pm011

// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void ParameterDump_OutputLine(void)
{
    TUSIGN8 bytesToSend = (TUSIGN8)strlen((const char *)parameterDumpOutputLine);
    TUSIGN8 olidx;  // parameterDumpOutputLine index
    TUSIGN8 clidx;  // ConvertedLine index

    TUSIGN8 ConvertedLine[LINELENGTH];

    memset((void*)ConvertedLine, 0, LINELENGTH);

    clidx = 0;
    for (olidx = 0; olidx < bytesToSend; olidx ++)
    {
        if ((parameterDumpOutputLine[olidx] == newline[0]) && (parameterDumpOutputLine[olidx+1] == newline[1]))
        {
            ConvertedLine[clidx] = parameterDumpOutputLine[olidx];
            clidx = (clidx < LINELENGTH - 2) ? clidx + 1 : LINELENGTH - 2;

            olidx = olidx + 1;
            ConvertedLine[clidx] = parameterDumpOutputLine[olidx];
            clidx = (clidx < LINELENGTH - 2) ? clidx + 1 : LINELENGTH - 2;
        }
        else
        {
            TUSIGN8 charToConvert = parameterDumpOutputLine[olidx];

            const TUSIGN8  *pInsertString = (const TUSIGN8 *)nonStdConversionString_Terminal[charToConvert];

            switch (FrameTableGetParameterDumpFont())
            {
            case 2:     pInsertString = (const TUSIGN8 *)nonStdConversionString_Standard[charToConvert];    break;
            case 1:     pInsertString = (const TUSIGN8 *)nonStdConversionString_System[charToConvert];      break;
            default:    pInsertString = (const TUSIGN8 *)nonStdConversionString_Terminal[charToConvert];    break;
            }

            TUSIGN8 nToInsert = (TUSIGN8)strlen((const char *)pInsertString);
            TUSIGN8 i;

            for (i = 0; i < nToInsert; i ++)
            {
                ConvertedLine[clidx] = pInsertString[i];
                clidx = (clidx < LINELENGTH - 2) ? clidx + 1 : LINELENGTH - 2;
            }
        }
    }

    ConvertedLine[LINELENGTH - 3] = newline[0];
    ConvertedLine[LINELENGTH - 2] = newline[1];
    ConvertedLine[LINELENGTH - 1] = newline[2];;

    (void)strcpy((char *)parameterDumpOutputLine,(const char *)ConvertedLine);

    bytesToSend = (TUSIGN8)strlen((const char *)parameterDumpOutputLine);

    if (bytesToSend > 0)
    {
        T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;
        TUSIGN16 acceptedBytes = 0;

        while (acceptedBytes < bytesToSend)
        {
            acceptedBytes += pServicePort->SendDataEXE(&parameterDumpOutputLine[acceptedBytes],
                                                       (TUSIGN16)(bytesToSend-acceptedBytes),
                                                       SP_ClientInfoParameterDump.id);
        }
        readyForNextLine = eFALSE;
    }
}

// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void SP_CallbackParameterDumpTransmissionComplete(TUSIGN8 *pMsg)
{
    // Service port has completed transmission of the message pointed to by pMsg
    VIP_ASSERT(pMsg == parameterDumpOutputLine);
    readyForNextLine = eTRUE;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void SP_CallbackParameterDumpDataReceived(void)
{
    // Service port has received HMI data

    T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;

    // Read Received bytes
    TUSIGN8 SP_ReceiveBuffer[10];
    TUSIGN8 SP_nReceivedBytes = pServicePort->ReadDataEXE(SP_ReceiveBuffer, 10) ;

    // Cannot start a new dump if one is already active
    if ((ConfigDumpActive == eFALSE)
     && (StatusDumpActive == eFALSE)
     && (SignalsDumpActive == eFALSE)
     && (HistoryDumpActive == eFALSE)
     && (InvalidCommandActive == eFALSE))
    {
        if (SP_nReceivedBytes == 1)
        {
            switch(SP_ReceiveBuffer[0])
            {
            case 'c':
            case 'C':
                ConfigDumpStart();
                break;

            case 'i':
            case 'I':
                InfoDumpStart();
                break;

            default:
                InvalidCommandDumpStart();
                break;
            }
        }
    }
}

// No restriction on the optimization of this function
void SP_CallbackParameterDumpCommsError(TSpCommsError err)
{
    if (err != 0)
    {
        ConfigDumpActive = eFALSE;
        StatusDumpActive = eFALSE;
        SignalsDumpActive = eFALSE;
        HistoryDumpActive = eFALSE;
        InvalidCommandActive = eFALSE;
    }
}

// No restriction on the optimization of this function
void SP_CallbackParameterDumpConnected(void)
{
        ConfigDumpActive = eFALSE;
        StatusDumpActive = eFALSE;
        SignalsDumpActive = eFALSE;
        HistoryDumpActive = eFALSE;
        InvalidCommandActive = eFALSE;
}

// No restriction on the optimization of this function
void SP_CallbackParameterDumpDisconnected(void)
{
}

#endif // end of #ifdef HMI_SUPPORTS_PARAMETER_DUMP

void ParameterDumpExecute(void)
{
    #ifdef HMI_SUPPORTS_PARAMETER_DUMP


    if (!RegisteredWithServicePort)
    {
        // Check that service port is initialized before attempting to register
        TUSIGN16 state;
        TUSIGN16 result = servicePort.unit.GetState(SERVICE_PORT_ME,&state);

        if ((result == (TUSIGN16)OK) && (state != (TUSIGN16)NOT_INITIALIZED))
        {
            RegisteredWithServicePort = ParameterDump_SP_RegisterClientEXE();
        }
    }

    if (RegisteredWithServicePort)
    {
        ConfigDumpExecute();
        InfoDumpExecute();
        InvalidCommandDumpExecute();
    }
    #endif
}



