//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Generic
 Subsystem				HMI
 Description			Frame Table of the HMI subsystem, contains definitions for all frames displayed on the local HMI.
 						Contains and manages string tables for all fixed strings that may be displayed in all supported languages

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

/****** FrameTable.h ******/

#ifndef _FRAMETABLE_H
#define _FRAMETABLE_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _FRAMETABLEDEFINITIONS_H
  #error frameTableDefinitions.h must be included
#endif

#ifndef _COMPILETIMEDEFINITIONS
  #error compileTimeDefinitions.h must be included
#endif

typedef enum
{
    STRING_TYPE_EUROPEAN = 0,
    STRING_TYPE_KATAKANA = 1,
    STRING_TYPE_TURKISH = 2,
    STRING_TYPE_RUSSIAN = 3,
    STRING_TYPE_LATIN1 = 4,
    STRING_TYPE_ASIAN = 5,
    STRING_TYPE_LIBIDX = 7
}E_STRING_TYPE;
extern TUSIGN8 const *PacketManagerGetEngUnits_s(TUSIGN16 subsysIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx, TUSIGN8 type, E_STRING_TYPE* pStringType);
extern void GetStrLibVer(TUSIGN8* pStrLibVer, TUSIGN8* pStdLibVer);

#ifdef LEFT_RIGHT_LOCKOUT
TUSIGN8 FrameTableGetLeftRightLockoutState(void);
TBOOL FrameTableGetLeftRightLockout(void);
void FrameTableUpdateLeftRightLockout(TUSIGN8 keyState);
void FrameTableSetFunctionKeyLock(TBOOL lockState);
#endif


/*!
 \brief		Initializes the class.
 			1. Sets the frame pointer to the first operator frame
 \author	Roger Arnold
 \param		TBOOL nvTest - If TRUE initialises the current frame to the NV test progress frame
                           If FALSE initialises the current frame to the first operator frame
 \return	none.
 \test		No details available
*/
void FrameTableInitialise(TBOOL nvTest);

/*!
 \brief         Tells the HMI that NV initilaization has terminated.
 \author        Roger Arnold
 \test          No details available
*/
void FrameTableNVInitComplete(void);

/*!
 \brief         Tells the HMI that NV initilaization has terminated.
 \author        Roger Arnold
 \return	    the NVinit frame status (eTRUE = active, eFALSE = inactive)
 \test          No details available
*/
TBOOL FrameTableIsNVInitActive(void);


/*!
 \brief		Get the currently active frame
 \author	Roger Arnold
 \param		
 \return	pointer to the definition of the currently active frame
 \test		No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetCurrentFrame(void);
#ifdef HMI_SUPPORTS_SERVICEPORT
FRAME_FORMAT_TYPE const *FrameTableGetCurrentFrame_LOCAL(void);
FRAME_FORMAT_TYPE const *FrameTableGetCurrentFrame_REMOTE(void);
#endif

/*!
 \brief		Gets the address of the first/last configuration header frame
 \author	Roger Arnold
 \param		first_last - 0, Gets the address of the first configuration header frame
 			             1, Gets the address of the last configuration header frame
 \return	address of the selected configuration header frame
 \test		No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetConfigHeaderAddress(TUSIGN8 first_last);

/*!
 \brief         Gets the address of the first operator frame
 \author        Roger Arnold
 \param         none
 \return        address of the first operator frame
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetFirstOpPageAddress(void);

/*!
 \brief         Gets the address of the operator menu
 \author        Roger Arnold
 \param         none
 \return        address of the operator menu
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetOperatorMenuAddress(void);

/*!
 \brief         Gets the address of the operator menu
 \author        Roger Arnold
 \param         none
 \return        address of the operator menu
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetSignalsViewAddress(void);

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
/*!
 \brief         Gets the address of the ARM Level selection menu
 \author        Roger Arnold
 \param         none
 \return        address of the operator menu
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetARMSelectionAddress(void);

/*!
 \brief         Gets the address of the ARM Password Entry Frame
 \author        Roger Arnold
 \param         none
 \return        address of the operator menu
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetARMPasswordEntryAddress(void);

/*!
 \brief         Sets the requested ARM Level
 \author        Roger Arnold
 \param         required ARM Level
 \return        none
 \test          No details available
*/
void FrameTableSetRequestedARMLevel(TUSIGN8 RequestedLevel);

