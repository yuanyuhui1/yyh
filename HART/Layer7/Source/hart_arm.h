//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Access right management

 Remarks


*/
//--------------------------------------------------------------------------------------------------


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif


#ifdef __HART_ARM_H__
  #error hart_arm.h included more than once
#endif

#define __HART_ARM_H__


#define HART_NOT_LOCKED   0
#define HART_LOCKED       1

#define ACCESS_READ       0
#define ACCESS_WRITE      1

#define HART_WRITE_PROTECTION 180
#define HART_FACTORY_PASSWD   128

#define HART_CMD_038    38
#define HART_CMD_039    39
#define HART_CMD_042    42
#define HART_CMD_048    48
#define HART_CMD_071    71
#define HART_CMD_109    109
#define HART_CMD_142    142
#define HART_CMD_147    147
#define HART_CMD_154    154
#define HART_CMD_155    155
#define HART_CMD_188    188
#define HART_CMD_217    217
#define HART_CMD_218    218
#define HART_CMD_242    242
#define HART_CMD_202    202
#define HART_CMD_209    209
#define HART_CMD_225    225
#define HART_CMD_243    243
#define HART_CMD_249    249
#define HART_CMD_185    185
//--------------------------------------------------------------------------------------------------
/*!
 \brief  check if HART can access parameter
 \author Stefan Tabelander, Horst Seele
 \date   2004-07-27, 2005-11-16
 \param
    <pre>
       cmdNumber:     Command number presently read or written to
       direction:     Either READ or WRITE
    <\pre>
 \return
    <pre>
       eTRUE,  if access accepted
       eFALSE, if access denied
    <\pre>
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 HARTCanAccess(TUSIGN16 cmdNumber, TUSIGN8 direction, TBOOL isPrimaryMasterActive);




