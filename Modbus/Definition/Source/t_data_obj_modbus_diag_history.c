//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         t_data_obj_modbus_diag_history.c
 Description    implementation of t_data_obj_modbus_diag_history methods.
				t_data_obj_modbus_diag_history handles modbus alarm history.

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
//#include "..\interface\t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"
#include "T_Unit/Interface/t_unit.h"
//#include "modbus_types.h"
//#include "modbus_attributes.h"
//#include "..\interface\modbus_idx.h"
//#include "..\interface\modbus.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
//#include "..\..\DiagnosisServices\interface\DiagnosisServices.h"
//#include "..\..\DiagnosisServices\interface\DiagnosisServices_idx.h"

//lint --e{793} by CNZUWAN AS code review is accetable.
//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ

    T_DATA_OBJ has no businessrules; the used data-type is TUSIGN8
*/
//----------------------------------------------------------------------------------------------------------
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = { ST_U8 };

// Prototypes for module tests of public methods.
TUSIGN16 Get_MODBUS_DIAG_HIS(const T_DATA_OBJ SLOW*me,
                             void FAST* ptrValue,
                             TINT16 attributeIndex,
                             T_DATA_SEMAPHORE FAST* ptrDataSemaphore);

TUSIGN16 Put_MODBUS_DIAG_HIS(const T_DATA_OBJ SLOW*me,
                             void FAST* ptrValue,
                             TINT16 attributeIndex,
                             T_DATA_SEMAPHORE FAST* ptrDataSemaphore);



//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ for ModBus diagnostic History.

    T_DATA_OBJ has no businessrules;
*/
//----------------------------------------------------------------------------------------------------------
const T_DATA_CLASS cgClassInitializer_MODBUS_DIAG_HISTORY =
{
    cmAryAttributeDataTypes,
    Check_DATAOBJ,                      // Check() not supported
    0,                                  // Transform2Internal() not supported
    0,                                  // Transform2External() not supported
    Get_MODBUS_DIAG_HIS,                // Overload
    Put_MODBUS_DIAG_HIS,                // Overload
    GetAttributeDescription_DATAOBJ,    // Inherit
    GetObjectDescription_DATAOBJ        // Inherit
};




