//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description                    PacketManager of the HMI subsystem.
                                                Is responsible for managing the packetTable by adding command packets and retrieveing reply packets

 Remarks
 $Id  $
 </pre>diag_suppress
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003

/****** PacketManager.c ******/
#include <intrinsics.h>
#include <math.h>
#include <string.h>
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
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Interface/ServicePort_idx.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "Coordinator/Interface/Coordinator_idx.h"

#include "Hmi/Interface/HMI_idx.h"
#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "hmi_overload.h"
#include "FrameTableDefinitions.h"
#include "FrameTable.h"
#include "keystateDefinitions.h"

#include "IncludeList_Frames_1.h"

#include "FixedIconDefinitions.h"

#ifdef AZ200_SPECIAL_FEATURES_HMI
#include "../../O2_Calibration/Interface/o2_calibration.h"
#include "../../O2_Calibration/Interface/o2_calibration_idx.h"
#include "../../O2_Calibration/Source/O2_Calibration_PerformanceLog.h"
#include "../../RelayOutput/Interface/RelayOutput.h"
#include "../../RelayOutput/Interface/RelayOutput_idx.h"
#include "../../nvinterface/Interface/NVinterface.h"
#include "../../nvinterface/Interface/NVinterface_idx.h"
#include "../../Measurement/Interface/Measurement.h"
#include "../../Measurement/Interface/Measurement_idx.h"
#include "../../OvenControl/Interface/OvenControl.h"
#include "../../OvenControl/Interface/OvenControl_idx.h"
#include "../../Digital_IO/Interface/Digital_IO.h"
#include "../../Digital_IO/Interface/Digital_IO_idx.h"
#include "../../Device/Interface/Device_idx.h"
#include "nvInitIndeces.h"
#endif

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
#pragma diag_suppress=Pm071
#include "low_level\core\data_handling.h"
#pragma diag_default=Pm071
#endif

#pragma diag_suppress=Pm071
#include "low_level\core\Driver.h"
#pragma diag_default=Pm071

#if !defined(HMIGEN_RELEASE_VERSION) || !defined(HMIGEN_MAJOR_VERSION) || !defined(HMIGEN_MINOR_VERSION) || !defined(HMIGEN_CONFIG_VERSION) || (HMIGEN_MAJOR_VERSION < 12) || ((HMIGEN_MAJOR_VERSION == 12) && (HMIGEN_MINOR_VERSION < 2))
    #include "layer2.h"
#else
    #include "IncludeList_layer2.h"
#endif

#include "Adjuster.h"
//#include "Enums.h"
#include "writeProtect.h"

#include "PacketManager.h"
#include "PacketManagerLocalData.h"
#include "PacketManagerBuilder.h"
#include "PacketManagerBuilder_s.h"
#include "PacketManagerBuilderServices.h"

TBOOL gHotHMI_ReinitializeWaveformFlag = eFALSE;

#ifdef HMI_SUPPORTS_SECURITY_ACCESS
//const TUSIGN8 SecurityIcon[12] = {0x18,0x3C,0x66,0x66,0x3C,0x18,0x18,0x18,0xF8,0x38,0xF8,0x18};
#endif

const TUSIGN8* operatorPageData[6][5] =
{
    //  display Mode                    1st Line                            2nd Line                            3rd line                            Bargraph
    /* page 1 */
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     },
    
    /* page 2 */
    #ifdef SECOND_OPERATOR_PAGE
    {   &SFD_MULTI_OP.displayMode_2,    &SFD_MULTI_OP.mainOperatorView_2_1, &SFD_MULTI_OP.mainOperatorView_2_2, &SFD_MULTI_OP.mainOperatorView_2_3, &SFD_MULTI_OP.mainOperatorView_2_bargraph   },
    #else
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     },
    #endif
    
    /* page 3 */
    #ifdef THIRD_OPERATOR_PAGE
    {   &SFD_MULTI_OP.displayMode_3,    &SFD_MULTI_OP.mainOperatorView_3_1, &SFD_MULTI_OP.mainOperatorView_3_2, &SFD_MULTI_OP.mainOperatorView_3_3, &SFD_MULTI_OP.mainOperatorView_3_bargraph   },
    #else
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     },
    #endif
    
    /* page 4 */
    #ifdef FOURTH_OPERATOR_PAGE
    {   &SFD_MULTI_OP.displayMode_4,    &SFD_MULTI_OP.mainOperatorView_4_1, &SFD_MULTI_OP.mainOperatorView_4_2, &SFD_MULTI_OP.mainOperatorView_4_3, &SFD_MULTI_OP.mainOperatorView_4_bargraph   },
    #else
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     },
    #endif
    
    /* page 5 */
    #ifdef FIFTH_OPERATOR_PAGE
    {   &SFD_MULTI_OP.displayMode_5,    &SFD_MULTI_OP.mainOperatorView_5_1, &SFD_MULTI_OP.mainOperatorView_5_2, &SFD_MULTI_OP.mainOperatorView_5_3, &SFD_MULTI_OP.mainOperatorView_5_bargraph   },
    #else
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     },
    #endif
    
    /* page 6 */
    #ifdef SIXTH_OPERATOR_PAGE
    {   &SFD_MULTI_OP.displayMode_6,    &SFD_MULTI_OP.mainOperatorView_6_1, &SFD_MULTI_OP.mainOperatorView_6_2, &SFD_MULTI_OP.mainOperatorView_6_3, &SFD_MULTI_OP.mainOperatorView_6_bargraph,  }
    #else
    {   &SFD_SIMPLE.displayMode_1,      &SFD_SIMPLE.mainOperatorView_1_1,   &SFD_SIMPLE.mainOperatorView_1_2,   &SFD_SIMPLE.mainOperatorView_1_3,   &SFD_SIMPLE.mainOperatorView_1_bargraph     }
    #endif
};

TBOOL readSwitchesIssued_1 = eFALSE;
#ifdef HMI_SUPPORTS_SERVICEPORT
TBOOL readSwitchesIssued_2 = eFALSE;
#endif

        // private methods
TUSIGN8 BuildErrorResponse(void);
TUSIGN8 BuildV2Init(TUSIGN8 contrast, TUSIGN8 lastPacket);
#ifdef TYPE_1_IMPLEMENTATION
TUSIGN8 BuildLowLevelInfo(TUSIGN8 lastPacket);
#endif

TUSIGN8 AddChecksum(TUSIGN8 checksumPosn);
TUSIGN8 AddPacketToBuffer(TUSIGN8 bytesToAdd, TUSIGN8 lastPacket);

//TUSIGN8 const *GetEngUnits(T_SUBSYSTEM_IDX subsysIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx, TUSIGN8 type);

        // private variables
#ifdef HMI_SUPPORTS_GRAPH_VIEW
TUSIGN8 GetGraphViewFrameNumber(void);
#endif

#ifdef HMI_SUPPORTS_DISPLAY_MODE_4x9
TBOOL TestDisplayMode_4x9(TUSIGN8 frameNumber);
#endif

#if defined(COMMON_TOP_WORKS_NEW_FEATURES) && defined(TYPE_1_IMPLEMENTATION)
static TUSIGN8  rotation = HMI_ROTATION_0;
TUSIGN8 BuildRotateDisplay(TUSIGN8 rotationRequired);
#endif


#ifdef HMI_SUPPORTS_SERVICEPORT

#define SERVICEPORT_RXBUFFER_SIZE   128
TBOOL   ServicePortConnected = eFALSE;
TBOOL   CommsInitReqd = eFALSE;
TBOOL   ServicePortTransmissionComplete = eFALSE;
TUSIGN8 SP_ReceiveBuffer[SERVICEPORT_RXBUFFER_SIZE];

static  TUSIGN8 SP_ReceiverWaiting;
static  TUSIGN8 SP_TransmitterBusy;
static  TUSIGN8 SP_RxCount;
static  TUSIGN8 SP_RxByte[15];
static  TBOOL   SP_CommsInitReqd;
static  TBOOL   SP_ServicePortConnected;
static  TBOOL   SP_ServicePortTransmissionComplete;

#pragma diag_suppress=Pm023
PACKETMANAGER_LOCAL_TYPE PacketManagerLocal_1 =
{
     NULL                           // FRAME_FORMAT_TYPE const *pPrevDisplayFrame = NULL;
    ,0                              // TUSIGN8                 prevEditEnable = 0;
    ,0                              // TUSIGN8                 packetBufferUsed = 0;
    ,eFALSE                         // TBOOL                   packetBufferPreparing = 0;
    ,0                              // TUSIGN8                 lastFunctionCode = 0;
    ,0                              // TUSIGN8                 SwitchErrorCount = 0;
    ,0                              // TUSIGN8                 TransmitterBusy = 0;
    ,0                              // TUSIGN8                 ReceiverWaiting = 0;
    ,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}// TUSIGN8                 RxByte[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    ,0                              // TUSIGN8                 LowLevelHMIFitted = 0/*1*/;
    ,eTRUE                          // TBOOL                   prvHMIcanAccessIO = eTRUE;
    ,eFALSE                         // TBOOL                   newCommsMode = eFALSE;
    ,0                              // TUSIGN8                 powerSwitchTimer = 0;
    ,0                              // TUSIGN8                 RxCount = 0;
    ,50                             // TUSIGN8                 DisplayDefaultContrast = 50;
    ,eFALSE                         // TBOOL                   Asian = eFALSE;
    ,eTRUE                          // TBOOL                   PrvAsian = eTRUE;
    ,0                              // TUSIGN8                 AsianCharacterSet = 0;
    #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
    ,0                              // TUSIGN8                 AsianBlockVersion = 0; (Not supported)
    #endif
    ,0                              // TUSIGN8                 flashState = 0;
    ,101                            // TUSIGN8                 last_contrast = 101;
    ,0xFF                           // TUSIGN8                 last_displayMode = 0xFF;
    ,0                              // TUSIGN8                 DisplayHardwareRevision = 0;
    ,0                              // TUSIGN8                 DisplaySoftwareRevision = 0;
    ,eFALSE                         // TBOOL                   overrangeValue_EditNotAllowed = eFALSE;
    ,powerUpState_InitialCondition  // POWERUP_STATE           powerUpState = powerUpState_InitialCondition;
    ,eFALSE                         // TBOOL                   displayAutoscrollIcon;
    ,0                              // TUSIGN8                 Field_3_Icon;
    ,0                              // TUSIGN8                 Field_5_Icon;
    ,0                              // TUSIGN8                 Field_7_Icon;
    #ifdef EDP300_SPECIAL_FEATURES_HMI
    ,0                              // TUSIGN8                 Field_9_Icon;
    #endif

    ,eFALSE                         // TBOOL                   displayWriteProtectIcon;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    ,eFALSE                         // TBOOL                   prvBlankHistory;
    ,0                              // TUSIGN8                 infoMessageID;
    ,0                              // TUSIGN16                infoMessageTimer;
    #endif
    #ifdef AZ200_SPECIAL_FEATURES_HMI
    ,eFALSE                         // TBOOL                   autoExit;
    #endif
    ,eFALSE                         // TBOOL                   COMMS_V2_INIT_reqd;
    ,eFALSE                         // TBOOL                   LOW_LEVEL_INFO_reqd;

//    #ifdef HMI_SUPPORTS_GRAPH_VIEW
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page1;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page2;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page3;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page4;
//    #endif
};

PACKETMANAGER_LOCAL_TYPE PacketManagerLocal_2 =
{
     NULL                           // FRAME_FORMAT_TYPE const *pPrevDisplayFrame = NULL;
    ,0                              // TUSIGN8                 prevEditEnable = 0;
    ,0                              // TUSIGN8                 packetBufferUsed = 0;
    ,eFALSE                         // TBOOL                   packetBufferPreparing = 0;
    ,0                              // TUSIGN8                 lastFunctionCode = 0;
    ,0                              // TUSIGN8                 SwitchErrorCount = 0;
    ,0                              // TUSIGN8                 TransmitterBusy = 0;
    ,0                              // TUSIGN8                 ReceiverWaiting = 0;
    ,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}// TUSIGN8                 RxByte[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    ,0                              // TUSIGN8                 LowLevelHMIFitted = 0/*1*/;
    ,eTRUE                          // TBOOL                   prvHMIcanAccessIO = eTRUE;
    ,eTRUE                          // TBOOL                   newCommsMode = eTRUE;
    ,0                              // TUSIGN8                 powerSwitchTimer = 0;
    ,0                              // TUSIGN8                 RxCount = 0;
    ,50                             // TUSIGN8                 DisplayDefaultContrast = 50;
    ,eFALSE                         // TBOOL                   Asian = eFALSE;
    ,eTRUE                          // TBOOL                   PrvAsian = eTRUE;
    ,0                              // TUSIGN8                 AsianCharacterSet = 0;
    #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
    ,0                              // TUSIGN8                 AsianBlockVersion = 0; (Not Supported)
    #endif
    ,0                              // TUSIGN8                 flashState = 0;
    ,101                            // TUSIGN8                 last_contrast = 101;
    ,0xFF                           // TUSIGN8                 last_displayMode = 0xFF;
    ,0                              // TUSIGN8                 DisplayHardwareRevision = 0;
    ,0                              // TUSIGN8                 DisplaySoftwareRevision = 0;
    ,eFALSE                         // TBOOL                   overrangeValue_EditNotAllowed = eFALSE;
    ,powerUpState_InitialCondition  // POWERUP_STATE           powerUpState = powerUpState_InitialCondition;
    ,eFALSE                         // TBOOL                   displayAutoscrollIcon;
    ,0                              // TUSIGN8                 Field_3_Icon;
    ,0                              // TUSIGN8                 Field_5_Icon;
    ,0                              // TUSIGN8                 Field_7_Icon;
    #ifdef EDP300_SPECIAL_FEATURES_HMI
    ,0                              // TUSIGN8                 Field_9_Icon;
    #endif

    ,eFALSE                         // TBOOL                   displayWriteProtectIcon;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    ,eFALSE                         // TBOOL                   prvBlankHistory;
    ,0                              // TUSIGN8                 infoMessageID;
    ,0                              // TUSIGN16                infoMessageTimer;
    #endif
    #ifdef AZ200_SPECIAL_FEATURES_HMI
    ,eFALSE                         // TBOOL                   autoExit;
    #endif
    ,eFALSE                         // TBOOL                   COMMS_V2_INIT_reqd;
    ,eFALSE                         // TBOOL                   LOW_LEVEL_INFO_reqd;
//    #ifdef HMI_SUPPORTS_GRAPH_VIEW
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page1;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page2;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page3;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page4;
//    #endif
};

#pragma diag_default=Pm023

extern PACKETMANAGER_LOCAL_TYPE  *pPacketManagerLocal = &PacketManagerLocal_1;

#else

#pragma diag_suppress=Pm023
PACKETMANAGER_LOCAL_TYPE PacketManagerLocal = {
     NULL                           // FRAME_FORMAT_TYPE const *pPrevDisplayFrame = NULL;
    ,0                              // TUSIGN8                 prevEditEnable = 0;
    ,0                              // TUSIGN8                 packetBufferUsed = 0;
    ,eFALSE                         // TBOOL                   packetBufferPreparing = 0;
    ,0                              // TUSIGN8                 lastFunctionCode = 0;
    ,0                              // TUSIGN8                 SwitchErrorCount = 0;
    ,0                              // TUSIGN8                 TransmitterBusy = 0;
    ,0                              // TUSIGN8                 ReceiverWaiting = 0;
    ,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}// TUSIGN8                 RxByte[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    ,0                              // TUSIGN8                 LowLevelHMIFitted = 0/*1*/;
    ,eTRUE                          // TBOOL                   prvHMIcanAccessIO = eTRUE;
    ,eFALSE                         // TBOOL                   newCommsMode = eFALSE;
    ,0                              // TUSIGN8                 powerSwitchTimer = 0;
    ,0                              // TUSIGN8                 RxCount = 0;
    ,50                             // TUSIGN8                 DisplayDefaultContrast = 50;
    ,eFALSE                         // TBOOL                   Asian = eFALSE;
    ,eTRUE                          // TBOOL                   PrvAsian = eTRUE;
    ,0                              // TUSIGN8                 AsianCharacterSet = 0;
    #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
    ,0                              // TUSIGN8                 AsianBlockVersion = 0; (Not Supported)
    #endif
    ,0                              // TUSIGN8                 flashState = 0;
    ,101                            // TUSIGN8                 last_contrast = 101;
    ,0xFF                           // TUSIGN8                 last_displayMode = 0xFF;
    ,0                              // TUSIGN8                 DisplayHardwareRevision = 0;
    ,0                              // TUSIGN8                 DisplaySoftwareRevision = 0;
    ,eFALSE                         // TBOOL                   overrangeValue_EditNotAllowed = eFALSE;
    ,powerUpState_InitialCondition  // POWERUP_STATE           powerUpState = powerUpState_InitialCondition;
    ,eFALSE                         // TBOOL                   displayAutoscrollIcon;
    ,0                              // TUSIGN8                 Field_3_Icon;
    ,0                              // TUSIGN8                 Field_5_Icon;
    ,0                              // TUSIGN8                 Field_7_Icon;
    #ifdef EDP300_SPECIAL_FEATURES_HMI
    ,0                              // TUSIGN8                 Field_9_Icon;
    #endif
    ,eFALSE                         // TBOOL                   displayWriteProtectIcon;
    #ifdef HMI_USES_COMMONDIAGNOSTICS
    ,eFALSE                         // TBOOL                   prvBlankHistory;
    ,0                              // TUSIGN8                 infoMessageID;
    ,0                              // TUSIGN16                infoMessageTimer;
    #endif
    #ifdef AZ200_SPECIAL_FEATURES_HMI
    ,eFALSE                         // TBOOL                   autoExit;
    #endif
    ,eFALSE                         // TBOOL                   COMMS_V2_INIT_reqd;
    ,eFALSE                         // TBOOL                   LOW_LEVEL_INFO_reqd;
//    #ifdef HMI_SUPPORTS_GRAPH_VIEW
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page1;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page2;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page3;
//    ,NULL                           // TUSIGN8                 *GraphPointer_Page4;
//    #endif
};
#pragma diag_default=Pm023

PACKETMANAGER_LOCAL_TYPE  *pPacketManagerLocal = &PacketManagerLocal;

#endif      /* #ifdef HMI_SUPPORTS_SERVICEPORT */


TBOOL ToggleBacklight = eFALSE;

#define LOWLEVEL_STARTUP_TIME   (HMI_DISPLAY_RATE_CYCLES * 3)

#if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)
static TUSIGN8 LowLevelStartupCount_LOCAL_HMI = LOWLEVEL_STARTUP_TIME;
#endif

#if defined(HMI_SUPPORTS_ASIAN) && defined(HMI_SUPPORTS_SERVICEPORT)
static TUSIGN8 LowLevelStartupCount_REMOTE_HMI = LOWLEVEL_STARTUP_TIME;
#endif




#ifdef HMI_SUPPORTS_SERVICEPORT
// No restriction on the optimization of this function
void PacketManagerSetLocalDataPointer(TUSIGN8 newInstance)
{
    if      (newInstance == 1)   {pPacketManagerLocal = &PacketManagerLocal_1;}
    else if (newInstance == 2)   {pPacketManagerLocal = &PacketManagerLocal_2;}
    else                         {VIP_ASSERT(0);}
}

TBOOL PacketManagerIsRemoteInstance(void)
{
    return (TBOOL)(pPacketManagerLocal == &PacketManagerLocal_2 ? eTRUE : eFALSE);
}
#endif

#ifdef HMI_SUPPORTS_SERVICEPORT
T_DATA_SEMAPHORE  Sema_HMI_REMOTE_INSTANCE;
#endif

//#ifdef HMI_SUPPORTS_GRAPH_VIEW
//void PacketManagerSetGraphPointer(TUSIGN8 frameNumber, TUSIGN8* pGraph)
//{
//    switch (frameNumber)
//    {
//    case 1:
//        #ifdef HMI_SUPPORTS_SERVICEPORT
//        PacketManagerLocal_1.GraphPointer_Page1 = pGraph;
//        PacketManagerLocal_2.GraphPointer_Page1 = pGraph;
//        #else
//        PacketManagerLocal.GraphPointer_Page1 = pGraph;
//        #endif
//        break;
//
//    case 2:
//        #ifdef HMI_SUPPORTS_SERVICEPORT
//        PacketManagerLocal_1.GraphPointer_Page2 = pGraph;
//        PacketManagerLocal_2.GraphPointer_Page2 = pGraph;
//        #else
//        PacketManagerLocal.GraphPointer_Page2 = pGraph;
//        #endif
//        break;
//
//    case 3:
//        #ifdef HMI_SUPPORTS_SERVICEPORT
//        PacketManagerLocal_1.GraphPointer_Page3 = pGraph;
//        PacketManagerLocal_2.GraphPointer_Page3 = pGraph;
//        #else
//        PacketManagerLocal.GraphPointer_Page3 = pGraph;
//        #endif
//        break;
//
//    case 4:
//        #ifdef HMI_SUPPORTS_SERVICEPORT
//        PacketManagerLocal_1.GraphPointer_Page4 = pGraph;
//        PacketManagerLocal_2.GraphPointer_Page4 = pGraph;
//        #else
//        PacketManagerLocal.GraphPointer_Page4 = pGraph;
//        #endif
//        break;
//
//    default:
//        asm("nop");
//        break;
//    }
//}
//#endif

// No restriction on the optimization of this function
TUSIGN8 PacketManagerGetLLHardwareType(TUSIGN8 instance)
{
    TUSIGN8 LowLevelType = HMI_HW_NOT_AVAILABLE;

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == HMI_LOCAL_INSTANCE)
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (PacketManagerLocal_1.LowLevelHMIFitted)
        {
            LowLevelType = PacketManagerLocal_1.DisplayHardwareRevision;
        }
        #else
        LowLevelType = HMI_HW_NOT_RECOGNISED;
        #endif
    }
    else if (instance == HMI_REMOTE_INSTANCE)
    {
        if (PacketManagerLocal_2.LowLevelHMIFitted)
        {
            LowLevelType = HMI_HW_REMOTE_HMI;
        }
    }
    else
    {
        VIP_ASSERT(0);
    }
#else

    #ifdef TYPE_1_IMPLEMENTATION
    if (PacketManagerLocal.LowLevelHMIFitted)
    {
        LowLevelType = PacketManagerLocal.DisplayHardwareRevision;
    }
    #else
    LowLevelType = HMI_HW_NOT_RECOGNISED;
    #endif

#endif
    return LowLevelType;
}

#ifdef TYPE_1_IMPLEMENTATION
TBOOL Type_1_HardwareSupportsExtendedCharacterSets(void)
{
    TBOOL ExtendedCharacterSetsSupported = eFALSE;

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (((PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_A_STD_SWITCH)       // HMI Size A with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_A_CAP_SWITCH)       //HMI Size A with capacitive switches with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_A_INDICATOR_ONLY)   //Size A indicator with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_B_STD_SWITCH)       //HMI Size B with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_B_HH_TERMINAL)      //Hand held terminal Size B with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_B_CAP_SWITCH)       //HMI Size B with capacitive switches with EEPROM
      || (PacketManagerLocal_1.DisplayHardwareRevision == HMI_HW_EEPROM_B_INDICATOR_ONLY))  //Size B indicator with EEPROM
     && (Type_1_SoftwareSupportsExtendedWriteString()))
#else
    if (((PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_A_STD_SWITCH)       // HMI Size A with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_A_CAP_SWITCH)       //HMI Size A with capacitive switches with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_A_INDICATOR_ONLY)   //Size A indicator with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_B_STD_SWITCH)       //HMI Size B with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_B_HH_TERMINAL)      //Hand held terminal Size B with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_B_CAP_SWITCH)       //HMI Size B with capacitive switches with EEPROM
      || (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_EEPROM_B_INDICATOR_ONLY))  //Size B indicator with EEPROM
     && (Type_1_SoftwareSupportsExtendedWriteString()))
#endif
    {
        ExtendedCharacterSetsSupported = eTRUE;
    }

    return ExtendedCharacterSetsSupported;
}

TBOOL Type_1_SoftwareSupportsExtendedWriteString(void)
{
    TBOOL ExtendedWriteStringSupported = eFALSE;

    const TUSIGN8 EARLIEST_VERSION_SUPPORTING_EXTENDED_CHARACTER_SET = 35;  // Low level s/w earlier than version 3.5 does not support the extended character set

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (PacketManagerLocal_1.DisplaySoftwareRevision >= EARLIEST_VERSION_SUPPORTING_EXTENDED_CHARACTER_SET)
#else
    if (PacketManagerLocal.DisplaySoftwareRevision >= EARLIEST_VERSION_SUPPORTING_EXTENDED_CHARACTER_SET)
#endif
    {
        ExtendedWriteStringSupported = eTRUE;
    }

    return ExtendedWriteStringSupported;
}

TBOOL Type_1_SupportsRotation(void)
{
    TBOOL RotationSupported = eFALSE;

    const TUSIGN8 EARLIEST_VERSION_SUPPORTING_ROTATION = 42;    // Low level s/w earlier than version 4.2 does not support rotation

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (PacketManagerLocal_1.DisplaySoftwareRevision >= EARLIEST_VERSION_SUPPORTING_ROTATION)
#else
    if (PacketManagerLocal.DisplaySoftwareRevision >= EARLIEST_VERSION_SUPPORTING_ROTATION)
#endif
    {
        RotationSupported = PacketManagerBuilderServices_IndicatorOnly();
    }
    return RotationSupported;
}


#endif


// No restriction on the optimization of this function
TUSIGN8 PacketManagerGetDefaultContrast(void)
{
    return LOCAL_DisplayDefaultContrast;
}

