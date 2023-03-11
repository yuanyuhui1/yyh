//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         2 Wire Top Works
 Module         ServicePort_uart.c
 Description    implementation of the uart-routines with hardware-access
 Remarks
201109: This file is ported from Klaus's LocalInterface and is renamed to serviceport.
*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "System/Interface/system.h"
#include "System/Interface/microcontroller.h"
#include "serviceport_uart.h"

#define INVALID_BAUD(Baud)      ((Baud != SERVICEPORT_UART_BAUD_HMI) && (Baud != SERVICEPORT_UART_BAUD_SP))
#define INVALID_PARITY(Parity)  ((Parity != SERVICEPORT_UART_PARITY_NONE) && (Parity != SERVICEPORT_UART_PARITY_ODD) && (Parity != SERVICEPORT_UART_PARITY_EVEN))
#define INVALID_STOP(Stop)      ((Stop != SERVICEPORT_UART_STOPBITS_1) && (Stop != SERVICEPORT_UART_STOPBITS_2))

static void HMI_UART_MspInitCallback(UART_HandleTypeDef *huart);
static void HMI_UART_MspDeInitCallback(UART_HandleTypeDef *huart);
static void HMI_UART_TxCpltCallback(UART_HandleTypeDef *huart);
static void HMI_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void (*mReceiveIrqService)(void);
void (*mTransmitIrqService)(void);

UART_HandleTypeDef HMI_UartHandle;
TUSIGN8 mSpHmiRxData;

