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
 \brief       Calculate the Energy flow for Gas or select from simulatin energy flow.
 \author      zuochen wang
\date 2012-9-12
\param 
	inVF - volume flow in unit m3/h
\return 
	Energy flow in unit KJ/h
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

TFLOAT CalculateGasQp(TFLOAT inVF)
{
	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h

	lDampValue = mVMeasurementDynamicDuplicated.QpDamped;
	lDampTime  = mVMeasurementStaticFrequentStaticDefault.QpDampingTime;
	lCutoffPer = sVMeasurementReplaceStaticFreqStaticDefault.QvLowCutOff;
	lRangHiLim = mVMeasurementStaticFrequentStaticDefault.QpRange.hiLim;

    if(mVMeasurementProtected.QpSimEnable == MVM_ENABLE)
	{
		baseUnitValue = mVMeasurementUnprotected.QpSim;
		lSimState = eTRUE;
	}
	else
	{
		baseUnitValue  = inVF *  mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.calorificEnergy*1000.0f;
		lSimState = eFALSE;
	}

	//LowCutoff and damping
	lCutoffAlarm = CalculateQ(CDT_GAS_POWER, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_GAS_POWER))
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
	
	//GasQp is present in m3/h. 
	mVMeasurementDynamicDuplicated.Qp = baseUnitValue;
	mVMeasurementDynamicDuplicated.QpDuplicated = baseUnitValue;

	mVMeasurementDynamicDuplicated.QpDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QpDampedDuplicated = lDampValue;
	
	//percentage
	mVMeasurementDynamicDuplicated.QpPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QpPercentageDuplicated =  mVMeasurementDynamicDuplicated.QpPercentage;
	
	return baseUnitValue;
}