//----------------------------------------------------------------------------------------------------------
/*

 Copyright      Copyright ABB, 2010.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Burst Message Configuration Objects
 Module         T_DATA_OBJ_BURSTMSG_MANAGEVALUES
 Description    Definition of class T_DATA_OBJ::BURSTMSG_MANAGEVALUES
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "HART/Layer2/Interface/layer2.h"
#include "objfunctrb.h"
#include "cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"

//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::BURSTMSG_MANAGEVALUES
*/
//----------------------------------------------------------------------------------------------------------
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = {ST_U32, ST_U32, ST_FLOAT, ST_U16};

const T_DATA_CLASS cgClassInitializer_BURSTMSG_MANAGEVALUES =
{
    cmAryAttributeDataTypes,
    Check_DATAOBJ,                              // no businessrules
    0,
    0,
    Get_DATAOBJ,                                // inherit
    Put_DATAOBJ,                                // inherit
    GetAttributeDescription_DATAOBJ,            // inherit
    GetObjectDescription_DATAOBJ                // inherit
};

