//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    implementation of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003

//@SubGen Version@<0x020101>

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
#include "Hmi/Interface/HMI.h"
//#include "hmi_types.h"
//#include "hmi_attributes.h"
#include "hmi_overload.h"

#include "HmiCoordinator.h"

#if defined(HMI_SUPPORTS_SIGNALS_VALIDITY_TEST) && defined(ENABLE_LINE_SOURCE_VALIDITY_TESTS)
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "Hmi/Interface/HMI_idx.h"
#include "FrameTableDefinitions.h"
#include "FrameTable.h"
#include "ValidityTest.h"
#include "constructorInclude.h"
#endif

/* Add for: reset language setting to default value (english) when use push button HMI. 
   Set this as global variable because there are many errors when using entry tool  */
TUSIGN8 gVersionFlag_HMI = TYPE_1_NVRAM_REQUIRED_VERSION;

//--------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author
 \date
 \param  typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  if a subsystem needs onle once a preparation for calculating, then
 </pre>
 \return error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inseid the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \test
 \n by:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
// this function not size optimized beyond low
#ifdef OPTIMIZE_V3_4
#pragma optimize=size low
#else
#pragma optimize=z 3
#endif
TUSIGN16 Initialize_HMI(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = OK;

    //lint -e{746}
    VIP_ASSERT(me==HMI_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);

    if( result==OK )
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
            HmiCoordinatorInitializeHardware();
        }

        if( (typeOfStartUp & INIT_TASKS) != 0)
        {
            HmiCoordinatorInitializeTasks(HMI_LOCAL_INSTANCE, eTRUE);
            #ifdef HMI_SUPPORTS_SERVICEPORT
            HmiCoordinatorInitializeTasks(HMI_REMOTE_INSTANCE, eTRUE);
            #endif
        }

        if( (typeOfStartUp & INIT_DATA) != 0)
        {
            HmiCoordinatorInitializeData();
        }

        if( (typeOfStartUp & INIT_CALCULATION) != 0)
        {
            asm("nop");
        }
    }

    return result;
}

#if defined(HMI_SUPPORTS_SIGNALS_VALIDITY_TEST) && defined(ENABLE_LINE_SOURCE_VALIDITY_TESTS)
/*
TUSIGN8  OverloadedMainOperatorViewList_0[OPMODE_0_N_OPTIONS]={OPMODE_0_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_1[OPMODE_1_N_OPTIONS]={OPMODE_1_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_2[OPMODE_2_N_OPTIONS]={OPMODE_2_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_3[OPMODE_3_N_OPTIONS]={OPMODE_3_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_4[OPMODE_4_N_OPTIONS]={OPMODE_4_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_5[OPMODE_5_N_OPTIONS]={OPMODE_5_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_6[OPMODE_6_N_OPTIONS]={OPMODE_6_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_7[OPMODE_7_N_OPTIONS]={OPMODE_7_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_8[OPMODE_8_N_OPTIONS]={OPMODE_8_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_9[OPMODE_9_N_OPTIONS]={OPMODE_9_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_10[OPMODE_10_N_OPTIONS]={OPMODE_10_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_11[OPMODE_11_N_OPTIONS]={OPMODE_11_OPTIONS_LIST};
TUSIGN8  OverloadedMainOperatorViewList_12[OPMODE_12_N_OPTIONS]={OPMODE_12_OPTIONS_LIST};

*/