/*!
 \brief         Reads most recent error codes from HMI Interface class and prepares PacketManagerBuildImage() to react to those errors
 \author        Roger Arnold
 \param
 \return        0 no errors were present , 1 errors were present
 \test          No details available
*/
// No restriction on the optimization of this function
TUSIGN8 PacketManagerHandleErrors(TBOOL isSmartHMI)
{
    TUSIGN8 ReturnVal = 0;

#ifdef HMI_SUPPORTS_SERVICEPORT
//-------------------------|-----|
//HMI_SUPPORTS_SERVICEPORT | Yes |
//TYPE_1_IMPLEMENTATION    | x   |
//TYPE_2_IMPLEMENTATION    | x   |
//TYPE_3_IMPLEMENTATION    | x   |
//TYPE_4_IMPLEMENTATION    | x   |
//TYPE_5_IMPLEMENTATION    | x   |	
//HMI_SUPPORTS_ASIAN       | x   |
//-------------------------|-----|

    if (pPacketManagerLocal == &PacketManagerLocal_1)
    {
        #ifdef TYPE_1_IMPLEMENTATION
        //-------------------------|-----|
        //HMI_SUPPORTS_SERVICEPORT | Yes |
        //TYPE_1_IMPLEMENTATION    | Yes |
        //TYPE_2_IMPLEMENTATION    | No  |
        //TYPE_3_IMPLEMENTATION    | No  |
        //TYPE_4_IMPLEMENTATION    | No  |
        //TYPE_5_IMPLEMENTATION    | No  |
        //HMI_SUPPORTS_ASIAN       | x   |
        //-------------------------|-----|

        if (LOCAL_newCommsMode)
        {
            TUSIGN8 RsumPos = PacketManagerLocal_1.RxByte[1] + 2;
            TUSIGN8 Rsum = PacketManagerLocal_1.RxByte[RsumPos];
            TUSIGN8 Csum = 0x01;
            TUSIGN8 i;

            for (i = 0; i < RsumPos; i ++)
            {
                Csum += PacketManagerLocal_1.RxByte[i];
            }

            if ((PacketManagerLocal_1.RxByte[0] == COMMS_V2_INIT) && (Csum == Rsum))
            {
                PacketManagerLocal_1.CommunicationsErrorCode = 0x00;
                PacketManagerLocal_1.SwitchPressedCode = 0x00;

                #ifdef HMI_SUPPORTS_ASIAN
                //-------------------------|-----|
                //HMI_SUPPORTS_SERVICEPORT | Yes |
                //TYPE_1_IMPLEMENTATION    | Yes |
                //TYPE_2_IMPLEMENTATION    | No  |
                //TYPE_3_IMPLEMENTATION    | No  |
                //TYPE_4_IMPLEMENTATION    | No  |
                //TYPE_5_IMPLEMENTATION    | No  |
                //HMI_SUPPORTS_ASIAN       | Yes |
                //-------------------------|-----|


//                TUSIGN8 AsianIndex = 0;
//                TUSIGN16 AsianSupportedMask = 0x01;
//                TUSIGN16 AsianSupported = ((TUSIGN16)PacketManagerLocal_1.RxByte[3] * 256) + PacketManagerLocal_1.RxByte[4];
//
//                AsianSupportedMask = 0x01;
//                i = 5;
//                AsianIndex = 0;
//
//                while (AsianSupportedMask)
//                {
//                    if (AsianSupported & AsianSupportedMask)
//                    {
//                        FrameTableSetAsianVersion(AsianIndex, PacketManagerLocal_1.RxByte[i]);
//                        i++;
//                    }
//                    else
//                    {
//                        FrameTableSetAsianVersion(AsianIndex, 0);
//                    }
//                    AsianIndex++;
//                    AsianSupportedMask = AsianSupportedMask <<1;
//                }
                  TUSIGN16 BlockSupportFlags = ((TUSIGN16)PacketManagerLocal_1.RxByte[3] * 256) + PacketManagerLocal_1.RxByte[4];

//                 ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//                | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
//                 ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//                   |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//                   |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    X ____ supports EXTENDED_EUROPEAN
//                   |    |    |    |    |    |    |    |    |    |    |    |    |    |    X _________ supports KATAKANA
//                   |    |    |    |    |    |    |    |    |    |    |    |    |    X ______________ supports TURKISH
//                   |    |    |    |    |    |    |    |    |    |    |    |    X ___________________ supports RUSSIAN
//                   |    |    |    |    |    |    |    |    |    |    |    X ________________________ supports ANIMATED_ICONS
//                   |    |    |    |    |    |    |    |    |    |    X _____________________________ supports SMALL_ICONS
//                   |    |    |    |    |    |    |    |    |    X __________________________________ supports LOGO
//                   |    |    |    |    |    |    |    |    X _______________________________________ supports ASIAN
//                   |    |    |    |    |    |    |    |
//                   X    X    X    X    X    X    X    X ____________________________________________ Reserved for future use

                  TUSIGN16 BlockSupportMask = 0x0001;
                  TUSIGN8  ReceiveBufferIndex = 5;
                  TUSIGN8  BlockID = 0;

                  while ((BlockSupportMask) && (ReceiveBufferIndex < PacketManagerLocal_1.RxByte[1] + 2) && (BlockID < 16))
                  {
                      if (BlockSupportFlags & BlockSupportMask)
                      {
                          if ((BlockSupportMask == 0x0080) && (BlockID == 7))
                          {
                              PacketManagerLocal_1.AsianBlockVersion = PacketManagerLocal_1.RxByte[ReceiveBufferIndex];
                          }
                          //SetBlockVersion(BlockID,PacketManagerLocal_1.RxByte[ReceiveBufferIndex]);
                          ReceiveBufferIndex ++;
                      }
                      BlockID ++;
                      BlockSupportMask = BlockSupportMask << 1;
                  }

                #endif
                //-------------------------|-----|
                //HMI_SUPPORTS_SERVICEPORT | Yes |
                //TYPE_1_IMPLEMENTATION    | Yes |
                //TYPE_2_IMPLEMENTATION    | No  |
                //TYPE_3_IMPLEMENTATION    | No  |
                //TYPE_4_IMPLEMENTATION    | No  |
                //TYPE_5_IMPLEMENTATION    | No  |
                //HMI_SUPPORTS_ASIAN       | x   |
                //-------------------------|-----|

                PacketManagerLocal_1.COMMS_V2_INIT_reqd = eFALSE;
            }
            else if((PacketManagerLocal_1.RxByte[0] == LOW_LEVEL_INFO) && (Csum == Rsum))
            {
                PacketManagerLocal_1.CommunicationsErrorCode = 0x00;
                PacketManagerLocal_1.SwitchPressedCode = 0x00;
                PacketManagerLocal_1.DisplayHardwareRevision = PacketManagerLocal_1.RxByte[3];
                PacketManagerLocal_1.DisplaySoftwareRevision = PacketManagerLocal_1.RxByte[4];
		hmi_dynamic_data.lowLevelSoftwareRevision = PacketManagerLocal_1.DisplaySoftwareRevision;
                PacketManagerLocal_1.LOW_LEVEL_INFO_reqd = eFALSE;
            }
            else if ((PacketManagerLocal_1.RxByte[0] == READ_SWITCHES) && (Csum == Rsum))
            {                
                if (PacketManagerLocal_1.DisplayHardwareType == HMI_HW_NORMAL_B_SIMPLE_INDICATOR)
                {
                    // RxByte[2] |-------|-------|-------|-------|-------|-------|-------|-------|
                    //           | Bit7  | Bit6  | Bit5  | Bit4  | Bit3  | Bit2  | Bit1  | Bit0  |
                    //           |-------|-------|-------|-------|-------|-------|-------|-------|
                    //           |  DIP  |  DIP  |  DIP  |  DIP  | Push  | Push  | Proc  | Comms |
                    //           |   4   |   3   |   2   |   1   |   1   |  2    | Error | Error |
                    //           |-------|-------|-------|-------|-------|-------|-------|-------|
                
                    PacketManagerLocal_1.CommunicationsErrorCode = 0x00;
                    PacketManagerLocal_1.CommunicationsErrorCode |= PacketManagerLocal_1.RxByte[2] & 0x01 ? COMMUNICATION_ERROR : 0x00;
                    PacketManagerLocal_1.CommunicationsErrorCode |= PacketManagerLocal_1.RxByte[2] & 0x02 ? PROCESSING_ERROR : 0x00;
                    PacketManagerLocal_1.SwitchPressedCode = 0x00;
                    PacketManagerLocal_1.SimpleIndicatorSwitchState = (PacketManagerLocal_1.RxByte[2] & 0xFC);
                }
                else
                {
                    // RxByte[2] |-------|-------|-------|-------|-------|-------|-------|-------|
                    //           | Bit7  | Bit6  | Bit5  | Bit4  | Bit3  | Bit2  | Bit1  | Bit0  |
                    //           |-------|-------|-------|-------|-------|-------|-------|-------|
                    //           | Down  |  Up   | Left  | Right | Proc  | Switch|Display| Comms |
                    //           |  Key  |  Key  |  Key  |  Key  | Error | Error | Error | Error |
                    //           |-------|-------|-------|-------|-------|-------|-------|-------|

                    PacketManagerLocal_1.CommunicationsErrorCode = PacketManagerLocal_1.RxByte[2] & 0x0F;
                    PacketManagerLocal_1.SwitchPressedCode = (PacketManagerLocal_1.RxByte[2] & 0xF0) >> 4;
                    PacketManagerLocal_1.SimpleIndicatorSwitchState = 0x00;
                }
            }
            else
            {
                PacketManagerLocal_1.CommunicationsErrorCode = COMMUNICATION_ERROR;
                PacketManagerLocal_1.SwitchPressedCode = 0x00;
            }
// ??           PacketManagerLocal_1.updateStaticArea = 1;
// ??           PacketManagerLocal_1.packetStage = 0;
        }
        else
        {
            PacketManagerLocal_1.CommunicationsErrorCode = PacketManagerLocal_1.RxByte[0] & 0x0F;
            PacketManagerLocal_1.SwitchPressedCode = (PacketManagerLocal_1.RxByte[0] & 0xF0) >> 4;
        }

        #else
        //-------------------------|-----|
        //HMI_SUPPORTS_SERVICEPORT | Yes |
        //TYPE_1_IMPLEMENTATION    | No  |
        //TYPE_2_IMPLEMENTATION    | Yes |
        //TYPE_3_IMPLEMENTATION    | Yes |
        //TYPE_4_IMPLEMENTATION    | Yes |
		//TYPE_5_IMPLEMENTATION    | Yes |
        //HMI_SUPPORTS_ASIAN       | x   |
        //-------------------------|-----|

        PacketManagerLocal_1.CommunicationsErrorCode = PacketManagerLocal_1.RxByte[0] & 0x0F;
        PacketManagerLocal_1.SwitchPressedCode = (PacketManagerLocal_1.RxByte[0] & 0xF0) >> 4;

        #endif
        
        //-------------------------|-----|
        //HMI_SUPPORTS_SERVICEPORT | Yes |
        //TYPE_1_IMPLEMENTATION    | x   |
        //TYPE_2_IMPLEMENTATION    | x   |
        //TYPE_3_IMPLEMENTATION    | x   |
        //TYPE_4_IMPLEMENTATION    | x   |
		//TYPE_5_IMPLEMENTATION    | x   |
        //HMI_SUPPORTS_ASIAN       | x   |
        //-------------------------|-----|
        
        PacketManagerLocal_1.RxCount = 0;
        ReturnVal = PacketManagerLocal_1.CommunicationsErrorCode;
        
    }
    else if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        TUSIGN8 RsumPos = PacketManagerLocal_2.RxByte[1] + 2;
        TUSIGN8 Rsum = PacketManagerLocal_2.RxByte[RsumPos];
        TUSIGN8 Csum = 0x01;
        TUSIGN8 i;

        for (i = 0; i < RsumPos; i ++)
        {
            Csum += PacketManagerLocal_2.RxByte[i];
        }

        if ((PacketManagerLocal_2.RxByte[0] == COMMS_V2_INIT) && (Csum == Rsum))
        {
            PacketManagerLocal_2.CommunicationsErrorCode = 0x00;
            PacketManagerLocal_2.SwitchPressedCode = 0x00;
            PacketManagerLocal_2.COMMS_V2_INIT_reqd = eFALSE;
        }
        else if((PacketManagerLocal_2.RxByte[0] == LOW_LEVEL_INFO) && (Csum == Rsum))
        {
            PacketManagerLocal_2.CommunicationsErrorCode = 0x00;
            PacketManagerLocal_2.SwitchPressedCode = 0x00;
            PacketManagerLocal_2.DisplayHardwareRevision = HMI_HW_REMOTE_HMI;
            PacketManagerLocal_2.DisplaySoftwareRevision = PacketManagerLocal_2.RxByte[4];
            PacketManagerLocal_2.LOW_LEVEL_INFO_reqd = eFALSE;
        }
        else if ((PacketManagerLocal_2.RxByte[0] == READ_SWITCHES) && (Csum == Rsum))
        {
            // RxByte[2] |-------|-------|-------|-------|-------|-------|-------|-------|
            //           | Bit7  | Bit6  | Bit5  | Bit4  | Bit3  | Bit2  | Bit1  | Bit0  |
            //           |-------|-------|-------|-------|-------|-------|-------|-------|
            //           | Down  |  Up   | Left  | Right | Proc  | Switch|Display| Comms |
            //           |  Key  |  Key  |  Key  |  Key  | Error | Error | Error | Error |
            //           |-------|-------|-------|-------|-------|-------|-------|-------|

            PacketManagerLocal_2.CommunicationsErrorCode = PacketManagerLocal_1.RxByte[2] & 0x0F;
            PacketManagerLocal_2.SwitchPressedCode = (PacketManagerLocal_1.RxByte[2] & 0xF0) >> 4;
            PacketManagerLocal_2.SimpleIndicatorSwitchState = 0x00;
         /*   PacketManagerLocal_2.CommunicationsErrorCode = 0x00;
            PacketManagerLocal_2.SwitchPressedCode = (PacketManagerLocal_2.RxByte[2] & 0xF0) >> 4;*/
        }
        else
        {
            PacketManagerLocal_2.CommunicationsErrorCode = COMMUNICATION_ERROR;
            PacketManagerLocal_2.SwitchPressedCode = 0x00;
        }

        PacketManagerLocal_2.RxCount = 0;
        ReturnVal = PacketManagerLocal_2.CommunicationsErrorCode;

    }
    else
    {
        VIP_ASSERT(0);
    }
#else
//-------------------------|-----|
//HMI_SUPPORTS_SERVICEPORT | No  |
//TYPE_1_IMPLEMENTATION    | x   |
//TYPE_2_IMPLEMENTATION    | x   |
//TYPE_3_IMPLEMENTATION    | x   |
//TYPE_4_IMPLEMENTATION    | x   |
//TYPE_5_IMPLEMENTATION    | x   |
//HMI_SUPPORTS_ASIAN       | x   |
//-------------------------|-----|

    #ifdef TYPE_1_IMPLEMENTATION
    //-------------------------|-----|
    //HMI_SUPPORTS_SERVICEPORT | No  |
    //TYPE_1_IMPLEMENTATION    | Yes |
    //TYPE_2_IMPLEMENTATION    | No  |
    //TYPE_3_IMPLEMENTATION    | No  |
    //TYPE_4_IMPLEMENTATION    | No  |
    //TYPE_5_IMPLEMENTATION    | No  |
    //HMI_SUPPORTS_ASIAN       | x   |
    //-------------------------|-----|

    if (LOCAL_newCommsMode)
    {
        TUSIGN8 RsumPos = PacketManagerLocal.RxByte[1] + 2;
        TUSIGN8 Rsum = PacketManagerLocal.RxByte[RsumPos];
        TUSIGN8 Csum = 0x01;
        TUSIGN8 i;

        if(LOCAL_RxCount == 0)
        {
            return 1;   //receive error
        }
        
        if(!isSmartHMI)
        {
            for (i = 0; i < RsumPos; i ++)
            {
                Csum += PacketManagerLocal.RxByte[i];
            }
            
            if(Csum != Rsum)
            {
                LOCAL_RxCount = 0;
                return 1;       //receive error
            }
        }

        if (PacketManagerLocal.RxByte[0] == COMMS_V2_INIT)
        {
            PacketManagerLocal.CommunicationsErrorCode = 0x00;
            PacketManagerLocal.SwitchPressedCode = 0x00;

            #ifdef HMI_SUPPORTS_ASIAN
            //-------------------------|-----|
            //HMI_SUPPORTS_SERVICEPORT | No  |
            //TYPE_1_IMPLEMENTATION    | Yes |
            //TYPE_2_IMPLEMENTATION    | No  |
            //TYPE_3_IMPLEMENTATION    | No  |
            //TYPE_4_IMPLEMENTATION    | No  |
            //TYPE_5_IMPLEMENTATION    | No  |
            //HMI_SUPPORTS_ASIAN       | Yes |
            //-------------------------|-----|
//            TUSIGN8 AsianIndex = 0;
//            TUSIGN16 AsianSupportedMask = 0x01;
//            TUSIGN16 AsianSupported = ((TUSIGN16)PacketManagerLocal.RxByte[3] * 256) + PacketManagerLocal.RxByte[4];
//
//            AsianSupportedMask = 0x01;
//            i = 5;
//            AsianIndex = 0;
//
//            while (AsianSupportedMask)
//            {
//                if (AsianSupported & AsianSupportedMask)
//                {
//                    FrameTableSetAsianVersion(AsianIndex, PacketManagerLocal.RxByte[i]);
//                    i++;
//                }
//                else
//                {
//                    FrameTableSetAsianVersion(AsianIndex, 0);
//                }
//                AsianIndex++;
//                AsianSupportedMask = AsianSupportedMask <<1;

            TUSIGN16 BlockSupportFlags = ((TUSIGN16)PacketManagerLocal.RxByte[3] * 256) + PacketManagerLocal.RxByte[4];

//           ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//          | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
//           ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//             |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//             |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    X ____ supports EXTENDED_EUROPEAN
//             |    |    |    |    |    |    |    |    |    |    |    |    |    |    X _________ supports KATAKANA
//             |    |    |    |    |    |    |    |    |    |    |    |    |    X ______________ supports TURKISH
//             |    |    |    |    |    |    |    |    |    |    |    |    X ___________________ supports RUSSIAN
//             |    |    |    |    |    |    |    |    |    |    |    X ________________________ supports ANIMATED_ICONS
//             |    |    |    |    |    |    |    |    |    |    X _____________________________ supports SMALL_ICONS
//             |    |    |    |    |    |    |    |    |    X __________________________________ supports LOGO
//             |    |    |    |    |    |    |    |    X _______________________________________ supports ASIAN
//             |    |    |    |    |    |    |    |
//             X    X    X    X    X    X    X    X ____________________________________________ Reserved for future use


            TUSIGN16 BlockSupportMask = 0x0001;
            TUSIGN8  ReceiveBufferIndex = 5;
            TUSIGN8  BlockID = 0;

            if (BlockSupportFlags == 0)
			{	/* Add for: reset language setting to default value (english) when use push button HMI.
			  	   each hot connect hmi, the function will be executed and check the HMI version. */
				gVersionFlag_HMI = LOCAL_AsianBlockVersion;
				PacketManagerLocal.AsianBlockVersion = 0;
			}
            else {}
            
            while ((BlockSupportMask) && (ReceiveBufferIndex < PacketManagerLocal.RxByte[1] + 2) && (BlockID < 16))
            {
                if (BlockSupportFlags & BlockSupportMask)
                {
                   if ((BlockSupportMask == 0x0080) && (BlockID == 7))
                    {
                      //KlocWork: ABV.GENERAL-Critical-Array 'RxByte' of size 15 may use index value(s)15...20
                        //Erick-2017/11/01
                        if(ReceiveBufferIndex < sizeof(PacketManagerLocal.RxByte))
                        {
                            PacketManagerLocal.AsianBlockVersion = PacketManagerLocal.RxByte[ReceiveBufferIndex];
                            if (PacketManagerLocal.AsianBlockVersion >= TYPE_1_NVRAM_REQUIRED_VERSION)
                            {
                                gVersionFlag_HMI = PacketManagerLocal.AsianBlockVersion;
                            }  
                            else {}
                        }
                        else
                        {
                            VIP_ASSERT(0);
                        }
                    }
                    //SetBlockVersion(BlockID,PacketManagerLocal.RxByte[ReceiveBufferIndex]);
                    ReceiveBufferIndex ++;
                }
                BlockID ++;
                //BlockSupportMask = BlockSupportMask << 1;

                BlockSupportMask *= 2;

            }


            #endif
            //-------------------------|-----|
            //HMI_SUPPORTS_SERVICEPORT | No  |
            //TYPE_1_IMPLEMENTATION    | Yes |
            //TYPE_2_IMPLEMENTATION    | No  |
            //TYPE_3_IMPLEMENTATION    | No  |
            //TYPE_4_IMPLEMENTATION    | No  |
            //TYPE_5_IMPLEMENTATION    | No  |
            //HMI_SUPPORTS_ASIAN       | x   |
            //-------------------------|-----|

            PacketManagerLocal.COMMS_V2_INIT_reqd = eFALSE;
            if(PacketManagerLocal.DisplaySoftwareRevision==0xFF)
            {
                PacketManagerLocal.LOW_LEVEL_INFO_reqd = eTRUE;
            }
        }
        else if(PacketManagerLocal.RxByte[0] == LOW_LEVEL_INFO)
        {
            PacketManagerLocal.CommunicationsErrorCode = 0x00;
            PacketManagerLocal.SwitchPressedCode = 0x00;
            PacketManagerLocal.DisplayHardwareRevision = PacketManagerLocal.RxByte[3];
            PacketManagerLocal.DisplaySoftwareRevision = PacketManagerLocal.RxByte[4];
            hmi_dynamic_data.lowLevelSoftwareRevision = PacketManagerLocal.DisplaySoftwareRevision;
            PacketManagerLocal.LOW_LEVEL_INFO_reqd = eFALSE;
        }
        else if (PacketManagerLocal.RxByte[0] == READ_SWITCHES)
        {
            if (PacketManagerLocal.DisplayHardwareRevision == HMI_HW_NORMAL_B_SIMPLE_INDICATOR)
            {
                // RxByte[2] |-------|-------|-------|-------|-------|-------|-------|-------|
                //           | Bit7  | Bit6  | Bit5  | Bit4  | Bit3  | Bit2  | Bit1  | Bit0  |
                //           |-------|-------|-------|-------|-------|-------|-------|-------|
                //           |  DIP  |  DIP  |  DIP  |  DIP  | Push  | Push  | Proc  | Comms |
                //           |   4   |   3   |   2   |   1   |   1   |  2    | Error | Error |
                //           |-------|-------|-------|-------|-------|-------|-------|-------|

                PacketManagerLocal.CommunicationsErrorCode = 0x00;
                PacketManagerLocal.CommunicationsErrorCode |= PacketManagerLocal.RxByte[2] & 0x01 ? COMMUNICATION_ERROR : 0x00;
                PacketManagerLocal.CommunicationsErrorCode |= PacketManagerLocal.RxByte[2] & 0x02 ? PROCESSING_ERROR : 0x00;
                PacketManagerLocal.SwitchPressedCode = 0x00;
                PacketManagerLocal.SimpleIndicatorSwitchState = (PacketManagerLocal.RxByte[2] & 0xFC);
            }
            else
            {
                // RxByte[2] |-------|-------|-------|-------|-------|-------|-------|-------|
                //           | Bit7  | Bit6  | Bit5  | Bit4  | Bit3  | Bit2  | Bit1  | Bit0  |
                //           |-------|-------|-------|-------|-------|-------|-------|-------|
                //           | Down  |  Up   | Left  | Right | Proc  | Switch|Display| Comms |
                //           |  Key  |  Key  |  Key  |  Key  | Error | Error | Error | Error |
                //           |-------|-------|-------|-------|-------|-------|-------|-------|

                PacketManagerLocal.CommunicationsErrorCode = PacketManagerLocal.RxByte[2] & 0x0F;
                PacketManagerLocal.SwitchPressedCode = (PacketManagerLocal.RxByte[2] & 0xF0) >> 4;
                PacketManagerLocal.SimpleIndicatorSwitchState = 0x00;
            }
        }
        else
        {
            PacketManagerLocal.CommunicationsErrorCode = COMMUNICATION_ERROR;
            PacketManagerLocal.SwitchPressedCode = 0x00;
        }
    }
    else
    {
        PacketManagerLocal.CommunicationsErrorCode = PacketManagerLocal.RxByte[0] & 0x0F;
        PacketManagerLocal.SwitchPressedCode = (PacketManagerLocal.RxByte[0] & 0xF0) >> 4;
    }
    #else
    //-------------------------|-----|
    //HMI_SUPPORTS_SERVICEPORT | No  |
    //TYPE_1_IMPLEMENTATION    | No  |
    //TYPE_2_IMPLEMENTATION    | Yes |
    //TYPE_3_IMPLEMENTATION    | Yes |
    //TYPE_4_IMPLEMENTATION    | Yes |
    //TYPE_5_IMPLEMENTATION    | Yes |
    //HMI_SUPPORTS_ASIAN       | x   |
    //-------------------------|-----|

    PacketManagerLocal.CommunicationsErrorCode = PacketManagerLocal.RxByte[0] & 0x0F;
    PacketManagerLocal.SwitchPressedCode = (PacketManagerLocal.RxByte[0] & 0xF0) >> 4;

    #endif
    //-------------------------|-----|
    //HMI_SUPPORTS_SERVICEPORT | No  |
    //TYPE_1_IMPLEMENTATION    | x   |
    //TYPE_2_IMPLEMENTATION    | x   |
    //TYPE_3_IMPLEMENTATION    | x   |
    //TYPE_4_IMPLEMENTATION    | x   |
    //TYPE_5_IMPLEMENTATION    | x   |
    //HMI_SUPPORTS_ASIAN       | x   |
    //-------------------------|-----|
    LOCAL_RxCount = 0;
    ReturnVal = PacketManagerLocal.CommunicationsErrorCode;

