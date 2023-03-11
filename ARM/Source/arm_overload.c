//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include "System/Interface/system.h"

#ifdef FRAMEWORK_VERSION_2_1_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif //FRAMEWORK_VERSION_2_1_0
#ifdef FRAMEWORK_VERSION_2_2_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif //FRAMEWORK_VERSION_2_2_0
#ifdef FRAMEWORK_VERSION_2_3_1
  #include "System/Interface/common_type.h"
  #include "System/Interface/assert.h"
#endif //FRAMEWORK_VERSION_2_3_1
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"
#include "arm_types.h"
#include "arm_attributes.h"
#include "arm_execute.h"
#include "arm_overload.h"
#include "arm_specialFunctions.h"			// contains project-dependent function variants
#ifdef DEVICE_SUBSYSTEM_PRESENT
	#include "../../device/interface/device.h"              	// to access the device's identity-
	#include "../../device/interface/device_idx.h"          	// -number, used in hashing algorithm
#endif

//-------------------------------------------------------------------------------------------------
//!

// to help manage initialization
static TBOOL hwInitialized = eFALSE;
static TBOOL timerInitialized = eFALSE;
static TBOOL dataInitialized = eFALSE;
static TBOOL calcInitialized = eFALSE;

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author
 \date
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \test
 \n by:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_ARM(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
#ifndef LOCALSERVER_SELECT
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
#endif

    TUSIGN16 result = (TUSIGN16) ILLEGAL_STATE;

    //lint -e{746}
    VIP_ASSERT(me==ARM_ME);

    if (me)
    {
    	result = Initialize_T_UNIT(me,typeOfStartUp);
    }

    if (result == (TUSIGN16) OK)
    {
        if ((typeOfStartUp & (TUSIGN8) INIT_HARDWARE) != 0)
        {
            VIP_ASSERT(!hwInitialized);     // only one occurrance

            if (!hwInitialized)
            {
                #ifndef LOCALSERVER_SELECT
                    // address the basic hardware requirements:
                    #ifdef CUSTODY_SWITCH
                    // Where applicable, get the custody switch status, else...
                    CUSTODY_SWITCH_PORT_DR = ARM_HW_SWITCH_INPUT_PORT;		// set port direction = I/P
                    // read the hardware port for read-only switch
                    if (CUSTODY_SWITCH_PORT == ARM_HW_SWITCH_INPUT_LOW)		// test I/P state
                    {
                        armDynamic.custodySwitchStatus = (TUSIGN8) ARM_CUSTODY_SWITCH_ACTIVE;
                    }
                    else
                    {
                        armDynamic.custodySwitchStatus = (TUSIGN8) ARM_CUSTODY_SWITCH_INACTIVE;
                    }
                    #else
                    // ...for no custody switch requirement, default status inactive
                    armDynamic.custodySwitchStatus = (TUSIGN8) ARM_CUSTODY_SWITCH_INACTIVE;
                    #endif
                    #ifdef READ_ONLY_SWITCH
                    // Where applicable, get the read-only switch status, else...
                    READ_ONLY_SWITCH_GPIO_CLK_ENABLE();
                    GPIO_InitStruct.Pin = READ_ONLY_SWITCH_GPIO_PIN;
                    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
                    GPIO_InitStruct.Pull = GPIO_NOPULL;
                    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                    HAL_GPIO_Init(READ_ONLY_SWITCH_GPIO_PORT, &GPIO_InitStruct);

                    // read the hardware port for read-only switch
                    if (HAL_GPIO_ReadPin(READ_ONLY_SWITCH_GPIO_PORT, READ_ONLY_SWITCH_GPIO_PIN) == ARM_HW_SWITCH_INPUT_LOW)	// test I/P state
                    {
                        armDynamic.readOnlySwitchStatus = (TUSIGN8) ARM_READ_ONLY_SWITCH_ACTIVE;
                    }
                    else
                    {
                        armDynamic.readOnlySwitchStatus = (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE;
                    }
                    #else
                    // ...for no read-only switch requirement, default status inactive
                    armDynamic.readOnlySwitchStatus = (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE;
                    #endif
                #endif
                hwInitialized = eTRUE;
            }
        }
        if ((typeOfStartUp & (TUSIGN8) INIT_TASKS) != 0)
        {
            VIP_ASSERT(hwInitialized);      // only after hw
            VIP_ASSERT(!timerInitialized);  // only one occurence

            if (!timerInitialized)
            {
                // no timers to initialise yet?
                timerInitialized = eTRUE;
            }
        }

        if ((typeOfStartUp & (TUSIGN8) INIT_DATA) != 0)
        {
            VIP_ASSERT(timerInitialized);   // only after timer
            VIP_ASSERT(!dataInitialized);   // only one occurence

            if (!dataInitialized)
            {
                // ARM data to initialise:
                AllClientAccessLevelInit_ARM();					// ARM Special Function
                // ..add any future clients' access-level initialization here:
                dataInitialized = eTRUE;
            }
        }

        if ((typeOfStartUp & (TUSIGN8) INIT_CALCULATION) != 0)
        {
            VIP_ASSERT(dataInitialized);    // only after data
            VIP_ASSERT(!calcInitialized);   // only one occurence

            if (!calcInitialized)
            {
                // no calcs to initialise yet?
                calcInitialized = eTRUE;
            }
        }
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
    /*!
     \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
     \author Heiko Kresse
     \date   2005-04-18
     \param  objectIndex    object index
     \param  attributeIndex attribute index; attributeIndex<0 --> get whole object
     \param  ptrValue  pointer for return-Value
     \return error-code
     <pre>
       OK                      operation was successful
       DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
       ILLEGAL_OBJ_IDX         DEBUG, unknown Object
       ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
     </pre>
     \test
    \n by: Heiko Kresse
    \n
    \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
    \n
    \n intention: 1. complete instruction-coverage
    \n result module test: 2006-02-14 OK
     \n result Lint Level 3: free // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_ARM(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    const T_DATA_OBJ SLOW* ptrObj=0;
    TUSIGN16 result=(TUSIGN16)OK;

    // check for zeropointer
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);

    // exception if not initialized
    VIP_ASSERT(*(me->ptrState)>=(TUSIGN16)INITIALIZED);

    // unknown object
    if( objectIndex>me->maxIdx )
    {
        result = (TUSIGN16)ILLEGAL_OBJ_IDX;
    }
    else
    {
        // handmade code optimizer
        ptrObj = &(me->ptrObjectList[objectIndex]);

		// ++ARM specific parameter checks
		(void)GetSpecial_ARM();									// ARM Special Function

		if (objectIndex == (TUSIGN16)ARM_IDX_transmitterIdStringForm)
		{
			// External wants the Device Id variable; we can only do this if the
			// Device subsystem is present; call its Get() method:

			#ifdef DEVICE_SUBSYSTEM_PRESENT
            result = device.unit.Get(DEVICE_ME, (TUSIGN16) DEVICE_IDX_transmitterIdStringForm,
                                                (TINT16) attributeIndex, ptrValue);
			#else
			result = (TUSIGN16) ILLEGAL_OBJ_IDX;
			#endif
		}
		if ((result == (TUSIGN16) OK) && (objectIndex != (TUSIGN16)ARM_IDX_transmitterIdStringForm))
		{
			// --ARM specific parameter checks
			//lint -e{713} Heiko Kresse 2004-10-12
			result = ptrObj->pClass->Get(ptrObj, ptrValue, attributeIndex, me->ptrDataSemaphore);
    	}
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
/*!
   \brief  put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
   \author Peter Vince
   \date   2004-04-18
   \param  objectIndex    object index
   \param  attributeIndex attribute index; <0 --> put whole object
   \param  ptrValue  pointer to attribute-value and return-Value
   \return error-code
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
   \warning: none
   \test
   \n by: Peter Vince
   \n intention:  1. complete code covarage;
   2. equivalence partitioning
   3. Boundary Value Analysis (parameter)
   \n result module test: 2006-03-10 OK
   \n result Lint Level 3: free // approved warnings and approved infos are suppressed
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Put_ARM(const T_UNIT SLOW * me,
                 TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST * ptrValue)
{
    TUSIGN16 result = (TUSIGN16) ILLEGAL_STATE;

	// ++ARM specific parameter checks
	switch (objectIndex)
	{
		// Externals must not write to these ARM subsystem variables,
		// at least not via the Put() method:
	case ARM_IDX_stdDefaultPassword:
	case ARM_IDX_advDefaultPassword:
	case ARM_IDX_svcDefaultPassword:
	case ARM_IDX_stdUserPassword:
	case ARM_IDX_advUserPassword:
	case ARM_IDX_svcUserPassword:
	case ARM_IDX_hmiLoginState:
	case ARM_IDX_hartLoginState:
	case ARM_IDX_transmitterIdStringForm:
	case ARM_IDX_custodySwitchStatus:
		result = (TUSIGN16) READ_ONLY_ERR;
		break;

		// These you CAN write to:
	case ARM_IDX_readOnlySwitchStatus:
	case ARM_IDX_hiSecAccessState:
	case ARM_IDX_adpin:
	case ARM_IDX_clientEnableState:
	default:
		result = (TUSIGN16) OK;
		break;
	}
	// --ARM specific parameter checks

	if (result == (TUSIGN16) OK)
	{
		result = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
	}

    return result;

}
// end Put_ARM

//-------------------------------------------------------------------------------------------------
// end arm_overload.c
//-------------------------------------------------------------------------------------------------
