//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      UHTE
 Module      WatchDog
 Description implementation of Subsystem "WatchDog"
 Remarks
 Author:     Heiko Kresse

\brief       Ported from UHTE project with some functions added. Xu Fei. 2011-10
*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/Coordinator.h"
#include "coordinator_execute.h"

#include "Coordinator/Interface/watchdog.h"
#include "Coordinator/Interface/watchdog_bitelist.h"

RTOS_TASK_HANDLE WatchdogTskHandle;
#define WATCHDOG_TASK_STACK_SIZE                 (1 * RTOS_TASK_STACK_SZMIN)
#define WATCHDOG_TASK_PRIORITY                   (RTOS_TASK_PRIO_IDLE + 7)

static TBOOL mInitialized=eFALSE;
static TUSIGN16 mAryWatchdogTimer[LENGTH_OF_BITEACTIONLIST];
static TUSIGN16 mAryWatchdogInfo[LENGTH_OF_BITEACTIONLIST];
static TUSIGN16 watchdogSchedule = DEFAULT_WATCHDOG_SCHEDULE;
static void WatchdogTask(void);

// global bite action list
//LENGTH_OF_BITEACTIONLIST  shall be equal to SIZEOF_WATCH_DOG_TASK_FUNCTION_FLAG
const T_BITEACTION uBiteActionList[LENGTH_OF_BITEACTIONLIST] =
{
	MainTaskWatchdogBiteAction,
	HartTaskWatchdogBiteAction,
	ServiceTaskWatchdogBiteAction,
	SlowTaskWatchdogBiteAction,
    FrontendInterfaceTaskWatchdogBiteAction,
    //CPULoadTaskWatchdogBiteAction,
};

// global bite action enable list
TUSIGN8 uBitEnableList[LENGTH_OF_BITEACTIONLIST] =
{
	0,//MainTaskWatchdogBiteAction,
	0,//HartTaskWatchdogBiteAction,
	0,//ServiceTaskWatchdogBiteAction,
	0,//SlowTaskWatchdogBiteAction,
    0,//FrontendInterfaceTaskWatchdogBiteAction,
    //0,CPULoadTaskWatchdogBiteAction,
};

void InitHardwareWatchdog(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    /* Enable GPIO Clock */
    ENABLE_WATCHDOG_GPIO_CLK_ENABLE();
    KICK_WATCHDOG_GPIO_CLK_ENABLE();

    /* Configure IO in output push-pull mode to drive external Hardware watchdog */
    GPIO_InitStruct.Pin = ENABLE_WATCHDOG_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ENABLE_WATCHDOG_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KICK_WATCHDOG_GPIO_PIN;
    HAL_GPIO_Init(KICK_WATCHDOG_GPIO_PORT, &GPIO_InitStruct);

#ifdef ENABLE_WATCHDOG
    /* Enable Haradware watchdog and kick the watchdog */
    HAL_GPIO_WritePin(ENABLE_WATCHDOG_GPIO_PORT, ENABLE_WATCHDOG_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KICK_WATCHDOG_GPIO_PORT, KICK_WATCHDOG_GPIO_PIN, GPIO_PIN_RESET);
#else
    /* Disable Haradware watchdog */
    HAL_GPIO_WritePin(ENABLE_WATCHDOG_GPIO_PORT, ENABLE_WATCHDOG_GPIO_PIN, GPIO_PIN_SET);
#endif
}

