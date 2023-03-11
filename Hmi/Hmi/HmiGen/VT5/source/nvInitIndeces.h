#ifndef _NVINITINDECES_H
#define _NVINITINDECES_H

#ifndef _COMPILETIMEDEFINITIONS
  #error CompileTimeDefinitions.h must be included
#endif

#ifndef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h must be included
#endif

#if NVINIT_STYLE > 0

//+@ HMIGen  @+ NVINIT FRAME OBJECTS @+
//
// The objects accessed by the NV Initialization Display must be defined here
//
//-----------------------------------------------------------------------------
// 1. define the initialization subsystem
//    eg #define NVINIT_SUBSYSTEM          NVINTERFACE_IDX
//
#error  NVINIT_SUBSYSTEM not defined

//-----------------------------------------------------------------------------
// 2. define the initialization progress object
//    eg #define NVINIT_PROGRESS_OBJECT    NVINTERFACE_IDX_Progress
//
#error  NVINIT_PROGRESS_OBJECT not defined

//-----------------------------------------------------------------------------
// 3. define the initialization status object
//    eg #define NVINIT_STATUS_OBJECT      NVINTERFACE_IDX_StartUpState
//
#error  NVINIT_STATUS_OBJECT not defined

//-----------------------------------------------------------------------------
//-@ HMIGen  @+ NVINIT FRAME OBJECTS @-

#endif // end of #if NVINIT_STYLE > 0


#endif
