//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      UHTE
 Module      WatchDog
 Description The list of bite-actions hold a pointer to a BiteAction for each software-watchdog that
             should be handled by the watchdog-unit.
             A subsystem that uses a watchdog must declare a BiteAction-method. The device-designer
             is responsible that the reference to this BiteAction is filled in the bite-action-list.
             The Watchdog-unit uses the pointer as handle for the SW-Watchdogs. --> Watchdogd must not
             share a BiteAction!!!



 Remarks
 Author:     Heiko Kresse

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before watchdog_bitelist.h
#endif

#ifndef __WATCHDOG_H__
  #error watchdog.h must be include before watchdog_bitelist.h
#endif


#ifdef __WATCHDOG_BITELIST_H__
  #error watchdog_bitelist.h included more than once
#endif

#define __WATCHDOG_BITELIST_H__

extern void MainTaskWatchdogBiteAction(TUSIGN16 info);
extern void ServiceTaskWatchdogBiteAction(TUSIGN16 info);
extern void SlowTaskWatchdogBiteAction(TUSIGN16 info);
extern void HartTaskWatchdogBiteAction(TUSIGN16 info);
extern void FrontendInterfaceTaskWatchdogBiteAction(TUSIGN16 info);
extern void CPULoadTaskWatchdogBiteAction(TUSIGN16 info);


#define LENGTH_OF_BITEACTIONLIST 5  //(sizeof(uBiteActionList)/sizeof(uBiteActionList[0]))

extern const T_BITEACTION uBiteActionList[LENGTH_OF_BITEACTIONLIST];
extern TUSIGN8 uBitEnableList[LENGTH_OF_BITEACTIONLIST];
