//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Parameter mapping between HART and subsystems.  User-interface
 Remarks

*/
//--------------------------------------------------------------------------------------------------

// General includes
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Layer1/serviceport_dio.h"
#include "ServicePort/Layer1/serviceport_uart.h"
#include "ServicePort/Layer2/serviceport_layer2.h"
#include "servicePort_layer7.h"
#include "ServicePort/Source/ServicePort_types.h"
#include "ServicePort/Source/ServicePort_attributes.h"
// Get the SUBSYSTEM IDs for local usage
#include "Coordinator/Interface/subsystem_idx.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"

#include "Hmi/Source/compileTimeDefinitions.h"
#include "Hmi/Source/FrameTableDefinitions.h"
#include "Hmi/Interface/HMI.h"
#include "Hmi/Source/PacketManagerLocalData.h"

#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/diagnosis.h"

#include "Mapper/Interface/Mapper_idx.h"		// to identify client subsystem(s)
#include "Mapper/Interface/Mapper.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"		// to identify client subsystem(s)

TUSIGN8 hotHMI_ReinitializeWaveformFlag;
TUSIGN16 SHMIConfigSendCount = 3;

#define PORT_FOR_HMI                0
#define PORT_FOR_SHMI               1
#define PORT_FOR_SP                 2
#define PORT_UN_KNOWN               3

//------------External variables---------------------------------------------
extern T_SPLAYER2_BUFFER ServPortTxBuffer;
extern T_SPLAYER2_BUFFER ServPortRxBuffer;


// -------------External functions--------------------------------------------
extern TBOOL TxRequestPending(void);
extern void  StartSpResponse_LAYER2(void);
extern void  InitializeSp_LAYER2(void);

//-------------Static Local functions-----------------------------------------
static TUSIGN16 spReadObj(void);
static TUSIGN16 spWriteObj(void);
static TUSIGN16 spReadMem(void);
static TUSIGN16 spWriteMem(void);
static TUSIGN16 spDisplayInitialization(void);
static TUSIGN16 spDisplayKeyInfo(void);
static TUSIGN16 spHartCMD(void);
static TUSIGN16 spReadObj_SHMI(void);


static TUSIGN16 GetObjectSize(const T_UNIT SLOW *subsystemPtr,TUSIGN16 objectIndex, TINT16 attributeIndex);

//------------Local Consts---------------------------------------------------
static const T_CMD_TBL cmdTbl[] =
{
    //CMD,              Resp Length (excludes firstResp & secResp),    Request Length,        Function pointer. 
              
    UARTI_IDENT_READ,                   41,     8,      spReadObj,
    UARTI_IDENT_WRITE,                  41,     10,     spWriteObj,		//8 is the min len
    UARTI_IDENT_READ_MEMORY,            37,     5,      spReadMem,
    UARTI_IDENT_WRITE_MEMORY,           37,     5,      spWriteMem,
    UARTI_IDENT_DISPLAY_FRAME,          1,      4,      spDisplayInitialization,
    UARTI_IDENT_DISPLAY_KEY_INFO,       1,      1,      spDisplayKeyInfo,
    UARTI_IDENT_COMMAND,                6,      4,      spHartCMD,
    UARTI_IDENT_SHMI_READ,              1,      1,      spReadObj_SHMI,

};


static const TUSIGN8 lenOfcmdTbl  = (sizeof(cmdTbl)/sizeof(T_CMD_TBL));

//---------Local Variables--------------------------------------------------------
static TUSIGN8	board; 
static TUSIGN8	feAddress; 
static TPOLY16  subsystemIdx;
static TPOLY16  objectIdx;
static TPOLY16  atrbtIdx;
static TPOLY16  memAdrs;
static TUSIGN8 	memLen;

static TUSIGN32 cnt_noResponse_layer7 = 0;
static TUSIGN32 cnt_RxCheckSumErr = 0;

HMI_PRESENCE_PIN hmiPresencePin=
{
    SP_HMI_PRESENCE_PINSTATUS_DISCONNECT,      // Previous state.
    SP_HMI_PRESENCE_PINSTATUS_DISCONNECT,      // Current state.
    eTRUE,      // First time check.
    SP_HMI_PRESENCE_BOUNCECOUNT_INIT       // Bounce count.
};

