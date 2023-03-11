//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "Common/Interface/common_functions.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "mapper_types.h"
#include "mapper_attributes.h"
#include "mapper_execute.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "ElettronicServices/Interface/ElettronicServices.h"

#include "Mapper_Measurement.h"
#include "mapper_overload.h"
#include "DeviceVariables\Mapper_Density.h"
#include "DeviceVariables\Mapper_GasContent.h"
#include "DeviceVariables\Mapper_Pressure.h"
#include "DeviceVariables\Mapper_TExternal.h"

#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
//MVMeasurement
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"

#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"



const T_DYNAMIC_CHECK_TABLE mapperDynamicVariablesTable[4] = 
{  
    {(void *)&mapperDynamicDuplicated.dvTExt,		(void *)&mapperDynamicDuplicated.dvTExtDuplicated, 		sizeof(mapperDynamicDuplicated.dvTExt)},		    
    {(void *)&mapperDynamicDuplicated.dvPressure,	(void *)&mapperDynamicDuplicated.dvPressureDuplicated, 	sizeof(mapperDynamicDuplicated.dvPressure)},
    {(void *)&mapperDynamicDuplicated.dvDensity,	(void *)&mapperDynamicDuplicated.dvDensityDuplicated, 	sizeof(mapperDynamicDuplicated.dvDensity)},
    {(void *)&mapperDynamicDuplicated.dvContent,	(void *)&mapperDynamicDuplicated.dvContentDuplicated, 	sizeof(mapperDynamicDuplicated.dvContent)},      
};

static const TUSIGN8 DEFAULT_PV_ASSIGNMENT[MAPPER_OPM_NUM]=
{
	MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QN,MAPPER_DEVICE_VAR_QM,MAPPER_DEVICE_VAR_POWER,//liquid mode
	MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QN,MAPPER_DEVICE_VAR_QM,MAPPER_DEVICE_VAR_POWER,//gas mode 
    MAPPER_DEVICE_VAR_PARTIAL_QV,MAPPER_DEVICE_VAR_PARTIAL_QN, //bio-gas mode
	MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QM,MAPPER_DEVICE_VAR_POWER  //steam                   
}; 
//Because the DEFAULT_SV_ASSIGNMENT is always "MAPPER_DEVICE_VAR_T"
static const TUSIGN8 DEFAULT_SV_ASSIGNMENT = MAPPER_DEVICE_VAR_T; 

static const TUSIGN8 DEFAULT_TV_ASSIGNMENT[MAPPER_OPM_NUM]=
{
    MAPPER_DEVICE_VAR_TOTQV,MAPPER_DEVICE_VAR_TOTQN,MAPPER_DEVICE_VAR_TOTQM,MAPPER_DEVICE_VAR_TOTENERGY,
    MAPPER_DEVICE_VAR_TOTQV,MAPPER_DEVICE_VAR_TOTQN,MAPPER_DEVICE_VAR_TOTQM,MAPPER_DEVICE_VAR_TOTENERGY,
    MAPPER_DEVICE_VAR_TOTQV_PARTIAL,MAPPER_DEVICE_VAR_TOTQN_PARTIAL,
    MAPPER_DEVICE_VAR_TOTQV,MAPPER_DEVICE_VAR_TOTQM,MAPPER_DEVICE_VAR_TOTENERGY    
}; 

static const TUSIGN8 DEFAULT_QV_ASSIGNMENT[MAPPER_OPM_NUM]=
{
    MAPPER_UNSUPPORTED_DEVICE_VAR,MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QV,
    MAPPER_UNSUPPORTED_DEVICE_VAR,MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QV,  //gas
    MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QN,
    MAPPER_UNSUPPORTED_DEVICE_VAR,MAPPER_DEVICE_VAR_QV,MAPPER_DEVICE_VAR_QM,
}; 


//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETDUMMYPERCENTAGESRV> \endif
 \brief  return dummy percentage for map list
 \if @SubGen end@ <METHODHEADER GETDUMMYPERCENTAGESRV> \endif
 \author Jax Yang
 \date  2012-07-29
 \param void
 \return void
 \warning:none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
