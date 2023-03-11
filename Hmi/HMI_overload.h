//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    prototypes of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hmi_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hmi_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before hmi_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HMI_OVERLOAD_H__
  #error hmi_overload.h included more than once
#endif

#define __HMI_OVERLOAD_H__

//----------------------------------------------------------------------------------------------------------
/* Add for: reset language setting to default value (english) when use push button HMI. */
#define PUSH_BUTTON_HMI		0		//not support chanese language
#define TOUCH_SCREEN_HMI	1		//support all language
extern TUSIGN8 gVersionFlag_HMI;
//----------------------------------------------------------------------------------------------------------
// Initialize will be overloaded usually.
TUSIGN16 Initialize_HMI(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);

#if defined(HMI_SUPPORTS_SIGNALS_VALIDITY_TEST) && defined(ENABLE_LINE_SOURCE_VALIDITY_TESTS)
TUSIGN16 GetAttributeDescription_HMI(const struct _T_UNIT SLOW*me, TUSIGN16 objectIndex, TUSIGN16 attributeIndex, T_DO_ATTRIB_RANGE FAST* ptrDescriptor);
#endif
