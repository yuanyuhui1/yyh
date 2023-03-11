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

/****** HmiCoordinator.h ******/

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

/*----------------------------------------------------------------------------------------------------------!
 \brief         Task body called from embos. Method does not return else task is terminated.
 \author        Roger Arnold
 \test          No details available
*/
void HmiCoordinatorEXE(TUSIGN8 instance);

/*----------------------------------------------------------------------------------------------------------!
 \brief         forces the display to be updated the next time the HMI is executed
 \author        Roger Arnold
 \test          No details available
*/
void HmiCoordinatorForceDisplayTimeoutSRV(TUSIGN8 instance);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Initializes the class. During initialization the internal task gets created.
 \author        Roger Arnold
 \param         none
 \return        none
 \test          No details available
*/
void HmiCoordinatorAllowReInit(TUSIGN8 InitFlags);

void HmiCoordinatorInitializeTasks(TUSIGN8 instance, TBOOL MainInit);
void HmiCoordinatorInitializeHardware(void);
void HmiCoordinatorInitializeData(void);

/*----------------------------------------------------------------------------------------------------------!
 \brief         No
 \author        Stefan Tabelander
 \param         TUSIGN8 instance (1 - standard instance, 2 - Service port instance
 \return        none
 \test          No details available
*/
void HmiCoordinatorPowerOn(TUSIGN8 instance);

/*----------------------------------------------------------------------------------------------------------!
 \brief         Allows / Denies HMI access to the I/O
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - standard instance, 2 - Service port instance
                TBOOL AllowState (eFALSE - deny access, eTRUE - allow access)
 \return        none
 \test          No details available
*/
void HmiCoordinatorSetAccess(TUSIGN8 instance, TBOOL AllowState);

TUSIGN8 HmiCoordinatorGetInstance(void);
void HmiCoordinatorResetConfigurationTimeout(void);

#ifdef HMI_SUPPORTS_SERVICEPORT
/*----------------------------------------------------------------------------------------------------------!
 \brief         Sets the HMI coordinator local data pointer to the appropriate instance
 \author        Roger Arnold
 \param         TUSIGN8 instance (1 - primary instance, 2 - Service port instance)
 \return        none
 \test          No details available
*/
void HmiCoordinatorSetLocalDataPointer(TUSIGN8 newInstance);
#endif

void HmiCoordinatorResetPreviousKeyState(void);
