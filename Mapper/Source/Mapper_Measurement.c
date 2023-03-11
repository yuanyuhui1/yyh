//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module         Mapper Measurement,which mange the device variable mapping
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Totalizer/Interface/Totalizer.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_Config.h"
#include "Mapper_Measurement.h"

CONSTRUCTOR_T_ATRLIST(mapperT_External) =
{
	(TUSIGN16)MAPPER_IDX_dvTExtObj,            (TINT16)ATTRIB_2,
	(TUSIGN16)CDT_IDX_temperature_UnitCode,    (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvTExtUnitType,       (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvTExtClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvTExtStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyDampingTime,   (TINT16)WHOLE_OBJECT,

	(TUSIGN16)MAPPER_IDX_dvTExtRangeObj,       (TINT16)ATTRIB_2,
	(TUSIGN16)MAPPER_IDX_dvTExtRangeObj,       (TINT16)ATTRIB_3,
	(TUSIGN16)MAPPER_IDX_dvDummyPercentage,    (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvTExtRangeObj,       (TINT16)ATTRIB_4,
    
    (TUSIGN16)MAPPER_IDX_dvTExtMaxDN,          (TINT16)WHOLE_OBJECT,  
    (TUSIGN16)MAPPER_IDX_dvTExtMinDN,          (TINT16)WHOLE_OBJECT,
    (TUSIGN16)MAPPER_IDX_dvTExtRangeObj,       (TINT16)ATTRIB_4,
    (TUSIGN16)MAPPER_IDX_dvTExtObj,            (TINT16)ATTRIB_2,
};
const T_SELECTION mapperT_ExternalSelection = MAPPER_SELECTION_LIQUIDPOWER | MAPPER_SELECTION_STEAMPOWER;

CONSTRUCTOR_T_ATRLIST(mapperPressure) =
{
	(TUSIGN16)MAPPER_IDX_dvPressureObj,           (TINT16)ATTRIB_2,
	(TUSIGN16)MAPPER_IDX_dvPressureUnitCode,      (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvPressureUnitType,      (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvPressureClassification,(TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvPressureStatus,        (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyDampingTime,      (TINT16)WHOLE_OBJECT,

	(TUSIGN16)MAPPER_IDX_dvPressureRangeObj,      (TINT16)ATTRIB_2,
	(TUSIGN16)MAPPER_IDX_dvPressureRangeObj,      (TINT16)ATTRIB_3,
	(TUSIGN16)MAPPER_IDX_dvDummyPercentage,       (TINT16)WHOLE_OBJECT,
    (TUSIGN16)MAPPER_IDX_dvPressureRangeObj,      (TINT16)ATTRIB_4,
    (TUSIGN16)MAPPER_IDX_dvPressureMaxDN,         (TINT16)WHOLE_OBJECT,  
    (TUSIGN16)MAPPER_IDX_dvPressureMinDN,         (TINT16)WHOLE_OBJECT,    
    (TUSIGN16)MAPPER_IDX_dvPressureRangeObj,      (TINT16)ATTRIB_4,
    (TUSIGN16)MAPPER_IDX_dvPressureObj,           (TINT16)ATTRIB_2,
};
const T_SELECTION mapperPressureSelection = MAPPER_SELECTION_GASQN | MAPPER_SELECTION_GASPARTIALQN | \
                                            MAPPER_SELECTION_STEAMQM | MAPPER_SELECTION_STEAMPOWER;

CONSTRUCTOR_T_ATRLIST(mapperDensity) =
{
	(TUSIGN16)MAPPER_IDX_dvDensityObj,            (TINT16)ATTRIB_2,
	(TUSIGN16)MAPPER_IDX_dvDensityUnitCode,       (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDensityUnitType,       (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDensityClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDensityStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyDampingTime,      (TINT16)WHOLE_OBJECT,

	(TUSIGN16)MAPPER_IDX_dvDensityRangeObj,       (TINT16)ATTRIB_2,
	(TUSIGN16)MAPPER_IDX_dvDensityRangeObj,       (TINT16)ATTRIB_3,
	(TUSIGN16)MAPPER_IDX_dvDummyPercentage,       (TINT16)WHOLE_OBJECT,
    (TUSIGN16)MAPPER_IDX_dvDensityRangeObj,       (TINT16)ATTRIB_4,
    (TUSIGN16)MAPPER_IDX_dvDensityMaxDN,          (TINT16)WHOLE_OBJECT,  
    (TUSIGN16)MAPPER_IDX_dvDensityMinDN,          (TINT16)WHOLE_OBJECT,   
    (TUSIGN16)MAPPER_IDX_dvDensityRangeObj,       (TINT16)ATTRIB_4,
    (TUSIGN16)MAPPER_IDX_dvDensityObj,            (TINT16)ATTRIB_2,
};
const T_SELECTION mapperDensitySelection = MAPPER_SELECTION_LIQUIDQM | MAPPER_SELECTION_GASQM | \
                                            MAPPER_SELECTION_STEAMQM | MAPPER_SELECTION_STEAMPOWER;

CONSTRUCTOR_T_ATRLIST(mapperContent) =
{
	(TUSIGN16)MAPPER_IDX_dvContent,               (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyUnitCode,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyUnitType,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvContentClassification, (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvContentStatus,         (TINT16)WHOLE_OBJECT,
	(TUSIGN16)MAPPER_IDX_dvDummyDampingTime,      (TINT16)WHOLE_OBJECT,

	(TUSIGN16)MAPPER_IDX_dvContentRange,          (TINT16)ATTRIB_0,
	(TUSIGN16)MAPPER_IDX_dvContentRange,          (TINT16)ATTRIB_1,
	(TUSIGN16)MAPPER_IDX_dvDummyPercentage,       (TINT16)WHOLE_OBJECT,
    (TUSIGN16)MAPPER_IDX_dvContentRange,          (TINT16)ATTRIB_2,
    (TUSIGN16)MAPPER_IDX_dvContentMaxDN,          (TINT16)WHOLE_OBJECT,  
    (TUSIGN16)MAPPER_IDX_dvContentMinDN,          (TINT16)WHOLE_OBJECT,  
    (TUSIGN16)MAPPER_IDX_dvContentRange,          (TINT16)ATTRIB_2,
    (TUSIGN16)MAPPER_IDX_dvContent,               (TINT16)WHOLE_OBJECT,
};
const T_SELECTION mapperContentSelection = MAPPER_SELECTION_GASPARTIALQV | MAPPER_SELECTION_GASPARTIALQN;

//! Measurement List
const MapList MeasurementList[M_NUMOF_DEVICEVARIABLES] =
{  
  //device variables managed by SVM
  {SVMEASUREMENT_ME,	&(sVMeasurement.GetQvPercentageSRV), SVMQv,&SVMQvSelection},
  //device variables managed by MVM
  {MVMEASUREMENT_ME,	&(mVMeasurement.GetQnPercentageSRV), MVMQn,&MVMQnSelection},
  {MVMEASUREMENT_ME,	&(mVMeasurement.GetQmPercentageSRV), MVMQm,&MVMQmSelection},
  {MVMEASUREMENT_ME,	&(mVMeasurement.GetQpPercentageSRV), MVMQp,&MVMQpSelection},
  {MVMEASUREMENT_ME,	&(mVMeasurement.GetQvPartialPercentageSRV), MVMQvPartial,&MVMQvPartialSelection},
  {MVMEASUREMENT_ME,	&(mVMeasurement.GetQnPartialPercentageSRV), MVMQnPartial,&MVMQnPartialSelection},

  //device variables managed by coordinator
  {COORDINATOR_ME,		&(coordinator.GetTemperaturePercentageSRV),coordinatorFreq,&coordinatorFreqSelection},
  {COORDINATOR_ME,		&(coordinator.GetTemperaturePercentageSRV),coordinatorTemperature,&coordinatorTemperatureSelection},
   
  //device variables managed by mapper
  {MAPPER_ME,			&(mapper.GetDummyPercentageSRV), mapperT_External,&mapperT_ExternalSelection},
  {MAPPER_ME,			&(mapper.GetDummyPercentageSRV), mapperPressure,&mapperPressureSelection},
  {MAPPER_ME,			&(mapper.GetDummyPercentageSRV), mapperDensity,&mapperDensitySelection},
  {MAPPER_ME,			&(mapper.GetDummyPercentageSRV), mapperContent,&mapperContentSelection},  
  
  //totalizer variables
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totQv,&totQvSelection}, 
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totQn,&totQnSelection}, 
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totQm,&totQmSelection}, 
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totEnergy,&totEnergySelection}, 
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totQvPartial,&totQvPartialSelection}, 
  {TOTALIZER_ME,		&(totalizer.GetTotPercentageSRV), totQnPartial,&totQnPartialSelection},
};


//--------------------------------------------------------------------------------------------------
/*!
\brief  put measurement attributes by the label, it will call T_Unit put function
\param  label       The label defined in _E_M_Label for measurement
\param  attributeIndex  Index of the attribute defined in _E_MVM_IDX
\param  ptrValue    value
\return Framework return codes
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: ok
\n result Lint Level 3:ok
\bug:none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_Measurement(TUSIGN8 label,enum _E_MVM_IDX attributeIndex,void FAST* ptrValue)
{      
    TUSIGN16 ObjIdx;
    const T_MAPLIST_MEMBER* measurement = NULL;
    //1 check the whether the label is supported
    if((TUSIGN16)OK == CheckIsSupportedMeasurement(label))
    {
        //2 check the validation of pointer
        VIP_ASSERT(MeasurementList[label].pSubsys);
        VIP_ASSERT(ptrValue);
        
        //3 check attribut validation
        VIP_ASSERT(attributeIndex < M_ATR_NUMOF_IDX);
        measurement = MeasurementList[label].pAtrlist;
        ObjIdx = measurement[attributeIndex].objectIndex;   
        
        //4 check attribut support
        VIP_ASSERT(ObjIdx != NOT_SUPPORTED);
        
        //5 call T_UNIT put function
        return MeasurementList[label].pSubsys->Put(MeasurementList[label].pSubsys,ObjIdx,measurement[attributeIndex].atributeIndex,ptrValue);
    }
    return (TUSIGN16)METHOD_NOT_SUPPORTED;
}



//--------------------------------------------------------------------------------------------------
/*!
\brief  get measurement attributes by the label, it will call T_Unit get function
\param  label       The label defined in _E_M_Label for measurement
\param  attributeIndex  Index of the attribute defined in _E_MVM_IDX
\param  ptrValue    value
\return Error code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: ok
\n result Lint Level 3:ok
\bug none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_Measurement(TUSIGN8 label,enum _E_MVM_IDX attributeIndex,void FAST* ptrValue)
{      
    TUSIGN16 ObjIdx ;
    const T_MAPLIST_MEMBER* measurement;
    //1 check the whether the label is supported
    if((TUSIGN16)OK == CheckIsSupportedMeasurement(label))
    {
        // 2 check the validation of pointer
        VIP_ASSERT(MeasurementList[label].pSubsys);
        VIP_ASSERT(ptrValue);
        
        //3 check attribute validation
        VIP_ASSERT(attributeIndex < M_ATR_NUMOF_IDX);
        measurement = MeasurementList[label].pAtrlist;
        ObjIdx = measurement[attributeIndex].objectIndex;   
        
        //4 check attribute support    
        VIP_ASSERT(ObjIdx != NOT_SUPPORTED);
        
        //5 call T_UNIT get function
        return MeasurementList[label].pSubsys->Get(MeasurementList[label].pSubsys,ObjIdx,measurement[attributeIndex].atributeIndex,ptrValue);
    }
    return (TUSIGN16)METHOD_NOT_SUPPORTED;
}