//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Declares all defines that are specific to data_handling.c.
 						Forward function declarations also here

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#ifndef		_DATA_HANDLING_H
#define		_DATA_HANDLING_H

TUSIGN8 PacketErrorCheck(void);
void WriteToDisplay(void);
void CommsErrorCheck(void);
TUSIGN8 CheckForPacketToProcess(void);
void RomCheck(void);
void TransmitReply(void);

#ifndef TYPE_1_IMPLEMENTATION
void LowLevelDriver_Receive(TUSIGN8 receivedByte);
void LowLevelDriverExe(void);
TUSIGN8 LowLevelDriver_Reply(void);
#endif

#endif 	// _DATA_HANDLING_H
