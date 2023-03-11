//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Prototypes of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper_action_fct.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before Mapper_action_fct.h
#endif

#ifndef __MAPPER_H__
  #error  Mapper.h must be included before Mapper_action_fct.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MAPPER_ACTION_FCT_H__
  #error Mapper_action_fct.h included more than once
#endif

#define __MAPPER_ACTION_FCT_H__

//@SubGen start@<METHODS>
TUSIGN16 diagClearHistory_MAPPER(void);
//@SubGen end@<METHODS>