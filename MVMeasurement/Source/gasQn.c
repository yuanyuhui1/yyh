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

extern const TFLOAT pressureRefTable[];
extern const TFLOAT temperatureRefTable[];
//-------------------------------------------------------------------------------------------------
/*!
 \brief       Calculate the Standard volome flow with T and P conpenstion.
 \author      zuochen wang
\date 2012-9-12
\param
	inVF -  Input volume flow
	tReal - Input Real Temperature
	pReal - Input real pressure
\return
	Standard volume flow at given condtion
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

TFLOAT CalculateGasQn(TFLOAT inVF, TFLOAT tReal, TFLOAT pReal)
{
	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h
	TFLOAT refPressure = 0.0f, refTemperature = 0.0f;
    TFLOAT opeTemperature = tReal;
	TFLOAT opePressure = pReal;

	lDampValue = mVMeasurementDynamicDuplicated.QnDamped;
	lDampTime  = mVMeasurementStaticFrequentStaticDefault.QnDampingTime;
	lCutoffPer = sVMeasurementReplaceStaticFreqStaticDefault.QvLowCutOff;
	lRangHiLim = mVMeasurementStaticFrequentStaticDefault.QnRange.hiLim;

	if(mVMeasurementProtected.QnSimEnable == MVM_ENABLE)
	{
		baseUnitValue = mVMeasurementUnprotected.QnSim;
		lSimState = eTRUE;
	}
	else
	{
		refPressure = pressureRefTable[ mVMeasurementStaticFrequentStaticDefault.gasRef];
		refTemperature = temperatureRefTable[mVMeasurementStaticFrequentStaticDefault.gasRef];

	  	//add The Gas Qn calculation here
        if(mVMeasurementStaticFrequentStaticDefault.gasComputeType == MVM_IDEAL_GAS)
        {
          baseUnitValue = ( inVF * opePressure * (refTemperature + 273.15f))
                          / ((opeTemperature + 273.15f) * refPressure );   
        }
        else
        {
          baseUnitValue = ( inVF * opePressure * (refTemperature + 273.15f) * mVMeasurementDynamicDuplicated.compressFactorS)
                          / ((opeTemperature + 273.15f) * refPressure * mVMeasurementDynamicDuplicated.compressFactorR);   
        }

		lSimState = eFALSE;
	}

	//LowCutoff
	lCutoffAlarm = CalculateQ(CDT_GAS_STANDARD_VOLUME, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_GAS_STANDARD_VOLUME))
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
	
	//GasQn is present in m3/h. 
	mVMeasurementDynamicDuplicated.Qn = baseUnitValue;
	mVMeasurementDynamicDuplicated.QnDuplicated = baseUnitValue;

	mVMeasurementDynamicDuplicated.QnDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QnDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QnPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QnPercentageDuplicated =  mVMeasurementDynamicDuplicated.QnPercentage;

	return baseUnitValue;
}