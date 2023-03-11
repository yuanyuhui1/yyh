//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Class definition T_UNIT::DIGITAL_OUTPUT

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before DigitalOutput.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before DigitalOutput.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before DigitalOutput.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __DIGITAL_OUTPUT_H__
  #error DigitalOutput.h included more than once
#endif

#define __DIGITAL_OUTPUT_H__

#define DIGITAL_OUTPUT_VERSION 0x010000   // Version 1.0.0

//-------------------------------------------------------------------------------------------------
//!  define macros, enums
#define MILLISECONDS_PER_SECOND         1000    // so se can express 1/pulse-width in seconds
//#define SECONDS_PER_MILLISECOND         0.001   // so we can express pulse-width in seconds

//@SubGen start@  <TYPES>
  // TOutputPulses
  #define DIGITALOUTPUT_PULSE_WIDTH_MIN 0.05f //The minimum pulse width is 0.05ms
  #define DIGITALOUTPUT_PULSE_WIDTH_MAX 2000.0f //The maximum pulse width is 2000ms
  #define DIGITALOUTPUT_PULSE_WIDTH_DEFAULT 20.0f //The default pulse width is 10.0ms
  #define DIGITALOUTPUT_PULSE_FACTOR_MIN 0.001f //The minimum pulse factor is 0.001pulses/uot
  #define DIGITALOUTPUT_PULSE_FACTOR_MAX 2000.0f //The maxmum pulse factor is 2000.0pulses/uot
  #define DIGITALOUTPUT_PULSE_FACTOR_DEFAULT 20.0f //The default pulse factor is 20.0pulses/uot

  // TOutputFrequencies
  #define DIGITALOUTPUT_FULLSCALE_FREQ_MIN 0.0f //The minimum fullscale frequency of real frequency is 0.0f
  #define DIGITALOUTPUT_FULLSCALE_FREQ_MAX 10000.0f //The maximum fullscale frequency of real frequency is 10000.0f
  #define DIGITALOUTPUT_FULLSCALE_FREQ_DEFAULT 2000.0f //The default fullscale frequency is 2000.0f
  #define DIGITALOUTPUT_OUTPUTFREQ_MIN 0.0f //The minimum allowed frequency for real frequency is 0.0f
  #define DIGITALOUTPUT_OUTPUTFREQ_MAX 10500.0f //The maximum allowed frequency for real frequency is 10500.0f
  #define DIGITALOUTPUT_OUTPUTFREQ_DEFAULT 2000.0f //The default allowed frequency for real frequency is 2000.0f

  // TOutputMode1Formats
  #define DIGITALOUTPUT_DO1_NONE 0 //Hardware output1 is disabled
  #define DIGITALOUTPUT_DO1_LOGIC 1 //Hardware output1 logic signal
  #define DIGITALOUTPUT_DO1_PULSE 2 //Hardware output1 pulse signal
  #define DIGITALOUTPUT_DO1_FREQUENCY 3 //Hardware output1 frequency signal

  // 
  #define DIGITALOUTPUT_NUMBER_OF_OUTPUT1_MODES 4 //TOutputMode1Formats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_SIMULATION_ENABLED 2 //TSimulationEnabledFormat numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_LOGIC_SIGNAL_SOURCES 2 //TLogicSignalSourceFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_LOGIC_ACTIVE_STATES 2 //TLogicActiveStateFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_OUTPUT_LOGICS 2 //
  #define DIGITALOUTPUT_NUMBER_OF_LOGIC_SIM_COMMANDS 2 //TLogicSimulationCommandFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_PULSE_OPERATIONS 2 //TPulseOperationFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_FLOW_DIRECTIONS 2 //TFlowDirectionFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_LOGIC_SIGNAL_VALIDS 2 //TLogicSignalValidFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_PULSE_SIGNAL_VALIDS 3 //TPulseSignalValidFormats numeric limit
  #define DIGITALOUTPUT_NUMBER_OF_PULSE_TYPES 2 //
  #define DIGITALOUTPUT_NUMBER_OF_ALM_BEHAV_ELEMENTS 2 //E_DIG_OUT_ALARM_BEHAVIOUR numeric limit
  #define DIGITALOUTPUT_DO_ALARM_SIZE 1 //
  #define DIGITALOUTPUT_NUMBER_OF_OUTPUT_CONNECTIONS 3 //
  #define DIGITALOUTPUT_T_ALARM_DTABLE_SIZE 2 //
  #define DIGITALOUTPUT_PULSE_FACTOR_MULTI_WIDTH 1.1f //

  // TSimulationEnabledFormat
  #define DIGITALOUTPUT_SIMU_DISABLED 0 //Output simulation is disabled
  #define DIGITALOUTPUT_SIMU_ENABLED 1 //Output simulation is enabled

  // TLogicSignalSourceFormats
  #define DIGITALOUTPUT_NO_LOGIC_SOURCE 0 //No source selected for logic output
  #define DIGITALOUTPUT_DIGITAL_OUTPUT_ALARM 1 //Dig out alarm signal selected for logic output

  // TLogicActiveStateFormats
  #define DIGITALOUTPUT_ACTIVE_HIGH 0 //Logic output Active State is high
  #define DIGITALOUTPUT_ACTIVE_LOW 1 //Logic output Active State is low

  // TOutputLogicFormat
  #define DIGITALOUTPUT_LOGIC_LOW 0 //Hardware logic output is low
  #define DIGITALOUTPUT_LOGIC_HIGH 1 //Hardware logic output is high

  // TLogicSimulationCommandFormats
  #define DIGITALOUTPUT_COMMAND_OFF 0 //Simulate logic OFF
  #define DIGITALOUTPUT_COMMAND_ON 1 //Simulate logic ON

  // TPulseOperationFormats
  #define DIGITALOUTPUT_PULSE_MODE_1 0 //Pulse frequency determined by pulse-factor
  #define DIGITALOUTPUT_PULSE_MODE_2 1 //Pulse frequency determined by fullscale-frequency

  // TFlowDirectionFormats
  #define DIGITALOUTPUT_FLOW_FORWARD 0 //Flow forward
  #define DIGITALOUTPUT_FLOW_REVERSE 1 //Flow reverse

  // TLogicSignalValidFormats
  #define DIGITALOUTPUT_LOGIC_SIGNAL_NOT_VALID 0 //Logic signal not valid
  #define DIGITALOUTPUT_LOGIC_SIGNAL_VALID 1 //Logic signal valid

  // TPulseSignalValidFormats
  #define DIGITALOUTPUT_PULSE_OUTPUT_NOT_VALID 0 //Pulse signal not valid
  #define DIGITALOUTPUT_NORMAL_PULSES_VALID 1 //Pulse signal valid in respect of normal mode
  #define DIGITALOUTPUT_SIMULATION_PULSES_VALID 2 //Pulse signal valid in respect of simulation mode

  // TPulseTypeFormats
  #define DIGITALOUTPUT_LONG_PULSE 0 //This is a long pulse
  #define DIGITALOUTPUT_SHORT_PULSE 1 //This is a short pulse

  // E_DIG_OUT_ALARM_BEHAVIOUR
  #define DIGITALOUTPUT_ALARM_BEHAVIOUR_OK 0 //Dig out alarm logic OFF
  #define DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1 1 //Dig out alarm logic ON (OP1)

  // DO_ALARM
  #define DIGITALOUTPUT_PULSENUMMAXALARM 0 //

  // TOutputConnectionFormats
  #define DIGITALOUTPUT_NO_CONNECT 0 //no connection
  #define DIGITALOUTPUT_FLOW_CONNECT 1 //flow output
  #define DIGITALOUTPUT_TEMP_CONNECT 2 //temp output

  // T_ALARM_STATUS
  #define DIGITALOUTPUT_ALARM_DISABLE 0 //
  #define DIGITALOUTPUT_ALARM_ENABLE 1 //

  // T_FUNCTION_STATUS
  #define DIGITALOUTPUT_FUNCTION_ENABLE 1 //
  #define DIGITALOUTPUT_FUNCTION_DISABLE 0 //
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output

