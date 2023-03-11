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

// General includes
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#include "UnitTests/stubs/include/microcontroller_stub.h"
#else
#include "System/Interface/microcontroller.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "hart_arm.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "HART/Source/hart_overload.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
// Local definition for HART access response
#define   ACCESS_OK             0
#define   ACCESS_RESTRICTED     16
#define   WRITE_PROTECTED       7

//--------------------------------------------------------------------------------------------------
/*!
 \brief     check if HART can access parameter
 \author    IIMS
 \date      2010-10-05
 \param
    <pre>
       cmdNumber:               Command number presently read or written to
       direction:               Either READ or WRITE
       isPrimaryMasterActive:   Information if primary master is accessing or not
    <\pre>
 \return
 \return
    <pre>
       HART_RESP_OK,                    if access accepted
       HART_RESP_ERR_ACCESS_RESTRICTED, if access denied due to Lock Device (#71) or access right management (#147)
       HART_RESP_ERR_WRITE_PROTECTED,   if access denied due to binary input write locking (ALL)
    <\pre>
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 HARTCanAccess(TUSIGN16 cmdNumber, TUSIGN8 direction, TBOOL isPrimaryMasterActive)
{
    TBOOL     ret;

    // @@adjust
    // The whole write protection strategy must be adapted to the individual device requirements
    // This shall only serve as an example
    TUSIGN8   physicalWriteLockState  = ELETTRONICSERVICES_WRITE_PROTECTION_NONE; //  Get the write protection state for the device



    TUSIGN8   lockDeviceState         = hart.GetDeviceLockStatus();          //  Get the write lock state from HART command 71 implementation


    //elettronicServices_Get(ELETTRONICSERVICES_IDX_swWriteProtection,WHOLE_OBJECT,&physicalWriteLockState);

    hart_Get(HART_IDX_hwWriteProtection,WHOLE_OBJECT,&physicalWriteLockState);

    // Design Note for command 71 Lock Device @@st:
    // this mechanism does not consider a second layer2. Here any master of the same kind
    // no matter if first layer 2 or second layer 2 can remove the protection.
    // EDP300 does not make use of a second layer 2.

    //***************************************************************************************************************************
    //          ACCESS RIGHTS IMPLEMENTATION STARTS HERE
    //***************************************************************************************************************************

    // Read access is never locked
    if (direction == ACCESS_READ)
    {
        ret = ACCESS_OK;
    }
    
    // add 2012-12-19; changed for HCF TEST
    else if(cmdNumber == HART_CMD_038)
    {
        ret = ACCESS_OK;
    }
    //@adjust: 2013-01-29; changed for disable software write protection
    else if( (!(PUSH_BUTTON_WRITE_PROTECT_IS_ON))&&(cmdNumber == HART_CMD_185) )
    {
        ret = ACCESS_OK;
    }
    // HART access locked due to device write protection mechanism
    else if(physicalWriteLockState == ELETTRONICSERVICES_WRITE_PROTECTION_ON)
    {
        ret = WRITE_PROTECTED;
    }
    else if( (lockDeviceState == HART_LOCK_STATUS_NOT_LOCKED) && (physicalWriteLockState != ELETTRONICSERVICES_WRITE_PROTECTION_ON))
    {
        ret = ACCESS_OK; 
    }
    else if((lockDeviceState & HART_LOCK_STATUS_ALL_LOCK) && (cmdNumber == HART_CMD_071))
    {
        ret = ACCESS_OK;
    }
    // Device was locked for both, primary and secondary master,
    // only removing the lock is allowed
    else if((lockDeviceState & HART_LOCK_STATUS_ALL_LOCK) && (cmdNumber != HART_CMD_071))
    {
        ret = ACCESS_RESTRICTED;
    }
    // only single device lock according to #71 possible from here...
    // If primary master locked the device, but secondary master is calling
    else if((lockDeviceState & HART_LOCK_STATUS_PRIM_MASTER_LOCK) && !isPrimaryMasterActive )
    {
        ret = ACCESS_RESTRICTED;
    }
    // If secondary master locked the device, but primary master is calling
    else if(!(lockDeviceState & HART_LOCK_STATUS_PRIM_MASTER_LOCK) && isPrimaryMasterActive)
    {
        ret = ACCESS_RESTRICTED;
    }
    
    // Protection is not set
    else
    {
        ret = ACCESS_OK;
    }

    return ret;
}
