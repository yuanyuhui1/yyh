//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      UHTE
 Module      WatchDog
 Description Interface definition of Subsystem "WatchDog".
             It also defines some behaviors of watchdog, which must be adapted to the system-behavior.
 Remarks
 Author:     Heiko Kresse

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before watchdog.h
#endif



#ifdef __WATCHDOG__H__
  #error watchdog.h included more than once
#endif

#define __WATCHDOG_H__

#ifndef DEBUG
//#ifdef DEBUG
  #define ENABLE_WATCHDOG
#endif

//! the master-watchog schedule-time. Consider that the OS_TASK needs 70µs for each watchdog.
//! consider also that the internal M16C-Watchdog should be handled.
#define WATCHDOG_SCHEDULE_20MS      20
#define DEFAULT_WATCHDOG_SCHEDULE   500
#define TIMEOUT_WATCHDOG_SCHEDULE   10000

//! define how many watchdogs are allowed to create 0..65534 !
//! Consider that the OS_TASK needs 70µs for each watchdog.
#define MAX_NUM_WATCHDOGS           10

//! defines the Interface of a Function that will be called if watchdog bites. A Subsystem that uses one
//! ore more watchdogs must declare a BiteAction for each watchdog in the BiteAction-List.
//! The declaration of BiteActionList is part of watchdog_bitelist.h
typedef void (*T_BITEACTION)(TUSIGN16);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     public; Initialize Master-Watchdog-task
 \author    Heiko Kresse
 \date      2004-07-16
 \param     -
 \return    -
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitializeMasterWatchDog_CDT(void);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     public;
 \author    Heiko Kresse
 \date      2004-07-16
 \param     BitAction Pointer to the action that will be called if watchdog bites
 \param     ms watchdog-time in ms; 0==65536ms!
 \param     info parameter for BiteAction, e.g. sourcecode-position
 \return    -
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
void TriggerWatchdog_CDT(T_BITEACTION BiteAction, TUSIGN16 ms, TUSIGN16 info);
void softwareWatchdogEnable(void);
void SetWatchdogSchedule(TUSIGN16 newWatchdogSchedule);
void InitHardwareWatchdog(void);
void TriggerHardwareWatchdog(void);