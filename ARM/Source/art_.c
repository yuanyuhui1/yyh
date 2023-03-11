//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2006
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         ARM
 Description    Access Right Table source and header file created by code-generator "Arm_Gen".

 Remarks
                None.
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef _ART__H
#define _ART__H


// system-wide include files added here
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
// subsystem-specific include files added here
#include "Coordinator/Interface/subsystem_idx.h"

//#include "..\..\shortsubsystemname\interface\shortsubsystemname_idx.h"
//#include "..\..\frameworkversion\interface\frameworkversion_idx.h"

// arm include files added here
#include "ARM/Interface/arm.h"

TUSIGN16 accessRightTable[] = 
{
/*    //#0 - 
    (TUSIGN16)SUBSYSTEMNAME_IDX,
    (TUSIGN16)SUBSYSTEMNAME_IDX_arm,
    ,
      BASIC_USER_NO_ACCESS +
      STANDARD_USER_NO_ACCESS +
      ADVANCED_USER_NO_ACCESS +
      SERVICE_USER_NO_ACCESS,

    //#1 - 
    (TUSIGN16)SHORTSUBSYSTEMNAME_IDX,
    (TUSIGN16)SHORTSUBSYSTEMNAME_IDX_arm,
    ,
      BASIC_USER_NO_ACCESS +
      STANDARD_USER_NO_ACCESS +
      ADVANCED_USER_NO_ACCESS +
      SERVICE_USER_NO_ACCESS,

    //#2 - 
    (TUSIGN16)FRAMEWORKVERSION_IDX,
    (TUSIGN16)FRAMEWORKVERSION_IDX_0x020101,
    ,
      BASIC_USER_NO_ACCESS +
      STANDARD_USER_NO_ACCESS +
      ADVANCED_USER_NO_ACCESS +
      SERVICE_USER_NO_ACCESS,*/

    TABLE_TERMINATION
};
#endif
