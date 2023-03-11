//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module         HAL for ADS1146
 Description    The hardware dreiver for ADS1146 SPI ADC
         The pubulic interface for ADS1146 is GetRawValue 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/microcontroller.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"


#include "AnalogInput_HW.h"

#define CHECK_DATA_LENGTH         4
#define MIN_SENDING_INTERVAL     10

_AD_CONVERTER adConverter;

SPI_HandleTypeDef SFE_SpiHandle;

static TUSIGN8 aiErrCnt = 0;
static TUSIGN8 spiBuffer[DATALENGTH];
static TUSIGN32 lastResetTime = 0x00;

static void SendOneByteSPI_AI(TCHAR data);
static void RecvOneByteSPI_AI(TCHAR* pData);
static void SFE_ADC_Cyclic_Reset(void);
static TBOOL Check_Stuck_At_Error(const TUSIGN8 *pBuf,TUSIGN8 size);
static void SFE_SPI_MspInitCallback(SPI_HandleTypeDef *hspi);

static void SFE_SPI_Init(void)
{
  SFE_SpiHandle.Instance = SFE_SPI_INSTANCE;
  SFE_SpiHandle.Init.Mode = SPI_MODE_MASTER;
  SFE_SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
  SFE_SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  SFE_SpiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SFE_SpiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
  SFE_SpiHandle.Init.NSS = SPI_NSS_HARD_OUTPUT;
  SFE_SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SFE_SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SFE_SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  SFE_SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SFE_SpiHandle.Init.CRCPolynomial = 0x0;
  SFE_SpiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  SFE_SpiHandle.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  SFE_SpiHandle.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  SFE_SpiHandle.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  SFE_SpiHandle.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  SFE_SpiHandle.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  SFE_SpiHandle.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  SFE_SpiHandle.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  SFE_SpiHandle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  SFE_SpiHandle.Init.IOSwap = SPI_IO_SWAP_DISABLE;

  if(HAL_SPI_RegisterCallback(&SFE_SpiHandle, HAL_SPI_MSPINIT_CB_ID,
                               &SFE_SPI_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_SPI_Init(&SFE_SpiHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

static void SFE_SPI_MspInitCallback(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(hspi->Instance == SFE_SPI_INSTANCE)
  {
    /* SPI, Peripheral clock enable */
    SFE_SPI_CLK_ENABLE();
    SFE_CS_N_GPIO_CLK_ENABLE();
    SFE_SCK_GPIO_CLK_ENABLE();
    SFE_MISO_GPIO_CLK_ENABLE();
    SFE_MOSI_GPIO_CLK_ENABLE();

    /* SPI GPIO Configuration */
    GPIO_InitStruct.Pin = SFE_CS_N_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = SFE_CS_N_GPIO_AF;
    HAL_GPIO_Init(SFE_CS_N_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SFE_SCK_GPIO_PIN;
    GPIO_InitStruct.Alternate = SFE_SCK_GPIO_AF;
    HAL_GPIO_Init(SFE_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SFE_MISO_GPIO_PIN;
    GPIO_InitStruct.Alternate = SFE_MISO_GPIO_AF;
    HAL_GPIO_Init(SFE_MISO_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SFE_MOSI_GPIO_PIN;
    GPIO_InitStruct.Alternate = SFE_MOSI_GPIO_AF;
    HAL_GPIO_Init(SFE_MOSI_GPIO_PORT, &GPIO_InitStruct);
  }
}

static void SFE_PMS_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  SFE_PMS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SFE_PMS_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SFE_PMS_GPIO_PORT, &GPIO_InitStruct);
}

static void SFE_POWER_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  SFE_3V0_EN_GPIO_CLK_ENABLE();
  SFE_3V3_EN_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = SFE_3V0_EN_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SFE_3V0_EN_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SFE_3V3_EN_GPIO_PIN;
  HAL_GPIO_Init(SFE_3V3_EN_GPIO_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(SFE_3V0_EN_GPIO_PORT, SFE_3V0_EN_GPIO_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SFE_3V3_EN_GPIO_PORT, SFE_3V3_EN_GPIO_PIN, GPIO_PIN_SET);
}

static void SFE_ADC_Reset(void)
{
  HAL_GPIO_WritePin(SFE_PMS_GPIO_PORT, SFE_PMS_GPIO_PIN, GPIO_PIN_RESET);
  Delay_RTOS_TASK(1);
  HAL_GPIO_WritePin(SFE_PMS_GPIO_PORT, SFE_PMS_GPIO_PIN, GPIO_PIN_SET);
  lastResetTime = GetTickCount_RTOS_TASK();
  Delay_RTOS_TASK(10);

} 
//
////-------------------------------------------------------------------------------------------------
///*!
// \brief  initialize the ADS1146 SPI register
// \author Spring-Kunli Zhou,Jax Yang,ZuoChen Wang
// \date   2018-03-6
// \param  none
// \return 0:initial ok
// \       1:initial fail
// \warning none
// \test
//      test-date:May 31 2013 10:41:22
//     \n by: Jax Yang
//     \n environment:IAR-CSPY with IO board
//     \n intention:  Cover branch,Cover statement,Cover decision
//     \n result module test: ok
// \bug none
//*/
////-------------------------------------------------------------------------------------------------
//
static void SFE_ADC_Init(void)
{
  SendOneByteSPI_AI(SLEEP);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  send one byte of the SPI communication
 \author Spring-Kunli Zhou,Jax Yang,ZuoChen.Wang
 \date   2018-03-06
 \param  none
 \return void
 \warning none
 \test
      test-date:
     \n by: 
     \n environment:IAR-CSPY with IO board
     \n intention:  Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
static void SendOneByteSPI_AI(TUSIGN8 data)
{
  TUSIGN8 txData = ~data;
  (void)HAL_SPI_Transmit(&SFE_SpiHandle, (uint8_t *)&txData, 1, MIN_SENDING_INTERVAL);
  Delay_RTOS_TASK(MIN_SENDING_INTERVAL);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  send one byte of the SPI communication
 \author Spring-Kunli Zhou,Jax Yang,ZuoChen.Wang
 \date   2018-03-06
\param  TCHAR* pData:pointer to recevie buffer pointer
 \return void
 \warning none
 \test
      test-date:
     \n by: 
     \n environment:IAR-CSPY with IO board
     \n intention:  Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
static void RecvOneByteSPI_AI(TUSIGN8* pData)
{
  (void)HAL_SPI_Receive(&SFE_SpiHandle, (uint8_t *)pData, 1, MIN_SENDING_INTERVAL);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initial the hardware driver
 \author Spring-Kunli Zhou,Jax Yang
 \date   2011-05-10
 \param  none
 \return TUSIGN16
 \warning none
 \test
      test-date:May 31 2013 10:41:22
     \n by: Jax Yang
     \n environment:IAR-CSPY with IO board
     \n intention:  Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 HW_initial_AI(void)
{

  TUSIGN16 result = (TUSIGN16)OK;

  adConverter.rawValue = 0;
  adConverter.state = 0;

  SFE_POWER_GPIO_Init();
  SFE_PMS_GPIO_Init();
  SFE_SPI_Init();

#ifdef SFE_ADC_RESET_FEATURE_ENABLE
  SFE_ADC_Reset();
#endif
  SFE_ADC_Init();

  return result;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  get the raw data from the AD converter 
 \author Spring-Kunli Zhou,Jax Yang
 \date   2011-05-10
 \param  void
 \return void
 \warning none
 \test
      test-date:May 31 2013 10:41:22
     \n by: Jax Yang
     \n environment:IAR-CSPY with IO board
     \n intention:  Cover branch,Cover statement,Cover decision
     \n result module test: ok
 \bug none
*/
//-------------------------------------------------------------------------------------------------
void GetRawValue(void)
{
  static TUSIGN16 rawValue = 0;

  SendOneByteSPI_AI(RDATA);
  SendOneByteSPI_AI(LP);
  RecvOneByteSPI_AI(&spiBuffer[0]);
  SendOneByteSPI_AI(RREG00);
  RecvOneByteSPI_AI(&spiBuffer[1]);
  //for low power comsumption,make the final AD dout low.
  SendOneByteSPI_AI(NBYTE01);
  RecvOneByteSPI_AI(&spiBuffer[2]);
  SendOneByteSPI_AI(LP);
  RecvOneByteSPI_AI(&spiBuffer[3]);
  SendOneByteSPI_AI(SLEEP);

  adConverter.rawValue = (((TUSIGN8)(~spiBuffer[0]) << 8) + (TUSIGN8)(~spiBuffer[1]));

  //Any of the error are the SPI Com error
  if(Check_Stuck_At_Error(spiBuffer, CHECK_DATA_LENGTH))
  {
    // accumulate the ai Error.
    aiErrCnt++;
    adConverter.rawValue = rawValue;   //Use last good condition values.
    
    //If reach  reset condition then do reset.
    if((aiErrCnt % MAX_ADC_RESET_ERR) == 0x00)
    {
#ifdef SFE_ADC_RESET_FEATURE_ENABLE
      SFE_ADC_Reset();
#endif
      SFE_ADC_Init();
    }

    //If reach alarm then give out alarm.  and reset all count.
    if(aiErrCnt >= MAX_SPI_COMM_ERR)
    {
      adConverter.state |= ERR_SPICOM_FAIL;
      //Make sure still have reset even increase the error.
      if(aiErrCnt > MAX_SPI_COMM_ERR + MAX_ADC_RESET_ERR)
      {
        aiErrCnt = MAX_SPI_COMM_ERR;
      }
    }
  }
  else
  {
    rawValue = adConverter.rawValue;
    if(aiErrCnt > 0)
    {
      aiErrCnt--;
    }
    if((adConverter.state & ERR_SPICOM_FAIL) && (aiErrCnt < SPI_ERR_RECOVER_CHECK_TIMES))
    {
      adConverter.state &= ~ERR_SPICOM_FAIL;
    }
  }

  SFE_ADC_Cyclic_Reset();
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  get the raw data from the AD converter 
 \author  CNZUWAN
 \date   2018-11-16
 \param  void
 \return void
 \warning none
 \test

 \bug none
*/
//-------------------------------------------------------------------------------------------------
static TBOOL Check_Stuck_At_Error(const TUSIGN8 *pBuf, TUSIGN8 size)
{
  TBOOL result = eTRUE;
  TUSIGN8 idx = 0;

  for(idx = 0; (idx < size - 1) && (idx < DATALENGTH - 1); idx++)
  {
    if(pBuf[idx] != pBuf[idx+1])
    {
      result = eFALSE;
      break;
    }
  }
  return result;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  get the raw data from the AD converter 
 \author  CNZUWAN
 \date   2018-11-16
 \param  void
 \return void
 \warning none
 \test

 \bug none
*/
//-------------------------------------------------------------------------------------------------

static void SFE_ADC_Cyclic_Reset(void)
{
  TUSIGN32 resetIntervalTime = GetTickCount_RTOS_TASK() - lastResetTime;
  if( resetIntervalTime > CYCLIC_RESET_TIMER)
  {
#ifdef SFE_ADC_RESET_FEATURE_ENABLE
    SFE_ADC_Reset();
#endif
    SFE_ADC_Init();
  }
  else
  {
  }
}