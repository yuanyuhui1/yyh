//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before AnalogInput_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before AnalogInput_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ANALOG_INPUT_TYPES_H__
  #error AnalogInput_types.h included more than once
#endif

#define __ANALOG_INPUT_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of StaticFreqStaticDefalutCalibration block
 typedef struct _T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION
 {
     //@SubGen start@    <DATACLASS StaticFreqStaticDefalutCalibration> 
    TFLOAT baseCalibration[2];
     //@SubGen end@    <DATACLASS StaticFreqStaticDefalutCalibration> 
 } T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION;



 //-------------------------------------------------------------------------------------------------
 //! structure of Unprotected block
 typedef struct _T_ANALOG_INPUT_UNPROTECTED
 {
     //@SubGen start@    <DATACLASS Unprotected> 
    TFLOAT aiInValue;
    TUSIGN8 aiAlarmSimulation;
    TUSIGN8 aiDiagnosis;
    TFLOAT aiSimulationData;
    TFLOAT aiTempCurr;
    TFLOAT trimAiLowerEndpoint;
    TFLOAT trimAiUpperEndpoint;
    TUSIGN8 calibrationState;
    TFLOAT aiRawCurr;
    TUSIGN8 aiExternalCutOffStatus;
     //@SubGen end@    <DATACLASS Unprotected> 
 } T_ANALOG_INPUT_UNPROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 aiSelection;
    TUSIGN8 aiExternalCutOffOption;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT;




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_ANALOG_INPUT_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_ANALOG_INPUT_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

