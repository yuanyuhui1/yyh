//----------------------------------------------------------------------------------------------------------
/*
Copyright              Copyright ABB, 2011.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Common Framework
Module         t_data_obj_modbus_diag.c
Description    implementation of t_data_obj_modbus_diag methods.
t_data_obj_modbus_diag handles modbus diagnostic alarm.

Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
//#include "..\..\services\interface\nv_mem.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
//#include "..\interface\t_data_obj_modbus_diag_history.h"
#include "T_Unit/Interface/t_unit.h"
//#include "modbus_types.h"
//#include "modbus_attributes.h"
//#include "..\interface\modbus_idx.h"
//#include "..\interface\modbus.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Coordinator/Interface/subsystem_idx.h"
//#include "..\..\diagnosis\interface\diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"

//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ

T_DATA_OBJ has no businessrules; the used data-type is TUSIGN8
*/
//----------------------------------------------------------------------------------------------------------
//lint --e{793} by CNZUWAN AS code review is accetable.
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = { ST_U8 };


TUSIGN16 Put_MODBUS_DIAG(const T_DATA_OBJ SLOW*me,
                         void FAST* ptrValue,
                         TINT16 attributeIndex,
                         T_DATA_SEMAPHORE FAST* ptrDataSemaphore);

TUSIGN16 Get_MODBUS_DIAG(const T_DATA_OBJ SLOW*me,
                         void FAST* ptrValue,
                         TINT16 attributeIndex,
                         T_DATA_SEMAPHORE FAST* ptrDataSemaphore);

const T_DATA_CLASS cgClassInitializer_MODBUS_DIAG =
{
    cmAryAttributeDataTypes,
    Check_DATAOBJ,                              // Check() not supported
    0,                                          // Transform2Internal() not supported
    0,                                          // Transform2External() not supported
    Get_MODBUS_DIAG,                            // Overload
    Put_MODBUS_DIAG,                            // Inherit
    GetAttributeDescription_DATAOBJ,            // Inherit
    GetObjectDescription_DATAOBJ                // Inherit
};




//--------------------------------------------------------------------------------------------------
/*!
\brief  This method blocks any put request, as the diagnostic bytes are read only, write request is
not acceptable.
\author Shen Quayum
\date   2011-01-01
\param  ptrValue the new object or attribute value; if return!=OK than return the
nearest possible value in ptrValue
\param  attributeIndex <0 --> Object; >=0 --> Attribute
\param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
\warning
\test
\n by: Shen Quayum
\n date   2011-02-01
\n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
\n result module test: 2005-07-25 OK
\n result Lint Level 3: free // approved warnings and approved infos are suppresed
\bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715}
TUSIGN16 Put_MODBUS_DIAG(const T_DATA_OBJ SLOW*me,
                         void FAST* ptrValue,
                         TINT16 attributeIndex,
                         T_DATA_SEMAPHORE FAST* ptrDataSemaphore)
{
    TUSIGN16 result;
    result = READ_ONLY_ERR;
    return (result);
}



T_DIAGNOSIS_ACTIVE_CONDITION mActiveConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
//--------------------------------------------------------------------------------------------------
/*!
\brief  This method extracts the active diagnostic alarm information from diagnostic sub-system,
and returns this to the external interface.

\author Shen Quayum
\date   2011-01-28
\param  the new object or attribute value will be returned in ptrValue
\param  attributeIndex ==-1 --> Object; >=0 --> Attribute
\param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
\return error-code

\warning
\test
\n by: Shen Quayum
\n date   2011-02-01
\n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
\n result module test: 2005-07-25 OK
\n result Lint Level 3: free // approved warnings and approved infos are suppresed
\bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_MODBUS_DIAG(const T_DATA_OBJ SLOW*me,
                         void FAST* ptrValue,
                         TINT16 attributeIndex,
                         T_DATA_SEMAPHORE FAST* ptrDataSemaphore)
{
    TUSIGN16 result = OK;
    TUSIGN16 alarmId = attributeIndex;
    TUSIGN8 selectedCondition = 0;
    T_DIAGNOSIS_ACTIVE_CONDITION mActiveConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
    
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->numberOfAttributes); // divide by zero
    VIP_ASSERT(me->ptrValue); // pointer
    VIP_ASSERT(me->pClass);   // pointer
    VIP_ASSERT(me->storageQualifier!=ROM); // this method does not support ROM
    
    (void)memset(mActiveConditionList, 0, sizeof(mActiveConditionList));

    // Also reset the diagnostic structure before populating it.
    (void)memset(me->ptrValue,0, sizeof(T_MODBUS_DIAG));
    
    // use semaphore for protected memory
    if( ptrDataSemaphore )
    {
        //lint -e{619} SD 2006-07-14 no actual loss of ptrDataSemaphore pointer precision.
        (void)ENABLE_DATA_PROTECTION(*ptrDataSemaphore);
    }
    
    //Update all the bits
    (void)diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, mActiveConditionList);  //del by Spring
    for(alarmId =0; (alarmId < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER) &&
        (selectedCondition !=  DIAGNOSIS_CONDITIONS_UNSET);
        alarmId++)
    {
        selectedCondition = mActiveConditionList[alarmId].activeConditionId;
        ((T_MODBUS_DIAG FAST*)me->ptrValue)->alarmBits[selectedCondition] = SET_ALARM;
    }    
    
    if(attributeIndex == WHOLE_OBJECT)
    {  
        (void)memcpy(ptrValue,(T_MODBUS_DIAG FAST*)me->ptrValue,sizeof(T_MODBUS_DIAG));
    }
    else    // Process when it is not a whole object.
    {
        result = ILLEGAL_ATTRIB_IDX;
        
        // attribute index must not exceed alarm buffer size sizeof(T_MODBUS_DIAG)
        if(attributeIndex < sizeof(T_MODBUS_DIAG))
        {
            result = OK;
            (void)memcpy(ptrValue, &(((T_MODBUS_DIAG FAST*)me->ptrValue)->alarmBits[attributeIndex]), 1);
        }
    }
    
    if( ptrDataSemaphore )
    {
        DISABLE_DATA_PROTECTION(*ptrDataSemaphore);
    }
    
    return (result);
}