#endif
//-------------------------|-----|
//HMI_SUPPORTS_SERVICEPORT | x   |
//TYPE_1_IMPLEMENTATION    | x   |
//TYPE_2_IMPLEMENTATION    | x   |
//TYPE_3_IMPLEMENTATION    | x   |
//TYPE_4_IMPLEMENTATION    | x   |
//TYPE_5_IMPLEMENTATION    | x   |
//HMI_SUPPORTS_ASIAN       | x   |
//-------------------------|-----|

    if (ReturnVal != 0)
    {
        ReturnVal = 1;
    }
    return ReturnVal;
}
/*!
 \brief         Reads and returns the most recent switch codes from HMI Interface class
 \author        Roger Arnold
 \param
 \return        most recent switch codes
 \test          No details available
*/
// No restriction on the optimization of this function
TUSIGN8 PacketManagerGetKeystate(TUSIGN8 instance)
{
    TUSIGN8 ReturnVal = 0x00;

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == 1)
    {
        ReturnVal = PacketManagerLocal_1.SwitchPressedCode;

        #ifdef AZ200_SPECIAL_FEATURES_HMI
        if ((ReturnVal == 0x00) && (PacketManagerLocal_1.autoExit))
        {
            PacketManagerLocal_1.autoExit = eFALSE;
            ReturnVal = KEYSTATE_LEFT;
        }
        #endif

        #if (HMI_DISPLAY_RATE_CYCLES < 7)
        TUSIGN8 FrameType = FrameTableGetCurrentFrame_LOCAL()->frameType;

        if (FrameType == FRAME_TYPE_ENUMERATED_PARAM)
        {
            if (readSwitchesIssued_1 == eFALSE)
            {
            ReturnVal = KEYSTATE_NONE;
            }
        }
        else if (FrameType == FRAME_TYPE_MENU)
        {
            TUSIGN8 specialMenuFlag = FrameTableGetCurrentFrame_LOCAL()->frameDefinition.menuFrame.specialMenuFlag;
            if ((readSwitchesIssued_1 == eFALSE) && ((specialMenuFlag == 0)   //std menu
                                                || (specialMenuFlag == 2)   //Signals view
                                                || (specialMenuFlag == 4))) //ARM Access level Selection
            {
            ReturnVal = KEYSTATE_NONE;
            }
        }
        else
        {
        asm("nop");
        }
        #endif

    }
    else if (instance == 2)
    {
        ReturnVal = PacketManagerLocal_2.SwitchPressedCode;

        #ifdef AZ200_SPECIAL_FEATURES_HMI
        if ((ReturnVal == 0x00) && (PacketManagerLocal_2.autoExit))
        {
            PacketManagerLocal_2.autoExit = eFALSE;
            ReturnVal = KEYSTATE_LEFT;
        }
        #endif

        #if (HMI_DISPLAY_RATE_CYCLES < 7) && defined(TYPE_1_IMPLEMENTATION)
        TUSIGN8 FrameType = FrameTableGetCurrentFrame_REMOTE()->frameType;

        if (FrameType == FRAME_TYPE_ENUMERATED_PARAM)
        {
            if (readSwitchesIssued_2 == eFALSE)
            {
            ReturnVal = KEYSTATE_NONE;
            }
        }
        else if (FrameType == FRAME_TYPE_MENU)
        {
            TUSIGN8 specialMenuFlag = FrameTableGetCurrentFrame_REMOTE()->frameDefinition.menuFrame.specialMenuFlag;
            if ((readSwitchesIssued_2 == eFALSE) && ((specialMenuFlag == 0)   //std menu
                                                || (specialMenuFlag == 2)   //Signals view
                                                || (specialMenuFlag == 4))) //ARM Access level Selection
            {
            ReturnVal = KEYSTATE_NONE;
            }
        }
        else
        {
        asm("nop");
        }
        #endif

    }
    else
    {
        VIP_ASSERT(0);
    }
#else

    ReturnVal = PacketManagerLocal.SwitchPressedCode;

    //check for some abnormal cases.
    if((ReturnVal!=KEYSTATE_LEFT)
    && (ReturnVal!=KEYSTATE_RIGHT)
    && (ReturnVal!=KEYSTATE_UP)
    && (ReturnVal!=KEYSTATE_DOWN)
    && (ReturnVal!=KEYSTATE_UP_AND_DOWN)
    && (ReturnVal!=KEYSTATE_DEFAULT_CONTRAST)
    && (ReturnVal!=KEYSTATE_NONE))
    {
        ReturnVal = KEYSTATE_NONE;
        return ReturnVal;
    }   

    #if (HMI_DISPLAY_RATE_CYCLES < 7) && defined(TYPE_1_IMPLEMENTATION)
    TUSIGN8 FrameType = FrameTableGetCurrentFrame()->frameType;

    if (FrameType == FRAME_TYPE_ENUMERATED_PARAM)
    {
        if (readSwitchesIssued_1 == eFALSE)
        {
	    ReturnVal = KEYSTATE_NONE;
        }
    }
    else if (FrameType == FRAME_TYPE_MENU)
    {
        TUSIGN8 specialMenuFlag = FrameTableGetCurrentFrame()->frameDefinition.menuFrame.specialMenuFlag;
        if ((readSwitchesIssued_1 == eFALSE) && ((specialMenuFlag == 0)   //std menu
                                            || (specialMenuFlag == 2)   //Signals view
                                            || (specialMenuFlag == 4))) //ARM Access level Selection
        {
	    ReturnVal = KEYSTATE_NONE;
        }
    }
    else
    {
	asm("nop");
    }
    #endif

    #ifdef AZ200_SPECIAL_FEATURES_HMI
    if ((ReturnVal == 0x00) && (PacketManagerLocal.autoExit))
    {
        PacketManagerLocal.autoExit = eFALSE;
        ReturnVal = KEYSTATE_LEFT;
    }
    #endif
#endif

    return ReturnVal;
}

