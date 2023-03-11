//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <limits.h>
#include <math.h>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
#include "T_Unit/Interface/t_unit.h"
//lint -e766 Ashwin Herur R 2009-07-14  Includes needed
#include "Common/Interface/common_functions.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "System/Interface/assert.h"
#include "Hmi/Interface/HMI.h"
#include "Hmi/Interface/HMI_idx.h"


#include "HART/Interface/Hart.h" 
#include "HART/Layer2/Interface/layer2.h"
#include "HART/Interface/Hart_idx.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Source/Hart_types.h"
#include "HART/Source/Hart_attributes.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/nv_startup.h"

#include "SVMeasurement/Interface/SVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "FrontEndInterface/Layer7/FED_layer7.h"

#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"

#include "ARM/Interface/arm_idx.h"
#include "ARM/Interface/arm.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "ElettronicServices/Source/ElettronicServices_types.h"
#include "ElettronicServices/Source/ElettronicServices_attributes.h"
#include "ElettronicServices/Source/ElettronicServices_execute.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"
#include "ElettronicServices/Source/elettronicServices_cpu.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Interface/ServicePort_idx.h"

#if defined(HART_COMMUNICATION)
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"
#endif

#ifdef HART_COMMUNICATION
#ifdef MV
#include "../../diagnosis/Interface/Hart_MV/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "../../diagnosis/Interface/Hart_MV/diagnosis_idx.h"
#include "../../diagnosis/Interface/Hart_MV/diagnosis.h"
#else
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#endif
#endif

#ifdef PA_COMMUNICATION
#include "../../Profibus/interface/Profibus.h"
#include "../../Profibus/interface/Profibus_idx.h"
#include "../../diagnosis/Interface/PA/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "../../diagnosis/Interface/PA/diagnosis_idx.h"
#include "../../diagnosis/Interface/PA/diagnosis.h"
#endif

#ifdef FF_COMMUNICATION
#include "../../diagnosis/Interface/FF/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "../../diagnosis/Interface/FF/diagnosis_idx.h"
#include "../../diagnosis/Interface/FF/diagnosis.h"
#endif

#ifdef MODBUS_COMMUNICATION
#include "../../diagnosis/Interface/Modbus_MV/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "../../diagnosis/Interface/Modbus_MV/diagnosis_idx.h"
#include "../../diagnosis/Interface/Modbus_MV/diagnosis.h"
#endif
#ifdef MV
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "../../flow/interface/flow.h"
#include "../../flow/interface/flow_idx.h"
#include "../../t_Proc/interface/t_Proc.h"
#include "../../t_Proc/interface/t_Proc_idx.h"
#endif

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"

#if defined(PA_COMMUNICATION) || defined(FF_COMMUNICATION)
#include "../../Softing/Interface/Stack_if.h"
#endif
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"
#include "Coordinator/Source/Coordinator_types.h"
#include "Coordinator/Source/coordinator_attributes.h"


#define VSUPP_MANAGER_ADC_FULLSCALE     	1023                    //0x3FF, 10 bits
#define VSUPP_MANAGER_ADC_VREF_mV       	2500 					//mv
#define VSUPP_MANAGER_VADC_mV(counts)   	(((counts)*VSUPP_MANAGER_ADC_VREF_mV)/VSUPP_MANAGER_ADC_FULLSCALE)
#define ADC_CON2_SETTING               			0x06                //(2-Wire/VT5:m16c63) (legacy m16c62:0x07)
#define ADC_CLOCK_FREQ                  		(OSZILLATOR_CLOCK/2) 	//Hz
#define ADC_CON1_SETTING               			0x30                //2-Wire/VT5:m16c63 (m16c62:0x38)
#define ADC_CON0_SETTING                		0x05
#define ADC_CON0_SETTING_P2_7              		0x07                //current out read back: p2_7
#define ADC_VREF_DISCONNECTED           		(~0x20)
#define SUPPLY_VOLT_READBACK_COEFF		24.5						//(R4M7 + R200K)/R200K
#define TP9VOLTAGE_MID_VALUE              594    // mV*2500/1023 1451
#define TP9VOLTAGE_RANGE                  24      //4% of mid value 

#define R100K                           				100  					//k-Ohm
#define R4M7                           					4700 					//k-Ohm
#define R200K                           				200  					//k-Ohm
#define RAM_BEGIN                                   (TUSIGN16)0x400
#define RAM_RESERVED_SIZE_FOR_E8A                   (TUSIGN16)0x80
#define RAM_RESERVED_NO_TEST                        (TUSIGN16)0x12
#define USER_RAM_BEGIN                			    (RAM_BEGIN+RAM_RESERVED_SIZE_FOR_E8A+RAM_RESERVED_NO_TEST)
#define USER_RAM_END                                0x7FFF                                  //increase it for bigger RAM usage
#define RAM_END_ADDRESS                 			(TUSIGN16)0x7FFF                        //legacy: 0x44DF
#define PATTERN                       				(TUSIGN16)0x5555
#define ANTIPATTERN                     				(TUSIGN16)0xAAAA

#ifndef MV
#define ROM_BASE_ADDRESS                			(TUSIGN32)0x80000//(TUSIGN32)0x80000   //rom check: 0x80000-0x80FFF
#endif
#define CYCLIC_ASYNC_SAVE_12_MIN				(720000)      
#define CYCLIC_ASYNC_SAVE_1_MIN			        (60000)
#define CYCLIC_ASYNC_SAVE_20_SEC		        (20000)
#define CYCLIC_CHECK_AUTO_ZERO			        (50000)      

#define CYCLIC_VSUPPLY_FAULT_LOOP              20 //about 20*5s  100s



#if defined(DEBUG) || defined(INCIRCUIT_DEBUG)    //debug
#define ROM_END_ADDRESS                			(TUSIGN32)0xFFFFB
#else                                            //release
#define ROM_END_ADDRESS                			(TUSIGN32)0xFFFFB   
#endif


//#define ROM_BLOCK_SIZE                  			(TUSIGN16)(4*1024 - 1)
#define ROM_BLOCK_SIZE                  			(TUSIGN16)(1024 - 1)
#define ROM_CRC_LOC                     				(TUSIGN32)(ROM_BASE_ADDRESS)
#define MAX_ROM_BLOCKS                  			(TUSIGN16)(( (ROM_END_ADDRESS + 1) - ROM_BASE_ADDRESS)/(ROM_BLOCK_SIZE + 1))

#ifdef HART_COMMUNICATION
#define RESET_FACTOR						40
#else
#define RESET_FACTOR						50
#endif

#define RESET_TO_DEFAULT_FACTOR			30
#define LOAD_ROM_DEFAULT_FACTOR			30


#define ALL_SUBSYSTEM -1

#define RAM_TEST_LEN                   200


typedef enum
{
  ADDRESS_BUS_TEST,
  DATA_BUS_TEST,
  DATA_RETAIN_TEST
}T_RAM_TEST_STATE_ENUM;

typedef struct
{
    TUSIGN16* address;
    TUSIGN16  walkOnePattern;
    T_RAM_TEST_STATE_ENUM   state;
    TUSIGN16  offset;
    TUSIGN16* addressOffset;
    TUSIGN16 bakup1;
    TUSIGN16 bakup2;
} T_RAM_TEST_STRUCT;


typedef struct
{
    TUSIGN16 crc16;
    TUSIGN8 HUGE* blkStartAdrs;
    TUSIGN8 HUGE* blkEndAdrs;
    TUSIGN8 HUGE* ptrAdrs;
    TUSIGN16 block;
    TUSIGN16 crcMem;
} T_ROM_TEST_STRUCT;

#ifdef HART_COMMUNICATION
//#pragma dataseg=NEAR NO_RAM_TEST_LOCATION
static __no_init T_RAM_TEST_STRUCT esRamTst;
//#pragma dataseg = default
#define esTaskCntInit   0x0U

TUSIGN8 esTaskCnt = esTaskCntInit;
static T_ROM_TEST_STRUCT esRomTst;
#endif
#define esTaskCntInit   0x0U
TUSIGN8 esTaskCnt = esTaskCntInit;

#ifdef PUSH_BUTTON
//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
static TUSIGN8	pbStatus = PB_IDLE;
//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
static TUSIGN8	pbPushedButton = PB_NULL;
static TUSIGN32	pbTimeStart;
#endif

#ifdef HART_COMMUNICATION
static TINT8 		esCurrentOutReadbackFailHighCnt = 0;
static TINT8 		esCurrentOutReadbackFailLowCnt = 0;
static TINT8 		esTP9VoltageReadbackFailHighCnt = 0;
static TINT8 		esTP9VoltageReadbackFailLowCnt = 0;
static TINT8        esTP9VoltageDiagStatus         = 0;

static TINT8 		esSupplyVoltReadbackFailCntHigh = 0;
static TINT8 		esSupplyVoltReadbackFailCntLow = 0;
//lint -e{40} Ashwin Herur R 2009-07-10
//lint -e{34} Ashwin Herur R 2009-07-10
static TUSIGN8			esDiagnosisSimulationStatus = DIAGNOSIS_SIMULATION_DISABLED;
static TUSIGN8			esOldDiagnosis = 0;
static TUSIGN8			esOldDiagnosisSimulation = 0;
#endif

static TUSIGN8 	esWriteProtection = ELETTRONICSERVICES_WRITE_PROTECTION_NONE;
static TUSIGN32	esAsynSaveTimeOld = 0;
static TUSIGN32	esAsynSaveCount = 0;
/* add to save NV data when votage is low*/
static TUSIGN8  esVsupplyFlag = 1;
static TUSIGN8 esVsupplySaveCount = 0;


TBOOL 			esResetDevice = eFALSE;
static TUSIGN8	esResetCounter = 0;
TBOOL 			esResetToDefault = eFALSE;
static TUSIGN8	esResetToDefaultCounter = 0;
TBOOL 			esLoadRomDefault = eFALSE;
static TUSIGN8	esLoadRomDefaultCounter = 0;

//static TUSIGN32 esAutoZeroTimeOld = 0;
TUSIGN32	esAutoZeroCount = 0;

