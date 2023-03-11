//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Rainbow
 Module         CHIP_HANDLER
 Description    segment table
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
  #error common_type.h must include before chip_handler_segtab.h
#endif

#ifdef __CHIP_HANDLER_SEGTAB_H__
  #error chip_handler_segtab.h included more than once
#endif

#define __CHIP_HANDLER_SEGTAB_H__

// flag definition
#define VARIABLE_MAINLOCATION_FLAG 0x01

#define CHIP_ADDRESS_EEPROM 0xA0    // 1010 A2 A1 A0 RW -> 1010 0000

// types needed for segment tabel
union  U_ADDR
{
    TUSIGN16 u16;
    TUSIGN8  u8[2];
};

struct S_SEGTAB
{
    TUSIGN8      chip;
    union U_ADDR addr;
};


extern const struct S_SEGTAB cmSegTab[CH_NUMOFPAGES][CH_SEGMENTS_PER_PAGE];