/*!
 \brief         Builds Packets required to display an image for Smart HMI
 \author        
 \param
 \return        1 on success, 0 on any error.
 \test          No details available
*/
TUSIGN8 PacketManagerBuildImage_SHMI(TUSIGN8 t_updateActiveArea, TUSIGN8 t_keypressProcessed)
{
    TUSIGN8 imageBuilt = 0;
    TUSIGN8 i;
    TUSIGN8 currEditEnable;
    TUSIGN8 currErrorAction;
    TUSIGN8 packetBuffSize;
    TUSIGN8 returnVal = 0;

    packetBuffSize = BUFFERSIZE_SMART_HMI;
    if(LOCAL_packetBufferUsed > 0)
    {//the last packet data is not sent yet, will not update the buffer
        return 0;
    }
    
    LOCAL_packetBufferPreparing = eTRUE;    

    LOCAL_stringtable_text_id.ssIdx = HMI_IDX;
    LOCAL_stringtable_text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;

    #ifdef HMI_USES_COMMONDIAGNOSTICS
        UpdateInfoMessageTimer();
    #endif


    for (i = 0; i < packetBuffSize; i++)
    {
        LOCAL_packetBuffer[i] = 0;
    }

    LOCAL_packetBufferUsed = 0;
    LOCAL_lastFunctionCode = 0;

    LOCAL_flashState = (LOCAL_flashState + LOCAL_updateActiveArea);
    if (LOCAL_flashState > 2)
    {
        LOCAL_flashState = 0;
    }

    currErrorAction = BuildErrorResponse_s();

    LOCAL_CommunicationsErrorCode = 0;

    // If Response has already been built by BuildErrorResponse() then set imageBuilt flag
    if (currErrorAction == LOW_LEVEL_ERROR_RESPONSE_BUILT)
    {
        imageBuilt = 1;
    }
    else if (PacketManagerLocal.COMMS_V2_INIT_reqd)
    {
        imageBuilt += BuildV2Init_s(SFD_SIMPLE.contrast, 1);
        PacketManagerLocal.updateStaticArea = 1;
        PacketManagerLocal.packetStage = 0;        
    }
    else if (PacketManagerLocal.LOW_LEVEL_INFO_reqd)
    {
        imageBuilt += BuildLowLevelInfo_s(1);
        PacketManagerLocal.updateStaticArea = 1;
        PacketManagerLocal.packetStage = 0;        
    } 
    else
    {
#if defined(COMMON_TOP_WORKS_NEW_FEATURES)        
        if ((Type_1_SupportsRotation()) &&  (SFD_SIMPLE_CTW.Rotation != rotation))
        {
            if (BuildRotateDisplay(SFD_SIMPLE_CTW.Rotation))
            {
                imageBuilt ++;
                LOCAL_updateStaticArea = 1;
                rotation = SFD_SIMPLE_CTW.Rotation;
                LOCAL_last_contrast = 101;
            }
        }          
#endif
        LOCAL_pCurrDisplayFrame = FrameTableGetCurrentFrame();
        currEditEnable = FrameTableIsEditEnabled();

        if ((LOCAL_pPrevDisplayFrame != LOCAL_pCurrDisplayFrame)
         || (currErrorAction == LOW_LEVEL_ERROR_REBUILD_FRAME)
         || (LOCAL_prevErrorAction == LOW_LEVEL_ERROR_RESPONSE_BUILT))
        {
            LOCAL_updateStaticArea = 1;
        }
        
        if (SFD_SIMPLE.contrast != LOCAL_last_contrast)
        {
            TUSIGN8 tempPacketStage = LOCAL_packetStage;
            imageBuilt += BuildSetContrast(SFD_SIMPLE.contrast, 0);
            LOCAL_packetStage = tempPacketStage;
        }

        LOCAL_updateActiveArea = t_updateActiveArea;
        LOCAL_keypressProcessed = t_keypressProcessed;

        if ((LOCAL_pCurrDisplayFrame->frameType != FRAME_TYPE_NUMERICAL_PARAM) 
            && (LOCAL_pCurrDisplayFrame->frameType != FRAME_TYPE_INTEGER_PARAM))
        {
            LOCAL_overrangeValue_EditNotAllowed = eFALSE;
        }
        
        if (ToggleBacklight)
        {
            ToggleBacklight = eFALSE;
            TUSIGN8 tempPacketStage = LOCAL_packetStage;
            imageBuilt += BuildSetupDisplay(SETUP_BACKLIGHT_ON, 0);
            LOCAL_packetStage = tempPacketStage;

        }

        //gVersionFlag_HMI = 9;   //this is a workaroud, grace, tobe compare with LMT code, and verify

        switch (LOCAL_pCurrDisplayFrame->frameType)
        {
#ifdef HMI_SUPPORTS_GRAPH_VIEW//compileTimeDefinition.h
            TUSIGN8 graphViewFrameNumber;
#endif
            
            case FRAME_TYPE_OPERATOR:            
#ifdef HMI_SUPPORTS_GRAPH_VIEW
                graphViewFrameNumber = GetGraphViewFrameNumber();
                if (graphViewFrameNumber != 0)
                {
                    imageBuilt += BuildSFrame_GraphicOperator(graphViewFrameNumber);
                }
                else
#endif
                {                    
                    imageBuilt += BuildSFrame_OperatorPage();
                }          
                break;
            
            case FRAME_TYPE_CONFIG_HEADER:
                imageBuilt += BuildSFrame_ConfigHeader();
                break;
 
            case FRAME_TYPE_MENU:
                // 0-std menu, 1-Operator Menu, 2-Signals view, 3-Diagnostics View, 4-ARM Access level Selection, 5-Alarm History, 6-Secure Alarm History
                if(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 1)
                {                
                    imageBuilt += BuildSFrame_OperatorMenu();
                }
                else if(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 2)
                {
                    imageBuilt += BuildSFrame_SignalMenu();
                }                
                else if(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 3)  
                {
                    imageBuilt += BuildSFrame_DiagnoseMenu3();
                }
                else if(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 4)
                {
                    imageBuilt += BuildSFrame_AccessLevelMenu();
                }
                else if((LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 5)
                    ||  (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 6))
                {
                    imageBuilt += BuildSFrame_DiagnoseMenu5(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag);
                }
                else
                {
                    imageBuilt += BuildSFrame_StdMenu();
                }
                break;
                
#ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES            
            case FRAME_TYPE_TIME_AND_DATE_PARAM:
            case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
                if (currEditEnable)
                {
                    if (!LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_TimeDateEdit();
                }
                else
                {
                    if (LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_TimeDateView();
                }
                break;
#endif

#ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
            case FRAME_TYPE_WAVEFORM_FRAME:
                imageBuilt += BuildSFrame_Waveform();
                break;
#endif

#ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
            case FRAME_TYPE_LINEARIZER:
                imageBuilt += BuildSFrame_Linearizer();
                break;
#endif

#ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
            case FRAME_TYPE_NUMERICAL_PARAM:
            case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
            case FRAME_TYPE_INTEGER_PARAM:
            case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
                if (currEditEnable)
                {
                    if (!LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_NumericalEdit();
                }
                else
                {
                    if (LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_NumericalView();
                }
                break;
#endif

#ifdef HMI_SUPPORTS_STANDARDASCII
            case FRAME_TYPE_STRING_PARAM:
            case FRAME_TYPE_STRING_PARAM_NO_EDIT:
#endif
#ifdef HMI_SUPPORTS_PACKEDASCII
            case FRAME_TYPE_STRING_PARAM_PACKED:
            case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
#endif
#if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
                if (currEditEnable)
                {
                    if (!LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_StringEdit();
                }
                else
                {
                    if (LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_StringView();
                }
                break;
#endif

#ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
            case FRAME_TYPE_ENUMERATED_PARAM:
            case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
            case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
                if (currEditEnable)
                {
                    if (!LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_EnumEdit();
                }
                else
                {
                    if (LOCAL_prevEditEnable)
                    {
                        LOCAL_updateStaticArea = 1;
                    }
                    imageBuilt += BuildSFrame_EnumView();
                }
                break;
#endif

#ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
        case FRAME_TYPE_IP_ADDRESS:
        case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildFrame_IPAddressEdit();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildFrame_IPAddressView();
            }
            break;
#endif
                
#ifdef HMI_SUPPORTS_MANUAL_ADJUST
            case FRAME_TYPE_MANUAL_ADJUST:
#endif
#ifdef HMI_SUPPORTS_AUTO_ADJUST
            case FRAME_TYPE_AUTO_ADJUST:
            case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
#endif
#if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)                
            #ifdef HMI_SUPPORTS_DISPLAY_TEST
                if ((LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx == HMI_IDX)
                 && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx == HMI_IDX_displayTest)
                 && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx == 0))
                {
                    imageBuilt += BuildSFrame_DisplayTest();
                }
                else
            #endif
                {
                    imageBuilt += BuildSFrame_Adjust();
                }
                break;
#endif

#ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
            case FRAME_TYPE_MANUAL_CALIBRATION:
                imageBuilt += BuildSFrame_ManulCal();
                break;
#endif

            default:
                imageBuilt += BuildReadSwitches_SHMI(1);
                break;
        }
        LOCAL_pPrevDisplayFrame = LOCAL_pCurrDisplayFrame;
        LOCAL_prevEditEnable = currEditEnable;
    }
    LOCAL_prevErrorAction = currErrorAction;


    if (imageBuilt)
    {
        returnVal = LOCAL_packetBufferUsed;
    }
    else
    {
        imageBuilt += BuildReadSwitches_SHMI(1);
        if(imageBuilt)
        {
            returnVal = LOCAL_packetBufferUsed;
        }
    }

    LOCAL_packetBufferPreparing = eFALSE;

    return returnVal;
    
}
/*!
 \brief         Builds Packets required to display an image
 \author        Roger Arnold
 \param
 \return        1 on success, 0 on any error.
 \test          No details available
*/
// No restriction on the optimization of this function
TUSIGN8 PacketManagerBuildImage_HMI(TUSIGN8 t_updateActiveArea, TUSIGN8 t_keypressProcessed)
{

    TUSIGN8 imageBuilt = 0;
    TUSIGN8 i;
    TUSIGN8 currEditEnable;
    TUSIGN8 currErrorAction;

    LOCAL_stringtable_text_id.ssIdx = HMI_IDX;
    LOCAL_stringtable_text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;

    #ifdef HMI_USES_COMMONDIAGNOSTICS
        UpdateInfoMessageTimer();
    #endif

    #ifdef HMI_SUPPORTS_ASIAN

    if (SFD_SIMPLE.language == HMI_LANGUAGE_CHINESE)
    {
        LOCAL_Asian = eTRUE;
        LOCAL_AsianCharacterSet = 1;
    }
    else
    #endif
    {
        LOCAL_Asian = eFALSE;
        LOCAL_AsianCharacterSet = 0;
    }

    LOCAL_updateStaticArea = LOCAL_Asian == LOCAL_PrvAsian ? 0 : 1;
    LOCAL_PrvAsian = LOCAL_Asian;

    for (i = 0; i < PACKETBUFFERSIZE; i++)
    {
        LOCAL_packetBuffer[i] = 0;
    }

    LOCAL_packetBufferUsed = 0;
    LOCAL_lastFunctionCode = 0;

    LOCAL_flashState = (LOCAL_flashState + LOCAL_updateActiveArea);
    if (LOCAL_flashState > 2)
    {
        LOCAL_flashState = 0;
    }

    currErrorAction = BuildErrorResponse();

    LOCAL_CommunicationsErrorCode = 0;

    // If Response has already been built by BuildErrorResponse() then set imageBuilt flag
    if (currErrorAction == LOW_LEVEL_ERROR_RESPONSE_BUILT)
    {
        imageBuilt = 1;
    }

    // If comms Init required then send COMMS_V2_INIT packet

    #ifdef HMI_SUPPORTS_SERVICEPORT
    else if ((pPacketManagerLocal == &PacketManagerLocal_2) && (CommsInitReqd == eTRUE))
    {
        imageBuilt += BuildV2Init(SFD_SIMPLE.contrast, 1);
        PacketManagerLocal_2.updateStaticArea = 1;
        PacketManagerLocal_2.packetStage = 0;
        CommsInitReqd = eFALSE;
    }
    #endif

    #if defined(TYPE_1_IMPLEMENTATION) && defined(HMI_SUPPORTS_SERVICEPORT)
    else if ((pPacketManagerLocal == &PacketManagerLocal_1) && (PacketManagerLocal_1.COMMS_V2_INIT_reqd))
    {
        imageBuilt += BuildV2Init(SFD_SIMPLE.contrast, 1);
        PacketManagerLocal_1.updateStaticArea = 1;
        PacketManagerLocal_1.packetStage = 0;
    }
    #endif

    #if defined(TYPE_1_IMPLEMENTATION) && !defined(HMI_SUPPORTS_SERVICEPORT)
    else if (PacketManagerLocal.COMMS_V2_INIT_reqd)
    {
        imageBuilt += BuildV2Init(SFD_SIMPLE.contrast, 1);
        PacketManagerLocal.updateStaticArea = 1;
        PacketManagerLocal.packetStage = 0;
    }
    #endif

    // If Low Level Info required then send LOW_LEVEL_INFO packet

    #if defined(TYPE_1_IMPLEMENTATION) && defined(HMI_SUPPORTS_SERVICEPORT)
    else if ((pPacketManagerLocal == &PacketManagerLocal_1) && (PacketManagerLocal_1.LOW_LEVEL_INFO_reqd))
    {
        imageBuilt += BuildLowLevelInfo(1);
        PacketManagerLocal_1.updateStaticArea = 1;
        PacketManagerLocal_1.packetStage = 0;
    }
    #endif

    #if defined(TYPE_1_IMPLEMENTATION) && !defined(HMI_SUPPORTS_SERVICEPORT)
    else if (PacketManagerLocal.LOW_LEVEL_INFO_reqd)
    {
        imageBuilt += BuildLowLevelInfo(1);
        PacketManagerLocal.updateStaticArea = 1;
        PacketManagerLocal.packetStage = 0;
    }
    #endif

    #if defined(HMI_SUPPORTS_ASIAN) && defined(HMI_SUPPORTS_SERVICEPORT)
    else if ((LowLevelStartupCount_REMOTE_HMI) && (pPacketManagerLocal == &PacketManagerLocal_2))
    {
        TUSIGN8 incompatibleNV_1[21];
        TUSIGN8 incompatibleNV_2[21];

        LowLevelStartupCount_REMOTE_HMI --;

        if (LOCAL_Asian)
        {
            LowLevelStartupCount_REMOTE_HMI = LOWLEVEL_STARTUP_TIME;
            strcpy((char *)incompatibleNV_1," SELECTED LANGUAGE  ");
            strcpy((char *)incompatibleNV_2,"   NOT SUPPORTED    ");

        }
        else
        {
            strcpy((char *)incompatibleNV_1," NOT ALL LANGUAGES  ");
            strcpy((char *)incompatibleNV_2,"SUPPORTED BY DISPLAY");
        }
    }
    #endif

    // If low level does not support the selected language then display a message to identify this
    #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION)

    #ifdef HMI_SUPPORTS_SERVICEPORT
    else if((LowLevelStartupCount_LOCAL_HMI) && (LOCAL_AsianBlockVersion < TYPE_1_NVRAM_REQUIRED_VERSION) && (pPacketManagerLocal == &PacketManagerLocal_1))
    #else
    else if((LowLevelStartupCount_LOCAL_HMI) && (LOCAL_AsianBlockVersion < TYPE_1_NVRAM_REQUIRED_VERSION))
    #endif
    {
        TUSIGN8 incompatibleNV_1[21];
        TUSIGN8 incompatibleNV_2[21];

        LowLevelStartupCount_LOCAL_HMI --;

        if (LOCAL_Asian)
        {
            LowLevelStartupCount_LOCAL_HMI = LOWLEVEL_STARTUP_TIME;
            strcpy((char *)incompatibleNV_1," SELECTED LANGUAGE  ");
            strcpy((char *)incompatibleNV_2,"   NOT SUPPORTED    ");
        }
        else
        {
            strcpy((char *)incompatibleNV_1," NOT ALL LANGUAGES  ");
            strcpy((char *)incompatibleNV_2,"SUPPORTED BY DISPLAY");
        }

        imageBuilt += BuildWriteString(incompatibleNV_1,HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 0, JUSTIFY_CENTRE, FONT_1, NO_INVERT, 0, 0, 20, 0);   // 26 bytes  \ 56 bytes in total
        imageBuilt += BuildWriteString(incompatibleNV_2,HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 2, JUSTIFY_CENTRE, FONT_1, NO_INVERT, 0, 0, 20, 0);   // 26 bytes  / (< packet buffer size)
        imageBuilt += BuildBlankArea(4,7,1);                                                                                                        //  4 bytes
        LOCAL_updateStaticArea = 1;
        LOCAL_packetStage = 0;
    }
    #endif

    else
    {
        #if defined(COMMON_TOP_WORKS_NEW_FEATURES) && defined(TYPE_1_IMPLEMENTATION)

            #ifdef HMI_SUPPORTS_SERVICEPORT
            if (pPacketManagerLocal == &PacketManagerLocal_1)
            #endif
            {
                if ((Type_1_SupportsRotation()) &&  (SFD_SIMPLE_CTW.Rotation != rotation))
                {
                    if (BuildRotateDisplay(SFD_SIMPLE_CTW.Rotation))
                    {
                        imageBuilt ++;
                        LOCAL_updateStaticArea = 1;
                        rotation = SFD_SIMPLE_CTW.Rotation;
                        LOCAL_last_contrast = 101;
                    }
                }
            }

        #endif

        LOCAL_pCurrDisplayFrame = FrameTableGetCurrentFrame();
        currEditEnable = FrameTableIsEditEnabled();

        if ((LOCAL_pPrevDisplayFrame != LOCAL_pCurrDisplayFrame)
         || (currErrorAction == LOW_LEVEL_ERROR_REBUILD_FRAME)
         || (LOCAL_prevErrorAction == LOW_LEVEL_ERROR_RESPONSE_BUILT))
        {
            LOCAL_updateStaticArea = 1;
        }

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if (pPacketManagerLocal == &PacketManagerLocal_1)
        {
            if (SFD_SIMPLE.contrast != PacketManagerLocal_1.last_contrast)
            {
                TUSIGN8 tempPacketStage = LOCAL_packetStage;
                imageBuilt += BuildSetContrast(SFD_SIMPLE.contrast, 0);
                LOCAL_packetStage = tempPacketStage;
            }
        }
        #else
        if (SFD_SIMPLE.contrast != LOCAL_last_contrast)
        {
            TUSIGN8 tempPacketStage = LOCAL_packetStage;
            imageBuilt += BuildSetContrast(SFD_SIMPLE.contrast, 0);
            LOCAL_packetStage = tempPacketStage;
        }
        #endif

        LOCAL_updateActiveArea = t_updateActiveArea;
        LOCAL_keypressProcessed = t_keypressProcessed;

        if ((LOCAL_pCurrDisplayFrame->frameType != FRAME_TYPE_NUMERICAL_PARAM) && (LOCAL_pCurrDisplayFrame->frameType != FRAME_TYPE_INTEGER_PARAM))
        {
            LOCAL_overrangeValue_EditNotAllowed = eFALSE;
        }

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if (pPacketManagerLocal == &PacketManagerLocal_1)
        #endif
        {
            if (ToggleBacklight)
            {
                ToggleBacklight = eFALSE;
                #ifdef TYPE_1_IMPLEMENTATION
                TUSIGN8 tempPacketStage = LOCAL_packetStage;
                imageBuilt += BuildSetupDisplay(SETUP_BACKLIGHT_ON, 0);
                LOCAL_packetStage = tempPacketStage;
                #endif
            }
        }

        switch (LOCAL_pCurrDisplayFrame->frameType)
        {
        #if defined (HMI_SUPPORTS_OPERATOR_FRAMES) && !defined (TYPE_4_IMPLEMENTATION) && !defined (TYPE_5_IMPLEMENTATION)
        case FRAME_TYPE_OPERATOR:

            #ifdef LOW_LEVEL_HMI_TEST
            if (FrameTableGetLowLevelHMITestFrame() == LOCAL_pCurrDisplayFrame)
            {
                imageBuilt += BuildLowLevelHMITestFrame();
            }
            else
            #endif
            {
            #ifdef HMI_SUPPORTS_GRAPH_VIEW
                TUSIGN8 graphViewFrameNumber = GetGraphViewFrameNumber();
                if (graphViewFrameNumber != 0)
                {
                    imageBuilt += BuildGraphicalOperatorFrame(graphViewFrameNumber);
                }
                else
            #endif
                {
                    #ifdef HMI_SUPPORTS_DISPLAY_MODE_4x9
                    if (TestDisplayMode_4x9(LOCAL_pCurrDisplayFrame->frameDefinition.operatorFrame.frameNumber) == eTRUE)
                    {
                        imageBuilt += Build4x9OperatorFrame(LOCAL_pCurrDisplayFrame->frameDefinition.operatorFrame.frameNumber);
                    }
                    else
                    #endif
                    {
                        imageBuilt += BuildAdvancedOperatorFrame();
                    }
                }
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_CONFIG_HEADER_FRAMES
        case FRAME_TYPE_CONFIG_HEADER:
            imageBuilt += BuildConfigHeader();
            break;
        #endif

        #ifdef HMI_SUPPORTS_MENU_FRAMES
        case FRAME_TYPE_MENU:
            #ifdef HMI_USES_COMMONDIAGNOSTICS
            if ((LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 3)
             || (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 5)
             || (LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag == 6))
            {
                imageBuilt += BuidDiagnosticsMenu(LOCAL_pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag);
            }
            else
            #endif
            {
                #ifdef EDP300_SPECIAL_FEATURES_HMI
                if (LOCAL_pCurrDisplayFrame == FrameTableGetEDP300ModeFrame())
                {
                    imageBuilt += BuildEDP300ModeFrame();
                }
                else if (LOCAL_pCurrDisplayFrame == FrameTableGetEDP300QuickAdjustFrame())
                {
                    imageBuilt += BuildEDP300QuickAdjustFrame(eFALSE);
                }
                else
                #endif
                {
                    imageBuilt += BuildMenuFrame();
                }
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_TIME_DATE_PARAM_FRAMES
        case FRAME_TYPE_TIME_AND_DATE_PARAM:
        case FRAME_TYPE_TIME_AND_DATE_PARAM_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildTimeDateEditFrame();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildTimeDateViewFrame();
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_IP_ADDRESS_FRAMES
        case FRAME_TYPE_IP_ADDRESS:
        case FRAME_TYPE_IP_ADDRESS_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildIPAddressEditFrame();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildIPAddressViewFrame();
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_GRAPH_PARAM_FRAMES
        case FRAME_TYPE_GRAPH_PARAM_FRAME_1_NO_EDIT:
            imageBuilt += BuildGraphViewFrame(1);
            break;

        case FRAME_TYPE_GRAPH_PARAM_FRAME_2_NO_EDIT:
            imageBuilt += BuildGraphViewFrame(2);
            break;
        #endif

        #ifdef HMI_SUPPORTS_WAVEFORM_FRAMES
        case FRAME_TYPE_WAVEFORM_FRAME:
            imageBuilt += BuildWaveformFrame();
            break;
        #endif

        #ifdef HMI_SUPPORTS_LINEARIZER_FRAMES
        case FRAME_TYPE_LINEARIZER:
            imageBuilt += BuildLinearizerFrame();
            break;
        #endif

        #ifdef HMI_SUPPORTS_MATRIX_FRAMES
        case FRAME_TYPE_MATRIX_X_Y_XY:
            imageBuilt += BuildMatrixFrame(eFALSE);
            break;
        case FRAME_TYPE_MATRIX_T_C_D:
            imageBuilt += BuildMatrixFrame(eTRUE);
            break;
        #endif

        #ifdef HMI_SUPPORTS_NUMERICAL_PARAM_FRAMES
        case FRAME_TYPE_NUMERICAL_PARAM:
        case FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT:
        case FRAME_TYPE_INTEGER_PARAM:
        case FRAME_TYPE_INTEGER_PARAM_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildNumericalEditFrame();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildNumericalViewFrame();
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_STANDARDASCII
        case FRAME_TYPE_STRING_PARAM:
        case FRAME_TYPE_STRING_PARAM_NO_EDIT:
        #endif
        #ifdef HMI_SUPPORTS_PACKEDASCII
        case FRAME_TYPE_STRING_PARAM_PACKED:
        case FRAME_TYPE_STRING_PARAM_PACKED_NO_EDIT:
        #endif
            #if defined (HMI_SUPPORTS_STANDARDASCII) || defined(HMI_SUPPORTS_PACKEDASCII)
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildStringEditFrame();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildStringViewFrame();
            }
            break;
            #endif

        #ifdef HMI_SUPPORTS_ENUM_PARAM_FRAMES
        case FRAME_TYPE_ENUMERATED_PARAM:
        case FRAME_TYPE_ENUMERATED_PARAM_OVERRIDE_WP:
        case FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildEnumEditFrame();
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildEnumViewFrame();
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_MANUAL_ADJUST
        case FRAME_TYPE_MANUAL_ADJUST:
        #endif
        #ifdef HMI_SUPPORTS_AUTO_ADJUST
        case FRAME_TYPE_AUTO_ADJUST:
        case FRAME_TYPE_AUTO_ADJUST_WITH_TIMER_AND_RESULT:
        #endif
        #if defined (HMI_SUPPORTS_MANUAL_ADJUST) || defined(HMI_SUPPORTS_AUTO_ADJUST)

            #ifdef HMI_SUPPORTS_MANUAL_VALVE_CAL
            if (FrameTableIsManualValveCalActive())
            {
                imageBuilt += BuildEDP300ManualValveCalFrame();
            }
            else
            #endif
            {
                #ifdef EDP300_SPECIAL_FEATURES_HMI
                if (FrameTableIfAutoAdjMarker((TUSIGN8)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx,
                                               (TUSIGN8)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx,
                                               (TUSIGN8)LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx))
                {
                    imageBuilt += BuildEDP300QuickAdjustFrame(eTRUE);
                }
                else
                #endif
                {
                    #ifdef AZ200_SPECIAL_FEATURES_HMI
                    if ((LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx == CAL_IDX)
                     && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx == CAL_IDX_PerformanceLogEntryIndex)
                     && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx == 0))
                    {
                        imageBuilt += BuildPerfomanceLogView();
                    }
                    else
                    #endif
                    {
                        #ifdef HMI_SUPPORTS_DISPLAY_TEST
                        if ((LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.value_ssIdx == HMI_IDX)
                         && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_objidx == HMI_IDX_displayTest)
                         && (LOCAL_pCurrDisplayFrame->frameDefinition.adjParamFrame.adjvalue_attribidx == 0))
                        {
                            imageBuilt += BuildDisplayTest();
                        }
                        else
                        #endif
                        {
                            imageBuilt += BuildAdjustFrame();
                        }
                    }
                }
            }
            break;
        #endif

        #ifdef HMI_SUPPORTS_PROGRESS_FRAMES
        case FRAME_TYPE_PROGRESS_BAR:
            imageBuilt += BuildProgressFrame();
            break;
        #endif

        #ifdef HMI_SUPPORTS_MANUAL_CALIBRATION
        case FRAME_TYPE_MANUAL_CALIBRATION:
            imageBuilt += BuildManualCalFrame();
            break;
        #endif

        #ifdef HMI_SUPPORTS_AUTO_CALIBRATION
        case FRAME_TYPE_AUTO_CALIBRATION:
            imageBuilt += BuildAutoCalFrame();
            break;
        #endif

        #ifdef HMI_SUPPORTS_2PT_PH_CALIBRATION
        case FRAME_TYPE_2_POINT_pH_CALIBRATION:
            imageBuilt += BuildTwoPointpHCalFrame();
            break;
        #endif


        #ifdef HMI_SUPPORTS_BIT_ENUM_PARAM_FRAMES
        case FRAME_TYPE_BIT_ENUMERATED_PARAM:
        case FRAME_TYPE_BIT_ENUMERATED_PARAM_NO_EDIT:
            if (currEditEnable)
            {
                if (!LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildBitEnumerationFrame(eTRUE); // edit frame
            }
            else
            {
                if (LOCAL_prevEditEnable)
                {
                    LOCAL_updateStaticArea = 1;
                }
                imageBuilt += BuildBitEnumerationFrame(eFALSE); // view frame
            }
            break;
        #endif

        default:
            imageBuilt += BuildReadSwitches(1);
            break;
        }
        LOCAL_pPrevDisplayFrame = LOCAL_pCurrDisplayFrame;
        LOCAL_prevEditEnable = currEditEnable;
    }
    LOCAL_prevErrorAction = currErrorAction;

//    LOCAL_passcount++;

    TUSIGN8 returnVal = 0;

    if (imageBuilt)
    {
        returnVal = LOCAL_packetBufferUsed;
    }

    return returnVal;
}

#ifdef HMI_SUPPORTS_DISPLAY_MODE_4x9
TBOOL TestDisplayMode_4x9(TUSIGN8 frameNumber)
{
    TBOOL returnVal = eFALSE;
    TUSIGN8 displayMode = HMI_DISPLAY_MODE_OFF;
    switch(frameNumber)
    {
    case 6:
        #if defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_6;
        #endif
        break;
    case 5:
        #if defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_5;
        #endif
        break;
    case 4:
        #if defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_4;
        #endif
        break;
    case 3:
        #if defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_3;
        #endif
        break;
    case 2:
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_2;
        #endif
        break;
    case 1:
        displayMode = SFD_SIMPLE.displayMode_1;
        break;
    default:
        displayMode = HMI_DISPLAY_MODE_OFF;
        break;
    }
    if (displayMode == HMI_DISPLAY_MODE_4x9)
    {
        returnVal = eTRUE;
    }
    return returnVal;
}
#endif //end of #ifdef HMI_SUPPORTS_DISPLAY_MODE_4x9

// No restriction on the optimization of this function
TBOOL PacketManagerIsNumericValueOverrange(void)
{
    return LOCAL_overrangeValue_EditNotAllowed;
}


#ifdef HMI_SUPPORTS_GRAPH_VIEW

TUSIGN8 GetGraphViewFrameNumber(void)
{
    TUSIGN8 graphViewFrameNumber = LOCAL_pCurrDisplayFrame->frameDefinition.operatorFrame.frameNumber;

    TUSIGN8 displayMode;

    switch (graphViewFrameNumber)
    {
    case 6:
        #if defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_6;
        #else
        displayMode = HMI_DISPLAY_MODE_OFF;
        #endif
        break;

    case 5:
        #if defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_5;
        #else
        displayMode = HMI_DISPLAY_MODE_OFF;
        #endif
        break;

    case 4:
        #if defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_4;
        #else
        displayMode = HMI_DISPLAY_MODE_OFF;
        #endif
        break;

    case 3:
        #if defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_3;
        #else
        displayMode = HMI_DISPLAY_MODE_OFF;
        #endif
        break;

    case 2:
        #if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
        displayMode = SFD_MULTI_OP.displayMode_2;
        #else
        displayMode = HMI_DISPLAY_MODE_OFF;
        #endif
        break;

    case 1:
        displayMode = SFD_SIMPLE.displayMode_1;
        break;

    default:
        displayMode = HMI_DISPLAY_MODE_OFF;
        break;
    }

    if (displayMode != HMI_DISPLAY_MODE_GRAPHIC)
    {
        // Not a graph view frame so clear graphViewFrameNumber
        graphViewFrameNumber = 0;
    }
    return graphViewFrameNumber;
}
#endif // #ifdef HMI_SUPPORTS_GRAPH_VIEW

////////////////////////////////////////////////////////////////////////////////
// Functions To Build packets
//

#if defined(COMMON_TOP_WORKS_NEW_FEATURES) && defined(TYPE_1_IMPLEMENTATION)

TUSIGN8 BuildRotateDisplay(TUSIGN8 rotationRequired)
{
    LOCAL_packet_HL[0] = ROTATE_DISPLAY;
    LOCAL_packet_HL[1] = 3;

    if (rotationRequired == HMI_ROTATION_180)
    {
        LOCAL_packet_HL[2] = ROTATION_180_COLUMN_MIRROR;
        LOCAL_packet_HL[3] = ROTATION_180_COMMMON_OP_MODE_SEL;
        LOCAL_packet_HL[4] = ROTATION_180_COLUMN_OFFSET;
    }
    else
    {
        LOCAL_packet_HL[2] = ROTATION_0_COLUMN_MIRROR;
        LOCAL_packet_HL[3] = ROTATION_0_COMMMON_OP_MODE_SEL;
        LOCAL_packet_HL[4] = ROTATION_0_COLUMN_OFFSET;
    }
    LOCAL_packet_HL[5] = AddChecksum(5);
    return AddPacketToBuffer(6, 0);
}
#endif

// No restriction on the optimization of this function
TUSIGN8 BuildBlankArea(TUSIGN8 start, TUSIGN8 end, TUSIGN8 lastPacket)
{
    LOCAL_packet_HL[0] = BLANK_AREA;
    LOCAL_packet_HL[1] = 1;

    if (start > end)
    {
        LOCAL_packet_HL[2] = (end & 0x07) | ((start & 0x07) << 3);   //Trap for start > end
    }
    else
    {
        LOCAL_packet_HL[2] = (start & 0x07) | ((end & 0x07) << 3);
    }

    LOCAL_packet_HL[3] = AddChecksum(3);

    return AddPacketToBuffer(4, lastPacket);
}

// No restriction on the optimization of this function
TUSIGN8 BuildSliderBar(TINT16 high, TINT16 actual, TUSIGN8 lastPacket)
{
    TUSIGN8 tempCalc = LOCAL_Asian ? 16: 20;	// MASH ???????

    if ((actual <= high) && (actual < 1638) && (high != 0))
    {
        tempCalc = (TUSIGN8)((tempCalc * actual) / high);
    }

    LOCAL_packet_HL[0] = WRITE_SLIDER_BAR;
    if (actual < 99)
    {
        LOCAL_packet_HL[2] = (TUSIGN8)actual + 1;
    }
    else
    {
        LOCAL_packet_HL[2] = 0;
    }

    LOCAL_packet_HL[3] = (tempCalc/4) + 1;

    if (LOCAL_packet_HL[3] == 6)
    {
        LOCAL_packet_HL[4] = 0x3E;
        LOCAL_packet_HL[5] = 0x41;
        LOCAL_packet_HL[6] = 0x3E;
        LOCAL_packet_HL[1] = 5;
    }
    else
    {
        switch (tempCalc & 0x03)   //(sliderPos%4)
        {
        case 3:
            LOCAL_packet_HL[4] = 0x80;
            LOCAL_packet_HL[5] = 0x7F;
            LOCAL_packet_HL[6] = 0x80;
            LOCAL_packet_HL[7] = 0x0F;
            LOCAL_packet_HL[8] = 0xF0;
            LOCAL_packet_HL[9] = 0x0F;
            LOCAL_packet_HL[1] = 8;
            break;

        case 2:
            LOCAL_packet_HL[4] = 0xE0;
            LOCAL_packet_HL[5] = 0x1F;
            LOCAL_packet_HL[6] = 0xE0;
            LOCAL_packet_HL[7] = 0x03;
            LOCAL_packet_HL[8] = 0xFC;
            LOCAL_packet_HL[9] = 0x03;
            LOCAL_packet_HL[1] = 8;
            break;

        case 1:
            LOCAL_packet_HL[4] = 0xF8;
            LOCAL_packet_HL[5] = 0x07;
            LOCAL_packet_HL[6] = 0xF8;
            LOCAL_packet_HL[1] = 5;
            break;

        default:    //case 0:
            LOCAL_packet_HL[4] = 0x3E;
            LOCAL_packet_HL[5] = 0xC1;
            LOCAL_packet_HL[6] = 0x3E;
            LOCAL_packet_HL[1] = 5;
            break;
        }
    }

    tempCalc = LOCAL_packet_HL[1] + 2;

    LOCAL_packet_HL[tempCalc] = AddChecksum(tempCalc);
    tempCalc ++;
    return AddPacketToBuffer(tempCalc, lastPacket);
}


// No restriction on the optimization of this function
TUSIGN8 BuildFrameTitle(const TUSIGN8 *titleText, TUSIGN8 lastPacket)
{
    TUSIGN8 stringStartPosition,n_format_chars,extendedCharacterPage;

    TUSIGN8 text[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    TUSIGN8 len = (TUSIGN8)strlen((const char *)titleText);
    if (len == 0)
    {
        text[0] = '-';
        text[1] = 0x00;
    }
    else
    {
        if (len > 25)
        {
            len = 25;
        }
        strncpy((char *)text,(const char *)titleText,(size_t)len);
    }

    switch (SFD_SIMPLE.language)
    {
    case HMI_LANGUAGE_RUSSIAN:  extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_RUSSIAN;    break;
    case HMI_LANGUAGE_TURKISH:  extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_TURKISH;    break;
    case HMI_LANGUAGE_JAPANESE: extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_KATAKANA;   break;
    default:                    extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN;   break;
    }


    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        convertExtendedCharacterSet(text,extendedCharacterPage,len);
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_HardwareSupportsExtendedCharacterSets() == eFALSE)
        {
            convertExtendedCharacterSet(text,extendedCharacterPage,len);
        }
        #endif
    }

    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        stringStartPosition = WRITE_TITLE_STRING_START_POSITION;
        n_format_chars      = WRITE_TITLE_N_STRING_FORMAT_CHARS;
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_SoftwareSupportsExtendedWriteString())
        {
            stringStartPosition = WRITE_TITLE_EXTENDED_STRING_START_POSITION;
            n_format_chars      = WRITE_TITLE_EXTENDED_N_STRING_FORMAT_CHARS;
        }
        else
        {
            stringStartPosition = WRITE_TITLE_STRING_START_POSITION;
            n_format_chars      = WRITE_TITLE_N_STRING_FORMAT_CHARS;
        }
        #else
        stringStartPosition = WRITE_TITLE_EXTENDED_STRING_START_POSITION;
        n_format_chars      = WRITE_TITLE_EXTENDED_N_STRING_FORMAT_CHARS;
        #endif
    }

    LOCAL_packet_HL[0] = WRITE_FRAME_TITLE;
    LOCAL_packet_HL[1] = len + n_format_chars;

    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal != &PacketManagerLocal_2)
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_SoftwareSupportsExtendedWriteString())
        #endif
        {
            LOCAL_packet_HL[2] = extendedCharacterPage;
        }
    }

    (void)strncpy((char *)&LOCAL_packet_HL[stringStartPosition],(const char *)text,25);

    // At this point len = length of string.
    // Re-calculate len. To equal length of message being sent to LL
    len += (2 + n_format_chars);        // add  function code byte,length byte * number of format chars
    LOCAL_packet_HL[len] = AddChecksum(len);
    len ++;

    return AddPacketToBuffer(len, lastPacket);
}

#ifdef HMI_SUPPORTS_ASIAN
// No restriction on the optimization of this function
TUSIGN8 BuildFrameTitle_w(const TUSIGN8 *titleText, TUSIGN8 maxLength, TUSIGN8 lastPacket)
{
#ifndef SINGLE_BYTE_ASIAN
    maxLength *= 2;
#endif

    TUSIGN8 len = (TUSIGN8)strlen((const char *)titleText);
    if (len > maxLength)
    {
        len = maxLength;
    }
    LOCAL_packet_HL[0] = WRITE_FRAME_TITLE_ASIAN;
    LOCAL_packet_HL[1] = len+1;
    LOCAL_packet_HL[2] = LOCAL_AsianCharacterSet;
    strncpy((char *)&LOCAL_packet_HL[3],(const char *)titleText,(size_t)len);
    len += 3;
    LOCAL_packet_HL[len] = AddChecksum(len);
    len ++;
    return AddPacketToBuffer(len, lastPacket);
}
#endif

