//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2019.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         4WCTW Optioncard Ethernet, Diagnostics, Counters
 Module         
 Description    Internal alarms
 Remarks
*/
//--------------------------------------------------------------------------------------------------

#ifndef __ALARMS_H__
#define __ALARMS_H__

typedef enum _E_ALARM_SEVERITY {
	ALARM_SEVERITY_MINOR,
	ALARM_SEVERITY_MAJOR,
	ALARM_SEVERITY_CRITICAL,
} E_ALARM_SEVERITY;

void AlarmsInit(void);

void AlarmsStart(TUSIGN8 id, TUSIGN8 object, E_ALARM_SEVERITY severity, TBOOL transient, const TCHAR* text, ...);

void AlarmsCancel(TUSIGN8 id, TUSIGN8 object);

char* AlarmsToJson(char* jsonStringOutput);

#endif