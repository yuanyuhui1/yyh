//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/assert.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/common_type.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"

#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "HART/Interface/Hart.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement.h"

#include "Hmi/Interface/HMI.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "ARM/Interface/arm.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Mapper/Interface/Mapper.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "ServicePort/Interface/servicePort.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "Totalizer/Interface/Totalizer.h"
#include "DigitalOutput/Interface/DigitalOutput.h"
#include "Ethernet/Interface/Ethernet.h"
#include "Modbus/Interface/Modbus.h"
#include "WebServer/Interface/WebServer.h"
//-------------------------------------------------------------------------------------------------
/*!
\brief  get subsystem ptr througth index
\author Lawrence Shi
\date  2012-11-05
\param  subsystem index
\return subsystem ptr
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
const T_UNIT SLOW* GetSubsystemPtr(T_SUBSYSTEM_IDX subsysIdx)
{
  const T_UNIT SLOW* unitPtr = 0;
  switch(subsysIdx)
  {
    //add one case if you implement a new subsystem
    case CDT_IDX:
        unitPtr = COORDINATOR_ME;
        break;
    case AI_IDX:
        unitPtr = ANALOG_INPUT_ME;
        break;
    case ARM_IDX:
        unitPtr = ARM_ME;
        break;
    case SVM_IDX:
        unitPtr = SVMEASUREMENT_ME;
        break;
    case MVM_IDX:
        unitPtr = MVMEASUREMENT_ME;
        break;
    case DIGITALOUTPUT_IDX:
        unitPtr = DIGITAL_OUTPUT_ME;
        break;
    case HART_IDX:
        unitPtr = HART_ME;
        break;
    case DIAG_IDX:
        unitPtr = DIAGNOSIS_ME;
        break;
    case MAPPER_IDX:
        unitPtr = MAPPER_ME;
        break;
    case FE_IDX:
        unitPtr = FRONT_END_INTERFACE_ME;
        break;
    case ELETTRONICSERVICES_IDX:
        unitPtr = ELETTRONIC_SERVICES_ME;
        break;
    case TOTALIZER_IDX:
        unitPtr = TOTALIZER_ME;
        break;
    case SERVICEPORT_IDX:
        unitPtr = SERVICE_PORT_ME;
        break;
    case HMI_IDX:
        unitPtr = HMI_ME;
        break;
    case ETH_IDX:
        unitPtr = ETHERNET_ME;
        break;
    case MODBUS_IDX:
        unitPtr = MODBUS_ME;
        break;
    case WEBSRV_IDX:
        unitPtr = WEB_SERVER_ME;
        break;
    default:
    VIP_ASSERT(0);
  }
  VIP_ASSERT(unitPtr);
  return(unitPtr);
}