// No restriction on the optimization of this function
TUSIGN8 BuildWriteString(const TUSIGN8 *suppliedText, TUSIGN8 extendedCharacterPage, TUSIGN8 line, TUSIGN8 justify, TUSIGN8 font, TUSIGN8 invert, TUSIGN8 invchar, TUSIGN8 blank, TUSIGN8 minLength, TUSIGN8 lastPacket)
{
    #if defined(TYPE_5_IMPLEMENTATION) && defined(DISPLAY_SIZE_D)
    if(line == SOFTKEY_PAGE)
    {
        minLength = 1;
    }
    #endif

    TUSIGN8 c = 6;
    TINT16 xpos = 0;
    TUSIGN8 totalLength;
    TUSIGN8 j;
    TUSIGN8 stringStartPosition = WRITE_STRING_EXTENDED_STRING_START_POSITION;

    TUSIGN8 text[22] = {0};
    TUSIGN8 leadingSpaces[22] = {0};
    TUSIGN8 trailingSpaces[22] = {0};
    memset((void*)LOCAL_packet_HL,0x00,30);

    TUSIGN8 len = (TUSIGN8)(strlen((const char *)suppliedText));

    if (len > 21)       {len = 21;}
    if (minLength > 21) {minLength = 21;}


    if (extendedCharacterPage == HMI_EXTENDED_CHARACTER_PAGE_USE_LANGUAGE)
    {
        switch (SFD_SIMPLE.language)
        {
        case HMI_LANGUAGE_RUSSIAN:  extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_RUSSIAN;    break;
        case HMI_LANGUAGE_TURKISH:  extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_TURKISH;    break;
        case HMI_LANGUAGE_JAPANESE: extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_KATAKANA;   break;
        default:                    extendedCharacterPage = HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN;   break;
        }
    }

    invchar = invchar & 0x1F;                               // maximum of 5 bits used for invert Char

    extendedCharacterPage = extendedCharacterPage & 0xE0;   // maximum of 3 bits used for extended character Page

    strncpy((char *)text,(const char *)suppliedText,(size_t)len);

    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        convertExtendedCharacterSet(text,extendedCharacterPage,len);
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_HardwareSupportsExtendedCharacterSets() == eFALSE)
        {
            convertExtendedCharacterSet(text,extendedCharacterPage,len);
        }
        #endif
    }

    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        stringStartPosition = WRITE_STRING_STRING_START_POSITION;
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_SoftwareSupportsExtendedWriteString())
        {
            stringStartPosition = WRITE_STRING_EXTENDED_STRING_START_POSITION;
        }
        else
        {
            stringStartPosition = WRITE_STRING_STRING_START_POSITION;
        }
        #else
        stringStartPosition = WRITE_STRING_EXTENDED_STRING_START_POSITION;
        #endif
    }

    totalLength = strlen((char*)leadingSpaces) + strlen((char*)text) + strlen((char*)trailingSpaces);

    switch (justify)
    {
    case JUSTIFY_RIGHT_WITH_INDENT_HALF:
    case JUSTIFY_RIGHT:
    case JUSTIFY_RIGHT_WITH_INDENT_1:
    case JUSTIFY_RIGHT_WITH_INDENT_2:
    case JUSTIFY_RIGHT_WITH_INDENT_3:
    case JUSTIFY_RIGHT_WITH_INDENT_4:
    case JUSTIFY_RIGHT_WITH_INDENT_5:
    case JUSTIFY_RIGHT_WITH_INDENT_6:
    case JUSTIFY_RIGHT_WITH_INDENT_7:
    case JUSTIFY_RIGHT_WITH_INDENT_8:
    case JUSTIFY_RIGHT_WITH_INDENT_9:

        while((minLength > 0) && (totalLength < minLength))
        {
            (void)strcat((char*)leadingSpaces," ");
            totalLength ++;
        }
        break;

    case JUSTIFY_LEFT_WITH_INDENT_1:
    case JUSTIFY_LEFT_WITH_INDENT_2:
    case JUSTIFY_LEFT_WITH_INDENT_3:
    case JUSTIFY_LEFT_WITH_INDENT_4:
    case JUSTIFY_LEFT_WITH_INDENT_5:
    case JUSTIFY_LEFT_WITH_INDENT_6:
    case JUSTIFY_LEFT_WITH_INDENT_7:
    case JUSTIFY_LEFT_WITH_INDENT_9:
    case JUSTIFY_LEFT_WITH_INDENT_10:
    case JUSTIFY_LEFT_WITH_INDENT_11:
    case JUSTIFY_LEFT_WITH_INDENT_12:
    case JUSTIFY_LEFT_WITH_INDENT_13:
    case JUSTIFY_LEFT_WITH_INDENT_14:
    case JUSTIFY_LEFT_WITH_INDENT_17:
    case JUSTIFY_LEFT_WITH_INDENT_18:
    case JUSTIFY_LEFT:
    case JUSTIFY_LEFT_WITH_INDENT_1_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_14_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_19_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_25_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_30_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_31_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_63_PIXEL:
#if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
    case JUSTIFY_LEFT_WITH_INDENT_35_PIXEL:
#endif
    case JUSTIFY_LEFT_WITH_INDENT_78_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_79_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_20_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_98_PIXEL:
#ifdef MANUAL_ADJUST_ACCELERATOR
    case JUSTIFY_FOR_MANUAL_ADJ_0:
    case JUSTIFY_FOR_MANUAL_ADJ_1:
    case JUSTIFY_FOR_MANUAL_ADJ_2:
    case JUSTIFY_FOR_MANUAL_ADJ_3:
    case JUSTIFY_FOR_MANUAL_ADJ_4:
    case JUSTIFY_FOR_MANUAL_ADJ_5:
    case JUSTIFY_FOR_MANUAL_ADJ_6:
    case JUSTIFY_FOR_MANUAL_ADJ_7:
    case JUSTIFY_FOR_MANUAL_ADJ_8:
    case JUSTIFY_FOR_MANUAL_ADJ_9:
    case JUSTIFY_FOR_MANUAL_ADJ_10:
    case JUSTIFY_FOR_MANUAL_ADJ_11:
    case JUSTIFY_FOR_MANUAL_ADJ_12:
    case JUSTIFY_FOR_MANUAL_ADJ_13:
    case JUSTIFY_FOR_MANUAL_ADJ_14:
    case JUSTIFY_FOR_MANUAL_ADJ_15:
    case JUSTIFY_FOR_MANUAL_ADJ_16:
    case JUSTIFY_FOR_MANUAL_ADJ_17:
    case JUSTIFY_FOR_MANUAL_ADJ_18:
    case JUSTIFY_FOR_MANUAL_ADJ_19:
    case JUSTIFY_FOR_MANUAL_ADJ_20:
    case JUSTIFY_FOR_MANUAL_ADJ_21:
    case JUSTIFY_FOR_MANUAL_ADJ_22:
    case JUSTIFY_FOR_MANUAL_ADJ_23:
    case JUSTIFY_FOR_MANUAL_ADJ_24:
    case JUSTIFY_FOR_MANUAL_ADJ_25:
    case JUSTIFY_FOR_MANUAL_ADJ_26:
    case JUSTIFY_FOR_MANUAL_ADJ_27:
    case JUSTIFY_FOR_MANUAL_ADJ_28:
    case JUSTIFY_FOR_MANUAL_ADJ_29:
    case JUSTIFY_FOR_MANUAL_ADJ_30:
    case JUSTIFY_FOR_MANUAL_ADJ_31:
    case JUSTIFY_FOR_MANUAL_ADJ_32:
    case JUSTIFY_FOR_MANUAL_ADJ_33:
    case JUSTIFY_FOR_MANUAL_ADJ_34:
    case JUSTIFY_FOR_MANUAL_ADJ_35:
    case JUSTIFY_FOR_MANUAL_ADJ_36:
    case JUSTIFY_FOR_MANUAL_ADJ_37:
    case JUSTIFY_FOR_MANUAL_ADJ_38:
    case JUSTIFY_FOR_MANUAL_ADJ_39:
    case JUSTIFY_FOR_MANUAL_ADJ_40:
    case JUSTIFY_FOR_MANUAL_ADJ_41:
    case JUSTIFY_FOR_MANUAL_ADJ_42:
    case JUSTIFY_FOR_MANUAL_ADJ_43:
    case JUSTIFY_FOR_MANUAL_ADJ_44:
    case JUSTIFY_FOR_MANUAL_ADJ_45:
    case JUSTIFY_FOR_MANUAL_ADJ_46:
    case JUSTIFY_FOR_MANUAL_ADJ_47:
    case JUSTIFY_FOR_MANUAL_ADJ_48:
    case JUSTIFY_FOR_MANUAL_ADJ_49:
    case JUSTIFY_FOR_MANUAL_ADJ_50:
    case JUSTIFY_FOR_MANUAL_ADJ_51:
    case JUSTIFY_FOR_MANUAL_ADJ_52:
    case JUSTIFY_FOR_MANUAL_ADJ_53:
    case JUSTIFY_FOR_MANUAL_ADJ_54:
    case JUSTIFY_FOR_MANUAL_ADJ_55:
    case JUSTIFY_FOR_MANUAL_ADJ_56:
    case JUSTIFY_FOR_MANUAL_ADJ_57:
    case JUSTIFY_FOR_MANUAL_ADJ_58:
    case JUSTIFY_FOR_MANUAL_ADJ_59:
    case JUSTIFY_FOR_MANUAL_ADJ_60:
    case JUSTIFY_FOR_MANUAL_ADJ_61:
    case JUSTIFY_FOR_MANUAL_ADJ_62:
    case JUSTIFY_FOR_MANUAL_ADJ_63:
    case JUSTIFY_FOR_MANUAL_ADJ_64:
    case JUSTIFY_FOR_MANUAL_ADJ_65:
    case JUSTIFY_FOR_MANUAL_ADJ_66:
    case JUSTIFY_FOR_MANUAL_ADJ_67:
    case JUSTIFY_FOR_MANUAL_ADJ_68:
    case JUSTIFY_FOR_MANUAL_ADJ_69:
#endif

        while((minLength > 0) && (totalLength < minLength))
        {
            (void)strcat((char*)trailingSpaces," ");
            totalLength ++;
        }
        break;

    default: /*case JUSTIFY_CENTRE:*/

        while((minLength > 0) && (totalLength < minLength))
        {
            (void)strcat((char*)trailingSpaces," ");
            totalLength ++;

            if (totalLength < minLength)
            {
                (void)strcat((char*)leadingSpaces," ");
                totalLength ++;
            }
        }
        break;
    }

    len = totalLength;

    // R.J.Arnold   2012-10-09  Klocwork issue suppressed.
    //-----------   ----------  -------------------------
    // ABV.GENERAL: Array '&pPacketManagerLocal->packet_HL[stringStartPosition]' of size 25 may use index value(s) 0..45.
    // total length of leadingSpaces + text + trailingSpaces cannot exceed 21 characters
    (void)strcpy((char *)&LOCAL_packet_HL[stringStartPosition],(const char *)leadingSpaces);
    (void)strcat((char *)&LOCAL_packet_HL[stringStartPosition],(const char *)text);
    (void)strcat((char *)&LOCAL_packet_HL[stringStartPosition],(const char *)trailingSpaces);

    line = line & 0x07;
    font = font & 0x38;

    LOCAL_packet_HL[0] = WRITE_STRING;


    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        LOCAL_packet_HL[1] = len +  WRITE_STRING_N_STRING_FORMAT_CHARS;
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_SoftwareSupportsExtendedWriteString())
        {
            LOCAL_packet_HL[1] = len + WRITE_STRING_EXTENDED_N_STRING_FORMAT_CHARS;
        }
        else
        {
            LOCAL_packet_HL[1] = len + WRITE_STRING_N_STRING_FORMAT_CHARS;
        }
        #else
        LOCAL_packet_HL[1] = len + WRITE_STRING_EXTENDED_N_STRING_FORMAT_CHARS;
        #endif
    }

#pragma diag_suppress=Pm060

    switch (font)
    {
#ifdef HMI_SUPPORTS_FONT_5
    case FONT_5:                            c = 30; /* 25x42        30x48 */        break;
#endif
#ifdef HMI_SUPPORTS_FONT_4
    case FONT_4:                            c = 24; /* 20x28        24x32 */        break;
#endif
#ifdef HMI_SUPPORTS_FONT_3
    case FONT_3:                            c = 18; /* 15x21        18x24 */        break;
#endif
#ifdef HMI_SUPPORTS_FONT_2
    case FONT_2:                            c = 12; /* 10x14        12x16 */        break;
#endif

    case FONT_1:                            c = 6;  /*  5x14         6x16 */        break;

    default: /*FONT_0:*/    font = 0;       c = 6;  /*  5x7          6x8 */         break;
    }
#pragma diag_default=Pm060

    switch (justify)
    {
    case JUSTIFY_RIGHT_WITH_INDENT_HALF:    xpos = MAX_COMMON_HMI_WIDTH - ((c * len) + (c/2));       break;

    case JUSTIFY_RIGHT:                     //xpos = 126 - (c * len);                 break;
    case JUSTIFY_RIGHT_WITH_INDENT_1:       //xpos = 126 - (c * (len + 1));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_2:       //xpos = 126 - (c * (len + 2));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_3:       //xpos = 126 - (c * (len + 3));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_4:       //xpos = 126 - (c * (len + 4));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_5:       //xpos = 126 - (c * (len + 5));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_6:       //xpos = 126 - (c * (len + 6));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_7:       //xpos = 126 - (c * (len + 7));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_8:       //xpos = 126 - (c * (len + 8));           break;
    case JUSTIFY_RIGHT_WITH_INDENT_9:       //xpos = 126 - (c * (len + 9));           break;
                                            xpos = MAX_COMMON_HMI_WIDTH - (c * (len + (justify - JUSTIFY_RIGHT))); break;


    case JUSTIFY_CENTRE:                    xpos = (MAX_COMMON_HMI_WIDTH/2)-1 - ((c/2) * len);              break;
    case JUSTIFY_LEFT_WITH_INDENT_1:        xpos = c;                               break;
    case JUSTIFY_LEFT_WITH_INDENT_2:        xpos = c * 2;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_3:        xpos = c * 3;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_4:        xpos = c * 4;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_5:        xpos = c * 5;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_6:        xpos = c * 6;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_7:        xpos = c * 7;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_9:        xpos = c * 9;                           break;
    case JUSTIFY_LEFT_WITH_INDENT_10:       xpos = c * 10;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_11:       xpos = c * 11;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_12:       xpos = c * 12;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_13:       xpos = c * 13;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_14:       xpos = c * 14;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_17:       xpos = c * 17;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_18:       xpos = c * 18;                          break;
    case JUSTIFY_LEFT_WITH_INDENT_1_PIXEL:  xpos = 1;                               break;
    case JUSTIFY_LEFT_WITH_INDENT_14_PIXEL: xpos = 14;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_19_PIXEL: xpos = 19;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_20_PIXEL: xpos = 20;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_25_PIXEL: xpos = 25;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_30_PIXEL: xpos = 30;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_31_PIXEL: xpos = 31;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_63_PIXEL: xpos = 63;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_78_PIXEL: xpos = 78;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_79_PIXEL: xpos = 79;                              break;
    case JUSTIFY_LEFT_WITH_INDENT_98_PIXEL: xpos = 98;                              break;
#if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
    case JUSTIFY_LEFT_WITH_INDENT_35_PIXEL: xpos = 35;                              break;
#endif


    #ifdef MANUAL_ADJUST_ACCELERATOR
    case JUSTIFY_FOR_MANUAL_ADJ_0:
    case JUSTIFY_FOR_MANUAL_ADJ_1:
    case JUSTIFY_FOR_MANUAL_ADJ_2:
    case JUSTIFY_FOR_MANUAL_ADJ_3:
    case JUSTIFY_FOR_MANUAL_ADJ_4:
    case JUSTIFY_FOR_MANUAL_ADJ_5:
    case JUSTIFY_FOR_MANUAL_ADJ_6:
    case JUSTIFY_FOR_MANUAL_ADJ_7:
    case JUSTIFY_FOR_MANUAL_ADJ_8:
    case JUSTIFY_FOR_MANUAL_ADJ_9:
    case JUSTIFY_FOR_MANUAL_ADJ_10:
    case JUSTIFY_FOR_MANUAL_ADJ_11:
    case JUSTIFY_FOR_MANUAL_ADJ_12:
    case JUSTIFY_FOR_MANUAL_ADJ_13:
    case JUSTIFY_FOR_MANUAL_ADJ_14:
    case JUSTIFY_FOR_MANUAL_ADJ_15:
    case JUSTIFY_FOR_MANUAL_ADJ_16:
    case JUSTIFY_FOR_MANUAL_ADJ_17:
    case JUSTIFY_FOR_MANUAL_ADJ_18:
    case JUSTIFY_FOR_MANUAL_ADJ_19:
    case JUSTIFY_FOR_MANUAL_ADJ_20:
    case JUSTIFY_FOR_MANUAL_ADJ_21:
    case JUSTIFY_FOR_MANUAL_ADJ_22:
    case JUSTIFY_FOR_MANUAL_ADJ_23:
    case JUSTIFY_FOR_MANUAL_ADJ_24:
    case JUSTIFY_FOR_MANUAL_ADJ_25:
    case JUSTIFY_FOR_MANUAL_ADJ_26:
    case JUSTIFY_FOR_MANUAL_ADJ_27:
    case JUSTIFY_FOR_MANUAL_ADJ_28:
    case JUSTIFY_FOR_MANUAL_ADJ_29:
    case JUSTIFY_FOR_MANUAL_ADJ_30:
    case JUSTIFY_FOR_MANUAL_ADJ_31:
    case JUSTIFY_FOR_MANUAL_ADJ_32:
    case JUSTIFY_FOR_MANUAL_ADJ_33:
    case JUSTIFY_FOR_MANUAL_ADJ_34:
    case JUSTIFY_FOR_MANUAL_ADJ_35:
    case JUSTIFY_FOR_MANUAL_ADJ_36:
    case JUSTIFY_FOR_MANUAL_ADJ_37:
    case JUSTIFY_FOR_MANUAL_ADJ_38:
    case JUSTIFY_FOR_MANUAL_ADJ_39:
    case JUSTIFY_FOR_MANUAL_ADJ_40:
    case JUSTIFY_FOR_MANUAL_ADJ_41:
    case JUSTIFY_FOR_MANUAL_ADJ_42:
    case JUSTIFY_FOR_MANUAL_ADJ_43:
    case JUSTIFY_FOR_MANUAL_ADJ_44:
    case JUSTIFY_FOR_MANUAL_ADJ_45:
    case JUSTIFY_FOR_MANUAL_ADJ_46:
    case JUSTIFY_FOR_MANUAL_ADJ_47:
    case JUSTIFY_FOR_MANUAL_ADJ_48:
    case JUSTIFY_FOR_MANUAL_ADJ_49:
    case JUSTIFY_FOR_MANUAL_ADJ_50:
    case JUSTIFY_FOR_MANUAL_ADJ_51:
    case JUSTIFY_FOR_MANUAL_ADJ_52:
    case JUSTIFY_FOR_MANUAL_ADJ_53:
    case JUSTIFY_FOR_MANUAL_ADJ_54:
    case JUSTIFY_FOR_MANUAL_ADJ_55:
    case JUSTIFY_FOR_MANUAL_ADJ_56:
    case JUSTIFY_FOR_MANUAL_ADJ_57:
    case JUSTIFY_FOR_MANUAL_ADJ_58:
    case JUSTIFY_FOR_MANUAL_ADJ_59:
    case JUSTIFY_FOR_MANUAL_ADJ_60:
    case JUSTIFY_FOR_MANUAL_ADJ_61:
    case JUSTIFY_FOR_MANUAL_ADJ_62:
    case JUSTIFY_FOR_MANUAL_ADJ_63:
    case JUSTIFY_FOR_MANUAL_ADJ_64:
    case JUSTIFY_FOR_MANUAL_ADJ_65:
    case JUSTIFY_FOR_MANUAL_ADJ_66:
    case JUSTIFY_FOR_MANUAL_ADJ_67:
    case JUSTIFY_FOR_MANUAL_ADJ_68:
    case JUSTIFY_FOR_MANUAL_ADJ_69:         xpos = justify - JUSTIFY_FOR_MANUAL_ADJ_0;  break;
    #endif

    default: /* JUSTIFY_LEFT */             xpos = 0;                               break;


    }

    if ((xpos >= 0) && (xpos <= 126))
    {
        LOCAL_packet_HL[2] = (TUSIGN8)xpos;
    }
    else
    {
        LOCAL_packet_HL[2] = 0;
    }

    if (blank)
    {
        LOCAL_packet_HL[2] |= PRE_BLANK;
    }

    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        if (invert == CHAR_INVERT)
        {
            j = stringStartPosition + invchar;
            LOCAL_packet_HL[j] |= 0x80;
            invert = NO_INVERT;
        }
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (Type_1_SoftwareSupportsExtendedWriteString())
        {
            if (invert == CHAR_INVERT)
            {
                LOCAL_packet_HL[4] = (invchar + 1) | extendedCharacterPage;
                invert = NO_INVERT;
            }
            else
            {
                LOCAL_packet_HL[4] = 0 | extendedCharacterPage;
            }
        }
        else
        {
            if (invert == CHAR_INVERT)
            {
                j = stringStartPosition + invchar;
                LOCAL_packet_HL[j] |= 0x80;
                invert = NO_INVERT;
            }
        }
        #else

        if (invert == CHAR_INVERT)
        {
            LOCAL_packet_HL[4] = (invchar + 1) | extendedCharacterPage;
            invert = NO_INVERT;
        }
        else
        {
            LOCAL_packet_HL[4] = 0 | extendedCharacterPage;
        }
        #endif
    }

    LOCAL_packet_HL[3] = (invert & 0xC0) | font | line;

    j = len + stringStartPosition;

    TUSIGN8 maxTextLen = (TUSIGN8)(MAX_COMMON_HMI_WIDTH - (LOCAL_packet_HL[2] & ~PRE_BLANK)) / c;

    if (len > maxTextLen)
    {
        LOCAL_packet_HL[1] -= (len - maxTextLen);
        j -= (len - maxTextLen);
    }

    // R.J.Arnold   2012-10-09  Klocwork issue suppressed.
    //-----------   ----------  -------------------------
    // ABV.GENERAL: Array 'packet_HL' of size 30 may use index value(s) 30..45
    // due to limit checking and text lengths j can never be greater than 26
    LOCAL_packet_HL[j] = AddChecksum(j);
    j ++;

    return AddPacketToBuffer(j, lastPacket);
}

#if defined(HMI_SUPPORTS_SERVICEPORT) || defined(TYPE_1_IMPLEMENTATION)

const TUSIGN8 CharacterReplacementTable[] =
{
    // 0x80 - 0x8F
    'C',    // C cedilla
    0x11,   // u umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x12,   // e acute      - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'a',    // a circumflex
    0x0F,   // a umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'a',    // a grave
    'a',    // a ring
    0x15,   // c cedilla    - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x14,   // e circumflex - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'e',    // e umlaut
    0x13,   // e grave      - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'i',    // i umlaut
    'i',    // i circumflex
    'i',    // i grave
    0x0C,   // A umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'A',    // A ring

    // 0x90 - 0x9F
    'E',    // E acute
    0x7F,   // ae merged
    0x7F,   // AE merged
    'o',    // o circumflex
    0x10,   // o umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'o',    // o grave
    'u',    // u circumflex
    'u',    // u grave
    'y',    // y umlaut
    0x0D,   // O umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x0E,   // U umlaut     - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'o',    // o slash
    0x7F,   // Ka (Cyrillic) lowercase
    'O',    // O slash
    0x7F,   // Be (Cyrillic) uppercase
    0x7F,   // Be (Cyrillic) lowercase

    // 0xA0 - 0xAF
    'a',    // a acute
    'i',    // i acute
    'o',    // o acute
    'u',    // u acute
    'n',    // n tilde
    'N',    // N tilde
    0x7F,   // Ge (Cyrillic) uppercase
    0x7F,   // Ge (Cyrillic) lowercase
    0x7F,   // De (Cyrillic) uppercase
    0x7F,   // De (Cyrillic) lowercase
    0x7F,   // Zhe (Cyrillic) uppercase
    0x7F,   // Zhe (Cyrillic) lowercase
    0x7F,   // Ze (Cyrillic) uppercase
    0x7F,   // Ze (Cyrillic) lowercase
    0x7F,   // I (Cyrillic) uppercase
    0x7F,   // I (Cyrillic) lowercase

    // 0xB0 - 0xBF
    0x7F,   // Short I (Cyrillic) uppercase
    0x7F,   // Short I (Cyrillic) lowercase
    0x7F,   // El (Cyrillic) uppercase
    0x7F,   // El (Cyrillic) lowercase
    0x7F,   // Em (Cyrillic) lowercase
    'A',    // A acute
    'A',    // A circumflex
    'A',    // A grave
    0x7F,   // Pe (Cyrillic) uppercase
    0x7F,   // Pe (Cyrillic) lowercase
    0x7F,   // U (Cyrillic) uppercase
    0x7F,   // U (Cyrillic) lowercase
    0x7F,   // Ef (Cyrillic) uppercase
    0x7F,   // Ef (Cyrillic) lowercase
    0x7F,   // Tse (Cyrillic) uppercase
    0x7F,   // Tse (Cyrillic) lowercase

    // 0xC0 - 0xCF
    0x7F,   // Che (Cyrillic) uppercase
    0x7F,   // Che (Cyrillic) lowercase
    0x7F,   // Sha (Cyrillic) uppercase
    0x7F,   // Sha (Cyrillic) lowercase
    0x7F,   // Shcha (Cyrillic) uppercase
    0x7F,   // Shcha (Cyrillic) lowercase
    'a',    // a tilde
    'A',    // A tilde
    0x7F,   // Yer (Cyrillic) uppercase
    0x7F,   // Yer (Cyrillic) lowercase
    0x7F,   // Yery (Cyrillic) uppercase
    0x7F,   // Yery (Cyrillic) lowercase
    0x7F,   // Soft sign (Cyrillic) uppercase
    0x7F,   // Soft sign (Cyrillic) lowercase
    0x7F,   // E (Cyrillic) uppercase
    0x7F,   // E (Cyrillic) lowercase

    // 0xD0 - 0xDF
    0x7F,   // Yu (Cyrillic)
    0x7F,   // Yu (Cyrillic)
    'E',    // E circumflex
    'E',    // E umlaut
    'E',    // E grave
    '?',    // En (Cyrillic) lowercase
    'I',    // I acute
    'I',    // I circumflex
    'I',    // I umlaut
    0x7F,   // Ya (Cyrillic) uppercase
    0x7F,   // Ya (Cyrillic) lowercase
    'A',    // A Ogonek
    'a',    // a Ogonek
    0x7F,   // Ve (Cyrillic) lowercase
    'I',    // I grave
    0x7F,   // Te (Cyrillic) lowercase

    // 0xE0 - 0xEF
    'O',    // O acute
    0x1C,   // Beta / scharfes S    - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'O',    // O circumflex
    'O',    // O grave
    'o',    // o tilde
    'O',    // O tilde
    0x18,    // mu                  - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'C',    // C acute
    'c',    // c acute
    'U',    // U acute
    'U',    // U circumflex
    'U',    // U grave
    'y',    // y acute
    'Y',    // Y acute
    'E',    // E Ogonek
    'e',    // e Ogonek

    // 0xF0 - 0xFF
    'L',    // L stroke
    'l',    // l stroke
    'N',    // N acute
    'n',    // n acute
    'S',    // S acute
    's',    // s acute
    'Z',    // Z acute
    'z',    // z acute
    0x0B,   // "?                  - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'Z',    // Z dotted
    'z',    // z dotted
    'Y',    // Y umlaut
    0x1A,   // superscript 3        - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x19,   // superscript 2        - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    ' ',    // spare
    ' ',    // spare
};

const TUSIGN8 CharacterReplacementFromLatin1Table[] =
{
    // 0xA0 - 0xAF
    ' ',    // non breaking space
    0x7f,   // inverted exclamation
    0x7f,   // cent (currency)
    0x7f,   // pound Sign
    0x7f,   // currency symbol
    0x7f,   // yen symbol
    '|',    // vertical bar
    0x7f,   // section sign
    0x7f,   // umlaut
    0x7f,   // copyright symbol
    0x7f,   // superscript a
    0x7f,   // guillemets <<
    0x7f,   // negation (logical complement) symbol
    '-',    // soft hyphen
    0x7f,   // registered trademark symbol
    0x7f,   // Macron

    // 0xB0 - 0xBF
    0x0B,   // "?                  - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x7f,   // plus/minus sign
    0x19,   // superscript 2        - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x1A,   // superscript 3        - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x7f,   // acute
    0x18,   // mu                   - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x7f,   // pilcrow
    0x7f,   // interpunct
    0x7f,   // cedilla
    0x7f,   // superscript 1
    0x7f,   // superscript o
    0x7f,   // guillemets >>
    0x7f,   // quarter
    0x7f,   // half
    0x7f,   // three quarters
    0x7f,   // inverted question mark

    // 0xC0 - 0xCF
    'A',    // A grave
    'A',    // A acute
    'A',    // A circumflex
    'A',    // A tilde
    0x0C,   // A umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'A',    // A ring
    0x7f,   // AE merged
    'C',    // C cedilla
    'E',    // E grave
    'E',    // E acute
    'E',    // E cedilla
    'E',    // E umlaut
    'I',    // I grave
    'I',    // I acute
    'I',    // I circumflex
    'I',    // I umlaut

    // 0xD0 - 0xDF
    0x7f,   // uppercase Eth
    'N',    // N tilde
    'O',    // O grave
    'O',    // O acute
    'O',    // O circumflex
    'O',    // O tilde
    0x0D,   // O umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'x',    // multiplication symbol
    'O',    // O slash
    'U',    // U grave
    'U',    // U acute
    'U',    // U circumflex
    0x0E,   // U umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'Y',    // Y acute
    0x7f,   // thorn
    0x1C,   // Beta / scharfes S    - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL

    // 0xE0 - 0xEF
    'a',    // a grave
    'a',    // a acute
    'a',    // a circumflex
    'a',    // a tilde
    0x0F,   // a umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'a',    // a ring
    0x7f,   // ae merged
    0x15,   // c cedilla            - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x13,   // e grave              - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x12,   // e acute              - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x14,   // e circumflex         - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'e',    // e umlaut
    'i',    // i grave
    'i',    // i acute
    'i',    // i circumflex
    'i',    // i umlaut

    // 0xF0 - 0xFF
    0x7F,   // lowercase Eth
    'n',    // n tilde
    'o',    // o grave
    'o',    // o acute
    'o',    // o circumflex
    'o',    // o tilde
    0x10,   // o umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    0x7F,   // division symbol
    'o',    // o slash
    'u',    // u grave
    'u',    // u acute
    'u',    // u circumflex
    0x11,   // u umlaut             - USE SPECIAL CHARACTER SUPPORTED BY TYPE 1 LOW LEVEL
    'y',    // y acute
    0x7f,   // thorn
    'y'     // y umlaut
};

