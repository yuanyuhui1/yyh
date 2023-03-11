//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 Qn.c
 Description            Qn measurement
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

//--------------------------------------------------------------------------------------------------
/*!
 \brief       Calculate the starndard volome flow for liquid .and Damping it. 
 \author      zuochen wang
 \date        2012-07-02
 \param       inFreq: TFLOAT  the frequency of the flow sensor
 \return      volume flow in m3/h
 \warning     none
 \test
 \test
        test-date    :  2013-07-03
        by           :  zuochen wang
        environment  :  CSPY
        result       :  OK

 \test  Lint level 3 OK

 \bug 
*/
//--------------------------------------------------------------------------------------------------
TFLOAT CalculateLiquidQn(TFLOAT inVF,TFLOAT inTReal)
{
	TBOOL lCutoffAlarm = eFALSE, lSimState = eFALSE;
	TFLOAT lDampValue = 0.0f, lDampTime = 0.0f, lCutoffPer = 0.0f, lRangHiLim = 0.0f;
	TFLOAT baseUnitValue = 0.0;//m3/h

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
	  	baseUnitValue = inVF / (1.0f + ((inTReal - mVMeasurementStaticFrequentStaticDefault.refTemperature) \
		  						* mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.volumeExpandBeta1));
		lSimState = eFALSE;
	}

	//LowCutoff: only for PV
	lCutoffAlarm = CalculateQ(CDT_LIQUID_NORMAL_VOLUME, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_LIQUID_NORMAL_VOLUME))
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
	
	//Qn is present in m3/h. 
	mVMeasurementDynamicDuplicated.Qn = baseUnitValue;
	mVMeasurementDynamicDuplicated.QnDuplicated = baseUnitValue;

	// damped
	mVMeasurementDynamicDuplicated.QnDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QnDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QnPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QnPercentageDuplicated =  mVMeasurementDynamicDuplicated.QnPercentage;
	
	return baseUnitValue;
}