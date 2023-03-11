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

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/microcontroller.h"
#include "Coordinator/Interface/TimeElapse.h"

static void TimeElapse_TIMER_BASE_MspInitCallback(TIM_HandleTypeDef *htim_base);
static TUSIGN32 elapseTimeCountLow = 0;
static TUSIGN8 timeStartFlag = 0xFF;

/* TIM handle declaration */
TIM_HandleTypeDef TimeElapse_TimHandle;

/**
  * @brief time elapse Timer Initialization Function
  * @param None
  * @retval None
  */
void TimeElapse_TIMER_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  uint32_t uwPrescalerValue = 0;
  /* -----------------------------------------------------------------------
    TIME_ELAPSE_TIMER input clock (TIME_ELAPSE_TIMER_CLK) is set to APB1 clock (PCLK1), 
    since APB1 prescaler is equal to 2.
    TIME_ELAPSE_TIMER_CLK = 2*PCLK1
    PCLK1 = HCLK/2 as AHB Clock divider is set to RCC_HCLK_DIV2
    => TIME_ELAPSE_TIMER_CLK = HCLK = SystemCoreClock
    Compute the prescaler value to have TIME_ELAPSE_TIMER counter clock equal to 1MHz
  ----------------------------------------------------------------------- */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 1000000) - 1;

  TimeElapse_TimHandle.Instance = TIME_ELAPSE_TIMER_INSTANCE;
  TimeElapse_TimHandle.Init.Prescaler = uwPrescalerValue;
  TimeElapse_TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimeElapse_TimHandle.Init.Period = 0xffff;
  TimeElapse_TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimeElapse_TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_RegisterCallback(&TimeElapse_TimHandle, HAL_TIM_BASE_MSPINIT_CB_ID,
                               &TimeElapse_TIMER_BASE_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_Base_Init(&TimeElapse_TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&TimeElapse_TimHandle, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TimeElapse_TimHandle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void TimeElapse_TIMER_BASE_MspInitCallback(TIM_HandleTypeDef *htim_base)
{
  if(htim_base->Instance == TIME_ELAPSE_TIMER_INSTANCE)
  {
    /* TIM Peripheral clock enable */
    TIME_ELAPSE_TIMER_CLK_ENABLE();
  }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function can be used to start timer
 \author Kevin Lu
 \date   2010-12-11
 \param :none
 \return : void
 \warning: none
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void TimeElapse_TIMER_Start()
{
  /* Stop the TIME_ELAPSE_TIMER */
  if (HAL_TIM_Base_Stop(&TimeElapse_TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /*Set counter to 0 */
  __HAL_TIM_SET_COUNTER(&TimeElapse_TimHandle, 0);

  /* Start the TIME_ELAPSE_TIMER */
  if (HAL_TIM_Base_Start(&TimeElapse_TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function can be get the high-resolution elapse time which is in us
 \author lawrence shi
 \date   2013-01-08
 \param  none
 \return : times in us
 \warning: none
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
static TUSIGN32 Calculate_us()
{
  TUSIGN32 time_us;
  time_us = __HAL_TIM_GET_COUNTER(&TimeElapse_TimHandle);
  TimeElapse_TIMER_Start();
  return time_us;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function can be get the high-resolution elapse time which is in ms,and accumate the remain us
 \author lawrence shi
 \date   2013-01-08
 \param  none
 \return : times in ms
 \warning: none
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN32 CalculateElapsedTime(void)
{	
  TUSIGN32 elapseTime = 0;
  if( timeStartFlag == 0xFF)
  {
    TimeElapse_TIMER_Start();
    timeStartFlag = 0;
  }

  if( timeStartFlag == 0)
  {
    elapseTime = Calculate_us();
  }

  elapseTimeCountLow += elapseTime%1000;
  elapseTime = elapseTime/1000;
  if(elapseTimeCountLow>1000)
  {
    elapseTime+=elapseTimeCountLow/1000;
    elapseTimeCountLow = elapseTimeCountLow%1000;
  }
  return elapseTime;

}