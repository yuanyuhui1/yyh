//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
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
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
//lint -e{91}
#include "MVMeasurement/Interface/MVMeasurement_idx.h"

#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"
#include "mVMeasurement_overload.h"
#include "mVMeasurement_execute.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"

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
TUSIGN16 Initialize_MVM(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = (TUSIGN16)FATAL_ERROR;
    //lint -e{746} 
    VIP_ASSERT(me==MVMEASUREMENT_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);

    
    if( result == (TUSIGN16)OK )
    {
        if( (typeOfStartUp & (TUSIGN8)INIT_HARDWARE) != 0)
        {
        }

        if( (typeOfStartUp & (TUSIGN8)INIT_TASKS) != 0)
        {
			//the freq and temperature is initialized at sv.
		}

        if( (typeOfStartUp & (TUSIGN8)INIT_DATA) != 0)
        {
		  //init data. after  data  read from NV. (we assume that internal keep value is in base unit.) 
            result |= mVMeasurement.unit.LoadRomDefaults(me, (TINT16)MVMEASUREMENT_DC_IDX_DynamicDuplicated);
            result |= mVMeasurement.unit.LoadRomDefaults(me, (TINT16)MVMEASUREMENT_DC_IDX_Unprotected);
            result |= mVMeasurement.unit.LoadRomDefaults(me, (TINT16)MVMEASUREMENT_DC_IDX_Protected);
        }
    }
    

    return result;
}
//-------------------------------------------------------------------------------------------------
/*!
\brief 
\author zuochen Wang
\date 2012-12-12
\param
	me  Pointer to T_UNIT 
	objectIndex
	attributeIndex
	attributeIndex
\return
	retsult
	OK
	ILLEGAL_OBJ_IDX
\warning
\test
test-date: 2013-06-18
\n by: zuochen Wang
\n environment:
\n intention:
\n result module test:87% 
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Put_T_MVM(const T_UNIT SLOW*me,  TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
	TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 operationMode =  CDT_STEAM_MASS;
    TUSIGN8 steamType =  MVM_STEAMTYPE_WATER;
	TUSIGN8 protectedDataStatus = MVM_CALCULATE;
    TUSIGN8 reqFlag = MVM_GONFIGING;
    TUSIGN8 KSet = SVM_KFACTOR_LIQUID, KSetOld = SVM_KFACTOR_LIQUID;
    
	VIP_ASSERT(me);
	VIP_ASSERT(ptrValue);
	VIP_ASSERT(*(me->ptrState) >= (TUSIGN16)INITIALIZED); // exception if not initialized
	VIP_ASSERT(me->ptrObjectList);

    // unknown object
    if( objectIndex>me->maxIdx )
    {
        result = (TUSIGN16)ILLEGAL_OBJ_IDX;
    }
    else
    {
		switch (objectIndex)
		{
            case MVM_IDX_gasConfFlag:        
                result |= Put_T_UNIT(MVMEASUREMENT_ME,(TUSIGN16)MVM_IDX_spline2dRequest,(TINT16)WHOLE_OBJECT,(void FAST*)&reqFlag);
	       		result |= (TUSIGN16)Put_T_UNIT(me,(TUSIGN16) MVM_IDX_protectedDataStatus, (TINT16)WHOLE_OBJECT, &protectedDataStatus);
                break;
				
            case MVM_IDX_mvmAlarmSimulation:        
                result |= Put_T_UNIT(MVMEASUREMENT_ME,(TUSIGN16)MVM_IDX_mvmAlarmSimulationDuplicated,(TINT16)attributeIndex,ptrValue);
                break;
				
            case MVM_IDX_gasRef:        
                result |= Put_T_UNIT(MVMEASUREMENT_ME, (TUSIGN16)MVM_IDX_updateCFSRequest,(TINT16)WHOLE_OBJECT,(void FAST*)&reqFlag);
	       		result |= (TUSIGN16)Put_T_UNIT(me,(TUSIGN16) MVM_IDX_protectedDataStatus, (TINT16)WHOLE_OBJECT, &protectedDataStatus);
                break;                                
				
            case MVM_IDX_steamType:
                steamType = *((TUSIGN8*)ptrValue);

                result = Put_T_UNIT(MVMEASUREMENT_ME, (TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT, &steamType);
                if (result == (TUSIGN16)OK)
                {
                    (void)coordinator_Get((TUSIGN16)CDT_IDX_operatingMode,(TINT16)WHOLE_OBJECT,&operationMode);
                    if((operationMode <= CDT_LIQUID_POWER) || ((operationMode >= CDT_STEAM_ACTUAL_VOLUME) && (steamType == MVM_STEAMTYPE_WATER))) 
                    {
                        KSet = SVM_KFACTOR_LIQUID;
                    }
                    else
                    {
                        KSet = SVM_KFACTOR_GAS;
                    }

                    //lint -e641 Enumer to in is acceptecable
                    (void)sVMeasurement_Get((TUSIGN16)SVM_IDX_KSET, (TINT16)WHOLE_OBJECT, &KSetOld);
                    if (KSetOld != KSet)
                    {
                        (void)sVMeasurement_Put((TUSIGN16)SVM_IDX_KSET, (TINT16)WHOLE_OBJECT, &KSet);

                        sVMeasurement.LoadQvMaxDNFrmTable();
                    }
                    else
                    {
                    }
                }
                else
                {
                }
                break;
                
			case MVM_IDX_QnSimEnable:
			case MVM_IDX_QmSimEnable:
			case MVM_IDX_QpSimEnable:
			case MVM_IDX_QnPartialSimEnable:
			case MVM_IDX_QvPartialSimEnable:
			case MVM_IDX_spline2dRequest:
			case MVM_IDX_updateCFSRequest:
			case MVM_IDX_updateCFPresetRequest:
			case MVM_IDX_updateMaxDNRequest:
			case MVM_IDX_protectedDataStatus:
	       		result = (TUSIGN16)Put_T_UNIT(me,(TUSIGN16) MVM_IDX_protectedDataStatus, (TINT16)WHOLE_OBJECT, &protectedDataStatus);
	            break;
	
			default: 				
	            break;
		}
		result |= Put_T_UNIT(MVMEASUREMENT_ME,(TUSIGN16)objectIndex, (TINT16)attributeIndex,ptrValue);
    }
	
	return result;
}