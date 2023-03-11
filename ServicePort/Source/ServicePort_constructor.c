//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         2 wire Top Works
 Module         Subsystem ServicePort
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
// include the data_obj-classes you need, e.g. T_DATA_OBJ::INT16
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "ServicePort/Interface/servicePort.h"
#include "servicePort_types.h"
#include "servicePort_attributes.h"
#include "servicePort_overload.h"
#include "servicePort_execute.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 servicePortState;     // Subsystem state
FAST T_DATA_SEMAPHORE servicePortDataSemaphore;



const SLOW T_DATACLASS servicePortDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(servicePortDynamic,servicePortDynamicInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_SERVICE_PORT_DYNAMIC servicePortDynamic;
const SLOW T_SERVICE_PORT_DYNAMIC servicePortDynamicInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC>
    SERVICEPORT_UNINITIALIZED_STATUS,    // SIMPLE_CHAR activationStatus
    19200    // SIMPLE_U16 baudRate
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ servicePortObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //activationStatus
    CONSTRUCT_SIMPLE_CHAR(
        &servicePortDynamic.activationStatus,
        RAM),

    //baudRate
    CONSTRUCT_SIMPLE_U16(
        &servicePortDynamic.baudRate,
        RAM)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the SERVICE_PORT-Class object
SLOW const T_SERVICE_PORT servicePort =
{
    {
        &servicePortState,
        &servicePortDataSemaphore,

        servicePortObjectList,
        //lint -e{778} 
        (sizeof(servicePortObjectList)/sizeof(servicePortObjectList[0]))-1,

        servicePortDataClassList,
        sizeof(servicePortDataClassList)/sizeof(servicePortDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_SERVICE_PORT, // will be overloaded
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
    ,ServPortCommandInterpreterEXE_SERVICEPORT
    ,ServPortUpdateActivationStatusSRV_SERVICEPORT
    ,ServPortGetActivationStatusSRV_SERVICEPORT
    ,ServPortRedetectHMISRV_SERVICEPORT
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: servicePort_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_SERVICE_PORT_TEMPLATE servicePortTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_SERVICE_PORT_TEMPLATE servicePortTemplate;
const SLOW T_SERVICE_PORT_TEMPLATE servicePortTemplateInitializer=
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
  #pragma bss_seg(".AAA$SERVICE_PORT_DATA")
    const FAST T_SERVICE_PORT_TEMPLATE servicePortTemplate;
    const FAST T_SERVICE_PORT_TEMPLATE servicePortTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="SERVICE_PORT_TEMPLATE"
    const FAST __no_init T_SERVICE_PORT_TEMPLATE servicePortTemplate;
  #pragma location="SERVICE_PORT_TEMPLATE_DEFAULT"
    const FAST __no_init T_SERVICE_PORT_TEMPLATE servicePortTemplateDefault;
#endif
const SLOW T_SERVICE_PORT_TEMPLATE servicePortTemplateInitializer=
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
  #pragma bss_seg(".AAA$SERVICE_PORT_DATA")
    const FAST T_SERVICE_PORT_TEMPLATE servicePortTemplate;
  #pragma bss_seg()
#else
  #pragma location="SERVICE_PORT_TEMPLATE"
    const FAST __no_init T_SERVICE_PORT_TEMPLATE servicePortTemplate;
#endif
const SLOW T_SERVICE_PORT_TEMPLATE servicePortTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
servicePortTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(servicePortTemplate,servicePortTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(servicePortTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(servicePortTemplate,servicePortTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(servicePortTemplate,servicePortTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(servicePortTemplate,servicePortTemplateInitializer,servicePortTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