//---------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*!
 \brief     return the siez (in byte) of the object
 \author    Martin Dahl
 \date      2006-11-23
 \param     subsystemPtr = pointer to subsystem
 \param     objectIndex = object index.
 \param     attributeIndex = attribute index.
 \warning
 \test
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 GetObjectSize(const T_UNIT SLOW *subsystemPtr,TUSIGN16 objectIndex, TINT16 attributeIndex)
{
    T_DO_OBJECT_DESCRIPTION objDescriptor;
    T_DO_ATTRIB_RANGE attrDescriptor;
    //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
    if (attributeIndex == WHOLE_OBJECT) 
    {
        (void)subsystemPtr->GetObjectDescription(subsystemPtr,objectIndex,&objDescriptor);
        return objDescriptor.objectLength;
    }
    else {
        //lint -e{732} Ashwin Herur R 2009-07-15 short to unsigned short is ok
        (void)subsystemPtr->GetAttributeDescription(subsystemPtr,objectIndex,attributeIndex,&attrDescriptor);
        return cgSimpleTypeLength[attrDescriptor.dataType];
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform Service Port conversion on the buffer
 \author    Martin Dahl
 \date      2006-11-24
 \param     size = size of the HART object
 \param     ptrValue = ptr to buffer
 \param     index = position of the hart object inside the buffer,
            i.e. object = ptrValue[index .. index+size]
 \warning
 \test
 test-date: 
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void ConvertBuffer(TUSIGN8 size,TUSIGN8 FAST* ptrValue,TUSIGN8 index)
{
    #define SWAP(x,y) { x = x^y; y = x^y; x = x^y; }

    VIP_ASSERT(ptrValue);
	
    switch (size)
    {
    // two byte types
    case 2:
        // convert if not out of bounds of transmit/receive buffer
        SWAP(ptrValue[index+0],ptrValue[index+1]);
        break;

    // four byte types
    case 4:
        SWAP(ptrValue[index+0],ptrValue[index+3]);
        SWAP(ptrValue[index+1],ptrValue[index+2]);
        break;
    //lint -e{744} Ashwin Herur R 2009-07-15 no need of default
    }
    #undef SWAP
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform Service Port conversion for a complete T_DATA_OBJECT
 \author    Martin Dahl
 \date      2006-11-24
 \param     subsystemPtr = pointer to subsystem
 \param     objIdx = object index.
 \param     attribIdx = attribute index.
 \param     ptrValue = pointer to object value.
 \warning
 \test
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed 
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void ServicePortConvert(const T_UNIT SLOW *subsystemPtr,
                        TUSIGN16 objectIndex,
                        TINT16 attributeIndex,
                        void FAST* ptrValue)
{
    T_DO_OBJECT_DESCRIPTION objDescriptor;
    T_DO_ATTRIB_RANGE attrDescriptor;
    TUSIGN8 i,size,index;
    //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
    if (attributeIndex == WHOLE_OBJECT) 
    {
        (void)subsystemPtr->GetObjectDescription(subsystemPtr,objectIndex,&objDescriptor);
        if (objDescriptor.typeQualifier == STRUCT) {
            // STRUCT 
            index = 0;
            for (i=0; i < objDescriptor.numberOfAttributes; i++) {
                size = cgSimpleTypeLength[objDescriptor.aryAttribDataTypes[i]];
                ConvertBuffer(size,ptrValue,index);
                index += size;
            }
        }
        else {
            // ARRAY or SIMPLE
            index = 0;
            size = cgSimpleTypeLength[objDescriptor.aryAttribDataTypes[0]];
            for (i=0; i < (objDescriptor.objectLength/size); i++) {
                ConvertBuffer(size,ptrValue,index);
                index += size;
            }
        }
    }
    else {
        //lint -e{732} Ashwin Herur R 2009-07-15 short to unsigned short is ok
        (void)subsystemPtr->GetAttributeDescription(subsystemPtr,objectIndex,attributeIndex,&attrDescriptor);
        size = cgSimpleTypeLength[attrDescriptor.dataType];
        ConvertBuffer(size,ptrValue,0);
    }
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief     ServicePort command interpreter, requested by layer2.
 \author    Ganapathi
 \date      2008-01-21
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: C-SPY, see file �Test_layer7.c?of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------

void ServPortCmdInterpreter_LAYER7(void)
{

    TUSIGN16 cmdNumber = 0;
    TUSIGN8 count;
    TBOOL cmdFound;
    
    // Check if transmit request is pending.    
    if ( TxRequestPending() )
    {
        if(SPRxTime<12)
        {
            SPRxTime++;
        }
        //This is to confirm not respond to the message imidiatly
        if(SPRxTime<2)
        {
            return;
        }
        // in general handle response as a reflection of the request
        // Copy Receive buffer to transmit buffer.
        ServPortTxBuffer = ServPortRxBuffer;
    
        if( ServPortRxBuffer.firstResp != SP_RESP_OK)
        {
            cnt_RxCheckSumErr++;
            // Error , only first and second response sent.
            ServPortTxBuffer.numByte = 2;
            ServPortTxBuffer.aryData[0] = SP_CHECKSUM_ERROR;
            ServPortTxBuffer.aryData[1] = cnt_RxCheckSumErr;
        }
        else        
        { 
            // get command number
            cmdNumber = (TUSIGN16)ServPortRxBuffer.command;
             
            // Start parsing the command table.
            for(count = 0; count < lenOfcmdTbl; count++)
            {
                // Reset the command found flag.
                cmdFound = eFALSE;
                // Parse the command table.
                if(cmdTbl[count].cmdTyp == cmdNumber)
                {
                     
                    // Check if the received length is correct.
                    if(ServPortRxBuffer.numByte < cmdTbl[count].reqLen)
                    {
                        ServPortTxBuffer.firstResp = SP_RESP_ERR_TOO_FEW_DATA;
                        // Set the response length.
                        ServPortTxBuffer.numByte = 2; // Only 2 bytes, first and second response.
                    }
                    else
                    {   
                        if(cmdTbl[count].funcPtr() == (TUSIGN16)OK) 
                        {
                          if((cmdNumber == UARTI_IDENT_READ)
                            || (cmdNumber == UARTI_IDENT_WRITE)
                            || (cmdNumber == UARTI_IDENT_READ_MEMORY)
                            || (cmdNumber == UARTI_IDENT_WRITE_MEMORY))
                            {
                            // Set the response length.
                            ServPortTxBuffer.numByte = cmdTbl[count].respLen + 2; // 2, include first reponse and secResponse bytes.
                            }
                          else
                          {
                            if((ServPortTxBuffer.numByte==0)
                                && (SPRxTime>=12))
                            {//the response is not prepared at current time
                                cnt_noResponse_layer7++;
                                if(cmdNumber==UARTI_IDENT_DISPLAY_KEY_INFO)
                                {
                                    ServPortTxBuffer.aryData[0] = 0x85;
                                    ServPortTxBuffer.numByte = 1;
                                }else if(cmdNumber==UARTI_IDENT_DISPLAY_FRAME)
                                {
                                    ServPortTxBuffer.command = UARTI_IDENT_DISPLAY_KEY_INFO;
                                    ServPortTxBuffer.aryData[0] = 0x85;
                                    ServPortTxBuffer.numByte = 1;
                                }else
                                {                                       
                                    // Error , only first and second response sent.
                                    ServPortTxBuffer.command = UARTI_IDENT_COMMAND;
                                    ServPortTxBuffer.numByte = 2;
                                    ServPortTxBuffer.aryData[0] = SP_RESP_ERR_DEVICE_BUSY;
                                    ServPortTxBuffer.aryData[1] = cnt_noResponse_layer7;
                                }
                            }
                          }
                        }
                        else
                        {
                            // Set first response.
                            ServPortTxBuffer.firstResp = SP_RESP_WARN_INVALID_SELECTION;
                            // Set the response length.
                            ServPortTxBuffer.numByte = 2;
                        }
                    }
                    
                    // Command found.
                    cmdFound = eTRUE;
                    // No need to loop further, end the loop.
                    count = lenOfcmdTbl;   
                    
                }// if(cmdTbl[count].cmdTyp == cmdNumber)
                
            }//for(count = 0; count < lenOfcmdTbl; count++) 
            
            if(cmdFound == eFALSE)
            {
                // Command was not found.
                ServPortTxBuffer.firstResp = SP_RESP_ERR_CMD_NOT_IMPLEMENTED; 
                // Set the response length.
                ServPortTxBuffer.numByte = 2; // Only 2 bytes, first and second response.
            }
                
        } // if( SpReceiveBuff_LAYER2->firstResp != SP_RESP_OK)
        
        ServPortTxBuffer.secResp = 0;
        if(ServPortTxBuffer.numByte>0)
        {            
            StartSpResponse_LAYER2();
        }
    } // if( TxRequestPending() )
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Service Port command implementation
 \author    Ganapathi 
 \date      2008-01-21
 \param
 \return    
            Error Codes.
 \warning
 \test
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 spReadObj(void)
{
	const T_UNIT SLOW   *pSub;          // Pointer to Subsystem
	TUSIGN8 bufIdx = 0;
	TUSIGN16 returnCode = (TUSIGN16)OK;
        TUSIGN8 ServPortObjectDataBuffer[SP_OBJECT_DATA_BUFFER_LEN] = {0};

	board = ServPortRxBuffer.aryData[bufIdx++];
	feAddress = ServPortRxBuffer.aryData[bufIdx++];
	subsystemIdx.aryU8[1] = ServPortRxBuffer.aryData[bufIdx++];
	subsystemIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
	objectIdx.aryU8[1] =  ServPortRxBuffer.aryData[bufIdx++];
	objectIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
	atrbtIdx.aryU8[1] = ServPortRxBuffer.aryData[bufIdx++];
	atrbtIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];

	switch(board)
	{
            case SERVICEPORT_CB_BOARD:
            {
                pSub = GetSubsystemPtr(subsystemIdx.u16);
                //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
                if(pSub == NULL) 
                {
                    returnCode = RULE_VIOLATION_ERR;
                }
                else
                {
                    //lint -e{734} Ashwin Herur R 2009-07-15 enough precision
                    TUSIGN16 dataLength = GetObjectSize(pSub,objectIdx.u16,atrbtIdx.s16);

                    ServPortTxBuffer.aryData[bufIdx] = (TUSIGN8)dataLength;

                    if(dataLength <= 32) 
                    {
                        //returnCode = pSub->Get(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortTxBuffer.aryData[bufIdx+1]);
                        //ServicePortConvert(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortTxBuffer.aryData[bufIdx+1]);
                        returnCode = pSub->Get(pSub, objectIdx.u16, atrbtIdx.s16, &ServPortObjectDataBuffer[0]);
                        
                        (void)memcpy((void*)&ServPortTxBuffer.aryData[bufIdx+1], (void*)&ServPortObjectDataBuffer[0],
                             SP_OBJECT_DATA_BUFFER_LEN);
                         
                        ServicePortConvert(pSub, objectIdx.u16, atrbtIdx.s16, &ServPortTxBuffer.aryData[bufIdx+1]);
                    }
                    //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
                    else 
                    {
                        returnCode = RULE_VIOLATION_ERR;
                    }
                 }
                  break;
              }


		case SERVICEPORT_FE_BOARD:
		{
#ifdef IIWIRE_FE
			ServPortTxBuffer.aryData[bufIdx] = 0;
			//lint -e{78,10} Ashwin Herur R 2009-07-15 ok
                        //lint -e{522} Ashwin Herur R 2009-07-15 return value is assigned to burstModeActive
			TUSIGN8 burstModeActive = sensor_Interface_GetBurstModeStateSRV;
                        //lint -e{40} Ashwin Herur R 2009-07-15 burstModeActive defined
			if(burstModeActive)
			{
				//lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 2)
				returnCode = sensor_Interface_StopBurstModeSRV(feAddress,NULL);
			}

			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			//lint -e{644} Ashwin Herur R 2009-07-15 returnCode is initialized
			if(returnCode == OK)
			{
				TUSIGN8 dataReceived[64];
				//lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 7)
				returnCode = sensor_Interface_GetSensorConfigSRV(feAddress,subsystemIdx.u16,objectIdx.u16,atrbtIdx.s16,&dataReceived[0],&ServPortTxBuffer.aryData[bufIdx],NULL);
				//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
				if(returnCode == OK)
				{
					ServPortTxBuffer.aryData[bufIdx] -= 6;
					
					if(ServPortTxBuffer.aryData[bufIdx] > 32 || ServPortTxBuffer.aryData[bufIdx]<0)
					{
                                                //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
						returnCode = RULE_VIOLATION_ERR;
					}
					else
					{
						(void)memset(&ServPortTxBuffer.aryData[bufIdx+1],0,32);
						(void)memcpy(&ServPortTxBuffer.aryData[bufIdx+1],&dataReceived[6],ServPortTxBuffer.aryData[bufIdx]);
					}
				}
			}

			//lint -e{40} Ashwin Herur R 2009-07-15 burstModeActive defined
			if(burstModeActive)
			{
				//lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 2)
				(void)sensor_Interface_StartBurstModeSRV(feAddress,NULL);
			}
#endif			
			break;
		}

		default:
                     //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = (TUSIGN16)RULE_VIOLATION_ERR;
			break;
	}

	return (returnCode);
}

/*
Description: this function is for smart HMI special usage.
             it is used to all the objects wanted.
*/

static TUSIGN16 spReadObj_SHMI(void)
{
    const T_UNIT SLOW   *pSub;          // Pointer to Subsystem
    TUSIGN8 bufIdx = 0;
    TUSIGN16 returnCode = (TUSIGN16)OK;

    board = ServPortRxBuffer.aryData[bufIdx++];
    feAddress = ServPortRxBuffer.aryData[bufIdx++];
    subsystemIdx.aryU8[1] = 0;
    subsystemIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
    objectIdx.aryU8[1] =  ServPortRxBuffer.aryData[bufIdx++];
    objectIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
    atrbtIdx.aryU8[1] = 0xFF;
    atrbtIdx.aryU8[0] = 0xFF;

    ServPortTxBuffer.command |= 0x80;

    if(subsystemIdx.u16==0xFF)
    {//indicate special read
        if(objectIdx.u16==0)
        {//command 0
            returnCode = diagnosis.GetNAMURStatusSRV((T_DIAGNOSIS_STATUS*)ServPortTxBuffer.aryData);
            
            ServPortTxBuffer.numByte = 6;
        }
        else
        {
            return RULE_VIOLATION_ERR;
        }

        return returnCode;
    }

    switch(board)
    {
        case SERVICEPORT_CB_BOARD:
        {
            pSub = GetSubsystemPtr(subsystemIdx.u16);
            //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
            if(pSub == NULL)
            {
                returnCode = RULE_VIOLATION_ERR;
            }
            else
            {
                //lint -e{734} Ashwin Herur R 2009-07-15 enough precision
                TUSIGN16 dataLength = GetObjectSize(pSub,objectIdx.u16,atrbtIdx.s16);

                

                if(dataLength <= 120)
                {
                    ServPortTxBuffer.aryData[0] = (TUSIGN8)dataLength;
                    ServPortTxBuffer.numByte = dataLength+1;
                    returnCode = pSub->Get(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortTxBuffer.aryData[1]);

                    ServicePortConvert(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortTxBuffer.aryData[1]);
                }
                //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
                else
                {
                    returnCode = RULE_VIOLATION_ERR;
                }
            }
            break;
        }
        default:
            //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
            returnCode = (TUSIGN16)RULE_VIOLATION_ERR;
            break;
    }

    return (returnCode);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Service Port command implementation
 \author    Ganapathi 
 \date      2008-01-21
 \param
 \return    
            Error Codes.
 \warning
 \test
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 spWriteObj(void)
{
	const T_UNIT SLOW   *pSub;          // Pointer to Subsystem
	TUSIGN8 bufIdx = 0;
	TUSIGN16 returnCode = (TUSIGN16)OK;
        TUSIGN8 ServPortObjectDataBuffer[SP_OBJECT_DATA_BUFFER_LEN] = {0};

	board = ServPortRxBuffer.aryData[bufIdx++];
	feAddress = ServPortRxBuffer.aryData[bufIdx++];
	subsystemIdx.aryU8[1] =   ServPortRxBuffer.aryData[bufIdx++];
	subsystemIdx.aryU8[0] =   ServPortRxBuffer.aryData[bufIdx++];
	objectIdx.aryU8[1] =  ServPortRxBuffer.aryData[bufIdx++];
	objectIdx.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
	atrbtIdx.aryU8[1] =   ServPortRxBuffer.aryData[bufIdx++];
	atrbtIdx.aryU8[0] =   ServPortRxBuffer.aryData[bufIdx++];

	switch(board)
	{
            case SERVICEPORT_CB_BOARD:
            {
                pSub = GetSubsystemPtr(subsystemIdx.u16);
                //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
                if(pSub == NULL)
                {
                    returnCode = RULE_VIOLATION_ERR;
                }
                else
                {
                    TUSIGN16 dataLength = GetObjectSize(pSub,objectIdx.u16,atrbtIdx.s16);

                    ServPortRxBuffer.aryData[bufIdx] = dataLength;
                    
                    //lint -e{734} Ashwin Herur R 2009-07-15 enough precision(assignment)
                    if(dataLength <= 32) 
                    {
                        //ServicePortConvert(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortRxBuffer.aryData[bufIdx+1]);     
                        //returnCode = pSub->Put(pSub,objectIdx.u16,atrbtIdx.s16,&ServPortRxBuffer.aryData[bufIdx+1]);
                        (void)memcpy((void*)&ServPortObjectDataBuffer[0], (void*)&ServPortTxBuffer.aryData[bufIdx+1],
                        SP_OBJECT_DATA_BUFFER_LEN);
                        
                        ServicePortConvert(pSub, objectIdx.u16, atrbtIdx.s16, &ServPortObjectDataBuffer[0]);     
                        
                        returnCode = pSub->Put(pSub, objectIdx.u16, atrbtIdx.s16, &ServPortObjectDataBuffer[0]);
                    }
                    //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
                    else 
                    {
                        returnCode = (TUSIGN16)RULE_VIOLATION_ERR;
                    }
                }
                break;
            }


		case SERVICEPORT_FE_BOARD:
		{
#ifdef IIWIRE_FE
			TUSIGN8 burstModeActive = sensor_Interface_GetBurstModeStateSRV;

			if(burstModeActive)
			{
                                //lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 2)
				returnCode = sensor_Interface_StopBurstModeSRV(feAddress,NULL);
			}
                        //lint -e{644} Ashwin Herur R 2009-07-15 returnCode is initialized
                        //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			if(returnCode == OK)
			{
                                //lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 9)
				returnCode = sensor_Interface_PutSensorConfigSRV(feAddress,subsystemIdx.u16,objectIdx.u16,atrbtIdx.s16,&ServPortRxBuffer.aryData[bufIdx+1],ServPortRxBuffer.aryData[bufIdx] ,NULL,NULL,NULL);
			}


			if(burstModeActive)
			{
                                //lint -e{619} Ashwin Herur R 2009-07-15 enough precision(arg 2)
				(void)sensor_Interface_StartBurstModeSRV(feAddress,NULL);
			}
#endif			
			break;
		}

		default:
                        //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = RULE_VIOLATION_ERR;
			break;
	}

	return (returnCode);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Service Port command implementation
 \author    Ganapathi 
 \date      2008-01-21
 \param
 \return    
            Error Codes.
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: C-SPY, see file �Test_layer7.c?of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 spReadMem(void)
{
    	TUSIGN8 bufIdx = 0;
        //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
	TUSIGN16 returnCode = OK;

	board = ServPortRxBuffer.aryData[bufIdx++];
	feAddress = ServPortRxBuffer.aryData[bufIdx++];
	memAdrs.aryU8[1] = ServPortRxBuffer.aryData[bufIdx++];
	memAdrs.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
	memLen =  ServPortRxBuffer.aryData[bufIdx++];

	switch(board)
	{
		case SERVICEPORT_CB_BOARD:
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			if (memLen > 32) 
            {
                return RULE_VIOLATION_ERR;
            }
            else
            {}

			// clear ptrValue first
			(void)memset(&ServPortTxBuffer.aryData[bufIdx],0,32);

			// get mem
			(void)memcpy(&ServPortTxBuffer.aryData[bufIdx],(TUSIGN8 FAST*)memAdrs.u16,memLen);
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = OK;
			break;


		//TODO
		case SERVICEPORT_FE_BOARD:

		default:
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = RULE_VIOLATION_ERR;
			break;
	}
		
    	return returnCode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Service Port command implementation, Command 245.
 \author    Ganapathi 
 \date      2008-01-21
 \param
 \return    
            Error Codes.
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: C-SPY, see file �Test_layer7.c?of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 spWriteMem(void)
{
	TUSIGN8 bufIdx = 0;
        //lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
	TUSIGN16 returnCode = OK;

	board = ServPortRxBuffer.aryData[bufIdx++];
	feAddress = ServPortRxBuffer.aryData[bufIdx++];
	memAdrs.aryU8[1] = ServPortRxBuffer.aryData[bufIdx++];
	memAdrs.aryU8[0] = ServPortRxBuffer.aryData[bufIdx++];
	memLen =  ServPortRxBuffer.aryData[bufIdx++];

	switch(board)
	{
		case SERVICEPORT_CB_BOARD:
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			if (memLen > 32) 
            {
                return RULE_VIOLATION_ERR;
            }
            else{}

			(void)memcpy((TUSIGN8 FAST*)memAdrs.u16,&ServPortRxBuffer.aryData[bufIdx],memLen);
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = OK;
			break;

		//TODO
		case SERVICEPORT_FE_BOARD:
			VIP_ASSERT(feAddress);
		default:
			//lint -e{641} Ashwin Herur R 2009-07-15 converting enum to int accepted
			returnCode = RULE_VIOLATION_ERR;
			break;
	}


	//(void)memcpy((void *)(memAdrs.u16), &ServPortRxBuffer.aryData[bufIdx], memLen);
	
	return returnCode;
}

static TUSIGN16 spDisplayKeyInfo(void)
{ 
    if(LOCAL_packetBufferUsed > 0)
    {
        
        if(LOCAL_packetBufferUsed < SP_XMIT_BUFFER_LEN)
        {
            ServPortTxBuffer.command = UARTI_IDENT_DISPLAY_FRAME|0x80;        
            if(LOCAL_packetBuffer[0] == 0x85)
            {//key request packet will not be sent with SP communication mode.
                TUSIGN16 BLESwitch, LCDLightMode;   
                //Enable power for LEDs
                TUSIGN8 powerControl = SWITCH_BLU_LED | SWITCH_GREEN_LED | 
                                       SWITCH_RED_LED | SWITCH_YELLOW_LED;
                  
                if(SHMIConfigSendCount > 0)
                {
#ifdef SERVICE_PORT_MODULE_TEST
                    BLESwitch = 1;
                    LCDLightMode = 0;
#else
                    elettronicServices_Get(ELETTRONICSERVICES_IDX_switchBLE, WHOLE_OBJECT, &BLESwitch);                
                    elettronicServices_Get(ELETTRONICSERVICES_IDX_modeLCDLight, WHOLE_OBJECT, &LCDLightMode); 
#endif
                    if(BLESwitch)
                    {
                        powerControl |= SWITCH_BLE_COMM;
                    }
                    
                    if(LCDLightMode==ELETTRONICSERVICES_LIGHT_ON)
                    {
                        powerControl |= (SWITCH_LCD|SWITCH_LCD_LIGHT);
                    }
                    else if(LCDLightMode==ELETTRONICSERVICES_LIGHT_DYNAMIC)
                    {
                        powerControl |= (SWITCH_LCD|SWITCH_LCD_LIGHT|SWITCH_LCD_LIGHT_CONTROL);
                    }
                    else
                    {
                        powerControl |= SWITCH_LCD;
                    }
                    
                    ServPortTxBuffer.numByte = 2;
                    ServPortTxBuffer.aryData[0] = 0x85;
                    ServPortTxBuffer.aryData[1] = powerControl;
                    SHMIConfigSendCount--;     
                    
                }
                else
                {
                    ServPortTxBuffer.numByte = 1;
                    ServPortTxBuffer.aryData[0] = 0x85;  
                }                          
            }  
            else
            {     
                ServPortTxBuffer.numByte = LOCAL_packetBufferUsed; 
                memcpy(ServPortTxBuffer.aryData, LOCAL_packetBuffer, LOCAL_packetBufferUsed);            
            }
        }

        LOCAL_packetBufferUsed = 0;        
        
    }  
    else
    { 
        ServPortTxBuffer.numByte = 0;  
    }
    
    
    return OK;
    
}


static TUSIGN16 spDisplayInitialization(void)
{
    //TUSIGN16 i;
    
    SHMIConfigSendCount=3;     //to update the configuration
    
    if((LOCAL_packetBufferUsed == 0) || (LOCAL_packetBufferPreparing))
    {        
        ServPortTxBuffer.numByte = 0;        
    }    
    else if(LOCAL_packetBufferUsed>0
        && (LOCAL_packetBufferUsed<=SP_XMIT_BUFFER_LEN))
    {
        memcpy(&ServPortTxBuffer.aryData[0], LOCAL_packetBuffer, LOCAL_packetBufferUsed);  
        
        ServPortTxBuffer.numByte = LOCAL_packetBufferUsed;  
        ServPortTxBuffer.command = UARTI_IDENT_DISPLAY_FRAME|0x80;
        
        LOCAL_packetBufferUsed = 0;
    }  
    else
    {
        LOCAL_packetBufferUsed = 0;
        ServPortTxBuffer.numByte = 0;    
        return WARNING;
    }

    return OK;
    
}

static TUSIGN16 spHartCMD(void)
{
    TUSIGN16 returnCode = OK;
 
    if(HART_SPbufferState==SP_BUFFER_STATE_WAIT_SEND)
    {   
        TUSIGN8 chkSum;
        TUSIGN16 u;
        TUSIGN8 len;
        TUSIGN8* pData;
        HART_SPbufferState = SP_BUFFER_STATE_FREE;
        
        ServPortTxBuffer.command = UARTI_IDENT_COMMAND|0x80;        
        ServPortTxBuffer.aryData[0] = HART_SPtransmitBuffer.delimiter|0x04;
        if(HART_SPtransmitBuffer.delimiter&0x80)
        {
            ServPortTxBuffer.aryData[1] = HART_SPtransmitBuffer.aryAdr[0];
            ServPortTxBuffer.aryData[2] = HART_SPtransmitBuffer.aryAdr[1];
            ServPortTxBuffer.aryData[3] = HART_SPtransmitBuffer.aryAdr[2];
            ServPortTxBuffer.aryData[4] = HART_SPtransmitBuffer.aryAdr[3];
            ServPortTxBuffer.aryData[5] = HART_SPtransmitBuffer.aryAdr[4];
            pData = &ServPortTxBuffer.aryData[6];
            len = 6;
        }
        else
        {            
            ServPortTxBuffer.aryData[1] = HART_SPtransmitBuffer.aryAdr[0];
            pData = &ServPortTxBuffer.aryData[2];
            len = 2;
        }
        
        *pData++ = HART_SPtransmitBuffer.command;
        *pData++ = HART_SPtransmitBuffer.numByte;         //include the response data
        *pData++ = HART_SPtransmitBuffer.firstResp; 
        *pData++ = HART_SPtransmitBuffer.secResp; 
        if(HART_SPtransmitBuffer.numByte > 2)
        {
            memcpy(pData, &HART_SPtransmitBuffer.aryData[0], HART_SPtransmitBuffer.numByte-2);
        }  
        else if(HART_SPtransmitBuffer.numByte < 2)
        {
            return RULE_VIOLATION;
        }
        len += (2+HART_SPtransmitBuffer.numByte);        

        //start checksum generating with the delimiter
        chkSum = ServPortTxBuffer.aryData[0];
        for( u=1; u<len; u++)
        {
            chkSum ^=ServPortTxBuffer.aryData[u];
        }     
        HART_SPtransmitBuffer.checksum = chkSum;   
        ServPortTxBuffer.aryData[len] = chkSum;
        ServPortTxBuffer.numByte = len+1;  
        
    }
    else
    {
        
        ServPortTxBuffer.numByte = 0;
    }
    
    return returnCode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Updates status of HMI presence pin. 
 \author    Ganapathi 
 \date      2008-01-21
 \param
 \return    
 \warning
 \test
 test-date: 
 \n date 05-11-2009
 \n by: Roberto Colombo
 \n intention: Code Coverage
 \n result module test: OK
 \n result Lint Level 3:  free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void updateHMIpresencePinStatus_LAYER7(void)
{
  // Read the HMI presence pin.
    static TUSIGN8 portStatus = PORT_UN_KNOWN;

    hmiPresencePin.currPinState = HMI_PRESENCE_STATE;

    if(hmiPresencePin.firstTimeCheck)
    {
        // We are checking the pin status for first time soon after power up.
        // Make sure UART1 is initialised immediately with out pin-bounce time.
        hmiPresencePin.firstTimeCheck = eFALSE;
        hmiPresencePin.bounceCount = SP_HMI_PRESENCE_BOUNCECOUNT_INIT; 
        // 1 because, to satisfy the elseif() condition (see below).
        hmiPresencePin.prevPinState = !hmiPresencePin.currPinState;
        isSmartHMIConnected = eFALSE;
    }
    if( hmiPresencePin.currPinState != hmiPresencePin.prevPinState)
    {
        // Allow some time to avoid "Pin Bouncing (or Disturbance)" while connecting or disconnecting the HMI.
        // Reset the bounce Count.
        hmiPresencePin.bounceCount = SP_HMI_PRESENCE_BOUNCECOUNT_REDETECT;
        hmiPresencePin.prevPinState = hmiPresencePin.currPinState;
        isSmartHMIConnected = eFALSE;
    }
    else if(hmiPresencePin.bounceCount > SP_HMI_PRESENCE_BOUNCECOUNT_INIT)
    {
        // Decrement the bounce count.
        hmiPresencePin.bounceCount--;
        
        if(hmiPresencePin.bounceCount == SP_HMI_PRESENCE_BOUNCECOUNT_INIT)
        {
            // Its time to change the UART1 configuration to HMI/ Service Port settings.
            if( (hmiPresencePin.currPinState == SP_HMI_PRESENCE_PINSTATUS_CONNECT) 
               && (isSmartHMIConnected) 
               && (portStatus != PORT_FOR_SHMI) )
            {//SmartHMI
                servicePortDynamic.activationStatus = SERVICEPORT_SHMI_STATUS;
                InitializeSmartHmi_LAYER2();
                portStatus = PORT_FOR_SHMI;
            }
            else if( (hmiPresencePin.currPinState == SP_HMI_PRESENCE_PINSTATUS_CONNECT) 
                    && (!isSmartHMIConnected) 
                    && (portStatus != PORT_FOR_HMI))
            {//HMI
                // Set activation status flag-> service port disabled.
                servicePortDynamic.activationStatus = SERVICEPORT_HMI_STATUS;
                 // Init Calcucation is required for an hmi hot plug out/in (ResetPacketManager variables)
                InitializeHMI_LAYER2();
                portStatus = PORT_FOR_HMI;
            }
            else if((hmiPresencePin.currPinState == SP_HMI_PRESENCE_PINSTATUS_DISCONNECT) 
                    && (!isSmartHMIConnected)
                    && (PORT_FOR_SP != portStatus))
            {//Service
                
                // Reset activation status flag-> Service port enabled.
                servicePortDynamic.activationStatus = SERVICEPORT_SERVICE_STATUS;
                // Re-Intialize the UART1 with SP settings.
                InitializeSp_LAYER2();
                portStatus = PORT_FOR_SP;
             
            }
            else
            {}
        }
        else
        {}
    }
    else
    {}
} 
  
//-----------------------------------------------------------------------------------------------
