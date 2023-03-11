//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Implementation of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

// @@adjust
#include "Coordinator/Interface/subsystem_idx.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"

#include "FrontEndInterface/Interface/FrontEndInterface_idx.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "HART/Layer7/Interface/layer7.h"
#include "hart_types.h"
#include "hart_action_fct.h"
#include "hart_attributes.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"

#include "ARM/Interface/arm.h"

// @@adjust
//#include "..\..\coordinator\interface\application_idx.h"//Eric

//@SubGen put@ <METHOD>
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER PERFORMACTIONS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER PERFORMACTIONS> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD PERFORMACTIONS>
TUSIGN16 performActions_HART(void)
//@SubGen end@<METHOD PERFORMACTIONS>
{
    return (TUSIGN16)OK;
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SETPVUPRANGEVALUE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SETPVUPRANGEVALUE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SETPVUPRANGEVALUE>
TUSIGN16 setPVUpRangeValue_HART(void)
//@SubGen end@<METHOD SETPVUPRANGEVALUE>
{
    TUSIGN8 data = MAPPER_UPPER_RANGE;
    return mapper_Put(MAPPER_IDX_setCurrentValueAsPVRange , WHOLE_OBJECT ,&data);  
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SETPVLOWERRANGEVALUE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SETPVLOWERRANGEVALUE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SETPVLOWERRANGEVALUE>
TUSIGN16 setPVLowerRangeValue_HART(void)
//@SubGen end@<METHOD SETPVLOWERRANGEVALUE>
{
    TUSIGN8 data = MAPPER_LOWERER_RANGE;        
    return mapper_Put(MAPPER_IDX_setCurrentValueAsPVRange,WHOLE_OBJECT ,&data);  
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SETDEVICEVARIABLENZERO> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SETDEVICEVARIABLENZERO> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SETDEVICEVARIABLENZERO>
TUSIGN16 setDeviceVariableNZero_HART(void)
//@SubGen end@<METHOD SETDEVICEVARIABLENZERO>
{  
    return (TUSIGN16)OK;       
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETDEVICEVARIABLENTRIM> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETDEVICEVARIABLENTRIM> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETDEVICEVARIABLENTRIM>
TUSIGN16 resetDeviceVariableNTrim_HART(void)
//@SubGen end@<METHOD RESETDEVICEVARIABLENTRIM>
{
    return (TUSIGN16)OK;    
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SERVICECODELOGOUT> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SERVICECODELOGOUT> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVICECODELOGOUT>
TUSIGN16 serviceCodeLogOut_HART(void)
//@SubGen end@<METHOD SERVICECODELOGOUT>
{
    TUSIGN16 result;
    result = arm.LogOffEXE(HART_IDX);
    hartDyn.HartAccessLevel = HART_SERVICE_service;
    return result;        
}



//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SQUAWK> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SQUAWK> \endif
 \author        Stefan Tabelander
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SQUAWK>
TUSIGN16 squawk_HART(void)
//@SubGen end@<METHOD SQUAWK>
{
    // NOTE:
    // HARTLayer7CommandInterpreter() checks for #72 and handles the squawk activation and de-activation directly.
    // The function will be automatically stopped if the squawk timer reaches 0 (see squawkCounter).

    // Here the function shall do nothing
    return(OK);
}



//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETMORESTATUSAVAILABLE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETMORESTATUSAVAILABLE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETMORESTATUSAVAILABLE>
TUSIGN16 resetMoreStatusAvailable_HART(void)
//@SubGen end@<METHOD RESETMORESTATUSAVAILABLE>
{
    TUSIGN32    diagCounter;
    TBOOL       resetMoreStatusAvailablePrimMaster = eFALSE;
    TBOOL       resetMoreStatusAvailableSecMaster  = eFALSE;
    T_LAYER2_BUFFER* pTxBuffer = NULL;
    T_LAYER2_BUFFER* pRxBuffer = NULL;

    if(HART_SPbufferState == SP_BUFFER_STATE_RECEIVED)
    {
        pTxBuffer = &HART_SPtransmitBuffer;
        pRxBuffer = &HART_SPreceiveBuffer;
    }
    else
    {
        return RULE_VIOLATION;
    }

    // prepare cmd 48 response corresponding to hart revision
    HARTReadSubscribedObjects (48, pRxBuffer, pTxBuffer);

    // if request contains data for comparison
    if (pRxBuffer->numByte == 24)
    {
        // if no difference between actual states in response and states in request
        if (memcmp(pTxBuffer->aryData,
            pRxBuffer->aryData,
            pRxBuffer->numByte) == 0)
        {
            // Data matches those contained in the device ==> carry on reset MSA

            // Check for master individual MSA bit
            if((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0)
            {
                // reset primary master MSA bit
                resetMoreStatusAvailablePrimMaster = eTRUE;
            }
            else
            {
                // reset secondary master MSA bit
                resetMoreStatusAvailableSecMaster = eTRUE;
            }
        }
        else
        {
            // Set status bytes mismatch response code in first response byte
            pTxBuffer->firstResp = HART_RESP_WARN_STATUS_BYTES_MISMATCH;
        }
    }
    else if (pRxBuffer->numByte == 0)
    {
        // Only in HART 5,6 #48 with 0 data-bytes is allowed to clear the MSA
        if(hartFreqCst.univCmdRev == 5)
        {
            // Reset MSA
            resetMoreStatusAvailablePrimMaster = eTRUE;
            resetMoreStatusAvailableSecMaster  = eTRUE;
        }
        else
        {}
    }
    else
    {
        // Invalid number of bytes received
        pTxBuffer->firstResp = HART_RESP_ERR_TOO_FEW_DATA;
    }

    // store diagnosis counter as new value for device status calculation
    // @@adjust
    (void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisCounter, &diagCounter);
    //(void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsAlarmCounter, &diagCounter);

    // Reset the MSA bit individually for each master
    // Primary master MSA reset
    if(resetMoreStatusAvailablePrimMaster)
    {
        hartDyn.diagCounterMoreStatusAvailablePrimMaster = diagCounter;
    }
    else{}

    // Primary master MSA reset
    if(resetMoreStatusAvailableSecMaster)
    {
        hartDyn.diagCounterMoreStatusAvailableSecMaster = diagCounter;
    }
    else{}

    return OK;
}




//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETDEVICE> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETDEVICE> \endif
 \author            Stefan Tabelander
 \date              2010-10-04
 \param void
 \return error-code
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \n lint check o.k.:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETDEVICE>
TUSIGN16 resetDevice_HART(void)
//@SubGen end@<METHOD RESETDEVICE>
{
    // Note:
    // The HART conform reset will be initiated here (not performed immediately).
    // In a transmit state (see IRQ_Transmit_LAYER2) the HART subsystem will add HART_PERFORM_RESET_GO to this value.
    // The result will be (HART_PERFORM_RESET_PREPARE | HART_PERFORM_RESET_GO) ==> HART_PERFORM_RESET
    // HARTCommandInterpreterEXE_HART() will check for HART_PERFORM_RESET and perform the reset with the correct timing
    //performDeviceReset = HART_PERFORM_RESET_PREPARE;
    performDeviceReset = HART_PERFORM_RESET;
    return OK;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETCONFIGFLAG> \endif
 \brief  Reset configuration changed flags
 \if @SubGen end@ <METHODHEADER RESETCONFIGFLAG> \endif
 \author    Martin Dahl
 \date      2006-01-12
 \param     void
 \return    PutObject ErrorCode
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: PD30F, HCF Application Layer Test sripts
 \n intention: sight check
 \n result  module test: ok
 \n result  Lint Level 3: ok, 2008-04-29
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETCONFIGFLAG>
TUSIGN16 resetConfigFlag_HART(void)
//@SubGen end@<METHOD RESETCONFIGFLAG>
{
    TUSIGN8     byteVariable = 0xFF;
    TUSIGN8     result = OK;
    TPOLY16     wordVariable, wordVariable1;
    T_LAYER2_BUFFER* pTxBuffer = NULL;
    T_LAYER2_BUFFER* pRxBuffer = NULL;

    if(HART_SPbufferState==SP_BUFFER_STATE_RECEIVED)
    {
        pTxBuffer = &HART_SPtransmitBuffer;
        pRxBuffer = &HART_SPreceiveBuffer;
    }
    else
    {
        return RULE_VIOLATION;
    }

    // actual configuration change counter value
    wordVariable.u16 = hartFreqNo.confChangeCounterValue;

    // if request contains configuration change counter
    if (pRxBuffer->numByte != 0)
    {
        // counter value of request as config change counter reference value
        wordVariable1.aryU8[0] = pRxBuffer->aryData[1];
        wordVariable1.aryU8[1] = pRxBuffer->aryData[0];
        // if config change counter from req != config change counter in device
        if (wordVariable1.u16 != wordVariable.u16)
        {
            // No reset of config flags because counter not equal
            pTxBuffer->firstResp = HART_RESP_ERR_CONFIG_CHANGE_COUNTER_MISMATCH;
            // adapt response length
            pTxBuffer->numByte = 2;
            // error break condition
            result = RULE_VIOLATION_ERR;
        }
    }
    // if without configuration counter compare or counter are equal
    if (result == OK)
    {
        // attribute index != -1 (here 0) resets configuration changed flag of requesting master
        (void)HARTPutAttribute(HART_IDX, HART_IDX_configurationFlags, 0, &byteVariable);
        // if compliant to hart rev 7
        if (hartFreqCst.univCmdRev == 7)
        {
            // adapt response length
            pTxBuffer->numByte = 4;
            // add configuration change counter value to response
            pTxBuffer->aryData[0] = wordVariable.aryU8[1];
            pTxBuffer->aryData[1] = wordVariable.aryU8[0];
        }
        else
        {
            // adapt response length
            pTxBuffer->numByte = 2;
        }
    }
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER STARTSELFTEST> \endif
 \brief  Starts self test
 \if @SubGen end@ <METHODHEADER STARTSELFTEST> \endif
 \author    Stefan Tabelander
 \date      2004-06-30
 \param     -
 \return    -
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: PD30F, HCF Application Layer Test sripts
 \n intention: sight check
 \n result  module test: ok
 \n result  Lint Level 3: ok, 2008-04-29
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD STARTSELFTEST>
TUSIGN16 StartSelfTest_HART(void)
//@SubGen end@<METHOD STARTSELFTEST>
{
   // @@adjust
   // The selftest is usually cyclically performed in the background (e.g. by ElectronicServices)
   // The result of the cyclic testing is mapped to #48 in this case

   // So this function may always return OK, but can be adjusted to the target device implementation requirements

   return OK;
}

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <METHOD>
//--------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2011-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/
