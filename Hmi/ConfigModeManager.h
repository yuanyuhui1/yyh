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

/****** ConfigModeManager.h ******/

#ifndef _CONFIGMODEMANAGER_H
#define _CONFIGMODEMANAGER_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif


/*!
 \brief		Returns the Configuration mode state
 \author	Roger Arnold
 \return	0 Configuration Mode inactive, 1 Configuration mode active.
 \test		No details available
*/
TUSIGN8 ConfigModeManagerIsConfigurationMode(void);

/*!
 \brief		Sets the Configuration mode state
 \author	Roger Arnold
 \param		newMode - 0 deactivate Configuration Mode, 1 activate Configuration Mode, other values do nothing
 \return	new configuration mode 0 Configuration Mode inactive, 1 Configuration mode active.
 \test		No details available
*/
TUSIGN8 ConfigModeManagerSetConfigurationMode(TUSIGN8 newMode);

/*!
 \brief		Monitors the key presses, forces an 'Exit Configuration' keyboard command if no keys have been pressed for the timeout period
 \author	Roger Arnold
 \param		pKeyState - pointer to current keyboard command (will be changed to 'Exit Configuration' command if timeout has occured)
 \return	none
 \test		No details available
*/
void ConfigModeManagerMonitorKeyboard(TUSIGN8 *pKeyState);
void ConfigModeManagerResetConfigurationTimeout(void);

#ifdef HMI_SUPPORTS_SERVICEPORT
/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the ConfigModeManager local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void ConfigModeManagerSetLocalDataPointer(TUSIGN8 newInstance);
#endif
#endif
