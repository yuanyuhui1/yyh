//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2010.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem Coordinator
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
#include "System/Interface/microcontroller.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
//Front-end Interface
#include "FrontEndInterface/Interface/FrontEndInterface.h"


#include "Mapper/Source/Mapper_types.h"
#include "Mapper/Source/Mapper_attributes.h"

#include "DigitalOutput/Interface/DigitalOutput.h"

//SVMeasurement
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "SVMeasurement/Source/SVMeasurement_types.h"
#include "SVMeasurement/Source/SVMeasurement_attributes.h"

//MVMeasurement
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Source/MVMeasurement_types.h"
#include "MVMeasurement/Source/MVMeasurement_attributes.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"

//Totalizer
#include "Totalizer/Interface/Totalizer.h"
#include "Totalizer/Interface/Totalizer_idx.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Diagnosis/Source/Diagnosis_mapper.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
//#include "..\..\AnalogInput\interface\analogInput.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"

//#include "..\..\hart\interface\hart.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"

//ServicePort
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Layer1/serviceport_dio.h"
#include "ServicePort/Layer2/serviceport_layer2.h"

#include "Hmi/Interface/HMI.h"

#include "nv_mem/Interface/nv_mem.h"
#include "Coordinator/Interface/nv_startup.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/watchdog.h"
#include "Coordinator/Interface/watchdog_bitelist.h"

#include "coordinator_types.h"
#include "coordinator_attributes.h"
#include "coordinator_execute.h"
#include "coordinator_overload.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Source/Mapper_Measurement.h"
#include "Mapper/Source/DeviceVariables/Mapper_Density.h"
#include "Mapper/Source/DeviceVariables/Mapper_GasContent.h"
#include "Mapper/Source/DeviceVariables/Mapper_Pressure.h"
#include "Mapper/Source/DeviceVariables/Mapper_TExternal.h"

#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"

#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"

#include "Ethernet/Interface/Ethernet.h"
#include "Ethernet/Interface/Ethernet_idx.h"

#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/Modbus_idx.h"

#include "WebServer/Interface/WebServer.h"
#include "WebServer/Source/decryption.h"
#include "WebServer/Source/WebServer_httpRedirect.h"
#include "WebServer/Source/WebServerObjects.h"

#include "Coordinator/Interface/Variable_calculatoin.h"
#include "Coordinator/Interface/TimeElapse.h"
#include "FrontEndInterface/Layer7/FED_layer7.h"
#include "Common/Interface/common_functions.h"
#include "Ethernet/Source/Ethernet_types.h"
#include "Ethernet/Source/Ethernet_attributes.h"
#include "timers.h"
#include "OSPL_task.h"
#include "OSPL_time.h"
#include "OSPL_soft_timer.h"
#include "PCFG_interface.h"
#include "HWPL_board_options_if.h"
#include "HWPL_led.h"
#include "HWPL_time.h"
#include "CI_system_api.h"
#include "IO_network_types.h"
#include "IO_app_info.h"

/* Tasks ID. */
extern RTOS_TASK_HANDLE WatchdogTskHandle;
extern RTOS_TASK_HANDLE MainTskHandle;
RTOS_TASK_HANDLE FrontendTskHandle;
RTOS_TASK_HANDLE SlowTskHandle;
RTOS_TASK_HANDLE HartTskHandle;
RTOS_TASK_HANDLE ServicesTskHandle;
RTOS_TASK_HANDLE ModbusTskHandle;

//--------------------------------------------------------------------------------------------------
// Settings for FreeRTOS
//--------------------------------------------------------------------------------------------------

#define SLOW_TASK_STACK_SIZE                     (2 * RTOS_TASK_STACK_SZMIN)
#define HART_TASK_STACK_SIZE                     (1 * RTOS_TASK_STACK_SZMIN)
#define MODBUS_TASK_STACK_SIZE                   (1 * RTOS_TASK_STACK_SZMIN)
#define SERVICES_TASK_STACK_SIZE                 (2 * RTOS_TASK_STACK_SZMIN)
#define FRONTEND_TASK_STACK_SIZE                 (1 * RTOS_TASK_STACK_SZMIN)
#ifdef CPU_LOAD_TEST
#define CPULOAD_TASK_STACK_SIZE                  (1 * RTOS_TASK_STACK_SZMIN)
#endif

#define FRONTEND_TASK_PRIORITY                   (RTOS_TASK_PRIO_IDLE + 5)
#define SERVICES_TASK_PRIORITY                   (RTOS_TASK_PRIO_IDLE + 4)
#define MODBUS_TASK_PRIORITY                     (RTOS_TASK_PRIO_IDLE + 3)
#define HART_TASK_PRIORITY                       (RTOS_TASK_PRIO_IDLE + 2)
#define SLOW_TASK_PRIORITY                       (RTOS_TASK_PRIO_IDLE + 1)
#ifdef CPU_LOAD_TEST
#define CPULOAD_TASK_PRIORITY                    (RTOS_TASK_PRIO_IDLE)
#endif

extern FAST T_FED_FRAME_RX_BUFFER uReceiveBuffer_FED_layer7;
extern FAST T_FED_FRAME_TX_BUFFER uTransmitBuffer_FED_layer7;

/*============================  D E F I N E S  ============================*/
#define NETWORK_APP_BASE_PRIORITY (4)
#define IO_APP_BASE_PRIORITY (8)

#define IDLE_TASK_TAG         ((void *) 1)
#define NETWORK_APP_TASK_TAG  ((void *) 2)
#define IO_APP_TASK_TAG       ((void *) 3)

#undef CPU_USAGE

#ifdef CPU_USAGE
#include "cpu_utils.h"
#endif

/*===============================  D A T A  ===============================*/
static volatile int32_t naStatus_g;
static REG_executeConstructors_tp IO_exeConstructorsFn_p;


/* pointer to network symbols */
extern const FIDO_NI_networkSymbols_t __network_symbols;
static FIDO_NI_networkSymbols_t *networkSymbols_p =
  (FIDO_NI_networkSymbols_t *)&__network_symbols;

/* Define the IO application information with default data.
 * Note that for product release this information will be update as
 * part of the post-build process */
#define __IO_APP_INFO __attribute__((section(".app_info")))
__IO_APP_INFO const IO_appInfo_t ioAppInfo_g = {
  {APP_INFO_VERSION},             /* appInfoVersion */
  {16},                            /* buildNumber */
  {"NI-STM32-ADIN11x0 Example IO Application"}, /* name */
  {"SC0000527"},                  /* partNum */
  {"001"}                         /* version */
};

/*=========================  P R O T O T Y P E S  =========================*/
int IO_ServiceMain(void);
const IO_appInfo_t *IO_GetApplicationInformation();
void IO_RefuseToConfigure(bool fatal);

static void IO_NetworkApplicationThread(void* arg);
static void IO_IoApplicationThread(void* arg);

#ifdef CPU_USAGE
static TimerHandle_t CPULoadTmr_Handle = NULL;
static TaskHandle_t CPULoadTaskHandle = NULL;
static void IO_CPU_UsageThread(void* arg);
static void PrintCPUUsageCallback(void* arg);
TaskHandle_t _printCPUUsageTaskHandle = NULL;
#endif

//lint -e641 Lawrence 2013-04-18
//lint -e716 Lawrence 2013-04-18
//lint -e655 Lawrence 2013-04-18

#define COORDINATOR_SYSTEM_TIME                    GetTickCount_RTOS_TASK()
#define COORDINATOR_MSEC_PER_HOUR                  3600000
#define COORDINATOR_HOUR_PER_DAY                   24
#define COORDINATOR_SEC_PER_HOUR                   3600
#define CYCLIC_ENTHALPY_CALCULATION                3000
#define CYCLIC_UPDATE_FE_TIME                      600000  // 10min

TBOOL readCalculationFlag = eFALSE;

#ifdef CPU_LOAD_TEST
void CPULoadTaskEXE(void);
RTOS_TASK_HANDLE CpuloadTskHandle;
#endif

static void ModbusTask(void* arg);

static TUSIGN32 lastTimeStamp = 0;
static TUSIGN8 lastFEConfParam = 0;
static TBOOL retrySendConfig = eTRUE;
static TUSIGN32 cdtOldDiagnosis = 0;
static TUSIGN32 cdtEnthalpyTimeOld = 0;
static TUSIGN32 cdtEnthalpyCount = 0;
static TUSIGN32 feTimeStamp = CYCLIC_UPDATE_FE_TIME;

static TUSIGN8 GetMeterParams(void);
   

/*****************************************************************************
 * brief       Error fall out point
 * details     The error fall out point for all all configuration and hardware
 *             errors in the IO application.  If fatal is not set the loop will
 *             sleep to allow debug queries via the shell.
 *
 * param[in]   fatal: true/false value indicates severity of the failure
 *
 * return      none
 */
void IO_RefuseToConfigure(bool fatal)
{
  OSPL_timerHandle_t errorTimer;

  if (!fatal) {
    CI_SetSystemStatus(CI_systemStatusFail);
    errorTimer = OSPL_SoftTimerCreate(OSPL_MsToTickCount(1000));
  }

  while (1) {
    if (fatal) {
      asm("nop;");
    } else {
      OSPL_SoftTimerDelayUntil(errorTimer);
    }
  }
}

/*****************************************************************************
 * brief       Performs the registration process for both applications
 *
 * details     This function executes the various constructors which have been
 *             placed in special program sections.
 *
 *
 * return      0: No error
 *             < 0: One or more constructor failed
 *
 */
#pragma section = REG_IO_REGISTER_SECTION_NAME
int32_t IO_SystemStartup()
{
  int32_t rv = -1;
  void *sectionAddr_p = __section_begin(REG_IO_REGISTER_SECTION_NAME);
  uint32_t sectionSize = __section_size(REG_IO_REGISTER_SECTION_NAME);

  if (networkSymbols_p->getRegistrarFuncs != NULL)
    rv = networkSymbols_p->getRegistrarFuncs(&IO_exeConstructorsFn_p);
  rv -= (rv == 0)? IO_exeConstructorsFn_p(REG_ioRegister, sectionAddr_p, sectionSize) : 0;

  return (rv);
}


/*****************************************************************************
 * brief       Primary execution thread for Network Application
 * details     This will translate into "context 0" for the network application.
 *             This thread will allocate and setup all other threads necessary
 *             for the network application to execute then sleep.
 *
 * param[in]   void
 *
 * return      none
 */
static void IO_NetworkApplicationThread(void* arg)
{
  /* run the network application */
  networkSymbols_p->applicationStart();

  /* task should never return but handle correctly if it does */
  vTaskDelete(NULL);
  return;
}

/*****************************************************************************
 * brief       IO application execution thread
 * details     Waits for network app to register common interface with system
 *             registrar then call the IO_ServiceMain.
 *
 * param[in]   void
 *
 * return      none
 */
