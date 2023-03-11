//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Framework
 Module         Electronic Services
 Description    prototypes of functions for common problems
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __SYSTEM_H__
  #error system.h must be included before common_functions.h
#endif


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before common_functions.h
#endif

#ifdef __DYNAMIC_CHECK_FUNCTIONS_H__
  #error dynamicVariablesCheck.h is included more than once
#endif

#define __DYNAMIC_CHECK_FUNCTIONS_H__


#define DYNAMIC_CHECK_VERSION (0x010000) // 1.0.0

typedef struct _T_DYNAMIC_CHECK_TABLE
{
    void    *mainValue;
    void    *backupValue;
    TUSIGN8 size;
} T_DYNAMIC_CHECK_TABLE;

//--------------------------------------------------------------------------------------------------
/*!
 \brief    Execute incremental variable check
 \author   Giovanni Invernizzi
 \date     2009-01-30
 \param
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void CheckDynamicVariables_ElectronicServices(void);