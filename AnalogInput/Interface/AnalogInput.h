//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Class definition T_UNIT::ANALOG_INPUT

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks		
 Version: 0.1.0
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before AnalogInput.h
#endif


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before AnalogInput.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before AnalogInput.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __ANALOG_INPUT_H__
  #error AnalogInput.h included more than once
#endif

#define __ANALOG_INPUT_H__

#define ANALOG_INPUT_VERSION 0x010001   // Version 1.0.1

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // _AI_SELECT
  #define AI_EXT_TEMPERATURE 0 //
  #define AI_PRESSURE 1 //
  #define AI_PRESSURE_ABS 2 //
  #define AI_DENSITY 3 //
  #define AI_GAS_CONTENT 4 //
  #define AI_EXTERNAL_OUTPUT_CUTOFF 5 //
  #define AI_WITHOUT 6 //
  #define AI_INTER_TEMPERATURE 7 //

  // _AI_DIAGNOSIS_ALARM_TABLE
  #define AI_ALM_SPICOM_FAIL 0 //
  #define AI_ALM_OUT_OF_RANGE 1 //
  #define AI_ALM_CUT_OFF 2 //

  // 
  #define AI_DIAGNOSIS_ALARM_TABLE_SIZE 3 //

  // _CALIBRATION_STATE
  #define AI_CALIBRATION_OFF 0 //
  #define AI_CALIBRATION_START 1 //
  #define AI_CALIBRATION_STOP 2 //

  // _EXTERNAL_CUT_OFF_STATUS
  #define AI_EXTERNAL_UNCUT_OFF 0 //
  #define AI_EXTERNAL_CUT_OFF 1 //

  // _EXTERNAL_CUT_OFF_OPTION
  #define AI_EXTERNAL_CUT_OFF_4MA 0 //
  #define AI_EXTERNAL_CUT_OFF_8MA 1 //
  #define AI_EXTERNAL_CUT_OFF_12MA 2 //
//@SubGen end@ <TYPES>


  //how many bits per word
#define AI_DIAG_WORD_SIZE   8
  
#if( AI_DIAG_WORD_SIZE == 8 )
    #define AI_DIAG_BIT_(index)   ((index) &  7)
    #define AI_DIAG_WRD_(index)   ((index) >> 3)
	  // words = (AI_DIAG_SIZE) >> 3
#elif( AI_DIAG_WORD_SIZE == 16 )
    #define AI_DIAG_BIT_(index)   ((index) & 15)
    #define AI_DIAG_WRD_(index)   ((index) >> 4)
	  // words = (AI_DIAG_SIZE) >> 4
#elif( AI_DIAG_WORD_SIZE == 32 )
    #define AI_DIAG_BIT_(index)   ((index) & 31)
    #define AI_DIAG_WRD_(index)   ((index) >> 5)
	  // words = (AI_DIAG_SIZE) >> 5
#else
    #error invalid definition for AI_DIAG_WORD_SIZE
#endif


#define AI_DIAG_SIZE  3

  //#if( CURR_DIAG_WRD_(CURR_DIAG_SIZE) > 1 )
  
#if(AI_DIAG_SIZE > AI_DIAG_WORD_SIZE)
  #define AI_SETDIAG_(index,var)           (var[AI_DIAG_WRD_(index)] |=  (1U << AI_DIAG_BIT_(index)))
  #define AI_CLRDIAG_(index,var)           (var[AI_DIAG_WRD_(index)] &= ~(1U << AI_DIAG_BIT_(index)))
  #define AI_GETDIAG_(index,var)    (TBOOL)(var[AI_DIAG_WRD_(index)] &   (1U << AI_DIAG_BIT_(index)))
#else
  #define AI_SETDIAG_(index,var)           (var |=  (1U << AI_DIAG_BIT_(index)))
  #define AI_CLRDIAG_(index,var)           (var &= ~(1U << AI_DIAG_BIT_(index)))
  #define AI_GETDIAG_(index,var)    (TBOOL)(var &   (1U << AI_DIAG_BIT_(index)))
#endif


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_ANALOG_INPUT_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    ANALOGINPUT_DC_IDX_Unprotected,
    ANALOGINPUT_DC_IDX_StaticFreqStaticDefalutCalibration,
    ANALOGINPUT_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT,
    ANALOGINPUT_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_ANALOG_INPUT
typedef struct _T_ANALOG_INPUT
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* UpdateAnalogInput)(void);
    void (* UpdateDiagnosis)(void);
    TUSIGN8 (* IsExternalCutOff)(void);
    //@SubGen end@    <METHODS>
} T_ANALOG_INPUT;


//-------------------------------------------------------------------------------------------------
//! define the T_ANALOG_INPUT-object
extern SLOW const T_ANALOG_INPUT analogInput;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_ANALOG_INPUT
#define ANALOG_INPUT_ME (&analogInput.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define analogInput_Get(objectIndex,attributeIndex,ptrValue) \
   analogInput.unit.Get(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue)
#define analogInput_Put(objectIndex,attributeIndex,ptrValue) \
   analogInput.unit.Put(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue)
#define analogInput_Check(objectIndex,attributeIndex,ptrValue) \
   analogInput.unit.Check(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrValue)

#define analogInput_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   analogInput.unit.GetAttributeDescription(ANALOG_INPUT_ME,objectIndex,attributeIndex,ptrDescriptor)
#define analogInput_GetObjectDescription(objectIndex,ptrDescriptor) \
   analogInput.unit.GetObjectDescription(ANALOG_INPUT_ME,objectIndex,ptrDescriptor)

