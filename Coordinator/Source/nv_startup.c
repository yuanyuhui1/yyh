//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
//#include "..\..\t_data_obj\interface\t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
//#include "..\..\nv_mem\file\interface\file.h"
//#include "..\..\nv_mem\source\filelist.h"
#include "nv_mem/nv_service/Interface/nv_service.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Coordinator/Interface/nv_startup.h"


//lint -e641 Lawrence 2013-04-18
//lint -e655 Lawrence 2013-04-18
//--------------------------------------------------------------------------------------------------
/*!
 \brief     With this function Format operation is forced

 \author    Roberto Colombo
 \date      2008-06-13
 \param     -
 \return    -
 \warning
 \test
test-date: 2013-04-25
\n by: Lawrence Shi
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TBOOL ButtonPressedAndKeyPresent(void)
{
    TUSIGN16 u = 0;
    if(PUSH_BUTTON_WRITE_PROTECT_IS_ON)
    {
        return  (TBOOL)(u>=100);
    }
    //check if Button is pressed
#ifdef NDEBUG
    //lint -e{960} Ashwin Herur R 2009-07-13 OK
    for( u=0; (u<100) && (DIP_SWITCH_2_IS_OFF && DIP_SWITCH_6_IS_ON); u++);
#else
    //lint -e{960} Ashwin Herur R 2009-07-13 OK
    //lint -e{722} Ashwin Herur R 2009-07-13 OK
    for( u=0; (u<100) && (DIP_SWITCH_2_IS_OFF && DIP_SWITCH_6_IS_ON); u++);
#endif
    //lint -e{722} Ashwin Herur R 2009-07-13 OK
    return (TBOOL)(u>=100);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief With this function all Storage-classes will be formated. Non-volatile data will be stored
        into the nv-ram

 \author    Heiko Kresse
 \date      2005-02-03
 \param     -format
 \return    -
 \warning
 \test
         test-date    : 2005-02-21
         by           : Heiko Kresse
         environment  : sight check and and Part of the StartupNVSubsystem()-Test
                        checked with PD30
         result       : OK
 \test   Lint level 3 OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS FormatAllData_intern(const TUSIGN16 format)
{
    TUSIGN16 result;
    T_NV_DIAGNOSIS saveResult;
    T_SUBSYSTEM_IDX subsysIdx;
    const T_UNIT SLOW* unitPtr;
    
    //Load rom data into ram
    //Get pointer subystem from pointer list
    //Call loadRomDefualt for each dataclass
    //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
    for(subsysIdx=0; subsysIdx<MEMBER_IDX_NUM_ELEMENTS; subsysIdx++)
    {
        unitPtr=GetSubsystemPtr(subsysIdx);
        (void)unitPtr->LoadRomDefaults(unitPtr, ALL_DATACLASSES);
    }
    
    //Reset or init service pages
    //This operation in not done into normal init any more
    //A manually operation is needed
    result = Format_NVSERVICE(FIDX_CYCLIC_COMMON_REPLACE);
    result |= Format_NVSERVICE(FIDX_STATIC_COMMON_REPLACE);
    result |= Format_NVSERVICE(FIDX_CYCLIC_CB);
    result |= Format_NVSERVICE(FIDX_STATIC_CB);
    if(format == FORMAT_ALL_DATA)
    {
        result |= Format_NVSERVICE(FIDX_CALIBRATION);
    }
    
    //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
    if(result == OK)
    {
        //Reset nv diangosis
        //Save rom data loaded above
        ClearDiagnosisNV(ALL_FILES);
        saveResult = Format(FIDX_CYCLIC_COMMON_REPLACE,NV_SYNC);
        saveResult |= Format(FIDX_STATIC_COMMON_REPLACE,NV_SYNC);
        saveResult |= Format(FIDX_CYCLIC_CB,NV_SYNC);
        saveResult |= Format(FIDX_STATIC_CB,NV_SYNC);
        if(format == FORMAT_ALL_DATA)
        {
            saveResult |= Format(FIDX_CALIBRATION,NV_SYNC);
        }
        /// did not load calibration data automatically
    }
    else
    {   //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
        saveResult = NV_ERROR;
    }
    
    return saveResult;
}


//-------------------------------------------------------------------------------------------------
/*
 \brief    things that must be initialized inside a task. Startup will be called from the MainTask.
 \author   Roberto Colombo
 \date     2008-12-01
 \param
 \warning
 \test
         test-date    : 2005-04-15
         by           : Heiko Kresse
         environment  : sight check and complete code-coverage
                        checked with PD30
         result       : OK
 \test   Lint level 3 OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StartupNVSubsystem(void)
{
    T_NV_DIAGNOSIS  nvResult;
    TUSIGN32 coordinatorExternalDiagnosis;
    TBOOL format = eFALSE;


    //First initialize NV_MEM
    //Initialize chip-handler
    //Initialize nv-service
    //Initialize file-system
    //Load main into ram, if main defect load backup, if both defect -EEPROM defect
    nvResult = InitializeNV();

    //Check if button is pressed
    //If both buttons are pressed or dip switch 6 is enabled, nv will be formatted
    format = ButtonPressedAndKeyPresent();
    if(format)
    {
        // nvResult = FormatAllData_intern(FORMAT_CONFIG_DATA);
        nvResult = FormatAllData_intern(FORMAT_ALL_DATA);
    }


    //If nv initializing, or formatting, is no ok, a cb nv failure will be set
    //Alarm is not set immediatly. It will be recognized into coordinator diagnosis update function
    //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
    if(nvResult == NV_ERROR)
    {
        T_SUBSYSTEM_IDX subsysIdx;
        const T_UNIT SLOW* unitPtr;

        //Load rom data into ram
        //Get pointer subystem from pointer list
        //Call loadRomDefualt for each dataclass
              //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
        for(subsysIdx=0; subsysIdx<MEMBER_IDX_NUM_ELEMENTS; subsysIdx++)
        {
            unitPtr=GetSubsystemPtr(subsysIdx);
            (void)unitPtr->LoadRomDefaults(unitPtr, ALL_DATACLASSES);
        }
        //lint -e{539} Lawrence 2013-04-18
        (void)coordinator_Get(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorExternalDiagnosis);
        coordinatorExternalDiagnosis |= (1<<CDT_ALARM_CB_NV_ERROR);
        (void)coordinator_Put(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorExternalDiagnosis);
    }
    
    //todo, user shall change the FE related function and initialize to realize NV replace function
    //(void)InitializeNV_REPLACE();//init NV Replace

#ifdef START_UP_NV_REPLACE
    TUSIGN16 result;
    TUSIGN8 sensorInterfaceLocalDiagnosis;
    TUSIGN32 hsCheckTime;

    //Check hs line
    //If hs is low, fe is starting up
    //Max time cb waits is 10s to avoid an hs corruption
    //If fe is not present, hs line will be detected as high and 10s waiting is jumped
       //lint -e{732} Ashwin Herur R 2009-07-13 Ok Only positive time is considered
    hsCheckTime = GetTickCount_RTOS_TASK();
    while(frontEndInterface.CheckFEreadySRV() == eFALSE)
    {       
        //lint -e{737} Ashwin Herur R 2009-07-13 Ok Only positive time is considered      
        if((GetTickCount_RTOS_TASK()-hsCheckTime)>SENSOR_INTERFACE_WAIT_FE_REBOOT) 
          break;
    };


    //Init Trasducer Id
    //Used for a potential service page writing
    result = frontEndInterface.ReadFrontEndObjectsSRV(SENSOR_INTERFACE_FE_ADDRESS_DEFAULT,SENSOR_INTERFACE_ARM_NV_READ);


    //If fe first reading is no ok, a fe failure will be set
    //Alarm is not set immediatly. It will be recognized into coordinator diagnosis update function
       //lint -e{641} Ashwin Herur R 2009-07-10 Converting enum to int accepted
    if(result == SENSOR_INTERFACE_COMMUNICATION_ERR)
    {
        (void)sensor_Interface_Get(SENSOR_INTERFACE_IDX_localDiagnosis,WHOLE_OBJECT,&sensorInterfaceLocalDiagnosis);
        sensorInterfaceLocalDiagnosis |= (1<<SENSOR_INTERFACE_ALARM_FE_FAILED);
        (void)sensor_Interface_Put(SENSOR_INTERFACE_IDX_localDiagnosis,WHOLE_OBJECT,&sensorInterfaceLocalDiagnosis);
    }
    else
    {
        TUSIGN8 forcedReplace = COORDINATOR_FORCE_REPLACE_NONE;
        
        //Load and check service pages
        //Perform numal replace if required
        //Perform forced replace if set by production
        (void)InitializeNV_REPLACE();

        //Reset Forced Replace
        coordinator_Put(COORDINATOR_IDX_forceReplace,WHOLE_OBJECT,&forcedReplace);
    }
#endif
    return nvResult;
}