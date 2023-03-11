//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Mapper Subsystem
 Module			Mapper_Config.h

 Description    Mapper_Config
 <brief>
 	This file is used to configure the device valid device variables
 </brief>
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------


#ifdef __MAPPER_CONFIG_H__
  #error Mapper_Config.h included more than once
#endif

#define __MAPPER_CONFIG_H__



extern TUSIGN16 CheckIsSupportedMeasurement(const TUSIGN8 inputCode);