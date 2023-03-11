//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi

 Module

 Description    this header file defines the complete list of public data-objects.
                other subsystems must use these macros instead of magic-numbers if they want to access
                the public data-objects.

                The index of an privat data-object should not be shown, but it is not forbidden to do so.

 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifdef __HMI_IDX_H__
  #error hmi_idx.h included more than once
#endif

#define __HMI_IDX_H__

enum _T_HMI_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Enumeration - Defines the language used
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_language,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 1st operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_1_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 1st operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_1_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 1st operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_1_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 1st operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_1_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 2nd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_2_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 2nd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_2_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 2nd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_2_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 2nd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_2_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 3rd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_3_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 3rd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_3_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 3rd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_3_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 3rd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_3_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 4th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_4_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 4th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_4_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 4th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_4_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 4th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_4_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display Contrast (0..100%)
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_contrast,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Defines if HMI is allowed to make changes to parameter values
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_localOperationEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 1
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 2
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 3
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 4
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Autoscroll on/off - used when multiple operator pages are available
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_autoscrollEnable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Software revision of low level HMI component
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HMI_IDX_lowLevelSoftwareRevision,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Default contrast setting for display glass
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HMI_IDX_defaultContrast,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed in signal view (diagnostics) frame
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HMI_IDX_diagnosisView,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_5,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_6,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_7,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_8,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_9,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_10,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Custom number of decimal places
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_customDP_11,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     String entered at "Enter Password" for comparison with ARM security passwords
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_Password,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new standard level password
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_StandardLevel,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new advanced level password
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_AdvancedLevel,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new service level password
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_ServiceLevel,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new default password for standard level
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_StandardLevelDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new default password for Advanced level
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_AdvancedLevelDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Used by the HMI user to set a new default password for Service level
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    HMI_IDX_SetPassword_ServiceLevelDefault,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Defines the way in which dates are represented on the HMI
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_DateFormat,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Action Object - used to start display test
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HMI_IDX_displayTest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Software rotation of display (only effective when indicator Only Low level is fitted)
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_Rotation,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Number of lines displayed in menu & enum edit frames
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_MenuFormat,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 5
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_5,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Display format for operator page 6
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_displayMode_6,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 5th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_5_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 5th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_5_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 5th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_5_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 5th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_5_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 1st line of 6th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_6_1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 2nd line of 6th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_6_2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 3rd line of 6th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_6_3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on bargraph of 6th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_6_bargraph,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HMI_IDX_Customlanguage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 1st operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_1_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 2nd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_2_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 3rd operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_3_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 4th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_4_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 5th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_5_4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Signal to be displayed on 4th line of 6th operator page
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HMI_IDX_mainOperatorView_6_4
    //@SubGen end@    <ENUMERATIONS>
};




