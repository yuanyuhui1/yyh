//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Prototypes of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices_action_fct.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before ElettronicServices_action_fct.h
#endif

#ifndef __ELETTRONIC_SERVICES_H__
  #error  ElettronicServices.h must be included before ElettronicServices_action_fct.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ELETTRONIC_SERVICES_ACTION_FCT_H__
  #error ElettronicServices_action_fct.h included more than once
#endif

#define __ELETTRONIC_SERVICES_ACTION_FCT_H__

//@SubGen start@<METHODS>
TUSIGN16 deviceReset_ELETTRONICSERVICES(void);
TUSIGN16 resetOutputTrimming_ELETTRONICSERVICES(void);
TUSIGN16 writeAutoZero_ELETTRONICSERVICES(void);
TUSIGN16 writeSelfDiagnosis_ELETTRONICSERVICES(void);
TUSIGN16 resetCB_Runhour_ELETTRONICSERVICES(void);
TUSIGN16 formatFENV_ELETTRONICSERVICES(void);
TUSIGN16 saveAllAsDefault_ELETTRONICSERVICES(void);
TUSIGN16 resetAllToDefault_ELETTRONICSERVICES(void);
//@SubGen end@<METHODS>