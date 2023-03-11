//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include "math.h"
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "T_Unit/Interface/t_unit.h"

#include "SVMeasurement/Interface/SVMeasurement.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
//lint -e{91}
#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"
#include "mVMeasurement_execute.h"
#include "splint_SplineCubicInterpolation.h"

#include ".\IAPWS_IF97_API.H"

#include "Coordinator/Interface/subsystem_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
 
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
//Mapper Subsystem
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Source/Mapper_Measurement.h"


#include "Common/Interface/common_functions.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"

#include "gasQvPartial.h"
#include "gasQnPartial.h"
#include "gasQn.h"
#include "gasQm.h"
#include "gasQp.h"
#include "liquidQn.h"
#include "liquidQm.h"
#include "liquidQp.h"
#include "SteamQm.h"
#include "SteamQp.h"

const TFLOAT pressureRefTable[]=
{
	101.3529322f,//14.7 psia
	101.3529322f, //14.7 psia
	101.325f, // kPa
	101.325f,//kPa
};

const TFLOAT temperatureRefTable[]=
{
   	15.555555556f,//60��F (15.555555556��C) ��UK)
   	21.111111111f,  //70 ��F (21.111111111��C) ��US)
	0.0f,
	20.0f
};



//---------------------------------------------------------------------------
const T_SELECTION MVMQvPartialSelection = MAPPER_SELECTION_GASPARTIALQV;
CONSTRUCTOR_T_ATRLIST(MVMQvPartial) =
{
	(TUSIGN16)MVM_IDX_QvPartialDampedObj,		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QvPartialDampedObj,		(TINT16)ATTRIB_0,
	(TUSIGN16)MVM_IDX_QvPartialDampedObj,		(TINT16)ATTRIB_1,
	(TUSIGN16)MVM_IDX_QvPartialClassification, 	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QvPartialStatus,         	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QvPartialDampingTime,    	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QvPartialRangeObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QvPartialRangeObj, 		(TINT16)ATTRIB_3,
	(TUSIGN16)MVM_IDX_QvPartialPercentage, 		(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QvPartialRangeObj, 		(TINT16)ATTRIB_4,
	(TUSIGN16)MVM_IDX_QvPartialMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QMinDN, 					(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QvPartialMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QvPartialObj,		(TINT16)ATTRIB_2,
};

const T_SELECTION MVMQnPartialSelection = MAPPER_SELECTION_GASPARTIALQN;
CONSTRUCTOR_T_ATRLIST(MVMQnPartial) =
{
	(TUSIGN16)MVM_IDX_QnPartialDampedObj,     	(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnPartialDampedObj,      	(TINT16)ATTRIB_0,
	(TUSIGN16)MVM_IDX_QnPartialDampedObj,       (TINT16)ATTRIB_1,
	(TUSIGN16)MVM_IDX_QnPartialClassification, 	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnPartialStatus,         	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnPartialDampingTime,    	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnPartialRangeObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnPartialRangeObj, 		(TINT16)ATTRIB_3,
	(TUSIGN16)MVM_IDX_QnPartialPercentage, 		(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnPartialRangeObj, 		(TINT16)ATTRIB_4,
	(TUSIGN16)MVM_IDX_QnPartialMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QMinDN, 					(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnPartialMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnPartialObj,				(TINT16)ATTRIB_2,
};

const T_SELECTION MVMQnSelection = MAPPER_SELECTION_GASQN|MAPPER_SELECTION_LIQUIDQN|MAPPER_SELECTION_GASPARTIALQN|MAPPER_SELECTION_GASQM;
CONSTRUCTOR_T_ATRLIST(MVMQn) =
{
	(TUSIGN16)MVM_IDX_QnDampedObj,     	(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnDampedObj,      (TINT16)ATTRIB_0,
	(TUSIGN16)MVM_IDX_QnDampedObj,       (TINT16)ATTRIB_1,
	(TUSIGN16)MVM_IDX_QnClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnDampingTime,    (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnRangeObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnRangeObj, 		(TINT16)ATTRIB_3,
	(TUSIGN16)MVM_IDX_QnPercentage, 	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnRangeObj, 		(TINT16)ATTRIB_4,
	(TUSIGN16)MVM_IDX_QnMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QMinDN, 			(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QnMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QnObj,			(TINT16)ATTRIB_2,
};

const T_SELECTION MVMQmSelection = MAPPER_SELECTION_GASQM|MAPPER_SELECTION_LIQUIDQM|MAPPER_SELECTION_STEAMQM|MAPPER_SELECTION_LIQUIDPOWER|MAPPER_SELECTION_STEAMPOWER;
CONSTRUCTOR_T_ATRLIST(MVMQm) =
{
	(TUSIGN16)MVM_IDX_QmDampedObj,     	(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QmDampedObj,      (TINT16)ATTRIB_0,
	(TUSIGN16)MVM_IDX_QmDampedObj,      (TINT16)ATTRIB_1,
	(TUSIGN16)MVM_IDX_QmClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QmStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QmDampingTime,    (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QmRangeObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QmRangeObj, 		(TINT16)ATTRIB_3,
	(TUSIGN16)MVM_IDX_QmPercentage, 	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QmRangeObj, 		(TINT16)ATTRIB_4,
	(TUSIGN16)MVM_IDX_QmMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QMinDN, 			(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QmMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QmObj,			(TINT16)ATTRIB_2,
};

const T_SELECTION MVMQpSelection = MAPPER_SELECTION_GASPOWER|MAPPER_SELECTION_LIQUIDPOWER|MAPPER_SELECTION_STEAMPOWER;
CONSTRUCTOR_T_ATRLIST(MVMQp) =
{
	(TUSIGN16)MVM_IDX_QpDampedObj,     	(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QpDampedObj,      (TINT16)ATTRIB_0,
	(TUSIGN16)MVM_IDX_QpDampedObj,      (TINT16)ATTRIB_1,
	(TUSIGN16)MVM_IDX_QpClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QpStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QpDampingTime,    (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QpRangeObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QpRangeObj, 		(TINT16)ATTRIB_3,
	(TUSIGN16)MVM_IDX_QpPercentage, 	(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QpRangeObj, 		(TINT16)ATTRIB_4,
	(TUSIGN16)MVM_IDX_QpMaxDNObj, 		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QMinDN, 			(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MVM_IDX_QpMaxDNObj,		(TINT16)ATTRIB_2,
	(TUSIGN16)MVM_IDX_QpObj,			(TINT16)ATTRIB_2,
};

const T_DYNAMIC_CHECK_TABLE MVMDynamicVariablesTable[] = 
{
  //lint -e{545} this is legal usage in ANSI C.
  {(void *)&( mVMeasurementDynamicDuplicated.Qn), 					(void *)&(mVMeasurementDynamicDuplicated.QnDuplicated),					sizeof(	mVMeasurementDynamicDuplicated.QnDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnDamped), 			(void *)&(mVMeasurementDynamicDuplicated.QnDampedDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.QnDampedDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnPercentage), 		(void *)&(mVMeasurementDynamicDuplicated.QnPercentageDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.QnPercentageDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnPartial), 			(void *)&(mVMeasurementDynamicDuplicated.QnPartialDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.QnPartialDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnPartialDamped), 		(void *)&(mVMeasurementDynamicDuplicated.QnPartialDampedDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.QnPartialDampedDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnPartialPercentage), 	(void *)&(mVMeasurementDynamicDuplicated.QnPartialPercentageDuplicated),	sizeof(	mVMeasurementDynamicDuplicated.QnPartialPercentageDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.Qm), 					(void *)&(mVMeasurementDynamicDuplicated.QmDuplicated),					sizeof(	mVMeasurementDynamicDuplicated.QmDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QmDamped), 			(void *)&(mVMeasurementDynamicDuplicated.QmDampedDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.QmDampedDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QmPercentage), 		(void *)&(mVMeasurementDynamicDuplicated.QmPercentageDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.QmPercentageDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.Qp), 					(void *)&(mVMeasurementDynamicDuplicated.QpDuplicated),					sizeof(	mVMeasurementDynamicDuplicated.QpDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QpDamped), 			(void *)&(mVMeasurementDynamicDuplicated.QpDampedDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.QpDampedDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QpPercentage), 		(void *)&(mVMeasurementDynamicDuplicated.QpPercentageDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.QpPercentageDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.compressFactorR), 		(void *)&(mVMeasurementDynamicDuplicated.compressFactorRDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.compressFactorRDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.compressFactorS), 		(void *)&(mVMeasurementDynamicDuplicated.compressFactorSDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.compressFactorSDuplicated)},
  {(void *)&( mVMeasurementDynamicDuplicated.mvmAlarmSimulation), 	(void *)&(mVMeasurementDynamicDuplicated.mvmAlarmSimulationDuplicated),		sizeof(	mVMeasurementDynamicDuplicated.mvmAlarmSimulationDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.mvmDiagnosis), 		(void *)&(mVMeasurementDynamicDuplicated.mvmDiagnosisDuplicated),			sizeof(	mVMeasurementDynamicDuplicated.mvmDiagnosisDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.Hw), 					(void *)&(mVMeasurementDynamicDuplicated.HwDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.HwDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.Hc), 					(void *)&(mVMeasurementDynamicDuplicated.HcDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.HcDuplicated	)},

  {(void *)&( mVMeasurementDynamicDuplicated.QnMaxDN), 					(void *)&(mVMeasurementDynamicDuplicated.QnMaxDNDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.QnMaxDNDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QmMaxDN), 					(void *)&(mVMeasurementDynamicDuplicated.QmMaxDNDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.QmMaxDNDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QpMaxDN), 					(void *)&(mVMeasurementDynamicDuplicated.QpMaxDNDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.QpMaxDNDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QvPartialMaxDN), 					(void *)&(mVMeasurementDynamicDuplicated.QvPartialMaxDNDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.QvPartialMaxDNDuplicated	)},
  {(void *)&( mVMeasurementDynamicDuplicated.QnPartialMaxDN), 					(void *)&(mVMeasurementDynamicDuplicated.QnPartialMaxDNDuplicated),						sizeof(	mVMeasurementDynamicDuplicated.QnPartialMaxDNDuplicated	)}
  
};

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEQALARMEXE> \endif
 \brief  
 \param  perc PV  Percentage.
 \if @SubGen end@ <METHODHEADER UPDATEQALARMEXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
	test-date: 2015-10-28
	\n by: zuochen wang
	\n environment: C-SPY
 \n intention:
 \n result module test: Pass
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEQALARMEXE>
void UpdateQAlarmEXE_MVM(TFLOAT perc)
//@SubGen end@<METHOD UPDATEQALARMEXE>
{
    static TUSIGN32     alarm_previoustime1 = 0x00000000;
    static TUSIGN32     alarm_previoustime2 = 0x00000000;
    static TUSIGN32     alarm_previoustime3 = 0x00000000;
    static TBOOL        alarmMax = eFALSE;
    static TBOOL        alarmMin = eFALSE;
    static TBOOL        alarmMax103 = eFALSE;
    
	TFLOAT delayTime = 0.5;
	TFLOAT Hysteresis = 0.01f;
	TFLOAT baseUnitValue = perc;//%


	//ALARM FOR 103 
	CheckUpperRangeLimit_Alarm(103.0f,baseUnitValue, Hysteresis, delayTime,&alarm_previoustime1, &alarmMax103);
	//ALARM FOR Max 
	CheckUpperRangeLimit_Alarm( mVMeasurementStaticRare.QAlmRange.hiLim, baseUnitValue, Hysteresis, delayTime,&alarm_previoustime2, &alarmMax);
	//ALARM FOR Min 
	CheckLowerRangeLimit_Alarm( mVMeasurementStaticRare.QAlmRange.loLim, baseUnitValue, Hysteresis, delayTime,&alarm_previoustime3, &alarmMin);
	
	if(alarmMax == (TBOOL)eTRUE)
	{
	  MVM_SETDIAG_(MVM_ALM_FLOWRATE_REACH_QMAX,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
	else
	{
	  MVM_CLRDIAG_(MVM_ALM_FLOWRATE_REACH_QMAX,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
    
	if(alarmMin == (TBOOL)eTRUE)
	{
	  MVM_SETDIAG_(MVM_ALM_FLOWRATE_REACH_QMIN,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
	else
	{
	  MVM_CLRDIAG_(MVM_ALM_FLOWRATE_REACH_QMIN,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
    
    if(alarmMax103 == (TBOOL)eTRUE)
	{
	  MVM_SETDIAG_(MVM_ALM_FLOWRATE_GT_103,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
	else
	{
	  MVM_CLRDIAG_(MVM_ALM_FLOWRATE_GT_103,mVMeasurementDynamicDuplicated.mvmDiagnosis);
	}
    
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEBIAGASQVMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEBIAGASQVMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATEBIAGASQVMAXDN>
void UpdateBiagasQvMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATEBIAGASQVMAXDN>
{
	TFLOAT QvMaxDn = 80.00f;
	TFLOAT QvBiogasMaxDN;
	TFLOAT gasPresetContent = 60.0f;
	
	(void) sVMeasurement_Get((TUSIGN16)(TUSIGN16)SVM_IDX_QvMaxDN,(TINT16)WHOLE_OBJECT,&QvMaxDn);
	(void) mapper_Get((TUSIGN16)MAPPER_IDX_dvContentPreset,(TINT16)WHOLE_OBJECT,&gasPresetContent);
	
	QvBiogasMaxDN = (QvMaxDn * gasPresetContent)/100.0f;
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QvPartialMaxDN,(TINT16)WHOLE_OBJECT,&QvBiogasMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QvPartialMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QvBiogasMaxDN);
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCCOMPRESSFACTOR> \endif
 \brief  CompressFactor IN real State
 \if @SubGen end@ <METHODHEADER CALCCOMPRESSFACTOR> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD CALCCOMPRESSFACTOR>
void CalcCompressFactor_MVM(void)
//@SubGen end@<METHOD CALCCOMPRESSFACTOR>
{
  	TUSIGN8 gasRef = MVM_UK;
  	static TFLOAT y2a[MAX_M_P * MAX_N_T];
	TFLOAT CRReal = 0.00f;
	TFLOAT CFStandard = 0.00f;
	TFLOAT CFPreset = 1.00f;
	TFLOAT pStandard = 1.0f;
	TFLOAT tStandard = 0.0f;
	TFLOAT inTPreset = 0.0f;
	TFLOAT inPPreset = 1.0f;
	TUSIGN8 pSize = 0x03;
	TUSIGN8 tSize = 0x03;
	TFLOAT pArray[9];
	TFLOAT tArray[7];
	TFLOAT compressFactorArray[63];
  	TFLOAT inTReal = 0.0f;
	TFLOAT inPReal = 0.0f;
	TUSIGN8 matrixSize = MVM_SIZE9X7;
	TUSIGN8 gasConfFlag = MVM_NONGONFIGED;
	TUSIGN8 updateCFSRequest = MVM_NONGONFIGED;
	TUSIGN8 spline2dRequest  = MVM_NONGONFIGED;
	TUSIGN8 updateCFPresetRequest  = MVM_NONGONFIGED;
	
	(void) mapper_Get((TUSIGN16)MAPPER_IDX_dvPressure,(TINT16)WHOLE_OBJECT,&inPReal);
	(void) coordinator_Get((TUSIGN16)CDT_IDX_temperature,(TINT16)WHOLE_OBJECT,&inTReal);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_compressFactorArray,(TINT16)WHOLE_OBJECT,compressFactorArray);
		//Set Value to pArray
	//DISABLE TASK SIWTCH
	SuspendAll_RTOS_TASK();
	pArray[0] = mVMeasurementStaticRare.CFPressArray0;
	pArray[1] = mVMeasurementStaticRare.CFPressArray1;
	pArray[2] = mVMeasurementStaticRare.CFPressArray2;
	pArray[3] = mVMeasurementStaticRare.CFPressArray3;
	pArray[4] = mVMeasurementStaticRare.CFPressArray4;
	pArray[5] = mVMeasurementStaticRare.CFPressArray5;
	pArray[6] = mVMeasurementStaticRare.CFPressArray6;
	pArray[7] = mVMeasurementStaticRare.CFPressArray7;
	pArray[8] = mVMeasurementStaticRare.CFPressArray8;
	
	//SET VAlue to T Array
	tArray[0] = mVMeasurementStaticRare.CFTempArray0;
	tArray[1] = mVMeasurementStaticRare.CFTempArray1;
	tArray[2] = mVMeasurementStaticRare.CFTempArray2;
	tArray[3] = mVMeasurementStaticRare.CFTempArray3;
	tArray[4] = mVMeasurementStaticRare.CFTempArray4;
	tArray[5] = mVMeasurementStaticRare.CFTempArray5;
	tArray[6] = mVMeasurementStaticRare.CFTempArray6;
	
	matrixSize = mVMeasurementStaticRare.matrixSize;
	gasConfFlag = mVMeasurementStaticRare.gasConfFlag;
	updateCFSRequest = mVMeasurementProtected.updateCFSRequest;
	spline2dRequest = mVMeasurementProtected.spline2dRequest;
	updateCFPresetRequest = mVMeasurementProtected.updateCFPresetRequest;
	gasRef = mVMeasurementStaticFrequentStaticDefault.gasRef;
    
    ResumeAll_RTOS_TASK();
	if(matrixSize == MVM_SIZE3X3)
	{
	  pSize = 0x03;
	  tSize = 0x03;
	}
	else if(matrixSize == MVM_SIZE4X3)
	{
	  pSize = 0x04;
	  tSize = 0x03;
	}
	else// MVM_SIZE9X7
	{
	  pSize = 0x09;
	  tSize = 0x07;
	}
	
	if(gasConfFlag == MVM_GONFIGED)
	{
		if(spline2dRequest  != MVM_GONFIGED )
		{
			spline2dRequest  = MVM_GONFIGED;
			spline2d((TFLOAT *)tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,  tSize,pSize, y2a);
		}
	}
	//If configed change calculated the compressfactor for standard status.
	if((updateCFSRequest == MVM_GONFIGING)&&(spline2dRequest == MVM_GONFIGED))
	{
		updateCFSRequest = MVM_GONFIGED;//Just Once
		pStandard = pressureRefTable[gasRef]; 
		tStandard = temperatureRefTable[gasRef];
		
		spline2d((TFLOAT *)tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,  tSize,pSize, y2a);
		splineInterpolate2d((TFLOAT *) tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,y2a, tSize, pSize, tStandard,pStandard, &CFStandard);
		//put value 
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_compressFactorS,(TINT16)WHOLE_OBJECT,&CFStandard);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_compressFactorSDuplicated,(TINT16)WHOLE_OBJECT,&CFStandard);
	}
	//. if Preset temperature and Pressure change calculate the compressfactor for Preset status.
	if( (updateCFPresetRequest == MVM_GONFIGING) && (spline2dRequest == MVM_GONFIGED))
	{
		updateCFPresetRequest = MVM_GONFIGED;
		//get preset pressure and tempereture
		(void) coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&inTPreset);
		(void) mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&inPPreset);
		spline2d((TFLOAT *)tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,  tSize,pSize, y2a);
		splineInterpolate2d((TFLOAT *) tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,y2a,  tSize,pSize, inTPreset,inPPreset, &CFPreset);
		//put value 
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_compressFactorPreset,(TINT16)WHOLE_OBJECT,&CFPreset);
	}
	
	if(spline2dRequest == MVM_GONFIGED)
	 {
		 //calculated the real status compress factor.
		spline2d((TFLOAT *)tArray,(TFLOAT *)pArray, (TFLOAT *)compressFactorArray,  tSize,pSize, y2a);
	    splineInterpolate2d( (TFLOAT *)tArray, (TFLOAT *)pArray,(TFLOAT *)compressFactorArray,y2a,tSize, pSize,  inTReal,inPReal, &CRReal);
		 //put value
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_compressFactorR,(TINT16)WHOLE_OBJECT,&CRReal);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_compressFactorRDuplicated,(TINT16)WHOLE_OBJECT,&CRReal);
	 }
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEQMMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEQMMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATEQMMAXDN>
void UpdateQmMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATEQMMAXDN>
{
	TFLOAT density = 1.0f;
    TFLOAT QvMaxDn = 80.00f;
    TFLOAT QmMaxDn = 80.00f;
	(void)mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT,&density);
	(void)unitsOfMeasure.ConvertFromBaseUnit(MVM_T_MASS_DENSITY,UM_KILOGRAM_PER_CUBIC_METER,&density); 
	//Get Volume flow
	(void) sVMeasurement_Get((TUSIGN16)SVM_IDX_QvMaxDN,(TINT16)WHOLE_OBJECT,&QvMaxDn);
	QmMaxDn = QvMaxDn * density;     
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QmMaxDN,(TINT16)WHOLE_OBJECT,&QmMaxDn);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QmMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QmMaxDn);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATELIQUIDQNMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATELIQUIDQNMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATELIQUIDQNMAXDN>
void UpdateLiquidQnMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATELIQUIDQNMAXDN>
{
  	  TFLOAT inTPreset = 0.0f;
	  TFLOAT QvMaxDn = 80.00f;
	  TFLOAT QnMaxDN = 80.00f;
	  TFLOAT refTemperature = 20.0f;
	  TFLOAT volumeExpandBeta1 = 0.01;
	  (void) coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&inTPreset);
	  (void) sVMeasurement_Get((TUSIGN16)SVM_IDX_QvMaxDN,(TINT16)WHOLE_OBJECT,&QvMaxDn);
	  (void) mVMeasurement_Get((TUSIGN16)MVM_IDX_refTemperature,(TINT16)WHOLE_OBJECT,&refTemperature);
	  (void) mVMeasurement_Get((TUSIGN16)MVM_IDX_volumeExpandBeta1,(TINT16)WHOLE_OBJECT,&volumeExpandBeta1);
  	  QnMaxDN = QvMaxDn / (1.0 + ((inTPreset - refTemperature) * volumeExpandBeta1));
	  (void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnMaxDN,(TINT16)WHOLE_OBJECT,&QnMaxDN);
	  (void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QnMaxDN);

}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEGASQNMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEGASQNMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATEGASQNMAXDN>
void UpdateGasQnMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATEGASQNMAXDN>
{
	TFLOAT inTPreset = 0.0f;
	TFLOAT inPPreset = 1.0f;
	TFLOAT QvMaxDn = 80.00f;
	TFLOAT QnMaxDN = 80.00f;
	TFLOAT refTemperature = 20.0f;
	TFLOAT refPressure = 101.325f;
	TUSIGN8 gasRef = MVM_UK;
	TFLOAT compressFactorS = 1.0f;
	TFLOAT compressFactorPreset = 1.0f;
    TUSIGN8 gasConfFlag = MVM_NONGONFIGED;
    TUSIGN8 updateCFSRequest = MVM_NONGONFIGED;
	static TUSIGN8 gasRefOld = 0x08;
	TFLOAT QnBiogasMaxDN;
	TFLOAT QvBiogasMaxDN;
	TFLOAT gasPresetContent = 60.0f;
    TUSIGN8 gasComputeType = MVM_IDEAL_GAS;
	
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_gasComputeType,(TINT16)WHOLE_OBJECT,&gasComputeType);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_gasConfFlag,(TINT16)WHOLE_OBJECT,&gasConfFlag);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_gasRef,(TINT16)WHOLE_OBJECT,&gasRef);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_compressFactorS,(TINT16)WHOLE_OBJECT,&compressFactorS);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_compressFactorPreset,(TINT16)WHOLE_OBJECT,&compressFactorPreset);

	(void) coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&inTPreset);
	(void) mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&inPPreset);
	(void) sVMeasurement_Get((TUSIGN16)SVM_IDX_QvMaxDN,(TINT16)WHOLE_OBJECT,&QvMaxDn);
	(void) mapper_Get((TUSIGN16)MAPPER_IDX_dvContentPreset,(TINT16)WHOLE_OBJECT,&gasPresetContent);
	
	refPressure = pressureRefTable[gasRef]; 
    refTemperature = temperatureRefTable[gasRef];
	
	//CALCULATE THE Qn_maxDN. AT Preset status.  
	if((MVM_GONFIGED != gasConfFlag)||(gasComputeType == MVM_IDEAL_GAS)) 
	{
		QnMaxDN = (QvMaxDn  * inPPreset * (refTemperature + (TFLOAT)273.15) ) / (( inTPreset + (TFLOAT)273.15) * refPressure);       
	}
	else
	{
		QnMaxDN =( QvMaxDn * inPPreset * (refTemperature + (TFLOAT)273.15) * compressFactorS) / ( (inTPreset + (TFLOAT)273.15) * refPressure * compressFactorPreset);       
	}
	QnBiogasMaxDN = (QnMaxDN * gasPresetContent)/100.0f;
	QvBiogasMaxDN = (QvMaxDn * gasPresetContent)/100.0f;
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnMaxDN,(TINT16)WHOLE_OBJECT,&QnMaxDN);
    (void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QnMaxDN);
    
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnPartialMaxDN,(TINT16)WHOLE_OBJECT,&QnBiogasMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QnPartialMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QnBiogasMaxDN);
    
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QvPartialMaxDN,(TINT16)WHOLE_OBJECT,&QvBiogasMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QvPartialMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QvBiogasMaxDN);
    
	if(gasRefOld != gasRef)
	{
	  	gasRefOld = gasRef;
		updateCFSRequest = MVM_GONFIGING;//Just Once

		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_updateCFSRequest,(TINT16)WHOLE_OBJECT,&updateCFSRequest);
	}
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATELIQUIDPOWERMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATELIQUIDPOWERMAXDN> \endif
 \author zuochen wang
 \date  2014-01-16
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATELIQUIDPOWERMAXDN>
void UpdateLiquidPowerMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATELIQUIDPOWERMAXDN>
{
  	TFLOAT QmMaxDN = 80.00f;
  	TFLOAT QpMaxDN = 80.00f;
	TFLOAT heatCapacity = 36.0f;


	//we consume that the biggest difference of the temperature t is 80 Deg. Deg work as baseunit.
	UpdateQmMaxDN_MVM();
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_QmMaxDN,(TINT16)WHOLE_OBJECT,&QmMaxDN);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_heatCapacity,(TINT16)WHOLE_OBJECT,&heatCapacity);
	//HeatCapacity unit is KJ/(kg.*deg). so Power Is KJ/h. 
	//We think the great difference temperature is  120 deg.
	QpMaxDN = QmMaxDN * heatCapacity * 80.0f;
	//add conversion to baseunit here.
	(void)unitsOfMeasure.ConvertToBaseUnit(MVM_T_POWER,MVM_KILOJOULE_PER_HOUR,&QpMaxDN); 
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDN,(TINT16)WHOLE_OBJECT,&QpMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QpMaxDN);
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEGASPOWERMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEGASPOWERMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATEGASPOWERMAXDN>
void UpdateGasPowerMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATEGASPOWERMAXDN>
{
  	  //KJ/m3 * m3/h = KJ/h.
    TFLOAT QnMaxDn = 80.00f;
	TFLOAT calorificEnergy = 36.0f; 
    TFLOAT QpMaxDN = 80.0f;

	//Get Standard Volume flow of Standard MaxDN value.
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_QnMaxDN,(TINT16)WHOLE_OBJECT,&QnMaxDn);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_calorificEnergy,(TINT16)WHOLE_OBJECT,&calorificEnergy);

	QpMaxDN = QnMaxDn *  calorificEnergy * 1000.0f;//MJ/m3.
	//add conversion to baseunit here.
	(void)unitsOfMeasure.ConvertToBaseUnit(MVM_T_POWER,MVM_KILOJOULE_PER_HOUR,&QpMaxDN); 			
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDN,(TINT16)WHOLE_OBJECT,&QpMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QpMaxDN);
}
//-------------------------------------------------------------------------------------------------
 /*!
 \brief  Calcualte the Preset H value for T1 and T2.
 \param  inPReal real  Pressure
 \param  inT1Real real Temperature
 \param  inT2Real real Temperature
 \author Zuochen.Wang
 \date
 \param void
 \return void
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
void CalcSteamHPreset()
{
   	//each measurement specific formula should be implemented here
	TFLOAT mTReal = 20.0f;//temperature read from sensor.
	TFLOAT mTExt = 20.0f;//externl temperature
	TFLOAT psSelf = 1.50f;
	TFLOAT tSelf = 27.50f;
	TFLOAT mPReal = 0.101325f;//pressure
	TFLOAT mPExt = 0.101325f;//pressure
	TFLOAT volume = 1.0f;
	TINT16  region = 1;
	TFLOAT  Hw = 1.0f;
	TFLOAT  Hc = 0.0f;

	TFLOAT fPressue = 0.101325f;
	TFLOAT fTInternal = 20.0f;
	TFLOAT fTExternal = 1.0f;
	TUSIGN8 steamType = MVM_STEAMTYPE_SATURATED;
    TUSIGN8  steamPwrrFR = MVM_FWD_ONLY;
	TUSIGN8 densitySelection = CDT_CALCULATION_FROM_T;
    TUSIGN8 hwUpdate = eFALSE;


	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType);
    (void) coordinator_Get((TUSIGN16)CDT_IDX_actualDensitySelection,(TINT16)WHOLE_OBJECT,&densitySelection);    
    (void) coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&fTInternal);
    (void) mapper_Get((TUSIGN16)MAPPER_IDX_dvPressurePreset,(TINT16)WHOLE_OBJECT,&fPressue);	
    (void) mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtPreset,(TINT16)WHOLE_OBJECT,&fTExternal);
	
    (void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_PRESSURE,UM_MEGAPASCAL,&fPressue); 
    (void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_TEMPERATURE,UM_KELVIN,&fTInternal); 
    (void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_TEMPERATURE,UM_KELVIN,&fTExternal);

    if(MVM_STEAMTYPE_SATURATED == steamType)
    {
        //lint -e736
        if(CDT_CALCULATION_FROM_P == densitySelection)
        {
			/* MVM_SATURATED_STEAM */
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPressue); //get the saturation temperature

            mTReal = tSelf;
			mPReal = fPressue*0.99999;
        }
        else if (CDT_CALCULATION_FROM_T == densitySelection)
        {
			/* MVM_SATURATED_STEAM */
			hwUpdate = eTRUE;

			psSelf = pbt_(&fTInternal); //get the saturation pressure

			mTReal = fTInternal;
            mPReal = psSelf*0.99999;
        }
		else if(CDT_CALCULATION_FROM_TP == densitySelection)
        {
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPressue);
			if(fTInternal <= (tSelf + 0.001))
			{
				// MVM_WATER: calculate it as saturation steam based on the temperature
				// MVM_SATURATED_STEAM: calculate it as it is: staturated steam

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*0.99999;
			}
			else
			{
				// MVM_OVERHEAT_STEAM:
				// calculate it as it is: overheated steam

				mTReal = fTInternal;
				mPReal = fPressue*0.99999;
			}		
        }
		else
		{
			hwUpdate = eFALSE;
		}
    }
	else if( steamType == MVM_STEAMTYPE_OVERHEAT )
	{
		if(CDT_CALCULATION_FROM_TP == densitySelection)
        {
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPressue);
			if(fTInternal <= (tSelf - 0.001))
			{
				// MVM_WATER or MVM_SATURATED_STEAM
				// calculate it as saturation steam based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*0.99999;
			}
			else
			{
				// MVM_OVERHEAT_STEAM:
				// calculate it as it is: overheated steam 

				mTReal = fTInternal;
				mPReal = fPressue*0.99999;
			}
        }
		else
		{
			hwUpdate = eFALSE;
		}
	}
	else
	{
		//Shall be water.
		if(CDT_CALCULATION_FROM_TP == densitySelection)
		{
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPressue);
			if(fTInternal < (tSelf - 0.001))
			{
				// MVM_WATER:
				// calculate it as it is: hot water 

				mTReal = fTInternal;
				mPReal = fPressue*1.001;
			}
			else 
			{
				// MVM_OVERHEAT_STEAM or MVM_SATURATED_STEAM
				// calculate it as saturation steam based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*1.001;
			}
		}	
		else if (CDT_CALCULATION_FROM_T == densitySelection)
		{
			if((fTInternal >= 300.15) && (fTInternal <=647.096))
			{
				hwUpdate = eTRUE;

				// calculate it as saturation water based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*1.001;
			}
		}
		else
		{
			hwUpdate = eFALSE;
		}	
	}
	
	if(hwUpdate)
	{
		(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_HwPreset,(TINT16)WHOLE_OBJECT,&Hw);
    	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_HcPreset,(TINT16)WHOLE_OBJECT,&Hc);
		(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_steamPwrrFR, (TINT16)WHOLE_OBJECT, &steamPwrrFR);

		(void)regsopt_( &mPReal, &mTReal, &region);
		switch ( region )
		{	
			case 1:
				Hw = (TFLOAT) hpt1n_(&mPReal,&mTReal);     
			break;
				
			case 2:
				Hw = (TFLOAT) hpt2n_(&mPReal,&mTReal);     
			break;
				
			case 3:
				volume = vpt3n_(&mPReal,&mTReal); 
				Hw = (TFLOAT) hvt3n_(&volume,&mTReal);     
			break;
		
			default: 
			break;
		}

		if(MVM_FWD_REV == steamPwrrFR)
		{
			// assume the reverse pressure is the same as the forward pressure calculated
			tSelf =(TFLOAT) tsatpn_(&mPReal);
			if(fTExternal < (tSelf - 0.001))
			{
				// it is water when using the forward pressure and reverse temperature
				mTExt = fTExternal;
				mPExt = mPReal;
			}
			else
			{
				// it is not water when using the forward pressure and reverse temperature
				// regarding it as saturation water to calculate parametere based on reverse temperature
				mTExt = fTExternal;
				mPExt = pbt_(&fTExternal)*1.001f;
			}

			(void)regsopt_( &mPExt, &mTExt, &region);
			switch ( region )
			{       
				case 1:
					Hc = (TFLOAT) hpt1n_(&mPExt,&mTExt);  
				break;
				
				case 3:
					volume = vpt3n_(&mPExt,&mTExt); 
					Hc = (TFLOAT) hvt3n_(&volume,&mTExt);    
				break;
			
				default: 
				break;
			} 
		}
		else
		{
			Hc = 0;
		}

		//lint +e736	
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_HwPreset,(TINT16)WHOLE_OBJECT,&Hw);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_HcPreset,(TINT16)WHOLE_OBJECT,&Hc);
	}
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATESTEAMPOWERMAXDN> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATESTEAMPOWERMAXDN> \endif
 \author zuochen wang
 \date  2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATESTEAMPOWERMAXDN>
