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

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before I2C_Bus.h
#endif

#ifndef __SYSTEM_H__
  #error system.h must include before I2C_Bus.h
#endif

#ifdef __SPI_BUS_H__
  #error I2C_Bus.h is included more than once
#endif

#define __SPI_BUS_H__


//-------------------------------------------------------------------------------------------------
#define BR25G128_WREN           (0x06)   // 0000 0110, Write Enable Command
#define BR25G128_WRDI           (0x04)   // 0000 0100, Write Disable Command
#define BR25G128_READ           (0x03)   // 0000 0110, Read Command
#define BR25G128_WRITE          (0x02)   // 0000 0010, Write Command
#define BR25G128_RDSR           (0x05)   // 0000 0101, Read Status Register Command
#define BR25G128_WRSR           (0x01)   // 0000 0001, Write Status Register Command
#define BR25G128_MEM_SIZE       (0x3FFF) // 128Kbit - 16K x 8Bit
#define BR25G128_PAGE_MAX_SIZE  (64)     // 64 bytes
#define BR25G128_WRITE_TIMEOUT  (5)      // 5ms

typedef enum
{
	WR_DIS_SECTIONO_NONE = 0,   //  Write disable block: None			- BP[1:0] = 0b00
	WR_DIS_SECTIONO_1_3  = 1,   //  Write disable block: 3000h-3FFFh	- BP[1:0] = 0b01
	WR_DIS_SECTIONO_2_3  = 2,   //  Write disable block: 2000h-3FFFh	- BP[1:0] = 0b10
	WR_DIS_SECTIONO_ALL  = 3    //  Write disable block: 0000h-3FFFh	- BP[1:0] = 0b11
}BR25G128_Write_Disable_Clock;

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t READY_BUSY:  1;  //Write cycle status (READY / BUSY) status confirmation bit.  READY=0, BUSY=1
        uint8_t WEN:         1;  // Write and write status register write enable / disable status confirmation bit
        uint8_t BP0:         1;  // EEPROM write disable block designation bit
        uint8_t BP1:         1;  // EEPROM write disable block designation bit
        uint8_t ReservedBit: 3;
        uint8_t WPEN:        1;  // WPB pin enable / disable designation bit

    }bit;
}BR25G128_Status_Reg;

void EEPROM_SPI_Init(void);
void EEPROM_SPI_ReadData(const TUSIGN16 ReadAddr, TUSIGN8* pRxBuffer, TUSIGN16 byteCount);
void EEPROM_SPI_WriteData(const TUSIGN16 WriteAddr, TUSIGN8* pTxBuffer, TUSIGN16 byteCount);