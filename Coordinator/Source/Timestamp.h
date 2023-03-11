//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Timestamp handling
 Module
 Description    Implementation of timestamp handling, conversions and comparisons

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __TIMESTMAP_H__
#define __TIMESTMAP_H__

typedef struct _T_TIME_GENERAL {
    TUSIGN8 sec;  	/* Seconds: 0-60 */
    TUSIGN8 min;  	/* Minutes: 0-59 */
    TUSIGN8 hour; 	/* Hours since midnight: 0-23 */
    TUSIGN8 day; 	/* Day of the month: 1-31 */
    TUSIGN8 mon;  	/* Months *since* January: 0-11 */
    TUSIGN16 year; 	/* Year */
} T_TIME_GENERAL;

typedef union _T_TIME_STAMP {
    TUSIGN32 raw[2];
    struct {
        TUSIGN32 era;
        TUSIGN32 seconds;
    } timestamp;
} T_TIME_STAMP;

#define TIMESTMAP_FORMAT_ISO8601_LENGTH 24
#define TIMESTMAP_FORMAT_CERTTIME_LENGTH 15

void IncrementTimeStamp(TUSIGN32 seconds, T_TIME_STAMP* Timestamp);

TINT32 TimeStampInISO8601(const T_TIME_STAMP* Timestamp, TCHAR* timeString);
TINT32 TimeStampInCertTime(const T_TIME_STAMP* Timestamp, TCHAR* timeString);

void TimeGeneralToTimeStamp(const T_TIME_GENERAL* timeGeneral, T_TIME_STAMP* Timestamp);
TBOOL NtpTimestampToTimeStamp(TUSIGN32 ntpTimeStamp, T_TIME_STAMP* Timestamp);
TBOOL DateToTimeStamp(const TCHAR* date, T_TIME_STAMP* Timestamp);

TINT8 TimeStampCmp(const T_TIME_STAMP* ocTimestampLeft, const T_TIME_STAMP* ocTimestampRight);

TINT32 TimeStampDaysDiffInDays(const T_TIME_STAMP* startDate, const T_TIME_STAMP* endDate);

void WorkingSecondsToTimeStamp(TUSIGN32 workingSeconds, T_TIME_STAMP* Timestamp);

void TimeStampToTimeGeneral(const T_TIME_STAMP* Timestamp, T_TIME_GENERAL* timeGeneral);

#endif
