//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description                    Adjuster of the HMI subsystem. Is responsible for changing the value of the displayed variable

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003
#pragma diag_suppress=Pm077

/****** Adjuster.c ******/
#include <intrinsics.h>
#include <string.h>
#include "IncludeList_Implementation.h"

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

#include "keystateDefinitions.h"
#include "FrameTableDefinitions.h"
#include "FrameTable.h"
#include <math.h>
#include "writeProtect.h"
#include "Adjuster.h"

#ifdef AZ200_SPECIAL_FEATURES_HMI
    #include "../../O2_Calibration/Interface/o2_calibration_idx.h"
    #include "../../O2_Calibration/Source/O2_Calibration_PerformanceLog.h"
    #include "../../nvinterface/Interface/NVinterface.h"
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
#include "wavefornFrameDefinitions.h"
#include "../../waveform/interface/waveform.h"
#endif

#ifdef EDP300_SPECIAL_FEATURES_HMI
#include "EDP300_Mode.h"
#endif

#if defined(HMI_USES_COMMONDIAGNOSTICS) && !defined(LOCAL_DUMMY_DIAGNOSTICS)

    #if !defined(HMIGEN_RELEASE_VERSION) || !defined(HMIGEN_MAJOR_VERSION) || !defined(HMIGEN_MINOR_VERSION) || !defined(HMIGEN_CONFIG_VERSION) || (HMIGEN_MAJOR_VERSION < 12) || ((HMIGEN_MAJOR_VERSION == 12) && (HMIGEN_MINOR_VERSION < 2))
        #include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"

        #ifdef AZ200_SPECIAL_FEATURES_HMI
            #include "../../t_data_obj/interface/t_data_obj_alarm.h"
        #else
            #include "Diagnosis/interface/t_data_obj_alarm.h"
        #endif

        #pragma diag_suppress=Pm084
        #include "Diagnosis/interface/t_data_obj_alarm_counter.h"
        #pragma diag_default=Pm084

        #pragma diag_suppress=Pm025
        #pragma diag_suppress=Pm084
        #include "Diagnosis/interface/diagnosis.h"
        #pragma diag_default=Pm084
        #pragma diag_default=Pm025

        #include "Diagnosis/interface/Diagnosis_idx.h"
        #include "Diagnosis/Source/diagnosis_types.h"
        #include "Diagnosis/Source/diagnosis_table_default.h"

    #else

        #include "IncludeList_Diagnosis.h"

    #endif

#endif

#if defined(HMI_SUPPORTS_SECURITY_ACCESS) && !defined(LOCAL_DUMMY_ARM)
    #include "ARM/Interface/arm.h"
    #include "ARM/Interface/arm_idx.h"

    #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #if !defined(ARM_VERSION) || ARM_VERSION < 0x010013
            #error HMI requires Access Rights Manager Version 1.0.13 or newer
        #endif
    #endif
#endif

#pragma diag_default=Pm077

// private methods
void BuildFloatFromString(TFLOAT* pFloatVal);
#ifdef HMI_USES_DOUBLE
void BuildDoubleFromString(TDOUBLE* pDoubleVal);
#endif

#ifdef HMI_SUPPORTS_INTEGER_FRAMES
void BuildUSIGN32FromString(TUSIGN32 *pUSIGN32Val);
#endif


#if defined(HMI_USES_COMMONDIAGNOSTICS) && !defined(LOCAL_DUMMY_DIAGNOSTICS)
TUSIGN8 ConvertClassification(T_DIAGNOSIS_CLASSES classification);
TUSIGN8 ConvertCategory(T_DIAGNOSIS_GROUPS group);
#endif

// private variables
#ifdef HMI_SUPPORTS_SERVICEPORT
typedef struct
{
    // elements that must be initialised
    TUSIGN8                 dataAdjusted;
    FRAME_FORMAT_TYPE const *pCurrAdjustFrame;
    TUSIGN8                 variableReady;
    TUSIGN8                 LimitError;
    TUSIGN16                errorCode;
    TBOOL                   AdjustNumberIsDouble;

    // elements that need not be initialised
    ATTRIB_IDENTIFIER_TYPE  variable;
    NUMBER_ATTRIBUTES_TYPE  Attributes;
    TUSIGN8                 dataType;
    ADJUST_DATA             adjValue;
    ADJUST_DATA             hiLimit;
    ADJUST_DATA             loLimit;
    ADJUST_DATA             incValue;
    ADJUST_DATA             unadjusted_adjvalue;
    const TUSIGN8 SLOW*     pEnum8Values;
    TUSIGN8                 nEnum8Values;
    #ifdef HMI_SUPPORTS_16BIT_ENUMS
    const TUSIGN16 SLOW*    pEnum16Values;
    TUSIGN16                nEnum16Values;
    #endif
    TUSIGN8                 stringVal[MAX_STRING_PARAMETER_LENGTH + 1];
    TUSIGN8                 stringNeg;
    TUSIGN8                 m_number[25];
    TUSIGN8                 DummyARMLevel;
    TUSIGN32                TimeAndDate;
}ADJUSTER_LOCAL_TYPE;

#pragma diag_suppress=Pm022
#pragma diag_suppress=Pm023
ADJUSTER_LOCAL_TYPE AdjusterLocal_1 = {
     0      // TUSIGN8                  dataAdjusted
    ,NULL   // FRAME_FORMAT_TYPE const *pCurrAdjustFrame
    ,0      // TUSIGN8                 variableReady = 0;
    ,0      // TUSIGN8                 LimitError = 0;
    ,OK     // TUSIGN16                errorCode = OK;
    ,eFALSE // TBOOL                   AdjustNumberIsDouble = eFALSE
    };

ADJUSTER_LOCAL_TYPE AdjusterLocal_2 = {
     0      // TUSIGN8                  dataAdjusted
    ,NULL   // FRAME_FORMAT_TYPE const *pCurrAdjustFrame
    ,0      // TUSIGN8                 variableReady = 0;
    ,0      // TUSIGN8                 LimitError = 0;
    ,OK     // TUSIGN16                errorCode = OK;
    ,eFALSE // TBOOL                   AdjustNumberIsDouble = eFALSE
    };
#pragma diag_default=Pm023
#pragma diag_default=Pm022


ADJUSTER_LOCAL_TYPE *pAdjusterLocal = &AdjusterLocal_1;

#define LOCAL_dataAdjusted      pAdjusterLocal->dataAdjusted
#define LOCAL_pCurrAdjustFrame  pAdjusterLocal->pCurrAdjustFrame
#define LOCAL_variableReady     pAdjusterLocal->variableReady
#define LOCAL_variable          pAdjusterLocal->variable
#define LOCAL_Attributes        pAdjusterLocal->Attributes
#define LOCAL_LimitError        pAdjusterLocal->LimitError
#define LOCAL_errorCode         pAdjusterLocal->errorCode
#define LOCAL_dataType          pAdjusterLocal->dataType
#define LOCAL_adjValue          pAdjusterLocal->adjValue
#define LOCAL_hiLimit           pAdjusterLocal->hiLimit
#define LOCAL_loLimit           pAdjusterLocal->loLimit
#define LOCAL_incValue          pAdjusterLocal->incValue
#define LOCAL_unadj_adjvalue    pAdjusterLocal->unadjusted_adjvalue
#define LOCAL_pEnum8Values      pAdjusterLocal->pEnum8Values
#define LOCAL_nEnum8Values      pAdjusterLocal->nEnum8Values
#ifdef HMI_SUPPORTS_16BIT_ENUMS
#define LOCAL_pEnum16Values     pAdjusterLocal->pEnum16Values
#define LOCAL_nEnum16Values     pAdjusterLocal->nEnum16Values
#endif
#define LOCAL_stringVal         pAdjusterLocal->stringVal
#define LOCAL_stringNeg         pAdjusterLocal->stringNeg
#define LOCAL_m_number          pAdjusterLocal->m_number
#define LOCAL_AdjIsDouble       pAdjusterLocal->AdjustNumberIsDouble
#define LOCAL_DummyARMLevel     pAdjusterLocal->DummyARMLevel
#define LOCAL_TimeAndDate       pAdjusterLocal->TimeAndDate
#else

#define LOCAL_dataAdjusted      dataAdjusted
#define LOCAL_pCurrAdjustFrame  pCurrAdjustFrame
#define LOCAL_variableReady     variableReady
#define LOCAL_variable          variable
#define LOCAL_Attributes        Attributes
#define LOCAL_LimitError        LimitError
#define LOCAL_errorCode         errorCode
#define LOCAL_dataType          dataType
#define LOCAL_adjValue          adjValue
#define LOCAL_hiLimit           hiLimit
#define LOCAL_loLimit           loLimit
#define LOCAL_incValue          incValue
#define LOCAL_unadj_adjvalue    unadjusted_adjvalue
#define LOCAL_pEnum8Values      pEnum8Values
#define LOCAL_nEnum8Values      nEnum8Values
#ifdef HMI_SUPPORTS_16BIT_ENUMS
#define LOCAL_pEnum16Values     pEnum16Values
#define LOCAL_nEnum16Values     nEnum16Values
#endif
#define LOCAL_stringVal         stringVal
#define LOCAL_stringNeg         stringNeg
#define LOCAL_m_number          m_number
#define LOCAL_AdjIsDouble       AdjustNumberIsDouble
#define LOCAL_DummyARMLevel     DummyARMLevel
#define LOCAL_TimeAndDate       TimeAndDate

TUSIGN8                 dataAdjusted = 0;
FRAME_FORMAT_TYPE const *pCurrAdjustFrame = NULL;
TUSIGN8                 variableReady = 0;
ATTRIB_IDENTIFIER_TYPE  variable;
NUMBER_ATTRIBUTES_TYPE  Attributes;
TUSIGN8                 LimitError = 0;
TUSIGN16                errorCode = OK;
TUSIGN8                 dataType;
ADJUST_DATA             adjValue;
ADJUST_DATA             hiLimit;
ADJUST_DATA             loLimit;
ADJUST_DATA             incValue;
ADJUST_DATA             unadjusted_adjvalue;
const TUSIGN8 SLOW*     pEnum8Values;
TUSIGN8                 nEnum8Values;
#ifdef HMI_SUPPORTS_16BIT_ENUMS
const TUSIGN16 SLOW*    pEnum16Values;
TUSIGN16                nEnum16Values;
#endif
TUSIGN8 FAST            stringVal[MAX_STRING_PARAMETER_LENGTH + 1];
TUSIGN8                 stringNeg;
TUSIGN8                 m_number[25];
TBOOL                   AdjustNumberIsDouble = eFALSE;
TUSIGN8                 DummyARMLevel;
TUSIGN32                TimeAndDate;
#endif

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES

#define SECONDS_IN_4_YEARS      126230400
#define SECONDS_IN_LEAP_YEAR    31622400
#define SECONDS_IN_YEAR         31536000
#define SECONDS_IN_28_DAYS      2419200
#define SECONDS_IN_29_DAYS      2505600
#define SECONDS_IN_30_DAYS      2592000
#define SECONDS_IN_31_DAYS      2678400

#define SECONDS_IN_JANUARY      SECONDS_IN_31_DAYS
//#define SECONDS_IN_FEBRUARY
#define SECONDS_IN_MARCH        SECONDS_IN_31_DAYS
#define SECONDS_IN_APRIL        SECONDS_IN_30_DAYS
#define SECONDS_IN_MAY          SECONDS_IN_31_DAYS
#define SECONDS_IN_JUNE         SECONDS_IN_30_DAYS
#define SECONDS_IN_JULY         SECONDS_IN_31_DAYS
#define SECONDS_IN_AUGUST       SECONDS_IN_31_DAYS
#define SECONDS_IN_SEPTEMBER    SECONDS_IN_30_DAYS
#define SECONDS_IN_OCTOBER      SECONDS_IN_31_DAYS
#define SECONDS_IN_NOVEMBER     SECONDS_IN_30_DAYS
#define SECONDS_IN_DECEMBER     SECONDS_IN_31_DAYS

#define SECONDS_IN_1_DAY        86400
#define SECONDS_IN_1_HOUR       3600
#define SECONDS_IN_1_MINUTE     60

#define SECONDS_IN_200_HOURS    720000

#endif

const TFLOAT limitArray[][11] =
{
    {
        /*       len = 0 */
        /*resolution = 0 */     0.0f,
        /*             1 */     0.0f,
        /*             2 */     0.0f,
        /*             3 */     0.0f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 1 */
        /*resolution = 0 */     9.0f,
        /*             1 */     0.0f,
        /*             2 */     0.0f,
        /*             3 */     0.0f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 2 */
        /*resolution = 0 */     99.0f,
        /*             1 */     0.0f,
        /*             2 */     0.0f,
        /*             3 */     0.0f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 3 */
        /*resolution = 0 */     999.0f,
        /*             1 */     9.9f,
        /*             2 */     0.0f,
        /*             3 */     0.0f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 4 */
        /*resolution = 0 */     9999.0f,
        /*             1 */     99.9f,
        /*             2 */     9.99f,
        /*             3 */     0.0f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 5 */
        /*resolution = 0 */     99999.0f,
        /*             1 */     999.9f,
        /*             2 */     99.99f,
        /*             3 */     9.999f,
        /*             4 */     0.0f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 6 */
        /*resolution = 0 */     999999.0f,
        /*             1 */     9999.9f,
        /*             2 */     999.99f,
        /*             3 */     99.999f,
        /*             4 */     9.9999f,
        /*             5 */     0.0f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 7 */
        /*resolution = 0 */     9999999.0f,
        /*             1 */     99999.9f,
        /*             2 */     9999.99f,
        /*             3 */     999.999f,
        /*             4 */     99.9999f,
        /*             5 */     9.99999f,
        /*             6 */     0.0f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 8 */
        /*resolution = 0 */     99999999.0f,
        /*             1 */     999999.9f,
        /*             2 */     99999.99f,
        /*             3 */     9999.999f,
        /*             4 */     999.9999f,
        /*             5 */     99.99999f,
        /*             6 */     9.999999f,
        /*             7 */     0.0f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*       len = 9 */
        /*resolution = 0 */     999999999.0f,
        /*             1 */     9999999.9f,
        /*             2 */     999999.99f,
        /*             3 */     99999.999f,
        /*             4 */     9999.9999f,
        /*             5 */     999.99999f,
        /*             6 */     99.999999f,
        /*             7 */     9.9999999f,
        /*             8 */     0.0f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*      len = 10 */
        /*resolution = 0 */     9999999999.0f,
        /*             1 */     99999999.9f,
        /*             2 */     9999999.99f,
        /*             3 */     999999.999f,
        /*             4 */     99999.9999f,
        /*             5 */     9999.99999f,
        /*             6 */     999.999999f,
        /*             7 */     99.9999999f,
        /*             8 */     9.99999999f,
        /*             9 */     0.0f,
        /*            10 */     0.0f
    },
    {
        /*      len = 11 */
        /*resolution = 0 */     99999999999.0f,
        /*             1 */     999999999.9f,
        /*             2 */     99999999.99f,
        /*             3 */     9999999.999f,
        /*             4 */     999999.9999f,
        /*             5 */     99999.99999f,
        /*             6 */     9999.999999f,
        /*             7 */     999.9999999f,
        /*             8 */     99.99999999f,
        /*             9 */     9.999999999f,
        /*            10 */     0.0f
    },
    {
        /*      len = 12 */
        /*resolution = 0 */     999999999999.0f,
        /*             1 */     9999999999.9f,
        /*             2 */     999999999.99f,
        /*             3 */     99999999.999f,
        /*             4 */     9999999.9999f,
        /*             5 */     999999.99999f,
        /*             6 */     99999.999999f,
        /*             7 */     9999.9999999f,
        /*             8 */     999.99999999f,
        /*             9 */     99.999999999f,
        /*            10 */     9.9999999999f
    }
};

