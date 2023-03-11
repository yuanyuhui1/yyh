//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
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
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"

#include "Coordinator/Interface/subsystem_idx.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Diagnosis/Source/Diagnosis_mapper.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/Source/diagnosis_types.h"
#include "Diagnosis/Source/diagnosis_table_default.h"

#include "MVMeasurement/Interface/MVMeasurement.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "Coordinator/Interface/Coordinator.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "DigitalOutput/Interface/DigitalOutput.h"
#include "Totalizer/Interface/Totalizer.h"
#include "Coordinator/Interface/Coordinator_idx.h"

#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
//#include "..\..\MVMeasurement\source\MeasureClass.h"

#include "mapper_types.h"
#include "mapper_attributes.h"
#include "mapper_overload.h"
#include "mapper_execute.h"
#include "Mapper_Measurement.h"

//! private functions
static TUSIGN16 Get_DiagnosisConditionDetails_MAPPER(void FAST* ptrValue);
static TUSIGN16 Get_DiagnosisDeviceStatus_MAPPER(void FAST* ptrValue);
static TUSIGN16 Get_DiagnosisAlarmNamurConfiguration_MAPPER(TINT16 attributeIndex, void FAST* ptrValue);
static TUSIGN16 Put_DiagnosisMasking_MAPPER(void FAST* ptrValue);
static TUSIGN16 Get_DiagnosisMasking_MAPPER(void FAST* ptrValue);
static TUSIGN16 Put_DiagnosisSimulation_MAPPER(void FAST* ptrValue);
static TUSIGN16 Get_DiagnosisSimulation_MAPPER(void FAST* ptrValue);
static TUSIGN16 Put_DiagnosisGroupMasking_MAPPER(TUSIGN8 Classification,void FAST* ptrValue);

