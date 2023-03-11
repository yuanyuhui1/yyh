//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Frame
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "generators/Subsystem_Templates/Interface/Frame.h"
#include "frame_types.h"
#include "frame_attributes.h"
#include "frame_overload.h"


//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author 
 \date   
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \test
 \n by: 
 \n intention: 
 \n result module test:
 \n result Lint Level 3: 
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_FRAME(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = FATAL_ERROR;

    //lint -e{746} 
    VIP_ASSERT(me==FRAME_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);

    /*
    if( result==OK )
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
        }

        if( (typeOfStartUp & INIT_TASKS) != 0)
        {
        }

        if( (typeOfStartUp & INIT_DATA) != 0)
        {
        }

        if( (typeOfStartUp & INIT_CALCULATION) != 0)
        {
        }
    }
    */

    return result;
}

