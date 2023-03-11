//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                 Subsystem Mapper
 Module
 Description            
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef _MAPPER_MVMEASUREMENT_H_
#define _MAPPER_MVMEASUREMENT_H_


typedef TUSIGN16 T_SELECTION; //could be extend tobe TUSIGN16 if have 

#define NOT_SUPPORTED 0xFFFF

//! 13 operating mode selection bit mask Marcos					
#define SELECTION_LIQUIDQV		0x0001
#define SELECTION_LIQUIDQN		0x0002
#define SELECTION_LIQUIDQM		0x0004
#define SELECTION_LIQUIDPOWER	0x0008
#define SELECTION_GASQV			0x0010
#define SELECTION_GASQN			0x0020
#define SELECTION_GASQM			0x0040
#define SELECTION_GASPOWER		0x0080
#define SELECTION_GASPARTIALQV	0x0100
#define SELECTION_GASPARTIALQN	0x0200
#define SELECTION_STEAMQV		0x0400
#define SELECTION_STEAMQM		0x0800
#define SELECTION_STEAMPOWER	0x1000
#define SELECTION_RESERD0		0x2000//!3 bits reserved
#define SELECTION_RESERD1		0x4000
#define SELECTION_RESERD2		0x8000
#define ALL_SELECTIONS_ENABLE	0x1FFF

//! attribute enum for device variable 
enum _E_MVM_IDX
{
    M_ATR_dampedValue,
    M_ATR_unitCode,
    M_ATR_unitType,
    M_ATR_classification,
    M_ATR_status,
    M_ATR_dampingTime,
    M_ATR_upperRange,
    M_ATR_lowerRange,
    M_ATR_percentage,
    
    //! Add mini span attribute for Hart by jax 2012-08-27
    M_ATR_minimumSpan,   
    
    M_ATR_limitHigh, 
    M_ATR_limitLow, 
    M_ATR_limitSpan,
    
    M_ATR_value,
    
    M_ATR_NUMOF_IDX
};

//! lable enum for device variable 
enum _E_M_Label
{	
    M_Label_mVMeasurementQv,
    M_Label_mVMeasurementQn,
    M_Label_mVMeasurementQm,
    M_Label_mVMeasurementPower,
    M_Label_mVMeasurementPartialQv,
    M_Label_mVMeasurementPartialQn,
    
    M_Label_coordinatorFreq,
    M_Label_coordinatorTemperature,
    
    M_Label_mapperT_External,
    M_Label_mapperPressure,
    M_Label_mapperDensity,
    M_Label_mapperContent,
   
    M_Label_totQv,
    M_Label_totQn,
    M_Label_totQm,
    M_Label_totEnergy,   
    M_Label_totQvPartial, 
    M_Label_totQnPartial, 

#ifdef MAPPER_MODULE_TEST
	M_Label_TestVar1,
    M_Label_TestVar2,
    M_Label_TestVar3,
#endif
	
    M_NUMOF_DEVICEVARIABLES    
};

typedef struct _T_ATRLIST_MEMBER
{
    TUSIGN16 objectIndex;
    TINT16 atributeIndex;
} T_MAPLIST_MEMBER;

#define CONSTRUCTOR_T_ATRLIST(objName) \
                const T_MAPLIST_MEMBER objName[M_ATR_NUMOF_IDX]

#define PTR2_ATRLIST T_MAPLIST_MEMBER * 

typedef struct 
{
    const T_UNIT SLOW* pSubsys;
    TFLOAT (*const SLOW*   GetPercentage )(void);
    const PTR2_ATRLIST pAtrlist;
    const T_SELECTION* selectionMask;
} MapList;
extern const MapList MeasurementList[M_NUMOF_DEVICEVARIABLES];

//-------------------------------------------------------------------------------------------------
//!T_MAPLIST_MEMBER declaration of device variables within MVM
extern const T_MAPLIST_MEMBER SVMQv[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER MVMQn[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER MVMQm[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER MVMQp[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER MVMQvPartial[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER MVMQnPartial[M_ATR_NUMOF_IDX];

//!T_MAPLIST_MEMBER declaration of device variables within CDT
extern const T_MAPLIST_MEMBER coordinatorFreq[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER coordinatorTemperature[M_ATR_NUMOF_IDX];

//!T_MAPLIST_MEMBER declaration of device variables within Mapper
extern const T_MAPLIST_MEMBER mapperT_External[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER mapperPressure[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER mapperDensity[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER mapperContent[M_ATR_NUMOF_IDX];

//!T_MAPLIST_MEMBER declaration of device variables within Totalizer
extern const T_MAPLIST_MEMBER totQv[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER totQn[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER totQm[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER totEnergy[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER totQvPartial[M_ATR_NUMOF_IDX];
extern const T_MAPLIST_MEMBER totQnPartial[M_ATR_NUMOF_IDX];

//-------------------------------------------------------------------------------------------------
//!T_SELECTION declaration of device variables within MVM
extern const T_SELECTION SVMQvSelection;
extern const T_SELECTION MVMQnSelection;
extern const T_SELECTION MVMQmSelection;
extern const T_SELECTION MVMQpSelection;
extern const T_SELECTION MVMQvPartialSelection;
extern const T_SELECTION MVMQnPartialSelection;

//!T_SELECTION declaration of device variables within CDT
extern const T_SELECTION coordinatorFreqSelection;
extern const T_SELECTION coordinatorTemperatureSelection;

//!T_SELECTION declaration of device variables within Mapper
extern const T_SELECTION mapperT_ExternalSelection;
extern const T_SELECTION mapperPressureSelection;
extern const T_SELECTION mapperDensitySelection;
extern const T_SELECTION mapperContentSelection;

//!T_SELECTION declaration of device variables within Totalizer
extern const T_SELECTION totQvSelection;
extern const T_SELECTION totQnSelection;
extern const T_SELECTION totQmSelection;
extern const T_SELECTION totEnergySelection;
extern const T_SELECTION totQvPartialSelection;
extern const T_SELECTION totQnPartialSelection;

//-------------------------------------------------------------------------------------------------
//! declare public functions
TUSIGN16 Put_Measurement(TUSIGN8 label,enum _E_MVM_IDX attributeIndex,void FAST* ptrValue);
TUSIGN16 Get_Measurement(TUSIGN8 label,enum _E_MVM_IDX attributeIndex,void FAST* ptrValue);

#endif