//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         UHTE
 Module         localinterface_uart.h
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
    #error common_type.h must be include before
#endif

//............................................
//!  extend include list if neccessary

#ifdef __LOCALINTERFACE_UART_H__
    #error localinterface_uart.h included more than once
#endif

#define __LOCALINTERFACE_UART_H__

//--------------------------------------------------------------------------------------------------
#define SERVICEPORT_UART_BAUD_HMI        9600
#define SERVICEPORT_UART_BAUD_SP         19200

#define SERVICEPORT_UART_PARITY_NONE     UART_PARITY_NONE
#define SERVICEPORT_UART_PARITY_ODD      UART_PARITY_ODD
#define SERVICEPORT_UART_PARITY_EVEN     UART_PARITY_EVEN

#define SERVICEPORT_UART_STOPBITS_1      UART_STOPBITS_1
#define SERVICEPORT_UART_STOPBITS_2      UART_STOPBITS_2

#define SERVICEPORT_UART_WORDLENGTH_7B   UART_WORDLENGTH_7B
#define SERVICEPORT_UART_WORDLENGTH_8B   UART_WORDLENGTH_8B
#define SERVICEPORT_UART_WORDLENGTH_9B   UART_WORDLENGTH_9B
//--------------------------------------------------------------------------------------------------

typedef struct
{
  TUSIGN32  baud;
  TUSIGN32  parity;
  TUSIGN32  stop;
  TUSIGN32  length;
  void      (*ReceiveIrqService)(void);
  void      (*TransmitIrqService)(void);
} T_SERVICEPORT_UART_PARAM;


//--------------------------------------------------------------------------------------------------
//  declaration of prototypes
void InitializeUart_SERVICEPORT(const T_SERVICEPORT_UART_PARAM SLOW *pParam);