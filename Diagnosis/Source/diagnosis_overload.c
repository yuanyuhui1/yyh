//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "diagnosis_overload.h"
#include "diagnosis_execute.h"
#include "diagnosis_types.h"
#include "diagnosis_attributes.h"
#include "diagnosis_table_default.h"

#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
#include "ARM/Interface/arm_idx.h"
#include "ARM/Interface/arm.h"
#endif

static const T_UNIT SLOW         *pSimulationSub;
static TUSIGN16                     objIdxSimulation;
static const T_DATA_OBJ SLOW    *ptrSimulationObj;
#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
const T_UNIT SLOW             *pArmSub;
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief      Initialize the subsystem; (hardware, tasks, data, calculation)

 \param      typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return     error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_DIAGNOSIS(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;

	VIP_ASSERT(me == DIAGNOSIS_ME);

	result = Initialize_T_UNIT(me, typeOfStartUp);

	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	if (result == OK)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		if ((typeOfStartUp & INIT_DATA) != 0)
		{
			// init execute local variables
			InitLocalVarables();

			// set up simulation pointers
			pSimulationSub = 0;
			ptrSimulationObj = 0;

#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
			// set up arm pointers
			pArmSub = GetSubsystemPtr(ARM_IDX);
			VIP_ASSERT(pArmSub);
#endif
			//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
			(void)ChangeState_T_UNIT(me, ACTIVE);

			// initialize dynamic data
			result |= diagnosis.unit.LoadRomDefaults(DIAGNOSIS_ME,DIAGNOSIS_DC_IDX_DYNAMIC);
		}
	}

	return result;
}

