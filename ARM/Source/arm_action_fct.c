//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Implementation of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"

#ifdef FRAMEWORK_VERSION_2_1_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif
#ifdef FRAMEWORK_VERSION_2_2_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif
#ifdef FRAMEWORK_VERSION_2_3_1
  #include "System/Interface/common_type.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ARM/Interface/arm.h"
#include "arm_action_fct.h"
#include "ARM/Interface/arm_idx.h"

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER ENABLEHIGHSECURITYMODE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER ENABLEHIGHSECURITYMODE> \endif
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
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD ENABLEHIGHSECURITYMODE>
TUSIGN16 enableHighSecurityMode_ARM(void)
//@SubGen end@<METHOD ENABLEHIGHSECURITYMODE>
{
    TUSIGN16 result = (TUSIGN16)OK;
    TUSIGN8 securityMode ;

    // Set the high security mode value
    securityMode = ARM_HIGH_SECURITY_ENABLED ;
    result = ARM_ME->Put(ARM_ME,(TUSIGN16)ARM_IDX_hiSecAccessState,(TINT16)WHOLE_OBJECT,(void FAST*)&securityMode) ;

    return(result) ;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER DISABLEHIGHSECURITYMODE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER DISABLEHIGHSECURITYMODE> \endif
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
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD DISABLEHIGHSECURITYMODE>
TUSIGN16 disableHighSecurityMode_ARM(void)
//@SubGen end@<METHOD DISABLEHIGHSECURITYMODE>
{
    TUSIGN16 result = (TUSIGN16)OK ;
    TUSIGN8 securityMode ;

    // Set the high security mode value
    securityMode = ARM_HIGH_SECURITY_DISABLED ;
    result = ARM_ME->Put(ARM_ME,(TUSIGN16)ARM_IDX_hiSecAccessState,(TINT16)WHOLE_OBJECT,(void FAST*)&securityMode) ;

    return(result) ;
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

