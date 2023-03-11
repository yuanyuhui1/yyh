//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Hart_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Hart_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_TYPES_H__
  #error Hart_types.h included more than once
#endif

#define __HART_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of FreqStatic block
 typedef struct _T_HART_FREQ_STATIC
 {
     //@SubGen start@    <DATACLASS FreqStatic>
    TUSIGN8 burstUpTxSelection;
     //@SubGen end@    <DATACLASS FreqStatic>
 } T_HART_FREQ_STATIC;



 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_HART_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT>
    TUSIGN8 currentClassification;
     //@SubGen end@    <DATACLASS CONSTANT>
 } T_HART_CONSTANT;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_RARE block
 typedef struct _T_HART_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS STATIC_RARE>
    TUSIGN8 countryCode[2];
    TUSIGN8 SiUnitOnly;
     //@SubGen end@    <DATACLASS STATIC_RARE>
 } T_HART_STATIC_RARE;



 //-------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_HART_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC>
    TUSIGN8 currentUnit[2];
    TUSIGN8 currentStatus;
    TUSIGN8 deviceVariableFamilyCodes;
    TUSIGN8 writeDeviceVariableCommCode;
    TUSIGN32 dumNAN;
    TFLOAT hartInSim;
    TFLOAT fixedOutputValue;
    TUSIGN8 ResetSoftWriteProtection[24];
    TUSIGN8 burstInStatus;
     //@SubGen end@    <DATACLASS DYNAMIC>
 } T_HART_DYNAMIC;



 //-------------------------------------------------------------------------------------------------
 //! structure of FreqNo block
 typedef struct _T_HART_FREQ_NO
 {
     //@SubGen start@    <DATACLASS FreqNo>
    TUSIGN8 burstDefaultMsg;
    T_BURSTMSG_CONFVALUES burstMsg0ConfData;
    T_BURSTMSG_CONFVALUES burstMsg1ConfData;
    T_BURSTMSG_CONFVALUES burstMsg2ConfData;
    TUSIGN16 confChangeCounterValue;
    TUSIGN8 lockDeviceSelection;
    TUSIGN8 lockDeviceStatus;
     //@SubGen end@    <DATACLASS FreqNo>
 } T_HART_FREQ_NO;



 //-------------------------------------------------------------------------------------------------
 //! structure of Dyn block
 typedef struct _T_HART_DYN
 {
     //@SubGen start@    <DATACLASS Dyn> 
    TUSIGN8 bolDeviceIsBusy;
    TINT16 dummys16;
    TUSIGN32 dummy32;
    TUSIGN8 dummyU8;
    TUSIGN16 dummyU16;
    TFLOAT dummyFloat;
    TDOUBLE dummyDouble;
    T_BURSTMSG_MANAGEVALUES burstMsg0ManageData;
    T_BURSTMSG_MANAGEVALUES burstMsg1ManageData;
    T_BURSTMSG_MANAGEVALUES burstMsg2ManageData;
    TUSIGN8 slot0;
    TUSIGN8 slot1;
    TUSIGN8 slot2;
    TUSIGN8 slot3;
    TUSIGN8 slot4;
    TUSIGN8 slot5;
    TUSIGN8 slot6;
    TUSIGN8 slot7;
    TUSIGN16 subsystemIndex;
    TUSIGN16 objectIndex;
    TUSIGN8 objectDataLength;
    TUSIGN16 attributeIndex;
    TUSIGN8 objectData[32];
    TUSIGN8 board;
    TUSIGN8 address;
    TUSIGN16 memoryAddress;
    TUSIGN8 memoryLength;
    TUSIGN8 memoryData[32];
    TUSIGN8 pvtLabelDistCode_Command15;
    TUSIGN8 PV_AlarmSelctionCode;
    TUSIGN8 PV_Transfer_FunctionCode;
    TFLOAT dampValue;
    TUSIGN8 activeDeadbandCode;
    TUSIGN8 counterReset;
    TUSIGN8 configExitTimer;
    TUSIGN16 configExitElapsedTime;
    TUSIGN16 diagCounterMoreStatusAvailablePrimMaster;
    TUSIGN16 diagCounterMoreStatusAvailableSecMaster;
    TUSIGN8 eepromControl;
    TUSIGN8 performDeviceReset;
    TUSIGN8 devVariableStatus;
    TUSIGN32 dataTimeStamp;
    TUSIGN8 deviceVariableCode;
    TUSIGN32 updatePeriod;
    TUSIGN8 extendedDeviceStatus;
    TUSIGN8 deviceOperatingMode;
    TUSIGN8 standardizedStatus0;
    TUSIGN8 analogChannelFixed;
    TUSIGN8 softModemInfo[8];
    TUSIGN8 findDeviceSwitch;
    TFLOAT transducerLimitsCmd14_54[3];
    TUSIGN8 burstUpUnitCode[2];
    TUSIGN8 burstUpUnitType;
    TFLOAT burstUpDataValue;
    TFLOAT burstUpBaseValue;
    TUSIGN8 transducerSerialNumber[3];
    TUSIGN32 timeValueHART;
    TCHAR ServiceCode[20];
    TUSIGN8 HartAccessLevel;
     //@SubGen end@    <DATACLASS Dyn>
 } T_HART_DYN;


 //-------------------------------------------------------------------------------------------------
 //! structure of Cst block
 typedef struct _T_HART_CST
 {
     //@SubGen start@    <DATACLASS Cst>
    TUSIGN8 dummy_0;
    TUSIGN8 dummy250;
    TUSIGN8 dummy_254;
    TUSIGN8 transSpecRev;
    TUSIGN8 deviceRevisionLevel;
    TUSIGN8 mfrId;
    TUSIGN8 mfrDeviceType;
    TUSIGN8 maxNoOfDevVariables;
    TUSIGN8 manufacturerIdCode[2];
    TUSIGN8 deviceProfile;
    TUSIGN8 flags;
    TUSIGN8 bursttotalNumberOfMsg;
    TUSIGN8 notUsed;
    TUSIGN8 pvt_Label_Dist_Code;
    TUSIGN8 devVariableClassification;
    TUSIGN8 devVariableTransducerSerialNo[3];
    TUSIGN8 devVariableFamilyCode;
    TUSIGN8 pvAnalogChannelFlags;
    TUSIGN8 nonValidDeviceVariableClassification;
    TUSIGN8 nonValidDeviceVariableUnitCode;
    TUSIGN8 nonValidDeviceVariableValue[4];
    TUSIGN8 nonValidDeviceVariableStatus;
     //@SubGen end@    <DATACLASS Cst>
 } T_HART_CST;


 //-------------------------------------------------------------------------------------------------
 //! structure of FreqCst block
 typedef struct _T_HART_FREQ_CST
 {
     //@SubGen start@    <DATACLASS FreqCst>
    TUSIGN8 hartCmdRev;
    TUSIGN8 univCmdRev;
    TUSIGN8 configurationFlags;
    TUSIGN8 pollAddr;
    TUSIGN8 tag[6];
    TUSIGN8 deviceMessage[24];
    TUSIGN8 finalAssyNumber[3];
    TUSIGN8 descriptor[12];
    TUSIGN8 deviceInstallDate[3];
    TUSIGN8 reserved;
    TUSIGN8 measuringTask[24];
    TUSIGN8 minSwitchingTime;
    TUSIGN8 loopCurrentMode;
    TUSIGN8 longTag[32];
    TUSIGN8 lastReceiveCommandNumber;
     //@SubGen end@    <DATACLASS FreqCst>
 } T_HART_FREQ_CST;


 //-------------------------------------------------------------------------------------------------
 //! structure of Rare block
 typedef struct _T_HART_RARE
 {
     //@SubGen start@    <DATACLASS Rare>
    TUSIGN8 bitRateCode;
    TUSIGN8 fskDisabled;
    TUSIGN8 hwWriteProtection;
    TUSIGN8 numberRquestPream;
    TUSIGN8 numberRespPream;
    TUSIGN16 privateLabelDistributorCode;
    TUSIGN8 deviceIdNumber[3];
     //@SubGen end@    <DATACLASS Rare>
 } T_HART_RARE;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_HART_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template>
    //@SubGen end@    <DATACLASS Template>
} T_HART_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

