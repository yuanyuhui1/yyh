//----------------------------------------------------------------------------------------------------------
//
//  H   H  M   M  III         222   000   000   222        1    1       000   000
//  H   H  MM MM   I       / 2   2 0   0 0   0 2   2      11   11      0   0 0   0
//  H   H  M M M   I      /      2 0  00 0  00     2       1    1      0  00 0  00
//  HHHHH  M   M   I     /      2  0 0 0 0 0 0    2        1    1      0 0 0 0 0 0
//  H   H  M   M   I    /      2   00  0 00  0   2         1    1      00  0 00  0
//  H   H  M   M   I   /      2    0   0 0   0  2     ..   1    1   .. 0   0 0   0
//  H   H  M   M  III        22222  000   000  22222  ..  111  111  ..  000   000
//
// NOTE: See also definition of "HIGH_LEVEL_VERSION_INFO" in "FrameTableDefinitions.h"
//
//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              Hmi
 Description                    Coordinator of the HMI subsystem. Is responsible to schedule internal activities etc.

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003
#include <intrinsics.h>

#include "IncludeList_Implementation.h"

#include "System/Interface/system.h"

#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ServicePort/Interface/servicePort.h"

#include "Hmi/Interface/HMI_idx.h"
#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"

#include "frameTableDefinitions.h"

#include "ConfigModeManager.h"
#include "KeystateDefinitions.h"
#include "FrameTable.h"
#include "Adjuster.h"
#include "FrameMover.h"
#include "PacketManager.h"
#include "writeProtect.h"

#if !defined(HMIGEN_RELEASE_VERSION) || !defined(HMIGEN_MAJOR_VERSION) || !defined(HMIGEN_MINOR_VERSION) || !defined(HMIGEN_CONFIG_VERSION) || (HMIGEN_MAJOR_VERSION < 12) || ((HMIGEN_MAJOR_VERSION == 12) && (HMIGEN_MINOR_VERSION < 2))
    #include "layer2.h"
#else
    #include "IncludeList_layer2.h"
#endif


#include "IncludeList_Constructor.h"

#include "HmiCoordinator.h"
#include "ParameterDump.h"

#if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
#include "TrendView.h"
#endif

#ifdef EDP300_TEST_ONLY
#include "EDP300_Mode.h"    // EDP300_TEST_ONLY
#endif


#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
#pragma diag_suppress=Pm071
#include "low_level\core\Driver.h"
#pragma diag_default=Pm071
#endif

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION)
#include "../../TFT_DISPLAY/Interface/TFT_DISPLAY.h"
#endif


#ifdef AZ200_SPECIAL_FEATURES_HMI
#include "../../O2_Calibration/Interface/o2_Calibration.h"
#include "../../O2_Calibration/Interface/o2_Calibration_idx.h"
#include "../../nvinterface/Interface/NVinterface.h"
#include "specialOperation.h"
#endif

#ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
#include "specialOperation.h"
#endif

        // private methods
TUSIGN8 HandleKeyRepeat(TUSIGN8 keyState);
void MonitorDefaultContrast(TUSIGN8 *pKeystate);
#if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
void ProcessAutoscroll(TUSIGN8 *pKeystate);
#endif
TUSIGN16 ProcessSwitch(TUSIGN8 keyState);
TUSIGN16 ProcessCommand(MOVE_COMMAND_TYPE moveCommand);
TUSIGN16 processCommand_CheckForSequence(MOVE_COMMAND_TYPE moveCommand);


#ifdef HMI_SUPPORTS_SECURITY_ACCESS
void LogoutInactiveUser(TUSIGN8 keyState);
void ResetInactiveUserTimeout(void);
#endif

#ifdef LOCAL_DATA_CHECK
TUSIGN8 ST_U8_InitializationCheck(TUSIGN8 old_value, TUSIGN16 objIdx, TINT16 attribIdx, TUSIGN8 defaultValue);
TUSIGN8 ST_E8_InitializationCheck(TUSIGN8 old_value, TUSIGN16 objIdx, TINT16 attribIdx, TUSIGN8 defaultValue);
#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
void modifyDisplayForAutocal(void);
#endif

#define  KEY_LOCKOUT_RIGHT                  0x01
#define  KEY_LOCKOUT_LEFT                   0x02
#define  KEY_LOCKOUT_UP                     0x04
#define  KEY_LOCKOUT_DOWN                   0x08
#define  KEY_LOCKOUT_EXIT_CONFIGURATION     0x80
#define  KEY_LOCKOUT_AUTOSCROLL             0x40

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
typedef struct
{
	TUSIGN16	seconds;
	TUSIGN16	milliseconds;
}LOGOUT_TIMER;
#endif

static TUSIGN8  HMI_Instance = 1;

#ifdef HMI_SUPPORTS_SERVICEPORT

typedef struct
{
    // elements that must be initialised
    TBOOL       devicePowerupComplete;
    TINT16      autoscrollTimer;
    TUSIGN8     waitForReply;
    TUSIGN8     forcedReplyCount;
    TBOOL       HMIcanAccessIO;
    TBOOL       TaskInit;
    TBOOL       HardwareInit;
    TBOOL       DataInit;
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
    LOGOUT_TIMER    logoutTimer;
#endif
#ifdef AZ200_SPECIAL_FEATURES_HMI
    TBOOL       prvSchedCalActive;
#endif
    // elements that need not be initialised
    TUSIGN8     defaultContrastCounter;
    TUSIGN8     previousKeyState;
    TUSIGN8     keyRepeatCounter;
    TUSIGN8     displayUpdateCounter;
    TUSIGN8     keyLockout;

#ifdef AZ200_SPECIAL_FEATURES_HMI
    FRAME_FORMAT_TYPE const *   savedFrame_cal;
    TUSIGN8                     savedCursor_cal;
    TBOOL                       savedAutoscroll_cal;
#endif
}COORDINATOR_LOCAL_TYPE;

#pragma diag_suppress=Pm022
#pragma diag_suppress=Pm023

COORDINATOR_LOCAL_TYPE  CoordinatorLocal_1 = {
#ifdef OVERRIDE_POWERUP_CHECK
     eTRUE  // TBOOL devicePowerupComplete;
#else
     eFALSE // TBOOL devicePowerupComplete
#endif
    ,0              // TINT16   autoscrollTimer
    ,0              // TUSIGN8  waitForReply
    ,0              // TUSIGN8  forcedReplyCount
    ,eFALSE         // TBOOL    HMIcanAccessIO
    ,eFALSE         // TBOOL    TaskInit
    ,eFALSE         // TBOOL    HardwareInit
    ,eFALSE         // TBOOL    DataInit
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
    ,0              // TUSIGN16 logoutTimer.seconds
    ,0              // TUSIGN16 logoutTimer.milliseconds
#endif
#ifdef AZ200_SPECIAL_FEATURES_HMI
    ,eFALSE         // TBOOL    prvSchedCalActive;
#endif
    };

COORDINATOR_LOCAL_TYPE  CoordinatorLocal_2 = {
#ifdef OVERRIDE_POWERUP_CHECK
     eTRUE  // TBOOL devicePowerupComplete;
#else
     eFALSE // TBOOL devicePowerupComplete
#endif
    ,0      // TINT16   autoscrollTimer
    ,0      // TUSIGN8  waitForReply
    ,0      // TUSIGN8  forcedReplyCount
    ,eTRUE  // TBOOL    HMIcanAccessIO
    ,eFALSE // TBOOL    TaskInit
    ,eFALSE // TBOOL    HardwareInit
    ,eFALSE // TBOOL    DataInit
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
    ,0      // TUSIGN16 logoutTimer.seconds
    ,0      // TUSIGN16 logoutTimer.milliseconds
#endif
#ifdef AZ200_SPECIAL_FEATURES_HMI
    ,eFALSE // TBOOL    prvSchedCalActive;
#endif
    };

#pragma diag_default=Pm023
#pragma diag_default=Pm022

COORDINATOR_LOCAL_TYPE  *pCoordinatorLocal = &CoordinatorLocal_1;

// No restriction on the optimization of this function
void HmiCoordinatorSetLocalDataPointer(TUSIGN8 newInstance)
{
    if      (newInstance == 1)  {pCoordinatorLocal = &CoordinatorLocal_1;}
    else if (newInstance == 2)  {pCoordinatorLocal = &CoordinatorLocal_2;}
    else                        {VIP_ASSERT(0);}
}

#define LOCAL_devicePowerupComplete     pCoordinatorLocal->devicePowerupComplete
#define LOCAL_autoscrollTimer           pCoordinatorLocal->autoscrollTimer
#define LOCAL_waitForReply              pCoordinatorLocal->waitForReply
#define LOCAL_forcedReplyCount          pCoordinatorLocal->forcedReplyCount
#define LOCAL_HMIcanAccessIO            pCoordinatorLocal->HMIcanAccessIO
#define LOCAL_TaskInit                  pCoordinatorLocal->TaskInit
#define LOCAL_HardwareInit              pCoordinatorLocal->HardwareInit
#define LOCAL_DataInit                  pCoordinatorLocal->DataInit
#define LOCAL_defaultContrastCounter    pCoordinatorLocal->defaultContrastCounter
#define LOCAL_previousKeyState          pCoordinatorLocal->previousKeyState
#define LOCAL_keyRepeatCounter          pCoordinatorLocal->keyRepeatCounter
#define LOCAL_displayUpdateCounter      pCoordinatorLocal->displayUpdateCounter
#define LOCAL_keyLockout                pCoordinatorLocal->keyLockout

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
#define LOCAL_logoutTimer               pCoordinatorLocal->logoutTimer
#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
#define LOCAL_prvSchedCalActive         pCoordinatorLocal->prvSchedCalActive
#define LOCAL_savedFrame_cal            pCoordinatorLocal->savedFrame_cal
#define LOCAL_savedCursor_cal           pCoordinatorLocal->savedCursor_cal
#define LOCAL_savedAutoscroll_cal       pCoordinatorLocal->savedAutoscroll_cal
#endif

#else

#define LOCAL_devicePowerupComplete     devicePowerupComplete
#define LOCAL_autoscrollTimer           autoscrollTimer
#define LOCAL_waitForReply              waitForReply
#define LOCAL_forcedReplyCount          forcedReplyCount
#define LOCAL_HMIcanAccessIO            HMIcanAccessIO
#define LOCAL_TaskInit                  TaskInit
#define LOCAL_HardwareInit              HardwareInit
#define LOCAL_DataInit                  DataInit
#define LOCAL_defaultContrastCounter    defaultContrastCounter
#define LOCAL_previousKeyState          previousKeyState
#define LOCAL_keyRepeatCounter          keyRepeatCounter
#define LOCAL_displayUpdateCounter      displayUpdateCounter
#define LOCAL_keyLockout                keyLockout
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
#define LOCAL_logoutTimer               logoutTimer
#endif
#ifdef AZ200_SPECIAL_FEATURES_HMI
#define LOCAL_prvSchedCalActive         prvSchedCalActive
#define LOCAL_savedFrame_cal            savedFrame_cal
#define LOCAL_savedCursor_cal           savedCursor_cal
#define LOCAL_savedAutoscroll_cal       savedAutoscroll_cal
#endif

#ifdef OVERRIDE_POWERUP_CHECK
TBOOL       devicePowerupComplete = eTRUE;
#else
TBOOL       devicePowerupComplete = eFALSE;
#endif
TINT16      autoscrollTimer = 0;
TUSIGN8     waitForReply = 0;
TUSIGN8     forcedReplyCount = 0;
TBOOL       HMIcanAccessIO = eFALSE;
TBOOL       TaskInit = eFALSE;
TBOOL       HardwareInit = eFALSE;
TBOOL       DataInit = eFALSE;
TUSIGN8     defaultContrastCounter;
TUSIGN8     previousKeyState;
TUSIGN8     keyRepeatCounter;
TUSIGN8     displayUpdateCounter;
TUSIGN8     keyLockout;
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
LOGOUT_TIMER     logoutTimer = {0,0};
#endif
#ifdef AZ200_SPECIAL_FEATURES_HMI
TBOOL                       prvSchedCalActive = eFALSE;
FRAME_FORMAT_TYPE const *   savedFrame_cal;
TUSIGN8                     savedCursor_cal;
TBOOL                       savedAutoscroll_cal;
#endif

#endif

// No restriction on the optimization of this function
void HmiCoordinatorSetAccess(TUSIGN8 instance, TBOOL AllowState)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_LOCAL_INSTANCE)
    {
        CoordinatorLocal_1.HMIcanAccessIO = AllowState;
    }
    else if  (instance == HMI_REMOTE_INSTANCE)
    {
        CoordinatorLocal_2.HMIcanAccessIO = eTRUE;
    }
    else
    {
        VIP_ASSERT(0);
    }
#else
    if (instance == HMI_LOCAL_INSTANCE)
    {
        HMIcanAccessIO = AllowState;
    }
#endif


#if defined(TYPE_1_IMPLEMENTATION) && defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
    if (!AllowState)
    {
        FrameMover_ResetFrameMemory(0);

        #ifdef HMI_SUPPORTS_SECURITY_ACCESS
            LogoutInactiveUser(KEYSTATE_EXIT_CONFIGURATION);
        #endif
    }
#endif
}



// No restriction on the optimization of this function
void HmiCoordinatorPowerOn(TUSIGN8 instance)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_LOCAL_INSTANCE)
    {
        CoordinatorLocal_1.devicePowerupComplete = eTRUE;
    }
    else if  (instance == HMI_REMOTE_INSTANCE)
    {
        CoordinatorLocal_2.devicePowerupComplete = PacketManager_SP_RegisterClientEXE();
    }
    else
    {
        VIP_ASSERT(0);
    }
#else
    if (instance == HMI_LOCAL_INSTANCE)
    {
        devicePowerupComplete = eTRUE;
    }
#endif
}

// No restriction on the optimization of this function
void HmiCoordinatorEXE(TUSIGN8 instance)
{
    TBOOL updateActiveArea;
    TBOOL isSmartHMI;
    
    if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_SHMI_STATUS)
    {
        isSmartHMI = eTRUE;
    }else
    {
        isSmartHMI = eFALSE;
    }
    
#ifdef EDP300_SPECIAL_FEATURES_HMI
    static TUSIGN16     AutoAdjustCompleteTimer = 0;
#endif

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_REMOTE_INSTANCE)
    {
        PacketManagerCopyServicePortData(eTRUE);
    }
#endif

#ifdef EDP300_TEST_ONLY
    if (instance == HMI_LOCAL_INSTANCE)         // EDP300_TEST_ONLY
    {                                           // EDP300_TEST_ONLY
        EDP300Mode_Execute();                   // EDP300_TEST_ONLY
                                                // EDP300_TEST_ONLY
        if (EDP300ModeGetControl())             // EDP300_TEST_ONLY
        {                                       // EDP300_TEST_ONLY
            PacketManagerSetOperatorIcon(7, 2); // EDP300_TEST_ONLY
        }                                       // EDP300_TEST_ONLY
        else                                    // EDP300_TEST_ONLY
        {                                       // EDP300_TEST_ONLY
            PacketManagerSetOperatorIcon(7, 3); // EDP300_TEST_ONLY
        }                                       // EDP300_TEST_ONLY
    }                                           // EDP300_TEST_ONLY
#endif

