//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description                    Adjuster of the HMI subsystem. Is responsible for changing the value of the displayed variable

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

/****** Adjuster.h ******/

#ifndef _ADJUSTER_H
#define _ADJUSTER_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _COMPILETIMEDEFINITIONS
  #error compileTimeDefinitions.h must be included
#endif

#ifndef _KEYSTATEDEFINITIONS_H
  #error keystateDefinitions.h must be included
#endif

#ifndef _FRAMETABLEDEFINITIONS_H
  #error frameTableDefinitions.h must be included
#endif


/*!
 \brief         Prepares the class for a new variable
 \author        Roger Arnold
 \param         saveOldVariable - Defines whether (or not) to save the currently accessed variable to its sub-system
 \return        1 on success, 0 on any error (frame has no associaterd adjustable value)
 \test          No details available
*/
TUSIGN16 AdjusterPrepareNewVariable(TUSIGN8 saveOldVariable);


/*!
 \brief         Initializes the class.
 \author        Roger Arnold
 \param         command - Defines whether to increment the value (COMMAND_ADJUST_UP)
                      or to decrement the value (COMMAND_ADJUST_DOWN)
                      all other command values are ignored
 \return        1 on success, 0 on any error.
 \test          No details available
*/
TUSIGN8 AdjusterAdjust(MOVE_COMMAND_TYPE command);

/*!
 \brief         Get the variable currently controlled by the Adjuster class.
 \author        Roger Arnold
 \param
 \return        Address of the data (in string format)
 \test          No details available
*/
TUSIGN8* AdjusterGetVariableAsString(TUSIGN8 *pNegVal);


/*!
 \brief         Get the variable currently controlled by the Adjuster class.
 \author        Roger Arnold
 \param
 \return        Value of the data
 \test          No details available
*/
TUSIGN16 AdjusterGetEnumVariable(TUSIGN8 *nBits);

/*!
 \brief         Initialises the Adjuster with a new variable.
                        Forces the Adjuster class to read the currently accessed variable from its sub-system
 \author        Roger Arnold
 \param
 \return        1 on success, 0 on any error.
 \test          No details available
*/
TUSIGN8 AdjusterInitialiseNewVariable(void* pAdjustFrame);

TUSIGN8 AdjusterDoAutoAdjust(void);

TUSIGN8 AdjusterIsDataAdjusted(void);
void AdjusterResetDataAdjusted(void);

void AdjusterAdjustResolutionForCustomDP(TUSIGN8 *pResolution);


TUSIGN8 *AdjusterBuildNumberString(TFLOAT value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under);

#ifdef HMI_USES_DOUBLE
TUSIGN8 *AdjusterBuildNumberStringFromDouble(TDOUBLE value, TUSIGN8 len, TUSIGN8 resolution, TUSIGN8 *pNegVal, TUSIGN8 *pWithin_over_under);
#endif

#ifdef HMI_SUPPORTS_INTEGER_FRAMES
TUSIGN8 *AdjusterBuildNumberStringFromUSIGN32(TUSIGN32 Value32, NUMBER_ATTRIBUTES_TYPE Attributes, TUSIGN8* pWithin_over_under);
#endif

TUSIGN16 AdjusterWriteAttribute(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue);
TUSIGN16 AdjusterGetAttribute(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void FAST *ptrValue);

TUSIGN16 AdjusterWriteValueAndCheckLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, const ADJUST_DATA *pValue, TUSIGN8 data_Type);

#ifdef HMI_SUPPORTS_INTEGER_FRAMES
TUSIGN16 AdjusterWriteInt32AndCheckLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, const ADJUST_DATA *pValue, TUSIGN8 data_Type);
#endif

TUSIGN16 AdjusterGetFloatValue(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, TUSIGN8 *pType);
TUSIGN16 AdjusterGetEnumValue(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, TUSIGN8 *pType);

TUSIGN16 AdjusterGetFloatValueAndLimits(TUSIGN16 subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, ADJUST_DATA *pValue, ADJUST_DATA *pHigh, ADJUST_DATA *pLow, ADJUST_DATA *pInc, TBOOL* pRulesAreActive, TUSIGN8 *pType);
TUSIGN16 AdjusterGetEnumValueAndRange(TUSIGN16 subsysIdx,
                                      TUSIGN16 objIdx,
                                      TUSIGN8 attribIdx,
                                      ADJUST_DATA *pValue,
                                      T_RANGE_E8* pEnum8Range,
#ifdef HMI_SUPPORTS_16BIT_ENUMS
                                      T_RANGE_E16* pEnum16Range,
#endif
                                      TBOOL* pRulesAreActive,
                                      TUSIGN8 *pType);



void AdjusterUnpackPASCII(const TUSIGN8 *pPackedStr, TUSIGN8 packedLen, TUSIGN8 *pUnpackedStr);
void AdjusterPackASCII(const TUSIGN8 *pUnpackedStr, TUSIGN8 unpackedLen, TUSIGN8 *pPackedStr);

//void AdjusterUnpackPASCII_8(const TUSIGN8 *pPackedStr, TUSIGN8 *pUnpackedStr);
//void AdjusterPackPASCII_8(TUSIGN8 *pPackedStr, TUSIGN8 *pUnpackedStr);