#pragma section = REG_IO_REQUEST_SECTION_NAME
static void IO_IoApplicationThread(void* arg)
{
  int32_t rv = -1;
  
  int32_t retry = 0;
  const int32_t retryMax = 1000; // wait loop counter
  void *sectionAddr_p = __section_begin(REG_IO_REQUEST_SECTION_NAME);
  uint32_t sectionSize = __section_size(REG_IO_REQUEST_SECTION_NAME);

  /* perform the remaining steps in the start up process for the IO application */
  if (IO_exeConstructorsFn_p != NULL) {
    rv = IO_exeConstructorsFn_p(REG_ioRequest, sectionAddr_p, sectionSize);
    rv -= (rv == 0)? IO_exeConstructorsFn_p(REG_ioSwInit, sectionAddr_p, sectionSize) : 0;
    rv -= (rv == 0)? IO_exeConstructorsFn_p(REG_ioHwInit, sectionAddr_p, sectionSize) : 0;
  }
  if (rv < 0) {
    IO_RefuseToConfigure(false);
  }


  /* wait for network application sync */
  do {
    Delay_RTOS_TASK(500); // loop delay timing

    // 1000 loops at 10 ms each = 10 sec for the network app to
    // complete initialization and registration

    naStatus_g = networkSymbols_p->getRegStatus();
  } while ((naStatus_g <= 0) && ((++retry) < retryMax));

  if (retry >= retryMax)
  {
    IO_RefuseToConfigure(false);
  }

  CI_SetSystemParam(CI_sysParamDhcpEnable, (void *)&ethernetStaticFrequentStaticDefault.useDhcp,
                    sizeof(ethernetStaticFrequentStaticDefault.useDhcp));
  CI_SetSystemParam(CI_sysParamIpAddress, (void *)&ethernetStaticFrequentStaticDefault.hostAddress,
                    sizeof(ethernetStaticFrequentStaticDefault.hostAddress));
  CI_SetSystemParam(CI_sysParamSubnetMask, (void *)&ethernetStaticFrequentStaticDefault.subnetMask,
                    sizeof(ethernetStaticFrequentStaticDefault.subnetMask));
  CI_SetSystemParam(CI_sysParamDefaultGateway, (void *)&ethernetStaticFrequentStaticDefault.gatewayAddress,
                    sizeof(ethernetStaticFrequentStaticDefault.gatewayAddress));

  /* run the IO service */
  IO_ServiceMain();

  /* task should never return but handle correctly if it does */
  vTaskDelete(NULL);
  return;
}

#ifdef CPU_USAGE
/*****************************************************************************
 * brief       IO Get CPU usage thread
 * details     
 *
 * param[in]   void
 *
 * return      none
 */
static void IO_CPU_UsageThread(void* arg)
{
  //char buff[50];
  char cBuffer[20];

  CPULoadTmr_Handle = xTimerCreate((const char *)"AutoReloadTimer",
                                    (TickType_t)1000,
                                    (UBaseType_t)pdTRUE,
                                    (void *)1,
                                    (TimerCallbackFunction_t)PrintCPUUsageCallback);
  if (CPULoadTmr_Handle != NULL)
  {
    xTimerStart(CPULoadTmr_Handle, 0);
  }
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTimerReset(CPULoadTmr_Handle, 1000);
    // if (ret.value.signals & 0x02)
    // {
    //   // after initial output, activate cyclic output
    //   // SystemTimeSetCyclicTimer(1000, PrintCPUUsageCallback, NULL);
    // }
      sprintf(cBuffer,"CPU Load: %u%%\n",osGetCPUUsage());
      // HWPL_UartWrite(HWPL_MAX_UART, cBuffer, strlen(cBuffer), NULL);
      // PROJ_TRACE(("trying to print CPU LOAD \n %s \n", cBuffer));
  }
}

/*****************************************************************************
 * brief       Print CPU Usage Callback
 * details     
 *
 * param[in]   void
 *
 * return      none
 */
static void PrintCPUUsageCallback(void* arg)
{
   // callback from SysTick ISR
  //  if (arg != NULL)
  //  {
  //     osSignalSet(_printCPUUsageTaskHandle, 0x0002);
  //  }
  //  else
  //  {
  //     osSignalSet(_printCPUUsageTaskHandle, 0x0001);
  //  }
    xTaskNotifyGive(CPULoadTaskHandle);
}
#endif

/*****************************************************************************
 * brief       Get the io application information
 * details     Get the io application information which includes the
 *              IO application name, part number, and version
 *
 * param[in]   void
 *
 * return      pointer to the IO_appInfo_t structure
 */
const IO_appInfo_t *IO_GetApplicationInformation()
{
  return (&ioAppInfo_g);
}

/* End Of File */


