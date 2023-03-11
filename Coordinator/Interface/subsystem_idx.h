//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must include before subsystem_idx.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be include before subsystem_idx.h
#endif


#ifdef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h included more than once
#endif

#define __SUBSYSTEM_IDX_H__

//----------------------------------------------------------------------------------------------------------
//! subsystem ids
//
// !! append new subsystems or re-compile all developement tools !!
//
enum _MEMBER_IDX
{
    ARM_IDX,
    CDT_IDX,
    DIAG_IDX,
    ELETTRONICSERVICES_IDX,
    FE_IDX,
    HART_IDX,
    HMI_IDX,
    MAPPER_IDX,
    MVM_IDX,
    SERVICEPORT_IDX,
    AI_IDX,
    SVM_IDX,
    DIGITALOUTPUT_IDX,
    TOTALIZER_IDX,
    ETH_IDX,
    MODBUS_IDX,
    WEBSRV_IDX,
    MEMBER_IDX_NUM_ELEMENTS      // Number of subsytems available
};

typedef TUSIGN16  T_SUBSYSTEM_IDX;

//----------------------------------------------------------------------------------------------------------
// function header to be added!
extern const T_UNIT SLOW* GetSubsystemPtr(T_SUBSYSTEM_IDX subsysIdx);


