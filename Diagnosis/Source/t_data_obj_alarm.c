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
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "nv_mem/Interface/nv_mem.h"


//--------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::ALARM
*/
//--------------------------------------------------------------------------------------------------
//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
static SLOW const TUSIGN8 cmAryAttributeDataTypes_ALARM[] = {ST_E8};

const T_DATA_CLASS cgClassInitializer_ALARM = {
	cmAryAttributeDataTypes_ALARM,
	0,                                  // Checked by read/write handler
	0,                                  // Transform2Internal 0==will not be called
	0,                                  // Transform2External 0==will not be called
	Get_ALARM,                          // overload
	Put_ALARM,                          // overload
	GetAttributeDescription_ALARM,      // overload
	GetObjectDescription_DATAOBJ        // inherit
};


//--------------------------------------------------------------------------------------------------
/*!
 \brief      Check the new value for an object or an attribute against the businessrules.
             an errorcode will be returned, the actual object or attribute value will be returned
             also.
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \param      ptrValue
 \param      attributeIndex -1 --> check Object; >=0 --> check Attribute
 \return     error-code
 <pre>
    OK                         operation was successful
    TDO_RULE_VIOLATION_ERR     the code is not part of the enumeration-table
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
TUSIGN16 Check_ALARM(const T_DATA_OBJ SLOW * me, void FAST * ptrValue, TINT16 attributeIndex)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;
	T_DATA_OBJ myself;
	TUSIGN8 tabIndex;

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrValue);

	// speed and code optimization
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	myself = *me;

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	if (attributeIndex != WHOLE_OBJECT)
	{
		T_RANGE_E8 descriptor;

        // validate data-object
        VIP_ASSERT(myself.ptrDescription);
        descriptor = *((const T_RANGE_E8 SLOW *)myself.ptrDescription);

		// part of enum-table?
		for (tabIndex = 0;
			(tabIndex < descriptor.numberOfCodes) &&
			attributeIndex != descriptor.ptrCodeTab[tabIndex]; tabIndex++) ;

		if (tabIndex >= descriptor.numberOfCodes)
		{
			//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
			result = RULE_VIOLATION_ERR;
		}

        VIP_ASSERT(result != RULE_VIOLATION_ERR);
    }

	return result;
	//lint -e{818} Giovanni Invernizzi 2007-06-26 interface is fixed
}

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
	void FAST * ptrValue, TINT16 attributeIndex, T_DATA_SEMAPHORE FAST * ptrDataSemaphore)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16    result = OK;
	T_DATA_OBJ  myself;

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrValue);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrDataSemaphore);

	// speed and code optimization
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	myself = *me;

    // validate data-object
    VIP_ASSERT(myself.numberOfAttributes);          // divide by zero
    VIP_ASSERT(myself.ptrValue);                    // pointer
    VIP_ASSERT(myself.pClass);                      // pointer
    VIP_ASSERT(myself.storageQualifier != ROM);     // this method does not support ROM

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	if (attributeIndex >= (TINT16) (myself.numberOfAttributes) || attributeIndex < ATTRIB_0)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = ILLEGAL_ATTRIB_IDX;
	}
	else
	{
		TUSIGN8 dataByte, dataBit;

		result = Check_ALARM(me, ptrValue, attributeIndex);

		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		if (result < ERROR_CODES)
		{
			// use semaphore for protected memory
			if (ptrDataSemaphore)
			{
				ENABLE_DATA_PROTECTION(*ptrDataSemaphore);
			}

			//lint -e{734} Giovanni Invernizzi 2007-06-26 Possible loss of precision is OK
			dataByte = (TUSIGN16)attributeIndex >> 3;       // equal to / 8
			dataBit = attributeIndex & 0x07;                // equal to % 8

			// set-up value
			if (*(((TUSIGN8 *) myself.ptrValue) + dataByte) & (1 << dataBit))
				//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
				*((TUSIGN8 *) ptrValue) = SET_ALARM;
			else
				//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
				*((TUSIGN8 *) ptrValue) = CLEAR_ALARM;

			if (ptrDataSemaphore)
			{
				DISABLE_DATA_PROTECTION(*ptrDataSemaphore);
			}
		}
	}

    VIP_ASSERT(result < DEBUG_ERROR);

	return result;
}

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
	void FAST * ptrValue, TINT16 attributeIndex, T_DATA_SEMAPHORE FAST * ptrDataSemaphore)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;
	TUSIGN8 FAST *ptrTarget = 0;
	T_DATA_OBJ myself;
	TUSIGN8 FAST *dataPointer = 0;
	TUSIGN8 value;
	TUSIGN8 tempalarm;

	// validate parameter
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrValue);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrDataSemaphore);

	// speed and code optimization
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	myself = *me;

	// speed and code optimization
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	value = *((TUSIGN8 *) ptrValue);

    // validate data-object
    VIP_ASSERT(myself.numberOfAttributes);          // divide by zero
    VIP_ASSERT(myself.ptrValue);                    // pointer
    VIP_ASSERT(myself.pClass);                      // pointer
    VIP_ASSERT(myself.storageQualifier != ROM);     // this method does not support ROM
    VIP_ASSERT(value == CLEAR_ALARM || value == SET_ALARM);

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	if (attributeIndex >= (TINT16) (myself.numberOfAttributes) || attributeIndex < WHOLE_OBJECT)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = ILLEGAL_ATTRIB_IDX;
	}
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	else if (attributeIndex == WHOLE_OBJECT && value == SET_ALARM)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = METHOD_NOT_SUPPORTED;
	}
	else
	{
		result = Check_ALARM(me, ptrValue, attributeIndex);

		if (result < ERROR_CODES)
		{
			// transform to internal view
			// optimized myself.pClass->Transform2Internal(me,ptrValue,attributeIndex);

			// the target is basicly the object-value
			ptrTarget = (TUSIGN8 FAST *) myself.ptrValue;

			// use semaphore for protected memory
			if (ptrDataSemaphore)
			{
				ENABLE_DATA_PROTECTION(*ptrDataSemaphore);
			}

			// copy the value
			//change for nv fast save t_alarm_data_object  Spring 2012.8.7
			if (attributeIndex == WHOLE_OBJECT)
			{
				TUSIGN8 counter;

				for (counter = 0; counter < myself.objectLength; counter++)
				{
					dataPointer = (ptrTarget + counter);
					tempalarm = 0;
					if(myself.storageQualifier == NON_VOLATILE)
					{
						if( NV_OK!=PutData_NV_MEM(dataPointer,&tempalarm,1))
						{
							result = FATAL_ERROR;
						}
					}
					else
					{
						*dataPointer = 0;
					}
				}
			}
			else
			{
				TUSIGN8 dataByte, dataBit;

				//lint -e{734} Giovanni Invernizzi 2007-06-26 Possible loss of precision is OK
				dataByte = (TUSIGN16)attributeIndex >> 3;       // equal to / 8
				dataBit = attributeIndex & 0x07;                // equal to % 8
				ptrTarget += dataByte;

				tempalarm = *ptrTarget;

				if (value == SET_ALARM)
                {
                    tempalarm |= (1 << dataBit);
                }
				else
                {
                    tempalarm &= ~((TUSIGN8)(1 << dataBit));
                }
					
				if(myself.storageQualifier == NON_VOLATILE)
				{
					if( NV_OK!=PutData_NV_MEM((TUSIGN8 const FAST*)ptrTarget,&tempalarm,1))
					{
						result = FATAL_ERROR;
					}
				}
				else
				{
					*ptrTarget = tempalarm;
				}

			}

			if (ptrDataSemaphore)
			{
				DISABLE_DATA_PROTECTION(*ptrDataSemaphore);
			}
		}
	}

    VIP_ASSERT(result < DEBUG_ERROR);

	return result;
}

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
	TUSIGN16 attributeIndex)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;
	T_DATA_OBJ myself;

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrDescriptor);

	// speed and code optimization
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	myself = *me;

	VIP_ASSERT(myself.pClass);              // pointer
	VIP_ASSERT(myself.ptrDescription);

	if (attributeIndex >= myself.numberOfAttributes)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = ILLEGAL_ATTRIB_IDX;
	}
	else
	{
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		ptrDescriptor->rulesAreActive = eTRUE;
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		ptrDescriptor->dataType = ST_E8;
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		ptrDescriptor->range.e8 = *((const T_RANGE_E8 SLOW *)myself.ptrDescription);
	}

	return result;
}