/*!
 \brief         Returns the requested ARM Level
 \author        Roger Arnold
 \param         none
 \return        requested ARM Level
 \test          No details available
*/
TUSIGN8 FrameTableGetRequestedARMLevel(void);

#endif

#ifdef HMI_USES_COMMONDIAGNOSTICS
/*!
 \brief         Gets the address of the diagnostics view menu
 \author        Roger Arnold
 \param         none
 \return        address of the operator menu
 \test          No details available
*/
FRAME_FORMAT_TYPE const *FrameTableGetDiagnosticsViewAddress(void);

/*!
 \brief         Gets the address of the diagnostics icon
 \author        Roger Arnold
 \param         classification 0 - CLASSIFICATION_NONE
                               1 - CLASSIFICATION_CHECKFUNCTION
                               2 - CLASSIFICATION_OFFSPEC
                               3 - CLASSIFICATION_MAINTENANCEREQD
                               4 - CLASSIFICATION_FAILURE
                               5 = CLASSIFICATION_INFORMATIONONLY

 \return        address of the icon
 \test          No details available
*/
TUSIGN8 const *FrameTableGetDiagnosticIconAddress(TUSIGN8 classification, TUSIGN8 iconLine);

/*!
 \brief         Gets the string ID for the description of the selected diagnostics condition
 \author        Roger Arnold
 \param         TUSIGN8 condition - the diagnostics condition
 \return        STR_REF - string ID for the description of the selected diagnostics condition
 \test          No details available
*/
#ifdef COMMON_TOP_WORKS_NEW_FEATURES    // DIAGNOSTIC_SPLIT_DESC_AND_RECOVERY
DIAGNOSTICS_TABLE_ENTRY_TYPE FrameTableGetDiagnosticTableEntry(TUSIGN8 condition);
#else
STR_REF FrameTableGetDiagnosticDescriptionStringID(TUSIGN8 condition);
#endif

/*!
 \brief         Gets the string ID for the selected diagnostics Category
 \author        Roger Arnold
 \param         TUSIGN8 Category - the diagnostics Category
 \return        STR_REF - string ID for the selected diagnostics Category
 \test          No details available
*/
STR_REF FrameTableGetNAMURCategoryString(TUSIGN8 Category);

#endif

/*!
 \brief		Changes the currently active frame
 \author	Roger Arnold
 \param		pointer to the definition of the new active frame
 \return	1 on success, 0 on any error.
 \test		No details available
*/
TUSIGN8 FrameTableSetCurrentFrame(FRAME_FORMAT_TYPE const *newFrame, TUSIGN8 cursor, TUSIGN16 extraSpecialOp);

/*!
 \brief		Enables editing of the data in the currently active frame
 \author	Roger Arnold
 \param		0 disable editing, 1 enable editing, other values do nothing
 \return	new edit enabled state - 0 editing disabled , 1 editing enabled
 \test		No details available
*/
TUSIGN8 FrameTableEnableEdit(TUSIGN8 enable);

/*!
 \brief		Returns the edit state of the data in the currently active frame
 \author	Roger Arnold
 \param		
 \return	edit enabled state - 0 editing disabled , 1 editing enabled
 \test		No details available
*/
TUSIGN8 FrameTableIsEditEnabled(void);

/*!
 \brief		Returns the cursor position in the currently active frame
 \author	Roger Arnold
 \param		
 \return	cursor position
 \test		No details available
*/
TUSIGN8 FrameTableGetCursorPosition(void);

/*!
 \brief		Returns the maximum cursor position in the currently active frame
 \author	Roger Arnold
 \param		
 \return	maximum cursor position
 \test		No details available
*/
TUSIGN8 FrameTableGetCursorPositionLimit(void);

/*!
 \brief		Sets a cursor position in the currently active frame
 			Will allow the cursor position to be set to any value from 0 to the maximum cursor position (inclusive)
 \author	Roger Arnold
 \param		
 \return	1 on success, 0 on any error (new position > maximum cursor position)
 \test		No details available
*/
TUSIGN8 FrameTableSetCursorPosition(TUSIGN8 position);

