//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         2 Wire Top Works
 Module         ServicePort_dio.c
 Description    implementation of the digital I/O of the local interface
 Remarks
201109: This file is ported from Klaus's LocalInterface and is renamed to serviceport.
*/
//--------------------------------------------------------------------------------------------------

#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "System/Interface/microcontroller.h"

#include "serviceport_dio.h"

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Public: Set the Power-Output-Pin of the localinterface
 \author  Klaus Pose
 \date    2005-11-14
 \param   bActive = set power supply eTRUE=PowerON , eFALSE=PowerOFF
 \test
 test-date: 2006-12-08
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions are done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
void SetPowerDio_SERVICEPORT(const TBOOL bActive)
{
  if( bActive )
  {
    SET_HMI_POWER_ON();
  }
  else
  {
    SET_HMI_POWER_OFF();
  }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief   Public: Return the actual State of the Power-Output-Pin of the localinterface
 \author  Klaus Pose
 \date    2005-11-14
 \return  get status of power supply : eTRUE=PowerON , eFALSE=PowerOFF
 \test
 test-date: 2006-12-08
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions are done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
TBOOL GetPowerDio_SERVICEPORT(void)
{
  if(HMI_POWER_IS_ON)
  {
    return eTRUE;
  }
  else
  {
    return eFALSE;
  }
}