#ifndef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_LOCAL_INSTANCE)
#endif
    {
        TUSIGN8 keypressProcessed = 0;
        TUSIGN8 keyState = 0;
        TUSIGN8 bytesToSend = 0;

        #ifdef HMI_SUPPORTS_SERVICEPORT
        HMI_Instance = instance;
        #else
        HMI_Instance = HMI_LOCAL_INSTANCE;
        #endif

        #ifdef HMI_SUPPORTS_SERVICEPORT
            HmiCoordinatorSetLocalDataPointer(instance);
            AdjusterSetLocalDataPointer(instance);
            ConfigModeManagerSetLocalDataPointer(instance);
            FrameMoverSetLocalDataPointer(instance);
            FrameTableSetLocalDataPointer(instance);
            PacketManagerSetLocalDataPointer(instance);

            #if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
            TrendViewSetLocalDataPointer(instance);
            #endif
        #else
            #if defined(HMI_SUPPORTS_GRAPH_VIEW) || defined(HMI_SUPPORTS_GRAPH_PARAM_FRAMES)
            TrendViewSetLocalDataPointer(HMI_LOCAL_INSTANCE);
            #endif
        #endif

        if ((LOCAL_TaskInit == eTRUE) && (LOCAL_HardwareInit == eTRUE) && (LOCAL_DataInit == eTRUE))
        {
            #if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION)
            ((const T_TFT_DISPLAY*)GetSubsystemPtr(TFT_IDX))->SetLanguageSRV(SFD_SIMPLE.language);
            #endif

            #ifdef HMI_SUPPORTS_PARAMETER_DUMP
            ParameterDumpExecute();
            #endif

            PacketManagerManageLowLevelPower(LOCAL_devicePowerupComplete, LOCAL_forcedReplyCount, LOCAL_HMIcanAccessIO);

            if (!PacketManagerIsLowLevelActive(instance))
            {
                LOCAL_forcedReplyCount = 0;
                HmiCoordinatorInitializeTasks(instance,eFALSE);
            }
            else
            {
                TBOOL WaitingForReply = PacketManagerProcessWaitForReply(instance, &LOCAL_waitForReply, &LOCAL_forcedReplyCount);

                if (!WaitingForReply)
                {
                    // Get Error reply from PacketManager
                    if (PacketManagerHandleErrors(isSmartHMI))
                    {
//                        #if (HMI_DISPLAY_RATE_CYCLES < 7) && defined(TYPE_1_IMPLEMENTATION)
//                        // Operator page cannot be fully built with less than 7 buffers worth of data
//                        // Therefore update rate must be slowed to allow operator page to be built
//                        if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_OPERATOR)
//                        {
//                            LOCAL_displayUpdateCounter = 7;
//                        }
//                        else
//                        #endif
                        {
                            LOCAL_displayUpdateCounter = HMI_DISPLAY_RATE_CYCLES;
                        }
                    }
                    else
                    {
                        // Get switch data from PacketManager
                        keyState = PacketManagerGetKeystate(instance);

                        #ifdef EDP300_SPECIAL_FEATURES_HMI
                        if (keyState != KEYSTATE_RIGHT)
                        {
                            FrameTableResetAutoAdjustTimer();
                        }

                        #pragma diag_suppress=Pm026
                        if (FrameTableGetAutoAdjustStatus() != 8)
                        {
                            AutoAdjustCompleteTimer = 0;
                        }

                        if ((FrameTableGetAutoAdjustStatus() == 8)
                         && (FrameTableGetCurrentFrame() == FrameTableGetEDP300QuickAdjustFrame()))
                        {
                            AutoAdjustCompleteTimer += (TUSIGN16)(HMI_UPDATE_RATE_MILLISEC);
                        }

                        if ((AutoAdjustCompleteTimer > 3000)
                         && (keyState == KEYSTATE_NONE))
                        {
                            keyState = KEYSTATE_LEFT;
                        }
                        #pragma diag_default=Pm026

                        #endif


                        // Cater for key repeats
                        if (HandleKeyRepeat(keyState))
                        {
                            // Check for default contrast selection
                            MonitorDefaultContrast(&keyState);

                            // Monitor configuration mode
                            ConfigModeManagerMonitorKeyboard(&keyState);

                            #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                            // Monitor Automatic Logout of Inactive User
                            LogoutInactiveUser(keyState);
                            #endif

                            #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
                            // Process Autoscroll
                            ProcessAutoscroll(&keyState);
                            #endif

                            //Decode switch data into a switch command and process the key
                            if (ProcessSwitch(keyState))
                            {
                                keypressProcessed = 1;
                                keyState = KEYSTATE_NONE;
                            }
                        }
                        else
                        {
                            keyState = KEYSTATE_NONE;
                        }
                    }

                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    modifyDisplayForAutocal();
                    #endif

                    LOCAL_displayUpdateCounter ++;

                    TUSIGN8 updateCountLimit = HMI_DISPLAY_RATE_CYCLES;         // Standard update rate is 1 second


                    FRAME_FORMAT_TYPE const *pCurrFrame = FrameTableGetCurrentFrame();

                    #if NVINIT_STYLE > 0
                    if ((pCurrFrame->frameType == FRAME_TYPE_PROGRESS_BAR)
                     && (pCurrFrame->frameDefinition.progressFrame.nVinitFlag != 0))
                    {
                        updateCountLimit = HMI_DISPLAY_RATE_CYCLES / 2;         // update rate of NV INIT frame is 0.5sec
                    }
                    #endif

                    #if (HMI_DISPLAY_RATE_CYCLES < 7) && defined(TYPE_1_IMPLEMENTATION)
                    // Operator page cannot be fully built with less than 7 buffers worth of data
                    // Therefore update rate must be slowed to allow operator page to be built
                    if (pCurrFrame->frameType == FRAME_TYPE_OPERATOR)
                    {
                        updateCountLimit = 7;
                    }
                    #endif
                    if (LOCAL_displayUpdateCounter >= updateCountLimit)
                    {
                        updateActiveArea = eTRUE;
                        LOCAL_displayUpdateCounter = 0;                        
                    }
                    else
                    {
                        updateActiveArea = eFALSE;
                    }
                    if(isSmartHMI)
                    {
#ifdef SUPPORT_SHMI
                        bytesToSend = PacketManagerBuildImage_SHMI(updateActiveArea, keypressProcessed);
#endif
                    }
                    else
                    {
#ifdef SUPPORT_OLD_HMI
                        bytesToSend = PacketManagerBuildImage_HMI(updateActiveArea, keypressProcessed);
#endif
                    }
                        

                    if (bytesToSend)
                    {
                        #ifdef HMI_SUPPORTS_SERVICEPORT
                        if (instance == 2)
                        {
                            //Send data to service port
                            PacketManagerStartTransmission_SP(bytesToSend);
                        }
                        else
                        #endif
                        {
                            #if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
                                PacketManagerSendDataToLowLevel(bytesToSend);
                                PacketManagerExecuteLowLevel();
                                PacketManagerReceiveDataFromLowLevel();
                            #endif

                            #ifdef TYPE_1_IMPLEMENTATION
                                if(!isSmartHMI)
                                {
                                    PacketManagerStartTransmission(bytesToSend);
                                }
                                else
                                {
                                    PacketManagerReadyToTransmission();
                                }
                            #endif
                        }
                    }
                }
                // Clear Frame memory 5 minutes after leaving configuration
                FrameMover_ProcessFrameMemoryTimedReset();
            }
        }
    }

    #ifdef HMI_SUPPORTS_GRAPH_VIEW
    TrendViewExecuteSampling();
    #endif

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_REMOTE_INSTANCE)
    {
        PacketManagerCopyServicePortData(eFALSE);
    }
#endif



    HMI_Instance = 0;
}

// No restriction on the optimization of this function
void HmiCoordinatorForceDisplayTimeoutSRV(TUSIGN8 instance)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_LOCAL_INSTANCE)
    {
        CoordinatorLocal_1.displayUpdateCounter = HMI_DISPLAY_RATE_CYCLES;
    }
    else if  (instance == HMI_REMOTE_INSTANCE)
    {
        CoordinatorLocal_2.displayUpdateCounter = HMI_DISPLAY_RATE_CYCLES;
    }
    else
    {
        VIP_ASSERT(0);
    }
#else
    if (instance == HMI_LOCAL_INSTANCE)
    {
        displayUpdateCounter = HMI_DISPLAY_RATE_CYCLES;
    }
#endif

}

// No restriction on the optimization of this function
void HmiCoordinatorAllowReInit(TUSIGN8 InitFlags)
{
    if( (InitFlags & INIT_HARDWARE) != 0)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        CoordinatorLocal_1.HardwareInit = eFALSE;
        CoordinatorLocal_2.HardwareInit = eFALSE;
        #else
        HardwareInit = eFALSE;
        #endif
    }

    if( (InitFlags & INIT_TASKS) != 0)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        CoordinatorLocal_1.TaskInit = eFALSE;
        CoordinatorLocal_2.TaskInit = eFALSE;
        #else
        TaskInit = eFALSE;
        #endif
    }

    if( (InitFlags & INIT_DATA) != 0)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        CoordinatorLocal_1.DataInit = eFALSE;
        CoordinatorLocal_2.DataInit = eFALSE;
        #else
        DataInit = eFALSE;
        #endif
    }

    if( (InitFlags & INIT_CALCULATION) != 0)
    {
        asm("nop");
    }
}

// No restriction on the optimization of this function
void HmiCoordinatorInitializeTasks(TUSIGN8 instance, TBOOL MainInit)
{

#ifdef HMI_SUPPORTS_SERVICEPORT
        HmiCoordinatorSetLocalDataPointer(instance);
        FrameTableSetLocalDataPointer(instance);
        FrameMoverSetLocalDataPointer(instance);
#endif

    if ((LOCAL_TaskInit == eFALSE) || (MainInit == eFALSE))
    {

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if ((instance == 2) && (MainInit))
        {
            /* Create the HMI_REMOTE_INSTANCE semaphore */
            CreateRecursive_RTOS_MUTEX(Sema_HMI_REMOTE_INSTANCE);
        }
        #endif

        FrameTableInitialise(MainInit);
        FrameMover_ResetFrameMemory(0);

        LOCAL_keyLockout = 0;

        LOCAL_defaultContrastCounter = 0;

        // initialise HandleKeyRepeat()
        LOCAL_previousKeyState = KEYSTATE_NONE;
        LOCAL_keyRepeatCounter = 0;

        // initialise display update control
        LOCAL_displayUpdateCounter = HMI_DISPLAY_RATE_CYCLES;

        if (LOCAL_TaskInit == eFALSE)
        {
            LOCAL_TaskInit = eTRUE;
        }
    }
}

// No restriction on the optimization of this function
void HmiCoordinatorInitializeHardware(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    CoordinatorLocal_2.HardwareInit = eTRUE;
#endif

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (CoordinatorLocal_1.HardwareInit != eTRUE)
#else
    if (HardwareInit != eTRUE)
#endif
    {
        #if defined(TYPE_1_IMPLEMENTATION) && defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
            LOCAL_HardwareInit = eTRUE;
        #else
            Layer2_initialiseIO();
            LOCAL_HardwareInit = eTRUE;
        #endif
    }
}

