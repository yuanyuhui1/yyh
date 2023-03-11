//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2007.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem ElettronicServices
Module
Description    Implementation of overloaded methods
for Subsystems based on T_UNIT.

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "Hmi/Interface/HMI.h"
#include "Hmi/Interface/HMI_idx.h"
#include "nv_mem/Interface/nv_mem.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "ElettronicServices/Source/ElettronicServices_types.h"
#include "ElettronicServices/Source/ElettronicServices_attributes.h"
#include "ElettronicServices/Source/ElettronicServices_overload.h"
#include "ElettronicServices/Source/ElettronicServices_execute.h"
#include "ElettronicServices/Source/elettronicServices_cpu.h"
#include "ElettronicServices/Source/ElettronicServices_InOut.h"
#include "ElettronicServices/Source/flash_operation.h"
#include "ElettronicServices/Source/versioncode.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Layer7/servicePort_layer7.h"
#include "ElettronicServices/Source/LP3907.h"
#include "ElettronicServices/Source/TMP112.h"

#ifdef MV 
#ifdef HART_COMMUNICATION 
#include "../../DigitalOut/interface/DigitalOut.h"
#endif
#endif

/* Private define ------------------------------------------------------------*/
#define CONTINUOUS_CONVERSATION_TIMES     ((uint32_t)16)
/* Size of array adc1ConvertedData[] */
#define ADC1_CONVERTED_DATA_BUFFER_SIZE   (ADC1_CHANNEL_NUM * CONTINUOUS_CONVERSATION_TIMES)
/* Size of array adc2ConvertedData[] */
#define ADC2_CONVERTED_DATA_BUFFER_SIZE   (ADC2_CHANNEL_NUM * CONTINUOUS_CONVERSATION_TIMES)
/* Max digital value with a full range of 16 bits */
#define RANGE_16BITS                      ((uint32_t)65535)

/* I2C TIMING Register define when I2C clock source is PLL3R = 20MHz */
/*
* - 100kHz, 100ns rise/fall: 0x00604E6E;
* - 400kHz, 100ns rise/fall: 0x0030081E;
*/
#define TMP_PMIC_I2C_100KHZ               0x00604E6E
#define TMP_PMIC_I2C_400KHZ               0x0030081E

/* Private variables */
static uint32_t HAL_RCC_ADC12_CLK_ENABLED = 0;

/* ADC1 and ADC2 handler declaration */
ADC_HandleTypeDef Adc1Handle;
ADC_HandleTypeDef Adc2Handle;

/* TMP and PMIC I2C handler declaration */
I2C_HandleTypeDef TMP_PMIC_I2cHandle;

/* Variable containing ADC conversions data */
ALIGN_32BYTES(volatile uint16_t adc1ConvertedData[CONTINUOUS_CONVERSATION_TIMES][ADC1_CHANNEL_NUM]);
ALIGN_32BYTES(volatile uint16_t adc2ConvertedData[CONTINUOUS_CONVERSATION_TIMES][ADC2_CHANNEL_NUM]);

TUSIGN16 adc1Results[ADC1_CHANNEL_NUM];
TUSIGN16 adc2Results[ADC2_CHANNEL_NUM];

/* Private function prototypes */
static void DipSwitch_Init_ElectronicServices(void);
#ifdef PUSH_BUTTON
static void PushButton_Init_ElectronicServices(void);
#endif
static void HAL_ADC_MspInitCallback(ADC_HandleTypeDef *hadc);
static void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
static void ADC1_Init_ElectronicServices(void);
static void ADC2_Init_ElectronicServices(void);
static void ADC1_Stop_ElectronicServices(void);
static void ADC2_Stop_ElectronicServices(void);
static void TMP_PMIC_I2C_MspInitCallback(I2C_HandleTypeDef *hi2c);
static void TMP_PMIC_I2C_Init_ElectronicServices(void);
static void LP3907_Config_ElectronicServices(void);
static void TMP112_Config_ElectronicServices(void);




