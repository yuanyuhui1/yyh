//--------------------------------------------------------------------------------------------------
/*!
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module         HART
 Description    Implementation of specialized methods for writing the short address
                Handling sets device in multidrop mode if short address was changed
                to a value greater zero.
 Remarks        Tested

*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "t_data_obj_hart_service_code.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ARM/Interface/arm.h"
//#include "..\..\MSP_Interface\Interface\msp_interface.h"
//#include "..\..\hart\interface\hart.h"
//#include "..\..\hart\interface\hart_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Source/Hart_types.h"
//#include "t_data_obj_hart_diag.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
//#include "..\..\hart\source\hart_types.h"
#include "HART/Source/Hart_attributes.h"
//--------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::HART_DIAG
*/
//--------------------------------------------------------------------------------------------------
//lint -e{641} Martin Dahl 2006-01-12 converting enum to int is OK
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = { ST_U8 };

const T_DATA_CLASS cgClassInitializer_HART_SERVICECODE = {
    cmAryAttributeDataTypes,
    0,
    0,  // no internal view
    0,
    Get_DATAOBJ,        // inherit
    Put_HART_SERVICECODE,       // inherit
    GetAttributeDescription_DATAOBJ,    // inherit
    GetObjectDescription_DATAOBJ        // inherit
};

//--------------------------------------------------------------------------------------------------
/*!
 \brief     check device short address and controlls multidrop mode
			

 \author    Stefan Tabelander, Heiko Kresse
 \date      2004-11-09
 \param     pointer to data-object, value pointer, attribute index
 \return
            OK                       operation was successful
            ILLEGAL_ATTRIB_IDX       DEBUG, unknown Attribute
            GREATERTHAN_RANGEMAX_ERR invalid address
            FATAL_ERROR              object outside nv-mem range
 \warning
 \test
 date:      2005-12-13
 \n by:     Horst Seele
 \n environment: C-SPY, see file �Test_hart_objects.c?of module test functions
 \n intention:  Instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------

//lint -esym(715,attributeIndex) JL 2006-05-11 it is not used but function must match T_UNIT
//lint -esym(818,ptrValue)  JL 2006-05-11 function must match T_UNIT convention
//lint -e{641} JL 2006-05-11 conversion from enum to int accepted
TUSIGN16 Put_HART_SERVICECODE(const T_DATA_OBJ SLOW * me, void FAST * ptrValue,
                              TINT16 attributeIndex, T_DATA_SEMAPHORE FAST * ptrDataSemaphore)
{
    TUSIGN16 result = OK;

    // debug zero pointer and illegal values
    VIP_ASSERT(me);     // lint ??
    VIP_ASSERT(ptrValue);
    TUSIGN8 charCount = 0;
    TUSIGN8 detectChar = 0;
    TCHAR valueWithNullTermination[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    // use semaphore for protected memory
    if( ptrDataSemaphore )
    {
		//lint -e{619} SD 2006-07-14 no actual loss of ptrDataSemaphore pointer precision.
        ENABLE_DATA_PROTECTION(*ptrDataSemaphore);
    }

    (void)memcpy((void *)valueWithNullTermination, (const void *)ptrValue, 20);
    //search for the amount of chars
    while (charCount <= 19)
    {
        if (valueWithNullTermination[charCount] >= 0x30
            && valueWithNullTermination[charCount] <= 0x5A)
        {
            detectChar++;
        }
        charCount++;
    }
    //make null termination at the end of the string
    valueWithNullTermination[detectChar] = 0;
    //put received bytes into transmit buffer
    if(hartDyn.HartAccessLevel == HART_SERVICE_standard)
    {
        result = arm.LogInEXE(HART_IDX, (TUSIGN8) ARM_STANDARD_USER, (void *)valueWithNullTermination);
        if (result == OK)
        {
            (void)memcpy((void *)hartDyn.ServiceCode, (const void *)ptrValue, 20);
        }
    }
    else if(hartDyn.HartAccessLevel == HART_SERVICE_advanced)
    {
        result = arm.LogInEXE(HART_IDX, (TUSIGN8) ARM_ADVANCED_USER, (void *)valueWithNullTermination);
        if (result == OK)
        {
            (void)memcpy((void *)hartDyn.ServiceCode, (const void *)ptrValue, 20);
        }
    }
    else if(hartDyn.HartAccessLevel == HART_SERVICE_service)
    {
        result = arm.LogInEXE(HART_IDX, (TUSIGN8) ARM_SERVICE_USER, (void *)valueWithNullTermination);
        if (result == OK)
        {
            (void)memcpy((void *)hartDyn.ServiceCode, (const void *)ptrValue, 20);
        }
    }
    else
    {
        hartDyn.HartAccessLevel = HART_SERVICE_service;
        result = RULE_VIOLATION_ERR;
    }
	if( ptrDataSemaphore )
    {
		//lint -e{619} SD 2006-07-14 no actual loss of ptrDataSemaphore pointer precision.
        DISABLE_DATA_PROTECTION(*ptrDataSemaphore);
    }
    return result;
}
