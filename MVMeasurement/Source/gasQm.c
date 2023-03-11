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
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Source/Coordinator_types.h"
#include "Coordinator/Source/coordinator_attributes.h"

#include "SVMeasurement/Source/SVMeasurement_types.h"
#include "SVMeasurement/Source/SVMeasurement_attributes.h"

#include "MVMeasurement/Interface/MVMeasurement.h"
#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"

#include "QCommonCal.h"

//-------------------------------------------------------------------------------------------------
/*!
 \brief       Calculate the mass flow use real density or ref density.
 \author      zuochen wang
\date 2012-9-12
\param
	 inVF - volume flow
	 inNVF - standard volume flow
	 densityReal - real density
\return
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

TFLOAT CalculateGasQm(TFLOAT inVF,TFLOAT inNVF,TFLOAT densityReal)
{
	TUSIGN8 lSimulationMode = coordinatorProtected.simulationMode;

	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h

	lDampValue = mVMeasurementDynamicDuplicated.QmDamped;
	lDampTime  = mVMeasurementStaticFrequentStaticDefault.QmDampingTime;
	lCutoffPer = sVMeasurementReplaceStaticFreqStaticDefault.QvLowCutOff;
	lRangHiLim = mVMeasurementStaticFrequentStaticDefault.QmRange.hiLim;

	if(lSimulationMode == CDT_SIMULATION_CB_QM_VALUE)      //simulation QmSim
	{
		baseUnitValue = mVMeasurementUnprotected.QmSim;
		lSimState = eTRUE;
	}
	else if(lSimulationMode == CDT_SIMULATION_CB_QM_PERCENTAGE)      //simulation QmSim Percentage
	{
		baseUnitValue = (mVMeasurementUnprotected.QmPerSim*lRangHiLim) /100.0f;
		lSimState = eTRUE;
	}
	else if(mVMeasurementProtected.QmSimEnable == MVM_ENABLE)
	{
		baseUnitValue = mVMeasurementUnprotected.QmSim;
		lSimState = eTRUE;
	}
	else
	{
		baseUnitValue = inVF * densityReal;

		lSimState = eFALSE;
	}

	//LowCutoff and damping
	lCutoffAlarm = CalculateQ(CDT_GAS_MASS, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_GAS_MASS))
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
	
	//Qm is present in m3/h. 
	mVMeasurementDynamicDuplicated.Qm = baseUnitValue;
	mVMeasurementDynamicDuplicated.QmDuplicated = baseUnitValue;

	mVMeasurementDynamicDuplicated.QmDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QmDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QmPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QmPercentageDuplicated =  mVMeasurementDynamicDuplicated.QmPercentage;
	
	return baseUnitValue;
}