// No restriction on the optimization of this function
void HmiCoordinatorInitializeData(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    if ((CoordinatorLocal_1.DataInit != eTRUE) || (CoordinatorLocal_2.DataInit != eTRUE))
#else
    if (DataInit != eTRUE)
#endif
    {
    #ifdef AZ200_SPECIAL_FEATURES_HMI

        if (((const T_NVINTERFACE*)GetSubsystemPtr(NVINTERFACE_IDX))->GetDirtyFlag_SRV(HMI_IDX) != 0)
        {
            // Dirty Flag is set (NV Data not reliable) - Re-initialise All Data using default values from ROM
            (void)LoadRomDefaults_T_UNIT(HMI_ME, ALL_DATACLASSES);
        }
        else
        {
            // Dirty Flag not set (NV Data is reliable) - Re-initialise Only dynamic data using default values from ROM
            (void)LoadRomDefaults_T_UNIT(HMI_ME,HMI_DC_IDX_DYNAMIC);
        }
    #else

        #ifdef EDP300_SPECIAL_FEATURES_HMI
            #ifdef SFCD
                #ifdef SFD_SPLIT
                    T_HMI_STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE* pData = (T_HMI_STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE*)&hmi_sTATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE_data;
                #else
                    T_HMI_STATIC_FREQUENT_CONSTANT_DEFAULT* pData = (T_HMI_STATIC_FREQUENT_CONSTANT_DEFAULT*)&hmi_static_frequent_constant_default_data;
                #endif
            #else
                #ifdef SFD_SPLIT
                    T_HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE* pData = (T_HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE*)&hmi_sTATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_data;
                #else
                    T_HMI_STATIC_FREQUENT_STATIC_DEFAULT* pData = (T_HMI_STATIC_FREQUENT_STATIC_DEFAULT*)&hmi_static_frequent_static_default_data;
                #endif
            #endif
            pData->contrast = 50;
        #endif

        #ifdef LOCAL_DATA_CHECK
            // Check NV Values
            ST_U8_InitializationCheck(SFD_SIMPLE.contrast,                         HMI_IDX_contrast,                    0, 50);

            ST_E8_InitializationCheck(SFD_SIMPLE.localOperationEnable,             HMI_IDX_localOperationEnable,        0, DEFAULT_LOCAL_OP_ENABLE);
            ST_E8_InitializationCheck(SFD_SIMPLE.language,                         HMI_IDX_language,                    0, DEFAULT_LANGUAGE);

            ST_E8_InitializationCheck(SFD_SIMPLE.displayMode_1,                    HMI_IDX_displayMode_1,               0, PAGE_1_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_SIMPLE.mainOperatorView_1_1,             HMI_IDX_mainOperatorView_1_1,        0, PAGE_1_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_SIMPLE.mainOperatorView_1_2,             HMI_IDX_mainOperatorView_1_2,        0, PAGE_1_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_SIMPLE.mainOperatorView_1_3,             HMI_IDX_mainOperatorView_1_3,        0, PAGE_1_DEFAULT_LINE_3);
            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            ST_E8_InitializationCheck(SFD_SIMPLE_CTW.mainOperatorView_1_4,         HMI_IDX_mainOperatorView_1_4,        0, PAGE_1_DEFAULT_LINE_4);
            #endif
            ST_E8_InitializationCheck(SFD_SIMPLE.mainOperatorView_1_bargraph,      HMI_IDX_mainOperatorView_1_bargraph, 0, PAGE_1_DEFAULT_LINE_B);


            #ifdef SECOND_OPERATOR_PAGE
            ST_E8_InitializationCheck(SFD_MULTI_OP.autoscrollEnable,               HMI_IDX_autoscrollEnable,            0, DEFAULT_AUTOSCROLL_ENABLE);

            ST_E8_InitializationCheck(SFD_MULTI_OP.displayMode_2,                  HMI_IDX_displayMode_2,               0, PAGE_2_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_2_1,           HMI_IDX_mainOperatorView_2_1,        0, PAGE_2_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_2_2,           HMI_IDX_mainOperatorView_2_2,        0, PAGE_2_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_2_3,           HMI_IDX_mainOperatorView_2_3,        0, PAGE_2_DEFAULT_LINE_3);
            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            ST_E8_InitializationCheck(SFD_MULTI_OP_CTW.mainOperatorView_2_4,       HMI_IDX_mainOperatorView_2_4,        0, PAGE_2_DEFAULT_LINE_4);
            #endif
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_2_bargraph,    HMI_IDX_mainOperatorView_2_bargraph, 0, PAGE_2_DEFAULT_LINE_B);
            #endif  // end of #ifdef SECOND_OPERATOR_PAGE


            #ifdef THIRD_OPERATOR_PAGE
            ST_E8_InitializationCheck(SFD_MULTI_OP.displayMode_3,                  HMI_IDX_displayMode_3,               0, PAGE_3_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_3_1,           HMI_IDX_mainOperatorView_3_1,        0, PAGE_3_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_3_2,           HMI_IDX_mainOperatorView_3_2,        0, PAGE_3_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_3_3,           HMI_IDX_mainOperatorView_3_3,        0, PAGE_3_DEFAULT_LINE_3);
            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            ST_E8_InitializationCheck(SFD_MULTI_OP_CTW.mainOperatorView_3_4,       HMI_IDX_mainOperatorView_3_4,        0, PAGE_3_DEFAULT_LINE_4);
            #endif
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_3_bargraph,    HMI_IDX_mainOperatorView_3_bargraph, 0, PAGE_3_DEFAULT_LINE_B);
            #endif  // end of #ifdef THIRD_OPERATOR_PAGE

            #ifdef FOURTH_OPERATOR_PAGE
            ST_E8_InitializationCheck(SFD_MULTI_OP.displayMode_4,                  HMI_IDX_displayMode_4,               0, PAGE_4_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_4_1,           HMI_IDX_mainOperatorView_4_1,        0, PAGE_4_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_4_2,           HMI_IDX_mainOperatorView_4_2,        0, PAGE_4_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_4_3,           HMI_IDX_mainOperatorView_4_3,        0, PAGE_4_DEFAULT_LINE_3);
            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            ST_E8_InitializationCheck(SFD_MULTI_OP_CTW.mainOperatorView_4_4,       HMI_IDX_mainOperatorView_4_4,        0, PAGE_4_DEFAULT_LINE_4);
            #endif
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_4_bargraph,    HMI_IDX_mainOperatorView_4_bargraph, 0, PAGE_4_DEFAULT_LINE_B);
            #endif  // end of #ifdef FOURTH_OPERATOR_PAGE

            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            #ifdef FIFTH_OPERATOR_PAGE
            ST_E8_InitializationCheck(SFD_MULTI_OP.displayMode_5,                  HMI_IDX_displayMode_5,               0, PAGE_5_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_5_1,           HMI_IDX_mainOperatorView_5_1,        0, PAGE_5_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_5_2,           HMI_IDX_mainOperatorView_5_2,        0, PAGE_5_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_5_3,           HMI_IDX_mainOperatorView_5_3,        0, PAGE_5_DEFAULT_LINE_3);
            ST_E8_InitializationCheck(SFD_MULTI_OP_CTW.mainOperatorView_5_4,       HMI_IDX_mainOperatorView_5_4,        0, PAGE_5_DEFAULT_LINE_4);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_5_bargraph,    HMI_IDX_mainOperatorView_5_bargraph, 0, PAGE_5_DEFAULT_LINE_B);
            #endif  // end of #ifdef FIFTH_OPERATOR_PAGE

            #ifdef SIXTH_OPERATOR_PAGE
            ST_E8_InitializationCheck(SFD_MULTI_OP.displayMode_6,                  HMI_IDX_displayMode_6,               0, PAGE_6_DEFAULT_FORMAT);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_6_1,           HMI_IDX_mainOperatorView_6_1,        0, PAGE_6_DEFAULT_LINE_1);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_6_2,           HMI_IDX_mainOperatorView_6_2,        0, PAGE_6_DEFAULT_LINE_2);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_6_3,           HMI_IDX_mainOperatorView_6_3,        0, PAGE_6_DEFAULT_LINE_3);
            ST_E8_InitializationCheck(SFD_MULTI_OP_CTW.mainOperatorView_6_4,       HMI_IDX_mainOperatorView_6_4,        0, PAGE_6_DEFAULT_LINE_4);
            ST_E8_InitializationCheck(SFD_MULTI_OP.mainOperatorView_6_bargraph,    HMI_IDX_mainOperatorView_6_bargraph, 0, PAGE_6_DEFAULT_LINE_B);
            #endif  // end of #ifdef SIXTH_OPERATOR_PAGE

            ST_E8_InitializationCheck(SFD_SIMPLE_CTW.Rotation,                     HMI_IDX_Rotation,                    0, HMI_ROTATION_0);
            ST_E8_InitializationCheck(SFD_SIMPLE_CTW.MenuFormat,                   HMI_IDX_MenuFormat,                  0, ITEMS_PER_MENU);
            #endif  // end of #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #endif  // end of #ifdef LOCAL_DATA_CHECK
    #endif


    #ifdef DEFAULT_LANGUAGE_STARTUP
        TUSIGN8 TempLang = DEFAULT_LANGUAGE;
        HMI_ME->Put(HMI_ME,HMI_IDX_language,0,(void FAST *)&TempLang);
    #endif

    #ifdef HMI_SUPPORTS_SERVICEPORT
        CoordinatorLocal_1.DataInit = eTRUE;
        CoordinatorLocal_2.DataInit = eTRUE;
    #else
        DataInit = eTRUE;
    #endif
    }
}

#ifndef AZ200_SPECIAL_FEATURES_HMI

#ifdef LOCAL_DATA_CHECK
// No restriction on the optimization of this function
TUSIGN8 ST_E8_InitializationCheck(TUSIGN8 old_value, TUSIGN16 objIdx, TINT16 attribIdx, TUSIGN8 defaultValue)
{
    TUSIGN8 valueNotValid = 1;
    T_UNIT const SLOW *pSub;   // Pointer to Subsystem
    T_DO_ATTRIB_RANGE Range;    // Pointer to range information

    TUSIGN8 i, ST_E8_value;

    pSub = GetSubsystemPtr(HMI_IDX);  // Get the pointer

    if (pSub->GetAttributeDescription(pSub,objIdx,(TUSIGN16)attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range) == OK)
    {
        ST_E8_value = old_value;
        i = 0;
        while ((valueNotValid) && (i < Range.range.e8.numberOfCodes))
        {
            if (Range.range.e8.ptrCodeTab[i] == ST_E8_value)
            {
                valueNotValid = 0;
            }
            i ++;
        }
    }
    if (valueNotValid)
    {
        ST_E8_value = defaultValue;
        // return value of "Put()" ignored
        //lint -e{534} Roger Arnold 2006-5-16
        pSub->Put(pSub,objIdx,attribIdx,(void FAST *)&ST_E8_value);
    }
    return valueNotValid;
}
#endif

#ifdef LOCAL_DATA_CHECK
// No restriction on the optimization of this function
TUSIGN8 ST_U8_InitializationCheck(TUSIGN8 old_value, TUSIGN16 objIdx, TINT16 attribIdx, TUSIGN8 defaultValue)
{
    TUSIGN8 valueNotValid = 1;
    T_UNIT const SLOW *pSub;   // Pointer to Subsystem
    T_DO_ATTRIB_RANGE Range;    // Pointer to range information

    TUSIGN8 ST_U8_value;

    pSub = GetSubsystemPtr(HMI_IDX);  // Get the pointer

    if (pSub->GetAttributeDescription(pSub,objIdx,(TUSIGN16)attribIdx,(T_DO_ATTRIB_RANGE FAST*)&Range) == OK)
    {
        ST_U8_value = old_value;
        if ((ST_U8_value <= Range.range.u8.max) && (ST_U8_value >= Range.range.u8.min))
        {
            valueNotValid = 0;
        }
    }
    if (valueNotValid)
    {
        ST_U8_value = defaultValue;
        // return value of "Put()" ignored
        //lint -e{534} Roger Arnold 2006-5-16
        pSub->Put(pSub,objIdx,attribIdx,(void FAST *)&ST_U8_value);
    }
    return valueNotValid;
}
#endif
#endif

// No restriction on the optimization of this function
TUSIGN8 HandleKeyRepeat(TUSIGN8 keyState)
{
    TUSIGN8 returnVal = 0;

    #if defined(ENUM_ACCELERATOR) || defined(STRING_ACCELERATOR) || defined(IPADDR_ACCELERATOR) || defined(MANUAL_ADJUST_ACCELERATOR) || defined(DATE_TIME_ACCELERATOR)
    if ((keyState != KEYSTATE_UP) && (keyState != KEYSTATE_DOWN))
    {
        #ifdef ENUM_ACCELERATOR
        FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
        #endif

        #ifdef STRING_ACCELERATOR
        FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
        #endif

        #ifdef IPADDR_ACCELERATOR
        FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
        #endif

        #ifdef MANUAL_ADJUST_ACCELERATOR
        FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
        #endif

        #ifdef DATE_TIME_ACCELERATOR
        FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
        #endif
    }
    #endif


    // if no key pressed or new key is different to the previous key or key repeat timeout expired
    // then reset counter/timer, update the previous key state and return true (so that new key will be processed)
    LOCAL_keyRepeatCounter ++;

    if ((keyState == KEYSTATE_NONE)
     || (keyState == KEYSTATE_DEFAULT_CONTRAST)
     || (keyState != LOCAL_previousKeyState)
     || (LOCAL_keyRepeatCounter >= HMI_KEY_REPEAT_CYCLES))
    {
        LOCAL_keyRepeatCounter = 0;
        LOCAL_previousKeyState = keyState;
        returnVal = 1;
    }
    return returnVal;
}

#if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
// No restriction on the optimization of this function
void ProcessAutoscroll(TUSIGN8 *pKeystate)
{
    if (FrameTableIsAutoscrollEnabled() == eFALSE)
    {
        LOCAL_autoscrollTimer = 0;
    }
    else
    {
        if (++LOCAL_autoscrollTimer >= HMI_AUTOSCROLL_COUNT_LIMIT)
        {
            if (*pKeystate == KEYSTATE_NONE)
            {
                *pKeystate = KEYSTATE_AUTOSCROLL;
                LOCAL_autoscrollTimer = 0;
            }
        }
    }
}
#endif


// No restriction on the optimization of this function
void MonitorDefaultContrast(TUSIGN8 *pKeystate)
{
    if (*pKeystate == KEYSTATE_DEFAULT_CONTRAST)
    {
        if (++LOCAL_defaultContrastCounter >= HMI_DEFAULT_CONTRAST_COUNT_LIMIT)
        {
            ADJUST_DATA    contrastData;
            contrastData.data_F = (TFLOAT)PacketManagerGetDefaultContrast();

            if (AdjusterWriteValueAndCheckLimits(HMI_IDX, HMI_IDX_contrast, 0, &contrastData, ST_U8) == OK)
            {
                LOCAL_defaultContrastCounter = 0;
            }
        }
        *pKeystate = KEYSTATE_NONE;
    }
    else
    {
        LOCAL_defaultContrastCounter = 0;
    }
}

