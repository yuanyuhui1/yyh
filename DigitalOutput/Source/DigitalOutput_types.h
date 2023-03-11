//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before DigitalOutput_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before DigitalOutput_types.h
#endif

#ifndef __DIGITALOUTPUT_PULSE_
  #error DigitalOutput_Pulse.h must be included before DigitalOutput_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __DIGITAL_OUTPUT_TYPES_H__
  #error DigitalOutput_types.h included more than once
#endif

#define __DIGITAL_OUTPUT_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 outputMode;
    TUSIGN8 lgcSgnlSrc;
    TUSIGN8 lgcActState;
    TUSIGN8 lgcSimCmd;
    T_FLOAT_FIXEDUNIT pulseWidth;
    T_FLOAT_FIXEDUNIT pulseFactor;
    T_FLOAT_FIXEDUNIT minFrequency;
    T_FLOAT_FIXEDUNIT maxFrequency;
     //@SubGen end@    <DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT;



 
 //-------------------------------------------------------------------------------------------------
 //! structure of StaticFrequentConstantDefault block
 typedef struct _T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
     //@SubGen start@    <DATACLASS StaticFrequentConstantDefault> 
    T_FLOATRANGE_FIXEDUNIT pulseFactorRange;
    T_FLOATRANGE_FIXEDUNIT pulseWidthRange;
    T_FLOATRANGE_FIXEDUNIT minFrequencyRange;
    T_FLOATRANGE_FIXEDUNIT maxFrequencyRange;
    TUSIGN8 generalAlarm;
    TUSIGN8 minFlowrateAlarm;
    TUSIGN8 maxFlowrateAlarm;
    TUSIGN8 maxTemperatureAlarm;
    TUSIGN8 lowFlowCutOff;
    TUSIGN8 minTemperatureAlarm;
     //@SubGen end@    <DATACLASS StaticFrequentConstantDefault> 
 } T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_DIGITAL_OUTPUT_DYNAMIC
 {
    TLogic logic_DO1;
    TBOOL	do1Available;
    TUSIGN8	oldOutputMode;
    TUSIGN8	oldMeasurement;
    TUSIGN8 onecycle;
    TFLOAT  Qv_Display;
    TFLOAT  DoPercentage;
    TUSIGN8 unitType;
    TUSIGN8 unitCode;
    TFLOAT   gMaxData;
    TUSIGN8  outputMode;
    TUSIGN32 sysTime;
    TUSIGN32 elapsedTime;
    TUSIGN32 oldelapsedTime;
    TFLOAT   timebaseforcalc;
     //@SubGen start@    <DATACLASS DYNAMIC> 
    TUSIGN8 alarmState_DO;
    TUSIGN8 simuEnabled;
    TFLOAT simuFrequency;
    TUSIGN8 alarmSimulation_DO;
    TINT16 simuPulsesNum;
    TUSIGN8 logicValue;
    TUSIGN32 pulseNum;
    TFLOAT frequencyValue;
    TUSIGN8 functionEnable;
    //@SubGen end@    <DATACLASS DYNAMIC> 
 } T_DIGITAL_OUTPUT_DYNAMIC;



/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_DIGITAL_OUTPUT_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_DIGITAL_OUTPUT_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

