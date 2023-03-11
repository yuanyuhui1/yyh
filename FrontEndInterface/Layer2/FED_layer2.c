//--------------------------------------------------------------------------------------------------
/*
Copyright      Copyright ABB, 2010.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.
System         Subsystem frontend
Module
Description    implementation layer2 interface methods
Remarks
uses FE_UART and FE_TIMER 
*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "System/Interface/microcontroller.h"
#include "RTOS/Interface/rtos.h"
#include "Common/Interface/common_functions.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "FrontEndInterface/Layer7/FED_layer7.h"
#include "FrontEndInterface/Layer2/FED_layer2.h"

static void FE_UART_MspInitCallback(UART_HandleTypeDef *huart);
static void FE_UART_RxCpltCallback(UART_HandleTypeDef *huart);
static void FE_TIMER_BASE_MspInitCallback(TIM_HandleTypeDef *htim_base);
static void FE_TIMER_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

extern RTOS_TASK_HANDLE FrontendTskHandle;
//----------------------------------------------------------------------------------------------------------
#pragma location="STATIC_INTERRUPT_DATA"
static TUSIGN8 mReceiverBuffer[MAX_LENGTH_REV];
#pragma location="STATIC_INTERRUPT_DATA"
static TUSIGN8 mSendBuffer[MAX_LENGTH_REV];

static TUSIGN8 mRxData;
static TUSIGN8 mBufferIndex;
static TUSIGN8 mTxLength;
//! flags for communication between timer, receiver and transmitter
static TUSIGN8 mMasterStatus = MASTERUNINITIALIZED;
/* UART handle declaration */
UART_HandleTypeDef FE_UartHandle;
/* TIM handle declaration */
TIM_HandleTypeDef FE_TimHandle;

//--------------------------------------------------------------------------
/*
\brief  Initialize FED. 
\author spring zhou
\date   2012-07-10
\param -void
\return - void
*/
//--------------------------------------------------------------------------
void Initialize_FED(void)
{
  TUSIGN16 counter = 80;
  while(counter-- > 0)  //wait not more than 8s
  {
    if(HAL_GPIO_ReadPin(FE_HANDSHAKE_IN_GPIO_PORT, FE_HANDSHAKE_IN_GPIO_PIN) == GPIO_PIN_RESET)
    {
      break;
    }
    else
    {
      Delay_RTOS_TASK(100);
    }
  }

  /* Enable receive */
  if (HAL_UART_Receive_IT(&FE_UartHandle, (uint8_t *)&mRxData, 1)!= HAL_OK)
  {
    Error_Handler();
  }

}

//--------------------------------------------------------------------------
/*
\brief  Initialize FE_UART. 
\author spring zhou
\date   2012-07-10
\param -void
\return - void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//--------------------------------------------------------------------------
static void FE_UART_Init(void)
{
  FE_UartHandle.Instance = FE_UART_INSTANCE;
  FE_UartHandle.Init.BaudRate = 19200;
  FE_UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
  FE_UartHandle.Init.StopBits = UART_STOPBITS_2;
  FE_UartHandle.Init.Parity = UART_PARITY_ODD;
  FE_UartHandle.Init.Mode = UART_MODE_TX_RX;
  FE_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  FE_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  FE_UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  FE_UartHandle.Init.ClockPrescaler = UART_PRESCALER_DIV1;

  if (HAL_UART_RegisterCallback(&FE_UartHandle, HAL_UART_MSPINIT_CB_ID,
                                &FE_UART_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_Init(&FE_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_RegisterCallback(&FE_UartHandle, HAL_UART_RX_COMPLETE_CB_ID,
                                &FE_UART_RxCpltCallback) != HAL_OK)
  {
    Error_Handler();
  }

}


//--------------------------------------------------------------------------
/**
  * @brief FE_TIMER Initialization Function
  * @param None
  * @retval None
  */
//--------------------------------------------------------------------------
static void FE_TIMER_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  uint32_t uwPrescalerValue = 0;
  /* -----------------------------------------------------------------------
    FE_TIMER input clock (FE_TIMERCLK) is set to APB1 clock (PCLK1), 
    since APB1 prescaler is equal to 2.
    FE_TIMERCLK = 2*PCLK1
    PCLK1 = HCLK/2 as AHB Clock divider is set to RCC_HCLK_DIV2
    => FE_TIMERCLK = HCLK = SystemCoreClock
    Compute the prescaler value to have FE_TIMER counter clock equal to 1MHz
  ----------------------------------------------------------------------- */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 1000000) - 1;

  FE_TimHandle.Instance = FE_TIMER_INSTANCE;
  FE_TimHandle.Init.Prescaler = uwPrescalerValue;
  FE_TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  FE_TimHandle.Init.Period = 2999; /* 3ms */
  FE_TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  FE_TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_RegisterCallback(&FE_TimHandle, HAL_TIM_BASE_MSPINIT_CB_ID,
                               &FE_TIMER_BASE_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_Base_Init(&FE_TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&FE_TimHandle, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&FE_TimHandle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_RegisterCallback(&FE_TimHandle, HAL_TIM_PERIOD_ELAPSED_CB_ID,
                               &FE_TIMER_PeriodElapsedCallback) != HAL_OK)
  {
    Error_Handler();
  }
}


