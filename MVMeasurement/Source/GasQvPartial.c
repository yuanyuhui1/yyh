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

#include "MVMeasurement/Interface/MVMeasurement.h"
#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"
#include "SVMeasurement/Source/SVMeasurement_types.h"
#include "SVMeasurement/Source/SVMeasurement_attributes.h"
#include "Coordinator/Interface/Coordinator.h"
#include "QCommonCal.h"

//-------------------------------------------------------------------------------------------------
/*!
 \brief       Calculate volome flow and Percentage flow for Biogas
 \author      zuochen wang
\date 2012-9-12
\param
	inVF -actula volume flow.
    biogasPerc -Content Biogas
\return
	Biogas Volume flow
\warning
\test
	test-date: 2014-01-10
	\n by: zuochen wang
	\n environment: C-SPY
\n intention:
\n result module test: Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------

TFLOAT CalculateGasQvPartial(TFLOAT inVF,TFLOAT biogasPerc)
{
	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h

	lDampValue = mVMeasurementDynamicDuplicated.QvPartialDamped;
	lDampTime  = mVMeasurementStaticFrequentStaticDefault.QvPartialDampingTime;
	lCutoffPer = sVMeasurementReplaceStaticFreqStaticDefault.QvLowCutOff;
	lRangHiLim = mVMeasurementStaticFrequentStaticDefault.QvPartialRange.hiLim;
	
	if(mVMeasurementProtected.QvPartialSimEnable == MVM_ENABLE)
	{
		baseUnitValue = mVMeasurementUnprotected.QvPartialSim;
		lSimState = eTRUE;
	}
	else
	{
		baseUnitValue  = (inVF *  biogasPerc)/100.0f;
		lSimState = eFALSE;
	}

	//LowCutoff and damping
	lCutoffAlarm = CalculateQ(CDT_BIOGAS_ACTUAL_VOLUME, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_BIOGAS_ACTUAL_VOLUME))
	{
		// only the Q which is set to PV can be cut off, and also the only one can set or clear the cut off alarm.
		if(lCutoffAlarm)
		{
			MVM_SETDIAG_(MVM_ALM_LOW_Q_VALUE_CUTOFF, mVMeasurementDynamicDuplicated.mvmDiagnosis);
		}
		else
		{
			MVM_CLRDIAG_(MVM_ALM_LOW_Q_VALUE_CUTOFF, mVMeasurementDynamicDuplicated.mvmDiagnosis);
		}
	}

	//GasQvPartial is present in m3/h. 
	mVMeasurementDynamicDuplicated.QvPartial = baseUnitValue;
	mVMeasurementDynamicDuplicated.QvPartialDuplicated = baseUnitValue;

	mVMeasurementDynamicDuplicated.QvPartialDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QvPartialDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QvPartialPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QvPartialPercentageDuplicated =  mVMeasurementDynamicDuplicated.QvPartialPercentage;

	return baseUnitValue;
}