/*!
 \brief		Gets a text string from the string table (or sub-system)
 \author	Roger Arnold
 \param		id - identifies the text string
 \return	pointer to the text string or NULL if error (string invalid, cannot access sub-system)
			if the configured language is invalid then the string for the default language is returned)
 \test		No details available
*/
TUSIGN8 FrameTableGetLanguageIndex(TUSIGN8 language);
TUSIGN8 const *FrameTableGetString(TEXT_IDENTIFIER_TYPE id);
TUSIGN8 const *FrameTableGetStringSpecificLanguage(TEXT_IDENTIFIER_TYPE id, TUSIGN8 SpecificLanguage);
extern TUSIGN8 const *FrameTableGetStringSpecificLanguage_s(E_STRING_TYPE* pStringType, TEXT_IDENTIFIER_TYPE id, TUSIGN8 SpecificLanguage);

/*!
 \brief         Gets an item from the Enum table
 \author        Roger Arnold
 \param         TUSIGN16 enumItem - index in the enum Table
 \return        STR_REF the selected item from the menu table
 \test          No details available
*/
STR_REF FrameTableGetEnumItem(TUSIGN16 enumItem);

/*!
 \brief		Gets an item from the menu table
 \author	Roger Arnold
 \param		T_USIGN16 menuItem - index in the menu Table
 \return	pointer to the item in the menu table, if error a NULL pointer is returned
 \test		No details available
*/
MENU_ITEM_TYPE const *FrameTableGetMenuItem(TUSIGN16 menuItem);

/*!
 \brief		Gets a character Set
 \author	Roger Arnold
 \param		T_USIGN8 charSet - identifies the required character set
 \return	pointer to the first character in the selected character set, if error(invalid Character Set) a NULL pointer is returned
 \test		No details available
*/
TUSIGN8 const *FrameTableGetCharacterSet(TUSIGN8 charSet);
TUSIGN8 FrameTableGetExtendedCharacterPage(TUSIGN8 charSet);
TUSIGN8 FrameTableGetStringObjectExtendedCharacterPage(ATTRIB_IDENTIFIER_TYPE StringObject);


/*!
 \brief		Gets the id of a part of a config icon
 \author	Roger Arnold
 \param		TUSIGN8 icon - identifies the required icon
            TUSIGN8 posn - identifies which part of the icon
 \return	the id of the selected part of the selected icon
 \test		No details available
*/
TUSIGN8 FrameTableGetConfigIconPart(TUSIGN8 icon, TUSIGN8 posn);

/*!
 \brief		Gets the address of an icon part
 \author	Roger Arnold
 \param		TUSIGN8 *pIconPart - identifies the required icon part
 \return	pointer to the first byte of the selected icon, if error(invalid icon number) a NULL pointer is returned
 \test		No details available
*/
TUSIGN8 const *FrameTableGetConfigIconAddress(TUSIGN8 *pIconPart);

/*!
 \brief		Gets the address of the signalTable
 \author	Roger Arnold
 \param		signal - identifies which signal (which entry in table)
 \return	pointer to the selected signal table entry
 \test		No details available
*/
SIGNAL_TABLE_TYPE const *FrameTableGetSignal(TUSIGN8 signal);

#if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
void FrameTableEnableAutoscroll(void);
void FrameTableDisableAutoscroll(void);
TBOOL FrameTableIsAutoscrollEnabled(void);
#endif

#ifdef HMI_SUPPORTS_SERVICEPORT
/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the Frame Table local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void FrameTableSetLocalDataPointer(TUSIGN8 newInstance);
#endif


#ifdef HMI_SUPPORTS_EASYSETUP
/*----------------------------------------------------------------------------------------------------------!
 \brief         Gets details of the easy setup menu
 \author        Roger Arnold
 \param         p_lastStage   - the number of the last frame in the easy setup menu
                p_actualStage - the current position in the easy setup menu
 \return        void
 \test          No details available
*/
void FrameTableGetEasySetupStages(TUSIGN8 *p_lastStage, TUSIGN8 *p_actualStage);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Gets the validity test index for an easy setup frame
 \author        Roger Arnold
 \param         frameNumber - the selected frame in an easy setup menu
                pageNumber  - the selected easy setup menu
 \return        TUSIGN8 the validity test index for the selected easy setup frame
 \test          No details available
*/
TUSIGN8 FrameTableGetEasySetupValidityTestIndex(TUSIGN8 frameNumber, TUSIGN8 pageNumber);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Increments the current position in the easy setup menu
 \author        Roger Arnold
 \param         none
 \return        TBOOL eTRUE - the increment was successful
                     eFALSE - the increment was unsuccessful because current position already at limit
 \test          No details available
*/
TBOOL FrameTableIncrementEasySetupStage(void);

