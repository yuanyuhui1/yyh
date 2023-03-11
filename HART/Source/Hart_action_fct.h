//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Prototypes of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Hart_action_fct.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before Hart_action_fct.h
#endif

#ifndef __HART_H__
  #error  Hart.h must be included before Hart_action_fct.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_ACTION_FCT_H__
  #error Hart_action_fct.h included more than once
#endif

#define __HART_ACTION_FCT_H__

//@SubGen start@<METHODS>
TUSIGN16 resetConfigFlag_HART(void);
TUSIGN16 StartSelfTest_HART(void);
TUSIGN16 resetDevice_HART(void);
TUSIGN16 resetMoreStatusAvailable_HART(void);
TUSIGN16 squawk_HART(void);
TUSIGN16 setPVUpRangeValue_HART(void);
TUSIGN16 setPVLowerRangeValue_HART(void);
TUSIGN16 setDeviceVariableNZero_HART(void);
TUSIGN16 resetDeviceVariableNTrim_HART(void);
TUSIGN16 serviceCodeLogOut_HART(void);
TUSIGN16 performActions_HART(void);
//@SubGen end@<METHODS>

