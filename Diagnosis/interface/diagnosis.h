//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Class definition T_UNIT::DIAGNOSIS

                Subsystem interface
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
#error system.h must be included before Diagnosis.h
#endif

#ifndef __COMMON_TYPE_H__
#error common_type.h must be included before Diagnosis.h
#endif

#ifndef __T_UNIT_H__
#error t_unit.h must be included before Diagnosis.h
#endif

#ifndef __DIAGNOSIS_COMPILE_TIME_DEFINITIONS_H__
#error DiagnosisCompileTimeDefinitions.h must be included before Diagnosis.h
#endif

#if (T_UNIT_VERSION<0x020100)
#error T_UNIT_VERSION must be greater than or equal to 2.1.0
#endif

#ifdef __DIAGNOSIS_H__
#error Diagnosis.h included more than once
#endif

#define __DIAGNOSIS_H__


#define DIAGNOSIS_VERSION (0x000701)      // Version 0.7.1


//-------------------------------------------------------------------------------------------------
//!  define macros, enums

// outputAlarmCodes
//@CodeGen start@ <ALARM CONDITIONS>
#define DIAGNOSIS_CONDITION_PV_SENSOR_FAILURE		0
#define DIAGNOSIS_CONDITION_SV_SENSOR_FAILURE		1
#define DIAGNOSIS_CONDITION_TV_SENSOR_FAILURE		2
#define DIAGNOSIS_CONDITION_FV_SENSOR_FAILURE		3
#define DIAGNOSIS_CONDITION_PV_SENSOR_OUT_LIMS		4
#define DIAGNOSIS_CONDITION_SV_SENSOR_OUT_LIMS		5
#define DIAGNOSIS_CONDITION_TV_SENSOR_OUT_LIMS		6
#define DIAGNOSIS_CONDITION_FV_SENSOR_OUT_LIMS		7
#define DIAGNOSIS_CONDITION_PV_SENSOR_OUT_RANGE	10
#define DIAGNOSIS_CONDITION_SV_SENSOR_OUT_RANGE	11
#define DIAGNOSIS_CONDITION_TV_SENSOR_OUT_RANGE	12
#define DIAGNOSIS_CONDITION_FV_SENSOR_OUT_RANGE	13
#define DIAGNOSIS_CONDITION_FE_BOARD_FAILURE		16
#define DIAGNOSIS_CONDITION_FE_NV_FAILURE			17
#define DIAGNOSIS_CONDITION_FE_NV_WARNING			18
#define DIAGNOSIS_CONDITION_FE_BOARD_NOT_DETECTED	19
#define DIAGNOSIS_CONDITION_FE_BOARD_COMM_ERROR	20
#define DIAGNOSIS_CONDITION_CB_NV_FAILURE			23
#define DIAGNOSIS_CONDITION_CB_NV_WARNING			24
#define DIAGNOSIS_CONDITION_NV_STORAGE_ACTIVE		25
#define DIAGNOSIS_CONDITION_NV_CONCISTENCY_CHECK	26
#define DIAGNOSIS_CONDITION_STEAM_TYPE_MISMATCH 30
#define DIAGNOSIS_CONDITION_MAINTENANCE_WARNING 31
#define DIAGNOSIS_CONDITION_CB_INSUF_INPUT_VOLTAGE 32
#define DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMIN 33
#define DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMAX 34
#define DIAGNOSIS_CONDITION_LOW_FLOWRATE_CUTOFF 35
#define DIAGNOSIS_CONDITION_PROCESS_FLOWRATE_MORE_THAN_103 36
#define DIAGNOSIS_CONDITION_DATA_SIMU_WARNING 37
#define DIAGNOSIS_CONDITION_ALARM_SIMU_WARNING 38
#define DIAGNOSIS_CONDITION_NV_REPLACE_WARN 43
#define DIAGNOSIS_CONDITION_FE_RAM_FAILURE 44
#define DIAGNOSIS_CONDITION_TOT_COUNTER_STOP 45
#define DIAGNOSIS_CONDITION_NO_HART_BURST_IN 47
#define DIAGNOSIS_CONDITIONS_NUM_ELEMENTS 33 				//Diagnosis conditions number
#define DIAGNOSIS_MASKING_NUM_ELEMENTS 24
#define DIAGNOSIS_CONDITIONS_UNSET 255
//@CodeGen end@ <ALARM CONDITIONS>

// simulationStatusCodes
#define DIAGNOSIS_SIMULATION_DISABLED       0   //Disable alarm simulation
#define DIAGNOSIS_SIMULATION_ENABLED        1   //Enable alarm simulation and set the output automatically at least to check function
#define DIAGNOSIS_SIMULATION_ENABLED_OUTPUT	2	//Enable output alarm simulation
#define DIAGNOSIS_SIMULATION_NUM_ELEMENTS   3   //Simulation status number of elements


