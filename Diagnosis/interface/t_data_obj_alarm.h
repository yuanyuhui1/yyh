//--------------------------------------------------------------------------------------------------
/*
<pre>
 Copyright      Copyright ABB, 2006.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Generic
 Module         Diagnosis Subsystem
 Description    T_DATA_OBJ::ALARM
 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
#error system.h must be included before t_data_obj_alarm.h
#endif

#ifndef __COMMON_TYPE_H__
#error common_type.h must be included before t_data_obj_alarm.h
#endif

#ifndef __T_DATA_OBJ_H__
#error t_data_obj.h must be included before t_data_obj_alarm.h
#endif

#if (T_DATA_OBJ_VERSION<0x020100)
#error T_DATA_OBJ_VERSION must be greater or equal than 2.1.0
#endif

#ifdef __T_DATA_OBJ_ALARM_H__
#error t_data_obj_alarm.h included more than once
#endif

#define __T_DATA_OBJ_ALARM_H__

//--------------------------------------------------------------------------------------------------
// Alarm Data Object Definition
#define CLEAR_ALARM     0
#define SET_ALARM       1

typedef union
{
    TUSIGN8 byte;
    struct
    {
        TUSIGN8 bit_0:1;        // byte 1
        TUSIGN8 bit_1:1;
        TUSIGN8 bit_2:1;
        TUSIGN8 bit_3:1;
        TUSIGN8 bit_4:1;
        TUSIGN8 bit_5:1;
        TUSIGN8 bit_6:1;
        TUSIGN8 bit_7:1;
    } bit;
} T_DIAGNOSIS_ALARM;

//--------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::ALARM
extern const T_DATA_CLASS cgClassInitializer_ALARM;

//--------------------------------------------------------------------------------------------------
/*!
 \brief    Constructor: initializer for objects of type array T_DATA_OBJ::ALARM
 \author   Giovanni Invernizzi
 \date     2006-01-09
 \param    w ptrValue; the address will be casted to (void FAST*)!
 \param    x objectLength
 \param    y storageQualifier
 \param    z ptr to range-description
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_ALARM(w,x,y,z) {(void FAST*)(w), 8 * (x), (x), (y), SIMPLE, (z), &cgClassInitializer_ALARM}

//--------------------------------------------------------------------------------------------------
//! prototypes of the method implementations of T_DATA_OBJ::ALARM

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Get the actual object or attribute value.
             Access to memory protected with a resource semaphore will be supported if the pointer
             to this semaphore is not zero.

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      The new object or attribute value will be returned in ptrValue
 \param      attributeIndex ==-1 --> Object; >=0 --> Attribute
 \param      ptrDataSemaphore pointer to the resource-semaphore of the used data-memory;
             0-ptr-->not protected
 \return     error-code
 <pre>
    OK                     operation was successful
    ILLEGAL_ATTRIB_IDX     DEBUG, unknown Attribute
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
TUSIGN16 Get_ALARM(const T_DATA_OBJ SLOW * me,
                   void FAST * ptrValue, TINT16 attributeIndex, T_DATA_SEMAPHORE FAST * ptrDataSemaphore);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Put a new object or attribute value into ram, if no businessrules are voilated
             Access to memory protected with a resource semaphore will be supported if the pointer
             to this semaphore is not zero.

 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      ptrValue the new object or attribute value; if return!=OK than return the nearest
             possible value in ptrValue
 \param      attributeIndex <0 --> Object; >=0 --> Attribute
 \param      ptrDataSemaphore pointer to the resource-semaphore of the used data-memory;
             0-ptr-->not protected
 \return     error-code
 <pre>
    OK                     operation was successful
    ILLEGAL_ATTRIB_IDX     DEBUG, unknown Attribute
    METHOD_NOT_SUPPORTED   DEBUG, the set alarm on WHOLE_OBJECT can't be done
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
TUSIGN16 Put_ALARM(const T_DATA_OBJ SLOW * me,
                   void FAST * ptrValue, TINT16 attributeIndex, T_DATA_SEMAPHORE FAST * ptrDataSemaphore);

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Reads the addressed attribute range-information
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      ptrValue the new object or attribute value
 \param      attributeIndex ==-1 --> putObject; >=0 --> putAttribute
 \param      ptrDataSemaphore pointer to the resource-semaphore of the used data-memory;
             0-ptr-->not protected
 \return     error-code
 <pre>
    OK                      operation was successful
    ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
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
TUSIGN16 GetAttributeDescription_ALARM(const T_DATA_OBJ SLOW * me,
                                       T_DO_ATTRIB_RANGE FAST * ptrDescriptor,
                                       TUSIGN16 attributeIndex);


//#define T_DATA_OBJ_ALARM_MODULETEST

#ifdef T_DATA_OBJ_ALARM_MODULETEST
TUSIGN8 T_DATA_OBJ_ALARM_ModuleTest(void);
#endif
