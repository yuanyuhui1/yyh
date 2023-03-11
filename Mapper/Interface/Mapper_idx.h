//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper

 Module

 Description    This header file defines the complete list of public data-objects.
                Other subsystems need to use these macros instead of magic-numbers  
                when accessing the public data-objects.

                The index of private data-object should not be available through
                the TUNIT interface.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifdef __MAPPER_IDX_H__
  #error Mapper_idx.h included more than once
#endif

#define __MAPPER_IDX_H__

enum _T_MAPPER_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagFunCheckMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagOffSpecMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagMaintainMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagSimulationStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagSimulationType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[16]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : INOUT
    */
    MAPPER_IDX_diagClearHistory,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagMinFlowMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagMaxFlowMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagFlow103Mask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagTSensorOffSpecMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagTxSensorOffSpecMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MAPPER_IDX_diagTSensorFailMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MAPPER_IDX_diagLowCutoffMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagDeviceStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisHighCondition,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisHighClass,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisHighGroup,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagHistory,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisConditionNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsClass,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsGroup,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsPriority,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsAlarmCounter,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsAlarmTimeCounterDay,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsAlarmTimeCounterMsec,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsTimeStampLastAlarmDay,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsTimeStampLastAlarmMsec,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[15]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_diagnosisConditionDetailsCodes,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagnosisMasking,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     PV object
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     SV object
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     TV object
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     QV object
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Unit of PV
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV_Unit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Unit of SV
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV_Unit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Unit of TV
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV_Unit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Unit of QV
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV_Unit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV_RangeUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV_RangeUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV_RangeUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV_RangeUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Upper range of PV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV_UpperRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Upper range of SV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV_UpperRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Upper range of TV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV_UpperRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Upper range of QV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV_UpperRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Lower range of PV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV_LowerRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Lower range of SV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV_LowerRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Lower range of TV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV_LowerRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Lower range of QV
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV_LowerRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     PV's assignment to a device variable code
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PV_Assignment,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     SV's assignment to a device variable code
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_SV_Assignment,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     TV's assignment to a device variable code
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_TV_Assignment,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     QV's assignment to a device variable code
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_QV_Assignment,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PV_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_SV_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_TV_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_QV_Classification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PV_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_SV_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_TV_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_QV_Status,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PV_TFCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PV_ACFlag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_PV_PercentageForDO,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVSensorLimitHigh,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVSensorLimitLow,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVSensorLimitUnitcode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVSensorminimumSpan,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVPercent_Unit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_PVPercent_Range,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PVPercent_Classifiction,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_PV_DampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_setCurrentValueAsPVRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MAPPER_IDX_flowOver103Bhv,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExt,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtPresetObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_dvTExtSrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtMaxDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtMinDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Absolute pressure.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressure,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Absolute pressure.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Absolute pressure.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureSimUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRangeRelative,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRangeRelativeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Absolute pressure.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Absolute pressure.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRelativeMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureRelativeMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureMaxDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureMinDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Relative pressure.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressurePreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressurePresetObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : RANGEFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureAlmRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_dvPressureSrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensity,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensitySimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensitySim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensitySimUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensitySimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityMaxDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityMinDN_Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityPresetObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_dvDensitySrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContent,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvContentPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MAPPER_IDX_dvContentSrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDummyPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDummyDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDummyUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDummyUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_localOperatingModeSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_mapperAlarmSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_mapperDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_presetChanged,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_diagnosisCounter,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_protectedDataStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_protectedDataCrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_hartBurstInAlarmPeriod,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureAlmRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvTExtUserUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvPressureUserUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MAPPER_IDX_dvDensityUserUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8[48]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MAPPER_IDX_diagAlarmNamurConfiguration
    //@SubGen end@    <ENUMERATIONS>
};




