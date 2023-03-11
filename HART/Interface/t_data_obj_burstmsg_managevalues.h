//----------------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2010.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Burst Message Configuration Objects
 Module         T_DATA_OBJ_BURSTMSG_MANAGEVALUES
 Description    Definition of class T_DATA_OBJ::BURSTMSG_MANAGEVALUES
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before t_data_obj_burstmsg_managevalues.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before t_data_obj_burstmsg_managevalues.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must be included before t_data_obj_burstmsg_managevalues.h 
#endif

#ifndef __CMDDEF_H__
  #error cmddef.h must be included before t_data_obj_burstmsg_confvalues.h 
#endif

#ifdef __T_DATA_OBJ_BURSTMSG_MANAGEVALUES_H__
  #error t_data_obj_burstmsg_managevalues.h included more than once
#endif

#define __T_DATA_OBJ_BURSTMSG_MANAGEVALUES__

//--------------------------------------------------------------------------------------------------

#define BURSTMSG_MANAGEVALUES_updatePeriodCounter           0
#define BURSTMSG_MANAGEVALUES_maximumUpdatePeriodCounter    1
#define BURSTMSG_MANAGEVALUES_lastSentProcessValue          2
#define BURSTMSG_MANAGEVALUES_lastDiagnosisCounter          4



typedef struct _T_BURSTMSG_MANAGEVALUES
{
     TUSIGN32   updatePeriodCounter;
     TUSIGN32   maximumUpdatePeriodCounter;
     TFLOAT     lastSentProcessValue;
     TUSIGN16   lastDiagnosisCounter;
} T_BURSTMSG_MANAGEVALUES;

//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::BURSTMSG_MANAGEVALUES
extern const T_DATA_CLASS cgClassInitializer_BURSTMSG_MANAGEVALUES;


//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::BURSTMSG_MANAGEVALUES
 \author Bernhard Tohermes
 \date   2010-02-05
 \param  v ptrValue; the address will be casted to (void FAST*)
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_BURSTMSG_MANAGEVALUES(v) {(void FAST*)(v),4,sizeof(T_BURSTMSG_MANAGEVALUES),RAM,STRUCT,0,&cgClassInitializer_BURSTMSG_MANAGEVALUES}

