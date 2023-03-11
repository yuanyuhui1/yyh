//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement

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

#ifdef __MVMEASUREMENT_IDX_H__
  #error MVMeasurement_idx.h included more than once
#endif

#define __MVMEASUREMENT_IDX_H__

enum _T_MVMEASUREMENT_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_Qn,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DNY_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow  after damping
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnDamped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow unit. the default is m3/h
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QnUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MVM_IDX_QnUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     an object to combine volume flow and the unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnDampedObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QnRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     range unit code. of volume flowrate
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnSimUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Object to combine the Volume flowrate  upperrange with the Range unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the max permission volume flowrate. it always mark on the nameplate.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnMaxDNDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QMinDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnMaxDNObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPerSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The volume flowrate percentage  of the deviceVariable2ax.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QnDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_Qm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DNY_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow  after damping
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmDamped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow unit. the default is m3/h
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QmUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MVM_IDX_QmUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     an object to combine volume flow and the unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QmObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QmDampedObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QmSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QmRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : RANGEFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QAlmRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     range unit code. of volume flowrate
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmSimUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Object to combine the Volume flowrate  upperrange with the Range unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QmRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the max permission volume flowrate. it always mark on the nameplate.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmMaxDNDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QmMaxDNObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmPerSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The volume flowrate percentage  of the deviceVariable2ax.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QmDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_Qp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DNY_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow  after damping
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpDamped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow unit. the default is m3/h
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QpUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    MVM_IDX_QpUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     an object to combine volume flow and the unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QpObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QpDampedObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QpSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QpRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     range unit code. of volume flowrate
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpSimUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Object to combine the Volume flowrate  upperrange with the Range unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QpRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the max permission volume flowrate. it always mark on the nameplate.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpMaxDNDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QpMaxDNObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpPerSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The volume flowrate percentage  of the deviceVariable2ax.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QpDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartial,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DNY_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow  after damping
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialDamped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     an object to combine volume flow and the unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialDampedObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Object to combine the Volume flowrate  upperrange with the Range unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the max permission volume flowrate. it always mark on the nameplate.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialMaxDNDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialMaxDNObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialPerSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The volume flowrate percentage  of the deviceVariable2ax.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartial,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialSimEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DNY_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     volume flow  after damping
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialDamped,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     an object to combine volume flow and the unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialDampedObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialSimObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : DEV_RNGFLT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Object to combine the Volume flowrate  upperrange with the Range unit.
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialRangeObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the max permission volume flowrate. it always mark on the nameplate.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialMaxDN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialMaxDNDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialMaxDNObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialPerSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The volume flowrate percentage  of the deviceVariable2ax.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialPercentage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialDampingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Steam Satus.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    MVM_IDX_steamStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     To store the steam type. It must be one of the saturated steam and Overheat steam
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    MVM_IDX_steamType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Correction for the liquid.could be no correction. volume correction or density correction.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_liquidCorrection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Heat Capacity. to calculate the power for liquid.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_heatCapacity,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_densityRef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_densigyRefObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_densityS,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_densityExpandBeta2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_volumeExpandBeta1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Standard Gas condition selection.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_gasRef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_gasComputeType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Gas Density Selection
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_gasDensitySelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Calorific Value
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_calorificEnergy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_gasConfFlag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     MolFractions[0]=Methane CH4 MolFractions[1]=Ethane    --C2H6   MolFractions[2]=Propane   --C3H8  MolFractions[3]=ISO_Butane  --C4H10  MolFractions[4]=n_Butane  --C4H10  MolFractions[5]=ISO_Petane  --C5H12  MolFractions[6]=n_Petane  --C5H12  MolFractions[7]=n-Hexane  --C6H14  MolFractions[8]=n-Heptane --C7H16  MolFractions[9]=n-Octane --C8H18  MolFractions[10]=n-Nonane --C9H20  MolFractions[11]=n-Decane --C10H22  MolFractions[12]=Carbon-Dioxide --CO2  MolFractions[13]=Nitrogen --N2  MolFractions[14]=Hydrogen Sulfide --HS  MolFractions[15]=Helium   --He  MolFractions[16]=Water    --H2O  MolFractions[17]=Oxygen   --O2  MolFractions[18]=Argon    --Ar  MolFractions[19]=Hydrogen --H2    MolFractions[20]]=Carbon Monoxide --CO
     \datatype          : FLOAT[21]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_molFractions,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     9*7 [0...6],[7...13]...[56...62]
     \datatype          : FLOAT[63]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_compressFactorArray,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray0,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray5,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFTempArray6,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray0Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray1Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray2Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray3Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray4Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray5Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFTempArray6Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray0,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray5,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray6,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray7,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_CFPressArray8,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray0Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray1Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray2Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray3Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray4Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray5Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray6Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray7Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_CFPressArray8Obj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Compress Factor at operating Status.
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MVM_IDX_compressFactorR,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MVM_IDX_relativeDensity,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_relativeDensityRef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Calorific Value at ref condition
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_refCalorifcValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    MVM_IDX_calorificValueRef,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MVM_IDX_mvmAlarmSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MVM_IDX_mvmDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_refTemperature,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_refTemperatureObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_refPressure,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : UOM_PF
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    MVM_IDX_refPressureObj,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_compressFactorS,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_compressFactorPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_matrixSize,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_spline2dRequest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Update ComressFactor for Standard Request.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_updateCFSRequest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_updateCFPresetRequest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_BIT8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_updateMaxDNRequest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_Hw,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_Hc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_HwPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_HcPreset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    MVM_IDX_steamPwrrFR,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialPercentageDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPartialDampedDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialPercentageDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QvPartialDampedDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnPercentageDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QnDampedDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmPercentageDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QmDampedDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpPercentageDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_QpDampedDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_HwDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_HcDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_compressFactorRDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_compressFactorSDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_mvmAlarmSimulationDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_mvmDiagnosisDuplicated,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_protectedDataCrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    MVM_IDX_protectedDataStatus
    //@SubGen end@    <ENUMERATIONS>
};