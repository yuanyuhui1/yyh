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


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif


#ifdef __BURST_FUNCTIONS_H__
  #error burst_functions.h included more than once
#endif


#define __BURST_FUNCTIONS_H__


//--------------------------------------------------------------------------------------------------
/*!
 \brief  PrepareBurstBuf_LAYER7: Prepare LAYER7 receive buffer with request data bytes of burst message.
 \author Bernhard Tohermes
 \date   2010-02-02
 \param
 \return Prepared Receive buffer of Layer7.
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

extern void PrepareBurstBuf_LAYER7 (void);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTCheckBurstTriggerCmdSettings: Check burst trigger parameter in data of command 104
 \author    Bernhard Tohermes
 \date      2010-02-02
 \param
 \return    errorCode:  OK      - trigger data ok: request data can be written to objects
 \return                not OK  - trigger data not ok: response contains specific fault code
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------

extern TUSIGN16 HARTCheckBurstTriggerCmdSettings (const T_BURSTMSG_CONFVALUES  *burstMsgConfDataPtr,
                                                       const CMD_104_REQ_RESP_DATA  *reqDataPtr,
                                                       TUSIGN8* pFirstResp);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTPrepareBurstTriggerObjectInfo: Prepare burst trigger objects classification, units code, value
 \author    Bernhard Tohermes
 \date      2010-02-02
 \param     slotCode:       number of slot code corresponding to defined slot codes for command 9
 \param     burstMsgNumber: number of burst message
 \return
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------

extern void HARTPrepareBurstTriggerObjectInfo(TUSIGN8 slotCode, TUSIGN8 burstMsgNumber);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform correction of Update Period values
 \author    Bernhard Tohermes
 \date      2010-01-28
 \param     ptrValue = pointer to update period counter value.
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
  \n result: response code 
  \bug
 */
 //--------------------------------------------------------------------------------------------------
 
 extern TUSIGN8 CorrectUpdatePeriod(TUSIGN32 *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Identify burst message with shortest update period as default burst message
 \author    Bernhard Tohermes
 \date      2010-01-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------

extern void IdentDefaultBurstMsg(void);
