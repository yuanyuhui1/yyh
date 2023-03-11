//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Class definition T_UNIT::FRONT_END_INTERFACE

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before FrontEndInterface.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before FrontEndInterface.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before FrontEndInterface.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __FRONT_END_INTERFACE_H__
  #error FrontEndInterface.h included more than once
#endif

#define __FRONT_END_INTERFACE_H__

#define FRONT_END_INTERFACE_VERSION 0x010000   // Version 1.0.0

#define MAX_Q_SIZE 10

//-------------------------------------------------------------------------------------------------
//!  define macros, enums
//@SubGen start@  <TYPES>
  // _FE_DIAGNOSIS_ALARM_TABLE
  #define FE_BOARD_COMM_ERROR 0 //

  // 
  #define FE_FE_DIAG_ALARM_SIZE 1 //
//@SubGen end@ <TYPES>


#define FE_DIAG_SIZE  1
#define FE_DIAG_WORD_SIZE  8

// _FE_LOCAL DIAGNOSIS_ALARM_TABLE
#define FE_LOCAL_BOARD_COMM_ERROR  0x0001 //

extern const SLOW TUSIGN8 _FE_DIAGNOSIS_ALARM_TABLE[];

 
typedef struct
{
  	TUSIGN8 cmd;
  	TUSIGN8 datalen;
  	void (*procData)(TUSIGN8 length,TUSIGN8* const pDataBuf);
	void (*setFlag)(TUSIGN8 flag);
	TUSIGN16 crc;
}CMD_MSG;

typedef struct
{
	TUSIGN8 putIndex;
	TUSIGN8 purgeIndex;
	TUSIGN8 size;
	CMD_MSG memBuf[MAX_Q_SIZE];
}Buffer_STRUCT;
  
#define UART_BUFFER_BUSY        1
#define UART_BUFFER_EMPTY       0
 
#define GENERAL_FRAME_HEADER_NUM 4
  
// Definition Command CODE FOR READ
#define READ_DATA_CMD       					0X00
#define READ_CMD_END			        		0X7F
    
    
// Definition Command CODE FOR Write
#define WRITE_DATA_CMD         		    	0X80
#define WRITE_CMD_END						    0XFF
  
  //-------------------------------------------------------------------------------------------------
  //!  define the datatypes for EXE and SRV-Methods input and output
  
#if( FE_DIAG_WORD_SIZE == 8 )
    #define FE_DIAG_BIT_(index)   ((index) &  7)
    #define FE_DIAG_WRD_(index)   ((index) >> 3)
        // words = (AI_DIAG_SIZE) >> 3
#elif( FE_DIAG_WORD_SIZE == 16 )
    #define FE_DIAG_BIT_(index)   ((index) & 15)
    #define FE_DIAG_WRD_(index)   ((index) >> 4)
        // words = (AI_DIAG_SIZE) >> 4
#elif( FE_DIAG_WORD_SIZE == 32 )
    #define FE_DIAG_BIT_(index)   ((index) & 31)
    #define FE_DIAG_WRD_(index)   ((index) >> 5)
        // words = (AI_DIAG_SIZE) >> 5
#else
    #error invalid definition for FE_DIAG_WORD_SIZE
#endif
    
#if(FE_DIAG_SIZE > FE_DIAG_WORD_SIZE)
  #define FE_SETDIAG_(index,var)           (var[FE_DIAG_WRD_(index)] |=  (1U << FE_DIAG_BIT_(index)))
  #define FE_CLRDIAG_(index,var)           (var[FE_DIAG_WRD_(index)] &= ~(1U << FE_DIAG_BIT_(index)))
  #define FE_GETDIAG_(index,var)    (TBOOL)(var[FE_DIAG_WRD_(index)] &   (1U << FE_DIAG_BIT_(index)))
#else
  #define FE_SETDIAG_(index,var)           (var |=  (1U << FE_DIAG_BIT_(index)))
  #define FE_CLRDIAG_(index,var)           (var &= ~(1U << FE_DIAG_BIT_(index)))
  #define FE_GETDIAG_(index,var)    (TBOOL)(var &   (1U << FE_DIAG_BIT_(index)))
#endif

//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_FRONT_END_INTERFACE_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    FRONTENDINTERFACE_DC_IDX_DynamicDuplicated,
    FRONTENDINTERFACE_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_FRONT_END_INTERFACE
typedef struct _T_FRONT_END_INTERFACE
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* UpdateDiagnosis)(void);
    void (* SetDiagnosis)(TUSIGN16 attributeIdx);
    void (* ClearDiagnosis)(TUSIGN16 attributeIdx);
    void (* InterpreterCmd)(const CMD_MSG* pMessage);
    void (* ProcessComData)(const CMD_MSG* pMessage);
    void (* ReadComCmd)(const CMD_MSG* pMessage);
    void (* WriteComCmd)(const CMD_MSG* pMessage);
    void (* ProcessCmd)(void);
    TINT16 (* PutCommand)(TUSIGN8 cmdnum, TUSIGN8 datalen, void (*pFunc)(TUSIGN8 length,TUSIGN8* const pDataBuf), void (*pFuncFlag)(TUSIGN8 flag));
    void (* SendCmd)(const CMD_MSG* pMessage);
    void (* UpdateDiagData)(TUSIGN16 setDiag, TUSIGN16 clearDiag);
    TBOOL (* CheckFEreadySRV)(void);
    //@SubGen end@    <METHODS>
} T_FRONT_END_INTERFACE;


//-------------------------------------------------------------------------------------------------
//! define the T_FRONT_END_INTERFACE-object
extern SLOW const T_FRONT_END_INTERFACE frontEndInterface;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_FRONT_END_INTERFACE
#define FRONT_END_INTERFACE_ME (&frontEndInterface.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define frontEndInterface_Get(objectIndex,attributeIndex,ptrValue) \
   frontEndInterface.unit.Get(FRONT_END_INTERFACE_ME,objectIndex,attributeIndex,ptrValue)
#define frontEndInterface_Put(objectIndex,attributeIndex,ptrValue) \
   frontEndInterface.unit.Put(FRONT_END_INTERFACE_ME,objectIndex,attributeIndex,ptrValue)
#define frontEndInterface_Check(objectIndex,attributeIndex,ptrValue) \
   frontEndInterface.unit.Check(FRONT_END_INTERFACE_ME,objectIndex,attributeIndex,ptrValue)

#define frontEndInterface_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   frontEndInterface.unit.GetAttributeDescription(FRONT_END_INTERFACE_ME,objectIndex,attributeIndex,ptrDescriptor)
#define frontEndInterface_GetObjectDescription(objectIndex,ptrDescriptor) \
   frontEndInterface.unit.GetObjectDescription(FRONT_END_INTERFACE_ME,objectIndex,ptrDescriptor)