// No restriction on the optimization of this function
TUSIGN16 ProcessSwitch(TUSIGN8 keyState)
{
  TUSIGN16 keyProcessed = 0;

#ifndef DISABLE_QUICK_EXIT
  static TUSIGN16 leftkeyHeld[2];
#endif

#ifdef LOW_LEVEL_HMI_TEST
  FRAME_FORMAT_TYPE const * LLTestFrame =  FrameTableGetLowLevelHMITestFrame();
  if (FrameTableGetCurrentFrame() == LLTestFrame)
  {
    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
  }
  else
#endif
  {
#if NVINIT_STYLE > 0
    TUSIGN8 currentFrameType = FrameTableGetCurrentFrame()->frameType;
    TUSIGN8 currentFramenvInitFlag = FrameTableGetCurrentFrame()->frameDefinition.progressFrame.nVinitFlag;
    if ((currentFrameType == FRAME_TYPE_PROGRESS_BAR) && (currentFramenvInitFlag != 0))
    {
        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
    }
    else
#endif
    {
        ATTRIB_IDENTIFIER_TYPE dataID;

        #ifdef LEFT_RIGHT_LOCKOUT
        FrameTableUpdateLeftRightLockout(keyState);
        #endif

        if (keyState != KEYSTATE_LEFT)                  {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_LEFT;}
        if (keyState != KEYSTATE_RIGHT)                 {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_RIGHT;}
        if (keyState != KEYSTATE_UP)                    {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_UP;}
        if (keyState != KEYSTATE_DOWN)                  {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_DOWN;}
        if (keyState != KEYSTATE_EXIT_CONFIGURATION)    {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_EXIT_CONFIGURATION;}
        if (keyState != KEYSTATE_AUTOSCROLL)            {LOCAL_keyLockout = LOCAL_keyLockout & ~KEY_LOCKOUT_AUTOSCROLL;}

        switch (keyState)
        {

        case KEYSTATE_EXIT_CONFIGURATION:
            if ((LOCAL_keyLockout & KEY_LOCKOUT_EXIT_CONFIGURATION) == 0)
            {
                keyProcessed = ProcessCommand(COMMAND_EXIT);
                if (keyProcessed)
                {
                    FrameMover_ResetFrameMemory(0);
                    LOCAL_keyLockout |= KEY_LOCKOUT_EXIT_CONFIGURATION;
                }
            }
            break;

        case KEYSTATE_RIGHT:
            if ((LOCAL_keyLockout & KEY_LOCKOUT_RIGHT) == 0)
            {
                #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                FRAME_FORMAT_TYPE const *pCurrentFrame_secAcc;
                #endif

                switch (FrameTableGetCurrentFrame()->frameType)
                {
                case FRAME_TYPE_OPERATOR:
                    #ifdef LEFT_RIGHT_LOCKOUT
                    if (FrameTableGetLeftRightLockout())
                    {
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    }
                    else
                    #endif
                    {
                        keyProcessed = ProcessCommand(COMMAND_MENU);
                    }
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_CONFIG_HEADER:
                    keyProcessed = ProcessCommand(COMMAND_SELECT_CONFIG_LEVEL);
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_MENU:
                    #ifdef EDP300_SPECIAL_FEATURES_HMI
                    if  (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 7)
                    {
                        if (FrameTableIsAccessToModePermitted())
                        {
                            switch (FrameTableGetCursorPosition())
                            {
                            case 0: /* AutoAdjust */
                                if (FrameTableIsAutoAdjustStartAllowed() == eTRUE)
                                {
                                    if (FrameTableUpdateAutoAdjustTimer())
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_LAUNCH_QUICK_ADJUST_FRAME);
                                        if (keyProcessed)
                                        {
                                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                        }
                                    }
                                    else
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                    }
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                }
                                break;

                            case 1: /* Adaption */
                                FrameTableSetAdaptionCheck();
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case 2: /* Control  */
                                FrameTableSetControlCheck();
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case 3: /* Manual SP */
                                FrameTableSetManualSPCheck();
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case 4: /* Manual Sensor */
                                FrameTableSetManualSensorCheck();
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case 5: /* Configuration */
                            default:
                                keyProcessed = ProcessCommand(COMMAND_MENU);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;
                            }
                        }
                        else
                        {
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                            if (keyProcessed)
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            }
                        }
                    }
                    else if (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 8)
                    {
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                        }
                    }
                    else
                    #endif
                    {
                        keyProcessed = ProcessCommand(COMMAND_SELECT_FROM_MENU);
                    }

                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_NUMERICAL_PARAM:
                case FRAME_TYPE_STRING_PARAM:
                case FRAME_TYPE_STRING_PARAM_PACKED:

                #ifdef HMI_SUPPORTS_INTEGER_FRAMES
                case FRAME_TYPE_INTEGER_PARAM:
                #endif

                #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
                case FRAME_TYPE_TIME_AND_DATE_PARAM:
                #endif

                #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
                case FRAME_TYPE_BIT_ENUMERATED_PARAM:
                #endif

                #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
                case FRAME_TYPE_IP_ADDRESS:
                #endif

                    switch (FrameTableGetCurrentFrame()->frameType)
                    {
                    case FRAME_TYPE_BIT_ENUMERATED_PARAM:   dataID = FrameTableGetCurrentFrame()->frameDefinition.bitEnumParamFrame.value;      break;
                    case FRAME_TYPE_IP_ADDRESS:             dataID = FrameTableGetCurrentFrame()->frameDefinition.timeDateFrame.value;          break;
                    case FRAME_TYPE_TIME_AND_DATE_PARAM:    dataID = FrameTableGetCurrentFrame()->frameDefinition.timeDateFrame.value;          break;
                    case FRAME_TYPE_NUMERICAL_PARAM:        dataID = FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.value;    break;
                    case FRAME_TYPE_INTEGER_PARAM:          dataID = FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.value;    break;
                    default:                                dataID = FrameTableGetCurrentFrame()->frameDefinition.stringParamFrame.stringText;  break;
                    }

                    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                    pCurrentFrame_secAcc = FrameTableGetCurrentFrame();
                    if ((pCurrentFrame_secAcc->frameType == FRAME_TYPE_STRING_PARAM)
                     && (pCurrentFrame_secAcc->frameDefinition.stringParamFrame.specialStringFlag != 0))
                    {
                        keyProcessed = ProcessCommand(COMMAND_CHECK_SECURITY_PASSWORD);
                    }
                    else
                    #endif
                    {
                        if (FrameTableIsEditEnabled())
                        {
                            if (WriteProtect_HMICanAccess(dataID,0))
                            {
                                keyProcessed = ProcessCommand(COMMAND_OK_CANCEL);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_CANCEL);
                            }
                        }
                        else
                        {
                            if (WriteProtect_HMICanAccess(dataID, 0))
                            {
                                if (PacketManagerIsNumericValueOverrange())
                                {
                                    FRAME_FORMAT_TYPE const *pNumFrame = FrameTableGetCurrentFrame();
                                    if (pNumFrame->frameType == FRAME_TYPE_NUMERICAL_PARAM)
                                    {
                                        TUSIGN16 ss = pNumFrame->frameDefinition.numericalParamFrame.value.ssIdx;
                                        TUSIGN16 ob = pNumFrame->frameDefinition.numericalParamFrame.value.objIdx;
                                        TUSIGN16 at = pNumFrame->frameDefinition.numericalParamFrame.value.attribIdx;

                                        ADJUST_DATA data_v,data_h,data_l,data_i;
                                        TUSIGN8 data_t;
                                        TBOOL data_rulesAreActive;

                                        (void)AdjusterGetFloatValueAndLimits(ss,ob,(TUSIGN8)at,&data_v,&data_h,&data_l,&data_i, &data_rulesAreActive, &data_t);
                                        (void)AdjusterWriteValueAndCheckLimits(ss, ob, (TUSIGN8)at, &data_l, data_t);

                                        #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
                                        FrameTableIncrementConfigChangeCounter();
                                        #endif
                                    }
                                    #ifdef HMI_SUPPORTS_INTEGER_FRAMES
                                    else if (pNumFrame->frameType == FRAME_TYPE_INTEGER_PARAM)
                                    {
                                        TUSIGN16 ss = pNumFrame->frameDefinition.numericalParamFrame.value.ssIdx;
                                        TUSIGN16 ob = pNumFrame->frameDefinition.numericalParamFrame.value.objIdx;
                                        TUSIGN16 at = pNumFrame->frameDefinition.numericalParamFrame.value.attribIdx;

                                        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)ss);
                                        T_DO_ATTRIB_RANGE Range;
                                        (void)pSub->GetAttributeDescription(pSub,ob,at,(T_DO_ATTRIB_RANGE FAST*)&Range);

                                        TUSIGN32 lowValue = Range.range.u32.min;
                                        (void)pSub->Put(pSub,ob,(TINT16)at,(void FAST *)&lowValue);

                                        #ifdef HMI_SUPPORTS_CONFIG_CHANGE_COUNTER
                                        FrameTableIncrementConfigChangeCounter();
                                        #endif
                                    }
                                    #endif
                                    else
                                    {
                                        asm("nop");
                                    }
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_EDIT);
                                }
                            }
                        }
                    }
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                case FRAME_TYPE_ENUMERATED_PARAM:
                    dataID = FrameTableGetCurrentFrame()->frameDefinition.enumParamFrame.value;
                    if (WriteProtect_HMICanAccess(dataID, 0))
                    {
                        if (FrameTableIsEditEnabled())
                        {
                            keyProcessed = ProcessCommand(COMMAND_OK);
                        }
                        else
                        {
                            keyProcessed = ProcessCommand(COMMAND_EDIT);
                        }
                    }

                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_MANUAL_ADJUST:
                    dataID.ssIdx     = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.value_ssIdx;
                    dataID.objIdx    = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_objidx;
                    dataID.attribIdx = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_attribidx;

                    #ifdef AZ200_SPECIAL_FEATURES_HMI

                    if ((dataID.ssIdx == CAL_IDX)
                     && (dataID.objIdx == CAL_IDX_PerformanceLogEntryIndex)
                     && (dataID.attribIdx == 0))
                    {
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    }
                    else
                    #endif
                    {

                        #ifdef HMI_SUPPORTS_MANUAL_ADJUST_MODIFIER
                        if (FrameTableGetManualAdjustModifier())
                        {
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        }
                        else
                        #endif
                        {
                            if (WriteProtect_HMICanAccess(dataID, 0))
                            {
                                #ifdef HMI_SUPPORTS_SEQUENCES
                                TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                                TUSIGN16 SeqMenuTableOffset;

                                if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                                {
                                    if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK_AND_BACK);
                                    }
                                    else
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK_AND_NEXT_SEQUENCE);
                                    }
                                }
                                else
                                #endif
                                {
                                    #ifdef HMI_SUPPORTS_EASYSETUP
                                    switch (FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.easySetupFlag)
                                    {
                                    case 2:     keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK_AND_BACK);             break;
                                    case 1:     keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK_AND_NEXT_EASYSETUP);   break;
                                    default:    keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK);                      break;
                                    }
                                    #else
                                    keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_OK);
                                    #endif
                                }
                            }
                        }
                    }

                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;

                case FRAME_TYPE_AUTO_ADJUST:
                case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
                    dataID.ssIdx     = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.value_ssIdx;
                    dataID.objIdx    = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_objidx;
                    dataID.attribIdx = (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_attribidx;

                    #ifdef HMI_SUPPORTS_MANUAL_VALVE_CAL
                    if (FrameTableIsManualValveCalActive())
                    {
                        // Get Current cal status
                        TUSIGN8 CalibrationStatus = 0;
                        TFLOAT tempStatus;
                        if (AdjusterGetAttribute((TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.value_ssIdx,
                                                 (TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.dispvalue_objidx,
                                                 (TUSIGN8) FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.dispvalue_attribidx,
                                                 (void FAST *)&tempStatus) == (TUSIGN16)OK)
                        {
                            CalibrationStatus = (TUSIGN8)tempStatus;
                        }
                        else
                        {
                            CalibrationStatus = HMI_VALVE_CAL_STATUS_INACTIVE;
                        }

                        // Do Auto Adjust only if status is currently inactive
                        if (CalibrationStatus == HMI_VALVE_CAL_STATUS_INACTIVE)
                        {
                            keyProcessed = ProcessCommand(COMMAND_DO_AUTO_ADJUST);
                            if (keyProcessed)
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            }
                        }
                    }
                    else
                    #endif
                    {
                        #ifdef EDP300_SPECIAL_FEATURES_HMI
                        if (FrameTableIfAutoAdjMarker(dataID.ssIdx, dataID.objIdx, dataID.attribIdx))
                        {
                            if (FrameTableIsAutoAdjustStartAllowed() == eTRUE)
                            {
                                if (FrameTableGetAutoAdjustStatus() == 0)
                                {
                                    // Auto Adjustment inactive
                                    FrameTableStartAdjust(FrameTableIsActuatorRotary());   // Start AutoAdjust(Rotary / Linear)
                                    keyProcessed = 1;
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                }
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                            }

                            if (keyProcessed)
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            }
                        }
                        else
                        #endif
                        {
                            if (WriteProtect_HMICanAccess(dataID, 0))
                            {
                                keyProcessed = ProcessCommand(COMMAND_DO_AUTO_ADJUST);
                                if (keyProcessed)
                                {
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                            }
                        }
                    }
                    break;

                #if defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)
                case FRAME_TYPE_2_POINT_pH_CALIBRATION:
                    {
                        TWO_POINT_pH_CALIBRATION_FRAME_TYPE const *pFrameData = &(FrameTableGetCurrentFrame()->frameDefinition.pHCalFrame);

                        dataID.ssIdx = pFrameData->ssidx;
                        dataID.objIdx = pFrameData->objidx_status;
                        dataID.attribIdx = 0;

                        TUSIGN8 calibrationStatus;

                        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pFrameData->ssidx);
                        (void)pSub->Get(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);

                        switch (calibrationStatus)
                        {
                        case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                            #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                            FrameTableActivateOutputHold();
                            #endif
                            calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_TEMP;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                            #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                            FrameTableReleaseOutputHold();
                            #endif
                            calibrationStatus = HMI_CAL_STATUS_ACCEPT;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                            #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                            FrameTableReleaseOutputHold();
                            #endif
                            calibrationStatus = HMI_CAL_STATUS_REJECT;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_VIEW_BUFFER_TEMP:
                        case HMI_CAL_STATUS_VIEW_BUFFER_1:
                        case HMI_CAL_STATUS_VIEW_BUFFER_2:
                            switch (calibrationStatus)
                            {
                            case HMI_CAL_STATUS_VIEW_BUFFER_1:      dataID.objIdx = pFrameData->objidx_buffer_1;    calibrationStatus = HMI_CAL_STATUS_EDIT_BUFFER_1;       break;
                            case HMI_CAL_STATUS_VIEW_BUFFER_2:      dataID.objIdx = pFrameData->objidx_buffer_2;    calibrationStatus = HMI_CAL_STATUS_EDIT_BUFFER_2;       break;
                            case HMI_CAL_STATUS_VIEW_BUFFER_TEMP:   dataID.objIdx = pFrameData->objidx_temperature; calibrationStatus = HMI_CAL_STATUS_EDIT_BUFFER_TEMP;    break;
                            default:                                VIP_ASSERT(0);                                                                                              break;
                            }

                            if (WriteProtect_HMICanAccess(dataID, 0))
                            {
                                if (PacketManagerIsNumericValueOverrange())
                                {
                                    ADJUST_DATA data_v,data_h,data_l,data_i;
                                    TUSIGN8 data_t;
                                    TBOOL data_rulesAreActive;

                                    (void)AdjusterGetFloatValueAndLimits((TUSIGN16)dataID.ssIdx,(TUSIGN16)dataID.objIdx,(TUSIGN8)dataID.attribIdx,&data_v,&data_h,&data_l,&data_i, &data_rulesAreActive, &data_t);
                                    (void)AdjusterWriteValueAndCheckLimits((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TUSIGN8)dataID.attribIdx, &data_l, data_t);
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_EDIT);
                                    if (keyProcessed)
                                    {
                                        (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                    }
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                            }
                            break;

                        case HMI_CAL_STATUS_EDIT_BUFFER_TEMP:
                        case HMI_CAL_STATUS_EDIT_BUFFER_1:
                        case HMI_CAL_STATUS_EDIT_BUFFER_2:
                            switch (calibrationStatus)
                            {
                            case HMI_CAL_STATUS_EDIT_BUFFER_1:      dataID.objIdx = pFrameData->objidx_buffer_1;    calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_1;    break;
                            case HMI_CAL_STATUS_EDIT_BUFFER_2:      dataID.objIdx = pFrameData->objidx_buffer_2;    calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_2;    break;
                            case HMI_CAL_STATUS_EDIT_BUFFER_TEMP:   dataID.objIdx = pFrameData->objidx_temperature; calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_TEMP; break;
                            default:                                VIP_ASSERT(0);                                                                                           break;
                            }

                            if (WriteProtect_HMICanAccess(dataID, 0))
                            {
                                keyProcessed = ProcessCommand(COMMAND_OK_CANCEL_2PT_PH);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_CANCEL);
                            }

                            if ((FrameTableIsEditEnabled() == 0) && (keyProcessed))
                            {
                                // The keypress has been processed & editing is no longer enabled (new value saved OK)
                                // change calibration status
                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            }
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_1:
                            calibrationStatus = HMI_CAL_STATUS_CALCULATING_1;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_2:
                            calibrationStatus = HMI_CAL_STATUS_CALCULATING_2;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            break;

                        case HMI_CAL_STATUS_SUCCESS:
                            {
                                MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;
                                calibrationStatus = HMI_CAL_STATUS_ACCEPT;

                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                if (FrameTable_IsOutputHeld() == eTRUE)
                                {
                                    calibrationStatus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT;
                                    moveCommand = COMMAND_DO_NOT_MOVE;
                                }
                                #endif

                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                keyProcessed = processCommand_CheckForSequence(moveCommand);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                            }
                            break;

                        default:
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                            break;
                        }
                    }
                    break;
                #endif

                #if defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_AUTO_CALIBRATION)
                case FRAME_TYPE_MANUAL_CALIBRATION:
                case FRAME_TYPE_AUTO_CALIBRATION:
                    {
                        // Get the current calibration status
                        ADJUST_DATA value;
                        TUSIGN8     type;
                        TUSIGN8     calibrationStaus;

                        dataID.ssIdx     = FrameTableGetCurrentFrame()->frameDefinition.manualCalFrame.ssidx;
                        dataID.objIdx    = FrameTableGetCurrentFrame()->frameDefinition.manualCalFrame.objidx_status;
                        dataID.attribIdx = 0;

                        if (AdjusterGetFloatValue((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, dataID.attribIdx, &value, &type) == (TUSIGN16)OK)
                        {
                            #ifdef AZ200_SPECIAL_FEATURES_HMI
                            TUSIGN8 calibrationType;
                            #endif
                            calibrationStaus = (TUSIGN8)value.data_F;

                            switch (calibrationStaus)
                            {
                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableReleaseOutputHold();
                                #endif
                                calibrationStaus = HMI_CAL_STATUS_ACCEPT;
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;

                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableReleaseOutputHold();
                                #endif
                                calibrationStaus = HMI_CAL_STATUS_REJECT;
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;

                            case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableActivateOutputHold();
                                #endif

                                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_MANUAL_CALIBRATION)
                                {
                                    calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_STABILITY;
                                }
                                else
                                {
                                    calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_STANDARD_1;
                                }
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;

                            #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
                            case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
                                if ((FrameTableIsSampleCollectionEnabled()) && (calibrationStaus == HMI_CAL_STATUS_WAITING_FOR_STABILITY))
                                {
                                    calibrationStaus = HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                }
                                else
                                {
                                    calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = ProcessCommand(COMMAND_EDIT);
                                }
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;


                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
                                calibrationStaus = HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED;
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;

                            case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                                if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_EXIT_ADJUST_CANCEL_AND_BACK;
                                    calibrationStaus = HMI_CAL_STATUS_REJECT;

                                    // Abort
                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);

                                    keyProcessed = ProcessCommand(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                else
                                {
                                    // Continue
                                    keyProcessed = ProcessCommand(COMMAND_OK_MANUAL_CAL);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            #else
                            case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
                            case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;
                            #endif

                            #ifdef HMI_SUPPORTS_AUTO_CALIBRATION
                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_1:
                                calibrationStaus = HMI_CAL_STATUS_CALCULATING_1;
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;

                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_2:
                                calibrationStaus = HMI_CAL_STATUS_CALCULATING_2;
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                break;
                            #else
                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_1:
                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_2:
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;

                            #endif

                            case HMI_CAL_STATUS_SUCCESS:
                                #ifdef AZ200_SPECIAL_FEATURES_HMI
                                if (((const T_O2_CALIBRATION*)GetSubsystemPtr((T_SUBSYSTEM_IDX)CAL_IDX))->IsScheduledCalibrationActiveSRV(&calibrationType))
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_DO_NOT_MOVE;
                                    calibrationStaus = HMI_CAL_STATUS_ACCEPT;

                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = ProcessCommand(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                else
                                #endif
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;
                                    calibrationStaus = HMI_CAL_STATUS_ACCEPT;

                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = processCommand_CheckForSequence(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case HMI_CAL_STATUS_CALCULATING_1:
                            case HMI_CAL_STATUS_CALCULATING_2:

                                #if defined(HMI_SUPPORTS_AUTO_CALIBRATION)
                                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_AUTO_CALIBRATION)
                                {
                                    if (FrameTableIsNavigationFromAutoCalAllowed())
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_BACK);
                                    }
                                    else
                                    {
                                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                    }
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                }
                                #else
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                #endif
                                break;


                            case HMI_CAL_STATUS_INACTIVE:
                                #if defined(HMI_SUPPORTS_AUTO_CALIBRATION)
                                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_AUTO_CALIBRATION)
                                {
                                    if (FrameTableIsNavigationFromAutoCalAllowed())
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_STANDARD_1;
                                        (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    }
                                }
                                #endif
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;


                            case HMI_CAL_STATUS_REJECT:
                            case HMI_CAL_STATUS_ACCEPT:
                            case HMI_CAL_STATUS_SUCCESS_NO_REJECT:
                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
                            default:
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;
                            }
                        }
                        else
                        {
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        }
                    }
                    break;
                #endif

                #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
                case FRAME_TYPE_WAVEFORM_FRAME:
                    {
                        TUSIGN8 waveformIndex = FrameTableGetCurrentFrame()->frameDefinition.waveformFrame.waveformIndex;
                        TUSIGN8 waveformCursor = FrameTable_GetWaveformCursor(waveformIndex);
                        if ((waveformCursor == WAVEFORM_CURSOR_CFG_REL_VAL_EDIT))
                        {
                            keyProcessed = ProcessCommand(COMMAND_OK_CANCEL);
                            if (!FrameTableIsEditEnabled())
                            {
                                FrameTable_WaveformCRV_Edit(waveformIndex, eFALSE);
                            }
                        }
                        else if ((WriteProtect_HMICanAccess(FrameTableGetSignal(FrameTable_GetWaveformCRVSignal(waveformIndex))->value, (TUSIGN8)0))
                              && (waveformCursor == WAVEFORM_CURSOR_CFG_REL_VAL))
                        {
                            FrameTable_WaveformCRV_Edit(waveformIndex, eTRUE);
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        }
                        else
                        {
                            keyProcessed = ProcessCommand(COMMAND_BACK);
                        }
                    }
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                    }
                    break;
                #endif

                #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
                case FRAME_TYPE_LINEARIZER:
                    switch (FrameTable_GetLinearizerCursor())
                    {
                    case LINEARIZER_CURSOR_INPOINT_EDIT:
                    case LINEARIZER_CURSOR_OUTPOINT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_OK_CANCEL);
                        if (!FrameTableIsEditEnabled())
                        {
                            FrameTable_LinearizerEdit(eFALSE);
                        }
                        break;

                    case LINEARIZER_CURSOR_INPOINT_VIEW:
                    case LINEARIZER_CURSOR_OUTPOINT_VIEW:
                        FrameTable_LinearizerEdit(eTRUE);
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                        break;

                    case LINEARIZER_CURSOR_EXIT:
                    default:
                        keyProcessed = ProcessCommand(COMMAND_BACK);
                        break;
                    }
                    break;
                #endif // end of #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES

                #ifdef HMI_SUPPORTS_MATRIX_FRAMES
                case FRAME_TYPE_MATRIX_T_C_D:
                case FRAME_TYPE_MATRIX_X_Y_XY:
                    switch (FrameTable_GetMatrixCursor())
                    {
                    case MATRIX_CURSOR_COLUMN_EDIT:
                    case MATRIX_CURSOR_ROW_EDIT:
                    case MATRIX_CURSOR_ELEMENT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_OK_CANCEL);
                        if (!FrameTableIsEditEnabled())
                        {
                            FrameTable_MatrixEdit(eFALSE);
                        }
                        break;

                    case MATRIX_CURSOR_COLUMN:
                    case MATRIX_CURSOR_ROW:
                    case MATRIX_CURSOR_ELEMENT:
                        FrameTable_MatrixEdit(eTRUE);
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        break;

                    case MATRIX_CURSOR_ELEMENT_RESET:
                        FrameTable_ResetMatrixElement();
                        keyProcessed = 1;
                        LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                        break;

                    case MATRIX_CURSOR_EXIT:
                    default:
                        keyProcessed = ProcessCommand(COMMAND_BACK);
                        break;
                    }
                    break;
                #endif

                default:
                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;
                }
            }
            break;

        case KEYSTATE_AUTOSCROLL:
            if ((LOCAL_keyLockout & KEY_LOCKOUT_AUTOSCROLL) == 0)
            {
                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_OPERATOR)
                {
                    keyProcessed = ProcessCommand(COMMAND_AUTO_NEXT_OPERATOR_FRAME);
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                    }
                }
            }
            break;

        case KEYSTATE_LEFT:
            #ifndef DISABLE_QUICK_EXIT
            if ((LOCAL_keyLockout & KEY_LOCKOUT_LEFT) != 0)
            {
                TBOOL QuickExitAvailable = eFALSE;
                TUSIGN8 tempInstance = 0;

                #ifdef HMI_SUPPORTS_SERVICEPORT
                if (PacketManagerIsRemoteInstance())
                {
                    tempInstance = 1;
                }
                #endif

                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_CONFIG_HEADER)
                {
                    QuickExitAvailable = eTRUE;
                }
                else if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_MENU)
                {
                    QuickExitAvailable = FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 0 ? eTRUE : eFALSE;
                }
                else
                {
                    QuickExitAvailable = eFALSE;
                }

                if (QuickExitAvailable)
                {
                    leftkeyHeld[tempInstance] ++;

                    if ((leftkeyHeld[tempInstance] * HMI_UPDATE_RATE_MILLISEC * HMI_KEY_REPEAT_CYCLES) > 1000)
                    {
                        keyProcessed = ProcessCommand(COMMAND_EXIT);
                        leftkeyHeld[tempInstance] = 0;
                    }
                }
                else
                {
                    leftkeyHeld[tempInstance] = 0;
                }
            }
            else
            {
                TUSIGN8 tempInstance = 0;

                #ifdef HMI_SUPPORTS_SERVICEPORT
                if (PacketManagerIsRemoteInstance())
                {
                    tempInstance = 1;
                }
                #endif

                leftkeyHeld[tempInstance] = 0;
            }
            #endif

            if ((LOCAL_keyLockout & KEY_LOCKOUT_LEFT) == 0)
            {
                switch (FrameTableGetCurrentFrame()->frameType)
                {
                case FRAME_TYPE_OPERATOR:
                    #ifdef LEFT_RIGHT_LOCKOUT
                    if (FrameTableGetLeftRightLockout())
                    {
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    else
                    #endif
                    {
                        #if defined(LEFT_KEY_SELECTS_STANDARD_LEVEL) || defined(LEFT_KEY_SELECTS_ADVANCED_LEVEL) || (defined LEFT_KEY_SELECTS_SERVICE_LEVEL)

                            #if defined(LEFT_KEY_SELECTS_STANDARD_LEVEL)            // LEFT_KEY_SELECTS_STANDARD_LEVEL
                            keyProcessed = ProcessCommand(COMMAND_STANDARD_LEVEL);
                            #elif defined(LEFT_KEY_SELECTS_ADVANCED_LEVEL)          // LEFT_KEY_SELECTS_ADVANCED_LEVEL
                            keyProcessed = ProcessCommand(COMMAND_ADVANCED_LEVEL);
                            #else                                                   // LEFT_KEY_SELECTS_SERVICE_LEVEL
                            keyProcessed = ProcessCommand(COMMAND_SERVICE_LEVEL);
                            #endif
                            keyProcessed = ProcessCommand(COMMAND_SELECT_FROM_MENU);
                            if (keyProcessed)
                            {
                                LOCAL_keyLockout |= (KEY_LOCKOUT_LEFT | KEY_LOCKOUT_RIGHT | KEY_LOCKOUT_UP | KEY_LOCKOUT_DOWN);
                            }
                        #else
                            keyProcessed = ProcessCommand(COMMAND_LAUNCH_OPERATOR_MENU);
                            if (keyProcessed)
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            }
                        #endif
                    }
                    break;

                case FRAME_TYPE_CONFIG_HEADER:
                    keyProcessed = ProcessCommand(COMMAND_EXIT);
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                    }
                    break;

                case FRAME_TYPE_MENU:
                    switch (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag)
                    {
                    #ifdef EDP300_SPECIAL_FEATURES_HMI
                    case 8:
                        switch (FrameTableGetAutoAdjustStatus())
                        {
                        case 0:  // Auto Adjustment inactive
                            keyProcessed = ProcessCommand(COMMAND_RETURN_TO_OPERATOR_PAGE);
                            break;

                        case 1:  // auto adjustment is searching for exhaust end limit
                        case 2:  // auto adjustment is searching for ventilate end limit
                        case 3:  // auto adjustment is preparing time
                        case 4:  // auto adjustment is measuring time (Down)
                        case 5:  // auto adjustment is measuring time (Up)
                        case 6:  // auto adjustment of ip-converter
                        case 7:  // auto adjustment of controller
                            FrameTableAbortAutoAdj();   // abort Auto Adjustment
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                            break;


                        case 8:  // auto adjustement procedure finished (SUCCESSFUL)
                        case 9:  // auto adjustement procedure stopped (UNSUCCESSFUL)
                        default: // Auto Adjustment Status Not recognised
                            FrameTableFinishAutoAdj();  // Acknowledge completion
                            keyProcessed = ProcessCommand(COMMAND_RETURN_TO_OPERATOR_PAGE);
                            break;
                        }

                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                        break;

                    case 7:
                        keyProcessed = ProcessCommand(COMMAND_RETURN_TO_OPERATOR_PAGE);
                        break;
                    #endif

                    #ifdef HMI_USES_COMMONDIAGNOSTICS
                    case 6: // Secure Alarm History
                    case 5: // Alarm History
                    case 3: // Diagnostics View
                        #ifdef DISPLAY_DIAGNOSIS_CONDITION_DETAILS
                        if (FrameTable_isDiagDetailsViewActive())
                        {
                            keyProcessed = FrameTableActivateDiagDetailsView(eFALSE);
                        }
                        else
                        #endif
                        {
                            if (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 3) // if Diagnostics View
                            {
                                keyProcessed = ProcessCommand(COMMAND_RELAUNCH_OPERATOR_MENU);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                            }
                        }
                        break;
                    #endif

                    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
                    case 4: // ARM Access level Selection
                        keyProcessed = ProcessCommand(COMMAND_RETURN_TO_OPERATOR_PAGE);
                        break;
                    #endif

                    case 2: // Signals view
                        keyProcessed = ProcessCommand(COMMAND_RELAUNCH_OPERATOR_MENU);
                        break;

                    case 1: // Operator Menu
                        keyProcessed = ProcessCommand(COMMAND_RETURN_TO_OPERATOR_PAGE);
                        break;

                    case 0:
                    default:
                        keyProcessed = ProcessCommand(COMMAND_BACK);
                        break;
                    }

                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                    }
                    break;

                #if defined(HMI_SUPPORTS_2PT_PH_CALIBRATION)
                case FRAME_TYPE_2_POINT_pH_CALIBRATION:
                    {
                        TWO_POINT_pH_CALIBRATION_FRAME_TYPE const *pFrameData = &(FrameTableGetCurrentFrame()->frameDefinition.pHCalFrame);

                        TUSIGN8 calibrationStatus;

                        T_UNIT const SLOW *pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)pFrameData->ssidx);
                        (void)pSub->Get(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);

                        switch (calibrationStatus)
                        {
                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableActivateOutputHold();
                                #endif
                                calibrationStatus = HMI_CAL_STATUS_ACCEPT;
                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                break;

                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableActivateOutputHold();
                                #endif
                                calibrationStatus = HMI_CAL_STATUS_REJECT;
                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                break;

                        case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                            #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                            FrameTableReleaseOutputHold();
                            #endif
                            calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_TEMP;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            (void)FrameTableSetCurrentFrame(FrameTableGetCurrentFrame(), 0, IDF_SPECIAL_NONE);
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            break;


                        case HMI_CAL_STATUS_VIEW_BUFFER_TEMP:
                            calibrationStatus = HMI_CAL_STATUS_VIEW_BUFFER_1;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            (void)FrameTableSetCurrentFrame(FrameTableGetCurrentFrame(), 0, IDF_SPECIAL_NONE);
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            break;

                        case HMI_CAL_STATUS_VIEW_BUFFER_1:
                            calibrationStatus = HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_1;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            break;

                        case HMI_CAL_STATUS_VIEW_BUFFER_2:
                            calibrationStatus = HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_2;
                            (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            break;

                        case HMI_CAL_STATUS_EDIT_BUFFER_TEMP:
                        case HMI_CAL_STATUS_EDIT_BUFFER_1:
                        case HMI_CAL_STATUS_EDIT_BUFFER_2:
                            keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                            // The cursor can be moved repeatedly until cancel is highlighted
                            // so do not set the key lockout unless cursor is at limit
                            if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            }
                            break;

                        case HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_1:
                        case HMI_CAL_STATUS_WAITING_FOR_STABILITY_BUFFER_2:
                        case HMI_CAL_STATUS_FAILURE:
                        case HMI_CAL_STATUS_SUCCESS:
                            {
                                MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;
                                calibrationStatus = HMI_CAL_STATUS_REJECT;

                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                if (FrameTable_IsOutputHeld() == eTRUE)
                                {
                                    calibrationStatus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT;
                                    moveCommand = COMMAND_DO_NOT_MOVE;
                                }
                                #endif

                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                keyProcessed = processCommand_CheckForSequence(moveCommand);
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            }
                            break;

                        case HMI_CAL_STATUS_SUCCESS_NO_REJECT:
                            {
                                MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;
                                calibrationStatus = HMI_CAL_STATUS_ACCEPT;

                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                if (FrameTable_IsOutputHeld() == eTRUE)
                                {
                                    calibrationStatus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT;
                                    moveCommand = COMMAND_DO_NOT_MOVE;
                                }
                                #endif

                                (void)pSub->Put(pSub,(TUSIGN16)pFrameData->objidx_status,0,(void FAST *)&calibrationStatus);
                                keyProcessed = processCommand_CheckForSequence(moveCommand);
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            }
                            break;

                        default:
                            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                            break;
                        }
                    }
                    break;
                #endif


                #if defined(HMI_SUPPORTS_MANUAL_CALIBRATION) || defined(HMI_SUPPORTS_AUTO_CALIBRATION)
                case FRAME_TYPE_MANUAL_CALIBRATION:
                case FRAME_TYPE_AUTO_CALIBRATION:
                    {
                        // Get the current calibration status
                        ADJUST_DATA value;
                        TUSIGN8     type;
                        TUSIGN8     calibrationStaus;

                        dataID.ssIdx     = FrameTableGetCurrentFrame()->frameDefinition.manualCalFrame.ssidx;
                        dataID.objIdx    = FrameTableGetCurrentFrame()->frameDefinition.manualCalFrame.objidx_status;
                        dataID.attribIdx = 0;

                        if (AdjusterGetFloatValue((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, dataID.attribIdx, &value, &type) == (TUSIGN16)OK)
                        {
                            #ifdef AZ200_SPECIAL_FEATURES_HMI
                            TUSIGN8 calibrationType;
                            #endif
                            calibrationStaus = (TUSIGN8)value.data_F;

                            switch (calibrationStaus)
                            {
                            case HMI_CAL_STATUS_SET_OUTPUT_HOLD:
                                #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                FrameTableReleaseOutputHold();
                                #endif

                                if (FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_MANUAL_CALIBRATION)
                                {
                                    calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_STABILITY;
                                }
                                else
                                {
                                    calibrationStaus = HMI_CAL_STATUS_WAITING_FOR_STANDARD_1;
                                }
                                (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                //(void)FrameTableSetCurrentFrame(FrameTableGetCurrentFrame(), 0);
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                break;

                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT:
                                #ifdef AZ200_SPECIAL_FEATURES_HMI
                                if (((const T_O2_CALIBRATION*)GetSubsystemPtr((T_SUBSYSTEM_IDX)CAL_IDX))->IsScheduledCalibrationActiveSRV(&calibrationType))
                                {
                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    FrameTableActivateOutputHold();
                                    #endif
                                    calibrationStaus = HMI_CAL_STATUS_ACCEPT;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                else
                                #endif
                                {
                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    FrameTableActivateOutputHold();
                                    #endif
                                    calibrationStaus = HMI_CAL_STATUS_ACCEPT;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT:
                                #ifdef AZ200_SPECIAL_FEATURES_HMI
                                if (((const T_O2_CALIBRATION*)GetSubsystemPtr((T_SUBSYSTEM_IDX)CAL_IDX))->IsScheduledCalibrationActiveSRV(&calibrationType))
                                {
                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    FrameTableActivateOutputHold();
                                    #endif
                                    calibrationStaus = HMI_CAL_STATUS_REJECT;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                else
                                #endif
                                {
                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    FrameTableActivateOutputHold();
                                    #endif
                                    calibrationStaus = HMI_CAL_STATUS_REJECT;
                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = processCommand_CheckForSequence(COMMAND_BACK);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;

                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_COMPLETE:
                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_IN_PROGRESS:
                            case HMI_CAL_STATUS_WAITING_FOR_STABILITY:
                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_1:
                            case HMI_CAL_STATUS_WAITING_FOR_STANDARD_2:
                            case HMI_CAL_STATUS_CALCULATING_1:
                            case HMI_CAL_STATUS_CALCULATING_2:
                            case HMI_CAL_STATUS_SUCCESS:
                            case HMI_CAL_STATUS_FAILURE:
                                #ifdef AZ200_SPECIAL_FEATURES_HMI
                                if (((const T_O2_CALIBRATION*)GetSubsystemPtr((T_SUBSYSTEM_IDX)CAL_IDX))->IsScheduledCalibrationActiveSRV(&calibrationType))
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_DO_NOT_MOVE; //COMMAND_BACK;
                                    calibrationStaus = HMI_CAL_STATUS_REJECT;

                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = ProcessCommand(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                }
                                else
                                #endif
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;

                                    #if defined(HMI_SUPPORTS_AUTO_CALIBRATION)
                                    if ((FrameTableGetCurrentFrame()->frameType == FRAME_TYPE_AUTO_CALIBRATION)
                                     && ((calibrationStaus == HMI_CAL_STATUS_CALCULATING_1) || (calibrationStaus == HMI_CAL_STATUS_CALCULATING_2)))
                                    {
                                        if (FrameTableIsNavigationFromAutoCalAllowed())
                                        {
                                            moveCommand = COMMAND_DO_NOT_MOVE;
                                        }
                                    }
                                    #endif

                                    calibrationStaus = HMI_CAL_STATUS_REJECT;

                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_REJECT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = processCommand_CheckForSequence(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                }
                                break;

                            case HMI_CAL_STATUS_SUCCESS_NO_REJECT:
                                {
                                    MOVE_COMMAND_TYPE moveCommand = COMMAND_BACK;
                                    calibrationStaus = HMI_CAL_STATUS_ACCEPT;

                                    #ifdef HMI_SUPPORTS_OUTPUT_HOLD_AND_RELEASE
                                    if (FrameTable_IsOutputHeld() == eTRUE)
                                    {
                                        calibrationStaus = HMI_CAL_STATUS_RELEASE_OUTPUT_HOLD_ACCEPT;
                                        moveCommand = COMMAND_DO_NOT_MOVE;
                                    }
                                    #endif

                                    (void)AdjusterWriteAttribute((TUSIGN16)dataID.ssIdx, (TUSIGN16)dataID.objIdx, (TINT16)dataID.attribIdx, (void FAST *)&calibrationStaus);
                                    keyProcessed = processCommand_CheckForSequence(moveCommand);
                                    LOCAL_keyLockout |= KEY_LOCKOUT_RIGHT;
                                }
                                break;


                            case HMI_CAL_STATUS_WAITING_FOR_NEW_VALUE:
                                if (FrameTableIsEditEnabled())
                                {
                                    keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                                    // The cursor can be moved repeatedly until cancel is highlighted
                                    // so do not set the key lockout unless cursor is at limit
                                    if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                                    {
                                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                                    }
                                }
                                break;

                            case HMI_CAL_STATUS_INACTIVE:
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                                break;

                            case HMI_CAL_STATUS_SAMPLE_COLLECTION_STOPPED:
                            default:
                                keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                                break;
                            }
                        }
                    }
                    break;
                #endif

                case FRAME_TYPE_BIT_ENUMERATED_PARAM:
                case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
                case FRAME_TYPE_TIME_AND_DATE_PARAM:
                case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
                case FRAME_TYPE_NUMERICAL_PARAM:
                case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
                case FRAME_TYPE_INTEGER_PARAM:
                case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
                case FRAME_TYPE_IP_ADDRESS:
                case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:
                    if (FrameTableIsEditEnabled())
                    {
                        keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                        // The cursor can be moved repeatedly until cancel is highlighted
                        // so do not set the key lockout unless cursor is at limit
                        if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    else
                    {
                        #ifdef HMI_SUPPORTS_EASYSETUP
                        if (FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.easySetupFlag == 1)
                        {
                            keyProcessed = ProcessCommand(COMMAND_NEXT_EASYSETUP);
                        }
                        else
                        #endif
                        {
                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;

                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                                {
                                    keyProcessed = ProcessCommand(COMMAND_BACK);
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_NEXT_SEQUENCE);
                                }
                            }
                            else
                            #endif
                            {
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                            }
                        }

                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    break;

                case FRAME_TYPE_STRING_PARAM:
                case FRAME_TYPE_STRING_PARAM_NO_EDIT:
                case FRAME_TYPE_STRING_PARAM_PACKED:
                case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
                    #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
                    if (FrameTableIsEditEnabled())
                    {
//                        #ifdef HMI_SUPPORTS_SECURITY_ACCESS
//                        TUSIGN8 i;
//                        TUSIGN8 stringSI = FrameTableGetCurrentFrame()->frameDefinition.stringParamFrame.stringText.ssIdx;
//                        TUSIGN8 stringOI = FrameTableGetCurrentFrame()->frameDefinition.stringParamFrame.stringText.objIdx;
//
//                        if ((FrameTableGetCurrentFrame()->frameDefinition.stringParamFrame.specialStringFlag != 0)
//                         || ((stringSI == HMI_IDX) && ((stringOI == HMI_IDX_SetPassword_StandardLevel)
//                                                   || (stringOI == HMI_IDX_SetPassword_AdvancedLevel)
//                                                   || (stringOI == HMI_IDX_SetPassword_ServiceLevel)
//                                                   || (stringOI == HMI_IDX_SetPassword_StandardLevelDefault)
//                                                   || (stringOI == HMI_IDX_SetPassword_AdvancedLevelDefault)
//                                                   || (stringOI == HMI_IDX_SetPassword_ServiceLevelDefault))))
//                        {
//                            TUSIGN8 CursorPos = FrameTableGetCursorPosition();
//                            TUSIGN8 CursorLim = FrameTableGetCursorPositionLimit();
//
//                            if ((AdjusterGetVariableAsString(&i)[FrameTableGetCursorPosition()] == 0x00)
//                              &&(CursorPos != CursorLim))
//                            {
//                                keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR_TO_END);
//                            }
//                            else
//                            {
//                                keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
//                            }
//                        }
//                        else
//                        #endif
                        {
                            TUSIGN8 i;
                            TUSIGN8 CursorPos = FrameTableGetCursorPosition();
                            TUSIGN8 CursorLim = FrameTableGetCursorPositionLimit();

                            if ((AdjusterGetVariableAsString(&i)[CursorPos] == 0x00)
                              &&(CursorPos != CursorLim))
                            {
                                keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR_TO_END);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                            }
                        }

                        // The cursor can be moved repeatedly until cancel is highlighted
                        // so do not set the key lockout unless cursor is at limit
                        if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    else
                    {
                        #ifdef HMI_SUPPORTS_EASYSETUP
                        if (FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.easySetupFlag == 1)
                        {
                            keyProcessed = ProcessCommand(COMMAND_NEXT_EASYSETUP);
                        }
                        else
                        #endif
                        {
                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;

                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                                {
                                    keyProcessed = ProcessCommand(COMMAND_BACK);
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_NEXT_SEQUENCE);
                                }
                            }
                            else
                            #endif
                            {
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                            }
                        }

                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    #else
                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    #endif
                    break;

                case FRAME_TYPE_ENUMERATED_PARAM:
                case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
                    if (FrameTableIsEditEnabled())
                    {
                        keyProcessed = ProcessCommand(COMMAND_CANCEL);
                    }
                    #ifdef HMI_SUPPORTS_EASYSETUP
                    else if (FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.easySetupFlag == 1)
                    {
                        keyProcessed = ProcessCommand(COMMAND_NEXT_EASYSETUP);
                    }
                    #endif
                    else
                    {
                        #ifdef HMI_SUPPORTS_SEQUENCES
                        TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                        TUSIGN16 SeqMenuTableOffset;

                        if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                        {
                            if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                            {
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_NEXT_SEQUENCE);
                            }
                        }
                        else
                        #endif
                        {
                            keyProcessed = ProcessCommand(COMMAND_BACK);
                        }
                    }

                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                    }
                    break;

                case FRAME_TYPE_MANUAL_ADJUST:
                    {
                        #ifdef HMI_SUPPORTS_SEQUENCES
                        TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                        TUSIGN16 SeqMenuTableOffset;

                        if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                        {
                            if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                            {
                                keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL_AND_BACK);
                            }
                            else
                            {
                                keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_SEQUENCE);
                            }
                        }
                        else
                        #endif
                        {
                            #ifdef HMI_SUPPORTS_EASYSETUP
                            switch (FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.easySetupFlag)
                            {
                            case 2:     keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL_AND_BACK);             break;
                            case 1:     keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_EASYSETUP);   break;
                            default:    keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL);                      break;
                            }
                            #else
                            keyProcessed = ProcessCommand(COMMAND_EXIT_ADJUST_CANCEL);
                            #endif
                        }
                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    break;

                case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
                    {
                        MOVE_COMMAND_TYPE commandToProcess = COMMAND_BACK;

                        if (AdjusterIsDataAdjusted() != 0)
                        {
                            // get timer value (timer goes from 0 to 100)
                            ADJUST_DATA tVal;
                            TUSIGN8 tType;
                            TUSIGN16 percentComplete;

                            if (AdjusterGetFloatValue((TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.value_ssIdx,
                                                      (TUSIGN16)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.dispvalue_objidx,
                                                      0,
                                                      &tVal,
                                                      &tType) == OK)
                            {
                                #ifdef HMI_USES_DOUBLE
                                if (tType == ST_DOUBLE)
                                {
                                    percentComplete = (TUSIGN16)(tVal.data_D + 0.5);
                                }
                                else
                                #endif
                                {
                                    percentComplete = (TUSIGN16)(tVal.data_F + 0.5F);
                                }

                                // if timer < 100 then adjust function not complete
                                if (percentComplete < 100)
                                {
                                    commandToProcess = COMMAND_DO_NOT_MOVE;
                                }
                            }
                        }

                        if (commandToProcess == COMMAND_BACK)
                        {
                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;
                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                commandToProcess = COMMAND_NEXT_SEQUENCE;
                            }
                            else
                            #endif
                            {
                                #ifdef HMI_SUPPORTS_EASYSETUP
                                switch (FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.easySetupFlag)
                                {
                                case 1:     commandToProcess = COMMAND_NEXT_EASYSETUP;  break;
                                case 2:     commandToProcess = COMMAND_NEXT_EASYSETUP;  break;
                                default:    commandToProcess = COMMAND_BACK;            break;
                                }
                                #else
                                commandToProcess = COMMAND_BACK;
                                #endif
                            }
                        }
                        keyProcessed = ProcessCommand(commandToProcess);
                    }
                    break;

                case FRAME_TYPE_AUTO_ADJUST:
                    #ifdef EDP300_SPECIAL_FEATURES_HMI
                    if (FrameTableIfAutoAdjMarker((TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.value_ssIdx,
                                                  (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_objidx,
                                                  (TUSIGN8)FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.adjvalue_attribidx))
                    {
                        TUSIGN8 AutoAdjustStatus = FrameTableGetAutoAdjustStatus();
                        MOVE_COMMAND_TYPE commandToProcess;

                        if ((AutoAdjustStatus >= 1) && (AutoAdjustStatus <= 7))
                        {
                            //  1 = auto adjustment is searching for exhaust end limit
                            //  2 = auto adjustment is searching for ventilate end limit
                            //  3 = auto adjustment is preparing time
                            //  4 = auto adjustment is measuring time (Down)
                            //  5 = auto adjustment is measuring time (Up)
                            //  6 = auto adjustment of ip-converter
                            //  7 = auto adjustment of controller

                            FrameTableAbortAutoAdj();   // abort Auto Adjustment
                            commandToProcess = COMMAND_DO_NOT_MOVE;
                        }
                        else
                        {
                            //  0 = Auto Adjustment inactive
                            //  8 = auto adjustement procedure finished (SUCCESSFUL)
                            //  9 = auto adjustement procedure stopped (UNSUCCESSFUL)
                            // >9 = Auto Adjustment Status Not recognised

                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;
                            #endif

                            if (AutoAdjustStatus != 0)
                            {
                                FrameTableFinishAutoAdj();  // Acknowledge completion
                            }

                            commandToProcess = COMMAND_BACK;

                            #ifdef HMI_SUPPORTS_SEQUENCES
                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                if (SeqMenuPosnCursor != SeqMenuPosnCursorLim)
                                {
                                    commandToProcess = COMMAND_NEXT_SEQUENCE;
                                }
                            }
                            #endif

                            #ifdef HMI_SUPPORTS_EASYSETUP
                            if (FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.easySetupFlag == 1)
                            {
                                commandToProcess = COMMAND_NEXT_EASYSETUP;
                            }
                            #endif
                        }
                        keyProcessed = ProcessCommand(commandToProcess);
                    }
                    else
                    #endif
                    {
                        #ifdef HMI_SUPPORTS_EASYSETUP
                        if (FrameTableGetCurrentFrame()->frameDefinition.adjParamFrame.easySetupFlag == 1)
                        {
                            keyProcessed = ProcessCommand(COMMAND_NEXT_EASYSETUP);
                        }
                        else
                        #endif
                        {
                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;

                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
                                {
                                    keyProcessed = ProcessCommand(COMMAND_BACK);
                                }
                                else
                                {
                                    keyProcessed = ProcessCommand(COMMAND_NEXT_SEQUENCE);
                                }
                            }
                            else
                            #endif
                            {
                                keyProcessed = ProcessCommand(COMMAND_BACK);
                            }
                        }
                    }
                    if (keyProcessed)
                    {
                        LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                    }
                    break;

                #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
                case FRAME_TYPE_WAVEFORM_FRAME:
                    {
                        TUSIGN8 waveformIndex = FrameTableGetCurrentFrame()->frameDefinition.waveformFrame.waveformIndex;
                        TUSIGN8 waveformCursor = FrameTable_GetWaveformCursor(waveformIndex);

                        if (waveformCursor == WAVEFORM_CURSOR_CFG_REL_VAL_EDIT)
                        {
                            keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                            // The cursor can be moved repeatedly until cancel is highlighted
                            // so do not set the key lockout unless cursor is at limit
                            if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                            {
                                LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                            }
                        }
                        else
                        {
                            FrameTable_NextWaveformCursor(waveformIndex);
                            keyProcessed = 1;
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    break;
                #endif

                #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
                case FRAME_TYPE_LINEARIZER:
                    if (FrameTable_NextLinearizerCursor())
                    {
                        // linearizer cursor has been moved
                        keyProcessed = 1;
                        // The cursor can be moved repeatedly until Exit is highlighted
                        // so do not set the key lockout unless cursor is at "Exit"
                        if (FrameTable_GetLinearizerCursor() == LINEARIZER_CURSOR_EXIT)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    else
                    {
                        // matrix cursor has not been moved LINEARIZER_EDIT is active
                        // move edit cursor
                        keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                        // The cursor can be moved repeatedly until cancel is highlighted
                        // so do not set the key lockout unless cursor is at limit
                        if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    break;
                #endif // end of #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES


                #ifdef HMI_SUPPORTS_MATRIX_FRAMES
                case FRAME_TYPE_MATRIX_T_C_D:
                case FRAME_TYPE_MATRIX_X_Y_XY:
                    if (FrameTable_NextMatrixCursor())
                    {
                        // matrix cursor has been moved
                        keyProcessed = 1;
                        // The cursor can be moved repeatedly until Exit is highlighted
                        // so do not set the key lockout unless cursor is at "Exit"
                        if (FrameTable_GetMatrixCursor() == MATRIX_CURSOR_EXIT)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    else
                    {
                        // matrix cursor has not been moved MATRIX_EDIT is active
                        // move edit cursor
                        keyProcessed = ProcessCommand(COMMAND_MOVE_CURSOR);
                        // The cursor can be moved repeatedly until cancel is highlighted
                        // so do not set the key lockout unless cursor is at limit
                        if (FrameTableGetCursorPosition() == FrameTableGetCursorPositionLimit())
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_LEFT;
                        }
                    }
                    break;
                #endif

                default:
                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;
                }
            }
            break;

        case KEYSTATE_UP:
            if ((LOCAL_keyLockout & KEY_LOCKOUT_UP) == 0)
            {
                switch (FrameTableGetCurrentFrame()->frameType)
                {
                case FRAME_TYPE_CONFIG_HEADER:
                    keyProcessed = ProcessCommand(COMMAND_MOVE_UP_LEVEL);
                    // Repeated movement through menu is allowed so do not set the key lockout
                    break;

                case FRAME_TYPE_MENU:
                    #ifdef EDP300_SPECIAL_FEATURES_HMI
                    if  (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 8)
                    {
                        TUSIGN8 AutoAdjStatus = FrameTableGetAutoAdjustStatus();
                        TUSIGN8 AutoAdjScope = FrameTableGetAutoAdjustScope();

                        if ((AutoAdjStatus == 0)    // Auto Adjustment inactive
                         && (AutoAdjScope <= 4))    // Auto Adjustment scope is valid
                        {
                            FrameTableStartAdjust(eTRUE);   // Start AutoAdjust(Rotary)
                        }
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_UP;
                        }
                    }
                    else
                    #endif
                    {
                        keyProcessed = ProcessCommand(COMMAND_MOVE_UP_MENU);
                        //Repeated movement in menu is allowed so do not set the key lockout
                    }
                    break;

                case FRAME_TYPE_NUMERICAL_PARAM:
                case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
                case FRAME_TYPE_ENUMERATED_PARAM:
                case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
                case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                case FRAME_TYPE_STRING_PARAM:
                case FRAME_TYPE_STRING_PARAM_NO_EDIT:
                case FRAME_TYPE_STRING_PARAM_PACKED:
                case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
                case FRAME_TYPE_TIME_AND_DATE_PARAM:
                case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
                case FRAME_TYPE_BIT_ENUMERATED_PARAM:
                case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
                case FRAME_TYPE_INTEGER_PARAM:
                case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
                case FRAME_TYPE_IP_ADDRESS:
                case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
                case FRAME_TYPE_MANUAL_ADJUST:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:

                    if (FrameTableIsEditEnabled())
                    {
                        #if defined(ENUM_ACCELERATOR) || defined(STRING_ACCELERATOR) || defined(IPADDR_ACCELERATOR) || defined(MANUAL_ADJUST_ACCELERATOR) || defined(DATE_TIME_ACCELERATOR)
                        switch (FrameTableGetCurrentFrame()->frameType)
                        {
                        case FRAME_TYPE_IP_ADDRESS:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_UP);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_STRING_PARAM:
                        case FRAME_TYPE_STRING_PARAM_PACKED:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_UP);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_ENUMERATED_PARAM:
                        case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_UP);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_MANUAL_ADJUST:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_UP);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_TIME_AND_DATE_PARAM:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_UP);
                            #endif
                            break;

                        default:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;
                        }
                        #endif // #if defined(ENUM_ACCELERATOR) || defined(STRING_ACCELERATOR) || defined(IPADDR_ACCELERATOR) || defined(MANUAL_ADJUST_ACCELERATOR) || defined(DATE_TIME_ACCELERATOR)

                        keyProcessed = ProcessCommand(COMMAND_ADJUST_UP);
                        //Repeated adjustment is allowed so do not set the key lockout
                    }
                    else
                    {
                        if (FrameTableGetCurrentFrame()->frameType != FRAME_TYPE_MANUAL_ADJUST)
                        {
                            MOVE_COMMAND_TYPE CommandToProcess = COMMAND_DO_NOT_MOVE;

                            #ifndef DISABLE_BACK_MOVE_SELECT

                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;

                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                CommandToProcess = COMMAND_DO_NOT_MOVE;
                            }
                            else
                            #endif
                            {
                                #ifdef HMI_SUPPORTS_EASYSETUP
                                // Do not process BACK_MOVE_SELECT if frame is within easy setup menu.
                                // Strictly the s/w should test the easySetupFlag from the correct element
                                // in the frameDefinition union but the easySetupFlag is in the same place irrespective of
                                // what the frameType actually is
                                if (FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.easySetupFlag != 0)
                                {
                                    CommandToProcess = COMMAND_DO_NOT_MOVE;
                                }
                                else
                                #endif
                                {
                                    CommandToProcess = COMMAND_BACK_MOVE_UP_SELECT;
                                    //Repeated adjustment is allowed so do not set the key lockout
                                }
                            }

                            #endif
                            keyProcessed = ProcessCommand(CommandToProcess);
                        }
                    }
                    break;

                case FRAME_TYPE_MANUAL_CALIBRATION:
                case FRAME_TYPE_2_POINT_pH_CALIBRATION:
                    if (FrameTableIsEditEnabled())
                    {
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_UP);
                        //Repeated adjustment is allowed so do not set the key lockout
                    }
                    break;

                case FRAME_TYPE_AUTO_ADJUST:
                case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;

                #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
                case FRAME_TYPE_WAVEFORM_FRAME:
                    {
                        TUSIGN8 waveform = FrameTableGetCurrentFrame()->frameDefinition.waveformFrame.waveformIndex;

                        switch (FrameTable_GetWaveformCursor(waveform))
                        {
                        case WAVEFORM_CURSOR_PRC_REL_VAL:       FrameTable_PrevWaveformPRVSignal(waveform);                 keyProcessed = 1;   break;
                        case WAVEFORM_CURSOR_CFG_REL_VAL:       FrameTable_PrevWaveformCRVSignal(waveform);                 keyProcessed = 1;   break;
                        case WAVEFORM_CURSOR_H_WIDTH:           keyProcessed = Adjuster_WaveformAction(WAVEFORM_CURSOR_H_WIDTH,  KEYSTATE_UP);  break;
                        case WAVEFORM_CURSOR_H_OFFSET:          keyProcessed = Adjuster_WaveformAction(WAVEFORM_CURSOR_H_OFFSET, KEYSTATE_UP);  break;
                        case WAVEFORM_CURSOR_CFG_REL_VAL_EDIT:  keyProcessed = ProcessCommand(COMMAND_ADJUST_UP);                               break;
                        default:                                                                                            keyProcessed = 0;   break;
                        }
                    }
                    break;
                #endif

                #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
                case FRAME_TYPE_LINEARIZER:
                    switch (FrameTable_GetLinearizerCursor())
                    {
                    case LINEARIZER_CURSOR_INPOINT_EDIT:
                    case LINEARIZER_CURSOR_OUTPOINT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_UP);
                        break;

                    case LINEARIZER_CURSOR_INPOINT_VIEW:
                        FrameTable_CaptureLinearizerInputPoint();
                        keyProcessed = 1;
                        break;

                    case LINEARIZER_CURSOR_OUTPOINT_VIEW:
                        FrameTable_SetLinearizerPointEnable(eTRUE);
                        keyProcessed = 1;
                        break;

                    case LINEARIZER_CURSOR_EXIT:
                        FrameTable_NextLinearizerPoint(eTRUE);
                        keyProcessed = 1;
                        break;

                    default:
                        keyProcessed = 0;
                        break;
                    }
                    break;
                #endif // end of #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES

                #ifdef HMI_SUPPORTS_MATRIX_FRAMES
                case FRAME_TYPE_MATRIX_T_C_D:
                case FRAME_TYPE_MATRIX_X_Y_XY:
                    switch (FrameTable_GetMatrixCursor())
                    {
                    case MATRIX_CURSOR_COLUMN:
                    case MATRIX_CURSOR_ROW:
                    case MATRIX_CURSOR_ELEMENT:
                        FrameTable_NextMatrixElement(eTRUE);
                        keyProcessed = 1;
                        break;

                    case MATRIX_CURSOR_COLUMN_EDIT:
                    case MATRIX_CURSOR_ROW_EDIT:
                    case MATRIX_CURSOR_ELEMENT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_UP);
                        break;

                    case MATRIX_CURSOR_ELEMENT_RESET:
                    case MATRIX_CURSOR_EXIT:
                    default:
                        keyProcessed = 0;
                        break;
                    }
                    break;
                #endif

                case FRAME_TYPE_OPERATOR:
                default:
                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;
                }
            }
            break;

        case KEYSTATE_DOWN:
            if ((LOCAL_keyLockout & KEY_LOCKOUT_DOWN) == 0)
            {
                switch (FrameTableGetCurrentFrame()->frameType)
                {
                case FRAME_TYPE_CONFIG_HEADER:
                    keyProcessed = ProcessCommand(COMMAND_MOVE_DOWN_LEVEL);
                    // Repeated movement through menu is allowed so do not set the key lockout
                    break;

                case FRAME_TYPE_MENU:
                    #ifdef EDP300_SPECIAL_FEATURES_HMI
                    if  (FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag == 8)
                    {
                        TUSIGN8 AutoAdjStatus = FrameTableGetAutoAdjustStatus();
                        TUSIGN8 AutoAdjScope = FrameTableGetAutoAdjustScope();

                        if ((AutoAdjStatus == 0)    // Auto Adjustment inactive
                         && (AutoAdjScope <= 4))    // Auto Adjustment scope is valid
                        {
                            FrameTableStartAdjust(eFALSE);   // Start AutoAdjust(Linear)
                        }
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                        if (keyProcessed)
                        {
                            LOCAL_keyLockout |= KEY_LOCKOUT_DOWN;
                        }
                    }
                    else
                    #endif
                    {
                        keyProcessed = ProcessCommand(COMMAND_MOVE_DOWN_MENU);
                        //Repeated movement in menu is allowed so do not set the key lockout
                    }
                    break;

                case FRAME_TYPE_NUMERICAL_PARAM:
                case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
                case FRAME_TYPE_ENUMERATED_PARAM:
                case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
                case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                case FRAME_TYPE_STRING_PARAM:
                case FRAME_TYPE_STRING_PARAM_NO_EDIT:
                case FRAME_TYPE_STRING_PARAM_PACKED:
                case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
                case FRAME_TYPE_TIME_AND_DATE_PARAM:
                case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
                case FRAME_TYPE_BIT_ENUMERATED_PARAM:
                case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
                case FRAME_TYPE_INTEGER_PARAM:
                case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
                case FRAME_TYPE_IP_ADDRESS:
                case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
                case FRAME_TYPE_MANUAL_ADJUST:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
                case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:

                    if (FrameTableIsEditEnabled())
                    {
                        #if defined(ENUM_ACCELERATOR) || defined(STRING_ACCELERATOR) || defined(IPADDR_ACCELERATOR) || defined(MANUAL_ADJUST_ACCELERATOR) || defined(DATE_TIME_ACCELERATOR)
                        switch (FrameTableGetCurrentFrame()->frameType)
                        {
                        case FRAME_TYPE_IP_ADDRESS:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_DOWN);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_STRING_PARAM:
                        case FRAME_TYPE_STRING_PARAM_PACKED:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_DOWN);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_ENUMERATED_PARAM:
                        case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_DOWN);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_MANUAL_ADJUST:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_DOWN);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;

                        case FRAME_TYPE_TIME_AND_DATE_PARAM:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_DOWN);
                            #endif
                            break;

                        default:
                            #ifdef IPADDR_ACCELERATOR
                            FrameTableUpdateIPAddressAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef STRING_ACCELERATOR
                            FrameTableUpdateStringAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef ENUM_ACCELERATOR
                            FrameTableUpdateEnumAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef MANUAL_ADJUST_ACCELERATOR
                            FrameTableUpdateManAdjustAccellerator(KEYSTATE_NONE);
                            #endif
                            #ifdef DATE_TIME_ACCELERATOR
                            FrameTableUpdateDateTimeAccellerator(KEYSTATE_NONE);
                            #endif
                            break;
                        }
                        #endif // #if defined(ENUM_ACCELERATOR) || defined(STRING_ACCELERATOR) || defined(IPADDR_ACCELERATOR) || defined(MANUAL_ADJUST_ACCELERATOR) || defined(DATE_TIME_ACCELERATOR)

                        keyProcessed = ProcessCommand(COMMAND_ADJUST_DOWN);
                        //Repeated adjustment is allowed so do not set the key lockout
                    }
                    else
                    {
                        if (FrameTableGetCurrentFrame()->frameType != FRAME_TYPE_MANUAL_ADJUST)
                        {
                            MOVE_COMMAND_TYPE CommandToProcess = COMMAND_DO_NOT_MOVE;

                            #ifndef DISABLE_BACK_MOVE_SELECT
                            #ifdef HMI_SUPPORTS_SEQUENCES
                            TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
                            TUSIGN16 SeqMenuTableOffset;

                            if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
                            {
                                CommandToProcess = COMMAND_DO_NOT_MOVE;
                            }
                            else
                            #endif
                            {
                                #ifdef HMI_SUPPORTS_EASYSETUP
                                // Do not process BACK_MOVE_SELECT if frame is within easy setup menu.
                                // Strictly the s/w should test the easySetupFlag from the correct element
                                // in the frameDefinition union but the easySetupFlag is in the same place irrespective of
                                // what the frameType actually is
                                if (FrameTableGetCurrentFrame()->frameDefinition.numericalParamFrame.easySetupFlag != 0)
                                {
                                    CommandToProcess = COMMAND_DO_NOT_MOVE;
                                }
                                else
                                #endif
                                {
                                    CommandToProcess = COMMAND_BACK_MOVE_DOWN_SELECT;
                                    //Repeated adjustment is allowed so do not set the key lockout
                                }
                            }
                            #endif
                            keyProcessed = ProcessCommand(CommandToProcess);
                        }
                    }
                    break;

                case FRAME_TYPE_MANUAL_CALIBRATION:
                case FRAME_TYPE_2_POINT_pH_CALIBRATION:
                    if (FrameTableIsEditEnabled())
                    {
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_DOWN);
                        //Repeated adjustment is allowed so do not set the key lockout
                    }
                    break;

                case FRAME_TYPE_AUTO_ADJUST:
                case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
                        keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;

                #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
                case FRAME_TYPE_WAVEFORM_FRAME:
                    {
                        TUSIGN8 waveform = FrameTableGetCurrentFrame()->frameDefinition.waveformFrame.waveformIndex;
                        switch (FrameTable_GetWaveformCursor(waveform))
                        {
                        case WAVEFORM_CURSOR_PRC_REL_VAL:       FrameTable_NextWaveformPRVSignal(waveform);                     keyProcessed = 1;   break;
                        case WAVEFORM_CURSOR_CFG_REL_VAL:       FrameTable_NextWaveformCRVSignal(waveform);                     keyProcessed = 1;   break;
                        case WAVEFORM_CURSOR_H_WIDTH:           keyProcessed = Adjuster_WaveformAction(WAVEFORM_CURSOR_H_WIDTH,  KEYSTATE_DOWN);    break;
                        case WAVEFORM_CURSOR_H_OFFSET:          keyProcessed = Adjuster_WaveformAction(WAVEFORM_CURSOR_H_OFFSET, KEYSTATE_DOWN);    break;
                        case WAVEFORM_CURSOR_CFG_REL_VAL_EDIT:  keyProcessed = ProcessCommand(COMMAND_ADJUST_DOWN);                                 break;
                        default:                                                                                                keyProcessed = 0;   break;
                        }
                    }
                    break;
                #endif

                #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
                case FRAME_TYPE_LINEARIZER:
                    switch (FrameTable_GetLinearizerCursor())
                    {
                    case LINEARIZER_CURSOR_INPOINT_EDIT:
                    case LINEARIZER_CURSOR_OUTPOINT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_DOWN);
                        break;

                    case LINEARIZER_CURSOR_OUTPOINT_VIEW:
                        FrameTable_SetLinearizerPointEnable(eFALSE);
                        keyProcessed = 1;
                        break;

                    case LINEARIZER_CURSOR_EXIT:
                        FrameTable_NextLinearizerPoint(eFALSE);
                        keyProcessed = 1;
                        break;

                    case LINEARIZER_CURSOR_INPOINT_VIEW:
                    default:
                        keyProcessed = 0;
                        break;
                    }
                    break;
                #endif // end of #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES


                #ifdef HMI_SUPPORTS_MATRIX_FRAMES
                case FRAME_TYPE_MATRIX_T_C_D:
                case FRAME_TYPE_MATRIX_X_Y_XY:
                    switch (FrameTable_GetMatrixCursor())
                    {
                    case MATRIX_CURSOR_COLUMN:
                    case MATRIX_CURSOR_ROW:
                    case MATRIX_CURSOR_ELEMENT:
                        FrameTable_NextMatrixElement(eFALSE);
                        keyProcessed = 1;
                        break;

                    case MATRIX_CURSOR_COLUMN_EDIT:
                    case MATRIX_CURSOR_ROW_EDIT:
                    case MATRIX_CURSOR_ELEMENT_EDIT:
                        keyProcessed = ProcessCommand(COMMAND_ADJUST_DOWN);
                        break;

                    case MATRIX_CURSOR_ELEMENT_RESET:
                    case MATRIX_CURSOR_EXIT:
                    default:
                        keyProcessed = 0;
                        break;
                    }
                    break;
                #endif

                default:
                    keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
                    break;
                }
            }
            break;

        default:
            keyProcessed = ProcessCommand(COMMAND_DO_NOT_MOVE);
            break;
        }
    }
  }
  return keyProcessed;
}

