//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2019.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         4WCTW
 Module         Ethernet
 Description    Counters for Ethernet
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef __ETHERNET_COUNTERS_IDX_H__
#define __ETHERNET_COUNTERS_IDX_H__

typedef enum _E_ETHERNET_ETH_COUNTERS {
    /* 
    Bytes received on port 1 and port 2
    Maps to index 0
    */
    ETHERNET_ETH_COUNTER_BYTES_RX = 0,

    /* 
    Packets (Unicast, Broadcast, Multicast) received on port 1 and port 2
    Maps to index 1, 2 and 3
    */
    ETHERNET_ETH_COUNTER_PACKETS_RX,

    /* 
    Bytes transmitted on port 1 and port 2
    Maps to index 8
    */
    ETHERNET_ETH_COUNTER_BYTES_TX,

    /* 
    Packets (Unicast, Broadcast, Multicast) transmited on port 1 and port 2
    Maps to index 9, 10 and 11
    */
    ETHERNET_ETH_COUNTER_PACKETS_TX,

    /* 
    Frames transmitted after collision/s on port 1 and port 2
    Maps to index 12 and 13
    */
    ETHERNET_ETH_COUNTER_FRAMES_COLISIONS_TX,

    /* 
    Frames transmitted after collision/s on port 1 and port 2
    Maps to index 14
    */
    ETHERNET_ETH_COUNTER_FRAMES_DROPPED_TX,

    /* 
    Frames received with alignment error, CRC/FCS error or large frame error on port 1 and port 2
    Maps to index 4, 5 and 6
    */
    ETHERNET_ETH_COUNTER_FRAMES_ERRORS_RX,

    /* 
    Frames delayed on port 1 and port 2
    Maps to index 16
    */
    ETHERNET_ETH_COUNTER_FRAMES_DELAYED_TX,

    /* 
    Number of supported ethernet counters
    */
    ETHERNET_ETH_COUNTER_COUNT

} E_ETHERNET_ETH_COUNTERS;

typedef enum _E_ETHERNET_TCP_IP_COUNTERS {
    /*
    TCP Connections active
    */
    ETHERNET_TCP_IP_COUNTER_OPENED_SOCKETS = 0,

    /*
    Sum of TCP Connections established 
    */
    ETHERNET_TCP_IP_COUNTER_TCP_CONNECTIONS_ESTABLISHED,

    /*
    Sum of UDP datagrams received
    */
    ETHERNET_TCP_IP_COUNTER_UDP_RX_DATAGRAMS,

    /*
    Sum of UDP datagrams errors
    */
    ETHERNET_TCP_IP_COUNTER_UDP_RX_ERRORS,

    /*
    Sum of UDP datagrams received
    */
    ETHERNET_TCP_IP_COUNTER_UDP_TX_DATAGRAMS,

    /*
    Sum of received TCP segments
    */
    ETHERNET_TCP_IP_COUNTER_TCP_RX_SEGMENTS,

    /*
    Sum of received TCP error segments
    */
    ETHERNET_TCP_IP_COUNTER_TCP_RX_ERRORS,

    /*
    Sum of TCP transmited segments
    */
    ETHERNET_TCP_IP_COUNTER_TCP_TX_SEGMENTS,

    /*
    Sum of TCP retransmited segments
    */
    ETHERNET_TCP_IP_COUNTER_TCP_TX_RETRANS_SEGMENTS,

    /*
    Number of segments sent with the RESET flag on
    */
    ETHERNET_TCP_IP_COUNTER_TCP_OUT_RESETS,

    /*
    The number of connections that were reset
    */
    ETHERNET_TCP_IP_COUNTER_TCP_ESTABLISHED_RESETS,

    /* 
    Number of supported TCP/IP counters
    */
    ETHERNET_TCP_IP_COUNTER_COUNT


} E_ETHERNET_TCP_IP_COUNTERS;

extern T_COUNTER* ethernetEthPort1Counters;
extern T_COUNTER* ethernetEthPort2Counters;
extern T_COUNTER* ethernetEthTcpIpCounters;

TBOOL EthernetCounterInit(void);

void EthernetCounterUpdate(void);

#endif