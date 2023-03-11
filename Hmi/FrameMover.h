//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Generic
 Subsystem				HMI
 Description			Frame Mover of the HMI subsystem. Is responsible for controlling the frame pointer and associated
 						variables within the Frame Table
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

/****** FrameMover.h ******/

#ifndef _FRAMEMOVER_H
#define _FRAMEMOVER_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _KEYSTATEDEFINITIONS_H
  #error keystateDefinitions.h must be included
#endif


void FrameMover_ResetFrameMemory(TUSIGN8 startPointer);
TUSIGN8 FrameMover_GetFrameMemoryPointer(void);
void FrameMover_ProcessFrameMemoryTimedReset(void);

/*!
 \brief		Acts on a command to change the currently active frame.
 \author	Roger Arnold
 \param		command
 \return	1 on success, 0 on any error (cannot perform requested command)
 \test		No details available
*/
TUSIGN16 FrameMoverMove(MOVE_COMMAND_TYPE command);


#ifdef HMI_SUPPORTS_SERVICEPORT
/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the Frame Mover local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void FrameMoverSetLocalDataPointer(TUSIGN8 newInstance);
#endif
#endif
