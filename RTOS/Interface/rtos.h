//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2018.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                 4WCTW
 Module                 RTOS macro abstraction layer
 Description            provides macros to abstract artefacts of the specific RTOS
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef RTOS_H_INCLUDED        /* Avoid multiple inclusion          */
#define RTOS_H_INCLUDED

/* include the specific RTOS */
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"


/* Begin of abstractive definitions */

/* types and constants */
#define RTOS_I32                    TINT32
//
#define RTOS_TRUE                   pdTRUE
#define RTOS_PASS                   pdPASS
#define RTOS_FALSE                  pdFALSE
#define RTOS_FAIL                   pdFAIL
//
#define RTOS_TICKTYPE               TickType_t
#define RTOS_BASETYPE               BaseType_t
#define RTOS_UBASETYPE              UBaseType_t
#define RTOS_STACKTYPE              StackType_t
#define RTOS_WAIT_NEVER             0
#define RTOS_WAIT_FOREVER           portMAX_DELAY
#define RTOS_TICK_PERIOD_MS         portTICK_PERIOD_MS
#define RTOS_TICKS_FROM_MS(ms)      ((ms) * portTICK_PERIOD_MS)
//
#define RTOS_TASK_PRIO_IDLE         tskIDLE_PRIORITY
#define RTOS_TASK_PRIO_MAX          configMAX_PRIORITIES
#define RTOS_TASK_NAME_MAXLEN       configMAX_TASK_NAME_LEN     
#define RTOS_TASK_STACK_SZMIN       configMINIMAL_STACK_SIZE
#define RTOS_TASK_HANDLE            TaskHandle_t
#define RTOS_QUEUE_HANDLE           QueueHandle_t 
#define RTOS_MUTEX_HANDLE           SemaphoreHandle_t 
#define RTOS_EVENT_HANDLE           EventGroupHandle_t
#define RTOS_EVENT_BITS             EventBits_t 

#define RTOS_STATIC_TASK            StaticTask_t


#define RTOS_TASK_NOTIFICATION_VALUE        uint32_t
#define RTOS_TASK_NOTIFY_ACTION_NONE        eNoAction
#define RTOS_TASK_NOTIFY_ACTION_SETBITS     eSetBits
#define RTOS_TASK_NOTIFY_ACTION_INCREMENT   eIncrement
#define RTOS_TASK_NOTIFY_ACTION_FORCEVAL    eSetValueWithOverwrite 
#define RTOS_TASK_NOTIFY_ACTION_PUSHVAL     eSetValueWithoutOverwrite

#define RTOS_INTERRUPT_PRIO_SYSCALL SYSCALL_PRIO
#define RTOS_INTERRUPT_PRIO_BITS    configPRIO_BITS
#define RTOS_INTERRUPT_PRIO_SET( value4bit )    ( value4bit << (8 - RTOS_INTERRUPT_PRIO_BITS))
//#define RTOS_INTERRUPT_PRIO_MAX     configMAX_API_CALL_INTERRUPT_PRIORITY

/* mutual exclusions */
//-------------------------------------------------------------------------------------------------
#define Create_RTOS_MUTEX(handle)                                   (handle) = xSemaphoreCreateMutex()
#define GetHolder_RTOS_MUTEX(handle, taskHandle)                    (taskHandle) = xSemaphoreGetMutexHolder(handle)
#define Take_RTOS_MUTEX(handle, timeout)                            xSemaphoreTake( (handle), (timeout) )
#define Give_RTOS_MUTEX(handle)                                     xSemaphoreGive( handle )
#define CreateRecursive_RTOS_MUTEX(handle)                          (handle) = xSemaphoreCreateRecursiveMutex()
#define TakeRecursive_RTOS_MUTEX(handle, timeout)                   xSemaphoreTakeRecursive( (handle), (timeout) )
#define GiveRecursive_RTOS_MUTEX(handle)                            xSemaphoreGiveRecursive(handle)
// freeRTOS version >=9.
#ifdef StaticSemaphore_t
    #define CreateRecursive_RTOS_MUTEX_STATIC(handle, sbuffer)      (handle) = xSemaphoreCreateMutexStatic(sbuffer)
#endif

/* scheduler related */
//-------------------------------------------------------------------------------------------------
#define Delay_RTOS_TASK(ticks)                              vTaskDelay( ticks )
#define DelayUntil_RTOS_TASK( prevWakeTime, timeIncrement)  vTaskDelayUntil( prevWakeTime, timeIncrement )
#define GetTickCount_RTOS_TASK(na)                          xTaskGetTickCount()
#define GetTickCountAtISR_RTOS_TASK(na) xTaskGetTickCountFromISR()

