//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System          Common Framework
 Module          System Main
 Description

 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/assert.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/Coordinator.h"
#include "stm32h7xx_hal.h"
#include "HWPL_system_clock.h"
#include "HWPL_hal_timebase_tim.h"
#include "System/Interface/stm32h7xx_it.h"
#include "HWPL_time.h"
#include "IO_app_info.h"

//--------------------------------------------------------------------------------------------------
/*!
 \brief
        main !
 \author
 \date
 \param
 \return
 \warning
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
#pragma default_function_attributes = @ ".fixed_main"   // force main function to fixed location
void main(void)
{
  /* Enable I-Cache */
  // SCB_EnableICache();
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  (void)HWPL_TimeInit();

  (void)coordinator.unit.Initialize(COORDINATOR_ME, INIT_HARDWARE);

  (void)coordinator.unit.Initialize(COORDINATOR_ME, INIT_TASKS);

  /* Start scheduler */
  vTaskStartScheduler();
}
/* release placement attribute */
#pragma default_function_attributes =