//--------------------------------------------------------------------------------------------------
/*!
 \brief  This method extracts the alarm history informationfrom diagnostic sub-system, and return these
           values to the caller.
 \author Shen Quayum
 \date   2011-01-28
 \param  the new object or attribute value will be returned in ptrValue
 \param  attributeIndex ==-1 --> Object; >=0 --> Attribute
 \param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \return error-code
 \warning
 \test
 \n by: Shen Quayum
 \data  2011-02-01
 \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
 \n result module test:
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_MODBUS_DIAG_HIS(const T_DATA_OBJ SLOW*me,
                             void FAST* ptrValue,
                             TINT16 attributeIndex,
                             T_DATA_SEMAPHORE FAST* ptrDataSemaphore)
{
  TUSIGN16 result;
  TUSIGN16 index = (TUSIGN16)attributeIndex;
  TUSIGN8 bitPosition = 0;
  TUSIGN8 bytePosition = 0;
  TUSIGN8 bitMask = 0;
  TUSIGN8 maxBitPosition = 8;
  TUSIGN8 historyBuffIndex = 0;
  TUSIGN8 historyBuff[6/*DIAGNOSIS_ALARM_HISTORY_SIZE*/];
  T_DO_ATTRIB_RANGE attribRange;
  TUSIGN8 counter;
  TUSIGN8 dataBit;
  TUSIGN8 dataByte;
  TUSIGN8 attributeIdx;
  TUSIGN8 getValue;

  // debug zero pointer and illegal values
  VIP_ASSERT(me);
  VIP_ASSERT(ptrValue);
  VIP_ASSERT(me->numberOfAttributes); // divide by zero
  VIP_ASSERT(me->ptrValue); // pointer
  VIP_ASSERT(me->pClass);   // pointer
  VIP_ASSERT(me->storageQualifier!=ROM); // this method does not support ROM

  // Reset alarm history buffer.
  (void)memset(historyBuff, 0,sizeof(historyBuff));

  // Also reset the diagnostic structure before populating it.
  (void)memset(me->ptrValue,0, sizeof(T_MODBUS_DIAG_HIS));

  // use semaphore for protected memory
  if( ptrDataSemaphore )
  {
    (void)ENABLE_DATA_PROTECTION(*ptrDataSemaphore);
  }

  result = diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, DIAGNOSIS_IDX_alarmHistory, ATTRIB_0, &attribRange);

  for (counter = 0; counter < attribRange.range.e8.numberOfCodes; counter++)
  {

    attributeIdx = attribRange.range.e8.ptrCodeTab[counter];
    result |= diagnosis.unit.Get(DIAGNOSIS_ME, DIAGNOSIS_IDX_alarmHistory, attributeIdx, &getValue);

    if (getValue == SET_ALARM)
    {
      dataByte = attributeIdx >> 3;   // equal to / 8
      dataBit = attributeIdx & 0x07;  // equal to % 8
      historyBuff[dataByte] |= (1 << dataBit);
    }
  }
  if(attributeIndex == WHOLE_OBJECT)
  {
    for(index = 0; index < sizeof(historyBuff); index++)
    {
      for(bitPosition = 0; bitPosition < maxBitPosition; bitPosition++)
      {
        // Calculate bit mask from bit position.
        bitMask = (1 << bitPosition);

        if(historyBuff[index] & bitMask)
        {
          // Buffer index should never exceed the size of maximum buffer.
          if(historyBuffIndex > sizeof(T_MODBUS_DIAG_HIS))
          {
              VIP_ASSERT(0);
          }
          // Write the captured bit in the history buffer.
          ((T_MODBUS_DIAG_HIS FAST*)me->ptrValue)->alarmBits[historyBuffIndex] = SET_ALARM ;
        }
        historyBuffIndex++;
      }
    }
    result = Get_DATAOBJ(me, ptrValue, WHOLE_OBJECT, NULL);
  }
  else        // When attribute index is not whole object.
  {
    result = ILLEGAL_ATTRIB_IDX;

    // attribute index must not exceed alarm buffer size sizeof(T_MODBUS_DIAG)
    if((TUSIGN16)attributeIndex < sizeof(T_MODBUS_DIAG_HIS))
    {
      result = OK;

      // Now calculate the history alarm byte and bit position from the provided attribute index value.
      bytePosition = (attributeIndex)/8;

      // bit position can be extracted by taking the remainder value.
      bitPosition = (attributeIndex)%8;

      // Calculate bit mask from bit position.
      bitMask = (1 << bitPosition);

      if(historyBuff[bytePosition] & bitMask)
      {
        // Write the captured bit in the history buffer.
        ((T_MODBUS_DIAG_HIS FAST*)me->ptrValue)->alarmBits[attributeIndex] = SET_ALARM ;
      }
    }
    // Return this history
    result = Get_DATAOBJ(me, ptrValue, attributeIndex, NULL);
  }

  if( ptrDataSemaphore )
  {
    DISABLE_DATA_PROTECTION(*ptrDataSemaphore);
  }

  return (result);
}




//--------------------------------------------------------------------------------------------------
/*!
 \brief  This method blocks any put request, as the diagnostic history bytes are read only,
         write request is not acceptable.
 \author Shen Quayum
 \date   2011-01-01
 \param  ptrValue the new object or attribute value; if return!=OK than return the
         nearest possible value in ptrValue
 \param  attributeIndex <0 --> Object; >=0 --> Attribute
 \param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \warning
 \data  2011-02-01
 \n by: Shen Quayum
 \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
 \n result module test: 2005-07-25 OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715,830,818}
TUSIGN16 Put_MODBUS_DIAG_HIS(const T_DATA_OBJ SLOW*me,
                             void FAST* ptrValue,
                             TINT16 attributeIndex,
                             T_DATA_SEMAPHORE FAST* ptrDataSemaphore)
{
    TUSIGN16 result;
    result = READ_ONLY_ERR;
    return (result);
}