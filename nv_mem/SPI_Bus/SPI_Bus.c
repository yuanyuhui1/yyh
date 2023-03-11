//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
    Microcontroller SPI bus master - single master bus
    using SPI bus interface
 Remarks

*/
//--------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "System/Interface/assert.h"
#include "System/Interface/microcontroller.h"
#include "SPI_Bus.h"

SPI_HandleTypeDef EEPROM_SpiHandle;

static void EEPROM_SPI_MspInitCallback(SPI_HandleTypeDef *hspi);

static void EEPROM_WPB_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  EEPROM_WPB_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = EEPROM_WPB_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EEPROM_WPB_GPIO_PORT, &GPIO_InitStruct);
}

static void EEPROM_HOLDB_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  EEPROM_HOLDB_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = EEPROM_HOLDB_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EEPROM_HOLDB_GPIO_PORT, &GPIO_InitStruct);
}

static void EEPROM_WPB_Enable(TBOOL bFlag)
{
  if(eTRUE == bFlag)
  {
    HAL_GPIO_WritePin(EEPROM_WPB_GPIO_PORT, EEPROM_WPB_GPIO_PIN, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(EEPROM_WPB_GPIO_PORT, EEPROM_WPB_GPIO_PIN, GPIO_PIN_SET);
  }
}

static void EEPROM_HOLDB_Enable(TBOOL bFlag)
{
  if(eTRUE == bFlag)
  {
    HAL_GPIO_WritePin(EEPROM_HOLDB_GPIO_PORT, EEPROM_HOLDB_GPIO_PIN, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(EEPROM_HOLDB_GPIO_PORT, EEPROM_HOLDB_GPIO_PIN, GPIO_PIN_SET);
  }
}

void EEPROM_SPI_Init(void)
{
  EEPROM_WPB_GPIO_Init();
  EEPROM_HOLDB_GPIO_Init();
  EEPROM_WPB_Enable(eFALSE);
  EEPROM_HOLDB_Enable(eFALSE);

  EEPROM_SpiHandle.Instance = EEPROM_SPI_INSTANCE;
  EEPROM_SpiHandle.Init.Mode = SPI_MODE_MASTER;
  EEPROM_SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
  EEPROM_SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  EEPROM_SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  EEPROM_SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  EEPROM_SpiHandle.Init.NSS = SPI_NSS_HARD_OUTPUT;
  EEPROM_SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  EEPROM_SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  EEPROM_SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  EEPROM_SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  EEPROM_SpiHandle.Init.CRCPolynomial = 0x0;
  EEPROM_SpiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  EEPROM_SpiHandle.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  EEPROM_SpiHandle.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  EEPROM_SpiHandle.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  EEPROM_SpiHandle.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  EEPROM_SpiHandle.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  EEPROM_SpiHandle.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  EEPROM_SpiHandle.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  EEPROM_SpiHandle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  EEPROM_SpiHandle.Init.IOSwap = SPI_IO_SWAP_DISABLE;

  if (HAL_SPI_RegisterCallback(&EEPROM_SpiHandle, HAL_SPI_MSPINIT_CB_ID,
                               &EEPROM_SPI_MspInitCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_SPI_Init(&EEPROM_SpiHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

static void EEPROM_SPI_MspInitCallback(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  static DMA_HandleTypeDef EEPROM_SPI_hdma_rx;
  static DMA_HandleTypeDef EEPROM_SPI_hdma_tx;
  
  if (hspi->Instance == EEPROM_SPI_INSTANCE)
  {
    /* SPI, Peripheral and DMA clock enable */
    EEPROM_SPI_CLK_ENABLE();
    EEPROM_CSB_GPIO_CLK_ENABLE();
    EEPROM_SCK_GPIO_CLK_ENABLE();
    EEPROM_MISO_GPIO_CLK_ENABLE();
    EEPROM_MOSI_GPIO_CLK_ENABLE();
    EEPROM_DMA_CLK_ENABLE();

    /* SPI GPIO Configuration */
    GPIO_InitStruct.Pin = EEPROM_CSB_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = EEPROM_CSB_GPIO_AF;
    HAL_GPIO_Init(EEPROM_CSB_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = EEPROM_SCK_GPIO_PIN;
    GPIO_InitStruct.Alternate = EEPROM_SCK_GPIO_AF;
    HAL_GPIO_Init(EEPROM_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = EEPROM_MISO_GPIO_PIN;
    GPIO_InitStruct.Alternate = EEPROM_MISO_GPIO_AF;
    HAL_GPIO_Init(EEPROM_MISO_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = EEPROM_MOSI_GPIO_PIN;
    GPIO_InitStruct.Alternate = EEPROM_MOSI_GPIO_AF;
    HAL_GPIO_Init(EEPROM_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the DMA */
    EEPROM_SPI_hdma_tx.Instance = EEPROM_SPI_TX_DMA_STREAM;
    EEPROM_SPI_hdma_tx.Init.Request = EEPROM_SPI_TX_DMA_REQUEST;
    EEPROM_SPI_hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    EEPROM_SPI_hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    EEPROM_SPI_hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
    EEPROM_SPI_hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    EEPROM_SPI_hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    EEPROM_SPI_hdma_tx.Init.Mode = DMA_NORMAL;
    EEPROM_SPI_hdma_tx.Init.Priority = DMA_PRIORITY_LOW;
    EEPROM_SPI_hdma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&EEPROM_SPI_hdma_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(hspi, hdmatx, EEPROM_SPI_hdma_tx);

    EEPROM_SPI_hdma_rx.Instance = EEPROM_SPI_RX_DMA_STREAM;
    EEPROM_SPI_hdma_rx.Init.Request = EEPROM_SPI_RX_DMA_REQUEST;
    EEPROM_SPI_hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    EEPROM_SPI_hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    EEPROM_SPI_hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
    EEPROM_SPI_hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    EEPROM_SPI_hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    EEPROM_SPI_hdma_rx.Init.Mode = DMA_NORMAL;
    EEPROM_SPI_hdma_rx.Init.Priority = DMA_PRIORITY_LOW;
    EEPROM_SPI_hdma_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&EEPROM_SPI_hdma_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(hspi, hdmarx, EEPROM_SPI_hdma_rx);

    /* Configure the NVIC for DMA */
    HAL_NVIC_SetPriority(EEPROM_SPI_DMA_TX_IRQn, EEPROM_SPI_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EEPROM_SPI_DMA_TX_IRQn);

    HAL_NVIC_SetPriority(EEPROM_SPI_DMA_RX_IRQn, EEPROM_SPI_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EEPROM_SPI_DMA_RX_IRQn);

    /* NVIC configuration for SPI transfer complete interrupt */
    HAL_NVIC_SetPriority(EEPROM_SPI_IRQn, EEPROM_SPI_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EEPROM_SPI_IRQn);

  }
}

void EEPROM_SPI_WriteStatusReg(TUSIGN8* RegStatus)
{
    TUSIGN16 nbBytes;
    TUSIGN8 TxBuffer[2];
    TxBuffer[0] = BR25G128_WRSR;
    TxBuffer[1] = *RegStatus;
    nbBytes = 2;
    EEPROM_WPB_Enable(eFALSE);
    if(HAL_SPI_Transmit_DMA(&EEPROM_SpiHandle, (uint8_t*)TxBuffer, nbBytes) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&EEPROM_SpiHandle) != HAL_SPI_STATE_READY)
    {
    }
    EEPROM_WPB_Enable(eTRUE);
}

void EEPROM_SPI_ReadStatusReg(TUSIGN8* pRxBuffer)
{
    TUSIGN16 nbBytes;
    TUSIGN8 TxBuffer[2] = {0};
    TUSIGN8 RxBuffer[2] = {0};
    TxBuffer[0] = BR25G128_RDSR;
    nbBytes = 2;
    EEPROM_WPB_Enable(eFALSE);
    if(HAL_SPI_TransmitReceive_DMA(&EEPROM_SpiHandle, (uint8_t*)TxBuffer, (uint8_t*)RxBuffer, nbBytes) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&EEPROM_SpiHandle) != HAL_SPI_STATE_READY)
    {
    }
    EEPROM_WPB_Enable(eTRUE);
    *pRxBuffer = RxBuffer[1];
}

void EEPROM_SPI_WriteCmd_Enable(TBOOL bFlag)
{
    TUSIGN16 nbBytes;
    TUSIGN8 TxBuffer = 0;

    if(eTRUE == bFlag)
    {
        TxBuffer = BR25G128_WREN;
    }
    else
    {
        TxBuffer = BR25G128_WRDI;
    }
    nbBytes = 1;
    EEPROM_WPB_Enable(eFALSE);
    if(HAL_SPI_Transmit_DMA(&EEPROM_SpiHandle, (uint8_t*)&TxBuffer, nbBytes) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&EEPROM_SpiHandle) != HAL_SPI_STATE_READY)
    {
    }
    EEPROM_WPB_Enable(eTRUE);
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief
    SPI bus data transfer format: master reads slave immediately after first byte
 \author
 \date
 \param
    slaveAddr       slave address of the i2c device
    ptrData         pointer to data to be read
    byteCount       size of data to read in byte
 \return
*/
//--------------------------------------------------------------------------------------------------
void EEPROM_SPI_ReadData(const TUSIGN16 ReadAddr, TUSIGN8* pRxBuffer, TUSIGN16 byteCount)
{
    
    TUSIGN16 nbBytes;
    TUSIGN8 TxBuffer[BR25G128_PAGE_MAX_SIZE] = {0};
    TUSIGN8 RxBuffer[BR25G128_PAGE_MAX_SIZE] = {0};
    VIP_ASSERT(ReadAddr < BR25G128_MEM_SIZE);
    VIP_ASSERT(pRxBuffer);

    TxBuffer[0] = BR25G128_READ;
    TxBuffer[1] = (ReadAddr >> 8) & 0xFF;
    TxBuffer[2] = (uint8_t)ReadAddr;
    nbBytes = byteCount + 3;
    EEPROM_WPB_Enable(eFALSE);
    if(HAL_SPI_TransmitReceive_DMA(&EEPROM_SpiHandle, (uint8_t*)TxBuffer, (uint8_t*)RxBuffer, nbBytes) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&EEPROM_SpiHandle) != HAL_SPI_STATE_READY)
    {
    }
    EEPROM_WPB_Enable(eTRUE);
    memcpy((void*)pRxBuffer, (void*)&RxBuffer[3], byteCount);
}

void EEPROM_SPI_WriteData(const TUSIGN16 WriteAddr, TUSIGN8* pTxBuffer, TUSIGN16 byteCount)
{
    
    TUSIGN16 nbBytes, i;
    TUSIGN8 TxBuffer[BR25G128_PAGE_MAX_SIZE + 3];

    VIP_ASSERT(WriteAddr < BR25G128_MEM_SIZE);
    VIP_ASSERT(pTxBuffer);
    VIP_ASSERT(byteCount < BR25G128_PAGE_MAX_SIZE);

    EEPROM_SPI_WriteCmd_Enable(eTRUE);

    TxBuffer[0] = BR25G128_WRITE;
    TxBuffer[1] = (WriteAddr >> 8) & 0xFF;
    TxBuffer[2] = (uint8_t)WriteAddr;
    nbBytes = byteCount + 3;
    for(i = 0; i < byteCount; i++)
    {
        TxBuffer[i + 3] = pTxBuffer[i];
    }
    EEPROM_WPB_Enable(eFALSE);
    if(HAL_SPI_Transmit_DMA(&EEPROM_SpiHandle, (uint8_t*)TxBuffer, nbBytes) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&EEPROM_SpiHandle) != HAL_SPI_STATE_READY)
    {
    }
    EEPROM_WPB_Enable(eTRUE);
}