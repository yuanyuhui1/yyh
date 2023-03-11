//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include <math.h>
#include <string.h>
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Totalizer/Interface/Totalizer.h"
#include "Totalizer/Interface/Totalizer_idx.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Source/MVMeasurement_types.h"
#include "MVMeasurement/Source/MVMeasurement_attributes.h"

#include "Coordinator/Interface/Coordinator.h"
#include "coordinator_types.h"
#include "coordinator_attributes.h"
#include "coordinator_overload.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/TimeElapse.h"

#include "Hmi/Source/compileTimeDefinitions.h"
#include "Hmi/Source/validityTest.h"
#include "Hmi/Interface/ValidTest_extern.h"

//lint -e641 Lawrence 2013-04-18

//lint -e760 lawrence-shi 2013-04-18
//#define LENGTH_OF_FUNCTION_NAME    10
//#define MAX_LENGTH_OF_INPUT_STR        (LENGTH_OF_ORDER_NUMBER + LENGTH_OF_FUNCTION_NAME + 1)
const TCHAR strIntRTDName[LENGTH_OF_FUNCTION_NAME] = {"intRTD"};
const TCHAR strAIName[LENGTH_OF_FUNCTION_NAME] = {"AI"};
const TCHAR strEnergyName[LENGTH_OF_FUNCTION_NAME] = {"energy"};
//lint +e760 lawrence-shi 2013-04-18
TCHAR strIntPassword[LENGTH_OF_FUNCTION_PASSWORD+1]={"******"};
TCHAR strAiPassword[LENGTH_OF_FUNCTION_PASSWORD+1]={"******"};
TCHAR strEnergyPassword[LENGTH_OF_FUNCTION_PASSWORD+1]={"******"};

RTOS_TASK_HANDLE MainTskHandle;
#define MAIN_TASK_STACK_SIZE                     (2 * RTOS_TASK_STACK_SZMIN)
#define MAIN_TASK_PRIORITY                       (RTOS_TASK_PRIO_IDLE + 6)


static TUSIGN8  simulationMode_Enum[14]={0};
static TUSIGN8  densitySel_Enum[3]={0,1,2};