#if 0
const T_DYNAMIC_CHECK_TABLE esDynamicVariablesTable[4] =
{
    {(void *)&elettronicServicesDynamicDuplicated.currentOutReadBack, 				(void *)&elettronicServicesDynamicDuplicated.currentOutReadBackDuplicated, 				sizeof(elettronicServicesDynamicDuplicated.currentOutReadBack)},
    {(void *)&elettronicServicesDynamicDuplicated.elettronicTemperature, 				(void *)&elettronicServicesDynamicDuplicated.elettronicTemperatureDuplicated, 				sizeof(elettronicServicesDynamicDuplicated.elettronicTemperature)},
    {(void *)&elettronicServicesDynamicDuplicated.vSupplyReadBack, 					(void *)&elettronicServicesDynamicDuplicated.vSupplyReadBackDuplicated, 					sizeof(elettronicServicesDynamicDuplicated.vSupplyReadBack)},
    {(void *)&elettronicServicesDynamicDuplicated.localDiagnosis, 					(void *)&elettronicServicesDynamicDuplicated.localDiagnosisDuplicated, 						sizeof(elettronicServicesDynamicDuplicated.localDiagnosis)},
};



const RegAndInstructionFunc RegAndInstructionFuncTable[25]=
{


#ifdef USE_TestGPRsCoupling
    TestGPRsCoupling,
#else
    TestGPRs,
#endif


    TestIntRegs,
    TestStackRegs,
    TestPCReg,
    TestFlagReg,
    //asm
    Test_Mov,
    Test_Adjnz,
    Test_And,
    Test_Bmcnd,
    Test_Logic,
    Test_Add,
    Test_Jcnd,
    Test_Ldc,
    Test_Calc,
    Test_Rot,
    Test_Shift,
    Test_Sbb,
    Test_Xchg,
    Test_Movdir,
    Test_Smov,
    Test_Stz,
    Test_Rmpa,
    Test_Tst,
    Test_Mova,
    Test_Jmp,
};
#endif

#ifdef HART_COMMUNICATION
static TUSIGN16 VsupplyMonitor(void);
static TUSIGN16 MeasureAdc(void);
static TUSIGN16 Filter(TUSIGN16 value);
static TFLOAT TemperatureElettronicMeasurement_ElectronicServices(void);
static void RomCheck_ElectronicServices(void);
static void VsupplyReadBack_ElectronicServices(void);
static void CheckCrcData_ElectronicServices(void);
#endif


static void SwWriteProtection_ElectronicServices(void);

#ifndef LOCALSERVER_SELECT
extern FAST T_FED_FRAME_RX_BUFFER uReceiveBuffer_FED_layer7;
extern FAST T_FED_FRAME_TX_BUFFER uTransmitBuffer_FED_layer7;
#endif

extern void writeDefault(TUSIGN8 length, TUSIGN8* const pDataBuf);