void AdjusterSetLimitError(TUSIGN8 err,TUSIGN16 Code);
TUSIGN8 AdjusterGetLimitError(TUSIGN16 *pErrorCode);

#ifdef HMI_USES_COMMONDIAGNOSTICS
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the worst case alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 *pNAMURClassification - The NAMUR classification of the device is saved here
         TUSIGN8 *pNAMURCategory - The NAMUR category of the device is saved here

 \return void
 \warning
 \test
         test-date    :
         by           :  Roger Arnold
         environment  :
         result       :
 \test   Lint level 3 :
 \bug
*/
TUSIGN8 AdjusterGetNAMURStatus(TUSIGN8 *pNAMURClassification, TUSIGN8 *pNAMURCategory);
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the details of an alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 AlarmNumber - selected alarm
         TUSIGN8 *pAlarmClassification - The classification of the selected alarm is saved here
         TUSIGN8 *pAlarmCategory - The category of the selected alarm is saved here
         TUSIGN8 *pAlarmPriority - The priority of the selected alarm is saved here
         TUSIGN8 *pAlarmID - The ID of the selected alarm is saved here

 \return TBOOL  eFALSE - no alarms active, eTRUE - selected alarm is active
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TBOOL AdjusterGetAlarmConditionDetails(TUSIGN8 SelectedCondition, TUSIGN8 *pAlarmClassification,  TUSIGN8 *pAlarmCategory, TUSIGN8 *pAlarmPriority, TUSIGN8 *pAlarmID);

TBOOL AdjusterGetAlarmHistoryDetails(TUSIGN8 HistoryNumber, TUSIGN8 SelectedCondition, TUSIGN8 *pAlarmClassification, TUSIGN8 *pAlarmCategory, TUSIGN8 *pAlarmPriority,  TUSIGN8 *pAlarmID, TUSIGN16 *pAlarmCount, TUSIGN32 *pAlarmTime_msec, TUSIGN16 *pAlarmTime_day, TUSIGN32 *pAlarmMostRecent_msec, TUSIGN16 *pAlarmMostRecent_day);
TINT16 AdjusterFindFirstDiagHistoryEntry(TUSIGN8 HistoryNumber);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the details of an alarm from the common diagnostics subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TBOOL ActiveAlarmsOnly - if eFALSE the total number of alarms is returned
                                - if eTRUE the number of active alarms is returned
 \return TUSIGN8 - The number of active alarm conditions
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN8 AdjusterGetNumberOfAlarmConditions(TBOOL ActiveAlarmsOnly);

#endif

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the security state from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TBOOL - eFALSE = ARM_HIGH_SECURITY_DISABLED, eTRUE = ARM_HIGH_SECURITY_ENABLED
 \warning
 \test
         test-date    :  2007-01-25
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TBOOL AdjusterGetARMhighSecurityState(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current ARM ID string from the common ARM subsystem
 \author Roger Arnold
 \date   2007-08-21
 \param  TUSIGN8 *pIDString - where to store the string
         TUSIGN8 nChars - the number of characters expected
                          NOTE: an attempt is made to read this number of characters from the access rights manager.
                          Any characters not ssuccessfully read will be substituded with a space
 \return none
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
void AdjusterGetArmID(TUSIGN8 *pIDString, TUSIGN8 nChars);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current hashcode pin from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TUSIGN16 - The current hashcode pin
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN16 AdjusterGetARMpin(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the current access level from the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return TUSIGN8 - The current access level
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TUSIGN8 AdjusterGetARMLevel(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Attempts to set an access level in the common ARM subsystem
 \author Roger Arnold
 \date   2005-04-14
 \param  TUSIGN8 AccessLevel - the required access level
         TUSIGN8 *pPassword  - pointer to a password string
 \return TBOOL  eFALSE = unsucessful login, eTRUE = sucessful login
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
TBOOL AdjusterSetARMlevel(TUSIGN8 AccessLevel ,TUSIGN8 *pPassword);
#endif

#define DIAG_NONE                   0
#define DIAG_DEVICE_DEFECT          1
#define DIAG_SIMULATION_ACTIVE      2
#define DIAG_MAINTENANCE_REQD       3
#define DIAG_PRESSURE_OVERLOAD      4
#define DIAG_PRESSURE_LIMITS        5
#define DIAG_TEMPERATURE_LIMITS     6

#ifdef HMI_SUPPORTS_SERVICEPORT
/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the Adjuster local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void AdjusterSetLocalDataPointer(TUSIGN8 newInstance);
#endif

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
TUSIGN32 AdjusterGetTimeDateVariable(void);
TBOOL AdjusterConvertDateToSeconds(TIMEANDDATE* pTimeAndDate, TUSIGN32 *pTime);
TBOOL AdjusterConvertSecondsToDate(TUSIGN32 time, TIMEANDDATE *pTimeAndDate);

#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
TUSIGN8 Adjuster_WaveformAction(TUSIGN8 waveformCursor, TUSIGN8 keystate);
#endif

#ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
TUSIGN8 Adjuster_GetDiagDetailsViewCursorMax(void);
TBOOL Adjuster_GetDiagDetails(TUSIGN8 AlarmID, TUSIGN8 detailNumber, TUSIGN8* p_subIdx, TUSIGN8* p_alarm);
#endif
#endif
