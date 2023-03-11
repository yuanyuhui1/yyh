//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2022.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Framework
 Module         Electronic Services
 Description    prototypes of functions for LP3907
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __LP3907_H__
#define __LP3907_H__


/* ------------------ LP3907 Control Registers ------------------  */
/*
REGISTER ADDRESS    REGISTER NAME   READ/WRITE          REGISTER DESCRIPTION
0x02                ICRA                R               Interrupt Status Register A
0x07                SCR1                R/W             System Control 1 Register
0x10                BKLDOEN             R/W             Buck and LDO Output Voltage Enable Register
0x11                BKLDOSR             R               Buck and LDO Output Voltage Status Register
0x20                VCCR                R/W             Voltage Change Control Register 1
0x23                B1TV1               R/W             Buck1 Target Voltage 1 Register
0x24                B1TV2               R/W             Buck1 Target Voltage 2 Register
0x25                B1RC                R/W             Buck1 Ramp Control
0x29                B2TV1               R/W             Buck2 Target Voltage 1 Register
0x2A                B2TV2               R/W             Buck2 Target Voltage 2 Register
0x2B                B2RC                R/W             Buck2 Ramp Control
0x38                BFCR                R/W             Buck Function Register
0x39                LDO1VCR             R/W             LDO1 Voltage Control Registers
0x3A                LDO2VCR             R/W             LDO2 Voltage Control Registers
*/

/* ------ PMIC LP3907 Register address definition ------------------*/
#define ICRA_ADDRESS                0x02
#define SCR1_ADDRESS                0x07
#define BKLDOEN_ADDRESS             0x10
#define BKLDOSR_ADDRESS             0x11
#define VCCR_ADDRESS                0x20
#define B1TV1_ADDRESS               0x23
#define B1TV2_ADDRESS               0x24
#define B1RC_ADDRESS                0x25
#define B2TV1_ADDRESS               0x29
#define B2TV2_ADDRESS               0x2A
#define B2RC_ADDRESS                0x2B
#define BFCR_ADDRESS                0x38
#define LDO1VCR_ADDRESS             0x39
#define LDO2VCR_ADDRESS             0x3A

/* ------ PMIC LP3907 LDO1/LDO2 output voltage definition ------------------*/
#define LDO_OUT_1V0                 0x00
#define LDO_OUT_1V1                 0x01
#define LDO_OUT_1V2                 0x02
#define LDO_OUT_1V3                 0x03
#define LDO_OUT_1V4                 0x04
#define LDO_OUT_1V5                 0x05
#define LDO_OUT_1V6                 0x06
#define LDO_OUT_1V7                 0x07
#define LDO_OUT_1V8                 0x08
#define LDO_OUT_1V9                 0x09
#define LDO_OUT_2V0                 0x0A
#define LDO_OUT_2V1                 0x0B
#define LDO_OUT_2V2                 0x0C
#define LDO_OUT_2V3                 0x0D
#define LDO_OUT_2V4                 0x0E
#define LDO_OUT_2V5                 0x0F
#define LDO_OUT_2V6                 0x10
#define LDO_OUT_2V7                 0x11
#define LDO_OUT_2V8                 0x12
#define LDO_OUT_2V9                 0x13
#define LDO_OUT_3V0                 0x14
#define LDO_OUT_3V1                 0x15
#define LDO_OUT_3V2                 0x16
#define LDO_OUT_3V3                 0x17
#define LDO_OUT_3V4                 0x18
#define LDO_OUT_3V5                 0x19

#define LP3907_CHIP_ADDRESS         0x60
#define LP3907_I2C_ADDRESS          (LP3907_CHIP_ADDRESS << 1)
#define LP3907_REGISTER_SIZE        1


void LP3907_Read_Reg(uint16_t regAddress, uint8_t *pRegValue);
void LP3907_Write_Reg(uint16_t regAddress, uint8_t *pRegValue);


#endif /* __LP3907_H__ */