#ifdef HMI_SUPPORTS_SERVICEPORT
// No restriction on the optimization of this function
void AdjusterSetLocalDataPointer(TUSIGN8 newInstance)
{
    if      (newInstance == 1)  {pAdjusterLocal = &AdjusterLocal_1;}
    else if (newInstance == 2)  {pAdjusterLocal = &AdjusterLocal_2;}
    else                        {VIP_ASSERT(0);}
}
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Saves old (adjusted) variable and prepares the Adjuster for a new variable
 \author Roger Arnold
 \date   2005-04-14
 \param  - saveOldVariable - Defines whether (or not) to save the currently accessed variable to its sub-system
 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN16 AdjusterPrepareNewVariable(TUSIGN8 saveOldVariable)
{
    TUSIGN16 saveSuccess = OK;

    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
    TUSIGN32 iv;
    #endif

    #if defined(HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES) || defined(HMI_SUPPORTS_MANUAL_CALIBRATION)

    TFLOAT fv;
        #ifdef HMI_USES_DOUBLE
        TDOUBLE dv;
        #endif
    #endif

    if ((LOCAL_variableReady) && (LOCAL_dataAdjusted) && (LOCAL_pCurrAdjustFrame))
    {
        if (!saveOldVariable)
        {
            #ifdef HMI_SUPPORTS_MANUAL_ADJUST
            if (LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_MANUAL_ADJUST)
            {
                saveSuccess = AdjusterWriteValueAndCheckLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                                               (TUSIGN16)LOCAL_variable.objIdx,
                                                               LOCAL_variable.attribIdx,
                                                               &LOCAL_unadj_adjvalue,
                                                               LOCAL_dataType);
            }
            #endif
        }
        else
        {
            switch (LOCAL_pCurrAdjustFrame->frameType)
            {
            case FRAME_TYPE_MANUAL_ADJUST:
                #ifdef HMI_SUPPORTS_MANUAL_ADJUST
                saveSuccess = AdjusterWriteValueAndCheckLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                                               (TUSIGN16)LOCAL_variable.objIdx,
                                                               LOCAL_variable.attribIdx,
                                                               &LOCAL_adjValue,
                                                               LOCAL_dataType);
                #endif
                break;

            case FRAME_TYPE_TIME_AND_DATE_PARAM:   
                #if defined(HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES)
                {
                    TUSIGN32 value32 = LOCAL_TimeAndDate;
                    //saveSuccess = 1;
                    saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                                         (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.objIdx,
                                                         (TINT16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                                         (void FAST *)&value32);
                }
                #endif
                break;
            case FRAME_TYPE_IP_ADDRESS:
                #if defined(HMI_SUPPORTS_IP_ADDRESS_FRAMES)
                {
                    TUSIGN32 value32;
                    #if defined(IP_ADDRESS_DATA_REVERSED) && (IP_ADDRESS_DATA_REVERSED == 1)
                        IP_ADDRESS_HMI *pIPValue32 = (IP_ADDRESS_HMI*)&LOCAL_TimeAndDate;
                        IP_ADDRESS_HMI *pReverseIPValue32 = (IP_ADDRESS_HMI*)&value32;
                        pReverseIPValue32->dotted_IP[0] = pIPValue32->dotted_IP[3];
                        pReverseIPValue32->dotted_IP[1] = pIPValue32->dotted_IP[2];
                        pReverseIPValue32->dotted_IP[2] = pIPValue32->dotted_IP[1];
                        pReverseIPValue32->dotted_IP[3] = pIPValue32->dotted_IP[0];
                    #else
                        value32 = LOCAL_TimeAndDate;
                    #endif
                    //saveSuccess = 1;
                    saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                                         (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.objIdx,
                                                         (TINT16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                                         (void FAST *)&value32);
                }
                #endif
                break;

            #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
            case FRAME_TYPE_MANUAL_CALIBRATION:
            #endif
            #ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
            case FRAME_TYPE_2_POINT_pH_CALIBRATION:
            #endif
            case FRAME_TYPE_NUMERICAL_PARAM:
                #if defined(HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES) || defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)

                #ifdef HMI_USES_DOUBLE
                if (LOCAL_AdjIsDouble)
                {
                    BuildDoubleFromString(&dv);
                    LOCAL_adjValue.data_D = dv;
                }
                else
                #endif
                {
                    BuildFloatFromString(&fv);
                    LOCAL_adjValue.data_F = fv;
                }

                saveSuccess = AdjusterWriteValueAndCheckLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                                               (TUSIGN16)LOCAL_variable.objIdx,
                                                               LOCAL_variable.attribIdx,
                                                               &LOCAL_adjValue,
                                                               LOCAL_dataType);
                #endif
                break;

            #ifdef HMI_SUPPORTS_INTEGER_FRAMES
            case FRAME_TYPE_INTEGER_PARAM:
                BuildUSIGN32FromString(&iv);
                LOCAL_adjValue.data_32 = iv;
                saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_variable.ssIdx, (TUSIGN16)LOCAL_variable.objIdx, (TINT16)LOCAL_variable.attribIdx, (void FAST *)&iv);
                break;
            #endif


            case FRAME_TYPE_STRING_PARAM_PACKED:
                {
                #ifdef HMI_SUPPORTS_PACKEDASCII
                TUSIGN8 j;
                TUSIGN8 packedtext[41];

                for (j = (TUSIGN8)strlen((char *)LOCAL_stringVal); j < LOCAL_Attributes.maxCharacters; )
                {
                    LOCAL_stringVal[j] = 0x20;
                    j++;
                    LOCAL_stringVal[j] = 0x00;
                }

                AdjusterPackASCII(LOCAL_stringVal, LOCAL_Attributes.maxCharacters, packedtext);

                saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                                     (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                                     -1,
                                                     (void FAST*)&packedtext[0]);

                saveSuccess = OK;
                #else
                saveSuccess = 1;
                #endif
                }
                break;

            case FRAME_TYPE_STRING_PARAM:
                {
                #ifdef HMI_SUPPORTS_STANDARDASCII

                #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                 && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevel))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_STANDARD_USER,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_StandardLevel[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                      && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevel))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_ADVANCED_USER,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_AdvancedLevel[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                     && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevel))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_SERVICE_USER,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_ServiceLevel[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                      && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevelDefault))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_STANDARD_DEFAULT,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_StandardLevelDefault[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                      && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevelDefault))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_ADVANCED_DEFAULT,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_AdvancedLevelDefault[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                      && (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevelDefault))
                {
                    TUSIGN8 i;
                    #ifdef LOCAL_DUMMY_ARM
                    saveSuccess = OK;
                    #else
                    saveSuccess = ((const T_ARM*)GetSubsystemPtr(ARM_IDX))->ChangePasswordEXE((T_SUBSYSTEM_IDX)HMI_IDX,
                                                                                              (TUSIGN8)ARM_SERVICE_DEFAULT,
                                                                                              (TCHAR*)LOCAL_stringVal);
                    #endif

                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.SetPassword_ServiceLevelDefault[i] = 0x00;
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else
                #endif

                {

                    TUSIGN8 i;
                    TUSIGN8 string[41];

                    for (i = 0; i < (TUSIGN8)strlen((char *)LOCAL_stringVal); i++ )
                    {
                        string[i] = LOCAL_stringVal[i];
                    }

                    for (; i < 41; i++ )
                    {
                        string[i] = 0x00;
                    }

//                  (void)strcpy((char*)string,(char*)(LOCAL_stringVal));

                    saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                                         (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                                         (TINT16) WHOLE_OBJECT,
                                                         (void FAST *)string);
                }
                #else
                saveSuccess = 1;
                #endif
                }
                break;

            case FRAME_TYPE_ENUMERATED_PARAM:
            case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                #ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
                    #ifdef HMI_SUPPORTS_16BIT_ENUMS

                    #ifdef SIZE_ST_E16
                    if ((LOCAL_dataType == ST_U16) || (LOCAL_dataType == ST_E16))
                    #else
                    if (LOCAL_dataType == ST_U16)
                    #endif
                    {
                        TUSIGN16 data_16 = LOCAL_adjValue.data_16;
                        saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_variable.ssIdx,
                                                             (TUSIGN16)LOCAL_variable.objIdx,
                                                             (TINT16)LOCAL_variable.attribIdx,
                                                             (void FAST *)&data_16);
                    }
                    else
                    #endif
                    {
                        TUSIGN16 data_8 = LOCAL_adjValue.data_8;
                        saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_variable.ssIdx,
                                                             (TUSIGN16)LOCAL_variable.objIdx,
                                                             (TINT16)LOCAL_variable.attribIdx,
                                                             (void FAST *)&data_8);
                    }
                #endif
                break;

            case FRAME_TYPE_BIT_ENUMERATED_PARAM:
                #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
                {
                    TUSIGN8 value8 = LOCAL_adjValue.data_8;
                    saveSuccess = AdjusterWriteAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.ssIdx,
                                                         (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.objIdx,
                                                         (TINT16)  LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.attribIdx,
                                                         (void FAST *)&value8);
                }
                #endif
                break;

            default:
                saveSuccess = 1;
                break;
            }
        }
    }

    if (saveSuccess == OK)
    {
        LOCAL_variableReady = 0;
        LOCAL_dataAdjusted = 0;
    }

    return saveSuccess;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Sets the selected variable to 1 - to perform auto adjustment
 \author Roger Arnold
 \date   2005-04-14
 \param  - none
 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8 AdjusterDoAutoAdjust(void)
{
    TUSIGN8 returnVal = 0;
#ifdef HMI_SUPPORTS_AUTO_ADJUST
    TUSIGN16 errCode = OK;

    if (LOCAL_dataAdjusted)
    {
        returnVal = 1;

        if (LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT)
        {
            // get timer value (timer goes from 0 to 100)
            ADJUST_DATA tVal;
            TUSIGN8 tType;
            TBOOL adjustInProgress = eFALSE;

            if (AdjusterGetFloatValue((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.value_ssIdx,
                                      (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.dispvalue_objidx,
                                      0,
                                      &tVal,
                                      &tType) == OK)
            {
                #ifdef HMI_USES_DOUBLE
                if (tType == ST_DOUBLE)
                {
                    adjustInProgress = (TUSIGN16)(tVal.data_D + 0.5) < 100 ? eTRUE : eFALSE;
                }
                else
                #endif
                {
                    adjustInProgress = (TUSIGN16)(tVal.data_F + 0.5F) < 100 ? eTRUE : eFALSE;
                }
            }

            TUSIGN8 abort_ss, abort_ob, abort_at;
            TBOOL AbortEnabled = FrameTableGetAutoAdjustAbortSettings(&abort_ss, &abort_ob, &abort_at);

            if ((adjustInProgress) && (AbortEnabled))
            {
                returnVal = 0;

                TUSIGN8 dummyValue = 1;
                errCode = AdjusterWriteAttribute((TUSIGN16)abort_ss,
                                                 (TUSIGN16)abort_ob,
                                                 (TINT16)abort_at,
                                                 (void FAST *)&dummyValue);

                if (errCode < ERROR_CODES)
                {
                    returnVal = 1;
                    dummyValue = 0;
                    LOCAL_dataAdjusted = 0;
                }
            }
        }
    }
    else
    {
        if ((LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_AUTO_ADJUST) || (LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
        {

            TUSIGN8     dummyValue = 1;

            #ifdef HMI_SUPPORTS_DISPLAY_TEST
            if ((LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.value_ssIdx == HMI_IDX) &&
                (LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_objidx == HMI_IDX_displayTest) &&
                (LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_attribidx == 0))
            {
                errCode = (TUSIGN16)OK;
            }
            else
            #endif
            {
                errCode = AdjusterWriteAttribute(LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.value_ssIdx,
                                                 LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_objidx,
                                                 (TINT16)LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_attribidx,
                                                 (void FAST *)&dummyValue);

            }

            if (errCode < ERROR_CODES)
            {
                returnVal = 1;
                dummyValue = 0;
            }
            LOCAL_dataAdjusted = 1;
            AdjusterSetLimitError(dummyValue,errCode);
        }
    }
#endif
    return returnVal;
}

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
TUSIGN8 Adjuster_WaveformAction(TUSIGN8 waveformCursor, TUSIGN8 keystate)
{
    TUSIGN8 returnVal = 1;

    TUSIGN16 waveformSubsystem = (TUSIGN16)(FrameTableGetCurrentFrame()->frameDefinition.waveformFrame.waveformSubsystemIdx);
    TUSIGN16 waveformObject;
    TBOOL   DataValid;
    TUSIGN8 parameter;

    const T_WAVEFORM *pWav = (T_WAVEFORM*)(GetSubsystemPtr(waveformSubsystem));

         if ((waveformCursor == WAVEFORM_CURSOR_H_WIDTH) && (keystate == KEYSTATE_UP))      {parameter = HMI_WAVEFORM_H_SCALE_RES_INC_OBJ;}
    else if ((waveformCursor == WAVEFORM_CURSOR_H_WIDTH) && (keystate == KEYSTATE_DOWN))    {parameter = HMI_WAVEFORM_H_SCALE_RES_DEC_OBJ;}
    else if ((waveformCursor == WAVEFORM_CURSOR_H_OFFSET) && (keystate == KEYSTATE_UP))     {parameter = HMI_WAVEFORM_H_SCALE_OFFSET_INC_OBJ;}
    else if ((waveformCursor == WAVEFORM_CURSOR_H_OFFSET) && (keystate == KEYSTATE_DOWN))   {parameter = HMI_WAVEFORM_H_SCALE_OFFSET_DEC_OBJ;}
    else                                                                                    {returnVal = 0;}

    if (returnVal)
    {
        waveformObject = (TUSIGN16)pWav->GetObjectIndexData(parameter, &DataValid);
        if (!DataValid)
        {
            returnVal = 0;
        }
    }

    if (returnVal)
    {
        TUSIGN8  dummyValue = 1;

        T_UNIT const SLOW *pSub = GetSubsystemPtr(waveformSubsystem);
        if (pSub->Put(pSub,waveformObject,0, (void FAST *)&dummyValue) >= ERROR_CODES)
//      if (AdjusterWriteAttribute(waveformSubsystem, waveformObject, 0, (void FAST *)&dummyValue) >= ERROR_CODES)
        {
            returnVal = 0;
        }
    }
    return returnVal;
}
#endif  // end of #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Performs normal adjustment
 \author Roger Arnold
 \date   2005-04-14
 \param  - MOVE_COMMAND_TYPE command - Defines whether to increment the value (COMMAND_ADJUST_UP)
                                       or to decrement the value (COMMAND_ADJUST_DOWN)
                                       all other command values are ignored
 \return - always returns 1
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
// This is a copy of the "EditFieldDataTable" from PacketManager.c
#define NUMBER_OF_FIELDS_TIME_AND_DATE 6
const TUSIGN8 Adjuster_EditFieldDataTable[3][4][NUMBER_OF_FIELDS_TIME_AND_DATE] =
{
    {   // DD-MM-YYYY format
        {4,5,6,1,2,3},  // Time & Date: DD-MM-YYYY HH:MM:SS
        {1,2,3,0,0,0},  // Time Only:   HH:MM:SS
        {4,5,6,0,0,0},  // Date Only:   DD-MM-YYYY
        {1,2,3,0,0,0}   // Duration:    HHH:MM:SS
    },
    {   // MM-DD-YYYY format
        {5,4,6,1,2,3},  // Time & Date: MM-DD-YY HH:MM:SS
        {1,2,3,0,0,0},  // Time Only:   HH:MM:SS
        {5,4,6,0,0,0},  // Date Only:   MM-DD-YY
        {1,2,3,0,0,0}   // Duration:    HHH:MM:SS
    },
    {   // YYYY-MM-DD format
        {6,5,4,1,2,3},  // Time & Date: YY-MM-DD HH:MM:SS
        {1,2,3,0,0,0},  // Time Only:   HH:MM:SS
        {6,5,4,0,0,0},  // Date Only:   YY-MM-DD
        {1,2,3,0,0,0}   // Duration:    HHH:MM:SS
    }
};
#endif

TUSIGN8 AdjusterAdjust(MOVE_COMMAND_TYPE command)
{
    #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
    TUSIGN8 timeDateCursor;
    #endif

    #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
    TUSIGN8 bitEnumCursor;
    #endif

    #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII) || defined(HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES)
    TUSIGN8 cursor;
    TUSIGN8 selectedChar;
    TINT16  i;
    #endif

    #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
    TUSIGN8 const *characterSet;
    #endif
    TUSIGN8 c;

    if ((FrameTableIsEditEnabled()) && (LOCAL_variableReady))
    {
        #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII) || defined(HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES)
        cursor = FrameTableGetCursorPosition();
        #endif

        #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
        timeDateCursor = FrameTableGetCursorPosition();
        #endif

        #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        bitEnumCursor = FrameTableGetCursorPosition();
        #endif

        switch (LOCAL_pCurrAdjustFrame->frameType)
        {
        #if defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)
        case FRAME_TYPE_2_POINT_pH_CALIBRATION:
            {
                TWO_POINT_pH_CALIBRATION_FRAME_TYPE const *pFrameData = &(LOCAL_pCurrAdjustFrame->frameDefinition.pHCalFrame);

                TUSIGN8 calibrationStatus;
                TBOOL editValid = eTRUE;

                TUSIGN8 ssidx = pFrameData->ssidx;
                TUSIGN8 obidx = pFrameData->objidx_status;

                T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)ssidx);
                (void)pSub->Get(pSub,(TUSIGN16)obidx,0,(void FAST *)&calibrationStatus);

                switch (calibrationStatus)
                {
                case HMI_CAL_STATUS_EDIT_BUFFER_1:      obidx = pFrameData->objidx_buffer_1;    break;
                case HMI_CAL_STATUS_EDIT_BUFFER_2:      obidx = pFrameData->objidx_buffer_2;    break;
                case HMI_CAL_STATUS_EDIT_BUFFER_TEMP:   obidx = pFrameData->objidx_temperature; break;
                default:                                editValid = eFALSE;                     break;
                }

                if (editValid)
                {
                    if (cursor == 0)
                    {
                        ADJUST_DATA temp_value,temp_high,temp_low,temp_inc;
                        TUSIGN8 temp_type;
                        //T_RANGE_E8 temp_rangeE8;
                        TBOOL temp_RulesAreActive;

                        TBOOL negAllowed = eTRUE;
                        TBOOL posAllowed = eTRUE;

                        //(void)AdjusterGetValueAndLimits((TUSIGN16)ssidx, (TUSIGN16)obidx, 0, &temp_value, &temp_high, &temp_low, &temp_inc, &temp_type, &temp_rangeE8, &temp_RulesAreActive);
                        (void)AdjusterGetFloatValueAndLimits((TUSIGN16)ssidx,
                                                             (TUSIGN16)obidx,
                                                             0,
                                                             &temp_value,
                                                             &temp_high,
                                                             &temp_low,
                                                             &temp_inc,
                                                             &temp_RulesAreActive,
                                                             &temp_type);

                        // if both limits are greater than zero therefore negative numbers not allowed
                        // if both limits are less than zero therefore positive numbers not allowed
                        if ((temp_high.data_F >= 0) && (temp_low.data_F >= 0))  {negAllowed = eFALSE;}
                        if ((temp_high.data_F < 0)  && (temp_low.data_F < 0))   {posAllowed = eFALSE;}

                        if      (!posAllowed)       {LOCAL_stringNeg = 1;}                          // +ve numbers not allowed
                        else if (!negAllowed)       {LOCAL_stringNeg = 0;}                          // -ve numbers not allowed
                        else                        {LOCAL_stringNeg = LOCAL_stringNeg ? 0 : 1;}    // at position 0 adjustment is +ve/-ve

                        LOCAL_dataAdjusted = 1;
                    }
                    else if (cursor < FrameTableGetCursorPositionLimit())
                    {
                        c = cursor - 1;
                        selectedChar = LOCAL_stringVal[c];

                        if (selectedChar != '.')
                        {
                            selectedChar = (command == COMMAND_ADJUST_UP) ? selectedChar + 1 : selectedChar - 1;
                            if ((selectedChar > '9') || (selectedChar < '0'))
                            {
                                // make the new character 0 or 9 depending upon direction
                                selectedChar = (command == COMMAND_ADJUST_UP) ? '0' : '9';
                            }
                            c = cursor - 1;
                            LOCAL_stringVal[c] = selectedChar;

                            LOCAL_dataAdjusted = 1;
                        }
                    }
                    else
                    {
                        asm("nop");
                    }
                }
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_MANUAL_ADJUST
        case FRAME_TYPE_MANUAL_ADJUST:
            if (WriteProtect_HMICanAccess(LOCAL_variable,0))
            {
                MOVE_COMMAND_TYPE action;

                #ifdef AZ200_SPECIAL_FEATURES_HMI
                if ((LOCAL_variable.ssIdx == CAL_IDX) && (LOCAL_variable.objIdx == CAL_IDX_PerformanceLogEntryIndex) && (LOCAL_variable.attribIdx == 0))
                {
                    action = command == COMMAND_ADJUST_UP ? COMMAND_ADJUST_DOWN : COMMAND_ADJUST_UP;
                }
                else
                {
                    action = command;
                }
                #else
                    action = command;
                #endif

                #ifdef HMI_USES_DOUBLE
                if (LOCAL_AdjIsDouble)
                {
                    TDOUBLE UpdateAmount = LOCAL_incValue.data_D;

                    #ifdef MANUAL_ADJUST_ACCELERATOR
                        UpdateAmount *= FrameTableGetManAdjustUpdateAmount();
                    #endif

                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    if ((LOCAL_variable.ssIdx == CAL_IDX) && (LOCAL_variable.objIdx == CAL_IDX_PerformanceLogEntryIndex) && (LOCAL_variable.attribIdx == 0))
                    {
                        UpdateAmount = 1;

                        #ifdef MANUAL_ADJUST_ACCELERATOR
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                        #endif
                    }
                    #endif

                    if (action == COMMAND_ADJUST_UP)
                    {
                        LOCAL_adjValue.data_D = LOCAL_adjValue.data_D + UpdateAmount;
                    }
                    else
                    {
                        LOCAL_adjValue.data_D = LOCAL_adjValue.data_D - UpdateAmount;
                    }

                    if (LOCAL_adjValue.data_D > LOCAL_hiLimit.data_D)           // IF new value is greater than maximum THEN
                    {                                                           //
                        LOCAL_adjValue.data_D = LOCAL_hiLimit.data_D;           //    apply maximum limit
                        #ifdef MANUAL_ADJUST_ACCELERATOR                        //
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);   //    and reset Accellerator
                        #endif                                                  //
                    }

                    if (LOCAL_adjValue.data_D < LOCAL_loLimit.data_D)           // IF new value is less than minimum THEN
                    {                                                           //
                        LOCAL_adjValue.data_D = LOCAL_loLimit.data_D;           //    apply minimum limit
                        #ifdef MANUAL_ADJUST_ACCELERATOR                        //
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);   //    and reset Accellerator
                        #endif                                                  //
                    }

                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    if ((LOCAL_variable.ssIdx == CAL_IDX) && (LOCAL_variable.objIdx == CAL_IDX_PerformanceLogEntryIndex) && (LOCAL_variable.attribIdx == 0))
                    {
                        TUSIGN8 nEntries = ((const T_NVINTERFACE*)GetSubsystemPtr(NVINTERFACE_IDX))->GetPerformanceLogEntryCount_SRV(sizeof(LOG_ENTRY));
                        if (nEntries == 0)
                        {
                            LOCAL_adjValue.data_D = (TDOUBLE)0;
                        }
                        else if ((TUSIGN8)LOCAL_adjValue.data_D >= nEntries)
                        {
                            LOCAL_adjValue.data_D = (TDOUBLE)(nEntries - 1);
                        }
                    }
                    #endif

                }
                else
                #endif
                {
                    TFLOAT      UpdateAmount = LOCAL_incValue.data_F;

                    #ifdef MANUAL_ADJUST_ACCELERATOR
                        UpdateAmount *= FrameTableGetManAdjustUpdateAmount();
                    #endif

                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    if ((LOCAL_variable.ssIdx == CAL_IDX) && (LOCAL_variable.objIdx == CAL_IDX_PerformanceLogEntryIndex) && (LOCAL_variable.attribIdx == 0))
                    {
                        UpdateAmount = 1;

                        #ifdef MANUAL_ADJUST_ACCELERATOR
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                        #endif
                    }
                    #endif

                    if (action == COMMAND_ADJUST_UP)
                    {
                        LOCAL_adjValue.data_F = LOCAL_adjValue.data_F + UpdateAmount;
                    }
                    else
                    {
                        LOCAL_adjValue.data_F = LOCAL_adjValue.data_F - UpdateAmount;
                    }

                    if (LOCAL_adjValue.data_F > LOCAL_hiLimit.data_F)           // IF new value is greater than maximum THEN
                    {                                                           //
                        LOCAL_adjValue.data_F = LOCAL_hiLimit.data_F;           //    apply maximum limit
                        #ifdef MANUAL_ADJUST_ACCELERATOR                        //
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);   //    and reset Accellerator
                        #endif                                                  //
                    }

                    if (LOCAL_adjValue.data_F < LOCAL_loLimit.data_F)           // IF new value is less than minimum THEN
                    {                                                           //
                        LOCAL_adjValue.data_F = LOCAL_loLimit.data_F;           //    apply minimum limit
                        #ifdef MANUAL_ADJUST_ACCELERATOR                        //
                        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);   //    and reset Accellerator
                        #endif                                                  //
                    }

                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    if ((LOCAL_variable.ssIdx == CAL_IDX) && (LOCAL_variable.objIdx == CAL_IDX_PerformanceLogEntryIndex) && (LOCAL_variable.attribIdx == 0))
                    {
                        TUSIGN8 nEntries = ((const T_NVINTERFACE*)GetSubsystemPtr(NVINTERFACE_IDX))->GetPerformanceLogEntryCount_SRV(sizeof(LOG_ENTRY));
                        if (nEntries == 0)
                        {
                            LOCAL_adjValue.data_F = (TFLOAT)0;
                        }
                        else
                        {
                            if ((TUSIGN8)LOCAL_adjValue.data_F >= nEntries)
                            {
                                LOCAL_adjValue.data_F = (TFLOAT)(nEntries - 1);
                            }
                        }
                    }
                    #endif
                }

                if (AdjusterWriteValueAndCheckLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                                     (TUSIGN16)LOCAL_variable.objIdx,
                                                     LOCAL_variable.attribIdx,
                                                     &LOCAL_adjValue,
                                                     LOCAL_dataType) == OK)
                {
                    LOCAL_dataAdjusted = 1;
                }
            }
          break;
        #endif

        case FRAME_TYPE_STRING_PARAM:
        case FRAME_TYPE_STRING_PARAM_PACKED:
            {
                #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)

                TUSIGN8 LimitedNumericCharacterSet[13];
                //TBOOL   LimitedNumericCharacterSetInUse = eFALSE;

                characterSet = FrameTableGetCharacterSet(LOCAL_Attributes.characterSet_dpPosn);

                if (LOCAL_Attributes.characterSet_dpPosn == CHARACTER_SET_NUMERIC)
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

                        for (jj = MinValid; jj <= MaxValid; jj++)
                        {
                            LimitedNumericCharacterSet[ii ++] = jj + 0x30;
                        }
                        characterSet = LimitedNumericCharacterSet;
                        //LimitedNumericCharacterSetInUse = eTRUE;
                    }
                }


                if ((cursor < FrameTableGetCursorPositionLimit())
                 && (cursor <= MAX_STRING_PARAMETER_LENGTH)
                 && (characterSet != NULL))
                {
                    TINT16 characterSetLength = (TINT16)strlen((const char *)characterSet);

                    TUSIGN8 UpdateAmount = 1;

                    #ifdef STRING_ACCELERATOR
                    if (characterSetLength > 10)
                    {
                        UpdateAmount = FrameTableGetStringUpdateAmount();
                    }
                    #endif

                    // Get current character and find its position in the appropriate character set
                    selectedChar = LOCAL_stringVal[cursor];
                    if (selectedChar == 0x00)
                    {
                        selectedChar = 0x08;
                    }

                    i = characterSetLength - 1;
                    while ((characterSet[i] != selectedChar) && (i))
                    {
                        i --;
                    }

                    if (command == COMMAND_ADJUST_UP)
                    {
                        i += UpdateAmount;

                        if (characterSetLength <= 10)
                        {
                            if (i >= characterSetLength)
                            {
                                i = characterSetLength - 1;
                            }
                        }


                        while (i >= characterSetLength)
                        {
                            i -= characterSetLength;
                        }
                    }
                    else
                    {
                        i -= UpdateAmount;

                        if (characterSetLength <= 10)
                        {
                            if (i < 0)
                            {
                                i = 0;
                            }
                        }

                        while ( i < 0 )
                        {
                            i += characterSetLength;
                        }
                    }

                    selectedChar = characterSet[i];
                    if (selectedChar == 0x08)
                    {
                        selectedChar = 0x00;
                    }

                    LOCAL_dataAdjusted = 1;
                    LOCAL_stringVal[cursor] = selectedChar;
                }
                #endif
            }
            break;

        #ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
        case FRAME_TYPE_ENUMERATED_PARAM:
        case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:

            #ifdef HMI_SUPPORTS_16BIT_ENUMS

            #ifdef SIZE_ST_E16
            if ((LOCAL_dataType == ST_U16) || (LOCAL_dataType == ST_E16))
            #else
            if (LOCAL_dataType == ST_U16)
            #endif
            {
                TUSIGN16 c_e16 = 0;

                // Find current place in list of enum values
                while ((LOCAL_pEnum16Values[c_e16] != LOCAL_adjValue.data_16) && (c_e16 < LOCAL_nEnum16Values - 1))
                {
                    c_e16 ++;
                }

                if (command == COMMAND_ADJUST_UP)
                {
                    #ifdef ENUM_ACCELERATOR
                    TUSIGN8 UpdateAmount = FrameTableGetEnumUpdateAmount();
                    #else
                    TUSIGN8 UpdateAmount = 1;
                    #endif

                    if (c_e16 == 0)                                             // IF current value at minimum THEN
                    {                                                           //
                        c_e16 = (LOCAL_nEnum16Values - 1);                      //    wrap to maximum
                        #ifdef ENUM_ACCELERATOR                                 //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else if (UpdateAmount >= c_e16)                             // ELSE IF decrement will reach or go below minimum THEN
                    {                                                           //
                        c_e16 = 0;                                              //    stop at maximum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else                                                        // ELSE
                    {                                                           //
                        c_e16 -= UpdateAmount;                                  //    decrement the value
                    }                                                           //
                }
                else
                {
                    #ifdef ENUM_ACCELERATOR
                    TUSIGN8 UpdateAmount = FrameTableGetEnumUpdateAmount();
                    #else
                    TUSIGN8 UpdateAmount = 1;
                    #endif

                    if (c_e16 == (LOCAL_nEnum16Values - 1))                     // IF current value at maximum THEN
                    {                                                           //
                        c_e16 = 0;                                              //    wrap to minimum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else if ((c_e16 + UpdateAmount) >= (LOCAL_nEnum16Values-1)) // ELSE IF increment will reach or go above maximum THEN
                    {                                                           //
                        c_e16 = (LOCAL_nEnum16Values - 1);                      //    stop at maximum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else                                                        // ELSE
                    {                                                           //
                        c_e16 += UpdateAmount;                                  //    increment the value
                    }                                                           //
                }

                LOCAL_adjValue.data_16 = LOCAL_pEnum16Values[c_e16];

                LOCAL_dataAdjusted = 1;
            }
            else
            #endif
            {
                TUSIGN8 c_e8 = 0;

                // Find current place in list of enum values
                while ((LOCAL_pEnum8Values[c_e8] != LOCAL_adjValue.data_8) && (c_e8 < LOCAL_nEnum8Values - 1))
                {
                    c_e8 ++;
                }

                if (command == COMMAND_ADJUST_UP)
                {
                    #ifdef ENUM_ACCELERATOR
                    TUSIGN8 UpdateAmount = FrameTableGetEnumUpdateAmount();
                    #else
                    TUSIGN8 UpdateAmount = 1;
                    #endif

                    if (c_e8 == 0)                                              // IF current value at minimum THEN
                    {                                                           //
                        c_e8 = (LOCAL_nEnum8Values - 1);                         //    wrap to maximum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else if (UpdateAmount >= c_e8)                              // ELSE IF decrement will reach or go below minimum THEN
                    {                                                           //
                        c_e8 = 0;                                               //    stop at maximum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else                                                        // ELSE
                    {                                                           //
                        c_e8 -= UpdateAmount;                                   //    decrement the value
                    }                                                           //
                }
                else
                {
                    #ifdef ENUM_ACCELERATOR
                    TUSIGN8 UpdateAmount = FrameTableGetEnumUpdateAmount();
                    #else
                    TUSIGN8 UpdateAmount = 1;
                    #endif

                    if (c_e8 == (LOCAL_nEnum8Values - 1))                       // IF current value at maximum THEN
                    {                                                           //
                        c_e8 = 0;                                               //    wrap to minimum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else if ((c_e8 + UpdateAmount) >= (LOCAL_nEnum8Values-1))   // ELSE IF increment will reach or go above maximum THEN
                    {                                                           //
                        c_e8 = (LOCAL_nEnum8Values - 1);                        //    stop at maximum
                        #ifdef ENUM_ACCELERATOR                                //
                        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);        //    and reset Accellerator
                        #endif                                                  //
                    }                                                           //
                    else                                                        // ELSE
                    {                                                           //
                        c_e8 += UpdateAmount;                                   //    increment the value
                    }                                                           //
                }

                LOCAL_adjValue.data_8 = LOCAL_pEnum8Values[c_e8];

                LOCAL_dataAdjusted = 1;
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        case FRAME_TYPE_BIT_ENUMERATED_PARAM:

            if (bitEnumCursor < LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.numberOfBits)
            {
                TUSIGN8 bitmask = 0x01;

                if (LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.displayStyle == 3)
                {
                    bitmask = bitmask << bitEnumCursor;
                }
                else
                {
                    bitmask = bitmask << (LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.numberOfBits - 1);
                    bitmask = bitmask >> bitEnumCursor;
                }

                if ((LOCAL_adjValue.data_8 & bitmask) == 0) // IF selected bit is clear THEN
                {                                           //
                    LOCAL_adjValue.data_8 |= bitmask;       //     set selected bit
                }                                           //
                else                                        // ELSE
                {                                           //
                    LOCAL_adjValue.data_8 &= ~bitmask;      //     clear selected bit
                }                                           //

                LOCAL_dataAdjusted = 1;
            }
	        break;
	    #endif




        #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
        case FRAME_TYPE_IP_ADDRESS:
            if ((timeDateCursor < FrameTableGetCursorPositionLimit())
            &&  (timeDateCursor < NUMBER_OF_FIELDS_IP_ADDRESS))
            {
                IP_ADDRESS_HMI  adjustable_IP_Addr;
                TUSIGN8 dotted_IP_field = timeDateCursor;

                #ifdef IPADDR_ACCELERATOR
                TUSIGN8 UpdateAmount = FrameTableGetIPAddressUpdateAmount();
                #else
                TUSIGN8 UpdateAmount = 1;
                #endif

                adjustable_IP_Addr.networkAddress = LOCAL_TimeAndDate;

                if (command == COMMAND_ADJUST_UP)
                {
                    if (adjustable_IP_Addr.dotted_IP[dotted_IP_field] > (255 - UpdateAmount))
                    {
                        adjustable_IP_Addr.dotted_IP[dotted_IP_field] = 0;
                        #ifdef IPADDR_ACCELERATOR
                        FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                        #endif
                    }
                    else
                    {
                        adjustable_IP_Addr.dotted_IP[dotted_IP_field] += UpdateAmount;
                    }
                }
                else
                {
                    if (adjustable_IP_Addr.dotted_IP[dotted_IP_field] < UpdateAmount)
                    {
                        adjustable_IP_Addr.dotted_IP[dotted_IP_field] = 255;
                        #ifdef IPADDR_ACCELERATOR
                        FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                        #endif
                    }
                    else
                    {
                        adjustable_IP_Addr.dotted_IP[dotted_IP_field] -= UpdateAmount;
                    }
                }

                LOCAL_TimeAndDate = adjustable_IP_Addr.networkAddress;

                LOCAL_dataAdjusted = 1;
            }
            break;
        #endif



        #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
    	case FRAME_TYPE_TIME_AND_DATE_PARAM:
            if ((timeDateCursor < FrameTableGetCursorPositionLimit())
             && (timeDateCursor < NUMBER_OF_FIELDS_TIME_AND_DATE))
            {
                TUSIGN8 format = SFD_SIMPLE.DateFormat;
                if (format > HMI_DATE_FORMAT_YYYY_MM_DD)
                {
                    format = HMI_DATE_FORMAT_YYYY_MM_DD;
                }

                TUSIGN8 EditField = Adjuster_EditFieldDataTable[format][LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.TimeAndOrDate][timeDateCursor];


                if (LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.TimeAndOrDate == 3)
                {
                    #ifdef DATE_TIME_ACCELERATOR
                    TUSIGN32 UpdateAmount = (TUSIGN32)FrameTableGetDateTimeUpdateAmount(EditField,(TUSIGN8)(FrameTableGetTimeDurationLimit() / 3600));

                    TUSIGN8 oldHrs = (TUSIGN8)(LOCAL_TimeAndDate / SECONDS_IN_1_HOUR);
                    TUSIGN8 oldMin = (TUSIGN8)((LOCAL_TimeAndDate - ((TUSIGN32)oldHrs * SECONDS_IN_1_HOUR))/SECONDS_IN_1_HOUR);
                    TUSIGN8 oldSec = (TUSIGN8)(LOCAL_TimeAndDate % SECONDS_IN_1_MINUTE);
                    #else
                    TUSIGN32 UpdateAmount = 1;
                    #endif

                    if (command == COMMAND_ADJUST_UP)
                    {
                        switch (EditField)
                        {
                        case 1: /* Hours   */
                            LOCAL_TimeAndDate += (UpdateAmount * SECONDS_IN_1_HOUR);
                            break;
                        case 2: /* Minutes */
                            LOCAL_TimeAndDate += (UpdateAmount * SECONDS_IN_1_MINUTE);
                            break;
                        case 3: /* Seconds */
                            LOCAL_TimeAndDate += UpdateAmount;
                            break;
                        default:
                            VIP_ASSERT(0);
                            break;
                        }
                    }
                    else
                    {
                        switch (EditField)
                        {
                        case 1: /* Hours   */
                            LOCAL_TimeAndDate += (FrameTableGetTimeDurationLimit() - (UpdateAmount * SECONDS_IN_1_HOUR));
                            break;
                        case 2: /* Minutes */
                            LOCAL_TimeAndDate += (FrameTableGetTimeDurationLimit() - (UpdateAmount * SECONDS_IN_1_MINUTE));
                            break;
                        case 3: /* Seconds */
                            LOCAL_TimeAndDate += (FrameTableGetTimeDurationLimit() - UpdateAmount);
                            break;
                        default:
                            VIP_ASSERT(0);
                            break;
                        }
                    }

                    #ifdef DATE_TIME_ACCELERATOR
                    TUSIGN8 newHrs = (TUSIGN8)(LOCAL_TimeAndDate / SECONDS_IN_1_HOUR);
                    TUSIGN8 newMin = (TUSIGN8)((LOCAL_TimeAndDate - ((TUSIGN32)newHrs * SECONDS_IN_1_HOUR))/SECONDS_IN_1_HOUR);
                    TUSIGN8 newSec = (TUSIGN8)(LOCAL_TimeAndDate % SECONDS_IN_1_MINUTE);

                    if (((newHrs != oldHrs) && (EditField != 1))    // if hours changed & edit field != hours
                     || ((newMin != oldMin) && (EditField != 2))    // if minutes changed & edit field != minutes
                     || ((newSec != oldSec) && (EditField != 3))    // if seconds changed & edit field != seconds
                     || (EditField == 0)
                     || (EditField > 3))
                    {
                        FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                    }
                    #endif

                    while (LOCAL_TimeAndDate >= FrameTableGetTimeDurationLimit())
                    {
                        LOCAL_TimeAndDate -= FrameTableGetTimeDurationLimit();
                        #ifdef DATE_TIME_ACCELERATOR
                        FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                        #endif
                    }
                }
                else
                {
                    #ifdef DATE_TIME_ACCELERATOR
                    TUSIGN32 UpdateAmount = (TUSIGN32)FrameTableGetDateTimeUpdateAmount(EditField,23);
                    #else
                    TUSIGN32 UpdateAmount = 1;
                    #endif

                    TIMEANDDATE AdjTime, oldAdjTime;
                    (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);
                    oldAdjTime = AdjTime;

                    if (command == COMMAND_ADJUST_UP)
                    {
                        switch (EditField)
                        {
                        case 1: /* Hours   */
                            LOCAL_TimeAndDate += (UpdateAmount * SECONDS_IN_1_HOUR);
                            break;
                        case 2: /* Minutes */
                            LOCAL_TimeAndDate += (UpdateAmount * SECONDS_IN_1_MINUTE);
                            break;
                        case 3: /* Seconds */
                            LOCAL_TimeAndDate += UpdateAmount;
                            break;

                        case 4: /* Date    */
                            LOCAL_TimeAndDate += SECONDS_IN_1_DAY;
                            break;
                        case 5: /* Month   */
                            (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);
                            switch (AdjTime.month)
                            {
                            case 1: // January - special case as next month is February
                                switch (AdjTime.date)
                                {
                                    case 31:        LOCAL_TimeAndDate += AdjTime.year % 4 ?   SECONDS_IN_28_DAYS : SECONDS_IN_29_DAYS;      break;
                                    case 30:        LOCAL_TimeAndDate += AdjTime.year % 4 ?   SECONDS_IN_29_DAYS : SECONDS_IN_30_DAYS;      break;
                                    case 29:        LOCAL_TimeAndDate += AdjTime.year % 4 ?   SECONDS_IN_30_DAYS : SECONDS_IN_31_DAYS;      break;
                                    default:        LOCAL_TimeAndDate += SECONDS_IN_31_DAYS;                                                break;
                                }
                                break;
                            case 2: // February - special case due to leap year
                                LOCAL_TimeAndDate += AdjTime.year % 4 ?   SECONDS_IN_28_DAYS : SECONDS_IN_29_DAYS;
                                break;

                            case 3:  // March     - next month has only 30 days but this month has 31 days
                            case 5:  // May       - next month has only 30 days but this month has 31 days
                            case 8:  // August    - next month has only 30 days but this month has 31 days
                            case 10: // October   - next month has only 30 days but this month has 31 days
                                LOCAL_TimeAndDate += AdjTime.date == 31 ? SECONDS_IN_30_DAYS : SECONDS_IN_31_DAYS;
                                break;

                            case 4:  // April     - 30 days
                            case 6:  // June      - 30 days
                            case 9:  // September - 30 days
                            case 11: // September - 30 days
                                LOCAL_TimeAndDate += SECONDS_IN_30_DAYS;
                                break;

                            case 7:  // July      - 31 days - next month also has 31
                            case 12: // December  - 31 days - next month also has 31
                                LOCAL_TimeAndDate += SECONDS_IN_31_DAYS;
                                break;

                            default:
                                VIP_ASSERT(0);
                                break;
                            }
                            break;

                        case 6: /* Year  */
                            (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);
                            switch (AdjTime.year % 4)
                            {
                                case 3: // Next year is a leap year
                                    if (AdjTime.month >= 3)
                                    {
                                        LOCAL_TimeAndDate += SECONDS_IN_LEAP_YEAR;
                                    }
                                    else
                                    {
                                        LOCAL_TimeAndDate += SECONDS_IN_YEAR;
                                    }
                                    break;

                                case 2: // No leap year to worry about
                                case 1: // No leap year to worry about
                                    LOCAL_TimeAndDate += SECONDS_IN_YEAR;
                                    break;

                                case 0: // This Year is a leap year
                                    if ((AdjTime.month >= 3) || ((AdjTime.month == 2) && (AdjTime.date == 29)))
                                    {
                                        LOCAL_TimeAndDate += SECONDS_IN_YEAR;
                                    }
                                    else
                                    {
                                        LOCAL_TimeAndDate += SECONDS_IN_LEAP_YEAR;
                                    }
                                    break;

                                default:
                                    VIP_ASSERT(0);
                                    break;
                            }
                            break;

                            default:
                                VIP_ASSERT(0);
                                break;
                        }

                        (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);

                        if (EditField <= 3)
                        {
                            if (oldAdjTime.date != AdjTime.date)
                            {
                                LOCAL_TimeAndDate -= SECONDS_IN_1_DAY;
                                #ifdef DATE_TIME_ACCELERATOR
                                FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                                #endif
                            }
                        }
                    }
                    else
                    {
                        switch (EditField)
                        {
                        case 1: /* Hours   */
                            LOCAL_TimeAndDate -= (UpdateAmount * SECONDS_IN_1_HOUR);
                            break;
                        case 2: /* Minutes */
                            LOCAL_TimeAndDate -= (UpdateAmount * SECONDS_IN_1_MINUTE);
                            break;
                        case 3: /* Seconds */
                            LOCAL_TimeAndDate -= UpdateAmount;
                            break;
                        case 4: /* Date    */
                            LOCAL_TimeAndDate -= SECONDS_IN_1_DAY;
                            break;
                        case 5: /* Month   */
                            (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);
                            switch (AdjTime.month)
                            {
                            case 12: // December  - 31 days but previous month has only 30
                            case 10: // October   - 31 days but previous month has only 30
                            case 7:  // July      - 31 days but previous month has only 30
                            case 5:  // May       - 31 days but previous month has only 30
                                LOCAL_TimeAndDate -= AdjTime.date == 31 ? SECONDS_IN_31_DAYS : SECONDS_IN_30_DAYS;
                                break;

                            case 11: // November  - previous month has 31
                            case 9:  // September - previous month has 31
                            case 8:  // August    - previous month has 31
                            case 6:  // June      - previous month has 31
                            case 4:  // April     - previous month has 31
                            case 2:  // February  - previous month has 31
                            case 1:  // January   - previous month has 31
                                LOCAL_TimeAndDate -= SECONDS_IN_31_DAYS;
                                break;

                           case 3:   // March     - special case as previous month is February
                                switch (AdjTime.date)
                                {
                                    case 31:        LOCAL_TimeAndDate -= SECONDS_IN_31_DAYS;      break;
                                    case 30:        LOCAL_TimeAndDate -= SECONDS_IN_30_DAYS;      break;
                                    case 29:        LOCAL_TimeAndDate -= SECONDS_IN_29_DAYS;      break;
                                    default:        LOCAL_TimeAndDate -= AdjTime.year % 4 ?   SECONDS_IN_28_DAYS : SECONDS_IN_29_DAYS; break;
                                }
                                break;

                            default:
                                VIP_ASSERT(0);
                                break;
                            }
                            break;

                        case 6: /* Year  */
                            (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);
                            switch (AdjTime.year % 4)
                            {
                            case 0: // This Year is a leap year
                                if ((AdjTime.month >= 3) || ((AdjTime.month == 2) && (AdjTime.date == 29)))
                                {
                                    LOCAL_TimeAndDate -= SECONDS_IN_LEAP_YEAR;
                                }
                                else
                                {
                                    LOCAL_TimeAndDate -= SECONDS_IN_YEAR;
                                }
                                break;

                            case 1: // Previous year is a leap year
                                if (AdjTime.month >= 3)
                                {
                                    LOCAL_TimeAndDate -= SECONDS_IN_YEAR;
                                }
                                else
                                {
                                    LOCAL_TimeAndDate -= SECONDS_IN_LEAP_YEAR;
                                }
                                break;

                            case 2: // No leap year to worry about
                            case 3: // No leap year to worry about
                                LOCAL_TimeAndDate -= SECONDS_IN_YEAR;
                                break;

                            default:
                                VIP_ASSERT(0);
                                break;
                            }
                            break;

                        default:
                            VIP_ASSERT(0);
                            break;
                        }

                        (void)AdjusterConvertSecondsToDate(LOCAL_TimeAndDate, &AdjTime);

                        if (EditField <= 3)
                        {
                            if (oldAdjTime.date != AdjTime.date)
                            {
                                LOCAL_TimeAndDate += SECONDS_IN_1_DAY;
                                #ifdef DATE_TIME_ACCELERATOR
                                FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                                #endif
                            }
                        }
                    }
                }
                LOCAL_dataAdjusted = 1;
            }
	        break;
	    #endif

        #ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
        case FRAME_TYPE_NUMERICAL_PARAM:
        #endif


        #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
        case FRAME_TYPE_MANUAL_CALIBRATION:
        #endif

        #if defined (HMI_SUPPORTS_MANUAL_CALIBRATION) || defined (HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES)

            if (cursor == 0)
            {
                if (LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_NUMERICAL_PARAM)
                {
                    ADJUST_DATA temp_value,temp_high,temp_low,temp_inc;
                    TUSIGN8 temp_type;
//                  T_RANGE_E8 temp_rangeE8;
                    TBOOL temp_RulesAreActive;

                    TBOOL negAllowed = eTRUE;   // assume negative numbers are allowed
                    TBOOL posAllowed = eTRUE;   // assume positive numbers are allowed

                    (void)AdjusterGetFloatValueAndLimits((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.value.ssIdx,
                                                         (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.value.objIdx,
                                                         LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.value.attribIdx,
                                                         &temp_value,
                                                         &temp_high,
                                                         &temp_low,
                                                         &temp_inc,
                                                         &temp_RulesAreActive,
                                                         &temp_type);
                    if (temp_RulesAreActive)
                    {
                        // rules are active so check if positive/negative numbers are allowed
                        #ifdef HMI_USES_DOUBLE
                        if (temp_type == ST_DOUBLE)
                        {
                            if (temp_high.data_D >= 0 && temp_low.data_D >= 0)
                            {
                                // both limits are greater than zero therefore negative numbers not allowed
                                negAllowed = eFALSE;
                            }
                            if (temp_high.data_D < 0 && temp_low.data_D < 0)
                            {
                                // both limits are less than zero therefore positive numbers not allowed
                                posAllowed = eFALSE;
                            }
                        }
                        else
                        #endif
                        {
                            if ((temp_high.data_F >= 0) && (temp_low.data_F >= 0))
                            {
                                // both limits are greater than zero therefore negative numbers not allowed
                                negAllowed = eFALSE;
                            }
                            if ((temp_high.data_F < 0) && (temp_low.data_F < 0))
                            {
                                // both limits are less than zero therefore positive numbers not allowed
                                posAllowed = eFALSE;
                            }
                        }
                    }
                    else
                    {
                        if ((temp_type == ST_U8) || (temp_type == ST_U16) || (temp_type == ST_U32) || (temp_type == ST_CHAR) || (temp_type == ST_WIDECHAR))
                        {
                            // unsigned data types therefore negative numbers not allowed
                            negAllowed = eFALSE;
                        }
                    }

                    if (!posAllowed)
                    {
                        LOCAL_stringNeg = 1;                        // +ve numbers not allowed
                    }
                    else if (!negAllowed)
                    {
                        LOCAL_stringNeg = 0;                        // -ve numbers not allowed
                    }
                    else
                    {
                        LOCAL_stringNeg = LOCAL_stringNeg ? 0 : 1;  // at position 0 adjustment is +ve/-ve
                    }
                }
                else
                {
                    LOCAL_stringNeg = LOCAL_stringNeg ? 0 : 1;  // at position 0 adjustment is +ve/-ve
                }
                LOCAL_dataAdjusted = 1;
            }
            else if ((cursor < FrameTableGetCursorPositionLimit())
                 && (cursor <= MAX_STRING_PARAMETER_LENGTH))
            {
                c = cursor - 1;
                selectedChar = LOCAL_stringVal[c];

                if (selectedChar == '.')
                {
                    selectedChar = (command == COMMAND_ADJUST_UP) ? '0' : '9';
                }
                else
                {
                    selectedChar = (command == COMMAND_ADJUST_UP) ? selectedChar + 1 : selectedChar - 1;

                    if ((selectedChar > '9') || (selectedChar < '0'))
                    {
                        c = 0;
                        for (i = (TINT16)strlen((char *)LOCAL_stringVal) - 1; i >= 0; i--)
                        {
                            if (LOCAL_stringVal[i] == '.')
                            {
                                c = 1;
                            }
                        }

                        // Decimal point cannot be selected if any of the following is true...
                        // ..(1) this is the last character
                        // ..(2) this is the first character
                        // ..(3) a decimal point already exists in the value
                        // ..(4) the value is displayed with a fixed dp position
                        if (/* (1) */ (cursor == (FrameTableGetCursorPositionLimit() - 1))
                         || /* (2) */ (cursor == 1)
                         || /* (3) */ c
                         || /* (4) */ (((LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_NUMERICAL_PARAM) && (LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn != MAX_RESOLUTION))))
                        {
                            // make the new character 0 or 9 depending upon direction
                            selectedChar = (command == COMMAND_ADJUST_UP) ? '0' : '9';
                        }
                        else
                        {
                            // make the new character . depending upon direction
                            selectedChar = '.';
                        }
                    }
                }
                c = cursor - 1;
                LOCAL_stringVal[c] = selectedChar;

                LOCAL_dataAdjusted = 1;
            }
            else
            {
                asm("nop");
            }

            break;
        #endif


        #ifdef HMI_SUPPORTS_INTEGER_FRAMES
        case FRAME_TYPE_INTEGER_PARAM:
            if (cursor == 0)
            {
                LOCAL_stringNeg = 0;                        // -ve numbers not allowed
                LOCAL_dataAdjusted = 1;
            }
            else if (cursor < FrameTableGetCursorPositionLimit())
            {
                c = cursor - 1;
                selectedChar = LOCAL_stringVal[c];

                if (selectedChar != '.')
                {

                    if (command == COMMAND_ADJUST_UP)
                    {
                        selectedChar = selectedChar >= '9' ? '0' : selectedChar + 1;
                    }
                    else
                    {
                        selectedChar = selectedChar <= '0' ? '9' : selectedChar - 1;
                    }

                    LOCAL_stringVal[c] = selectedChar;
                    LOCAL_dataAdjusted = 1;
                }
            }
            else
            {
                asm("nop");
            }
            break;
        #endif

        default:
            break;
        }
    }
    return 1;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Initialises the adjuster with a new variable
 \author Roger Arnold
 \date   2005-04-14
 \param  - none
 \return - 0: adjuster not initialised - new variable is not ready for adjustment
         - 1: adjuster is initialised - new variable is ready for adjustment
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8 AdjusterInitialiseNewVariable(void* pAdjustFrame)
{
    T_RANGE_E8  enum8Range;
    #ifdef HMI_SUPPORTS_16BIT_ENUMS
    T_RANGE_E16 enum16Range;
    #endif

    TBOOL rulesAreActive;

    if (pAdjustFrame == NULL)
    {
        LOCAL_pCurrAdjustFrame = FrameTableGetCurrentFrame();
    }
    else
    {
        LOCAL_pCurrAdjustFrame = (FRAME_FORMAT_TYPE const *)pAdjustFrame;
    }

    switch (LOCAL_pCurrAdjustFrame->frameType)
    {
    case FRAME_TYPE_MANUAL_ADJUST:
        LOCAL_Attributes = LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.attributes;

        LOCAL_variable.ssIdx = LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.value_ssIdx;
        LOCAL_variable.objIdx = LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_objidx;
        LOCAL_variable.attribIdx = LOCAL_pCurrAdjustFrame->frameDefinition.adjParamFrame.adjvalue_attribidx;

        if (AdjusterGetFloatValueAndLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                           (TUSIGN16)LOCAL_variable.objIdx,
                                           LOCAL_variable.attribIdx,
                                           &LOCAL_adjValue,
                                           &LOCAL_hiLimit,
                                           &LOCAL_loLimit,
                                           &LOCAL_incValue,
                                           &rulesAreActive,
                                           &LOCAL_dataType) == OK)
        {
            if ((LOCAL_dataType == ST_U8)
             || (LOCAL_dataType == ST_U16)
             || (LOCAL_dataType == ST_I16)
             || (LOCAL_dataType == ST_U32)
             || (LOCAL_dataType == ST_FLOAT)
             #ifdef HMI_USES_DOUBLE
             || (LOCAL_dataType == ST_DOUBLE)
             #endif
             )
            {
                LOCAL_variableReady = 1;
                LOCAL_unadj_adjvalue = LOCAL_adjValue;
            }

            #ifdef HMI_USES_DOUBLE
                LOCAL_AdjIsDouble = LOCAL_dataType == ST_DOUBLE ? eTRUE : eFALSE;
            #endif
        }
        break;

    #ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
    case FRAME_TYPE_2_POINT_pH_CALIBRATION:
        {
            TWO_POINT_pH_CALIBRATION_FRAME_TYPE const *pFrameData = &(LOCAL_pCurrAdjustFrame->frameDefinition.pHCalFrame);

            TUSIGN8 calibrationStatus;
            T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pFrameData->ssidx);
            (void)pSub->Get(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);

            TUSIGN8 edit_decpt, edit_chars;
            TFLOAT abs_max, abs_min;

            T_DO_ATTRIB_RANGE Range; // Pointer to range information

            switch (calibrationStatus)
            {
            case HMI_CAL_STATUS_VIEW_BUFFER_TEMP:
            case HMI_CAL_STATUS_EDIT_BUFFER_TEMP:
                LOCAL_variable.ssIdx = pFrameData->ssidx;
                LOCAL_variable.objIdx = pFrameData->objidx_temperature;
                LOCAL_variable.attribIdx = 0;
                edit_decpt = 1;
                edit_chars = 5;
                LOCAL_variableReady = 1;
                break;

            case HMI_CAL_STATUS_VIEW_BUFFER_1:
            case HMI_CAL_STATUS_EDIT_BUFFER_1:
            case HMI_CAL_STATUS_VIEW_BUFFER_2:
            case HMI_CAL_STATUS_EDIT_BUFFER_2:
                LOCAL_variable.ssIdx = pFrameData->ssidx;
                if ((calibrationStatus == HMI_CAL_STATUS_VIEW_BUFFER_1) || (calibrationStatus == HMI_CAL_STATUS_EDIT_BUFFER_1))
                {
                    LOCAL_variable.objIdx = pFrameData->objidx_buffer_1;
                }
                else
                {
                    LOCAL_variable.objIdx = pFrameData->objidx_buffer_2;
                }
                LOCAL_variable.attribIdx = 0;

                pSub->GetAttributeDescription(pSub,(TUSIGN16)LOCAL_variable.objIdx,0,&Range);

                // get dp associated with process value
                edit_decpt = FrameTableGetSignal((TUSIGN8)pFrameData->ProcessSignal_1)->dpPosn;
                AdjusterAdjustResolutionForCustomDP(&edit_decpt);

                // calculate number of characters for buffer value
                if (edit_decpt == 0)
                {
                    edit_chars = 0;             // no characters reqd for dp
                }
                else
                {
                    edit_chars = edit_decpt + 1; // characters reqd for decimal places and '.' symbol
                }

                // calculate largest potential number
                abs_max = Range.range.flt.max < 0 ? (Range.range.flt.max * -1) : Range.range.flt.max;
                abs_min = Range.range.flt.min < 0 ? (Range.range.flt.min * -1) : Range.range.flt.min;
                if (abs_min > abs_max)  {abs_max = abs_min;}

                if      (abs_max <  10.0f)  {edit_chars += 1;}  // character(s) reqd for units
                else if (abs_max < 100.0f)  {edit_chars += 2;}  // character(s) reqd for tens & units
                else if (abs_max < 1000.0f) {edit_chars += 3;}  // character(s) reqd for hundreds,tens & units
                else                        {edit_chars += 4;}  // character(s) reqd for thousands,hundreds,tens & units

                LOCAL_variableReady = 1;
                break;


            default:
                LOCAL_variableReady = 0;
                break;
            }

            if (LOCAL_variableReady)
            {
                LOCAL_Attributes.maxCharacters = edit_chars;
                LOCAL_Attributes.characterSet_dpPosn = edit_decpt;

                if (AdjusterGetFloatValueAndLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                                   (TUSIGN16)LOCAL_variable.objIdx,
                                                   LOCAL_variable.attribIdx,
                                                   &LOCAL_adjValue,
                                                   &LOCAL_hiLimit,
                                                   &LOCAL_loLimit,
                                                   &LOCAL_incValue,
                                                   &rulesAreActive,
                                                   &LOCAL_dataType) == OK)
                {
                    TUSIGN8 tempString[20];
                    TUSIGN8 tempStringIndex = 0;
                    TUSIGN8 within_over_under = 0;


                    (void)strcpy((char *)tempString,(char *)AdjusterBuildNumberString(LOCAL_adjValue.data_F, LOCAL_Attributes.maxCharacters, LOCAL_Attributes.characterSet_dpPosn, &LOCAL_stringNeg, &within_over_under));
                    while(tempString[tempStringIndex] == 0x20)
                    {
                        tempString[tempStringIndex] = 0x30;
                        tempStringIndex ++;
                    }
                    (void)strcpy((char *)LOCAL_stringVal,(char *)tempString);

                }
            }
        }
        break;
    #endif

    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
    case FRAME_TYPE_INTEGER_PARAM:
        {
            LOCAL_Attributes = LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.attributes;
            LOCAL_variable = LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.value;

            T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)LOCAL_variable.ssIdx);

            // Get Value
            TUSIGN32 Value32 = 0;
            (void)pSub->Get(pSub,(TUSIGN16)LOCAL_variable.objIdx,(TINT16)LOCAL_variable.attribIdx,(void FAST *)&Value32);
            LOCAL_adjValue.data_32 = Value32;

            //
            T_DO_ATTRIB_RANGE Range;
            Range.rulesAreActive = eFALSE;
            Range.dataType = ST_U32;
            Range.range.u32.min = 0;
            Range.range.u32.max = 0;
            Range.range.u32.increment = 0;
            (void)pSub->GetAttributeDescription(pSub,(TUSIGN16)LOCAL_variable.objIdx,(TUSIGN16)LOCAL_variable.attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range);

            LOCAL_dataType =  Range.dataType;
            rulesAreActive = Range.rulesAreActive;
            LOCAL_hiLimit.data_32  = Range.range.u32.max;
            LOCAL_loLimit.data_32  = Range.range.u32.min;
            LOCAL_incValue.data_32 = Range.range.u32.increment;


            if (LOCAL_dataType == ST_U32)
            {
                TUSIGN8 within_over_under = 0;

                LOCAL_variableReady = 1;

                (void)strcpy((char *)LOCAL_stringVal,(char *)AdjusterBuildNumberStringFromUSIGN32(LOCAL_adjValue.data_32,LOCAL_Attributes,&within_over_under));
                LOCAL_stringNeg = 0;    // INTEGER_PARAM cannot be less than zero so clear negative flag
            }
        }
        break;
    #endif


    #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
    case FRAME_TYPE_MANUAL_CALIBRATION:
    #endif
    case FRAME_TYPE_NUMERICAL_PARAM:


        #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
        if (LOCAL_pCurrAdjustFrame->frameType == FRAME_TYPE_MANUAL_CALIBRATION)
        {
            #ifdef APA592_SPECIAL_FEATURES_HMI
            LOCAL_Attributes.maxCharacters = 9;
            #else
            LOCAL_Attributes.maxCharacters = 6;
            #endif

            LOCAL_Attributes.characterSet_dpPosn = FrameTableGetSignal(LOCAL_pCurrAdjustFrame->frameDefinition.manualCalFrame.ProcessSignal_1)->dpPosn;

            LOCAL_variable.ssIdx = LOCAL_pCurrAdjustFrame->frameDefinition.manualCalFrame.ssidx;
            LOCAL_variable.objIdx = LOCAL_pCurrAdjustFrame->frameDefinition.manualCalFrame.objidx_targetValue;
            LOCAL_variable.attribIdx = 0;

            LOCAL_dataAdjusted = 1;     //Set data adjusted to ensure that data is written even if it not changed
        }
        else
        #endif
        {
            LOCAL_Attributes = LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.attributes;
            LOCAL_variable = LOCAL_pCurrAdjustFrame->frameDefinition.numericalParamFrame.value;
        }

        if (AdjusterGetFloatValueAndLimits((TUSIGN16)LOCAL_variable.ssIdx,
                                           (TUSIGN16)LOCAL_variable.objIdx,
                                           LOCAL_variable.attribIdx,
                                           &LOCAL_adjValue,
                                           &LOCAL_hiLimit,
                                           &LOCAL_loLimit,
                                           &LOCAL_incValue,
                                           &rulesAreActive,
                                           &LOCAL_dataType) == OK)
        {
            if ((LOCAL_dataType == ST_U8) || (LOCAL_dataType == ST_U16) || (LOCAL_dataType == ST_I16) || (LOCAL_dataType == ST_U32) || (LOCAL_dataType == ST_FLOAT)
            #ifdef HMI_USES_DOUBLE
             || (LOCAL_dataType == ST_DOUBLE)
            #endif
            )
            {
                TUSIGN8 tempString[20];
                TUSIGN8 tempStringIndex = 0;
                TUSIGN8 within_over_under = 0;

                LOCAL_variableReady = 1;

                #ifdef HMI_USES_DOUBLE
                LOCAL_AdjIsDouble = LOCAL_dataType == ST_DOUBLE ? eTRUE : eFALSE;
                #endif

                #ifdef HMI_USES_DOUBLE
                if (LOCAL_AdjIsDouble)
                {
                    // return value of "strcpy()" ignored
                    //lint -e{534} Roger Arnold 2005-1-27
                    strcpy((char *)tempString,(char *)AdjusterBuildNumberStringFromDouble(LOCAL_adjValue.data_D, LOCAL_Attributes.maxCharacters, LOCAL_Attributes.characterSet_dpPosn, &LOCAL_stringNeg, &within_over_under));
                }
                else
                #endif
                {
                    // return value of "strcpy()" ignored
                    //lint -e{534} Roger Arnold 2005-1-27
                    strcpy((char *)tempString,(char *)AdjusterBuildNumberString(LOCAL_adjValue.data_F, LOCAL_Attributes.maxCharacters, LOCAL_Attributes.characterSet_dpPosn, &LOCAL_stringNeg, &within_over_under));
                }

                while(tempString[tempStringIndex] == 0x20)
                {
                    tempString[tempStringIndex] = 0x30;
                    tempStringIndex ++;
                }
                // return value of "strcpy()" ignored
                //lint -e{534} Roger Arnold 2005-1-27
                strcpy((char *)LOCAL_stringVal,(char *)tempString);
            }
        }
        break;

        case FRAME_TYPE_ENUMERATED_PARAM:
        case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
            LOCAL_variable = LOCAL_pCurrAdjustFrame->frameDefinition.enumParamFrame.value;

            if (AdjusterGetEnumValueAndRange((TUSIGN16)LOCAL_variable.ssIdx,
                                             (TUSIGN16)LOCAL_variable.objIdx,
                                             LOCAL_variable.attribIdx,
                                             &LOCAL_adjValue,
                                             &enum8Range,
                                             #ifdef HMI_SUPPORTS_16BIT_ENUMS
                                             &enum16Range,
                                             #endif
                                             &rulesAreActive,
                                             &LOCAL_dataType) == OK)
            {
                if (LOCAL_dataType == ST_E8)
                {
                    LOCAL_pEnum8Values = enum8Range.ptrCodeTab;
                    LOCAL_nEnum8Values = enum8Range.numberOfCodes;

                    LOCAL_variableReady = 1;
                }
                #ifdef HMI_SUPPORTS_16BIT_ENUMS
                #ifdef SIZE_ST_E16
                else if ((LOCAL_dataType == ST_U16) || (LOCAL_dataType == ST_E16))
                #else
                else if (LOCAL_dataType == ST_U16)
                #endif
                {
                    LOCAL_pEnum16Values = enum16Range.ptrCodeTab;
                    LOCAL_nEnum16Values = enum16Range.numberOfCodes;

                    LOCAL_variableReady = 1;
                }
                #endif
                else
                {
                    asm("nop");
                }
            }
            break;

        case FRAME_TYPE_STRING_PARAM_PACKED:
            {
#ifdef HMI_SUPPORTS_PACKEDASCII
            TUSIGN8 len, i;
            TUSIGN8 packedtext[41] = {0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,
                                      0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,
                                      0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,
                                      0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x80,
                                      0x00};

            LOCAL_Attributes = LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.attributes;
            len = LOCAL_Attributes.maxCharacters;

            // len cannot be more than 40 characters therefore len * 6 cannot be more than 240
            //lint -e{734} Roger Arnold 2005-1-27
            i =  len * 6;
            len = i / 8;
            if (i % 8) {len ++;}


            for (i = 0; i < len; i++)
            {
                // Return value of "AdjusterGetAttribute()" ignored
                //lint -e{534} Roger Arnold 2005-1-27
                AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                     (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                     i,
                                     (void FAST *)&packedtext[i]);
            }

            AdjusterUnpackPASCII(packedtext, len, LOCAL_stringVal);

            LOCAL_variableReady = 1;
#else
            LOCAL_variableReady = 0;
#endif
            }
            break;

        case FRAME_TYPE_STRING_PARAM:
            {
                #ifdef HMI_SUPPORTS_STANDARDASCII
                TUSIGN8 len, i;

                LOCAL_Attributes = LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.attributes;
                len = LOCAL_Attributes.maxCharacters;

                if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                    && ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevel) ||
                        (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevel)))
                {
                    for (i = 0; i < len; i++)
                    {
                        LOCAL_stringVal[i] = 0x00;
                    }
                }
                else
                {
                    TUSIGN8 tempChar;

                    for (i = 0; i < len; i++)
                    {
                        AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx,
                                             (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx,
                                             i,
                                             (void FAST *)&tempChar);
                                             //&LOCAL_stringVal[i]);

                     LOCAL_stringVal[i] = tempChar;
                    }
                }
                LOCAL_variableReady = 1;
                #else
                LOCAL_variableReady = 0;
                #endif

                #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                if ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.ssIdx == HMI_IDX)
                 && ((LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevel)
                  || (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevel)
                  || (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevel)
                  || (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_StandardLevelDefault)
                  || (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_AdvancedLevelDefault)
                  || (LOCAL_pCurrAdjustFrame->frameDefinition.stringParamFrame.stringText.objIdx == HMI_IDX_SetPassword_ServiceLevelDefault)))
                {
                    // Setr flag to indicate that data has been adjusted so that password will be saved (as NULL) if data is not adjusted
                    LOCAL_dataAdjusted = 1;
                }
                #endif

            }
            break;

    	case FRAME_TYPE_TIME_AND_DATE_PARAM:
            #if defined(HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES)
    	    {
    	        TUSIGN32 value32;
                (void) AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                            (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.objIdx,
                                            LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                            (void FAST*)&value32);
                LOCAL_TimeAndDate = value32;
                LOCAL_variableReady = 1;
            }
	        #endif
	        break;
                
        case FRAME_TYPE_IP_ADDRESS:
            #if defined(HMI_SUPPORTS_IP_ADDRESS_FRAMES)
    	    {
    	        TUSIGN32 value32;
                (void) AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.ssIdx,
                                            (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.objIdx,
                                            LOCAL_pCurrAdjustFrame->frameDefinition.timeDateFrame.value.attribIdx,
                                            (void FAST*)&value32);
                #if defined(IP_ADDRESS_DATA_REVERSED) && (IP_ADDRESS_DATA_REVERSED == 1)
                    IP_ADDRESS_HMI *pIPValue32 = (IP_ADDRESS_HMI*)&value32;
                    IP_ADDRESS_HMI *pReverseIPValue32 = (IP_ADDRESS_HMI*)&LOCAL_TimeAndDate;
                    pReverseIPValue32->dotted_IP[0] = pIPValue32->dotted_IP[3];
                    pReverseIPValue32->dotted_IP[1] = pIPValue32->dotted_IP[2];
                    pReverseIPValue32->dotted_IP[2] = pIPValue32->dotted_IP[1];
                    pReverseIPValue32->dotted_IP[3] = pIPValue32->dotted_IP[0];
                #else
                    LOCAL_TimeAndDate = value32;
                #endif
                LOCAL_variableReady = 1;
            }
	        #endif
	        break;


        #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        case FRAME_TYPE_BIT_ENUMERATED_PARAM:
            {
                TUSIGN8 value8;

                (void) AdjusterGetAttribute((TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.ssIdx,
                                            (TUSIGN16)LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.objIdx,
                                            LOCAL_pCurrAdjustFrame->frameDefinition.bitEnumParamFrame.value.attribIdx,
                                            (void FAST *)&value8);

                LOCAL_adjValue.data_8 = value8;
                LOCAL_variableReady = 1;
            }
            break;
        #endif

        case FRAME_TYPE_AUTO_ADJUST:
        case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
            LOCAL_variableReady = 1;
            break;

        default:
            LOCAL_variableReady = 0;
            break;
        }
        return LOCAL_variableReady;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Writes the new value to the variable being adjusted
 \author Roger Arnold
 \date   2005-04-14
 \param  - TUSIGN16 subsysIdx   \
           TUSIGN16 objIdx       > indeces of the data
           TINT16 attribIdx     /
           void *ptrValue       pointer to the value to be written

 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterWriteAttribute(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue)
{
    TUSIGN16 returnCode;
    ATTRIB_IDENTIFIER_TYPE Data;

    Data.ssIdx     = (TUSIGN8)subsysIdx;    // identifies the sub-system that "owns" the data
    Data.objIdx    = (TUSIGN8)objIdx;       // identifies the object that "owns" the data
    Data.attribIdx = (TUSIGN8)attribIdx;    // identifies the attribute within the object that "owns" the data

    if (WriteProtect_HMICanAccess(Data,0))
    {
        T_UNIT const SLOW *pSub; // Pointer to Subsystem
        pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)subsysIdx);  // Get the pointer
        returnCode = pSub->Put(pSub,objIdx,attribIdx,ptrValue);
    }
    else
    {
        returnCode = READ_ONLY_ERR;
    }
    return returnCode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current value of the variable to be adjusted
 \author Roger Arnold
 \date   2005-04-14
 \param  - TUSIGN16 subsysIdx   \
           TUSIGN16 objIdx       > indeces of the data
           TINT16 attribIdx     /
           void *ptrValue       the value is read from the subsystem and written to this location

 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterGetAttribute(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void FAST *ptrValue)
{

    TUSIGN16 returnCode;
    T_UNIT const SLOW *pSub; // Pointer to Subsystem
    pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)subsysIdx);  // Get the pointer
    returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,ptrValue);
    return returnCode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Writes the new value to the variable being adjusted
 \author Roger Arnold
 \date   2005-04-14
 \param  - TUSIGN16 subsysIdx   \
           TUSIGN16 objIdx       > indeces of the data
           TINT16 attribIdx     /
           ADJUST_DATA *pValue - pointer to structure containing the value to be written
           TUSIGN8 data_Type    - defines the type (float int etc) of the data to be written

 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterWriteValueAndCheckLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, const ADJUST_DATA *pValue, TUSIGN8 data_Type)
{
    TUSIGN16 returnCode;

    TUSIGN8     ST_U8_value;
    TUSIGN16    ST_U16_value;
    TINT16      ST_I16_value;
    TUSIGN32    ST_U32_value;
    TFLOAT      ST_FLOAT_value;
    TUSIGN8     ST_E8_value;
#ifdef HMI_USES_DOUBLE
    TDOUBLE    ST_DOUBLE_value;
#endif

    switch (data_Type)
    {
        case ST_U8:
            ST_U8_value = (TUSIGN8)pValue->data_F;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U8_value);
            break;

        case ST_U16:
            ST_U16_value = (TUSIGN16)pValue->data_F;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U16_value);
            break;

        case ST_I16:
            ST_I16_value = (TINT16)pValue->data_F;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_I16_value);
            break;

        case ST_U32:
            ST_U32_value = (TUSIGN32)pValue->data_F;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U32_value);
            break;

        case ST_FLOAT:
            ST_FLOAT_value = (TFLOAT)pValue->data_F;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_FLOAT_value);
            break;

        case ST_E8:
            ST_E8_value = (TUSIGN8)pValue->data_8;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_E8_value);
            break;