static void FE_UART_MspInitCallback(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance == FE_UART_INSTANCE)
  {
    /* Peripheral clock enable */
    FE_UART_CLK_ENABLE();
    FE_UART_RX_GPIO_CLK_ENABLE();
    FE_UART_TX_GPIO_CLK_ENABLE();

    /* UART GPIO Configuration */
    GPIO_InitStruct.Pin = FE_UART_TX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = FE_UART_TX_GPIO_AF;
    HAL_GPIO_Init(FE_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FE_UART_RX_GPIO_PIN;
    GPIO_InitStruct.Alternate = FE_UART_RX_GPIO_AF;
    HAL_GPIO_Init(FE_UART_RX_GPIO_PORT, &GPIO_InitStruct);

    /* UART interrupt Init */
    HAL_NVIC_SetPriority(FE_UART_IRQn, FE_UART_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(FE_UART_IRQn);
  }
}

static void FE_TIMER_BASE_MspInitCallback(TIM_HandleTypeDef *htim_base)
{
  if(htim_base->Instance == FE_TIMER_INSTANCE)
  {
    /* TIM Peripheral clock enable */
    FE_TIMER_CLK_ENABLE();

    /* Set the TIM priority */
    HAL_NVIC_SetPriority(FE_TIMER_IRQn, FE_TIMER_IT_PRIORITY, 0);

    /* Enable the TIM global Interrupt */
    HAL_NVIC_EnableIRQ(FE_TIMER_IRQn);
  }
}

//--------------------------------------------------------------------------
/*!
\brief  Initialize the FE_UART, FE_TIMER.
\author spring zhou
\date   2011-08-10
\param -void
\return -void
*/
//--------------------------------------------------------------------------
void Initialize_HW(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  FE_HANDSHAKE_IN_GPIO_CLK_ENABLE();
  FE_HANDSHAKE_OUT_GPIO_CLK_ENABLE();
  FE_CLK_GPIO_CLK_ENABLE();
  FE_PWR_SENSE_GPIO_CLK_ENABLE();
  FE_POWER_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = FE_HANDSHAKE_IN_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FE_HANDSHAKE_IN_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FE_HANDSHAKE_OUT_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FE_HANDSHAKE_OUT_GPIO_PORT, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = FE_CLK_GPIO_PIN;
  HAL_GPIO_Init(FE_CLK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FE_PWR_SENSE_GPIO_PIN;
  HAL_GPIO_Init(FE_PWR_SENSE_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FE_POWER_GPIO_PIN;
  HAL_GPIO_Init(FE_POWER_GPIO_PORT, &GPIO_InitStruct);

  FE_RESET_DISABLE();
  FE_POWER_ENABLE();

  /*  This step shall be excute in CB bootlaoder.
   *  When CB bootloader is ready, this step shall be removed 
   */
  // Active HS_O, FE startup from bootloader
  FE_HANDSHAKE_OUT_HIGH();
  /* Waiting for FE check the handshake to startup bootloader, HS_O shall be in (1s, 8s) */
  Delay_RTOS_TASK(2000);
  FE_HANDSHAKE_OUT_LOW();

  FE_UART_Init();
  FE_TIMER_Init();
  mMasterStatus = MASTERINITIALIZED;
}

//--------------------------------------------------------------------------
/*!
\brief  . copy cmd frame from  layer7 and create CRC check.to start send the frame.
\author spring zhou
\date   2011-08-10
\param -void
\return -void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//--------------------------------------------------------------------------
void SendFrame_LAYER2(void)
{
  //Set the length=datas +6(msgType+command+frame number +data length+CRC)
  mTxLength = pTxBuf->datalen + 6; 

  //copy the send frame from layer7 to layer2
  memcpy(mSendBuffer, pTxBuf, mTxLength-2);
  
  pTxBuf->checkCRC = InitCRC16_COMMON();
  pTxBuf->checkCRC = CalcCrc16Mem_COMMON((TUSIGN8*)&mSendBuffer[0], pTxBuf->checkCRC, mTxLength-2);
  
  mSendBuffer[mTxLength-2] = (TUSIGN8)pTxBuf->checkCRC;
  mSendBuffer[mTxLength-1] = (pTxBuf->checkCRC >> 8); 
  
  /* Initialize receive counter to 0 */
  mBufferIndex = 0;
  mMasterStatus = MASTERWAITINGFORREPLY;

  /* Start DMA transmit */
  if(HAL_UART_Transmit_IT(&FE_UartHandle, (uint8_t*)mSendBuffer, mTxLength)!= HAL_OK)
  {
    Error_Handler();
  }
}

//--------------------------------------------------------------------------
/*!
\brief  . CRC Check for the receiverd data. if there is crc error. return FRAME_NOK_CRCERROR
\author spring zhou
\date   2010-08-10
\param -void
\return TUSIGN8
FRAME_OK
FRAME_CRCERROR
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//--------------------------------------------------------------------------
TUSIGN8 FrameCRCCheck(void)
{
  TUSIGN16 crcRec;
  TUSIGN16 tempcrc;
  TUSIGN8 result = FRAME_OK;
  TUSIGN16 size = (TUSIGN16)0;

  if(mBufferIndex >= UART_MIN_RCVDATA_SIZE)
  {
    tempcrc = InitCRC16_COMMON();
    size = mReceiverBuffer[3]+GENERAL_FRAME_HEADER_NUM;
    tempcrc = CalcCrc16Mem_COMMON((TUSIGN8*)&mReceiverBuffer[0], tempcrc, size);
    crcRec = (mReceiverBuffer[size+1]<<8)+ mReceiverBuffer[size];
    if(tempcrc == crcRec)
    {
      /* put the data into  frame buffer */
      (void)memcpy(pRxBuf,mReceiverBuffer,size);
      pRxBuf->checkCRC = crcRec;
    }
    else
    {
      result = FRAME_CRCERROR;
    }
  }
  else
  {
    result = FRAME_CRCERROR;
  }
  return result;
}

