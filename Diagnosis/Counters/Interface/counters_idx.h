//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Counters
 Module         
 Description    Performance counters
 Remarks
*/
//--------------------------------------------------------------------------------------------------

#ifndef __COUNTERS_IDX_H__
#define __COUNTERS_IDX_H__

typedef enum _E_COUNTERS_GROUPS {
    COUNTER_GROUP_MODBUSTCP = 1,
    COUNTER_GROUP_ETHERNET_PORT1 = 2,
    COUNTER_GROUP_ETHERNET_PORT2 = 3,
    COUNTER_GROUP_TCPIP = 4
} E_COUNTERS_GROUPS;

#endif