/* Private function definition */
static void DipSwitch_Init_ElectronicServices(void)
{
#ifndef LOCALSERVER_SELECT
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    DIP_SWITCH_1_GPIO_CLK_ENABLE();
    DIP_SWITCH_2_GPIO_CLK_ENABLE();
    DIP_SWITCH_3_GPIO_CLK_ENABLE();
    DIP_SWITCH_4_GPIO_CLK_ENABLE();
    DIP_SWITCH_5_GPIO_CLK_ENABLE();
    DIP_SWITCH_6_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = DIP_SWITCH_1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DIP_SWITCH_1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIP_SWITCH_2_GPIO_PIN;
    HAL_GPIO_Init(DIP_SWITCH_2_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIP_SWITCH_3_GPIO_PIN;
    HAL_GPIO_Init(DIP_SWITCH_3_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIP_SWITCH_4_GPIO_PIN;
    HAL_GPIO_Init(DIP_SWITCH_4_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIP_SWITCH_5_GPIO_PIN;
    HAL_GPIO_Init(DIP_SWITCH_5_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIP_SWITCH_6_GPIO_PIN;
    HAL_GPIO_Init(DIP_SWITCH_6_GPIO_PORT, &GPIO_InitStruct);
#endif
}

#ifdef PUSH_BUTTON
static void PushButton_Init_ElectronicServices(void)
{
#ifndef LOCALSERVER_SELECT
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    PUSH_BUTTON_ZERO_GPIO_CLK_ENABLE();
    PUSH_BUTTON_SPAN_GPIO_CLK_ENABLE();
    PUSH_BUTTON_WRITE_PROTECT_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = PUSH_BUTTON_ZERO_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PUSH_BUTTON_ZERO_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PUSH_BUTTON_SPAN_GPIO_PIN;
    HAL_GPIO_Init(PUSH_BUTTON_SPAN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PUSH_BUTTON_WRITE_PROTECT_GPIO_PIN;
    HAL_GPIO_Init(PUSH_BUTTON_WRITE_PROTECT_GPIO_PORT, &GPIO_InitStruct);
#endif
}
#endif

static void ADC1_Init_ElectronicServices(void)
{
    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    Adc1Handle.Instance = ADC1;
    Adc1Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
    Adc1Handle.Init.Resolution = ADC_RESOLUTION_16B;
    Adc1Handle.Init.ScanConvMode = ADC_SCAN_ENABLE;
    Adc1Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    Adc1Handle.Init.LowPowerAutoWait = DISABLE;
    Adc1Handle.Init.ContinuousConvMode = ENABLE;
    Adc1Handle.Init.NbrOfConversion = ADC1_CHANNEL_NUM;
    Adc1Handle.Init.DiscontinuousConvMode = DISABLE;
    Adc1Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    Adc1Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    Adc1Handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    Adc1Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    Adc1Handle.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
    Adc1Handle.Init.OversamplingMode = DISABLE;

    if (HAL_ADC_RegisterCallback(&Adc1Handle, HAL_ADC_MSPINIT_CB_ID,
                                &HAL_ADC_MspInitCallback) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_Init(&Adc1Handle) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_RegisterCallback(&Adc1Handle, HAL_ADC_CONVERSION_COMPLETE_CB_ID,
                                &HAL_ADC_ConvCpltCallback) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure the ADC multi-mode */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&Adc1Handle, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    /* Start calibration */
    if (HAL_ADCEx_Calibration_Start(&Adc1Handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = HW_VER2_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_1V1_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = HW_VER1_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

static void ADC2_Init_ElectronicServices(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    Adc2Handle.Instance = ADC2;
    Adc2Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
    Adc2Handle.Init.Resolution = ADC_RESOLUTION_16B;
    Adc2Handle.Init.ScanConvMode = ADC_SCAN_ENABLE;
    Adc2Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    Adc2Handle.Init.LowPowerAutoWait = DISABLE;
    Adc2Handle.Init.ContinuousConvMode = ENABLE;
    Adc2Handle.Init.NbrOfConversion = ADC2_CHANNEL_NUM;
    Adc2Handle.Init.DiscontinuousConvMode = DISABLE;
    Adc2Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    Adc2Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    Adc2Handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    Adc2Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    Adc2Handle.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
    Adc2Handle.Init.OversamplingMode = DISABLE;

    if (HAL_ADC_RegisterCallback(&Adc2Handle, HAL_ADC_MSPINIT_CB_ID,
                                &HAL_ADC_MspInitCallback) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_Init(&Adc2Handle) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_RegisterCallback(&Adc2Handle, HAL_ADC_CONVERSION_COMPLETE_CB_ID,
                                &HAL_ADC_ConvCpltCallback) != HAL_OK)
    {
        Error_Handler();
    }

    /* Start calibration */
    if (HAL_ADCEx_Calibration_Start(&Adc2Handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_BUS_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = CUR_BUS_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_1V2_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_1V8A_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_3V3_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_3V0_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_6;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_REF_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_7;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = VOL_1V8_ADC_CHAN;
    sConfig.Rank = ADC_REGULAR_RANK_8;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = ADC_CHANNEL_VBAT;
    sConfig.Rank = ADC_REGULAR_RANK_9;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_10;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure Regular Channel */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_11;
    if (HAL_ADC_ConfigChannel(&Adc2Handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
static void HAL_ADC_MspInitCallback(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  static DMA_HandleTypeDef hdma_adc1;
  static DMA_HandleTypeDef hdma_adc2;

  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    HW_VER2_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_1V1_ADC_CHAN_GPIO_CLK_ENABLE();
    HW_VER1_ADC_CHAN_GPIO_CLK_ENABLE();

    /* Enable DMA clock */
    ADC1_DMA_CLK_ENABLE();

    GPIO_InitStruct.Pin = HW_VER2_ADC_CHAN_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HW_VER2_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_1V1_ADC_CHAN_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VOL_1V1_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = HW_VER1_ADC_CHAN_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HW_VER1_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    hdma_adc1.Instance = ADC1_DMA_STREAM;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);

    /* NVIC configuration for DMA Input data interrupt */
    HAL_NVIC_SetPriority(ADC1_DMA_STREAM_IRQn, ADC1_DMA_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ADC1_DMA_STREAM_IRQn);
  }
  else if(hadc->Instance==ADC2)
  {
    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    VOL_BUS_ADC_CHAN_GPIO_CLK_ENABLE();
    CUR_BUS_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_1V2_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_1V8A_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_3V3_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_3V0_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_REF_ADC_CHAN_GPIO_CLK_ENABLE();
    VOL_1V8_ADC_CHAN_GPIO_CLK_ENABLE();

    /* Enable DMA clock */
    ADC2_DMA_CLK_ENABLE();

    GPIO_InitStruct.Pin = VOL_1V2_ADC_CHAN_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VOL_1V2_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_1V8A_ADC_CHAN_GPIO_PIN;
    HAL_GPIO_Init(VOL_1V8A_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_3V3_ADC_CHAN_GPIO_PIN;
    HAL_GPIO_Init(VOL_3V3_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_3V0_ADC_CHAN_GPIO_PIN;
    HAL_GPIO_Init(VOL_3V0_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_REF_ADC_CHAN_GPIO_PIN;
    HAL_GPIO_Init(VOL_REF_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VOL_1V8_ADC_CHAN_GPIO_PIN;
    HAL_GPIO_Init(VOL_1V8_ADC_CHAN_GPIO_PORT, &GPIO_InitStruct);

    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_OPEN);
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_OPEN);

    /* ADC2 DMA Init */
    /* ADC2 Init */
    hdma_adc2.Instance = ADC2_DMA_STREAM;
    hdma_adc2.Init.Request = DMA_REQUEST_ADC2;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc2);

    /* NVIC configuration for DMA Input data interrupt */
    HAL_NVIC_SetPriority(ADC2_DMA_STREAM_IRQn, ADC2_DMA_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ADC2_DMA_STREAM_IRQn);
  }
}

/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
static void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    TUSIGN32 repetitiveAdc1Results[ADC1_CHANNEL_NUM] = {0};
    TUSIGN32 repetitiveAdc2Results[ADC2_CHANNEL_NUM] = {0};
    TUSIGN8 channelLoop, timesLoop;
    if (hadc->Instance==ADC1)
    {
        for (channelLoop = 0; channelLoop < ADC1_CHANNEL_NUM; channelLoop++)
        {
            for (timesLoop = 0; timesLoop < CONTINUOUS_CONVERSATION_TIMES; timesLoop++)
            {
                repetitiveAdc1Results[channelLoop] += adc1ConvertedData[timesLoop][channelLoop];
            }
            adc1Results[channelLoop] = (TUSIGN16)(repetitiveAdc1Results[channelLoop] / CONTINUOUS_CONVERSATION_TIMES);
        }
        ADC1_Stop_ElectronicServices();
    }
    else if(hadc->Instance==ADC2)
    {
        for (channelLoop = 0; channelLoop < ADC2_CHANNEL_NUM; channelLoop++)
        {
            for (timesLoop = 0; timesLoop < CONTINUOUS_CONVERSATION_TIMES; timesLoop++)
            {
                repetitiveAdc2Results[channelLoop] += adc2ConvertedData[timesLoop][channelLoop];
            }
            adc2Results[channelLoop] = (TUSIGN16)(repetitiveAdc2Results[channelLoop] / CONTINUOUS_CONVERSATION_TIMES);
        }
        ADC2_Stop_ElectronicServices();
    }
}

void ADC1_Start_ElectronicServices(void)
{
    /* Start conversion in DMA mode */
    if (HAL_ADC_Start_DMA(&Adc1Handle, (uint32_t *)adc1ConvertedData, ADC1_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }
}

static void ADC1_Stop_ElectronicServices(void)
{
    /* Stop conversion in DMA mode */
    if (HAL_ADC_Stop_DMA(&Adc1Handle) != HAL_OK)
    {
        Error_Handler();
    }
}

void ADC2_Start_ElectronicServices(void)
{
    /* Start conversion in DMA mode */
    if (HAL_ADC_Start_DMA(&Adc2Handle, (uint32_t *)adc2ConvertedData, ADC2_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }
}

static void ADC2_Stop_ElectronicServices(void)
{
    /* Stop conversion in DMA mode */
    if (HAL_ADC_Stop_DMA(&Adc2Handle) != HAL_OK)
    {
        Error_Handler();
    }
}

static void TMP_PMIC_I2C_Init_ElectronicServices(void)
{
    TMP_PMIC_I2cHandle.Instance = TMP_PMIC_I2C_INSTANCE;
    TMP_PMIC_I2cHandle.Init.Timing = TMP_PMIC_I2C_400KHZ;
    TMP_PMIC_I2cHandle.Init.OwnAddress1 = 0;
    TMP_PMIC_I2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    TMP_PMIC_I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    TMP_PMIC_I2cHandle.Init.OwnAddress2 = 0;
    TMP_PMIC_I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    TMP_PMIC_I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    TMP_PMIC_I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_RegisterCallback(&TMP_PMIC_I2cHandle, HAL_I2C_MSPINIT_CB_ID,
                                 &TMP_PMIC_I2C_MspInitCallback) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_I2C_Init(&TMP_PMIC_I2cHandle) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&TMP_PMIC_I2cHandle, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&TMP_PMIC_I2cHandle, 0) != HAL_OK)
    {
        Error_Handler();
    }
}

static void TMP_PMIC_I2C_MspInitCallback(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hi2c->Instance == TMP_PMIC_I2C_INSTANCE)
    {
        TMP_PMIC_I2C_CLK_ENABLE();
        TMP_PMIC_I2C_SCL_GPIO_CLK_ENABLE();
        TMP_PMIC_I2C_SDA_GPIO_CLK_ENABLE();

        GPIO_InitStruct.Pin = TMP_PMIC_I2C_SCL_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = TMP_PMIC_I2C_SCL_GPIO_AF;
        HAL_GPIO_Init(TMP_PMIC_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = TMP_PMIC_I2C_SDA_GPIO_PIN;
        GPIO_InitStruct.Alternate = TMP_PMIC_I2C_SDA_GPIO_AF;
        HAL_GPIO_Init(TMP_PMIC_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(TMP_PMIC_I2C_EV_IRQn, TMP_PMIC_I2C_IT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(TMP_PMIC_I2C_EV_IRQn);
    }
}

/**
  * @brief  Read LP3907 register value
  */
void LP3907_Read_Reg(uint16_t regAddress, uint8_t *pRegValue)
{
    if(HAL_I2C_Mem_Read_IT(&TMP_PMIC_I2cHandle, (uint16_t)(LP3907_I2C_ADDRESS), regAddress,
                           I2C_MEMADD_SIZE_8BIT, pRegValue, LP3907_REGISTER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }

    /* Wait for the end of the transfer */
    while (HAL_I2C_GetState(&TMP_PMIC_I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

/**
  * @brief Write LP3907 register value
  */
void LP3907_Write_Reg(uint16_t regAddress, uint8_t *pRegValue)
{
    if(HAL_I2C_Mem_Write_IT(&TMP_PMIC_I2cHandle, (uint16_t)(LP3907_I2C_ADDRESS), regAddress,
                           I2C_MEMADD_SIZE_8BIT, pRegValue, LP3907_REGISTER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }

    /* Wait for the end of the transfer */
    while (HAL_I2C_GetState(&TMP_PMIC_I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

/**
  * @brief  TMP112 Read Register
  */
void TMP112_Read_Reg(uint16_t regAddress, uint8_t *pRegValue)
{
    if (HAL_I2C_Mem_Read_IT(&TMP_PMIC_I2cHandle, (uint16_t)(TMP112_I2C_ADDRESS), regAddress,
                            I2C_MEMADD_SIZE_8BIT, pRegValue, TMP112_REGISTER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }

    /* Wait for the end of the transfer */  
    while (HAL_I2C_GetState(&TMP_PMIC_I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

/**
  * @brief TMP112 write Register
  */
void TMP112_Write_Reg(uint16_t regAddress, uint8_t *pRegValue)
{
    if (HAL_I2C_Mem_Write_IT(&TMP_PMIC_I2cHandle, (uint16_t)TMP112_I2C_ADDRESS, regAddress,
                             I2C_MEMADD_SIZE_8BIT, pRegValue, TMP112_REGISTER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }

    /* Wait for the end of the transfer */  
    while (HAL_I2C_GetState(&TMP_PMIC_I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

static void LP3907_Config_ElectronicServices(void)
{
    uint16_t ui16RegAdd = 0;
    uint8_t ui8RegVal = 0;

    ui16RegAdd = BKLDOEN_ADDRESS;
    LP3907_Read_Reg(ui16RegAdd, &ui8RegVal);

    /* Config LDO1 output voltage 3.0V */
    ui16RegAdd = LDO1VCR_ADDRESS;
    ui8RegVal = LDO_OUT_3V0;
    LP3907_Write_Reg(ui16RegAdd, &ui8RegVal);

    /* Config LDO2 output voltage 1.8V */
    ui16RegAdd = LDO2VCR_ADDRESS;
    ui8RegVal = LDO_OUT_1V8;
    LP3907_Write_Reg(ui16RegAdd, &ui8RegVal);
}

static void TMP112_Config_ElectronicServices(void)
{
    uint16_t ui16RegAdd = 0;
    uint8_t ui8RegVal[TMP112_REGISTER_SIZE] = {0};

    /* Config Configuration Register */
    ui16RegAdd = TMP112_CONFIG_REGISTER;
    ui8RegVal[0] = CONFIG_REGISTER_BYTE_1;
    ui8RegVal[1] = CONFIG_REGISTER_BYTE_2;
    TMP112_Write_Reg(ui16RegAdd, ui8RegVal);

    /* Config the T-LOW limit register */
    ui16RegAdd = TMP112_TLOW_REGISTER;
    ui8RegVal[0] = (uint8_t)((TLOW_LIMIT_TMP_REG >> 8) & 0xFF);
    ui8RegVal[1] = (uint8_t)(TLOW_LIMIT_TMP_REG & 0xFF);
    TMP112_Write_Reg(ui16RegAdd, ui8RegVal);

    /* Config the T-HIGH limit register */
    ui16RegAdd = TMP112_THIGH_REGISTER;
    ui8RegVal[0] = (uint8_t)((THIGH_LIMIT_TMP_REG >> 8) & 0xFF);
    ui8RegVal[1] = (uint8_t)(THIGH_LIMIT_TMP_REG & 0xFF);
    TMP112_Write_Reg(ui16RegAdd, ui8RegVal);
}

/**
  * @brief This function read temperature from TMP112
  */
void TMP112_ReadTemp_ElectronicServices(TFLOAT *pTempData)
{
    uint16_t ui16RegAdd = 0;
    uint8_t ui8RegVal[TMP112_REGISTER_SIZE] = {0};
    uint16_t ui16Data = 0;

#if (TMP112_SHUTDOWN_MODE == 1)
    /* Instruct sensor to One-Shot (OS) Conversion */
    ui16RegAdd = TMP112_CONFIG_REGISTER;
    ui8RegVal[0] = CONFIG_REGISTER_BYTE_1 | ONESHORT_MODE_1;
    ui8RegVal[1] = CONFIG_REGISTER_BYTE_2;
    TMP112_Write_Reg(ui16RegAdd, ui8RegVal);

    /* Wait Conversion at least 35 ms */
    Delay_RTOS_TASK(TMP112_CONVERSION_TIME);
#endif

    /* Read the temperature register */
    ui16RegAdd = TMP112_TEMP_REGISTER;
    TMP112_Read_Reg(ui16RegAdd, ui8RegVal);

    /* convert the digital data format to temperature */
    ui16Data = (ui8RegVal[0] << 8) | ui8RegVal[1];
    *pTempData = (TFLOAT)((TINT16)ui16Data >> NORMAL_MODE_TMP_BITS_SHIFT) * NORMAL_MODE_RESOLUTION;
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  initialize the subsystem; (hardware, tasks, data, calculation)
\author 	Roberto Colombo
\date
\param  typeOfStartUp
<pre>
INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
INIT_DATA       =0x20  initialize data and data structures
INIT_CALCULATION=0x10  initialize calculations used by the subsystem
</pre>
\param  me subsystem index
\return error-code
<pre>
OK                      operation was successful
ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
ERROR_RAM_STORAGE       attributes inside the ram are corrupted
ERROR_HW                hardware used by the subsystem doesn't work
</pre>
\test
\n by: 				Ganapathi R
\n intention: 			Code Coverage 100%
\n result module test:	OK
\n result Lint Level 3: 	OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_ELETTRONIC_SERVICES(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    TUSIGN16 result = FATAL_ERROR;
    TUSIGN8 calibrationdata = 0x0;
    TUSIGN8 versionNum[3];
    TFLOAT tempValue = 0.0f;

    VIP_ASSERT(me==ELETTRONIC_SERVICES_ME);
    
    result = Initialize_T_UNIT(me,typeOfStartUp);
    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    if( result==OK )
    {       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
            DipSwitch_Init_ElectronicServices();
#ifdef PUSH_BUTTON
            PushButton_Init_ElectronicServices();
#endif
            TMP_PMIC_I2C_Init_ElectronicServices();
            LP3907_Config_ElectronicServices();
            TMP112_Config_ElectronicServices();
            ADC1_Init_ElectronicServices();
            ADC2_Init_ElectronicServices();
            
#if 0
            (void)API_TestCPU();
            SafetyPinsTest();
#endif
#ifdef HART_COMMUNICATION
            RamCheck_ElectronicServices(START_UP);
            RomCheckInitCrc_ElectronicServices();
            RomCheckStartup_ElectronicServices();
#endif
        }
        
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        if( (typeOfStartUp & INIT_DATA) != 0)
        {
            ADC1_Start_ElectronicServices();
            ADC2_Start_ElectronicServices();
            TMP112_ReadTemp_ElectronicServices(&tempValue);

            
            (void)elettronicServices.unit.LoadRomDefaults(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_DC_IDX_DynamicDuplicated);
            (void)elettronicServices.unit.LoadRomDefaults(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_DC_IDX_Unprotected);
            (void)elettronicServices.unit.LoadRomDefaults(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_DC_IDX_Protected);
            
            // add for default calibration data. fix one bug cause by 2wire version 1.6.0
            ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
            calibrationdata = elettronicServicesStaticFrequentStaticDefault.hardwareCalibrationLoad;
            DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
            if(calibrationdata != ELETTRONICSERVICES_CALIBRATION_LOAD)
            {
                result |= LoadRomDefaults_T_UNIT(ELETTRONIC_SERVICES_ME, ELETTRONICSERVICES_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT);
            }
            // add CRC check for rare change ram data
            elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_CALCULATE;
            /* Read SNR threshold and front end version number. */
            result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_READ_SNR_THRESHOLD,ELETTRONICSERVICES_SNR_THRESHOLD_LENGTH,comRead_es[ELETTRONICSERVICES_READ_SNR_THRESHOLD_INDEX],NULL);
            result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_READ_FE_VERSION,ELETTRONICSERVICES_FE_VERSION_LENGTH,comRead_es[ELETTRONICSERVICES_READ_FE_VERSION_INDEX],NULL);
            result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_READ_AUTOZERO_STATUS,ELETTRONICSERVICES_AUTOZERO_STAUS_LENGTH,comRead_es[ELETTRONICSERVICES_READ_AUTOZERO_STATUS_INDEX],NULL);
            /* Read the bootloader */
#if 0
            (void)memcpy((void *)&versionNum[0], (const void *)(ELETTRONICSERVICES_BOOT_VER_NUM_ADDR), ELETTRONICSERVICES_BOOT_VER_NUM_LEN);
#endif
            elettronicServicesUnprotected.bootloaderRevision[0] = (versionNum[0]/0x0A) + 0x30;
            elettronicServicesUnprotected.bootloaderRevision[1] = (versionNum[0]%0x0A) + 0x30;
            elettronicServicesUnprotected.bootloaderRevision[2] = '.';
            elettronicServicesUnprotected.bootloaderRevision[3] = (versionNum[1]/0x0A) + 0x30;
            elettronicServicesUnprotected.bootloaderRevision[4] = (versionNum[1]%0x0A) + 0x30;
            elettronicServicesUnprotected.bootloaderRevision[5] = '.';
            elettronicServicesUnprotected.bootloaderRevision[6] = (versionNum[2]/0x0A) + 0x30;
            elettronicServicesUnprotected.bootloaderRevision[7] = (versionNum[2]%0x0A) + 0x30;
#if 0           
            Read_Flashdata(versionNum,(TUSIGN8 *)BLOCK_A,3); 
#endif
            elettronicServicesUnprotected.swRevisionHmi_transmitter[0] += (TUSIGN8)(CB_SW_VersionCode.swVersion[0]/0x0A);
            elettronicServicesUnprotected.swRevisionHmi_transmitter[1] += (TUSIGN8)(CB_SW_VersionCode.swVersion[0]%0x0A);
            elettronicServicesUnprotected.swRevisionHmi_transmitter[3] += (TUSIGN8)(CB_SW_VersionCode.swVersion[1]/0x0A);
            elettronicServicesUnprotected.swRevisionHmi_transmitter[4] += (TUSIGN8)(CB_SW_VersionCode.swVersion[1]%0x0A);
            elettronicServicesUnprotected.swRevisionHmi_transmitter[6] += (TUSIGN8)(CB_SW_VersionCode.swVersion[2]/0x0A);
            elettronicServicesUnprotected.swRevisionHmi_transmitter[7] += (TUSIGN8)(CB_SW_VersionCode.swVersion[2]%0x0A);
            
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[0] += (TUSIGN8)(versionNum[0]/0x0A);
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[1] += (TUSIGN8)(versionNum[0]%0x0A);
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[3] += (TUSIGN8)(versionNum[1]/0x0A);
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[4] += (TUSIGN8)(versionNum[1]%0x0A);
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[6] += (TUSIGN8)(versionNum[2]/0x0A);
            elettronicServicesUnprotected.hwRevisionHmi_transmitter[7] += (TUSIGN8)(versionNum[2]%0x0A);
            
            elettronicServicesUnprotected.hardwareRevision[0] = versionNum[0];
            elettronicServicesUnprotected.hardwareRevision[1] = versionNum[1];
            elettronicServicesUnprotected.hardwareRevision[2] = versionNum[2];
            
            elettronicServicesUnprotected.softwareMainRevision = CB_SW_VersionCode.swVersion[0];
            elettronicServicesUnprotected.hardwareMainRevision = versionNum[0]<<3;
        }
    }
    return result;
}



//--------------------------------------------------------------------------------------------------
/*!
\brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
\author Heiko Kresse
\date   2005-04-18
\param  objectIndex    object index
\param  attributeIndex attribute index; attributeIndex<0 --> get whole object
\param  ptrValue  pointer for return-Value
\param  me subsystem index
\return error-code
<pre>
OK                      operation was successful
DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
ILLEGAL_OBJ_IDX         DEBUG, unknown Object
ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
</pre>
\test
\n by: Heiko Kresse
\n
\n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
\n
\n intention: 1. complete instruction-coverage 100%
\n result module test: 2006-02-14 OK
\n result Lint Level 3: free // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_ELETTRONIC_SERVICES(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{  //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    TUSIGN16 result=OK;
    
    VIP_ASSERT(me);
    // VIP_ASSERT(me==ELETTRONIC_SERVICES_ME);  //edited by Hao.wu 2012-09-14, change from:VIP_ASSERT(me) ,still need be checked by adminstrator
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);
    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    //lint -e{613} Ashwin Herur R 2009-07-14 Ok
    VIP_ASSERT(*(me->ptrState)>=INITIALIZED);
    //lint -e{613} Ashwin Herur R 2009-07-14 Ok
    if( objectIndex>me->maxIdx )
    {       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        result = ILLEGAL_OBJ_IDX;
    }
    else
    {     
        if(objectIndex == (TUSIGN16)ELETTRONICSERVICES_IDX_lowLevelHmiHwSwRevision)
        {
            TUSIGN8 hmiSwRevision;
            TUSIGN8 hmiHwRevision;
            
            result = hmi_Get((TUSIGN16)HMI_IDX_lowLevelSoftwareRevision, (TINT16)WHOLE_OBJECT, &hmiSwRevision);
            
            hmiHwRevision = hmi.HMI_Get_LLHardwareType_SRV(0);
            if(result == (TUSIGN16)OK)
            {
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[7] = (hmiSwRevision%10)+48;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[6] = 48;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[5] = 46;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[4] = (TUSIGN8)(hmiSwRevision/10)+48;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[3] = 48;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[2] = 46;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[1] = (hmiHwRevision%10)+48;
                elettronicServicesUnprotected.lowLevelHmiHwSwRevision[0] = (TUSIGN8)(hmiHwRevision/10)+48;
            }
            
            if(attributeIndex == (TINT16)WHOLE_OBJECT)
            {
                (void) memcpy(ptrValue,elettronicServicesUnprotected.lowLevelHmiHwSwRevision,sizeof(elettronicServicesUnprotected.lowLevelHmiHwSwRevision));
            }
            else
            {
                *(TUSIGN8*)ptrValue = elettronicServicesUnprotected.lowLevelHmiHwSwRevision[attributeIndex];
            }
        }
        
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
        else if(objectIndex == (TUSIGN16)ELETTRONICSERVICES_IDX_dipSwitchAndPushButtonsStatus)
        {
            TUSIGN16 status = 0x00;
            
        #ifndef LOCALSERVER_SELECT
#ifdef PUSH_BUTTON
            if(PUSH_BUTTON_ZERO_IS_ON) status |= ELETTRONICSERVICES_PUSH_BUTTONS_ZERO_STATUS;
            if(PUSH_BUTTON_SPAN_IS_ON) status |= ELETTRONICSERVICES_PUSH_BUTTONS_SPAN_STATUS;
#endif
            if(PUSH_BUTTON_WRITE_PROTECT_IS_ON) status |= ELETTRONICSERVICES_PUSH_BUTTONS_WRITE_PROTECT_STATUS;
            if(DIP_SWITCH_1_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_1_STATUS;
            if(DIP_SWITCH_2_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_2_STATUS;
            if(DIP_SWITCH_3_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_3_STATUS;
            if(DIP_SWITCH_4_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_4_STATUS;
            if(DIP_SWITCH_5_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_5_STATUS;
            if(DIP_SWITCH_6_IS_ON) status |= ELETTRONICSERVICES_DIP_SWITCH_6_STATUS;
        #endif

            (void)memcpy(ptrValue,&status,sizeof(status));
        }
        else if(objectIndex == (TUSIGN16)ELETTRONICSERVICES_IDX_cbNvWriteIsActive)
        {
            result = Get_T_UNIT(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue);
            
            if(CheckWriteActive() && (result == (TUSIGN16)OK))
            { 
                TUSIGN8 nvCheck = ELETTRONICSERVICES_WRITE_ACTIVE;
                (void)memcpy(ptrValue,&nvCheck,sizeof(nvCheck));
            }
            
        }
        else if(objectIndex == (TUSIGN16)ELETTRONICSERVICES_IDX_feNvWriteIsActive)
        {
            result = Get_T_UNIT(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue);
            if(CheckWriteActive_REPLACE() && (result == (TUSIGN16)OK))
            { 
                TUSIGN8 nvCheck = ELETTRONICSERVICES_WRITE_ACTIVE;
                (void)memcpy(ptrValue,&nvCheck,sizeof(nvCheck));
            }
        }		
#if defined(HART_COMMUNICATION) // In HART version, should read the object: deviceIdNumber (HART subsystem)  2013-02-05
        else if (objectIndex == (TUSIGN16)ELETTRONICSERVICES_IDX_cbSerialNumber)
        {		 
            result = hart_Get((TUSIGN16)HART_IDX_deviceIdNumber,attributeIndex,ptrValue); 
        }
#endif
        else
        {       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
            result = Get_T_UNIT(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue);
        }
    }
    
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
\brief  put the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
\author Heiko Kresse
\date   2004-04-18
\param  objectIndex    object index
\param  attributeIndex attribute index; <0 --> put whole object
\param  ptrValue  pointer to attribute-value and return-Value
\param me subsystem index
\return error-code
<pre>
OK                      operation was successful
LESSTHAN_RANGEMIN       at least one value is less than its minimum value
GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
SPAN_TO_SMALL           then span between two values is to small
NOT_ON_GRID             at least one value is not a multiple of its increment
RULE_VIOLATION          this shows a violation of one or more businessrules
DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
ILLEGAL_OBJ_IDX         DEBUG, unknown Object
ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
</pre>
\test
\n by: Heiko Kresse
\n by: Heiko Kresse
\n
\n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
\n
\n intention: 1. complete instruction-coverage 96.72%
\n result module test: 2006-02-14 OK
\n result Lint Level 3: free // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_ELETTRONIC_SERVICES(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{       //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
    TUSIGN16 result=OK;
    TUSIGN16 tmp[2]; 
    TUSIGN8 flag = 0;
     TFLOAT fixedOutputValue = 20.0f;
	VIP_ASSERT(me);
	VIP_ASSERT(ptrValue);
	VIP_ASSERT(me->ptrObjectList);
	//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
	//lint -e{613} Ashwin Herur R 2009-07-14 Ok
	VIP_ASSERT(*(me->ptrState)>=INITIALIZED);
	//lint -e{613} Ashwin Herur R 2009-07-14 Ok
	if( objectIndex>me->maxIdx )
	{
		//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
		result = ILLEGAL_OBJ_IDX;
	}
	else
	{
		//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
		result = Put_T_UNIT(ELETTRONIC_SERVICES_ME,objectIndex,attributeIndex,ptrValue);
		//lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
		if(result == OK)
		{
			switch(objectIndex)
			{

            case  ELETTRONICSERVICES_IDX_vSupplyReadBack:
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                result = Put_T_UNIT(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_IDX_vSupplyReadBackDuplicated,attributeIndex,ptrValue);
                break;
            case  ELETTRONICSERVICES_IDX_localDiagnosis:
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                result = Put_T_UNIT(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_IDX_localDiagnosisDuplicated,attributeIndex,ptrValue);
                break;
            case ELETTRONICSERVICES_IDX_elettronicTemperature:
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                result = Put_T_UNIT(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_IDX_elettronicTemperatureDuplicated,attributeIndex,ptrValue);
                break;                            
            case ELETTRONICSERVICES_IDX_resetToDefault:
                {       
                    //lint -e{613} Ashwin Herur R 2009-07-14 Ok
                    TINT16 subIdx = *(TINT16*)ptrValue;
                    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_CALCULATE;
                    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));

                    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                    if((subIdx == WHOLE_OBJECT) || ((subIdx > WHOLE_OBJECT) && (subIdx < MEMBER_IDX_NUM_ELEMENTS)))
                    {
                        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                        esResetToDefault = eTRUE;
                    }
                    else
                    {
                        return RULE_VIOLATION_ERR;
                    }
                    break;
                }

                case ELETTRONICSERVICES_IDX_classLoadRomDefault:
                {
                    TINT16 subIdx;
                    TINT16 classIdx = *(TINT16*)ptrValue;

                    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_CALCULATE;
                    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                    result = elettronicServices_Get(ELETTRONICSERVICES_IDX_loadRomDefault, WHOLE_OBJECT, &subIdx);

                    if((subIdx == WHOLE_OBJECT) || ((subIdx > WHOLE_OBJECT) && (subIdx < MEMBER_IDX_NUM_ELEMENTS)))
                    {
                        if(subIdx != WHOLE_OBJECT)
                        {
                            if(classIdx != WHOLE_OBJECT)
                            {
                                //lint -e{732} hao wu,  2013-01-15 Ok
                                const T_UNIT SLOW* unitPtr = GetSubsystemPtr(subIdx);
                                //lint -e{737} hao wu,  2013-01-15 loss of sign in promotion from int to unsigned int is Ok
                                //lint -e{574} hao wu,  2013-01-15 Ok
                                if((classIdx < WHOLE_OBJECT) || (classIdx >= unitPtr->numOfDataClasses))
                                {
                                    return RULE_VIOLATION_ERR;
                                }
                                else
                                {
                                }
                            }
                        }

                        //lint -e{641} Ashwin Herur R 2009-07-14 Converting TBOOL to int accepted
                        esLoadRomDefault = eTRUE;
                    }
                    else
                    {
                        return RULE_VIOLATION_ERR;
                    }

                    break;
                }

                case ELETTRONICSERVICES_IDX_saveAsDefault:
                    //lint -e{613} Ashwin Herur R 2009-07-14 Ok
                    result = SaveAsDefault_ElectronicServices(*(TINT16*)ptrValue);
                    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_CALCULATE;
                    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    break;
                    // add CRC check for rare change ram data
                    // can add the other objects(device defined here)
                case ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj:
                case ELETTRONICSERVICES_IDX_electronicServicesAlarmSimulation:
                case ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation:
                case ELETTRONICSERVICES_IDX_initializeNv:
                case ELETTRONICSERVICES_IDX_protectedDataStatus:
                    ENABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    elettronicServicesProtected.protectedDataStatus = ELETTRONICSERVICES_CALCULATE;
                    DISABLE_DATA_PROTECTION(*(elettronicServices.unit.ptrDataSemaphore));
                    break;


#if defined(HART_COMMUNICATION) // in hart version we should change the object.
            case ELETTRONICSERVICES_IDX_cbSerialNumber:  //remap to object: deviceIdNumber (HART subsystem), as the two object store the same information.  2013-02-05
                result = hart_Put(HART_IDX_deviceIdNumber,attributeIndex,ptrValue);
                break;
#endif 				
            case ELETTRONICSERVICES_IDX_hwRevisionHmi_fe:			
                result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_WRITE_FE_HW_VERSION,ELETTRONICSERVICES_FE_HD_VERSION_LENGTH,comWrite_es[ELETTRONICSERVICES_WRITE_HW_VERSION_INDEX],NULL);
                break;
            case ELETTRONICSERVICES_IDX_snrThreshold:
                result |= (TUSIGN16)frontEndInterface.PutCommand(ELETTRONICSERVICES_WRITE_SNR_THRESHOLD,ELETTRONICSERVICES_SNR_THRESHOLD_LENGTH,comWrite_es[ELETTRONICSERVICES_WRITE_SNR_THRESHOLD_INDEX],NULL);
                break;	
            case ELETTRONICSERVICES_IDX_hardwareRevision:
               
                tmp[0] = *((TUSIGN16*)ptrValue);
                tmp[1] = *((TUSIGN16*)ptrValue+1);
                fixedOutputValue = 20.0f;
                (void)hart_Put((TUSIGN16)HART_IDX_fixedOutputValue,WHOLE_OBJECT, &fixedOutputValue); 
                result |=  Erase_Flashblock(ERS_BLOCK_VRESION);
                result |=  Write_Flashdata(tmp,BLOCK_A,2); 
                fixedOutputValue = 0.0f;
                (void)hart_Put((TUSIGN16)HART_IDX_fixedOutputValue,WHOLE_OBJECT, &fixedOutputValue); 
                
                break;
            case ELETTRONICSERVICES_IDX_tranSapErpNo:
                flag = 1 ;
                result |= Put_T_UNIT(ELETTRONIC_SERVICES_ME,ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag,0,&flag);
                break;

            case ELETTRONICSERVICES_IDX_switchBLE:
            case ELETTRONICSERVICES_IDX_modeLCDLight:
                SHMIConfigSendCount = 3;
                break;

            default:
                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
                result = OK;
                break;
            }
        }
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
\brief   Overload of LoadRomDefaults to update Calibration bit,
loopCurrentMode and a fixed current output.
\author  Paul
\date    2012-9-6
\param   me = pointer to current out subsystem
\param   dataClassIndex = index of the dataclass to be defaulted
\return  error code
OK                      operation was successful
LESSTHAN_RANGEMIN       at least one value is less than its minimum value
GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
SPAN_TO_SMALL           then span between two values is to small
NOT_ON_GRID             at least one value is not a multiple of its increment
RULE_VIOLATION          this shows a violation of one or more businessrules
DECLINE                 DEBUG, operation isn't permitted in the aktual unit-state
ILLEGAL_OBJ_IDX         DEBUG, unknown Object
\test
test-date: 2012-09-10
\n by: Klaus Pose
\n environment: 
\n intention: instruction coverage 100%
\n result module test: OK
\n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 LoadRomDefaults_ELETTRONIC_SERVICES(const T_UNIT SLOW *me, TINT16 dataClassIndex)
{
    TUSIGN16    result = (TUSIGN16)OK;
    
    VIP_ASSERT(me==ELETTRONIC_SERVICES_ME);     // if invalid me pointer exit
    
    VIP_ASSERT(me->ptrDataSemaphore);   // if no semaphore available => exit
    //lint -e{641} hao wu,  2013-01-15 Ok
    // All data class shall do load rom default except the calibration data;
    // the calibration data shall do load rom default by ES initializeNV function;
    
    if(dataClassIndex == ELETTRONICSERVICES_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT) 
    {
        //those are calibration data; shall not load rom default;
        result = (TUSIGN16)OK;
    }
    else if(ALL_DATACLASSES == dataClassIndex)
    {
        //  load rom default data of es subsystem
        // shall not include calibration data class here;
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_ReplaceStaticFrequentNoDefault);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_Unprotected);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_Protected);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_DynamicDuplicated);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_ReplaceStaticRare);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_CONSTANT);
        result |= LoadRomDefaults_T_UNIT(me, ELETTRONICSERVICES_DC_IDX_STATIC_RARE);
    }
    else
    {
        result = LoadRomDefaults_T_UNIT(me, dataClassIndex);
    }
    return result;
}