#ifdef HMI_USES_DOUBLE
        case ST_DOUBLE:
            ST_DOUBLE_value = pValue->data_D;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_DOUBLE_value);
#endif

        default:
            returnCode = WARNING;
            break;

    }
    return returnCode;
}


#ifdef HMI_SUPPORTS_INTEGER_FRAMES
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterWriteInt32AndCheckLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, const ADJUST_DATA *pValue, TUSIGN8 data_Type)
{
    TUSIGN16 returnCode;

    TUSIGN8     ST_U8_value;
    TUSIGN16    ST_U16_value;
    TINT16      ST_I16_value;
    TUSIGN32    ST_U32_value;
    TFLOAT      ST_FLOAT_value;
    TUSIGN8     ST_E8_value;
#ifdef HMI_USES_DOUBLE
    TDOUBLE    ST_DOUBLE_value;
#endif

    switch (data_Type)
    {
        case ST_U8:
            ST_U8_value = (TUSIGN8)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U8_value);
            break;

        case ST_U16:
            ST_U16_value = (TUSIGN16)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U16_value);
            break;

        case ST_I16:
            ST_I16_value = (TINT16)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_I16_value);
            break;

        case ST_U32:
            ST_U32_value = (TUSIGN32)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_U32_value);
            break;

        case ST_FLOAT:
            ST_FLOAT_value = (TFLOAT)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_FLOAT_value);
            break;

        case ST_E8:
            ST_E8_value = (TUSIGN8)pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_E8_value);
            break;

