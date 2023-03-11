//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must include before t_data_obj_conf.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must include before t_data_obj_conf.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must include before t_data_obj_conf.h
#endif

#ifdef __T_DATA_OBJ_CONF_H__
  #error t_data_obj_conf.h included more than once
#endif

#define __T_DATA_OBJ_CONF_H__


//--------------------------------------------------------------------------------------------------
//! Defines for Config-change-flag Handling
#define HART_MASTER_CONFFLAG_1  0x01    // Config-Changed-Flag of Primary Master first Layer 2
#define HART_SECMAS_CONFFLAG_1  0x02    // Config-Changed-Flag of Secondary Master first Layer 2
#define HART_MASTER_CONFFLAG_2  0x04    // Config-Changed-Flag of Primary Master second Layer 2
#define HART_SECMAS_CONFFLAG_2  0x08    // Config-Changed-Flag of Secondary Master second Layer 2

//--------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::CONF
extern const T_DATA_CLASS cgClassInitializer_CONF;

//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::CONF
 \author Heiko Kresse
 \date   2004-11-09
 \param  x ptrValue; the address will be casted to (void FAST*)!
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_CONF(x) { (void FAST*)(x), 1,sizeof(TUSIGN8),NON_VOLATILE,SIMPLE, 0, &cgClassInitializer_CONF }


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Get configuration changed flag dependant on communication type (master/slave)
 \author Stefan Tabelander, Heiko Kresse
 \date   2004-11-09
 \param  the actual value of the config-change-flag related to the actual Master will be returned
 \param  attribIdx ==-1 --> getObject; ==0 --> getAttribute  ; else illegal attribute
 \param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \return error-code
 <pre>
     TDO_OK                    operation was successful
     TDO_ILLEGAL_ATTRIB_IDX    DEBUG, unknown Attribute
     TDO_FATAL_ERROR           object outside nv-mem range
 </pre>
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_CONF(const T_DATA_OBJ SLOW*me,
                  void FAST* ptrValue,
                  TINT16 attribIdx,
                  T_DATA_SEMAPHORE FAST* ptrDataSemaphore);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Set both config-change-flags in value
 \author Heiko Kresse
 \date   2004-11-09
 \param  ptrValue object or attribute in internal format
 \param  attribIdx not used
 \return -
 \warning
 \test
 \n by: Heiko Kresse
 \n intention:
 \n result module test:
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Transform2Internal_CONF(const T_DATA_OBJ SLOW*me, void FAST* ptrValue, TINT16 attributeIndex );
