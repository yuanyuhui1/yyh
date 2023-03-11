//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      UHTE
 Module      Version code
 Remarks
 Author:     Paul Li

\brief       no function
*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif

#include "ElettronicServices/Source/versioncode.h"

//segment left for bootloader to visit
#pragma location="VERSION_CODE"
__root const T_VERSION_CODE CB_SW_VersionCode = 
{
    {SW_REV0, SW_REV1, SW_REV2},
    {SW_YY,   SW_MM,   SW_DD},
};