#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
FRAME_FORMAT_TYPE const* FrameTableGetCalFrameAddress(TUSIGN8 CalibrationType);
#endif


TBOOL FrameTableIsOperatorFrameActive(TUSIGN8 instance);
TBOOL FrameTableGetAutoScrollState(TUSIGN8 instance);
TUSIGN8 FrameTableGetOperatorPageNumber(TUSIGN8 instance);
TBOOL FrameTableSetStartupFrame(TUSIGN8 instance, TBOOL autoscrollState, TUSIGN8 operatorPage);

void FrameTableAccessMenuPositionDetails(TUSIGN8* pCursor, TUSIGN8* pCursorLim, TBOOL Get);

#ifdef HMI_SUPPORTS_MANUAL_VALVE_CAL
void FrameTableActivateManualValveCal(void);
void FrameTableDeactivateManualValveCal(void);
TBOOL FrameTableIsManualValveCalActive(void);
#endif

#ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
void FrameTableDeactivateConfigChangeCounter(void);
void FrameTableActivateConfigChangeCounter(TUSIGN16 ss_Idx, TUSIGN16 ob_Idx, TINT16 at_Idx);
TBOOL FrameTableIncrementConfigChangeCounter(void);
#endif

#ifdef HMI_SUPPORTS_SEQUENCES
void FrameTableEnableSequence(void);
void FrameTableDisableSequence(void);
TBOOL FrameTableIsSequenceEnabled(void);
void FrameTableSetSequenceSettings(TUSIGN8 SeqMenuPosnCursor, TUSIGN8 SeqMenuPosnCursorLim, TUSIGN16 SeqMenuTableOffset);
TBOOL FrameTableGetSequenceSettings(TUSIGN8* pSequentialMenuPosnCursor, TUSIGN8* pSequentialMenuPosnCursorLim, TUSIGN16* pSequentialMenuTableOffset);
#endif

#ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
void  FrameTableSetOutputHoldSettings(TBOOL enable, TUSIGN8 ssIdx, TUSIGN8 objIdx, TUSIGN8 attIdx);
TBOOL FrameTableGetOutputHoldSettings(TUSIGN8 *pssIdx, TUSIGN8 *pobjIdx, TUSIGN8 *pattIdx);
void FrameTableActivateOutputHold(void);
void FrameTableReleaseOutputHold(void);
TBOOL FrameTable_IsOutputHeld(void);
#endif

#ifdef HMI_SUPPORTS_AUTO_ADJUST
void FrameTableSetAutoAdjustAbortSettings(TBOOL enable, TUSIGN8 ssIdx, TUSIGN8 objIdx, TUSIGN8 attIdx);
TBOOL FrameTableGetAutoAdjustAbortSettings(TUSIGN8 *pssIdx, TUSIGN8 *pobjIdx, TUSIGN8 *pattIdx);
#endif

#ifdef HMI_SUPPORTS_MANUAL_ADJUST_MODIFIER
void FrameTableSetManualAdjustModifier(TUSIGN8 Modifier);
TUSIGN8 FrameTableGetManualAdjustModifier(void);
#endif

#ifdef HMI_SUPPORTS_SECONDARY_IDENTIFIER

#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
#define SECONDARY_IDENTIFIER_LEN    16
#else
#define SECONDARY_IDENTIFIER_LEN    11
#endif

void FrameTableSetSecondaryIdentifier(TBOOL enable, TUSIGN8* pSecondaryIdentifierText);
TBOOL FrameTableGetSecondaryIdentifier(TUSIGN8* pSecondaryIdentifierText);
#endif

#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
void FrameTableSetTimeDurationLimit(TUSIGN8 durationLimit);
TUSIGN32 FrameTableGetTimeDurationLimit(void);
#endif


#ifdef ENUM_ACCELERATOR
void FrameTableUpdateEnumAccellerator(TUSIGN8 keystate);
TUSIGN8 FrameTableGetEnumUpdateAmount(void);
#endif //#ifdef ENUM_ACCELERATOR

#ifdef STRING_ACCELERATOR
void FrameTableUpdateStringAccellerator(TUSIGN8 keystate);
TUSIGN8 FrameTableGetStringUpdateAmount(void);
#endif //#ifdef STRING_ACCELERATOR

#ifdef IPADDR_ACCELERATOR
void FrameTableUpdateIPAddressAccellerator(TUSIGN8 keystate);
TUSIGN8 FrameTableGetIPAddressUpdateAmount(void);
#endif //#ifdef IPADDR_ACCELERATOR

