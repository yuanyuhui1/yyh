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
#ifndef _ART_HART_H
#define _ART_HART_H


// system-wide include files added here
#include "System/Interface/system.h"
#ifdef FRAMEWORK_VERSION_2_1_0
#include "../../common/interface/common_type.h"
#include "../../embos/interface/rtos.h"
#endif
#ifdef FRAMEWORK_VERSION_2_2_0
#include "../../common/interface/common_type.h"
#endif
#ifdef FRAMEWORK_VERSION_2_3_1
#include "System/Interface/common_type.h"
#endif

#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

// subsystem-specific include files added here
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "HART/Interface/Hart_idx.h"

// arm include files added here
#include "ARM/Interface/arm.h"
#include "art_hart.h"

const TUSIGN16 accessRightTableHart[] = 
{
    //#0 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_board,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#1 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_address,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#2 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_subsystemIndex,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#3 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_objectIndex,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#4 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_attributeIndex,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#5 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_objectDataLength,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#6 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_objectData,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#7 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_memoryAddress,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#8 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_memoryLength,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    //#9 - 
    (TUSIGN16)HART_IDX,
    (TUSIGN16)HART_IDX_memoryData,
    ANY_ATTRIBUTE,
      BASIC_USER_READ_ONLY +
      STANDARD_USER_READ_ONLY +
      ADVANCED_USER_READ_ONLY +
      SERVICE_USER_READ_WRITE,

    TABLE_TERMINATION
};
#endif