//-------------------------------------------------------------------------------------------------
/*!
\brief  read front end RTC information
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//lint -e818 
void readFeRtc_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_FE_RTC_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uReceiveBuffer_FED_layer7)+4)))
    {
        elettronicServicesUnprotected.frontendRtc[0]= *((TUSIGN8*) pDataBuf);  
        elettronicServicesUnprotected.frontendRtc[1]= *((TUSIGN8*) (pDataBuf+1));  
        elettronicServicesUnprotected.frontendRtc[2]= *((TUSIGN8*) (pDataBuf+2));  
        elettronicServicesUnprotected.frontendRtc[3]= *((TUSIGN8*) (pDataBuf+3));  
        elettronicServicesUnprotected.frontendRtc[4]= *((TUSIGN8*) (pDataBuf+4));  
    }
    ResumeAll_RTOS_TASK();   
#endif           
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read front end software and hardware version and store as ASCII
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void readFeVersion_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_FE_VERSION_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uReceiveBuffer_FED_layer7)+4)))
    {
        elettronicServicesUnprotected.hwRevisionHmi_fe[0]= ((*((TUSIGN8*) (pDataBuf+2)))/0x0A) + 0x30;  
        elettronicServicesUnprotected.hwRevisionHmi_fe[1]= ((*((TUSIGN8*) (pDataBuf+2)))%0x0A) + 0x30;   
        elettronicServicesUnprotected.hwRevisionHmi_fe[3]= ((*((TUSIGN8*) (pDataBuf+1)))/0x0A) + 0x30;  
        elettronicServicesUnprotected.hwRevisionHmi_fe[4]= ((*((TUSIGN8*) (pDataBuf+1)))%0x0A) + 0x30;    
        elettronicServicesUnprotected.hwRevisionHmi_fe[6]= ((*((TUSIGN8*)  pDataBuf))/0x0A) + 0x30;  
        elettronicServicesUnprotected.hwRevisionHmi_fe[7]= ((*((TUSIGN8*)  pDataBuf))%0x0A) + 0x30;  
        
        elettronicServicesUnprotected.swRevisionHmi_fe[0]= ((*((TUSIGN8*) (pDataBuf+5)))/0x0A) + 0x30; 
        elettronicServicesUnprotected.swRevisionHmi_fe[1]= ((*((TUSIGN8*) (pDataBuf+5)))%0x0A) + 0x30;
        elettronicServicesUnprotected.swRevisionHmi_fe[3]= ((*((TUSIGN8*) (pDataBuf+4)))/0x0A) + 0x30; 
        elettronicServicesUnprotected.swRevisionHmi_fe[4]= ((*((TUSIGN8*) (pDataBuf+4)))%0x0A) + 0x30;   
        elettronicServicesUnprotected.swRevisionHmi_fe[6]= ((*((TUSIGN8*) (pDataBuf+3)))/0x0A) + 0x30; 
        elettronicServicesUnprotected.swRevisionHmi_fe[7]= ((*((TUSIGN8*) (pDataBuf+3)))%0x0A) + 0x30;
    }
    ResumeAll_RTOS_TASK();
#endif      
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read front end auto zero information, includes autozero status and lowflowthresh
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void readAutoZeroStatus_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_AUTOZERO_STAUS_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uReceiveBuffer_FED_layer7)+4)))
    {
        elettronicServicesUnprotected.autozeroStatus = *((TUSIGN8*) pDataBuf); 
        coordinatorUnprotected.lowFlowThresh =  *(TINT16*)((TUSIGN8*) pDataBuf+1); 
    }
    ResumeAll_RTOS_TASK();
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  read front end snrThreshold
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void readSnrThreshold_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_SNR_THRESHOLD_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uReceiveBuffer_FED_layer7)+4)))
    {
        //lint -e826 allow the conversion
        elettronicServicesUnprotected.snrThreshold = *((TINT16*) pDataBuf);  
    }
    ResumeAll_RTOS_TASK();
#endif   
}


//-------------------------------------------------------------------------------------------------
/*!
\brief  write  front end RTC
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void wrtieFeRtc_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_FE_RTC_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uTransmitBuffer_FED_layer7)+4)))
    {
        *(pDataBuf)    =  elettronicServicesUnprotected.frontendRtc[0];
        *(pDataBuf+1)  =  elettronicServicesUnprotected.frontendRtc[1];
        *(pDataBuf+2)  =  elettronicServicesUnprotected.frontendRtc[2];
        *(pDataBuf+3)  =  elettronicServicesUnprotected.frontendRtc[3];
        *(pDataBuf+4)  =  elettronicServicesUnprotected.frontendRtc[4];  
    }
    ResumeAll_RTOS_TASK();
#endif    
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  write  front end software and hardware version
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void wrtieFeHwVersion_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_FE_HD_VERSION_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uTransmitBuffer_FED_layer7)+4)))
    {
        //lint -e734
        *(pDataBuf)    =  (elettronicServicesUnprotected.hwRevisionHmi_fe[7]-0x30);
        *(pDataBuf)   +=  (elettronicServicesUnprotected.hwRevisionHmi_fe[6]-0x30)*0x0A;
        *(pDataBuf+1)  =  (elettronicServicesUnprotected.hwRevisionHmi_fe[4]-0x30);
        *(pDataBuf+1) +=  (elettronicServicesUnprotected.hwRevisionHmi_fe[3]-0x30)*0x0A;
        *(pDataBuf+2)  =  (elettronicServicesUnprotected.hwRevisionHmi_fe[1]-0x30);
        *(pDataBuf+2) +=  (elettronicServicesUnprotected.hwRevisionHmi_fe[0]-0x30)*0x0A;
    }
    ResumeAll_RTOS_TASK();
#endif     
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  write  front end SnrThreshold
\author
\date
\param  length     message length
\param  pDataBuf   message address
\return  none
\warning
\test 
test-date: 2013-05-23
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void writeSnrThreshold_es(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
#ifndef LOCALSERVER_SELECT
    SuspendAll_RTOS_TASK();
    if((length==ELETTRONICSERVICES_SNR_THRESHOLD_LENGTH)&&((pDataBuf) == ((TUSIGN8*)(&uTransmitBuffer_FED_layer7)+4)))
    {
        *((TINT16*) pDataBuf) = elettronicServicesUnprotected.snrThreshold ;
    }
    ResumeAll_RTOS_TASK();
#endif
}


void ( * comRead_es[])(TUSIGN8 , TUSIGN8* const)=
{
  //readSignalAmplitude_es,
  //readVabrationAmplitude_es,
  readFeRtc_es,
  readFeVersion_es,
  readAutoZeroStatus_es,
  readSnrThreshold_es
};


void ( * comWrite_es[])(TUSIGN8 , TUSIGN8* const)=
{
     writeDefault,  // write auto zero cmd
     writeDefault, // write self diagnosis cmd
     wrtieFeRtc_es,// write front end diagnosis
     wrtieFeHwVersion_es,// write FE hardware version
     writeSnrThreshold_es,
     writeDefault
};

static TUSIGN32 ESSequenceFlag = 0x0;

enum FUNCTION_FLAG
{
    WP_FLAG = 0,
#ifdef HART_COMMUNICATION
    COREAD_FLAG,
    RESET_FLAG,
    CYCLE_FLAG,
    //PUSHBUTTON_FLAG,
    LOADROM_FLAG,
    RESETDEFAULT_FLAG,
    INITNV_FLAG,
#else
    CYCLESAVE_FLAG,
#endif
    SIZEOF_FLAG
};


//-------------------------------------------------------------------------------------------------
/*!
 \brief  set flag of maintask.
\author
\date
\param index-
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
*/
//-------------------------------------------------------------------------------------------------
static void SetFlag(TUSIGN16 index)
{
    VIP_ASSERT(index < (TUSIGN16)SIZEOF_FLAG);
    ESSequenceFlag |= ((TUSIGN32)1)<<index;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  check flag of maintask. if the flag from 0 to index are all set, return. otherwise, reset device.
\author
\date
\param index
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
*/
//-------------------------------------------------------------------------------------------------
static void CheckFlag(TUSIGN16 index)
{
    TUSIGN32 flag = 0x0;
    VIP_ASSERT(index <= (TUSIGN16)SIZEOF_FLAG);
    flag = ( (TUSIGN32)1<<index ) -1;
    if(ESSequenceFlag == flag)
    {
    }
    else
    {
        elettronicServices.ExitSRV();// reset device;
    }
}

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKDECREASINGVOLTAGE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER CHECKDECREASINGVOLTAGE> \endif
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
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKDECREASINGVOLTAGE>
TBOOL CheckDecreasingVoltage_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD CHECKDECREASINGVOLTAGE>
{
#if 0
    /* 1. there is a requriement that when voltage is dropping from normal to 0 */
    /* the output must be maintein or decrese to alarm current immidiately */
    /* the issue is when voltage is drop to about 11 V, the output cannot maintein high current output */
    /* so the output will drop unexpected. What we do is to set the output to alarm current output when */
    /* a low voltage is detected, so it can keep the alarm current until the voltage drop to power off the device */
    /* 2. when set the output to alarm current, if a resistor is connected, the terminal voltage will rise because of the resistor */
    /* so the alarm current output will last at least 7 seconds to make sure no back and forth to the output */
    typedef enum __E_VOLT_DROP_STATE__
    {
        CHECKING_NOT_START = 0,
        CHECKING_START = 1,
        CONFIRM_STATE = 2
    }E_VOLT_DROP_STATE;

    #define MIN_VOLT_MAINTAIN_OUTPUT_MV 11500
    #define MIN_VOLT_START_CHECKING_DROP (MIN_VOLT_MAINTAIN_OUTPUT_MV + 500)
    #define MIN_ALARM_TIME_WHEN_DROP_DETECTED_MS 7000

    static TUSIGN8 sContinuousDroppingCount = 0;
    static TUSIGN16 sVsupplyLast = 0;
    static E_VOLT_DROP_STATE sCheckState = CHECKING_NOT_START;
    static TUSIGN32 sLastTimeStamp = 0;

    TBOOL voltageDecreaseHappens = eFALSE;

    TFLOAT  	temperature = elettronicServicesDynamicDuplicated.elettronicTemperature;
    TUSIGN16 	vsupply = 0;
    TUSIGN8  	count = 0;
    TUSIGN32 currentTimeStamp = GetTickCount_RTOS_TASK();

    // check Vsupply 8 times for better results with filtering
    for(count = 0; count < 8; count++)
    {
        vsupply = VsupplyMonitor();
    }

    vsupply = vsupply - (TUSIGN16)(2.5f*(20.0f-temperature)*3.0f);

    elettronicServicesDynamicDuplicated.vSupplyReadBack = vsupply;
    elettronicServicesDynamicDuplicated.vSupplyReadBackDuplicated = vsupply;

    switch(sCheckState)
    {
        case CHECKING_NOT_START:
        {
            if (vsupply <= MIN_VOLT_START_CHECKING_DROP)
            {
                sCheckState = CHECKING_START;
            }
        }
        break;

        case CHECKING_START:
        {
            if (vsupply < sVsupplyLast)
            {
                sContinuousDroppingCount ++;
                if (sContinuousDroppingCount >= 2)
                {
                    sContinuousDroppingCount = 0;

                    if (vsupply <= MIN_VOLT_MAINTAIN_OUTPUT_MV)
                    {
                        sCheckState = CONFIRM_STATE;

                        sLastTimeStamp = currentTimeStamp;
                    }
                }
            }
            else
            {
                sContinuousDroppingCount = 0;
            }
        }
        break;

        case CONFIRM_STATE:  // voltage continuouse dropping to 12 V
        {
            if (vsupply < MIN_VOLT_MAINTAIN_OUTPUT_MV)
            {
            }
            else
            {
                if ((currentTimeStamp - sLastTimeStamp) >= MIN_ALARM_TIME_WHEN_DROP_DETECTED_MS)
                {
                    sCheckState = CHECKING_NOT_START;
                }
            }
        }
        break;

        default:
        {
            sCheckState = CHECKING_NOT_START;
        }
        break;
    }

    sVsupplyLast = vsupply;

    voltageDecreaseHappens = (CONFIRM_STATE == sCheckState) ? eTRUE : eFALSE;

    return voltageDecreaseHappens;
#endif
    return eFALSE;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKAUTOZEROSTATUS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER CHECKAUTOZEROSTATUS> \endif
 \author Lawrence Shi
 \date 2013-01-16
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKAUTOZEROSTATUS>
void CheckAutoZeroStatus_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD CHECKAUTOZEROSTATUS>
{
#if 0
    TUSIGN8 autozeroStatus = ELETTRONICSERVICES_STATUS_UNCOMPLETED;
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN32 time = esTime; 
    
    result = elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_autozeroStatus,(TINT16)WHOLE_OBJECT,&autozeroStatus);
    if((autozeroStatus ==ELETTRONICSERVICES_STATUS_UNCOMPLETED)&&(esAutoZeroCount <= CYCLIC_CHECK_AUTO_ZERO))
    {

        result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_READ_AUTOZERO_STATUS,ELETTRONICSERVICES_AUTOZERO_STAUS_LENGTH,comRead_es[ELETTRONICSERVICES_READ_AUTOZERO_STATUS_INDEX],NULL);
        if(result == (TUSIGN16)OK)
        {
            if(esAutoZeroCount == 0)
            {
                esAutoZeroTimeOld = 0;
            }
            else
            {
                if(time > esAutoZeroTimeOld)
                {
                    esAutoZeroCount += (time - esAutoZeroTimeOld);
                }
                else
                {
                    esAutoZeroCount += SLOW_UPDATE_TIME;		
                }		
            }
            
            esAutoZeroTimeOld = time;
        }
        else
        {
        }
    }
    else
    {
    }
#endif
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKDATACRC> \endif
 \brief  CRC check all rarely change data
 \if @SubGen end@ <METHODHEADER CHECKDATACRC> \endif
 \author Paul
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKDATACRC>
void CheckDataCrc_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD CHECKDATACRC>
{
#if 0
    TUSIGN16 crc = InitCRC16_COMMON();
    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
    //lint -e{539} hao wu,  2013-01-15 Ok
    if(elettronicServicesProtected.protectedDataStatus == ELETTRONICSERVICES_CALCULATE)
    {
        elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_PROTECTED;
        crc = CalcCrc16Mem_COMMON((TUSIGN8*)&elettronicServicesProtected,crc,sizeof(elettronicServicesProtected)/sizeof(TUSIGN8));
        elettronicServicesUnprotected.protectedDataCrc = crc;
    }
    //lint -e{830} hao wu,  2013-01-15 Ok
    else if(elettronicServicesProtected.protectedDataStatus == ELETTRONICSERVICES_PROTECTED)
    {
        crc = CalcCrc16Mem_COMMON((TUSIGN8*)&elettronicServicesProtected,crc,sizeof(elettronicServicesProtected)/sizeof(TUSIGN8));
        if(elettronicServicesUnprotected.protectedDataCrc != crc)
        {
            elettronicServices.ExitSRV();
        }
    }
    else if(ELETTRONICSERVICES_UNPROTECTED != elettronicServicesProtected.protectedDataStatus)
    {
        elettronicServices.ExitSRV();
    }
    else
    {
    }
    //lint -e{539} hao wu,  2013-01-15 Ok
    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKCPUASM> \endif
 \brief  Cyclic Check Asm and Cpu Register or other
 \if @SubGen end@ <METHODHEADER CHECKCPUASM> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKCPUASM>
void CheckCpuAsm_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD CHECKCPUASM>
{
#if 0
    /*TestGPRs is not required if using TestGPRsCoupling*/

    static TUSIGN16 index = 0;

    if(index <(sizeof(RegAndInstructionFuncTable)/sizeof(RegAndInstructionFunc)))
    {
    }
    else
    {
        index = 0;
    }
        
    if((index <= 4)&&(index != 0))
    {
        asm("FCLR I");
    }
    (*(RegAndInstructionFuncTable[index]))();
    if((index <= 4)&&(index != 0))
    {
        asm("FSET I");
    }
    index++;
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER DEVICERESETSRV> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER DEVICERESETSRV> \endif
 \author		Roberto Colombo
 \date		30-07-2008
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
\n bug:
 \n 
*/  
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD DEVICERESETSRV>
void DeviceResetSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD DEVICERESETSRV>
{
    if(esResetDevice)
    {
        esResetCounter++;

        if(esResetCounter > RESET_FACTOR)
        {

            coordinator.StopTasksSRV(CDT_MAIN_TASK|CDT_CPULOAD_TASK|CDT_SERVICE_TASK|CDT_FE_TASK|CDT_HART_TASK);


            diagnosisCommonResultInitialization = REPLACE_DISABLED;
    
            
            (void)Save((TINT16)ALL_FILES,NV_SYNC);

            ExitSRV_ELETTRONICSERVICES();
            while(1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER LOADROMDEFAULTSRV> \endif
 \brief  
\if @SubGen end@ <METHODHEADER LOADROMDEFAULTSRV> \endif
\author				Roberto Colombo
\date				2008-10-10
\param void
\return void
\warning
\test
test-date:			12-05-2009
\n by:				Ganapathi R
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD LOADROMDEFAULTSRV>
void LoadRomDefaultSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD LOADROMDEFAULTSRV>
{
    if(esLoadRomDefault)
    {
        esLoadRomDefaultCounter++;

        if(esLoadRomDefaultCounter > LOAD_ROM_DEFAULT_FACTOR)
        {
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            TUSIGN16 result = OK;
            TINT16 subIdx;
            TINT16 classIdx;
            TUSIGN16 idx, startIdx, endIdx;
            const T_UNIT SLOW* unitPtr = 0;

            coordinator.StopTasksSRV(CDT_MAIN_TASK|CDT_CPULOAD_TASK|CDT_SERVICE_TASK|CDT_FE_TASK|CDT_HART_TASK);


            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            result = elettronicServices_Get(ELETTRONICSERVICES_IDX_loadRomDefault,WHOLE_OBJECT,&subIdx);
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            result |= elettronicServices_Get(ELETTRONICSERVICES_IDX_classLoadRomDefault,WHOLE_OBJECT,&classIdx);
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            if(subIdx == ALL_SUBSYSTEM)
            {
                startIdx = 0;
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                endIdx = MEMBER_IDX_NUM_ELEMENTS;
            }
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            else //if((subIdx > ALL_SUBSYSTEM) && (subIdx < MEMBER_IDX_NUM_ELEMENTS))
            {
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                startIdx = subIdx;
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                endIdx = subIdx + 1;
            }
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            //lint -e{644} Ashwin Herur R 2009-07-14 Initialization Ok
            for(idx=startIdx; (idx<endIdx) && (result == OK); idx++)
            {
                unitPtr = GetSubsystemPtr(idx);

                result = unitPtr->LoadRomDefaults(unitPtr,classIdx);
            }

            diagnosisCommonResultInitialization = REPLACE_DISABLED;

            (void)Save((TINT16)ALL_FILES,NV_SYNC);

            elettronicServices.ExitSRV();
            while(1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER RESETTODEFAULTSRV> \endif
 \brief  
\if @SubGen end@ <METHODHEADER RESETTODEFAULTSRV> \endif
\author				Roberto Colombo
\date				2008-10-10
\param void
\return void
\warning
\test
test-date:			12-05-2009
\n by:				Ganapathi R
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETTODEFAULTSRV>
void ResetToDefaultSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD RESETTODEFAULTSRV>
{
    if(esResetToDefault)
    {
        esResetToDefaultCounter++;

        if(esResetToDefaultCounter > RESET_TO_DEFAULT_FACTOR)
        {
            TUSIGN16 result = 0;
            TINT16 subIdx;
            TUSIGN16 idx, startIdx, endIdx;
            const T_UNIT SLOW* unitPtr = 0;

            coordinator.StopTasksSRV(CDT_MAIN_TASK|CDT_CPULOAD_TASK|CDT_SERVICE_TASK|CDT_FE_TASK|CDT_HART_TASK);

            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            // (void)elettronicServices_Get(ELETTRONICSERVICES_IDX_resetToDefault, WHOLE_OBJECT, &subIdx);
            result = elettronicServices_Get(ELETTRONICSERVICES_IDX_resetToDefault, WHOLE_OBJECT, &subIdx); //edit by Hao.Wu  i think it haS a defect.  07/09/2012
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            if(subIdx == ALL_SUBSYSTEM)
            {
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                startIdx = 0;
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                endIdx = MEMBER_IDX_NUM_ELEMENTS;
            }
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            else //if((subIdx > ALL_SUBSYSTEM) && (subIdx < MEMBER_IDX_NUM_ELEMENTS))
            {
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                startIdx = subIdx;
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                endIdx = subIdx + 1;
            }
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            //lint -e{644} Ashwin Herur R 2009-07-14 Initialization Ok
            for(idx=startIdx; (idx<endIdx) && (result == OK); idx++)
            {
                unitPtr = GetSubsystemPtr(idx);

                result = unitPtr->ResetToDefault(unitPtr,ALL_DATACLASSES);
            }

            //p_dP_Put(PDP_IDX_limitSet, WHOLE_OBJECT, &limitSet);

            diagnosisCommonResultInitialization = REPLACE_DISABLED;

            (void)Save((TINT16)ALL_FILES,NV_SYNC);

            elettronicServices.ExitSRV();
            while(1);
        }
    }
    else
    {
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER INITIALIZENVCHECKSRV> \endif
 \brief  
\if @SubGen end@ <METHODHEADER INITIALIZENVCHECKSRV> \endif
\author				Roberto Colombo
\date				2008-10-10
\param void
\return void
\warning
\test
test-date:			12-05-2009
\n by:				Ganapathi R
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD INITIALIZENVCHECKSRV>
TUSIGN8 InitializeNvCheckSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD INITIALIZENVCHECKSRV>
{
    return elettronicServicesProtected.initializeNv;
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER INIZIALIZENVSRV> \endif
 \brief  
\if @SubGen end@ <METHODHEADER INIZIALIZENVSRV> \endif
\author				Roberto Colombo, Paul
\date				2008-10-10, 2012,10,29
\param void
\return void
\warning
\test
test-date:			10-29-2012
\n by:				Ganapathi R, xiaojin kuang
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD INIZIALIZENVSRV>
void InizializeNvSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD INIZIALIZENVSRV>
{
    TUSIGN8     initiallizeNv = ELETTRONICSERVICES_NV_NOT_INITIALIZE;
    TUSIGN16 result = (TUSIGN16)OK;
    result = elettronicServices_Get((TINT16)ELETTRONICSERVICES_IDX_initializeNv,(TINT16)WHOLE_OBJECT,&initiallizeNv);
    //first check whether initiallizeNv need to be done?
    if((result == (TUSIGN16)OK) && (initiallizeNv == ELETTRONICSERVICES_NV_INITIALIZE_START))
    {
//modify from 1.6.0
// Current out will be possible to output low current than 3.5, if we format NV;
// if all ram data are formatted/load rom default, some subsystem will act different, so just stop all other task, and wait for CB to reset by watchdog;
        coordinator.StopTasksSRV(CDT_MAIN_TASK|CDT_CPULOAD_TASK|CDT_SERVICE_TASK|CDT_FE_TASK|CDT_HART_TASK);
        diagnosisCommonResultInitialization = REPLACE_DISABLED;

        //P6_bit.P6_4 = 0;	//Hmi Freezing
#ifdef HART_COMMUNICATION //1. load rom default for all calibration data. (this will base on value of initiallizeNv)

		 result |= LoadRomDefaults_T_UNIT(ELETTRONIC_SERVICES_ME,(TINT16)ELETTRONICSERVICES_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT);
		 result |= LoadRomDefaults_T_UNIT(ANALOG_INPUT_ME, (TINT16)ANALOGINPUT_DC_IDX_StaticFreqStaticDefalutCalibration);
		 result |= LoadRomDefaults_T_UNIT(SVMEASUREMENT_ME,(TINT16)SVMEASUREMENT_DC_IDX_ReplaceStaticRareCalibration);
		 result |= LoadRomDefaults_T_UNIT(COORDINATOR_ME,(TINT16)COORDINATOR_DC_IDX_ReplaceStaticRare_Calibration);

		 VIP_ASSERT((TUSIGN16)OK == result);
#endif
        //2.  load all other default value include initiallizeNv object.
        //3.  Format all NV include calibration data 
        //4.  Save all data into NV, include calibration data
        (void)FormatAllData_intern(FORMAT_ALL_DATA);


#if defined(PA_COMMUNICATION) || defined(FF_COMMUNICATION)
        ResetToDefaultsStack();  // Now reset parameters of the communication stack

        (void)Format(FIDX_STACK_STATIC_UNCOMMON_REPLACE,NV_SYNC);

        (void)Save(ALL_FILES,NV_SYNC);

#endif
        //5. reset
        elettronicServices.ExitSRV();

    }
    else
    {
        if(result == (TUSIGN16)OK)
        {
           initiallizeNv = ELETTRONICSERVICES_NV_NOT_INITIALIZE;
           result = elettronicServices_Put((TUSIGN16)ELETTRONICSERVICES_IDX_initializeNv,(TINT16)WHOLE_OBJECT,&initiallizeNv);
        }    
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  
\if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
\author				Roberto Colombo
\date				2008-10-10
\param void
\return void
\warning
\test
test-date:			12-05-2009
\n by:				Ganapathi R
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
#ifdef HART_COMMUNICATION
    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    TUSIGN16 result = OK;
    TUSIGN8 diagnosisSimulationStatus;
    TUSIGN8 setAlarm = SET_ALARM;
    TUSIGN8 clearAlarm = CLEAR_ALARM;
    TUSIGN8 setDiagnosis;
    TUSIGN8 clearDiagnosis;
    TINT16  i=0;
    result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,&diagnosisSimulationStatus);

    if(diagnosisSimulationStatus != esDiagnosisSimulationStatus)
    {
        if(diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED)
        {
            esOldDiagnosisSimulation = esOldDiagnosis;
            esOldDiagnosis = 0;
        }
        else
        {
            esOldDiagnosis = esOldDiagnosisSimulation;
            esOldDiagnosisSimulation = 0;
        }

        esDiagnosisSimulationStatus = diagnosisSimulationStatus;
    }
    else
    {
    }
    if((diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED) && (result == (TUSIGN16)OK))
    {
        TUSIGN8 alarmSimulation = 0;

        for(i=0; i<ELETTRONICSERVICES_ALARM_CODES; i++)
        {
            TUSIGN8 alarmBitSimulation = 0;

            result = elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_electronicServicesAlarmSimulation, i, &alarmBitSimulation);

            if((result == (TUSIGN16)OK) && (alarmBitSimulation == SET_ALARM))
            {
                alarmSimulation |= 1<<i;
            }
            else
            {	
            }
        }

        setDiagnosis = alarmSimulation & ~esOldDiagnosisSimulation;

        clearDiagnosis = ~alarmSimulation & esOldDiagnosisSimulation;

        esOldDiagnosisSimulation = alarmSimulation;

    }
    else
    {
        //TUSIGN8 inputDiangosis;
        TUSIGN8 inputDiangosis=0;  //edit by Hao.wu change from (TUSIGN8 inputDiangosis;) It has a defect

        result = elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&inputDiangosis);

        setDiagnosis = inputDiangosis & ~esOldDiagnosis;
        clearDiagnosis = ~inputDiangosis & esOldDiagnosis;

        esOldDiagnosis = inputDiangosis;
    }

    for(i=0; i<ELETTRONICSERVICES_ALARM_CODES; i++)
    {
        if(setDiagnosis & (1<<i))
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarm,i,&setAlarm);
        }
        else if(clearDiagnosis & (1<<i))
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_elettronicservicesAlarm,i,&clearAlarm);
        }
        else
        {
        }
    }
#endif
}

#ifdef PUSH_BUTTON
//-------------------------------------------------------------------------------------------------
/*!
\author				Roberto Colombo
\date				2008-10-10
\param void
\return void
\warning
\test
test-date:			12-05-2009
\n by:				Ganapathi R
\n environment:		IAR-CSPY
\n intention:			Code coverage 100%
\n result module test: 	OK
\n result Lint Level 3:	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void PushButtonsSRV_ELETTRONICSERVICES(void)
{
    if(servicePort.ServPortGetActivationStatusSRV())
    {
        esPushButtonsResult = -1;
    }

    switch(pbStatus)
    {
    case PB_IDLE:
        if(esPushButtonsResult == -1)
        {
            if(PUSH_BUTTON_ZERO_IS_ON && PUSH_BUTTON_SPAN_IS_OFF)
            {
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbPushedButton = PB_ZERO;
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbStatus = PB_PUSH_CHECK;
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                pbTimeStart = esTime;
            }
            else if(PUSH_BUTTON_ZERO_IS_OFF && PUSH_BUTTON_SPAN_IS_ON)
            {
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbPushedButton = PB_SPAN;
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbStatus = PB_PUSH_CHECK;
                //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
                pbTimeStart = esTime;
            }
            else
            {
            }
        }
        else
        {
        }
        break;


    case PB_PUSH_CHECK:
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        if((((pbPushedButton == PB_ZERO) && PUSH_BUTTON_ZERO_IS_ON) ||((pbPushedButton == PB_SPAN) && PUSH_BUTTON_SPAN_IS_ON) )  && !(PUSH_BUTTON_ZERO_IS_ON && PUSH_BUTTON_SPAN_IS_ON))
        {
            //lint -e{737} Ashwin Herur R 2009-07-14 Loss of sign ok
            if((esTime - pbTimeStart) >= PUSH_BUTTONS_ACTIVATION)
            {
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbStatus = PB_EXECUTE_OPERATION;
            }
        }
        else
        {
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            pbStatus = PB_IDLE;
        }
        break;



    case PB_EXECUTE_OPERATION:
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        //lint -e{731} Ashwin Herur R 2009-07-14 Ok
        if((((pbPushedButton == PB_ZERO) && PUSH_BUTTON_ZERO_IS_OFF) ||((pbPushedButton == PB_SPAN) && PUSH_BUTTON_SPAN_IS_OFF))  && ((PUSH_BUTTON_ZERO_IS_ON || PUSH_BUTTON_SPAN_IS_ON) == 0))
        {
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted

#ifdef UNKNOWN_REQUIREMENTS_PUSH_BUTTON
            TUSIGN16 result = OK;
            TUSIGN8 localOperEnable;
            (void)hmi_Get(HMI_IDX_localOperationEnable,WHOLE_OBJECT,&localOperEnable);
            //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            if(esWriteProtection == ELETTRONICSERVICES_WRITE_PROTECTION_OFF && localOperEnable == HMI_LOCAL_OP_ENABLED)
            {
#ifdef HART_COMMUNICATION
                TUSIGN8 mapping;

                mapper_Get(MAPPER_IDX_primaryValueMapping,WHOLE_OBJECT,&mapping);

                if(DIP_SWITCH_3_IS_ON)
                {
                    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                    if(pbPushedButton == PB_ZERO)
                    {
                        switch(mapping)
                        {
                        case MAPPER_PV_P_DP:
                            result = p_dP.PdPSetBiasSRV(0.0f,KILOPASCAL);
                            break;

                        case MAPPER_PV_STATIC_PRESSURE:
                            result = p_dP.StaticPressureSetBiasSRV(0.0f,MEGAPASCAL);
                            break;
#ifdef MV
                        case MAPPER_PV_LEVEL:
                            result = flow_Put(FLOW_IDX_flowSetLevelToZero,WHOLE_OBJECT,&mapping);// Put to Action Object
                            break;
#endif
                        default:
                            result = RULE_VIOLATION_ERR;
                            break;
                        }
                    }
                    else
                    {
                        switch(mapping)
                        {
                        case MAPPER_PV_P_DP:
                            result = p_dP.PdPResetBiasSRV();
                            break;

                        case MAPPER_PV_STATIC_PRESSURE:
                            result = p_dP.StaticPressureResetBiasSRV();
                            break;
#ifdef MV
                        case MAPPER_PV_LEVEL:
                            result = flow_Put(FLOW_IDX_flowResetLevelOffset,WHOLE_OBJECT,&mapping);// Put to Action Object
                            break;
#endif
                        default:
                            result = RULE_VIOLATION_ERR;
                            break;
                        }
                    }

                }
                else
                {
                    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                    if(pbPushedButton == PB_ZERO)
                    {
                        switch(mapping){

                                case MAPPER_PV_P_DP:
                                    result = p_dP.PdPWetZeroRangingEXE(0.0f);
                                    break;

                                case MAPPER_PV_STATIC_PRESSURE:
                                    result = p_dP.StaticPressureWetZeroRangingEXE(0.0f);
                                    break;

#ifdef MV
                                case MAPPER_PV_MASS_FLOW:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_MASS_FLOW, 0.0f);
                                    break;

                                case MAPPER_PV_VOLUME_FLOW:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_VOLUME_FLOW, 0.0f);
                                    break;

                                case MAPPER_PV_HEAT_TRANSFER_RATE:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_HEAT_TRANS_RATE, 0.0f);
                                    break;

                                case MAPPER_PV_LEVEL:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_LEVEL, 0.0f);
                                    break;

                                case MAPPER_PV_VOLUME:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_VOLUME, 0.0f);
                                    break;

                                case MAPPER_PV_MASS:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_MASS, 0.0f);
                                    break;

                                case MAPPER_PV_PROCESS_TEMPERATURE:
                                    result = t_Proc.DryZeroRangingEXE(0.0f);
                                    break;
#endif
                                default:
                                    result = RULE_VIOLATION_ERR;
                                    break;
                            }

                        }else{

                            switch(mapping){

                                case MAPPER_PV_P_DP:
                                {
                                    TUSIGN8 spanCheck = eTRUE;
                                    p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                                    result = p_dP.PdPWetSpanRangingEXE(100.0f);
                                    spanCheck = eFALSE;
                                    p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                                }

                                case MAPPER_PV_STATIC_PRESSURE:
                                {
                                    TUSIGN8 spanCheck = eTRUE;
                                    p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                                    result = p_dP.StaticPressureWetSpanRangingEXE(100.0f);
                                    spanCheck = eFALSE;
                                    p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                                }

#ifdef MV
                                case MAPPER_PV_MASS_FLOW:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_MASS_FLOW, 100.0f);
                                    break;

                                case MAPPER_PV_VOLUME_FLOW:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_VOLUME_FLOW, 100.0f);
                                    break;

                                case MAPPER_PV_HEAT_TRANSFER_RATE:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_HEAT_TRANS_RATE, 100.0f);
                                    break;

                                case MAPPER_PV_LEVEL:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_LEVEL, 100.0f);
                                    break;

                                case MAPPER_PV_VOLUME:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_VOLUME, 100.0f);
                                    break;

                                case MAPPER_PV_MASS:
                                    result = flow.WetZeroRangingEXE(FLOW_ID_MASS, 100.0f);
                                    break;

                                case MAPPER_PV_PROCESS_TEMPERATURE:
                                    result = t_Proc.DryZeroRangingEXE(100.0f);
                                    break;
#endif
                                default:
                                    result = RULE_VIOLATION_ERR;
                                    break;
                            }
                        }
                    }

#else

                    if(DIP_SWITCH_3_IS_ON)
                    {
                        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                        if(pbPushedButton == PB_ZERO)
                        {
                            result = p_dP.PdPWetZeroRangingEXE(0.0f);
                        }
                        else
                        {
                            TUSIGN8 spanCheck = eTRUE;
                            p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                            result = p_dP.PdPWetSpanRangingEXE(100.0f);
                            spanCheck = eFALSE;
                            p_dP_Put(PDP_IDX_spanCheck, WHOLE_OBJECT, &spanCheck);
                        }

                    }
                    else
                    {
                        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                        if(pbPushedButton == PB_ZERO)
                        {
                            result = p_dP.PdPSetBiasSRV(0.0f,KILOPASCAL);
                        }
                        else
                        {
                            result = p_dP.PdPResetBiasSRV();
                        }
                    }
#endif
                }
                else
                {
                    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                    result = READ_ONLY_ERR;
                }


                switch(result)
                {
                    case OK:
                        esPushButtonsResult = 0;
                        break;

                    case APPLIED_PROCESS_TOO_LOW_ERR:
                        esPushButtonsResult = 1;
                        break;

                    case APPLIED_PROCESS_TOO_HIGH_ERR:
                        esPushButtonsResult = 2;
                        break;

                    case LOWER_RANGE_TOO_LOW_ERR:
                        esPushButtonsResult = 3;
                        break;

                    case LOWER_RANGE_TOO_HIGH_ERR:
                        esPushButtonsResult = 4;
                        break;

                    case UPPER_RANGE_TOO_LOW_ERR:
                        esPushButtonsResult = 5;
                        break;

                    case UPPER_RANGE_TOO_HIGH_ERR:
                        esPushButtonsResult = 6;
                        break;

                    case SPAN_TO_SMALL_ERR:
                        esPushButtonsResult = 7;
                        break;

                    case SPAN_TO_SMALL:
                        esPushButtonsResult = 8;
                        break;

                    case READ_ONLY_ERR:
                        esPushButtonsResult = 9;
                        break;

                    case NEW_LRV_URV_OUTSIDE_SENSOR:
                        esPushButtonsResult = 11;
                        break;

                    default:
                        esPushButtonsResult = 10;
                        break;

                }
#else //UNKNOWN_REQUIREMENTS_PUSH_BUTTON
                if(pbPushedButton == PB_ZERO)
                {
                    ButtonZeroReleaseFunction();
                }
                if(pbPushedButton == PB_SPAN)
                {
                    ButtonSpanReleaseFunction();
                }
#endif
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbPushedButton = PB_NULL;
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                pbStatus = PB_IDLE;
                break;
            }

    //lint -e{744} Ashwin Herur R 2009-07-14 Default not needed
    }
}
#endif /* #ifdef PUSH_BUTTON */


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CYCLICASYNCSAVESRV> \endif
 \brief  Cyclic variables Asyncronous Save on Eeprom
 \if @SubGen end@ <METHODHEADER CYCLICASYNCSAVESRV> \endif
 \author				Roberto Colombo
 \date				2008-10-10
 \param void
 \return void
 \warning
 \test
 test-date:			12-05-2009
 \n by:				Ganapathi R
 \n environment:		IAR-CSPY
 \n intention:			Code coverage 100%
 \n result module test: 	OK
 \n result Lint Level 3:	OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CYCLICASYNCSAVESRV>
void CyclicAsyncSaveSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD CYCLICASYNCSAVESRV>
{       //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
    TUSIGN32 time = esTime;
    TUSIGN8 inputDiangosis = 0;

    esAsynSaveCount += (time -esAsynSaveTimeOld);


    if(esAsynSaveCount > CYCLIC_ASYNC_SAVE_12_MIN)
    {       
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        (void)Save(FIDX_CYCLIC_COMMON_REPLACE,NV_ASYNC);

        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        (void)Save(FIDX_CYCLIC_CB,NV_ASYNC);
        esAsynSaveCount = 0;
    }

    esAsynSaveTimeOld = time;
    
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&inputDiangosis);  
    
    if((inputDiangosis & (1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW))|| \
         (inputDiangosis & (1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH)))
    {
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        if(esVsupplyFlag == 1)
        {       
            (void)Save(FIDX_CYCLIC_COMMON_REPLACE,NV_ASYNC);
            esVsupplyFlag = 0;
            esVsupplySaveCount = 0;
        }
        else
        {
            esVsupplySaveCount ++;
            if(esVsupplySaveCount>CYCLIC_VSUPPLY_FAULT_LOOP)
            {
                (void)Save(FIDX_CYCLIC_COMMON_REPLACE,NV_ASYNC);
                esVsupplySaveCount = 0;
            }              
        }  
    }
    else
    {
        esVsupplyFlag = 1;
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER ELETTORNICSERVICESEXE> \endif
 \brief  Perform the elettronic services
 \if @SubGen end@ <METHODHEADER ELETTORNICSERVICESEXE> \endif
 \author				Roberto Colombo
 \date				2007-04-12
 \param void
 \return void
 \warning
 \test
 test-date:			12-05-2009
 \n by:				Ganapathi R
 \n environment:		IAR-CSPY
 \n intention:			Code coverage 100%
 \n result module test: 	OK
 \n result Lint Level 3:	OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD ELETTORNICSERVICESEXE>
void ElettornicServicesEXE_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD ELETTORNICSERVICESEXE>
{
    static TUSIGN8 esTaskCnt_duplicated = esTaskCntInit;
    TFLOAT tmpValue = 0.0f;

    VIP_ASSERT(esTaskCnt == esTaskCnt_duplicated);
    CheckFlag((TUSIGN16)WP_FLAG );
    SwWriteProtection_ElectronicServices();
    SetFlag((TUSIGN16)WP_FLAG );

    TMP112_ReadTemp_ElectronicServices(&tmpValue);
    ADC1_Start_ElectronicServices();
    ADC2_Start_ElectronicServices();

#ifdef HART_COMMUNICATION
    CheckFlag((TUSIGN16)COREAD_FLAG );
#ifdef NDEBUG    
    /* when hart is not in transmitting state,check TP9 voltage for monitor 2.5V*/
    if(P6_bit.P6_1 == 1) 
    {
        elettronicServices.CheckTP9Voltage();
    }
#endif
	SetFlag((TUSIGN16)COREAD_FLAG );

    CheckFlag((TUSIGN16)RESET_FLAG );
    DeviceResetSRV_ELETTRONICSERVICES();
    SetFlag((TUSIGN16)RESET_FLAG );

    switch (esTaskCnt)
    {
    case 0:
       CheckFlag((TUSIGN16)CYCLE_FLAG );
       RamCheck_ElectronicServices(CYCLIC); //one time  modified by hao wu- according to paul  2013-01-17
       SetFlag((TUSIGN16)CYCLE_FLAG );
       break; 
                    
    case 1:
	case 2:
	case 3:
	case 10:
		CheckFlag((TUSIGN16)CYCLE_FLAG );
		RomCheck_ElectronicServices(); //three time  modified by hao wu- according to paul  2013-01-17
		SetFlag((TUSIGN16)CYCLE_FLAG );
		break;

    case 5:    
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        CyclicAsyncSaveSRV_ELETTRONICSERVICES();
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;

    case 6:     
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        VsupplyReadBack_ElectronicServices();
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;

    case 7:  
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        CheckCrcData_ElectronicServices();
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;

    case 8:                  
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        CheckDynamicVariables_ElectronicServices();
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;
        
    case 9:                 
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        CheckCpuAsm_ELETTRONICSERVICES();
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;

    default:  
        CheckFlag((TUSIGN16)CYCLE_FLAG );
        esTaskCnt = 0;
        esTaskCnt_duplicated = 0;
        SetFlag((TUSIGN16)CYCLE_FLAG );
        break;
    }

    if(esTaskCnt == 10)
    {
        esTaskCnt = 0;
    }
    else
    {
        esTaskCnt++;
    }

    if(esTaskCnt_duplicated == 10)
    {
        esTaskCnt_duplicated = 0;
    }
    else
    {
        esTaskCnt_duplicated++;
    }

    //elettronicServices.PushButtonsSRV();//add push_button SRV in ElettornicServicesEXE
    CheckFlag((TUSIGN16)LOADROM_FLAG );
    elettronicServices.LoadRomDefaultSRV();
    SetFlag((TUSIGN16)LOADROM_FLAG );
        
    CheckFlag((TUSIGN16)RESETDEFAULT_FLAG );
    elettronicServices.ResetToDefaultSRV();
    SetFlag((TUSIGN16)RESETDEFAULT_FLAG );
    
    CheckFlag((TUSIGN16)INITNV_FLAG );
    if(elettronicServices.InitializeNvCheckSRV()== ELETTRONICSERVICES_NV_INITIALIZE_START)
    {
        elettronicServices.InizializeNvSRV();
    }
    SetFlag((TUSIGN16)INITNV_FLAG );
#else
    CheckFlag((TUSIGN16)CYCLESAVE_FLAG );
    CyclicAsyncSaveSRV_ELETTRONICSERVICES();
    SetFlag((TUSIGN16)CYCLESAVE_FLAG );
#endif
    
    CheckFlag((TUSIGN16)SIZEOF_FLAG );
    ESSequenceFlag = 0;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER EXITSRV> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER EXITSRV> \endif
 \author				Roberto Colombo
 \date				2008-10-10
 \param void
 \return void
 \warning
 \test
 test-date:			12-05-2009
 \n by:				Ganapathi R
 \n environment:		IAR-CSPY
 \n intention:			Code coverage 100%
 \n result module test: 	OK
 \n result Lint Level 3:	OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD EXITSRV>
void ExitSRV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD EXITSRV>
{
#ifndef LOCALSERVER_SELECT
    ResetAction();
#endif
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


//------------------------------------------------------------------------------------------------
/*
 \author	Roberto Colombo
 \date
 \brief     When write protection is set from OFF to ON, other subsystem's crc dataclass will
            be called as protectedDataStatus set to CURR_CALCULATE then CURR_PROTECTED
 \param void
 \return void
 \warning
 \test
 test-date: 200x-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
 \version   XuFei   In 2-Wire and VT5, there is no PUSH_BUTTON_WRITE_PROTECT but DIP_SWITCH_1_IS_ON
                    is used as write ptotection switch.
*/
//-----------------------------------------------------------------------------------------------
static void SwWriteProtection_ElectronicServices(void)
{
#ifndef LOCALSERVER_SELECT
    //lint -e{830} hao wu,	2013-01-15 Ok
    TUSIGN8 writeProtection = elettronicServicesReplaceStaticFrequentNoDefault.swWriteProtection || PUSH_BUTTON_WRITE_PROTECT_IS_ON;
    if(esWriteProtection != writeProtection)
    {
#ifdef HART_COMMUNICATION
        (void)hart_Put((TUSIGN16)HART_IDX_hwWriteProtection,(TINT16)WHOLE_OBJECT,&writeProtection);
#endif
		(void)arm_Put((TUSIGN16)ARM_IDX_readOnlySwitchStatus,(TINT16)WHOLE_OBJECT,&writeProtection);  //add: 2013-01-21 changed the code for software write protection.
	}

    esWriteProtection = writeProtection;
#endif
}

//------------------------------------------------------------------------------------------------
/*
 brief		Save as default
 \author		Roberto Colombo
 \date		25-06-2009
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
*/
//------------------------------------------------------------------------------------------------
TUSIGN16 SaveAsDefault_ElectronicServices(TINT16 subIdx)
{       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    TUSIGN16 result = OK;
    TUSIGN16 idx, startIdx, endIdx;
    const T_UNIT SLOW* unitPtr = 0;

    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    if(subIdx == WHOLE_OBJECT)
    {
        //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
        startIdx = 0;
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
        endIdx = MEMBER_IDX_NUM_ELEMENTS;
    }
    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    else if((subIdx > WHOLE_OBJECT) && (subIdx < MEMBER_IDX_NUM_ELEMENTS))
    {
        //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
        startIdx = subIdx;
        //lint -e{732} Ashwin Herur R 2009-07-14 Loss of sign is Ok
        endIdx = subIdx + 1;
    }
    else
    {
        return RULE_VIOLATION_ERR;
    }

    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    for(idx=startIdx; (idx<endIdx) && (result == OK); idx++)
    {
        unitPtr = GetSubsystemPtr(idx);

        result = unitPtr->SaveAsDefault(unitPtr,ALL_DATACLASSES);
    }

    return result;
}

#ifdef HART_COMMUNICATION
//------------------------------------------------------------------------------------------------
/*
 \author	Roberto Colombo
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static void CheckCrcData_ElectronicServices(void)
{
    sVMeasurement.CheckDataCrcSRV();
    mVMeasurement.CheckDataCrcSRV();
    elettronicServices.CheckDataCrc();
    coordinator.CheckDataCrc();
    mapper.CheckDataCrc();
}


//------------------------------------------------------------------------------------------------
/*
 \author	Roberto Colombo
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static void VsupplyReadBack_ElectronicServices(void)
{
    TUSIGN8 	inputDiangosis = 0;
    TUSIGN16 	vsupply = 0;
    TINT8		esSupplyVoltReadbackFailCnt = 0;

    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&inputDiangosis);

    if(elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackEnable)
    {
        /* vsupply is already read in main task */
        (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_vSupplyReadBack,(TINT16)WHOLE_OBJECT,&vsupply);

        //Low or High threshold
        if(vsupply < elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackLowThreshold)
        {
            if(esSupplyVoltReadbackFailCntLow <= elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackRetry)
            {
                esSupplyVoltReadbackFailCntLow+=2;
            }
            if(esSupplyVoltReadbackFailCntHigh >0)
            {
                esSupplyVoltReadbackFailCntHigh -= 1;
            }
        }
        else if(vsupply > elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackHighThreshold)
        {
            if(esSupplyVoltReadbackFailCntHigh <= elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackRetry)
            {
                esSupplyVoltReadbackFailCntHigh+=2;
            }
            if( esSupplyVoltReadbackFailCntLow > 0)
            {
                esSupplyVoltReadbackFailCntLow -= 1;
            }
        }
        else
        {
            if( esSupplyVoltReadbackFailCntLow > 0)
            {
                esSupplyVoltReadbackFailCntLow -= 1;
            }
            if(esSupplyVoltReadbackFailCntHigh >0)
            {
                esSupplyVoltReadbackFailCntHigh -= 1;
            }
        }
    }
    else
    {
        esSupplyVoltReadbackFailCntLow = 0;
        esSupplyVoltReadbackFailCntHigh = 0;
    }

    esSupplyVoltReadbackFailCnt = esSupplyVoltReadbackFailCntLow + esSupplyVoltReadbackFailCntHigh;

    if(esSupplyVoltReadbackFailCnt > elettronicServicesReplaceStaticFrequentNoDefault.vSupplyReadbackRetry)
    {
        if(esSupplyVoltReadbackFailCntLow >= esSupplyVoltReadbackFailCntHigh)
        {
            inputDiangosis |= (1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW);
            inputDiangosis &= ~(1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH);
        }
        else
        {
            inputDiangosis |= (1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH);
            inputDiangosis &= ~(1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW);
        }
    }
    else if(esSupplyVoltReadbackFailCnt == 0)
    {
        inputDiangosis &= ~(1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_LOW);
        inputDiangosis &= ~(1<<ELETTRONICSERVICES_ALARM_POWER_SUPPLY_TOO_HIGH);
    }
    else
    {
    }

    (void)elettronicServices_Put((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&inputDiangosis);

}

//------------------------------------------------------------------------------------------------
/*
 \author
 \date
 \param void
 \return TUSIGN16 Vsupply
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static TUSIGN16 VsupplyMonitor(void)
{
    TUSIGN32 tmpU32;
    TUSIGN16 raw_ADC_Value;
    TUSIGN32 filtered_ADC_Value;
    TUSIGN32 ADC_ValueIn_mv;
    TUSIGN32 Vsupply;

    //Read ADC value.
    raw_ADC_Value = MeasureAdc();

    //Filter the ADC value.
    filtered_ADC_Value = Filter(raw_ADC_Value);

    //Convert ADC value to mV reading.
    ADC_ValueIn_mv = VSUPP_MANAGER_VADC_mV((TUSIGN32)filtered_ADC_Value);

    //Calculate the supply voltage, based on the resistor network.
    //Please see the schematics for details.
    //Old board gain
    //tmpU32 = (ADC_ValueIn_mv * R100K)/ (R100K + R100K);

    //New board gain
    tmpU32 = ADC_ValueIn_mv;

    Vsupply = (TUSIGN32)(tmpU32 * SUPPLY_VOLT_READBACK_COEFF);

    return(((TUSIGN16)Vsupply));
}


//------------------------------------------------------------------------------------------------
/*
 \author
 \date
 \param void
 \return TUSIGN16 
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
 \version
*/
//------------------------------------------------------------------------------------------------
static TUSIGN16 MeasureAdc(void)
{
#if 0
    TUSIGN16 tmpU16;
    TUSIGN8 loopcnt=0;

#if ( ADC_CLOCK_FREQ!=1843200 )
#error timing has to be adjusted!
#endif

    ADIC_bit.IR = 0;
    // P2.5 must be input.
    PD2_bit.PD2_5 = 0;
    // ADC register settings.
    ADCON0 = ADC_CON0_SETTING;
    ADCON1 = ADC_CON1_SETTING; // fad=f1/4  ,f1=f/2 add by hao wu
    ADCON2 = ADC_CON2_SETTING;
    // wait at least 1|��s
    asm("nop");
    asm("nop");

    // ADC_VREF_CONNECTED had to be set more than 1|��s before
    ADCON0_bit.ADST = 1;

    //lint -e{960} 2013-02-26 && is ok
    while((ADIC_bit.IR != 1) && (loopcnt++ < 50))   //// fAD=f1=f/2,  fcpu=f/2, min conversion time=43 fAD, 50 is enough for timeout   added by Hao Wu  2013-01-29
    {
        //wait for the convertion ends
    }

    tmpU16 = AD5;

    // switch off ADCs Power consumption immediately
    ADCON1_bit.ADSTBY = 0;
    ADIC_bit.IR = 0;
    return(tmpU16);
#endif
    return 0;
}

//------------------------------------------------------------------------------------------------
/*
 \author
 \date
 \param void
 \return TUSIGN16
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static TUSIGN16 Filter(TUSIGN16 value)
{
    static TUSIGN16 buffer[8] = { 0,0,0,0,0,0,0,0 };
    static TUSIGN32 filteredValue = 0;
    static TUSIGN8  idx = 0xff;
    static TUSIGN16 returnVal;

    // init
    if(idx==0xff)
    {
        for(idx=0; idx<8; idx++)
        {
            buffer[idx] = value;
        }
        filteredValue = (TUSIGN32)(value)*8;
        idx = 0;
    }
    else
    {
        filteredValue -= buffer[idx];
        buffer[idx] = value;
        filteredValue += buffer[idx];

        idx++;
        idx &= 0x07;
    }

    returnVal = (TUSIGN16)(filteredValue >> 3);
    return(returnVal);
}

//------------------------------------------------------------------------------------------------
/*
 \brief		Crc Initialization for Rom Check.
             When Electronic Services is initialized, memory crc is read.
 \author		Roberto Colombo
 \date		30-07-2008
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
void RomCheckInitCrc_ElectronicServices(void)
{
    esRomTst.ptrAdrs = (TUSIGN8 HUGE*)(ROM_BASE_ADDRESS);
    esRomTst.crcMem = *(esRomTst.ptrAdrs++);
    esRomTst.crcMem |= *(esRomTst.ptrAdrs++)<<8;
    esRomTst.block = 0;
}

//------------------------------------------------------------------------------------------------
/*
 brief		Startup Rom Check
          Time to check rom1 excluding last 64k(0x80002-0xEFFFB, 448k):
            sw table:  24s
            hw crc:    24s (time is almost same)
 \author		Roberto Colombo
 \date		19-12-2008
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
void RomCheckStartup_ElectronicServices(void)
{
    //ROM_BASE_ADDRESS + 2 --> ROM_END_ADDRESS
    esRomTst.crc16 = CalcCrc16hugeMem_COMMON(InitCRC16_COMMON(), (TUSIGN8 HUGE*)(ROM_BASE_ADDRESS+2),(TUSIGN8 HUGE*)(ROM_END_ADDRESS));

    //Assembly-No Hw
    //esRomTst.crc16 = CalcCrc16hugeMem_COMMON(InitCRC16_COMMON(), (TUSIGN8 HUGE*)(ROM_BASE_ADDRESS + 2),(TUSIGN8 HUGE*)(ROM_END_ADDRESS));

#ifdef NDEBUG
    if(esRomTst.crc16 != esRomTst.crcMem)
    {
        ExitSRV_ELETTRONICSERVICES();
        //lint -e{716} Ashwin Herur R 2009-07-13 Ok
        //lint -e{722} Ashwin Herur R 2009-07-13 Ok
        //lint -e{960} Ashwin Herur R 2009-07-13 Ok
        while(1);
    }
    
#endif
}

//------------------------------------------------------------------------------------------------
/*
 brief
 \author		Roberto Colombo
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 73.81% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static TFLOAT TemperatureElettronicMeasurement_ElectronicServices(void)
{
    static TINT8 errorTempCountHigh = 0;
    static TINT8 errorTempCountLow = 0;
    TINT8 errorTempCount = 0;

    TUSIGN8 i = 0;
    TFLOAT temperature = 0.0f;
    TFLOAT temperaturenew = 0.0f;
    TFLOAT temperatureMax = 0.0f;
    TFLOAT temperatureMin = 0.0f;
    TUSIGN8 localDiagnosis = 0;
    TUSIGN8 loopCnt=0;
    TFLOAT voltage = 0.0f;

    TUSIGN8 SimMode = 0;
    (void)coordinator_Get((TUSIGN16)CDT_IDX_simulationMode,0,&SimMode);
    if(SimMode == CDT_SIMULATION_CB_AMBIENT_TEMP)      //simulation data mode
    {
        // get the base unit sim temperature value(celsius)
        (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation,(TINT16)WHOLE_OBJECT,&temperature);
    }
    else
    {
        //use hardware LM20 to get the temperature
        ADIC_bit.IR = 0;
        PD2_bit.PD2_6 = 0;				//Set PD2 direction as input

        PRCR_bit.PRC0 = 1;
        PCLKSTP1_bit.PCKSTP14 = 0;      //0: f1 provide enabled
        PRCR_bit.PRC0 = 0;

        ADCON2_bit.CKS3 = 0;            //fAD = f1

        ADCON0_bit.CH0 = 0;
        ADCON0_bit.CH1 = 1;
        ADCON0_bit.CH2 = 1;				//AN6
        ADCON0_bit.MD0 = 0;
        ADCON0_bit.MD1 = 0;				//One-shot mode
        ADCON0_bit.TRG = 0;				//Software are trigger
        ADCON0_bit.ADST = 0;			//A/D conversion disable
        ADCON0_bit.CKS0 = 0;

        ADCON1_bit.SCAN0 = 0;
        ADCON1_bit.SCAN1 = 0;			//Invalid in One-shot mode
        ADCON1_bit.MD2 = 0;				//0 if One-shot mode is selected
        ADCON1_bit.CKS1 = 1;			//One-shot mode
        ADCON1_bit.ADSTBY = 1;          //enabled, new in m16c63

        ADCON2_bit.ADGSEL0 = 1;
        ADCON2_bit.ADGSEL1 = 1;			//P2 selected
        ADCON2_bit.CKS2 = 0;			//If fAD is divided by 2 or 4

        for(i = 0; i< 10; i++)
        {
            /* Wait over A/D 1cycle after A/D converter ready */
        }
        ADCON0_bit.ADST = 1;			//A/D conversion enable

        /*
        In one-shot mode and single sweep mode, use the IR bit in the ADIC register to detect completion of
        A/D conversion. When not using an interrupt, set the IR bit to 0 by a program after detection.
        */
        //lint -e{960} 2013-02-26 && is ok
        while((ADIC_bit.IR != 1) && ((loopCnt++) < 50))   // fAD=f1=f/2,  fcpu=f/2, min conversion time=43 fAD, 50 is enough for timeout   added by Hao Wu  2013-01-29
        {
            //wait for the convertion ends
        }

        // switch off ADCs Power consumption immediately
        ADCON1_bit.ADSTBY = 0;                //ADSTBY


        voltage = (AD6 * 2.5f)/1023.0f;     //VREF=2.5V, 10bit A/D

        temperature = -1481.96f + (TFLOAT)sqrt((2.1962*1000000)+((1.8639-voltage)/(3.88*0.000001))); //LM20
        //  temperature from AD always bases on celsius.          
        (void)elettronicServices_Put((TUSIGN16)ELETTRONICSERVICES_IDX_acttelettronicTemperature, (TINT16)WHOLE_OBJECT , &temperature);
    }
    ADIC_bit.IR = 0;    //set the IR bit to 0 (interrupt not requested) by a program after reading the ADi register.
     //  put the base unit temperature value  
    (void)elettronicServices_Put((TUSIGN16)ELETTRONICSERVICES_IDX_elettronicTemperature, (TINT16)WHOLE_OBJECT , &temperature);
    //duplicated data;
    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
    elettronicServicesDynamicDuplicated.elettronicTemperatureDuplicated = elettronicServicesDynamicDuplicated.elettronicTemperature;
    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));


    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&localDiagnosis);
    //add alarm rang check
    //alarmTRng_Obj same unit_code with elettronicTemperature_Obj
    //For device to integrate, shall define the rang value for itself.
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_alarmTRng_Obj,(TINT16)UOM_PF_value_0,&temperatureMax);
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_alarmTRng_Obj,(TINT16)UOM_PF_value_1,&temperatureMin);
    // add 2013-06-18, for the compare need tobe based on same unit code;
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_elettronicTemperature_Obj,(TINT16)UOM_PF_value_0,&temperaturenew);
    if(temperaturenew > temperatureMax)
    {
        if(ELETTRONICSERVICES_TEMPERATURE_READBACK_RETRY > errorTempCountHigh)
        {
            errorTempCountHigh+=2;
        }
        if(errorTempCountLow > 0)
        {
            errorTempCountLow -= 1;
        }
    }
    else if(temperaturenew < temperatureMin)
    {
        if(ELETTRONICSERVICES_TEMPERATURE_READBACK_RETRY > errorTempCountLow)
        {
            errorTempCountLow+=2;
        }
        if(errorTempCountHigh > 0)
        {
            errorTempCountHigh -= 1;
        }
    }
    else
    {
        if(errorTempCountHigh > 0)
        {
            errorTempCountHigh -= 1;
        }
        if(errorTempCountLow > 0)
        {
            errorTempCountLow -= 1;
        }
    }
    errorTempCount = errorTempCountHigh + errorTempCountLow ;
    if(errorTempCount > ELETTRONICSERVICES_TEMPERATURE_READBACK_RETRY)
    {
        if(errorTempCountHigh < errorTempCountLow)
        {
            localDiagnosis &= ~(1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH);
            localDiagnosis |= (1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW);
        }
        else
        {
            localDiagnosis |= (1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH);
            localDiagnosis &= ~(1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW);
        }
    }
    else if(errorTempCount == 0)
    {
        localDiagnosis &= ~(1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_HIGH);
        localDiagnosis &= ~(1<<ELETTRONICSERVICES_ALARM_AMBIENT_TEMP_TOO_LOW);	
    }
    else
    {
    }
    (void)elettronicServices_Put((TUSIGN16)ELETTRONICSERVICES_IDX_localDiagnosis,(TINT16)WHOLE_OBJECT,&localDiagnosis);
    return temperature;
}

//------------------------------------------------------------------------------------------------
/*
 brief		    Only check the used RAM,(USER_RAM_BEGIN - USER_RAM_END), so that the time can be reduced
                for a little bit
 \author		Roberto Colombo, Xu Fei
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 92.54% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
void RamCheck_ElectronicServices(TUSIGN8 mode)
{
    TINT16 ramLoop = 0;
    if(elettronicServicesReplaceStaticFrequentNoDefault.ramTestEnable)
    {
        TBOOL error = eFALSE;
        static TBOOL firstTime = eTRUE;


        if(firstTime)
        {
            esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;
            esRamTst.offset = 1;
            esRamTst.addressOffset = esRamTst.address + esRamTst.offset;
            esRamTst.state = ADDRESS_BUS_TEST;
            firstTime = eFALSE;
        }
        do
        {

            switch(esRamTst.state)
            {

                case ADDRESS_BUS_TEST:

                    esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;

                    if((TUSIGN32)(esRamTst.addressOffset) <= USER_RAM_END)
                    {
                        __disable_interrupt();

                        esRamTst.bakup1 = *esRamTst.address;

                        esRamTst.bakup2 = *(TUSIGN16*)(esRamTst.addressOffset);

                        *esRamTst.address = PATTERN;

                        *esRamTst.addressOffset = ANTIPATTERN;

                        if(*(TUSIGN16*)esRamTst.address != PATTERN)
                        {
                            error = eTRUE;
                            break;
                        }

                        *esRamTst.address = esRamTst.bakup1;

                        if(*(TUSIGN16*)(esRamTst.addressOffset) != ANTIPATTERN)
                        {
                            error = eTRUE;
                            break;
                        }

                        *esRamTst.addressOffset = esRamTst.bakup2;

                        __enable_interrupt();

                        esRamTst.offset <<= 1;

                        esRamTst.addressOffset = esRamTst.address + esRamTst.offset;
                    }
                    else
                    {
                        esRamTst.state = DATA_BUS_TEST;
                        esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;
                        esRamTst.walkOnePattern = 0x01;
                    }

                    break;

                case DATA_BUS_TEST:
                    __disable_interrupt();

                    esRamTst.bakup1 = *esRamTst.address;

                    *esRamTst.address = esRamTst.walkOnePattern;

                    if(*esRamTst.address != esRamTst.walkOnePattern) { error = eTRUE;}
                    else
                    {
                    }
                    *esRamTst.address = esRamTst.bakup1;

                    __enable_interrupt();

                    if(esRamTst.walkOnePattern == 0x8000)
                    {
                        esRamTst.state = DATA_RETAIN_TEST;
                        esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;
                    }
                    else
                    {
                        esRamTst.walkOnePattern <<= 1;
                    }
                    break;


                case DATA_RETAIN_TEST:
                    for(ramLoop = 0; (ramLoop<RAM_TEST_LEN) && ((TUSIGN32)esRamTst.address <= RAM_END_ADDRESS) && !error; ramLoop++)
                    {
                        __disable_interrupt();
                        esRamTst.bakup1 = *esRamTst.address;

                        // Write a pattern.
                        *esRamTst.address = PATTERN;

                        error = (*esRamTst.address != PATTERN)?eTRUE:eFALSE;
                        // Now write an antipattern.
                        *esRamTst.address = ANTIPATTERN;
                        //lint -e{514} 2013-02-26 Boolean is ok
                        error |= (*esRamTst.address != ANTIPATTERN)?eTRUE:eFALSE;
                        // Restore the value.
                        *esRamTst.address = esRamTst.bakup1;
                        __enable_interrupt();

                        // Check next location.
                        esRamTst.address++;
                    }


                    if((TUSIGN32)esRamTst.address > USER_RAM_END)
                    {
                        esRamTst.state = ADDRESS_BUS_TEST;
                        esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;
                        esRamTst.offset = 1;
                        esRamTst.addressOffset = esRamTst.address + esRamTst.offset;
                        mode = CYCLIC;
                    }
                    break;

                default:
                    esRamTst.state = ADDRESS_BUS_TEST;
                    esRamTst.address = (TUSIGN16*)USER_RAM_BEGIN;
                    esRamTst.offset = 1;
                    esRamTst.addressOffset = esRamTst.address + esRamTst.offset;
                    mode = CYCLIC;
                    break;

            }
        //lint -e{731} 2013-02-26 Boolean is ok
        }while( (!error) && (mode == START_UP));   // If mode = startup, test all the address location.
#ifdef NDEBUG
        if(error)
        {
            ExitSRV_ELETTRONICSERVICES();
            //lint -e{716}
            //lint -e{960}
            //lint -e{722}
            while(1);
        }
#endif
    }
}

//------------------------------------------------------------------------------------------------
/*
 brief
 \author		Roberto Colombo
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//------------------------------------------------------------------------------------------------
static void RomCheck_ElectronicServices(void)
{
    if(elettronicServicesReplaceStaticFrequentNoDefault.romTestEnable)
    {
        if(esRomTst.block == 0)
        {
            esRomTst.ptrAdrs = (TUSIGN8 HUGE*)(ROM_BASE_ADDRESS + 2);
            esRomTst.blkEndAdrs = (TUSIGN8 HUGE*)(ROM_BASE_ADDRESS + ROM_BLOCK_SIZE);
            esRomTst.crc16 = InitCRC16_COMMON();
        }
        else
        {
            esRomTst.ptrAdrs = esRomTst.blkEndAdrs + 1;

            if(esRomTst.block == (MAX_ROM_BLOCKS - 1))
            {
                esRomTst.blkEndAdrs = (TUSIGN8 HUGE*)(ROM_END_ADDRESS);
            }
            else
            {
                esRomTst.blkEndAdrs = (TUSIGN8 HUGE*)(esRomTst.ptrAdrs + ROM_BLOCK_SIZE);
            }
        }

        esRomTst.crc16 = CalcCrc16hugeMem_COMMON(esRomTst.crc16,esRomTst.ptrAdrs,esRomTst.blkEndAdrs);


        // Increment to next block.
        esRomTst.block++;

        //Break if all blocks are calculated.
        if(esRomTst.block >= MAX_ROM_BLOCKS)
        {
            esRomTst.block = 0;
        }

#ifdef NDEBUG
        if(esRomTst.blkEndAdrs == (TUSIGN8 HUGE*)ROM_END_ADDRESS)
        {
            if(esRomTst.crc16 != esRomTst.crcMem)
            {
                ExitSRV_ELETTRONICSERVICES();
                //lint -e{716} Ashwin Herur R 2009-07-13 Ok
                //lint -e{722} Ashwin Herur R 2009-07-13 Ok
                while(1);
            }
        }

#endif
    }
}
#endif