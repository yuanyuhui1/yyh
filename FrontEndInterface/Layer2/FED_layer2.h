//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2010.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Frontend
 Module
 Description    interface Layer 2

 Remarks

*/
//--------------------------------------------------------------------------------------------------

#define __FED_LAYER2_H__
#include "System/Interface/microcontroller.h"

#define FERX_EVENT                      0x01
#define FERX_TIMEOUT                    200
#define MAX_LENGTH_REV                  70
#define UART_MIN_RCVDATA_SIZE           6
//--------------------------------------------------------------------------------------------------
// transmit and receive status machine
#define MASTERUNINITIALIZED             0x00
#define MASTERINITIALIZED               0x01
#define MASTERIDLE                      0x02
#define MASTERWAITINGFORREPLY           0x03


//--------------------------------------------------------------------------------------------------
#define FRAME_OK                        0x00
#define FRAME_CRCERROR                  0x01
    
#define FRAMENUM_OK                     0x00
#define FRAMENUM_ERROR                  0x01

#define FRAME_FE_ACCESS_ERROR           0x01
#define FRAME_FE_CRC_ERROR              0x02
#define FRAME_FE_BUSY                   0x03

// HS_O 1 --> FE_HS 0; HS_O 0 --> FE_HS 1; 
#define FE_HANDSHAKE_OUT_HIGH()    HAL_GPIO_WritePin(FE_HANDSHAKE_OUT_GPIO_PORT, FE_HANDSHAKE_OUT_GPIO_PIN, GPIO_PIN_RESET)
#define FE_HANDSHAKE_OUT_LOW()     HAL_GPIO_WritePin(FE_HANDSHAKE_OUT_GPIO_PORT, FE_HANDSHAKE_OUT_GPIO_PIN, GPIO_PIN_SET)
#define FE_POWER_ENABLE()          HAL_GPIO_WritePin(FE_POWER_GPIO_PORT, FE_POWER_GPIO_PIN, GPIO_PIN_SET)
#define FE_POWER_DISABLE()         HAL_GPIO_WritePin(FE_POWER_GPIO_PORT, FE_POWER_GPIO_PIN, GPIO_PIN_RESET)
#define FE_RESET_ENABLE()          { HAL_GPIO_WritePin(FE_CLK_GPIO_PORT, FE_CLK_GPIO_PIN, GPIO_PIN_RESET); \
                                     HAL_GPIO_WritePin(FE_PWR_SENSE_GPIO_PORT, FE_PWR_SENSE_GPIO_PIN, GPIO_PIN_RESET); }
#define FE_RESET_DISABLE()         { HAL_GPIO_WritePin(FE_CLK_GPIO_PORT, FE_CLK_GPIO_PIN, GPIO_PIN_SET); \
                                     HAL_GPIO_WritePin(FE_PWR_SENSE_GPIO_PORT, FE_PWR_SENSE_GPIO_PIN, GPIO_PIN_SET); }

void Initialize_FED(void);
void Initialize_HW(void);
void SendFrame_LAYER2(void);

TUSIGN8 FrameCRCCheck(void);
TUSIGN8 CheckFrameNum(void);
TUSIGN8 CheckMessageStatus(void);