/*
typedef struct _TDigitalOutputDVGroup
{

    TFLOAT	MassFlow_Qm_kgh;
    TFLOAT	NormalVolumeFlow_Qn_nm3s;
    TFLOAT	NetMassFlow_Qnet_kgh;
    TFLOAT	NetVolumeFlow_Qnet_ls;
    TFLOAT	VolumeFlow_Qv_ls;
    TFLOAT	Power_kW;
    TFLOAT	PowerByConc_kW;
    
    TFLOAT	MassFlow_InPercent;
    TFLOAT	NormalVolumeFlow_InPercent;
    TFLOAT	VolumeFlow_InPercent;
    TFLOAT	NetMassFlow_InPercent;
    TFLOAT	NetVolumeFlow_InPercent;
    TFLOAT	Power_InPercent;
    TFLOAT	PowerByConc_InPercent;
     
    TFLOAT	Pressure_InPercent;
    TFLOAT	Temperature_InPercent;
	
} TDigitalOutputDVGroup;*/


typedef struct _T_DIGTALINPUT_PARAMETER
{
    TUSIGN8  simulationMode;
    //TUSIGN8  label;
    TUSIGN8  measurementChange;
    TFLOAT   flowValue;
    TFLOAT   DoPercentage;
    TFLOAT   upperRange;
    TUSIGN8  unitType;
    TUSIGN8  unitCode;
    TUSIGN8  doAlarm;
    //TUSIGN8  tAlarm;
    TUSIGN32 elapseTime;
}T_DIGTALINPUT_PARAMETER;


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_DIGITAL_OUTPUT_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    DIGITALOUTPUT_DC_IDX_StaticFrequentConstantDefault,
    DIGITALOUTPUT_DC_IDX_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT,
    DIGITALOUTPUT_DC_IDX_DYNAMIC,
    DIGITALOUTPUT_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_DIGITAL_OUTPUT