#ifdef HMI_USES_DOUBLE
        case ST_DOUBLE:
            ST_DOUBLE_value = pValue->data_32;
            returnCode = AdjusterWriteAttribute(subsysIdx, objIdx, (TINT16)attribIdx, (void FAST *)&ST_DOUBLE_value);
#endif

        default:
            returnCode = WARNING;
            break;

    }
    return returnCode;
}

#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current value of the variable to be adjusted
 \author Roger Arnold
 \date   2005-04-14
 \param  - TUSIGN16 subsysIdx   \
           TUSIGN16 objIdx       > indeces of the data
           TINT16 attribIdx     /
           ADJUST_DATA *pValue - pointer to structure within which the value read from the subsystem will be written
           TUSIGN8 *pType       - the value type (float int etc) is read from the subsystem and written to this location


 \return - T_DATA_OBJ_RETURNCODES
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this function as it is a wrapper for AdjusterGetValueAndLimits()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN16 AdjusterGetFloatValue(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, TUSIGN8 *pType)
{
    ADJUST_DATA High,Low,Inc;
    TBOOL       rulesAreActive;

    return AdjusterGetFloatValueAndLimits(subsysIdx,
                                          objIdx,
                                          attribIdx,
                                          pValue,

                                          &High,
                                          &Low,
                                          &Inc,
                                          &rulesAreActive,

                                          pType);
}

