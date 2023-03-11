//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description                    Frame Mover of the HMI subsystem. Is responsible for controlling the frame pointer and associated
                                                variables within the Frame Table
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003
/****** FrameMover.c ******/
#include <intrinsics.h>

#include "IncludeList_Implementation.h"

#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"


#include "T_Unit/Interface/t_unit.h"
#include "Hmi/Interface/HMI_idx.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "Hmi/Interface/HMI.h"
#include "keystateDefinitions.h"
#include "FrameTableDefinitions.h"
#include "ConfigModeManager.h"

#include "FrameTable.h"
#include "Adjuster.h"

#include "IncludeList_Frames_1.h"

#include "FrameMover.h"

        // private variables
typedef struct
{
	TUSIGN16	seconds;
	TUSIGN16	milliseconds;
}FRAME_MEMORY_TIMER;

#ifdef HMI_SUPPORTS_SERVICEPORT
typedef struct
{
    // elements that must be initialised
    FRAME_MEMORY_TIMER  frameMemoryResetTimer;
    TUSIGN8             currentOperatorPageNumber;
    TUSIGN8             frameMemoryPointer;
    // elements that need not be initialised
    TBOOL               frameMemoryResetRequired;
    FRAME_MEMORY_ENTRY  frameMemory[FRAME_MEMORY_SIZE];
    TBOOL               savedAutoscroll;
    TUSIGN8             easySetup_number;
}FRAME_MOVER_LOCAL_TYPE;

#pragma diag_suppress=Pm022
#pragma diag_suppress=Pm023
FRAME_MOVER_LOCAL_TYPE  FrameMoverLocal_1 = {
     0      // TUSIGN16 frameMemoryResetTimer.seconds
    ,0      // TUSIGN16 frameMemoryResetTimer.milliseconds
    ,1      // TUSIGN8  currentOperatorPageNumber
    ,0      // TUSIGN8  frameMemoryPointer
    };

FRAME_MOVER_LOCAL_TYPE  FrameMoverLocal_2 = {
     0      // TUSIGN16 frameMemoryResetTimer.seconds
    ,0      // TUSIGN16 frameMemoryResetTimer.milliseconds
    ,1      // TUSIGN8  currentOperatorPageNumber
    ,0      // TUSIGN8  frameMemoryPointer
    };
#pragma diag_default=Pm023
#pragma diag_default=Pm022

FRAME_MOVER_LOCAL_TYPE  *pFrameMoverLocal = &FrameMoverLocal_1;

#define LOCAL_frameMemoryResetTimer     pFrameMoverLocal->frameMemoryResetTimer
#define LOCAL_currentOperatorPageNumber pFrameMoverLocal->currentOperatorPageNumber
#define LOCAL_frameMemoryPointer        pFrameMoverLocal->frameMemoryPointer
#define LOCAL_frameMemoryResetRequired  pFrameMoverLocal->frameMemoryResetRequired
#define LOCAL_frameMemory               pFrameMoverLocal->frameMemory
#define LOCAL_savedAutoscroll           pFrameMoverLocal->savedAutoscroll

#define LOCAL_easySetup_number          pFrameMoverLocal->easySetup_number


#else

#define LOCAL_frameMemoryResetTimer     frameMemoryResetTimer
#define LOCAL_currentOperatorPageNumber currentOperatorPageNumber
#define LOCAL_frameMemoryPointer        frameMemoryPointer
#define LOCAL_frameMemoryResetRequired  frameMemoryResetRequired
#define LOCAL_frameMemory               frameMemory
#define LOCAL_savedAutoscroll           savedAutoscroll
#define LOCAL_easySetup_number          easySetup_number

FRAME_MEMORY_TIMER  frameMemoryResetTimer = {0,0};		// 0.0Sec   = Initial value
TUSIGN8             currentOperatorPageNumber = 1;
TUSIGN8             frameMemoryPointer = 0;
TBOOL               frameMemoryResetRequired;
FRAME_MEMORY_ENTRY  frameMemory[FRAME_MEMORY_SIZE];
TBOOL               savedAutoscroll;
TUSIGN8             easySetup_number;

#endif

        // private methods

FRAME_MEMORY_ENTRY FrameMemory_Forward(FRAME_MEMORY_ENTRY pushedFrame);
FRAME_MEMORY_ENTRY FrameMemory_Back(TUSIGN8 first);
void FrameMemory_Sideways(FRAME_MEMORY_ENTRY pushedFrame);


TUSIGN8 LaunchOperatorMenu(TBOOL saveAutoscroll);
TUSIGN8 Menu(MOVE_COMMAND_TYPE command);
TUSIGN8 Exit(void);
TUSIGN8 NextConfigLevel(TUSIGN8 up);
TUSIGN8 SelectConfigLevel(void);
TUSIGN8 SelectFromMenu(void);
TUSIGN8 SelectFromOperatorMenu(void);
TUSIGN8 Edit(TUSIGN8 enable);
TUSIGN8 MoveInMenu(TUSIGN8 up);
TUSIGN8 MoveCursor(TUSIGN8 toEnd);
TUSIGN8 Back(void);
TUSIGN8 NextOperatorFrame(TBOOL Auto);
#ifdef HMI_SUPPORTS_EASYSETUP
TUSIGN8 NextEasySetup(void);
#endif

#ifdef HMI_SUPPORTS_SEQUENCES
TUSIGN8 NextSequence(void);
#endif

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
TUSIGN8 EnterConfig_AccessAllowed(void);
TUSIGN8 EnterConfig_CheckPassword(void);
TUSIGN8 EnterConfig_SelectionMade(void);
#endif
TUSIGN8 ReturnToOperatorPage(void);

#ifdef HMI_SUPPORTS_SERVICEPORT
// No restriction on the optimization of this function
void FrameMoverSetLocalDataPointer(TUSIGN8 newInstance)
{
    if      (newInstance == 1)  {pFrameMoverLocal = &FrameMoverLocal_1;}
    else if (newInstance == 2)  {pFrameMoverLocal = &FrameMoverLocal_2;}
    else                        {assert(0);}
}
#endif

// No restriction on the optimization of this function
TUSIGN8 FrameMover_GetFrameMemoryPointer(void)
{
    return LOCAL_frameMemoryPointer;
}

// No restriction on the optimization of this function
void FrameMover_ResetFrameMemory(TUSIGN8 startPointer)
{
    TUSIGN8 i;
    for (i = startPointer; i < FRAME_MEMORY_SIZE; i++)
    {
        LOCAL_frameMemory[i].pFrame = NULL;
        LOCAL_frameMemory[i].cursor = 0xFF;
    }

    if (startPointer == 0)
    {
        LOCAL_frameMemoryPointer = 0;
        LOCAL_frameMemoryResetRequired = eFALSE;
    }
}

// No restriction on the optimization of this function
void FrameMover_ProcessFrameMemoryTimedReset(void)
{
    if (ConfigModeManagerIsConfigurationMode())
    {
        LOCAL_frameMemoryResetTimer.seconds = 0;
        LOCAL_frameMemoryResetTimer.milliseconds = 0;

        LOCAL_frameMemoryResetRequired = eTRUE;
    }
    else
    {
        if (LOCAL_frameMemoryResetRequired == eTRUE)
        {
            LOCAL_frameMemoryResetTimer.milliseconds += HMI_UPDATE_RATE_MILLISEC;

            if (LOCAL_frameMemoryResetTimer.milliseconds >= 1000)
            {
                LOCAL_frameMemoryResetTimer.milliseconds -= 1000;
                LOCAL_frameMemoryResetTimer.seconds ++;
            }

            if (LOCAL_frameMemoryResetTimer.seconds > HMI_CONFIGMODE_TIMEOUT_SEC)
            {
                FrameMover_ResetFrameMemory(0);
            }
        }
    }
}

// No restriction on the optimization of this function
FRAME_MEMORY_ENTRY FrameMemory_Forward(FRAME_MEMORY_ENTRY pushedFrame)
{
    FRAME_MEMORY_ENTRY returnEntry;

    LOCAL_frameMemory[LOCAL_frameMemoryPointer] = pushedFrame;
    LOCAL_frameMemoryPointer ++;

    if (LOCAL_frameMemoryPointer > (FRAME_MEMORY_SIZE - 1))
    {
        LOCAL_frameMemoryPointer = FRAME_MEMORY_SIZE - 1;
        returnEntry.pFrame = NULL;
        returnEntry.cursor = 0xFF;
    }
    else
    {
        returnEntry = LOCAL_frameMemory[LOCAL_frameMemoryPointer];
    }
    return returnEntry;
}

// No restriction on the optimization of this function
void FrameMemory_Sideways(FRAME_MEMORY_ENTRY pushedFrame)
{
    TUSIGN8 i = LOCAL_frameMemoryPointer;

    LOCAL_frameMemory[i] = pushedFrame;

    i ++;

    for ( ; i < FRAME_MEMORY_SIZE; i++)
    {
         LOCAL_frameMemory[i].pFrame = NULL;
         LOCAL_frameMemory[i].cursor = 0xFF;
    }
}

// No restriction on the optimization of this function
FRAME_MEMORY_ENTRY FrameMemory_Back(TUSIGN8 first)
{
    FRAME_MEMORY_ENTRY PulledEntry;
    TUSIGN8 displayMode;

    if (first || (LOCAL_frameMemoryPointer == 0) || (LOCAL_frameMemoryPointer > (FRAME_MEMORY_SIZE - 1)))
    {
        LOCAL_frameMemoryPointer = 0;
    }
    else
    {
        LOCAL_frameMemoryPointer --;
    }

    PulledEntry = LOCAL_frameMemory[LOCAL_frameMemoryPointer];

    if (PulledEntry.pFrame->frameType == FRAME_TYPE_OPERATOR)
    {
        #pragma diag_suppress=Pm060
        switch (PulledEntry.pFrame->frameDefinition.operatorFrame.frameNumber)
        {
        case 1:     displayMode = SFD_SIMPLE.displayMode_1;     break;
        #ifdef SECOND_OPERATOR_PAGE
        case 2:     displayMode = SFD_MULTI_OP.displayMode_2;   break;
        #endif
        #ifdef THIRD_OPERATOR_PAGE
        case 3:     displayMode = SFD_MULTI_OP.displayMode_3;   break;
        #endif
        #ifdef FOURTH_OPERATOR_PAGE
        case 4:     displayMode = SFD_MULTI_OP.displayMode_4;   break;
        #endif
        #ifdef FIFTH_OPERATOR_PAGE
        case 5:     displayMode = SFD_MULTI_OP.displayMode_5;   break;
        #endif
        #ifdef SIXTH_OPERATOR_PAGE
        case 6:     displayMode = SFD_MULTI_OP.displayMode_6;   break;
        #endif

        default:    displayMode = HMI_DISPLAY_MODE_OFF;         break;
        }
        #pragma diag_default=Pm060

        if ((displayMode == HMI_DISPLAY_MODE_OFF) || (displayMode > HMI_DISPLAY_MODE_MAX))
        {
            PulledEntry.pFrame = FrameTableGetFirstOpPageAddress();
            PulledEntry.cursor = 0;
        }
    }
    return PulledEntry;
}

