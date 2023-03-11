//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput

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

#ifdef __DIGITAL_OUTPUT_IDX_H__
  #error DigitalOutput_idx.h included more than once
#endif

#define __DIGITAL_OUTPUT_IDX_H__

enum _T_DIGITAL_OUTPUT_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Alarms to be represented as logic outputs by the digital output subsystem. only used by DO subsystem
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    DIGITALOUTPUT_IDX_alarmState_DO,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Output mode of hardware output logic/freq/pulse/none
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_outputMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     to disable/enable DO simulation output(base on the output mode and simulation value(freqsimu/pulse simu/logic simu))
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_simuEnabled,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Logic signal source for hardware output. None/Alarm
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_lgcSgnlSrc,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Logic output NC/NO
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_lgcActState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Logic simulation command for hardware (On-output/Off-close)
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_lgcSimCmd,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Pulse Width.
     \datatype          : FLOAT_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_pulseWidth,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Pulse factor.
     \datatype          : FLOAT_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_pulseFactor,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Pulse factor range.
     \datatype          : FLOATRANGE_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    DIGITALOUTPUT_IDX_pulseFactorRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Pulse width range.
     \datatype          : FLOATRANGE_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    DIGITALOUTPUT_IDX_pulseWidthRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Simulation frequency for frequency outputs.
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_simuFrequency,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     frequency minimum range
     \datatype          : FLOAT_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_minFrequency,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     frequency maximum range
     \datatype          : FLOAT_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_maxFrequency,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the range of minimum Range value
     \datatype          : FLOATRANGE_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    DIGITALOUTPUT_IDX_minFrequencyRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the range of maximum Range value
     \datatype          : FLOATRANGE_FIXEDUNIT
     \typequalifier     : STRUCT
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    DIGITALOUTPUT_IDX_maxFrequencyRange,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     logic alarm simulation value
     \datatype          : T_DIAGNOSIS_ALARM
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_alarmSimulation_DO,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     enable/disable general alarm for diagnosis.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_generalAlarm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     enable/disable min flow rate alarm for diagnosis.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_minFlowrateAlarm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     enable/disable max flow rate alarm for diagnosis.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_maxFlowrateAlarm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     enable/disable temperature alarm for diagnosis.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_maxTemperatureAlarm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     enable/disable low flow cut off alarm for diagnosis.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_lowFlowCutOff,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     simulate pulse number
     \datatype          : INT16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_simuPulsesNum,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     logic output value(on/off)
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_logicValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     pulse output value(the actual number of pulse output)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_pulseNum,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     the actual frequency the do output
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_frequencyValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     to enable/disable DO function.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_functionEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    DIGITALOUTPUT_IDX_minTemperatureAlarm
    //@SubGen end@    <ENUMERATIONS>
};