\bug:none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD GETDUMMYPERCENTAGESRV>
TFLOAT GetDummyPercentageSRV_MAPPER(void)
//@SubGen end@<METHOD GETDUMMYPERCENTAGESRV>
{
    return 0.0f;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  Update mapper Diagnosis
 \if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \author Jax Yang
 \date 2012-07-20
 \param void
 \return void
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_MAPPER(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
    static TUSIGN8 mapperDiagnosisSimulationStatus = DIAGNOSIS_SIMULATION_DISABLED;
    static TUSIGN8 mapperOldDiagnosis = 0;
    static TUSIGN8 mapperOldDiagnosisSimulation = 0;    
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 alarmSimulation=0x00,setDiagnosis=0x00,clearDiagnosis=0x00;
    TUSIGN8 alarmBitSimulation = 0x00,i=0;
    TUSIGN8 diagnosisSimulationStatus=0xff; 
    TUSIGN8 inputDiangosis = 0;
	TUSIGN8 setAlarm = SET_ALARM;
	TUSIGN8 clearAlarm = CLEAR_ALARM;      
    
    result = (TUSIGN16)diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16) WHOLE_OBJECT,&diagnosisSimulationStatus);
	if(diagnosisSimulationStatus != mapperDiagnosisSimulationStatus)
	{
		if(diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED)
		{
			mapperOldDiagnosisSimulation = mapperOldDiagnosis;
			mapperOldDiagnosis = 0;
		}
		else
		{
			mapperOldDiagnosis = mapperOldDiagnosisSimulation;
			mapperOldDiagnosisSimulation = 0;
		}

		mapperDiagnosisSimulationStatus = diagnosisSimulationStatus;
	}  
    
	if( (diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED) && (result == (TUSIGN16)OK) )
	{
		for (i=0; i<MAPPER_DIAG_SIZE; i++)
		{
			result = (TUSIGN16)mapper_Get((TUSIGN16)MAPPER_IDX_mapperAlarmSimulation,i,&alarmBitSimulation);
			if((result == (TUSIGN16)OK) && (alarmBitSimulation == SET_ALARM) )
			{
				alarmSimulation |= 1<<i;
			}
		}
		setDiagnosis = alarmSimulation & ~mapperOldDiagnosisSimulation;
		clearDiagnosis = ~alarmSimulation & mapperOldDiagnosisSimulation;
		mapperOldDiagnosisSimulation = alarmSimulation;
	}
	else
	{
		result = mapper_Get((TUSIGN16)MAPPER_IDX_mapperDiagnosis,(TINT16)WHOLE_OBJECT,&inputDiangosis);
		setDiagnosis = inputDiangosis & ~mapperOldDiagnosis;
		clearDiagnosis = ~inputDiangosis & mapperOldDiagnosis;
		mapperOldDiagnosis = inputDiangosis;
	}
	
	for(i=0; i<MAPPER_DIAG_SIZE; i++)
	{
		if(setDiagnosis & (1<<i)) 		
		{
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_mapperAlarm,i,&setAlarm);
		}
		else if(clearDiagnosis & (1<<i))
		{
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_mapperAlarm,i,&clearAlarm);
		}
		else
		{}
	}    
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEPVSVASSIGNMENT> \endif
 \brief  Update PV SV Assignment
 \if @SubGen end@ <METHODHEADER UPDATEPVSVASSIGNMENT> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param void
 \return void
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEPVSVASSIGNMENT>
TUSIGN16 UpdatePVSVAssignment_MAPPER(void)
//@SubGen end@<METHOD UPDATEPVSVASSIGNMENT>
{
    TUSIGN16 errorCode = (TUSIGN16)OK;

    return errorCode;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER UPDATEDEVICEVARIABLESEXE> \endif
 \brief  update the device variables in slow task
 \if @SubGen end@ <METHODHEADER UPDATEDEVICEVARIABLESEXE> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param void
 \return framework return codes
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDEVICEVARIABLESEXE>
TUSIGN16 UpdateDeviceVariablesEXE_MAPPER(void)
//@SubGen end@<METHOD UPDATEDEVICEVARIABLESEXE>
{
	TUSIGN16 errorCode = (TUSIGN16)OK;	
    
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;//HART input Content Selection.    
    TUSIGN32 currentOsTick = 0,intervalTime = 0;
    TUSIGN8  burstInStatus = 0xff;
    TUSIGN8 operatingMode = 0;
    TUSIGN8 gasDensitySelection = 0; 
    static TUSIGN32 hartInTickCounter = 0;
    static TUSIGN8 firstEntry = 0xff;
    TUSIGN32 hartBurstInAlarmPeriod = 0;
    TFLOAT conversionVar0 = 0.0f;
    TFLOAT conversionVar1 = 0.0f;
    TFLOAT conversionVar2 = 0.0f; 
    
	errorCode  = UpdateDensity();
    (void)coordinator_Get((TUSIGN16)CDT_IDX_operatingMode, 0, &operatingMode);
    if((operatingMode == CDT_BIOGAS_ACTUAL_VOLUME)||(operatingMode == CDT_BIOGAS_STANDARD_VOLUME))       
    {        
        errorCode |= UpdateGasContent();
    } 
    
    
    if((operatingMode == CDT_GAS_STANDARD_VOLUME )||(operatingMode == CDT_BIOGAS_STANDARD_VOLUME )||\
        (operatingMode == CDT_STEAM_POWER )||(operatingMode == CDT_GAS_POWER))
    {
        errorCode |= UpdatePressure();
    }
    else if(operatingMode == CDT_STEAM_MASS )
    {
        TUSIGN8 DensitySelection = 0;
        (void)coordinator_Get((TUSIGN16)CDT_IDX_actualDensitySelection,(TINT16)WHOLE_OBJECT,&DensitySelection);
        if((DensitySelection != CDT_EXT_DENSITY)&&(DensitySelection !=CDT_CALCULATION_FROM_T))
        {
            errorCode |= UpdatePressure();
        }   
    }
    else if(operatingMode == CDT_GAS_MASS)
    {
        (void)mVMeasurement_Get((TUSIGN16)MVM_IDX_gasDensitySelection,(TINT16)WHOLE_OBJECT,&gasDensitySelection);
        if(gasDensitySelection == MVM_REFERENCY_DENSITY)
        {
            errorCode |= UpdatePressure();
        }
    }
    else
    {
    }
    	
    	
	errorCode |= UpdateT_External();
    errorCode |= coordinator.UpdateInternalTemperature();	
    
    currentOsTick = (TUSIGN32)GetTickCount_RTOS_TASK();
    
    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);    
    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, 0, &HartSelection);    
    errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_hartBurstInAlarmPeriod, 0, &hartBurstInAlarmPeriod); 
    
    if( (errorCode == (TUSIGN16)OK) && (HartSelection!=HART_SEC_WITHOUT) )
    {
        if(firstEntry == 0xff)
        {
            firstEntry = 1;
            hartInTickCounter = currentOsTick;
        }
        else
        {
            intervalTime = (currentOsTick-hartInTickCounter)/1000;
                        
            if(intervalTime >= hartBurstInAlarmPeriod)
            {
                hartInTickCounter = currentOsTick;
                
                if(burstInStatus == HART_BURST_IN_FAILED)
                {
                    //lint -e{534} 
                    ENABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
                    MAPPER_SETDIAG_((TUSIGN16)MAPPER_ALM_NO_HART_BURST_IN,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
                    DISABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
                }
                else
                {
                    burstInStatus = HART_BURST_IN_FAILED;
                    errorCode = hart_Put((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);
                    
                    //lint -e{534} 
                    ENABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
                    MAPPER_CLRDIAG_((TUSIGN16)MAPPER_ALM_NO_HART_BURST_IN,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
                    DISABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
                }
            }
            else
            {

            }
        }
    } 
    else if( (errorCode == (TUSIGN16)OK) && (HartSelection==HART_SEC_WITHOUT))
    {
        firstEntry = 0xff;
        
        //lint -e{534} 
        ENABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
        MAPPER_CLRDIAG_((TUSIGN16)MAPPER_ALM_NO_HART_BURST_IN,MAPPER_OBJECT_LOCAL_DIAGNOSIS);
        DISABLE_DATA_PROTECTION(*(mapper.unit.ptrDataSemaphore));
    }
    else
    {}
    
    
    (void)mapper_Get((TUSIGN16)MAPPER_IDX_dvPressureObj,(TINT16)ATTRIB_2,&conversionVar0); 
    (void)mapper_Put((TUSIGN16)MAPPER_IDX_dvPressureUserUnit,(TINT16)WHOLE_OBJECT,&conversionVar0);
    (void)mapper_Get((TUSIGN16)MAPPER_IDX_dvTExtObj,(TINT16)ATTRIB_2,&conversionVar1); 
    (void)mapper_Put((TUSIGN16)MAPPER_IDX_dvTExtUserUnit,(TINT16)WHOLE_OBJECT,&conversionVar1);   
    (void)mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityObj,(TINT16)ATTRIB_2,&conversionVar2); 
    (void)mapper_Put((TUSIGN16)MAPPER_IDX_dvDensityUserUnit,(TINT16)WHOLE_OBJECT,&conversionVar2); 
       
	return errorCode;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKDATACRC> \endif
 \brief  Calculate and check crc
 \if @SubGen end@ <METHODHEADER CHECKDATACRC> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param void
 \return void
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKDATACRC>
void CheckDataCrc_MAPPER(void)
//@SubGen end@<METHOD CHECKDATACRC>
{
    TUSIGN16 crc = InitCRC16_COMMON();
    
     SuspendAll_RTOS_TASK();
	if(mapperProtected.protectedDataStatus == MAPPER_CALCULATE)
	{
		mapperProtected.protectedDataStatus = MAPPER_PROTECTED;
		//lint -e{10,40,737,52,78,520,574} Ashwin Herur R 2009-07-14 its OK to declare 'i' here       
  		crc = CalcCrc16Mem_COMMON((TUSIGN8*)&mapperProtected,crc,sizeof(mapperProtected)/sizeof(TUSIGN8));
        
        mapperUnprotected.protectedDataCrc  = crc;
	}
	else if(mapperProtected.protectedDataStatus == MAPPER_PROTECTED)
	{
	    //lint -e{10,40,737,52,78,520,574} Ashwin Herur R 2009-07-14 its OK to declare 'i' here
  		crc = CalcCrc16Mem_COMMON((TUSIGN8*)&mapperProtected,crc,sizeof(mapperProtected)/sizeof(TUSIGN8));
		if(mapperUnprotected.protectedDataCrc != crc)
		{
			elettronicServices.ExitSRV();
		}
	}
    else if( MAPPER_UNPROTECTED != mapperProtected.protectedDataStatus)
	{
			elettronicServices.ExitSRV();
	}
    else{}
    ResumeAll_RTOS_TASK();

}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETPVPERCENTAGEFORDOSRV> \endif
 \brief  error code
 \param  ptrValue percentage value for Digital Output
 \if @SubGen end@ <METHODHEADER GETPVPERCENTAGEFORDOSRV> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param: ptrValue for pv percentage
 \return framework return codes
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD GETPVPERCENTAGEFORDOSRV>
TUSIGN16 GetPVPercentageForDOSRV_MAPPER(TFLOAT* ptrValue)
//@SubGen end@<METHOD GETPVPERCENTAGEFORDOSRV>
{
	TUSIGN16 errorCode = (TUSIGN16)OK;

    /*lint -e613 */
	VIP_ASSERT(ptrValue);

    /*lint +e613 */
	
	return errorCode;   
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETPVPERCENTAGEFORCOSRV> \endif
 \brief  error code
 \param  ptrValue percentage value for Current Output
 \if @SubGen end@ <METHODHEADER GETPVPERCENTAGEFORCOSRV> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param: ptrValue for pv percentage
 \return framework return codes
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD GETPVPERCENTAGEFORCOSRV>
TUSIGN16 GetPVPercentageForCOSRV_MAPPER(TFLOAT* ptrValue)
//@SubGen end@<METHOD GETPVPERCENTAGEFORCOSRV>
{
  	TINT16 label = (TINT16)(mapperProtected.PV_Assignment);
    
    /*lint -e613 */
    VIP_ASSERT(ptrValue);
    
	//current out source
	if(label < (TINT16)M_NUMOF_DEVICEVARIABLES)
    {
        VIP_ASSERT(((*(MeasurementList[label].selectionMask)) & mapperProtected.localOperatingModeSelection));
        //lint -e{610} suppress pointer combination between __far and NULL
		VIP_ASSERT((MeasurementList[label].GetPercentage != NULL));
        *ptrValue = (*(MeasurementList[label].GetPercentage))();    
        
        return (TUSIGN16)OK;
    }
    else
    {
        return (TUSIGN16)RULE_VIOLATION_ERR;  
    }
    /*lint +e613 */
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SETSELECTION> \endif
 \brief  update PV SV TV QV assignment when operating mode changed
 \param  selection 
 \if @SubGen end@ <METHODHEADER SETSELECTION> \endif
 \author:Jax Yang
 \date 2012-07-19
 \param: TUSIGN16 selection,bit definition for operating mode mask
 \return framework return codes
 \warning none
 \test
 test-date: May 14 2013 16:46:23
 \n by: Jax Yang
 \n environment:CSPY
 \n intention:Cover branch,Cover statement,Cover decision
 \n result module test:ok
 \n result Lint Level 3:ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SETSELECTION>
TUSIGN16 SetSelection_MAPPER(TUSIGN16 selection)
//@SubGen end@<METHOD SETSELECTION>
{
	TUSIGN8 opmIndex;
    TUSIGN16 errorCode = (TUSIGN16)OK;
    TUSIGN8 rangeUnit[2] = {0,0};

    errorCode  = (TUSIGN16)mapper_Put( (TUSIGN16)MAPPER_IDX_localOperatingModeSelection,(TINT16)WHOLE_OBJECT, (void FAST*)&selection );
    errorCode |= (TUSIGN16)coordinator_Get( (TUSIGN16)CDT_IDX_operatingMode,(TINT16)WHOLE_OBJECT, &opmIndex );

    if(errorCode != (TUSIGN16)OK)
    {
        errorCode = (TUSIGN16)FATAL_ERROR;
    }
    
    return errorCode;
}

 

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/