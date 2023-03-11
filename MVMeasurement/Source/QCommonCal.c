//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 Qv.c
 Description            Qv measurement
                          - Compute
                          - Unit conversion
 Remarks

*/
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"

#include "T_Unit/Interface/t_unit.h"
#include "Common/Interface/common_functions.h"

#include "MVMeasurement/Interface/MVMeasurement.h"
#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"

#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Source/Mapper_Measurement.h"

#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"

#include "QCommonCal.h"


static const TUSIGN16 sSupportQMaskOfOp[MAPPER_OPM_NUM] = 
{
    /* CDT_LIQUID_ACTUAL_VOLUME   */  SELECTION_LIQUIDQV,
    /* CDT_LIQUID_NORMAL_VOLUME   */  SELECTION_LIQUIDQV | SELECTION_LIQUIDQN,
    /* CDT_LIQUID_MASS            */  SELECTION_LIQUIDQV | SELECTION_LIQUIDQM,
    /* CDT_LIQUID_POWER           */  SELECTION_LIQUIDQV | SELECTION_LIQUIDQM     | SELECTION_LIQUIDPOWER,
    /* CDT_GAS_ACTUAL_VOLUME      */  SELECTION_GASQV,
    /* CDT_GAS_STANDARD_VOLUME    */  SELECTION_GASQV    | SELECTION_GASQN,
    /* CDT_GAS_MASS               */  SELECTION_GASQV    | SELECTION_GASQN        | SELECTION_GASQM,
    /* CDT_GAS_POWER              */  SELECTION_GASQV    | SELECTION_GASQN        | SELECTION_GASPOWER,
    /* CDT_BIOGAS_ACTUAL_VOLUME   */  SELECTION_GASQV    | SELECTION_GASPARTIALQV,
    /* CDT_BIOGAS_STANDARD_VOLUME */  SELECTION_GASQV    | SELECTION_GASPARTIALQV | SELECTION_GASQN | SELECTION_GASPARTIALQN,
    /* CDT_STEAM_ACTUAL_VOLUME    */  SELECTION_STEAMQV,
    /* CDT_STEAM_MASS             */  SELECTION_STEAMQV  | SELECTION_STEAMQM,
    /* CDT_STEAM_POWER            */  SELECTION_STEAMQV  | SELECTION_STEAMQM      | SELECTION_STEAMPOWER
};

static const TUSIGN8 sPVOfOp[MAPPER_OPM_NUM] = 
{
    /* CDT_LIQUID_ACTUAL_VOLUME   */ MAPPER_DEVICE_VAR_QV,
    /* CDT_LIQUID_NORMAL_VOLUME   */ MAPPER_DEVICE_VAR_QN,
    /* CDT_LIQUID_MASS            */ MAPPER_DEVICE_VAR_QM,
    /* CDT_LIQUID_POWER           */ MAPPER_DEVICE_VAR_POWER,
    /* CDT_GAS_ACTUAL_VOLUME      */ MAPPER_DEVICE_VAR_QV,
    /* CDT_GAS_STANDARD_VOLUME    */ MAPPER_DEVICE_VAR_QN,
    /* CDT_GAS_MASS               */ MAPPER_DEVICE_VAR_QM,
    /* CDT_GAS_POWER              */ MAPPER_DEVICE_VAR_POWER,
    /* CDT_BIOGAS_ACTUAL_VOLUME   */ MAPPER_DEVICE_VAR_PARTIAL_QV,
    /* CDT_BIOGAS_STANDARD_VOLUME */ MAPPER_DEVICE_VAR_PARTIAL_QN,
    /* CDT_STEAM_ACTUAL_VOLUME    */ MAPPER_DEVICE_VAR_QV,
    /* CDT_STEAM_MASS             */ MAPPER_DEVICE_VAR_QM,
    /* CDT_STEAM_POWER            */ MAPPER_DEVICE_VAR_POWER
};

static TUSIGN8  sDampInit[MAPPER_OPM_NUM] = {0};
static TUSIGN32 sDampPreviousTime[MAPPER_OPM_NUM] = {0};
static TUSIGN32 sAlarmPreviousTime[MAPPER_OPM_NUM] = {0};
static TBOOL    sCutoffAlarm[MAPPER_OPM_NUM] = {eTRUE, eTRUE, eTRUE, eTRUE, eTRUE,\
                                                eTRUE, eTRUE, eTRUE, eTRUE, eTRUE,\
                                                eTRUE, eTRUE, eTRUE};