//-------------------------------------------------------------------------------------------------
/*!
\brief  read configure message
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readMeterConfig_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length == CDT_METER_TYPE_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {     
        coordinatorUnprotected.frontEndConfParamRd =*(pDataBuf);   
        coordinatorUnprotected.frontEndTempTypeRd =*(pDataBuf+1);
    }
    ResumeAll_RTOS_TASK();       
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read calculation data
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readCalculationData_cdt(TUSIGN8 length,  TUSIGN8* const pDataBuf)
{
    TUSIGN8 alarm0 = 0;
    TUSIGN8 alarm1 = 0;
    TUSIGN8 oldAlarm = 0;
    SuspendAll_RTOS_TASK();
    readCalculationFlag = eFALSE;
    if((length == CDT_CALCULATION_DATA_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        if((coordinatorProtected.simulationMode != CDT_SIMULATION_FE_SENSOR_FREQ)&&  \
            (coordinatorProtected.frequencySim_Enable != CDT_SIMULATION_ENABLE)) 
        {
            coordinatorDynamicDuplicated.frequency =  *((TFLOAT*) pDataBuf);
            coordinatorDynamicDuplicated.frequency_Duplicated =  coordinatorDynamicDuplicated.frequency;
        }
        
        if((coordinatorProtected.passwordEnable&CDT_INTERNALRTD_ENABLE)&&  \
            (coordinatorReplaceStaticRare.intRTDType!=CDT_TEMP_NONE))
        {
            coordinatorUnprotected.rawTemperatureSensor =  *((TFLOAT*) pDataBuf+1);

            coordinatorDynamicDuplicated.temperatureSensor = coordinatorUnprotected.rawTemperatureSensor + \
                                                             coordinatorReplaceStaticRare.temperatureOffsetCorrect + \
                                                             coordinatorStaticRare.temperatureOffsetCorrectUser;
            coordinatorDynamicDuplicated.temperatureSensor_Duplicated = coordinatorDynamicDuplicated.temperatureSensor;
        }
        coordinatorUnprotected.snr =  *((TFLOAT*) pDataBuf+2);
        //coordinatorUnprotected.gainLowThresh =  *(TINT16*)((TUSIGN8*) pDataBuf+8);
        //byte1 &byte2 from frontend diagnosis
        alarm0 = *((TUSIGN8*) pDataBuf+12);
        alarm1 = (*((TUSIGN8*) pDataBuf+13))& 0x7F;
        oldAlarm = coordinatorDynamicDuplicated.cdtDiagnosis[0] & 0x7F;
        coordinatorDynamicDuplicated.cdtDiagnosis[0] = ((alarm0 & 0x01) << 0x07) | oldAlarm;
        coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated[0] = coordinatorDynamicDuplicated.cdtDiagnosis[0];
        coordinatorDynamicDuplicated.cdtDiagnosis[1] = ((alarm0 & 0xFE) >> 0x01) | ((alarm1 & 0x01) << 0x07);
        coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated[1] = coordinatorDynamicDuplicated.cdtDiagnosis[1];
        oldAlarm = coordinatorDynamicDuplicated.cdtDiagnosis[2] & 0xC0;   
        coordinatorDynamicDuplicated.cdtDiagnosis[2] = ((alarm1 & 0x7E) >> 0x01) | oldAlarm;
        coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated[2] = coordinatorDynamicDuplicated.cdtDiagnosis[2];

        coordinatorUnprotected.sigAmplitude =  *(TUSIGN16*)((TUSIGN8*) pDataBuf+14);
        coordinatorUnprotected.vbrAmplitude =  *(TUSIGN16*)((TUSIGN8*) pDataBuf+16);
        coordinatorUnprotected.sigMagnitude =  *(TUSIGN16*)((TUSIGN8*) pDataBuf+18);
    }
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read operation hour 
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readOperationHour_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    /*lint -e{826}  Lawrence shi 2012-10-16*/
    SuspendAll_RTOS_TASK();
    if((length == CDT_OPERATION_HOUR_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        coordinatorUnprotected.frontendOperationHour= *((TUSIGN32*) pDataBuf);
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  read maintenance info
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readMaintenance_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length == CDT_FE_MAINTAIN_CYCLE_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        coordinatorUnprotected.frontendMaintenance= *((TUSIGN32*) pDataBuf);  
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  read dsp parameters
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readDspParams_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length == CDT_DSP_PARAM_CMD_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{419} 2013-04-16 Lawrence shi
        (void)memcpy(&coordinatorUnprotected.vbrCorrectionOn,pDataBuf,length);
    } 
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  read autoselfcheck_Enable
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readautoselfcheck_Enable_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();    
    if((length==CDT_AUTOSELFDIAG_EN_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
        coordinatorDynamicDuplicated.autoselfcheck_Enable = *((TUSIGN8*) pDataBuf);
        coordinatorDynamicDuplicated.autoselfcheck_Enable_Duplicated = coordinatorDynamicDuplicated.autoselfcheck_Enable;
    }   
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read spanUpperRange
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readspanUpperRange_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();   
    if((length==CDT_SPAN_UPPRERRANGE_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
        coordinatorUnprotected.spanUpperRange = *((TINT16*) pDataBuf);
    }
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  read FE output filter algorithm type
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readOutputFilterAlgType_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    TUSIGN16 ext_algSelection = 0;

    SuspendAll_RTOS_TASK();   
    if((length==CDT_OUTPUT_FILTER_ALG_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        ext_algSelection = pDataBuf[0] + (((TUSIGN16)pDataBuf[1])<<8);

        if (ext_algSelection <= 0xFF)
        {
            coordinatorFE_OUTPUT_FILTER_PARA.ext_algSelection = (TUSIGN8)(ext_algSelection&0x00FF);
        }
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  read FE output filter algorithm para
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
//lint -e{818} 2013-04-16 Lawrence shi ignore const pointer because of read/write use same function
void readOutputFilterAlgPara_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    TUSIGN8 ii = 0;
    TUSIGN16 lIntAlgType = 0;

    SuspendAll_RTOS_TASK();   
    if((length==CDT_OUTPUT_FILTER_ALG_PARA_LENGTH)&&(pDataBuf == &uReceiveBuffer_FED_layer7.arydata[0]))
    {
        lIntAlgType = pDataBuf[ii++];
        lIntAlgType += (((TUSIGN16)pDataBuf[ii++])<<8);

        if (lIntAlgType <= 0xFF)
        {
            //lint -e{420} 2013-04-16 Lawrence shi
            coordinatorFE_OUTPUT_FILTER_PARA.int_algSelection = (TUSIGN8)(lIntAlgType & 0x00FF);

            (void)memcpy(coordinatorFE_OUTPUT_FILTER_PARA.ss_paras, &pDataBuf[ii], sizeof(coordinatorFE_OUTPUT_FILTER_PARA.ss_paras));
            ii += sizeof(coordinatorFE_OUTPUT_FILTER_PARA.ss_paras);
            (void)memcpy(coordinatorFE_OUTPUT_FILTER_PARA.sf_paras, &pDataBuf[ii], sizeof(coordinatorFE_OUTPUT_FILTER_PARA.sf_paras));
        }
    }
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  write config message
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
void  writeMeterConfig_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length==CDT_METER_TYPE_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        *(pDataBuf)  = coordinatorUnprotected.frontEndConfParamWr;
        *(pDataBuf+1)  = coordinatorReplaceStaticRare.intRTDType;
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write maintenance info
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
void writeFeMaintenance_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length==CDT_FE_MAINTAIN_CYCLE_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        
        *((TUSIGN32*) pDataBuf) =  coordinatorUnprotected.frontendMaintenance;
    }

    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write dsp parameters
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
void writeDspParam_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length==CDT_DSP_PARAM_CMD_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
        (void)memcpy(pDataBuf,&coordinatorUnprotected.vbrCorrectionOn,length);
    }
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  write autoselfcheck_Enable
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
void writeautoselfcheck_Enable_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length==CDT_AUTOSELFDIAG_EN_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
         *((TUSIGN8*) pDataBuf) =  coordinatorDynamicDuplicated.autoselfcheck_Enable;
         *((TUSIGN8*) pDataBuf + 1) =  0;
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write spanUpperRange
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
void writespanUpperRange_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    
    if((length==CDT_SPAN_UPPRERRANGE_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
         *((TINT16*) pDataBuf) =  coordinatorUnprotected.spanUpperRange;
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write spanUpperRange
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
void writeLowflowThreshold_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    
    if((length==CDT_LOWFLOW_THRESHOLD_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        //lint -e{420} 2013-04-16 Lawrence shi
         *((TINT16*) pDataBuf) =  coordinatorUnprotected.lowFlowThresh;
    }
    ResumeAll_RTOS_TASK();
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  write command, use command type only
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
void writeDefault(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length==CDT_DEFAULT_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        *(pDataBuf)  = 0x01;
    }
    
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write OutputFilterAlgType
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
void writeOutputFilterAlgType_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    
    if((length==CDT_OUTPUT_FILTER_ALG_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        pDataBuf[0] = coordinatorFE_OUTPUT_FILTER_PARA.ext_algSelection;
        pDataBuf[1] = 0x00;
    }
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  write OutputFilterAlgPara
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
void writeOutputFilterAlgPara_cdt(TUSIGN8 length, TUSIGN8* const pDataBuf)
{
    TUSIGN8 ii = 0;

    SuspendAll_RTOS_TASK();
    
    if((length==CDT_OUTPUT_FILTER_ALG_PARA_LENGTH)&&(pDataBuf == &uTransmitBuffer_FED_layer7.arydata[0]))
    {
        pDataBuf[ii++] = coordinatorFE_OUTPUT_FILTER_PARA.int_algSelection;
        pDataBuf[ii++] = 0x00;

        (void)memcpy(&pDataBuf[ii], coordinatorFE_OUTPUT_FILTER_PARA.ss_paras, sizeof(coordinatorFE_OUTPUT_FILTER_PARA.ss_paras));
        ii += sizeof(coordinatorFE_OUTPUT_FILTER_PARA.ss_paras);

        (void)memcpy(&pDataBuf[ii], coordinatorFE_OUTPUT_FILTER_PARA.sf_paras, sizeof(coordinatorFE_OUTPUT_FILTER_PARA.sf_paras));
    }
    ResumeAll_RTOS_TASK();
}

void (* comRead_cdt[])(TUSIGN8 , TUSIGN8* const)=
{
    readMeterConfig_cdt,
    readCalculationData_cdt,
    readOperationHour_cdt,
    readMaintenance_cdt,
    readDspParams_cdt,
    readautoselfcheck_Enable_cdt,
    readspanUpperRange_cdt,
    readOutputFilterAlgType_cdt,
    readOutputFilterAlgPara_cdt
};


void (* comWrite_cdt[])(TUSIGN8 , TUSIGN8* const)=
{
    writeMeterConfig_cdt,
    writeDefault,//writeResetHour_proc
    writeFeMaintenance_cdt,
    writeDspParam_cdt,
    writeDefault,//writeDspDefault_proc,
    writeDefault,//writeDspFactory_proc,
    writeDefault,//writeSaveDspDefault_proc,
    writeDefault,//writeSaveDspFactory_proc,
    writeDefault,//CDT_WRITE_SAVE_REPLACE_INDEX
    writeautoselfcheck_Enable_cdt,
    writespanUpperRange_cdt,
    writeLowflowThreshold_cdt,
    writeOutputFilterAlgType_cdt,
    writeOutputFilterAlgPara_cdt
};



const T_DYNAMIC_CHECK_TABLE cdtDynamicVariablesTable[] = 
{
    //lint -e{826} lawrence-shi 2013-04-18
    {(void *)&( coordinatorDynamicDuplicated.frequency), (void *)&(coordinatorDynamicDuplicated.frequency_Duplicated), sizeof(coordinatorDynamicDuplicated.frequency_Duplicated)},
    {(void *)&( coordinatorDynamicDuplicated.frequency_Damped), (void *)&( coordinatorDynamicDuplicated.frequency_Damped_Duplictated), sizeof( coordinatorDynamicDuplicated.frequency_Damped_Duplictated)},   
    {(void *)&( coordinatorDynamicDuplicated.temperature), (void *)&( coordinatorDynamicDuplicated.temperature_Duplicated), sizeof(coordinatorDynamicDuplicated.temperature_Duplicated)},
    {(void *)&( coordinatorDynamicDuplicated.temperature_Damped), (void *)&( coordinatorDynamicDuplicated.temperature_Damped_Duplicated), sizeof(coordinatorDynamicDuplicated.temperature_Damped_Duplicated)},
    {(void *)&( coordinatorDynamicDuplicated.temperature_Percentage), (void *)&(coordinatorDynamicDuplicated.temperature_Percentage_Duplicated), sizeof(coordinatorDynamicDuplicated.temperature_Percentage_Duplicated)},
    {(void *)&( coordinatorDynamicDuplicated.cdtDiagnosis[0]), (void *)&(coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated[0]), sizeof(coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated)},
    {(void *)&( coordinatorDynamicDuplicated.temperatureSensor), (void *)&(coordinatorDynamicDuplicated.temperatureSensor_Duplicated), sizeof(coordinatorDynamicDuplicated.temperatureSensor_Duplicated)},
     {(void *)&( coordinatorDynamicDuplicated.autoselfcheck_Enable), (void *)&(coordinatorDynamicDuplicated.autoselfcheck_Enable_Duplicated), sizeof(coordinatorDynamicDuplicated.autoselfcheck_Enable_Duplicated)}
};


static TUSIGN32 taskFlag[FLAG_TASK_SIZE] = {0,0,0};

//-------------------------------------------------------------------------------------------------
/*!
 \brief  set flag of task.
\author
\date
\param idxTask
\param idxSequence
\return -
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
void SetFlag(TUSIGN8 idxTask, TUSIGN8 idxSequence)
{
    VIP_ASSERT(idxTask < FLAG_TASK_SIZE);
    switch(idxTask)
    {
      case FLAG_MAIN_TASK:
        VIP_ASSERT(idxSequence < SIZEOF_MAIN_TASK_FUNCTION_FLAG);
        break;
      case FLAG_SLOW_TASK:
        VIP_ASSERT(idxSequence < SIZEOF_SLOW_TASK_FUNCTION_FLAG);
        break;
      case FLAG_WATCH_DOG_TASK:
        VIP_ASSERT(idxSequence < SIZEOF_WATCH_DOG_TASK_FUNCTION_FLAG);
        break;
      default:
        break;
    }
    
    taskFlag[idxTask] |= ((TUSIGN32)1)<<idxSequence;
}



//-------------------------------------------------------------------------------------------------
/*!
\brief  check flag of task. if the flag from 0 to idxSequence are all set, return. otherwise, reset device.
\author
\date
\param idxTask
\param idxSequence

\return -
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
void CheckFlag(TUSIGN8 idxTask, TUSIGN8 idxSequence)
{
    TUSIGN32 flag = 0x00;
    VIP_ASSERT(idxTask < FLAG_TASK_SIZE);
    switch(idxTask)
    {
      case FLAG_MAIN_TASK:
        VIP_ASSERT(idxSequence <= SIZEOF_MAIN_TASK_FUNCTION_FLAG);
        break;
      case FLAG_SLOW_TASK:
        VIP_ASSERT(idxSequence <= SIZEOF_SLOW_TASK_FUNCTION_FLAG);
        break;
      case FLAG_WATCH_DOG_TASK:
        VIP_ASSERT(idxSequence <= SIZEOF_WATCH_DOG_TASK_FUNCTION_FLAG);
        break;
      default:
        break;
    }
    
    flag = ( (TUSIGN32)1<<idxSequence ) -1;
    if(taskFlag[idxTask] == flag)
    {
    }
    else
    {
        elettronicServices.ExitSRV();// reset device;
    }
}


//-------------------------------------------------------------------------------------------------
/*!
\brief  clear flags. 
\author
\date
\param idxTask
\return -
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
void ClearFlag(TUSIGN16 idxTask)
{
    VIP_ASSERT(idxTask < FLAG_TASK_SIZE);
    taskFlag[idxTask] = 0;
}


//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATESTEAMENTHALPY> \endif
 \brief  
 \param  opModeShift 
\if @SubGen end@ <METHODHEADER UPDATESTEAMENTHALPY> \endif
\author Lawrence Shi
\date  2012-11-05
\param -
\return -
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATESTEAMENTHALPY>
TUSIGN16 UpdateSteamEnthalpy_CDT(TUSIGN16 opModeShift)
//@SubGen end@<METHOD UPDATESTEAMENTHALPY>
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN32 time = 0;
    TFLOAT realTemperature = 0.0f;
    TFLOAT extTemperature = 0.0f;
    TFLOAT realPressure = 0.0f;
    
    if(opModeShift & SELECTION_STEAMPOWER)
    {
        time  = (TUSIGN32)COORDINATOR_SYSTEM_TIME;
        
        if(cdtEnthalpyTimeOld == 0)
        {
            cdtEnthalpyCount = 0;
        }
        else
        {
            if(time > cdtEnthalpyTimeOld)
            {
                cdtEnthalpyCount += (time - cdtEnthalpyTimeOld);
            }
            else
            {
                cdtEnthalpyCount += SLOW_UPDATE_TIME;
            }
        }
        if(cdtEnthalpyCount >= CYCLIC_ENTHALPY_CALCULATION)
        {
            result |=  coordinator_Get((TUSIGN16)CDT_IDX_temperature,(TINT16)WHOLE_OBJECT,&realTemperature); 
            result |=  mapper_Get((TUSIGN16)MAPPER_IDX_dvPressure,(TINT16)WHOLE_OBJECT,&realPressure);
            result |=  mapper_Get((TUSIGN16)MAPPER_IDX_dvTExt,(TINT16)WHOLE_OBJECT,&extTemperature);
            mVMeasurement.CalcSteamHEXE(realPressure,realTemperature, extTemperature);
            cdtEnthalpyCount = 0;
        }
        cdtEnthalpyTimeOld = time;

    }
    return result;
}



//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEINTERNALTEMPERATURE> \endif
 \brief  
\if @SubGen end@ <METHODHEADER UPDATEINTERNALTEMPERATURE> \endif
\author
\date
\param -
\return -
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
//@SubGen start@<METHOD UPDATEINTERNALTEMPERATURE>
TUSIGN16 UpdateInternalTemperature_CDT(void)
//@SubGen end@<METHOD UPDATEINTERNALTEMPERATURE>
{
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;//HART input Content Selection.
    TUSIGN8 Act_InputFail = CDT_PRESET;
    TUSIGN8 AISelection = AI_WITHOUT ;//Analog input Content Selection.
    TUSIGN8 simulationMode_CDT = CDT_SIMULATION_CB_OFF;
    TUSIGN8 tInterSimEnable = CDT_SIMULATION_DISABLE; 
    TFLOAT  TInternalInput = 0.0f ;//To store the Input Temperature.
    TFLOAT temperatureSensor = 0.0f;
    
    TUSIGN16 errorCode = (TUSIGN16)OK;
    TUSIGN8 source = 0;
    TUSIGN8  burstInStatus = 0xff;
    TFLOAT aiCurrent = 0.0f;
    static TFLOAT  lastgoodTemp = 0.0f;
    //Get Information from Hart Subsystem
    errorCode  = hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, 0, &HartSelection);
    //Get Information from AI Subsystem
    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiSelection, 0, &AISelection);
    
    errorCode |= coordinator_Get((TUSIGN16)CDT_IDX_simulationMode, 0, &simulationMode_CDT);
    errorCode |= coordinator_Get((TUSIGN16)CDT_IDX_temperatureSim_Enable, 0, &tInterSimEnable);
    
    errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, 0, &burstInStatus);
    errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiTempCurr,0,&aiCurrent);

    if(errorCode == (TUSIGN16)OK)
    {
        if((simulationMode_CDT == CDT_SIMULATION_CB_MEDIUM_TEMP)||  \
            (tInterSimEnable == CDT_SIMULATION_ENABLE)) 
        {           
            errorCode = coordinator_Get((TUSIGN16)CDT_IDX_temperature_Sim,(TINT16)WHOLE_OBJECT,&TInternalInput);
        }
        else if((coordinatorProtected.passwordEnable& CDT_INTERNALRTD_ENABLE)&&   \
            (coordinatorReplaceStaticRare.intRTDType != CDT_TEMP_NONE))
        {
            errorCode |= coordinator_Get((TUSIGN16)CDT_IDX_temperatureSensor,(TINT16)WHOLE_OBJECT,&temperatureSensor);

            if ((temperatureSensor >= CDT_FE_SENSOR_TEMPERATURE_VALID_MAX) ||
                (temperatureSensor <= CDT_FE_SENSOR_TEMPERATURE_VALID_MIN))
            {
                errorCode = coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&TInternalInput);
                source = CDT_T_PRESET;
                errorCode = coordinator_Put((TUSIGN16)CDT_IDX_temperature_Selection, (TINT16)WHOLE_OBJECT, &source);
            }
            else
            {
                TInternalInput = temperatureSensor;
                source = CDT_T_SENSOR;
                errorCode = coordinator_Put((TUSIGN16)CDT_IDX_temperature_Selection, (TINT16)WHOLE_OBJECT, &source);
            }
        }
        else
        {
            // AISelection is AI_TEMPERATURE
            if(AISelection == AI_INTER_TEMPERATURE) 
            {
                if((aiCurrent>=3.8) && (aiCurrent<=20.5))
                {
                    errorCode = analogInput_Get((TUSIGN16)AI_IDX_aiInValue, 0, &TInternalInput); 
                    source = CDT_T_AI;
                    errorCode  =coordinator_Put((TUSIGN16)CDT_IDX_temperature_Selection, 0, &source);
                }
                else
                {
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                        TInternalInput =  lastgoodTemp;
                    }
                    else
                    {
                        errorCode = coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&TInternalInput);
                    }
                }
            }
            //HART burst input enabled and HartSelection is HART_SEC_TEMPERATURE
            else if(HartSelection == HART_SEC_INTER_TEMPERATURE) 
            {
                if(burstInStatus == HART_BURST_IN_OK)
                {
                    errorCode = hart_Get((TUSIGN16)HART_IDX_burstUpBaseValue, 0, &TInternalInput);
                    source = CDT_T_HART;
                    errorCode  =coordinator_Put((TUSIGN16)CDT_IDX_temperature_Selection, 0, &source);
                }
                else
                {
                    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                    if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                    {
                        TInternalInput =  lastgoodTemp;
                    }
                    else
                    {
                        errorCode = coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&TInternalInput);
                    }
                }
            }
            //other cases except (AI_Enable&&AI input is temperature) and (HART_IN_Enable&&Hart input is temperature)
            else
            {
                errorCode = coordinator_Get((TUSIGN16)CDT_IDX_temperaturePreset,(TINT16)WHOLE_OBJECT,&TInternalInput);
                source = CDT_T_PRESET;   
                errorCode  =coordinator_Put((TUSIGN16)CDT_IDX_temperature_Selection, 0, &source);
            }
        }
        
        lastgoodTemp = TInternalInput;


        if(errorCode == (TUSIGN16)OK)
        {
            errorCode |= coordinator_Put((TUSIGN16)CDT_IDX_temperature,(TINT16)WHOLE_OBJECT,&TInternalInput);
            errorCode |= coordinator_Put((TUSIGN16)CDT_IDX_temperature_Duplicated,(TINT16)WHOLE_OBJECT,&TInternalInput); 
        }
    }
    else
    {

    }
    
    return errorCode; 
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEFUNCTIONPASSWORD> \endif
 \brief  
\if @SubGen end@ <METHODHEADER UPDATEFUNCTIONPASSWORD> \endif
\author  Lawrence Shi
\date    2013-04-25
\param - none
\return -none
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEFUNCTIONPASSWORD>
void UpdateFunctionPassword_CDT(void)
//@SubGen end@<METHOD UPDATEFUNCTIONPASSWORD>
{
    TUSIGN16 result = (TUSIGN16)OK;
    TCHAR strOrderNum[LENGTH_OF_ORDER_NUMBER+1] = {"0"};
    TCHAR tmpPassword[LENGTH_OF_FUNCTION_PASSWORD+1] = {"******"};
    TUSIGN8 ChangeFlag = 0;    
    result |= elettronicServices_Get(ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag,0,&ChangeFlag);
    if((result == OK)&&(ChangeFlag == 1))
    {
        (void)memset((void*)strOrderNum, '\0', (LENGTH_OF_ORDER_NUMBER+1));
        result = elettronicServices_Get(ELETTRONICSERVICES_IDX_tranSapErpNo, WHOLE_OBJECT, strOrderNum);
        GetFunctionPasswd(strOrderNum, strIntRTDName, strIntPassword);
        GetFunctionPasswd(strOrderNum, strAIName, strAiPassword);
        GetFunctionPasswd(strOrderNum, strEnergyName, strEnergyPassword);
        result |= coordinator_Put(CDT_IDX_internalRTDPassword, WHOLE_OBJECT, &tmpPassword[0]);
        result |= coordinator_Put(CDT_IDX_analogInputPassword, WHOLE_OBJECT, &tmpPassword[0]);
        result |= coordinator_Put(CDT_IDX_energyFlowPassword, WHOLE_OBJECT, &tmpPassword[0]);
    }
    ChangeFlag = 0;
    result |= elettronicServices_Put(ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag,0,&ChangeFlag);
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER CHECKDATACRC> \endif
 \brief  
\if @SubGen end@ <METHODHEADER CHECKDATACRC> \endif
\author
\date
\param -
\return -
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 84.62% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKDATACRC>
void CheckDataCrc_CDT(void)
//@SubGen end@<METHOD CHECKDATACRC>
{
    TUSIGN16 crc = InitCRC16_COMMON();
    ENABLE_DATA_PROTECTION(*(coordinator.unit.ptrDataSemaphore));
    if(coordinatorProtected.protectedDataStatus == CDT_CALCULATE)
    {
        coordinatorProtected.protectedDataStatus = CDT_PROTECTED;
        crc = CalcCrc16Mem_COMMON((TUSIGN8*)&coordinatorProtected,crc,sizeof(coordinatorProtected)/sizeof(TUSIGN8));
        coordinatorUnprotected.protectedDataCrc  = crc;
    }
    else if(coordinatorProtected.protectedDataStatus == CDT_PROTECTED)
    {
       
        crc = CalcCrc16Mem_COMMON((TUSIGN8*)&coordinatorProtected,crc,sizeof(coordinatorProtected)/sizeof(TUSIGN8));
        
        if(coordinatorUnprotected.protectedDataCrc != crc)
        {
            elettronicServices.ExitSRV();
        }
    }
    else if( CDT_UNPROTECTED != coordinatorProtected.protectedDataStatus)
    {
        elettronicServices.ExitSRV();
    }
    else
    {
    }
    DISABLE_DATA_PROTECTION(*(coordinator.unit.ptrDataSemaphore));
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEMAXTEMPERATURE> \endif
 \brief  
\if @SubGen end@ <METHODHEADER UPDATEMAXTEMPERATURE> \endif
\author Lawrence Shi
\date  2012-11-05
\param -
\return -
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEMAXTEMPERATURE>
void UpdateMaxTemperature_CDT(void)
//@SubGen end@<METHOD UPDATEMAXTEMPERATURE>
{   
    TFLOAT temperature =  0.0f;
    TFLOAT temperatureMaxRecord = 0.0f;
    TFLOAT actambientTemperature = 0.0f;
    TFLOAT ambientTemperatureMaxRecord = 0.0f;
    
    (void)coordinator_Get((TUSIGN16)CDT_IDX_temperatureSensor,(TINT16)WHOLE_OBJECT,&temperature);
    (void)coordinator_Get((TUSIGN16)CDT_IDX_temperatureMaxRecord,(TINT16)WHOLE_OBJECT,&temperatureMaxRecord);
    
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_acttelettronicTemperature,(TINT16)WHOLE_OBJECT,&actambientTemperature);
    (void)elettronicServices_Get((TUSIGN16)ELETTRONICSERVICES_IDX_ambientTemperatureMax,(TINT16)WHOLE_OBJECT,&ambientTemperatureMaxRecord);

    //Record medium temperature.
    if(temperature > temperatureMaxRecord)
    {
        (void)Put_T_UNIT(COORDINATOR_ME,(TUSIGN16)CDT_IDX_temperatureMaxRecord,(TINT16)WHOLE_OBJECT,(void FAST*)&temperature);
    }
      
    //Record ambient temperature.
    if(actambientTemperature > ambientTemperatureMaxRecord)
    {
        (void)Put_T_UNIT(ELETTRONIC_SERVICES_ME,(TUSIGN16)ELETTRONICSERVICES_IDX_ambientTemperatureMax,(TINT16)WHOLE_OBJECT,(void FAST*)&actambientTemperature);
    }    
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER GETTEMPERATUREPERCENTAGESRV> \endif
 \brief  
\if @SubGen end@ <METHODHEADER GETTEMPERATUREPERCENTAGESRV> \endif
\author Lawrence Shi
\date  2012-11-05
\param -
\return -
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD GETTEMPERATUREPERCENTAGESRV>
TFLOAT GetTemperaturePercentageSRV_CDT(void)
//@SubGen end@<METHOD GETTEMPERATUREPERCENTAGESRV>
{  
    return coordinatorDynamicDuplicated.temperature_Percentage ;  
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER STOPTASKSSRV> \endif
 \brief  
 \param  taskIdx 
\if @SubGen end@ <METHODHEADER STOPTASKSSRV> \endif
\author Lawrence Shi
\date  2012-11-05
\param -
\return -
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment: C-SPY
\n intention:
\n result module test: OK
\n result Lint Level 3:OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD STOPTASKSSRV>
void StopTasksSRV_CDT(TUSIGN16 taskIdx)
//@SubGen end@<METHOD STOPTASKSSRV>
{
    /*
    TUSIGN8 uBitEnableList[LENGTH_OF_BITEACTIONLIST] =
    {
    1,//MainTaskWatchdogBiteAction,
    1,//HartTaskWatchdogBiteAction,
    1,//ServiceTaskWatchdogBiteAction,
    1,//SlowTaskWatchdogBiteAction,
    1,//FrontendInterfaceTaskWatchdogBiteAction,
    //1, CPULoadTaskWatchdogBiteAction,
    };
    */
    
    TUSIGN8 i =0;
    for(i=0;i<LENGTH_OF_BITEACTIONLIST;i++)
    {
        uBitEnableList[i] = 0;
    }   
    if(CDT_MAIN_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(MainTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        uBitEnableList[0] = 0;
        Delete_RTOS_TASK(MainTskHandle);
    }
    if(CDT_MAIN_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(HartTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        uBitEnableList[1] = 0;
        Delete_RTOS_TASK(HartTskHandle);
    }
    if(CDT_SERVICE_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(ServiceTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        uBitEnableList[2] = 0;
        Delete_RTOS_TASK(ServicesTskHandle);
    }
    if(CDT_SLOW_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(SlowTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        uBitEnableList[3] = 0;
        Delete_RTOS_TASK(SlowTskHandle);
    }
    if(CDT_FE_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(FrontendInterfaceTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        uBitEnableList[4] = 0;
        Delete_RTOS_TASK(FrontendTskHandle);
    }
#ifdef CPU_LOAD_TEST
    if(CDT_CPULOAD_TASK & taskIdx)
    {
        TriggerWatchdog_CDT(FrontendInterfaceTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
        Delete_RTOS_TASK(CpuloadTskHandle);
    }
#endif    


   
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  initialize all subsystem.
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 InitializeSubsystems(void)
{
    TUSIGN16 result = (TUSIGN16)OK;

    InitializeMasterWatchDog_CDT();  //create & run WatchdogTask as early as possible
    // Set Watchdog Schedule
    SetWatchdogSchedule(400);
    
    //lint -e{641,655} 2013-04-16 lawrence shi
    result |= elettronicServices.unit.Initialize(ELETTRONIC_SERVICES_ME, INIT_HARDWARE|INIT_TASKS);
    result |= sVMeasurement.unit.Initialize(SVMEASUREMENT_ME, INIT_HARDWARE|INIT_TASKS);
    
    //SwitchToExClock_SYSTEM();
    
    result |= frontEndInterface.unit.Initialize(FRONT_END_INTERFACE_ME, INIT_HARDWARE|INIT_TASKS);
    
    result |= hart.unit.Initialize(HART_ME, INIT_DATA|INIT_CALCULATION|INIT_TASKS);

    //Init mVMeasurement
    result |= mVMeasurement.unit.Initialize(MVMEASUREMENT_ME, INIT_HARDWARE|INIT_TASKS);
    
    result |= arm.unit.Initialize(ARM_ME, INIT_HARDWARE|INIT_TASKS);

    result |= StartupNVSubsystem();

    //Init Coordinator ,Init Coordinator data
    result |= coordinator.unit.Initialize(COORDINATOR_ME, INIT_DATA);
    
    result |= arm.unit.Initialize(ARM_ME, INIT_DATA|INIT_CALCULATION);

    result |= sVMeasurement.unit.Initialize(SVMEASUREMENT_ME, INIT_DATA|INIT_CALCULATION);
    result |= mVMeasurement.unit.Initialize(MVMEASUREMENT_ME, INIT_DATA|INIT_CALCULATION);
    
    //Diagnosis Init 
    result |= diagnosis.unit.Initialize(DIAGNOSIS_ME, INIT_DATA|INIT_TASKS); 
    
    //Init Totalizer
    result |= totalizer.unit.Initialize(TOTALIZER_ME, INIT_HARDWARE|INIT_DATA|INIT_TASKS);
    
    //Init UOM
    result |= unitsOfMeasure.unit.Initialize(UNITS_OF_MEASURE_ME, INIT_TASKS|INIT_DATA);
    
    result |= frontEndInterface.unit.Initialize(FRONT_END_INTERFACE_ME, INIT_DATA|INIT_CALCULATION);
    // Init ES DATA after front end interface, as ES read SNR and FE version from front end interface.
    //lint -e{641,655} 2013-04-16 lawrence shi   
    result |= elettronicServices.unit.Initialize(ELETTRONIC_SERVICES_ME, INIT_DATA);
    //lint -e{641,655} 2013-04-16 lawrence shi   
    result |= digitalOutput.unit.Initialize(DIGITAL_OUTPUT_ME, INIT_HARDWARE|INIT_TASKS|INIT_DATA);
    //lint -e{641,655} 2013-04-16 lawrence shi  
     
    result |= analogInput.unit.Initialize(ANALOG_INPUT_ME, INIT_HARDWARE|INIT_DATA|INIT_TASKS);


    //lint -e{641,655} 2013-04-16 lawrence shi
    //result |= servicePort.unit.Initialize(SERVICE_PORT_ME, INIT_DATA|INIT_HARDWARE|INIT_TASKS);
    result |= servicePort.unit.Initialize(SERVICE_PORT_ME, INIT_DATA|INIT_TASKS);
    
    //Init Mapper
    result |= mapper.unit.Initialize(MAPPER_ME, INIT_TASKS|INIT_DATA);
    
    result |= hart.unit.Initialize(HART_ME, INIT_HARDWARE);
#ifdef COMPILE_WITH_ETHERNET_SUBSYSTEM
    result |= ethernet.unit.Initialize(ETHERNET_ME, INIT_TASKS|INIT_DATA);
#endif
#ifdef COMPILE_WITH_MODBUS_SUBSYSTEM
    result |= modbus.unit.Initialize(MODBUS_ME, INIT_HARDWARE|INIT_TASKS|INIT_DATA);
#endif


    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER MAINTASKEXE> \endif
 \brief  Metering, Current out, Digital out, Diagnosis  and Totalizer subsystem are executed in this path.
\if @SubGen end@ <METHODHEADER MAINTASKEXE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 0% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD MAINTASKEXE>
void MainTaskEXE_CDT(void)
//@SubGen end@<METHOD MAINTASKEXE>
{
    TUSIGN8 lDummy = 0;
    TUSIGN8 forcedReplace = CDT_FORCE_REPLACE_NONE;
    T_DIGTALINPUT_PARAMETER digitalInput;
    TUSIGN32 elapseTime = 0;
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN16 opModeBit;
    TFLOAT frequency = 0.0f;
    TFLOAT temperature = 0.0f;
    TFLOAT temperatureExt = 0.0f;
    TFLOAT pressure = 0.0f;
    TFLOAT density = 0.0f;
    TFLOAT gasContent = 0.0f;
    TFLOAT Qv = 0.0f;
    TFLOAT Qn = 0.0f;
    TFLOAT Qm = 0.0f;
    TFLOAT Qpower = 0.0f;
    TFLOAT patialQv = 0.0f;
    TFLOAT patialQn = 0.0f;
    T_DIAGNOSIS_STATUS diagnosisStatus;
    static  TUSIGN8 opMode = 0xff;
    TUSIGN8 writeProtection;
    TUSIGN8 notUse = 0;
    TUSIGN8 burstInSelection = HART_SEC_PRESSURE;
    
    HWPL_boardConfig_t *brdCfg_p;

    TaskHandle_t taskHandle = xTaskGetIdleTaskHandle();

    OSPL_SetTaskTag(taskHandle, IDLE_TASK_TAG);
#if 0
    /* Verify valid board config */
    brdCfg_p = PCFG_GetBoardOptions();
    if(brdCfg_p == NULL)
    {
        IO_RefuseToConfigure(true);
    }

    // ADI load the platform configuration, this also initializes external RAM, e.g. for slow heap usage 
    if ((PCFG_Init()) < 0)
    {
        IO_RefuseToConfigure(true);
    }

      /* verify network symbols are valid */
    if ((networkSymbols_p->magicNumber != NETWORK_SYMBOL_MAGIC_NUMBER) ||
        (networkSymbols_p->version < NETWORK_SYMBOL_VERSION) ||
        (networkSymbols_p->applicationStart == NULL) ||
        (networkSymbols_p->getRegStatus == NULL))
    {
        IO_RefuseToConfigure(true);
    }

    if (IO_SystemStartup() < 0) 
    {
        IO_RefuseToConfigure(false);
    }
#endif

    result = InitializeSubsystems();

    /*
    Description:
        Object HART_IDX_burstUpTxSelection has unit type and unit code
        HART_IDX_burstUpTxSelection saved in NV
        HART_IDX_burstUpUnitCode and HART_IDX_burstUpUnitType saved in RAM
    Bug: 
        When restart the device, the HART_IDX_burstUpUnitCode and HART_IDX_burstUpUnitType can be not consistent with HART_IDX_burstUpTxSelection.
    Fix Bug:
        When start the device, overload HART_IDX_burstUpUnitCode and HART_IDX_burstUpUnitType to refresh HART_IDX_burstUpUnitType and HART_IDX_burstUpUnitCode   
    */  
    (void)hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection,WHOLE_OBJECT,&burstInSelection);
    (void)hart_Put((TUSIGN16)HART_IDX_burstUpTxSelection,WHOLE_OBJECT,&burstInSelection);
    
    //Create Front end Task (Frontend)
    if (Create_RTOS_TASK((TaskFunction_t)coordinator.FrontendTaskEXE, "FrontendTask", FRONTEND_TASK_STACK_SIZE,
                        NULL, FRONTEND_TASK_PRIORITY, &FrontendTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }
    //Check and Repaire FE and CB.
    (void)arm_Get((TINT16)ARM_IDX_readOnlySwitchStatus,(TINT16)WHOLE_OBJECT,&writeProtection);
    if (writeProtection == (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE)
    {    // if Read-Only = inactive
        result = InitializeNV_REPLACE();
        if(result == REPLACE_OK)
        {
           //lint -e{732} 2013-06-16 Lawrence
          result = frontEndInterface.PutCommand(CDT_WRITE_SAVE_REPLACE,CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_SAVE_REPLACE_INDEX],NULL);
        }    
        //disable replace.
        forcedReplace = CDT_FORCE_REPLACE_NONE;
        result |= coordinator_Put(CDT_IDX_forceReplace,WHOLE_OBJECT,&forcedReplace);
    }
    result = totalizer_Put((TUSIGN16)TOTALIZER_IDX_StartAllTotalizr,(TINT16)WHOLE_OBJECT,&notUse);
    
        //Init Coordinator ,Init Coordinator calculaton data (password related)
    result |= coordinator.unit.Initialize(COORDINATOR_ME,INIT_CALCULATION);
#if 0
    /* create application threads */
    if (Create_RTOS_TASK(IO_NetworkApplicationThread,
                    "Network Application Main",
                    4*configMINIMAL_STACK_SIZE,
                    NULL,
                    NETWORK_APP_BASE_PRIORITY,
                    &taskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }

    OSPL_SetTaskTag(taskHandle, NETWORK_APP_TASK_TAG);
    OSPL_SetTaskTagName(NETWORK_APP_TASK_TAG, "Network App. Main");

    // startup CI, initialize the ADI CI Interface
    ethernet.VerifyMainProtocolEXE();

    if (Create_RTOS_TASK(IO_IoApplicationThread,
                    "IO Thread",
                    2*configMINIMAL_STACK_SIZE,
                    NULL,
                    IO_APP_BASE_PRIORITY,
                    &taskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }

    OSPL_SetTaskTag(taskHandle, IO_APP_TASK_TAG);
    OSPL_SetTaskTagName(IO_APP_TASK_TAG, "IO App. Main");
#endif

#ifdef COMPILE_WITH_WEBSERVER_SUBSYSTEM
    result |= webServer.unit.Initialize(WEB_SERVER_ME, INIT_HARDWARE|INIT_TASKS|INIT_DATA|INIT_CALCULATION );
#endif

#ifdef CPU_USAGE
    if (Create_RTOS_TASK(IO_CPU_UsageThread,
                    "CPU Usage",
                    2*configMINIMAL_STACK_SIZE,
                    NULL,
                    1,
                    &CPULoadTaskHandle) != pdPASS) {
    IO_RefuseToConfigure(false);
    }
#endif

#ifdef ENABLE_WATCHDOG
    /* enable software watch dog bite function*/
    softwareWatchdogEnable();
#endif

#ifdef COMPILE_WITH_WEBSERVER_SUBSYSTEM
    {
        if (webServer.CertificateFilesExistEXE() == eFALSE) {
            (void)webServer.GenerateCertificateEXE();
        }
        webServer.ReadCertificateInfoFromFsEXE();
    }
#endif

    //Creat Hart Task (Hart)
    if (Create_RTOS_TASK((TaskFunction_t)(coordinator.HartTaskEXE), "HartTask", HART_TASK_STACK_SIZE,
                        NULL, HART_TASK_PRIORITY, &HartTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }
    //Create Service Task (Hmi/ServicePort)
    if (Create_RTOS_TASK((TaskFunction_t)(coordinator.ServicesTaskEXE), "ServicesTask", SERVICES_TASK_STACK_SIZE,
                        NULL, SERVICES_TASK_PRIORITY, &ServicesTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }

#ifdef COMPILE_WITH_MODBUS_SUBSYSTEM
    //Creat Modbus Task (Modbus)
    if (Create_RTOS_TASK((TaskFunction_t)ModbusTask, "ModbusTask", MODBUS_TASK_STACK_SIZE,
                        NULL, MODBUS_TASK_PRIORITY, &ModbusTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }
#endif

    //Create Slow Task (Nv Storage, Fe Nv Storage, ElectronicService)
    if (Create_RTOS_TASK((TaskFunction_t)(coordinator.SlowTaskEXE), "SlowTask", SLOW_TASK_STACK_SIZE,
                        NULL, SLOW_TASK_PRIORITY, &SlowTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }

#ifdef CPU_LOAD_TEST
    if (Create_RTOS_TASK((TaskFunction_t)CpuLoadTaskEXE, "CpuloadTask", CPULOAD_TASK_STACK_SIZE,
                        NULL, CPULOAD_TASK_PRIORITY, &CpuloadTskHandle) != RTOS_PASS)
    {
        IO_RefuseToConfigure(false);
    }
#endif

    (void)coordinator_Put(CDT_IDX_read_alg_block, WHOLE_OBJECT, &lDummy);

    while(1)
    {

#ifdef ENABLE_WATCHDOG
        /* Trigger main task software watchdog. */
        TriggerWatchdog_CDT(MainTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
#endif
        CheckFlag(FLAG_MAIN_TASK, MAIN_TASK_DATA_FLAG);
        readCalculationFlag = eTRUE;
        SetFlag(FLAG_MAIN_TASK, MAIN_TASK_DATA_FLAG);
        
        CheckFlag(FLAG_MAIN_TASK, MAIN_TASK_SVM_FLAG); 
        if(coordinatorReplaceStaticFreqStaticDefault.operatingMode!=opMode)
        {
            opMode = coordinatorReplaceStaticFreqStaticDefault.operatingMode;
            digitalInput.measurementChange = 1;
        }
        else
        {
            digitalInput.measurementChange = 0;
        }
        
        frequency   =  Calculate_Frequency();
        temperature =  Calculate_Temperature();
        density = GetDensity();
        //lint -e{732}{747} 2013-06-16 Lawrence 
        opModeBit = 1<<coordinatorReplaceStaticFreqStaticDefault.operatingMode;
        Qv = sVMeasurement.CalculateEXE(frequency,opModeBit);
        SetFlag(FLAG_MAIN_TASK, MAIN_TASK_SVM_FLAG);
              
        CheckFlag(FLAG_MAIN_TASK, MAIN_TASK_MVM_FLAG);
        digitalInput.simulationMode= coordinatorProtected.simulationMode;
        
        elapseTime = CalculateElapsedTime();
        digitalInput.elapseTime = elapseTime;
        result |= totalizer.UpdateQvEXE(Qv,elapseTime);

        switch(opModeBit)
        {
          case SELECTION_LIQUIDQV:
          case SELECTION_GASQV:
          case SELECTION_STEAMQV:
            digitalInput.flowValue = Qv;
            digitalInput.upperRange = sVMeasurementReplaceStaticFreqStaticDefault.QvRange.hiLim;
            digitalInput.unitType = UM_T_VOLUME_FLOW;  
            digitalInput.unitCode = sVMeasurementReplaceStaticFreqStaticDefault.QvUnitCode;
            break;
          case SELECTION_LIQUIDQN:
          case SELECTION_GASQN:
            if(opModeBit==SELECTION_LIQUIDQN)
            {
                Qn = mVMeasurement.CalculateLiquidQnEXE(Qv,temperature);
            }
            else
            {
                pressure= GetPressure();
                Qn = mVMeasurement.CalculateGasQnEXE(Qv,temperature,pressure);
            }
            result |=  totalizer.UpdateQnEXE(Qn,elapseTime);
            digitalInput.upperRange = mVMeasurementStaticFrequentStaticDefault.QnRange.hiLim;
            digitalInput.flowValue = Qn;
            digitalInput.unitType = UM_T_VOLUME_FLOW; 
            digitalInput.unitCode = mVMeasurementStaticFrequentStaticDefault.QnUnitCode;
            break;
          case SELECTION_LIQUIDQM:
          case SELECTION_GASQM:
          case SELECTION_STEAMQM:
            if(opModeBit==SELECTION_LIQUIDQM)
            {
                Qm= mVMeasurement.CalculateLiquidQmEXE(Qv,density, temperature);
            }
            else if(opModeBit==SELECTION_GASQM)
            {
                pressure= GetPressure();
                Qn = mVMeasurement.CalculateGasQnEXE(Qv,temperature,pressure);
                Qm = mVMeasurement.CalculateGasQmEXE(Qv,Qn,density);
                result |=  totalizer.UpdateQnEXE(Qn,elapseTime);
            }
            else
            {
                Qm = mVMeasurement.CalculateSteamQmEXE(Qv,density);
            }
            
            result |= totalizer.UpdateQmEXE(Qm,elapseTime);  
            digitalInput.upperRange = mVMeasurementStaticFrequentStaticDefault.QmRange.hiLim;
            
            digitalInput.flowValue = Qm;
            digitalInput.unitType = UM_T_MASS_FLOW;
            digitalInput.unitCode = mVMeasurementStaticFrequentStaticDefault.QmUnitCode;
            break;
          case SELECTION_LIQUIDPOWER:
          case SELECTION_GASPOWER:
          case SELECTION_STEAMPOWER:
            if(opModeBit==SELECTION_LIQUIDPOWER)
            {
                temperatureExt = GetT_External();
                Qm = mVMeasurement.CalculateLiquidQmEXE(Qv,density, temperature);
                result |= totalizer.UpdateQmEXE(Qm,elapseTime);
                Qpower =  mVMeasurement.CalculateLiquidQpEXE(Qm,temperature,temperatureExt);
                
            }
            else if(opModeBit==SELECTION_GASPOWER)
            {
                pressure= GetPressure();
                Qn = mVMeasurement.CalculateGasQnEXE(Qv,temperature,pressure);
                result |= totalizer.UpdateQnEXE(Qn,elapseTime); 
                Qpower =  mVMeasurement.CalculateGasQpEXE(Qn);
            }
            else
            {
                Qm = mVMeasurement.CalculateSteamQmEXE(Qv,density);// density from slow task
                Qpower =  mVMeasurement.CalculateSteamQpEXE(Qm);
                result = totalizer.UpdateQmEXE(Qm,elapseTime);
            }
            result |= totalizer.UpdateEnergyEXE(Qpower,elapseTime);
            digitalInput.upperRange = mVMeasurementStaticFrequentStaticDefault.QpRange.hiLim;
            digitalInput.flowValue = Qpower;
            digitalInput.unitType = UM_T_POWER;
            digitalInput.unitCode = mVMeasurementStaticFrequentStaticDefault.QpUnitCode;   
            break;
          case SELECTION_GASPARTIALQV:
            gasContent = GetGasContent();
            patialQv = mVMeasurement.CalculateGasQvPartialEXE(Qv,gasContent);
            result |= totalizer.UpdateQvPartialEXE(patialQv,elapseTime);
            digitalInput.upperRange = mVMeasurementStaticFrequentStaticDefault.QvPartialRange.hiLim;
            digitalInput.flowValue = patialQv;
            digitalInput.unitType = UM_T_VOLUME_FLOW;
            digitalInput.unitCode = sVMeasurementReplaceStaticFreqStaticDefault.QvUnitCode;
            break;
          case SELECTION_GASPARTIALQN:
            pressure= GetPressure();
            gasContent = GetGasContent();
            patialQv =  mVMeasurement.CalculateGasQvPartialEXE(Qv,gasContent);
            result |= totalizer.UpdateQvPartialEXE(patialQv,elapseTime);
            
            Qn = mVMeasurement.CalculateGasQnEXE(Qv,temperature,pressure);
            result |= totalizer.UpdateQnEXE(Qn,elapseTime);
            
            patialQn  =mVMeasurement.CalculateGasQnPartialEXE(Qn,gasContent);
            result |= totalizer.UpdateQnPartialEXE(patialQn,elapseTime);
            
            digitalInput.upperRange = mVMeasurementStaticFrequentStaticDefault.QnPartialRange.hiLim;
            digitalInput.flowValue = patialQn;
            digitalInput.unitType = UM_T_VOLUME_FLOW;
            digitalInput.unitCode = mVMeasurementStaticFrequentStaticDefault.QnUnitCode;
            break;
          default:
            break;
        }
        
        SetFlag(FLAG_MAIN_TASK, MAIN_TASK_MVM_FLAG);

        CheckFlag(FLAG_MAIN_TASK, MAIN_TASK_DO_FLAG);


#ifdef DIAGNOSIS_USE_CONFIGURABLE_BEHAVIOUR
        result |= diagnosis.GetNAMURStatusSRV(&diagnosisStatus);
        digitalInput.doAlarm = GetOutputStatusSRV(DIGITALOUTPUT_IDX,diagnosisStatus.classification);

#endif

        result |= mapper_Get(MAPPER_IDX_PV_PercentageForDO,WHOLE_OBJECT,&digitalInput.DoPercentage);
        result |= digitalOutput.UpdateEXE(&digitalInput);

#ifdef COMPILE_WITH_MODBUS_SUBSYSTEM
        modbus.UpdateScanRegister();
#endif

        SetFlag(FLAG_MAIN_TASK, MAIN_TASK_DO_FLAG);
        
        CheckFlag(FLAG_MAIN_TASK, SIZEOF_MAIN_TASK_FUNCTION_FLAG);
        ClearFlag(FLAG_MAIN_TASK);

        Delay_RTOS_TASK(MAIN_UPDATE_TIME);
    }
}
//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SENDCONFIGTOFE> \endif
 \brief  Send configuration parameters to FE
\if @SubGen end@ <METHODHEADER SENDCONFIGTOFE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SENDCONFIGTOFE>
void SendConfigToFE_CDT(void)
//@SubGen end@<METHOD SENDCONFIGTOFE>
{
    TINT16 result;    
    TUSIGN8 currentConfParam;
    static TUSIGN8 waitForVerify = 0;
    TUSIGN8 intTempType = CDT_TEMP_PT100;
    TUSIGN8 frontEndConfParamWr = 0;
    TUSIGN8 frontEndConfParamRd = 0;
    TUSIGN8 frontEndTempTypeRd =0;
    /* Send the meter configuration to sensor board */
    if(retrySendConfig)
    {
        //coordinatorUnprotected.frontEndConfParamWr = GetMeterParams();
        frontEndConfParamWr = GetMeterParams();
        (void)coordinator_Put((TUSIGN16)CDT_IDX_frontEndConfParamWr, (TINT16)WHOLE_OBJECT, &frontEndConfParamWr);
        result = frontEndInterface.PutCommand(CDT_WRITE_METER_TYPE,CDT_METER_TYPE_LENGTH,comWrite_cdt[CDT_WRITE_METER_TYPE_INDEX],NULL);
        if(result == WRONG_STATE_ERR)
        {
            retrySendConfig = eTRUE;
        }
        else
        {
            retrySendConfig = eFALSE;
            frontEndConfParamRd = 255;
            (void)coordinator_Put((TUSIGN16)CDT_IDX_frontEndConfParamRd, (TINT16)WHOLE_OBJECT, &frontEndConfParamRd);
            //coordinatorUnprotected.frontEndConfParamRd = 255;
            waitForVerify = 3;
        }
    }
    /* If the meter configuration is changed or read parameters is not equal to write parameters, 
    * retry to send the current configuration. */
    if(waitForVerify == 0)
    {
        (void)coordinator_Get((TUSIGN16)CDT_IDX_frontEndConfParamRd, (TINT16)WHOLE_OBJECT, &frontEndConfParamRd);
        (void)coordinator_Get((TUSIGN16)CDT_IDX_frontEndTempTypeRd, (TINT16)WHOLE_OBJECT, &frontEndTempTypeRd);
        lastFEConfParam = frontEndConfParamRd;
        currentConfParam = GetMeterParams();
        (void)coordinator_Get((TUSIGN16)CDT_IDX_intRTDType, (TINT16)WHOLE_OBJECT, &intTempType);
        if((currentConfParam!=lastFEConfParam) || (frontEndTempTypeRd!=intTempType))
        {
            retrySendConfig = eTRUE;
        }
    }
    else
    {
        /* Read the configuration from sensor board. */
        waitForVerify--;
        if(waitForVerify == 1)
        {
            result = frontEndInterface.PutCommand(CDT_READ_METER_TYPE,CDT_METER_TYPE_LENGTH,comRead_cdt[CDT_READ_METER_TYPE],NULL);
            if(result == WRONG_STATE_ERR)
            {
                waitForVerify = 3;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEWORKINGTIME> \endif
 \brief  Update total operation hours.
\if @SubGen end@ <METHODHEADER UPDATEWORKINGTIME> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEWORKINGTIME>
void UpdateWorkingTime_CDT(void)
//@SubGen end@<METHOD UPDATEWORKINGTIME>
{
    TUSIGN16 result = OK;
    TUSIGN32  alarmStatus = 0;
    TUSIGN32 workingTimeMSec;
    TUSIGN32 systemTime = (TUSIGN32)COORDINATOR_SYSTEM_TIME;
    TUSIGN32 maintenanceCyc;
    TUSIGN32 partialWorkingTimeHour=0;
    TUSIGN32 totalWorkingTimeHour;
    TUSIGN32 timeStamp = 0;
    TUSIGN32 deltaTime = 0;
    if(systemTime < lastTimeStamp)
    {
        // UpdateWorkingTime interval is SLOW_UPDATE_TIME
        deltaTime = SLOW_UPDATE_TIME;   
    }
    else
    {       
        deltaTime = systemTime - lastTimeStamp;
    }
    
    result |= coordinator_Get((TUSIGN16)CDT_IDX_workingTimeMSec, (TINT16)WHOLE_OBJECT, &workingTimeMSec);
    workingTimeMSec += deltaTime;
    
    // update FE time related 
    
    if(feTimeStamp>=CYCLIC_UPDATE_FE_TIME)
    {
        feTimeStamp = 0;
        //lint -e{732}{737} 2013-04-16 Lawrence Loss of sign is Ok
        result |= frontEndInterface.PutCommand(CDT_READ_OPERATION_HOUR,CDT_OPERATION_HOUR_LENGTH,comRead_cdt[CDT_READ_OPERATION_HOUR_INDEX],NULL);
        //lint -e{732}{737} 2013-04-16 Lawrence Loss of sign is Ok
        result |= frontEndInterface.PutCommand(CDT_READ_FE_MAINTAIN_CYCLE,CDT_FE_MAINTAIN_CYCLE_LENGTH,comRead_cdt[CDT_READ_FE_MAINTAIN_CYCLE_INDEX],NULL);
    }
    else
    {
        feTimeStamp += deltaTime;
    }
    
    result |= coordinator_Get((TUSIGN16)CDT_IDX_tranMaintenanceCyc, (TINT16)WHOLE_OBJECT, &maintenanceCyc);

    if(workingTimeMSec >= COORDINATOR_MSEC_PER_HOUR)
    {
        result |= coordinator_Get((TUSIGN16)CDT_IDX_totalWorkingTimeHour, (TINT16)WHOLE_OBJECT, &totalWorkingTimeHour);
        result |= coordinator_Get((TUSIGN16)CDT_IDX_partialWorkingTimeHour, (TINT16)WHOLE_OBJECT, &partialWorkingTimeHour);
        
        workingTimeMSec -= COORDINATOR_MSEC_PER_HOUR;
        totalWorkingTimeHour += 1;
        partialWorkingTimeHour +=1;
        
        if(maintenanceCyc > 0)
        {
            maintenanceCyc --;
        }
        else
        {
        }
        
        result |= coordinator_Put((TUSIGN16)CDT_IDX_tranMaintenanceCyc, (TINT16)WHOLE_OBJECT, &maintenanceCyc);
        result |= coordinator_Put((TUSIGN16)CDT_IDX_partialWorkingTimeHour, (TINT16)WHOLE_OBJECT, &partialWorkingTimeHour);
        result |= coordinator_Put((TUSIGN16)CDT_IDX_totalWorkingTimeHour, (TINT16)WHOLE_OBJECT, &totalWorkingTimeHour);
    }

    // always check the maintenance cycle to set or clear cb maintenance warning 
    result |= coordinator_Get(CDT_IDX_cdtDiagnosis, WHOLE_OBJECT, &alarmStatus);
    if(maintenanceCyc > 0)
    {
        alarmStatus &= ~((TUSIGN32)1<<CDT_ALARM_CB_MAINTENANCE);
    }
    else
    {
        alarmStatus |= ((TUSIGN32)1<<CDT_ALARM_CB_MAINTENANCE);
    }
    result |= coordinator_Put(CDT_IDX_cdtDiagnosis, WHOLE_OBJECT, &alarmStatus);
    
    result |= coordinator_Put((TUSIGN16)CDT_IDX_workingTimeMSec, (TINT16)WHOLE_OBJECT, &workingTimeMSec);
    // timeStamp set as 1/32seconds
    timeStamp = ((partialWorkingTimeHour%COORDINATOR_HOUR_PER_DAY)*COORDINATOR_SEC_PER_HOUR*32000) +(workingTimeMSec * 32);
    
    result |= coordinator_Put((TUSIGN16)CDT_IDX_timeStamp, (TINT16)WHOLE_OBJECT, &timeStamp);
    
    if(result == OK) 
    {
        lastTimeStamp = systemTime;
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  NV diagnosis update.
\if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_CDT(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
    TUSIGN8 i;
    TUSIGN16 result = OK;
    TUSIGN32 alarm = 0, setDiagnosis, clearDiagnosis,shift=0;
    TUSIGN32 alarmSimulation = 0; 
    TUSIGN8 inputDiagnosis;
    TUSIGN8 setAlarm = SET_ALARM;
    TUSIGN8 clearAlarm = CLEAR_ALARM;
    TUSIGN8 simulationMode = 0;
    TUSIGN8 diagnosisSimulationStatus;
    TUSIGN8 alarmBitSimulation = 0x00;
    
    (void)coordinator_Get((TUSIGN16)CDT_IDX_cdtDiagnosis, (TINT16)WHOLE_OBJECT, &alarm);
    
    inputDiagnosis = GetNvDiagnosisNV(ALL_FILES);
    if(inputDiagnosis == NV_ERROR)
    {
        alarm |= (TUSIGN32)1<<CDT_ALARM_CB_NV_ERROR;
    }  
    
    result = coordinator_Get((TUSIGN16)CDT_IDX_simulationMode, WHOLE_OBJECT, &simulationMode);
    if((result == (TUSIGN16)OK) && (simulationMode != CDT_SIMULATION_CB_OFF))
    {
        alarm |= ((TUSIGN32)1 << CDT_ALARM_CB_SIMUDATA);
    }
    SuspendAll_RTOS_TASK();
    if((coordinatorProtected.frequencySim_Enable==CDT_SIMULATION_ENABLE)||(coordinatorProtected.temperatureSim_Enable==CDT_SIMULATION_ENABLE)||\
        (sVMeasurementProtected.QvSimEnable==SVM_ENABLE)||(mVMeasurementProtected.QnSimEnable==MVM_ENABLE)||\
            (mVMeasurementProtected.QmSimEnable==MVM_ENABLE)||(mVMeasurementProtected.QpSimEnable==MVM_ENABLE)||\
                (mVMeasurementProtected.QvPartialSimEnable==MVM_ENABLE)||(mVMeasurementProtected.QnPartialSimEnable==MVM_ENABLE)||\
                    (mapperProtected.dvTExtSimEnable== MAPPER_SIMU_ENABLE)||(mapperProtected.dvPressureSimEnable==MAPPER_SIMU_ENABLE)||\
                        (mapperProtected.dvDensitySimEnable== MAPPER_SIMU_ENABLE)||(mapperProtected.dvContentSimEnable== MAPPER_SIMU_ENABLE))
    {
        alarm |= ((TUSIGN32)1 << CDT_ALARM_CB_SIMUDATA);
    }
    ResumeAll_RTOS_TASK();
     
    result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus, WHOLE_OBJECT, &diagnosisSimulationStatus);
    
    if((diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED) && (result == (TUSIGN16)OK))
    {
        for(i=0; i< CDT_ALARM_NUM_CODES; i++)  //change CDT_ALARM_NUM_CODES  
        {
            result = coordinator_Get((TUSIGN16)CDT_IDX_coodinatorAlarmSimulation, i, &alarmBitSimulation);
            
            if((result == OK) && (alarmBitSimulation == SET_ALARM)) 
            {
                alarmSimulation |= ((TUSIGN32)1<<i);
            }
        }
        alarm  = alarmSimulation;
        alarm |= (TUSIGN32)1<<CDT_ALARM_CB_ALARM;
    }
       
    setDiagnosis = alarm & ~cdtOldDiagnosis;
    clearDiagnosis = ~alarm & cdtOldDiagnosis;
    
    cdtOldDiagnosis = alarm;
    
    for(i = 0; i < CDT_ALARM_NUM_CODES; i++)
    {
        shift = (TUSIGN32)1<<i;
        if(setDiagnosis & shift) 
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarm, i, &setAlarm);
        }
        if(clearDiagnosis & shift)  
        {
            (void)diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_coordinatorAlarm, i, &clearAlarm);
        }
    }
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER FRONTENDTASKEXE> \endif
 \brief  Front end subsystem is executed in this path.
\if @SubGen end@ <METHODHEADER FRONTENDTASKEXE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 0% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD FRONTENDTASKEXE>
void FrontendTaskEXE_CDT(void)
//@SubGen end@<METHOD FRONTENDTASKEXE>
{
    while(1)
    {
#ifdef ENABLE_WATCHDOG
        /* Trigger Frontend task software watchdog */
        TriggerWatchdog_CDT(FrontendInterfaceTaskWatchdogBiteAction, TIMEOUT_WATCHDOG_SCHEDULE, 1);
#endif

        frontEndInterface.ProcessCmd();

        //only dummy delay, as simulation has no data from real Front-end HW
        Delay_RTOS_TASK(FE_UPDATE_TIME);
    }
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER HARTTASKEXE> \endif
 \brief  Hart subsystem is executed in this path.
\if @SubGen end@ <METHODHEADER HARTTASKEXE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 0% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HARTTASKEXE>
void HartTaskEXE_CDT(void)
//@SubGen end@<METHOD HARTTASKEXE>
{
    while(1)
    {
#ifdef ENABLE_WATCHDOG
        /* Trigger hart task software watchdog */
        TriggerWatchdog_CDT(HartTaskWatchdogBiteAction, TIMEOUT_WATCHDOG_SCHEDULE, 1);
#endif
        Take_RTOS_TASK_NOTIFY(RTOS_TRUE, HART_UPDATE_TIME);
        //Execute HART Task Rountine
        hart.HARTCommandInterpreterEXE();
        HWPL_LedChange(HWPL_led0, HWPL_ledToggle);

#ifdef COMPILE_WITH_WEBSERVER_SUBSYSTEM
        webServer.UpdateAlarmsEXE();
        HttpRedirect_Task();
        //DiagRegistry_Update();
#endif
    } 
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SLOWTASKEXE> \endif
 \brief  AI, Electronic services and NV subsystem are executed in this path.
\if @SubGen end@ <METHODHEADER SLOWTASKEXE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 0% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SLOWTASKEXE>
void SlowTaskEXE_CDT(void)
//@SubGen end@<METHOD SLOWTASKEXE>
{
    static TBOOL runFlag = eFALSE;
    TUSIGN8 operationMode = 0;
    TUSIGN16 opModeShift = 0;
    TUSIGN16 result = (TUSIGN16)OK;
    TFLOAT density = 998.0f;
    
    //InitResetCounter();
    while (1)
    {
#ifdef ENABLE_WATCHDOG
        /* Trigger slow task software watchdog */
        TriggerWatchdog_CDT(SlowTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
#endif

        CheckFlag(FLAG_SLOW_TASK, SLOW_TASK_MAPPER_FLAG);
        //result |= mapper.UpdateDeviceVariablesEXE();
        SetFlag(FLAG_SLOW_TASK, SLOW_TASK_MAPPER_FLAG);
        
        if(runFlag)
        {
            ExecuteNV();
            result |=  mapper_Get((TUSIGN16)MAPPER_IDX_dvDensity,(TINT16)WHOLE_OBJECT,&density);
            sVMeasurement.CalculateReEXE(density);
            result |= coordinator_Get((TUSIGN16)CDT_IDX_operatingMode,(TINT8)WHOLE_OBJECT,&operationMode);
            opModeShift = 1<<operationMode;
            
            if(opModeShift&SELECTION_LIQUIDQN)
            {
                mVMeasurement.UpdateLiquidQnMaxDN();
            }
            else if(opModeShift&SELECTION_GASPARTIALQV)
            {
                mVMeasurement.UpdateBiagasQvMaxDN();
            }
            else if(opModeShift & (SELECTION_GASQN|SELECTION_GASPARTIALQN))
            {
                mVMeasurement.CalcCompressFactor();
                mVMeasurement.UpdateGasQnMaxDN();
        
            }
            else if(opModeShift &(SELECTION_LIQUIDQM|SELECTION_GASQM|SELECTION_STEAMQM))
            {
                if(opModeShift &SELECTION_GASQM)
                {
                    mVMeasurement.CalcCompressFactor();
                    mVMeasurement.UpdateGasQnMaxDN();
                }
                mVMeasurement.UpdateQmMaxDN();
            }
            else if(opModeShift &SELECTION_LIQUIDPOWER)
            {
                mVMeasurement.UpdateLiquidPowerMaxDN();
            }
            else if(opModeShift &SELECTION_GASPOWER)
            {
                mVMeasurement.CalcCompressFactor();
                mVMeasurement.UpdateGasQnMaxDN();
                mVMeasurement.UpdateGasPowerMaxDN();
            }
            else if(opModeShift &SELECTION_STEAMPOWER)
            {
                mVMeasurement.UpdateSteamPowerMaxDN();
            }
            else
            {
            
            }
            
            analogInput.UpdateAnalogInput();
            elettronicServices.ElettornicServicesEXE();
            elettronicServices.CheckAutoZeroStatus();
            coordinator.UpdateFunctionPassword();
            // update Steam Enthalpy for SELECTION_STEAMPOWER mode
            result |= coordinator.UpdateSteamEnthalpy(opModeShift);
            runFlag = eFALSE;
        }
        else
        {
            CheckFlag(FLAG_SLOW_TASK, SLOW_TASK_OTHERDIA_FLAG);
            coordinator.UpdateWorkingTime();
            coordinator.UpdateMaxTemperature();
            frontEndInterface.UpdateDiagnosis();
            coordinator.UpdateDiagnosis(); 
            elettronicServices.UpdateDiagnosis();
            analogInput.UpdateDiagnosis();
            digitalOutput.UpdateDiagnosis();
            sVMeasurement.UpdateDiagnosisEXE();
            mVMeasurement.UpdateDiagnosisEXE();
            mapper.UpdateDiagnosis();
            totalizer.UpdateDiagnosisEXE();
            SetFlag(FLAG_SLOW_TASK, SLOW_TASK_OTHERDIA_FLAG);
            
            CheckFlag(FLAG_SLOW_TASK, SLOW_TASK_DIADIA_FLAG);
            result |= diagnosis.UpdateDeviceStatusEXE();
            SetFlag(FLAG_SLOW_TASK, SLOW_TASK_DIADIA_FLAG);
            
            /* Send the configuration parameters to front end. */
            coordinator.SendConfigToFE(); 
            runFlag = eTRUE;
            CheckFlag(FLAG_SLOW_TASK, SIZEOF_SLOW_TASK_FUNCTION_FLAG);
        }
        ClearFlag(FLAG_SLOW_TASK);
  
        Delay_RTOS_TASK(SLOW_UPDATE_TIME);
    }
    //lint -e{550} 2013-06-16 Lawrence "result"not accsee is Ok
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SERVICESTASKEXE> \endif
 \brief  HMI, Service port, Diagnosis update subsystem are executed in this path.
\if @SubGen end@ <METHODHEADER SERVICESTASKEXE> \endif
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 0% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVICESTASKEXE>
void ServicesTaskEXE_CDT(void)
//@SubGen end@<METHOD SERVICESTASKEXE>
{
    TFLOAT temperature;
    TBOOL hightempFlag = eFALSE;
    TUSIGN8 SimMode = 0;
    TUSIGN8 activeStatus = SERVICEPORT_UNINITIALIZED_STATUS;
    
    (void)servicePort.unit.Initialize(SERVICE_PORT_ME,(TUSIGN8)INIT_HARDWARE);
    
    while(1)
    {
      
        if(sendStateCnt_SP>0)
        {            
            sendStateCnt_SP++;
        }
       //this is running in both smart hmi and service tool mode    
        if((activeStatus==SERVICEPORT_SERVICE_STATUS)
        || (activeStatus==SERVICEPORT_SHMI_STATUS))
        {            
            servicePort.ServPortCommandInterpreterEXE();
        }
        
#ifdef ENABLE_WATCHDOG
        /* Trigger services task software watchdog */ 
        TriggerWatchdog_CDT(ServiceTaskWatchdogBiteAction,TIMEOUT_WATCHDOG_SCHEDULE,1);
#endif
        
        servicePort.ServPortUpdateActivationStatusSRV();
        activeStatus = servicePort.ServPortGetActivationStatusSRV();
       
        if(activeStatus == SERVICEPORT_SERVICE_STATUS)
        {//Service Port Mode
            //lint -e{746} Lawrence 2015-06-23
//            hmi.HMI_ReInitialiseLowLevelDetection_SRV();
//            servicePort.ServPortCommandInterpreterEXE();

            hightempFlag = eFALSE;
        }
        else 
        {//HMI Mode
            (void)elettronicServices_Get(ELETTRONICSERVICES_IDX_elettronicTemperature, WHOLE_OBJECT, &temperature);
            (void)coordinator_Get(CDT_IDX_simulationMode,0,&SimMode);
            if(((temperature > -20.0)&&(temperature < 70.0))||(SimMode == CDT_SIMULATION_CB_AMBIENT_TEMP))
            {
                if(hightempFlag == (TBOOL)eTRUE)
                {
                    SetPowerDio_SERVICEPORT(1);
                    hightempFlag = eFALSE;
                }
                (void)hmi.HMI_EXE(HMI_LOCAL_INSTANCE);
            }
            else
            {
                if(hightempFlag == (TBOOL)eFALSE)
                {
                    SetPowerDio_SERVICEPORT(0);
                    hightempFlag = eTRUE;
                }
            }
        }

        Delay_RTOS_TASK(SERVICE_UPDATE_TIME);
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Modbus TCP task
 \author    Michal Kochel
 \date      2019-07-22 
 \param     arg pointer to arguments
 \return    void
 \warning 
 \test 
 \n test-date:          2019-MM-DD
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
#ifdef COMPILE_WITH_MODBUS_SUBSYSTEM
static void ModbusTask(void* arg) 
{
    (void)arg;  // not used
    while (1)
    {
        modbus.TaskEXE();
        // HWPL_LedChange(HWPL_led0, HWPL_ledToggle);
        Delay_RTOS_TASK(MODBUS_UPDATE_TIME);
    }
}
#endif
//-------------------------------------------------------------------------------------------------
/*!
\brief  Get meter configuration parameter.
\author Lawrence Shi
\date  2012-11-05
\param void
\return void
\warning
\test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 GetMeterParams(void)
{
    TUSIGN16 operatingMode = 0;
    TUSIGN8 meterSize = 0;
    TUSIGN8 convertType = 0;
    TUSIGN8 calNum = 0;
    TUSIGN8 steamType =  0xFF;
    (void)coordinator_Get((TUSIGN16)CDT_IDX_converterType,(TINT16)0,&convertType);
    if(convertType == CDT_SWIREL)
    {
        calNum = SWIRLTYPENUM;
        (void)coordinator_Get((TUSIGN16)CDT_IDX_swirlmeterSize,0,&meterSize);
    }
    else
    {
        calNum = VOTEXTYPENUM;
        (void)coordinator_Get((TUSIGN16)CDT_IDX_vortexmeterSize,0,&meterSize);
    }
    (void)coordinator_Get((TUSIGN16)CDT_IDX_operatingMode,0,&operatingMode);
    
    if(operatingMode <= CDT_LIQUID_POWER)
    {
        calNum += LIQUIDTYPENUM;
    }
    else if(operatingMode <= CDT_BIOGAS_STANDARD_VOLUME)
    {
        calNum += GASTYPENUM;
    }
    else 
    {        
        (void)mVMeasurement_Get((TUSIGN16)MVM_IDX_steamType,(TINT16)WHOLE_OBJECT,&steamType); 
        if(steamType == MVM_STEAMTYPE_WATER)
        {
            calNum += LIQUIDTYPENUM;
        }
        else
        {
            calNum += STEAMTYPENUM;
        }             
    }
    calNum += meterSize;
    return calNum;
}




#ifdef CPU_LOAD_TEST
void CPULoadTaskEXE(void)
{
    TUSIGN32 loadCnt = 0;
    TUSIGN32 initTime,endTime = 0;
    TUSIGN32 noLoadTime = 0;
    TUSIGN32 fullLoadTime = 0;
    TUSIGN8 loadFlag = 1;
    P10_bit.P10_1 = 1;
    osThreadSetPriority(MainTskHandle, osPriorityLow);
    osThreadSetPriority(HartTskHandle, osPriorityLow);
    osThreadSetPriority(FrontendTskHandle, osPriorityLow);
    osThreadSetPriority(ServicesTskHandle, osPriorityLow);
    osThreadSetPriority(SlowTskHandle, osPriorityLow);
    loadFlag = 1;    
    while(1)
    {
        initTime = GetTickCount_RTOS_TASK();
        while(1)
        {
            loadCnt ++;
            if(loadCnt >= 0xCCCC8)
            {
                P10_bit.P10_1 = 0;  
                break;
            }
        }          
        endTime = osKernelGetSysTimerCount();
        loadCnt = 0;
        if(loadFlag == 1)
        {
            //Get idle time
            noLoadTime = endTime - initTime;
            osThreadSetPriority(MainTskHandle,MAIN_TASK_PRIORITY);
            osThreadSetPriority(HartTskHandle, HART_TASK_PRIORITY);       
            osThreadSetPriority(FrontendTskHandle, FRONTEND_TASK_PRIORITY);
            osThreadSetPriority(ServicesTskHandle, SERVICES_TASK_PRIORITY);
            osThreadSetPriority(SlowTskHandle, SLOW_TASK_PRIORITY);  
            loadFlag = 0;
        } 
        else
        {
            //Get full load time
            fullLoadTime = endTime - initTime;
            coordinatorDynamic.cpuLoad = (TFLOAT)(fullLoadTime - noLoadTime)*100 / fullLoadTime;
        }               
    }
}
#endif


CONSTRUCTOR_T_ATRLIST(coordinatorFreq) =
{
    (TUSIGN16)CDT_IDX_frequency_Damped_Obj,     (TINT16) ATTRIB_2,
    (TUSIGN16)CDT_IDX_frequency_UnitCode,       (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_UnitType,       (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_Classification, (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_Status,         (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_DampingTime,    (TINT16)WHOLE_OBJECT,
    
    (TUSIGN16)CDT_IDX_frequency_range_Obj,      (TINT16)ATTRIB_2,
    (TUSIGN16)CDT_IDX_frequency_range_Obj,      (TINT16)ATTRIB_3,
    (TUSIGN16)CDT_IDX_frequency_Percentage,     (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_range_Obj,      (TINT16)ATTRIB_4,
    
    (TUSIGN16)CDT_IDX_frequency_MaxDN,          (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_MinDN,          (TINT16)WHOLE_OBJECT,
    (TUSIGN16)CDT_IDX_frequency_range_Obj,      (TINT16)ATTRIB_4,
    (TUSIGN16)CDT_IDX_frequency_Obj,            (TINT16)ATTRIB_2,
};


const T_SELECTION coordinatorFreqSelection = ALL_SELECTIONS_ENABLE;


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
\param -
\return -
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