TUSIGN16 AdjusterGetEnumValue(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, TUSIGN8 *pType)
{
    T_RANGE_E8      Enum8Range;
    #ifdef HMI_SUPPORTS_16BIT_ENUMS
    T_RANGE_E16     Enum16Range;
    #endif
    TBOOL       rulesAreActive;

    return AdjusterGetEnumValueAndRange(subsysIdx,
                                          objIdx,
                                          attribIdx,
                                          pValue,

                                          &Enum8Range,
    #ifdef HMI_SUPPORTS_16BIT_ENUMS
                                          &Enum16Range,
    #endif
                                          &rulesAreActive,
                                          pType);
}

//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterGetFloatValueAndLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, ADJUST_DATA *pHigh, ADJUST_DATA *pLow, ADJUST_DATA *pInc, TBOOL* pRulesAreActive, TUSIGN8 *pType)
{
    TUSIGN16 returnCode;
    T_UNIT const SLOW *pSub; // Pointer to Subsystem
    T_DO_ATTRIB_RANGE Range; // Pointer to range information

    TUSIGN8     ST_U8_value;
    TUSIGN16    ST_U16_value;
    TINT16      ST_I16_value;
    TUSIGN32    ST_U32_value;
    TFLOAT      ST_FLOAT_value;
#ifdef HMI_USES_DOUBLE
    TDOUBLE    ST_DOUBLE_value;
#endif

    pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)subsysIdx);
    returnCode = pSub->GetAttributeDescription(pSub,objIdx,(TUSIGN16)attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range);

    if (returnCode == OK)
    {
        *pType = Range.dataType;
        *pRulesAreActive = Range.rulesAreActive;

        switch (Range.dataType)
        {
            case ST_U8:
                pHigh->data_F = (float)(Range.range.u8.max);
                pLow->data_F  = (float)(Range.range.u8.min);
                pInc->data_F  = (float)(Range.range.u8.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_U8_value);
                pValue->data_F = (float)ST_U8_value;
                break;

            case ST_U16:
                pHigh->data_F = (float)(Range.range.u16.max);
                pLow->data_F  = (float)(Range.range.u16.min);
                pInc->data_F  = (float)(Range.range.u16.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_U16_value);
                pValue->data_F = (float)ST_U16_value;
                break;

            case ST_I16:
                pHigh->data_F = (float)(Range.range.i16.max);
                pLow->data_F  = (float)(Range.range.i16.min);
                pInc->data_F  = (float)(Range.range.i16.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_I16_value);
                pValue->data_F = (float)ST_I16_value;
                break;

            case ST_U32:
                pHigh->data_F = (float)(Range.range.u32.max);
                pLow->data_F  = (float)(Range.range.u32.min);
                pInc->data_F  = (float)(Range.range.u32.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_U32_value);
                pValue->data_F = (float)ST_U32_value;
                break;

            case ST_FLOAT:
                pHigh->data_F = (float)(Range.range.flt.max);
                pLow->data_F  = (float)(Range.range.flt.min);
                pInc->data_F  = (float)(Range.range.flt.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_FLOAT_value);
                pValue->data_F = (float)ST_FLOAT_value;
                break;

#ifdef HMI_USES_DOUBLE
        case ST_DOUBLE:
                pHigh->data_D = (double)(Range.range.dbl.max);
                pLow->data_D  = (double)(Range.range.dbl.min);
                pInc->data_D  = (double)(Range.range.dbl.increment);
                returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&ST_DOUBLE_value);
                pValue->data_D = (double)ST_DOUBLE_value;
                break;
#endif

            default:
                pHigh->data_F = 0.0F;
                pLow->data_F  = 1.0F;
                pInc->data_F  = 1.0F;
                pValue->data_F = 0.0F;
                returnCode = WARNING;
                break;
        }
    }
    return (returnCode);
}

//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN16 AdjusterGetEnumValueAndRange(TUSIGN16 subsysIdx,
                                      TUSIGN16 objIdx,
                                      TUSIGN8 attribIdx,
                                      ADJUST_DATA *pValue,
                                      T_RANGE_E8* pEnum8Range,
#ifdef HMI_SUPPORTS_16BIT_ENUMS
                                      T_RANGE_E16* pEnum16Range,
#endif
                                      TBOOL* pRulesAreActive,
                                      TUSIGN8 *pType)
{
    TUSIGN16 returnCode;
    T_UNIT const SLOW *pSub; // Pointer to Subsystem
    T_DO_ATTRIB_RANGE Range; // Pointer to range information


    pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)subsysIdx);
    returnCode = pSub->GetAttributeDescription(pSub,objIdx,(TUSIGN16)attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range);

    if (returnCode == OK)
    {
        if (Range.dataType == ST_E8)
        {
            TUSIGN8     value_8bit;
            VIP_ASSERT(Range.range.e8.ptrCodeTab);
            returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&value_8bit);
            pValue->data_8 = value_8bit;
            pEnum8Range->ptrCodeTab = Range.range.e8.ptrCodeTab;
            pEnum8Range->numberOfCodes = Range.range.e8.numberOfCodes;

            #ifdef HMI_SUPPORTS_16BIT_ENUMS
            pEnum16Range->ptrCodeTab = NULL;
            pEnum16Range->numberOfCodes = 0;
            #endif

            *pType = ST_E8;
            *pRulesAreActive = Range.rulesAreActive;

        }
        #ifdef HMI_SUPPORTS_16BIT_ENUMS
        #ifdef SIZE_ST_E16
        else if ((Range.dataType == ST_U16) || (Range.dataType == ST_E16))
        #else
        else if (Range.dataType == ST_U16)
        #endif
        {
            TUSIGN16    value_16bit;
            VIP_ASSERT(Range.range.e16.ptrCodeTab);
            returnCode = pSub->Get(pSub,objIdx,(TINT16)attribIdx,(void FAST *)&value_16bit);
            pValue->data_16 = value_16bit;
            pEnum16Range->ptrCodeTab = Range.range.e16.ptrCodeTab;
            pEnum16Range->numberOfCodes = Range.range.e16.numberOfCodes;

            pEnum8Range->ptrCodeTab = NULL;
            pEnum8Range->numberOfCodes = 0;

            *pType = Range.dataType;
            *pRulesAreActive = Range.rulesAreActive;

        }
        #endif
        else
        {
            pEnum8Range->ptrCodeTab = NULL;
            pEnum8Range->numberOfCodes = 0;

            #ifdef HMI_SUPPORTS_16BIT_ENUMS
            pEnum16Range->ptrCodeTab = NULL;
            pEnum16Range->numberOfCodes = 0;
            #endif

            *pType = ST_NIL;
            *pRulesAreActive = eFALSE;

            returnCode = WARNING;
        }
    }
    return returnCode;
}

#ifdef HMI_SUPPORTS_INTEGER_FRAMES

const TUSIGN32 divisorArray[9] =
{
    100000000,
    10000000,
    1000000,
    100000,
    10000,
    1000,
    100,
    10,
    1
};


TUSIGN8 *AdjusterBuildNumberStringFromUSIGN32(TUSIGN32 Value32, NUMBER_ATTRIBUTES_TYPE Attributes, TUSIGN8* pWithin_over_under)
{
    TUSIGN8 stringIndex = 9;
    TUSIGN8 i;

    TUSIGN8 dp_posn = Attributes.characterSet_dpPosn;

    // Calculate number of decimal places to display
    AdjusterAdjustResolutionForCustomDP(&dp_posn);
    if (dp_posn == 0xFF)
    {
        dp_posn = 0;
    }

    // Calculate Max value that can be displayed (based upon maxCharacters
    TUSIGN32 MaxVal = 0;
    switch (Attributes.maxCharacters)
    {
    case 0:     MaxVal = 0;             break;
    case 1:     MaxVal = 9;             break;
    case 2:     MaxVal = 99;            break;
    case 3:     MaxVal = 999;           break;
    case 4:     MaxVal = 9999;          break;
    case 5:     MaxVal = 99999;         break;
    case 6:     MaxVal = 999999;        break;
    case 7:     MaxVal = 9999999;       break;
    case 8:     MaxVal = 99999999;      break;
    case 9:     MaxVal = 999999999;     break;
    default:    MaxVal = 0xFFFFFFFF;    break;
    }

    // Apply Limit
    if (Value32 > MaxVal)
    {
        Value32 = MaxVal;
        *pWithin_over_under = 1;
    }

    // Build String
    stringIndex = 4 + dp_posn;

    for (i = 0; i < 9; i ++)
    {
        LOCAL_m_number[stringIndex] = (Value32 / divisorArray[i]) + '0';

        Value32 =  Value32 % divisorArray[i];
        stringIndex++;

        if (stringIndex == 13)
        {
            LOCAL_m_number[stringIndex] = '.';
            stringIndex++;
        }
    }

    // Calculate terminator position
    stringIndex = dp_posn == 0 ? 13 : dp_posn + 14;

    // add terminator
    LOCAL_m_number[stringIndex] = 0x00;

    // calculate position of most significant displayed digit
    stringIndex -= Attributes.maxCharacters;

    if (dp_posn != 0)
    {
        stringIndex--;
    }
    return &LOCAL_m_number[stringIndex];
}

void BuildUSIGN32FromString(TUSIGN32 *pUSIGN32Val)
{
    TUSIGN8     i = 0;
    TUSIGN32    IntVal = 0;

    while (LOCAL_stringVal[i] != 0x00)
    {
        if ((LOCAL_stringVal[i] >= '0') && (LOCAL_stringVal[i] <= '9'))
        {
            IntVal *= 10;
            IntVal += (LOCAL_stringVal[i] - '0');
        }
        i ++;
    }
    *pUSIGN32Val = IntVal;
}


#endif

