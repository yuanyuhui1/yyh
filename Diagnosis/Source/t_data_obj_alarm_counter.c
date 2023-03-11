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
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"

//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::ALARM_COUNTER
*/
//----------------------------------------------------------------------------------------------------------
//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
static SLOW const TUSIGN8 cmAryAttributeDataTypes_ALARM_COUNTER[] = { ST_U16, ST_U32, ST_U16, ST_U32, ST_U16 };

const T_DATA_CLASS cgClassInitializer_ALARM_COUNTER = {
	cmAryAttributeDataTypes_ALARM_COUNTER,
	Check_ALARM_COUNTER,                // Checked by read/write handler
	0,                                  // Transform2Internal 0==will not be called
	0,                                  // Transform2External 0==will not be called
	Get_DATAOBJ,                        // inherit
	Put_DATAOBJ,                        // inherit
	GetAttributeDescription_DATAOBJ,    // inherit
	GetObjectDescription_DATAOBJ        // inherit
};


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
                             TINT16 attributeIndex)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;

	// debug zero pointer and illegal values
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrValue);

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	if (attributeIndex == ALARM_COUNTER_TIME_MSEC || attributeIndex == ALARM_COUNTER_TIME_STAMP_MSEC)
	{
		TUSIGN32 *valuePointer = (TUSIGN32 *)ptrValue;

		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		if (*valuePointer >= ALARM_COUNTER_MSEC_TO_DAY)
		{
			//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
			result = GREATERTHAN_RANGEMAX_ERR;
		}
	}

	return result;
}