void convertExtendedCharacterSet(TUSIGN8 *text,TUSIGN8 extendedCharacterPage, TUSIGN8 len)
{
    // Function converts characters that LL HMI (or Remote HMI) will not recognise to something that will be recognised
    TUSIGN8 character;
    TUSIGN8 i;

    for (i = 0; i < len; i++)
    {
        character = text[i];

        if (extendedCharacterPage == HMI_EXTENDED_CHARACTER_PAGE_LATIN1)
        {
            if (character < 0x20)
            {
                character = 0x7F;
            }
            else if (character < 0x80)
            {
                character = character;
            }
            else if (character < 0xA0)
            {
                character = 0x7F;
            }
            else
            {
                character = character - 0xA0;
                character = CharacterReplacementFromLatin1Table[character];
            }
        }

        if (extendedCharacterPage == HMI_EXTENDED_CHARACTER_PAGE_TURKISH)
        {
            switch (character)
            {
            case 0xA6:  character = 'g';    break;      //   g breve -> g
            case 0xA7:  character = 'i';    break;      // i dotless -> i
            case 0xA8:  character = 's';    break;      // s cedilla -> s
            case 0xA9:  character = 'I';    break;      //  I dotted -> I
            case 0xAA:  character = 'G';    break;      //   G breve -> G
            case 0xAB:  character = 'G';    break;      // S cedilla -> S
            default:                        break;
            }
        }

        if (extendedCharacterPage == HMI_EXTENDED_CHARACTER_PAGE_KATAKANA)
        {
            if ((character >= 0xA1) && (character <= 0xDF))
            {
                character = 0x7F;
            }
        }

        if (extendedCharacterPage == HMI_EXTENDED_CHARACTER_PAGE_RUSSIAN)
        {
            if ((character >= 0x8A) && (character <= 0x9A))
            {
                character = 0x7F;
            }
        }

        if (character >= 0x80)
        {
            character = character - 0x80;
            character = CharacterReplacementTable[character];
        }

        text[i] = character;
    }
}
#endif

#ifdef HMI_SUPPORTS_ASIAN
// No restriction on the optimization of this function
TUSIGN8 BuildWriteString_w(const TUSIGN8 *text, TUSIGN8 line, TUSIGN8 justify, TUSIGN8 invert, TUSIGN8 blank, TUSIGN8 minLength, TUSIGN8 maxLength, TUSIGN8 lastPacket)
{
    #if defined(TYPE_5_IMPLEMENTATION) && defined(DISPLAY_SIZE_D)
    if(line == SOFTKEY_PAGE)
    {
        minLength = 1;
    }
    #endif

    TINT16 xpos = 0;

    if (maxLength > 10)
    {
        maxLength = 10;
    }

    if (minLength > maxLength)
    {
        minLength = maxLength;
    }

    #ifndef SINGLE_BYTE_ASIAN
    maxLength *= 2;
    minLength *= 2;
    #endif

    TUSIGN8 len = (TUSIGN8)(strlen((const char *)text));

    TINT16 nCh;     // number of characters that will be extracted from the supplied text
    TINT16 nSp;     // number of spaces that must be added to bring length up to minimum
    TINT16 nLSp;    // number of leading spaces
    TINT16 nTSp;    // number of trailing spaces

    TUSIGN8 i;      // counter - counts number of characters/spaces added to packet
    TUSIGN8 j;      // index   - position in packet where next character/space is to be added

    // Calculate the number of characters that will be extracted from the supplied text
    nCh = len;
    if (nCh > maxLength)
    {
        nCh = maxLength;
    }

    // Calculate the number of spaces that must be added to bring length up to minimum
    nSp = minLength - len;
    if (nSp < 0)
    {
        nSp = 0;
    }

    switch (justify)
    {
    case JUSTIFY_RIGHT:
    case JUSTIFY_RIGHT_WITH_INDENT_1:
    case JUSTIFY_RIGHT_WITH_INDENT_49_PIXEL:
        nLSp = nSp;
        nTSp = 0;

        break;

    case JUSTIFY_LEFT:
    case JUSTIFY_LEFT_WITH_INDENT_1_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_2:
    case JUSTIFY_LEFT_WITH_INDENT_19_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_30_PIXEL:
    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    case JUSTIFY_LEFT_WITH_INDENT_6_PIXEL:
    case JUSTIFY_LEFT_WITH_INDENT_12_PIXEL:
    #endif
        nLSp = 0;
        nTSp = nSp;
        break;

    default: /*case JUSTIFY_CENTRE:*/
        nLSp = nSp/2;
        #ifndef SINGLE_BYTE_ASIAN
        if ((nLSp & 0x0001) != 0)
        {
            nLSp --;
        }
        #endif
        nTSp = nSp - nLSp;
        break;
    }

    j = 4;
    // Add leading spaces
    for (i = 0; i < nLSp; i ++)
    {
        #ifndef SINGLE_BYTE_ASIAN
        if ((i & 0x01) == 0)
        {
            LOCAL_packet_HL[j] = ASIAN_BLANK_MSBYTE;
        }
        else
        #endif
        {
            LOCAL_packet_HL[j] = ASIAN_BLANK_LSBYTE;
        }
        j ++;
        LOCAL_packet_HL[j] = 0x00;
    }

    // Extract characters from supplied text
    for (i = 0; i < nCh; i ++)
    {
        LOCAL_packet_HL[j] = text[i];
        j ++;
        LOCAL_packet_HL[j] = 0x00;
    }

    // Add trailing spaces


    for (i = 0; i < nTSp; i ++)
    {

        #ifndef SINGLE_BYTE_ASIAN
        if ((i & 0x01) == 0)
        {
            LOCAL_packet_HL[j] = ASIAN_BLANK_MSBYTE;
        }
        else
        #endif
        {
            LOCAL_packet_HL[j] = ASIAN_BLANK_LSBYTE;
        }
        j ++;
        LOCAL_packet_HL[j] = 0x00;

    }


    len = (TUSIGN8)(strlen((char *)&LOCAL_packet_HL[4]));

    LOCAL_packet_HL[0] = WRITE_STRING_ASIAN;
    LOCAL_packet_HL[1] = len + 2;

    switch (justify)
    {
    case JUSTIFY_RIGHT:                         xpos = (MAX_COMMON_HMI_WIDTH) - (FONT_ASIAN_WIDTH_2 * len);                     break;
    case JUSTIFY_RIGHT_WITH_INDENT_1:           xpos = (MAX_COMMON_HMI_WIDTH - FONT_ASIAN_WIDTH) - (FONT_ASIAN_WIDTH_2 * len);  break;
    case JUSTIFY_RIGHT_WITH_INDENT_49_PIXEL:    xpos = (MAX_COMMON_HMI_WIDTH - 49) - (FONT_ASIAN_WIDTH_2 * len);                break;
    case JUSTIFY_CENTRE:                        xpos = (MAX_COMMON_HMI_WIDTH/2) - ((FONT_ASIAN_WIDTH_2/2) * len);               break;  //xpos = 63 - ((character width/2) * len);
    case JUSTIFY_LEFT_WITH_INDENT_1_PIXEL:      xpos = 1;                                                                       break;
    case JUSTIFY_LEFT_WITH_INDENT_2:            xpos = 2 * FONT_ASIAN_WIDTH;                                                    break;
    case JUSTIFY_LEFT_WITH_INDENT_19_PIXEL:     xpos = 19;                                                                      break;
    case JUSTIFY_LEFT_WITH_INDENT_30_PIXEL:     xpos = 30;                                                                      break;
    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    case JUSTIFY_LEFT_WITH_INDENT_6_PIXEL:      xpos = 6;                                                                       break;
    case JUSTIFY_LEFT_WITH_INDENT_12_PIXEL:     xpos = 12;                                                                      break;
    #endif
    default: /* JUSTIFY_LEFT */                 xpos = 0;                                                                       break;
    }

    if ((xpos >= 0) && (xpos <= 126))
    {
        LOCAL_packet_HL[2] = (TUSIGN8)xpos;
    }
    else
    {
        LOCAL_packet_HL[2] = 0;
    }

    if (blank)
    {
        LOCAL_packet_HL[2] |= PRE_BLANK;
    }

    if (invert == CHAR_INVERT)
    {
        invert = NO_INVERT;
    }

    LOCAL_packet_HL[3] = (invert & 0xC0) | (LOCAL_AsianCharacterSet << 3) & 0x38 | (line & 0x07);

    j = len + 4;
    LOCAL_packet_HL[j] = AddChecksum(j);
    j ++;
    return AddPacketToBuffer(j, lastPacket);
}
#endif

// No restriction on the optimization of this function
TUSIGN8 BuildCustomCharacterString(const TUSIGN8 *pCustomCharacter, TUSIGN8 nChars, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 nRepeats, TUSIGN8 lastPacket)
{
    TUSIGN8 i,j;

    LOCAL_packet_HL[0] = DRAW_AND_REPEAT_CUSTOM_CHAR;
    LOCAL_packet_HL[1] = nChars + 3;
    LOCAL_packet_HL[2] = x_pos;
    LOCAL_packet_HL[3] = y_pos & 0x07;
    LOCAL_packet_HL[4] = nRepeats;

    for (i = 0,j = 5; i < nChars; i++,j++)
    {
        LOCAL_packet_HL[j] = pCustomCharacter[i];
    }

    j = 5 + nChars;
    LOCAL_packet_HL[j] = AddChecksum(j);

    return AddPacketToBuffer((TUSIGN8)(6 + nChars), lastPacket);
}

// No restriction on the optimization of this function
TUSIGN8 BuildWriteIcon(TUSIGN8 iconID, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 lastPacket)
{
    LOCAL_packet_HL[0] = WRITE_ICON;
    LOCAL_packet_HL[1] = 3;
    LOCAL_packet_HL[2] = x_pos;
    LOCAL_packet_HL[3] = y_pos & 0x07;
    LOCAL_packet_HL[4] = iconID;
    LOCAL_packet_HL[5] = AddChecksum(5);
    return AddPacketToBuffer(6,lastPacket);
}

// No restriction on the optimization of this function
TUSIGN8 BuildWriteArray(const TUSIGN8 *array, TUSIGN8 arraySize, TUSIGN8 x_pos, TUSIGN8 y_pos, TUSIGN8 lastPacket)
{
    TUSIGN8 chksumPosn;

    if (arraySize > 25) {arraySize = 25;}

    LOCAL_packet_HL[0] = WRITE_ARRAY;
    LOCAL_packet_HL[1] = arraySize + 2;
    LOCAL_packet_HL[2] = x_pos;
    LOCAL_packet_HL[3] = y_pos & 0x07;
    // Return value of "memcpy()" ignored
    //lint -e{534} Roger Arnold 2005-2-01
    memcpy((void *)&LOCAL_packet_HL[4], (void const *)array, (size_t)arraySize);

    chksumPosn = arraySize + 4;
    LOCAL_packet_HL[chksumPosn] = AddChecksum(chksumPosn);
    return AddPacketToBuffer((TUSIGN8)(chksumPosn + 1), lastPacket);
}

// No restriction on the optimization of this function
TUSIGN8 BuildReadSwitches(TUSIGN8 lastPacket)
{
    LOCAL_packet_HL[0] = READ_SWITCHES;
    LOCAL_packet_HL[1] = 1;
    LOCAL_packet_HL[2] = 0;
    LOCAL_packet_HL[3] = AddChecksum(3);
    return AddPacketToBuffer(4, lastPacket);
}
#ifdef SUPPORT_SHMI
TUSIGN8 BuildReadSwitches_SHMI(TUSIGN8 lastPacket)
{    
    LOCAL_packet_HL[0] = READ_SWITCHES; 
    return AddPacketToBuffer(1, lastPacket);
}
#endif
// No restriction on the optimization of this function
TUSIGN8 BuildSetupDisplay(TUSIGN8 f, TUSIGN8 lastPacket)
{
    LOCAL_packet_HL[0] = SETUP_DISPLAY;
    LOCAL_packet_HL[1] = 1;
    LOCAL_packet_HL[2] = f;
    LOCAL_packet_HL[3] = AddChecksum(3);
    return AddPacketToBuffer(4, lastPacket);
}

// No restriction on the optimization of this function
TUSIGN8 BuildSetContrast(TUSIGN8 contrast, TUSIGN8 lastPacket)
{
//    contrast = 50;
    LOCAL_last_contrast = contrast;
    LOCAL_packet_HL[0] = SET_CONTRAST;
    LOCAL_packet_HL[1] = 1;

    // LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 63) / 100);
    // Contrast adjustment on COG controller is 0..63  where 0 gives "white" & 63 gives "black"
    // We are only using a range of 40% to 80% of the COG controller range 40% = 25, 80% = 51
    // Therefore contrast setting of  0% -> 25
    //                              100% -> 51
    //
    LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 26) / 100) + 25;
    LOCAL_packet_HL[3] = AddChecksum(3);
    return AddPacketToBuffer(4, lastPacket);
}

TUSIGN8 BuildV2Init(TUSIGN8 contrast, TUSIGN8 lastPacket)
{
    LOCAL_last_contrast = contrast;
    LOCAL_packet_HL[0] = COMMS_V2_INIT;
    LOCAL_packet_HL[1] = 3;

    // LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 63) / 100);
    // Contrast adjustment on COG controller is 0..63  where 0 gives "white" & 63 gives "black"
    // We are only using a range of 40% to 80% of the COG controller range 40% = 25, 80% = 51
    // Therefore contrast setting of  0% -> 25
    //                              100% -> 51
    //
    LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 26) / 100) + 25;
    LOCAL_packet_HL[3] = (TUSIGN8)(HIGH_LEVEL_VERSION_INFO_TUSIGN16);
    LOCAL_packet_HL[4] = (TUSIGN8)(HIGH_LEVEL_VERSION_INFO_TUSIGN16 >> 8) ;
    LOCAL_packet_HL[5] = AddChecksum(5);
    return AddPacketToBuffer(6, lastPacket);
}

#ifdef SUPPORT_SHMI

TUSIGN8 BuildV2Init_s(TUSIGN8 contrast, TUSIGN8 lastPacket)
{
    LOCAL_last_contrast = contrast;
    LOCAL_packet_HL[0] = COMMS_V2_INIT;
    LOCAL_packet_HL[1] = 5; 

    // LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 63) / 100);
    // Contrast adjustment on COG controller is 0..63  where 0 gives "white" & 63 gives "black"
    // We are only using a range of 40% to 80% of the COG controller range 40% = 25, 80% = 51
    // Therefore contrast setting of  0% -> 25
    //                              100% -> 51
    //
    LOCAL_packet_HL[2] = (TUSIGN8)((((TINT16)contrast) * 26) / 100) + 25;    
    LOCAL_packet_HL[3] = (TUSIGN8)((TUSIGN16)HIGH_LEVEL_VERSION_INFO_TUSIGN16);
    LOCAL_packet_HL[4] = (TUSIGN8)((TUSIGN16)HIGH_LEVEL_VERSION_INFO_TUSIGN16 >> 8) ;
    LOCAL_packet_HL[5] = SHMI_MAJOR_VER;
    LOCAL_packet_HL[6] = SHMI_MINOR_VER;
    LOCAL_packet_HL[7] = AddChecksum(7);
    return AddPacketToBuffer(8, lastPacket);
}

#endif

#ifdef TYPE_1_IMPLEMENTATION
// No restriction on the optimization of this function
TUSIGN8 BuildLowLevelInfo(TUSIGN8 lastPacket)
{
    LOCAL_packet_HL[0] = LOW_LEVEL_INFO;
    LOCAL_packet_HL[1] = 1;
    LOCAL_packet_HL[2] = 0;
    LOCAL_packet_HL[3] = AddChecksum(3);
    return AddPacketToBuffer(4, lastPacket);
}
  #ifdef SUPPORT_SHMI
TUSIGN8 BuildLowLevelInfo_s(TUSIGN8 lastPacket)
{
    TUSIGN8 deviceSN[14];
    TUSIGN8 hartPollAddr;
    TUSIGN8 hartMfrID;
    TUSIGN8 hartDevType;
    TUSIGN8 hartDevNum[3];    
    TUSIGN8 strLibVer, stdLibVer;
    
    
    (void)elettronicServices_Get(ELETTRONICSERVICES_IDX_deviceSerialNumber, WHOLE_OBJECT, &deviceSN[0]);
    (void)hart_Get(HART_IDX_mfrId, WHOLE_OBJECT, &hartMfrID);
    (void)hart_Get(HART_IDX_mfrDeviceType, WHOLE_OBJECT, &hartDevType);
    (void)hart_Get(HART_IDX_deviceIdNumber, WHOLE_OBJECT, &hartDevNum[0]);
    (void)hart_Get(HART_IDX_pollAddr, WHOLE_OBJECT, &hartPollAddr);
    GetStrLibVer(&strLibVer, &stdLibVer);
    
    LOCAL_packet_HL[0] = LOW_LEVEL_INFO;
    LOCAL_packet_HL[1] = 32;
    LOCAL_packet_HL[2] = deviceSN[0];   //deviceSN
    LOCAL_packet_HL[3] = deviceSN[1];   //deviceSN
    LOCAL_packet_HL[4] = deviceSN[2];   //deviceSN
    LOCAL_packet_HL[5] = deviceSN[3];   //deviceSN
    LOCAL_packet_HL[6] = deviceSN[4];   //deviceSN
    LOCAL_packet_HL[7] = deviceSN[5];   //deviceSN
    LOCAL_packet_HL[8] = deviceSN[6];   //deviceSN
    LOCAL_packet_HL[9] = deviceSN[7];   //deviceSN
    LOCAL_packet_HL[10] = deviceSN[8];    //deviceSN
    LOCAL_packet_HL[11] = deviceSN[9];    //deviceSN
    LOCAL_packet_HL[12] = deviceSN[10];   //deviceSN
    LOCAL_packet_HL[13] = deviceSN[11];   //deviceSN
    LOCAL_packet_HL[14] = deviceSN[12];   //deviceSN
    LOCAL_packet_HL[15] = deviceSN[13];   //deviceSN
    LOCAL_packet_HL[16] = CUS_DT_HART_VT5;       //device type
    LOCAL_packet_HL[17] = CUS_STRING_LIB_HART_VT5;    //stringLibName,  1--General, 2-LMTHART,....
    LOCAL_packet_HL[18] = strLibVer;    //stringLibVersion
    LOCAL_packet_HL[19] = hartMfrID;               
    LOCAL_packet_HL[20] = hartDevType;           
    LOCAL_packet_HL[21] = hartDevNum[0];               
    LOCAL_packet_HL[22] = hartDevNum[1];                     
    LOCAL_packet_HL[23] = hartDevNum[2];  
    LOCAL_packet_HL[24] = hartPollAddr; 
    LOCAL_packet_HL[25] = CDT_IDX;                        //for   externalDiagnosis, subIdx
    LOCAL_packet_HL[26] = CDT_IDX_cdtDiagnosis>>8;      //index  
    LOCAL_packet_HL[27] = CDT_IDX_cdtDiagnosis;      //index       
    LOCAL_packet_HL[28] = CDT_IDX;                        //for   deviceReset, subIdx
    LOCAL_packet_HL[29] = 0;//CDT_IDX_softwareReset>>8;       //index  for device reset  
    LOCAL_packet_HL[30] = 0;//CDT_IDX_softwareReset;            //index  
    LOCAL_packet_HL[31] = stdLibVer;      //index 
    LOCAL_packet_HL[32] = ASIAN_LANGUAGE_VERSION;
    LOCAL_packet_HL[33] = DTYPE_UNDEFINED;  //adv device Type
    LOCAL_packet_HL[34] = AddChecksum(34);
    return AddPacketToBuffer(35, lastPacket);
    
}
  #endif
#endif
// No restriction on the optimization of this function
TUSIGN8 AddChecksum(TUSIGN8 checksumPosn)
{
    TUSIGN8 sum = 1;
    TUSIGN8 i;

    for (i = 0;  i < checksumPosn; i++)
    {
        sum += LOCAL_packet_HL[i];
    }
    return sum;
}

// No restriction on the optimization of this function
TUSIGN8 AddPacketToBuffer(TUSIGN8 bytesToAdd, TUSIGN8 lastPacket)
{
    TUSIGN8 bufferSize;
    TUSIGN8 packetSize;
    TUSIGN8 returnVal;
    if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_SHMI_STATUS)
    {
        bufferSize = BUFFERSIZE_SMART_HMI;
        packetSize = PACKETSIZE_SMART_HMI;
    }
    else if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_HMI_STATUS)
    {
        bufferSize = PACKETBUFFERSIZE;
        packetSize = PACKET_HL_SIZE;
    }

    if (bytesToAdd <= packetSize)
    {

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if (pPacketManagerLocal == &PacketManagerLocal_2)
        {
            if (LOCAL_packet_HL[0] == READ_SWITCHES)
            {
                readSwitchesIssued_2 = eTRUE;
            }
            else
            {
                readSwitchesIssued_2 = eFALSE;
            }
        }
        else
        #endif
        {
            if (LOCAL_packet_HL[0] == READ_SWITCHES)
            {
                readSwitchesIssued_1 = eTRUE;
            }
            else
            {
                readSwitchesIssued_1 = eFALSE;
            }
        }

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if (pPacketManagerLocal == &PacketManagerLocal_2)
        {
            bufferSize = PACKETBUFFERSIZE_SP;
        }
        #endif

        #ifdef TYPE_1_IMPLEMENTATION

        #ifdef HMI_SUPPORTS_SERVICEPORT
        if ((pPacketManagerLocal == &PacketManagerLocal_1) && (PacketManagerLocal_1.DisplaySoftwareRevision < EARLIEST_TYPE_1_LOW_LEVEL_SUPPORTING_240BYTE_BUFFER))
        #else
        if (LOCAL_DisplaySoftwareRevision < EARLIEST_TYPE_1_LOW_LEVEL_SUPPORTING_240BYTE_BUFFER)
        #endif
        {
            bufferSize = PACKETBUFFERSIZE_BEFORE_240;
        }
        #endif

        if ((LOCAL_packetBufferUsed + bytesToAdd) <= bufferSize)
        {
            if (lastPacket)
            {
                LOCAL_packet_HL[0] += 0x80;                           // modify function code to request reply
                if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_HMI_STATUS)
                {
                    TUSIGN8 chksumPosn = bytesToAdd - 1;
                    LOCAL_packet_HL[chksumPosn] += 0x80;                  // modify checksum accordingly
                }
            }

            LOCAL_lastFunctionCode = LOCAL_packetBufferUsed;
            LOCAL_packetStage ++;


            // R.J.Arnold   2012-10-09  Klocwork issue suppressed.
            //-----------   ----------  -------------------------
            // ABV.GENERAL: Array 'packetBuffer' of size 120 may use index value(s) 120..255
            // data from LOCAL_packet_HL[] will not be copied to LOCAL_packetBuffer[] if it will not fit
            (void)memcpy((void *)&(LOCAL_packetBuffer[LOCAL_packetBufferUsed]),(void const *)&LOCAL_packet_HL[0],(size_t)bytesToAdd);

            LOCAL_packetBufferUsed += bytesToAdd;
            returnVal = 1;
        }
        else
        {
            LOCAL_packetBuffer[LOCAL_lastFunctionCode] += 0x80;             // modify function code to request reply
            if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_HMI_STATUS)
            {
                TUSIGN8 chksumPosn = LOCAL_packetBufferUsed - 1;
                LOCAL_packetBuffer[chksumPosn] += 0x80;                   // modify checksum accordingly
            }
            returnVal = 0;
        }
    }
    else
    {
        returnVal = 0;
    }
    return returnVal;
}