//@CodeGen start@ <T_DIAGNOSIS_GROUPS>
typedef enum _T_DIAGNOSIS_GROUPS
{
  DIAGNOSIS_GROUP_HW_STATUS_ELECTRONICS = 0,
  DIAGNOSIS_GROUP_HW_STATUS_SENSOR,
  DIAGNOSIS_GROUP_CONFIG_STATUS,
  DIAGNOSIS_GROUP_OPERATING_CONDITION,
  DIAGNOSIS_GROUPS_NUM_ELEMENTS = 4,
  DIAGNOSIS_GROUP_ALL = 0xFF
} T_DIAGNOSIS_GROUPS;
//@CodeGen end@ <T_DIAGNOSIS_GROUPS>

typedef enum _T_DIAGNOSIS_CLASSES
{
    DIAGNOSIS_CLASS_NONE = 0,
    DIAGNOSIS_CLASS_CHECK_FUNCTION,
    DIAGNOSIS_CLASS_OFF_SPECIFICATION,
    DIAGNOSIS_CLASS_MAINTENANCE,
    DIAGNOSIS_CLASS_FAILURE
} T_DIAGNOSIS_CLASSES;

//----------------------------------------------------------------------------------------------------------
//! FAST Output User Defined Behaviours Constant Definitions
// todoCode: Verify the position of the following defines
#define ALARM_BEHAVIOUR_OK                  0
#define ALARM_BEHAVIOUR_GENERAL_FAILURE     1

//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output
typedef TUSIGN8 T_BEHAVIOUR;

#define INACTIVE_GROUP          0x00
#define ACTIVE_GROUP            0xFF

typedef struct _T_DIAGNOSIS_STATUS
{
    T_DIAGNOSIS_CLASSES classification;
    TUSIGN8             activeGroups[DIAGNOSIS_GROUPS_NUM_ELEMENTS];
} T_DIAGNOSIS_STATUS;

typedef struct _T_DIAGNOSIS_ACTIVE_CONDITION
{
    TUSIGN8 activeConditionId;
    T_DIAGNOSIS_CLASSES classification;
    T_DIAGNOSIS_GROUPS group;
    TUSIGN8 priority;
} T_DIAGNOSIS_ACTIVE_CONDITION;

typedef struct _T_CONDITION_DETAIL
{
    T_SUBSYSTEM_IDX subIdx;
    TUSIGN8         alarm;
} T_CONDITION_DETAIL;

typedef struct _T_DIAGNOSIS_DETAILED_CONDITION
{
    T_DIAGNOSIS_CLASSES classification;
    T_DIAGNOSIS_GROUPS group;
    TUSIGN8 priority;
    T_DIAGNOSIS_ALARM_COUNTER runtimeCounter;
#ifdef DIAGNOSIS_USE_CONDITION_DETAILS
    T_CONDITION_DETAIL details[DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER];
#endif
} T_DIAGNOSIS_DETAILED_CONDITION;

//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
//@CodeGen start@ <E_DIAGNOSIS_DC_INDEXES>
enum _E_DIAGNOSIS_DC_INDEXES
{
  DIAGNOSIS_DC_IDX_DYNAMIC,
  DIAGNOSIS_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT,
  DIAGNOSIS_DC_IDX_STATIC_CYCLIC,
  DIAGNOSIS_DC_IDX_NUMBER_OF_DATACLASSES
};
//@CodeGen end@ <E_DIAGNOSIS_DC_INDEXES>

//-------------------------------------------------------------------------------------------------
//!  definition of class T_DIAGNOSIS
typedef struct _T_DIAGNOSIS
{
    // inherit T_UNIT
    T_UNIT unit;

#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
      T_BEHAVIOUR(*GetStatusForOutputSRV) (T_SUBSYSTEM_IDX);
#endif
      TUSIGN16(*UpdateDeviceStatusEXE) (void);
      TUSIGN16(*GetNAMURStatusSRV) (T_DIAGNOSIS_STATUS *);
      TUSIGN16(*GetActiveDiagnosticConditionsSRV) (T_DIAGNOSIS_GROUPS,
                                                   T_DIAGNOSIS_ACTIVE_CONDITION *);
      TUSIGN16(*GetDetailsOfDiagnosticConditionSRV) (TUSIGN8, T_DIAGNOSIS_DETAILED_CONDITION *);
} T_DIAGNOSIS;

//-------------------------------------------------------------------------------------------------
//! define the T_DIAGNOSIS-object
extern SLOW const T_DIAGNOSIS diagnosis;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_DIAGNOSIS
#define DIAGNOSIS_ME (&diagnosis.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define diagnosis_Get(objectIndex,attributeIndex,ptrValue) \
   diagnosis.unit.Get(DIAGNOSIS_ME,objectIndex,attributeIndex,ptrValue)
#define diagnosis_Put(objectIndex,attributeIndex,ptrValue) \
   diagnosis.unit.Put(DIAGNOSIS_ME,objectIndex,attributeIndex,ptrValue)
#define diagnosis_Check(objectIndex,attributeIndex,ptrValue) \
   diagnosis.unit.Check(DIAGNOSIS_ME,objectIndex,attributeIndex,ptrValue)

#define diagnosis_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME,objectIndex,attributeIndex,ptrDescriptor)
#define diagnosis_GetObjectDescription(objectIndex,ptrDescriptor) \
   diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME,objectIndex,ptrDescriptor)
   
   



