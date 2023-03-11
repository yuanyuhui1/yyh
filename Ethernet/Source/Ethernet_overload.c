//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include <string.h>
//#include "system_dbug.h"
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_Unit/Interface/t_unit.h"
#include "Ethernet/Interface/Ethernet.h"
#include "Ethernet/Interface/Ethernet_idx.h"
#include "ethernet_types.h"
#include "ethernet_attributes.h"
#include "ethernet_overload.h"
#include "Modbus/Interface/Modbus.h"
#include "WebServer/Interface/WebServer.h"
#include "Diagnosis/Counters/Interface/counters.h"
#include "Ethernet_counters.h"
#include "Ethernet_execute.h"

//#include "..\..\Coordinator\Source\Coordinator_types.h"
//#include "..\..\Coordinator\Source\Coordinator_attributes.h"
//#include "..\..\Coordinator\Interface\Coordinator.h"
//#include "..\..\Services\SoftwareUpdate\Interface\SU_Interface.h"
//#include "..\..\Diagnosis/AuditLog/Interface/auditlog.h"

//#include "CI_items_if.h"
#include "CI_codes.h"
#include "CI_io_types.h"
//#include "CI_io_api.h"
#include "CI_devices.h"
#include "CI_system_api.h"

#define IDX_PHY_AUTO_NEGOTIATION    0
#define IDX_PHY_LINK_SPEED          1
#define IDX_PHY_DUPLEX_MODE         2

#define DEVICE_HOST_NAME_LENGTH 64
#define DEVICE_DOMAIN_NAME_LENGTH 64
#define MAC_ADDRESS_LENGTH      6
#define MAC_ADDRESS_NUMOF       3

