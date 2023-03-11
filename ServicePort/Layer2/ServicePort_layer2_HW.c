//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Service Port
 Module
 Description    implementation layer2 interface methods

 Remarks
                uses UART1.
              
*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Hmi/Interface/HMI.h"
#include "ServicePort_layer2.h"
#include "ServicePort/Layer1/serviceport_uart.h"
#include "ServicePort/Layer1/serviceport_dio.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Hmi/Source/Implementation.h"
#include "Hmi/Source/PacketManager.h"


//----------------Functions defined externally------------------------------------------------------


//---------------Externally declared variables------------------------------------------------------
extern T_SPLAYER2_FLAGS    SpLayer2flags;
TUSIGN8 isSmartHMIConnected = eFALSE;

//! Initializer for the UART at local HART-Mode
//! The baud rate will be changed at run time
static const T_SERVICEPORT_UART_PARAM spUartParam =
{
    SERVICEPORT_UART_BAUD_SP,          //TUSIGN16 baud; support only 19200
    SERVICEPORT_UART_PARITY_ODD,       //TUSIGN8  parity;
    SERVICEPORT_UART_STOPBITS_1,       //TUSIGN8  stop;
    SERVICEPORT_UART_WORDLENGTH_9B,    //TUSIGN8  length
    servicePortReceive,                //(*ReceiveIrqService)(void);
    servicePortTransmit,               //(*TransmitIrqService)(void);
};

//! Initializer for the UART at HMI-Mode
static const T_SERVICEPORT_UART_PARAM hmiUartParam =
{
    SERVICEPORT_UART_BAUD_HMI,             //TUSIGN16 baud;
    SERVICEPORT_UART_PARITY_NONE,          //TUSIGN8  parity;
    SERVICEPORT_UART_STOPBITS_1,           //TUSIGN8  stop;
    SERVICEPORT_UART_WORDLENGTH_8B,        //TUSIGN8  length;
    PacketManagerReceiveInterupt,          //(*ReceiveIrqService)(void);
    PacketManagerTransmitInterrupt,        //(*TransmitIrqService)(void);
};
static const T_SERVICEPORT_UART_PARAM smarthmiUartParam =
{
    SERVICEPORT_UART_BAUD_HMI,          //TUSIGN16 baud; support only 19200
    SERVICEPORT_UART_PARITY_NONE,       //TUSIGN8  parity;
    SERVICEPORT_UART_STOPBITS_1,       //TUSIGN8  stop;
    SERVICEPORT_UART_WORDLENGTH_8B,    //TUSIGN8  length
    servicePortReceive,                //(*ReceiveIrqService)(void);
    servicePortTransmit,               //(*TransmitIrqService)(void);
};

void HMI_Presence_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    HMI_PRESENCE_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = HMI_PRESENCE_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HMI_PRESENCE_GPIO_PORT, &GPIO_InitStruct);
}

void HMI_Power_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    HMI_POWER_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = HMI_POWER_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HMI_POWER_GPIO_PORT, &GPIO_InitStruct);
}

void HMI_Reset_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    HMI_RESET_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = HMI_RESET_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HMI_RESET_GPIO_PORT, &GPIO_InitStruct);
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief   This function activates the Power-Supply at local interface
 \author  Klaus Pose
 \date    2006-07-03
 \test
 test-date: 2006-11-30
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
void layer2_ApplyPower(void)
{
    if(!GetPowerDio_SERVICEPORT())
    {
        // activate power
        SetPowerDio_SERVICEPORT(1);
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Initializes the Local-Interface for the HMI
 \author  Klaus Pose
 \date    2006-07-03
 \test
 test-date: 2006-11-30, 2009-08-17
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
void InitializeHmi()
{
    //set the interupt vector first, otherwise the mReceiveIrqService is not initialized.
    TUSIGN8 statu_init = INIT_HARDWARE;
    statu_init |= INIT_DATA;
    statu_init |= INIT_CALCULATION;
    statu_init |= INIT_TASKS;
    
    HMI_Power_GPIO_Init();
    HMI_Reset_GPIO_Init();
    HMI_Presence_GPIO_Init();
    layer2_ApplyPower();
    SET_HMI_RESET_HIGH();
    (void)InitializeUart_SERVICEPORT(&hmiUartParam);
    hmi.HMI_DenyIOaccessSRV(HMI_LOCAL_INSTANCE);
    // Init Calcucation is required for an hmi hot plug out/in (ResetPacketManager variables)
    if(OK == (T_DATA_OBJ_RETURNCODES)hmi.unit.Initialize(HMI_ME,statu_init))
    {
        hmi.HMI_PowerOnSRV(HMI_LOCAL_INSTANCE/*TODO*/);
        hmi.HMI_AllowIOaccessSRV(HMI_LOCAL_INSTANCE/*TODO*/);
    }
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief   Initializes the Local-Interface for the HMI
 \author  Klaus Pose
 \date    2006-07-03
 \test
 test-date: 2006-11-30, 2009-08-17
 \n by: Klaus Pose
 \n environment: sight check
 \n intention: required actions done
 \n result module test: OK
 \n result Lint Level 3: OK
*/
//--------------------------------------------------------------------------------------------------
void InitializeHMI_LAYER2()
{
    layer2_ApplyPower();
    (void)InitializeUart_SERVICEPORT(&hmiUartParam);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize Layer 2 and Service Port-UART
 \author    Ganapathi
 \date      2008-01-21
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:    
 \n environment: 
 \n intention: 
 \n result  link layer test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitializeSp_LAYER2(void)
{
    // Onetime initialization of flags
    layer2_ApplyPower();
    SpLayer2flags.layer7requestedBit  = 0;
    SpLayer2flags.receiverActivBit = 1;
    InitializeUart_SERVICEPORT(&spUartParam);
    // Initialize service port receive state machine.
    InitServicePortRxStateMachine();
}

void InitializeSmartHmi_LAYER2(void)
{
    // Onetime initialization of flags
    layer2_ApplyPower();
    SpLayer2flags.layer7requestedBit  = 0;
    SpLayer2flags.receiverActivBit = 1;
    InitializeUart_SERVICEPORT(&smarthmiUartParam);
    // Initialize service port receive state machine.
    InitServicePortRxStateMachine();
}