extern void ClearDiagnosis_MVM(TUSIGN16 attributeIdx);
//-------------------------------------------------------------------------------------------------
/*!
 \brief GetFunctionPasswd generate the password for special function, for example, internal RTD.
 \author Merrick Huang
 \date   2012-11-14
 \param[in] strOrderNum is a pointer to null terminated string (containing order number of device)
 \param[in] strFunctionName is a pointer to null terminated string (containing function name)
 \param[out] strPassword is a pointer to array of password
 \return : void
 \warning: none
 \test
 \n by: Merrick Huang
 \n result module test: 2012-11-15 OK
 \n result Lint Level 3: free
 \bug
*/
//-------------------------------------------------------------------------------------------------
void GetFunctionPasswd(TCHAR const *strOrderNum, TCHAR const *strFunctionName, TCHAR *strPassword) 
{
    TUSIGN32 Num1 = 0;
    TUSIGN32 Num2 = 0;
    TUSIGN32 Num3 = 0;
    TCHAR strInput[MAX_LENGTH_OF_INPUT_STR];
    TUSIGN16 i,len;
    /* get the password length */
    TUSIGN8 passwdLen = (TUSIGN8)strlen((const char *)strPassword);
    TUSIGN8 stepLen = passwdLen/3;
    (void)memset((void*)strInput, '\0', MAX_LENGTH_OF_INPUT_STR);
    /* construct Input string by concatenating all inputs */
    (void)strcpy((char *)strInput, (const char *)strOrderNum);
    (void)strcat((char *)strInput, (const char *)strFunctionName);
    /* get the order number and function name length */
    len = (TUSIGN8)strlen((const char *)strInput);
    
    if(len > 0)
    {
        for(i=1; i<=len; i++)
        {
            /* get first generator factor */
            //lint -e{961,747,790} lawrence-shi 2013-04-18
            Num1 = ((TUSIGN32)(Num1 + ((TUSIGN32)(strInput[i-1])*i*i) * (i*sqrt(strInput[i-1])+1))) % 100000;
            /* get second generator factor */
            //lint -e{961,747,790} lawrence-shi 2013-04-18
            Num2 = (Num2*i + ((TUSIGN32)(pow(strInput[i-1],2)*i))) % 100000;
            /* get third generator factor */
            //lint -e{961,747,790} lawrence-shi 2013-04-18
            Num3 = (Num2 + (TUSIGN32)sqrt(Num1)) % 100000;
        }
        for(i=0; i<stepLen; i++)
        {
            //lint -e{961,647} lawrence-shi 2013-04-18
            strPassword[i] = (TUSIGN8)((Num1+31+i*i*i) % 128);
        }
        
        for(i=stepLen; i<(stepLen*2); i++)
        {
            //lint -e{961,647} lawrence-shi 2013-04-18
            strPassword[i] = (TUSIGN8)((Num2+31+i*i*i) % 128);
        }
        
        for(i=stepLen*2; i<passwdLen; i++)
        {
            //lint -e{961,647} lawrence-shi 2013-04-18
            strPassword[i] = (TUSIGN8)((Num3+31+i*i*i) % 128);
        }
        /* Convert all character to "0---9��A---Z��a----z" */
        for(i=0; i<passwdLen; i++)
        {
            while(((strPassword[i]<'0') || (strPassword[i]>'9')) && \
                ((strPassword[i]<'A') || (strPassword[i]>'Z')) && \
                    ((strPassword[i]<'a') || (strPassword[i]>'z')) )
            {
                //lint -e{961,747,790} lawrence-shi 2013-04-18
                strPassword[i] = (strPassword[i]+31+7*i) % 128;
            }
        }
    }
}


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
 \n by: Lawrence shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_COORDINATOR(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = FATAL_ERROR;
    TUSIGN8 intTempType = CDT_TEMP_NONE;
    TUSIGN8 passwordEnable = 0;
    TUSIGN32 alarmStatus = 0;
    TCHAR strOrderNum[LENGTH_OF_ORDER_NUMBER+1] = {"0"};
    TCHAR nvPassword[LENGTH_OF_FUNCTION_PASSWORD+1] = {"******"};
    TCHAR tmpPassword[LENGTH_OF_FUNCTION_PASSWORD+1] = {"******"};
    //lint -e{746} 
    VIP_ASSERT(me==COORDINATOR_ME);
    
    result = Initialize_T_UNIT(me,typeOfStartUp);
    
    if( result==OK )
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
            TimeElapse_TIMER_Init();
        }

        if( (typeOfStartUp & INIT_TASKS) != 0)
        {                       
            if(Create_RTOS_TASK((TaskFunction_t)(coordinator.MainTaskEXE), "MainTask", MAIN_TASK_STACK_SIZE,
                                NULL, MAIN_TASK_PRIORITY, &MainTskHandle) != RTOS_PASS)
            {
            
            }
        }
        if( (typeOfStartUp & INIT_DATA) != 0)
        {
            result |= coordinator.unit.LoadRomDefaults(me, (TINT16)COORDINATOR_DC_IDX_DynamicDuplicated);
            result |= coordinator.unit.LoadRomDefaults(me, (TINT16)COORDINATOR_DC_IDX_Protected);
            result |= coordinator.unit.LoadRomDefaults(me, (TINT16)COORDINATOR_DC_IDX_Unprotected);
            result |= coordinator.unit.LoadRomDefaults(me, (TINT16)COORDINATOR_DC_IDX_FE_OUTPUT_FILTER_PARA);

        }        
        if( (typeOfStartUp & INIT_CALCULATION) != 0)
        {
            (void)memset((void*)strOrderNum, '\0', (LENGTH_OF_ORDER_NUMBER+1));
            result = elettronicServices_Get(ELETTRONICSERVICES_IDX_tranSapErpNo, WHOLE_OBJECT, strOrderNum);
            // check intRTD password
            GetFunctionPasswd(strOrderNum, strIntRTDName, strIntPassword);
            
            result |= coordinator_Get(CDT_IDX_internalRTDPassword, WHOLE_OBJECT, &nvPassword[0]);
            if(memcmp(strIntPassword, nvPassword, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                passwordEnable |= CDT_INTERNALRTD_ENABLE;
            }
            else
            {
                result = Put_T_UNIT(me,CDT_IDX_internalRTDPassword,WHOLE_OBJECT,&tmpPassword[0]);
            }
            // check Ai password
            GetFunctionPasswd(strOrderNum, strAIName, strAiPassword);
            
            result |= coordinator_Get(CDT_IDX_analogInputPassword, WHOLE_OBJECT, &nvPassword[0]);
            
            if(memcmp(strAiPassword, nvPassword, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                passwordEnable |= CDT_ANALOGINPUT_ENABLE;
            }
            else
            {
                result = Put_T_UNIT(me,CDT_IDX_analogInputPassword,WHOLE_OBJECT,&tmpPassword[0]);
            }
            // check energy password
            GetFunctionPasswd(strOrderNum, strEnergyName, strEnergyPassword);
            
            result |= coordinator_Get(CDT_IDX_energyFlowPassword, WHOLE_OBJECT, &nvPassword[0]);
            
            if(memcmp(strEnergyPassword, nvPassword, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                passwordEnable |= CDT_ENERGYFLOW_ENABLE;
            }
            else
            {
                result = Put_T_UNIT(me,CDT_IDX_energyFlowPassword,WHOLE_OBJECT,&tmpPassword[0]);
            }
            
            coordinatorProtected.passwordEnable = passwordEnable;          
           /*set temperature_Selection as preset and intRTDType as without when passwrod dismatch  */
  
            if((passwordEnable & CDT_INTERNALRTD_ENABLE) == 0)
            {
                result |= Put_T_UNIT(me,CDT_IDX_intRTDType,0,&intTempType);

            }
            
            if(coordinatorReplaceStaticRare_Calibration.calibrationStatus == CDT_SENSOR_IS_CALIBRATED)
            {
                alarmStatus &= ~((TUSIGN32)1<<CDT_ALARM_CB_UNCALIBRATED);
            }
            else
            {
                alarmStatus |= (TUSIGN32)1<<CDT_ALARM_CB_UNCALIBRATED;
            }

            (void)memcpy(&coordinatorDynamicDuplicated.cdtDiagnosis[0], (const void *)&alarmStatus, sizeof(alarmStatus));
            (void)memcpy(&coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated[0], (const void *)&alarmStatus, sizeof(alarmStatus));
            
            /*get the autoself check enable status*/
            (void)frontEndInterface.PutCommand(CDT_READ_AUTOSELFDIAG_EN,CDT_AUTOSELFDIAG_EN_LENGTH,comRead_cdt[CDT_READ_AUTOSELFCHECK_INDEX],NULL);
            /*get the spanUpperRange value*/
            (void)frontEndInterface.PutCommand(CDT_READ_SPAN_UPPRERRANGE,CDT_SPAN_UPPRERRANGE_LENGTH,comRead_cdt[CDT_READ_SPAN_UPPRERRANGE_INDEX],NULL);
            /*get FE parameters first time*/
            (void)frontEndInterface.PutCommand(CDT_READ_DSP_PARAM_CMD,CDT_DSP_PARAM_CMD_LENGTH,comRead_cdt[CDT_READ_DSP_PARAM_CMD_INDEX],NULL);
                            
             coordinatorProtected.protectedDataStatus = CDT_CALCULATE;  
        }
    }
    
    return result;
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief  put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
 \author Merrick 
 \date   2011-12-07
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
  \n by:  
  \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
  \n intention: 1. complete instruction-coverage
  \n result module test: 2011-12-14 OK
  \n result Lint Level 3: free // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_CDT(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{ 
    TUSIGN16 result=OK;
    TUSIGN16  opModeBit=0;
    TUSIGN8 tmpValue = 0;
    TUSIGN8 protectedDataStatus = CDT_CALCULATE;
    TUSIGN32 alarmStatus = 0;
    TUSIGN8  t_unit;
    TFLOAT   t_offset = 0.0f;
    TFLOAT MaxDN = 0.0f,MinDN = 0.0f,Span_DN = 0.0f;
    TUSIGN8 KSet = SVM_KFACTOR_LIQUID;
    TUSIGN8 passwordEnable = 0;
    TUSIGN8 notuse = 0;
    TUSIGN8 updateCFPresetRequest = MVM_GONFIGING;
    TCHAR tmpPassword[LENGTH_OF_FUNCTION_PASSWORD+1] = {"******"};
    TUSIGN32 lMaintenanceCycle = 0;
    TUSIGN8 mvmDiag = 0;
    
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);
    
    VIP_ASSERT(*(me->ptrState)>=INITIALIZED);
    
    if(objectIndex>me->maxIdx )
    {       
        result = ILLEGAL_OBJ_IDX;
    }
    else
    {       
        if(objectIndex == CDT_IDX_cdtDiagnosis)
        {
           result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(result == (TINT16)OK)
            {
                result = Put_T_UNIT(me,CDT_IDX_cdtDiagnosis_Duplicated,attributeIndex,ptrValue);
            }
        }
        else if(objectIndex == CDT_IDX_frontendMaintenance)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(result == (TINT16)OK)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_FE_MAINTAIN_CYCLE,CDT_FE_MAINTAIN_CYCLE_LENGTH,comWrite_cdt[CDT_WRITE_FE_MAINTAIN_CYCLE_INDEX],NULL);
            }
        }
        else if(objectIndex == CDT_IDX_frontendOperationHour)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(result == (TINT16)OK)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_RESET_OP_HOUR,CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_RESET_OP_HOUR_INDEX],NULL);
            }
        }
        else if(objectIndex == CDT_IDX_operatingMode)
        {     

            if((*(TUSIGN8*)(ptrValue)==CDT_LIQUID_POWER)||(*(TUSIGN8*)(ptrValue)==CDT_GAS_POWER)||(*(TUSIGN8*)(ptrValue)==CDT_STEAM_POWER))
            {
                result |= Get_T_UNIT(me,CDT_IDX_passwordEnable,WHOLE_OBJECT,&passwordEnable);
                if(passwordEnable & CDT_ENERGYFLOW_ENABLE)
                {
                    result |= totalizer_Put((TUSIGN16)TOTALIZER_IDX_StopAllTotalizer,(TINT16)WHOLE_OBJECT,&notuse);
                    result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                    result |= totalizer_Put((TUSIGN16)TOTALIZER_IDX_StartAllTotalizr,(TINT16)WHOLE_OBJECT,&notuse);
                    
                    opModeBit = 1<<(*(TUSIGN8*)(ptrValue));
                    result |= mapper.SetSelection(opModeBit);
                    sVMeasurement.LoadQvMaxDNFrmTable();
                    for(tmpValue=0;tmpValue<MVM_DIAG_SIZE;tmpValue++)
                    {
                        ClearDiagnosis_MVM(tmpValue);
                    }
                }
                else
                {
                    result = WRONG_STATE_ERR;
                }
            }
            else
            {
                result |= totalizer_Put((TUSIGN16)TOTALIZER_IDX_StopAllTotalizer,(TINT16)WHOLE_OBJECT,&notuse);
                result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                result |= totalizer_Put((TUSIGN16)TOTALIZER_IDX_StartAllTotalizr,(TINT16)WHOLE_OBJECT,&notuse);
                opModeBit = 1<<(*(TUSIGN8*)(ptrValue));
                result = mapper.SetSelection(opModeBit);
                sVMeasurement.LoadQvMaxDNFrmTable();
                for(tmpValue=0;tmpValue<MVM_DIAG_SIZE;tmpValue++)
                {
                    ClearDiagnosis_MVM(tmpValue);
                }  
            }
            (void)Get_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            
            if(*(TUSIGN8*)(ptrValue)>CDT_LIQUID_POWER)
            {
                /* set the kset = liquid if hot water type on steam mode  */
                if((*(TUSIGN8*)(ptrValue)==CDT_STEAM_MASS)||(*(TUSIGN8*)(ptrValue)==CDT_STEAM_POWER)||(*(TUSIGN8*)(ptrValue)==CDT_STEAM_ACTUAL_VOLUME))
                {
                    TUSIGN8 steamType =  0xFF;
                    (void)mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType);
                    if(steamType == MVM_STEAMTYPE_WATER) 
                    {
                        KSet = SVM_KFACTOR_LIQUID;
                    }
                    else
                    {
                        KSet = SVM_KFACTOR_GAS;
                    }
                }
                else
                {
                    KSet = SVM_KFACTOR_GAS;
                }
                (void)sVMeasurement_Put(SVM_IDX_KSET,WHOLE_OBJECT,&KSet);
            }
            else
            {
                KSet = SVM_KFACTOR_LIQUID;
                (void)sVMeasurement_Put(SVM_IDX_KSET,WHOLE_OBJECT,&KSet);             
            }
            if((*(TUSIGN8*)(ptrValue)==CDT_LIQUID_ACTUAL_VOLUME)||(*(TUSIGN8*)(ptrValue)==CDT_GAS_ACTUAL_VOLUME)||(*(TUSIGN8*)(ptrValue)==CDT_STEAM_ACTUAL_VOLUME))
            {
                TUSIGN8 selection = AI_WITHOUT;
                (void)analogInput_Get(AI_IDX_aiSelection, 0,&selection);
                if((selection!=AI_WITHOUT)&&(selection!=AI_EXTERNAL_OUTPUT_CUTOFF))
                {
                    selection = AI_WITHOUT;
                    (void)analogInput_Put(AI_IDX_aiSelection, 0,&selection);
                }

                selection = HART_SEC_WITHOUT;
                (void)hart_Put((TUSIGN16)HART_IDX_burstUpTxSelection,WHOLE_OBJECT, &selection);
            }

        }
        else if(objectIndex == CDT_IDX_internalRTDPassword)
        {
            
            if(memcmp(strIntPassword, ptrValue, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                /* Enable special funtion */
                result |= Get_T_UNIT(me,CDT_IDX_passwordEnable,WHOLE_OBJECT,&passwordEnable);
                passwordEnable |= CDT_INTERNALRTD_ENABLE;
                SuspendAll_RTOS_TASK();
                coordinatorProtected.passwordEnable = passwordEnable;
                coordinatorProtected.protectedDataStatus = CDT_CALCULATE;
                ResumeAll_RTOS_TASK();
                result = coordinator_Put(CDT_IDX_protectedDataStatus, WHOLE_OBJECT, &protectedDataStatus);
        
            }
            else
            {
                result = Put_T_UNIT(me,objectIndex,attributeIndex,&tmpPassword[0]);
                result = WRONG_STATE_ERR;
            }
        }
        else if(objectIndex == CDT_IDX_analogInputPassword)
        {
            
            if(memcmp(strAiPassword, ptrValue, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                /* Enable special funtion */
                result |= Get_T_UNIT(me,CDT_IDX_passwordEnable,WHOLE_OBJECT,&passwordEnable);
                passwordEnable |= CDT_ANALOGINPUT_ENABLE;
                SuspendAll_RTOS_TASK();
                coordinatorProtected.passwordEnable = passwordEnable;
                coordinatorProtected.protectedDataStatus = CDT_CALCULATE;
                ResumeAll_RTOS_TASK();
            }
            else
            {
                result = Put_T_UNIT(me,objectIndex,attributeIndex,&tmpPassword[0]);
                result = WRONG_STATE_ERR;
            }
        }
        else if(objectIndex == CDT_IDX_energyFlowPassword)
        {
            
            if(memcmp(strEnergyPassword, ptrValue, LENGTH_OF_FUNCTION_PASSWORD) == 0)
            {
                result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                /* Enable special funtion */
                result |= Get_T_UNIT(me,CDT_IDX_passwordEnable,WHOLE_OBJECT,&passwordEnable);
                passwordEnable |= CDT_ENERGYFLOW_ENABLE;
                SuspendAll_RTOS_TASK();
                coordinatorProtected.passwordEnable = passwordEnable;
                coordinatorProtected.protectedDataStatus = CDT_CALCULATE;
                ResumeAll_RTOS_TASK();
            }
            else
            {
                result = Put_T_UNIT(me,objectIndex,attributeIndex,&tmpPassword[0]);
                result = WRONG_STATE_ERR;
            }
        }
        else if((objectIndex == CDT_IDX_frequency_MaxDN)||(objectIndex == CDT_IDX_frequency_MinDN))
        {
            result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            result |= Get_T_UNIT(me,CDT_IDX_frequency_MinDN,WHOLE_OBJECT,&MinDN);
            result |= Get_T_UNIT(me,CDT_IDX_frequency_MaxDN,WHOLE_OBJECT,&MaxDN);
            if(result == OK)
            {
                Span_DN = MaxDN - MinDN;
                result = Put_T_UNIT(me,CDT_IDX_frequency_Span,WHOLE_OBJECT,&Span_DN);
            }  
        }
        else if((objectIndex == CDT_IDX_temperature_MinDN)||(objectIndex == CDT_IDX_temperature_MaxDN))
        {
            result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            result |= Get_T_UNIT(me,CDT_IDX_temperature_MinDN,WHOLE_OBJECT,&MinDN);
            result |= Get_T_UNIT(me,CDT_IDX_temperature_MaxDN,WHOLE_OBJECT,&MaxDN);
            if(result == OK)
            {
                Span_DN = MaxDN - MinDN;
                result = Put_T_UNIT(me,CDT_IDX_temperature_Span,WHOLE_OBJECT,&Span_DN);
            }
        }
        else if(objectIndex == CDT_IDX_intRTDType)
        {
            result |= Get_T_UNIT(me,CDT_IDX_passwordEnable,WHOLE_OBJECT,&passwordEnable);
            if(((passwordEnable & CDT_INTERNALRTD_ENABLE) > 0)||((*(TUSIGN8*)ptrValue) == CDT_TEMP_NONE))
            {
                result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            }
            else
            {
                result = WRONG_STATE_ERR;
            }
        }
        else if(objectIndex == CDT_IDX_temperaturePreset)
        {
            result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(result == OK)
            {
                tmpValue = 1;
                result |= Put_T_UNIT(me,CDT_IDX_tPresetChangeFlag,WHOLE_OBJECT,&tmpValue);
                result |= mVMeasurement_Put(MVM_IDX_updateCFPresetRequest,WHOLE_OBJECT,&updateCFPresetRequest); 
            }
        }
        else if(objectIndex == CDT_IDX_calibrationStatus)
        {
            result |= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            result |= Get_T_UNIT(me,CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&alarmStatus);
            if(CDT_SENSOR_IS_CALIBRATED ==  *(TUSIGN8*)ptrValue)
            {
                alarmStatus &= ~((TUSIGN32)1<<CDT_ALARM_CB_UNCALIBRATED);
            }
            else
            {
                alarmStatus |= (TUSIGN32)1<<CDT_ALARM_CB_UNCALIBRATED;
            }
            result |= Put_T_UNIT(me,CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&alarmStatus);
        }

        else if(objectIndex == CDT_IDX_passwordEnable)
        {
            // Password enable can't be wroten.
            result = WRONG_STATE_ERR;
        }
        else if((objectIndex ==CDT_IDX_converterType)||(objectIndex ==CDT_IDX_swirlmeterSize)||\
            (objectIndex ==CDT_IDX_vortexmeterSize))
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            sVMeasurement.LoadQvMaxDNFrmTable();
        }
        else if(objectIndex ==CDT_IDX_autoselfcheck_Enable)
        {
          result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
          result |= Put_T_UNIT(me,CDT_IDX_autoselfcheck_Enable_Duplicated,attributeIndex,ptrValue);
          if(result == (TINT16)OK)
          {
              //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
            result = frontEndInterface.PutCommand(CDT_WRITE_AUTOSELFDIAG_EN,CDT_AUTOSELFDIAG_EN_LENGTH,comWrite_cdt[CDT_WRITE_AUTOSELFCHECK_INDEX],NULL);
          }
        }
        else if(objectIndex ==CDT_IDX_spanUpperRange)
        {
          result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
          if(result == (TINT16)OK)
          {
              //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
            (void)frontEndInterface.PutCommand(CDT_WRITE_SPAN_UPPRERRANGE,CDT_SPAN_UPPRERRANGE_LENGTH,comWrite_cdt[CDT_WRITE_SPAN_UPPRERRANGE_INDEX],NULL);
          }
        }
        else if(objectIndex == CDT_IDX_simulationMode)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            result = Put_T_UNIT(me,CDT_IDX_protectedDataStatus,WHOLE_OBJECT,&protectedDataStatus);
        }
        else if(objectIndex == CDT_IDX_lowFlowThresh)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(result == (TINT16)OK)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_LOWFLOW_THRESHOLD,CDT_LOWFLOW_THRESHOLD_LENGTH,comWrite_cdt[CDT_WRITE_LOWFLOW_THRESHOLD_INDEX],NULL);
            }
        }
        else if(objectIndex == CDT_IDX_temperatureOffsetCorrectDisp)
        {
            result |= Get_T_UNIT(me,CDT_IDX_temperature_UnitCode,WHOLE_OBJECT,&t_unit);
            t_offset = *(TFLOAT*)ptrValue;
            if(t_unit == UM_FAHRENHEIT)
            {          
                t_offset = t_offset/1.8;
            } 
            result |= Put_T_UNIT(me,(TUSIGN16)CDT_IDX_temperatureOffsetCorrect, WHOLE_OBJECT, &t_offset); 
        }
        else if(objectIndex == CDT_IDX_temperatureOffsetCorrectUserDisp)
        {
            result |= Get_T_UNIT(me,CDT_IDX_temperature_UnitCode,WHOLE_OBJECT,&t_unit);
            t_offset = *(TFLOAT*)ptrValue;
            if(t_unit == UM_FAHRENHEIT)
            {
                t_offset = t_offset/1.8;
            } 
            result |= Put_T_UNIT(me,(TUSIGN16)CDT_IDX_temperatureOffsetCorrectUser, WHOLE_OBJECT, &t_offset);
        }
        else if(CDT_IDX_BothMaintenanceCycle == objectIndex)
        {
            lMaintenanceCycle = *((TUSIGN32*)ptrValue);

            // this object will put both FE and CB maintenance cycle

            result = Put_T_UNIT(COORDINATOR_ME, (TUSIGN16)CDT_IDX_frontendMaintenance, (TINT16)WHOLE_OBJECT, &lMaintenanceCycle);
            if(result == (TUSIGN16)OK)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_FE_MAINTAIN_CYCLE,CDT_FE_MAINTAIN_CYCLE_LENGTH,comWrite_cdt[CDT_WRITE_FE_MAINTAIN_CYCLE_INDEX],NULL);

                if(result == (TUSIGN16)OK)
                {
                    result  = Put_T_UNIT(COORDINATOR_ME, (TUSIGN16)CDT_IDX_tranMaintenanceCyc, (TINT16)WHOLE_OBJECT, &lMaintenanceCycle);
                    result |= Put_T_UNIT(COORDINATOR_ME, (TUSIGN16)CDT_IDX_BothMaintenanceCycle, (TINT16)WHOLE_OBJECT, &lMaintenanceCycle);
                }
            }
        }
        else if(objectIndex == CDT_IDX_actualDensitySelection)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if(*(TUSIGN8*)ptrValue != CDT_CALCULATION_FROM_TP)
            {
                (void)mVMeasurement_Get((TUSIGN16)MVM_IDX_mvmDiagnosis, (TINT16)WHOLE_OBJECT, &mvmDiag);
                if(mvmDiag & (1U << MVM_ALM_STEAM_STATUS_MISMATCH))
                {
                    MVM_CLRDIAG_(MVM_ALM_STEAM_STATUS_MISMATCH,mVMeasurementDynamicDuplicated.mvmDiagnosis);
                }
            }
        }
        else if(objectIndex == CDT_IDX_ext_algSelection)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if((TUSIGN16)OK == result)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_OUTPUT_FILTER_ALG, CDT_OUTPUT_FILTER_ALG_LENGTH,comWrite_cdt[CDT_WRITE_OUTPUT_FILTER_ALG_INDEX],NULL);
            }
        }
        else if(CDT_IDX_vbrCorrectionOn == objectIndex)
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if((TUSIGN16)OK == result)
            {
                //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
                result = frontEndInterface.PutCommand(CDT_WRITE_DSP_PARAM_CMD, CDT_DSP_PARAM_CMD_LENGTH,comWrite_cdt[CDT_WRITE_DSP_PARAM_CMD_INDEX],NULL);
            }
        }
        else
        {
            result = Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            if((TUSIGN16)OK ==result)
            {
                if((objectIndex == CDT_IDX_coodinatorAlarmSimulation)||(objectIndex == CDT_IDX_forceReplace) \
                    ||(objectIndex == CDT_IDX_frequencySim_Enable)||(objectIndex == CDT_IDX_temperatureSim_Enable))
                {
                     result = Put_T_UNIT(me,CDT_IDX_protectedDataStatus,WHOLE_OBJECT,&protectedDataStatus);
                }           
            }
        }
    }
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief   Overload of LoadRomDefaults to update Calibration bit
 \author  Paul
 \date    2012-9-6
 \param   me = pointer to current out subsystem
 \param   dataClassIndex = index of the dataclass to be defaulted
 \return  error code
          OK                      operation was successful
          LESSTHAN_RANGEMIN       at least one value is less than its minimum value
          GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
          SPAN_TO_SMALL           then span between two values is to small
          NOT_ON_GRID             at least one value is not a multiple of its increment
          RULE_VIOLATION          this shows a violation of one or more businessrules
          DECLINE                 DEBUG, operation isn't permitted in the aktual unit-state
          ILLEGAL_OBJ_IDX         DEBUG, unknown Object
 \test
 test-date: 2012-09-10
 \n by: Klaus Pose
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 LoadRomDefaults_COORDINATOR(const T_UNIT SLOW *me, TINT16 dataClassIndex)
{
    TUSIGN16    result = (TUSIGN16)OK;
    VIP_ASSERT(me==COORDINATOR_ME);
    VIP_ASSERT(me->ptrDataSemaphore);   // if no semaphore available => exit
    if(COORDINATOR_DC_IDX_ReplaceStaticRare_Calibration == dataClassIndex)
    {
        result = (TUSIGN16)OK;
    }
    else if(ALL_DATACLASSES == dataClassIndex)
    {
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_ReplaceStaticRare);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_DynamicDuplicated);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_Protected);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_Unprotected);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_STATIC_RARE);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_STATIC_CYCLIC);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT);
        result |= LoadRomDefaults_T_UNIT(me, COORDINATOR_DC_IDX_ReplaceStaticFreqStaticDefault);
    }
    else
    {
        result = LoadRomDefaults_T_UNIT(me, dataClassIndex);
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
    /*!
     \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
     \author Merrick
     \date   2012-10-18
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
TUSIGN16 Get_CDT(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{       
    //lint -e{641} Merrick 2012-10-25 Converting enum to int accepted
    TUSIGN16 result=OK;
    TUSIGN8  t_unit;
    TFLOAT   t_offset = 0.0f;
    TUSIGN32 lMaintenanceCycleCB = 0, lMaintenanceCycleFE = 0;
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);
      
    VIP_ASSERT(*(me->ptrState) >= INITIALIZED);
    if(objectIndex > me->maxIdx)
    {       
        result = ILLEGAL_OBJ_IDX;
    }
    else if(objectIndex == CDT_IDX_temperatureOffsetCorrectDisp)
    {
        result |= Get_T_UNIT(me,CDT_IDX_temperature_UnitCode,WHOLE_OBJECT,&t_unit);  
        result |= Get_T_UNIT(me,(TUSIGN16)CDT_IDX_temperatureOffsetCorrect, WHOLE_OBJECT, &t_offset); 
        
        if(t_unit == UM_FAHRENHEIT)
        {          
            t_offset = t_offset*1.8f;                 
        } 
        *(TFLOAT*)ptrValue = t_offset; 
    }
    else if(objectIndex == CDT_IDX_temperatureOffsetCorrectUserDisp)
    {
        result |= Get_T_UNIT(me,CDT_IDX_temperature_UnitCode,WHOLE_OBJECT,&t_unit);  
        result |= Get_T_UNIT(me,(TUSIGN16)CDT_IDX_temperatureOffsetCorrectUser, WHOLE_OBJECT, &t_offset); 
        
        if(t_unit == UM_FAHRENHEIT)
        {          
            t_offset = t_offset*1.8f;                 
        } 
        *(TFLOAT*)ptrValue = t_offset;        
    }
    else if (CDT_IDX_BothMaintenanceCycle == objectIndex)
    {
        // this object will get the minimum value of FE and CB maintenance cycle
        result  = Get_T_UNIT(COORDINATOR_ME, (TUSIGN16)CDT_IDX_frontendMaintenance, (TINT16)WHOLE_OBJECT, &lMaintenanceCycleFE);
        result |= Get_T_UNIT(COORDINATOR_ME, (TUSIGN16)CDT_IDX_tranMaintenanceCyc, (TINT16)WHOLE_OBJECT, &lMaintenanceCycleCB);
        if (result == (TUSIGN16)OK)
        {
            *((TUSIGN32*)ptrValue) = (lMaintenanceCycleFE < lMaintenanceCycleCB) ? lMaintenanceCycleFE : lMaintenanceCycleCB;
        }
    }
    else
    {
        result = Get_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
    }
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Overload of GetAttributeDescription to hide some HMI enum menu
 \author  Lawrence
 \date    2013-11-6
     \param  objectIndex    object index
     \param  attributeIndex attribute index; attributeIndex<0 --> get whole object
     \param  ptrDescriptor  pointer for range descriptor
 \return  error code
          OK                      operation was successful
          LESSTHAN_RANGEMIN       at least one value is less than its minimum value
          GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
          SPAN_TO_SMALL           then span between two values is to small
          NOT_ON_GRID             at least one value is not a multiple of its increment
          RULE_VIOLATION          this shows a violation of one or more businessrules
          DECLINE                 DEBUG, operation isn't permitted in the aktual unit-state
          ILLEGAL_OBJ_IDX         DEBUG, unknown Object
 \test
 test-date: 2013-12-10
 \n by: Klaus Pose
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetAttributeDescription_CDT(const struct _T_UNIT SLOW*me,
                                        TUSIGN16 objectIndex,
                                        TUSIGN16 attributeIndex,
                                        T_DO_ATTRIB_RANGE FAST* ptrDescriptor)
{
    TUSIGN16 returnVal = GetAttributeDescription_T_UNIT(me, objectIndex, attributeIndex, ptrDescriptor);
    TUSIGN8 newNumberOfCodes = 0;
    TUSIGN8 i,enumIndex;
    TUSIGN16 testIdx;
     
     if(objectIndex == CDT_IDX_simulationMode)
     {
        for (i = 0; i < ptrDescriptor->range.e8.numberOfCodes; i ++)
        {
            enumIndex = ptrDescriptor->range.e8.ptrCodeTab[i];
            testIdx = simulationMode_validtest_table[enumIndex];
            if(ValidityTestCheck(
            #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
                (FRAME_FORMAT_TYPE const*)NULL,
            #endif
                testIdx
            #ifdef VALIDITY_TEST_CALLER
                , SIGNALS_TEST_NOT_APPLICABLE
            #endif
            ))
            {
                simulationMode_Enum[newNumberOfCodes] = enumIndex;
                newNumberOfCodes ++;
            }
        }
        ptrDescriptor->range.e8.ptrCodeTab = simulationMode_Enum;
        ptrDescriptor->range.e8.numberOfCodes = newNumberOfCodes;
     }
     else if(objectIndex == CDT_IDX_actualDensitySelection)
     {
         TUSIGN8 steamType =  0xFF;
         TUSIGN8 operationMode =  CDT_LIQUID_POWER;
         (void)coordinator_Get((TUSIGN16)CDT_IDX_operatingMode,(TINT16)WHOLE_OBJECT,&operationMode);
         (void)mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType);
        
        if(operationMode == CDT_STEAM_POWER)
        {
         if(steamType == MVM_STEAMTYPE_OVERHEAT)
         {
            densitySel_Enum[0] = CDT_CALCULATION_FROM_TP;//1;
            ptrDescriptor->range.e8.ptrCodeTab = densitySel_Enum;
            ptrDescriptor->range.e8.numberOfCodes = 1;
             }
         else if(steamType == MVM_STEAMTYPE_SATURATED)
         {
            densitySel_Enum[0] = CDT_CALCULATION_FROM_TP;//1;
            densitySel_Enum[1] = CDT_CALCULATION_FROM_T;//2;
            densitySel_Enum[2] = CDT_CALCULATION_FROM_P;//3;
            ptrDescriptor->range.e8.ptrCodeTab = densitySel_Enum;
            ptrDescriptor->range.e8.numberOfCodes = 3;
         }
         else
         {
            densitySel_Enum[0] = CDT_CALCULATION_FROM_TP;//1;
            densitySel_Enum[1] = CDT_CALCULATION_FROM_T;//2;
            ptrDescriptor->range.e8.ptrCodeTab = densitySel_Enum;
            ptrDescriptor->range.e8.numberOfCodes = 2;
         }
        }
        else
        {
         if(steamType == MVM_STEAMTYPE_OVERHEAT)
         {
            densitySel_Enum[0] = CDT_EXT_DENSITY;//0;
            densitySel_Enum[1] = CDT_CALCULATION_FROM_TP;//1;
            ptrDescriptor->range.e8.ptrCodeTab = densitySel_Enum;
            ptrDescriptor->range.e8.numberOfCodes = 2;
         }
         else if(steamType == MVM_STEAMTYPE_SATURATED)
         {
             ptrDescriptor->range.e8.numberOfCodes = 4; 
         }
         else
         {
            densitySel_Enum[0] = CDT_EXT_DENSITY;//0;
            densitySel_Enum[1] = CDT_CALCULATION_FROM_TP;//1;
            densitySel_Enum[2] = CDT_CALCULATION_FROM_T;//2;
            ptrDescriptor->range.e8.ptrCodeTab = densitySel_Enum;
            ptrDescriptor->range.e8.numberOfCodes = 3;
         }
        }
     }
     else if (objectIndex == CDT_IDX_ext_algSelection)
     {
        static TUSIGN8 lExtAlgSelection[] =
        // Table referenced by: ext_algSelection
        {
            CDT_EXT_ALG_NO_FILTER,
            CDT_EXT_ALG_PRESET_1,
            CDT_EXT_ALG_PRESET_2,
        };

        ptrDescriptor->range.e8.ptrCodeTab = lExtAlgSelection;
        ptrDescriptor->range.e8.numberOfCodes = sizeof(lExtAlgSelection)/sizeof(lExtAlgSelection[0]);
     }
     else
     {
    
     }

    return returnVal;
}