typedef struct _T_DIGITAL_OUTPUT
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    TUSIGN16 (* UpdateEXE)(const T_DIGTALINPUT_PARAMETER* digitalInput);
    void (* UpdateDiagnosis)(void);
    //@SubGen end@    <METHODS>
} T_DIGITAL_OUTPUT;


//-------------------------------------------------------------------------------------------------
//! define the T_DIGITAL_OUTPUT-object
extern SLOW const T_DIGITAL_OUTPUT digitalOutput;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_DIGITAL_OUTPUT
#define DIGITAL_OUTPUT_ME (&digitalOutput.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define digitalOutput_Get(objectIndex,attributeIndex,ptrValue) \
   digitalOutput.unit.Get(DIGITAL_OUTPUT_ME,objectIndex,attributeIndex,ptrValue)
#define digitalOutput_Put(objectIndex,attributeIndex,ptrValue) \
   digitalOutput.unit.Put(DIGITAL_OUTPUT_ME,objectIndex,attributeIndex,ptrValue)
#define digitalOutput_Check(objectIndex,attributeIndex,ptrValue) \
   digitalOutput.unit.Check(DIGITAL_OUTPUT_ME,objectIndex,attributeIndex,ptrValue)

#define digitalOutput_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   digitalOutput.unit.GetAttributeDescription(DIGITAL_OUTPUT_ME,objectIndex,attributeIndex,ptrDescriptor)
#define digitalOutput_GetObjectDescription(objectIndex,ptrDescriptor) \
   digitalOutput.unit.GetObjectDescription(DIGITAL_OUTPUT_ME,objectIndex,ptrDescriptor)


//#define IO_REVERT_PIN8_0    1


//#define DO_MODULE_TEST  
#ifdef DO_MODULE_TEST  

extern void TestUpdateFrequency_DIGITALOUTPUT();

 extern void TestUpdateEXE_DIGITALOUTPUT(void);
 
#endif
 
