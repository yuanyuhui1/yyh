//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      Coordinate
 Module      timeElapse
 Description get accurate time(us),which is use in totalizer and digtal out modue
 Remarks
 Author:     lawrence

\brief       functions added. lawrence. 2013-01
*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before watchdog.h
#endif



#ifdef __TIMEELAPSE__H__
  #error TimeElapse.h included more than once
#endif

#define __TIMEELAPSE__H__

//time_us = 16*((TUSIGN32)Cycles/(TIMER_F1F2_CLOCK/1000000));
//! to reduce the calculation, define the timer cycle time as a float
//! as the cycle time is divide from f32TIMAB,so the period is 32* (1000000/OSZILLATOR_CLOCK) (us)

//#define CYCLE_TIME (8.681)  //(8.6806)???

/**
  * @brief time elapse timer Initialization Function
  * @param None
  * @retval None
  */
void TimeElapse_TIMER_Init(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     public;
 \author    lawrence
 \date      2013-01-08
 \param     BitAction Pointer to the action that will be called if watchdog bites
 \param     ms watchdog-time in ms; 0==65536ms!
 \param     info parameter for BiteAction, e.g. sourcecode-position
 \return    -
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN32 CalculateElapsedTime(void);