static TUSIGN16 GetValue(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue);
static TUSIGN16 GetStatus(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue);
static TUSIGN16 GetClassification(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue);
//--------------------------------------------------------------------------------------------------
/*
 \brief	Data used to convert diagnosis simulation object to Hart and viceversa
 \author	Roberto Colombo	
 \date	10-09-2008
*/
//--------------------------------------------------------------------------------------------------
const T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ activeConditionDetailsMapping[] = {
    {0,(TUSIGN16)AI_IDX,(TUSIGN16)DIAGNOSIS_IDX_analoginputAlarmSimulation,AI_ALM_SPICOM_FAIL},
    {1,(TUSIGN16)AI_IDX,(TUSIGN16)DIAGNOSIS_IDX_analoginputAlarmSimulation,AI_ALM_OUT_OF_RANGE},
    {2,(TUSIGN16)AI_IDX,(TUSIGN16)DIAGNOSIS_IDX_analoginputAlarmSimulation,AI_ALM_CUT_OFF},
    {3,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_CB_SIMUDATA},
    {4,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_CB_ALARM},
    {5,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_CB_NV_ERROR},
    {6,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_CB_MAINTENANCE},
    {7,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_CB_UNCALIBRATED},
    {8,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALRAM_T_MAX},
    {9,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALRAM_T_MIN},
    {10,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VORTEX_SENSOR_CIRCUIT_FAILURE},
    {11,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VORTEX_SENSOR_OPEN},
    {12,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VORTEX_SENSOR_SHORT},
    {13,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VORTEX_SENSOR_DRIFT},
    {14,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VIBRAT_SENSOR_CIRCUIT_FAILURE},
    {15,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VIBRAT_SENSOR_OPEN},
    {16,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VIBRAT_SENSOR_SHORT},
    {17,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_VIBRAT_SENSOR_DRIFT},
    {18,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_SYNC_MISSING},
    {19,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_SNR_FAILURE},
    {20,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_TEMP_SENSOR_UPPER_LIMIT},
    {21,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_TEMP_SENSOR_LOWER_LIMIT},
    {22,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_FE_RAM_FAILURE},
    {23,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_FE_NV_FAILURE},
    {24,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_FE_MAINTENANCE_MATURE},
    {25,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED},
    {26,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_REPLACE_REQUIRED_FE_TO_CB_BLOCKED},
    {27,(TUSIGN16)CDT_IDX,(TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarmSimulation,CDT_ALARM_REPLACE_REQUIRED_BOTH_DIRECTION_ENABLED},
    {38,(TUSIGN16)DIGITALOUTPUT_IDX,(TUSIGN16)DIAGNOSIS_IDX_digitaloutputAlarmSimulation,DIGITALOUTPUT_PULSENUMMAXALARM},
    {39,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW},
    {40,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH},
    {44,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH},
    {45,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW},
    {46,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH},
    {47,(TUSIGN16)ELETTRONICSERVICES_IDX,(TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarmSimulation,ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW},
    {48,(TUSIGN16)FE_IDX,(TUSIGN16)DIAGNOSIS_IDX_frontendinterfaceAlarmSimulation,FE_BOARD_COMM_ERROR},
    {49,(TUSIGN16)MAPPER_IDX,(TUSIGN16)DIAGNOSIS_IDX_mapperAlarmSimulation,MAPPER_ALM_PRESSURE_REACH_MAX},
    {50,(TUSIGN16)MAPPER_IDX,(TUSIGN16)DIAGNOSIS_IDX_mapperAlarmSimulation,MAPPER_ALM_PRESSURE_REACH_MIN},
    {51,(TUSIGN16)MVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,MVM_ALM_FLOWRATE_REACH_QMAX},
    {52,(TUSIGN16)MVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,MVM_ALM_FLOWRATE_REACH_QMIN},
    {53,(TUSIGN16)MVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,MVM_ALM_FLOWRATE_GT_103},
    {54,(TUSIGN16)SVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,SVM_ALM_RE_OUT_LINEAR_RANGE},
    {55,(TUSIGN16)SVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,SVM_ALM_FLOWRATE_REACH_QMAX},
    {56,(TUSIGN16)SVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_mvmeasurementAlarmSimulation,SVM_ALM_FLOWRATE_REACH_QMIN},
    {57,(TUSIGN16)SVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_svmeasurementAlarmSimulation,SVM_ALM_FLOWRATE_GT_103},
    {58,(TUSIGN16)SVM_IDX,(TUSIGN16)DIAGNOSIS_IDX_svmeasurementAlarmSimulation,SVM_ALM_LOW_FLOWRATE_CUTOFF},
    {59,(TUSIGN16)TOTALIZER_IDX,(TUSIGN16)DIAGNOSIS_IDX_totalizerAlarmSimulation,TOTALIZER_STOP_ALARM}
};

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author Jax Yang
 \date   2012-07-19
 \param	 me	this pointer
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
 \n by: Jax Yang
 \n intention: Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3: ok
 \bug :none
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_MAPPER(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = (TUSIGN16)FATAL_ERROR;
    static TBOOL dataInitialized = eFALSE;
    TUSIGN16 opModeBit = 0;
    TUSIGN8 operatingMode = CDT_LIQUID_ACTUAL_VOLUME;
    TUSIGN8 lowflowMask = MAPPER_ENABLE;
    
    //lint -e{746} 
    VIP_ASSERT(me==MAPPER_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);
    if( result==(TUSIGN16)OK )
    {

        if( (typeOfStartUp & (TUSIGN16)INIT_DATA) != 0)
        {
            VIP_ASSERT(!dataInitialized);     // only one occurence

            if (!dataInitialized)         // if not already done
            {
                // Initialize subsystem variables held in the dynamic data class.
                // (All others will be loaded up by the NV subsystem.)
                (void)me->LoadRomDefaults(me, (TINT16)MAPPER_DC_IDX_Protected);

                //Init RAM Dataclasses with Default Values.
                (void)me->LoadRomDefaults(me, (TINT16) MAPPER_DC_IDX_Unprotected);
                (void)me->LoadRomDefaults(me, (TINT16) MAPPER_DC_IDX_DynamicDuplicated);
                           
                dataInitialized = eTRUE;
            }
            
            result |= coordinator_Get((TUSIGN16)CDT_IDX_operatingMode, 0, &operatingMode);
            opModeBit = 1<<(TUSIGN16)operatingMode;
            result |= mapper.SetSelection(opModeBit);
            lowflowMask = mapperStaticFrequentConstantDefault.diagLowCutoffMask;
            result |= Put_DiagnosisMasking(DIAGNOSIS_CONDITION_LOW_FLOWRATE_CUTOFF,&lowflowMask);
        }
    }

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief	Put overload
 \author	Jax Yang
 \date	2012-07-19
 \param  me = pointer to subsystem
 \param  objectIndex = object index.
 \param  attributeIndex = attribute index.
 \param  ptrValue = pointer to object value.
 \return T_UNIT error code.
 \warning none
 \test
 \n by: Jax Yang
 \n environment: C-SPY, HART 
 \n intention: 1. complete instruction-coverage 2. see if can work with HART
 \n result module test:ok
 \n result Lint Level 3: PASS
 \bug :none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_T_MAPPER(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 presetState = 0;
    TUSIGN8 protectState = 0;
    TUSIGN8 rangeUnit[2] = {0,250};
    TUSIGN8 PV_Unit[2] = {0,250};
    VIP_ASSERT(me==MAPPER_ME);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(*(me->ptrState)>= (TUSIGN16) INITIALIZED); // exception if not initialized
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
            case MAPPER_IDX_diagFunCheckMask:
                // save the value in Mapper subsystem, then tell diagnosis to do the job
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisGroupMasking_MAPPER((TUSIGN8)DIAGNOSIS_CLASS_CHECK_FUNCTION,ptrValue);
                }
                break;
                
            case MAPPER_IDX_diagOffSpecMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisGroupMasking_MAPPER((TUSIGN8)DIAGNOSIS_CLASS_OFF_SPECIFICATION,ptrValue);
                }
                break;
                
            case MAPPER_IDX_diagMaintainMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisGroupMasking_MAPPER((TUSIGN8)DIAGNOSIS_CLASS_MAINTENANCE,ptrValue);
                }
                break;
                
            case MAPPER_IDX_diagSimulationStatus:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,ptrValue);
                }
                break;
                
            case MAPPER_IDX_diagSimulationType:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisSimulationType(ptrValue);
                }
                break;
                
            case MAPPER_IDX_diagnosisSimulation:
                result = Put_DiagnosisSimulation_MAPPER(ptrValue);
                break;

            case MAPPER_IDX_diagnosisMasking:
                result = Put_DiagnosisMasking_MAPPER(ptrValue);
                break;
                    
            case MAPPER_IDX_diagMaxFlowMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_PV_SENSOR_OUT_LIMS,ptrValue);
                }
                break;
            case MAPPER_IDX_diagMinFlowMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_PV_SENSOR_OUT_RANGE,ptrValue);
                }
                break;
            case MAPPER_IDX_diagFlow103Mask:
                // save the value in Mapper subsystem, then tell diagnosis
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_PROCESS_FLOWRATE_MORE_THAN_103,ptrValue);
                }
                break;            
            case MAPPER_IDX_diagTSensorOffSpecMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_SV_SENSOR_OUT_LIMS,ptrValue);
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_SV_SENSOR_OUT_RANGE,ptrValue);
                }
                break;
            case MAPPER_IDX_diagTxSensorOffSpecMask:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMAX,ptrValue);
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_AMBIENT_TEMP_REACH_TMIN,ptrValue);
                }
                break;
            case MAPPER_IDX_diagTSensorFailMask:
                // save the value in Mapper subsystem, then tell diagnosis
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_SV_SENSOR_FAILURE,ptrValue);
                }
                break;

            case MAPPER_IDX_diagLowCutoffMask:
                // save the value in Mapper subsystem, then tell diagnosis
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                if(result == (TUSIGN16)OK)
                {
                    result = Put_DiagnosisMasking(DIAGNOSIS_CONDITION_LOW_FLOWRATE_CUTOFF,ptrValue);
                }
                break;				
    
            // for HART    
            // PV, for PUT, only unit/upperrange/lowrange can be changed
            case MAPPER_IDX_PV_DampingTime:        //
                result = Put_Measurement(mapperProtected.PV_Assignment,M_ATR_dampingTime,ptrValue);
                break;
            case MAPPER_IDX_PV_Unit:
                result  = Put_Measurement(mapperProtected.PV_Assignment,M_ATR_unitCode,ptrValue);
                result |= mapper_Put((TUSIGN16) MAPPER_IDX_PV_RangeUnit,(TINT16)ATTRIB_0, ptrValue); 
                break;
               
            // 2012 12 11 jax command 35 
            // The Primary Variable Range Units received with this command do not effect the Primary Variable 
            // Units of the device.
            case MAPPER_IDX_PV_UpperRange:
                result = mapper_Get( (TUSIGN16)MAPPER_IDX_PV_RangeUnit,(TINT16)WHOLE_OBJECT, rangeUnit); 
                result = mapper_Get( (TUSIGN16)MAPPER_IDX_PV_Unit,(TINT16)WHOLE_OBJECT, PV_Unit); 
                if( result == (TUSIGN16)OK)
                {
                    if(unitsOfMeasure.ConvertToBaseUnit(rangeUnit[1],rangeUnit[0],ptrValue))
                    {
                        if(unitsOfMeasure.ConvertFromBaseUnit(PV_Unit[1],PV_Unit[0],ptrValue))
                        {
                            result = Put_Measurement(mapperProtected.PV_Assignment,M_ATR_upperRange,ptrValue);
                        }
                        else
                        {
                            result = (TUSIGN16)FATAL_ERROR;
                        }
                    }
                    else
                    {
                        result = (TUSIGN16)FATAL_ERROR;
                    }
                }
                break;
                
            // 2012 12 11 jax command 35 
            // The Primary Variable Range Units received with this command do not effect the Primary Variable 
            // Units of the device.
            case MAPPER_IDX_PV_LowerRange:
                result  = mapper_Get( (TUSIGN16)MAPPER_IDX_PV_RangeUnit,(TINT16)WHOLE_OBJECT, rangeUnit); 
                result |= mapper_Get( (TUSIGN16)MAPPER_IDX_PV_Unit,(TINT16)WHOLE_OBJECT, PV_Unit); 
                if( result == (TUSIGN16)OK)
                {
                    if(unitsOfMeasure.ConvertToBaseUnit(rangeUnit[1],rangeUnit[0],ptrValue))
                    {
                        if(unitsOfMeasure.ConvertFromBaseUnit(PV_Unit[1],PV_Unit[0],ptrValue))
                        {
                            result = Put_Measurement(mapperProtected.PV_Assignment,M_ATR_lowerRange,ptrValue);
                        }
                        else
                        {
                            result = (TUSIGN16)FATAL_ERROR;
                        }
                    }  
                    else
                    {
                        result = (TUSIGN16)FATAL_ERROR;
                    }
                }
                break;
            
            // SV    
            case MAPPER_IDX_SV_Unit:
                result = Put_Measurement(mapperProtected.SV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_SV_RangeUnit:
                result = Put_Measurement(mapperProtected.SV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_SV_UpperRange:
                result = Put_Measurement(mapperProtected.SV_Assignment,M_ATR_upperRange,ptrValue);
                break;
                      
            case MAPPER_IDX_SV_LowerRange:
                result = Put_Measurement(mapperProtected.SV_Assignment,M_ATR_lowerRange,ptrValue);
                break;
                
            // TV    
            case MAPPER_IDX_TV_Unit:
                result = Put_Measurement(mapperUnprotected.TV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_TV_RangeUnit:
                result = Put_Measurement(mapperUnprotected.TV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_TV_UpperRange:
                result = Put_Measurement(mapperUnprotected.TV_Assignment,M_ATR_upperRange,ptrValue);
                break;
                      
            case MAPPER_IDX_TV_LowerRange:
                result = Put_Measurement(mapperUnprotected.TV_Assignment,M_ATR_lowerRange,ptrValue);
                break;
                
            // QV    
            case MAPPER_IDX_QV_Unit:
                result = Put_Measurement(mapperUnprotected.QV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_QV_RangeUnit:
                result = Put_Measurement(mapperUnprotected.QV_Assignment,M_ATR_unitCode,ptrValue);
                break;
            case MAPPER_IDX_QV_UpperRange:
                result = Put_Measurement(mapperUnprotected.QV_Assignment,M_ATR_upperRange,ptrValue);
                break;
                      
            case MAPPER_IDX_QV_LowerRange:
                result = Put_Measurement(mapperUnprotected.QV_Assignment,M_ATR_lowerRange,ptrValue);
                break;

            case MAPPER_IDX_dvTExtPreset:
            case MAPPER_IDX_dvPressurePreset:
                presetState = MAPPER_PRESET_CHANGED;
                result  = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                result |= Put_T_UNIT(MAPPER_ME,(TUSIGN16)MAPPER_IDX_presetChanged,(TINT16)WHOLE_OBJECT,&presetState);
                mapperProtected.protectedDataStatus = MAPPER_CALCULATE;
                if(objectIndex == (TUSIGN16)MAPPER_IDX_dvPressurePreset)
                {
                    TUSIGN8 updateCFPresetRequest = MVM_GONFIGING;
                    result |= mVMeasurement_Put((TUSIGN16)MVM_IDX_updateCFPresetRequest,(TINT16)WHOLE_OBJECT,&updateCFPresetRequest);
                }
                break;
            
            case MAPPER_IDX_presetChanged:
            case MAPPER_IDX_protectedDataStatus:
            case MAPPER_IDX_PV_Assignment:
            case MAPPER_IDX_SV_Assignment:
            case MAPPER_IDX_dvTExtSimEnable:
            case MAPPER_IDX_dvDensitySimEnable:
            case MAPPER_IDX_dvContentSimEnable:
            case MAPPER_IDX_dvPressureSimEnable:
            case MAPPER_IDX_localOperatingModeSelection:
                result  = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                protectState = MAPPER_CALCULATE;
                result |= Put_T_UNIT(MAPPER_ME,(TUSIGN16)MAPPER_IDX_protectedDataStatus,(TINT16)WHOLE_OBJECT,&protectState);
            break; 
                
            default:
                result = Put_T_UNIT(MAPPER_ME,objectIndex,attributeIndex,ptrValue);
                break;
        }
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
    /*!
     \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
     \author Jax Yang
     \date   2012-07-19
     \param  me				this pointer
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
    \n by: Jax Yang
    \n
    \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
    \n
    \n intention: 1. complete instruction-coverage
    \n result module test: 2006-02-14 OK
    \n result Lint Level 3: free 
    \n bug:none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_T_MAPPER(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 rangeUnit[2]={0,0};
    TUSIGN8 PV_Unit[2] = {0,250};
    // check for zeropointer
  //  VIP_ASSERT(me);
  //  VIP_ASSERT(me==MAPPER_ME);
  //  VIP_ASSERT(ptrValue);
  //  VIP_ASSERT(me->ptrObjectList);

    // exception if not initialized
    /*lint -e613 */
   // VIP_ASSERT(*(me->ptrState)>=(TUSIGN16)INITIALIZED);

    // unknown object
    if( objectIndex>me->maxIdx )
    {
        result = (TUSIGN16)ILLEGAL_OBJ_IDX;
    }
    else
    {
        switch(objectIndex)
        {
          case MAPPER_IDX_dvTExtObj:
          case MAPPER_IDX_dvPressureObj:
          case MAPPER_IDX_dvDensityObj:
            result = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break; 
 
          case MAPPER_IDX_dvTExtUserUnit: 
           *(TFLOAT*)ptrValue = mapperUnprotected.dvTExtUserUnit;
           break;  
          case MAPPER_IDX_dvPressureUserUnit: 
           *(TFLOAT*)ptrValue = mapperUnprotected.dvPressureUserUnit;
           break; 
          case MAPPER_IDX_dvDensityUserUnit:
           *(TFLOAT*)ptrValue = mapperUnprotected.dvDensityUserUnit;
           break;
          case MAPPER_IDX_dvContent:
           *(TFLOAT*)ptrValue = mapperDynamicDuplicated.dvContent;
           break;
           
           
          case MAPPER_IDX_dvTExtClassification:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvTExtClassification;
            break;
          case MAPPER_IDX_dvPressureClassification:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvPressureClassification;
            break;
          case MAPPER_IDX_dvDensityClassification:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvDensityClassification;
            break;
          case MAPPER_IDX_dvContentClassification:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvContentClassification;
            break;            
          case MAPPER_IDX_dvTExtStatus:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvTExtStatus;
            break;
          case MAPPER_IDX_dvPressureStatus:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvPressureStatus;
            break;
          case MAPPER_IDX_dvDensityStatus:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvDensityStatus;
            break;
          case MAPPER_IDX_dvContentStatus:
            *(TUSIGN8*)ptrValue = mapperUnprotected.dvContentStatus;
            break;              
            
  
          case MAPPER_IDX_dvDummyUnitCode:
            *(TUSIGN8*)ptrValue = 0;
            break;
            
        //diagnosis
        case MAPPER_IDX_diagnosisConditionDetailsClass:
            /*lint -e613 */
            result = Get_DiagnosisConditionDetails_MAPPER(ptrValue);
            /*lint +e613 */
            break;
            
        case MAPPER_IDX_diagDeviceStatus:
            result = Get_DiagnosisDeviceStatus_MAPPER(ptrValue);
            break;

        case MAPPER_IDX_diagAlarmNamurConfiguration:
            result = Get_DiagnosisAlarmNamurConfiguration_MAPPER(attributeIndex, ptrValue);
            break;

        case MAPPER_IDX_diagnosisMasking:
            result = Get_DiagnosisMasking_MAPPER(ptrValue);
            break;
        case MAPPER_IDX_diagnosisSimulation:
            result = Get_DiagnosisSimulation_MAPPER(ptrValue);
            break;
            
        case MAPPER_IDX_diagnosisCounter:
            result = diagnosis.unit.Get(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_diagnosisCounter, attributeIndex, ptrValue);
            break;
        //PV    
        case MAPPER_IDX_PV:
            result = GetValue(mapperProtected.PV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_PV_DampingTime:
            result = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_dampingTime,(void FAST*)ptrValue);
            break;
            
        case MAPPER_IDX_PV_Unit:        //no 'break', range unit is same to unit
            //lint -e{613}
            result = GetUnit(mapperProtected.PV_Assignment, attributeIndex, ptrValue);
            break;

        // 2012 12 11 jax command 35 
        // The Primary Variable Range Units received with this command do not effect the Primary Variable 
        // Units of the device.
        case MAPPER_IDX_PV_UpperRange:
            result  = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_upperRange,ptrValue);
            result |= mapper_Get( (TUSIGN16)MAPPER_IDX_PV_RangeUnit,(TINT16)WHOLE_OBJECT, rangeUnit);
            result |= mapper_Get( (TUSIGN16)MAPPER_IDX_PV_Unit,(TINT16)WHOLE_OBJECT, PV_Unit);
            if( result == (TUSIGN16)OK)
            {
                if(unitsOfMeasure.ConvertToBaseUnit(PV_Unit[1],PV_Unit[0],ptrValue))
                {
                    if(unitsOfMeasure.ConvertFromBaseUnit(rangeUnit[1],rangeUnit[0],ptrValue))
                    {
                        result = (TUSIGN16)OK;
                    }  
                    else
                    {
                        result = (TUSIGN16)RULE_VIOLATION_ERR;
                    }
                }  
                else
                {
                    result = (TUSIGN16)RULE_VIOLATION_ERR;
                }
            }
            break;
                  
        // 2012 12 11 jax command 35 
        // The Primary Variable Range Units received with this command do not effect the Primary Variable 
        // Units of the device.
        case MAPPER_IDX_PV_LowerRange:
            result = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_lowerRange,ptrValue);
            result |= mapper_Get( (TUSIGN16)MAPPER_IDX_PV_RangeUnit,(TINT16)WHOLE_OBJECT, rangeUnit);
            result |= mapper_Get( (TUSIGN16)MAPPER_IDX_PV_Unit,(TINT16)WHOLE_OBJECT, PV_Unit);
            if( result == (TUSIGN16)OK)
            {
                if(unitsOfMeasure.ConvertToBaseUnit(PV_Unit[1],PV_Unit[0],ptrValue))
                {
                    if(unitsOfMeasure.ConvertFromBaseUnit(rangeUnit[1],rangeUnit[0],ptrValue))
                    {
                        result = (TUSIGN16)OK;
                    }
                    else
                    {
                        result = (TUSIGN16)RULE_VIOLATION_ERR;
                    }
                }  
                else
                {
                    result = (TUSIGN16)RULE_VIOLATION_ERR;
                }
            }
            break;
                  
            
        case MAPPER_IDX_PV_Classification:
            result = GetClassification(mapperProtected.PV_Assignment,attributeIndex,ptrValue);
            break;
            
        case MAPPER_IDX_PV_Status:
            result = GetStatus(mapperProtected.PV_Assignment,attributeIndex,ptrValue);
            break;
            
        // SV
        case MAPPER_IDX_SV:
            result = GetValue(mapperProtected.SV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_SV_Unit:        //no 'break', range unit is same to unit
        case MAPPER_IDX_SV_RangeUnit:
            result = GetUnit(mapperProtected.SV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_SV_UpperRange:
            result = Get_Measurement(mapperProtected.SV_Assignment,M_ATR_upperRange,ptrValue);
            break;
                  
        case MAPPER_IDX_SV_LowerRange:
            result = Get_Measurement(mapperProtected.SV_Assignment,M_ATR_lowerRange,ptrValue);
            break;

        case MAPPER_IDX_SV_Classification:
            result = GetClassification(mapperProtected.SV_Assignment,attributeIndex,ptrValue);
            break;
            
        case MAPPER_IDX_SV_Status:
            result = GetStatus(mapperProtected.SV_Assignment,attributeIndex,ptrValue);
            break;
            
         // TV    
        case MAPPER_IDX_TV:
            result = GetValue(mapperUnprotected.TV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_TV_Unit:        //no 'break', range unit is same to unit
        case MAPPER_IDX_TV_RangeUnit:
            result = GetUnit(mapperUnprotected.TV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_TV_UpperRange:
            result = Get_Measurement(mapperUnprotected.TV_Assignment,M_ATR_upperRange,ptrValue);
            break;
                  
        case MAPPER_IDX_TV_LowerRange:
            result = Get_Measurement(mapperUnprotected.TV_Assignment,M_ATR_lowerRange,ptrValue);
            break;

        case MAPPER_IDX_TV_Classification:
            result = GetClassification(mapperUnprotected.TV_Assignment,attributeIndex,ptrValue);
            break;
            
        case MAPPER_IDX_TV_Status:
            result = GetStatus(mapperUnprotected.TV_Assignment,attributeIndex,ptrValue);
            break;
            
        // QV
        case MAPPER_IDX_QV:
            result = GetValue(mapperUnprotected.QV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_QV_Unit:        //no 'break', range unit is same to unit
        case MAPPER_IDX_QV_RangeUnit:
            result = GetUnit(mapperUnprotected.QV_Assignment,attributeIndex ,ptrValue);
            break;
            
        case MAPPER_IDX_QV_UpperRange:
            result = Get_Measurement(mapperUnprotected.QV_Assignment,M_ATR_upperRange,ptrValue);
            break;
                  
        case MAPPER_IDX_QV_LowerRange:
            result = Get_Measurement(mapperUnprotected.QV_Assignment,M_ATR_lowerRange,ptrValue);
            break;
            
        case MAPPER_IDX_QV_Classification:
            result = GetClassification(mapperUnprotected.QV_Assignment,attributeIndex,ptrValue);
            break;
            
        case MAPPER_IDX_QV_Status:
            result = GetStatus(mapperUnprotected.QV_Assignment,attributeIndex,ptrValue);
            break;
            

        // command 14 2013 0109 jax
        case MAPPER_IDX_PVSensorLimitHigh:
            result = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_limitHigh,ptrValue);
            break;
        case MAPPER_IDX_PVSensorLimitLow:
            result = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_limitLow,ptrValue);
            break;
        case MAPPER_IDX_PVSensorLimitUnitcode:
            result = GetUnit(mapperProtected.PV_Assignment, attributeIndex, ptrValue);
            break;
        case MAPPER_IDX_PVSensorminimumSpan:
            result = Get_Measurement(mapperProtected.PV_Assignment,M_ATR_limitSpan,ptrValue);
            break;
            
        case MAPPER_IDX_PVPercent_Unit:
            // Todo for HART CMD 9 slot 244
            result = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break;
        case MAPPER_IDX_PVPercent_Range:
            // Todo for HART CMD 9 slot 244
            result = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break;
        case MAPPER_IDX_PVPercent_Classifiction:
            // Todo for HART CMD 9 slot 244
            result = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break;

        // PV_Percentage
       case MAPPER_IDX_PV_PercentageForDO:
            result = GetPVPercentageForDOSRV_MAPPER(ptrValue);
            break;
           
        case MAPPER_IDX_flowOver103Bhv:
            result = diagnosis_Get( (TUSIGN16)DIAGNOSIS_IDX_FlowOver103Behavior,attributeIndex, ptrValue );
            break;
        
        case MAPPER_IDX_diagSimulationStatus:
            result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,ptrValue);
            break;
            
        default:
            result = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
            break;
        }
    }
    /*lint +e613 */
    
    return (TUSIGN16)result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform command XX reading
 \author	Roberto Colombo
 \date	11-09-2008
 \return T_UNIT error code.
\warning:none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:
 \bug none
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2009-07-14 Ok
static TUSIGN16 Get_DiagnosisConditionDetails_MAPPER(void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 i;
    TUSIGN8 j;
    
    T_DIAGNOSIS_DETAILED_CONDITION diagnosisDetails;
    
    /*lint -e613 */
    VIP_ASSERT(ptrValue);

    result = diagnosis.GetDetailsOfDiagnosticConditionSRV(mapperUnprotected.diagnosisConditionNumber,&diagnosisDetails);	
      
    //lint -e{668}
    (void)memcpy(ptrValue,&diagnosisDetails.classification,sizeof(TUSIGN8));
    (void)memset(mapperUnprotected.diagnosisConditionDetailsCodes,DIAGNOSIS_CONDITIONS_UNSET,sizeof(mapperUnprotected.diagnosisConditionDetailsCodes));
    
    mapperUnprotected.diagnosisConditionDetailsGroup = (TUSIGN8)diagnosisDetails.group;
    mapperUnprotected.diagnosisConditionDetailsPriority = diagnosisDetails.priority;
    mapperUnprotected.diagnosisConditionDetailsAlarmCounter = diagnosisDetails.runtimeCounter.alarmCounter;
    mapperUnprotected.diagnosisConditionDetailsAlarmTimeCounterDay = diagnosisDetails.runtimeCounter.alarmTimeCounterDay;
    mapperUnprotected.diagnosisConditionDetailsAlarmTimeCounterMsec =  diagnosisDetails.runtimeCounter.alarmTimeCounterMsec;
    mapperUnprotected.diagnosisConditionDetailsTimeStampLastAlarmDay = diagnosisDetails.runtimeCounter.timeStampLastAlarmDay;
    mapperUnprotected.diagnosisConditionDetailsTimeStampLastAlarmMsec = diagnosisDetails.runtimeCounter.timeStampLastAlarmMsec;
            
    for( i=0; ((i<DIAGNOSIS_CONDITION_DETAILS_MAX_NUMBER) && (diagnosisDetails.details[i].alarm != DIAGNOSIS_CONDITIONS_UNSET)); i++)
    {         
        for( j=0; j<(sizeof(activeConditionDetailsMapping)/sizeof(T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ)); j++)
        {       
            if((diagnosisDetails.details[i].subIdx == activeConditionDetailsMapping[j].subsystemIndex) && \
               (diagnosisDetails.details[i].alarm == activeConditionDetailsMapping[j].attributeIndex))
            {
                mapperUnprotected.diagnosisConditionDetailsCodes[i] = activeConditionDetailsMapping[j].index;
                break;
            }
        }
    }
    /*lint +e613 */
    
    return result;
}

static TUSIGN16 Get_DiagnosisAlarmNamurConfiguration_MAPPER(TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 alarmOutputIdx;
    TUSIGN8 temp[48];
    T_DIAGNOSIS_DETAILED_CONDITION diagnosisDetails;
    
    VIP_ASSERT(ptrValue);

    if (attributeIndex == WHOLE_OBJECT)
    {
        for (alarmOutputIdx = 0; alarmOutputIdx < 48; alarmOutputIdx++)
        {
            result = diagnosis.GetDetailsOfDiagnosticConditionSRV(alarmOutputIdx, &diagnosisDetails);
            if (result == OK)
            {
                temp[alarmOutputIdx] = diagnosisDetails.classification;
            }
            else
            {
                temp[alarmOutputIdx] = 0;
            }
        }
        (void)memcpy(ptrValue, temp, 48 * sizeof(TUSIGN8));
    }
    else
    {
        result = diagnosis.GetDetailsOfDiagnosticConditionSRV(attributeIndex, &diagnosisDetails);
        if (result == OK)
        {
            *(TUSIGN8*)ptrValue = diagnosisDetails.classification;
        }
        else
        {
            *(TUSIGN8*)ptrValue = 0;
        }

    }
      
    return OK;
}


//--------------------------------------------------------------------------------------------------
/*!
\brief  Get the unit by the label
\param  label       The label defined in _E_M_Label
\param  attributeIndex  Index of the attribute    
\param  ptrValue    Lower range
\return Error code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:ok
\bug none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetUnit(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    const TUSIGN8 Unit_Hart = 250;//unsupported
    TUSIGN8 unitCode[2];
    
    /*lint -e613 */
    VIP_ASSERT(ptrValue);
    
    if(attributeIndex == (TINT16)ATTRIB_0)
    {
        result = Get_Measurement(label,M_ATR_unitCode,ptrValue);
        if(result != (TUSIGN16)OK)
        {
            //lint -e{613}
            *((TUSIGN8*)ptrValue) = Unit_Hart;
            result = (TUSIGN16)OK;
        }
    }
    else if(attributeIndex == (TINT16)ATTRIB_1)
    {
        result = Get_Measurement(label,M_ATR_unitType,ptrValue);
        if(result != (TUSIGN16)OK)
        {
            //lint -e{613}
            *((TUSIGN8*)ptrValue) = 0;
            result = (TUSIGN16)OK;
        }
    }
    else if(attributeIndex == (TINT16)WHOLE_OBJECT)
    {
        result = Get_Measurement(label,M_ATR_unitCode,(void FAST*)&(unitCode[0]));
        if(result != (TUSIGN16)OK)
        {
            unitCode[0] = Unit_Hart;
            result = (TUSIGN16)OK;
        }
        result = Get_Measurement(label,M_ATR_unitType,(void FAST*)&(unitCode[1]));
        if(result != (TUSIGN16)OK)
        {
            unitCode[1] = 0;
            result = (TUSIGN16)OK;
        }
        //lint -e{668}
        (void)memcpy(ptrValue,&unitCode[0],sizeof(unitCode));
    }
    else{}
    /*lint +e613 */
    
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
\brief  Get the value with the label
\param  label       The label defined in _E_M_Label
\param  attributeIndex  Index of the attribute    
\param  ptrValue    Lower range
\return Error code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Paul
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:ok
\bug none
*/
//--------------------------------------------------------------------------------------------------
/*lint -e715 */
static TUSIGN16 GetValue(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    const TUSIGN32 Nan_Hart = 0x7FA00000;//unsupported
    
    /*lint -e613 */
    VIP_ASSERT(ptrValue);
    
    VIP_ASSERT(((attributeIndex == ATTRIB_0) ||( attributeIndex == WHOLE_OBJECT)));
    result = Get_Measurement( label, M_ATR_dampedValue, ptrValue);
    if(result != (TUSIGN16)OK)
    {
        //lint -e{613}
        *(TUSIGN32*)ptrValue = Nan_Hart;
        result = (TUSIGN16)OK;
    }
    /*lint +e613 */
    
    return result;
}
/*lint +e715 */

//--------------------------------------------------------------------------------------------------
/*!
\brief  Get the classification by the label
\param  label       The label defined in _E_M_Label
\param  attributeIndex  Index of the attribute    
\param  ptrValue    Lower range
\return Error code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:ok
\bug none
*/
//--------------------------------------------------------------------------------------------------
/*lint -e715 */
static TUSIGN16 GetClassification(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    const TUSIGN8 Classific_Hart = 0x0;//unsupported

    VIP_ASSERT(ptrValue);
    
    VIP_ASSERT(((attributeIndex == ATTRIB_0) ||( attributeIndex == WHOLE_OBJECT)));
    result = Get_Measurement( label, M_ATR_classification, ptrValue);
    if(result != (TUSIGN16)OK)
    {
        //lint -e{613}
        *(TUSIGN8*)ptrValue = Classific_Hart;
        result = (TUSIGN16)OK;
    }

    return result;
}
/*lint +e715 */

//--------------------------------------------------------------------------------------------------
/*!
\brief  Get the status by the label
\param  label       The label defined in _E_M_Label
\param  attributeIndex  Index of the attribute    
\param  ptrValue    Lower range
\return Error code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:ok
\bug none
*/
//--------------------------------------------------------------------------------------------------
/*lint -e715 */
static TUSIGN16 GetStatus(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = (TUSIGN16)OK;
    const TUSIGN8 Status_Hart = 0x30;//unsupported
    
     
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(((attributeIndex == ATTRIB_0) ||( attributeIndex == WHOLE_OBJECT)));
    result = Get_Measurement( label, M_ATR_status, ptrValue);
    if(result != (TUSIGN16)OK)
    {
        //lint -e{613}
        *(TUSIGN8*)ptrValue = Status_Hart;
        result = (TUSIGN16)OK;
    }    
    
    return result;
}
/*lint +e715 */

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform command 48 reading
 \author	Roberto Colombo
 \date	11-09-2008
 \return T_UNIT error code.
 \warning none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2009-07-14 Ok
static TUSIGN16 Get_DiagnosisDeviceStatus_MAPPER(void FAST* ptrValue)
{   
    //lint -e{522} Ashwin Herur R 2009-07-14 Ok 
    //lint -e{10} Ashwin Herur R 2009-07-14 Ok
    T_DIAGNOSIS_ACTIVE_CONDITION activeCondiList[DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER];
    TUSIGN8 conditionCounter;
    TUSIGN8 counter;
    TUSIGN8 temp[6];
    T_DO_ATTRIB_RANGE attribRange;
    TUSIGN16 result = (TUSIGN16)OK;
    VIP_ASSERT(ptrValue);
    
    (void)memset(temp,0x00,sizeof(temp));
    result = diagnosis.GetActiveDiagnosticConditionsSRV(DIAGNOSIS_GROUP_ALL, activeCondiList);

    mapperUnprotected.diagnosisHighCondition = DIAGNOSIS_CONDITIONS_UNSET;
    mapperUnprotected.diagnosisHighClass = 0;
    mapperUnprotected.diagnosisHighGroup = 0;
    for 	(conditionCounter = 0; 
        (conditionCounter < DIAGNOSIS_ACTIVE_CONDITION_MAX_NUMBER) && (activeCondiList[conditionCounter].activeConditionId != DIAGNOSIS_CONDITIONS_UNSET);
              conditionCounter++)
    {
        TUSIGN8 bitMask, dataByte;

        dataByte = activeCondiList[conditionCounter].activeConditionId >> 3;          // equal to / 8
        bitMask = (1 <<( activeCondiList[conditionCounter].activeConditionId & 0x07));  // equal to % 8

        temp[dataByte] |= bitMask;

        if(conditionCounter == 0)
        {
            mapperUnprotected.diagnosisHighCondition = activeCondiList[conditionCounter].activeConditionId;
            mapperUnprotected.diagnosisHighClass = (TUSIGN8)activeCondiList[conditionCounter].classification;
            mapperUnprotected.diagnosisHighGroup = (TUSIGN8)activeCondiList[conditionCounter].group;
        }
    }
    
    //lint -e{613}
    //lint -e{668}
    (void)memcpy(ptrValue,temp,sizeof(temp));
    
    result = diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmHistory, (TINT16)ATTRIB_0, &attribRange);

    (void)memset(mapperUnprotected.diagHistory,0x00,sizeof(mapperUnprotected.diagHistory));
    
    for(counter = 0; counter < attribRange.range.e8.numberOfCodes; counter++)
    {
        TUSIGN8 attributeIdx, getValue, dataBit, dataByte;

        attributeIdx = attribRange.range.e8.ptrCodeTab[counter];
        //lint -e{746} Ashwin Herur R 2009-07-13 Prototype is present
        //lint -e{718} Ashwin Herur R 2009-07-10
        (void)diagnosis.unit.Get(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmHistory, attributeIdx, &getValue);

        if (getValue == SET_ALARM)
        {
            dataByte = attributeIdx >> 3;   // equal to / 8
            dataBit = attributeIdx & 0x07;  // equal to % 8
            //lint -e{734} Ashwin Herur R 2009-07-10
            mapperUnprotected.diagHistory[dataByte] |= (1 << dataBit);
        }
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform simulation reading
 \author	Roberto Colombo
 \date	11-09-2008
 \return T_UNIT error code.
 \warning
 \test
 \n by: Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:
 \bug none
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 Get_DiagnosisSimulation_MAPPER(void FAST* ptrValue)
{
    TUSIGN8   counter;
    TUSIGN16 result = (TUSIGN16)OK;

    VIP_ASSERT(ptrValue);
    
    //lint -e{613}
    //lint -e{668}
    (void)memset(ptrValue,0x00,16);
    
    for (counter = 0; counter < (sizeof(activeConditionDetailsMapping)/sizeof(T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ)); counter++)
    {
        TUSIGN8 getValue;
        TUSIGN8 dataByte, dataBit;
        
        result = diagnosis.unit.Get(DIAGNOSIS_ME, activeConditionDetailsMapping[counter].objectIndex, activeConditionDetailsMapping[counter].attributeIndex, &getValue);

        if (getValue == SET_ALARM)
        {
            dataByte = activeConditionDetailsMapping[counter].index >> 3;   // equal to / 8
            dataBit = activeConditionDetailsMapping[counter].index & 0x07;  // equal to % 8
            *((TUSIGN8*)ptrValue + dataByte) |= (1 << dataBit);
        }
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform simulation writing
 \author	Roberto Colombo
 \date	11-09-2008
 \return T_UNIT error code.
 \warning none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2009-07-13 Ok
//lint -e{818} Ashwin Herur R 2009-07-14 Ok
static TUSIGN16 Put_DiagnosisSimulation_MAPPER(void FAST* ptrValue)
{
    TUSIGN8   counter;
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8   putValue;
    TUSIGN8 diagnosisSimulationStatus;
    
    
    VIP_ASSERT(ptrValue);

    result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,&diagnosisSimulationStatus);

    if(diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED)
    {
        for (counter = 0; counter < (sizeof(activeConditionDetailsMapping)/sizeof(T_ACTIVE_CONDITION_DETAILS_MAPPING_OBJ)); counter++)
        {
            TUSIGN8 dataByte, dataBit;

            dataByte = activeConditionDetailsMapping[counter].index >> 3;   // equal to / 8
            dataBit = activeConditionDetailsMapping[counter].index & 0x07;  // equal to % 8

            //lint -e{613}
            if (*((TUSIGN8*)ptrValue + dataByte) & (1 << dataBit))
            {
                putValue = SET_ALARM;
            }
            else
            {
                putValue = CLEAR_ALARM;
            }

            result = diagnosis.unit.Put(DIAGNOSIS_ME, activeConditionDetailsMapping[counter].objectIndex, activeConditionDetailsMapping[counter].attributeIndex, &putValue);
        }
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform masking reading
 \author	Roberto Colombo
 \date	11-09-2008
 \return T_UNIT error code.
 \warning none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2009-07-13 Ok
static TUSIGN16 Get_DiagnosisMasking_MAPPER( void FAST* ptrValue)
{
    T_DO_ATTRIB_RANGE attribRange;
    TUSIGN8 counter;
    TUSIGN16 result = (TUSIGN16)OK;
    
    VIP_ASSERT(ptrValue);
    
    //lint -e{613}
    //lint -e{668}
    (void)memset(ptrValue,0x00,6);
    result = diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmMasking, (TINT16)ATTRIB_0, &attribRange);
    
    for(counter = 0; counter < attribRange.range.e8.numberOfCodes; counter++)
    {
        TUSIGN8 attributeIdx, getValue, dataBit, dataByte;

        attributeIdx = attribRange.range.e8.ptrCodeTab[counter];
        (void)diagnosis.unit.Get(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmMasking, attributeIdx, &getValue);

        if (getValue == SET_ALARM)
        {       
            dataByte = attributeIdx >> 3;   // equal to / 8
            dataBit = attributeIdx & 0x07;  // equal to % 8
            *((TUSIGN8*)ptrValue + dataByte) |= (1 << dataBit);
        }
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform masking writing
 \author	Roberto Colombo Jax yang
 \date	11-09-2008
 \param void FAST* ptrValue the pointer of 6 bytes masking command
 \return T_UNIT error code.
 \warning none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2009-07-14 Ok
//lint -e{818} Ashwin Herur R 2009-07-14 Ok
static TUSIGN16 Put_DiagnosisMasking_MAPPER(void FAST* ptrValue)
{
    T_DO_ATTRIB_RANGE attribRange;
    TUSIGN8 counter;
    TUSIGN16 result = (TUSIGN16)OK;
   
    VIP_ASSERT(ptrValue);
    
    result = diagnosis.unit.GetAttributeDescription(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmMasking, (TINT16)ATTRIB_0, &attribRange);
    
    for(counter = 0; counter < attribRange.range.e8.numberOfCodes; counter++)
    {
        TUSIGN8 attributeIdx, dataByte, dataBit, putValue;

        attributeIdx = attribRange.range.e8.ptrCodeTab[counter];
        dataByte = attributeIdx >> 3;   // equal to / 8
        dataBit = attributeIdx & 0x07;  // equal to % 8

        if (*((TUSIGN8*)ptrValue + dataByte) & (1 << dataBit))
        {
            putValue = SET_ALARM;
        }
        else
        {
            putValue = CLEAR_ALARM;
        }
        
        result |= diagnosis.unit.Put(DIAGNOSIS_ME, (TUSIGN16)DIAGNOSIS_IDX_alarmMasking, attributeIdx, &putValue);
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief	Perform Group masking writing
 \author	Spring Kunli.Zhou
 \date	20-06-2011
 \return T_UNIT error code.
 \warning none
 \test
 \n by:Jax Yang
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:passed
 \n result Lint Level 3:ok
 \bug none
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 Put_DiagnosisGroupMasking_MAPPER(TUSIGN8 Classification,void FAST* ptrValue)
{
    TUSIGN8 counter;
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 attributeIdx;

    VIP_ASSERT(ptrValue);
    for (counter = 0; counter < INPUT_TABLE_SIZE; counter++)
    {
        if(*inputTable[counter].classPtr == Classification)
        {
            attributeIdx = inputTable[counter].outputIdx;
            result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_alarmMasking,attributeIdx,ptrValue);
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/*!
\brief  There is DIAG_SIMUTYPE_VALUEOFFSET between the defined MACRO and passed in 
        parameter. Offset the parameter before call for diagnosis.
\param  ptrValue    value passed in from HMI
\return ERROR code
\warning none
\test  
\n test-date: May 14 2013 16:46:23
\n by: Jax Yang
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:ok
\bug none
*/
//------------------------------------------------------------------------------
TUSIGN16 Put_DiagnosisSimulationType(const void FAST* ptrValue)
{
    TUSIGN8 value;
    TUSIGN16 result;
    TUSIGN8 almTemp = DIAGNOSIS_SIMULATION_DISABLED;
        
    VIP_ASSERT(ptrValue);
    
    //lint -e{613}
    value = *((TUSIGN8*)ptrValue);
    
    if(value == 0) //OFF
    {
        result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,0x0,&almTemp);
    }
    else if (value >= DIAG_SIMUTYPE_VALUEOFFSET) //simulate Alarm output
    {        
        value = value - DIAG_SIMUTYPE_VALUEOFFSET;
        result = Put_DiagnosisAlarmSimulation(&value);
    }
    else  //simulate FASTOUTPUT
    {
        almTemp = DIAGNOSIS_SIMULATION_ENABLED;
        result = diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,0x0,&almTemp);
    }
    
    return result;
}