//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                 Generic
 Subsystem              HMI
 Description            Write protection access for the HMI subsystem.

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

/****** WriteProtect.h ******/

#ifndef _WRITE_PROTECT_H
#define _WRITE_PROTECT_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

//#define ACCESS_MODULE_TEST

#ifdef COMMON_TOP_WORKS_NEW_FEATURES
//--------------------------------------------------------------------------------------------------
/*!
 \brief         Gets the Write protection status set by the user.
 \author        Roger Arnold
 \param         TBOOL DataSpecific
                TUSIGN8 ssIdx_of_Data
                TUSIGN8 objIdx_of_Data
                TUSIGN8 attribIdx_of_Data
 \return        eTRUE - Locked
                eFALSE - not locked
 \test          No details available
*/
TBOOL WriteProtect_GetUserLock(TBOOL DataSpecific, TUSIGN8 ssIdx_of_Data, TUSIGN8 objIdx_of_Data, TUSIGN8 attribIdx_of_Data);

#else   // else condition for #ifdef COMMON_TOP_WORKS_NEW_FEATURES

//--------------------------------------------------------------------------------------------------
/*!
 \brief         Gets the Write protection status set by the remote (HART) user.
 \author        Roger Arnold
 \param
 \return        Value of the data
 \test          No details available
*/
TUSIGN8 WriteProtect_GetHARTLock(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief         Gets the Write protection status set by the local (HMI) user..
 \author        Roger Arnold
 \param
 \return        Value of the data
 \test          No details available
*/

TUSIGN8 WriteProtect_GetHMILock(void);

#endif  // end of else condition for #ifdef COMMON_TOP_WORKS_NEW_FEATURES

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Determines if the HMI can access the attribute selected
 \author Roger Arnold
 \date   2005-12-01
 \param  ATTRIB_IDENTIFIER_TYPE     Data - contains subsystem, object and attribute indices,
         TUSIGN8 ReadNotWrite       0 - the selected attribute is tested for Write access,
                                  >=1 - the selected attribute is tested for read access

 \return True  - access permitted / False - access denied
 \warning
 \test
         test-date    :  2005-12-01
         by           :  Roger Arnold
         environment  :  CSPY - AccessModuleTest();
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/

TBOOL WriteProtect_HMICanAccess(ATTRIB_IDENTIFIER_TYPE Data, TUSIGN8 ReadNotWrite);

#ifdef ACCESS_MODULE_TEST
TUSIGN8 AccessModuleTest(void);
#endif

#endif