TBOOL IsPVMatchWithQ(TUSIGN8 cal_Q_type)
{
    TUSIGN8 lPvAssignment = 0;
    TUSIGN8 lQType = cal_Q_type;
    TBOOL lMatch = eFALSE;

    if (lQType < MAPPER_OPM_NUM)
    {
        (void)mapper_Get((TUSIGN16)MAPPER_IDX_PV_Assignment, (TINT16)WHOLE_OBJECT, &lPvAssignment);
        if (lPvAssignment == sPVOfOp[lQType])
        {
            lMatch = eTRUE;
        }
    }

    return lMatch;
}

static TBOOL IsSupportedQCalculation(TUSIGN8 cal_Q_type, TUSIGN8 operationMode)
{
    TBOOL lSupport = eFALSE;
    TUSIGN16 lOpModeBit = 0;

    if ((operationMode < MAPPER_OPM_NUM) && (cal_Q_type < MAPPER_OPM_NUM))
    {
        lOpModeBit = (((TUSIGN16)0x01)<<cal_Q_type);

        if (sSupportQMaskOfOp[operationMode] & lOpModeBit)
        {
            lSupport = eTRUE;
        }
    }

    return lSupport;
}

TBOOL CalculateQ(TUSIGN8 cal_Q_type, TFLOAT* pBaseValue, TFLOAT* pDampValue, \
                    TBOOL simulation, TFLOAT hiLimit, TFLOAT cutoffPercentage, TFLOAT dampingTime)
{
    const TFLOAT DELAY_TIME = 0.5f;

    TUSIGN8 lIndex = cal_Q_type;
    TUSIGN8 lOpMode = CDT_LIQUID_ACTUAL_VOLUME;
    TFLOAT lHysteresis = 0.0f;
	TFLOAT lCutoffValue = 0.0f;
    TFLOAT lBaseValue = *pBaseValue;
    TFLOAT lDampValue = *pDampValue;
    TBOOL lCutoff = eFALSE;

    (void)coordinator_Get( (TUSIGN16)CDT_IDX_operatingMode, (TINT16)WHOLE_OBJECT, &lOpMode );
    if (IsSupportedQCalculation(lIndex, lOpMode))
    {
        if (!simulation)
        {
            if (IsPVMatchWithQ(lIndex))
            {
                lCutoffValue = (hiLimit * cutoffPercentage) / 100.0f;
                lHysteresis = hiLimit * 0.001f;// 0.1% Upperrange .example for 0.08 of 80m3/h.

                /* if cut off did not happen at last time */
                if (!sCutoffAlarm[lIndex])
                {
                    /* damping firstly, then cut off */
                    Damping(lBaseValue, &lDampValue, &sDampInit[lIndex], &sDampPreviousTime[lIndex], dampingTime);

                    CheckLowerRangeLimit_Alarm(lCutoffValue, lDampValue, lHysteresis, DELAY_TIME, \
                                                &sAlarmPreviousTime[lIndex], &sCutoffAlarm[lIndex]);

                    if (sCutoffAlarm[lIndex])
                    {
                        lBaseValue = 0.0f;
                        lDampValue = 0.0f;
                    }
                }
                else
                {
                    /* cut off firstly, then damping */
                    CheckLowerRangeLimit_Alarm(lCutoffValue, lBaseValue, lHysteresis, DELAY_TIME, \
                                                &sAlarmPreviousTime[lIndex], &sCutoffAlarm[lIndex]);

                    if (!sCutoffAlarm[lIndex]) /* no more cut off */
                    {
                        /* set to cut off frequency immediately firstly */
                        Damping(lCutoffValue, &lDampValue, &sDampInit[lIndex], &sDampPreviousTime[lIndex], 0);
                    }
                    else /* still cut off */
                    {
                        Damping(0, &lDampValue, &sDampInit[lIndex], &sDampPreviousTime[lIndex], 0);

                        lBaseValue = 0.0f;
                        lDampValue = 0.0f;
                    }
                }
            }
            else
            {
                /* no cut off, keep damping */
                sCutoffAlarm[lIndex] = eFALSE;

                /* keep damping */
                Damping(lBaseValue, &lDampValue, &sDampInit[lIndex], &sDampPreviousTime[lIndex], dampingTime);
            }
        }
        else
        {
            /* no cut off, keep damping */
            sCutoffAlarm[lIndex] = eFALSE;

            /* keep damping */
            Damping(lBaseValue, &lDampValue, &sDampInit[lIndex], &sDampPreviousTime[lIndex], dampingTime);
        }

        *pBaseValue = lBaseValue;
        *pDampValue = lDampValue;

        lCutoff = sCutoffAlarm[lIndex];
    }

    return lCutoff;
}