#ifdef DATE_TIME_ACCELERATOR
void FrameTableUpdateDateTimeAccellerator(TUSIGN8 keystate);
TUSIGN8 FrameTableGetDateTimeUpdateAmount(TUSIGN8 field, TUSIGN8 hoursMax);
#endif //#ifdef DATE_TIME_ACCELERATOR

#ifdef MANUAL_ADJUST_ACCELERATOR
void FrameTableUpdateManAdjustAccellerator(TUSIGN8 keystate);
TUSIGN16 FrameTableGetManAdjustUpdateAmount(void);

void FrameTableSetSpecialManualAdjust(TUSIGN8 maxAccelerationRate, TUSIGN8 DisplayBargraphValueCharacters, TUSIGN8 DisplayBargraphValuePrecision);
TUSIGN8 FrameTableGetSpecialManualAdjustDisplayBargraphValueCharacters(void);
TUSIGN8 FrameTableGetSpecialManualAdjustDisplayBargraphValuePrecision(void);
#endif

#ifdef HMI_SUPPORTS_AUTO_CALIBRATION
void  FrameTableAllowNavigationFromAutoCal(TBOOL allow);
TBOOL FrameTableIsNavigationFromAutoCalAllowed(void);
void  FrameTableRenameAutocal(TBOOL renameAutocal);
TBOOL FrameTableIsAutoCalRenamed(void);
void  FrameTableRemoveSettling(TBOOL removeSettling);
TBOOL FrameTableIsSettlingRemoved(void);
#endif

#ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
void FrameTableEnableSampleCollection(TBOOL enable);
TBOOL FrameTableIsSampleCollectionEnabled(void);
#endif

#ifndef DISABLE_BACK_MOVE_SELECT
TBOOL FrameTableIsBackMoveSelectAvailable(const FRAME_FORMAT_TYPE *pCallingFrame);
#endif

#ifdef EDP300_SPECIAL_FEATURES_HMI
FRAME_FORMAT_TYPE const *FrameTableGetEDP300ModeFrame(void);
FRAME_FORMAT_TYPE const *FrameTableGetEDP300QuickAdjustFrame(void);
void FrameTableResetAutoAdjustTimer(void);
TBOOL FrameTableUpdateAutoAdjustTimer(void);
TBOOL FrameTableIsAutoAdjustTimerRunning(void);
TUSIGN8 FrameTableAutoAdjustTimeRemaining(void);
TBOOL FrameTableGetAdaptionCheck(void);
TBOOL FrameTableSetAdaptionCheck(void);
TBOOL FrameTableGetControlCheck(void);
TBOOL FrameTableSetControlCheck(void);
TBOOL FrameTableGetManualSPCheck(void);
TBOOL FrameTableSetManualSPCheck(void);
TBOOL FrameTableGetManualSensorCheck(void);
TBOOL FrameTableSetManualSensorCheck(void);
TBOOL FrameTableIsWriteProtected(ATTRIB_IDENTIFIER_TYPE Data);
TBOOL FrameTableIsAutoAdjustStartAllowed(void);


void FrameTableGetEDP300Mode(TBOOL* pControlCheck,TBOOL* pManSPCheck,TBOOL* pManSenCheck);
TBOOL FrameTableIsAccessToModePermitted(void);

TUSIGN8 FrameTableGetAutoAdjustScope(void);
TUSIGN8 FrameTableGetAutoAdjustStatus(void);
TUSIGN8 FrameTableGetAutoAdjustPercentComplete(void);
TUSIGN8 FrameTableGetAutoAdjustErrorReason(void);

void FrameTableAbortAutoAdj(void);          // abort Auto Adjustment
void FrameTableFinishAutoAdj(void);         // Acknowledge completion
void FrameTableStartAdjust(TBOOL Rotary);   // Start AutoAdjust()

TBOOL FrameTableIfAutoAdjMarker(TUSIGN8 SSIdx, TUSIGN8 ObIdx, TUSIGN8 AtIdx);

TBOOL FrameTableIsActuatorRotary(void);
TBOOL FrameTableModeIsExhaustPosition0(void);


#endif // #ifdef EDP300_SPECIAL_FEATURES_HMI

#ifdef LOW_LEVEL_HMI_TEST
FRAME_FORMAT_TYPE const *FrameTableGetLowLevelHMITestFrame(void);
#endif

