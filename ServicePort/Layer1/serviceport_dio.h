//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         UHTE
 Module         dio.h
 Description    header for the digital I/O of the local interface
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before dio.h
#endif


#ifdef __LOCALINTERFACE_DIO_H__
  #error localinterface_dio.h included more than once
#endif

#define __LOCALINTERFACE_DIO_H__

#include "System/Interface/microcontroller.h"

#define HMI_PRESENCE_STATE     HAL_GPIO_ReadPin(HMI_PRESENCE_GPIO_PORT, HMI_PRESENCE_GPIO_PIN)

// HMI power enable
#define SET_HMI_POWER_ON()     HAL_GPIO_WritePin(HMI_POWER_GPIO_PORT, HMI_POWER_GPIO_PIN, GPIO_PIN_SET)
#define SET_HMI_POWER_OFF()    HAL_GPIO_WritePin(HMI_POWER_GPIO_PORT, HMI_POWER_GPIO_PIN, GPIO_PIN_RESET)
#define HMI_POWER_IS_ON        (HAL_GPIO_ReadPin(HMI_POWER_GPIO_PORT, HMI_POWER_GPIO_PIN) == GPIO_PIN_SET)

// HMI Reset output high
#define SET_HMI_RESET_HIGH()   HAL_GPIO_WritePin(HMI_RESET_GPIO_PORT, HMI_RESET_GPIO_PIN, GPIO_PIN_SET)

//--------------------------------------------------------------------------------------------------
//  declaration of prototypes
void SetPowerDio_SERVICEPORT(const TBOOL bActive);
TBOOL GetPowerDio_SERVICEPORT(void);