// No restriction on the optimization of this function
TUSIGN8 BuildErrorResponse(void)
{
    TUSIGN8 returnVal = LOW_LEVEL_ERROR_NOT_PRESENT;
/*
        LOW_LEVEL_ERROR_NOT_PRESENT
        LOW_LEVEL_ERROR_IGNORED
        LOW_LEVEL_ERROR_RESPONSE_BUILT
        LOW_LEVEL_ERROR_REBUILD_FRAME

*/
    if ((LOCAL_CommunicationsErrorCode & HW_SWITCH_ERROR)== 0)
    {
        LOCAL_SwitchErrorCount = 0;
    }

    if ((LOCAL_CommunicationsErrorCode & PROCESSING_ERROR) != 0)
    {
        LOCAL_last_contrast = 101;              // set last_contrast to an invalid value to force the contrast to be re-sent
        returnVal = LOW_LEVEL_ERROR_REBUILD_FRAME;
    }
    else if ((LOCAL_CommunicationsErrorCode & COMMUNICATION_ERROR) != 0)
    {
        returnVal = LOW_LEVEL_ERROR_REBUILD_FRAME;
    }
    else if ((LOCAL_CommunicationsErrorCode & HW_DISPLAY_ERROR) != 0)
    {
        LOCAL_packetBufferUsed = 0;
        // packetBufferUsed has been reset to zero
        // The setup display message is only 4 bytes therefore packet_HL will fit into buffer
        // no need to check return value of "BuildSetupDisplay()"
        //lint -e{534} Roger Arnold 2005-2-01
        BuildSetupDisplay(SETUP_RESET_DISPLAY|SETUP_DISPLAY_ON,1);
        returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
    }
    else if ((LOCAL_CommunicationsErrorCode & HW_SWITCH_ERROR) != 0)
    {
        LOCAL_SwitchErrorCount++;
        LOCAL_packetBufferUsed = 0;
        /*  Sequence of operation in the event of a switch error
         1      Re-read switches
         2      Re-read switches
         3      Re-read switches
         4      Blank display
         5      Write "  Switch Error   " message
         6      Write "Resetting Display" message
         7..13  Do nothing
        14+     Reset Display
        */

        if (LOCAL_SwitchErrorCount <= 3)
        {
            // packetBufferUsed has been reset to zero
            // The read switches message is only 4 bytes therefore packet_HL will fit into buffer
            // no need to check return value of "BuildReadSwitches()"
            //lint -e{534} Roger Arnold 2005-2-01
            BuildReadSwitches(1);
            LOCAL_packetStage = 0;
            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
        else if (LOCAL_SwitchErrorCount == 4)
        {
            // packetBufferUsed has been reset to zero
            // The blank area message is only 4 bytes therefore packet_HL will fit into buffer
            // no need to check return value of "BuildBlankArea()"
            //lint -e{534} Roger Arnold 2005-2-01
            BuildBlankArea(0,7,1);
            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
        else if (LOCAL_SwitchErrorCount == 5)
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_SWITCH_ERROR;
            // packetBufferUsed has been reset to zero
            // The write string message cannot be more than 25 bytes therefore packet_HL will fit into buffer
            // no need to check return value of "BuildBlankArea()"
            //lint -e{534} Roger Arnold 2005-2-01
            BuildWriteString(FrameTableGetString(LOCAL_stringtable_text_id),
                             HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,
                             3,
                             JUSTIFY_CENTRE,
                             FONT_0,
                             NO_INVERT,
                             0,
                             0,
                             FONT1_MAX_CHARS,
                             1);

            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
        else if (LOCAL_SwitchErrorCount == 6)
        {
            LOCAL_stringtable_text_id.attribIdx = IDS_RESETTING_HW;
            // packetBufferUsed has been reset to zero
            // The write string message cannot be more than 25 bytes therefore packet_HL will fit into buffer
            // no need to check return value of "BuildBlankArea()"
            //lint -e{534} Roger Arnold 2005-2-01
            BuildWriteString(FrameTableGetString(LOCAL_stringtable_text_id),
                             HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN,
                             4,
                             JUSTIFY_CENTRE,
                             FONT_0,
                             NO_INVERT,
                             0,
                             0,
                             FONT1_MAX_CHARS,
                             1);

            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
        else if (LOCAL_SwitchErrorCount < 14)
        {
            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
        else
        {
            // packetBufferUsed has been reset to zero
            // The setup display message is only 4 bytes therefore packet_HL will fit into buffer
            // no need to check return value of "BuildSetupDisplay()"
            //lint -e{534} Roger Arnold 2005-2-01
            BuildSetupDisplay(SETUP_RESET_DISPLAY|SETUP_DISPLAY_ON,1);
            returnVal = LOW_LEVEL_ERROR_RESPONSE_BUILT;
        }
    }
    else
    {
        returnVal = LOW_LEVEL_ERROR_NOT_PRESENT;
    }
    return returnVal;
}

// No restriction on the optimization of this function
#ifdef SUPPORT_SHMI
TUSIGN8 const *PacketManagerGetEngUnits_s(T_SUBSYSTEM_IDX subsysIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx, TUSIGN8 type, E_STRING_TYPE* pStringType)
{
    TEXT_IDENTIFIER_TYPE text_id;
    
    *pStringType = STRING_TYPE_EUROPEAN;

    text_id.ssIdx = HMI_IDX;
    text_id.objIdx = type;
    text_id.attribIdx = 0;

    if (type == USE_NEXT_ATTRIBUTE)
    {
        TUSIGN8     unitTable = USE_NEXT_ATTRIBUTE;

        TUSIGN16 getResult = AdjusterGetAttribute((TUSIGN16)subsysIdx,
                                                  (TUSIGN16)objIdx,
                                                  (TUSIGN8)(attribIdx + 1),
                                                  (void FAST *)&unitTable);

        if ((unitTable < USE_UOM_1) || (unitTable >= USE_NEXT_ATTRIBUTE) || (getResult != (TUSIGN16)OK))
        {
            type = USE_NEXT_ATTRIBUTE;
        }
        else
        {
            type = unitTable;
        }
    }

    if (type == FIXED_TEXT_UNITS)
    {
        text_id.ssIdx = HMI_IDX;
        text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
        #if MAX_STRINGS > 255
            text_id.attribIdx = (TUSIGN16)(attribIdx << 8) + (TUSIGN16)objIdx;
        #else
            text_id.attribIdx = objIdx;
        #endif
    }
    else if (type < USE_NEXT_ATTRIBUTE)
    {
        ADJUST_DATA t_val;
        TUSIGN8     t_type;

        if(AdjusterGetEnumValue((TUSIGN16)subsysIdx,
                                (TUSIGN16)objIdx,
                                (TUSIGN8)attribIdx,
                                &t_val,
                                &t_type) != OK)
        {
            text_id.ssIdx = HMI_IDX;
            text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
            text_id.attribIdx = IDS_BLANK;
        }
        else
        {
            if (t_type == ST_E8)
            {

                text_id.ssIdx = HMI_IDX;
                text_id.objIdx = SS_HMI_OBJ_STRINGTABLE - type; // units tables are defined as "SS_HMI_OBJ_STRINGTABLE - x" where x = string table number
                text_id.attribIdx = t_val.data_8;
            }

            #ifdef HMI_SUPPORTS_16BIT_ENUMS
            #ifdef SIZE_ST_E16
            else if ((t_type == ST_U16) || (t_type == ST_E16))
            #else
            else if (t_type == ST_U16)
            #endif
            {
                text_id.ssIdx = HMI_IDX;
                text_id.objIdx = SS_HMI_OBJ_STRINGTABLE - type; // units tables are defined as "SS_HMI_OBJ_STRINGTABLE - x" where x = string table number
                text_id.attribIdx = t_val.data_16;
            }
            #endif
            else
            {
            text_id.ssIdx = HMI_IDX;
            text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
            text_id.attribIdx = IDS_BLANK;
            }
        }
    }
    else
    {
        text_id.ssIdx = HMI_IDX;
        text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
        text_id.attribIdx = IDS_BLANK;
    }
    
    return FrameTableGetStringSpecificLanguage_s(pStringType, text_id, HMI_LANGUAGE_ENGLISH);
    
}
#endif

// No restriction on the optimization of this function
TUSIGN8 const *PacketManagerGetEngUnits(T_SUBSYSTEM_IDX subsysIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx, TUSIGN8 type)
{
    TEXT_IDENTIFIER_TYPE text_id;

    text_id.ssIdx = HMI_IDX;
    text_id.objIdx = type;
    text_id.attribIdx = 0;

    if (type == USE_NEXT_ATTRIBUTE)
    {
        TUSIGN8     unitTable = USE_NEXT_ATTRIBUTE;

        TUSIGN16 getResult = AdjusterGetAttribute((TUSIGN16)subsysIdx,
                                                  (TUSIGN16)objIdx,
                                                  (TUSIGN8)(attribIdx + 1),
                                                  (void FAST *)&unitTable);

        if ((unitTable < USE_UOM_1) || (unitTable >= USE_NEXT_ATTRIBUTE) || (getResult != (TUSIGN16)OK))
        {
            type = USE_NEXT_ATTRIBUTE;
        }
        else
        {
            type = unitTable;
        }
    }

    if (type == FIXED_TEXT_UNITS)
    {
        text_id.ssIdx = HMI_IDX;
        text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
        #if MAX_STRINGS > 255
            text_id.attribIdx = (TUSIGN16)(attribIdx << 8) + (TUSIGN16)objIdx;
        #else
            text_id.attribIdx = objIdx;
        #endif
    }
    else if (type < USE_NEXT_ATTRIBUTE)
    {
        ADJUST_DATA t_val;
        TUSIGN8     t_type;

        if(AdjusterGetEnumValue((TUSIGN16)subsysIdx,
                                (TUSIGN16)objIdx,
                                (TUSIGN8)attribIdx,
                                &t_val,
                                &t_type) != OK)
        {
            text_id.ssIdx = HMI_IDX;
            text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
            text_id.attribIdx = IDS_BLANK;
        }
        else
        {
            if (t_type == ST_E8)
            {

                text_id.ssIdx = HMI_IDX;
                text_id.objIdx = SS_HMI_OBJ_STRINGTABLE - type; // units tables are defined as "SS_HMI_OBJ_STRINGTABLE - x" where x = string table number
                text_id.attribIdx = t_val.data_8;
            }

            #ifdef HMI_SUPPORTS_16BIT_ENUMS
            #ifdef SIZE_ST_E16
            else if ((t_type == ST_U16) || (t_type == ST_E16))
            #else
            else if (t_type == ST_U16)
            #endif
            {
                text_id.ssIdx = HMI_IDX;
                text_id.objIdx = SS_HMI_OBJ_STRINGTABLE - type; // units tables are defined as "SS_HMI_OBJ_STRINGTABLE - x" where x = string table number
                text_id.attribIdx = t_val.data_16;
            }
            #endif
            else
            {
            text_id.ssIdx = HMI_IDX;
            text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
            text_id.attribIdx = IDS_BLANK;
            }
        }
    }
    else
    {
        text_id.ssIdx = HMI_IDX;
        text_id.objIdx = SS_HMI_OBJ_STRINGTABLE;
        text_id.attribIdx = IDS_BLANK;
    }
    return FrameTableGetString(text_id);
}

#ifdef HMI_SUPPORTS_CUSTOM_UNITS
void PacketManagerRemoveCustomfromUnits(TUSIGN8 *txt)
{
    TUSIGN8 i = (TUSIGN8)strlen((const char *)txt);

    if (i >= 8)
    {
        i -= 8;

        if ((txt[i]   == '*')
         && (txt[i+1] == 'C')
         && (txt[i+2] == 'U')
         && (txt[i+3] == 'S')
         && (txt[i+4] == 'T')
         && (txt[i+5] == 'O')
         && (txt[i+6] == 'M')
         && (txt[i+7] == '*'))
        {
            txt[i] = 0x00;
        }
    }
}
#endif


// No restriction on the optimization of this function
void LimitTextLength(TUSIGN8 *txt, TUSIGN8 maxlen)
{
    TBOOL   abbreviated = eFALSE;
    TUSIGN8 len = strlen((char*)txt);
    TUSIGN8 i;

    if (len > maxlen)
    {
        // Length is greater than maximum allowed so some characters must be removed
        i = maxlen;

        // Determine if relevant characters are being removed
        while ((abbreviated == eFALSE) && (i < len))
        {
            if (txt[i] != ' ')
            {
                abbreviated = eTRUE;
            }
            i ++;
        }

        if (!abbreviated)
        {
            // Removing spaces only
            txt[maxlen] = 0x00;             // terminate string at maxlen position

            i = maxlen - 1;
            while ((i > 0) && (txt[i] == ' '))  // remove trailing spaces
            {
                txt[i] = 0x00;
                i--;
            }
        }
        else
        {
            // Removing characters that have some relevance
            i = maxlen;
            txt[i] = 0x00;                  // terminate string at maxlen position
            i --;
            txt[i] = 0x09;                  // replace last character with '...'
            i --;

            while ((i > 0) && (txt[i] == ' '))  // remove trailing spaces
            {
                i ++;
                txt[i] = 0x00;
                i --;
                txt[i] = 0x09;
                i --;
            }
        }
    }
}


// No restriction on the optimization of this function
TBOOL PacketManagerIsLowLevelActive(TUSIGN8 instance)
{
    TBOOL returnVal = eFALSE;

#ifdef HMI_SUPPORTS_SERVICEPORT

    if (instance == 2)
    {
        if (PacketManagerLocal_2.LowLevelHMIFitted)
        {
            returnVal = eTRUE;
        }
    }

    if (instance == 1)
    {
        if (PacketManagerLocal_1.LowLevelHMIFitted)
        {
            returnVal = eTRUE;
        }
    }

#else
    if (PacketManagerLocal.LowLevelHMIFitted)
    {
        returnVal =  eTRUE;
    }
#endif

    return returnVal;
}

// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerManageLowLevelPower(TBOOL devicePowerupComplete, TUSIGN8 forcedReplyCount, TBOOL HMIcanAccessIO)
{
    #ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        if (ServicePortConnected)
        {
            PacketManagerLocal_2.LowLevelHMIFitted = 1;
        }
        else
        {
            PacketManagerLocal_2.LowLevelHMIFitted = 0;

            #if defined(HMI_SUPPORTS_ASIAN) && defined(HMI_SUPPORTS_SERVICEPORT)
            LowLevelStartupCount_REMOTE_HMI = LOWLEVEL_STARTUP_TIME;
            #endif
        }
    }
    else
    #endif
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (HMIcanAccessIO)
        {
            if (!LOCAL_prvHMIcanAccessIO)
            {
                #if defined(TYPE_1_IMPLEMENTATION) && defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                    asm("nop");
                #else
                    // HMI's access to the I/O has just been allowed - re-enable serial i/o
                    Layer2_initialiseIO();
                #endif
            }

            switch (LOCAL_powerUpState)
            {
            case powerUpState_InitialCondition:

                #ifdef TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE
                Layer2_ActivateReset();             // keep Low Level in reset state
                Layer2_RemovePower();               // disable serial i/o and switch power off

                #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                Layer2_RemovePower();               // disable serial i/o and switch power off

                #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                asm("nop");

                #else
                Layer2_RemovePower();               // disable serial i/o and switch power off

                #endif

                //LOCAL_RxCount = 0;                  // no data received from Low Level

                if (devicePowerupComplete)
                {
                    #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                    if (Layer2_PresenceDetected())
                    {
                        // when device power up complete (& presence of HMI detected)
                        // a 2 sec timer is started
                        LOCAL_powerUpState = powerUpState_OffDelay;
                        LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;
                    }

                    #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                    LOCAL_powerUpState = powerUpState_WaitingForHMI;
                    Layer2_ApplyPower();                // enable serial i/o and switch power on

                    #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                    LOCAL_powerUpState = powerUpState_WaitingForHMI;

                    #else
                    // when device power up complete (& presence of HMI detected)
                    // a 2 sec timer is started
                    LOCAL_powerUpState = powerUpState_OffDelay;
                    LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;

                    #endif
                }
                break;

            case powerUpState_OffDelay:
                #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                if (!Layer2_PresenceDetected())
                {
                    LOCAL_powerUpState = powerUpState_InitialCondition;
                }
                else
                {
                    Layer2_ActivateReset();             // keep Low Level in reset state
                    Layer2_ApplyPower();                // keep power on
                    LOCAL_RxCount = 0;                  // no data received from Low Level

                    LOCAL_powerSwitchTimer --;
                    if (LOCAL_powerSwitchTimer == 0)
                    {
                        // when delay timed out
                        LOCAL_powerUpState = powerUpState_WaitingForHMI;
                        LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 10;     // 10 seconds
                    }
                }

                #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                LOCAL_powerUpState = powerUpState_WaitingForHMI;
                Layer2_ApplyPower();

                #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                LOCAL_powerUpState = powerUpState_WaitingForHMI;

                #else
                Layer2_RemovePower();               // disable serial i/o and switch power off
                LOCAL_RxCount = 0;                  // no data received from Low Level

                LOCAL_powerSwitchTimer --;
                if (LOCAL_powerSwitchTimer == 0)
                {
                    // when delay timed out
                    LOCAL_powerUpState = powerUpState_WaitingForHMI;
                    LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 10;     // 10 seconds
                }

                #endif
                break;

            case powerUpState_WaitingForHMI:
                #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                Layer2_ApplyPower();                // enable serial i/o and switch power on
                Layer2_DeactivateReset();           // Allow Low Level to run

                #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                asm("nop");

                #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                asm("nop");

                #else
                Layer2_ApplyPower();                // enable serial i/o and switch power on

                #endif

                // Check if "initialization packet" received
                if ((LOCAL_RxCount >= 6)
                 && (LOCAL_RxByte[0] == INITIALISE_HMI)
//                 && (LOCAL_RxByte[1] == 3)
//                 && (LOCAL_RxByte[5] == (TUSIGN8)(LOCAL_RxByte[0] + LOCAL_RxByte[1] + LOCAL_RxByte[2] + LOCAL_RxByte[3] + LOCAL_RxByte[4] + 1))
                   )
                {
                    TFLOAT  calc;
                    // "initialization packet" received - HMI has been detected
                    // (1) RxByte[2] & RxByte[3] determine the low level HMI version and therefore determine what do do next.
                    if (LOCAL_RxByte[3] >= 20)
                    {
                        // This (interim) version of Low level HMI (interim version)...
                        // (1) Does not distinguish between low level hardware revisions.
                        // (2) Has valid software version at RxByte[3]
                        // (3) supports the COMMS_V2_INIT command
                        // (4) does not recognise the LOW_LEVEL_INFO command
                        // (5) supports new comms mode (after COMMS_V2_INIT)
                        LOCAL_DisplayHardwareRevision = HMI_HW_NOT_RECOGNISED;
                        LOCAL_DisplaySoftwareRevision = LOCAL_RxByte[3];
                        LOCAL_COMMS_V2_INIT_reqd = eTRUE;
                        LOCAL_LOW_LEVEL_INFO_reqd = eFALSE;
                        LOCAL_newCommsMode = eTRUE;
                    }
                    else if ((LOCAL_RxByte[2] == 1) && (LOCAL_RxByte[3] == 19))
                    {
                        // This (most recent) version of Low level HMI...
                        // (1) Returns the low level hardware revision from LOW_LEVEL_INFO command - so initially DisplayHardwareRevision is HMI_HW_NOT_RECOGNISED
                        // (2) Returns the low level software revision from LOW_LEVEL_INFO command - so initially DisplaySoftwareRevision is 0xFF
                        // (3) supports the COMMS_V2_INIT command
                        // (4) supports the LOW_LEVEL_INFO command
                        // (5) supports new comms mode (after COMMS_V2_INIT)
                        LOCAL_DisplayHardwareRevision = HMI_HW_NOT_RECOGNISED;
                        LOCAL_DisplaySoftwareRevision = 0xFF;
                        LOCAL_COMMS_V2_INIT_reqd = eTRUE;
                        if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_SHMI_STATUS)
                        {
                            LOCAL_LOW_LEVEL_INFO_reqd = eFALSE;//enable after receiving the comms_V2 messages
                        }
                        else if(servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_HMI_STATUS)
                        {
                            LOCAL_LOW_LEVEL_INFO_reqd = eTRUE;
                        }
                        LOCAL_newCommsMode = eTRUE;
                    }
                    else
                    {
                        // This (early) version of Low level HMI...
                        // (1) Does not distinguish between low level hardware revisions.
                        // (2) Has valid software version at RxByte[3]
                        // (3) does not recognise the COMMS_V2_INIT command
                        // (4) does not recognise the LOW_LEVEL_INFO command
                        // (5) does not support new comms mode
                        LOCAL_DisplayHardwareRevision = HMI_HW_NOT_RECOGNISED;
                        LOCAL_DisplaySoftwareRevision = LOCAL_RxByte[3];
                        LOCAL_COMMS_V2_INIT_reqd = eFALSE;
                        LOCAL_LOW_LEVEL_INFO_reqd = eFALSE;
                        LOCAL_newCommsMode = eFALSE;
                    }

                    // (2) Extract appropriate data from initialization packet
                    hmi_dynamic_data.lowLevelSoftwareRevision = LOCAL_DisplaySoftwareRevision;

                    // Contrast adjustment on COG controller is 0..63  where 0 gives "white" & 63 gives "black"
                    // We are only using a range of 30% to 80% of the COG controller range 30% = 19, 80% = 51
                    // Therefore contrast setting of  0% -> 19
                    //                              100% -> 51
                    //
                    calc = (TUSIGN8)((TFLOAT)(LOCAL_RxByte[4] - 19) * 3.125F);
                    if (calc < 0.0F)            {LOCAL_DisplayDefaultContrast = 0;}
                    else if (calc > 100.0F)     {LOCAL_DisplayDefaultContrast = 100;}
                    else                        {LOCAL_DisplayDefaultContrast = (TUSIGN8)calc;}
                    hmi_dynamic_data.defaultContrast = LOCAL_DisplayDefaultContrast;

                    // (3) reset reply counter
                    //LOCAL_RxByte[0] = 0;

                    // (4) set new state
                    LOCAL_powerUpState = powerUpState_HMIDetected;
                }
                else
                {
                    #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                    // HMI not detected yet so update timer - power will be re-cycled if timer expires
                    LOCAL_powerSwitchTimer --;
                    if (LOCAL_powerSwitchTimer == 0)
                    {
                        LOCAL_powerUpState = powerUpState_OffDelay;
                        LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;     // 2 seconds
                    }

                    #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                    asm("nop");

                    #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                    asm("nop");

                    #else
                    // HMI not detected yet so update timer - power will be re-cycled if timer expires
                    LOCAL_powerSwitchTimer --;
                    if (LOCAL_powerSwitchTimer == 0)
                    {
                        LOCAL_powerUpState = powerUpState_OffDelay;
                        LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;     // 2 seconds
                    }

                    #endif
                }
                break;

            case powerUpState_HMIDetected:
                #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                Layer2_ApplyPower();                // enable serial i/o and switch power on
                Layer2_DeactivateReset();           // Allow Low Level to run

                #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                asm("nop");

                #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                asm("nop");

                #else
                Layer2_ApplyPower();                // enable serial i/o and switch power on

                #endif

                if ((forcedReplyCount >= 3)
                  || ((LOCAL_RxCount>=6) 
                  && (LOCAL_RxByte[0]==INITIALISE_HMI)))
                {
                    // Low Level HMI has not responded to the last three transmissions
                    // Assume that the HMI has been removed.

                    #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
                    LOCAL_powerUpState = powerUpState_OffDelay;
                    LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;   // 1 second
                    LOCAL_last_contrast = 101;                              // set last_contrast to an invalid value to force the contrast to be re-sent

                    #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
                    LOCAL_powerUpState = powerUpState_WaitingForHMI;
                    Layer2_ApplyPower();                                    // enable serial i/o and switch power on

                    #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
                    LOCAL_powerUpState = powerUpState_WaitingForHMI;

                    #else
                    LOCAL_powerUpState = powerUpState_OffDelay;
                    LOCAL_powerSwitchTimer = HMI_DISPLAY_RATE_CYCLES * 2;   // 1 second
                    LOCAL_last_contrast = 101;                              // set last_contrast to an invalid value to force the contrast to be re-sent

                    #endif
                    LOCAL_RxCount = 0;
                }
                break;
            default:
                break;
            }
        }
        else
        {
            // HMI does not have access to I/O

            #if defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE)
            if (LOCAL_prvHMIcanAccessIO)
            {
                // HMI's access to the I/O has just been denied - disable serial i/o and switch power off
                Layer2_RemovePower();
            }
            LOCAL_powerUpState = powerUpState_InitialCondition;     // go back to initial condition

            #elif defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)
            LOCAL_RxCount = 0;
            LOCAL_powerUpState = powerUpState_WaitingForHMI;        // go back to waiting for HMI, do not cycle power

            #elif defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)
            LOCAL_RxCount = 0;
            LOCAL_powerUpState = powerUpState_WaitingForHMI;        // go back to waiting for HMI, do not cycle power

            #else
            if (LOCAL_prvHMIcanAccessIO)
            {
                // HMI's access to the I/O has just been denied - disable serial i/o and switch power off
                Layer2_RemovePower();
            }
            LOCAL_powerUpState = powerUpState_InitialCondition;     // go back to initial condition

            #endif
        }

        LOCAL_prvHMIcanAccessIO = HMIcanAccessIO;

        if (LOCAL_powerUpState == powerUpState_HMIDetected)
        {
            LOCAL_LowLevelHMIFitted = 1;
        }
        else
        {
            LOCAL_LowLevelHMIFitted = 0;

            #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            rotation = HMI_ROTATION_0;
            #endif

            #if defined(HMI_SUPPORTS_ASIAN) && defined(TYPE_1_IMPLEMENTATION) && defined(SUPPORT_OLD_HMI)
            LowLevelStartupCount_LOCAL_HMI = LOWLEVEL_STARTUP_TIME;
            #endif
        }

        #elif defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
        LOCAL_LowLevelHMIFitted = devicePowerupComplete;

        #else
        VIP_ASSERT(0);

        #endif
    }
}

void PacketManagerReInitialiseLowLevelDetection(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    PacketManagerLocal_1.powerUpState = powerUpState_InitialCondition;
#else
    PacketManagerLocal.powerUpState = powerUpState_InitialCondition;
#endif
}


// No restriction on the optimization of this function
TUSIGN8 PacketManagerIsWaitingForReply(void)
{
    return (TUSIGN8)(LOCAL_TransmitterBusy | LOCAL_ReceiverWaiting);
}

// No restriction on the optimization of this function
TBOOL PacketManagerProcessWaitForReply(TUSIGN8 instance, TUSIGN8 *pTimer, TUSIGN8 *pTimeoutCount)
{
    TBOOL WaitingForReply = eTRUE;

#ifdef HMI_SUPPORTS_SERVICEPORT
    if (instance == 2)
    {
        if (ServicePortTransmissionComplete)
        {
            *pTimer = 1;
            ServicePortTransmissionComplete = eFALSE;
        }
    }
#endif

    if ((LOCAL_TransmitterBusy != 0) || (LOCAL_ReceiverWaiting != 0))
    {
        *pTimer = *pTimer + 1;

        if (*pTimer >= HMI_DISPLAY_RATE_CYCLES)
        {
            PacketManagerForceReply();
            *pTimeoutCount = *pTimeoutCount + 1;
            *pTimer = 0;
            WaitingForReply = eFALSE;
        }
    }
    else
    {
        *pTimer = 0;
        *pTimeoutCount = 0;
        WaitingForReply = eFALSE;
    }
    return WaitingForReply;
}


// No restriction on the optimization of this function
void PacketManagerForceReply(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    if (pPacketManagerLocal == &PacketManagerLocal_1)
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (PacketManagerLocal_1.newCommsMode)
        {
            PacketManagerLocal_1.RxByte[2] = PROCESSING_ERROR;
            PacketManagerLocal_1.RxByte[3] = (TUSIGN8)(PacketManagerLocal_1.RxByte[0] + PacketManagerLocal_1.RxByte[1] + PacketManagerLocal_1.RxByte[2] + 0x01);
        }
        else
        {
            PacketManagerLocal_1.RxByte[0] = PROCESSING_ERROR;
        }
        #else
        {
            PacketManagerLocal_1.RxByte[0] = PROCESSING_ERROR;
        }
        #endif

        PacketManagerLocal_1.ReceiverWaiting = 0;
    }
    else if (pPacketManagerLocal == &PacketManagerLocal_2)
    {
        PacketManagerLocal_2.RxByte[2] = PROCESSING_ERROR;
        PacketManagerLocal_2.RxByte[3] = (TUSIGN8)(PacketManagerLocal_2.RxByte[0] + PacketManagerLocal_2.RxByte[1] + PacketManagerLocal_2.RxByte[2] + 0x01);
        PacketManagerLocal_2.ReceiverWaiting = 0;
    }
    else
    {
        VIP_ASSERT(0);
    }

#else
    #ifdef TYPE_1_IMPLEMENTATION
    if (PacketManagerLocal.newCommsMode)
    {
        PacketManagerLocal.RxByte[2] = PROCESSING_ERROR;
        PacketManagerLocal.RxByte[3] = (TUSIGN8)(PacketManagerLocal.RxByte[0] + PacketManagerLocal.RxByte[1] + PacketManagerLocal.RxByte[2] + 0x01);
    }
    else
    {
        PacketManagerLocal.RxByte[0] = PROCESSING_ERROR;
    }
    #else
    PacketManagerLocal.RxByte[0] = PROCESSING_ERROR;
    #endif
    PacketManagerLocal.ReceiverWaiting = 0;

#endif
}

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerSendDataToLowLevel(TUSIGN8 bytesToSend)
{
    LOCAL_RxCount = 0;
    //LOCAL_RxByte[0] = 0;
    LOCAL_TxLength = bytesToSend;
    if (LOCAL_TxLength)
    {
        LOCAL_TxCount = 0;
        LOCAL_TransmitterBusy = 1;

        while (LOCAL_TxCount < LOCAL_TxLength)
        {
            LowLevelDriver_Receive(LOCAL_packetBuffer[LOCAL_TxCount]);
            LOCAL_TxCount++;
        }
        LOCAL_ReceiverWaiting = 1;
        LOCAL_TransmitterBusy = 0;
    }
}

