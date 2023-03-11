//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         t_data_obj_modbus_diag_history.c
 Description    implementation of t_data_obj_modbus_diag_history methods.
				t_data_obj_modbus_diag_history handles modbus alarm history.

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

#ifdef __T_DATA_OBJ_MODBUS_DIAG_HISTORY_H__
  #error t_data_obj_modbus_diag_history.h included more than once
#endif

#define __T_DATA_OBJ_MODBUS_DIAG_HISTORY_H__

//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::ADDR
//lint -e{793} by CNZUWAN AS code review is accetable.
extern const T_DATA_CLASS cgClassInitializer_MODBUS_DIAG_HISTORY;

#define MB_MAX_NO_DIAG_HIS     (48)      // Maximum number of history bits for ModBus

//#define MODBUS_DIAG_HIS_diagHisByte0Bit1
#define MODBUS_DIAG_HIS_diagHisByte0Bit0   (0)
#define MODBUS_DIAG_HIS_diagHisByte0Bit1   (1)
#define MODBUS_DIAG_HIS_diagHisByte0Bit2   (2)
#define MODBUS_DIAG_HIS_diagHisByte0Bit3   (3)
#define MODBUS_DIAG_HIS_diagHisByte0Bit4   (4)
#define MODBUS_DIAG_HIS_diagHisByte0Bit5   (5)
#define MODBUS_DIAG_HIS_diagHisByte0Bit6   (6)
#define MODBUS_DIAG_HIS_diagHisByte0Bit7   (7)

#define MODBUS_DIAG_HIS_diagHisByte1Bit0   (8)
#define MODBUS_DIAG_HIS_diagHisByte1Bit1   (9)
#define MODBUS_DIAG_HIS_diagHisByte1Bit2   (10)
#define MODBUS_DIAG_HIS_diagHisByte1Bit3   (11)
#define MODBUS_DIAG_HIS_diagHisByte1Bit4   (12)
#define MODBUS_DIAG_HIS_diagHisByte1Bit5   (13)
#define MODBUS_DIAG_HIS_diagHisByte1Bit6   (14)
#define MODBUS_DIAG_HIS_diagHisByte1Bit7   (15)

#define MODBUS_DIAG_HIS_diagHisByte2Bit0   (16)
#define MODBUS_DIAG_HIS_diagHisByte2Bit1   (17)
#define MODBUS_DIAG_HIS_diagHisByte2Bit2   (18)
#define MODBUS_DIAG_HIS_diagHisByte2Bit3   (19)
#define MODBUS_DIAG_HIS_diagHisByte2Bit4   (20)
#define MODBUS_DIAG_HIS_diagHisByte2Bit5   (21)
#define MODBUS_DIAG_HIS_diagHisByte2Bit6   (22)
#define MODBUS_DIAG_HIS_diagHisByte2Bit7   (23)

#define MODBUS_DIAG_HIS_diagHisByte3Bit0   (24)
#define MODBUS_DIAG_HIS_diagHisByte3Bit1   (25)
#define MODBUS_DIAG_HIS_diagHisByte3Bit2   (26)
#define MODBUS_DIAG_HIS_diagHisByte3Bit3   (27)
#define MODBUS_DIAG_HIS_diagHisByte3Bit4   (28)
#define MODBUS_DIAG_HIS_diagHisByte3Bit5   (29)
#define MODBUS_DIAG_HIS_diagHisByte3Bit6   (30)
#define MODBUS_DIAG_HIS_diagHisByte3Bit7   (31)

#define MODBUS_DIAG_HIS_diagHisByte4Bit0   (32)
#define MODBUS_DIAG_HIS_diagHisByte4Bit1   (33)
#define MODBUS_DIAG_HIS_diagHisByte4Bit2   (34)
#define MODBUS_DIAG_HIS_diagHisByte4Bit3   (35)
#define MODBUS_DIAG_HIS_diagHisByte4Bit4   (36)
#define MODBUS_DIAG_HIS_diagHisByte4Bit5   (37)
#define MODBUS_DIAG_HIS_diagHisByte4Bit6   (38)
#define MODBUS_DIAG_HIS_diagHisByte4Bit7   (39)

#define MODBUS_DIAG_HIS_diagHisByte5Bit0   (40)
#define MODBUS_DIAG_HIS_diagHisByte5Bit1   (41)
#define MODBUS_DIAG_HIS_diagHisByte5Bit2   (42)
#define MODBUS_DIAG_HIS_diagHisByte5Bit3   (43)
#define MODBUS_DIAG_HIS_diagHisByte5Bit4   (44)
#define MODBUS_DIAG_HIS_diagHisByte5Bit5   (45)
#define MODBUS_DIAG_HIS_diagHisByte5Bit6   (46)
#define MODBUS_DIAG_HIS_diagHisByte5Bit7   (47)

typedef struct _T_MODBUS_DIAG_HIS
{
    TUSIGN8 alarmBits[MB_MAX_NO_DIAG_HIS] ;
}T_MODBUS_DIAG_HIS ;





//--------------------------------------------------------------------------------------------------
/*!
 \brief  	constructor; Initializer for objects of type MODBUS DIAG HISTORY
 \author 	Shen Quayum
 \date   	2011-01-01
 \param  	x, reference for
 \param	y storageQualifier, defines whether the data type is RAM, ROM etc.
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_MODBUS_DIAG_HIS(x,y)\
        {(void FAST*)(x),   /*data object for RAM */\
        MB_MAX_NO_DIAG_HIS, /* Maximum number of attributes*/\
        sizeof(T_MODBUS_DIAG_HIS),  /*Object length*/\
        y, /*RAM, Storage Qualifier*/\
        ARRAY,  /*Data type  */\
        0,  /*Check method  */\
        &cgClassInitializer_MODBUS_DIAG_HISTORY /*class specific methods and attributes */\
        }


//#define TEST_T_DATA_MODBUS_DIAG_HISTORY

#ifdef TEST_T_DATA_MODBUS_DIAG_HISTORY
	void TestModBusDiagHistory();
#endif

