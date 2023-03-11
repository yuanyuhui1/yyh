//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Implementation of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"//must use the following sequence
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/diagnosis.h"
#include "Diagnosis/Source/Diagnosis_execute.h"

#include "Mapper/Interface/Mapper.h"
#include "mapper_action_fct.h"

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER DIAGCLEARHISTORY> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER DIAGCLEARHISTORY> \endif
 \author Jax Yang
 \date 2012-09-17
 \param void
 \return TUSIGN16 frame work return codes
 \warning none
 \test
 test-date: May 14 2013 16:41:06
 \n by: Jax Yang
 \n environment:CSPY
 \n intention: Cover branch,Cover statement,Cover decision
 \n result module test: OK
 \n result Lint Level 3:OK
 \bug none
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD DIAGCLEARHISTORY>
TUSIGN16 diagClearHistory_MAPPER(void)
//@SubGen end@<METHOD DIAGCLEARHISTORY>
{
    diagClearHistory();
    
    return (TUSIGN16)OK;  
}


/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/