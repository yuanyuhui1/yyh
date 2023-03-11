//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Generic
 Subsystem				HMI
 Description			Configuration Mode Manager of the HMI subsystem. Is responsible for monitoring the configuration mode and
 						ensuring that the HMI system is not left in configuration mode if user activity has ceased.

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003

/****** ConfigModeManager.c ******/

#include "IncludeList_Implementation.h"

#include <intrinsics.h>
#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif

#include "keystateDefinitions.h"
#include "ConfigModeManager.h"

#ifdef AZ200_SPECIAL_FEATURES_HMI
#include "FrameTableDefinitions.h"
#include "FrameTable.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "../../O2_Calibration/Interface/O2_Calibration.h"
#include "../../Device/Interface/Device.h"
#include "Coordinator/Interface/subsystem_idx.h"

#endif

// private variables

typedef struct
{
	TUSIGN16	seconds;
	TUSIGN16	milliseconds;
}CONFIG_MODE_TIMER;

#ifdef HMI_SUPPORTS_SERVICEPORT

typedef struct
{
	CONFIG_MODE_TIMER   configModeTimer;
    TUSIGN8		        configMode;
}CONFIGMODEMANAGER_LOCAL_TYPE;

#pragma diag_suppress=Pm022
#pragma diag_suppress=Pm023
CONFIGMODEMANAGER_LOCAL_TYPE    ConfigModeLocal_1 = {
     0      // TUSIGN16 configModeTimer.seconds
    ,0      // TUSIGN16	configModeTimer.milliseconds
    ,0      // TUSIGN8  configMode
};

CONFIGMODEMANAGER_LOCAL_TYPE    ConfigModeLocal_2 = {
     0      // TUSIGN16 configModeTimer.seconds
    ,0      // TUSIGN16	configModeTimer.milliseconds
    ,0      // TUSIGN8  configMode
};
#pragma diag_default=Pm023
#pragma diag_default=Pm022

CONFIGMODEMANAGER_LOCAL_TYPE *pConfigModeLocal = &ConfigModeLocal_1;

#define LOCAL_Timer     pConfigModeLocal->configModeTimer
#define LOCAL_Mode      pConfigModeLocal->configMode

#else

#define LOCAL_Timer     configModeTimer
#define LOCAL_Mode      configMode

CONFIG_MODE_TIMER       configModeTimer = {0,0};
TUSIGN8                 configMode = 0;

#endif

#ifdef HMI_SUPPORTS_SERVICEPORT
// No restriction on the optimization of this function
void ConfigModeManagerSetLocalDataPointer(TUSIGN8 newInstance)
{
    if      (newInstance == 1)  {pConfigModeLocal = &ConfigModeLocal_1;}
    else if (newInstance == 2)  {pConfigModeLocal = &ConfigModeLocal_2;}
    else                        {VIP_ASSERT(0);}
}
#endif

/*!
 \brief		Returns the Configuration mode state
 \author	Roger Arnold
 \return	0 Configuration Mode inactive, 1 Configuration mode active.
 \test		No details available
*/
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Resets the  local "configMode"
 \author Roger Arnold
 \date   2005-04-14
 \param  none
 \return configMode
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  No module test for this simple function
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN8 ConfigModeManagerIsConfigurationMode(void)
{
	return	LOCAL_Mode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Sets the  local "configMode"
         Handles resetting of the configuration mode timeout if required
 \author Roger Arnold
 \date   2005-04-14
 \param  configMode - 0 deactivate Configuration Mode, 1 activate Configuration Mode, other values do nothing
 \return new configuration mode 0=Configuration Mode inactive, 1=Configuration mode active.
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - ConfigModeManager_ModuleTest
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
TUSIGN8 ConfigModeManagerSetConfigurationMode(TUSIGN8 newMode)
{
	if (newMode == 0)
	{
		if (LOCAL_Mode != 0)
		{
			LOCAL_Timer.seconds = 0;		// timer initial value
			LOCAL_Timer.milliseconds = 0;
		}
		LOCAL_Mode = 0;
	}
	else if (newMode == 1)
	{
		LOCAL_Mode = 1;
	}
    else
    {
        asm("nop");
    }

    #ifdef AZ200_SPECIAL_FEATURES_HMI
        #ifdef HMI_SUPPORTS_SERVICEPORT
            TBOOL configModeAlarmState = ((ConfigModeLocal_2.configMode == 1) || (ConfigModeLocal_1.configMode == 1)) ? eTRUE : eFALSE;
        #else
            TBOOL configModeAlarmState = (configMode == 1) ? eTRUE : eFALSE;
        #endif
        ((const T_DEVICE*)GetSubsystemPtr((T_SUBSYSTEM_IDX)DEV_IDX))->SetConfigModeAlarm(configModeAlarmState);
    #endif

	return LOCAL_Mode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Monitors the key presses,
         forces an 'Exit Configuration' keyboard command if no keys have been pressed for the timeout period
 \author Roger Arnold
 \date   2005-04-14
 \param  pKeyState - pointer to current keyboard command (will be changed to 'Exit Configuration' command if timeout has occured)
 \return none
 \warning
 \test
         test-date    :  2005-04-14
         by           :  Roger Arnold
         environment  :  CSPY - ConfigModeManager_ModuleTest
         result       :  OK
 \test   Lint level 3 :  OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void ConfigModeManagerMonitorKeyboard(TUSIGN8 *pKeyState)
{
#ifndef HMI_CONFIG_MODE_TIMEOUT_DISABLE
    if ((*pKeyState == KEYSTATE_NONE) && (LOCAL_Mode == 1))
    {
        LOCAL_Timer.milliseconds += HMI_UPDATE_RATE_MILLISEC;

        if (LOCAL_Timer.milliseconds >= 1000)
        {
            LOCAL_Timer.milliseconds -= 1000;
            LOCAL_Timer.seconds ++;
        }



        if (LOCAL_Timer.seconds >= HMI_CONFIGMODE_TIMEOUT_SEC)
        {
            *pKeyState = KEYSTATE_EXIT_CONFIGURATION;
        }
    }
    else
    {
        if (*pKeyState != KEYSTATE_NONE)
        {
            ConfigModeManagerResetConfigurationTimeout();
        }
    }

    #ifdef AZ200_SPECIAL_FEATURES_HMI

    if (((const T_O2_CALIBRATION*)(GetSubsystemPtr(CAL_IDX)))->IsCalibrationActiveSRV() == eTRUE)
    {
        ConfigModeManagerResetConfigurationTimeout();
    }
    #endif
}
#else
    // if HMI_CONFIG_MODE_TIMEOUT_DISABLE is defined (timeout disabled)
    // then..
    // pKeyState is not referenced        -e{715}
    // pKeyState could be const pointer   -e{818}
    // compileTimeDefinitions.h not used  -e{766}
    // keystateDefinitions.h not used     -e{766}
    //lint -e{715} -e{818} Roger Arnold 2005-1-27
}
#endif
    //lint -e{766} Roger Arnold 2005-1-27


void ConfigModeManagerResetConfigurationTimeout(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT

    // Timer reset will reset the timer on both HMI's
    ConfigModeLocal_1.configModeTimer.seconds = 0;
    ConfigModeLocal_1.configModeTimer.milliseconds = 0;

    ConfigModeLocal_2.configModeTimer.seconds = 0;
    ConfigModeLocal_2.configModeTimer.milliseconds = 0;

#else

    configModeTimer.seconds = 0;
    configModeTimer.milliseconds = 0;

#endif
}
