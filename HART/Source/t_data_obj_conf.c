//----------------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Components HART
 Module
 Description    Definition of class T_DATA_OBJ::CONF (_CHANGE_FLAGS)
 <pre>
   This is a Singleton Class-definition. That means only one object with this class is possible.
   The config-change-flags data-object is a very special one, reuse seems not mecessary.

   The Put and Get-method will be overloaded. The Constructor need no parameter

   Get ignores attribIdx
   Put with attribIdx==0 as Reset Config-Change-Flag
   Put with attribIdx==-1 set both Config-Change-Flags

 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "hart_types.h"
#include "hart_attributes.h"
#include "t_data_obj_conf.h"
#include "HART/Layer7/Interface/layer7.h"


//----------------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::CONF
*/
//----------------------------------------------------------------------------------------------------------

//lint -e{641} Martin Dahl 2006-01-12 converting enum to int is OK
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = { ST_U8 };

const T_DATA_CLASS cgClassInitializer_CONF =
{
    cmAryAttributeDataTypes,
    Check_DATAOBJ,                      // no business rules
    Transform2Internal_CONF,            // set or reset the two configuration-changed-flags
    0,
    Get_CONF,                           // overload
    Put_DATAOBJ,                        // inherit
    GetAttributeDescription_DATAOBJ,    // inherit
    GetObjectDescription_DATAOBJ        // inherit
};

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Get configuration changed flag dependant on communication type (master/slave)
 \author    Stefan Tabelander, Heiko Kresse, Horst Seele
 \date      2005-11-09
 \param     the actual value of the config-change-flag related to the actual Master will be returned
 \param     attribIdx ==-1 --> getObject; ==0 --> getAttribute  ; else illegal attribute
 \param     ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \return    error-code
 <pre>
     TDO_OK                    operation was successful
     TDO_ILLEGAL_ATTRIB_IDX    DEBUG, unknown Attribute
     TDO_FATAL_ERROR           object outside nv-mem range
 </pre>
 \warning
 \test
 test-date: 2005-12-01
 \n by:     Horst Seele
 \n environment: C-SPY, see file �Test_t_data_obj_conf.c?of module test functions
 \n intention: Instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Get_CONF(const T_DATA_OBJ SLOW *me, void FAST *ptrValue, TINT16 attribIdx, T_DATA_SEMAPHORE FAST *ptrDataSemaphore)
{
    TUSIGN8 configurationFlags;
    TUSIGN16 ret = OK;

    // get value considering the semaphore
    ret = Get_DATAOBJ(me,&configurationFlags,attribIdx,ptrDataSemaphore);

    if (ret < ERROR_CODES)
    {
        //lint -e{746} Heiko Kresse 2004-11-09
        VIP_ASSERT(ptrValue);

        // second Layer 2 use bits 2 and 3 of configuration flags object
        if (requestsLayer2_LAYER7 == SECOND_LAYER2)
        {
            configurationFlags >>= 2;
        }

        // if secondary master shift used configuration changed flag bit to bit 0
        if ((transmitBuffer_LAYER7.aryAdr[0] & LAYER2_PRIMARY_MASTER) == 0)
        {
            configurationFlags >>= 1;
        }

        // resulting configuration changed flag is bit 0
        //lint -e{643} Marcel Schilg : cast of pobjTxValue is intentional
        *(TUSIGN8*)ptrValue = configurationFlags & 0x01;

    }
    return ret;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Set/Reset configuration change flags in value
 \author    Heiko Kresse, Horst Seele
 \date      2005-11-09
 \param     ptrValue object or attribute in internal format
 \param     attribIdx not used
 \return    OK
 \warning
 \test
 test-date: 2005-12-01
 \n by:     Horst Seele
 \n environment: C-SPY, see file �Test_t_data_obj_conf.c?of module test functions
 \n intention:  Instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Transform2Internal_CONF(const T_DATA_OBJ SLOW *me, void FAST *ptrValue, TINT16 attributeIndex)
{
    // TUSIGN8 will not be interrupted
    TUSIGN8 configurationFlags = hartFreqCst.configurationFlags;

    VIP_ASSERT(ptrValue);

    if (attributeIndex == -1)
    {
        // Set all configuration changed flags
        configurationFlags = HART_MASTER_CONFFLAG_1 + HART_SECMAS_CONFFLAG_1 + \
                             HART_MASTER_CONFFLAG_2 + HART_SECMAS_CONFFLAG_2;
    }
    else
    {
        // Reset flags depending on communication type individually
        if (requestsLayer2_LAYER7 == SECOND_LAYER2)
        {
            // Reset second Layer 2 primary master config changed flag
            if ((transmitBuffer_LAYER7.aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0)
            {
                configurationFlags &= ~HART_MASTER_CONFFLAG_2;
            }
            // Reset second Layer 2 secondary master config changed flag
            else
            {
                configurationFlags &= ~HART_SECMAS_CONFFLAG_2;
            }
        }
        else
        {
            // Reset first Layer 2 primary master config changed flag
            if ((transmitBuffer_LAYER7.aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0)
            {
                configurationFlags &= ~HART_MASTER_CONFFLAG_1;
            }
            // Reset first Layer 2 secondary master config changed flag
            else
            {
                configurationFlags &= ~HART_SECMAS_CONFFLAG_1;
            }
        }
    }

    *(TUSIGN8 FAST*)ptrValue = configurationFlags;

    return OK;
    //lint -e{715} Heiko Kresse 2004-11-09
}