/*!
 \brief         Acts on a command to change the currently active frame.
 \author        Roger Arnold
 \param         command
 \return        1 on success, 0 on any error (cannot perform requested command)
 \test          No details available
*/
// No restriction on the optimization of this function
TUSIGN16 FrameMoverMove(MOVE_COMMAND_TYPE command)
{
    TUSIGN16 returnCode = OK;

    switch (command)
    {
    case COMMAND_RETURN_TO_OPERATOR_PAGE:
        returnCode = ReturnToOperatorPage();
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
    case COMMAND_CHECK_SECURITY_PASSWORD:
	    returnCode = EnterConfig_CheckPassword();
	    break;

    #endif

    #ifdef EDP300_SPECIAL_FEATURES_HMI
    case COMMAND_LAUNCH_QUICK_ADJUST_FRAME:
        returnCode = FrameTableSetCurrentFrame(FrameTableGetEDP300QuickAdjustFrame(),0,IDF_SPECIAL_NONE);
        break;
    #endif


    case COMMAND_LAUNCH_OPERATOR_MENU:
        returnCode = LaunchOperatorMenu(eTRUE);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_RELAUNCH_OPERATOR_MENU:
        returnCode = LaunchOperatorMenu(eFALSE);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_MENU:
    case COMMAND_STANDARD_LEVEL:
    case COMMAND_ADVANCED_LEVEL:
    case COMMAND_SERVICE_LEVEL:

        returnCode = Menu(command);

        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_EXIT:
        returnCode = Exit();
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_MOVE_UP_LEVEL:
    case COMMAND_MOVE_DOWN_LEVEL:
        returnCode = NextConfigLevel((TUSIGN8)(command == COMMAND_MOVE_UP_LEVEL ? 1 : 0));
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_SELECT_CONFIG_LEVEL:
        returnCode = SelectConfigLevel();
        if (returnCode)
        {
            TUSIGN8 CurrentFrameType = FrameTableGetCurrentFrame()->frameType;
            (void)AdjusterPrepareNewVariable(0);
            if (CurrentFrameType == FRAME_TYPE_MANUAL_ADJUST)
            {
                returnCode = Edit(1);           // a manual adjust frame has been selected - enable edit
                if (returnCode)
                {
                    returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // editing has been enabled - initialise the adjuster
                }
            }
            else if ((CurrentFrameType == FRAME_TYPE_AUTO_ADJUST) || (CurrentFrameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
            {
                returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // initialise the adjuster for auto adjustment
            }
            else
            {
                asm("nop");
            }
        }
        break;

    case COMMAND_MOVE_UP_MENU:
    case COMMAND_MOVE_DOWN_MENU:
        returnCode = MoveInMenu((TUSIGN8)(command == COMMAND_MOVE_UP_MENU ? 1 : 0));
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_SELECT_FROM_MENU:
        switch (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag)
        {
        #ifdef HMI_USES_COMMONDIAGNOSTICS
        case 6: // Alarm Secure History         - Do nothing
        case 5: // Alarm History                - Do nothing
            break;
        #endif  // end of #ifdef HMI_USES_COMMONDIAGNOSTICS

        #ifdef HMI_SUPPORTS_SECURITY_ACCESS
        case 4: // ARM Access level Selection   - Selection of Access level has been made
            returnCode = EnterConfig_SelectionMade();
            break;
        #endif  // end of #ifdef HMI_SUPPORTS_SECURITY_ACCESS

        #ifdef HMI_USES_COMMONDIAGNOSTICS
        case 3:     // Diagnostics View         - Exit & return to operator page
            #ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
            {
                #ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS_OVERRIDE_ARM
                TUSIGN8 ARM_Level  = ARM_ACCESS_SERVICE;
                #else
                TUSIGN8 ARM_Level  = AdjusterGetARMLevel();
                #endif

                if ((FrameTable_isDiagDetailsViewActive() == eFALSE) && (ARM_Level == ARM_ACCESS_SERVICE))
                {
                    // Diagnostics info view available but currently inactive - activate Diagnostics info view
                    returnCode = FrameTableActivateDiagDetailsView(eTRUE);
                }
            }
            #else
            returnCode = ReturnToOperatorPage();
            #endif
            break;
        #endif  // end of #ifdef HMI_USES_COMMONDIAGNOSTICS

        case 2:     // Signals view             - Exit & return to operator page
            returnCode = ReturnToOperatorPage();
            break;

        case 1:     // Operator Menu            - Select from operator menu
            returnCode = SelectFromOperatorMenu();
            break;

        case 0:     // std menu                 - Select from menu
        default:
            returnCode = SelectFromMenu();
            break;
        }

        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);

            FRAME_FORMAT_TYPE const *pCurrFrame = FrameTableGetCurrentFrame();

            if ((FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_MANUAL_ADJUST)
            #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                 ||
                 ((pCurrFrame->frameType == FRAME_TYPE_STRING_PARAM) && (pCurrFrame->frameDefinition.stringParamFrame.specialStringFlag != 0))
            #endif
                 )
            {
                returnCode = Edit(1);           // a manual adjust frame has been selected - enable edit
                if (returnCode)
                {
                    returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // editing has been enabled - initialise the adjuster
                }
            }
            else if ((pCurrFrame->frameType == FRAME_TYPE_AUTO_ADJUST) || (pCurrFrame->frameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
            {
                returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // initialise the adjuster for auto adjustment
            }
            else
            {
                asm("nop");
            }
        }
        break;

    case COMMAND_EDIT:
        returnCode = Edit(1);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
            returnCode = AdjusterInitialiseNewVariable((void *)NULL);
        }
        break;

    case COMMAND_MOVE_CURSOR:
        returnCode = MoveCursor(0);
        break;

    case COMMAND_MOVE_CURSOR_TO_END:
        returnCode = MoveCursor(1);
        break;

    case COMMAND_BACK:
        returnCode = Back();
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_AUTO_NEXT_OPERATOR_FRAME:
        returnCode = NextOperatorFrame(eTRUE);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_NEXT_OPERATOR_FRAME:
        returnCode = NextOperatorFrame(eFALSE);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_OK:
        returnCode = Edit(0);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(1);

            #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
                FrameTableIncrementConfigChangeCounter();
            #endif
        }
        break;

    case COMMAND_OK_MANUAL_CAL:
        returnCode = Edit(0);
        if (returnCode)
        {
            if (AdjusterPrepareNewVariable(1) != (TUSIGN16)OK)
            {
                returnCode = Edit(1);
            }

        }
        break;

    case COMMAND_CANCEL:
        returnCode = Edit(0);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
        }
        break;

    case COMMAND_OK_CANCEL:
    case COMMAND_OK_CANCEL_2PT_PH:
        {
        TUSIGN8 saveOldVariable = FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit() ? 0 : 1;
        returnCode = AdjusterPrepareNewVariable(saveOldVariable);

        if (returnCode < ERROR_CODES)
        {
            returnCode = Edit(0);
            AdjusterSetLimitError(0,returnCode);

            #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
            if (command == COMMAND_OK_CANCEL)
            {
                FrameTableIncrementConfigChangeCounter();
            }
            #endif
        }
        else
        {
            if ((returnCode == LESSTHAN_RANGEMIN_ERR) ||
                (returnCode == RANGE_TOO_LOW_ERR) ||
                (returnCode == UPPER_RANGE_TOO_LOW_ERR) ||
                (returnCode == LOWER_RANGE_TOO_LOW_ERR))
            {
                AdjusterSetLimitError(0x01,returnCode);
            }
            else if ((returnCode == GREATERTHAN_RANGEMAX_ERR) ||
                     (returnCode == RANGE_TOO_HIGH_ERR) ||
                     (returnCode == UPPER_RANGE_TOO_HIGH_ERR) ||
                     (returnCode == LOWER_RANGE_TOO_HIGH_ERR))
            {
                AdjusterSetLimitError(0x02,returnCode);
            }
            else
            {
                AdjusterSetLimitError(0x03,returnCode);
            }
        }
        }
        break;

    case COMMAND_EXIT_ADJUST_OK:
    case COMMAND_EXIT_ADJUST_OK_AND_BACK:
        returnCode = Edit(0);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(1);

            #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
            FrameTableIncrementConfigChangeCounter();
            #endif

            returnCode = Back();
        }
        break;

    case COMMAND_EXIT_ADJUST_CANCEL:
    case COMMAND_EXIT_ADJUST_CANCEL_AND_BACK:
        returnCode = Edit(0);
        if (returnCode)
        {
            (void)AdjusterPrepareNewVariable(0);
            returnCode = Back();
        }
        break;


    case COMMAND_EXIT_ADJUST_OK_AND_NEXT_EASYSETUP:
    case COMMAND_EXIT_ADJUST_OK_AND_NEXT_SEQUENCE:
        #if defined(HMI_SUPPORTS_EASYSETUP) || defined(HMI_SUPPORTS_SEQUENCES)
        returnCode = Edit(0);
        if (returnCode)
        {
            TUSIGN8 CurrentFrameType = FrameTableGetCurrentFrame()->frameType;

            AdjusterPrepareNewVariable(1);

            #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
            FrameTableIncrementConfigChangeCounter();
            #endif

            if (command == COMMAND_EXIT_ADJUST_OK_AND_NEXT_EASYSETUP)
            {
                #ifdef HMI_SUPPORTS_EASYSETUP
                returnCode = NextEasySetup();
                #else
                returnCode =  1;
                #endif
            }
            else
            {
                #ifdef HMI_SUPPORTS_SEQUENCES
                returnCode = NextSequence();
                #else
                returnCode =  1;
                #endif
            }

            if (CurrentFrameType == FRAME_TYPE_MANUAL_ADJUST)
            {
                returnCode = Edit(1);           // a manual adjust frame has been selected - enable edit
                if (returnCode)
                {
                    returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // editing has been enabled - initialise the adjuster
                }
            }
            else if ((CurrentFrameType == FRAME_TYPE_AUTO_ADJUST) || (CurrentFrameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
            {
                returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // initialise the adjuster for auto adjustment
            }
            else
            {
                asm("nop");
            }
        }
        #else
        returnCode =  1;
        #endif
        break;

    case COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_EASYSETUP:
    case COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_SEQUENCE:
        #if defined(HMI_SUPPORTS_EASYSETUP) || defined(HMI_SUPPORTS_SEQUENCES)
        returnCode = Edit(0);
        if (returnCode)
        {
            TUSIGN8 CurrentFrameType = FrameTableGetCurrentFrame()->frameType;

            AdjusterPrepareNewVariable(0);

            if (command == COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_EASYSETUP)
            {
                #ifdef HMI_SUPPORTS_EASYSETUP
                returnCode = NextEasySetup();
                #else
                returnCode =  1;
                #endif
            }
            else
            {
                #ifdef HMI_SUPPORTS_SEQUENCES
                returnCode = NextSequence();
                #else
                returnCode =  1;
                #endif
            }

            if (CurrentFrameType == FRAME_TYPE_MANUAL_ADJUST)
            {
                returnCode = Edit(1);           // a manual adjust frame has been selected - enable edit
                if (returnCode)
                {
                    returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // editing has been enabled - initialise the adjuster
                }
            }
            else if ((CurrentFrameType == FRAME_TYPE_AUTO_ADJUST) || (CurrentFrameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
            {
                returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // initialise the adjuster for auto adjustment
            }
            else
            {
                asm("nop");
            }
        }
        #else
        returnCode =  1;
        #endif
        break;

    case COMMAND_NEXT_SEQUENCE:
    case COMMAND_NEXT_EASYSETUP:
        #if defined(HMI_SUPPORTS_EASYSETUP) || defined(HMI_SUPPORTS_SEQUENCES)
        if (command == COMMAND_NEXT_EASYSETUP)
        {
            #ifdef HMI_SUPPORTS_EASYSETUP
            returnCode = NextEasySetup();
            #else
            returnCode =  1;
            #endif
        }
        else
        {
            #ifdef HMI_SUPPORTS_SEQUENCES
            returnCode = NextSequence();
            #else
            returnCode =  1;
            #endif
        }
        if (returnCode)
        {
            TUSIGN8 CurrentFrameType = FrameTableGetCurrentFrame()->frameType;

            AdjusterPrepareNewVariable(0);
            if (CurrentFrameType == FRAME_TYPE_MANUAL_ADJUST)
            {
                returnCode = Edit(1);           // a manual adjust frame has been selected - enable edit
                if (returnCode)
                {
                    returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // editing has been enabled - initialise the adjuster
                }
            }
            else if ((CurrentFrameType == FRAME_TYPE_AUTO_ADJUST) || (CurrentFrameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT))
            {
                returnCode = AdjusterInitialiseNewVariable((void *)NULL);   // initialise the adjuster for auto adjustment
            }
            else
            {
                asm("nop");
            }
        }
        #else
        returnCode =  1;
        #endif
        break;

#ifndef HMI_SUPPORTS_SECURITY_ACCESS
    case COMMAND_CHECK_SECURITY_PASSWORD:
#endif

    case COMMAND_DO_NOT_MOVE:
    case COMMAND_ADJUST_UP:
    case COMMAND_ADJUST_DOWN:
    case COMMAND_DEFAULT_CONTRAST:
    case COMMAND_DO_AUTO_ADJUST:
    default:
        returnCode =  1;
        break;
    }
    return returnCode;
}

// No restriction on the optimization of this function
TUSIGN8 LaunchOperatorMenu(TBOOL saveAutoscroll)
{
    TUSIGN8 cursor;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    TUSIGN8 alarmClassification, alarmCategory;
    #endif

    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    if (saveAutoscroll)
    {
        LOCAL_savedAutoscroll = FrameTableIsAutoscrollEnabled();
    }

    FrameTableDisableAutoscroll();
    #endif

    if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_OPERATOR)
    {
        LOCAL_currentOperatorPageNumber = FrameTableGetCurrentFrame()->frameDefinition.operatorFrame.frameNumber;
    }

    #ifdef HMI_USES_COMMONDIAGNOSTICS
	// Check if any alarms active
        AdjusterGetNAMURStatus(&alarmClassification, &alarmCategory);
        if (alarmClassification != CLASSIFICATION_NONE)
        {
            cursor = 0;
        }
        else
        {
            cursor = LOCAL_currentOperatorPageNumber;
        }
    #else
        cursor = LOCAL_currentOperatorPageNumber - 1;
    #endif

    return FrameTableSetCurrentFrame(FrameTableGetOperatorMenuAddress(),cursor,IDF_SPECIAL_NONE);
}

// No restriction on the optimization of this function
TUSIGN8 Menu(MOVE_COMMAND_TYPE command)
{
#ifdef HMI_SUPPORTS_SECURITY_ACCESS

    TUSIGN8 cursor;
    TUSIGN8 currentAccessLevel;

    switch (command)
    {
    case COMMAND_STANDARD_LEVEL:    currentAccessLevel = ARM_ACCESS_STANDARD;       break;
    case COMMAND_ADVANCED_LEVEL:    currentAccessLevel = ARM_ACCESS_ADVANCED;       break;
    case COMMAND_SERVICE_LEVEL:     currentAccessLevel = ARM_ACCESS_SERVICE;        break;
    default:                        currentAccessLevel = AdjusterGetARMLevel();     break;
    }

    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    LOCAL_savedAutoscroll = FrameTableIsAutoscrollEnabled();
    FrameTableDisableAutoscroll();
    #endif

    if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_OPERATOR)
    {
        LOCAL_currentOperatorPageNumber = FrameTableGetCurrentFrame()->frameDefinition.operatorFrame.frameNumber;
    }

    cursor = 1;
    switch (currentAccessLevel)
    {
    case ARM_ACCESS_SERVICE:      //Service user logged in
        #ifdef HMI_SUPPORTS_ARM_SERVICE
            #ifdef HMI_SUPPORTS_ARM_READONLY
            cursor ++;
            #endif
            #ifdef HMI_SUPPORTS_ARM_STANDARD
            cursor ++;
            #endif
            #ifdef HMI_SUPPORTS_ARM_ADVANCED
            cursor ++;
            #endif
        #else
            cursor = 0;
        #endif
        break;

    case ARM_ACCESS_ADVANCED:     //Advanced user logged in
        #ifdef HMI_SUPPORTS_ARM_ADVANCED
            #ifdef HMI_SUPPORTS_ARM_READONLY
            cursor ++;
            #endif
            #ifdef HMI_SUPPORTS_ARM_STANDARD
            cursor ++;
            #endif
        #else
            cursor = 0;
        #endif
        break;

    case ARM_ACCESS_STANDARD:     //Standard user logged in
        #ifdef HMI_SUPPORTS_ARM_STANDARD
            #ifdef HMI_SUPPORTS_ARM_READONLY
            cursor ++;
            #endif
        #else
            cursor = 0;
        #endif
        break;

    case ARM_ACCESS_LOGOUT:     // No user logged in
    case ARM_ACCESS_READ_ONLY:
    default:
        #ifdef HMI_SUPPORTS_ARM_READONLY
            cursor = 1;             // Set cursor to first item (after logout)
        #else
            cursor = 0;
        #endif
        break;
    }
    return FrameTableSetCurrentFrame(FrameTableGetARMSelectionAddress(),cursor,IDF_SPECIAL_NONE);

#else // else condition for #ifdef HMI_SUPPORTS_SECURITY_ACCESS

    TUSIGN8 returnVal;

    #ifdef EDP300_SPECIAL_FEATURES_HMI
    if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_OPERATOR)
    {
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
            LOCAL_savedAutoscroll = FrameTableIsAutoscrollEnabled();
            FrameTableDisableAutoscroll();
        #endif

        returnVal = FrameTableSetCurrentFrame(FrameTableGetEDP300ModeFrame(),0,IDF_SPECIAL_NONE);
    }
    else

    #endif // #ifdef EDP300_SPECIAL_FEATURES_HMI
    {
        FRAME_MEMORY_ENTRY pushedFrame, nextFrame;

        #ifdef EDP300_SPECIAL_FEATURES_HMI
        pushedFrame.pFrame = FrameTableGetFirstOpPageAddress();
        #else
        pushedFrame.pFrame = FrameTableGetCurrentFrame();
        #endif
        pushedFrame.cursor = 0;

        // Push the current operator frame to the frame stack
        nextFrame = FrameMemory_Forward(pushedFrame);

        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        LOCAL_savedAutoscroll = FrameTableIsAutoscrollEnabled();
        FrameTableDisableAutoscroll();
        #endif

        if (nextFrame.pFrame == NULL)
        {
            nextFrame.pFrame = pushedFrame.pFrame->frameDefinition.operatorFrame.pNextFrame;
        }

        if (!ValidityTestCheck((TUSIGN16)nextFrame.pFrame->frameDefinition.configHeaderFrame.validTest))
        {
            // Pulled frame not valid - Start with first config header and search for first valid header
            FRAME_FORMAT_TYPE const *ConfigHeaderFrame = FrameTableGetConfigHeaderAddress(0);

            while (!ValidityTestCheck((TUSIGN16)ConfigHeaderFrame->frameDefinition.configHeaderFrame.validTest))
            {
                // No alternative but to use pointer arithmetic
                #pragma diag_suppress=Pm088
                ConfigHeaderFrame ++;
                #pragma diag_default=Pm088
            }

            nextFrame.pFrame = ConfigHeaderFrame;
            nextFrame.cursor = 0;
            FrameMemory_Sideways(nextFrame);
        }

        #if defined(IDF_SPECIAL_INH_ENTER_CONFIG)
        returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_INH_ENTER_CONFIG);
        #else
        returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_NONE);
        #endif

        if (returnVal == 1)
        {
            returnVal = ConfigModeManagerSetConfigurationMode(1);
        }
    }
    return returnVal;
