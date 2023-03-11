//----------------------------------------------------------------------------------------------------------
/*
Copyright      Copyright ABB, 2010.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Burst Message Configuration Objects
 Module         T_DATA_OBJ_BURSTMSG_CONFVALUES
 Description    Definition of class T_DATA_OBJ::BURSTMSG_CONFVALUES
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before t_data_obj_burstmsg_confvalues.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before t_data_obj_burstmsg_confvalues.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must be included before t_data_obj_burstmsg_confvalues.h 
#endif

#ifndef __CMDDEF_H__
  #error cmddef.h must be included before t_data_obj_burstmsg_confvalues.h 
#endif

#ifdef __T_DATA_OBJ_BURSTMSG_CONFVALUES_H__
  #error t_data_obj_burstmsg_confvalues.h included more than once
#endif

#define __T_DATA_OBJ_BURSTMSG_CONFVALUES_H__

//--------------------------------------------------------------------------------------------------

#define BURSTMSG_CONFVALUES_burstMsgNumber  0
#define BURSTMSG_CONFVALUES_modeControlCode 1                        
#define BURSTMSG_CONFVALUES_cmdExpansionFlag 2
#define BURSTMSG_CONFVALUES_commandNumber 3
#define BURSTMSG_CONFVALUES_burstSlot0 4
#define BURSTMSG_CONFVALUES_burstSlot1 5
#define BURSTMSG_CONFVALUES_burstSlot2 6
#define BURSTMSG_CONFVALUES_burstSlot3 7
#define BURSTMSG_CONFVALUES_burstSlot4 8
#define BURSTMSG_CONFVALUES_burstSlot5 9
#define BURSTMSG_CONFVALUES_burstSlot6 10
#define BURSTMSG_CONFVALUES_burstSlot7 11
#define BURSTMSG_CONFVALUES_updatePeriod 12
#define BURSTMSG_CONFVALUES_maximumUpdatePeriod 13
#define BURSTMSG_CONFVALUES_triggerModeSelectionCode 14
#define BURSTMSG_CONFVALUES_triggerDevVariableClassification 15
#define BURSTMSG_CONFVALUES_triggerUnitsCode 16
#define BURSTMSG_CONFVALUES_triggerValue 17
#define BURSTMSG_CONFVALUES_processValueClassification 18
#define BURSTMSG_CONFVALUES_processValueUnitsCode 19
#define BURSTMSG_CONFVALUES_processValueSubSysId 20
#define BURSTMSG_CONFVALUES_processValueObjId 21

typedef struct _T_BURSTMSG_CONFVALUES
{
    TUSIGN8     burstMsgNumber;
    TUSIGN8     modeControlCode;
    TUSIGN8     cmdExpansionFlag;
    TUSIGN16    commandNumber;
    TUSIGN8     burstSlot0;
    TUSIGN8     burstSlot1;
    TUSIGN8     burstSlot2;
    TUSIGN8     burstSlot3;
    TUSIGN8     burstSlot4;
    TUSIGN8     burstSlot5;
    TUSIGN8     burstSlot6;
    TUSIGN8     burstSlot7;
    TUSIGN32    updatePeriod;
    TUSIGN32    maximumUpdatePeriod;
    TUSIGN8     triggerModeSelectionCode;
    TUSIGN8     triggerDevVariableClassification;
    TUSIGN8     triggerUnitsCode;
    TFLOAT      triggerValue;
    TUSIGN8     processValueClassification;
    TUSIGN8     processValueUnitsCode;
    TUSIGN8     processValueSubSysId;
    TUSIGN8     processValueObjId;
} T_BURSTMSG_CONFVALUES;

//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::BURSTMSG_CONFVALUES
extern const T_DATA_CLASS cgClassInitializer_BURSTMSG_CONFVALUES;


//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::BURSTMSG_CONFVALUES
 \author Bernhard Tohermes
 \date   2010-02-05
 \param  v ptrValue; the address will be casted to (void FAST*)
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_BURSTMSG_CONFVALUES(v) {(void FAST*)(v),22,sizeof(T_BURSTMSG_CONFVALUES),NON_VOLATILE,STRUCT,0,&cgClassInitializer_BURSTMSG_CONFVALUES}

