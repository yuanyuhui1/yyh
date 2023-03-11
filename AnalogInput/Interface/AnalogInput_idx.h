//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput

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

#ifdef __ANALOG_INPUT_IDX_H__
  #error AnalogInput_idx.h included more than once
#endif


#define __ANALOG_INPUT_IDX_H__

enum _T_ANALOG_INPUT_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     AI Select
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    AI_IDX_aiSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The Value of the Input via AI. it is baseunit value. for example. Pressure is MPa.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    AI_IDX_aiInValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    AI_IDX_aiAlarmSimulation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    AI_IDX_aiDiagnosis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    AI_IDX_aiSimulationData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    AI_IDX_aiTempCurr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT[2]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    AI_IDX_baseCalibration,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    AI_IDX_trimAiLowerEndpoint,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    AI_IDX_trimAiUpperEndpoint,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    AI_IDX_calibrationState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    AI_IDX_aiRawCurr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    AI_IDX_aiExternalCutOffStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    AI_IDX_aiExternalCutOffOption
    //@SubGen end@    <ENUMERATIONS>
};




