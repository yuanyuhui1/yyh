//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Components HART
 Module
 Description    Interface Layer 7

 Remarks

*/
//--------------------------------------------------------------------------------------------------




#ifndef __SP_LAYER7_H__
#define __SP_LAYER7_H__

//power control
//switch to control the LED, lcd backlight
#define SWITCH_BLU_LED                 0x80
#define SWITCH_GREEN_LED               0x40
#define SWITCH_RED_LED                 0x20
#define SWITCH_YELLOW_LED              0x10

#define SWITCH_BLE_COMM                0x08  
#define SWITCH_LCD                     0x04
#define SWITCH_LCD_LIGHT               0x02  
#define SWITCH_LCD_LIGHT_CONTROL       0x01  

// Definition of extension command number
#define SP_EXTENSION_COMMAND_NUMBER    31

// Definition of not used slot code
#define SP_SLOT_CODE_NONE  251

// Definition of LAYER 2 request codes
#define SP_NO_LAYER2_REQ   0x00        // No Layer 2 request of Layer 7
#define SP_FIRST_LAYER2    0x01        // First Layer 2 requests Layer 7
#define SP_SECOND_LAYER2   0x02        // Second Layer 2 requests Layer 7

#define SP_SPECIAL_RW_ON            1
#define SP_SPECIAL_RW_OFF           2
#define SP_SPECIAL_RW               SP_SPECIAL_RW_OFF

#define SP_HMI_PRESENCE_PINSTATUS_DISCONNECT    GPIO_PIN_RESET
#define SP_HMI_PRESENCE_PINSTATUS_CONNECT       GPIO_PIN_SET

#define SP_HMI_PRESENCE_BOUNCECOUNT_INIT        0
#define SP_HMI_PRESENCE_BOUNCECOUNT_REDETECT    3

/*
#define PORT_FOR_HMI                0
#define PORT_FOR_SP                 1
#define PORT_UN_KNOWN               2
*/
/// Typedefs--------------------------------------
typedef TUSIGN16 (*PFUNC)(void);

// Structure for command table.
typedef struct 
{
  TUSIGN8   cmdTyp;
  TUSIGN8   respLen;
  TUSIGN8   reqLen;
  PFUNC     funcPtr;
}T_CMD_TBL;
//------------------------------------------------------

typedef struct {
    TUSIGN8 prevPinState;
    TUSIGN8 currPinState;
    TBOOL firstTimeCheck;
    TUSIGN8 bounceCount;
}HMI_PRESENCE_PIN;

extern HMI_PRESENCE_PIN hmiPresencePin;
extern TUSIGN8 s_addr;
extern TUSIGN8 s_addrLong[5];
extern TUSIGN16 SHMIConfigSendCount;

void updateHMIpresencePinStatus_LAYER7(void);
void ServPortCmdInterpreter_LAYER7(void);

#endif
