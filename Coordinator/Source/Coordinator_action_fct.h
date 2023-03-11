//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Prototypes of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator_action_fct.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before Coordinator_action_fct.h
#endif

#ifndef __COORDINATOR_H__
  #error  Coordinator.h must be included before Coordinator_action_fct.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __COORDINATOR_ACTION_FCT_H__
  #error Coordinator_action_fct.h included more than once
#endif

#define __COORDINATOR_ACTION_FCT_H__

//@SubGen start@<METHODS>
TUSIGN16 readParameters_CDT(void);
TUSIGN16 writeParameters_CDT(void);
TUSIGN16 saveCustomerDef_CDT(void);
TUSIGN16 saveFactoryDef_CDT(void);
TUSIGN16 resetCustomerDef_CDT(void);
TUSIGN16 resetFactoryDef_CDT(void);
TUSIGN16 resetFE_Runhour_CDT(void);
TUSIGN16 config_alg_paras_CDT(void);
TUSIGN16 read_alg_block_CDT(void);
//@SubGen end@<METHODS>

