//----------------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Components HART
 Module
 Description    Definition of class T_DATA_OBJ::ADDR (Polling-Addr or Shortframe-Addr)
 <pre>
   This is a Singleton Class-definition. That means only one object with this class is possible.
   The shortframe-address data-object is a very special one, reuse seems not mecessary.

   The put method will be overloaded. The Constructor need no parameter

 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must include before t_data_obj_addr.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must include before t_data_obj_addr.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must include before t_data_obj_addr.h
#endif

#ifdef __T_DATA_OBJ_ADDR_H__
  #error t_data_obj_addr.h included more than once
#endif

#define __T_DATA_OBJ_ADDR_H__


//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::ADDR
extern const T_DATA_CLASS cgClassInitializer_ADDR;

//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::ADDR
 \author Heiko Kresse
 \date   2004-11-09
 \param  x ptrValue; the address will be casted to (void FAST*)!
 \param  z ptr to range-description
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_ADDR(x,z) { (void FAST*)(x),1,sizeof(TUSIGN8),NON_VOLATILE,SIMPLE, z, &cgClassInitializer_ADDR }

//--------------------------------------------------------------------------------------------------
/*!
 \brief                   check device short address and controlls multidrop mode
 \author                  Stefan Tabelander, Heiko Kresse
 \date                    2004-11-09
 \param                   pointer to data-object, value pointer, attribute index
 \return
                      <pre>
                          TDO_OK                       operation was successful
                          TDO_ILLEGAL_ATTRIB_IDX       DEBUG, unknown Attribute
                          TDO_GREATERTHAN_RANGEMAX_ERR invalid address
                          TDO_FATAL_ERROR              object outside nv-mem range
                      </pre>
 \warning                 None
 \test                    Open
 date:                    2004-10-07
 \n by:                   Stefan Tabelander
 \n environment:          HART_ModuleTest
 \n intention:            Multidrop, correct writing
 \n result module test:   OK
 \n result Lint Level 3:  OK
 \bug                     None
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16   Check_ADDR(const T_DATA_OBJ SLOW*me, void FAST* ptrValue, TINT16 attribIdx);
