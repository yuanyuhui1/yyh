//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2012.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before nv_startup.h
#endif


#ifdef __VERSION_CODE_H__
  #error versioncode.h included more than once
#endif

#define __VERSION_CODE_H__


//keep same as CB_SW_VersionCode in versioncode.c
#define SW_REV0 1
#define SW_REV1 0
#define SW_REV2 0
#define SW_YY   21
#define SW_MM   8
#define SW_DD   1

typedef struct _T_VERSION_CODE_
{
    TUSIGN8 swVersion[3];
    TUSIGN8 swBuildDate[3];
}T_VERSION_CODE;

#pragma location="VERSION_CODE"
extern __root const T_VERSION_CODE CB_SW_VersionCode;