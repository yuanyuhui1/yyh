//--------------------------------------------------------------------------------------------------
/*!
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module         HART
 Description    Implementation of specialized methods for writing the short address
                Handling sets device in multidrop mode if short address was changed
                to a value greater zero.
 Remarks        Tested

*/
//--------------------------------------------------------------------------------------------------
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
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "Hart_types.h"
#include "Hart_attributes.h"
#include "t_data_obj_addr.h"

//--------------------------------------------------------------------------------------------------
/*! definition of the initializer for ojects of class T_DATA_OBJ::ADDR
*/
//--------------------------------------------------------------------------------------------------
//lint -e{641} Martin Dahl 2006-01-12 converting enum to int is OK
static SLOW const TUSIGN8 cmAryAttributeDataTypes[] = { ST_U8 };

const T_DATA_CLASS cgClassInitializer_ADDR =
{
    cmAryAttributeDataTypes,
    Check_ADDR,                         // overload
    0,                                  // no internal view
    0,
    Get_DATAOBJ,                        // inherit
    Put_DATAOBJ,                        // inherit
    GetAttributeDescription_USIGN8,     // overload
    GetObjectDescription_DATAOBJ        // inherit
};

//--------------------------------------------------------------------------------------------------
/*!
 \brief     check device short address and controlls multidrop mode
 \author    Stefan Tabelander, Heiko Kresse, Horst Seele
 \date      2006-02-06
 \param     pointer to data-object, value pointer, attribute index
 \return
            OK                       operation was successful
            ILLEGAL_ATTRIB_IDX       DEBUG, unknown Attribute
            GREATERTHAN_RANGEMAX_ERR invalid address
            FATAL_ERROR              object outside nv-mem range
 \warning
 \test
 date:      2008-04-21
 \n by:     Marcel Schilg
 \n environment: PD30F
 \n intention:  sight check
 \n result  module test: OK
 \n result  Lint Level 3: OK
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16   Check_ADDR(const T_DATA_OBJ SLOW*me, void FAST* ptrValue, TINT16 attribIdx)
{
   //lint -e643 Martin Dahl 2006-01-12 converting enum to int is OK
   TUSIGN8  pollAddr        = *(TUSIGN8*)ptrValue;
   TUSIGN16 ret             = OK;
   TUSIGN8  adrLimit;

   // debug zero pointer and illegal values
   //lint -e{746} Horst Seele 2006-01-06 lint has problems with the first intrinsic-function
   VIP_ASSERT(me);
   VIP_ASSERT(ptrValue);

   // if device working compliant to hart rev 7
   //    && request contains also loop current mode
   if (hartFreqCst.univCmdRev == 7)
       //&& (transmitBuffer_LAYER7.numByte == 2))
       // limit = 63
   {
       adrLimit = 63;
   }
   else
   {
       // limit = 15
       adrLimit = 15;
   }
   // check range
   if( pollAddr > adrLimit )
   {
       ret = RULE_VIOLATION_ERR;
   }
   return ret;

   //lint -e{715} -e{818} Stefan Tabelander 2005-01-12
   //lint +e641 Martin Dahl 2006-01-12 turn errorchecking on again
}