#endif // end of else condition for #ifdef HMI_SUPPORTS_SECURITY_ACCESS

}


// No restriction on the optimization of this function
TUSIGN8 Exit(void)
{
    #if defined(IDF_SPECIAL_INH_EXIT_CONFIG)
    TUSIGN8 returnVal = FrameTableSetCurrentFrame(FrameMemory_Back(1).pFrame,0,IDF_SPECIAL_INH_EXIT_CONFIG);
    #else
    TUSIGN8 returnVal = FrameTableSetCurrentFrame(FrameMemory_Back(1).pFrame,0,IDF_SPECIAL_NONE);
    #endif

    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    if (LOCAL_savedAutoscroll)
    {
        FrameTableEnableAutoscroll();
    }
    #endif

    if (returnVal == 1)
    {
        if (ConfigModeManagerSetConfigurationMode(0) != 0)
        {
            returnVal = 0;
        }
    }

    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 NextConfigLevel(TUSIGN8 up)
{
    FRAME_FORMAT_TYPE const *pFrameAtStart = FrameTableGetCurrentFrame();
    FRAME_FORMAT_TYPE const *pFrame = pFrameAtStart;
    TUSIGN8  frameValid;

    do
    {
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        if (up)
        {
            // move to next frame
            pFrame --;
        }
        else
        {
            // move to next frame
            pFrame ++;
        }
        #pragma diag_default=Pm088

        // if we have moved past the config header frames then wrap
        // if (up == 1) then FrameTableGetConfigHeaderAddress(up) will return the last config header
        // if (up == 0) then FrameTableGetConfigHeaderAddress(up) will return the first config header
        //                             ____________
        // first config header ------ |   Device   |
        //                            |  __________|_       ^
        //                            |_|   Display  |      | Up Key
        //                              |  __________|_     |    (Up = 1)
        //                              |_|   Alarm    |    |
        //                                |  __________|_   |  |
        //                                |_|   Comms    |     |
        //                                  |  __________|_    | Down Key
        //                                  |_|   Calib.   |   |   (Up = 0)
        //                                    |            |   v
        //  last config header -------------- |____________|
        //
        if (pFrame->frameType != FRAME_TYPE_CONFIG_HEADER)
        {
            pFrame = FrameTableGetConfigHeaderAddress(up);
        }

        // check that the new frame (config level) is available
#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
		frameValid = ValidityTestCheck(
#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
		(FRAME_FORMAT_TYPE const*)NULL,
#endif
		(TUSIGN16)pFrame->frameDefinition.configHeaderFrame.validTest
#ifdef VALIDITY_TEST_CALLER
		, SIGNALS_TEST_NOT_APPLICABLE
#endif
		);
#endif
    } while ((frameValid == 0) && (pFrame != pFrameAtStart));

    if (pFrame != pFrameAtStart)
    {
        FRAME_MEMORY_ENTRY pushedFrame;
        pushedFrame.pFrame = pFrame;
        pushedFrame.cursor = 0;
        FrameMemory_Sideways(pushedFrame);
    }

    return FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
}

// No restriction on the optimization of this function
TUSIGN8 SelectConfigLevel(void)
{
    TUSIGN8 returnVal;
    FRAME_MEMORY_ENTRY pushedFrame, nextFrame;
    pushedFrame.pFrame = FrameTableGetCurrentFrame();
    pushedFrame.cursor = 0;

    #ifdef HMI_SUPPORTS_EASYSETUP
    LOCAL_easySetup_number = pushedFrame.pFrame->frameDefinition.configHeaderFrame.easySetupFlag;
    #endif


    if (pushedFrame.pFrame->frameDefinition.configHeaderFrame.pNextFrame != pushedFrame.pFrame)
    {
        nextFrame = FrameMemory_Forward(pushedFrame);
        if (nextFrame.pFrame == NULL)
        {
            nextFrame.pFrame = pushedFrame.pFrame->frameDefinition.configHeaderFrame.pNextFrame;
            nextFrame.cursor = 0;
        }
        returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_NONE);


        #ifdef HMI_SUPPORTS_EASYSETUP
        if (LOCAL_easySetup_number)
        {

#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
			if(ValidityTestCheck(
#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
			(FRAME_FORMAT_TYPE const*)NULL,
#endif
			(TUSIGN16)FrameTableGetEasySetupValidityTestIndex(0, LOCAL_easySetup_number)
#ifdef VALIDITY_TEST_CALLER
			, SIGNALS_TEST_NOT_APPLICABLE
#endif
			) == 0)
#endif
            {
                // first frame of easy setup not valid
                returnVal = NextEasySetup();
            }
        }
        #endif
    }
    else
    {
        returnVal = 0;
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 SelectFromMenu(void)
{
    TUSIGN8 returnVal;
    FRAME_MEMORY_ENTRY pushedFrame, nextFrame;

    pushedFrame.pFrame = FrameTableGetCurrentFrame();
    pushedFrame.cursor = FrameTableGetCursorPosition();

    // Push the current operator frame to the frame stack
    nextFrame = FrameMemory_Forward(pushedFrame);

    if (nextFrame.pFrame == NULL)
    {
        nextFrame.pFrame = pushedFrame.pFrame->frameDefinition.menuFrame.pNextFrame;
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        nextFrame.pFrame += FrameTableGetCursorPosition();
        #pragma diag_default=Pm088
        nextFrame.cursor = 0;
    }

    TUSIGN8 MenuPositionCursor = FrameTableGetCursorPosition();
    TUSIGN8 MenuPositionCursorLim = FrameTableGetCursorPositionLimit();

    FrameTableAccessMenuPositionDetails(&MenuPositionCursor, &MenuPositionCursorLim, eFALSE);

    // FrameTableUpdateMenuPositionDetailsForTFT(TUSIGN8 cursorPos, TUSIGN8 cursorPosLimit, TUSIGN16 firstItem);

    #ifdef HMI_SUPPORTS_SEQUENCES
    if (nextFrame.pFrame->frameType == FRAME_TYPE_MENU)
    {
        FrameTableDisableSequence();
        returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_NONE);

        if (FrameTableIsSequenceEnabled())
        {
            MENU_FRAME_TYPE SequenceMenu = FrameTableGetCurrentFrame()->frameDefinition.menuFrame;

            FrameTableSetSequenceSettings(FrameTableGetCursorPosition(),
                                          FrameTableGetCursorPositionLimit(),
                                          (TUSIGN16)((SequenceMenu.firstItem_MSByte << 8) + SequenceMenu.firstItem_LSByte));

            if (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.pNextFrame->frameType == FRAME_TYPE_MENU)
            {
                TUSIGN8 c  = FrameTableGetCursorPosition();
                TUSIGN8 cl = FrameTableGetCursorPositionLimit();

                FrameTableDisableSequence();

                FrameTableAccessMenuPositionDetails(&c, &cl, eFALSE);

                #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
                FrameTableUpdateMenuPositionDetailsForTFT(c, cl, (TUSIGN16)((SequenceMenu.firstItem_MSByte << 8) + SequenceMenu.firstItem_LSByte));
                #endif
            }
            else
            {
                returnVal = FrameTableSetCurrentFrame(FrameTableGetCurrentFrame()->frameDefinition.menuFrame.pNextFrame,FrameTableGetCursorPosition(),IDF_SPECIAL_NONE);
            }
        }
    }
    else
    #endif
    {
        returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_NONE);
    }
    return returnVal;
}

