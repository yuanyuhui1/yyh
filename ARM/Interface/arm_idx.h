//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm

 Module

 Description    This header file defines the complete list of public data-objects.
                Other subsystems need to use these macros instead of magic-numbers  
                when accessing the public data-objects.

                The index of private data-object should not be available through
                the TUNIT interface.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifdef __ARM_IDX_H__
  #error Arm_idx.h included more than once
#endif

#define __ARM_IDX_H__

enum _T_ARM_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Standard Default)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_stdDefaultPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Advanced Default)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_advDefaultPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Service Default)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_svcDefaultPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Standard)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_stdUserPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Advanced)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_advUserPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Hashed result of 5-digit alphanumeric password (Service)
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_svcUserPassword_dummy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Per-access Incrementing Number
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_adpin,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Client write-enabled status.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_clientEnableState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     For service level, 0 = password login, 1 = login via hasher/pin
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_hiSecAccessState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     HMI client login status.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ARM_IDX_hmiLoginState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     HART client login status.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ARM_IDX_hartLoginState,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ARM_IDX_transmitterIdStringForm,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Relays the status of the hardware input to a dynamic object for externals to read.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ARM_IDX_readOnlySwitchStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Relays the status of the hardware input to a dynamic object for externals to read.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    ARM_IDX_custodySwitchStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : INPUT
    */
    ARM_IDX_enableHighSecurityMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : INPUT
    */
    ARM_IDX_disableHighSecurityMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    ARM_IDX_hardwareCalibrationLoad,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_serviceAccountSwitch,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_stdDefaultPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_advDefaultPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_svcDefaultPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_stdUserPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_advUserPassword,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32[8]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INPUT
    */
    ARM_IDX_svcUserPassword
    //@SubGen end@    <ENUMERATIONS>
};