#ifdef HMI_USES_DOUBLE
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a double value to a string
 \author Roger Arnold
 \date   2005-04-14
 \param  - double value                 - the value to be converted
           TUSIGN8 len                  - The maximum number of characters allowed in the resultant string
           TUSIGN8 resolution           - The number of dec places to which the number should be calculated
           TUSIGN8 *pNegVal             - if the value to be converted is <0 then 1 is written to this location
                                          if the value to be converted is >=0 then 0 is written to this location
           TUSIGN8 *pWithin_over_under  - if the value to be converted is too high to fit into a string of length 'len' then 1 is written to this location
                                          if the value to be converted is too low to fit into a string of length 'len' then 2 is written to this location
                                          if the value to be converted fits into a string of length 'len' then 0 is written to this location


 \return - TUSIGN8* pointer to resultant string
 \warning
 \test
         test-date    :
         by           :
         environment  :
         result       :
 \test   Lint level 3 :
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN8 *AdjusterBuildNumberStringFromDouble(TDOUBLE value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under)
{
    int x3, x2, x1, x_1, x_2, x_3, x_4;
    TUSIGN8 i,returnOffset;
    TDOUBLE limitL,limitH;

    if (len > 12)
    {
        len = 12;
    }

    // --------------------------------------------------------------------------------------------
    // 2. If a configuarable DP position has been selected then get the actual DP position
    AdjusterAdjustResolutionForCustomDP(&resolution);

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
        if      (value <= 9.9f)            /*   1 digit before dp  */  {i = 1;}
        else if (value <= 99.9f)           /*   2 digits before dp */  {i = 2;}
        else if (value <= 999.9f)          /*   3 digits before dp */  {i = 3;}
        else if (value <= 9999.9f)         /*   4 digits before dp */  {i = 4;}
        else if (value <= 99999.9f)        /*   5 digits before dp */  {i = 5;}
        else if (value <= 999999.9f)       /*   6 digits before dp */  {i = 6;}
        else if (value <= 9999999.9f)      /*   7 digits before dp */  {i = 7;}
        else if (value <= 99999999.9f)     /*   8 digits before dp */  {i = 8;}
        else if (value <= 999999999.9f)    /*   9 digits before dp */  {i = 9;}
        else if (value <= 9999999999.9f)   /*  10 digits before dp */  {i = 10;}
        else if (value <= 99999999999.9f)  /*  11 digits before dp */  {i = 11;}
        else if (value <= 999999999999.9f) /*  12 digits before dp */  {i = 12;}
        else                               /* >12 digits before dp */  {i = 13;}

        if (len <= i)
        {
            resolution = 0;
        }
        else
        {
            resolution = len - (i + 1);
        }

    }

    // --------------------------------------------------------------------------------------------
    // 5. Limit resolution
    if (resolution > 10)
    {
        resolution = 10;
    }

    while (resolution > (len - 2) && resolution != 0)
    {
        resolution --;
    }

    // --------------------------------------------------------------------------------------------
    // 6. Calculate high limit
    limitH = (TDOUBLE)limitArray[len][resolution];

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

    LOCAL_m_number[0] = ' ';

    // Xxxxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[1] = ((TUSIGN8)(x3 / 1000)) + 0x30;
    x3 %= 1000;

    // xXxxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[2] = ((TUSIGN8)(x3 / 100)) + 0x30;
    x3 %= 100;

    // xxXxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[3] = ((TUSIGN8)(x3 / 10)) + 0x30;
    x3 %= 10;

    // xxxXxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[4] = ((TUSIGN8)(x3)) + 0x30;

    // xxxxXxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[5] = ((TUSIGN8)(x2 / 1000)) + 0x30;
    x2 %= 1000;

    // xxxxxXxxxxxx.xxxxxxxxxx
    LOCAL_m_number[6] = ((TUSIGN8)(x2 / 100)) + 0x30;
    x2 %= 100;

    // xxxxxxXxxxxx.xxxxxxxxxx
    LOCAL_m_number[7] = ((TUSIGN8)(x2 / 10)) + 0x30;
    x2 %= 10;

    // xxxxxxxXxxxx.xxxxxxxxxx
    LOCAL_m_number[8] = ((TUSIGN8)(x2)) + 0x30;

    // xxxxxxxxXxxx.xxxxxxxxxx
    LOCAL_m_number[9] = ((TUSIGN8)(x1 / 1000)) + 0x30;
    x1 %= 1000;

    // xxxxxxxxxXxx.xxxxxxxxxx
    LOCAL_m_number[10] = ((TUSIGN8)(x1 / 100)) + 0x30;
    x1 %= 100;

    // xxxxxxxxxxXx.xxxxxxxxxx
    LOCAL_m_number[11] = ((TUSIGN8)(x1 / 10)) + 0x30;
    x1 %= 10;

    // xxxxxxxxxxxX.xxxxxxxxxx
    LOCAL_m_number[12] = ((TUSIGN8)(x1)) + 0x30;

    LOCAL_m_number[13] = '.';

    // xxxxxxxxxxxx.Xxxxxxxxxx
    LOCAL_m_number[14] = ((TUSIGN8)(x_1 / 10)) + 0x30;
    x_1 %= 10;

    // xxxxxxxxxxxx.xXxxxxxxxx
    LOCAL_m_number[15] = ((TUSIGN8)(x_1)) + 0x30;

    // xxxxxxxxxxxx.xxXxxxxxxx
    LOCAL_m_number[16] = ((TUSIGN8)(x_2 / 1000)) + 0x30;
    x_2 %= 1000;

    // xxxxxxxxxxxx.xxxXxxxxxx
    LOCAL_m_number[17] = ((TUSIGN8)(x_2 / 100)) + 0x30;
    x_2 %= 100;

    // xxxxxxxxxxxx.xxxxXxxxxx
    LOCAL_m_number[18] = ((TUSIGN8)(x_2 / 10)) + 0x30;
    x_2 %= 10;

    // xxxxxxxxxxxx.xxxxxXxxxx
    LOCAL_m_number[19] = ((TUSIGN8)(x_2)) + 0x30;

    // xxxxxxxxxxxx.xxxxxxXxxx
    LOCAL_m_number[20] = ((TUSIGN8)(x_3 / 1000)) + 0x30;
    x_3 %= 1000;

    // xxxxxxxxxxxx.xxxxxxxXxx
    LOCAL_m_number[21] = ((TUSIGN8)(x_3 / 100)) + 0x30;
    x_3 %= 100;

    // xxxxxxxxxxxx.xxxxxxxxXx
    LOCAL_m_number[22] = ((TUSIGN8)(x_3 / 10)) + 0x30;
    x_3 %= 10;

    // xxxxxxxxxxxx.xxxxxxxxxX
    LOCAL_m_number[23] = ((TUSIGN8)(x_3)) + 0x30;


    i = 1;
    while (LOCAL_m_number[i] == 0x30 && i < 12)
    {
        LOCAL_m_number[i] = 0x20;
        i ++;
    }

    // --------------------------------------------------------------------------------------------
    // 11. Terminate string and return the address of the most significant digit
    if (resolution == 0)
    {
        LOCAL_m_number[13] = 0;                 // terminate string at dp position
        returnOffset = 13 - len;                // start of displayed string is "dp position" - "length"
    }
    else
    {
        i = resolution + 14;
        LOCAL_m_number[i] = 0;                  // terminate string after "least significant digit"
        returnOffset = i - len;                 // start of displayed string is "least significant digit" - "length"
    }
    return &LOCAL_m_number[returnOffset];
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a string of numbers to a double value
 \author Roger Arnold
 \date   2005-04-14
 \param  - double *pDoubleVal - the resultant float value is written to this location

        - NOT STRICTLY PARAMETERS OF THIS FUNCTION
        string[]  - local variable that contains the string to be converted
        stringNeg - local variable that defines if the value is >0 (i.e. -ve)


 \return - none
 \warning
 \test
         test-date    :
         by           :
         environment  :
         result       :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
void BuildDoubleFromString(TDOUBLE* pDoubleVal)
{
    TDOUBLE val = 0;
    TUSIGN8 i;
    TINT16 dpFound = 0;

    val = 0;
    for (i = 0; i < strlen((char *)LOCAL_stringVal); )
    {
        if (LOCAL_stringVal[i] != '.')
        {
            val += LOCAL_stringVal[i] - 0x30;
            i ++;

            if (((LOCAL_stringVal[i] == '.') || (dpFound)) && (LOCAL_stringVal[i] != 0x00))
            {
                dpFound++;
            }

            val *= 10;
        }
        else
        {
            i ++;
        }
    }

    while (dpFound >= 0)
    {
        val /= 10;
        dpFound --;
    }

    if (LOCAL_stringVal[0] == '-')  {val *= -1;}

    if (LOCAL_stringNeg)            {val *= -1;}

    *pDoubleVal = val;
}
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a float value to a string
 \author Roger Arnold
 \date   2005-04-14
 \param  - float value                  - the value to be converted
           TUSIGN8 len                  - The maximum number of characters allowed in the resultant string
           TUSIGN8 resolution           - The number of dec places to which the number should be calculated
           TUSIGN8 *pNegVal             - if the value to be converted is <0 then 1 is written to this location
                                          if the value to be converted is >=0 then 0 is written to this location
           TUSIGN8 *pWithin_over_under  - if the value to be converted is too high to fit into a string of length 'len' then 1 is written to this location
                                          if the value to be converted is too low to fit into a string of length 'len' then 2 is written to this location
                                          if the value to be converted fits into a string of length 'len' then 0 is written to this location


 \return - TUSIGN8* pointer to resultant string
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug

*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8 *AdjusterBuildNumberString(TFLOAT value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under)
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


  TFLOAT testNAN = value;

  if (((*((TUSIGN32 *) &testNAN) & 0x7f800000) == 0x7f800000)
   && ((*((TUSIGN32 *) &testNAN) & 0x007fffff) != 0))
  {
    TUSIGN8 nanLen = len > 6 ? 6: len;

    for (i = 0; i < nanLen; i ++)
    {
        LOCAL_m_number[i] = '-';
    }
    LOCAL_m_number[i] = 0x00;

    returnOffset = 0;
    *pNegVal = 0;
    *pWithin_over_under = 0;
  }
  else
  {
    // --------------------------------------------------------------------------------------------
    // 2. If a configuarable DP position has been selected then get the actual DP position
    AdjusterAdjustResolutionForCustomDP(&resolution);

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
    // 4. If maximum resolution has been selected then calculate the actual DP position
    //    according to length and absolute value
    if (resolution == 0xFF)
    {
        if      (value <= 9.9f)            /*   1 digit before dp  */  {i = 1;}
        else if (value <= 99.9f)           /*   2 digits before dp */  {i = 2;}
        else if (value <= 999.9f)          /*   3 digits before dp */  {i = 3;}
        else if (value <= 9999.9f)         /*   4 digits before dp */  {i = 4;}
        else if (value <= 99999.9f)        /*   5 digits before dp */  {i = 5;}
        else if (value <= 999999.9f)       /*   6 digits before dp */  {i = 6;}
        else if (value <= 9999999.9f)      /*   7 digits before dp */  {i = 7;}
        else if (value <= 99999999.9f)     /*   8 digits before dp */  {i = 8;}
        else if (value <= 999999999.9f)    /*   9 digits before dp */  {i = 9;}
        else if (value <= 9999999999.9f)   /*  10 digits before dp */  {i = 10;}
        else if (value <= 99999999999.9f)  /*  11 digits before dp */  {i = 11;}
        else if (value <= 999999999999.9f) /*  12 digits before dp */  {i = 12;}
        else                               /* >12 digits before dp */  {i = 13;}

        if (len <= i)
        {
            resolution = 0;
        }
        else
        {
            resolution = len - (i + 1);
        }
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
    limitH = limitArray[len][resolution];

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

#if defined (LONGEST_NUMBER_IS_12_DIGITS)||defined (LONGEST_NUMBER_IS_8_DIGITS)
    // Extract 7 significant digits (max resolution of float value)
    if      (x3 >= 1000)    {x2  = (TUSIGN16)(10.0f   * (TUSIGN16)(x2  / 10.0f));       x1 = 0; x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x3 >= 100)     {                                                           x1 = 0; x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x3 >= 10)      {x1  = (TUSIGN16)(1000.0f * (TUSIGN16)(x1  / 1000.0f));             x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x3 >= 1)       {x1  = (TUSIGN16)(100.0f  * (TUSIGN16)(x1  / 100.0f));              x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x2 >= 1000)    {x1  = (TUSIGN16)(10.0f   * (TUSIGN16)(x1  / 10.0f));               x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x2 >= 100)     {                                                                   x_1 = 0; x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x2 >= 10)      {x_1 = (TUSIGN16)(10.0f   * (TUSIGN16)(x_1 / 10.0f));                        x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x2 >= 1)       {                                                                            x_2 = 0; x_3 = 0; x_4 = 0;}
    else if (x1 >= 1000)    {x_2 = (TUSIGN16)(1000.0f * (TUSIGN16)(x_2 / 1000.0f));                               x_3 = 0; x_4 = 0;}
    else if (x1 >= 100)     {x_2 = (TUSIGN16)(100.0f  * (TUSIGN16)(x_2 / 100.0f));                                x_3 = 0; x_4 = 0;}
    else if (x1 >= 10)      {x_2 = (TUSIGN16)(10.0f   * (TUSIGN16)(x_2 / 10.0f));                                 x_3 = 0; x_4 = 0;}
    else if (x1 >= 1)       {                                                                                     x_3 = 0; x_4 = 0;}
    else if (x_1 >= 10)     {x_3 = (TUSIGN16)(1000.0f * (TUSIGN16)(x_3 / 1000.0f));                                        x_4 = 0;}
    else if (x_1 >= 1)      {x_3 = (TUSIGN16)(100.0f  * (TUSIGN16)(x_3 / 100.0f));                                         x_4 = 0;}
    else if (x_2 >= 1000)   {x_3 = (TUSIGN16)(10.0f   * (TUSIGN16)(x_3 / 10.0f));                                          x_4 = 0;}
    else if (x_2 >= 100)    {                                                                                              x_4 = 0;}
    else if (x_2 >= 10)     {x_4 = (TUSIGN16)(1000.0f * (TUSIGN16)(x_4 / 1000.0f));                                                }
    else if (x_2 >= 1)      {x_4 = (TUSIGN16)(100.0f  * (TUSIGN16)(x_4 / 100.0f));                                                 }
    else if (x_3 >= 1000)   {x_4 = (TUSIGN16)(10.0f   * (TUSIGN16)(x_4 / 10.0f));                                                  }
    else                    {                                                                                                      }
#endif

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
    default:    VIP_ASSERT(0);      break;
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


    LOCAL_m_number[0] = ' ';

#if defined(LONGEST_NUMBER_IS_12_DIGITS)
    // Xxxxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[1] = ((TUSIGN8)(x3 / 1000)) + 0x30;
    x3 %= 1000;

    // xXxxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[2] = ((TUSIGN8)(x3 / 100)) + 0x30;
    x3 %= 100;

    // xxXxxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[3] = ((TUSIGN8)(x3 / 10)) + 0x30;
    x3 %= 10;

    // xxxXxxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[4] = ((TUSIGN8)(x3)) + 0x30;
#endif

#if defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)
    // xxxxXxxxxxxx.xxxxxxxxxx
    LOCAL_m_number[5] = ((TUSIGN8)(x2 / 1000)) + 0x30;
    x2 %= 1000;

    // xxxxxXxxxxxx.xxxxxxxxxx
    LOCAL_m_number[6] = ((TUSIGN8)(x2 / 100)) + 0x30;
    x2 %= 100;

    // xxxxxxXxxxxx.xxxxxxxxxx
    LOCAL_m_number[7] = ((TUSIGN8)(x2 / 10)) + 0x30;
    x2 %= 10;

    // xxxxxxxXxxxx.xxxxxxxxxx
    LOCAL_m_number[8] = ((TUSIGN8)(x2)) + 0x30;
#endif

    // xxxxxxxxXxxx.xxxxxxxxxx
    LOCAL_m_number[9] = ((TUSIGN8)(x1 / 1000)) + 0x30;
    x1 %= 1000;

    // xxxxxxxxxXxx.xxxxxxxxxx
    LOCAL_m_number[10] = ((TUSIGN8)(x1 / 100)) + 0x30;
    x1 %= 100;

    // xxxxxxxxxxXx.xxxxxxxxxx
    LOCAL_m_number[11] = ((TUSIGN8)(x1 / 10)) + 0x30;
    x1 %= 10;

    // xxxxxxxxxxxX.xxxxxxxxxx
    LOCAL_m_number[12] = ((TUSIGN8)(x1)) + 0x30;

    LOCAL_m_number[13] = '.';

    // xxxxxxxxxxxx.Xxxxxxxxxx
    LOCAL_m_number[14] = ((TUSIGN8)(x_1 / 10)) + 0x30;
    x_1 %= 10;

    // xxxxxxxxxxxx.xXxxxxxxxx
    LOCAL_m_number[15] = ((TUSIGN8)(x_1)) + 0x30;

#if defined(LONGEST_NUMBER_IS_12_DIGITS) || defined(LONGEST_NUMBER_IS_8_DIGITS)
    // xxxxxxxxxxxx.xxXxxxxxxx
    LOCAL_m_number[16] = ((TUSIGN8)(x_2 / 1000)) + 0x30;
    x_2 %= 1000;

    // xxxxxxxxxxxx.xxxXxxxxxx
    LOCAL_m_number[17] = ((TUSIGN8)(x_2 / 100)) + 0x30;
    x_2 %= 100;

    // xxxxxxxxxxxx.xxxxXxxxxx
    LOCAL_m_number[18] = ((TUSIGN8)(x_2 / 10)) + 0x30;
    x_2 %= 10;

    // xxxxxxxxxxxx.xxxxxXxxxx
    LOCAL_m_number[19] = ((TUSIGN8)(x_2)) + 0x30;
#endif

#if defined(LONGEST_NUMBER_IS_12_DIGITS)
    // xxxxxxxxxxxx.xxxxxxXxxx
    LOCAL_m_number[20] = ((TUSIGN8)(x_3 / 1000)) + 0x30;
    x_3 %= 1000;

    // xxxxxxxxxxxx.xxxxxxxXxx
    LOCAL_m_number[21] = ((TUSIGN8)(x_3 / 100)) + 0x30;
    x_3 %= 100;

    // xxxxxxxxxxxx.xxxxxxxxXx
    LOCAL_m_number[22] = ((TUSIGN8)(x_3 / 10)) + 0x30;
    x_3 %= 10;

    // xxxxxxxxxxxx.xxxxxxxxxX
    LOCAL_m_number[23] = ((TUSIGN8)(x_3)) + 0x30;
#endif
    // --------------------------------------------------------------------------------------------
    // Blank leading zeros


    #if defined(LONGEST_NUMBER_IS_4_DIGITS)
    i = 9;
    #elif defined(LONGEST_NUMBER_IS_8_DIGITS)
    i = 5;
    #else
    i = 1;
    #endif

    while ((LOCAL_m_number[i] == 0x30) && (i < 12))
    {
        LOCAL_m_number[i] = 0x20;
        i ++;
    }

    // --------------------------------------------------------------------------------------------
    // 11. Terminate string and return the address of the most significant digit
    if (resolution == 0)
    {
        LOCAL_m_number[13] = 0;                 // terminate string at dp position
        returnOffset = 13 - len;                // start of displayed string is "dp position" - "length"
    }
    else
    {
        i = resolution + 14;
        LOCAL_m_number[i] = 0;                  // terminate string after "least significant digit"
        returnOffset = i - len;                 // start of displayed string is "least significant digit" - "length"
    }
  }
  return &LOCAL_m_number[returnOffset];
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a string of numbers to a float value
 \author Roger Arnold
 \date   2005-04-14
 \param  - float *pFloatVal - the resultant float value is written to this location

        - NOT STRICTLY PARAMETERS OF THIS FUNCTION
        string[]  - local variable that contains the string to be converted
        stringNeg - local variable that defines if the value is >0 (i.e. -ve)


 \return - none
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
void BuildFloatFromString(TFLOAT* pFloatVal)
{
    TUSIGN8 c = 0;

    // Remove any leading spaces
    while (LOCAL_stringVal[c] == ' ')
    {
        c++;
    }

    // Determine if number has polarity sign
    TUSIGN8 sign = LOCAL_stringNeg ? 1 : 0;
    if (LOCAL_stringVal[c] == '-')
    {
        c++;
        sign = 1;
    }
    else if (LOCAL_stringVal[c] == '+')
    {
        c++;
    }
    else
    {
        // Do nothing
    }

    // Get integer part of number
    TFLOAT val = 0;
    while ((LOCAL_stringVal[c] >= '0') && (LOCAL_stringVal[c] <= '9'))
    {
        val = val * 10 + (LOCAL_stringVal[c] - '0');
        c++;
    }

    // Get fractional part of number
    TFLOAT power = 1;
    if (LOCAL_stringVal[c] == '.')
    {
        c++;
        while ((LOCAL_stringVal[c] >= '0') && (LOCAL_stringVal[c] <= '9'))
        {
            val = val * 10 + (LOCAL_stringVal[c] - '0');
            power = power * 10;
            c++;
        }
    }

    // Apply divider
    val /= power;

    // Set float value
    *pFloatVal = sign ? -val : val;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Returns the address of the variable currently controlled by the Adjuster class in string format
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 *pNegVal - if the variable currently controlled by the Adjuster is <0 then 1 is written to this location
                            if the variable currently controlled by the Adjuster is >=0 then 0 is written to this location

 \return - TUSIGN8* pointer to resultant string - Address of the data (in string format)
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8* AdjusterGetVariableAsString(TUSIGN8 *pNegVal)
{
    *pNegVal = LOCAL_stringNeg;
    return &LOCAL_stringVal[0];
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Returns the value of the ENUM variable currently controlled by the Adjuster class
 \author Roger Arnold
 \date   2005-04-14
 \param  None
 \return TUSIGN8 the enum variable currently controlled by the Adjuster class
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN16 AdjusterGetEnumVariable(TUSIGN8 *nBits)
{
    TUSIGN16 retVal;

    #ifdef HMI_SUPPORTS_16BIT_ENUMS
    #ifdef SIZE_ST_E16
    if ((LOCAL_dataType == ST_U16) || (LOCAL_dataType == ST_E16))
    #else
    if (LOCAL_dataType == ST_U16)
    #endif
    {
        retVal = LOCAL_adjValue.data_16;
        *nBits = 16;
    }
    else
    #endif
    {
        retVal = (TUSIGN16)LOCAL_adjValue.data_8;
        *nBits = 8;
    }

    return retVal;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Returns the adjustment state of the data currently controlled by the Adjuster class
 \author Roger Arnold
 \date   2005-04-14
 \param  None
 \return TUSIGN8 0 - the variable currently controlled by the Adjuster class has not been adjusted
 \               1 - the variable currently controlled by the Adjuster class has been adjusted
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8 AdjusterIsDataAdjusted(void)
{
    return LOCAL_dataAdjusted;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Resets the adjustment state of the data currently controlled by the Adjuster class
 \author Roger Arnold
 \date   2005-04-14
 \param  None
 \return None
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
void AdjusterResetDataAdjusted(void)
{
    LOCAL_dataAdjusted = 0;
}

#ifdef HMI_SUPPORTS_PACKEDASCII
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a standard ascii string into a packed ascii string
 \author Roger Arnold
 \date   2005-04-14
 \param  const TUSIGN8 *pUnpackedStr    - pointer to the standard string
         TUSIGN8 unpackedLen            - the length of the unpacked string
         TUSIGN8 *pPackedStr            - the resultant packed ascii string is written to this location
 \return None
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
void AdjusterPackASCII(const TUSIGN8 *pUnpackedStr, TUSIGN8 unpackedLen, TUSIGN8 *pPackedStr)
{
    TUSIGN8 multiplesOfFour = unpackedLen/4;
    TUSIGN8 remainder = unpackedLen%4;
    TUSIGN8 unpackedOffset=0;
    TUSIGN8 packedIndex=0;
    TUSIGN8 unpackedChar;
    TUSIGN8 t;

    while(multiplesOfFour || remainder)
    {
        if( ((multiplesOfFour==0) && (remainder)) || (multiplesOfFour))
        {
            t = unpackedOffset;
            unpackedChar = pUnpackedStr[t];
            if ((unpackedChar < 0x20) || (unpackedChar > 0x5F))
            {
                unpackedChar = 0x3F;
            }
            // 543210 (unpacked byte0) -> 765432xx (packed byte0)
            pPackedStr[packedIndex] = ((unpackedChar & 0x3F) << 2);
            if(multiplesOfFour==0){remainder--;}
         }

        if( ((multiplesOfFour==0) && (remainder)) || (multiplesOfFour))
        {
            t = unpackedOffset + 1;
            unpackedChar = pUnpackedStr[t];
            if ((unpackedChar < 0x20) || (unpackedChar > 0x5F))
            {
                unpackedChar = 0x3F;
            }

            // 54 (unpacked byte1) -> xxxxxx10 (packed byte0)
            pPackedStr[packedIndex] |= ((unpackedChar&0x30) >> 4);
            packedIndex++;
            // 3210 (unpacked byte1) -> 7654xxxx (packed byte1)
            pPackedStr[packedIndex] = ((unpackedChar&0x0F) << 4);
            if(multiplesOfFour==0) {remainder--;}
        }

        if( ((multiplesOfFour==0) && (remainder)) || (multiplesOfFour))
        {
            t = unpackedOffset + 2;
            unpackedChar = pUnpackedStr[t];
            if ((unpackedChar < 0x20) || (unpackedChar > 0x5F))
            {
                unpackedChar = 0x3F;
            }

            // 5432 (unpacked byte2) -> xxxx3210 (packed byte1)
            pPackedStr[packedIndex] |= ((unpackedChar&0x3C) >> 2);
            packedIndex++;

            // 10 (unpacked byte2) -> 76xxxxxx (packed byte2)
            pPackedStr[packedIndex] = ((unpackedChar&0x03) << 6);
            if(multiplesOfFour==0) {remainder--;}
         }

        if(multiplesOfFour)
        {
            t = unpackedOffset + 3;
            unpackedChar = pUnpackedStr[t];
            if ((unpackedChar < 0x20) || (unpackedChar > 0x5F))
            {
                unpackedChar = 0x3F;
            }

            // 543210 (unpacked byte3) -> xx543210 (packed byte2)
            pPackedStr[packedIndex] |= ( unpackedChar&0x3F);
            packedIndex++;
        }

        unpackedOffset += 4;
        if(multiplesOfFour) {multiplesOfFour--;}
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a packed ascii string into a standard ascii string
 \author Roger Arnold
 \date   2005-04-14
 \param  const TUSIGN8 *pPackedStr    - pointer to the Packed ascii string
         TUSIGN8 unpackedLen          - the length of the Packed ascii string
         TUSIGN8 *pPackedStr          - the resultant standard ascii string is written to this location
 \return None
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - Adjuster_ModuleTest()
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
void AdjusterUnpackPASCII(const TUSIGN8 *pPackedStr, TUSIGN8 packedLen, TUSIGN8 *pUnpackedStr)
{
    TUSIGN8 multiplesOfThree = packedLen/3;
    TUSIGN8 remainder = packedLen%3;
    TUSIGN8 unpackedIndex=0;
    TUSIGN8 packedOffset=0;
    TUSIGN8 t0,t1;

    while(multiplesOfThree || remainder)
    {
        if( ((multiplesOfThree==0) && (remainder)) || (multiplesOfThree))
        {
            // 765432xx (packed byte0) -> xx543210 (unpacked byte0)
            t0 = packedOffset;
            pUnpackedStr[unpackedIndex] = (pPackedStr[t0]&0xFC) >> 2;
            if((pUnpackedStr[unpackedIndex] & 0x20) == 0 )
            {
                pUnpackedStr[unpackedIndex] += 0x40;
            }
            unpackedIndex++;

            if(multiplesOfThree==0) { remainder--; }
        }

        if( ((multiplesOfThree==0) && remainder) || multiplesOfThree)
        {
            // xxxxxx10 (packed byte0) -> xx54xxxx (unpacked byte1)
            // 7654xxxx (packed byte1) -> xxxx3210 (unpacked byte1)
            t0 = packedOffset;
            t1 = packedOffset + 1;
            pUnpackedStr[unpackedIndex] = ((pPackedStr[t0]&0x03) << 4) | ((pPackedStr[t1]&0xF0) >> 4);
            if((pUnpackedStr[unpackedIndex]&0x20) == 0)
            {
                pUnpackedStr[unpackedIndex] += 0x40;
            }
            unpackedIndex++;

            if(multiplesOfThree==0) { remainder--; }
        }

        if(multiplesOfThree)
        {
            // xxxx3210 (packed byte1) -> xx5432xx (unpacked byte2)
            // 76xxxxxx (packed byte2) -> xxxxxx10 (unpacked byte2)
            t0 = packedOffset + 1;
            t1 = packedOffset + 2;
            pUnpackedStr[unpackedIndex] = ((pPackedStr[t0]&0x0F) << 2) | ((pPackedStr[t1]&0xC0) >> 6);
            if((pUnpackedStr[unpackedIndex]&0x20) == 0)
            {
                pUnpackedStr[unpackedIndex] += 0x40;
            }
            unpackedIndex++;

            // xx543210 (packed byte2) -> xx543210 (unpacked byte3)
            pUnpackedStr[unpackedIndex] = (pPackedStr[t1]&0x3F);
            if((pUnpackedStr[unpackedIndex]&0x20) == 0)
            {
                pUnpackedStr[unpackedIndex] += 0x40;
            }
            unpackedIndex++;
        }

        if(multiplesOfThree) { multiplesOfThree--; }

        packedOffset += 3;
    }
    pUnpackedStr[unpackedIndex]=0;
}
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Sets the  local limit error settings
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 err    - defines that one of the write functions returned an error
         TUSIGN16 Code  - the error code (T_DATA_OBJ_RETURNCODES)
 \return None
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------

// No restriction on the optimization of this function
void AdjusterSetLimitError(TUSIGN8 err,TUSIGN16 Code)
{
    LOCAL_errorCode = Code;
    LOCAL_LimitError = err;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Returns the  local limit error settings
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN16 *pErrorCode - The error code is written to this location
  \return LOCAL_LimitError
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// No restriction on the optimization of this function
TUSIGN8 AdjusterGetLimitError(TUSIGN16 *pErrorCode)
{
    *pErrorCode = LOCAL_errorCode;
    return LOCAL_LimitError;
}

#ifdef HMI_USES_COMMONDIAGNOSTICS
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the worst case alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 *pNAMURClassification - The NAMUR classification of the device is saved here
         TUSIGN8 *pNAMURCategory - The NAMUR category of the device is saved here

 \return TUSIGN8 ID of alarm (highest priority)
 \warning
 \test
         test-date    :
         by           :  Roger Arnold
         environment  :
         result       :
 \test   Lint level 3 :
 \bug
*/
TUSIGN8 AdjusterGetNAMURStatus(TUSIGN8 *pNAMURClassification, TUSIGN8 *pNAMURCategory)
{
    TUSIGN8 returnVal = DIAGNOSIS_CONDITIONS_UNSET;

#ifdef LOCAL_DUMMY_DIAGNOSTICS
    *pNAMURClassification = CLASSIFICATION_FAILURE;
    *pNAMURCategory = CATEGORY_ELECTRONICS;

#else
    T_DIAGNOSIS_ACTIVE_CONDITION	aActiveConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];

    diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, aActiveConditionList);

    if (aActiveConditionList[0].activeConditionId != DIAGNOSIS_CONDITIONS_UNSET)
    {
        #ifdef EDP300_SPECIAL_FEATURES_HMI
            TUSIGN8 EDP300_NamurClass = 0;
            TUSIGN8 EDP300_NamurCategory = 0;

            GetEDP300NamurClassCategory(&EDP300_NamurClass, &EDP300_NamurCategory);
            *pNAMURClassification = ConvertClassification((T_DIAGNOSIS_CLASSES)EDP300_NamurClass);
            *pNAMURCategory = ConvertCategory((T_DIAGNOSIS_GROUPS)EDP300_NamurCategory);
        #else
            // Display details of highest priority alarm
            *pNAMURClassification = ConvertClassification((T_DIAGNOSIS_CLASSES) aActiveConditionList[0].classification);
            *pNAMURCategory = ConvertCategory((T_DIAGNOSIS_GROUPS) aActiveConditionList[0].group);
        #endif

        returnVal = aActiveConditionList[0].activeConditionId;
    }
    else
    {
        *pNAMURClassification = CLASSIFICATION_NONE;
        *pNAMURCategory = CATEGORY_UNSPECIFIED;
    }

#endif
    return returnVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the details of an alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 AlarmNumber - selected alarm
         TUSIGN8 *pAlarmClassification - The classification of the selected alarm is saved here
         TUSIGN8 *pAlarmCategory - The category of the selected alarm is saved here
         TUSIGN8 *pAlarmPriority - The priority of the selected alarm is saved here
         TUSIGN8 *pAlarmID - The ID of the selected alarm is saved here

 \return TBOOL  eFALSE - no alarms active, eTRUE - selected alarm is active
 \warning
 \test
         test-date    :
         by           :  Roger Arnold
         environment  :
         result       :
 \test   Lint level 3 :
 \bug
*/
TBOOL AdjusterGetAlarmConditionDetails(TUSIGN8 SelectedCondition, TUSIGN8 *pAlarmClassification,  TUSIGN8 *pAlarmCategory, TUSIGN8 *pAlarmPriority, TUSIGN8 *pAlarmID)
{
#ifdef LOCAL_DUMMY_DIAGNOSTICS
    if (SelectedCondition == 0)
    {
    *pAlarmClassification = CLASSIFICATION_FAILURE;
    *pAlarmCategory = CATEGORY_SENSOR;
    *pAlarmPriority = 100;
    *pAlarmID       = 0;
    return eFALSE;
    }
    else if (SelectedCondition == 1)
    {
    *pAlarmClassification = CLASSIFICATION_FAILURE;
    *pAlarmCategory = CATEGORY_ELECTRONICS;
    *pAlarmPriority = 90;
    *pAlarmID       = 1;
    return eTRUE;
    }
    else if (SelectedCondition == 2)
    {
    *pAlarmClassification = CLASSIFICATION_OFFSPEC;
    *pAlarmCategory = CATEGORY_INSTALLATION;
    *pAlarmPriority = 80;
    *pAlarmID       = 2;
    return eFALSE;  //eTRUE;
    }
    else if (SelectedCondition == 3)
    {
    *pAlarmClassification = CLASSIFICATION_CHECKFUNCTION;
    *pAlarmCategory = CATEGORY_CONFIGURATION;
    *pAlarmPriority = 70;
    *pAlarmID       = 128;
    return eTRUE;
    }
    else if (SelectedCondition == 4)
    {
    *pAlarmClassification = CLASSIFICATION_MAINTENANCEREQD;
    *pAlarmCategory = CATEGORY_OPERATION;
    *pAlarmPriority = 70;
    *pAlarmID       = 129;
    return eFALSE;  //eTRUE;
    }
    return eFALSE;
#else
    TBOOL returnVal = eFALSE;
	T_DIAGNOSIS_ACTIVE_CONDITION	activeAlarmConditions[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
	
    diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, activeAlarmConditions);
	
	if (activeAlarmConditions[SelectedCondition].activeConditionId != DIAGNOSIS_CONDITIONS_UNSET)
	{
        *pAlarmClassification = ConvertClassification(activeAlarmConditions[SelectedCondition].classification);
        *pAlarmCategory = ConvertCategory(activeAlarmConditions[SelectedCondition].group);
        *pAlarmPriority = activeAlarmConditions[SelectedCondition].priority;
        *pAlarmID       = activeAlarmConditions[SelectedCondition].activeConditionId;
	    	
	    returnVal = eTRUE;
	}
    return returnVal;
#endif
}

TBOOL AdjusterGetAlarmHistoryDetails(TUSIGN8 HistoryNumber, TUSIGN8 SelectedCondition, TUSIGN8 *pAlarmClassification, TUSIGN8 *pAlarmCategory, TUSIGN8 *pAlarmPriority,  TUSIGN8 *pAlarmID, TUSIGN16 *pAlarmCount, TUSIGN32 *pAlarmTime_msec, TUSIGN16 *pAlarmTime_day, TUSIGN32 *pAlarmMostRecent_msec, TUSIGN16 *pAlarmMostRecent_day)
{
    TBOOL returnVal = eFALSE;

#ifdef LOCAL_DUMMY_DIAGNOSTICS

    if (SelectedCondition == 0)
    {
        *pAlarmClassification = CLASSIFICATION_FAILURE;
        *pAlarmCategory = CATEGORY_SENSOR;
        *pAlarmPriority = 100;
        *pAlarmID       = 0;

        *pAlarmCount = 0;//1;
        *pAlarmTime_msec = 17280000;
        *pAlarmTime_day = 0;
        *pAlarmMostRecent_msec = 34560000;
        *pAlarmMostRecent_day = 1;

        returnVal = eTRUE;
    }
    else if (SelectedCondition == 1)
    {
        *pAlarmClassification = CLASSIFICATION_FAILURE;
        *pAlarmCategory = CATEGORY_ELECTRONICS;
        *pAlarmPriority = 90;
        *pAlarmID       = 1;

        *pAlarmCount = 10;
        *pAlarmTime_msec = 51840000;
        *pAlarmTime_day = 2;
        *pAlarmMostRecent_msec = 69120000;
        *pAlarmMostRecent_day = 55555;

        returnVal = eTRUE;
    }
    else if (SelectedCondition == 2)
    {
        *pAlarmClassification = CLASSIFICATION_OFFSPEC;
        *pAlarmCategory = CATEGORY_INSTALLATION;
        *pAlarmPriority = 80;
        *pAlarmID       = 2;

        *pAlarmCount = 0;//100;
        *pAlarmTime_msec = 82943999;
        *pAlarmTime_day = 3;
        *pAlarmMostRecent_msec = 800;
        *pAlarmMostRecent_day = 4444;

        returnVal = eTRUE;
    }
    else if (SelectedCondition == 3)
    {
        *pAlarmClassification = CLASSIFICATION_CHECKFUNCTION;
        *pAlarmCategory = CATEGORY_CONFIGURATION;
        *pAlarmPriority = 70;
        *pAlarmID       = 3;

        *pAlarmCount = 100;
        *pAlarmTime_msec = 1600;
        *pAlarmTime_day = 5;
        *pAlarmMostRecent_msec = 3200;
        *pAlarmMostRecent_day = 333;

        returnVal = eTRUE;
    }
    else if (SelectedCondition == 4)
    {
        *pAlarmClassification = CLASSIFICATION_MAINTENANCEREQD;
        *pAlarmCategory = CATEGORY_OPERATION;
        *pAlarmPriority = 70;
        *pAlarmID       = 4;

        *pAlarmCount = 0;//100;
        *pAlarmTime_msec = 1800000;
        *pAlarmTime_day = 0;
        *pAlarmMostRecent_msec = 5400000;
        *pAlarmMostRecent_day = 18;

        returnVal = eTRUE;
    }
#else
    // Alarm condition range information
    T_DO_ATTRIB_RANGE alarmConditionInfo;

    // Details of active alarms
   	T_DIAGNOSIS_ACTIVE_CONDITION	activeAlarmConditions[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];

	// Alarm Details
	T_DIAGNOSIS_DETAILED_CONDITION  alarmDetails;

    // Create array of active alarms
	diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, activeAlarmConditions);

	// Get the alarm condition range information
	if (diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, DIAGNOSIS_IDX_alarmHistory, ATTRIB_0, &alarmConditionInfo) == (TUSIGN16)OK)
	{
	    // Ensure that alarm condition range information contains necessary information
        if(alarmConditionInfo.range.e8.ptrCodeTab)
        {
            // Get the ID of the selected alarm
            *pAlarmID = alarmConditionInfo.range.e8.ptrCodeTab[SelectedCondition];

            // Get details of the selected alarm
            if (diagnosis.GetDetailsOfDiagnosticConditionSRV(*pAlarmID, &alarmDetails) == (TUSIGN16)OK)
            {
                TUSIGN16 i = 0;
                returnVal = eFALSE;

                // interrogate active alarm list to see if it contains the ID of the selected alarm condition
                while ((returnVal == eFALSE)  && (i < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER))
                {
                    // if active alarm list contains the ID of the selected alarm condition then set returnVal to true (alarm is active)
                    if (activeAlarmConditions[i].activeConditionId == *pAlarmID)
                    {
                        returnVal = eTRUE;
                    }
                    else
                    {
                        i ++;
                    }
                }

                // Extract alarm details
                *pAlarmClassification  = ConvertClassification(alarmDetails.classification);
                *pAlarmCategory        = ConvertCategory(alarmDetails.group);
                *pAlarmPriority        = alarmDetails.priority;
                *pAlarmCount           = alarmDetails.runtimeCounter.alarmCounter;
                *pAlarmTime_msec       = alarmDetails.runtimeCounter.alarmTimeCounterMsec;
                *pAlarmTime_day        = alarmDetails.runtimeCounter.alarmTimeCounterDay;
                *pAlarmMostRecent_msec = alarmDetails.runtimeCounter.timeStampLastAlarmMsec;
                *pAlarmMostRecent_day  = alarmDetails.runtimeCounter.timeStampLastAlarmDay;
            }
        }
    }
#endif
    return returnVal;
}

TINT16 AdjusterFindFirstDiagHistoryEntry(TUSIGN8 HistoryNumber)
{
    TINT16 FirstEntry = -1;

#ifdef LOCAL_DUMMY_DIAGNOSTICS

    FirstEntry = 0;

#else

	// Alarm Details
	T_DIAGNOSIS_DETAILED_CONDITION  alarmDetails;

    T_DO_ATTRIB_RANGE alarmConditionInfo;
	
    TUSIGN8 i = 0;

	// Get the alarm condition range information
	if (diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, DIAGNOSIS_IDX_alarmHistory, ATTRIB_0, &alarmConditionInfo) == (TUSIGN16)OK)
	{
        while ((i < alarmConditionInfo.range.e8.numberOfCodes) && (FirstEntry == -1))
        {
            // Get details of the selected alarm
            diagnosis.GetDetailsOfDiagnosticConditionSRV(alarmConditionInfo.range.e8.ptrCodeTab[i], &alarmDetails); //RAST

            if (alarmDetails.runtimeCounter.alarmCounter != 0)
            {
                FirstEntry = (TINT16)i;
            }
            else
            {
                i++;
            }
        }
    }
#endif
    return FirstEntry;
}

#if defined(HMI_USES_COMMONDIAGNOSTICS) && !defined(LOCAL_DUMMY_DIAGNOSTICS)
TUSIGN8 ConvertCategory(T_DIAGNOSIS_GROUPS group)
{
    TUSIGN8 returnVal = CATEGORY_UNSPECIFIED;

    switch (group)
    {
    #ifdef GROUP_HW_STATUS_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS:                         returnVal = CATEGORY_HARDWARE;          break;
    #endif

    #ifdef GROUP_HW_STATUS_ELECTRONICS_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_ELECTRONICS:             returnVal =  CATEGORY_ELECTRONICS;      break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR:                  returnVal =  CATEGORY_SENSOR;           break;
    #endif

    #ifdef GROUP_HW_STATUS_ACTUATOR_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_ACTUATOR:                returnVal =  CATEGORY_ACTUATOR;         break;
    #endif

    #ifdef GROUP_CONFIG_STATUS_SUPPORTED
    case DIAGNOSIS_GROUP_CONFIG_STATUS:                     returnVal =  CATEGORY_CONFIGURATION;    break;
    #endif

    #ifdef GROUP_CONFIG_STATUS_SETUP_SUPPORTED
    case DIAGNOSIS_GROUP_CONFIG_STATUS_SETUP:               returnVal =  CATEGORY_CONFIGURATION;    break;
    #endif

    #ifdef GROUP_OPERATING_CONDITION_SUPPORTED
    case DIAGNOSIS_GROUP_OPERATING_CONDITION:               returnVal =  CATEGORY_OPERATION;        break;
    #endif

    #ifdef GROUP_OPERATING_CONDITION_POWER_SUPPLY_SUPPORTED
    case DIAGNOSIS_GROUP_OPERATING_CONDITION_POWER_SUPPLY:  returnVal =  CATEGORY_POWERSUPPLY;      break;
    #endif

    #ifdef GROUP_OPERATING_CONDITION_PROCESS_SUPPORTED
    case DIAGNOSIS_GROUP_OPERATING_CONDITION_PROCESS:       returnVal =  CATEGORY_PROCESS;          break;
    #endif

    #ifdef GROUP_INFORMATION_SUPPORTED
    case DIAGNOSIS_GROUP_INFORMATION:                       returnVal =  CATEGORY_INFORMATION;      break;
    #endif

//  #ifdef DIAGNOSIS_USE_GROUP_PROCESS_INSTALLATION
//	case DIAGNOSIS_GROUP_PROCESS_INSTALLATION:              returnVal =  CATEGORY_INSTALLATION;     break;
//	#endif

    #ifdef GROUP_CALIBRATION_SUPPORTED
    case DIAGNOSIS_GROUP_CALIBRATION:                       returnVal =  CATEGORY_CALIBRATION;      break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_1_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_1:                returnVal =  CATEGORY_SENSOR_1;         break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_2_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_2:                returnVal =  CATEGORY_SENSOR_2;         break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_3_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_3:                returnVal =  CATEGORY_SENSOR_3;         break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_4_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_4:                returnVal =  CATEGORY_SENSOR_4;         break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_5_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_5:                returnVal =  CATEGORY_SENSOR_5;         break;
    #endif

    #ifdef GROUP_HW_STATUS_SENSOR_6_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_SENSOR_6:                returnVal =  CATEGORY_SENSOR_6;         break;
    #endif

    #ifdef GROUP_HW_STATUS_TRANSMITTER_SUPPORTED
    case DIAGNOSIS_GROUP_HW_STATUS_TRANSMITTER:             returnVal =  CATEGORY_TRANSMITTER;      break;
    #endif

	
	default:                                                returnVal = CATEGORY_UNSPECIFIED;       break;
    }

    return returnVal;
}

TUSIGN8 ConvertClassification(T_DIAGNOSIS_CLASSES classification)
{
    TUSIGN8 returnVal = CLASSIFICATION_NONE;

    switch (classification)
    {
    //case DIAGNOSIS_CLASS_INFO:                      returnVal = CLASSIFICATION_INFORMATIONONLY; break;
    case DIAGNOSIS_CLASS_FAILURE:                   returnVal = CLASSIFICATION_FAILURE;         break;
    case DIAGNOSIS_CLASS_MAINTENANCE:               returnVal = CLASSIFICATION_MAINTENANCEREQD; break;
    case DIAGNOSIS_CLASS_OFF_SPECIFICATION:         returnVal = CLASSIFICATION_OFFSPEC;         break;
    case DIAGNOSIS_CLASS_CHECK_FUNCTION:            returnVal = CLASSIFICATION_CHECKFUNCTION;   break;
    case DIAGNOSIS_CLASS_NONE:                      returnVal = CLASSIFICATION_NONE;            break;

    #ifdef EDP300_SPECIAL_FEATURES_HMI
    default:                                        returnVal = CLASSIFICATION_NOT_NAMUR;       break;
    #else
    default:                                        returnVal = CLASSIFICATION_NONE;            break;
    #endif
    }
    return returnVal;
}
#endif
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the details of an alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TUSIGN8 - The number of active alarm conditions
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN8 AdjusterGetNumberOfAlarmConditions(TBOOL ActiveAlarmsOnly)
{
    TUSIGN8 nConditions = 0;
#ifdef LOCAL_DUMMY_DIAGNOSTICS

    if (ActiveAlarmsOnly)
    {
        nConditions = 2;
    }
    else
    {
        nConditions = 5;
    }
#else
    if (ActiveAlarmsOnly)
    {
        TUSIGN8 i = 0;
        T_DIAGNOSIS_ACTIVE_CONDITION	activeAlarmConditions[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];

        diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, activeAlarmConditions);

        for (i = 0; i < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER; i ++)
        {
            if  (activeAlarmConditions[i].activeConditionId != DIAGNOSIS_CONDITIONS_UNSET)
            {
                nConditions++;
            }
        }
    }
    else
    {
        // Alarm condition range information
        T_DO_ATTRIB_RANGE alarmConditionInfo;

	    // Get the alarm condition range information
        if (diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, DIAGNOSIS_IDX_alarmHistory, ATTRIB_0, &alarmConditionInfo) == (TUSIGN16)OK)
        {
            // Get the number of alarms
            nConditions = alarmConditionInfo.range.e8.numberOfCodes;
        }
    }
#endif
    return nConditions;
}
#endif

#ifdef HMI_SUPPORTS_SECURITY_ACCESS

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the security state from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TBOOL - eFALSE = ARM_HIGH_SECURITY_DISABLED, eTRUE = ARM_HIGH_SECURITY_ENABLED
 \warning
 \test
         test-date    :  2007-01-25
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TBOOL AdjusterGetARMhighSecurityState(void)
{
    TBOOL returnVal = eFALSE;

#ifndef LOCAL_DUMMY_ARM
    TUSIGN8 HighSecurityState;
   (void)AdjusterGetAttribute((TUSIGN16)ARM_IDX, (TUSIGN16)ARM_IDX_hiSecAccessState, 0, (void FAST*)&HighSecurityState);

    if (HighSecurityState == ARM_HIGH_SECURITY_ENABLED)
    {
        returnVal = eTRUE;
    }
#endif
    return returnVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current ARM ID string from the common ARM subsystem
 \author Roger Arnold
 \date   2007-08-21
 \param  TUSIGN8 *pIDString - where to store the string
         TUSIGN8 nChars - the number of characters expected
                          NOTE: an attempt is made to read this number of characters from the access rights manager.
                          Any characters not ssuccessfully read will be substituded with a space
 \return none
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
void AdjusterGetArmID(TUSIGN8 *pIDString, TUSIGN8 nChars)
{
#ifdef LOCAL_DUMMY_ARM
    TUSIGN8 attribute;
    for (attribute = 0; attribute < nChars; attribute ++)
    {
        pIDString[attribute] = 'A' + attribute;
    }
#else
    TUSIGN8 attribute;
    TUSIGN8 character;
    for (attribute = 0; attribute < nChars; attribute ++)
    {
        if (AdjusterGetAttribute((TUSIGN16)ARM_IDX,(TUSIGN16)ARM_IDX_transmitterIdStringForm,attribute,(void FAST*)&character) != (TUSIGN16)OK)
        {
            character = ' ';
        }

        character = character & 0x7F;
        if (character == 0x00)
        {
            character = ' ';
        }
        pIDString[attribute] = character;
    }
#endif
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current hashcode pin from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TUSIGN16 - The current hashcode pin
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN16 AdjusterGetARMpin(void)
{
    TUSIGN16    ARM_pin;

#ifdef LOCAL_DUMMY_ARM
    ARM_pin = 0x5AF0;
#else

    #if (HMIGEN_MAJOR_VERSION >= 23)
        TUSIGN16 ObjectIndex = (TUSIGN16)ARM_PIN_OBJECT;
        // If the above line generates a compile error because the object index is not defined
        // then the definition of ARM_PIN_OBJECT must be corrected in compileTimeDefinitions.h
    #else
        TUSIGN16 ObjectIndex = (TUSIGN16)ARM_IDX_pin;
    #endif



   (void)AdjusterGetAttribute((TUSIGN16)ARM_IDX, ObjectIndex, 0, (void FAST*)&ARM_pin);
#endif
    return ARM_pin;
}

/*!
 \brief  Gets the current access level from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TUSIGN8 - The current access level
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN8 AdjusterGetARMLevel(void)
{
    TUSIGN8 HMI_LoginState;

#ifdef LOCAL_DUMMY_ARM
    HMI_LoginState = LOCAL_DummyARMLevel;
#else
    TUSIGN8 ARM_LoginState;

    if (AdjusterGetAttribute((TUSIGN16)ARM_IDX, (TUSIGN16)ARM_IDX_hmiLoginState, 0, (void FAST*)&ARM_LoginState) == (TUSIGN16)OK)
    {
        switch (ARM_LoginState)
        {
        case ARM_NO_USER:           HMI_LoginState = (TUSIGN8)ARM_ACCESS_READ_ONLY;      break;
        case ARM_STANDARD_USER:     HMI_LoginState = (TUSIGN8)ARM_ACCESS_STANDARD;       break;
        case ARM_ADVANCED_USER:     HMI_LoginState = (TUSIGN8)ARM_ACCESS_ADVANCED;       break;
        case ARM_SERVICE_USER:      HMI_LoginState = (TUSIGN8)ARM_ACCESS_SERVICE;        break;

        case ARM_CUSTODY_USER:
        case ARM_STANDARD_DEFAULT:
        case ARM_ADVANCED_DEFAULT:
        case ARM_SERVICE_DEFAULT:
        default:                    HMI_LoginState = (TUSIGN8)ARM_ACCESS_READ_ONLY;     break;
        }
    }
    else
    {
        HMI_LoginState = (TUSIGN8)ARM_ACCESS_READ_ONLY;
    }
#endif
    return HMI_LoginState;
}

/*!
 \brief  Attempts to set an access level in the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 AccessLevel - the required access level
         TUSIGN8 *pPassword  - pointer to a password string
 \return TBOOL  eFALSE = unsucessful login, eTRUE = sucessful login
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TBOOL AdjusterSetARMlevel(TUSIGN8 AccessLevel, TUSIGN8 *pPassword)
{
    TBOOL   returnVal = eFALSE;

    #ifndef LOCAL_DUMMY_ARM
	T_ARM const SLOW *pArm = (const T_ARM*)GetSubsystemPtr(ARM_IDX);
    #endif

    switch (AccessLevel)
    {
    case ARM_ACCESS_LOGOUT:
    case ARM_ACCESS_READ_ONLY:
        #ifdef LOCAL_DUMMY_ARM
            LOCAL_DummyARMLevel = (TUSIGN8)ARM_ACCESS_READ_ONLY;
            returnVal = eTRUE;
        #else
            if (pArm->LogOffEXE(HMI_IDX) == (TUSIGN16)OK)
            {
                returnVal = eTRUE;
            }
        #endif
	    break;
	
	case ARM_ACCESS_STANDARD:
        #ifdef LOCAL_DUMMY_ARM
        if (pPassword[0] == 0x31 && pPassword[1] == 0x31 && pPassword[2] == 0x31 && pPassword[3] == 0x31 && pPassword[4] == 0x31 && pPassword[5] == 0x31)
        #else	
        if (pArm->LogInEXE(HMI_IDX, ARM_STANDARD_USER, (TCHAR*)pPassword) == (TUSIGN16)OK)
        #endif
        {
            #ifdef LOCAL_DUMMY_ARM
            LOCAL_DummyARMLevel = (TUSIGN8)ARM_ACCESS_STANDARD;
            #endif
            returnVal = eTRUE;
        }
	    break;
	
	case ARM_ACCESS_ADVANCED:
        #ifdef LOCAL_DUMMY_ARM
        if (pPassword[0] == 0x32 && pPassword[1] == 0x32 && pPassword[2] == 0x32 && pPassword[3] == 0x32 && pPassword[4] == 0x32 && pPassword[5] == 0x32)
        #else
        if (pArm->LogInEXE(HMI_IDX, ARM_ADVANCED_USER, (TCHAR*)pPassword) == (TUSIGN16)OK)
        #endif
        {
            #ifdef LOCAL_DUMMY_ARM
            LOCAL_DummyARMLevel = (TUSIGN8)ARM_ACCESS_ADVANCED;
            #endif
            returnVal = eTRUE;
        }
	    break;
	
	case ARM_ACCESS_SERVICE:
        #ifdef LOCAL_DUMMY_ARM
        if (pPassword[0] == 0x33 && pPassword[1] == 0x33 && pPassword[2] == 0x33 && pPassword[3] == 0x33 && pPassword[4] == 0x33 && pPassword[5] == 0x33)
        #else
        if (pArm->LogInEXE(HMI_IDX, ARM_SERVICE_USER, (TCHAR*)pPassword) == (TUSIGN16)OK)
        #endif
        {
            #ifdef LOCAL_DUMMY_ARM
            LOCAL_DummyARMLevel = (TUSIGN8)ARM_ACCESS_SERVICE;
            #endif
            returnVal = eTRUE;
        }
	    break;

    default:
        returnVal = eFALSE;
        break;
	}
	return returnVal;
}
#endif

#ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS

#ifndef HMI_SUPPORTS_SECURITY_ACCESS
#error system must include common ARM in order to display diagnosis condition details
#endif

#ifndef HMI_USES_COMMONDIAGNOSTICS
#error system must include common diagnostics in order to display diagnosis condition details
#endif

#ifndef DIAGNOSIS_USE_CONDITION_DETAILS
#error diagnostics subsystem must use condition details in order to allow HMI to display condition details
#endif

TUSIGN8 Adjuster_GetDiagDetailsViewCursorMax(void)
{
    return DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER - 1;
}

TBOOL Adjuster_GetDiagDetails(TUSIGN8 AlarmID, TUSIGN8 detailNumber, TUSIGN8* p_subIdx, TUSIGN8* p_alarm)
{
    TBOOL returnVal = eFALSE;

	// Alarm Details
	T_DIAGNOSIS_DETAILED_CONDITION  alarmDetails;
    T_CONDITION_DETAIL conditionDetail;
    if (diagnosis.GetDetailsOfDiagnosticConditionSRV(AlarmID, &alarmDetails) == (TUSIGN16)OK)
    {
        conditionDetail = alarmDetails.details[detailNumber];
        // Extract alarm details

        if (conditionDetail.alarm != 0xFF)
        {
            *p_subIdx = (TUSIGN8)(conditionDetail.subIdx);
            *p_alarm  = conditionDetail.alarm;
            returnVal = eTRUE;
        }
    }
    return returnVal;
}
#endif


#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES

TBOOL AdjusterConvertSecondsToDate(TUSIGN32 time, TIMEANDDATE *pTimeAndDate)
{
    TBOOL   returnVal;

    TUSIGN16 year   = 2000;
    TUSIGN8  month  = 1;
    TUSIGN8  date   = 1;
    TUSIGN8  hour   = 0;
    TUSIGN8  minute = 0;
    TUSIGN8  second = 0;

    TUSIGN32 SECONDS_IN_FEBRUARY = SECONDS_IN_29_DAYS;

    // Calculate Year
    while (time >= SECONDS_IN_4_YEARS)
    {
        time -= SECONDS_IN_4_YEARS;
        year += 4;
    }

    if (time >= SECONDS_IN_LEAP_YEAR)
    {
        time -= SECONDS_IN_LEAP_YEAR;
        year ++;

        SECONDS_IN_FEBRUARY = SECONDS_IN_28_DAYS;

        while (time >= SECONDS_IN_YEAR)
        {
            time -= SECONDS_IN_YEAR;
            year ++;
        }
    }

    // Calculate Month
    // assume month is January
    if (time >= SECONDS_IN_JANUARY)
    {
        time -= SECONDS_IN_JANUARY; // remove January
        month ++;                   // increment to February

        if (time >= SECONDS_IN_FEBRUARY)
        {
            time -= SECONDS_IN_FEBRUARY;// remove February
            month ++;                   // increment to March

            if (time >= SECONDS_IN_MARCH)
            {
                time -= SECONDS_IN_MARCH; // remove March
                month ++;                   // increment to April

                if (time >= SECONDS_IN_APRIL)
                {
                    time -= SECONDS_IN_APRIL; // remove April
                    month ++;                   // increment to May

                    if (time >= SECONDS_IN_MAY)
                    {
                        time -= SECONDS_IN_MAY; // remove May
                        month ++;                   // increment to June

                        if (time >= SECONDS_IN_JUNE)
                        {
                            time -= SECONDS_IN_JUNE; // remove June
                            month ++;                   // increment to July

                            if (time >= SECONDS_IN_JULY)
                            {
                                time -= SECONDS_IN_JULY; // remove July
                                month ++;                   // increment to August

                                if (time >= SECONDS_IN_AUGUST)
                                {
                                    time -= SECONDS_IN_AUGUST; // remove August
                                    month ++;                   // increment to September

                                    if (time >= SECONDS_IN_SEPTEMBER)
                                    {
                                        time -= SECONDS_IN_SEPTEMBER; // remove September
                                        month ++;                   // increment to October

                                        if (time >= SECONDS_IN_OCTOBER)
                                        {
                                            time -= SECONDS_IN_OCTOBER; // remove October
                                            month ++;                   // increment to November

                                            if (time >= SECONDS_IN_NOVEMBER)
                                            {
                                                time -= SECONDS_IN_NOVEMBER; // remove November
                                                month ++;                   // increment to December
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Calculate Date
    while (time >= SECONDS_IN_1_DAY)
    {
        time -= SECONDS_IN_1_DAY;
        date ++;
    }

    // Calculate Hour
    while (time >= SECONDS_IN_1_HOUR)
    {
        time -= SECONDS_IN_1_HOUR;
        hour ++;
    }

    // Calculate Minute
    while (time >= SECONDS_IN_1_MINUTE)
    {
        time -= SECONDS_IN_1_MINUTE;
        minute ++;
    }

    // Calculate seconds
    second += (TUSIGN8)time;

    // Check result
    if ((second >= 60)
     || (minute >= 60)
     || (hour   >= 24)
     || (date < 1)
     || (date > 31)
     || (((date > 30) && ((month == 4) || (month == 6) || (month == 9) || (month == 11))))
     || (((date > 29) && (month == 2)))
     || (((date > 28) && (month == 2) && (year % 4)))
     || (month < 1)
     || (month > 12)
     || (year < 2000))

    {
        pTimeAndDate->year   = 2000;
        pTimeAndDate->month  = 1;
        pTimeAndDate->date   = 1;
        pTimeAndDate->hour   = 0;
        pTimeAndDate->minute = 0;
        pTimeAndDate->second = 0;
        returnVal = eFALSE;
    }
    else
    {
        pTimeAndDate->year   = year;
        pTimeAndDate->month  = month;
        pTimeAndDate->date   = date;
        pTimeAndDate->hour   = hour;
        pTimeAndDate->minute = minute;
        pTimeAndDate->second = second;
        returnVal = eTRUE;
    }
    return returnVal;
}

TBOOL AdjusterConvertDateToSeconds(TIMEANDDATE* pTimeAndDate, TUSIGN32 *pTime)
{
    TBOOL returnVal;

    // Check input date/time
    if ( (pTimeAndDate->second >= 60)
     ||  (pTimeAndDate->minute >= 60)
     ||  (pTimeAndDate->hour   >= 24)
     ||  (pTimeAndDate->date < 1)
     ||  (pTimeAndDate->date > 31)
     || (((pTimeAndDate->date > 30) && ((pTimeAndDate->month == 4) || (pTimeAndDate->month == 6) || (pTimeAndDate->month == 9) || (pTimeAndDate->month == 11))))
     || (((pTimeAndDate->date > 29) && (pTimeAndDate->month == 2)))
     || (((pTimeAndDate->date > 28) && (pTimeAndDate->month == 2) && (pTimeAndDate->year % 4)))
     ||  (pTimeAndDate->month < 1)
     ||  (pTimeAndDate->month > 12)
     ||  (pTimeAndDate->year < 2000))
    {
        *pTime = 0;
        returnVal = eFALSE;
    }
    else
    {
        TUSIGN32 time = 0;
        TUSIGN16 year = pTimeAndDate->year - 2000;
        TUSIGN32 SECONDS_IN_FEBRUARY = SECONDS_IN_29_DAYS;

        while (year >= 4)
        {
            year -= 4;
            time += SECONDS_IN_4_YEARS;
        }

        if (year != 0)
        {
            SECONDS_IN_FEBRUARY = SECONDS_IN_28_DAYS;
            switch (year)
            {
            // break intentionally omitted from each case
            #pragma diag_suppress=Pm057
            case 3:     time += SECONDS_IN_YEAR;
            case 2:     time += SECONDS_IN_YEAR;
            case 1:     time += SECONDS_IN_LEAP_YEAR;
            default:    asm("nop");
            #pragma diag_default=Pm057
            }
        }

        switch (pTimeAndDate->month)
        {
            // break intentionally omitted from each case
            #pragma diag_suppress=Pm057
            case 12:    time += SECONDS_IN_NOVEMBER;
            case 11:    time += SECONDS_IN_OCTOBER;
            case 10:    time += SECONDS_IN_SEPTEMBER;
            case 9:     time += SECONDS_IN_AUGUST;
            case 8:     time += SECONDS_IN_JULY;
            case 7:     time += SECONDS_IN_JUNE;
            case 6:     time += SECONDS_IN_MAY;
            case 5:     time += SECONDS_IN_APRIL;
            case 4:     time += SECONDS_IN_MARCH;
            case 3:     time += SECONDS_IN_FEBRUARY;
            case 2:     time += SECONDS_IN_JANUARY;
            case 1:
            default:    asm("nop");
            #pragma diag_default=Pm057
        }

        time += (((TUSIGN32)pTimeAndDate->date - 1)   * SECONDS_IN_1_DAY);
        time += ((TUSIGN32)pTimeAndDate->hour   * SECONDS_IN_1_HOUR);
        time += ((TUSIGN32)pTimeAndDate->minute * SECONDS_IN_1_MINUTE);
        time += ((TUSIGN32)pTimeAndDate->second);

        *pTime = time;
        returnVal = eTRUE;
    }
    return returnVal;
}

TUSIGN32 AdjusterGetTimeDateVariable(void)
{
    return LOCAL_TimeAndDate;
}

// end of #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES

#endif

void AdjusterAdjustResolutionForCustomDP(TUSIGN8 *pResolution)
{
    switch (*pResolution)
    {
    case CONFIGURABLE_DP_1:     *pResolution = SFD_CUSTOM_DP.customDP_1;  break;
    case CONFIGURABLE_DP_2:     *pResolution = SFD_CUSTOM_DP.customDP_2;  break;
    case CONFIGURABLE_DP_3:     *pResolution = SFD_CUSTOM_DP.customDP_3;  break;
    case CONFIGURABLE_DP_4:     *pResolution = SFD_CUSTOM_DP.customDP_4;  break;
    case CONFIGURABLE_DP_5:     *pResolution = SFD_CUSTOM_DP.customDP_5;  break;
    case CONFIGURABLE_DP_6:     *pResolution = SFD_CUSTOM_DP.customDP_6;  break;
    case CONFIGURABLE_DP_7:     *pResolution = SFD_CUSTOM_DP.customDP_7;  break;
    case CONFIGURABLE_DP_8:     *pResolution = SFD_CUSTOM_DP.customDP_8;  break;
    case CONFIGURABLE_DP_9:     *pResolution = SFD_CUSTOM_DP.customDP_9;  break;
    case CONFIGURABLE_DP_10:    *pResolution = SFD_CUSTOM_DP.customDP_10; break;
    case CONFIGURABLE_DP_11:    *pResolution = SFD_CUSTOM_DP.customDP_11; break;
    default:                    asm("nop");                                                         break;
    }
}
