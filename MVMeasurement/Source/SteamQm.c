//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 Qm.c
 Description            Qm measurement
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

#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Source/MVMeasurement_types.h"
#include "MVMeasurement/Source/MVMeasurement_attributes.h"
#include "SVMeasurement/Source/SVMeasurement_types.h"
#include "SVMeasurement/Source/SVMeasurement_attributes.h"
#include "Mapper/Source/Mapper_Measurement.h"
#include "QCommonCal.h"

/*!
 \brief       Calculate the mass flow for steam .or selection simulation mass flow. 
 \author      zuochen wang
 \date        2012-07-02
 \param      
 	inVF - input Volume flow
 	inDensityReal - input Real denisty
 \return  
	mass flow of steam
 \warning     none
 \test
 \test
        test-date    :  2013-06-28
        by           :  zuochen wang
        environment  :  CSPY
        result       :  OK

 \test  Lint level 3 OK
 \bug 
*/
//--------------------------------------------------------------------------------------------------
TFLOAT CalculateSteamQm(TFLOAT inVF,TFLOAT inDensityReal)
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
		lSimState = eTRUE;
		baseUnitValue = mVMeasurementUnprotected.QmSim; 
	}
	else if(lSimulationMode == CDT_SIMULATION_CB_QM_PERCENTAGE)      //simulation QmSim Percentage
	{
		lSimState = eTRUE;
		baseUnitValue = (mVMeasurementUnprotected.QmPerSim*lRangHiLim) /100.0f; 
	}
	else if(mVMeasurementProtected.QmSimEnable == MVM_ENABLE)
	{
		lSimState = eTRUE;
		baseUnitValue = mVMeasurementUnprotected.QmSim; 
	}
	else
	{  
		lSimState = eFALSE;
		baseUnitValue = inVF * inDensityReal;       
	}

	//LowCutoff and damping
	lCutoffAlarm = CalculateQ(CDT_STEAM_MASS, &baseUnitValue, &lDampValue, lSimState, lRangHiLim, lCutoffPer, lDampTime);
	if (IsPVMatchWithQ(CDT_STEAM_MASS))
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

	//Damping
	mVMeasurementDynamicDuplicated.QmDamped = lDampValue;
	mVMeasurementDynamicDuplicated.QmDampedDuplicated = lDampValue;

	//percentage
	mVMeasurementDynamicDuplicated.QmPercentage = (100.0f * lDampValue) / lRangHiLim;// Calculation
	mVMeasurementDynamicDuplicated.QmPercentageDuplicated =  mVMeasurementDynamicDuplicated.QmPercentage;
	
	return baseUnitValue;
}