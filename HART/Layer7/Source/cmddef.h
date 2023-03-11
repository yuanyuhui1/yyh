//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Layer 7 - Command interpreter interface definition

 Remarks


*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before cmddef.h
#endif

#ifndef __OBJ_FUNCT_RB_H__
  #error ObjfunctRB.h must be include before cmddef.h
#endif

#ifdef __CMDDEF_H__
  #error cmddef.h included more than once
#endif

#define __CMDDEF_H__

// Code of Analog channel 0
#define ANALOG_CHANNEL_0    0


// invalid index of command table
#define INVALID_CMD_NUMBER_INDEX    0xffff


//#pragma constseg = SLOW HART_COMMAND_TABLE_DATA

// length of command table
extern const TUSIGN16   lengthOfCommandTable;

// Command table list
extern const COMMAND commands[];

// number of entries in table of not valid HART Revision 5 Commands
extern const TUSIGN8 rev5CmdTabLen;

// table of command 9 specific non valid device variable slot objects
extern const COM_OBJ_DESCR specObjCom9_nonValidDeviceVariableObjDescrTab[];


//added 20110509  for 
// table of command 33 specific non valid device variable slot objects
extern const COM_OBJ_DESCR specObjCom33_nonValidDeviceVariableObjDescrTab[];
// First response mapping table -> ERROR
extern const TUSIGN8 mapErrFirstResponse[];

// First response mapping table -> WARNING
extern const TUSIGN8 mapWarnFirstResponse[];

// Mapping of parameter length code
extern const TUSIGN8 mapParaLength[];

// Mapping of data type length code (in bytes)
extern const TUSIGN8 dataTypeLength[];

// Object description for T_RANGE_FLOAT_U
extern const T_TYPE_CONV t_range_float_u[];

// Object description for T_RANGE_FLOAT
extern const T_TYPE_CONV t_range_float[];


//#pragma constseg = default
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Determines the HART second response byte
 \author Stefan Tabelander, Horst Seele
 \date   2005-11-08
 \param  requestsLayer2: Flags which Layer 2 requests Layer 7.
 \return secRespByte:   Second response byte
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN8 HARTCheckSecRespByte (TUSIGN8 requestsLayer2, T_LAYER2_BUFFER* pTxBuffer);


//--------------------------------------------------------------------------------------------------
/*!
 \brief  HARTFindCmdTableIndex: Search command table index for command
 \author Horst Seele
 \date   2005-11-15
 \param  cmdNumber: command number.
 \return Index in command table or invalid index if not found.
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTFindCmdTableIndex (TUSIGN16 commandNumber);