static void HMI_UART_Init(const T_SERVICEPORT_UART_PARAM *pParam)
{
  HMI_UartHandle.Instance = HMI_UART_INSTANCE;
  HMI_UartHandle.Init.BaudRate = pParam->baud;
  HMI_UartHandle.Init.WordLength = pParam->length;
  HMI_UartHandle.Init.StopBits = pParam->stop;
  HMI_UartHandle.Init.Parity = pParam->parity;
  HMI_UartHandle.Init.Mode = UART_MODE_TX_RX;
  HMI_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HMI_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  HMI_UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  HMI_UartHandle.Init.ClockPrescaler = UART_PRESCALER_DIV1;

  if (HAL_UART_RegisterCallback(&HMI_UartHandle, HAL_UART_MSPINIT_CB_ID,
                                &HMI_UART_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }
  
  if (HAL_UART_Init(&HMI_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_RegisterCallback(&HMI_UartHandle, HAL_UART_TX_COMPLETE_CB_ID,
                                &HMI_UART_TxCpltCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_RegisterCallback(&HMI_UartHandle, HAL_UART_RX_COMPLETE_CB_ID,
                                &HMI_UART_RxCpltCallback) != HAL_OK)
  {
    Error_Handler();
  }
}

static void HMI_UART_DeInit()
{
  if (HAL_UART_RegisterCallback(&HMI_UartHandle, HAL_UART_MSPDEINIT_CB_ID,
                                &HMI_UART_MspDeInitCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_DeInit(&HMI_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

static void HMI_UART_MspInitCallback(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance == HMI_UART_INSTANCE)
  {
    /* Peripheral clock enable */
    HMI_UART_CLK_ENABLE();
    HMI_UART_TX_GPIO_CLK_ENABLE();
    HMI_UART_RX_GPIO_CLK_ENABLE();

    /* UART GPIO Configuration */
    GPIO_InitStruct.Pin = HMI_UART_TX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = HMI_UART_TX_GPIO_AF;
    HAL_GPIO_Init(HMI_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = HMI_UART_RX_GPIO_PIN;
    GPIO_InitStruct.Alternate = HMI_UART_RX_GPIO_AF;
    HAL_GPIO_Init(HMI_UART_RX_GPIO_PORT, &GPIO_InitStruct);

    /* UART interrupt Init */
    HAL_NVIC_SetPriority(HMI_UART_IRQn, HMI_UART_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(HMI_UART_IRQn);
  }
}

static void HMI_UART_MspDeInitCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == HMI_UART_INSTANCE)
  {
    /* Reset peripherals */
    HMI_UART_FORCE_RESET();
    HMI_UART_RELEASE_RESET();

    /* Disable peripherals and GPIO Clocks */
    HAL_GPIO_DeInit(HMI_UART_TX_GPIO_PORT, HMI_UART_TX_GPIO_PIN);
    HAL_GPIO_DeInit(HMI_UART_RX_GPIO_PORT, HMI_UART_RX_GPIO_PIN);

    /* Disable the NVIC for UART */
    HAL_NVIC_DisableIRQ(HMI_UART_IRQn);
  }
}

static void HMI_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == HMI_UART_INSTANCE)
  {
    mTransmitIrqService();
  }
}

static void HMI_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == HMI_UART_INSTANCE)
  {
    mReceiveIrqService();
  }
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief   Initializes the LocalUniversalInterface Data-Link
 \author  Klaus Pose
 \date    2005-10-26
 \param   pParam = pointer to a parameter structure (use prepared macro's like SERVICEPORT_BAUD() )
 \return  eTRUE if succeeded
 \test
 date: 2006-12-12
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
void InitializeUart_SERVICEPORT(const T_SERVICEPORT_UART_PARAM SLOW * pParam)
{
  static T_SERVICEPORT_UART_PARAM currentConfig =
  {
    0,      // TUSIGN16 baud;
    0,      // TUSIGN8  parity;
    0,      // TUSIGN8  stop;
    0,      // TUSIGN8  source;
    NULL,   // void     (*ReceiveIrqService)(void);
    NULL    // void     (*TransmitIrqService)(void);
  };

  if(pParam)
  {
    if ((INVALID_BAUD(pParam->baud)) || (INVALID_PARITY(pParam->parity)) || (INVALID_STOP(pParam->stop)))
    {
      HMI_UART_DeInit();
      currentConfig.baud               = 0;
      currentConfig.parity             = 0;
      currentConfig.stop               = 0;
      currentConfig.length             = 0;
      currentConfig.ReceiveIrqService  = NULL;
      currentConfig.TransmitIrqService = NULL;
    }
    else
    {
      if ((pParam->baud   != currentConfig.baud)
        || (pParam->parity != currentConfig.parity)
        || (pParam->stop   != currentConfig.stop)
        || (pParam->length != currentConfig.length)
        || (pParam->ReceiveIrqService  != currentConfig.ReceiveIrqService)
        || (pParam->TransmitIrqService != currentConfig.TransmitIrqService))
      {
        HMI_UART_DeInit();

        if( pParam->ReceiveIrqService )
        {
          mReceiveIrqService = pParam->ReceiveIrqService;
          // redundant inverse
          //mReceiveIrqServiceInv = ~(TUSIGN32)(pParam->ReceiveIrqService);
          //SERVICEPORT_SET_UART_RX_IRQ( SERVICEPORT_UART_RX_INTERRUPT_LEVEL );
        }

        if( pParam->TransmitIrqService )
        {
          mTransmitIrqService = pParam->TransmitIrqService;
          // redundant inverse
          //mTransmitIrqServiceInv = ~(TUSIGN32)(pParam->TransmitIrqService);
          //SERVICEPORT_SET_UART_TX_IRQ( SERVICEPORT_UART_TX_INTERRUPT_LEVEL );
        }

        HMI_UART_Init(pParam);
        // Enable Receive interrupt
        if(HAL_UART_Receive_IT(&HMI_UartHandle, (uint8_t*)&mSpHmiRxData, 1)!= HAL_OK)
        {
          Error_Handler();
        }
        currentConfig.baud   = pParam->baud;
        currentConfig.parity = pParam->parity;
        currentConfig.stop   = pParam->stop;
        currentConfig.length = pParam->length;
        currentConfig.ReceiveIrqService  = pParam->ReceiveIrqService;
        currentConfig.TransmitIrqService = pParam->TransmitIrqService;
      }
    }
  }
  else
  {
    HMI_UART_DeInit();
  }
}