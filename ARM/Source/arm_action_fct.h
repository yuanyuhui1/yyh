//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Prototypes of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_action_fct.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before Arm_action_fct.h
#endif

#ifndef __ARM_H__
  #error  Arm.h must be included before Arm_action_fct.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_ACTION_FCT_H__
  #error Arm_action_fct.h included more than once
#endif

#define __ARM_ACTION_FCT_H__

//@SubGen start@<METHODS>
TUSIGN16 enableHighSecurityMode_ARM(void);
TUSIGN16 disableHighSecurityMode_ARM(void);
//@SubGen end@<METHODS>