TUSIGN8 SelectFromOperatorMenu(void)
{
typedef enum
    {
        OPRMENU_DoNothing,
        OPRMENU_SelectDiagnostics,
        OPRMENU_SelectOprPage1,
        OPRMENU_SelectOprPage2,
        OPRMENU_SelectOprPage3,
        OPRMENU_SelectOprPage4,
        OPRMENU_SelectOprPage5,
        OPRMENU_SelectOprPage6,
        OPRMENU_SelectAutoscroll,
        OPRMENU_SelectSignals
    }OPERATORMENUFUNCTION;

    OPERATORMENUFUNCTION OperatorMenuFunctionTab[] = {
        #ifdef HMI_USES_COMMONDIAGNOSTICS
        OPRMENU_SelectDiagnostics,
        #endif
        OPRMENU_SelectOprPage1,
        #ifdef SECOND_OPERATOR_PAGE
        OPRMENU_SelectOprPage2,
        #endif
        #ifdef THIRD_OPERATOR_PAGE
        OPRMENU_SelectOprPage3,
        #endif
        #ifdef FOURTH_OPERATOR_PAGE
        OPRMENU_SelectOprPage4,
        #endif
        #ifdef FIFTH_OPERATOR_PAGE
        OPRMENU_SelectOprPage5,
        #endif
        #ifdef SIXTH_OPERATOR_PAGE
        OPRMENU_SelectOprPage6,
        #endif

        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        OPRMENU_SelectAutoscroll,
        #endif
        OPRMENU_SelectSignals,
        OPRMENU_DoNothing,
        OPRMENU_DoNothing,
        OPRMENU_DoNothing,
        OPRMENU_DoNothing,
        OPRMENU_DoNothing,
        OPRMENU_DoNothing
        };

    TUSIGN8 returnVal = 0;
    FRAME_FORMAT_TYPE const *pFrame;

    OPERATORMENUFUNCTION OperatorMenuFunction = OPRMENU_DoNothing;
    TUSIGN8 cursorPos = FrameTableGetCursorPosition();

    if (cursorPos < sizeof(OperatorMenuFunctionTab)/sizeof(OPERATORMENUFUNCTION))
    {
        OperatorMenuFunction = OperatorMenuFunctionTab[cursorPos];
    }

    switch (OperatorMenuFunction)
    {
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    case OPRMENU_SelectDiagnostics:
        pFrame = FrameTableGetDiagnosticsViewAddress();
        returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        break;
    #endif

    case OPRMENU_SelectOprPage1:
        if ((SFD_SIMPLE.displayMode_1 > HMI_DISPLAY_MODE_OFF) && (SFD_SIMPLE.displayMode_1 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;

    #ifdef SECOND_OPERATOR_PAGE
    case OPRMENU_SelectOprPage2:
        if ((SFD_MULTI_OP.displayMode_2 > HMI_DISPLAY_MODE_OFF) && (SFD_MULTI_OP.displayMode_2 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            // No alternative but to use pointer arithmetic
            #pragma diag_suppress=Pm088
            pFrame += 1;
            #pragma diag_default=Pm088
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;
    #endif

    #ifdef THIRD_OPERATOR_PAGE
    case OPRMENU_SelectOprPage3:
        if ((SFD_MULTI_OP.displayMode_3 > HMI_DISPLAY_MODE_OFF) && (SFD_MULTI_OP.displayMode_3 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            // No alternative but to use pointer arithmetic
            #pragma diag_suppress=Pm088
            pFrame += 2;
            #pragma diag_default=Pm088
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;
    #endif

    #ifdef FOURTH_OPERATOR_PAGE
    case OPRMENU_SelectOprPage4:
        if ((SFD_MULTI_OP.displayMode_4 > HMI_DISPLAY_MODE_OFF) && (SFD_MULTI_OP.displayMode_4 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            // No alternative but to use pointer arithmetic
            #pragma diag_suppress=Pm088
            pFrame += 3;
            #pragma diag_default=Pm088
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;
    #endif

    #ifdef FIFTH_OPERATOR_PAGE
    case OPRMENU_SelectOprPage5:
        if ((SFD_MULTI_OP.displayMode_5 > HMI_DISPLAY_MODE_OFF) && (SFD_MULTI_OP.displayMode_5 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            // No alternative but to use pointer arithmetic
            #pragma diag_suppress=Pm088
            pFrame += 4;
            #pragma diag_default=Pm088
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;
    #endif

    #ifdef SIXTH_OPERATOR_PAGE
    case OPRMENU_SelectOprPage6:
        if ((SFD_MULTI_OP.displayMode_6 > HMI_DISPLAY_MODE_OFF) && (SFD_MULTI_OP.displayMode_6 <= HMI_DISPLAY_MODE_MAX))
        {
            pFrame = FrameTableGetFirstOpPageAddress();
            // No alternative but to use pointer arithmetic
            #pragma diag_suppress=Pm088
            pFrame += 5;
            #pragma diag_default=Pm088
            returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        }
        break;
    #endif

    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    case OPRMENU_SelectAutoscroll:

        pFrame = FrameTableGetFirstOpPageAddress();
        switch (LOCAL_currentOperatorPageNumber)
        {
        // break intentionally omitted from each case
        #pragma diag_suppress=Pm057
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        case 6:     pFrame ++;
        case 5:     pFrame ++;
        case 4:     pFrame ++;
        case 3:     pFrame ++;
        case 2:     pFrame ++;
        default:    asm("nop");
        #pragma diag_default=Pm088
        #pragma diag_default=Pm057
        }
        returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);

        // switch autoscroll on
        FrameTableEnableAutoscroll();

        break;
    #endif

    case OPRMENU_SelectSignals:
        pFrame = FrameTableGetSignalsViewAddress();
        returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        break;

    case OPRMENU_DoNothing:
    default:
        returnVal = 0;
        break;
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 Edit(TUSIGN8 enable)
{
    TUSIGN8 returnVal = 0;
    if (FrameTableEnableEdit(enable) == enable)
    {
        returnVal = 1;
    }
    return returnVal;
}

TUSIGN8 MoveInMenu(TUSIGN8 up)
{
    TUSIGN8 cursor;
    TUSIGN8 specialMenuFlag = FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag;

    switch (specialMenuFlag)
    {
    #ifdef EDP300_SPECIAL_FEATURES_HMI
    case 7:
        cursor = FrameTableGetCursorPosition();
        if (up) {   cursor = cursor > 0 ? cursor - 1 : 5;   }
        else    {   cursor = cursor < 5 ? cursor + 1 : 0;   }
        break;
    #endif

    case 3: // Diagnostics View
        #ifdef HMI_USES_COMMONDIAGNOSTICS
        #ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
        if (FrameTable_isDiagDetailsViewActive())
        {
            FrameTable_nextDiagDetail(up);
        }
        else
        #endif
        {
            TUSIGN8 cursorLimit = AdjusterGetNumberOfAlarmConditions(eTRUE) - 1;
            cursor = FrameTableGetCursorPosition();

            if (up)
            {
                if (cursor > 0)
                {
                    cursor --;
                }
            }
            else
            {
                if (cursor < cursorLimit)
                {
                    cursor ++;
                }
            }

            if (cursor > cursorLimit)
            {
                cursor = cursorLimit;
            }
        }
        #endif  // end of #ifdef HMI_USES_COMMONDIAGNOSTICS
        break;

    case 5: // Alarm History
    case 6: // Secure Alarm History
        #ifdef HMI_USES_COMMONDIAGNOSTICS
        #ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
        if (FrameTable_isDiagDetailsViewActive())
        {
            FrameTable_nextDiagDetail(up);
        }
        else
        #endif
        {
            TUSIGN8 cursorLimit = AdjusterGetNumberOfAlarmConditions(eFALSE) - 1;

            TUSIGN8 AlarmClassification, AlarmCategory, AlarmPriority, AlarmID;
            TUSIGN16 AlarmCount, AlarmTime_day, AlarmMostRecent_day;
            TUSIGN32 AlarmTime_msec, AlarmMostRecent_msec;

            TUSIGN8 StartCursor = FrameTableGetCursorPosition();
            cursor = StartCursor;

            do
            {
                StartCursor = cursor;

                if (up)
                {
                    if (cursor > 0)
                    {
                        cursor --;
                    }
                }
                else
                {
                    if (cursor < cursorLimit)
                    {
                        cursor ++;
                    }
                }

                (void)AdjusterGetAlarmHistoryDetails((TUSIGN8)(specialMenuFlag - 4),    // TUSIGN8 HistoryNumber (1 or 2)
                                                     cursor,                            // TUSIGN8 SelectedCondition
                                                     &AlarmClassification,              // TUSIGN8 *pAlarmClassification
                                                     &AlarmCategory,                    // TUSIGN8 *pAlarmCategory
                                                     &AlarmPriority,                    // TUSIGN8 *pAlarmPriority
                                                     &AlarmID,                          // TUSIGN8 *pAlarmID
                                                     &AlarmCount,                       // TUSIGN16 *pAlarmCount
                                                     &AlarmTime_msec,                   // TUSIGN32 *pAlarmTime_msec
                                                     &AlarmTime_day,                    // TUSIGN16 *pAlarmTime_day
                                                     &AlarmMostRecent_msec,             // TUSIGN32 *pAlarmMostRecent_msec
                                                     &AlarmMostRecent_day);             // TUSIGN16 *pAlarmMostRecent_day


            } while ((AlarmCount == 0) && (cursor != StartCursor));

            if (cursor == StartCursor)
            {
                cursor = FrameTableGetCursorPosition();
            }
        }
        #endif  // end of #ifdef HMI_USES_COMMONDIAGNOSTICS
        break;


    case 2: // Signals view
        {
            TUSIGN8 cursorAtStart = FrameTableGetCursorPosition();
            TUSIGN8 cursorValid;
            cursor = cursorAtStart;

            do
            {
                if (up)
                {
                    // move up to next item (if we have moved past the first item then wrap to last item)
                    cursor = cursor > 0 ? cursor - 1 : FrameTableGetCursorPositionLimit();
                }
                else
                {
                    // move down to next item (if we have moved past the last item then wrap to first item)
                    cursor = cursor < FrameTableGetCursorPositionLimit() ? cursor + 1 : 0;
                }
                #ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
				cursorValid = ValidityTestCheck(
					#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
						(FRAME_FORMAT_TYPE const*)NULL,
					#endif
						(TUSIGN16)FrameTableGetSignal(cursor)->SignalsViewTest
					#ifdef VALIDITY_TEST_CALLER
						, SIGNALS_TEST_NOT_APPLICABLE
					#endif
				);
                #else
                cursorValid = FrameTableGetSignal(cursor)->SignalsViewEnable;
                #endif
            }
            while ((cursorValid == 0) && (cursor != cursorAtStart));
        }
        break;

    case 1: // Operator Menu
    case 0: // Standard Menu
    default:
        {
            TUSIGN8 cursorAtStart = FrameTableGetCursorPosition();
            TUSIGN8 cursorValid = 0;
            TUSIGN8 cursorLim = FrameTableGetCursorPositionLimit();

            MENU_ITEM_TYPE const *pMenuItem;
            TUSIGN16 testIdx;
            TUSIGN16 firstItem;

            cursor = cursorAtStart;

            do
            {
                if (up)
                {
                    // move up to next item (if we have moved past the first item then wrap to last item)
                    cursor = cursor > 0 ? cursor - 1 : cursorLim;
                }
                else
                {
                    // move down to next item (if we have moved past the last item then wrap to first item)
                    cursor = cursor < cursorLim ? cursor + 1 : 0;
                }

                // check that the new item is available...
                // ...1 Get pointer to the menu item
                firstItem = ((TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.menuFrame.firstItem_MSByte << 8) + (TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.menuFrame.firstItem_LSByte;
                pMenuItem = FrameTableGetMenuItem((TUSIGN16)(firstItem + cursor));
                if (pMenuItem == NULL)
                {
                    cursorValid = 0;
                }
                else
                {
                    // ...2 Get the test function index
                    testIdx = pMenuItem->validTest;
                    // ...3 Perform the test
					#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
						cursorValid = ValidityTestCheck(
						#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
							(FRAME_FORMAT_TYPE const*)NULL,
						#endif
							testIdx
						#ifdef VALIDITY_TEST_CALLER
							, SIGNALS_TEST_NOT_APPLICABLE
						#endif
						);
					#else
						cursorValid = FrameTableGetSignal(cursor)->SignalsViewEnable;
					#endif
                }

                // 201805 ...4 check whether the service user menu is valid. This is for cyber security test
                if((specialMenuFlag == 4)   //access leavel Frame
                && (cursor == cursorLim))    //service user menu
                {
                    extern TUSIGN8 valid_if_service_account_is_allowed(void);
                    cursorValid = valid_if_service_account_is_allowed();
                }
            }
            while ((cursorValid == 0) && (cursor != cursorAtStart));

            if (cursor != cursorAtStart)
            {
                if (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 0)
                {
                    FRAME_MEMORY_ENTRY pushedFrame;
                    pushedFrame.pFrame = FrameTableGetCurrentFrame();
                    pushedFrame.cursor = cursor;
                    FrameMemory_Sideways(pushedFrame);
                }
            }
            FrameTableAccessMenuPositionDetails(&cursor, &cursorLim, eFALSE);

            #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
            FrameTableUpdateMenuPositionDetailsForTFT(cursor, cursorLim, (TUSIGN16)(((TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.menuFrame.firstItem_MSByte << 8) + (TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.menuFrame.firstItem_LSByte));
            #endif
        }
        break;
    }
    return FrameTableSetCursorPosition(cursor);
}

TUSIGN8 MoveCursor(TUSIGN8 toEnd)
{
    TUSIGN8 returnVal;
    FRAME_FORMAT_TYPE const *pFrame = FrameTableGetCurrentFrame();
    TUSIGN8  cursorLimit = FrameTableGetCursorPositionLimit();
    TUSIGN8  cursor = FrameTableGetCursorPosition();

    #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
    if (pFrame->frameType == FRAME_TYPE_IP_ADDRESS)
    {
        if      (toEnd)                 {cursor = cursorLimit;}
        else if (cursor < cursorLimit)  {cursor ++;}
        else                            {cursor = 0;}
    }
    else
    {
	#endif


    #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
    if (pFrame->frameType == FRAME_TYPE_BIT_ENUMERATED_PARAM)
    {
        if      (toEnd)                 {cursor = cursorLimit;}
        else if (cursor < cursorLimit)  {cursor ++;}
        else                            {cursor = 0;}
    }
    else
    {
	#endif

    #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
    if (pFrame->frameType == FRAME_TYPE_TIME_AND_DATE_PARAM)
    {
        if      (toEnd)                 {cursor = cursorLimit;}
        else if (cursor < cursorLimit)  {cursor ++;}
        else                            {cursor = 0;}
    }
    else
    {
    #endif

    if (toEnd)
    {
        cursor = cursorLimit;
    }
    else
    {
        if (cursor < cursorLimit)
        {
            TUSIGN8 n,j;
            j = cursor;
            cursor ++;
            TBOOL skipOverDecPoint = eFALSE;

            #ifdef HMI_SUPPORTS_MATRIX_FRAMES
            if ((pFrame->frameType == FRAME_TYPE_MATRIX_T_C_D)
             || (pFrame->frameType == FRAME_TYPE_MATRIX_X_Y_XY))
            {
                MATRIX_CURSOR_POS matrixCursor = FrameTable_GetMatrixCursor();

                if (((matrixCursor == MATRIX_CURSOR_COLUMN_EDIT)  && (pFrame->frameDefinition.matrixFrame.dpPosn_Col != MAX_RESOLUTION))
                 || ((matrixCursor == MATRIX_CURSOR_ROW_EDIT)     && (pFrame->frameDefinition.matrixFrame.dpPosn_Row != MAX_RESOLUTION))
                 || ((matrixCursor == MATRIX_CURSOR_ELEMENT_EDIT) && (pFrame->frameDefinition.matrixFrame.dpPosn_Elm != MAX_RESOLUTION)))
                {
                    skipOverDecPoint = eTRUE;
                }
            }
            #endif

            #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
            if (pFrame->frameType == FRAME_TYPE_LINEARIZER)
            {
                LINEARIZER_CURSOR_POS linearizerCursor = FrameTable_GetLinearizerCursor();

                if (((linearizerCursor == LINEARIZER_CURSOR_INPOINT_EDIT)  && (pFrame->frameDefinition.linearizerFrame.dpPosn_inpoint  != MAX_RESOLUTION))
                 || ((linearizerCursor == LINEARIZER_CURSOR_OUTPOINT_EDIT) && (pFrame->frameDefinition.linearizerFrame.dpPosn_outpoint != MAX_RESOLUTION)))
                {
                    skipOverDecPoint = eTRUE;
                }
            }
            #endif

            if ((pFrame->frameType == FRAME_TYPE_NUMERICAL_PARAM) && (pFrame->frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn != MAX_RESOLUTION))
            {
                skipOverDecPoint = eTRUE;
            }

            if (pFrame->frameType == FRAME_TYPE_2_POINT_pH_CALIBRATION)
            {
                skipOverDecPoint = eTRUE;
            }

            #ifdef HMI_SUPPORTS_INTEGER_FRAMES
            if (pFrame->frameType == FRAME_TYPE_INTEGER_PARAM)
            {
                skipOverDecPoint = eTRUE;
            }
            #endif

            if ((AdjusterGetVariableAsString(&n)[j] == '.')
             && (skipOverDecPoint == eTRUE))
            {
                cursor = cursor < cursorLimit ? cursor + 1 : 0;
            }
        }
        else
        {
            cursor = 0;
        }
    }

    #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
    }
    #endif

    #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
    }
    #endif

    #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
    }
    #endif

    returnVal = FrameTableSetCursorPosition(cursor);
    return returnVal;

}

TUSIGN8 Back(void)
{
    FRAME_MEMORY_ENTRY pulledFrame = FrameMemory_Back(0);
    TUSIGN8 returnVal;

    returnVal = FrameTableSetCurrentFrame(pulledFrame.pFrame,pulledFrame.cursor,IDF_SPECIAL_NONE);

    return returnVal;
}

#ifdef HMI_SUPPORTS_SEQUENCES
TUSIGN8 NextSequence(void)
{
    TUSIGN8 returnVal, frameValid;
    FRAME_FORMAT_TYPE const *pFrame = FrameTableGetCurrentFrame();
    TUSIGN8 MenuPosCursor, MenuPosCursorLim;
    FRAME_TYPE_TYPE FrameType;

    TUSIGN16 FirstItemOfSequence;

    (void)FrameTableGetSequenceSettings(&MenuPosCursor, &MenuPosCursorLim, &FirstItemOfSequence);

    do
    {
        // increment to next frame
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        pFrame ++;
        #pragma diag_default=Pm088
        MenuPosCursor++;

        FrameType = pFrame->frameType;

        if ((FrameType == FRAME_TYPE_NUMERICAL_PARAM)
         || (FrameType == FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT)
         || (FrameType == FRAME_TYPE_ENUMERATED_PARAM)
         || (FrameType == FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT)
         || (FrameType == FRAME_TYPE_MANUAL_ADJUST)
         || (FrameType == FRAME_TYPE_AUTO_ADJUST)
         || (FrameType == FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP)
         || (FrameType == FRAME_TYPE_STRING_PARAM)
         || (FrameType == FRAME_TYPE_STRING_PARAM_NO_EDIT)
         || (FrameType == FRAME_TYPE_STRING_PARAM_PACKED)
         || (FrameType == FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT)
         || (FrameType == FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT)
         || (FrameType == FRAME_TYPE_TIME_AND_DATE_PARAM)
         || (FrameType == FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT)
         || (FrameType == FRAME_TYPE_BIT_ENUMERATED_PARAM)
         || (FrameType == FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT)
         || (FrameType == FRAME_TYPE_INTEGER_PARAM)
         || (FrameType == FRAME_TYPE_INTEGER_PARAM_NO_EDIT)

         #if defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_AUTO_CALIBRATION)
         || (FrameType == FRAME_TYPE_MANUAL_CALIBRATION)
         || (FrameType == FRAME_TYPE_AUTO_CALIBRATION)
         #endif

         #if defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)
         || (FrameType == FRAME_TYPE_2_POINT_pH_CALIBRATION)
         #endif

         #ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
         || (FrameType == FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT)
         || (FrameType == FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT)
         #endif
         || (FrameType == FRAME_TYPE_IP_ADDRESS)
         || (FrameType == FRAME_TYPE_IP_ADDRESS_NO_EDIT))
        {
            // Check that new frame is valid
            MENU_ITEM_TYPE const *pMenuItem = FrameTableGetMenuItem((TUSIGN16)(FirstItemOfSequence + MenuPosCursor));
            if (pMenuItem != NULL)
            {
                frameValid = ValidityTestCheck((TUSIGN16)(pMenuItem->validTest));
            }
            else
            {
                frameValid = 0;
            }
        }
        else
        {
            frameValid = 0;
        }

    } while ((frameValid == 0) && (MenuPosCursor <= MenuPosCursorLim));

    if (MenuPosCursor > MenuPosCursorLim)
    {
        // gone beyond end of page - return to header
        returnVal = Back();
    }
    else
    {
        FrameTableSetSequenceSettings(MenuPosCursor, MenuPosCursorLim, FirstItemOfSequence);

        // Set new frame
        FrameTableAccessMenuPositionDetails(&MenuPosCursor, &MenuPosCursorLim, eFALSE);

        #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
        FrameTableUpdateMenuPositionDetailsForTFT(MenuPosCursor, MenuPosCursorLim, FirstItemOfSequence);
        #endif


        returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        if (returnVal)
        {
            returnVal = FrameTableSetCursorPosition(0);
        }
    }
    return returnVal;
}
#endif

#ifdef HMI_SUPPORTS_EASYSETUP
TUSIGN8 NextEasySetup(void)
{
    TUSIGN8 returnVal,frameValid;

    // Get current frame
    FRAME_FORMAT_TYPE const *pFrame = FrameTableGetCurrentFrame();

    // Get details of where we are in easy setup menu
    TUSIGN8 pageNumber = LOCAL_easySetup_number;
    TUSIGN8 lastStage, currentStage;
    FrameTableGetEasySetupStages(&lastStage, &currentStage);

    do
    {
        // increment to next frame
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        pFrame ++;
        #pragma diag_default=Pm088
        (void)FrameTableIncrementEasySetupStage();
        currentStage ++;

        // Check that new frame is valid
#ifdef HMI_SUPPORTS_SIGNALS_VALIDITY_TEST
		frameValid = ValidityTestCheck(
	#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
		(FRAME_FORMAT_TYPE const*)NULL,
	#endif
		(TUSIGN16)FrameTableGetEasySetupValidityTestIndex(currentStage, pageNumber)
	#ifdef VALIDITY_TEST_CALLER
			, SIGNALS_TEST_NOT_APPLICABLE
	#endif
		);
#else
		cursorValid = FrameTableGetSignal(cursor)->SignalsViewEnable;
#endif
    } while ((frameValid == 0) && (currentStage <= lastStage));

    if (currentStage > lastStage)
    {
        // gone beyond end of page - return to header
        returnVal = Back();
    }
    else
    {
        // Set new frame
        returnVal = FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
        if (returnVal)
        {
            returnVal = FrameTableSetCursorPosition(0);
        }
    }
    return returnVal;
}
#endif

TUSIGN8 NextOperatorFrame(TBOOL Auto)
{
    FRAME_FORMAT_TYPE const *pFrameAtStart = FrameTableGetCurrentFrame();
    FRAME_FORMAT_TYPE const *pFrame = pFrameAtStart;
    TUSIGN8  frameValid = 0;
    TUSIGN8 displayMode;

    do
    {
        // move to next frame
        // No alternative but to use pointer arithmetic
        #pragma diag_suppress=Pm088
        pFrame ++;
        #pragma diag_default=Pm088

        // if we have moved past the last header frames then wrap
        //                             ____________
        // first Operator Page ------ | Op. Page 1 |
        //                            |  __________|_
        //                            |_| Op. Page 2 |      |
        //                              |  __________|_     |
        //                              |_| Op. Page 3 |    |
        //                                |  __________|_   |
        //                                |_| Op. Page 4 |  v
        //                                  |            |
        // last Operator Page ------------- |____________|
        //

        if (pFrame->frameType != FRAME_TYPE_OPERATOR)
        {
            pFrame = FrameTableGetFirstOpPageAddress();

            #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
            // switch autoscroll on
            if (SFD_MULTI_OP.autoscrollEnable == eTRUE)
            {
                FrameTableEnableAutoscroll();
            }
            #endif
        }

        // check that the new operator page is available
        #pragma diag_suppress=Pm060
        switch (pFrame->frameDefinition.operatorFrame.frameNumber)
        {
        case 1:     displayMode = SFD_SIMPLE.displayMode_1;     break;

        #ifdef SECOND_OPERATOR_PAGE
        case 2:     displayMode = SFD_MULTI_OP.displayMode_2;   break;
        #endif
        #ifdef THIRD_OPERATOR_PAGE
        case 3:     displayMode = SFD_MULTI_OP.displayMode_3;   break;
        #endif
        #ifdef FOURTH_OPERATOR_PAGE
        case 4:     displayMode = SFD_MULTI_OP.displayMode_4;   break;
        #endif
        #ifdef FIFTH_OPERATOR_PAGE
        case 5:     displayMode = SFD_MULTI_OP.displayMode_5;   break;
        #endif
        #ifdef SIXTH_OPERATOR_PAGE
        case 6:     displayMode = SFD_MULTI_OP.displayMode_6;   break;
        #endif

        default:    displayMode = HMI_DISPLAY_MODE_OFF;         break;
        }
        #pragma diag_default=Pm060

        #ifdef HMI_SUPPORTS_GRAPH_VIEW
        if (Auto)
        {
            if ((displayMode > HMI_DISPLAY_MODE_OFF) && (displayMode <= HMI_DISPLAY_MODE_MAX) && (displayMode != HMI_DISPLAY_MODE_GRAPHIC))
            {
                frameValid = 1;
            }
        }
        else
        #endif
        {
             if ((displayMode > HMI_DISPLAY_MODE_OFF) && (displayMode <= HMI_DISPLAY_MODE_MAX))
            {
                frameValid = 1;
            }
        }

    } while ((frameValid == 0) && (pFrame != pFrameAtStart));

    return FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
}

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
// No restriction on the optimization of this function
TUSIGN8 EnterConfig_SelectionMade(void)
{
    TUSIGN8 ARMLevelFunctionTab[] =
    {
        #if defined(HMI_SUPPORTS_ARM_STANDARD) || defined(HMI_SUPPORTS_ARM_ADVANCED) || defined(HMI_SUPPORTS_ARM_SERVICE)
        ARM_ACCESS_LOGOUT,
        #endif
        #ifdef  HMI_SUPPORTS_ARM_READONLY
        ARM_ACCESS_READ_ONLY,
        #endif
        #ifdef  HMI_SUPPORTS_ARM_STANDARD
        ARM_ACCESS_STANDARD,
        #endif
        #ifdef  HMI_SUPPORTS_ARM_ADVANCED
        ARM_ACCESS_ADVANCED,
        #endif
        #ifdef  HMI_SUPPORTS_ARM_SERVICE
        ARM_ACCESS_SERVICE,
        #endif
        ARM_ACCESS_INVALID,
        ARM_ACCESS_INVALID,
        ARM_ACCESS_INVALID,
        ARM_ACCESS_INVALID
    };

    TUSIGN8 currentAccessLevel = AdjusterGetARMLevel();
    TUSIGN8 returnVal;
    TUSIGN8 requestedAccessLevel = ARM_ACCESS_INVALID;
    TUSIGN8 password[6] = {0,0,0,0,0,0};
    TUSIGN8 i;

    TUSIGN8 cursorPos = FrameTableGetCursorPosition();

    if (cursorPos < sizeof(ARMLevelFunctionTab)/sizeof(TUSIGN8))
    {
        requestedAccessLevel = ARMLevelFunctionTab[cursorPos];
    }

    switch (requestedAccessLevel)
    {
        case ARM_ACCESS_SERVICE:
        case ARM_ACCESS_ADVANCED:
        case ARM_ACCESS_STANDARD:
            if (currentAccessLevel == requestedAccessLevel)
            {
                returnVal = EnterConfig_AccessAllowed();
            }
            else
            {
                FrameTableSetRequestedARMLevel(requestedAccessLevel);
                if (AdjusterSetARMlevel(FrameTableGetRequestedARMLevel(), password))
                {
                    returnVal = EnterConfig_AccessAllowed();
                }
                else
                {
                    for (i = 0; i < PASSWORD_LENGTH; i++)
                    {
                        hmi_dynamic_data.Password[i] = 0x00;
                    }
                    FrameTableSetRequestedARMLevel(requestedAccessLevel);
                    returnVal = FrameTableSetCurrentFrame(FrameTableGetARMPasswordEntryAddress(),0,IDF_SPECIAL_NONE);
                }
            }
            break;

        case ARM_ACCESS_READ_ONLY:
            (void)AdjusterSetARMlevel(ARM_ACCESS_READ_ONLY,password);
            returnVal = EnterConfig_AccessAllowed();
            break;

        case ARM_ACCESS_LOGOUT:
        default:
            (void)AdjusterSetARMlevel(ARM_ACCESS_LOGOUT,password);
            FrameMover_ResetFrameMemory(0);
            returnVal = ReturnToOperatorPage();
            break;
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 EnterConfig_CheckPassword(void)
{
    TUSIGN8 returnVal;
    TUSIGN8 password[PASSWORD_LENGTH + 1];
    TUSIGN8 i;

    if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
    {
        returnVal = ReturnToOperatorPage();
    }
    else
    {
        returnVal = AdjusterPrepareNewVariable(1);

        for (i = 0; i < PASSWORD_LENGTH; i++)
        {
            password[i] = hmi_dynamic_data.Password[i];
        }
        password[PASSWORD_LENGTH] = 0x00;

        if (AdjusterSetARMlevel(FrameTableGetRequestedARMLevel(), password))
        {
            returnVal = EnterConfig_AccessAllowed();
        }
        else
        {
            returnVal = ReturnToOperatorPage();
        }
    }
    for (i = 0; i < PASSWORD_LENGTH; i++)
    {
        hmi_dynamic_data.Password[i] = 0x00;
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 EnterConfig_AccessAllowed(void)
{
    TUSIGN8 returnVal;
    FRAME_MEMORY_ENTRY pushedFrame, nextFrame;
    pushedFrame.pFrame = FrameTableGetFirstOpPageAddress();
    #pragma diag_suppress=Pm088
    #pragma diag_suppress=Pm057
    switch (LOCAL_currentOperatorPageNumber)
    {
    case 6:     pushedFrame.pFrame ++;
    case 5:     pushedFrame.pFrame ++;
    case 4:     pushedFrame.pFrame ++;
    case 3:     pushedFrame.pFrame ++;
    case 2:     pushedFrame.pFrame ++;
    default:    asm("nop");
    }
    #pragma diag_default=Pm057
    #pragma diag_default=Pm088
    pushedFrame.cursor = 0;

    // Push the last accessed operator frame to the frame stack and get the most recently accessed config frame
    nextFrame = FrameMemory_Forward(pushedFrame);

    if (nextFrame.pFrame == NULL)
    {
        nextFrame.pFrame = pushedFrame.pFrame->frameDefinition.operatorFrame.pNextFrame;
    }
	if (!ValidityTestCheck(
#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
	(FRAME_FORMAT_TYPE const*)NULL,
#endif
		(TUSIGN16)(nextFrame.pFrame->frameDefinition.configHeaderFrame.validTest)
#ifdef VALIDITY_TEST_CALLER
		, SIGNALS_TEST_NOT_APPLICABLE
#endif
	))
    {
        // Pulled frame not valid - Start with first config header and search for first valid header
        FRAME_FORMAT_TYPE const *ConfigHeaderFrame = FrameTableGetConfigHeaderAddress(0);

		while (!ValidityTestCheck(
#ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
		(FRAME_FORMAT_TYPE const*)NULL,
#endif
			(TUSIGN16)(ConfigHeaderFrame->frameDefinition.configHeaderFrame.validTest)
#ifdef VALIDITY_TEST_CALLER
			, SIGNALS_TEST_NOT_APPLICABLE
#endif
		))
        {
            #pragma diag_suppress=Pm088
            ConfigHeaderFrame ++;
            #pragma diag_default=Pm088
        }

        nextFrame.pFrame = ConfigHeaderFrame;
        nextFrame.cursor = 0;
        FrameMemory_Sideways(nextFrame);
    }

    #if defined(IDF_SPECIAL_INH_ENTER_CONFIG)
    returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_INH_ENTER_CONFIG);
    #else
    returnVal = FrameTableSetCurrentFrame(nextFrame.pFrame,nextFrame.cursor,IDF_SPECIAL_NONE);
    #endif

    if (returnVal == 1)
    {
        returnVal = ConfigModeManagerSetConfigurationMode(1);
    }
    return returnVal;
}
#endif

// No restriction on the optimization of this function
TUSIGN8 ReturnToOperatorPage(void)
{
    FRAME_FORMAT_TYPE const *pFrame = FrameTableGetFirstOpPageAddress();
    switch (LOCAL_currentOperatorPageNumber)
    {
    // break intentionally omitted from each case
    #pragma diag_suppress=Pm057
    // No alternative but to use pointer arithmetic
    #pragma diag_suppress=Pm088
    case 6:     pFrame ++;
    case 5:     pFrame ++;
    case 4:     pFrame ++;
    case 3:     pFrame ++;
    case 2:     pFrame ++;
    default:    asm("nop");
    #pragma diag_default=Pm088
    #pragma diag_default=Pm057
    }
    #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    if (LOCAL_savedAutoscroll)
    {
        FrameTableEnableAutoscroll();
    }
    #endif

    #if defined(IDF_SPECIAL_INH_RETURN_TO_OPERATOR_PAGE)
    return FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_INH_RETURN_TO_OPERATOR_PAGE);
    #else
    return FrameTableSetCurrentFrame(pFrame,0,IDF_SPECIAL_NONE);
    #endif
}


////////////////////////////////////////////////////////////////////////////
//
//  M   M   OOO   DDD    U   U  L      EEEEE      TTTTT  EEEEE   SSSS  TTTTT
//  MM MM  O   O  D  D   U   U  L      E            T    E      S        T
//  M M M  O   O  D   D  U   U  L      EEE          T    EEE     SSS     T
//  M   M  O   O  D  D   U   U  L      E            T    E          S    T
//  M   M   OOO   DDD     UUU   LLLLL  EEEEE        T    EEEEE  SSSS     T
//
////////////////////////////////////////////////////////////////////////////

#if defined (HMI_MODULE_TEST) && defined(HMI_MODULE_TEST_FRAMEMOVER)

//#include "..\..\coordinator\interface\subsystem_idx.h"
#include "HART/Interface/Hart_idx.h"
#include "Hmi/Interface/HMI_idx.h"

void InitialiseFrameStack_ForModuleTest(void);

//TUSIGN8 FrameMoverInitialise_ModuleTest(void);
TUSIGN8 FrameStack_Push_Pull_ModuleTest(void);

TUSIGN8 Menu_Exit_ModuleTest(void);
TUSIGN8 NextConfigLevel_ModuleTest(void);
TUSIGN8 SelectConfigLevel_ModuleTest(void);
TUSIGN8 MoveInMenu_ModuleTest(void);
TUSIGN8 SelectFromMenu_Edit_Back_ModuleTest(void);
TUSIGN8 MoveCursor_ModuleTest(void);
// -TO-DO- TUSIGN8 NextOperatorFrame_ModuleTest(void);
// -TO-DO- TUSIGN8 NextEasySetup_ModuleTest(void);
TUSIGN8 FrameMoverMove_ModuleTest(void);


// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
void InitialiseFrameStack_ForModuleTest(void)
{
    TUSIGN8 i;
    for (i = 0; i < 5; i ++)
        frameStack[i] = NULL;

    frameStackPointer = 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 FrameMover_ModuleTest(void)
{
    TUSIGN8 rv = 0;

    rv |= FrameStack_Push_Pull_ModuleTest();
    rv |= Menu_Exit_ModuleTest();
    rv |= NextConfigLevel_ModuleTest();
    rv |= SelectConfigLevel_ModuleTest();
    rv |= MoveInMenu_ModuleTest();
    rv |= SelectFromMenu_Edit_Back_ModuleTest();
    rv |= MoveCursor_ModuleTest();
    rv |= FrameMoverMove_ModuleTest();

    return rv;
}



// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 FrameStack_Push_Pull_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_1;
    FRAME_FORMAT_TYPE   dummyFrame_2;
    FRAME_FORMAT_TYPE   dummyFrame_3;
    FRAME_FORMAT_TYPE   dummyFrame_4;
    FRAME_FORMAT_TYPE   dummyFrame_5;
    FRAME_FORMAT_TYPE   dummyFrame_6;

    FRAME_FORMAT_TYPE const *pPulledFrame;

    InitialiseFrameStack_ForModuleTest();


    // Push 1 frame onto stack
    if (FrameStack_Push(&dummyFrame_1) != 1)    return 1;

    if (frameStack[0] != &dummyFrame_1 ||
        frameStack[1] != NULL ||
        frameStack[2] != NULL ||
        frameStack[3] != NULL ||
        frameStack[4] != NULL ||
        frameStackPointer != 1)
        return 1;

    // Pull 1 frame

    pPulledFrame = FrameStack_Pull(0);
    if (pPulledFrame != &dummyFrame_1 ||
        frameStackPointer != 0)

        return 1;

    //push 2
    //pull 1
    if (FrameStack_Push(&dummyFrame_2) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_3) != 1)    return 1;
    pPulledFrame = FrameStack_Pull(0);

    if (pPulledFrame != &dummyFrame_3 ||
        frameStack[0] != &dummyFrame_2 ||
        frameStack[1] != &dummyFrame_3 ||
        frameStack[2] != NULL ||
        frameStack[3] != NULL ||
        frameStack[4] != NULL ||
        frameStackPointer != 1)

        return 1;

    // push 4
    // pull First

    if (FrameStack_Push(&dummyFrame_5) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_4) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_3) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_1) != 0)    return 1;

    if (frameStack[0] != &dummyFrame_2 ||
        frameStack[1] != &dummyFrame_5 ||
        frameStack[2] != &dummyFrame_4 ||
        frameStack[3] != &dummyFrame_3 ||
        frameStack[4] != &dummyFrame_1 ||
        frameStackPointer != 4)

        return 1;

    pPulledFrame = FrameStack_Pull(1);

    if (pPulledFrame != &dummyFrame_2 ||
        frameStackPointer != 0)

        return 1;

    // push 6
    if (FrameStack_Push(&dummyFrame_1) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_2) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_3) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_4) != 1)    return 1;
    if (FrameStack_Push(&dummyFrame_5) != 0)    return 1;
    if (FrameStack_Push(&dummyFrame_6) != 0)    return 1;

    if (frameStack[0] != &dummyFrame_1 ||
        frameStack[1] != &dummyFrame_2 ||
        frameStack[2] != &dummyFrame_3 ||
        frameStack[3] != &dummyFrame_4 ||
        frameStack[4] != &dummyFrame_6 ||
        frameStackPointer != 4)

        return 1;

    return 0;
}


// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 Menu_Exit_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_1;
    FRAME_FORMAT_TYPE   dummyFrame_2;

    InitialiseFrameStack_ForModuleTest();

    dummyFrame_1.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_1.frameDefinition.operatorFrame.pNextFrame = &dummyFrame_2;

    dummyFrame_2.frameType = FRAME_TYPE_CONFIG_HEADER;


    if (FrameTableSetCurrentFrame(&dummyFrame_1) != 1)  return 1;
    if (Menu() != 1)                                    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    if (Exit() != 1)                                    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_1)   return 1;

    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 NextConfigLevel_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_1;
    FRAME_FORMAT_TYPE   dummyFrame_2;
    FRAME_FORMAT_TYPE   dummyFrame_3;
    FRAME_FORMAT_TYPE   dummyFrame_4;
    FRAME_FORMAT_TYPE   dummyFrame_5;

    // Three config headers
    dummyFrame_1.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_2.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_3.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_4.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_5.frameType = FRAME_TYPE_OPERATOR;

    FrameTableSetConfigHeaders_ModuleTest(&dummyFrame_2, &dummyFrame_4);

    InitialiseFrameStack_ForModuleTest();

    if (FrameTableSetCurrentFrame(&dummyFrame_2) != 1)  return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_3)   return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_4)   return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_4)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_3)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;



    // two config headers
    dummyFrame_1.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_2.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_3.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_4.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_5.frameType = FRAME_TYPE_OPERATOR;

    FrameTableSetConfigHeaders_ModuleTest(&dummyFrame_2, &dummyFrame_3);

    InitialiseFrameStack_ForModuleTest();

    if (FrameTableSetCurrentFrame(&dummyFrame_2) != 1)  return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_3)   return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_3)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    // one config header
    dummyFrame_1.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_2.frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame_3.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_4.frameType = FRAME_TYPE_OPERATOR;
    dummyFrame_5.frameType = FRAME_TYPE_OPERATOR;

    FrameTableSetConfigHeaders_ModuleTest(&dummyFrame_2, &dummyFrame_2);

    InitialiseFrameStack_ForModuleTest();

    if (FrameTableSetCurrentFrame(&dummyFrame_2) != 1)  return 1;

    if (NextConfigLevel(0) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    if (NextConfigLevel(1) != 1)                        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_2)   return 1;

    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 SelectConfigLevel_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_C;
    FRAME_FORMAT_TYPE   dummyFrame_M;
    FRAME_FORMAT_TYPE   dummyFrame_N;
    FRAME_FORMAT_TYPE   dummyFrame_S;
    FRAME_FORMAT_TYPE   dummyFrame_E;
    FRAME_FORMAT_TYPE   dummyFrame_A;

    dummyFrame_C.frameType = FRAME_TYPE_CONFIG_HEADER;

    dummyFrame_M.frameType = FRAME_TYPE_MENU;
    dummyFrame_M.frameDefinition.menuFrame.firstItem = 1;
    dummyFrame_M.frameDefinition.menuFrame.lastItem = 5;

    dummyFrame_N.frameType = FRAME_TYPE_NUMERICAL_PARAM;        //FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT
    dummyFrame_N.frameDefinition.numericalParamFrame.attributes.maxCharacters = 6;

    dummyFrame_S.frameType = FRAME_TYPE_STRING_PARAM_PACKED;    //FRAME_TYPE_STRING_PARAM
    dummyFrame_S.frameDefinition.stringParamFrame.attributes.maxCharacters = 20;

    dummyFrame_E.frameType = FRAME_TYPE_ENUMERATED_PARAM;       //FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP

    dummyFrame_A.frameType = FRAME_TYPE_MANUAL_ADJUST;          //FRAME_TYPE_AUTO_ADJUST;

    // select a menu frame
    dummyFrame_C.frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame_M;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_C) != 1)  return 1;
    if (SelectConfigLevel() != 1)                       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPositionLimit() != 4)        return 1;

    // select a numerical frame
    dummyFrame_C.frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame_N;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_C) != 1)  return 1;
    if (SelectConfigLevel() != 1)                       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_N)   return 1;
    if (FrameTableGetCursorPositionLimit() != 7)        return 1;

    // select a string frame
    dummyFrame_C.frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame_S;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_C) != 1)  return 1;
    if (SelectConfigLevel() != 1)                       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_S)   return 1;
    if (FrameTableGetCursorPositionLimit() != 20)       return 1;

    // select an enum frame
    dummyFrame_C.frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame_E;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_C) != 1)  return 1;
    if (SelectConfigLevel() != 1)                       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_E)   return 1;

    // select an adjust frame
    dummyFrame_C.frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame_A;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_C) != 1)  return 1;
    if (SelectConfigLevel() != 1)                       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_A)   return 1;

    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 MoveInMenu_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_M;

    dummyFrame_M.frameType = FRAME_TYPE_MENU;
    dummyFrame_M.frameDefinition.menuFrame.firstItem = 1;
    dummyFrame_M.frameDefinition.menuFrame.lastItem = 5;


    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_M) != 1)  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPositionLimit() != 4)        return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 2)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 3)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 4)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 4)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 3)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 2)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;


    dummyFrame_M.frameDefinition.menuFrame.firstItem = 1;
    dummyFrame_M.frameDefinition.menuFrame.lastItem = 1;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_M) != 1)  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPositionLimit() != 0)        return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 SelectFromMenu_Edit_Back_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame_M;

    FRAME_FORMAT_TYPE   dummyFrame[4];

