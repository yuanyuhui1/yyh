//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         common component
 Module         NV_MEM
 Description    file-list NV_MEM
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __NV_MEM_H__
  #error nv_mem.h must be included before filelist.h
#endif

#ifndef __FILE_H__
  #error file.h must be included before filelist.h
#endif

#ifdef __FILELIST_H__
  #error filelist.h included more than once
#endif

#define __FILELIST_H__

extern SLOW const T_FILE SLOW *const fileList[NUM_OF_FILES];

