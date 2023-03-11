//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         t_data_obj_modbus_diag.c
 Description    implementation of t_data_obj_modbus_diag methods.
				t_data_obj_modbus_diag handles modbus diagnostic alarm.

 Remarks

*/
//----------------------------------------------------------------------------------------------------------


#ifndef __SYSTEM_H__
  #error system.h must be included before t_data_obj.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before t_data_obj.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must include before t_data_obj_modbus_diag.h
#endif

#ifdef __T_DATA_OBJ_MODBUS_DIAG_H__
  #error t_data_obj_modbus_diag.h included more than once
#endif

#define __T_DATA_OBJ_MODBUS_DIAG_H__


#define MODBUS_DIAG_diagByte0Bit0   (0)
#define MODBUS_DIAG_diagByte0Bit1   (1)
#define MODBUS_DIAG_diagByte0Bit2   (2)
#define MODBUS_DIAG_diagByte0Bit3   (3)
#define MODBUS_DIAG_diagByte0Bit4   (4)
#define MODBUS_DIAG_diagByte0Bit5   (5)
#define MODBUS_DIAG_diagByte0Bit6   (6)
#define MODBUS_DIAG_diagByte0Bit7   (7)

#define MODBUS_DIAG_diagByte1Bit0   (8)
#define MODBUS_DIAG_diagByte1Bit1   (9)
#define MODBUS_DIAG_diagByte1Bit2   (10)
#define MODBUS_DIAG_diagByte1Bit3   (11)
#define MODBUS_DIAG_diagByte1Bit4   (12)
#define MODBUS_DIAG_diagByte1Bit5   (13)
#define MODBUS_DIAG_diagByte1Bit6   (14)
#define MODBUS_DIAG_diagByte1Bit7   (15)

#define MODBUS_DIAG_diagByte2Bit0   (16)
#define MODBUS_DIAG_diagByte2Bit1   (17)
#define MODBUS_DIAG_diagByte2Bit2   (18)
#define MODBUS_DIAG_diagByte2Bit3   (19)
#define MODBUS_DIAG_diagByte2Bit4   (20)
#define MODBUS_DIAG_diagByte2Bit5   (21)
#define MODBUS_DIAG_diagByte2Bit6   (22)
#define MODBUS_DIAG_diagByte2Bit7   (23)

#define MODBUS_DIAG_diagByte3Bit0   (24)
#define MODBUS_DIAG_diagByte3Bit1   (25)
#define MODBUS_DIAG_diagByte3Bit2   (26)
#define MODBUS_DIAG_diagByte3Bit3   (27)
#define MODBUS_DIAG_diagByte3Bit4   (28)
#define MODBUS_DIAG_diagByte3Bit5   (29)
#define MODBUS_DIAG_diagByte3Bit6   (30)
#define MODBUS_DIAG_diagByte3Bit7   (31)

#define MODBUS_DIAG_diagByte4Bit0   (32)
#define MODBUS_DIAG_diagByte4Bit1   (33)
#define MODBUS_DIAG_diagByte4Bit2   (34)
#define MODBUS_DIAG_diagByte4Bit3   (35)
#define MODBUS_DIAG_diagByte4Bit4   (36)
#define MODBUS_DIAG_diagByte4Bit5   (37)
#define MODBUS_DIAG_diagByte4Bit6   (38)
#define MODBUS_DIAG_diagByte4Bit7   (39)

#define MODBUS_DIAG_diagByte5Bit0   (40)
#define MODBUS_DIAG_diagByte5Bit1   (41)
#define MODBUS_DIAG_diagByte5Bit2   (42)
#define MODBUS_DIAG_diagByte5Bit3   (43)
#define MODBUS_DIAG_diagByte5Bit4   (44)
#define MODBUS_DIAG_diagByte5Bit5   (45)
#define MODBUS_DIAG_diagByte5Bit6   (46)
#define MODBUS_DIAG_diagByte5Bit7   (47)


//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::ADDR
extern const T_DATA_CLASS cgClassInitializer_MODBUS_DIAG;

#define MB_MAX_NO_DIAG      (48)


typedef struct _T_MODBUS_DIAG
{
    TUSIGN8 alarmBits[MB_MAX_NO_DIAG] ;
}T_MODBUS_DIAG ;





//--------------------------------------------------------------------------------------------------
/*!
 \brief  	constructor; Initializer for objects of type MODBUS DIAG
 \author 	Shen Quayum
 \date   	2011-01-01
 \param  	x, reference for
 \param	y storageQualifier, defines whether the data type is RAM, ROM etc.
*/
//--------------------------------------------------------------------------------------------------


#define CONSTRUCT_MODBUS_DIAG(x,y)\
        {(void FAST*)(x),       /*data object for RAM */\
        MB_MAX_NO_DIAG,/* Maximum number of attributes*/\
        sizeof(T_MODBUS_DIAG),/*Object length*/\
        y,/*RAM, Storage Qualifier*/\
        ARRAY,/*Data type  */\
        0,/*Check method  */\
        &cgClassInitializer_MODBUS_DIAG/*class specific methods and attributes */\
        }



//#define TEST_T_DATA_MODBUS_DIAG

#ifdef TEST_T_DATA_MODBUS_DIAG
    void TestModBusDiag(void);
#endif
