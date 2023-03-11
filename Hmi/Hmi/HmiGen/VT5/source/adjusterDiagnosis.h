#ifndef _ADJUSTERDIAGNOSIS_H
#define _ADJUSTERDIAGNOSIS_H

#ifndef _COMPILETIMEDEFINITIONS
  #error CompileTimeDefinitions.h must be included
#endif

#ifndef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h must be included
#endif

#ifdef HMI_SUPPORTS_DIAGNOSIS
// include the header file for the diagnosis subsystem
#error DIAGNOSIS SUBSYSTEM HEADER FILE MUST BE INCLUDED

#define DIAGNOSIS_SUBSYSTEM 
#define DIAGNOSIS_OBJECT    
#endif


#endif
