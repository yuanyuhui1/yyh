#ifndef __ALARMSIDX_H__
#define __ALARMSIDX_H__

typedef enum _E_ALARM_ID {
	ALARM_CERTIFICATE_EXPIRING,
	ALARM_CERTIFICATE_EXPIRED,
	ALARM_MEMORY_ERROR
} E_ALARM_ID;

typedef enum _E_ALARM_OBJECT {
	ALARM_OBJECT_MODULE,
	ALARM_OBJECT_WEBSERVER_CERTIFICATE,
	ALARM_OBJECT_INTERNAL_FLASH,
	ALARM_OBJECT_NV_MEMORY
} E_ALARM_OBJECT;

#endif