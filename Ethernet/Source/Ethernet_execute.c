//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
//#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Hmi/Source/compileTimeDefinitions.h"
#include "Hmi/Source/validityTest.h"
#include "Ethernet/Interface/Ethernet.h"
#include "Ethernet/Interface/Ethernet_idx.h"
// #include "..\..\Diagnosis/AuditLog/Interface/auditlog.h"
#include "ethernet_types.h"
#include "ethernet_attributes.h"
#include "ethernet_execute.h"
//#include "..\..\Services\SoftwareUpdate\Interface\SU_Interface.h"
//#include "CI_codes.h"
#include "CI_system_api.h"
#include <string.h>


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Convert protocol Id to text
 \author    Michal Kochel
 \date      2020-09-07
 \param     protocolId Protocol Id
 \return    TCHAR* Protocol name as string
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
const TCHAR* MainProtocolIdToName(TUSIGN8 protocolId) {
    if (protocolId == ETH_PROFINETIO_PROTOCOL) {
        return (const TCHAR*)"PROFINET IO";
    }
    else {
        return (const TCHAR*)"EtherNet/IP";
    }
}

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER REFRESHHMIVALIDITYTESTRESULTSEXE> \endif
\brief  Update the object value of Update the object value of hmiValidityTestResults at cycle complete
 \if @SubGen end@ <METHODHEADER REFRESHHMIVALIDITYTESTRESULTSEXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD REFRESHHMIVALIDITYTESTRESULTSEXE>
void RefreshHmiValidityTestResultsEXE_ETH(void)
//@SubGen end@<METHOD REFRESHHMIVALIDITYTESTRESULTSEXE>
{
    TUSIGN16 testIdx;
    TUSIGN8 myU8;

    (void)ENABLE_DATA_PROTECTION( ethernetDataSemaphore );
    for( testIdx = 0; testIdx < LAST_IDF_VALID; testIdx++ )
    {
#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
		myU8 = ValidityTestCheck(
#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
		(FRAME_FORMAT_TYPE const*)NULL,
#endif
			testIdx
#ifdef VALIDITY_TEST_CALLER
			, SIGNALS_TEST_NOT_APPLICABLE
#endif
		);
#endif
        if( myU8 == 0)
        {   // 0=not valid, clear bit
            ethernetDynamic.hmiValidityTestResults[testIdx/8] &= ~(1u << (testIdx%8));
        } else
        {   // valid, set bit
            ethernetDynamic.hmiValidityTestResults[testIdx/8] |=  (1u << (testIdx%8));
        }
    }
    (void)DISABLE_DATA_PROTECTION( ethernetDataSemaphore );
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CONTROLFACTORYDEFAULTSEXE> \endif
 \brief  Prepares setup to control behaviour of next resetToFactory method
 \param  controlId 
 \if @SubGen end@ <METHODHEADER CONTROLFACTORYDEFAULTSEXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CONTROLFACTORYDEFAULTSEXE>
void ControlFactoryDefaultsEXE_ETH(TUSIGN8 controlId)
//@SubGen end@<METHOD CONTROLFACTORYDEFAULTSEXE>
{
    ethernet_ControlFactoryDefaults = controlId;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER VERIFYMAINPROTOCOLEXE> \endif
 \brief  Verifies main protocol between value from NV and CI
 \if @SubGen end@ <METHODHEADER VERIFYMAINPROTOCOLEXE> \endif
 \author Michal Kochel, Klaus Pose
 \date                  2021-01-15
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD VERIFYMAINPROTOCOLEXE>
void VerifyMainProtocolEXE_ETH(void)
//@SubGen end@<METHOD VERIFYMAINPROTOCOLEXE>
{
    CI_protocol_t activeCiProtocol = CI_GetProtocol();
    TUSIGN8 activeEnumProtocol;
//    TCHAR errorLogBuffer[ADI_SOFTWARE_UPGRADE_ERROR_LOG_MAX_SIZE];

    // Convert CI_protocol_t to enum
    switch (activeCiProtocol) {
        case CI_protocolEtherNetIP: activeEnumProtocol = ETH_ETHERNETIP_PROTOCOL; break;
        case CI_protocolProfinet: activeEnumProtocol = ETH_PROFINETIO_PROTOCOL; break;

        case CI_protocolError:
        case CI_protocolModbusTCP:
        case CI_protocolEtherCAT:
        case CI_protocolPowerlink:
        case CI_protocolIP:
        case CI_protocolLast:
        default:
            activeEnumProtocol = ETH_PROFINETIO_PROTOCOL; break;
    }

    // Update main protocol selector to always reflect current base protocol stack
    (void)Put_T_UNIT(ETHERNET_ME, ETH_IDX_selectedMainProtocol, 0, &activeEnumProtocol);
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SINKEVENTEXE> \endif
 \brief  
 \param  pEvent Pointer to the event structure
 \if @SubGen end@ <METHODHEADER SINKEVENTEXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SINKEVENTEXE>
void SinkEventEXE_ETH(const void* pEvent)
//@SubGen end@<METHOD SINKEVENTEXE>
{
    static uint32_t ipAddress = 0;
    CI_eventInfo_t eventInfo;
    int32_t result;
    uint32_t newIpAddress = 0;
    int32_t szIpAddress = sizeof(newIpAddress);

    if (*(ethernet.unit.ptrState) < INITIALIZED) {
        return;
    }

    eventInfo = *((const CI_eventInfo_t *)pEvent);
    //lint -e{788} enum constant xxx not used within defaulted switch is OK, because not all are needed
    switch (eventInfo.event) 
    {
    case CI_eventIpConfigChange: 
        if (eventInfo.info != 0) {
        
            result = CI_GetSystemParam(CI_sysParamIpAddress, &newIpAddress, &szIpAddress);
            if (result == (int32_t)sizeof(newIpAddress)) {
                if (ipAddress != newIpAddress) {
                    ipAddress = newIpAddress;
                }
            }
        }
        break;

    default:
        break;
    }
}


/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/

