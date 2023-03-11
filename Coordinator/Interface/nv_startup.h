//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before nv_startup.h
#endif


#ifdef __NV_STARTUP_H__
  #error nv_startup.h included more than once
#endif

#define __NV_STARTUP_H__

//--------------------------------------------------------------------------------------------------
#define FORMAT_ALL_DATA     ((TUSIGN16)1)
#define FORMAT_CONFIG_DATA  ((TUSIGN16)0)

TUSIGN16 StartupNVSubsystem(void);
T_NV_DIAGNOSIS FormatAllData_intern(const TUSIGN16 format);
void LoadAllRomDefaut(void);