void TriggerHardwareWatchdog(void)
{
#ifdef ENABLE_WATCHDOG
    HAL_GPIO_TogglePin(KICK_WATCHDOG_GPIO_PORT, KICK_WATCHDOG_GPIO_PIN);
#else
    /* The watchdog is disabled, no need to kick watchdog */
#endif
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief     public; Initialize Master-Watchdog-task
 \author    Heiko Kresse
 \date      2004-07-16
 \param     -
 \return    -
 \warning
 \test
 \test
    <pre>
    2004-09-13 - Heiko Kresse
    Modultest OK
    Lint level 3 OK
    </pre>
 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitializeMasterWatchDog_CDT(void)
{
    TUSIGN16 u = 0;
#ifndef NDEBUG
    TUSIGN16 v = 0;
#endif
    // initialize Watchdog-timer and Watchdog-info
    for( u=0; u<LENGTH_OF_BITEACTIONLIST; u++)
    {
        mAryWatchdogTimer[u]=0xFFFF;
        mAryWatchdogInfo[u]=0;

#ifndef NDEBUG
        for( v=u+1; v<LENGTH_OF_BITEACTIONLIST; v++)
        {
            //lint -e{746} Heiko Kresse 2004-11-08
            VIP_ASSERT(uBiteActionList[u]!=uBiteActionList[v]); //! sharing of a biteaction --> error  ??Lint 1.intrinsic??
        }
#endif
    }

    VIP_ASSERT(!mInitialized);

    if(Create_RTOS_TASK((TaskFunction_t)WatchdogTask, "WatchdogTask", WATCHDOG_TASK_STACK_SIZE,
                        NULL, WATCHDOG_TASK_PRIORITY, &WatchdogTskHandle) != RTOS_PASS)
    {
    }

    mInitialized=eTRUE;
}




//--------------------------------------------------------------------------------------------------
/*!
 \brief     public; triggers the watchdog addressed by the BiteAction-Pointer
 \author    Heiko Kresse
 \date      2004-07-16
 \param     BiteAction Pointer to the action that will be called if watchdog bites
 \param     ms watchdog-time in ms; 0==65536ms!
 \param     info parameter for BiteAction, e.g. sourcecode-position
 \return    -
 \warning
 \test
    <pre>
    2004-09-13 - Heiko Kresse
    modultest OK
    lint level 3 OK
    </pre>
 \bug
*/
//--------------------------------------------------------------------------------------------------
void TriggerWatchdog_CDT(T_BITEACTION BiteAction, TUSIGN16 ms, TUSIGN16 info)
{
    TUSIGN16 wdIdx = 0;

    VIP_ASSERT(mInitialized);
    VIP_ASSERT(BiteAction);

    // find watchdog-index
    for( wdIdx=0;
        (wdIdx<LENGTH_OF_BITEACTIONLIST) && (uBiteActionList[wdIdx]!=BiteAction);
         wdIdx++)
    {
    }

    // unknown biteaction --> error
    VIP_ASSERT(wdIdx<LENGTH_OF_BITEACTIONLIST);

    // the following operations must not be interrupted by watchdog-task
    SuspendAll_RTOS_TASK();
    mAryWatchdogTimer[wdIdx]=ms;
    mAryWatchdogInfo[wdIdx]=info;
    ResumeAll_RTOS_TASK();
}




//--------------------------------------------------------------------------------------------------
/*!
 \brief     privat; the task that handle the watchdog-list and retriggers the external watchdog.
            duration = 60�s per watchdog (BCLK=1.843MHz)
            trigger and task are not synchronized, so the time for the first count varies between
            0ms and WATCHDOG_SCHEDULE!
 \author    Heiko Kresse
 \date      2004-07-16
 \param     -
 \return    -
 \warning
 \test
    <pre>
    2004-09-13 - Heiko Kresse
    modultest OK
    lint level 3 OK
    </pre>
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void WatchdogTask(void)
{
    TUSIGN8 wdIdx = 0;

    while(1) // OK for TASK
    {
        TriggerHardwareWatchdog(); // external hw-watchdog and depending to cpu also the internal

        // realize schedule;
        Delay_RTOS_TASK(watchdogSchedule);
        
        for(wdIdx=0; wdIdx<LENGTH_OF_BITEACTIONLIST; wdIdx++)
        {
            CheckFlag((TUSIGN8)FLAG_WATCH_DOG_TASK, wdIdx); 
            if( mAryWatchdogTimer[wdIdx]<watchdogSchedule )
            {
                // Watchdog will bite again in 1000ms
                mAryWatchdogTimer[wdIdx] = TIMEOUT_WATCHDOG_SCHEDULE;
                // bite if enabled.
                if(uBitEnableList[wdIdx] != 0)
                {
                    uBiteActionList[wdIdx](mAryWatchdogInfo[wdIdx]);
                }

            }
            else
            {
                mAryWatchdogTimer[wdIdx]-=watchdogSchedule;
            }
            SetFlag((TUSIGN8)FLAG_WATCH_DOG_TASK, wdIdx); 
        }
        
        CheckFlag((TUSIGN8)FLAG_WATCH_DOG_TASK, LENGTH_OF_BITEACTIONLIST); //LENGTH_OF_BITEACTIONLIST  shall be equal to SIZEOF_WATCH_DOG_TASK_FUNCTION_FLAG
        ClearFlag((TUSIGN8)FLAG_WATCH_DOG_TASK);
    }
}



//------------------------------------------------------------------------------
/*!
\brief      Set the watchdog shedule according to the system's requirement.
            If not called, the DEFAULT_WATCHDOG_SCHEDULE will be used.
\param
\return
\warning
\test
\n test-date: 2011-10-18
\n by: Xu Fei
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:
\bug
*/
//------------------------------------------------------------------------------
void SetWatchdogSchedule(TUSIGN16 newWatchdogSchedule)
{
	watchdogSchedule = newWatchdogSchedule;
}

//------------------------------------------------------------------------------
/*!
\brief    enable softwareWatchdog         
\param
\return
\warning
\test
\n test-date: 2011-10-18
\n by: Xu Fei
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:
\bug
*/
//------------------------------------------------------------------------------
void softwareWatchdogEnable(void)
{
    TUSIGN8 wdIdx = 0;
    SuspendAll_RTOS_TASK();
    for(wdIdx=0;wdIdx<LENGTH_OF_BITEACTIONLIST;wdIdx++)
    {
        uBitEnableList[wdIdx] = 1;
        mAryWatchdogTimer[wdIdx] = TIMEOUT_WATCHDOG_SCHEDULE;
    }
    ResumeAll_RTOS_TASK();
}


//------------------------------------------------------------------------------
/*!
\brief This function will be called at the highest priority
\param  info
\return -
\warning
\test
\n test-date: 2011-10-17
\n by: Xu Fei
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:
\bug
*/
//------------------------------------------------------------------------------
void MainTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}

void ServiceTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}

void SlowTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}

void HartTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}

void CPULoadTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}

//------------------------------------------------------------------------------
/*!
\brief      When frontendinterface task is stopped, do something
\param      info
\return     -
\warning
\test
\n test-date: 2011-10-18
\n by: Xu Fei
\n environment: C-SPY
\n intention: Boundary value analysis
\n result module test: Passed
\n result Lint Level 3:
\bug
*/
//------------------------------------------------------------------------------
void FrontendInterfaceTaskWatchdogBiteAction(TUSIGN16 info)
{
    info = info;
    VIP_ASSERT(0);
}