TBOOL FrameTableIsEditDeniedBySpecialOp(void);
void FrameTableSpecialOpDenyEdit(void);

TUSIGN8 FrameTableGetParameterDumpFont(void);
void FrameTableSetParameterDumpFont(TUSIGN8 font);

#endif

#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
void FrameTableUpdateMenuPositionDetailsForTFT(TUSIGN8 cursorPos, TUSIGN8 cursorPosLimit, TUSIGN16 firstItem);
void FrameTableGetMenuPositionDetailsForTFT(TUSIGN8* p_selectedValidItem, TUSIGN8* p_nValidItems);
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
TUSIGN8 FrameTable_GetWaveformPRVSignal(TUSIGN8 waveformNumber);
TUSIGN8 FrameTable_GetWaveformCRVSignal(TUSIGN8 waveformNumber);
void FrameTable_NextWaveformPRVSignal(TUSIGN8 waveformNumber);
void FrameTable_PrevWaveformPRVSignal(TUSIGN8 waveformNumber);
void FrameTable_NextWaveformCRVSignal(TUSIGN8 waveformNumber);
void FrameTable_PrevWaveformCRVSignal(TUSIGN8 waveformNumber);
WAVEFORM_CURSOR_POS FrameTable_GetWaveformCursor(TUSIGN8 waveformNumber);
void FrameTable_NextWaveformCursor(TUSIGN8 waveformNumber);
TBOOL FrameTable_IsWaveformCursorChanged(TUSIGN8 waveformNumber);
void FrameTable_AcknowledgeWaveformCursorChange(TUSIGN8 waveformNumber);
void FrameTable_SetupCRV_NumericFrame(TUSIGN8 Signal);
void FrameTable_WaveformCRV_Edit(TUSIGN8 waveformNumber, TBOOL enable);
TBOOL FrameTable_IsWaveformEditStateChanged(TUSIGN8 waveformNumber);
void FrameTable_AcknowledgeWaveformEditStateChange(TUSIGN8 waveformNumber);
#endif

#ifdef HMI_SUPPORTS_MATRIX_FRAMES
MATRIX_CURSOR_POS FrameTable_GetMatrixCursor(void);
TBOOL FrameTable_NextMatrixCursor(void);
TUSIGN8 FrameTable_GetMatrixColumn(void);
void FrameTable_NextMatrixElement(TBOOL up);
TUSIGN8 FrameTable_GetMatrixRow(void);
TBOOL FrameTable_ResetMatrixElement(void);
TBOOL FrameTable_MatrixEdit(TBOOL enable);
void FrameTable_ModifyDataForWriteProtection(ATTRIB_IDENTIFIER_TYPE* pData);
#endif

#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
LINEARIZER_CURSOR_POS FrameTable_GetLinearizerCursor(void);
TBOOL FrameTable_NextLinearizerCursor(void);
TUSIGN8 FrameTable_GetLinearizerPoint(void);
void FrameTable_NextLinearizerPoint(TBOOL up);
TBOOL FrameTable_LinearizerEdit(TBOOL enable);
TBOOL FrameTable_CaptureLinearizerInputPoint(void);
TBOOL FrameTable_SetLinearizerPointEnable(TBOOL enable);
TBOOL FrameTable_GetLinearizerPointEnable(TUSIGN8 PointEnable_ssIdx, TUSIGN8 PointEnable_objIdx, TUSIGN8 PointEnable_attribIdx);

TBOOL FrameTable_isLinearizerProcessValueDisplayEnabled(void);
TBOOL FrameTable_isLinearizerInpointCapture_Enabled(void);
TBOOL FrameTable_isLinearizerInpointEdit_Enabled(void);
TBOOL FrameTable_isLinearizerOutpointEdit_Enabled(void);
TBOOL FrameTable_isLinearizerPointEnableDisable_Enabled(void);
#endif

#ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
TBOOL FrameTable_isDiagDetailsViewActive(void);
TUSIGN8 FrameTable_GetDiagDetailsCursor(void);
TUSIGN8 FrameTableActivateDiagDetailsView(TBOOL activate);
void FrameTable_nextDiagDetail(TUSIGN8 up);
#endif


void FrameTableRestrictNumericCharacterSet(TBOOL AllowSpace, TUSIGN8 MinValid, TUSIGN8 MaxValid);
void FrameTableGetNumericCharacterSetRestrictions(TBOOL* pAllowSpace, TUSIGN8* pMinValid, TUSIGN8* pMaxValid);