//    FRAME_FORMAT_TYPE   dummyFrame_A;
//    FRAME_FORMAT_TYPE   dummyFrame_E;
//    FRAME_FORMAT_TYPE   dummyFrame_S;
//    FRAME_FORMAT_TYPE   dummyFrame_N;

    dummyFrame_M.frameType = FRAME_TYPE_MENU;
    dummyFrame_M.frameDefinition.menuFrame.pNextFrame = &dummyFrame[0];
    dummyFrame_M.frameDefinition.menuFrame.firstItem = 10;
    dummyFrame_M.frameDefinition.menuFrame.lastItem = 13;

    dummyFrame[3].frameType = FRAME_TYPE_NUMERICAL_PARAM;        //FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT
    dummyFrame[3].frameDefinition.numericalParamFrame.attributes.maxCharacters = 6;

    dummyFrame[2].frameType = FRAME_TYPE_STRING_PARAM_PACKED;    //FRAME_TYPE_STRING_PARAM
    dummyFrame[2].frameDefinition.stringParamFrame.attributes.maxCharacters = 20;

    dummyFrame[1].frameType = FRAME_TYPE_ENUMERATED_PARAM;       //FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP

    dummyFrame[0].frameType = FRAME_TYPE_MANUAL_ADJUST;          //FRAME_TYPE_AUTO_ADJUST;

    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame_M) != 1)  return 1;   //Menu Frame
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPositionLimit() != 3)        return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;   //cursor at 0

    if (SelectFromMenu() != 1)                          return 1;   //Select Adjust frame (0)
    if (FrameTableGetCurrentFrame() != &dummyFrame[0])  return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;
    if (Back() != 1)                                    return 1;   //Go back to menu
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;   //cursor at 1
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (SelectFromMenu() != 1)                          return 1;   //Select Enum frame (1)
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])  return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Edit(1) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 1)                 return 1;
    if (Edit(0) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Back() != 1)                                    return 1;   //Go back to menu
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;   //cursor at 2
    if (FrameTableGetCursorPosition() != 2)             return 1;

    if (SelectFromMenu() != 1)                          return 1;   //Select String frame (2)
    if (FrameTableGetCurrentFrame() != &dummyFrame[2])  return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Edit(1) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 1)                 return 1;
    if (Edit(0) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Back() != 1)                                    return 1;   //Go back to menu
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPosition() != 2)             return 1;

    if (MoveInMenu(0) != 1)                             return 1;   //cursor at 3
    if (FrameTableGetCursorPosition() != 3)             return 1;

    if (SelectFromMenu() != 1)                          return 1;   //Select Numeric frame (3)
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])  return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Edit(1) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 1)                 return 1;
    if (Edit(0) != 1)                                   return 1;
    if (FrameTableIsEditEnabled() != 0)                 return 1;
    if (Back() != 1)                                    return 1;   //Go back to menu
    if (FrameTableGetCurrentFrame() != &dummyFrame_M)   return 1;
    if (FrameTableGetCursorPosition() != 3)             return 1;

    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 MoveCursor_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame;
    TUSIGN8             i;
    TUSIGN8             negVal;
    TUSIGN8             varStr[10];

    dummyFrame.frameType = FRAME_TYPE_STRING_PARAM_PACKED;    //FRAME_TYPE_STRING_PARAM
    dummyFrame.frameDefinition.stringParamFrame.attributes.maxCharacters = 10;
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame) != 1)    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame)     return 1;
    if (FrameTableGetCursorPositionLimit() != 10)       return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveCursor(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 10)            return 1;

    for (i = 0; i <= 10; i ++)
    {
        if (MoveCursor(0) != 1)                         return 1;
        if (FrameTableGetCursorPosition() != i)         return 1;
    }

    if (MoveCursor(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    //Numerical parameter frame
    dummyFrame.frameType = FRAME_TYPE_NUMERICAL_PARAM;
    dummyFrame.frameDefinition.numericalParamFrame.attributes.maxCharacters = 5;
    dummyFrame.frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn = MAX_RESOLUTION;

    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame) != 1)    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame)     return 1;
    if (FrameTableGetCursorPositionLimit() != 6)        return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveCursor(1) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 6)             return 1;

    for (i = 0; i <= 6; i ++)
    {
        if (MoveCursor(0) != 1)                         return 1;
        if (FrameTableGetCursorPosition() != i)         return 1;
    }

    if (MoveCursor(0) != 1)                             return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    //Numerical parameter frame
    dummyFrame.frameType = FRAME_TYPE_NUMERICAL_PARAM;    //FRAME_TYPE_STRING_PARAM
    dummyFrame.frameDefinition.numericalParamFrame.attributes.maxCharacters = 3;
    dummyFrame.frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn = 1;

    strcpy((char *)varStr,"1.2");
    AdjusterSetVariableAsString_ModuleTest(varStr);
    if (strcmp((char *)AdjusterGetVariableAsString(&negVal),"1.2") != 0)    return 1;

    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame) != 1)    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame)     return 1;
    if (FrameTableGetCursorPositionLimit() != 4)        return 1;
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveCursor(1) != 1)                             return 1;       // move cursor to +/-
    if (FrameTableGetCursorPosition() != 4)             return 1;

    if (MoveCursor(0) != 1)                             return 1;       // move cursor to +/-
    if (FrameTableGetCursorPosition() != 0)             return 1;

    if (MoveCursor(0) != 1)                             return 1;       // move cursor to '1'
    if (FrameTableGetCursorPosition() != 1)             return 1;

    if (MoveCursor(0) != 1)                             return 1;       // skip '.' move cursor to '1'
    if (FrameTableGetCursorPosition() != 3)             return 1;

    if (MoveCursor(0) != 1)                             return 1;       // move cursor to 'CANCEL'
    if (FrameTableGetCursorPosition() != 4)             return 1;
    return 0;
}

// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN8 FrameMoverMove_ModuleTest(void)
{
    FRAME_FORMAT_TYPE   dummyFrame[11];
    TUSIGN8 i;

    FrameTableSetConfigHeaders_ModuleTest(&dummyFrame[1], &dummyFrame[2]);

    dummyFrame[0].frameType = FRAME_TYPE_OPERATOR;
    dummyFrame[0].frameDefinition.operatorFrame.pNextFrame= &dummyFrame[1];

    dummyFrame[1].frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame[1].frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame[3];

    dummyFrame[2].frameType = FRAME_TYPE_CONFIG_HEADER;
    dummyFrame[2].frameDefinition.configHeaderFrame.pNextFrame = &dummyFrame[4];

    dummyFrame[3].frameType = FRAME_TYPE_MENU;
    dummyFrame[3].frameDefinition.menuFrame.pNextFrame = &dummyFrame[5];
    dummyFrame[3].frameDefinition.menuFrame.firstItem = 0;
    dummyFrame[3].frameDefinition.menuFrame.lastItem = 1;

    dummyFrame[4].frameType = FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT;
    dummyFrame[4].frameDefinition.numericalParamFrame.value.ssIdx = 1; //HMI_IDX;
    dummyFrame[4].frameDefinition.numericalParamFrame.value.objIdx = HMI_IDX_contrast;
    dummyFrame[4].frameDefinition.numericalParamFrame.value.attribIdx = 0;
    dummyFrame[4].frameDefinition.numericalParamFrame.attributes.maxCharacters = 3;
    dummyFrame[4].frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn = 0;

    dummyFrame[5].frameType = FRAME_TYPE_MENU;
    dummyFrame[5].frameDefinition.menuFrame.pNextFrame = &dummyFrame[7];
    dummyFrame[5].frameDefinition.menuFrame.firstItem = 2;
    dummyFrame[5].frameDefinition.menuFrame.lastItem = 5;

    dummyFrame[6].frameType = FRAME_TYPE_NUMERICAL_PARAM;
    dummyFrame[6].frameDefinition.numericalParamFrame.value.ssIdx = 1; //HMI_IDX;
    dummyFrame[6].frameDefinition.numericalParamFrame.value.objIdx = HMI_IDX_contrast;
    dummyFrame[6].frameDefinition.numericalParamFrame.value.attribIdx = 0;
    dummyFrame[6].frameDefinition.numericalParamFrame.attributes.maxCharacters = 4;
    dummyFrame[6].frameDefinition.numericalParamFrame.attributes.characterSet_dpPosn = 1;

    dummyFrame[7].frameType = FRAME_TYPE_STRING_PARAM_PACKED;
    dummyFrame[7].frameDefinition.stringParamFrame.stringText.ssIdx = 0; //HART_IDX;
    dummyFrame[7].frameDefinition.stringParamFrame.stringText.objIdx = HART_IDX_tag;
    dummyFrame[7].frameDefinition.stringParamFrame.stringText.attribIdx = -1;
    dummyFrame[7].frameDefinition.stringParamFrame.attributes.maxCharacters = 8;
    dummyFrame[7].frameDefinition.stringParamFrame.attributes.characterSet_dpPosn = 0;

    dummyFrame[8].frameType = FRAME_TYPE_ENUMERATED_PARAM;
    dummyFrame[8].frameDefinition.enumParamFrame.value.ssIdx = 1; //HMI_IDX;
    dummyFrame[8].frameDefinition.enumParamFrame.value.objIdx = HMI_IDX_mainOperatorView;
    dummyFrame[8].frameDefinition.enumParamFrame.value.attribIdx = 0;
    dummyFrame[8].frameDefinition.enumParamFrame.engUnitsFlag = 0;

    dummyFrame[9].frameType = FRAME_TYPE_MANUAL_ADJUST;
    dummyFrame[9].frameDefinition.adjParamFrame.value_ssIdx = 1; //HMI_IDX;
    dummyFrame[9].frameDefinition.adjParamFrame.adjvalue_objidx = HMI_IDX_contrast;
    dummyFrame[9].frameDefinition.adjParamFrame.adjvalue_attribidx = 0;

    dummyFrame[10].frameType = FRAME_TYPE_AUTO_ADJUST;
    dummyFrame[10].frameDefinition.adjParamFrame.value_ssIdx = 1; //HMI_IDX;
    dummyFrame[10].frameDefinition.adjParamFrame.adjvalue_objidx = HMI_IDX_contrast;
    dummyFrame[10].frameDefinition.adjParamFrame.adjvalue_attribidx = 0;

/*
     --------------      --------------      --------------      --------------      --------------
    |   Op. Frame  |____|   Cfg Header |____|     Menu     |____|     Menu     |____|    String    |----------
    |     [0]      |    |     [1]      |    |     [3]      | |  |     [5]      | |  |     [7]      | Edit     |
     --------------      --------------      --------------  |   --------------  |   --------------           |
                               |                             |                   |              --------------
                               |                             |                   |   --------------
                               |                             |                   |__|     Enum     |----------
                               |                             |                   |  |     [8]      | Edit     |
                               |                             |                   |   --------------           |
                               |                             |                   |              --------------
                               |                             |                   |   --------------
                               |                             |                   |__|    Man Adj   |
                               |                             |                   |  |     [9]      |
                               |                             |                   |   --------------
                               |                             |                   |
                               |                             |                   |   --------------
                               |                             |                   |__|   Auto Adj   |
                               |                             |                      |     [10]     |
                               |                             |                       --------------
                               |                             |   --------------
                               |                             |__|    Number    |----------
                               |                                |     [6]      | Edit     |
                               |                                 --------------           |
                         --------------      --------------                 --------------
                        |   Cfg Header |____|    Number    |
                        |     [2]      |    |     [4]      |
                         --------------      --------------

*/
    InitialiseFrameStack_ForModuleTest();
    if (FrameTableSetCurrentFrame(&dummyFrame[0]) != 1)    return 1;

    // Operator Frame
    // Left Key     - COMMAND_NEXT_OPERATOR_FRAME   - has no effect
    // Up Key       - COMMAND_DO_NOT_MOVE           - has no effect
    // Down Key     - COMMAND_DO_NOT_MOVE           - has no effect
    // Right Key    - COMMAND_MENU                  - selects 1st config header

    if (FrameMoverMove(COMMAND_NEXT_OPERATOR_FRAME) != 1)   return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[0])      return 1;

    if (FrameMoverMove(COMMAND_DO_NOT_MOVE) != 1)           return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[0])      return 1;

    if (FrameMoverMove(COMMAND_MENU) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])      return 1;

    // Config Header Frame
    // Up Key       - COMMAND_MOVE_UP_LEVEL
    // Down Key     - COMMAND_MOVE_DOWN_LEVEL
    // Left Key     - COMMAND_EXIT
    // Right Key    - COMMAND_SELECT_CONFIG_LEVEL

    if (FrameMoverMove(COMMAND_MOVE_UP_LEVEL) != 1)         return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[2])      return 1;

    if (FrameMoverMove(COMMAND_MOVE_DOWN_LEVEL) != 1)       return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])      return 1;

    if (FrameMoverMove(COMMAND_EXIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[0])      return 1;

    if (FrameMoverMove(COMMAND_MENU) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])      return 1;

    if (FrameMoverMove(COMMAND_SELECT_CONFIG_LEVEL) != 1)   return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;

    //Menu Frame
    // Up Key       - COMMAND_MOVE_UP_MENU
    // Down Key     - COMMAND_MOVE_DOWN_MENU
    // Left Key     - COMMAND_BACK
    // Right Key    - COMMAND_SELECT_FROM_MENU

    if (FrameTableGetCursorPosition() != 0)                 return 1;

    if (FrameMoverMove(COMMAND_MOVE_UP_MENU) != 1)          return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;
    if (FrameTableGetCursorPosition() != 1)                 return 1;

    if (FrameMoverMove(COMMAND_MOVE_DOWN_MENU) != 1)        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])      return 1;

    if (FrameMoverMove(COMMAND_SELECT_CONFIG_LEVEL) != 1)   return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;

    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;

    // Sub menu
    // Select first item (String param frame)
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;

    // String param Frame (edit disabled)
    // Right Key    - COMMAND_EDIT
    // Up Key       -
    // Down Key     -
    // Left Key     - COMMAND_BACK
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // String param frame (Edit Enabled)
    // Right Key    - COMMAND_OK_CANCEL
    // Up Key       - COMMAND_ADJUST_UP (Adjuster)
    // Down Key     - COMMAND_ADJUST_DOWN (Adjuster)
    // Left Key     - COMMAND_MOVE_CURSOR

    // COMMAND_OK_CANCEL - with cursor = 0 therefore "OK" - single step to check operation
    // Data has not been adjusted therfore writing of data will not happen
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Adjust 1st character
    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_OK_CANCEL - with cursor = 0 therefore "OK" - single step to check operation
    // Data has been adjusted therfore writing of data will happen
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    AdjusterAdjust(COMMAND_ADJUST_DOWN);
    for (i = 0; i < 8; i++)
    {
        if (FrameMoverMove(COMMAND_MOVE_CURSOR) != 1)       return 1;
    }

    if (FrameTableGetCursorPosition() != FrameTableGetCursorPositionLimit())
                                                            return 1;

    // COMMAND_OK_CANCEL - with cursor = limit therefore "CANCEL" - single step to check operation
    // Data has been adjusted but writing of data should not happen because "CANCEL"
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[7])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;


    // Return to sub menu
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // move to next item in sub menu
    if (FrameMoverMove(COMMAND_MOVE_DOWN_MENU) != 1)        return 1;
    if (FrameTableGetCursorPosition() != 1)                 return 1;

    // select 2nd frame (Enum Param frame)
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;

    // Enum param Frame (edit disabled)
    // Right Key    - COMMAND_EDIT
    // Up Key       -
    // Down Key     -
    // Left Key     - COMMAND_BACK

    if (FrameTableIsEditEnabled() != 0)                     return 1;

    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Enum param frame (Edit Enabled)
    // Right Key    - COMMAND_OK
    // Up Key       - COMMAND_ADJUST_UP (Adjuster)
    // Down Key     - COMMAND_ADJUST_DOWN (Adjuster)
    // Left Key     - COMMAND_CANCEL

    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_CANCEL - single step to check operation
    // Data has been adjusted but writing of data should not happen because "CANCEL"
    if (FrameMoverMove(COMMAND_CANCEL) != 1)                return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;

    // COMMAND_OK - single step to check operation
    // Data has not been adjusted so writing of data should not happen
    if (FrameMoverMove(COMMAND_OK) != 1)                    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;

    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_OK - single step to check operation
    // Data has been adjusted so writing of data should happen
    if (FrameMoverMove(COMMAND_OK) != 1)                    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[8])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Return to sub menu
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 1)                 return 1;

    // move to next item in sub menu
    if (FrameMoverMove(COMMAND_MOVE_DOWN_MENU) != 1)        return 1;
    if (FrameTableGetCursorPosition() != 2)                 return 1;

    // select 3nd frame (Manual Adjust frame)
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[9])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Manual Adjust frame (Edit automatically Enabled)
    // Right Key    - COMMAND_EXIT_ADJUST_OK
    // Up Key       - COMMAND_ADJUST_UP (Adjuster)
    // Down Key     - COMMAND_ADJUST_DOWN (Adjuster)
    // Left Key     - COMMAND_EXIT_ADJUST_CANCEL

    // COMMAND_OK - single step to check operation
    // Data has not been adjusted so writing of data should not happen
    if (FrameMoverMove(COMMAND_EXIT_ADJUST_OK) != 1)        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 2)                 return 1;

    // re-select Manual Adjust frame
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[9])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_OK - single step to check operation
    // Data has been adjusted so writing of data should happen
    if (FrameMoverMove(COMMAND_EXIT_ADJUST_OK) != 1)        return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 2)                 return 1;

    // re-select Manual Adjust frame
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[9])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_OK - single step to check operation
    // Data is written when it is adjusted so unadjusted value must be re-written
    if (FrameMoverMove(COMMAND_EXIT_ADJUST_CANCEL) != 1)    return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 2)                 return 1;

    // move to next item in sub menu
    if (FrameMoverMove(COMMAND_MOVE_DOWN_MENU) != 1)        return 1;
    if (FrameTableGetCursorPosition() != 3)                 return 1;

    // select 4th frame (Auto Adjust frame)
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[10])     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Auto Adjust frame (There  is no edit state)
    // Right Key    - COMMAND_DO_AUTO_ADJUST (Adjuster)
    // Up Key       - COMMAND_DO_NOT_MOVE
    // Down Key     - COMMAND_DO_NOT_MOVE
    // Left Key     - COMMAND_BACK

    if (FrameMoverMove(COMMAND_DO_NOT_MOVE) != 1)           return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[10])     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    //Return to sub menu
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[5])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 3)                 return 1;

    //Return to menu
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // move to next item in menu
    if (FrameMoverMove(COMMAND_MOVE_UP_MENU) != 1)          return 1;
    if (FrameTableGetCursorPosition() != 1)                 return 1;

    // select numeric frame
    if (FrameMoverMove(COMMAND_SELECT_FROM_MENU) != 1)      return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // numeric parameter frame (Edit Disabled)
    // Right Key    - COMMAND_EDIT
    // Up Key       -
    // Down Key     -
    // Left Key     - COMMAND_BACK

    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // numeric param frame (Edit Enabled)
    // Right Key    - COMMAND_OK_CANCEL
    // Up Key       - COMMAND_ADJUST_UP (Adjuster)
    // Down Key     - COMMAND_ADJUST_DOWN (Adjuster)
    // Left Key     - COMMAND_MOVE_CURSOR

    // COMMAND_OK_CANCEL - with cursor = 0 therefore "OK" - single step to check operation
    // Data has not been adjusted therfore writing of data will not happen
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Adjust 1st character
    AdjusterAdjust(COMMAND_ADJUST_UP);
    // COMMAND_OK_CANCEL - with cursor = 0 therefore "OK" - single step to check operation
    // Data has been adjusted therfore writing of data will happen
    // NOTE:- 1st character (+/-) has been changed from + to - therefore data is outside limits
    //        and error code will be returned
    if (FrameMoverMove(COMMAND_OK_CANCEL) < ERROR)          return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    AdjusterAdjust(COMMAND_ADJUST_DOWN);
    // COMMAND_OK_CANCEL - with cursor = 0 therefore "OK" - single step to check operation
    // Data has been adjusted therfore writing of data will happen
    // NOTE:- 1st character (+/-) has been changed back to + therefore data is inside limits
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    // Re-enable editing
    if (FrameMoverMove(COMMAND_EDIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 1)                     return 1;
    if (FrameTableGetCursorPosition() != 0)                 return 1;

    AdjusterAdjust(COMMAND_ADJUST_DOWN);
    for (i = 0; i < 4; i++)
    {
        if (FrameMoverMove(COMMAND_MOVE_CURSOR) != 1)       return 1;
    }

    if (FrameTableGetCursorPosition() != FrameTableGetCursorPositionLimit())
                                                            return 1;

    // COMMAND_OK_CANCEL - with cursor = limit therefore "CANCEL" - single step to check operation
    // Data has been adjusted but writing of data should not happen because "CANCEL"
    if (FrameMoverMove(COMMAND_OK_CANCEL) != 1)             return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[6])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;


    // Return to menu
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[3])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;
    if (FrameTableGetCursorPosition() != 1)                 return 1;

    // Return to config Header
    if (FrameMoverMove(COMMAND_BACK) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[1])      return 1;
    if (FrameTableIsEditEnabled() != 0)                     return 1;

    // Return to operator page
    if (FrameMoverMove(COMMAND_EXIT) != 1)                  return 1;
    if (FrameTableGetCurrentFrame() != &dummyFrame[0])      return 1;

    return 0;
}




#endif