//--------------------------------------------------------------------------
/*!
\brief  . Framenum Check . if there is frarmnum error. return FRAMENUM_ERROR
\author spring zhou
\date   2011-07-10
\param -void
\return TUSIGN8
FRAMENUM_OK
FRAMENUM_ERROR
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//--------------------------------------------------------------------------
TUSIGN8 CheckFrameNum(void)
{
  T_FED_FRAME_RX_BUFFER *pRecBufTemp = (T_FED_FRAME_RX_BUFFER*)mReceiverBuffer;
  
  if(pRecBufTemp->framenum  == GetFrameNum())
  {
    return FRAMENUM_OK;
  }
  else
  {
    return FRAMENUM_ERROR;
  }
}

//--------------------------------------------------------------------------
/*!
\brief  . Check message status
\author spring zhou
\date   2012-09-12
\param -
\return TUSIGN8
FRAME_OK
FRAME_ERROR: FRAME_FE_CRC_ERROR;FRAME_FE_ACCESS_ERROR;FRAME_FE_BUSY
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//--------------------------------------------------------------------------
TUSIGN8 CheckMessageStatus(void)
{
  T_FED_FRAME_RX_BUFFER *pRecBufTemp = (T_FED_FRAME_RX_BUFFER*)mReceiverBuffer;

  if(pRecBufTemp->msgtype & FED_FRAMETYPE_CRCERROR)
  {
    return FRAME_FE_CRC_ERROR;
  }
  else if(pRecBufTemp->msgtype & FED_DATA_ACCESS_ERROR)
  {
    return FRAME_FE_ACCESS_ERROR;
  }
  else if(pRecBufTemp->msgtype & FED_FRAMETYPE_FEBUSY)
  {
    return FRAME_FE_BUSY;
  }
  else
  {
    return FRAME_OK;
  }
}

static void FE_TIMER_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == FE_TIMER_INSTANCE)
  {
    RTOS_BASETYPE xHigherPriorityTaskWoken;
    if (mMasterStatus == MASTERWAITINGFORREPLY)
    {
      HAL_TIM_Base_Stop_IT(&FE_TimHandle);
      mMasterStatus = MASTERIDLE;
      xHigherPriorityTaskWoken = RTOS_FALSE;
      NotifyISR_RTOS_TASK_NOTIFY(FrontendTskHandle, FERX_EVENT, RTOS_TASK_NOTIFY_ACTION_SETBITS, &xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  }
}


static void FE_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == FE_UART_INSTANCE)
  {
    if (mMasterStatus == MASTERWAITINGFORREPLY)
    {
      if (HAL_TIM_Base_Stop_IT(&FE_TimHandle) != HAL_OK)
      {
        Error_Handler();
      }
      __HAL_TIM_CLEAR_IT(&FE_TimHandle, TIM_IT_UPDATE);
      __HAL_TIM_SET_COUNTER(&FE_TimHandle, 0);
      /* Start the FE_TIMER Base generation in interrupt mode */
      if (HAL_TIM_Base_Start_IT(&FE_TimHandle) != HAL_OK)
      {
        Error_Handler();
      }

      if (mBufferIndex < MAX_LENGTH_REV)
      {
        mReceiverBuffer[mBufferIndex] = mRxData;
        mBufferIndex++;
      }

      /* Enable receive */
      if (HAL_UART_Receive_IT(&FE_UartHandle, (uint8_t *)&mRxData, 1)!= HAL_OK)
      {
        Error_Handler();
      }
    }
  }
}