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
 \brief       Calculate the biogas volome flow with given volume flow and Content Biogas
 \author      zuochen wang
\date 2012-9-12
\param
	inNVF -standard volume flow.
    biogasPerc -Content Biogas
\return
	Biogas standard volume flow
\warning
\test
	test-date: 2013-06-28
	\n by: zuochen wang
	\n environment: C-SPY
\n intention:
\n result module test: Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------

TFLOAT CalculateGasQnPartial(TFLOAT inNVF,TFLOAT biogasPerc)
{
	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h

	lDampValue = mVMeasurementDynamicDuplicated.QnPartialDamped;
	lDampTime  = mVMeasurementStaticFrequentStaticDefault.QnPartialDampingTime;
	lCutoffPer = sVMeasurementReplaceStaticFreqStaticDefault.QvLowCutOff;
	lRangHiLim = mVMeasurementStaticFrequentStaticDefault.QnPartialRange.hiLim;
	
	if(mVMeasurementProtected.QnPartialSimEnable == MVM_ENABLE)
	{
		baseUnitValue = mVMeasurementUnprotected.QnPartialSim;
		lSimState = eTRUE;
	}
	else
	{
		baseUnitValue  = (inNVF*biogasPerc)/100.0f;
		lSimState = eFALSE;
	}

	//LowCutoff and damping
	lCutoffAlarm = CalculateQ(CDT_BIOGAS_STANDARD_VOLUME, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_BIOGAS_STANDARD_VOLUME))
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
	
	//GasQnPartial is present in m3/h. 
	mVMeasurementDynamicDuplicated.QnPartial = baseUnitValue;
	mVMeasurementDynamicDuplicated.QnPartialDuplicated = baseUnitValue;

	mVMeasurementDynamicDuplicated.QnPartialDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QnPartialDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QnPartialPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QnPartialPercentageDuplicated =  mVMeasurementDynamicDuplicated.QnPartialPercentage;

	return baseUnitValue;
}