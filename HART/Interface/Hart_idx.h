//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart

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

#ifdef __HART_IDX__H__
  #error hart_idx.h included more than once
#endif

#define __HART_IDX_H__

enum _T_HART_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Reset configuration changed flags
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_resetConfigFlag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Starts self test
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_StartSelfTest,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Internal information for status
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_bolDeviceIsBusy,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Not used (Value = 0)
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_dummy_0,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_I16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummys16,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummy32,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_dummy250,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Fixed to 254
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_dummy_254,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummyU8,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummyU16,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummyFloat,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_DOUBLE
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dummyDouble,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Bit rate code
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_bitRateCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     A value <>0 disables the FSK-Function.
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_fskDisabled,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Write protect code
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_hwWriteProtection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Min Number Request Preambles
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_numberRquestPream,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : OUTPUT
    */
    HART_IDX_numberRespPream,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Switch between HART 5 and HART 7 command revision.  Change takes only effect after a power cycle.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_hartCmdRev,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Universal Command Major Revision Number
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_univCmdRev,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Device Revision Level
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_transSpecRev,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_deviceRevisionLevel,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Manufacturer Identification Code 0x1A or ABB
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_mfrId,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Device Type as assigned by the HART Foundation.  Must be adjusted to the specific device where this subsystem is used
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_mfrDeviceType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_maxNoOfDevVariables,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_manufacturerIdCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_privateLabelDistributorCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_deviceProfile,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Fixed to 2 01 = Multisensor Field Device 02 = Eeprom Control 04 = Protocol bridge Device 08 = IEEE 802.15.4.2  4GHz DSSS
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_flags,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Configuration Flag 1 and 2
     \datatype          : CONF
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_configurationFlags,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     polling address
     \datatype          : ADDR
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_pollAddr,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_bursttotalNumberOfMsg,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_burstDefaultMsg,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_CONFVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg0ConfData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_MANAGEVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg0ManageData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_CONFVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg1ConfData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_MANAGEVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg1ManageData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_CONFVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg2ConfData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : BURSTMSG_MANAGEVALUES
     \typequalifier     : STRUCT
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_burstMsg2ManageData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Device-Tag
     \datatype          : SIMPLE_U8[6]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_tag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Device Message as 24 byte PASCII character
     \datatype          : SIMPLE_U8[24]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_deviceMessage,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Final Assembly Number
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_finalAssyNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Descriptor
     \datatype          : SIMPLE_U8[12]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_descriptor,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Device identification number
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_deviceIdNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_deviceInstallDate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 0
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot0,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 1
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 2
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 3
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot3,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 4
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot4,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 5
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot5,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 6
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot6,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Slot 7
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_slot7,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Reserved
     \datatype          : USIGN8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_reserved,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object for custom HART command handling arbitrary objects
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    HART_IDX_subsystemIndex,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object for custom HART command handling arbitrary objects
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    HART_IDX_objectIndex,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_objectDataLength,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object for custom HART command handling arbitrary objects
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    HART_IDX_attributeIndex,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object for custom HART command handling direct memory access
     \datatype          : SIMPLE_U8[32]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_objectData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     object for custom HART command handling arbitrary objects
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_board,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     object for custom HART command handling arbitrary objects
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_address,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     dummy object for custom HART command handling direct memory access
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    HART_IDX_memoryAddress,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_memoryLength,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[32]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_memoryData,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_pvtLabelDistCode_Command15,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_notUsed,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The Alarm Selection Code indicates the action taken by the device under error conditions. For transmitters, the code indicates the action by Loop Current. For Actuators, the action taken by the positioner is indicated
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_PV_AlarmSelctionCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     The transfer function code must return "0", Linear if transfer functions are not supported by the device
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_PV_Transfer_FunctionCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dampValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_pvt_Label_Dist_Code,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_activeDeadbandCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[24]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_measuringTask,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_minSwitchingTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_counterReset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_configExitTimer,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_configExitElapsedTime,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_diagCounterMoreStatusAvailablePrimMaster,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_diagCounterMoreStatusAvailableSecMaster,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_eepromControl,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     A write to this object carries out a device reset and caters for the proper response handling by walking through a reset state machine
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    HART_IDX_performDeviceReset,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_resetDevice,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_devVariableClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_devVariableTransducerSerialNo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_devVariableFamilyCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_BIT8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_devVariableStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dataTimeStamp,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_loopCurrentMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[32]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_longTag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_confChangeCounterValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_resetMoreStatusAvailable,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_pvAnalogChannelFlags,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_nonValidDeviceVariableClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_nonValidDeviceVariableUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8[4]
     \typequalifier     : ARRAY
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_nonValidDeviceVariableValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_nonValidDeviceVariableStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_deviceVariableCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_updatePeriod,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_extendedDeviceStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_deviceOperatingMode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_standardizedStatus0,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_analogChannelFixed,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     This object contains the softmodem firmware version.  Only supported if a softmodem is used in the device hardware design - not for single processor solution
     \datatype          : SIMPLE_U8[8]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_softModemInfo,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_squawk,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Configures find device HART function as used with command 73 (> HART 7 only)
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_findDeviceSwitch,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     implementation for command 71.  This funciton locks the device temporarily or permanent.  For details refer to HART 7.2 spec.
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_lockDeviceSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     See GetDeviceLockState(). This parameter reflects the access rights as applied by command 71 Lock Device.
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_lockDeviceStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT[3]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_transducerLimitsCmd14_54,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_burstUpUnitCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_burstUpUnitType,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_burstUpDataValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_burstUpBaseValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    HART_IDX_burstUpTxSelection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[3]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    HART_IDX_transducerSerialNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    HART_IDX_lastReceiveCommandNumber,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_timeValueHART,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : CONST_U8
     \typequalifier     : SIMPLE
     \storage qualifier : ROM
     \direction         : CONTAINED
    */
    HART_IDX_currentClassification,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_currentUnit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_currentStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_setPVUpRangeValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_setPVLowerRangeValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_setDeviceVariableNZero,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_resetDeviceVariableNTrim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_deviceVariableFamilyCodes,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_writeDeviceVariableCommCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : HART_SERVICECODE[20]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_ServiceCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : INPUT
    */
    HART_IDX_serviceCodeLogOut,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_dumNAN,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[2]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_countryCode,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : CONTAINED
    */
    HART_IDX_SiUnitOnly,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : ACTION
     \typequalifier     : SIMPLE
     \storage qualifier : 
     \direction         : CONTAINED
    */
    HART_IDX_performActions,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_hartInSim,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : FLOAT
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_fixedOutputValue,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_HartAccessLevel,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U8[24]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INPUT
    */
    HART_IDX_ResetSoftWriteProtection,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : CONTAINED
    */
    HART_IDX_burstInStatus
    //@SubGen end@    <ENUMERATIONS>
};




