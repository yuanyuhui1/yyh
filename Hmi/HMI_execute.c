//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003

//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include <limits.h>

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
#include "Hmi/Interface/HMI.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "hmi_execute.h"


#include "PacketManager.h"
#include "hmiCoordinator.h"
#include "frameTableDefinitions.h"
#include "FrameTable.h"
#include "KeystateDefinitions.h"

#ifdef HMI_SUPPORTS_GRAPH_VIEW
#include "TrendView.h"
#endif

TBOOL   HMI_Processing = eFALSE;

//@SubGen put@ <METHOD>
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_GETLANGUAGEINDEX_SRV> \endif
 \brief  
 \param  language 
 \if @SubGen end@ <METHODHEADER HMI_GETLANGUAGEINDEX_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_GETLANGUAGEINDEX_SRV>
TUSIGN8 HMI_GetLanguageIndex_SRV_HMI(TUSIGN8 language)
//@SubGen end@<METHOD HMI_GETLANGUAGEINDEX_SRV>
{
    return FrameTableGetLanguageIndex(language);
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_INITIALISEGLOBALCONFIGCHANGECOUNTER_SRV> \endif
 \brief  
 \param  ss_Idx 
 \param  ob_Idx 
 \param  at_Idx 
 \if @SubGen end@ <METHODHEADER HMI_INITIALISEGLOBALCONFIGCHANGECOUNTER_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_INITIALISEGLOBALCONFIGCHANGECOUNTER_SRV>
void HMI_InitialiseGlobalConfigChangeCounter_SRV_HMI(TUSIGN16 ss_Idx, TUSIGN16 ob_Idx, TINT16 at_Idx)
//@SubGen end@<METHOD HMI_INITIALISEGLOBALCONFIGCHANGECOUNTER_SRV>
{
    #ifdef HMI_SUPPORTS_GLOBAL_CONFIG_CHANGE_COUNTER
    FrameTableInitialiseGlobalConfigChangeCounter(ss_Idx, ob_Idx, at_Idx);
    #endif
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_REINITIALISELOWLEVELDETECTION_SRV> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER HMI_REINITIALISELOWLEVELDETECTION_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_REINITIALISELOWLEVELDETECTION_SRV>
void HMI_ReInitialiseLowLevelDetection_SRV_HMI(void)
//@SubGen end@<METHOD HMI_REINITIALISELOWLEVELDETECTION_SRV>
{
    PacketManagerReInitialiseLowLevelDetection();
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_ALLOWREINIT_SRV> \endif
 \brief  
 \param  InitFlags 
 \if @SubGen end@ <METHODHEADER HMI_ALLOWREINIT_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_ALLOWREINIT_SRV>
void HMI_AllowReInit_SRV_HMI(TUSIGN8 InitFlags)
//@SubGen end@<METHOD HMI_ALLOWREINIT_SRV>
{
  HmiCoordinatorAllowReInit(InitFlags);
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_ISOPERATORVIEWACTIVE> \endif
 \brief  Returns eTRUE if HMI currently displaying an operator level frame
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_ISOPERATORVIEWACTIVE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_ISOPERATORVIEWACTIVE>
TBOOL HMI_IsOperatorViewActive_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_ISOPERATORVIEWACTIVE>
{
    return FrameTableIsOperatorFrameActive(instance);
}



//--------------------------------------------------------------------------------------------------
/*! \if @SubGen start@ <METHODHEADER HMI_EXE> \endif
 \brief  This is the high level HMI executive
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_EXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_EXE>
TBOOL HMI_EXE_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_EXE>
{
    TBOOL returnVal;

    if ((HMI_Processing) || (instance < 1) || (instance > 2 ))
    {
        returnVal = eFALSE;
    }
    else
    {
        HMI_Processing = eTRUE;
        HmiCoordinatorEXE(instance);
        HMI_Processing = eFALSE;
        returnVal = eTRUE;
    }
    return returnVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_FORCEDISPLAYTIMEOUTSRV> \endif
 \brief  Forces a timeout of the display update timer, thus forcing the active area of the display to be updated the next time HMI_EXE() is called.
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_FORCEDISPLAYTIMEOUTSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_FORCEDISPLAYTIMEOUTSRV>
void HMI_ForceDisplayTimeoutSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_FORCEDISPLAYTIMEOUTSRV>
{
    HmiCoordinatorForceDisplayTimeoutSRV(instance);
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_POWERONSRV> \endif
 \brief  Controls the power to the display hardware. Power will not be applied to the hardware until this function has been called
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_POWERONSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_POWERONSRV>
void HMI_PowerOnSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_POWERONSRV>
{
    HmiCoordinatorPowerOn(instance);
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_ALLOWIOACCESSSRV> \endif
 \brief  Allows the HMI to access the I/O. Only after HMI_PowerOnSRV() and HMI_AllowIOaccessSRV() have been called will the High Level HMI attempt to establish communication with the low level HMI.
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_ALLOWIOACCESSSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_ALLOWIOACCESSSRV>
void HMI_AllowIOaccessSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_ALLOWIOACCESSSRV>
{
    HmiCoordinatorSetAccess(instance, eTRUE);
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_DENYIOACCESSSRV> \endif
 \brief  Stops the HMI from using the I/O. If HMI_DenyIOaccessSRV() is called then the High Level HMI will assume that the low level HMI has been removed. This allows other subsystems to use the I/O.
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_DENYIOACCESSSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_DENYIOACCESSSRV>
void HMI_DenyIOaccessSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_DENYIOACCESSSRV>
{
    HmiCoordinatorSetAccess(instance, eFALSE);
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_TRANSMITINTERRUPTSRV> \endif
 \brief  Transmit to Low Level HMI Interrupt handler
 \if @SubGen end@ <METHODHEADER HMI_TRANSMITINTERRUPTSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_TRANSMITINTERRUPTSRV>
void HMI_TransmitInterruptSRV_HMI(void)
//@SubGen end@<METHOD HMI_TRANSMITINTERRUPTSRV>
{
    PacketManagerTransmitInterrupt();
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_RECEIVEINTERRUPTSRV> \endif
 \brief  Receive from Low Level HMI Interrupt handler
 \if @SubGen end@ <METHODHEADER HMI_RECEIVEINTERRUPTSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_RECEIVEINTERRUPTSRV>
void HMI_ReceiveInterruptSRV_HMI(void)
//@SubGen end@<METHOD HMI_RECEIVEINTERRUPTSRV>
{
    PacketManagerReceiveInterupt();
}


//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_ISIOACTIVESRV> \endif
 \brief  Returns the status of the communication between the high level HMI and low level HMI
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_ISIOACTIVESRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_ISIOACTIVESRV>
TBOOL HMI_IsIOActiveSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_ISIOACTIVESRV>
{
    return PacketManagerIsLowLevelActive(instance);
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_NV_INITCOMPLETESRV> \endif
 \brief  Tells the HMI that NV initialization is complete, returns sucess
 \if @SubGen end@ <METHODHEADER HMI_NV_INITCOMPLETESRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_NV_INITCOMPLETESRV>
TBOOL HMI_NV_InitCompleteSRV_HMI(void)
//@SubGen end@<METHOD HMI_NV_INITCOMPLETESRV>
{
    TBOOL   returnVal;

    if (HMI_Processing)
    {
        returnVal = eFALSE;
    }
    else
    {
        HMI_Processing = eTRUE;
        FrameTableNVInitComplete();
        HMI_Processing = eFALSE;
        returnVal = eTRUE;
    }
    return returnVal;
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_GETKEYPRESSSRV> \endif
 \brief  Returns the current keypress data
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_GETKEYPRESSSRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_GETKEYPRESSSRV>
TUSIGN8 HMI_GetKeypressSRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_GETKEYPRESSSRV>
{
    TUSIGN8 Keypress;
    switch (PacketManagerGetKeystate(instance))
    {
    case KEYSTATE_RIGHT:                    Keypress = HMI_RIGHT_KEYPRESS;      break;
    case KEYSTATE_LEFT:                     Keypress = HMI_LEFT_KEYPRESS;       break;
    case KEYSTATE_UP:                       Keypress = HMI_UP_KEYPRESS;         break;
    case KEYSTATE_DOWN:                     Keypress = HMI_DOWN_KEYPRESS;       break;

    #ifdef EDP300_SPECIAL_FEATURES_HMI
    case KEYSTATE_UP | KEYSTATE_RIGHT:      Keypress = HMI_FAST_UP_KEYPRESS;    break;
    case KEYSTATE_DOWN | KEYSTATE_RIGHT:    Keypress = HMI_FAST_DOWN_KEYPRESS;  break;
    #endif

    default:                Keypress = HMI_NO_KEYPRESS;     break;
    }
    return Keypress;
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_GET_LLHARDWARETYPE_SRV> \endif
 \brief  Returns the Low Level HMI Type
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_GET_LLHARDWARETYPE_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_GET_LLHARDWARETYPE_SRV>
TUSIGN8 HMI_Get_LLHardwareType_SRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_GET_LLHARDWARETYPE_SRV>
{
    return PacketManagerGetLLHardwareType(instance);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_GETAUTOSCROLLSTATE_SRV> \endif
 \brief  Returns eTRUE if autoscroll is active
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_GETAUTOSCROLLSTATE_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_GETAUTOSCROLLSTATE_SRV>
TBOOL HMI_GetAutoscrollState_SRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_GETAUTOSCROLLSTATE_SRV>
{
    return FrameTableGetAutoScrollState(instance);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_GETOPERATORPAGENUMBER_SRV> \endif
 \brief  Returns the number of the operator page being displayed
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_GETOPERATORPAGENUMBER_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_GETOPERATORPAGENUMBER_SRV>
TUSIGN8 HMI_GetOperatorPageNumber_SRV_HMI(TUSIGN8 instance)
//@SubGen end@<METHOD HMI_GETOPERATORPAGENUMBER_SRV>
{
    return FrameTableGetOperatorPageNumber(instance);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETSTARTUPFRAME_SRV> \endif
 \brief  Allows a remote subsystem to select the operator page & autoscroll state used at startup
 \param  autoscrollState 
 \param  operatorPage 
 \param  instance 
 \if @SubGen end@ <METHODHEADER HMI_SETSTARTUPFRAME_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETSTARTUPFRAME_SRV>
TBOOL HMI_SetStartupFrame_SRV_HMI(TBOOL autoscrollState, TUSIGN8 operatorPage, TUSIGN8 instance)
//@SubGen end@<METHOD HMI_SETSTARTUPFRAME_SRV>
{
    return FrameTableSetStartupFrame(instance, autoscrollState, operatorPage);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETOPERATORICON_SRV> \endif
 \brief  Select an icon to be displayed on the lower line of the operator  page
 \param  field 
 \param  iconID 
 \if @SubGen end@ <METHODHEADER HMI_SETOPERATORICON_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETOPERATORICON_SRV>
void HMI_SetOperatorIcon_SRV_HMI(TUSIGN8 field, TUSIGN8 iconID)
//@SubGen end@<METHOD HMI_SETOPERATORICON_SRV>
{
    PacketManagerSetOperatorIcon(field, iconID);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETOPERATORINFO_SRV> \endif
 \brief  
 \param  infoID 
 \if @SubGen end@ <METHODHEADER HMI_SETOPERATORINFO_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETOPERATORINFO_SRV>
void HMI_SetOperatorInfo_SRV_HMI(TUSIGN8 infoID)
//@SubGen end@<METHOD HMI_SETOPERATORINFO_SRV>
{
    PacketManagerSetOperatorInfo(infoID);
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETGRAPHPOINTER_SRV> \endif
 \brief  
 \param  frameNumber 
 \param  pGraph 
 \if @SubGen end@ <METHODHEADER HMI_SETGRAPHPOINTER_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETGRAPHPOINTER_SRV>
void HMI_SetGraphPointer_SRV_HMI(TUSIGN8 frameNumber, TUSIGN8* pGraph)
//@SubGen end@<METHOD HMI_SETGRAPHPOINTER_SRV>
{
    asm("nop");
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETGRAPHTIMEBASE_SRV> \endif
 \brief  
 \param  frameNumber 
 \param  timebase 
 \if @SubGen end@ <METHODHEADER HMI_SETGRAPHTIMEBASE_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETGRAPHTIMEBASE_SRV>
TBOOL HMI_SetGraphTimebase_SRV_HMI(TUSIGN8 frameNumber, TUSIGN8 timebase)
//@SubGen end@<METHOD HMI_SETGRAPHTIMEBASE_SRV>
{
    #ifdef HMI_SUPPORTS_GRAPH_VIEW
    (void)TrendViewStopSampling();
    return TrendViewSetGraphTimebase(frameNumber,timebase);
    #else
    return eFALSE;
    #endif
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_RESETCONFIGURATIONTIMEOUT> \endif
 \brief  Resets the configuration mode timeout (and inactive user timeout)
 \if @SubGen end@ <METHODHEADER HMI_RESETCONFIGURATIONTIMEOUT> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_RESETCONFIGURATIONTIMEOUT>
void HMI_resetConfigurationTimeout_HMI(void)
//@SubGen end@<METHOD HMI_RESETCONFIGURATIONTIMEOUT>
{
    HmiCoordinatorResetConfigurationTimeout();
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_TOGGLEBACKLIGHT_SRV> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER HMI_TOGGLEBACKLIGHT_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_TOGGLEBACKLIGHT_SRV>
void HMI_ToggleBacklight_SRV_HMI(void)
//@SubGen end@<METHOD HMI_TOGGLEBACKLIGHT_SRV>
{
    PacketManagerToggleBacklight();
}
//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETFUNCTIONKEYLOCK_SRV> \endif
 \brief  
 \param  lockState 
 \if @SubGen end@ <METHODHEADER HMI_SETFUNCTIONKEYLOCK_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETFUNCTIONKEYLOCK_SRV>
void HMI_SetFunctionKeyLock_SRV_HMI(TBOOL lockState)
//@SubGen end@<METHOD HMI_SETFUNCTIONKEYLOCK_SRV>
{
    #ifdef LEFT_RIGHT_LOCKOUT
    FrameTableSetFunctionKeyLock(lockState);
    #else
    asm("nop");
    #endif
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_SETPARAMETERDUMPFONT_SRV> \endif
 \brief  
 \param  font 
 \if @SubGen end@ <METHODHEADER HMI_SETPARAMETERDUMPFONT_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_SETPARAMETERDUMPFONT_SRV>
void HMI_SetParameterDumpFont_SRV_HMI(TUSIGN8 font)
//@SubGen end@<METHOD HMI_SETPARAMETERDUMPFONT_SRV>
{
    FrameTableSetParameterDumpFont(font);
}

//--------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HMI_PutPacketBuffer_SRV> \endif
 \brief  
 \param  systemIdx 
 \param  processCmd 
 \param  ptrArray 
 \param  arrayLength 
 \if @SubGen end@ <METHODHEADER HMI_PutPacketBuffer_SRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HMI_PutPacketBuffer_SRV>
TBOOL HMI_PutPacketBuffer_SRV_HMI(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength)
//@SubGen end@<METHOD HMI_PutPacketBuffer_SRV>
{
    return PacketManagerOtherSubsystemPutPacketBuffer(systemIdx, processCmd, ptrArray, arrayLength);
}

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//--------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//--------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/
