//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Mapper Subsystem
 Module			Mapper_Config.c

 Description    Mapper_Config
 <brief>
 	This file is used to configure the Device Variable Configuration
 </brief>
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <limits.h>
#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Mapper/Interface/Mapper_Config.h"

//@ToDO include specific files for different projects
#include "Mapper_types.h"
#include "Mapper_attributes.h"
#include "Mapper_Measurement.h"
//@End TODO

//--------------------------------------------------------------------------------------------------
/*!
 \brief	 This method is used to Check whether the device variable is supported in current selection mask
 \author Jax Yang	
 \date   2012-07-29
\param[in] inputCode  input measurement label valid range:0~17,244~249
 \return : Check result
 \warning:none
 \test
 \n by: Jax Yang
 \n result module test:OK
 \n result Lint Level 3:OK
 \bug: none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckIsSupportedMeasurement(const TUSIGN8 inputCode)
{
    TUSIGN16 errorCode = (TUSIGN16)RULE_VIOLATION_ERR;	
	
    if(inputCode>=(TUSIGN8)M_NUMOF_DEVICEVARIABLES)
    {
        // hart spec cmd 9 slotcode
        if( (inputCode >= 244) && (inputCode <=249) )
        {
            errorCode = (TUSIGN16)OK;
        }
        else
        {
            errorCode = (TUSIGN16)RULE_VIOLATION_ERR;
        }
    }
    else if((*(MeasurementList[inputCode].selectionMask)) & mapperProtected.localOperatingModeSelection)
    {
        errorCode = (TUSIGN16)OK;
    }
    else
    {
        errorCode = (TUSIGN16)RULE_VIOLATION_ERR;
    }
    return errorCode;
}