// No restriction on the optimization of this function
void PacketManagerExecuteLowLevel(void)
{
    LowLevelDriverExe();
}

// No restriction on the optimization of this function
void PacketManagerReceiveDataFromLowLevel(void)
{
    LOCAL_RxByte[0] = LowLevelDriver_Reply();
    LOCAL_ReceiverWaiting = 0;
}

#else
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerStartTransmission(TUSIGN8 bytesToSend)
{
    LOCAL_RxCount = 0;
    LOCAL_RxByte[0] = 0;
    LOCAL_TxLength = bytesToSend;
    if (LOCAL_TxLength)
    {
        #if 0
        LOCAL_TxCount = 0;
        LOCAL_TransmitterBusy = 1;
        Layer2_TransmitByte(LOCAL_packetBuffer[LOCAL_TxCount],eTRUE);
        LOCAL_TxCount ++;
        #endif
        LOCAL_TransmitterBusy = 1;
        if(HAL_UART_Transmit_IT(&HMI_UartHandle, (uint8_t*)LOCAL_packetBuffer, LOCAL_TxLength)!= HAL_OK)
        {
            Error_Handler();
        }
    }
}

void PacketManagerReadyToTransmission(void)
{
    LOCAL_ReceiverWaiting = 1;
    LOCAL_RxByte[0] = 0;
    LOCAL_RxCount = 0; 
}

#endif

#ifdef HMI_SUPPORTS_SERVICEPORT

#pragma diag_suppress=Pm077
#include "../../ServicePort/interface/SpClientInfo.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Interface/ServicePort_idx.h"
#pragma diag_default=Pm077


void SP_CallbackDataReceived(void);
void SP_CallbackTransmissionComplete(TUSIGN8 *pMsg);
void SP_CallbackDummy(void);
void SP_CallbackConnected(void);
void SP_CallbackDisconnected(void);
void SP_CallbackPoweredUp(void);
void SP_CallbackPoweredDown(void);
void SP_CallbackCommsError(TSpCommsError err);

#pragma diag_suppress=Pm023

TSpCallbacks SP_Callbacks =
{
    SP_CallbackDataReceived,            // evtDataReceived;            typedef void (*TCallbackDataReceived)(void)
    SP_CallbackTransmissionComplete,    // evtTransmissionComplete;    typedef void (*TCallbackTransmissionComplete)(TUSIGN8 *pMsg)
    SP_CallbackCommsError,              // evtCommsError;              typedef void (*TCallbackCommsError)(TSpCommsError err)
    SP_CallbackDummy,                   // evtTxDataFlowHalted;        typedef void (*TCallbackTxDataFlowHalted)(void)
    SP_CallbackDummy,                   // evtTxDataFlowResumed;       typedef void (*TCallbackTxDataFlowResumed)(void)
    SP_CallbackDummy,                   // evtRxDataFlowHalted;        typedef void (*TCallbackRxDataFlowHalted)(void)
    SP_CallbackDummy,                   // evtRxDataFlowResumed;       typedef void (*TCallbackRxDataFlowResumed)(void)
    SP_CallbackConnected,               // evtConnected;               typedef void (*TCallbackConnected)(void)
    SP_CallbackDisconnected,            // evtDisconnected;            typedef void (*TCallbackDisconnected)(void)
    SP_CallbackDummy,                   // evtPoweredUp;               typedef void (*TCallbackPoweredUp)(void)
    SP_CallbackDummy,                   // evtPoweredDown;             typedef void (*TCallbackPoweredDown)(void)
};

#pragma diag_default=Pm023

#define HMI_SP_CLIENT_IDENTITY 4
TCHAR HMI_ClientProtocolName[] = {"Hmi SP Client"} ;

#pragma diag_suppress=Pm022
TSpClientInfo SP_ClientInfo = CONSTRUCT_SP_CLIENTINFO(HMI_SP_CLIENT_IDENTITY, (TSpCallbacks*)&SP_Callbacks, HMI_ClientProtocolName);
#pragma diag_default=Pm022


TBOOL PacketManager_SP_RegisterClientEXE(void)
{

    T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;
    TBOOL returnVal = pServicePort->RegisterClientEXE(&SP_ClientInfo);
    return returnVal;
}


// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerStartTransmission_SP(TUSIGN8 bytesToSend)
{
    T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;
    TUSIGN16 acceptedBytes = 0;

    while (acceptedBytes < bytesToSend)
    {
        //TUSIGN16 SendDataEXE (TUSIGN8 *pMsg, TUSIGN16 msgLen, TUSIGN8 originId)
        acceptedBytes += pServicePort->SendDataEXE (&PacketManagerLocal_2.packetBuffer[acceptedBytes],
                                                     (TUSIGN16)(bytesToSend-acceptedBytes),
                                                     SP_ClientInfo.id);
    }
    PacketManagerLocal_2.TransmitterBusy = 1;
    PacketManagerLocal_2.ReceiverWaiting = 1;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void SP_CallbackTransmissionComplete(TUSIGN8 *pMsg)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    // Service port has completed transmission of the message pointed to by pMsg
    SP_TransmitterBusy = 0;             // Transmitter finished
    SP_ServicePortTransmissionComplete = eTRUE;

    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);  // Release the HMI_REMOTE_INSTANCE
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void SP_CallbackDataReceived(void)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    // Service port has received HMI data
    T_SERVICE_PORT const SLOW *pServicePort = ((const T_SERVICE_PORT*)GetSubsystemPtr((T_SUBSYSTEM_IDX)SERVICEPORT_IDX)) ;

    // Read Received bytes
    TUSIGN8 SP_nReceivedBytes = pServicePort->ReadDataEXE(SP_ReceiveBuffer, SERVICEPORT_RXBUFFER_SIZE) ;

    // START New (2007-11-06) method of copying data from "SP_ReceiveBuffer[]" to "PacketManagerLocal_2.RxByte[]"
    if (SP_RxCount + SP_nReceivedBytes <= 15)
    {
        (void)memcpy((void *)&SP_RxByte[SP_RxCount],
                     (void *)SP_ReceiveBuffer,
                     (size_t)SP_nReceivedBytes);

    }
    SP_RxCount += SP_nReceivedBytes;

    if (SP_RxCount > (SP_RxByte[1] + 2))
    {
        // sufficient number of bytes received - receiver no longer waiting
        SP_ReceiverWaiting = 0;
    }
    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);  // Release the HMI_REMOTE_INSTANCE
}

// No restriction on the optimization of this function
void SP_CallbackConnected(void)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    SP_ServicePortConnected = eTRUE;
    SP_CommsInitReqd = eTRUE;

    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);  // Release the HMI_REMOTE_INSTANCE
}

// No restriction on the optimization of this function
void SP_CallbackDisconnected(void)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    SP_ServicePortConnected = eFALSE;

    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);  // Release the HMI_REMOTE_INSTANCE
}

// No restriction on the optimization of this function
void SP_CallbackDummy(void)
{
}

// No restriction on the optimization of this function
void SP_CallbackCommsError(TSpCommsError err)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    if (err != 0)
    {
        SP_RxByte[2] = COMMUNICATION_ERROR;
    }

    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);  // Release the HMI_REMOTE_INSTANCE
}

void PacketManagerCopyServicePortData(TBOOL toLocal)
{
    ENABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE

    if (toLocal)
    {
        PacketManagerLocal_2.ReceiverWaiting = SP_ReceiverWaiting;
        PacketManagerLocal_2.TransmitterBusy = SP_TransmitterBusy;
        PacketManagerLocal_2.RxCount         = SP_RxCount;
        CommsInitReqd                        = SP_CommsInitReqd;
        ServicePortConnected                 = SP_ServicePortConnected;
        ServicePortTransmissionComplete      = SP_ServicePortTransmissionComplete;

        memcpy((void *)PacketManagerLocal_2.RxByte, (void *)SP_RxByte, 15);
    }
    else
    {
        SP_ReceiverWaiting                   = PacketManagerLocal_2.ReceiverWaiting;
        SP_TransmitterBusy                   = PacketManagerLocal_2.TransmitterBusy;
        SP_RxCount                           = PacketManagerLocal_2.RxCount;
        SP_CommsInitReqd                     = CommsInitReqd;
        SP_ServicePortConnected              = ServicePortConnected;
        SP_ServicePortTransmissionComplete   = ServicePortTransmissionComplete;

        memcpy((void *)SP_RxByte, (void *)PacketManagerLocal_2.RxByte, 15);
    }
    DISABLE_DATA_PROTECTION(Sema_HMI_REMOTE_INSTANCE);    // Make sure nobody else uses the HMI_REMOTE_INSTANCE
}

#endif


// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerTransmitInterrupt(void)
{
#ifdef HMI_SUPPORTS_SERVICEPORT
    #ifdef TYPE_1_IMPLEMENTATION
    if (PacketManagerLocal_1.TxCount < PacketManagerLocal_1.TxLength)
    {
        Layer2_TransmitByte(PacketManagerLocal_1.packetBuffer[PacketManagerLocal_1.TxCount],eFALSE);
        PacketManagerLocal_1.TxCount ++;
    }
    else
    {
        PacketManagerLocal_1.ReceiverWaiting = 1;
        PacketManagerLocal_1.TransmitterBusy = 0;
    }
    #endif
#else
    #ifdef TYPE_1_IMPLEMENTATION
    #if 0
    if (PacketManagerLocal.TxCount < PacketManagerLocal.TxLength)
    {
        Layer2_TransmitByte(PacketManagerLocal.packetBuffer[PacketManagerLocal.TxCount],eFALSE);
        PacketManagerLocal.TxCount ++;
    }
    else
    {
        PacketManagerLocal.ReceiverWaiting = 1;
        PacketManagerLocal.TransmitterBusy = 0;
    }
    #endif
    PacketManagerLocal.ReceiverWaiting = 1;
    PacketManagerLocal.TransmitterBusy = 0;
    #endif
#endif
}

// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
void PacketManagerReceiveInterupt(void)
{   
#ifdef HMI_SUPPORTS_SERVICEPORT
    #ifdef TYPE_1_IMPLEMENTATION
    if (PacketManagerLocal_1.LowLevelHMIFitted)
    {
        if (PacketManagerLocal_1.newCommsMode == eTRUE)
        {
            PacketManagerLocal_1.RxByte[PacketManagerLocal_1.RxCount] = mSpHmiRxData;
            PacketManagerLocal_1.RxCount ++;

            if (PacketManagerLocal_1.RxCount >= 2)
            {
                if (PacketManagerLocal_1.RxCount >= (PacketManagerLocal_1.RxByte[1] + 3))
                    PacketManagerLocal_1.ReceiverWaiting = 0;
            }
        }
        else
        {
            PacketManagerLocal_1.RxByte[0] = mSpHmiRxData;
            PacketManagerLocal_1.ReceiverWaiting = 0;
        }
    }
    else
    {
        PacketManagerLocal_1.RxByte[PacketManagerLocal_1.RxCount] = mSpHmiRxData;

        // Do not increment byte count until 0x32 has been rec'd - The first byte must be an "Initialise Hmi Command Packet"
        // Do not increment byte count past 14 - buffer size is [15]
        if (PacketManagerLocal_1.RxByte[0] == 0x32 && PacketManagerLocal_1.RxCount < 14)
        {
            PacketManagerLocal_1.RxCount ++;
        }
    }
    #endif
#else
    #ifdef TYPE_1_IMPLEMENTATION

    if (PacketManagerLocal.LowLevelHMIFitted)
    {
        if (PacketManagerLocal.newCommsMode == eTRUE)
        {
            if (PacketManagerLocal.RxCount < 15)
            {
                PacketManagerLocal.RxByte[PacketManagerLocal.RxCount] = mSpHmiRxData;
                if(PacketManagerLocal.RxCount == 0)
                {
                    if((PacketManagerLocal.RxByte[0] == INITIALISE_HMI)
                    || (PacketManagerLocal.RxByte[0] == COMMS_V2_INIT)
                    || (PacketManagerLocal.RxByte[0] == LOW_LEVEL_INFO)
                    || (PacketManagerLocal.RxByte[0] == READ_SWITCHES))
                    {
                        PacketManagerLocal.RxCount++;
                    }
                }
                else if(PacketManagerLocal.RxCount == 1)
                {
                    if(PacketManagerLocal.RxByte[1] <=14)
                    {
                        PacketManagerLocal.RxCount++;
                    }
                }
                else if (PacketManagerLocal.RxCount <= 14)
                {
                    if(PacketManagerLocal.RxCount < (PacketManagerLocal.RxByte[1] + 3))
                    {
                        PacketManagerLocal.RxCount++;
                        if (PacketManagerLocal.RxCount == (PacketManagerLocal.RxByte[1] + 3))
                        {
                            PacketManagerLocal.ReceiverWaiting = 0;
                        }
                    }
                }
            }
        }
        else
        {
            PacketManagerLocal.RxByte[0] = mSpHmiRxData;
            PacketManagerLocal.ReceiverWaiting = 0;
        }
    }
    else
    {
        // Do not increment byte count until 0x32 has been rec'd - The first byte must be an "Initialise Hmi Command Packet"
        // Do not increment byte count past 14 - buffer size is [15]
        if(PacketManagerLocal.RxCount < 15)
        {
            PacketManagerLocal.RxByte[PacketManagerLocal.RxCount] = mSpHmiRxData;
            if (PacketManagerLocal.RxCount == 0)
            {
                if (PacketManagerLocal.RxByte[0] == INITIALISE_HMI)
                {
                    PacketManagerLocal.RxCount++;
                }
                else if(PacketManagerLocal.RxByte[0] == UARTI_IDENT_DISPLAY_FRAME)
                {
                    PacketManagerLocal.RxCount++;
                }
            }
            else if(PacketManagerLocal.RxCount == 1)
            {
                if(PacketManagerLocal.RxByte[1] <= 14)
                {
                    PacketManagerLocal.RxCount++;
                }
                else
                {//wrong data, receive again
                    PacketManagerLocal.RxCount = 0;
                }
            }
            else if (PacketManagerLocal.RxCount <= 14)
            {
                if(PacketManagerLocal.RxCount <= (PacketManagerLocal.RxByte[1]+3))//change by pluto
                {
                    PacketManagerLocal.RxCount++;
                    if(PacketManagerLocal.RxCount == (PacketManagerLocal.RxByte[1]+3))
                    {//receive OK
                        if((PacketManagerLocal.RxByte[0] == UARTI_IDENT_DISPLAY_FRAME) 
                           && PacketManagerLocal.RxByte[2] == INITIALISE_HMI)
                        {
                            if(servicePort.ServPortGetActivationStatusSRV() != SERVICEPORT_SHMI_STATUS
                               && servicePort.ServPortGetActivationStatusSRV() == SERVICEPORT_HMI_STATUS)
                            {
                                servicePort.ServPortRedetectHMISRV(eTRUE);
                            }
                            PacketManagerLocal.RxCount = 0;
                        }
                        PacketManagerLocal.ReceiverWaiting = 0;
                    }
                }
            }
        }
    }
#endif
#endif

    if(HAL_UART_Receive_IT(&HMI_UartHandle, (uint8_t*)&mSpHmiRxData, 1)!= HAL_OK)
    {
        Error_Handler();
    }
}

#ifdef LOCAL_INTERRUPT_HANDLERS

#pragma vector = 19
__interrupt void UART1transmit(void)
{
    __enable_interrupt();
    PacketManagerTransmitInterrupt();
}

#pragma vector = 20
__interrupt void UART1receive(void)
{
    __enable_interrupt();
    PacketManagerReceiveInterupt();
}

#endif

TUSIGN8 softKeyIconNextOperatorFrame_autoscroll[12] = {0x00, 0x7E, 0x77, 0x73, 0x41, 0x53, 0x57, 0x5D, 0x41, 0x7F, 0x7E, 0x00};

void PacketManagerSetOperatorIcon(TUSIGN8 field, TUSIGN8 iconID)
{
    if (field == 3)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        PacketManagerLocal_1.Field_3_Icon = iconID;
        PacketManagerLocal_2.Field_3_Icon = iconID;
        #else
        PacketManagerLocal.Field_3_Icon = iconID;
        #endif
    }

    if (field == 7)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        PacketManagerLocal_1.Field_7_Icon = iconID;
        PacketManagerLocal_2.Field_7_Icon = iconID;
        #else
        PacketManagerLocal.Field_7_Icon = iconID;
        #endif
    }

    if (field == 5)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        PacketManagerLocal_1.Field_5_Icon = iconID;
        PacketManagerLocal_2.Field_5_Icon = iconID;
        #else
        PacketManagerLocal.Field_5_Icon = iconID;
        #endif
    }

#ifdef EDP300_SPECIAL_FEATURES_HMI
    if (field == 9)
    {
        #ifdef HMI_SUPPORTS_SERVICEPORT
        PacketManagerLocal_1.Field_9_Icon = iconID;
        PacketManagerLocal_2.Field_9_Icon = iconID;
        #else
        PacketManagerLocal.Field_9_Icon = iconID;
        #endif
    }
#endif
}

void PacketManagerSetOperatorInfo(TUSIGN8 infoID)
{
    #ifdef HMI_USES_COMMONDIAGNOSTICS
        #ifdef HMI_SUPPORTS_SERVICEPORT

        if (PacketManagerLocal_1.infoMessageID != infoID)
        {
            PacketManagerLocal_1.infoMessageTimer = 0;
        }
        PacketManagerLocal_1.infoMessageID = infoID;

        if (PacketManagerLocal_2.infoMessageID != infoID)
        {
            PacketManagerLocal_2.infoMessageTimer = 0;
        }
        PacketManagerLocal_2.infoMessageID = infoID;
        #else
        if (PacketManagerLocal.infoMessageID != infoID)
        {
            PacketManagerLocal.infoMessageTimer = 0;
        }
        PacketManagerLocal.infoMessageID = infoID;
        #endif
    #else
        asm("nop");
    #endif
}

#ifdef HMI_USES_COMMONDIAGNOSTICS
TUSIGN8 GetInfoMessageID(TBOOL OverrideTimer)
{
    TUSIGN8 returnVal = 0;
    if ((LOCAL_infoMessageTimer < (TUSIGN16)(HMI_DISPLAY_RATE_CYCLES * 5))
     || (OverrideTimer))
    {
        returnVal = LOCAL_infoMessageID;
    }
    return returnVal;
}

void UpdateInfoMessageTimer(void)
{
    if (LOCAL_infoMessageID == 0)
    {
        LOCAL_infoMessageTimer = 0;
    }
    else
    {
        LOCAL_infoMessageTimer ++;
        if (LOCAL_infoMessageTimer > (TUSIGN16)(HMI_DISPLAY_RATE_CYCLES * 6))
        {
            LOCAL_infoMessageTimer = (TUSIGN16)(HMI_DISPLAY_RATE_CYCLES * 4);
        }
    }
}
#endif

#if defined(HMI_USES_COMMONDIAGNOSTICS) || defined(AZ200_SPECIAL_FEATURES_HMI)
TUSIGN8 PacketManagerConvertValue(TUSIGN16 value, TUSIGN8 *pText, TBOOL min2)
{
    TUSIGN8 j = 3;
    TUSIGN8 nCharsAdded = 0;
    TUSIGN8 tth,th,h,t,u;

    if      (value > 9999)  {j = 0;}
    else if (value > 999)   {j = 1;}
    else if (value > 99)    {j = 2;}
    else if (value > 9)     {j = 3;}
    else if (!min2)         {j = 4;}
    else                    {j = 3;}



    #pragma diag_suppress=Pm088
    #pragma diag_suppress=Pm057
    switch (j)
    {
    case 0:     tth   = (TUSIGN8)(value / 10000);   // calculate number of tens of thousands
                value = value % 10000;              // remove tens of thousands from value
                *pText = tth + 0x30;                // add tens of thousands to string
                pText ++;                           // point to next character
                nCharsAdded ++;
    case 1:     th    = (TUSIGN8)(value / 1000);    // calculate number of thousands
                value = value % 1000;               // remove thousands from value
                *pText = th + 0x30;                 // add thousands to string
                pText ++;                           // point to next character
                nCharsAdded ++;
    case 2:     h     = (TUSIGN8)(value / 100);     // calculate number of hundreds
                value = value % 100;                // remove hundreds from value
                *pText = h + 0x30;                  // add hundreds to string
                pText ++;                           // point to next character
                nCharsAdded ++;
    case 3:     t     = (TUSIGN8)(value / 10);      // calculate number of tens
                value = value % 10;                 // remove tens from value
                *pText = t + 0x30;                  // add tens to string
                pText ++;                           // point to next character
                nCharsAdded ++;
    case 4:     u     = (TUSIGN8)value;             // calculate number of units
                *pText = u + 0x30;                  // add units to string
                pText ++;                           // point to next character
                nCharsAdded ++;
                *pText = 0x00;                      // terminate string
    default:    asm("nop");
    }
    #pragma diag_default=Pm057
    #pragma diag_default=Pm088

    return nCharsAdded;
}

// end of "#if defined(HMI_USES_COMMONDIAGNOSTICS) || defined(AZ200_SPECIAL_FEATURES_HMI)"
#endif

void PacketManagerToggleBacklight(void)
{
    ToggleBacklight = eTRUE;
}

TBOOL PacketManagerOtherSubsystemPutPacketBuffer(T_SUBSYSTEM_IDX systemIdx, TUSIGN8 processCmd, TUSIGN8 *ptrArray, TUSIGN8 arrayLength)
{
    switch(systemIdx)
    {
    case SERVICEPORT_IDX:
        switch(processCmd)
        {
        case SP_HMI_PROCESS_HMIFRAME:
            if(ptrArray[0]!=LOW_LEVEL_INFO)
            {        
                LOCAL_packetBufferUsed = 0;
            }
            
            LOCAL_RxCount = 0;
            memcpy(LOCAL_RxByte, ptrArray, arrayLength);
            LOCAL_RxCount = arrayLength;
            LOCAL_ReceiverWaiting = 0;
            break;
        case SP_HMI_PROCESS_HMIKEY:
            if(ptrArray[0]!=0)
            {        
                LOCAL_packetBufferUsed = 0;    
            }
            else
            {
                //keep the old data
            }
            LOCAL_RxCount = 0;    
            LOCAL_RxByte[0] = READ_SWITCHES;
            LOCAL_RxByte[1] = 1;
            LOCAL_RxByte[2] = ptrArray[0];
            LOCAL_RxCount = 3;
            LOCAL_ReceiverWaiting = 0;
            break;
        default:
          return eFALSE;
        }
        break;
    default:
      return eFALSE;
    }
    return eTRUE;
}