void UpdateSteamPowerMaxDN_MVM(void)
//@SubGen end@<METHOD UPDATESTEAMPOWERMAXDN>
{
    TFLOAT QmMaxDN = 80.0f ;
    TFLOAT QpMaxDN = 80.0f;
	TFLOAT HwPreset = 2820.0f;
    TFLOAT HcPreset = 2800.0f;

  	UpdateQmMaxDN_MVM();
	CalcSteamHPreset();
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_QmMaxDN,(TINT16)WHOLE_OBJECT,&QmMaxDN);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_HwPreset,(TINT16)WHOLE_OBJECT,&HwPreset);
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_HcPreset,(TINT16)WHOLE_OBJECT,&HcPreset);

	//HeatCapacity unit is KJ/(kg). so Power Is kJ/h. 
	QpMaxDN = QmMaxDN * (TFLOAT)fabs((TDOUBLE)(HwPreset-HcPreset));
	//add conversion to baseunit here.
	(void) unitsOfMeasure.ConvertToBaseUnit(MVM_T_POWER,MVM_KILOJOULE_PER_HOUR,&QpMaxDN); 	
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDN,(TINT16)WHOLE_OBJECT,&QpMaxDN);
	(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_QpMaxDNDuplicated,(TINT16)WHOLE_OBJECT,&QpMaxDN);
}
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATEGASQVPARTIALEXE> \endif
 \brief  volume flow of Biogas
 \param  inVF volume flow in m3/h
 \param  biogasPerc the content of biogas .in percentage.
 \if @SubGen end@ <METHODHEADER CALCULATEGASQVPARTIALEXE> \endif
 \author  zuochen Wang
 \date   2013-03-28
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATEGASQVPARTIALEXE>
TFLOAT CalculateGasQvPartialEXE_MVM(TFLOAT inVF, TFLOAT biogasPerc)
//@SubGen end@<METHOD CALCULATEGASQVPARTIALEXE>
{
	return  CalculateGasQvPartial( inVF, biogasPerc);
}
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATELIQUIDQNEXE> \endif
 \brief  normal volume flow in m3/h
 \param  inVF volume flow in m3/h
 \param  tReal the real temperaturein base unit.
 \if @SubGen end@ <METHODHEADER CALCULATELIQUIDQNEXE> \endif
 \author zuochen Wang
 \date  2013-04-15
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATELIQUIDQNEXE>
TFLOAT CalculateLiquidQnEXE_MVM(TFLOAT inVF, TFLOAT tReal)
//@SubGen end@<METHOD CALCULATELIQUIDQNEXE>
{
  return CalculateLiquidQn( inVF,  tReal);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCSTEAMHEXE> \endif
 \brief  
 \param  inPReal real  Pressure
 \param  inT1Real real Temperature
 \param  inT2Real real Temperature
 \if @SubGen end@ <METHODHEADER CALCSTEAMHEXE> \endif
 \author   zuochen wang
 \date     2013-04-15
 \param void
 \return void
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
//@SubGen start@<METHOD CALCSTEAMHEXE>
void CalcSteamHEXE_MVM(TFLOAT inPReal, TFLOAT inT1Real, TFLOAT inT2Real)
//@SubGen end@<METHOD CALCSTEAMHEXE>
{
   	//each measurement specific formula should be implemented here
	TFLOAT psSelf = 1.0f;// saturated pressure at vt5 meter
	TFLOAT tSelf = 1.0f;// saturated t at vt5 meter
	TFLOAT mTReal = 20.0f;//temperature read from sensor.
	TFLOAT mTExt = 20.0f;//externl temperature
	TFLOAT mPReal = 0.101325f;//pressure
	TFLOAT mPExt = 0.101325f;//pressure
	TINT16  region = 1;
    TUSIGN8  steamPwrrFR = MVM_FWD_ONLY;
	TUSIGN8 steamType = MVM_STEAMTYPE_SATURATED;
	TUSIGN8 densitySelection = CDT_CALCULATION_FROM_T;
    TUSIGN8 hwUpdate = eFALSE;
	TFLOAT  Hw = 1.0f;
	TFLOAT  Hc = 0.0f;

	TFLOAT volume = 1.0f; 
    
	TFLOAT fPReal = 0.101325f;
	TFLOAT fTInternal = 20.0f;
	TFLOAT fTExternal =1.0f;
	
	fTInternal = inT1Real;
	fTExternal = inT2Real;//from extern 
	fPReal = inPReal ; //from KPa to MPa
    
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType);
    (void) coordinator_Get((TUSIGN16)CDT_IDX_actualDensitySelection,(TINT16)WHOLE_OBJECT,&densitySelection);    
	(void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_PRESSURE,UM_MEGAPASCAL,&fPReal); 
	(void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_TEMPERATURE,UM_KELVIN,&fTInternal); 
	(void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_TEMPERATURE,UM_KELVIN,&fTExternal); 

    if(MVM_STEAMTYPE_SATURATED == steamType)
    {
        //lint -e736
        if(CDT_CALCULATION_FROM_P == densitySelection)
        {
			/* MVM_SATURATED_STEAM */
			hwUpdate = eTRUE;
			
			tSelf =(TFLOAT) tsatpn_(&fPReal); //get the saturation temperature

			mTReal = tSelf;
			mPReal = fPReal*0.99999;
        }
        else if (CDT_CALCULATION_FROM_T == densitySelection)
		{
			/* MVM_SATURATED_STEAM */
			hwUpdate = eTRUE;

			psSelf = pbt_(&fTInternal); //get the saturation pressure

			mTReal = fTInternal;
			mPReal = psSelf*0.99999;
		} 
		else if(CDT_CALCULATION_FROM_TP == densitySelection)
        {
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPReal);
			if(fTInternal <= (tSelf + 0.001))
			{
				// MVM_WATER: calculate it as saturation steam based on the temperature
				// MVM_SATURATED_STEAM: calculate it as it is: staturated steam

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*0.99999;
			}
			else
			{
				// MVM_OVERHEAT_STEAM:
				// calculate it as it is: overheated steam 

				mTReal = fTInternal;
				mPReal = fPReal*0.99999;
			}	
        }
		else
		{
			hwUpdate = eFALSE;
		}
    }
	else if(steamType == MVM_STEAMTYPE_OVERHEAT)
	{
 		if(CDT_CALCULATION_FROM_TP == densitySelection)
        {
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPReal);
			if(fTInternal <= (tSelf + 0.001))
			{
				// MVM_WATER or MVM_SATURATED_STEAM
				// calculate it as saturation steam based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*0.99999;
			}
			else
			{
				// MVM_OVERHEAT_STEAM:
				// calculate it as it is: overheated steam 

				mTReal = fTInternal;
				mPReal = fPReal*0.99999;
			}
        }
		else
		{
			hwUpdate = eFALSE;
		}	  
	}
	else
	{
		//Shall be water.
 		if(CDT_CALCULATION_FROM_TP == densitySelection)
        {
			hwUpdate = eTRUE;

			tSelf =(TFLOAT) tsatpn_(&fPReal);
			if(fTInternal < (tSelf - 0.001))
			{
				// MVM_WATER:
				// calculate it as it is: hot water 

				mTReal = fTInternal;
				mPReal = fPReal*1.001;
			}
			else
			{
				// MVM_OVERHEAT_STEAM or MVM_SATURATED_STEAM
				// calculate it as saturation steam based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*1.001;
			}
        }
		else if (CDT_CALCULATION_FROM_T == densitySelection)
        {
			if((fTInternal >= 300.15) && (fTInternal <=647.096))
			{
				hwUpdate = eTRUE;

				// calculate it as saturation water based on the temperature

				psSelf = pbt_(&fTInternal);

				mTReal = fTInternal;
				mPReal = psSelf*1.001;
			}
        }
		else
		{
			hwUpdate = eFALSE;
		}	
	}
	
	if(hwUpdate)
	{
		(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_Hw,(TINT16)WHOLE_OBJECT,&Hw);
    	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_Hc,(TINT16)WHOLE_OBJECT,&Hc);
		(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_steamPwrrFR, (TINT16)WHOLE_OBJECT, &steamPwrrFR); 

		(void)regsopt_( &mPReal, &mTReal, &region);
		switch ( region )
		{	
			case 1:
				Hw = (TFLOAT) hpt1n_(&mPReal,&mTReal);     
			break;
				
			case 2:
				Hw = (TFLOAT) hpt2n_(&mPReal,&mTReal);     
			break;
				
			case 3:
				volume = vpt3n_(&mPReal,&mTReal); 
				Hw = (TFLOAT) hvt3n_(&volume,&mTReal);     
			break;
		
			default: 
			break;  
		}
		
		if(MVM_FWD_REV == steamPwrrFR)
		{        
			// assume the reverse pressure is the same as the forward pressure calculated
			tSelf =(TFLOAT) tsatpn_(&mPReal);
			if(fTExternal < (tSelf - 0.001))
			{
				// it is water when using the forward pressure and reverse temperature
				mTExt = fTExternal;
				mPExt = mPReal;
			}
			else
			{
				// it is not water when using the forward pressure and reverse temperature
				// regarding it as saturation water to calculate parametere based on reverse temperature
				mTExt = fTExternal;
				mPExt = pbt_(&fTExternal)*1.001f;
			}

			(void)regsopt_( &mPExt, &mTExt, &region);
			switch ( region )
			{       
				case 1:
					Hc = (TFLOAT) hpt1n_(&mPExt,&mTExt);  
				break;
				
				case 3:
					volume = vpt3n_(&mPExt,&mTExt); 
					Hc = (TFLOAT) hvt3n_(&volume,&mTExt);    
				break;
			
				default: 
				break;
			} 
		}
		else
		{
			Hc = 0.0f;	  
		}
		
		 //lint +e736	
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_Hw,(TINT16)WHOLE_OBJECT,&Hw);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_HwDuplicated,(TINT16)WHOLE_OBJECT,&Hw);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_Hc,(TINT16)WHOLE_OBJECT,&Hc);
		(void) mVMeasurement_Put((TUSIGN16)MVM_IDX_HcDuplicated,(TINT16)WHOLE_OBJECT,&Hc);		
	}
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCSTEAMDENSITYEXE> \endif
 \brief  real Density
 \param  inTReal real Temperature
 \param  inPReal real  Pressure
 \if @SubGen end@ <METHODHEADER CALCSTEAMDENSITYEXE> \endif
 \author zuochen wang 
 \date   2015-05-12
 \param inTReal in baseunit Celsius
 \param inPReal in baseunit kPa
 \return Densigy Kg/m3
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
//@SubGen start@<METHOD CALCSTEAMDENSITYEXE>
TFLOAT CalcSteamDensityEXE_MVM(TFLOAT inTReal, TFLOAT inPReal)
//@SubGen end@<METHOD CALCSTEAMDENSITYEXE>
{
   	//each measurement specific formula should be implemented here
	TFLOAT psSelf = 1.0;// saturated pressure 
	TFLOAT tSelf = 1.0;// saturated t
	TFLOAT inputT = 20.0f, inputP = 0.101325;
	TFLOAT mTReal = 20.0f, mPReal = 0.101325;
	TINT16  region = 1;
	static TFLOAT density = 1.0f;
	static TUSIGN8 alarmCounter = 0;
	TDOUBLE volume = 1.0; 
        
    TUSIGN8 densitySelection = CDT_CALCULATION_FROM_T;
	TUSIGN8 steamStatus = 0xFF;
	TUSIGN8 steamType = MVM_STEAMTYPE_SATURATED;
	TUSIGN8 densityUpdate = eFALSE;

	const TUSIGN8 STEAM_NOT_MATCH_COUNTER = 5;


	inputP = inPReal;
	inputT = inTReal;
    
	(void) mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType);
	(void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_PRESSURE,UM_MEGAPASCAL,&inputP);  //from KPa to MPa
	(void) unitsOfMeasure.ConvertFromBaseUnit(UM_T_TEMPERATURE,UM_KELVIN,&inputT);  //from celsius to Kelvin
    (void) coordinator_Get((TUSIGN16)CDT_IDX_actualDensitySelection,(TINT16)WHOLE_OBJECT,&densitySelection);
	
	switch(steamType)
	{
		case MVM_STEAMTYPE_SATURATED:
		{
    		//lint -e736
			switch(densitySelection)
			{
				case CDT_CALCULATION_FROM_T:
				{
					densityUpdate = eTRUE;

					steamStatus = MVM_SATURATED_STEAM;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*0.99999;

					alarmCounter = 0;
				}
				break;

				case CDT_CALCULATION_FROM_P:
				{
					densityUpdate = eTRUE;

					steamStatus = MVM_SATURATED_STEAM;

					tSelf =(TFLOAT) tsatpn_(&inputP);

					mTReal = tSelf;
					mPReal = inputP*0.99999;

					alarmCounter = 0;
				}
				break;

				case CDT_CALCULATION_FROM_TP:
				{
					densityUpdate = eTRUE;

					tSelf =(TFLOAT) tsatpn_(&inputP);
					if(inputT < (tSelf - 0.001))
					{
						steamStatus = MVM_WATER;

						psSelf = (TFLOAT) pbt_(&inputT);

						mTReal = inputT;
						mPReal = psSelf*0.99999;

						alarmCounter ++;
					}
					else if((inputT >= (tSelf - 0.001)) && (inputT <= (tSelf + 0.001)))
					{
						steamStatus = MVM_SATURATED_STEAM;

						psSelf = (TFLOAT) pbt_(&inputT);

						mTReal = inputT;
						mPReal = psSelf*0.99999;

						alarmCounter = 0;
					}
					else 
					{
						steamStatus = MVM_OVERHEAT_STEAM;

						mTReal = inputT;
						mPReal = inputP*0.99999;

						alarmCounter = 0;
					}
				}
				break;

				default:
				break;
			}
		}
		break;

		case MVM_STEAMTYPE_OVERHEAT:
		{
			if(densitySelection == CDT_CALCULATION_FROM_TP)
			{
				densityUpdate = eTRUE;

				tSelf =(TFLOAT) tsatpn_(&inputP);
				if(inputT < (tSelf - 0.001))
				{
					steamStatus = MVM_WATER;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*0.99999;

					alarmCounter++;
				}
				else if((inputT >= (tSelf - 0.001)) && (inputT <= (tSelf + 0.001)))
				{
					steamStatus = MVM_SATURATED_STEAM;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*0.99999;

					alarmCounter = 0;
				}
				else 
				{
					steamStatus = MVM_OVERHEAT_STEAM;

					mTReal = inputT;
					mPReal = inputP*0.99999;

					alarmCounter = 0;
				}
			}
		}
		break;

		case MVM_STEAMTYPE_WATER:
		{
			if(densitySelection == CDT_CALCULATION_FROM_TP)
			{
				densityUpdate = eTRUE;

				tSelf =(TFLOAT) tsatpn_(&inputP);
				if(inputT < (tSelf - 0.001))
				{
					steamStatus = MVM_WATER;

					mTReal = inputT;
					mPReal = inputP*1.001;

					alarmCounter = 0;
				}
				else if((inputT >= (tSelf - 0.001)) && (inputT <= (tSelf + 0.001)))
				{
					steamStatus = MVM_SATURATED_STEAM;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*1.001;

					alarmCounter++;
				}
				else 
				{
					steamStatus = MVM_OVERHEAT_STEAM;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*1.001;

					alarmCounter++;
				}
			}
			else if (densitySelection ==CDT_CALCULATION_FROM_T)
			{
				if((inputT >= 300.15) && (inputT <=647.096))
				{
					densityUpdate = eTRUE;

					steamStatus = MVM_WATER;

					psSelf = (TFLOAT) pbt_(&inputT);

					mTReal = inputT;
					mPReal = psSelf*1.001;

					alarmCounter = 0;
				}
			}
			else
			{
			}
		}
		break;	

		default:
		break;
	}

	// avoid over flow
	alarmCounter = (alarmCounter >= (STEAM_NOT_MATCH_COUNTER+1)) ? (STEAM_NOT_MATCH_COUNTER+1) : alarmCounter;

    //lint +e736 -e731
	if(densityUpdate)
	{
		(void)mVMeasurement_Put((TUSIGN16)MVM_IDX_steamStatus,(TINT16)WHOLE_OBJECT,&steamStatus);
		if (MVM_STEAMTYPE_WATER == steamType)
		{
			if ((steamStatus != MVM_WATER) && (alarmCounter >= STEAM_NOT_MATCH_COUNTER))
			{
				MVM_SETDIAG_(MVM_ALM_STEAM_STATUS_MISMATCH,mVMeasurementDynamicDuplicated.mvmDiagnosis);
			}
			else
			{
				MVM_CLRDIAG_(MVM_ALM_STEAM_STATUS_MISMATCH,mVMeasurementDynamicDuplicated.mvmDiagnosis);
			}
		}
		else
		{
			if ((steamStatus == MVM_WATER) && (alarmCounter >= STEAM_NOT_MATCH_COUNTER))
			{
				MVM_SETDIAG_(MVM_ALM_STEAM_STATUS_MISMATCH,mVMeasurementDynamicDuplicated.mvmDiagnosis);
			}
			else
			{
				MVM_CLRDIAG_(MVM_ALM_STEAM_STATUS_MISMATCH,mVMeasurementDynamicDuplicated.mvmDiagnosis);
			}
		}

		//region decision.
		(void)regsopt_( &mPReal, &mTReal, &region);
		switch ( region )
		{		
			case 1:
				volume = vpt1n_(&mPReal,&mTReal);
			break;

			case 2:
				volume = vpt2n_(&mPReal,&mTReal);
			break;		
			
			case 3:
				volume = vpt3n_(&mPReal,&mTReal);
			break;

			default:
			break;
		}
	
		if(volume > 0.0)
		{
			density = 1.0/volume;
		}
		else
		{
		}    
	}

	return density;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETQNPERCENTAGESRV> \endif
 \brief  return the Qn percentage value
 \if @SubGen end@ <METHODHEADER GETQNPERCENTAGESRV> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD GETQNPERCENTAGESRV>