//--------------------------------------------------------------------------------------------------
/*!
\brief      put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
\author     Giovanni Invernizzi
\date       2006-01-09
\param      objectIndex    object index
\param      attributeIndex attribute index; <0 --> put whole object
\param      ptrValue  pointer to attribute-value and return-Value
\return     error-code
<pre>
    OK                      operation was successful
    LESSTHAN_RANGEMIN       at least one value is less than its minimum value
    GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
    SPAN_TO_SMALL           then span between two values is to small
    NOT_ON_GRID             at least one value is not a multiple of its increment
    RULE_VIOLATION          this shows a violation of one or more businessrules
    DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
    ILLEGAL_OBJ_IDX         DEBUG, unknown Object
    ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
</pre>
\test
 \author     Giovanni Invernizzi
 \date       2006-01-09
 \test
 \n by:                  Giovanni Invernizzi
 \n date:                2008-12-17
 \n environment:         CSpy
 \n result:              OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_T_UNIT_DIAGNOSIS(const T_UNIT SLOW*me,
                              TUSIGN16 objectIndex,
                              TINT16 attributeIndex,
                              void FAST* ptrValue)
{
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	TUSIGN16 result = OK;
	const T_DATA_OBJ SLOW* ptrObj=0;

	// check for zeropointer
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(me);
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	VIP_ASSERT(ptrValue);
	VIP_ASSERT(me->ptrObjectList);

	// exception if not initialized
	//lint -e{746} Giovanni Invernizzi 2007-06-26
	//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	VIP_ASSERT(*(me->ptrState) >= INITIALIZED);

	// unknown object
	//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
	if (objectIndex > me->maxIdx)
	{
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		result = ILLEGAL_OBJ_IDX;
	}
	else
	{
		// handmade code optimizer
		//lint -e{613} Giovanni Invernizzi 2007-06-26 Pointer is checked via VIP_ASSERT
		ptrObj = &(me->ptrObjectList[objectIndex]);

		// simulation indexes
		//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
		if (objectIndex >= FIRST_SIMULATION_OBJECT && objectIndex <= LAST_SIMULATION_OBJECT)
		{
			// get simulation status
			if (diagnosisDynamic.simulationStatus == DIAGNOSIS_SIMULATION_DISABLED)
			{
				//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
				result = RULE_VIOLATION_ERR;
			}
			else
			{
				TUSIGN8     value;
				TUSIGN16    simulationObjTableIdx;
#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
				TUSIGN8     userType;
#endif
				//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
				simulationObjTableIdx = objectIndex - FIRST_SIMULATION_OBJECT;

#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
				// here the diagnosis subsystem shall check the simulation business rule
				// 1 - If the request come from an advanced user no check over simulation
				// 2 - If the request come from a standard user only only one alarm simulation at time is permitted
				pArmSub->Get(pArmSub, ARM_IDX_userType, WHOLE_OBJECT, &userType);

				if (userType <= ARM_STANDARD_USER)
				{
#endif
					if (*((TUSIGN8 *)ptrValue) == SET_ALARM)
					{
						// if necessary, clear last alarm before setting the new one
						if (pSimulationSub != 0) {
							value = CLEAR_ALARM;
							result = pSimulationSub->Put(pSimulationSub, objIdxSimulation, (TINT16)WHOLE_OBJECT, &value);
							//lint -e{746} Giovanni Invernizzi 2007-06-26
							VIP_ASSERT(result == OK);

							result = ptrSimulationObj->pClass->Put(ptrSimulationObj, &value, (TINT16)WHOLE_OBJECT, me->ptrDataSemaphore);
							//lint -e{746} Giovanni Invernizzi 2007-06-26
							VIP_ASSERT(result == OK);
						}

						// store in local variables the information about the alarm simulation subsystem
						pSimulationSub = simulationObjTable[simulationObjTableIdx].pSub;
						objIdxSimulation = simulationObjTable[simulationObjTableIdx].objIdx;

						// store in local variables the information about the alarm simulation local objects
						ptrSimulationObj = ptrObj;
					}
					else
					{
						if (attributeIndex > WHOLE_OBJECT)
							result = ptrObj->pClass->Get(ptrObj, &value, attributeIndex, me->ptrDataSemaphore);

						if (value == SET_ALARM || attributeIndex == WHOLE_OBJECT)
						{
							// if the user clear simulation initialize local variables
							pSimulationSub = 0;
							ptrSimulationObj = 0;
						}
					}
#ifdef DIAGNOSIS_USE_ACCESS_RIGHT_MANAGEMENT
				}
#endif
				// set the simulation value in the right subsystem
				result = simulationObjTable[simulationObjTableIdx].pSub->Put(simulationObjTable[simulationObjTableIdx].pSub, simulationObjTable[simulationObjTableIdx].objIdx, attributeIndex, ptrValue);
				//lint -e{746} Giovanni Invernizzi 2007-06-26
				VIP_ASSERT(result == OK);

				result = ptrObj->pClass->Put(ptrObj, ptrValue, attributeIndex, me->ptrDataSemaphore);

				VIP_ASSERT(result == OK);

			}
		}
		else
		{
			// if the user disable the simulation all the alarms are cleared automatically
			//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
			if (objectIndex == DIAGNOSIS_IDX_simulationStatus && *((TUSIGN8 *)ptrValue) == DIAGNOSIS_SIMULATION_DISABLED)
			{
				TUSIGN8 clearValue, iterator;
				const T_DATA_OBJ SLOW* ptrDiagnosisObj=0;

				clearValue = CLEAR_ALARM;

				//lint -e{641} Giovanni Invernizzi 2007-06-26 Converting enum to int accepted
				for (iterator = FIRST_SIMULATION_OBJECT; iterator <= LAST_SIMULATION_OBJECT; iterator++)
				{
					// clear simulation value in diagnosis subsystem
					ptrDiagnosisObj = &(me->ptrObjectList[iterator]);
					result = ptrDiagnosisObj->pClass->Put(ptrDiagnosisObj, &clearValue, (TINT16)WHOLE_OBJECT, me->ptrDataSemaphore);
					//lint -e{746} Giovanni Invernizzi 2007-06-26
					VIP_ASSERT(result == OK);
				}

				//lint -e{656} Giovanni Invernizzi 2007-06-26 Arithmetic operation uses (compatible) enum's accepted
				for (iterator = 0; iterator < SIMULATION_OBJ_TABLE_SIZE; iterator++)
				{
					// clear simulation value in all subsystems
					result = simulationObjTable[iterator].pSub->Put(simulationObjTable[iterator].pSub, simulationObjTable[iterator].objIdx, (TINT16)WHOLE_OBJECT, &clearValue);
					//lint -e{746} Giovanni Invernizzi 2007-06-26
					VIP_ASSERT(result == OK);
				}

				// if the user clear simulation re-initialize local variables
				pSimulationSub = 0;
				ptrSimulationObj = 0;

			}

			// other data type indexes
			result = ptrObj->pClass->Put(ptrObj, ptrValue, attributeIndex, me->ptrDataSemaphore);
		}
	}

	return result;
}

