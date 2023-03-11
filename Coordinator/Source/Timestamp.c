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
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Source/Timestamp.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "CI_codes.h"
//#include "CI_system_types.h"
//#include "CI_system_api.h"

static const TUSIGN16 TimeStampYearBase = 2000;
static const TUSIGN32 ntpTimeStampAtYear2000 = 3155673600UL;
static const TUSIGN32 TimeStampAtNtpEra1 = 1139293696;
// static const TUSIGN32 cmMaxDaysInEra = 49710;
static const TCHAR* const cmMonths[] = { (const TCHAR*)"Jan", (const TCHAR*)"Feb", (const TCHAR*)"Mar", (const TCHAR*)"Apr", (const TCHAR*)"May", (const TCHAR*)"Jun", (const TCHAR*)"Jul", (const TCHAR*)"Aug", (const TCHAR*)"Sep", (const TCHAR*)"Oct", (const TCHAR*)"Nov", (const TCHAR*)"Dec" };
static const TUSIGN8 cmDaysOfMonth[12] =  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define DAY_IN_SECONDS  (24 * 60 * 60)
#define HOUR_IN_SECONDS (60 * 60)
#define MINUTE_IN_SECONDS (60)
#define YEARDAYS(noleap, leap)   ((noleap)*365+(leap)*366)

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Is given year a leap year
 \author    Michał Kochel
 \date      2020-04-08
 \param     year Year to check
 \return    TBOOL is given year a leap year
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
static TBOOL isLeapYear(const TUSIGN16 year) {
    if( (year % 4) != 0 ) {     // divide by 4
        return eFALSE;          // remainder: no leap (most case)
    } else                      // maybe a leap
    if( (year % 400) == 0 ) {   // divide by 400
        return eTRUE;           // no remainder: leap
    } else                      // maybe a leap
    if( (year % 100) == 0 ) {   // divide by 100
        return eFALSE;          // no remainder: no leap
    } else
    {
        return eTRUE;           // leap
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Appends timestamp by given number of secodns
 \author    Michał Kochel
 \date      2020-04-08
 \param     seconds Number of seconds by which to decrease time stamp
 \param     Timestamp timestamp
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
void IncrementTimeStamp(TUSIGN32 seconds, T_TIME_STAMP* Timestamp) {
    TUSIGN32 TimestampSecondsBefore = Timestamp->timestamp.seconds;

    Timestamp->timestamp.seconds += seconds;

    if (TimestampSecondsBefore > Timestamp->timestamp.seconds) {
        Timestamp->timestamp.era++;
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Decrements timestamp by given number of secodns
 \author    Michał Kochel
 \date      2020-04-08
 \param     seconds Number of seconds by which to decrease time stamp
 \param     Timestamp timestamp
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
void DecrementTimeStamp(TUSIGN32 seconds, T_TIME_STAMP* Timestamp) {
    if (seconds > Timestamp->timestamp.seconds) {
        Timestamp->timestamp.era--;

        seconds -= Timestamp->timestamp.seconds;

        Timestamp->timestamp.seconds = 0xFFFFFFFF;
    }

    Timestamp->timestamp.seconds -= seconds;
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts timestamp into time general structure
 \author    Michał Kochel, Klaus Pose
 \date      2020-04-08   , 2021-01-25
 \param     Timestamp timestamp
 \param     timeGeneral output date in time general structure
 \warning 
 \test 
 \n test-date:          2021-01-25
 \n by:                 Klaus Pose
 \n environment:        ICE
 \n intention:          see Testcode at bottom
 \n result module test: OK
 \n result Lint Level3: OK
*/
//--------------------------------------------------------------------------------------------------
void TimeStampToTimeGeneral(const T_TIME_STAMP* Timestamp, T_TIME_GENERAL* timeGeneral) {
    TPOLY64 seconds;
    TUSIGN64 u64;
    TUSIGN32 u32;

    seconds.aryU32[1] = Timestamp->timestamp.era;
    seconds.aryU32[0] = Timestamp->timestamp.seconds;
    timeGeneral->year = TimeStampYearBase;

    // convert to full days
    u64 = seconds.u64 / DAY_IN_SECONDS;
    if( u64 >= 366 ) { // at least the first year
        u64--;
        // Calculate calendar year (0 based) by elapsed full days
        //lint -e{712} Loss of precision: TUSIGN32 days are more than enough
        u32 = ((u64 - u64 / YEARDAYS(3,1)) + u64 / YEARDAYS(75,25) - u64 / YEARDAYS(300,100) ) / 365;
        //lint -e{734} Loss of precision: TUSIGN16 to state year is enough
        timeGeneral->year += u32;
        // Calculate elapsed days by elapsed full years
        if( u32 ) {
            //lint -e{647} Suspicious truncation: TUSIGN32 days are more than enough
            u64  = u32 * 365;
            //lint -e{776} Possible truncation of addition: TUSIGN32 days are more than enough
            u64 += (u32 + 3 ) / 4;
            u32--;
            u64 -= u32 / 100;
            u64 += u32 / 400;
            // subtract elapsed days of full elapsed years
            seconds.u64 -= u64 * DAY_IN_SECONDS;    // remaining days shall now be less than a year;
        }
    } // else less than a day

    // Calculate elapsed months as seconds
    u64 = cmDaysOfMonth[0] * DAY_IN_SECONDS;    // seconds of January
    for( u32 = 0; (seconds.u64 >= u64) && (u32 < 11); )
    {
        seconds.u64 -= u64;   // subtract days of month, finally shall be less than a month
        u32++;
        u64 = cmDaysOfMonth[u32];
        if( u32 == 1 && isLeapYear(timeGeneral->year) == eTRUE ) {
            u64++;
        }
        u64 *= DAY_IN_SECONDS;
    }
    //lint -e{734} Loss of precision: TUSIGN8 to state month is enough
    timeGeneral->mon = u32;

    // Calculating full elapsed days
    timeGeneral->day = (TUSIGN8)(seconds.u64 / DAY_IN_SECONDS);
    seconds.u64 -= timeGeneral->day * DAY_IN_SECONDS;       // subtract, shall now be less than a day
    // Change callers day to start from 1
        timeGeneral->day++;

    // Calculating hours
    timeGeneral->hour = (TUSIGN8)(seconds.u64 / HOUR_IN_SECONDS);
    seconds.u64 -= timeGeneral->hour * HOUR_IN_SECONDS;     // subtract shall now be less than a hour

    // Calculating minutes
    timeGeneral->min = (TUSIGN8)(seconds.u64 / MINUTE_IN_SECONDS);
    seconds.u64 -= timeGeneral->min * MINUTE_IN_SECONDS;   // shall now be less than a minute

    timeGeneral->sec = (TUSIGN8)seconds.u64;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts timestamp into string in ISO8601 format 
 \author    Michał Kochel
 \date      2020-04-08
 \param     Timestamp timestamp
 \param     timeString Output string with date
 \return    TINT32 number of characters written to string
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TINT32 TimeStampInISO8601(const T_TIME_STAMP* Timestamp, TCHAR* timeString) {
    T_TIME_GENERAL timeGeneral;

    TimeStampToTimeGeneral(Timestamp, &timeGeneral);

    return (TINT32)sprintf((char*)timeString,
        "%04d-%02d-%02dT%02d:%02d:%02d.000Z",
        timeGeneral.year, timeGeneral.mon + 1, timeGeneral.day, timeGeneral.hour, timeGeneral.min, timeGeneral.sec);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts timestamp into string in X509 Certificate format 
 \author    Michał Kochel
 \date      2020-04-08
 \param     Timestamp timestamp
 \param     timeString Output string with date
 \return    TINT32 number of characters written to string
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TINT32 TimeStampInCertTime(const T_TIME_STAMP* Timestamp, TCHAR* timeString) {
    T_TIME_GENERAL timeGeneral;

    TimeStampToTimeGeneral(Timestamp, &timeGeneral);

    return (TINT32)sprintf((char*)timeString,
        "%04d%02d%02d%02d%02d%02d",
        timeGeneral.year, timeGeneral.mon + 1, timeGeneral.day, timeGeneral.hour, timeGeneral.min, timeGeneral.sec);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts Time General structure to OC timestamp
 \author    Michał Kochel, Klaus Pose
 \date      2020-04-08   , 2021-01-25
 \param     timeGeneral input date in time general structure
 \param     ocTimestamp Output OC timestamp
 \return    void
 \warning 
 \test 
 \n test-date:          2021-01-25
 \n by:                 Klaus Pose
 \n environment:        ICE
 \n intention:          see Testcode at bottom
 \n result module test: OK
 \n result Lint Level3: OK
*/
//--------------------------------------------------------------------------------------------------
void TimeGeneralToTimeStamp(const T_TIME_GENERAL* timeGeneral, T_TIME_STAMP* Timestamp) {
    TPOLY64 seconds;
    TUSIGN32 u32;

    u32 = timeGeneral->year;
    // subtract base
    u32 -= TimeStampYearBase;

    if( u32 ) {
    // elapsed days by elapsed full years, followed-up by leap-correction
        //lint -e{647} Suspicious truncation: TUSIGN32 days are more than enough
        seconds.u64 = u32 * 365 + (u32 + 3) / 4;
        u32--;
        seconds.u64 -= u32 / 100;
        seconds.u64 += u32 / 400;
    } else
    {
        seconds.u64 = 0;
    }

    // Add days of elapsed months
    for(u32 = 0; u32 < timeGeneral->mon; ++u32)
    {
        seconds.u64 += cmDaysOfMonth[u32];
        if (u32 == 1 && isLeapYear(timeGeneral->year) == eTRUE) {
            seconds.u64++;
        }
    }

    seconds.u64 += (timeGeneral->day - 1);  // we start the day at 1
    // days to seconds
    seconds.u64 *= DAY_IN_SECONDS;

    seconds.u64 += timeGeneral->hour * HOUR_IN_SECONDS;
    seconds.u64 += timeGeneral->min * MINUTE_IN_SECONDS;
    seconds.u64 += timeGeneral->sec;

    Timestamp->timestamp.era = seconds.aryU32[1];
    Timestamp->timestamp.seconds = seconds.aryU32[0];
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts NTP timestamp to timestamp. This solution will work till 26.02.2104 09:42:24
 \author    Michał Kochel
 \date      2020-04-08
 \param     ntpTimeStamp NTP timestamp
 \param     Timestamp Output timestamp
 \return    eTRUE conversion successful, else eFALSE
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TBOOL NtpTimestampToTimeStamp(TUSIGN32 ntpTimeStamp, T_TIME_STAMP* Timestamp) {
    // According to https://tools.ietf.org/html/rfc4330#section-3 0  by convention is interpreted as an invalid or unavailable timestamp.
    if (ntpTimeStamp == 0) {
        return eFALSE;
    }

    Timestamp->timestamp.era = 0;

    // Time before 2036-02-07 06:28:16, NTP era 0
    if ((ntpTimeStamp >> 31) > 0) {
        Timestamp->timestamp.seconds = ntpTimeStamp - ntpTimeStampAtYear2000;
    }
    // Time after, NTP era 1
    else {
        Timestamp->timestamp.seconds = TimeStampAtNtpEra1;
        IncrementTimeStamp(ntpTimeStamp, Timestamp);
    }

    return eTRUE;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Converts date in format Mmm dd yyyy to timestamp
 \author    Michał Kochel
 \date      2020-04-08
 \param     date String with date
 \param     Timestamp Output timestamp
 \return    eTRUE conversion successful, else eFALSE
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TBOOL DateToTimeStamp(const TCHAR* date, T_TIME_STAMP* Timestamp) {
    T_TIME_GENERAL generalTime;
    TCHAR* yearChar;

    generalTime.sec = 0;
    generalTime.min = 0;
    generalTime.hour = 0;
    generalTime.day = 32;
    generalTime.mon = 12;
    generalTime.year = 1990;

    // Extract month
    for (TUSIGN8 month = 0; month < (sizeof(cmMonths) / sizeof(cmMonths[0])); ++month) {
        if (strncmp((const char*)date, (const char*)cmMonths[month], strlen((const char*)cmMonths[month])) == 0) {
            generalTime.mon = month;

            date += strlen((const char*)cmMonths[month]) + 1;

            break;
        }
    }
    
    // Extract day and year
    generalTime.day = (TUSIGN8)strtol((const char*)date, (char**)&yearChar, 10);
    generalTime.year = (TUSIGN16)strtol((const char*)yearChar, NULL, 10);

    if ((generalTime.year >= 2000 && generalTime.year <= 9999) && (generalTime.day >= 1 && generalTime.day <= 31) && (generalTime.mon <= 11)) {
        TimeGeneralToTimeStamp(&generalTime, Timestamp);

        return eTRUE;
    }
    else {
        return eFALSE;
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Compares two timestamps 
 \author    Michał Kochel
 \date      2020-04-08
 \param     ocTimestampLeft timestamp
 \param     ocTimestampRight timestamp
 \return    Returns an integral value indicating the relationship between two timestamps:
            <0	the first timestamp is before second
            0	timestamps are equal
            >0	the first timestamp is after second
 \warning 
 \test 
 \n test-date:          2020-04-08
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TINT8 TimeStampCmp(const T_TIME_STAMP* TimestampLeft, const T_TIME_STAMP* TimestampRight) {
    if (TimestampLeft->timestamp.era != TimestampRight->timestamp.era) {
        return TimestampLeft->timestamp.era < TimestampRight->timestamp.era ? -1 : 1;
    }
    else {
        if (TimestampLeft->timestamp.seconds == TimestampRight->timestamp.seconds) {
            return 0;
        }
        else {
            return TimestampLeft->timestamp.seconds < TimestampRight->timestamp.seconds ? -1 : 1;
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Calculates diffrence in days between two timestamp
 \author    Michał Kochel
 \date      2020-04-08
 \param     TimestampLeft timestamp
 \param     TimestampRight timestamp
 \return    Returns an integral value indicating the relationship between two timestamps:
            <0	the first timestamp is before second
            0	timestamps are equal
            >0	the first timestamp is after second
 \warning 
 \test 
 \n test-date:          2020-04-16
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
TINT32 TimeStampDaysDiffInDays(const T_TIME_STAMP* startDate, const T_TIME_STAMP* endDate) {
    TINT32 difference = 0;
    TINT32 sign = 1;

    // If end time is before start date
    if (TimeStampCmp(startDate, endDate) > 0) {
        const T_TIME_STAMP* tempDate = startDate;
        startDate = endDate;
        endDate = tempDate;

        sign = -1;
    }

    if (endDate->timestamp.era == startDate->timestamp.era) {
        difference = (TINT32)(endDate->timestamp.seconds - startDate->timestamp.seconds);
    }
    else if (
        (endDate->timestamp.era - startDate->timestamp.era == 1) && 
        (endDate->timestamp.seconds < startDate->timestamp.seconds)
    ) {
        TUSIGN32 unsignedDifference = (0xFFFFFFFF - startDate->timestamp.era) + endDate->timestamp.seconds;

        difference = (TINT32)(unsignedDifference > 0x7FFFFFFF ? 0x7FFFFFFF : unsignedDifference);
    }
    else {
        difference = 0x7FFFFFFF;
    }

    return sign * difference / DAY_IN_SECONDS;
}

//--------------------------------------------------------------------------------------------------
/*! Adjusts given timestamp to show time when given workingSeconds were set
 \brief     
 \author    Michał Kochel
 \date      2020-04-08
 \param     workingSeconds timestamp
 \param     Timestamp timestamp
 \warning 
 \test 
 \n test-date:          2020-04-16
 \n by:                 nobody
 \n environment:        Unit tests
 \n intention:          Instruction coverage
 \n result module test: 
 \n result Lint Level3: 
*/
//--------------------------------------------------------------------------------------------------
void WorkingSecondsToTimeStamp(TUSIGN32 workingSeconds, T_TIME_STAMP* Timestamp) {
    TUSIGN32 currentTime;
    (void)coordinator_Get(CDT_IDX_timeStamp, WHOLE_OBJECT, &currentTime);
    //(void)coordintor_Get(CDT_IDX_timeStampNow, WHOLE_OBJECT, Timestamp);
    
    DecrementTimeStamp((currentTime - workingSeconds), Timestamp);
}