TFLOAT GetQnPercentageSRV_MVM(void)
//@SubGen end@<METHOD GETQNPERCENTAGESRV>
{
    return mVMeasurementDynamicDuplicated.QnPercentage;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETQMPERCENTAGESRV> \endif
 \brief  return the Qm percentage value
 \if @SubGen end@ <METHODHEADER GETQMPERCENTAGESRV> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD GETQMPERCENTAGESRV>
TFLOAT GetQmPercentageSRV_MVM(void)
//@SubGen end@<METHOD GETQMPERCENTAGESRV>
{
  return mVMeasurementDynamicDuplicated.QmPercentage;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETQPPERCENTAGESRV> \endif
 \brief  return the power percentage value
 \if @SubGen end@ <METHODHEADER GETQPPERCENTAGESRV> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD GETQPPERCENTAGESRV>
TFLOAT GetQpPercentageSRV_MVM(void)
//@SubGen end@<METHOD GETQPPERCENTAGESRV>
{
    return mVMeasurementDynamicDuplicated.QpPercentage;

}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETQNPARTIALPERCENTAGESRV> \endif
 \brief  return the partial Qv percentage value
 \if @SubGen end@ <METHODHEADER GETQNPARTIALPERCENTAGESRV> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD GETQNPARTIALPERCENTAGESRV>
TFLOAT GetQnPartialPercentageSRV_MVM(void)
//@SubGen end@<METHOD GETQNPARTIALPERCENTAGESRV>
{
    return mVMeasurementDynamicDuplicated.QnPartialPercentage;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETQVPARTIALPERCENTAGESRV> \endif
 \brief  return the partial Qn percentage value
 \if @SubGen end@ <METHODHEADER GETQVPARTIALPERCENTAGESRV> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD GETQVPARTIALPERCENTAGESRV>
TFLOAT GetQvPartialPercentageSRV_MVM(void)
//@SubGen end@<METHOD GETQVPARTIALPERCENTAGESRV>
{
      return mVMeasurementDynamicDuplicated.QvPartialPercentage;

}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATEGASQMEXE> \endif
 \brief  gas mass flow
 \param  inVF volume flow in m3/h
 \param  inNVF normal Volume flow
 \param  densityReal the real density in base unit.
 \if @SubGen end@ <METHODHEADER CALCULATEGASQMEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATEGASQMEXE>
TFLOAT CalculateGasQmEXE_MVM(TFLOAT inVF, TFLOAT inNVF, TFLOAT densityReal)
//@SubGen end@<METHOD CALCULATEGASQMEXE>
{
  return CalculateGasQm( inVF,  inNVF,  densityReal);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATEGASQPEXE> \endif
 \brief  Gas power flow
 \param  inVF volume flow in m3/h
 \if @SubGen end@ <METHODHEADER CALCULATEGASQPEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATEGASQPEXE>
TFLOAT CalculateGasQpEXE_MVM(TFLOAT inVF)
//@SubGen end@<METHOD CALCULATEGASQPEXE>
{
  return CalculateGasQp( inVF);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATELIQUIDQMEXE> \endif
 \brief  liquid mass flow in kg/h
 \param  inVF volume flow in m3/h
 \param  DensityReal real density
 \param  tReal the real temperaturein base unit.
 \if @SubGen end@ <METHODHEADER CALCULATELIQUIDQMEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATELIQUIDQMEXE>
TFLOAT CalculateLiquidQmEXE_MVM(TFLOAT inVF, TFLOAT DensityReal, TFLOAT tReal)
//@SubGen end@<METHOD CALCULATELIQUIDQMEXE>
{
  return CalculateLiquidQm( inVF,  DensityReal,  tReal);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATELIQUIDQPEXE> \endif
 \brief  liquid Power flow in kj/h
 \param  inVF volume flow in m3/h
 \param  T1Real the real temperature T1 in base unit.
 \param  T2Real real temperature T2
 \if @SubGen end@ <METHODHEADER CALCULATELIQUIDQPEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATELIQUIDQPEXE>
TFLOAT CalculateLiquidQpEXE_MVM(TFLOAT inVF, TFLOAT T1Real, TFLOAT T2Real)
//@SubGen end@<METHOD CALCULATELIQUIDQPEXE>
{
  return CalculateLiquidQp( inVF,  T1Real,  T2Real);
} 

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATESTEAMQMEXE> \endif
 \brief  Steam mass flow in kg/h
 \param  inVF volume flow in m3/h
 \param  DensityReal real density
 \if @SubGen end@ <METHODHEADER CALCULATESTEAMQMEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATESTEAMQMEXE>
TFLOAT CalculateSteamQmEXE_MVM(TFLOAT inVF, TFLOAT DensityReal)
//@SubGen end@<METHOD CALCULATESTEAMQMEXE>
{
  return CalculateSteamQm( inVF,  DensityReal);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATESTEAMQPEXE> \endif
 \brief  Steam Power  flow in kj/h
 \param  inMF mass flow in kg/h
 \if @SubGen end@ <METHODHEADER CALCULATESTEAMQPEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATESTEAMQPEXE>
TFLOAT CalculateSteamQpEXE_MVM(TFLOAT inMF)
//@SubGen end@<METHOD CALCULATESTEAMQPEXE>
{
  return CalculateSteamQp(inMF);
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATEGASQNEXE> \endif
 \brief  gas normal flow
 \param  volumeFlow volume flow in m3/h
 \param  tReal the real Tempearture in Celsius
 \param  pReal the real Pressure in base unit.
 \if @SubGen end@ <METHODHEADER CALCULATEGASQNEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
 \warning
 \test
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
//@SubGen start@<METHOD CALCULATEGASQNEXE>
TFLOAT CalculateGasQnEXE_MVM(TFLOAT volumeFlow, TFLOAT tReal, TFLOAT pReal)
//@SubGen end@<METHOD CALCULATEGASQNEXE>
{
  return CalculateGasQn( volumeFlow,  tReal,  pReal);
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CALCULATEGASQNPARTIALEXE> \endif
 \brief  Calculate Gas Qn Partial
 \param  inNVF normal volume flow in m3/h
 \param  biogasPerc the content of biogas .in percentage.
 \if @SubGen end@ <METHODHEADER CALCULATEGASQNPARTIALEXE> \endif
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
//@SubGen start@<METHOD CALCULATEGASQNPARTIALEXE>
TFLOAT CalculateGasQnPartialEXE_MVM(TFLOAT inNVF, TFLOAT biogasPerc)
//@SubGen end@<METHOD CALCULATEGASQNPARTIALEXE>
{
  return CalculateGasQnPartial(inNVF, biogasPerc);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Set  Diagnosis
 \param  attributeIdx 
 \author zuochen wang 
 \date   2013-04-25
 \param void
 \return void
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
void SetDiagnosis_MVM(TUSIGN16 attributeIdx)
{
    VIP_ASSERT( attributeIdx < (TINT16)MVM_DIAG_SIZE );
    VIP_ASSERT( (TINT16)attributeIdx >= 0 );

	//set local
	MVM_SETDIAG_((TUSIGN16)attributeIdx, mVMeasurementDynamicDuplicated.mvmDiagnosis);  
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief   Clear Diagnosis
 \param  attributeIdx 
 \date   2013-04-25
 \param void
 \param void
 \return void
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
void ClearDiagnosis_MVM(TUSIGN16 attributeIdx)
{
    VIP_ASSERT( attributeIdx < (TINT16)MVM_DIAG_SIZE );
    VIP_ASSERT( (TINT16)attributeIdx >= 0 );

	//clear local
	MVM_CLRDIAG_((TUSIGN16)attributeIdx, mVMeasurementDynamicDuplicated.mvmDiagnosis); // The Simulation Alarm do not need to affect local.
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEDIAGNOSISEXE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER UPDATEDIAGNOSISEXE> \endif
 \date   2013-04-25
 \param void
 \param void
 \return void
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
//@SubGen start@<METHOD UPDATEDIAGNOSISEXE>
void UpdateDiagnosisEXE_MVM(void)
//@SubGen end@<METHOD UPDATEDIAGNOSISEXE>
{
    TUSIGN16 result;
	TUSIGN8 i;
	TUSIGN8 alarm = 0x0000;
	TUSIGN8 alarmSimulation = 0x0000;

	TUSIGN8 setAlarm = SET_ALARM;
	TUSIGN8 clearAlarm = CLEAR_ALARM;
	TUSIGN8 diagnosisSimulationStatus = DIAGNOSIS_SIMULATION_DISABLED;
	TUSIGN8 setDiagnosis = 0x00;
	TUSIGN8 clearDiagnosis = 0x00;
	static TUSIGN8 oldAlarm = 0x00;

	
	ENABLE_DATA_PROTECTION(*(mVMeasurement.unit.ptrDataSemaphore));
	alarm =  MVM_OBJECT_LOCAL_DIAGNOSIS;
	DISABLE_DATA_PROTECTION(*(mVMeasurement.unit.ptrDataSemaphore));
	result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,&diagnosisSimulationStatus);
	
	if((diagnosisSimulationStatus == (TUSIGN8)DIAGNOSIS_SIMULATION_ENABLED) && (result == (TUSIGN16)OK))
	{
	  for (i=0; i<MVM_DIAG_SIZE; i++)
	  {
		TUSIGN8 alarmBitSimulation = 0x00;
		result = mVMeasurement_Get((TUSIGN16)MVM_IDX_mvmAlarmSimulation, i, &alarmBitSimulation);
		if ((result == (TUSIGN16)OK) && (alarmBitSimulation == (TUSIGN8)SET_ALARM))
		{
		  alarmSimulation |= (TUSIGN8)(1<<i);
		}
		else
		{
		}
	  }
	  alarm = alarmSimulation;
	}

    //only update it when it's changed
	setDiagnosis = alarm& ~oldAlarm;
	clearDiagnosis = ~ alarm&oldAlarm;
	oldAlarm = alarm;
	
	//Push Diagnosis to 
	for(i=0; i<MVM_DIAG_SIZE; i++)
	{
		if(setDiagnosis & ((TUSIGN8)0X01<<i))			
		{
			(void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarm,(TUSIGN16)i,&setAlarm); //Enable when DIAG subsystem add the alarm for SVM.
		}
		else if(clearDiagnosis & ((TUSIGN8)0X01<<i))
		{
			(void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarm,(TUSIGN16)i,&clearAlarm);
		}
		else 
		{

		}
	}

}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKDATACRCSRV> \endif
 \brief  CRC Check for RAM
 \if @SubGen end@ <METHODHEADER CHECKDATACRCSRV> \endif
 \date   2013-04-25
 \param void
 \param void
 \return void
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
//@SubGen start@<METHOD CHECKDATACRCSRV>
void CheckDataCrcSRV_MVM(void)
//@SubGen end@<METHOD CHECKDATACRCSRV>
{
	TUSIGN16 crc = InitCRC16_COMMON();
	ENABLE_DATA_PROTECTION(*(mVMeasurement.unit.ptrDataSemaphore));
	if(mVMeasurementProtected.protectedDataStatus == MVM_CALCULATE)
	{
		mVMeasurementProtected.protectedDataStatus = MVM_PROTECTED;
  		crc = CalcCrc16Mem_COMMON((TUSIGN8*)&mVMeasurementProtected,crc,sizeof(mVMeasurementProtected)/sizeof(TUSIGN8));
		mVMeasurementUnprotected.protectedDataCrc = crc;
	}
	else if(mVMeasurementProtected.protectedDataStatus == MVM_PROTECTED)
	{
  		crc = CalcCrc16Mem_COMMON((TUSIGN8*)&mVMeasurementProtected,crc,sizeof(mVMeasurementProtected)/sizeof(TUSIGN8));
		if(mVMeasurementUnprotected.protectedDataCrc != crc)
		{
			if(mVMeasurementUnprotected.protectedDataCrc != crc)
			{
				elettronicServices.ExitSRV();
			}
		}
	}
    else if( SVM_UNPROTECTED != mVMeasurementProtected.protectedDataStatus)
	{
        elettronicServices.ExitSRV();
	}
	else
	{
	}
	DISABLE_DATA_PROTECTION(*(mVMeasurement.unit.ptrDataSemaphore));
}

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
	test-date: 2013-06-28
	\n by: zuochen wang
	\n environment: C-SPY
 \n intention:
 \n result module test: Pass
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/