TUSIGN16 processCommand_CheckForSequence(MOVE_COMMAND_TYPE moveCommand)
{
    TUSIGN16 returnVal;

    if ((moveCommand == COMMAND_BACK) || (COMMAND_EXIT_ADJUST_CANCEL_AND_BACK))
    {
        #ifdef HMI_SUPPORTS_SEQUENCES
        TUSIGN8 SeqMenuPosnCursor, SeqMenuPosnCursorLim;
        TUSIGN16 SeqMenuTableOffset;

        if (FrameTableGetSequenceSettings(&SeqMenuPosnCursor, &SeqMenuPosnCursorLim, &SeqMenuTableOffset) == eTRUE)
        {
            if (SeqMenuPosnCursor == SeqMenuPosnCursorLim)
            {
                returnVal = ProcessCommand(moveCommand);
            }
            else
            {
                returnVal = ProcessCommand((MOVE_COMMAND_TYPE)(moveCommand == COMMAND_EXIT_ADJUST_CANCEL_AND_BACK ? COMMAND_EXIT_ADJUST_CANCEL_AND_NEXT_SEQUENCE : COMMAND_NEXT_SEQUENCE));
            }
        }
        else
        #endif
        {
            returnVal = ProcessCommand(COMMAND_BACK);
        }
    }
    else
    {
        returnVal = ProcessCommand(moveCommand);
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN16 ProcessCommand(MOVE_COMMAND_TYPE moveCommand)
{
#if defined (HMI_MODULE_TEST) && defined(HMI_MODULE_TEST_COORDINATOR_2)
    moduleTestCommand = moveCommand;
    return 1;
#else

    if (moveCommand > COMMAND_DO_NOT_MOVE)
    {
        AdjusterSetLimitError(0,OK);
    }

    if (moveCommand == COMMAND_NEXT_OPERATOR_FRAME)
    {
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        if (FrameTableIsAutoscrollEnabled() == eTRUE)
        {
            FrameTableDisableAutoscroll();
            moveCommand = COMMAND_DO_NOT_MOVE;
        }
        #endif
    }

//    if (moveCommand == COMMAND_AUTO_NEXT_OPERATOR_FRAME)
//    {
//        moveCommand = COMMAND_NEXT_OPERATOR_FRAME;
//    }

    TUSIGN16 returnVal;

    if ((moveCommand == COMMAND_ADJUST_UP) || (moveCommand == COMMAND_ADJUST_DOWN))
    {
        returnVal = AdjusterAdjust(moveCommand);
    }
    else if ((moveCommand >= COMMAND_MENU) && (moveCommand <= COMMAND_SELECT_FROM_MENU))
    {
        returnVal = FrameMoverMove(moveCommand);
    }
    else if (moveCommand == COMMAND_DO_AUTO_ADJUST)
    {
        returnVal = AdjusterDoAutoAdjust();
    }
    #ifndef DISABLE_BACK_MOVE_SELECT
    else if ((moveCommand == COMMAND_BACK_MOVE_UP_SELECT) || (moveCommand == COMMAND_BACK_MOVE_DOWN_SELECT))
    {
        MOVE_COMMAND_TYPE Up_Down = moveCommand == COMMAND_BACK_MOVE_UP_SELECT ? COMMAND_MOVE_UP_MENU : COMMAND_MOVE_DOWN_MENU;
        TUSIGN8 newFrameType;
        do
        {
            returnVal = FrameMoverMove(COMMAND_BACK);
            returnVal = FrameMoverMove(Up_Down);
            returnVal = FrameMoverMove(COMMAND_SELECT_FROM_MENU);

            newFrameType = FrameTableGetCurrentFrame()->frameType;
        }
        while((newFrameType != FRAME_TYPE_NUMERICAL_PARAM)
           && (newFrameType != FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_ENUMERATED_PARAM)
           && (newFrameType != FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP)
           && (newFrameType != FRAME_TYPE_STRING_PARAM)
           && (newFrameType != FRAME_TYPE_STRING_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_STRING_PARAM_PACKED)
           && (newFrameType != FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT)
           && (newFrameType != FRAME_TYPE_TIME_AND_DATE_PARAM)
           && (newFrameType != FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_BIT_ENUMERATED_PARAM)
           && (newFrameType != FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_INTEGER_PARAM)
           && (newFrameType != FRAME_TYPE_INTEGER_PARAM_NO_EDIT)
           && (newFrameType != FRAME_TYPE_IP_ADDRESS)
           && (newFrameType != FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT)
           && (newFrameType != FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT)
           && (newFrameType != FRAME_TYPE_IP_ADDRESS_NO_EDIT));
    }
    #endif

    else
    {
        returnVal = 0;
    }
#endif

    return returnVal;
}

TUSIGN8 HmiCoordinatorGetInstance(void)
{
    return  HMI_Instance;
}

void HmiCoordinatorResetConfigurationTimeout(void)
{
    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
    ResetInactiveUserTimeout();
    #endif
    ConfigModeManagerResetConfigurationTimeout();
}

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
void LogoutInactiveUser(TUSIGN8 keyState)
{
    TUSIGN8 requiredAction = 0; // Do nothing

    #ifdef AZ200_SPECIAL_FEATURES_HMI
    if (((const T_O2_CALIBRATION*)(GetSubsystemPtr(CAL_IDX)))->IsCalibrationActiveSRV() == eTRUE)
    {
        requiredAction = 1;     // Clear Timer
    }
    else
    #endif
    {
        if (AdjusterGetARMLevel() == ARM_ACCESS_READ_ONLY)
        {
            requiredAction = 1;     // Clear Timer
        }
        else if (keyState == KEYSTATE_EXIT_CONFIGURATION)
        {
            requiredAction = 2;     // Logout & Clear Timer
        }
        else if ((keyState & (KEYSTATE_RIGHT | KEYSTATE_LEFT | KEYSTATE_UP | KEYSTATE_DOWN)) != 0)
        {
            requiredAction = 1;     // Clear Timer
        }
        else
        {
            // increment timer
            LOCAL_logoutTimer.milliseconds += HMI_UPDATE_RATE_MILLISEC;

            if (LOCAL_logoutTimer.milliseconds >= 1000)
            {
                LOCAL_logoutTimer.milliseconds -= 1000;
                LOCAL_logoutTimer.seconds ++;
            }


            // check for timeout
            if (LOCAL_logoutTimer.seconds >= HMI_CONFIGMODE_TIMEOUT_SEC)
            {
                requiredAction = 2;     // Logout & Clear Timer
            }
            else
            {
                requiredAction = 0;
            }
        }
    }

    if (requiredAction >= 2)    // Logout & Clear Timer
    {
        AdjusterSetARMlevel(ARM_ACCESS_LOGOUT, (unsigned char*)NULL);   // No password required for Logout
    }

    if (requiredAction >= 1)     // Clear Timer
    {
        ResetInactiveUserTimeout();
    }
}

void ResetInactiveUserTimeout(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT

    // Clear Timer  on either HMI will clear the timer on both HMI's
    CoordinatorLocal_1.logoutTimer.seconds = 0;
    CoordinatorLocal_1.logoutTimer.milliseconds = 0;

    CoordinatorLocal_2.logoutTimer.seconds = 0;
    CoordinatorLocal_2.logoutTimer.milliseconds = 0;

#else

    logoutTimer.seconds = 0;
    logoutTimer.milliseconds = 0;

#endif
}



#endif

#ifdef AZ200_SPECIAL_FEATURES_HMI
void modifyDisplayForAutocal(void)
{
    T_UNIT const SLOW *pCal = GetSubsystemPtr((T_SUBSYSTEM_IDX)CAL_IDX);
    TUSIGN8 CalibrationType;
    TBOOL SchedCalActive = ((T_O2_CALIBRATION*)pCal)->IsScheduledCalibrationActiveSRV(&CalibrationType);

    if ((SchedCalActive) && (!LOCAL_prvSchedCalActive))
    {
        // if scheduled calibration has just been activated then...
        // ...(1) keep a copy of the frame currently being displayed
        // ...(1.1) frame pointer
        LOCAL_savedFrame_cal = FrameTableGetCurrentFrame();
        // ...(1.2) cursor position
        LOCAL_savedCursor_cal = FrameTableGetCursorPosition();
        // ...(1.3) autoscroll setting
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        LOCAL_savedAutoscroll_cal = FrameTableIsAutoscrollEnabled();
        #endif

        // ...(2) disable autoscroll
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        FrameTableDisableAutoscroll();
        #endif

        // ...(3) force display to the Calibration Frame
        (void)FrameTableSetCurrentFrame(FrameTableGetCalFrameAddress(CalibrationType), 0, IDF_SPECIAL_NONE);
    }
    else if ((LOCAL_prvSchedCalActive) && (!SchedCalActive))
    {
        // if scheduled calibration has just been de-activated then...
        // ...(1) force display back to original frame
        (void)FrameTableSetCurrentFrame(LOCAL_savedFrame_cal, LOCAL_savedCursor_cal, IDF_SPECIAL_NONE);

        // ...(2) restore autoscroll
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        if (LOCAL_savedAutoscroll_cal)
        {
            FrameTableEnableAutoscroll();
        }
        else
        {
            FrameTableDisableAutoscroll();
        }
        #endif
    }
    else
    {
        asm("nop");
    }

    LOCAL_prvSchedCalActive = SchedCalActive;
}
#endif

void HmiCoordinatorResetPreviousKeyState(void)
{
    LOCAL_previousKeyState = KEYSTATE_NONE;
}
