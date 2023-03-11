//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet

 Module

 Description    This header file defines the complete list of public data-objects.
                Other subsystems need to use these macros instead of magic-numbers  
                when accessing the public data-objects.

                The index of private data-object should not be available through
                the TUNIT interface.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifdef __ETHERNET_IDX_H__
  #error Ethernet_idx.h included more than once
#endif

#define __ETHERNET_IDX_H__

enum _T_ETHERNET_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls which industrial Ethernet based protocol is active
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_selectedMainProtocol,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls if Modbus/TCP is enabled or not
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_enableModbusTcp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls if OPC UA is enabled or not
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_enableOpcUa,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls if EtherNet/IP is enabled or not
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_enableEthernetIp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls if PROFINET IO is enabled or not
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_enableProfinetIo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Controls if Webserver is enabled or not
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_enableWebserver,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Chasis MAC address
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_chasisMAC,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 1 MAC address
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_port1MAC,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 1 auto negotiation state
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port1AutoNegotiate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 1 speed
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port1Speed,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 1 duplex mode
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port1DuplexMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 2 MAC address
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_port2MAC,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 2 auto negotiation state
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port2AutoNegotiate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 2 speed
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port2Speed,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 2 duplex mode
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_port2DuplexMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     DHCP state
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_useDhcp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Host name
     \datatype          : SIMPLE_CHAR[65]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_hostName,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Host IP address
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_hostAddress,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Subnet mask
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_subnetMask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Gateway IP address
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_gatewayAddress,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     NTP server IP address
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ETH_IDX_ntpServerAddress,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Statistics for Ethernet port 1 [0] - Bytes received [1] - Bytes transmitted  [2] - Error frames received [3] - Packets with collisions
     \datatype          : SIMPLE_U32[4]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_port1EthStatsHMI,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Statistics for Ethernet port 2 [0] - Bytes received [1] - Bytes transmitted  [2] - Error frames received [3] - Packets with collisions
     \datatype          : SIMPLE_U32[4]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_port2EthStatsHMI,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Active TCP connections
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_ipStats,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Domain Name Server Addresses
     \datatype          : SIMPLE_U32[2]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ETH_IDX_dnsAddress,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Domain Name
     \datatype          : SIMPLE_CHAR[65]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    ETH_IDX_domainName,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Chasis MAC address as array of characters
     \datatype          : SIMPLE_CHAR[17]
     \typequalifier     : ARRAY
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    ETH_IDX_chasisMACstr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 1 MAC address as array of characters
     \datatype          : SIMPLE_CHAR[17]
     \typequalifier     : ARRAY
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    ETH_IDX_port1MACstr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port 2 MAC address as array of characters
     \datatype          : SIMPLE_CHAR[17]
     \typequalifier     : ARRAY
     \storage qualifier : ROM
     \direction         : OUTPUT
    */
    ETH_IDX_port2MACstr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     NTP server IP address
     \datatype          : SIMPLE_U32[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    ETH_IDX_ntpServerAddress2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port speed PHY_actual
     \datatype          : TABENUM8[2]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_portSpeedStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Ethernet port duplex PHY_actual
     \datatype          : TABENUM8[2]
     \typequalifier     : ARRAY
     \storage qualifier : READONLY_RAM
     \direction         : OUTPUT
    */
    ETH_IDX_portDuplexStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Results of the hmi validityTests one test per bit
     \datatype          : SIMPLE_U8[32]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ETH_IDX_hmiValidityTestResults
    //@SubGen end@    <ENUMERATIONS>
};