TUSIGN16 GetAttributeDescription_HMI(const struct _T_UNIT SLOW*me,
                                        TUSIGN16 objectIndex,
                                        TUSIGN16 attributeIndex,
                                        T_DO_ATTRIB_RANGE FAST* ptrDescriptor)
{   
    TUSIGN16 returnVal = GetAttributeDescription_T_UNIT(me, objectIndex, attributeIndex, ptrDescriptor);

     TUSIGN8* p_newList; //= FrameTableGetOverloadedMainOperatorViewList(objectIndex);
/*   TUSIGN8 newNumberOfCodes = 0;
     
    TUSIGN8 opMode; 
    TUSIGN16 result = (TUSIGN16)OK;
    (void)(TUSIGN16)coordinator_Get( (TUSIGN16)CDT_IDX_operatingMode,(TINT16)WHOLE_OBJECT, &opMode);  
    if(result == OK)
    {
      switch(opMode)
	 {
          case CDT_LIQUID_ACTUAL_VOLUME:
            p_newList = OverloadedMainOperatorViewList_0;
            newNumberOfCodes = OPMODE_0_N_OPTIONS;
               break;
          case CDT_LIQUID_NORMAL_VOLUME:
            p_newList = OverloadedMainOperatorViewList_1;
            newNumberOfCodes = OPMODE_1_N_OPTIONS;
               break;
          case CDT_LIQUID_MASS:
            p_newList = OverloadedMainOperatorViewList_2;
            newNumberOfCodes = OPMODE_2_N_OPTIONS;
               break;
          case CDT_LIQUID_POWER:
            p_newList = OverloadedMainOperatorViewList_3;
            newNumberOfCodes = OPMODE_3_N_OPTIONS;
               break;
          case CDT_GAS_ACTUAL_VOLUME: 
            p_newList = OverloadedMainOperatorViewList_4;
            newNumberOfCodes = OPMODE_4_N_OPTIONS;
               break;
          case CDT_GAS_STANDARD_VOLUME: 
            p_newList = OverloadedMainOperatorViewList_5;
            newNumberOfCodes = OPMODE_5_N_OPTIONS;
               break;
          case CDT_GAS_MASS: 
            p_newList = OverloadedMainOperatorViewList_6;
            newNumberOfCodes = OPMODE_6_N_OPTIONS;
               break;
          case CDT_GAS_POWER: 
            p_newList = OverloadedMainOperatorViewList_7;
            newNumberOfCodes = OPMODE_7_N_OPTIONS;
               break;
          case CDT_BIOGAS_ACTUAL_VOLUME:
            p_newList = OverloadedMainOperatorViewList_8;
            newNumberOfCodes = OPMODE_8_N_OPTIONS;
               break;
          case CDT_BIOGAS_STANDARD_VOLUME:
            p_newList = OverloadedMainOperatorViewList_9;
            newNumberOfCodes = OPMODE_9_N_OPTIONS;
               break;              
          case CDT_STEAM_ACTUAL_VOLUME:
            p_newList = OverloadedMainOperatorViewList_10;
            newNumberOfCodes = OPMODE_10_N_OPTIONS;
               break;
          case CDT_STEAM_MASS:
            p_newList = OverloadedMainOperatorViewList_11;
            newNumberOfCodes = OPMODE_11_N_OPTIONS;
               break;               
          case CDT_STEAM_POWER:
            p_newList = OverloadedMainOperatorViewList_12;
            newNumberOfCodes = OPMODE_12_N_OPTIONS;
               break;    
          default: 
            p_newList = NULL;
               break;                         
      }   
    }
 */
     
    static TUSIGN8  OverloadedMainOperatorViewList_1[PAGE_1_LINE_1_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_2[PAGE_1_LINE_2_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_3[PAGE_1_LINE_3_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_4[PAGE_1_LINE_4_N_OPTIONS];


    switch (objectIndex)
    {      
    case HMI_IDX_mainOperatorView_1_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_1_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_1_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_1_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;

    #ifdef SECOND_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_2_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_2_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_2_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_2_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;
    #endif

    #ifdef THIRD_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_3_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_3_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_3_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_3_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;
    #endif

    #ifdef FOURTH_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_4_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_4_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_4_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_4_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;
    #endif

    #ifdef FIFTH_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_5_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_5_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_5_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_5_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;
    #endif

    #ifdef SIXTH_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_6_1:          p_newList = OverloadedMainOperatorViewList_1;   break;
    case HMI_IDX_mainOperatorView_6_2:          p_newList = OverloadedMainOperatorViewList_2;   break;
    case HMI_IDX_mainOperatorView_6_3:          p_newList = OverloadedMainOperatorViewList_3;   break;
    case HMI_IDX_mainOperatorView_6_bargraph:   p_newList = OverloadedMainOperatorViewList_4;   break;
    #endif

    default:                                    p_newList = NULL;                                       break;
    }
    if (p_newList)
    {
        TUSIGN8 newNumberOfCodes = 0;
        TUSIGN8 i;
        TUSIGN8 signalNumber;

        for (i = 0; i < ptrDescriptor->range.e8.numberOfCodes; i ++)
        {
            signalNumber = ptrDescriptor->range.e8.ptrCodeTab[i];
            TUSIGN16 testIdx = FrameTableGetSignal(signalNumber)->SignalsViewTest;

            if ((ValidityTestCheck(testIdx
#ifdef VALIDITY_TEST_CALLER
				, SIGNALS_TEST_NOT_APPLICABLE
#endif
			)) || (testIdx == IDF_VALID_always))
            {
                p_newList[newNumberOfCodes] = signalNumber;
                newNumberOfCodes ++;
            }
        }
        ptrDescriptor->range.e8.ptrCodeTab = p_newList;
        ptrDescriptor->range.e8.numberOfCodes = newNumberOfCodes;
    }

    return returnVal;
}

/*
TUSIGN8* FrameTableGetOverloadedMainOperatorViewList(TUSIGN16 objectIndex)
{
    static TUSIGN8  OverloadedMainOperatorViewList_1_1[PAGE_1_LINE_1_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_1_2[PAGE_1_LINE_2_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_1_3[PAGE_1_LINE_3_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_1_4[PAGE_1_LINE_4_N_OPTIONS];

    #ifdef SECOND_OPERATOR_PAGE
    static TUSIGN8  OverloadedMainOperatorViewList_2_1[PAGE_2_LINE_1_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_2_2[PAGE_2_LINE_2_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_2_3[PAGE_2_LINE_3_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_2_4[PAGE_2_LINE_4_N_OPTIONS];
    #endif

    #ifdef THIRD_OPERATOR_PAGE
    static TUSIGN8  OverloadedMainOperatorViewList_3_1[PAGE_3_LINE_1_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_3_2[PAGE_3_LINE_2_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_3_3[PAGE_3_LINE_3_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_3_4[PAGE_3_LINE_4_N_OPTIONS];
    #endif

    #ifdef FOURTH_OPERATOR_PAGE
    static TUSIGN8  OverloadedMainOperatorViewList_4_1[PAGE_4_LINE_1_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_4_2[PAGE_4_LINE_2_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_4_3[PAGE_4_LINE_3_N_OPTIONS];
    static TUSIGN8  OverloadedMainOperatorViewList_4_4[PAGE_4_LINE_4_N_OPTIONS];
    #endif

    TUSIGN8 *returnPtr = NULL;

    switch (objectIndex)
    {
    case HMI_IDX_mainOperatorView_1_1:          returnPtr = OverloadedMainOperatorViewList_1_1;   break;
    case HMI_IDX_mainOperatorView_1_2:          returnPtr = OverloadedMainOperatorViewList_1_2;   break;
    case HMI_IDX_mainOperatorView_1_3:          returnPtr = OverloadedMainOperatorViewList_1_3;   break;
    case HMI_IDX_mainOperatorView_1_bargraph:   returnPtr = OverloadedMainOperatorViewList_1_4;   break;

    #ifdef SECOND_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_2_1:          returnPtr = OverloadedMainOperatorViewList_2_1;   break;
    case HMI_IDX_mainOperatorView_2_2:          returnPtr = OverloadedMainOperatorViewList_2_2;   break;
    case HMI_IDX_mainOperatorView_2_3:          returnPtr = OverloadedMainOperatorViewList_2_3;   break;
    case HMI_IDX_mainOperatorView_2_bargraph:   returnPtr = OverloadedMainOperatorViewList_2_4;   break;
    #endif

    #ifdef THIRD_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_3_1:          returnPtr = OverloadedMainOperatorViewList_3_1;   break;
    case HMI_IDX_mainOperatorView_3_2:          returnPtr = OverloadedMainOperatorViewList_3_2;   break;
    case HMI_IDX_mainOperatorView_3_3:          returnPtr = OverloadedMainOperatorViewList_3_3;   break;
    case HMI_IDX_mainOperatorView_3_bargraph:   returnPtr = OverloadedMainOperatorViewList_3_4;   break;
    #endif

    #ifdef FOURTH_OPERATOR_PAGE
    case HMI_IDX_mainOperatorView_4_1:          returnPtr = OverloadedMainOperatorViewList_4_1;   break;
    case HMI_IDX_mainOperatorView_4_2:          returnPtr = OverloadedMainOperatorViewList_4_2;   break;
    case HMI_IDX_mainOperatorView_4_3:          returnPtr = OverloadedMainOperatorViewList_4_3;   break;
    case HMI_IDX_mainOperatorView_4_bargraph:   returnPtr = OverloadedMainOperatorViewList_4_4;   break;
    #endif

    default:                                    returnPtr = NULL;                                       break;
    }

    return returnPtr;
}*/
#endif





