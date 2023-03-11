//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Frame
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_int16.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "generators/Subsystem_Templates/Interface/Frame.h"
#include "frame_types.h"
#include "frame_attributes.h"
#include "frame_overload.h"
#include "frame_execute.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 frameState;     // Subsystem state
FAST T_DATA_SEMAPHORE frameDataSemaphore;



const SLOW T_DATACLASS frameDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>


//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ frameObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the FRAME-Class object
SLOW const T_FRAME frame =
{
    {
        &frameState,
        &frameDataSemaphore,

        frameObjectList,
        //lint -e{778} 
        sizeof(frameObjectList)/sizeof(frameObjectList[0])-1,

        frameDataClassList,
        sizeof(frameDataClassList)/sizeof(frameDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_FRAME, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        Put_T_UNIT,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: frame_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_FRAME_TEMPLATE frameTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_FRAME_TEMPLATE frameTemplate;
const SLOW T_FRAME_TEMPLATE frameTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$FRAME_DATA")
    const FAST T_FRAME_TEMPLATE frameTemplate;
    const FAST T_FRAME_TEMPLATE frameTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="FRAME_TEMPLATE"
    const FAST __no_init T_FRAME_TEMPLATE frameTemplate;
  #pragma location="FRAME_TEMPLATE_DEFAULT"
    const FAST __no_init T_FRAME_TEMPLATE frameTemplateDefault;
#endif
const SLOW T_FRAME_TEMPLATE frameTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$FRAME_DATA")
    const FAST T_FRAME_TEMPLATE frameTemplate;
  #pragma bss_seg()
#else
  #pragma location="FRAME_TEMPLATE"
    const FAST __no_init T_FRAME_TEMPLATE frameTemplate;
#endif
const SLOW T_FRAME_TEMPLATE frameTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
frameTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(frameTemplate,frameTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(frameTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(frameTemplate,frameTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(frameTemplate,frameTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(frameTemplate,frameTemplateInitializer,frameTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/
