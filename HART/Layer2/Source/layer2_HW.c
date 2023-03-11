//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    implementation layer2 interface methods

 Remarks
                uses UART0 with CTS AND! RTS function if FSK is activated
                Interrupt-Level 6

*/
//--------------------------------------------------------------------------------------------------
#include <string.h>
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Source/Hart_types.h"
#include "HART/Source/Hart_attributes.h"
#include "HART/Interface/Hart_idx.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Interface/Hart.h"



// flags for communication between timer, receiver and transmitter
//#pragma dataseg=FAST STATIC_INTERRUPT_DATA
__no_init FAST T_LAYER2_FLAGS uLayer2flags;
__no_init FAST T_LAYER2_T_UNIT_MAPPER t_unit_mapper;

__no_init FAST T_LAYER2_BUFFER uTransmitBuffer_LAYER2;
__no_init FAST T_LAYER2_BUFFER uReceiveBuffer_LAYER2;

__no_init FAST TUSIGN8 uExpansionBytesCounter;
__no_init FAST TUSIGN8 performDeviceReset;
__no_init FAST TUSIGN8 bitRateCode;

__no_init FAST T_LAYER2_BUFFER mBurstBuffer;
//#pragma dataseg=default

//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize mapper for T_UNIT parameters used in ISR
 \author    Stefan Tabelander
 \date      2011-04-11
 \param
 \return
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
void Initialize_T_UNIT_MAPPER(TUSIGN16 objectIndex)
{
    // Note:
    // all these objects must be considered in the Put overload.

    // Update the full scope of objects
    if(objectIndex == 0xFFFF)
    {
        // Objects in hartDyn
        t_unit_mapper.findDeviceSwitch      = hartDyn.findDeviceSwitch;

        // Objects in hartFreqCst
        t_unit_mapper.pollAddr              = hartFreqCst.pollAddr;
        (void)memcpy(t_unit_mapper.tag, hartFreqCst.tag, 6);            // memcpy(dest, source, num)
        (void)memcpy(t_unit_mapper.longTag, hartFreqCst.longTag, 32);   // memcpy(dest, source, num)
        t_unit_mapper.univCmdRev            = hartFreqCst.univCmdRev;

        // Objects in hartRare
        (void)memcpy(t_unit_mapper.deviceIdNumber, hartRare.deviceIdNumber, 3); // memcpy(dest, source, num)
        t_unit_mapper.numberRespPream       = hartRare.numberRespPream;
    }
    else
    {
        switch (objectIndex)
        {
            case HART_IDX_findDeviceSwitch:
                // Objects in hartDyn
                t_unit_mapper.findDeviceSwitch = hartDyn.findDeviceSwitch;
                break;

            case HART_IDX_pollAddr:
                // Objects in hartFreqCst
                t_unit_mapper.pollAddr = hartFreqCst.pollAddr;
                break;

            case HART_IDX_tag:
                (void)memcpy(t_unit_mapper.tag, hartFreqCst.tag, 6);            // memcpy(dest, source, num)
                break;

            case HART_IDX_longTag:
                (void)memcpy(t_unit_mapper.longTag, hartFreqCst.longTag, 32);   // memcpy(dest, source, num)
                break;

            case HART_IDX_univCmdRev:
                t_unit_mapper.univCmdRev = hartFreqCst.univCmdRev;
                break;

            case HART_IDX_deviceIdNumber:
                (void)memcpy(t_unit_mapper.deviceIdNumber, hartRare.deviceIdNumber, 3); // memcpy(dest, source, num)
                break;

            case HART_IDX_numberRespPream:
                t_unit_mapper.numberRespPream = hartRare.numberRespPream;
                break;

            default:
                break;
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize Layer 2 and the HART-UART
 \author    Heiko Kresse, Stefan Tabelander
 \date      2011-01-06
 \param
 \return
 \warning
 \test
 test-date:
 \n by:
 \n environment: defined in HCF-Specification
 \n intention: 1. complete instruction covarage (guarantied by link-layer-test)
 \n            2. black-box-test --> link-layer-test
 \n result  link layer test:
 \n result  Lint Level 3:
 \n lint check:
 \bug
*/
//--------------------------------------------------------------------------------------------------
void Initialize_LAYER2(void)
{
    bitRateCode = hartRare.bitRateCode;

    #ifdef PROVIDE_HIGHER_BITRATES
    if( (size_t)bitRateCode >= sizeof(clAryBitrate)/sizeof(clAryBitrate[0]) )
    #endif
    {
        bitRateCode=0;
    }
    
    // disable receiver
    uLayer2flags.receiverActivBit = 0;
    // clear pending layer7 request
    uLayer2flags.layer7requestedBit = 0;

    uLayer2flags.burstMsgReqBuilt = 0;
    uLayer2flags.burstMsgRespBuilt = 0;
    uLayer2flags.burstMsg1stSent = 0;
    uLayer2flags.burstMsgWoutBuilt = 0;
    uLayer2flags.burstCmdChanged = 0;
    uLayer2flags.bolDeviceIsBusy = 0;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     returns eTRUE if layer 7 is requested
 \author    Heiko Kresse
 \date      2004-02-24
 \param
 \return
 \warning
 \test
 test-date: 2005-03-14
 \n by:     Heiko Kresse
 \n environment: defined in HCF-Specification
 \n intention: 1. sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL AskIfLayer7requested_LAYER2(void)
{
    return(TBOOL)(uLayer2flags.layer7requestedBit!=0);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     clears the layer7 request
 \author    Heiko Kresse
 \date      2004-02-24
 \param
 \return
 \warning
 \test
 test-date: 2005-03-14
 \n by:     Heiko Kresse
 \n environment: defined in HCF-Specification
 \n intention: 1. sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
void ClearLayer7request_LAYER2(void)
{
    uLayer2flags.layer7requestedBit=0;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialialize burst buffer with always identical frame start bytes
 \author    Bernhard tohermes
 \date      2010-04-15
 \param
 \return
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010 \bug
*/
//--------------------------------------------------------------------------------------------------
void InitBurstBuffer_LAYER2(void)
{
    // *** prepare response frame start ***
    // number of response preambles
    mBurstBuffer.cntPreamble = t_unit_mapper.numberRespPream;
    // delimiter = burst frame without expansion-bytes
    mBurstBuffer.delimiter = LAYER2_BURST_DELIMITER;
    // set address
    mBurstBuffer.aryAdr[0] = hartCst.mfrId;
    mBurstBuffer.aryAdr[1] = hartCst.mfrDeviceType;
    mBurstBuffer.aryAdr[2] = t_unit_mapper.deviceIdNumber[0];
    mBurstBuffer.aryAdr[3] = t_unit_mapper.deviceIdNumber[1];
    mBurstBuffer.aryAdr[4] = t_unit_mapper.deviceIdNumber[2];
    // no expansionbytes
    mBurstBuffer.aryExp[0]=0;
    mBurstBuffer.aryExp[1]=0;
    mBurstBuffer.aryExp[2]=0;
    uExpansionBytesCounter=0;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialialize burst buffer with always identical frame start bytes
 \author    Bernhard tohermes
 \date      2010-04-15
 \param
 \return    eTRUE: device in burst mode
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n lint check o.k.: B. Tohermes; 30-08-2010 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL DetectBurstMode_LAYER2(void)
{
    TUSIGN8                         idx;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;

    // for all burst messages
    for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
    {
        // pointer of burst message specific confi objects reference table
        burstMsgConfDataPtr = burstMsgConfObjRefTable[idx].burstMsgConfObjPtr;
        // if burst mode activated for current burst message
        if(burstMsgConfDataPtr->modeControlCode != 0)
        {
            return(eTRUE);
        }
    }
    return(eFALSE);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     prepare transmitbuffer for transmission of a burst frame
 \author    Heiko Kresse
 \date      2004-02-23
 \param
 \return
 \warning
 \test
 test-date: 2005-03-14
 \n by:     Heiko Kresse
 \n environment: defined in HCF-Specification
 \n intention: 1. complete instruction covarage (guarantied by link-layer-test)
 \n            2. black-box-test --> link-layer-test
 \n result  link layer test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
void PrepareBurstBuf_LAYER2(TUSIGN8 cmdNumber)
{
    // Delimiter = burst fram without expansion-bytes
    uTransmitBuffer_LAYER2.delimiter = LAYER2_BURST_DELIMITER;
    // number of response preambles
    uTransmitBuffer_LAYER2.cntPreamble = t_unit_mapper.numberRespPream;

    // set address
    uTransmitBuffer_LAYER2.aryAdr[0] = hartCst.mfrId;
    uTransmitBuffer_LAYER2.aryAdr[1] = hartCst.mfrDeviceType;
    uTransmitBuffer_LAYER2.aryAdr[2] = t_unit_mapper.deviceIdNumber[0];
    uTransmitBuffer_LAYER2.aryAdr[3] = t_unit_mapper.deviceIdNumber[1];
    uTransmitBuffer_LAYER2.aryAdr[4] = t_unit_mapper.deviceIdNumber[2];

    // no expansionbytes
    uTransmitBuffer_LAYER2.aryExp[0]=0;
    uTransmitBuffer_LAYER2.aryExp[1]=0;
    uTransmitBuffer_LAYER2.aryExp[2]=0;
    uExpansionBytesCounter=0;

    // command number
    uTransmitBuffer_LAYER2.command = cmdNumber;
}
