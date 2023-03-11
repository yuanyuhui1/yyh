//+@ HMIGen Layer2_c_include_microDefinition @+
//include the appropriate microprocessor I/O definitions header file here
#error microprocessor not defined
//-@ HMIGen Layer2_c_include_microDefinition @-

//+@ HMIGen Layer2_TransmitByte @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
void Layer2_TransmitByte(TUSIGN8 txByte, TBOOL startTransmitter)
{   // Transmit byte and start transmitter if requested
    #error Transmit byte function not complete
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
    #error Receive byte function not complete
}
//-@ HMIGen Layer2_ReceiveByte @-