// module static functions
static void mUpdateMacAddress(void);
static TUSIGN16 mUpdatePhySettingsFromCI(void);
static TBOOL mUpdatePhySettingsToCI( void );
static TUSIGN16 mUpdateIPsettingsFromCI(TUSIGN16 objectIndex, TINT16 attributeIndex);
static TBOOL mUpdateIPsettingsToCI( TUSIGN16 objectIndex, TINT16 attributeIndex );
static TUSIGN16 mFilter_NetworkName(TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Module-Private: Update MAC address objects
 \author    Michal Kochel (Mobica), Klaus Pose
 \date      2019-03-08            , 2019-11-22
 \return    void
 \warning 
 \test 
 \n test-date:          2019-06-28
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static void mUpdateMacAddress(void) 
{
    int32_t i32;
    uint8_t allMacs[MAC_ADDRESS_NUMOF][MAC_ADDRESS_LENGTH]; 
    const uint8_t emptyMac[MAC_ADDRESS_LENGTH] = {0,0,0,0,0,0};
    int32_t szParam;

    if( memcmp(ethernetDynamic.chasisMAC , (void *)emptyMac, MAC_ADDRESS_LENGTH) == 0 ||
        memcmp(ethernetDynamic.port1MAC , (void *)emptyMac, MAC_ADDRESS_LENGTH) == 0  ||
        memcmp(ethernetDynamic.port2MAC , (void *)emptyMac, MAC_ADDRESS_LENGTH) == 0  )
    {
        szParam = sizeof(allMacs);
        i32 = CI_GetSystemParam( CI_sysParamMacAddresses, allMacs, &szParam );
        if( (unsigned)i32 == sizeof(allMacs) )
        {
            (void)memcpy( ethernetDynamic.chasisMAC , &(allMacs[0][0]), MAC_ADDRESS_LENGTH );
            (void)memcpy( ethernetDynamic.port1MAC , &(allMacs[1][0]), MAC_ADDRESS_LENGTH );
            (void)memcpy( ethernetDynamic.port2MAC , &(allMacs[2][0]), MAC_ADDRESS_LENGTH );
        } else
        {
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Private: Get PHY Status using CI_GetSystemParam( CI_sysParamPortPhyState )
            The objects are represented by framework objects and will be Put'ted
 \author    Klaus Pose
 \date      2020-05-07
 \return    the or'ed result of all related Put methods
 \warning 
 \test 
 \n test-date:          2020-05-07
 \n by:                 Klaus Pose
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 mUpdatePhyStatusFromCI(void)
{
    int32_t adiCiResult;
    int32_t szParam;
    uint32_t portPhyState[2][3];        

    szParam = sizeof(portPhyState);
    //  select status enum (not parameter enum) for getting status
    adiCiResult = CI_GetSystemParam( CI_sysParamPortPhyState , portPhyState, &szParam);
    if( (unsigned)adiCiResult != sizeof(portPhyState))
    {
        return RULE_VIOLATION_ERR;
    }

    ethernetDynamic.portSpeedStatus[0] = 
        (portPhyState[0][IDX_PHY_LINK_SPEED] == 10 ? ETH_PORT_SPEED_10:ETH_PORT_SPEED_100);
    ethernetDynamic.portSpeedStatus[1] = 
        (portPhyState[1][IDX_PHY_LINK_SPEED] == 10 ? ETH_PORT_SPEED_10:ETH_PORT_SPEED_100);
    
    ethernetDynamic.portDuplexStatus[0] = 
        (portPhyState[0][IDX_PHY_DUPLEX_MODE] == CI_phyDuplexModeHalf ? ETH_HALF_DUPLEX:ETH_FULL_DUPLEX);
    ethernetDynamic.portDuplexStatus[1] = 
        (portPhyState[1][IDX_PHY_DUPLEX_MODE] == CI_phyDuplexModeHalf ? ETH_HALF_DUPLEX:ETH_FULL_DUPLEX);

    return OK;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Private: Get PHY Parameters using CI_GetSystemParam( CI_sysParamPortPhyStateNvm, ...)
            The parameters are represented by framework objects and will be Put'ted
 \author    Klaus Pose
 \date      2020-05-15
 \return    the or'ed result of all related Put methods
 \warning 
 \test 
 \n test-date:          2020-02-25
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 mUpdatePhySettingsFromCI(void)
{
    int32_t adiCiResult;
    int32_t szParam;
    TUSIGN16 result = OK;
    uint32_t portPhyState[2][3];        
    TUSIGN8 u8;

    szParam = sizeof(portPhyState);
    // select parameter enum (not status enum) for getting settings
    adiCiResult = CI_GetSystemParam( CI_sysParamPortPhyStateNvm, portPhyState, &szParam);
    if( (unsigned)adiCiResult != sizeof(portPhyState))
    {
        return RULE_VIOLATION_ERR;
    }

    if( portPhyState[0][IDX_PHY_AUTO_NEGOTIATION] == 0 ) {
        u8 = ETH_AUTO_NEGOTIATE_DISABLED;
    } else {
        u8 = ETH_AUTO_NEGOTIATE_ENABLED;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1AutoNegotiate, 0, &u8 );
    if( portPhyState[1][IDX_PHY_AUTO_NEGOTIATION] == 0 ) {
        u8 = ETH_AUTO_NEGOTIATE_DISABLED;
    } else {
        u8 = ETH_AUTO_NEGOTIATE_ENABLED;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2AutoNegotiate, 0, &u8 );
    //
    if( portPhyState[0][IDX_PHY_LINK_SPEED] == 10 ) {
        u8 = ETH_PORT_SPEED_10;
    } else {
        u8 = ETH_PORT_SPEED_100;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1Speed, 0, &u8 );
    if( portPhyState[1][IDX_PHY_LINK_SPEED] == 10 ) {
        u8 = ETH_PORT_SPEED_10;
    } else {
        u8 = ETH_PORT_SPEED_100;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2Speed, 0, &u8 );
    //
    if( portPhyState[0][IDX_PHY_DUPLEX_MODE] == CI_phyDuplexModeHalf ) {
        u8 = ETH_HALF_DUPLEX;
    } else {
        u8 = ETH_FULL_DUPLEX;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1DuplexMode, 0, &u8 );
    if( portPhyState[1][IDX_PHY_DUPLEX_MODE] == CI_phyDuplexModeHalf ) {
        u8 = ETH_HALF_DUPLEX;
    } else {
        u8 = ETH_FULL_DUPLEX;
    }
    result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2DuplexMode, 0, &u8 );
    
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Private: Change PHY Parameters using CI_SetSystemParam(CI_sysParamPortPhyState, ...)
            At the end it also uses CI_StoreSystemEthParam() to store them at CI's NV
            The parameters are represented by framework objects and will be read from shadow-RAM
 \author    Klaus Pose
 \date      2020-05-25
 \return    eTRUE, if successful
 \warning 
 \test 
 \n test-date:          2020-05-25
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TBOOL mUpdatePhySettingsToCI( void )
{
    int32_t adiCiResult;
    uint32_t portPhyState[2][3];        
    int32_t szParam;
    uint32_t portPhyState_actual[2][3];        

    szParam = sizeof(portPhyState_actual);
    // select parameter enum (not status enum) 
    adiCiResult = CI_GetSystemParam( CI_sysParamPortPhyStateNvm, portPhyState_actual, &szParam);
    if( (unsigned)adiCiResult != sizeof(portPhyState_actual))
    {
        return eFALSE;
    }

    if( ethernetStaticFrequentStaticDefault.port1AutoNegotiate == ETH_AUTO_NEGOTIATE_ENABLED ) {
        portPhyState[0][IDX_PHY_AUTO_NEGOTIATION] = 1;
    } else
    {
        portPhyState[0][IDX_PHY_AUTO_NEGOTIATION] = 0;
    }
    if( ethernetStaticFrequentStaticDefault.port2AutoNegotiate == ETH_AUTO_NEGOTIATE_ENABLED ) {
        portPhyState[1][IDX_PHY_AUTO_NEGOTIATION] = 1;
    } else
    {
        portPhyState[1][IDX_PHY_AUTO_NEGOTIATION] = 0;
    }
    //
    if( ethernetStaticFrequentStaticDefault.port1Speed == ETH_PORT_SPEED_10 ) {
        portPhyState[0][IDX_PHY_LINK_SPEED] = 10;
    } else
    {
        portPhyState[0][IDX_PHY_LINK_SPEED] = 100;
    }
    if( ethernetStaticFrequentStaticDefault.port2Speed == ETH_PORT_SPEED_10 ) {
        portPhyState[1][IDX_PHY_LINK_SPEED] = 10;
    } else
    {
        portPhyState[1][IDX_PHY_LINK_SPEED] = 100;
    }
    //
    if( ethernetStaticFrequentStaticDefault.port1DuplexMode == ETH_HALF_DUPLEX ) {
        portPhyState[0][IDX_PHY_DUPLEX_MODE] = CI_phyDuplexModeHalf;
    } else
    {
        portPhyState[0][IDX_PHY_DUPLEX_MODE] = CI_phyDuplexModeFull;
    }
    if( ethernetStaticFrequentStaticDefault.port2DuplexMode == ETH_HALF_DUPLEX ) {
        portPhyState[1][IDX_PHY_DUPLEX_MODE] = CI_phyDuplexModeHalf;
    } else
    {
        portPhyState[1][IDX_PHY_DUPLEX_MODE] = CI_phyDuplexModeFull;
    }

    // hierachical business rule for change
    if( (portPhyState_actual[0][IDX_PHY_AUTO_NEGOTIATION] != portPhyState[0][IDX_PHY_AUTO_NEGOTIATION]) || 
        (portPhyState_actual[1][IDX_PHY_AUTO_NEGOTIATION] != portPhyState[1][IDX_PHY_AUTO_NEGOTIATION]) ||
        (portPhyState[0][IDX_PHY_AUTO_NEGOTIATION] == 0 && 
            (portPhyState_actual[0][IDX_PHY_LINK_SPEED]  != portPhyState[0][IDX_PHY_LINK_SPEED] || 
             portPhyState_actual[0][IDX_PHY_DUPLEX_MODE] != portPhyState[0][IDX_PHY_DUPLEX_MODE] )    ) ||
        (portPhyState[1][IDX_PHY_AUTO_NEGOTIATION] == 0 &&
            (portPhyState_actual[1][IDX_PHY_LINK_SPEED]  != portPhyState[1][IDX_PHY_LINK_SPEED] || 
             portPhyState_actual[1][IDX_PHY_DUPLEX_MODE] != portPhyState[1][IDX_PHY_DUPLEX_MODE] )    )  )
    {
        // select parameter enum (not status enum) 
        adiCiResult = CI_SetSystemParam(CI_sysParamPortPhyState, portPhyState, sizeof(portPhyState));
        if( adiCiResult == CI_OK )
        {
            adiCiResult = CI_StoreSystemEthParam();
            if( adiCiResult != CI_OK )
            {
                return eFALSE;
            }
        } else
        {
            return eFALSE;
        }
    } // else no relevant change
    return eTRUE;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Private: Get IP Parameters using CI_GetSystemParam( CI_sysParam..., ...)
            The parameters are represented by framework objects and will be Put'ted
 \author    Klaus Pose
 \date      2020-05-25
 \return    the or'ed result of all related Put methods
 \warning 
 \test 
 \n test-date:          2020-05-25
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 mUpdateIPsettingsFromCI(TUSIGN16 objectIndex, TINT16 attributeIndex)
{
    TUSIGN16 result = OK;
    int32_t adiCiResult;
    int32_t szParam = 0;
    TUSIGN8 u8array[65];
    TUSIGN8 u8;
    TUSIGN32 u32;

    switch (objectIndex) 
    {
    case ETH_IDX_useDhcp:
        szParam = sizeof(TUSIGN8);
        adiCiResult = CI_GetSystemParam(CI_sysParamDhcpEnable, &(u8array[0]), &szParam);
        if ((unsigned)adiCiResult == sizeof(TUSIGN8)) {
            u8 = (u8array[0] == 0 ? ETH_DHCP_STATE_DISABLED : ETH_DHCP_STATE_ENABLED);
            if( u8 != ethernetStaticFrequentStaticDefault.useDhcp ) {
                result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, &u8);
            }
        }
        else {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_hostAddress: 
        szParam = sizeof(TUSIGN32);
        adiCiResult = CI_GetSystemParam(CI_sysParamIpAddress, &u32, &szParam);
        // link-down fix: always valid if DHCP is disabled
        if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED)) {
            if( u32 != ethernetStaticFrequentStaticDefault.hostAddress ) {
                result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, &u32);
            }
        } else {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_subnetMask:
        szParam = sizeof(TUSIGN32);
        adiCiResult = CI_GetSystemParam(CI_sysParamSubnetMask, &u32, &szParam);
        // link-down fix: always valid if DHCP is disabled
        if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED)) {
            if( u32 != ethernetStaticFrequentStaticDefault.subnetMask ) {
                result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, &u32);
            }
        } else {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_gatewayAddress:
        szParam = sizeof(TUSIGN32);
        adiCiResult = CI_GetSystemParam(CI_sysParamDefaultGateway, &u32, &szParam);
        // link-down fix: always valid if DHCP is disabled
        if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED)) {
            if(u32 != ethernetStaticFrequentStaticDefault.gatewayAddress) {
                result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, &u32);            
            }
        } else {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_dnsAddress:
        szParam = sizeof(TUSIGN32);
        if( attributeIndex == 0 )
        {
            adiCiResult = CI_GetSystemParam(CI_sysParamPrimaryDns, &u32, &szParam);
            // link-down fix: always valid if DHCP is disabled
            if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED))
            {
                if( u32 != ethernetStaticFrequentStaticDefault.dnsAddress[0] )
                {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, 0, &u32);
                }
            }
            else
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if( attributeIndex == 1 )
        {
            adiCiResult = CI_GetSystemParam(CI_sysParamSecondaryDns, &u32, &szParam);
            // link-down fix: always valid if DHCP is disabled
            if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED))
            {
                if( u32 != ethernetStaticFrequentStaticDefault.dnsAddress[1] )
                {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, 1, &u32);
                }
            }
            else
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if( attributeIndex == WHOLE_OBJECT )
        {
            result = mUpdateIPsettingsFromCI( objectIndex, 0 );
            if( result < ERROR_CODES ) {
                result = mUpdateIPsettingsFromCI( objectIndex, 1 );
            }
        }
        else
        {
        }
        break;

    case ETH_IDX_ntpServerAddress2:
        szParam = sizeof(TUSIGN32);
        if( attributeIndex == 0 )
        {
            adiCiResult = CI_GetSystemParam(CI_sysParamPrimarySntp, &u32, &szParam);
            // link-down fix: always valid if DHCP is disabled
            if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED))
            {
                if( u32 != ethernetDynamic.ntpServerAddress2[0] )
                {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, 0, &u32);
                }
            }
            else
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if( attributeIndex == 1 )
        {
            adiCiResult = CI_GetSystemParam(CI_sysParamSecondarySntp , &u32, &szParam);
            // link-down fix: always valid if DHCP is disabled
            if (((unsigned)adiCiResult == sizeof(TUSIGN32)) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED))
            {
                if( u32 != ethernetDynamic.ntpServerAddress2[1] )
                {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, 1, &u32);
                }
            }
            else
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if( attributeIndex == WHOLE_OBJECT )
        {
            result = mUpdateIPsettingsFromCI( objectIndex, 0 );
            if( result < ERROR_CODES )
            {
                result = mUpdateIPsettingsFromCI( objectIndex, 1 );
            }
        }
        else
        {
        }
        break;

    case ETH_IDX_hostName:
        szParam = DEVICE_HOST_NAME_LENGTH;
        VIP_ASSERT( szParam <= sizeof(u8array) );
        memset( u8array, 0, sizeof(u8array));
        adiCiResult = CI_GetSystemParam(CI_sysParamHostname, u8array, &szParam);
        // link-down fix: always valid if DHCP is disabled
        if((adiCiResult >= 0) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED)) {  // ADI acts like str, not like char[64]
            if( (attributeIndex == WHOLE_OBJECT) ||
                (attributeIndex >= 0) && (attributeIndex < DEVICE_HOST_NAME_LENGTH) )
            {   
                if( memcmp(ethernetStaticFrequentStaticDefault.hostName, u8array, DEVICE_HOST_NAME_LENGTH) != 0 ) {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, u8array);
                }
            } else {
                result = ILLEGAL_ATTRIB_IDX;
            }
        } else {
            result = ERROR_NV_STORAGE;
        } 
        break;

    case ETH_IDX_domainName:
        szParam = DEVICE_DOMAIN_NAME_LENGTH;
        VIP_ASSERT( szParam <= sizeof(u8array) );
        memset( u8array, 0, (size_t)szParam);
        adiCiResult = CI_GetSystemParam(CI_sysParamDomainName, u8array, &szParam);
        // link-down fix: always valid if DHCP is disabled
        if((adiCiResult >= 0) || (ethernetStaticFrequentStaticDefault.useDhcp==ETH_DHCP_STATE_DISABLED))
        {  // ADI acts like str, not like char[64]
            if( (attributeIndex == WHOLE_OBJECT) ||
                (attributeIndex >= 0) && (attributeIndex < DEVICE_DOMAIN_NAME_LENGTH) )
            {   
                if( memcmp(ethernetStaticFrequentStaticDefault.domainName, u8array, DEVICE_DOMAIN_NAME_LENGTH) != 0 ) {
                    result = Put_T_UNIT( ETHERNET_ME, objectIndex, WHOLE_OBJECT, u8array);
                }
            } else {
                result = ILLEGAL_ATTRIB_IDX;
            }
        } else {
            result = ERROR_NV_STORAGE;
        } 
        break;

    default:
        break;
    }    
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Private: Change IP Parameters using CI_SetSystemParam(CI_sysParam..., ...)
            At the end it also uses CI_ApplySystemIpParam(true) to store them at CI's NV
            The parameters are represented by framework objects and will be read from shadow-RAM
 \author    Klaus Pose
 \date      2020-12-09
 \return    eTRUE, if successful
 \warning 
 \test 
 \n test-date:          2020-12-09
 \n by:                 nobody
 \n environment:        J-Link debug interface, IAR debug SW, device: Coriolis
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TBOOL mUpdateIPsettingsToCI( TUSIGN16 objectIndex, TINT16 attributeIndex )
{
    TUSIGN16 result = OK;
    int32_t adiCiResult;
    TUSIGN8 u8;
    TUSIGN32 u32;

    switch (objectIndex)
    {
    case ETH_IDX_useDhcp:
        if( ethernetStaticFrequentStaticDefault.useDhcp == ETH_DHCP_STATE_DISABLED )
        {
            u8 = 0; // CI definition ==0: disable DHCP (use static IP address)
        }
        else
        {
            u8 = 1; // CI defintion !=0: enable DHCP 
        }
        adiCiResult = CI_SetSystemParam(CI_sysParamDhcpEnable, &u8, 1);
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_hostAddress:
        u32 = ethernetStaticFrequentStaticDefault.hostAddress;
        adiCiResult = CI_SetSystemParam(CI_sysParamIpAddress, &u32, sizeof(TUSIGN32));
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_subnetMask:
        u32 = ethernetStaticFrequentStaticDefault.subnetMask;
        adiCiResult = CI_SetSystemParam(CI_sysParamSubnetMask, &u32, sizeof(TUSIGN32));
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_gatewayAddress:
        u32 = ethernetStaticFrequentStaticDefault.gatewayAddress;
        adiCiResult = CI_SetSystemParam(CI_sysParamDefaultGateway, &u32, sizeof(TUSIGN32));
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break;

    case ETH_IDX_dnsAddress:
        if((attributeIndex == 0) || (attributeIndex==WHOLE_OBJECT))
        {
            u32 = ethernetStaticFrequentStaticDefault.dnsAddress[0];
            adiCiResult = CI_SetSystemParam(CI_sysParamPrimaryDns, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if(attributeIndex == 1)
        {
            u32 = ethernetStaticFrequentStaticDefault.dnsAddress[1];
            adiCiResult = CI_SetSystemParam(CI_sysParamSecondaryDns, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else
        {
            result = ILLEGAL_ATTRIB_IDX;
        }
        if((attributeIndex == WHOLE_OBJECT) && (result < ERROR_CODES))
        {
            u32 = ethernetStaticFrequentStaticDefault.dnsAddress[1];
            adiCiResult = CI_SetSystemParam(CI_sysParamSecondaryDns, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        break;

    case ETH_IDX_ntpServerAddress2:
        if((attributeIndex == 0) || (attributeIndex==WHOLE_OBJECT))
        {
            u32 = ethernetDynamic.ntpServerAddress2[0];
            adiCiResult = CI_SetSystemParam(CI_sysParamPrimarySntp, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else if(attributeIndex == 1)
        {
            u32 = ethernetDynamic.ntpServerAddress2[1];
            adiCiResult = CI_SetSystemParam(CI_sysParamSecondarySntp, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        else
        {
            result = ILLEGAL_ATTRIB_IDX;
        }
        if((attributeIndex == WHOLE_OBJECT) && (result < ERROR_CODES))
        {
            u32 = ethernetDynamic.ntpServerAddress2[1];
            adiCiResult = CI_SetSystemParam(CI_sysParamSecondarySntp, &u32, sizeof(TUSIGN32));
            if (adiCiResult != CI_OK)
            {
                result = ERROR_NV_STORAGE;
            }
        }
        break;

    case ETH_IDX_hostName:
        adiCiResult = CI_SetSystemParam(CI_sysParamHostname, (void *)&(ethernetStaticFrequentStaticDefault.hostName[0]), DEVICE_HOST_NAME_LENGTH);
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break; 

    case ETH_IDX_domainName:
        adiCiResult = CI_SetSystemParam(CI_sysParamDomainName, (void *)&(ethernetStaticFrequentStaticDefault.domainName[0]), DEVICE_DOMAIN_NAME_LENGTH);
        if (adiCiResult != CI_OK)
        {
            result = ERROR_NV_STORAGE;
        }
        break; 

    default:
        result = ILLEGAL_OBJ_IDX;
        break;
    }

    if( result < ERROR_CODES )
    {
        adiCiResult = CI_ApplySystemIpParam((bool)true);
        if( adiCiResult != CI_OK )
        {
        }
    }
    else
    {
        return eFALSE;
    }
    return eTRUE;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Convert a 48bit MAC stated by 6 bytes to a 17 characters with hexadecimal numbers
 \author Klaus Pose
 \date   2020-01-22
 \param  mac6U8 an array of 6 bytes
 \param  pMACascii pointer to a buffer to place the generated string
 \return void
 \test
 \n test-date:          2020-01-22
 \n by:                 Klaus Pose
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          complete code coverage
 \n result module test: OK
 \n result Lint check:  OK
 \warning
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
static void mConvertMAC6U8to17Ascii( const TUSIGN8 * mac6U8, TCHAR * pMACascii )
{
    #define MAC_NUM_OF_BYTES      6
    #define MAC_ASCII_SEPARATOR   '-'
    const TCHAR binToAsciiTable[16] = {'0','1','2','3','4','5','6','7','8','9',
                                       'A','B','C','D','E','F' };
    TUSIGN8 i,k;
    TCHAR * pAscii;

    VIP_ASSERT( pMACascii );

    pAscii = pMACascii;
    for( i=0; i < MAC_NUM_OF_BYTES; i++ )
    {   // one U8 byte = two ascii hex digits
        if( mac6U8 )
        {
            k = mac6U8[i] >> 4;   // hi nibble
            *(pAscii++) = binToAsciiTable[k];
            k = mac6U8[i] & 0x0F; // lo nibble
            *(pAscii++) = binToAsciiTable[k];
        }
        else
        {   // null pointer
            *(pAscii++) = '?';
            *(pAscii++) = '?';
        }
        if( i < (MAC_NUM_OF_BYTES-1) )
        {
            // double-digit separator
            *(pAscii++) = MAC_ASCII_SEPARATOR;
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Get specified MAC address as string
 \author Klaus Pose
 \date   2020-01-22
 \param  objectIndex of the MAC address
 \param  attributeIndex of the MAC address as string (one attribute per ascii character)
 \param  ptrValue pointer to a string buffer
 \return void
 \test
 \n test-date:          2020-01-22
 \n by:                 Klaus Pose
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          complete code coverage
 \n result module test: OK
 \n result Lint check:  OK
 \warning
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 mGetMacStr( TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue )
{
    TCHAR strMAC[17];

    switch (objectIndex)
    {
    case ETH_IDX_chasisMACstr:
        mConvertMAC6U8to17Ascii( ethernetDynamic.chasisMAC, strMAC );
        break;
    case ETH_IDX_port1MACstr:
        mConvertMAC6U8to17Ascii( ethernetDynamic.port1MAC, strMAC );
        break;
    case ETH_IDX_port2MACstr:
        mConvertMAC6U8to17Ascii( ethernetDynamic.port2MAC, strMAC );
        break;
    default:
        return WRONG_STATE_ERR;
    }
    if( attributeIndex == WHOLE_OBJECT )
    {
        memcpy( ptrValue, strMAC, sizeof(strMAC) );
    }
    else if( attributeIndex>=0 && attributeIndex<(TINT16)sizeof(strMAC) )
    {
        *(TCHAR * )ptrValue = strMAC[attributeIndex];
    }
    else
    {
        return ILLEGAL_ATTRIB_IDX;
    }
    return OK;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Module Private: Filter Network related Strings
 \brief  to be used at overloaded Put
 \author Klaus Pose
 \date   2020-12-15
 \param  objectIndex    object index
 \param  attributeIndex attribute index; <0 --> put whole object
 \param  ptrValue  pointer to attribute-value and return-Value
 \return error-code
 \test
 \n test-date: 2020-12-15
 \n by:        Klaus Pose
 \n environment:
 \n intention:    Code coverage
 \n result module test: ok
 \n result Lint check:  ok
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 mFilter_NetworkName(TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result = OK;
    TUSIGN8 i, k;
    TCHAR c8;
    TCHAR * pc8;
    TCHAR * pc8write;
    T_DATA_OBJ myObject;

    VIP_ASSERT(ptrValue);

    myObject = ETHERNET_ME->ptrObjectList[objectIndex];

    if( attributeIndex == WHOLE_OBJECT )
    {
        pc8 = ptrValue;
        pc8write = ptrValue;
        k = 0;
        for( i=0; i < (myObject.numberOfAttributes-1); i++ )
        {
            c8 = pc8[i];
            if( c8 > ' ') { // remove all unprintable and spaces
                pc8write[k++] = c8;
            } else
            if( c8 == '\0' ) {  // finish
                break;
            } else
            {}  // filter
        }
        if( k == 0 ) {  // result: empty string
            result = RULE_VIOLATION_ERR;  // not allowed
            return result;
        }
        if( k < myObject.numberOfAttributes )
        {
            // erase trailing ghost characters
            (void)memset( &(pc8write[k]), '\0', myObject.numberOfAttributes - k );
        }
        return result;
    } else
    if( attributeIndex < myObject.numberOfAttributes )
    {   // Caution: avoid put attribute, with arbitrary index the results can be strange
        c8 = *((TCHAR *)ptrValue);
        if( !((c8 == 0) || (c8 > ' ')) ) {
            result = RULE_VIOLATION_ERR;
        } else
        {}
    } else
    {   // illegal attribute index
        result = RULE_VIOLATION_ERR;
    }
    return result;
}
#if 0 // currently not used
static TUSIGN16 mCheck_NetworkName(TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    #define _CASE_FORCE       0       // 0=none, 1=lowercase, 2=uppercase
    #define _EMPTY_HOSTNAME   "ABB-Flow"
    #define _EMPTY_DOMAINNAME "my-domain"
    TUSIGN16 result = OK;
    TUSIGN8 i, k;
    TCHAR c8;
    TCHAR * pc8;
    TCHAR * pc8write;
    T_DATA_OBJ myObject;

    assert(ptrValue);

    myObject = ETHERNET_ME->ptrObjectList[objectIndex];

    assert( sizeof(buffer) >= myObject.numberOfAttributes );

    if( attributeIndex == WHOLE_OBJECT )
    {
        pc8 = ptrValue;
        pc8write = ptrValue;
        k = 0;
        for( i=0; i < (myObject.numberOfAttributes-1); i++ )
        {   // allowed chars: 'A'to'Z' everywhere, '0' to '9' not at start
            c8 = pc8[i];
            if( (c8 >= 'A') && (c8 <= 'Z') ) {
#if( _CASE_FORCE == 1 )
                pc8write[k] = c8 + 0x20;  // always store lowercase
#else
                pc8write[k] = c8;
#endif
                k++;
            } else
            if( (c8 >= 'a') && (c8 <= 'z') ) {
#if( _CASE_FORCE == 2 )
                pc8write[k] = c8 - 0x20;  // always store uppercase ?
#else
                pc8write[k] = c8;
#endif
                k++;
            } else
            if( c8 == '\0' ) {  // finish
                break;
            } else
            if( k != 0 ) {  // equiv. those are not allowed as 1st character
                if( (c8 >= '0') && (c8 <= '9') ) {
                    pc8write[k] = c8;
                    k++;
                } else
                if( objectIndex == ETHFBB_IDX_hostName ) {
                    if( c8 == '-' ) {
                        pc8write[k] = c8;
                        k++;
                    }
                } else
                if( objectIndex == ETHFBB_IDX_domainName ) {
                    if( (c8 == '-') || (c8 == '.') ) {
                        pc8write[k] = c8;
                        k++;
                    }
                } else
                { // filtered out
                    result = RULE_VIOLATION_ERR;
                }
            } else
            { // filtered out
                result = RULE_VIOLATION_ERR;
            }
        }
//        pc8write[k] = '\0'; // k=size of visible string
//        if( k > 0 )
        {   // check and clean-up ending
            if( objectIndex == ETHFBB_IDX_hostName ) {
                for( ; k ; ) {
                    k--;
                    if( pc8write[k] == '-' ) {
//                        pc8write[k] = '\0';
                    } else {
                        k++;
                        break;
                    }
                }
            } else
            if( objectIndex == ETHFBB_IDX_domainName ) {
                for( ; k ; ) {
                    k--;
                    if( (pc8write[k] == '-') || (pc8write[k] == '.') ) {
//                        pc8write[k] = '\0';
                    } else {
                        k++;
                        break;
                    }
                }
            } else
            {}
        }
        if( k == 0 ) {
            // TODO ? empty string replacement
            if( objectIndex == ETHFBB_IDX_hostName ) {
                k = sizeof(_EMPTY_HOSTNAME);
                (void)memcpy( pc8write, _EMPTY_HOSTNAME, k-- );
            } else
            if( objectIndex == ETHFBB_IDX_domainName ) {
                k = sizeof(_EMPTY_DOMAINNAME);
                (void)memcpy( pc8write, _EMPTY_DOMAINNAME, k-- );
            } else
            {}
        }
        if( k < myObject.numberOfAttributes )
        {
            // erase trailing ghost characters
            (void)memset( &(pc8write[k]), '\0', myObject.numberOfAttributes - k );
        }
        return result;
    } else
    if( attributeIndex < myObject.numberOfAttributes )
    {
        TCHAR buffer[65];
        (void)memcpy( buffer, myObject.ptrValue, myObject.numberOfAttributes );
        buffer[attributeIndex] = *((TCHAR *)ptrValue);
        result = mCheck_NetworkName( objectIndex, WHOLE_OBJECT, buffer );
    } else
    {   // illegal attribute index
        result = RULE_VIOLATION_ERR;
    }
    return result;
}
#endif


//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author Klaus Pose
 \date   2020-12-15
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  initialize calculations used by the subsystem
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inside the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \test
 \n by: 
 \n intention: 
 \n result module test:    2020-12-15, OK
 \n result Lint Level 3: 
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_ETHERNET(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result;
    TUSIGN8 u8;

    VIP_ASSERT(me == ETHERNET_ME);

    result = Initialize_T_UNIT(me, typeOfStartUp);

    if (result == OK)
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
        }

        if( (typeOfStartUp & INIT_TASKS) != 0)
        {   // override invalid defaults at nv memory
            if( ethernetStaticFrequentStaticDefault.enableWebserver == ETH_SECURED_ACCESS_INVALID)
            {
                u8 = ETH_SECURED_ACCESS_DISABLED;
                (void)Put_T_UNIT( me, ETH_IDX_enableWebserver, WHOLE_OBJECT, &u8 );
            }

            if( ethernetStaticFrequentStaticDefault.enableEthernetIp == ETH_UNSECURED_ACCESS_INVALID)
            {
                u8 = ETH_UNSECURED_ACCESS_DISABLED;
                (void)Put_T_UNIT( me, ETH_IDX_enableEthernetIp, WHOLE_OBJECT, &u8 );
            }

            if( ethernetStaticFrequentStaticDefault.enableProfinetIo == ETH_UNSECURED_ACCESS_INVALID)
            {
                u8 = ETH_UNSECURED_ACCESS_DISABLED;
                (void)Put_T_UNIT( me, ETH_IDX_enableProfinetIo, WHOLE_OBJECT, &u8 );
            }

            if( ethernetStaticFrequentStaticDefault.enableModbusTcp == ETH_UNSECURED_ACCESS_INVALID)
            {
                u8 = ETH_UNSECURED_ACCESS_DISABLED;
                (void)Put_T_UNIT( me, ETH_IDX_enableModbusTcp, WHOLE_OBJECT, &u8 );
            }

            if( ethernetStaticFrequentStaticDefault.enableOpcUa == ETH_FULL_ACCESS_INVALID)
            {
                u8 = ETH_FULL_ACCESS_DISABLED;
                (void)Put_T_UNIT( me, ETH_IDX_enableOpcUa, WHOLE_OBJECT, &u8 );
            }
        }

        if( (typeOfStartUp & INIT_DATA) != 0)
        {
            //Init RAM Dataclasses with Default Values.
            (void)me->LoadRomDefaults(me, (TINT16)ETHERNET_DC_IDX_DYNAMIC);

            if (EthernetCounterInit() == eFALSE)
            {
            }
        }

        if( (typeOfStartUp & INIT_CALCULATION) != 0)
        {
        }
    }

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Get objects based on values in ADI stack
 \author Michal Kochel (Mobica), Klaus Pose
 \date   2019-03-08            , 2020-09-28
 \param  me = pointer to subsystem
 \return T_UNIT error code.
 \test
 \n test-date:          
 \n by:                
 \n environment:        
 \n intention:          
 \n result module test: 
 \n result Lint check:  
 \warning
 \bug      
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_ETHERNET(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 getFunctionResult = OK;
    CI_systemStatus_t netStatus;

    VIP_ASSERT(me == ETHERNET_ME);

    if( attributeIndex >= me->ptrObjectList[objectIndex].numberOfAttributes )
    {
        return ILLEGAL_ATTRIB_IDX;
    }

    netStatus = CI_GetSystemStatus();
    if( ((netStatus != CI_systemStatusOk)) &&
        ( objectIndex > ETH_IDX_enableWebserver ) )
    {   // CI functions are not available this time
        return WRONG_STATE_ERR; 
    }
    
    switch (objectIndex) 
    {
    case ETH_IDX_ntpServerAddress:
        objectIndex = ETH_IDX_ntpServerAddress2;    // redirection
        if( attributeIndex == WHOLE_OBJECT ) {
            attributeIndex = 0; // prevent ptrValue overrun
        }
        //lint -fallthrough
    case ETH_IDX_useDhcp:
    case ETH_IDX_hostAddress: 
    case ETH_IDX_subnetMask:
    case ETH_IDX_gatewayAddress:
    case ETH_IDX_dnsAddress:
    case ETH_IDX_ntpServerAddress2:
    case ETH_IDX_hostName:
    case ETH_IDX_domainName:
        // first we must update from CI because the we don't get notified by changes via EIP
        getFunctionResult = mUpdateIPsettingsFromCI(objectIndex, attributeIndex);
        if( getFunctionResult < ERROR_CODES ) {
            getFunctionResult = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        } else {
            getFunctionResult = DECLINE;
        }
        break;

    case ETH_IDX_port1AutoNegotiate:
    case ETH_IDX_port2AutoNegotiate:
    case ETH_IDX_port1Speed:
    case ETH_IDX_port2Speed: 
    case ETH_IDX_port1DuplexMode: 
    case ETH_IDX_port2DuplexMode: 
        // first we must update from CI because the we don't get notified by changes via EIP
        getFunctionResult = mUpdatePhySettingsFromCI();
        if( getFunctionResult < ERROR_CODES ) {
            getFunctionResult = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        } else {
            getFunctionResult = DECLINE;
        }
        break;

    case ETH_IDX_portSpeedStatus:
    case ETH_IDX_portDuplexStatus:
        // first we must update frame objects data from CI 
        getFunctionResult = mUpdatePhyStatusFromCI();
        if( getFunctionResult < ERROR_CODES ) {
            getFunctionResult = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        } else {
            getFunctionResult = DECLINE;
        }
        break;

    case ETH_IDX_port1EthStatsHMI: 
    case ETH_IDX_port2EthStatsHMI: {
      TUSIGN32* intValues = (TUSIGN32*)ptrValue;
      T_COUNTER* ethernetEthCounters = NULL;

      if (objectIndex == ETH_IDX_port1EthStatsHMI) {
        ethernetEthCounters = ethernetEthPort1Counters;
      }
      else if (objectIndex == ETH_IDX_port2EthStatsHMI) {
        ethernetEthCounters = ethernetEthPort2Counters;
      }
      else {
          break;
      }

      if (attributeIndex == WHOLE_OBJECT) {
        CountersCurrentValue(ethernetEthCounters + ETHERNET_ETH_COUNTER_BYTES_RX, intValues + 0);
        CountersCurrentValue(ethernetEthCounters + ETHERNET_ETH_COUNTER_BYTES_TX, intValues + 1);
        CountersCurrentValue(ethernetEthCounters + ETHERNET_ETH_COUNTER_FRAMES_ERRORS_RX, intValues + 2);
        CountersCurrentValue(ethernetEthCounters + ETHERNET_ETH_COUNTER_FRAMES_COLISIONS_TX, intValues + 3);
      }
      else {
        E_ETHERNET_ETH_COUNTERS counterIndex = ETHERNET_ETH_COUNTER_BYTES_RX;
        switch (attributeIndex) {
            case 0: counterIndex = ETHERNET_ETH_COUNTER_BYTES_RX; break;
            case 1: counterIndex = ETHERNET_ETH_COUNTER_BYTES_TX; break;
            case 2: counterIndex = ETHERNET_ETH_COUNTER_FRAMES_ERRORS_RX; break;
            case 3: counterIndex = ETHERNET_ETH_COUNTER_FRAMES_COLISIONS_TX; break;
            default: break;
        }
        CountersCurrentValue(ethernetEthCounters + counterIndex, intValues);
      }
    } break;

    case ETH_IDX_ipStats: {
      TUSIGN32* intValues = (TUSIGN32*)ptrValue;

      CountersCurrentValue(ethernetEthTcpIpCounters + ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS, intValues);
    } break;

    case ETH_IDX_chasisMACstr:
    case ETH_IDX_port1MACstr:
    case ETH_IDX_port2MACstr:
        mUpdateMacAddress();
        getFunctionResult = mGetMacStr( objectIndex, attributeIndex, ptrValue );
        break;

    case ETH_IDX_chasisMAC: 
    case ETH_IDX_port1MAC: 
    case ETH_IDX_port2MAC: 
        mUpdateMacAddress();
        if( ptrValue == NULL )
        {   // update only
            return OK;
        }
    case ETH_IDX_hmiValidityTestResults:
        ethernet.RefreshHmiValidityTestResultsEXE();
        getFunctionResult = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        break;
        
    default:
        getFunctionResult = Get_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        break;
    }

    return getFunctionResult;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief Puts data in objects and additionaly sets relevant values in ADI stack
 \author Michal Kochel (Mobica), Klaus Pose
 \date   2019-03-07            , 2021-01-12
 \param  me = pointer to subsystem
 \param  objectIndex = object index.
 \param  attributeIndex = attribute index.
 \param  ptrValue = pointer to object value.
 \return T_UNIT error code.
 \test
 \n test-date:          
 \n by:                
 \n environment:        
 \n intention:          
 \n result module test: 
 \n result Lint check:  
 \warning
 \bug      
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_ETHERNET(const T_UNIT SLOW *me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST *ptrValue)
{
    TUSIGN16 putFunctionResult;
    TUSIGN8 u8;
    // E_SUP_RET settingSuFirmwareSourceResult;

    VIP_ASSERT(me == ETHERNET_ME);

    if( attributeIndex >= me->ptrObjectList[objectIndex].numberOfAttributes )
    {
        return ILLEGAL_ATTRIB_IDX;
    }

    // pre-check for attribute rules, which are dynamic at runtime
    putFunctionResult = Check_ETHERNET( me, objectIndex, attributeIndex, ptrValue );
    if( putFunctionResult >= ERROR_CODES )
    {
        return putFunctionResult;
    } 

    putFunctionResult = OK;
    switch (objectIndex) 
    {
    case ETH_IDX_ntpServerAddress:
        objectIndex = ETH_IDX_ntpServerAddress2;    // redirection
        if( attributeIndex == WHOLE_OBJECT )
        {
            attributeIndex = 0;
        }
        //lint -fallthrough
    case ETH_IDX_hostAddress: 
    case ETH_IDX_subnetMask: 
    case ETH_IDX_gatewayAddress: 
    case ETH_IDX_dnsAddress:
    case ETH_IDX_ntpServerAddress2:
        // first we must update DHCP from CI because the we don't get notified by changes via EIP
        (void)mUpdateIPsettingsFromCI(ETH_IDX_useDhcp, 0); 
        if( ethernetStaticFrequentStaticDefault.useDhcp == ETH_DHCP_STATE_ENABLED )
        {
            putFunctionResult = RULE_VIOLATION_ERR;
            break;
        }
        //lint -fallthrough
    case ETH_IDX_useDhcp: 
    case ETH_IDX_hostName:
    case ETH_IDX_domainName:
        // first we must update from CI because the we don't get notified by changes via EIP
        (void)mUpdateIPsettingsFromCI(objectIndex, attributeIndex); 
        putFunctionResult = Put_T_UNIT( me, objectIndex, attributeIndex, ptrValue );
        if( putFunctionResult < ERROR_CODES )
        {   // the parameter must take effect at the ADI CI
            if( mUpdateIPsettingsToCI(objectIndex, attributeIndex) == eFALSE )
            {   // problem to update at CI
                putFunctionResult = RULE_VIOLATION_ERR;
            }
        }
        break;

    case ETH_IDX_port1DuplexMode: 
    case ETH_IDX_port1Speed:
        // first we must update from CI because the we don't get notified by last changes via EIP
        (void)mUpdatePhySettingsFromCI();
        if( ethernetStaticFrequentStaticDefault.port1AutoNegotiate == ETH_AUTO_NEGOTIATE_ENABLED ) {
            putFunctionResult = RULE_VIOLATION_ERR;
            break;
        }
        //lint -fallthrough
    case ETH_IDX_port1AutoNegotiate:
        // first we must update from CI because the we don't get notified by last changes via EIP
        (void)mUpdatePhySettingsFromCI();
        putFunctionResult = Put_T_UNIT( me, objectIndex, attributeIndex, ptrValue );
        if( putFunctionResult < ERROR_CODES )
        {   // the parameter must take effect at the ADI CI
            if( mUpdatePhySettingsToCI() == eFALSE )
            {   // problem to update at CI
                putFunctionResult = RULE_VIOLATION_ERR;
            }
        }
        break;

    case ETH_IDX_port2DuplexMode: 
    case ETH_IDX_port2Speed: 
        // first we must update from CI because the we don't get notified by last changes via EIP
        (void)mUpdatePhySettingsFromCI();
        if( ethernetStaticFrequentStaticDefault.port2AutoNegotiate == ETH_AUTO_NEGOTIATE_ENABLED ) {
            putFunctionResult = RULE_VIOLATION_ERR;
            break;
        }
        //lint -fallthrough
    case ETH_IDX_port2AutoNegotiate:
        // first we must update from CI because the we don't get notified by last changes via EIP
        (void)mUpdatePhySettingsFromCI();
        putFunctionResult = Put_T_UNIT( me, objectIndex, attributeIndex, ptrValue );
        if( putFunctionResult < ERROR_CODES )
        {   // the parameter must take effect at the ADI CI
            if( mUpdatePhySettingsToCI() == eFALSE )
            {   // problem to update at CI
                putFunctionResult = RULE_VIOLATION_ERR;
            }
        }
        break;

    case ETH_IDX_selectedMainProtocol:
#if 0
        // check for change
        u8 = *(TUSIGN8 * )ptrValue; 
        if( u8 > ETH_INVALID_PROTOCOL ) 
        {   // a valid request
            if( u8 != ethernetStaticFrequentStaticDefault.selectedMainProtocol )
            {   // it is a change, try Put
                putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, &u8);
                if( putFunctionResult < ERROR_CODES )
                {   // the Put was successful
                    // Call SoftwareUpdate which arms the ADI Bootloader properly and execute a OC Reset
                    settingSuFirmwareSourceResult = setBootloaderFirmwareSource(u8, eFALSE);

                    if (settingSuFirmwareSourceResult == E_SUP_OK) {

                        (void)coordinator.TriggerRuntimeActionEXE(CORD_RUNTIME_ACTION_SWITCH_MAIN_PROTOCOL_IO_APP);
                        putFunctionResult = WARNING;    // signal change to caller
                    }
                    else {

                        putFunctionResult = RULE_VIOLATION_ERR;
                    }
                }
            } // else no change, nothing to do
        } else
        {
            putFunctionResult = RULE_VIOLATION_ERR;
        }
        break;
#endif

    case ETH_IDX_enableEthernetIp:
        // check for change
        u8 = *(TUSIGN8 * )ptrValue; 
        if( u8 != ethernetStaticFrequentStaticDefault.enableEthernetIp )
        {   // it is a change
#ifdef COMPILE_WITH_ETHERNETIP_SUBSYSTEM
            etherNetIP.UpdateEnableStateEXE( u8 );
#endif
            // final store
            putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        }
        break;
		
    case ETH_IDX_enableModbusTcp:
        // check for change
        u8 = *(TUSIGN8 * )ptrValue; 
        if( u8 != ethernetStaticFrequentStaticDefault.enableModbusTcp )
        {   // it is a change
#ifdef COMPILE_WITH_MODBUS_SUBSYSTEM
            modbus.UpdateEnableStateEXE( u8 );
#endif
            // final store
            putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        }
        break;



    case ETH_IDX_enableProfinetIo:
        // check for change
        u8 = *(TUSIGN8 *)ptrValue; 
        if( u8 != ethernetStaticFrequentStaticDefault.enableProfinetIo )
        {   // it is a change
#ifdef COMPILE_WITH_PROFINET_SUBSYSTEM
            // profinetIO.UpdateEnableStateEXE( u8 );
#endif
            // final store
            putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        }
        break;

    case ETH_IDX_enableWebserver:
        // check for change
        u8 = *(TUSIGN8 *)ptrValue; 
        if( u8 != ethernetStaticFrequentStaticDefault.enableWebserver )
        {   // it is a change
#ifdef COMPILE_WITH_WEBSERVER_SUBSYSTEM
            webServer.UpdateEnableStateEXE( u8 );
#endif
            // final store
            putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        }
        break;

    default:
        putFunctionResult = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);
        break;
    }

    return putFunctionResult;
}


//--------------------------------------------------------------------------------------------------
    /*!
     \brief  check the attribute attributeIndex of the data-object objectIndex ; only in "INITIALIZED"
     \brief  to be used at overloaded Put if GetAttributeDescription is overloaded (dynamic descriptor)
     \author Klaus Pose
     \date   2020-12-15
     \param  me
     \param  objectIndex    object index
     \param  attributeIndex attribute index; <0 --> put whole object
     \param  ptrValue  pointer to attribute-value and return-Value
     \return error-code
     <pre>
       OK                      operation was successful
       LESSTHAN_RANGEMIN       at least one value is less than its minimum value
       GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
       SPAN_TO_SMALL           then span between two values is to small
       NOT_ON_GRID             at least one value is not a multiple of its increment
       RULE_VIOLATION          this shows a violation of one or more businessrules
       DECLINE                 DEBUG, operation isn't permitted in the actual unit-state
       ILLEGAL_OBJ_IDX         DEBUG, unknown Object
       ILLEGAL_ATTRIB_IDX      DEBUG, unknown Attribute
     </pre>
    \test
  \n test-date:
  \n by:
  \n environment:
  \n intention:          Code coverage
  \n result module test:
  \n result Lint check:
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Check_ETHERNET(const T_UNIT SLOW*me,
                      TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue)
{
    TUSIGN16 result;
    T_DATA_OBJ myObject;
    T_DO_ATTRIB_RANGE myDescriptor;
    TUSIGN16 attributes;
    TUSIGN8 * ptrValueCheck;

    // check pointer
    VIP_ASSERT( me == ETHERNET_ME );
    VIP_ASSERT(ptrValue);
    //lint -e613 Possible use of null pointer now prevented
    VIP_ASSERT(me->ptrObjectList);

    // exception if not initialized
    VIP_ASSERT(*(me->ptrState)>=INITIALIZED);

    // unknown object
    if( objectIndex>me->maxIdx )
    {
        result = ILLEGAL_OBJ_IDX;
    } else 
    if( (objectIndex == ETH_IDX_hostName) || (objectIndex == ETH_IDX_domainName) ) 
    {   // special checking for hostName and domainName (can't filter direct EIP access, ask ADI)
        result = mFilter_NetworkName( objectIndex, attributeIndex, ptrValue );
    } else
    {
        // copy the original object
        myObject = me->ptrObjectList[objectIndex];
        attributes = myObject.numberOfAttributes;
        if( attributeIndex == WHOLE_OBJECT )
        {
            attributeIndex = 0;
        } else
        if( (attributeIndex >= 0) && (attributeIndex < attributes) )
        {
            attributes = 1;
        } else
        {
            return ILLEGAL_ATTRIB_IDX;
        }
        result = OK;
        ptrValueCheck = ptrValue;
        for( ; attributes && (result < ERROR_CODES); )
        {
            if( GetAttributeDescription_ETHERNET( me, objectIndex, (TUSIGN16)attributeIndex, &myDescriptor ) < ERROR_CODES )
            {   // get the T_DO_ATTRIB_RANGE
                if( myDescriptor.rulesAreActive == eTRUE )
                {   // rules are active
                    // overload ptrDescription for known classes at the copied object
                    if( myObject.pClass == &cgClassInitializer_TABENUM8 )
                    {   // TABENUM8
                        myObject.ptrDescription = &(myDescriptor.range.e8);
                        // check the copied object (with the probably overloaded ptrDescription
                        result = myObject.pClass->Check(&myObject, ptrValueCheck, attributeIndex);
                    } else
                    {   // no overload, no check
                        // improvement if needed: add overload and check for other classes
                    }
                }
                if( myObject.typeQualifier == STRUCT ) {
                    ptrValueCheck += cgSimpleTypeLength[myObject.pClass->aryAttribDataTypes[attributeIndex]];
                } else
                {
                    ptrValueCheck += cgSimpleTypeLength[myObject.pClass->aryAttribDataTypes[0]];
                }
                attributeIndex++;
                attributes--;
            } else
            {
                result = RULE_VIOLATION_ERR;
            }
        }
    }        
    return result;
}
//lint +e613 Possible use of null pointer


//--------------------------------------------------------------------------------------------------
/*!
     \brief  Get the description of the addressed attribute
     \author Heiko Kresse, Klaus Pose
     \date   2004-10-12  , 2020-07-28
     \param  me
     \param  objecIndex
     \param  attributeIndex ==-1 --> putObject; >=0 --> putAttribute
     \param  ptrDescriptor range-information
       <pre>
              ptrDescriptor->rulesAreActive ==eFALSE--> this attribute will not be validated --> no range-info
              ptrDescriptor->dataType       :  type info for the following union
              ptrDescriptor->range          : (union) Container for range-information

              example for simple type ST_INT16 (refer to simple_type.h unit T_DATA_OBJ)

                ptrDescriptor->range.i16.min       : minimum
                ptrDescriptor->range.i16.max       : maximum
                ptrDescriptor->range.i16.increment : increment=5 --> data value must be a multiple of 5

              for ST_E8 (tabled enumeration)
                ptrDescriptor->range.e8.numberOfCodes                  : as the name says
                ptrDescriptor->range.e8.aryCodeTab[0..numberOfCodes-1] :  ""
       </pre>
     \return error-code
     <pre>
         OK                    operation was successful
         ILLEGAL_ATTRIB_IDX    DEBUG, unknown Attribute
     </pre>
     \test
    \n by: Heiko Kresse
    \n
    \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
    \n
    \n intention: 1. complete instruction-coverage
    \n result module test: 2006-02-14 OK
     \n result Lint Level 3: free // approved warnings and informations are suppressed
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetAttributeDescription_ETHERNET(const struct _T_UNIT SLOW*me,
                                        TUSIGN16 objectIndex,
                                        TUSIGN16 attributeIndex,
                                        T_DO_ATTRIB_RANGE FAST* ptrDescriptor)
{
    TUSIGN16 result;
    TUSIGN8 u8;
    static const SLOW TUSIGN8 E_SelectedMainProtocol_R1[] =
    // Table referenced by: selectedMainProtocol
    {
        ETH_PROFINETIO_PROTOCOL
    };
    static const SLOW T_RANGE_E8 selectedMainProtocolDescriptor_R1 =
    {
        E_SelectedMainProtocol_R1,  // TUSIGN8* ptrCodeTab
        sizeof(E_SelectedMainProtocol_R1)  // TUSIGN8 numberOfCodes
    };
    static const SLOW TUSIGN8 E_AccessStateFull_R1[] =
    // Table referenced by: enableOpcUa
    {
        ETH_FULL_ACCESS_DISABLED,
    };
    static const SLOW T_RANGE_E8 enableOpcUaDescriptor_R1 =
    {
        E_AccessStateFull_R1,  // TUSIGN8* ptrCodeTab
        sizeof(E_AccessStateFull_R1)  // TUSIGN8 numberOfCodes
    };


    VIP_ASSERT(me == ETHERNET_ME);

    result = GetAttributeDescription_T_UNIT(me,objectIndex,attributeIndex,ptrDescriptor);

    if( result >= ERROR_CODES )
    {   // error: ptrDescriptor not valid, exit
        return result;
    }

    u8 = 1;
    switch( objectIndex )
    {
    case ETH_IDX_selectedMainProtocol:
        ptrDescriptor->range.e8 = selectedMainProtocolDescriptor_R1;
        break;
    case ETH_IDX_enableOpcUa:
        ptrDescriptor->range.e8 = enableOpcUaDescriptor_R1;
        break;
    case ETH_IDX_port1Speed:
    case ETH_IDX_port2Speed:
        u8 = 2;  // first and last
        //lint -fallthrough
    case ETH_IDX_enableModbusTcp:
    case ETH_IDX_enableEthernetIp:
    case ETH_IDX_enableProfinetIo:
    case ETH_IDX_enableWebserver:
    case ETH_IDX_port1AutoNegotiate:
    case ETH_IDX_port1DuplexMode:
    case ETH_IDX_port2AutoNegotiate:
    case ETH_IDX_port2DuplexMode:
        // remove the first entry of enum table,
        ptrDescriptor->range.e8.ptrCodeTab    ++;
        // reduce number of entries
        ptrDescriptor->range.e8.numberOfCodes -= u8;
        break;
    default:
        break;
    }

    return result;
}


//--------------------------------------------------------------------------------------------------
    /*!
    \brief  SaveAsDefault method overloaded for subsystem-special handling
    \author Klaus Pose
    \date   2020-05-06
    \param  me
    \param  dataClassIndex
    <pre>
       -1 > dataClassIndex >=numOfDataClasses --> ILLEGAL_DATACLASS_INDEX
       -1 --> all data-classes of the subsystem will be called
       0 - numOfDataClasses-1 --> dataClass[dataClassIndex] will be called
    </pre>
    \return error-code
    <pre>
      OK                       operation was successful
      DECLINE                  DEBUG, operation isn't permitted in the aktual unit-state
      ILLEGAL_DATACLASS_INDEX  -1 > dataClassIndex >=numOfDataClasses
    </pre>
    \test
    \n by: Heiko Kresse
    \n
    \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
       \n
    \n intention: 1. complete instruction-coverage
    \n result module test: 2006-02-14 OK
    \n result Lint Level 3: free // approved warnings and informations are suppressed
    \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 SaveAsDefault_ETHERNET(const T_UNIT SLOW*me, TINT16 dataClassIndex)
{
    TUSIGN16 result;

    VIP_ASSERT(me==ETHERNET_ME);

    if( dataClassIndex==ETHERNET_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT || dataClassIndex==ALL_DATACLASSES )
    {   // data at ADI stack shall be transfered to the non-volatile framework storage
        (void)mUpdateIPsettingsFromCI(ETH_IDX_useDhcp, WHOLE_OBJECT);
        // optimize maybe in future ?: check DHCP=on and then skip some of the following ?
        (void)mUpdateIPsettingsFromCI(ETH_IDX_hostAddress, WHOLE_OBJECT);
        (void)mUpdateIPsettingsFromCI(ETH_IDX_subnetMask, WHOLE_OBJECT);
        (void)mUpdateIPsettingsFromCI(ETH_IDX_gatewayAddress, WHOLE_OBJECT);
        (void)mUpdateIPsettingsFromCI(ETH_IDX_dnsAddress, WHOLE_OBJECT);
        // write those always:
        (void)mUpdateIPsettingsFromCI(ETH_IDX_hostName, WHOLE_OBJECT);
        (void)mUpdateIPsettingsFromCI(ETH_IDX_domainName, WHOLE_OBJECT);

        (void)mUpdatePhySettingsFromCI();
    }

    result = SaveAsDefault_T_UNIT( me, dataClassIndex);

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
    \brief  ResetToDefault method overloaded for subsystem-special handling
    \author Klaus Pose
    \date   2020-12-14
    \param  me
    \param  dataClassIndex
    <pre>
       -1 > dataClassIndex >=numOfDataClasses --> ILLEGAL_DATACLASS_INDEX
       -1 --> all data-classes of the subsystem will be called
       0 - numOfDataClasses-1 --> dataClass[dataClassIndex] will be called
    </pre>
    \return error-code
    <pre>
      OK                       operation was successful
      DECLINE                  DEBUG, operation isn't permitted in the aktual unit-state
      ILLEGAL_DATACLASS_INDEX  -1 > dataClassIndex >=numOfDataClasses
    </pre>
 \test
 \n by: 
 \n
 \n environment: C-SPY module-test is part of the framework, the module testing is part of the framework
 \n
 \n intention: 1. complete instruction-coverage
 \n result module test: 
 \n result Lint Level 3: 
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 ResetToDefault_ETHERNET(const T_UNIT SLOW*me, TINT16 dataClassIndex)
{
    TUSIGN16 result;
    TUSIGN8 i;
    static TBOOL bInAllLoop = eFALSE;

    VIP_ASSERT(me==ETHERNET_ME);

    if( dataClassIndex==ALL_DATACLASSES ) {
        bInAllLoop = eTRUE;
        result = OK;
        for( i = 0; i < ETHERNET_DC_IDX_NUMBER_OF_DATACLASSES; i++ ) 
        {   // recursively class by class
            result |= ResetToDefault_ETHERNET( me, i );
        }
        bInAllLoop = eFALSE;
        // ensure to be called last (even with additional data classes), because it can trigger the restart to early
        if( ethernet_Put( ETH_IDX_selectedMainProtocol, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.selectedMainProtocol) ) == WARNING ) {
            // change in protocol, a restart will be initiated within another context
            for( ; ; ) {
                Delay_RTOS_TASK( 30000);
            }
        }
    } else
    if( dataClassIndex==ETHERNET_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT )
    {
        result = OK;
        if( ethernet_ControlFactoryDefaults != 2 ) 
        {   // not EIP reset type 2, so we shall also reset "connection parameters"
            // IP settings:
            result |= ethernet_Put( ETH_IDX_useDhcp, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.useDhcp) );
            if( ethernetStaticFrequentStaticDefault.useDhcp == ETH_DHCP_STATE_DISABLED ) {
                result |= ethernet_Put( ETH_IDX_hostAddress, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.hostAddress) );
                result |= ethernet_Put( ETH_IDX_subnetMask, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.subnetMask) );
                result |= ethernet_Put( ETH_IDX_gatewayAddress, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.gatewayAddress) );
                result |= ethernet_Put( ETH_IDX_dnsAddress, WHOLE_OBJECT, (void *)(ethernetStaticFrequentStaticDefaultDefault.dnsAddress) );
                result |= ethernet_Put( ETH_IDX_domainName, WHOLE_OBJECT, (void *)(ethernetStaticFrequentStaticDefaultDefault.domainName) );
                // the ntpServerAddress is not part of the default-set (not used, ADI-NV only), therefore not resetted
            }
            result |= ethernet_Put( ETH_IDX_hostName, WHOLE_OBJECT, (void *)(ethernetStaticFrequentStaticDefaultDefault.hostName) );
            // PHY settings:
            result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1AutoNegotiate, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port1AutoNegotiate) );
            if( ethernetStaticFrequentStaticDefault.port1AutoNegotiate == ETH_AUTO_NEGOTIATE_DISABLED ) {
                result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1Speed, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port1Speed) );
                result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port1DuplexMode, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port1DuplexMode) );
            }
            result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2AutoNegotiate, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port2AutoNegotiate) );
            if( ethernetStaticFrequentStaticDefault.port2AutoNegotiate == ETH_AUTO_NEGOTIATE_DISABLED ) {
                result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2Speed, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port2Speed) );
                result |= Put_T_UNIT( ETHERNET_ME, ETH_IDX_port2DuplexMode, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.port2DuplexMode) );
            }
            (void)mUpdatePhySettingsToCI();
        }

        result |= ethernet_Put( ETH_IDX_enableModbusTcp, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.enableModbusTcp) );
        result |= ethernet_Put( ETH_IDX_enableOpcUa, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.enableOpcUa) );
        result |= ethernet_Put( ETH_IDX_enableEthernetIp, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.enableEthernetIp) );
        result |= ethernet_Put( ETH_IDX_enableProfinetIo, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.enableProfinetIo) );
        result |= ethernet_Put( ETH_IDX_enableWebserver, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.enableWebserver) );
    
        if( bInAllLoop == eFALSE ) {
            if( ethernet_Put( ETH_IDX_selectedMainProtocol, WHOLE_OBJECT, (void *)&(ethernetStaticFrequentStaticDefaultDefault.selectedMainProtocol) ) == WARNING ) {
                // change in protocol, a restart will be initiated within another context
                for( ; ; ) {
                    Delay_RTOS_TASK( 30000 );
                }
            }
        }
    } else
    {
        result = ResetToDefault_T_UNIT( me, dataClassIndex );
    }

    return result;
}
