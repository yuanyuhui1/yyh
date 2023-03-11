//--------------------------------------------------------------------------------------------------
/*
<pre>
 Copyright      Copyright ABB, 2006.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Generic
 Module         Diagnosis Subsystem
 Description    T_DATA_OBJ::ALARM_COUNTER
 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
#error system.h must be included before t_data_obj_alarm_counter.h
#endif

#ifndef __COMMON_TYPE_H__
#error common_type.h must be included before t_data_obj_alarm_counter.h
#endif

#ifndef __T_DATA_OBJ_H__
#error t_data_obj.h must be included before t_data_obj_alarm_counter.h
#endif

#ifndef __DIAGNOSIS_COMPILE_TIME_DEFINITIONS_H__
#error DiagnosisCompileTimeDefinitions.h must be included before Diagnosis.h
#endif

#if (T_DATA_OBJ_VERSION<0x020100)
#error T_DATA_OBJ_VERSION must be greater or equal than 2.1.0
#endif

#ifdef __T_DATA_OBJ_ALARM_COUNTER_H__
#error t_data_obj_alarm_counter.h included more than once
#endif

#define __T_DATA_OBJ_ALARM_COUNTER_H__

//--------------------------------------------------------------------------------------------------
// Alarm Data Object Definition
#define ALARM_COUNTER_MSEC_TO_DAY    0x5265C00

enum E_DIAGNOSIS_ALARM_COUNTER
{
    ALARM_COUNTER = 0,
    ALARM_COUNTER_TIME_MSEC,
    ALARM_COUNTER_TIME_DAY,
    ALARM_COUNTER_TIME_STAMP_MSEC,
    ALARM_COUNTER_TIME_STAMP_DAY
};

typedef struct _T_DIAGNOSIS_ALARM_COUNTER
{
    TUSIGN16 alarmCounter;
    TUSIGN32 alarmTimeCounterMsec;
    TUSIGN16 alarmTimeCounterDay;

/*#ifdef DIAGNOSIS_USE_SYSTEM_TIME
    TUSIGN32 timeStampLastAlarmHi;      // full time stamp
    TUSIGN32 timeStampLastAlarmLo;
#else*/
    TUSIGN32 timeStampLastAlarmMsec;    // simple time stamp
    TUSIGN16 timeStampLastAlarmDay;
//#endif
} T_DIAGNOSIS_ALARM_COUNTER;

//--------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::ALARM_COUNTER
extern const T_DATA_CLASS cgClassInitializer_ALARM_COUNTER;

//--------------------------------------------------------------------------------------------------
/*!
 \brief     constructor; Initializer for objects of type array T_DATA_OBJ::ALARM_COUNTER
 \author    Giovanni Invernizzi
 \date      2007-03-12
 \param     w ptrValue; the address will be casted to (void FAST*)!
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_ALARM_COUNTER(w) {(void FAST*)(w), 5, sizeof(T_DIAGNOSIS_ALARM_COUNTER), \
                                    NON_VOLATILE, STRUCT, NULL, &cgClassInitializer_ALARM_COUNTER}

//----------------------------------------------------------------------------------------------------------
//! prototypes of the method implementations of T_DATA_OBJ::ALARM_COUNTER

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Check the new value for an object or an attribute against the businessrules.
             An errorcode will be returned, the actual object or attribute value will be returned
             also
 \author     Giovanni Invernizzi
 \date       2007-03-12
 \param      ptrValue
 \param      attributeIndex -1 --> check Object; >=0 --> check Attribute
 \return     error-code
 <pre>
    OK                         Operation was successful
    GREATERTHAN_RANGEMAX_ERR   The maximum value for ALARM_COUNTER_TIME_MSEC and
                               ALARM_COUNTER_TIME_STAMP_MSEC attributes is
                               ALARM_COUNTER_MSEC_TO_DAY
 </pre>
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Check_ALARM_COUNTER(const T_DATA_OBJ SLOW * me, void FAST * ptrValue,
                             TINT16 attributeIndex);

//#define T_DATA_OBJ_ALARM_COUNTER_MODULETEST

#ifdef T_DATA_OBJ_ALARM_COUNTER_MODULETEST
TUSIGN8 T_DATA_OBJ_ALARM_COUNTER_ModuleTest(void);
#endif
