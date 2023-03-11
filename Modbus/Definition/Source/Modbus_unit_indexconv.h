//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Implementation units index conversion

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
#error common_type.h must be included before MODBUS_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MODBUS_UNIT_INDEXCONV_H__
#error MODBUS_unit_indexconv.h included more than once
#endif

#define __MODBUS_UNIT_INDEXCONV_H__


void Out2inConv(TUSIGN8 index, TUSIGN8 *indexA, TUSIGN8 *indexB);
TBOOL In2outConv(TUSIGN8 indexA, TUSIGN8 indexB, TUSIGN8 *index);