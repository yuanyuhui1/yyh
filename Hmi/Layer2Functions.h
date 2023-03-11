//+@ HMIGen Layer2_c_include_microDefinition @+
//include the appropriate microprocessor I/O definitions header file here
#include "System/Interface/microcontroller.h"
//-@ HMIGen Layer2_c_include_microDefinition @-

//+@ HMIGen Layer2_TransmitByte @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
void Layer2_TransmitByte(TUSIGN8 txByte, TBOOL startTransmitter)
{   // Transmit byte and start transmitter if requested
//2010.11.18 zuochen wang  
  
#if(HMI_UART==2)
    U2TBL = txByte;
    if (startTransmitter == eTRUE)
        U2C1 |= 0x01;
#else
    U1TBL = txByte;
    if (startTransmitter == eTRUE)
        U1C1 |= 0x01;
#endif
}
//-@ HMIGen Layer2_TransmitByte @-

//+@ HMIGen Layer2_ReceiveByte @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 Layer2_ReceiveByte(void)
{   // return byte from receive register
//change . 2010.11.17 zuochen wang
#if(HMI_UART==2)
    return U2RBL; 
#else
    return U1RBL;  
#endif 
}
//-@ HMIGen Layer2_ReceiveByte @-