/* event groups */
//-------------------------------------------------------------------------------------------------
#define Create_RTOS_EVENT(handle)                       (handle) = xEventGroupCreate()
#define Delete_RTOS_EVENT(handle)                       vEventGroupDelete(handle)
#define SetBits_RTOS_EVENT(handle, setbits, ...)        __VA_ARGS__ xEventGroupSetBits( (handle), (setbits) )
#define SetBitsISR_RTOS_EVENT(handle, setbits, pContextSwitch, ...) __VA_ARGS__ xEventGroupSetBitsFromISR( (handle), (setbits), (pContextSwitch))
#define WaitBits_RTOS_EVENT(handle,waitbits, bClrDone,bAll,timeout, ...) \
                                                        __VA_ARGS__ xEventGroupWaitBits((handle),(waitbits),(bClrDone),(bAll),(timeout))
//
#ifdef StaticEventGroup_t
    #define Create_RTOS_EVENT_STATIC(pEvent)                xQueueCreateStatic(pEvent)
#endif /* StaticEventGroup_t */


/* queue functions */
//-------------------------------------------------------------------------------------------------
#define Create_RTOS_QUEUE(handle, length, itemSize)         (handle) = xQueueCreate( (length), (itemSize) )
#define Peek_RTOS_QUEUE(handle, pItem, timeout)             xQueuePeek( (handle), (pItem), (timeout) )
#define IsFullISR_RTOS_QUEUE(handle)                        xQueueIsQueueFullFromISR( handle )
#define SendBack_RTOS_QUEUE(handle, pItem, timeout)         xQueueSendToBack((handle), (pItem), (timeout))
#define SendFront_RTOS_QUEUE(handle, pItem, timeout)        xQueueSendToFront((handle), (pItem), (timeout))
#define Send_RTOS_QUEUE(handle, pItem, timeout)             xQueueSend((handle), (pItem), (timeout))
#define SendBackISR_RTOS_QUEUE(handle, pItem, pContextSwitch)   xQueueSendToBackFromISR((handle), (pItem), (pContextSwitch))
#define SendFrontISR_RTOS_QUEUE(handle, pItem, pContextSwitch)  xQueueSendToFrontFromISR((handle), (pItem), (pContextSwitch))
#define SendISR_RTOS_QUEUE(handle, pItem, pContextSwitch)   xQueueSendFromISR((handle), (pItem), (pContextSwitch))
#define Receive_RTOS_QUEUE(handle, pItem, timeout)          xQueueReceive( (handle),(pItem), (timeout))
#define Overwrite_RTOS_QUEUE(handle, pItem)                 xQueueOverwrite((handle), (pItem))
#define OverwriteISR_RTOS_QUEUE(handle, pItem, pContextSwitch)  xQueueOverwriteFromISR((handle), (pItem),(pContextSwitch))
#define GetCountWaitingMessages_RTOS_QUEUE(handle)          uxQueueMessagesWaiting(handle)
#ifdef StaticQueue_t
    #define Create_RTOS_QUEUE_STATIC(length, itemSize, pBuffer, pQueue) \
                                                            xQueueCreateStatic( (length), (itemSize), (pBuffer), (pQueue) )
#endif /* StaticQueue_t */


/* task functions */
//-------------------------------------------------------------------------------------------------
//
// name: up to RTOS_TASK_NAME_MAXLEN - 1 characters are used
// stackDepth: stack size in words (16/32bit), choose multiple of RTOS_TASK_STACK_SZMIN
// priority: choose 0 up to RTOS_TASK_PRIO_MAX - 1
#define Create_RTOS_TASK(code, name, stackDepth, parameters, priority, handle ) \
                                                xTaskCreate( code, name, stackDepth, parameters, priority, handle )
#define GetPriority_RTOS_TASK(handle)           uxTaskPriorityGet( handle )
#define SetPriority_RTOS_TASK(handle, newPrio)  vTaskPrioritySet( (handle), (newPrio) )
#define Delete_RTOS_TASK(handle)                vTaskDelete( handle )
#define GetHandleCurrent_RTOS_TASK(na)          xTaskGetCurrentTaskHandle()
#define GetName_RTOS_TASK(handle)               pcTaskGetName( handle )
#define GetStackFree_RTOS_TASK(handle)          uxTaskGetStackHighWaterMark( handle )
#define HookStackOverflow_RTOS_TASK( handle, name )  \
                                                vApplicationStackOverflowHook( handle, name )
