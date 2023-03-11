#ifndef _HMI_LAYER2_H
#define _HMI_LAYER2_H

#ifndef _IMPLEMENTATIONDEFINITIONS
#error implementation.h must be included
#endif

void    Layer2_TransmitByte(TUSIGN8 txByte, TBOOL startTransmitter);
TUSIGN8 Layer2_ReceiveByte(void);

#endif
