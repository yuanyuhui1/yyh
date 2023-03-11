//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2005.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem Modbus
Module
Description    Implementation of overloaded methods
for Subsystems based on T_UNIT.

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/Modbus_idx.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"

#include "modbus_overload.h"
#include "modbus_types.h"
#include "modbus_counters.h"
#include "modbus_tcp.h"
#include "Modbus_memory.h"
#include "Modbus_attributes.h"
#include "Modbus_scanregister.h"
#include "Coordinator/Interface/subsystem_idx.h"
//#include "..\..\elettronicServices\interface\elettronicServices.h"
//#include "..\..\elettronicServices\interface\elettronicServices_idx.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Ethernet/Interface/Ethernet.h"
#include "Ethernet/Interface/Ethernet_idx.h"



//-------------------------------------------------------------------------------------------------
/*!
\brief  initialize the subsystem; (hardware, tasks, data, calculation)
\author Heiko Kresse
\date   2004-04-18
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
\test-date: 2013-3-28
\n by: Jax Yang
\n environment:IAR Embedded Workbench Version 2.30.6
\n intention:
\n result module test:OK
\n result Lint Level 3:OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_MODBUS(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = OK;

    VIP_ASSERT(me == MODBUS_ME);

    //lint -e{838}
    result = Initialize_T_UNIT(me, typeOfStartUp);
    if (result == OK)
    {
        if ((typeOfStartUp & INIT_HARDWARE) != 0)
        {
        }
        if ((typeOfStartUp & INIT_TASKS) != 0)
        {
            InitModbusTaskState();

            TUSIGN8 enableState;
            result |= ethernet_Get(ETH_IDX_enableModbusTcp, WHOLE_OBJECT, &enableState);

            if (result == OK)
            {
                modbus.UpdateEnableStateEXE(enableState);
            }
            
            if (ModbusCounterInit() == eFALSE)
            {
                result |= (TUSIGN16)FATAL_ERROR;
            }

            if (ModbusMemoryInit() == eFALSE)
            {
                result |= (TUSIGN16)FATAL_ERROR;
            }
        }

        if ((typeOfStartUp & INIT_DATA) != 0)
        {
            (void)me->LoadRomDefaults(me, MODBUS_DC_IDX_DYNAMIC);
            (void)me->LoadRomDefaults(me, MODBUS_DC_IDX_CONSTANT);
            TranslateConfigScanReg2Intern((TUSIGN16*)modbusStaticFrequentConstantDefault.configScanRegister1, configScanRegisterIntern1);
            TranslateConfigScanReg2Intern((TUSIGN16*)modbusStaticFrequentConstantDefault.configScanRegister2, configScanRegisterIntern2);
        }

        if ((typeOfStartUp & INIT_CALCULATION) != 0)
        {
        }

    }
    return (result);
}





//--------------------------------------------------------------------------------------------------
/*!
\brief  put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
\author Heiko Kresse, Klaus Pose, Florian Reuss
\date   2004-04-18    2017-04-07, 2018-03-14
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
\test
\test-date: 2013-3-28,  2018-03-04
\n by: Jax Yang,        Florian Reuss
\n environment:IAR Embedded Workbench Version 2.30.6
\n intention:
\n result module test:OK
\n result Lint Level 3:OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_MODBUS(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    // Result of T_Unit Put operation
    TUSIGN16 result ;
    //TUSIGN8 index = 0;
    //TUSIGN8 unitOject[2];
    //TUSIGN32 localSensorSerialID;
    //TUSIGN8 localSlaveID = 0;
    TUSIGN16 numberOfAttributes;
	TINT16 aIndex;

    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);

    switch(objectIndex)
    {   
        case MODBUS_IDX_configScanRegister1:
        case MODBUS_IDX_configScanRegister2:
            numberOfAttributes = me->ptrObjectList[objectIndex].numberOfAttributes;
            if( (attributeIndex >= 0) && (attributeIndex < numberOfAttributes) )
            {
                aIndex = attributeIndex;
                numberOfAttributes = 1;
                result = OK;
            } else
            if( attributeIndex == WHOLE_OBJECT )
            {
                aIndex = 0;
                result = OK;
            } else
            {
                result = ILLEGAL_ATTRIB_IDX;
            }
            if (result < ERROR_CODES)
            {
                for( aIndex = 0; aIndex < numberOfAttributes; aIndex++ )
                {
                    result |= CheckModbusAddress( ((TUSIGN16 *)ptrValue)[aIndex] );
                }
            }
            if (result < ERROR_CODES)
            {
                result = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
                if (result == OK)
                {
                    if( objectIndex == MODBUS_IDX_configScanRegister1 )
                    {
                        TranslateConfigScanReg2Intern((TUSIGN16*)modbusStaticFrequentConstantDefault.configScanRegister1, configScanRegisterIntern1);
                    } else  // objectIndex == MODBUS_IDX_configScanRegister2
                    {
                        TranslateConfigScanReg2Intern((TUSIGN16*)modbusStaticFrequentConstantDefault.configScanRegister2, configScanRegisterIntern2);
                    }
                }
            }
            break;

        case MODBUS_IDX_ScanRegister1:
        case MODBUS_IDX_ScanRegister2:
            result = READ_ONLY_ERR;
            break;

        default:
            result = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break ;
    }

    return (result) ;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
 \author Jax-Yang
 \date   2016-04-28
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
 \test-date: 2016-08-24
 \n by:Zuochen Wang
 \n intention:
 \n result module test:
 \n result Lint Level 3:  // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_MODBUS(const T_UNIT SLOW*me,TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    const T_DATA_OBJ SLOW* ptrObj = 0;

    TUSIGN16 result = OK;
    // Gets set when the Framework Get is not required
    TBOOL getPerformed = eFALSE;
    //TUSIGN8 unitOject[2];

    T_DIAGNOSIS_ACTIVE_CONDITION mActiveConditionList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
    TUSIGN16 alarmId;
    TUSIGN8 selectedCondition = 0;
    TUSIGN16 localDiagCondenseStatus[3] = {0,0,0};

    // check for zeropointer
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);

    // exception if not initialized
    VIP_ASSERT(*(me->ptrState)>=INITIALIZED);

    // Unknown object ?
    if(objectIndex > me->maxIdx)
    {
        result = ILLEGAL_OBJ_IDX;
    }
    else
    {
        switch(objectIndex)
        {
            case MODBUS_IDX_mDiagCondenseStatus:
                // Also reset the diagnostic structure before populating it.
                (void)memset(mActiveConditionList, 0,sizeof(mActiveConditionList));
                (void)diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, mActiveConditionList) ;

                for(alarmId =0; (alarmId < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER) &&
                (selectedCondition !=  DIAGNOSIS_CONDITIONS_UNSET);
                alarmId++)
                {
                  selectedCondition = mActiveConditionList[alarmId].activeConditionId;

                  if(selectedCondition != DIAGNOSIS_CONDITIONS_UNSET)
                    {
                      TUSIGN8 dataRegister;
                      TUSIGN16 dataBit= 0x0000;

                      dataRegister = selectedCondition >> 4; //equal to / 16
                      //lint -e{838}
                      dataBit = selectedCondition & 0x0F; //equal to % 16
                      //lint -e{701,734}
                      localDiagCondenseStatus[dataRegister] |= (1 << dataBit);
                    }
                }
                (void)memcpy(modbusDynamic.mDiagCondenseStatus,localDiagCondenseStatus,sizeof(localDiagCondenseStatus));
                getPerformed = eFALSE;
                break;

            default:
                break;
        }


        if (getPerformed == eFALSE)
        {
            if (result == OK)
            {
                // handmade code optimizer
                ptrObj = &(me->ptrObjectList[objectIndex]);
                //lint -e{713} Heiko Kresse 2004-10-12
                result = ptrObj->pClass->Get(ptrObj, ptrValue, attributeIndex, me->ptrDataSemaphore);
            }
        }
    }
    return result;
}