#define Suspend_RTOS_TASK(handle)               vTaskSuspend( handle )
#define Resume_RTOS_TASK(handle)                vTaskResume( handle )
#define ResumeTaskISR_RTOS_TASK(handle)         xTaskResumeFromISR( handle )
#define SuspendAll_RTOS_TASK(na)                vTaskSuspendAll()
#define ResumeAll_RTOS_TASK(na)                 xTaskResumeAll()
#define EnterCritical_RTOS_TASK(na)             taskENTER_CRITICAL()
#define ExitCritical_RTOS_TASK(na)              taskEXIT_CRITICAL()
#ifdef RTOS_STATIC_TASK
    #define Create_RTOS_TASK_STATIC(code, name, stackDepth, parameters, priority, pStack, pTCB ) \
                                                xTaskCreateStatic( code, name, stackDepth, parameters, priority, pStack, pTCB )
#endif  /* RTOS_STATIC_TASK */

/* task notification */
//-------------------------------------------------------------------------------------------------
// waits, with an optional timeout, for the calling task to receive a notification. 
#define Wait_RTOS_TASK_NOTIFY( clrBitsEntry, clrBitsExit, pNotificationVal, timeout, ...)  \
                                                __VA_ARGS__ xTaskNotifyWait( (clrBitsEntry), (clrBitsExit), \
                                                                            (pNotificationVal), (timeout) )
// waits, with an optional timeout, for the calling task to receive a notification, use for simple bool notify, faster.
#define Take_RTOS_TASK_NOTIFY( clearCountOnExit, timeout, ...)  \
                                                __VA_ARGS__ ulTaskNotifyTake((clearCountOnExit), (timeout))
// used to send an event directly to and potentially unblock an RTOS task, and optionally update the receiving task's
// notification value in one of the following ways: Write 32-bit number, increment, Set one or more bits, none
#define Notify_RTOS_TASK_NOTIFY( TaskHandle, value, eAction, ... )   \
                                                __VA_ARGS__ xTaskNotify((TaskHandle), (value), (eAction) )
// a version of Notify_RTOS_TASK_NOTIFY() that can be called from an ISR. 
#define NotifyISR_RTOS_TASK_NOTIFY( TaskHandle, value, eAction, pContextSwitch, ... )   \
                                                __VA_ARGS__ xTaskNotifyFromISR((TaskHandle), (value), (eAction), (pContextSwitch) )
// used to send a simple notification to task, the function can ben called from an ISR
#define NotifyISR_Give_RTOS_TASK_NOTIFY( TaskHandle, pContextSwitch, ... )      \
                                                __VA_ARGS__ vTaskNotifyGiveFromISR((TaskHandle), (pContextSwitch))
// used to clear (Pending to Not Pending) a pending notification without the receiving task first needing to read its notification value. 
#define ClearState_RTOS_TASK_NOTIFY( TaskHandle, ... )  \
                                                __VA_ARGS__ xTaskNotifyStateClear( TaskHandle )
//-----------------------------------------------------------------------------------
// definitions used by the framework.
//-----------------------------------------------------------------------------------
#ifndef TYPE_OF_DATA_PROTECTION
/* define TYPE_OF_DATA_PROTECTION in the compiler command line or in the next line. */
/* 2Wire specific, use OS_EnterRegion instead of TakeRecursive_RTOS_MUTEX() for T_UNIT & T_DATA_OBJ */
/* 0=semaphore, 1=Region, >1 your own definitions. */
#define TYPE_OF_DATA_PROTECTION    0
#endif

/* using recursive mutex from FreeRTOS */
#if (TYPE_OF_DATA_PROTECTION == 0) 
    typedef RTOS_MUTEX_HANDLE T_DATA_SEMAPHORE;
    #define INITIALIZE_DATA_PROTECTION(x)  CreateRecursive_RTOS_MUTEX(x)
    #define ENABLE_DATA_PROTECTION(x)      TakeRecursive_RTOS_MUTEX(x, RTOS_WAIT_FOREVER)
    #define DISABLE_DATA_PROTECTION(x)     GiveRecursive_RTOS_MUTEX(x)
/* Suspends the scheduler */
#elif (TYPE_OF_DATA_PROTECTION == 1) 
    typedef RTOS_MUTEX_HANDLE T_DATA_SEMAPHORE;
    #define INITIALIZE_DATA_PROTECTION(x)
    #define ENABLE_DATA_PROTECTION(x)      SuspendAll_RTOS_TASK()
    #define DISABLE_DATA_PROTECTION(x)     ResumeAll_RTOS_TASK()
#else
#error Undefined type of data protection. Change type or put in a new definition.
#endif

#endif /* RTOS_H_INCLUDED */