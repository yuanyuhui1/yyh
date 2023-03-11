//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2006.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Components HART
 Module
 Description    Command definition and response code mapping
 Remarks

*/
//--------------------------------------------------------------------------------------------------
//@HARTGen Version@<0x010000>

#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "RTOS/Interface/rtos.h"
#include "Common/Interface/common_units.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"
//#include "..\..\..\Totalizer\interface\t_data_obj_totdispunits.h" 2013-07-25 
#include "Totalizer/Interface/t_data_obj_totdisp.h"
// @@adjust
#include "Coordinator/Interface/subsystem_idx.h"
#include "HART/Layer2/Interface/layer2.h"
#include "ObjfunctRB.h"
#include "cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "T_DATA_OBJ/Interface/t_data_obj_fixflt_Q7_8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_fixflt78_range.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"

//added 20110503 for UOM_PF, if not, compile will fail
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
#include "Totalizer/Source/t_data_obj_totfloatacc.h"
//added 20110922
#include "Totalizer/Interface/Totalizer.h"

//get the SUBSYSTEM IDs
//@HARTGen start@<Includes>
#include "HART/Interface/Hart_idx.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Totalizer/Interface/Totalizer_idx.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "DigitalOutput/Interface/DigitalOutput_idx.h"
#include "Hmi/Interface/HMI_idx.h"
//@HARTGen end@<Includes>


//#pragma constseg=SLOW HART_COMMAND_TABLE_DATA

/* Response mapping */
// First response
const TUSIGN8 mapErrFirstResponse[] =
{
   HART_RESP_ERR_DATA_TOO_SMALL,            //!<  LESSTHAN_RANGEMIN_ERR=32, at least one value is less than its minimum value
   HART_RESP_ERR_DATA_TOO_LARGE,            //!<  GREATERTHAN_RANGEMAX_ERR, at least one value is greater than its maximum
   HART_RESP_ERR_LOWER_RANGE_TOO_LOW ,      //!<  RANGE_TOO_LOW_ERR,        one value of range parameter is less than the allowed minimum value
   HART_RESP_ERR_UPPER_RANGE_TOO_HIGH,      //!<  RANGE_TOO_HIGH_ERR,       one value of range parameter is greater than the maximum
   HART_RESP_ERR_UPPER_RANGE_TOO_LOW,       //!<  UPPER_RANGE_TOO_LOW_ERR,
   HART_RESP_ERR_UPPER_RANGE_TOO_HIGH,      //!<  UPPER_RANGE_TOO_HIGH_ERR,
   HART_RESP_ERR_LOWER_RANGE_TOO_LOW,       //!<  LOWER_RANGE_TOO_LOW_ERR,
   HART_RESP_ERR_LOWER_RANGE_TOO_HIGH,      //!<  LOWER_RANGE_TOO_HIGH_ERR,
   HART_RESP_ERR_SPAN_TOO_SMALL,            //!<  SPAN_TO_SMALL_ERR,        then span between two values is too small
   HART_RESP_ERR_SET_TO_NEAREST_VALUE,      //!<  NOT_ON_GRID_ERR,          at least one value is not a multiple of its increment
   HART_RESP_ERR_INVALID_SELECTION,         //!<  RULE_VIOLATION_ERR,       this shows a violation of one or more businessrules
   HART_RESP_ERR_WRITE_PROTECTED,           //!<  READ_ONLY_ERR,            written parameter is read only
   HART_RESP_ERR_ACCESS_RESTRICTED          //!<  WRONG_STATE_ERR,          written parameter is read only in current subsystem state
};


const TUSIGN8 mapWarnFirstResponse[] =
{
   HART_RESP_OK,                            //!< OK                    Correct
   HART_RESP_WARN_LOWER_RANGE_TOO_LOW,      //!< LESSTHAN_RANGEMIN=1,  at least one value is less than its minimum value
   HART_RESP_WARN_UPPER_RANGE_TOO_HIGH,     //!< GREATERTHAN_RANGEMAX, at least one value is greater than its maximum
   HART_RESP_OK,                            //!< RANGE_TOO_LOW,        one value of range parameter is less than the allowed minimum value
   HART_RESP_OK,                            //!< RANGE_TOO_HIGH,       one value of range parameter is greater than the maximum
   HART_RESP_OK,                            //!< UPPER_RANGE_TOO_LOW,    upper value of range parameter is less than the allowed minimum value
   HART_RESP_OK,                            //!< UPPER_RANGE_TOO_HIGH,   upper value of range parameter is greater than the maximum
   HART_RESP_OK,                            //!< LOWER_RANGE_TOO_LOW,    lower value of range parameter is less than the allowed minimum value
   HART_RESP_OK,                            //!< LOWER_RANGE_TOO_HIGH,   lower value of range parameter is greater than the maximum
   HART_RESP_WARN_SPAN_TOO_SMALL,           //!< SPAN_TO_SMALL,        then span between two values is too small
   HART_RESP_WARN_SET_TO_NEAREST_VALUE,     //!< NOT_ON_GRID,          at least one value is not a multiple of its increment
   HART_RESP_WARN_INVALID_SELECTION,        //!< RULE_VIOLATION,       this shows a violation of one or more businessrules
   HART_RESP_WARN_ACCESS_RESTRICTED,        //!< READ_ONLY,            written parameter is read only
   HART_RESP_WARN_ACCESS_RESTRICTED         //!< WRONG_STATE,          written parameter is read only in current subsystem state
};


const TUSIGN8 mapParaLength[] =
{
     0,   //    HART_LEN_0
     1,   //    HART_LEN_1
     2,   //    HART_LEN_2
     3,   //    HART_LEN_3
     4,   //    HART_LEN_4
     5,   //    HART_LEN_5
     6,   //    HART_LEN_6
     8,   //    HART_LEN_8
     9,   //    HART_LEN_9
     12,  //    HART_LEN_12
     15,  //    HART_LEN_15
     16,  //    HART_LEN_16
     20,  //    HART_LEN_20
     24,  //    HART_LEN_24
     28,  //    HART_LEN_28
     32,   //    HART_LEN_32
};

const TUSIGN8 dataTypeLength[] =
{
     1,   // ONE_BYTES     0
     2,   // TWO_BYTES     1
     3,   // THREE_BYTES   2
     4,   // FOUR_BYTES    3
     6,   // SIX_BYTES     4
     8,   // EIGHT_BYTES   5
     9    // NINE_BYTES    6
};

const T_TYPE_CONV t_range_float_u[] =
{
     {TD_USIGN8, 1},
     {TD_FLOAT,  4},
     {TD_FLOAT,  4}
};

const T_TYPE_CONV t_range_float[] =
{
     {TD_FLOAT, 4},
     {TD_FLOAT, 4}
};

// Second response

/* End: Response mapping */

// table of command 9 specific non valid device variable slot objects
const COM_OBJ_DESCR specObjCom9_nonValidDeviceVariableObjDescrTab[] =
{
  {HART_IDX, HART_IDX_nonValidDeviceVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_nonValidDeviceVariableUnitCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_nonValidDeviceVariableValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_4}, // FIXFLOAT_Q7_8
  {HART_IDX, HART_IDX_nonValidDeviceVariableStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_BIT8
};

//added 20110509  for special non valid slot index......
// table of command 33 specific non valid device variable slot objects
const COM_OBJ_DESCR specObjCom33_nonValidDeviceVariableObjDescrTab[] =
{
  {HART_IDX, HART_IDX_nonValidDeviceVariableUnitCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_nonValidDeviceVariableValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_4} // FIXFLOAT_Q7_8
};


//@HARTGen start@<COMMANDDEFINITION> 
/*
,--- read-command 0 -------------------------------------------------------
| ReadUniqueIdentifier
| Read Unique Identifier
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                                            byte(s)     type        object (default)

 +--------------------------------------->       #0  TD_USIGN8  Hart.dummy_254 (254)
 | +------------------------------------->       #1  TD_USIGN8  Hart.mfrId (0x1A)
 | | +----------------------------------->       #2  TD_USIGN8  Hart.mfrDeviceType (163)
 | | | +--------------------------------->       #3  TD_USIGN8  Hart.numberRquestPream (5)
 | | | | +------------------------------->       #4  TD_USIGN8  Hart.univCmdRev (HART_REVISION_7)
 | | | | | +----------------------------->       #5  TD_USIGN8  Hart.transSpecRev (1)
 | | | | | | +--------------------------->       #6  TD_USIGN8  ElettronicServices.softwareMainRevision (0)
 | | | | | | | +------------------------->       #7  TD_USIGN8  ElettronicServices.hardwareMainRevision (0)
 | | | | | | | | +----------------------->       #8  TD_USIGN8  Hart.flags (2)
 | | | | | | | | |  +-------------------->  #9..#11 TD_USIGN24  Hart.deviceIdNumber (1,0,0)
 | | | | | | | | |  |  +----------------->      #12  TD_USIGN8  Hart.numberRespPream (5)
 | | | | | | | | |  |  | +--------------->      #13  TD_USIGN8  Hart.maxNoOfDevVariables (18)
 | | | | | | | | |  |  | |  +------------> #14..#15 TD_USIGN16  Hart.confChangeCounterValue (0)
 | | | | | | | | |  |  | |  | +---------->      #16  TD_USIGN8  Hart.extendedDeviceStatus (DEFAULT_EXTENDED_DEVICE_STATUS)
 | | | | | | | | |  |  | |  | |  +-------> #17..#18  TD_USIGN8  Hart.manufacturerIdCode[0..1] (0x00,0x1A)
 | | | | | | | | |  |  | |  | |  |  +----> #19..#20 TD_USIGN16  Hart.privateLabelDistributorCode (0x001A)
 | | | | | | | | |  |  | |  | |  |  | +-->      #21  TD_USIGN8  Hart.deviceProfile (1)
 | | | | | | | | |  |  | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
| | | | | | | | | |   | | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
 0                                   21


*/
const COM_OBJ_DESCR objCom0[] = 
{
  {HART_IDX, HART_IDX_dummy_254, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrId, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrDeviceType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_numberRquestPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_univCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_transSpecRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_softwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hardwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_flags, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_deviceIdNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  {HART_IDX, HART_IDX_numberRespPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_maxNoOfDevVariables, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_confChangeCounterValue, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_extendedDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_manufacturerIdCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // CONST_U8[0..1]
  {HART_IDX, HART_IDX_privateLabelDistributorCode, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_deviceProfile, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // CONST_U8
};


/*
,--- read-command 1 -------------------------------------------------------
| ReadPrimaryVariable
| Read Primary Variable
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 |   +--->   #1..#4   TD_FLOAT  Mapper.PV (6.1f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR objCom1[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- read-command 2 -------------------------------------------------------
| ReadLoopCurrentAndPercentOfRange
| Read Loop Current And Percend of Range
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    +--->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


*/
const COM_OBJ_DESCR objCom2[] = 
{
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- read-command 3 -------------------------------------------------------
| ReadDynamicVariablesAndLoopCurrent
| Read Dynamic Variables and Loop Current
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                                      byte(s)     type        object (default)

   +------------------------------->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |  +---------------------------->       #4    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
   |  |   +------------------------>   #5..#8   TD_FLOAT  Mapper.PV (6.1f)
   |  |   |  +--------------------->       #9    TD_UNIT  Mapper.SV_Unit[0] (CELSIUS)
   |  |   |  |   +-----------------> #10..#13   TD_FLOAT  Mapper.SV (0.0f)
   |  |   |  |   |  +-------------->      #14    TD_UNIT  Mapper.TV_Unit[0] (KILOGRAM_PER_HOUR)
   |  |   |  |   |  |   +----------> #15..#18   TD_FLOAT  Mapper.TV (0.0f)
   |  |   |  |   |  |   |  +------->      #19    TD_UNIT  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
   |  |   |  |   |  |   |  |   +---> #20..#23   TD_FLOAT  Mapper.QV (0.0f)
   |  |   |  |   |  |   |  |   | 
+----+-+----+-+----+-+----+-+----+
|    | |    | |    | |    | |    |
+----+-+----+-+----+-+----+-+----+
 0                             23


*/
const COM_OBJ_DESCR objCom3[] = 
{
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_SV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_SV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_TV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_TV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- write-command 6 -------------------------------------------------------
| WritePollingAdress
| Write Polling Address
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.pollAddr (0)
 | +-->       #1  TD_USIGN8  Hart.loopCurrentMode (1)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.pollAddr (0)
 | +-->       #1  TD_USIGN8  Hart.loopCurrentMode (1)
 | |
+-+-+
| | |
+-+-+
 0 1


*/
const COM_OBJ_DESCR specObjCom6_Resp[] = 
{
  {HART_IDX, HART_IDX_pollAddr, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // ADDR
  {HART_IDX, HART_IDX_loopCurrentMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom6_Req[] = 
{
  {HART_IDX, HART_IDX_pollAddr, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // ADDR
  {HART_IDX, HART_IDX_loopCurrentMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom6_ReqRespObjCnt[] = 
{
  { 1, 1, 2},   // 1 REQUEST Objects (1 Byte), 2 RESPOND Objects
  { 2, 2, 2}   // 2 REQUEST Objects (2 Byte), 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom6[] = 
{
  { specObjCom6_Req, specObjCom6_Resp, specObjCom6_ReqRespObjCnt, 2, NULL, 0, 0, NULL}
};


/*
,--- read-command 7 -------------------------------------------------------
| ReadLoopConfiguration
| Read Loop Configuration
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.pollAddr (0)
 | +-->       #1  TD_USIGN8  Hart.loopCurrentMode (1)
 | |
+-+-+
| | |
+-+-+
 0 1


*/
const COM_OBJ_DESCR objCom7[] = 
{
  {HART_IDX, HART_IDX_pollAddr, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // ADDR
  {HART_IDX, HART_IDX_loopCurrentMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};


/*
,--- read-command 8 -------------------------------------------------------
| ReadDynamicVariableClassification
| Read Dynamic Variable Classification
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

             byte(s)     type        object (default)

 +-------->       #0 TD_UNITTYPE  Mapper.PV_Classification (0)
 | +------>       #1 TD_UNITTYPE  Mapper.SV_Classification (0)
 | | +---->       #2 TD_UNITTYPE  Mapper.TV_Classification (0)
 | | | +-->       #3 TD_UNITTYPE  Mapper.QV_Classification (0)
 | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
 0     3


*/
const COM_OBJ_DESCR objCom8[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_SV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_TV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_QV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1} // USIGN8
};


/*
,--- read-command 9 -------------------------------------------------------
| ReadDeviceVariablesWithStatus
| Read Device Variables With Status
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                     byte(s)     type        object (default)

 +---------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-------------->       #1  TD_USIGN8  Hart.slot1 (SLOT_CODE_NONE)
 | | +------------>       #2  TD_USIGN8  Hart.slot2 (SLOT_CODE_NONE)
 | | | +---------->       #3  TD_USIGN8  Hart.slot3 (SLOT_CODE_NONE)
 | | | | +-------->       #4  TD_USIGN8  Hart.slot4 (SLOT_CODE_NONE)
 | | | | | +------>       #5  TD_USIGN8  Hart.slot5 (SLOT_CODE_NONE)
 | | | | | | +---->       #6  TD_USIGN8  Hart.slot6 (SLOT_CODE_NONE)
 | | | | | | | +-->       #7  TD_USIGN8  Hart.slot7 (SLOT_CODE_NONE)
 | | | | | | | |
+-+-+-+-+-+-+-+-+
| | | | | | | | |
+-+-+-+-+-+-+-+-+
 0             7


,----------.
| RESPONSE |
`----------´

                                                                                                                    byte(s)     type        object (default)

 +--------------------------------------------------------------------------------------------------------------->       #0  TD_USIGN8  Hart.extendedDeviceStatus (DEFAULT_EXTENDED_DEVICE_STATUS)
 | +------------------------------------------------------------------------------------------------------------->       #1  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | | +----------------------------------------------------------------------------------------------------------->       #2  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | +--------------------------------------------------------------------------------------------------------->       #3    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 | | | |   +----------------------------------------------------------------------------------------------------->   #4..#7   TD_FLOAT  Mapper.PV (6.1f)
 | | | |   |  +-------------------------------------------------------------------------------------------------->       #8  TD_USIGN8  Mapper.PV_Status (0)
 | | | |   |  | +------------------------------------------------------------------------------------------------>       #9  TD_USIGN8  Hart.slot1 (SLOT_CODE_NONE)
 | | | |   |  | | +---------------------------------------------------------------------------------------------->      #10  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | +-------------------------------------------------------------------------------------------->      #11  TD_USIGN8  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 | | | |   |  | | | |   +----------------------------------------------------------------------------------------> #12..#15   TD_FLOAT  Mapper.PV (6.1f)
 | | | |   |  | | | |   |  +------------------------------------------------------------------------------------->      #16  TD_USIGN8  Mapper.PV_Status (0)
 | | | |   |  | | | |   |  | +----------------------------------------------------------------------------------->      #17  TD_USIGN8  Hart.slot2 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | +--------------------------------------------------------------------------------->      #18  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | +------------------------------------------------------------------------------->      #19  TD_USIGN8  Mapper.SV_Unit[0] (CELSIUS)
 | | | |   |  | | | |   |  | | | |   +---------------------------------------------------------------------------> #20..#23   TD_FLOAT  Mapper.SV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  +------------------------------------------------------------------------>      #24  TD_USIGN8  Mapper.SV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | +---------------------------------------------------------------------->      #25  TD_USIGN8  Hart.slot3 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | | |   |  | | +-------------------------------------------------------------------->      #26  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | |   |  | | | +------------------------------------------------------------------>      #27  TD_USIGN8  Mapper.TV_Unit[0] (KILOGRAM_PER_HOUR)
 | | | |   |  | | | |   |  | | | |   |  | | | |   +--------------------------------------------------------------> #28..#31   TD_FLOAT  Mapper.TV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  +----------------------------------------------------------->      #32  TD_USIGN8  Mapper.TV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | +--------------------------------------------------------->      #33  TD_USIGN8  Hart.slot4 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | +------------------------------------------------------->      #34  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | +----------------------------------------------------->      #35  TD_USIGN8  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   +-------------------------------------------------> #36..#39   TD_FLOAT  Mapper.QV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  +---------------------------------------------->      #40  TD_USIGN8  Mapper.QV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | +-------------------------------------------->      #41  TD_USIGN8  Hart.slot5 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | +------------------------------------------>      #42  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | +---------------------------------------->      #43    TD_UNIT  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   +------------------------------------> #44..#47   TD_FLOAT  Mapper.QV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  +--------------------------------->      #48  TD_USIGN8  Mapper.QV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | +------------------------------->      #49  TD_USIGN8  Hart.slot6 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | +----------------------------->      #50  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | +--------------------------->      #51  TD_USIGN8  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   +-----------------------> #52..#55   TD_FLOAT  Mapper.QV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  +-------------------->      #56  TD_USIGN8  Mapper.QV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | +------------------>      #57  TD_USIGN8  Hart.slot7 (SLOT_CODE_NONE)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | +---------------->      #58  TD_USIGN8  Hart.devVariableClassification (0x00)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | +-------------->      #59    TD_UNIT  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   +----------> #60..#63   TD_FLOAT  Mapper.QV (0.0f)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  +------->      #64  TD_USIGN8  Mapper.QV_Status (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  |   +---> #65..#68 TD_USIGN32  Coordinator.timeStamp (0)
 | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  | | | |   |  |   | 
+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+----+
| | | | |    | | | | |    | | | | |    | | | | |    | | | | |    | | | | |    | | | | |    | | | | |    | |    |
+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+-+-+-+----+-+----+
 0                                                                                                           68


,------------.
| SLOTCODE 0 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  SVMeasurement.QvClassification (UT_VOLUME_FLOW)
 | +--------->       #1    TD_UNIT  SVMeasurement.QvDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  SVMeasurement.QvDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  SVMeasurement.QvStatus (0x00)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 1 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  MVMeasurement.QnClassification (T_VOLUME_FLOW)
 | +--------->       #1    TD_UNIT  MVMeasurement.QnDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QnDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QnStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 2 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  MVMeasurement.QmClassification (T_MASS_FLOW)
 | +--------->       #1    TD_UNIT  MVMeasurement.QmDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QmDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QmStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 3 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  MVMeasurement.QpClassification (T_POWER)
 | +--------->       #1    TD_UNIT  MVMeasurement.QpDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QpDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QpStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 4 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  MVMeasurement.QvPartialClassification (T_VOLUME_FLOW)
 | +--------->       #1    TD_UNIT  MVMeasurement.QvPartialDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QvPartialDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QvPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 5 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  MVMeasurement.QnPartialClassification (T_VOLUME_FLOW)
 | +--------->       #1    TD_UNIT  MVMeasurement.QnPartialDampedObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QnPartialDampedObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QnPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 6 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Coordinator.frequency_Classification (T_FREQUENCY)
 | +--------->       #1    TD_UNIT  Coordinator.frequency_Damped_Obj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Coordinator.frequency_Damped_Obj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  Coordinator.frequency_Status (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 7 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Coordinator.temperature_Classification (T_TEMPERATURE)
 | +--------->       #1    TD_UNIT  Coordinator.temperature_Damped_Obj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Coordinator.temperature_Damped_Obj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  Coordinator.temperature_Status (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 8 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.dvTExtClassification (T_TEMPERATURE)
 | +--------->       #1    TD_UNIT  Mapper.dvTExtObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvTExtUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvTExtStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 9 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.dvPressureClassification (T_PRESSURE)
 | +--------->       #1    TD_UNIT  Mapper.dvPressureObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvPressureUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvPressureStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 10 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.dvDensityClassification (T_MASS_DENSITY)
 | +--------->       #1    TD_UNIT  Mapper.dvDensityObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvDensityUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvDensityStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 11 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.dvContentClassification (T_ANALYTICAL)
 | +--------->       #1  TD_USIGN8  Mapper.dvDummyUnitCode (0)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvContent (1.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvDensityStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 12 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totQvClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
 | +--------->       #1    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totQvFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totQvStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 13 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totQnClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
 | +--------->       #1    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totQnFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totQnStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 14 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totQmClassification (TOTALIZER_TotalizerUnitsTypeIDXMass)
 | +--------->       #1    TD_UNIT  Totalizer.totQmUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKilograms)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totQmFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totQmStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 15 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totEnergyClassification (TOTALIZER_TotalizerUnitsTypeIDXEnergy)
 | +--------->       #1    TD_UNIT  Totalizer.totEnergyUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKiloWattHour)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totEnergyFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totEnergyStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 16 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totQvPartialClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
 | +--------->       #1    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totQvPartialFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totQvPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 17 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Totalizer.totQnPartialClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
 | +--------->       #1    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 | |   +----->   #2..#5   TD_FLOAT  Totalizer.totQnPartialFloatUserUnit (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Totalizer.totQnPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 245 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Hart.currentClassification (T_ELECTRIC_CURRENT)
 | +--------->       #1    TD_UNIT  Hart.currentUnit[0] (CURRENT_MILLIAMPERES)
 | |   +----->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   |  +-->       #6  TD_USIGN8  Hart.currentStatus (0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 246 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.PV_Classification (0)
 | +--------->       #1    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.PV (6.1f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.PV_Status (0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 247 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.SV_Classification (0)
 | +--------->       #1    TD_UNIT  Mapper.SV_Unit[0] (CELSIUS)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.SV (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.SV_Status (0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 248 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.TV_Classification (0)
 | +--------->       #1    TD_UNIT  Mapper.TV_Unit[0] (KILOGRAM_PER_HOUR)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.TV (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.TV_Status (0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 249 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0 TD_UNITTYPE  Mapper.QV_Classification (0)
 | +--------->       #1    TD_UNIT  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.QV (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.QV_Status (0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


*/
const COM_OBJ_DESCR specObjCom9_Resp[] = 
{
  {HART_IDX, HART_IDX_extendedDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_SV_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_SV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_SV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_TV_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_TV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_TV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot5, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot6, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_slot7, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {CDT_IDX, CDT_IDX_timeStamp, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_OBJ_DESCR specObjCom9_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot5, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot6, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot7, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom9_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,245,246,247,248,249
};

const COM_OBJ_DESCR specObjCom9_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {SVM_IDX, SVM_IDX_QvStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QmStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QpStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QvPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequency_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_frequency_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_Damped_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_frequency_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperature_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_temperature_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvTExtStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvPressureStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensityClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvDensityStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_dvContentClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvDummyUnitCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContent, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvDensityStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 14
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 15
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 16
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 17
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 245
  {HART_IDX, HART_IDX_currentClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_currentUnit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // SIMPLE_U8[0]
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_currentStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 246
  {MAPPER_IDX, MAPPER_IDX_PV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 247
  {MAPPER_IDX, MAPPER_IDX_SV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_SV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_SV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_SV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 248
  {MAPPER_IDX, MAPPER_IDX_TV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_TV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_TV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_TV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 249
  {MAPPER_IDX, MAPPER_IDX_QV_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // USIGN8
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_QV_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // USIGN8
};

const COM_REQ_RESP_CNT specObjCom9_ReqRespObjCnt[] = 
{
  { 1, 1, 6},   // slot 0: 1 REQUEST Objects (1 Byte) - 6 RESPOND Objects
  { 2, 2,11},   // slot 1: 2 REQUEST Objects (2 Byte) - 11 RESPOND Objects
  { 3, 3,16},   // slot 2: 3 REQUEST Objects (3 Byte) - 16 RESPOND Objects
  { 4, 4,21},   // slot 3: 4 REQUEST Objects (4 Byte) - 21 RESPOND Objects
  { 5, 5,26},   // slot 4: 5 REQUEST Objects (5 Byte) - 26 RESPOND Objects
  { 6, 6,31},   // slot 5: 6 REQUEST Objects (6 Byte) - 31 RESPOND Objects
  { 7, 7,36},   // slot 6: 7 REQUEST Objects (7 Byte) - 36 RESPOND Objects
  { 8, 8,42}    // slot 7: 8 REQUEST Objects (8 Byte) - 42 RESPOND Objects
};

const COM_SPEC_TAB specObjCom9[] = 
{
  { specObjCom9_Req, specObjCom9_Resp, specObjCom9_ReqRespObjCnt, 8, specObjCom9_slotCodeObjDescrTab, 22, 4, specObjCom9_slotCodeTranslationTable}
};


/*
,--- read-command 11 -------------------------------------------------------
| ReadUniqueIdentifierAssiciatedWithTag
| Read Unique Identifier Assiciated With Tag
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |  
+------+
|      |
+------+
 0    5


,----------.
| RESPONSE |
`----------´

                                            byte(s)     type        object (default)

 +--------------------------------------->       #0  TD_USIGN8  Hart.dummy_254 (254)
 | +------------------------------------->       #1  TD_USIGN8  Hart.mfrId (0x1A)
 | | +----------------------------------->       #2  TD_USIGN8  Hart.mfrDeviceType (163)
 | | | +--------------------------------->       #3  TD_USIGN8  Hart.numberRquestPream (5)
 | | | | +------------------------------->       #4  TD_USIGN8  Hart.univCmdRev (HART_REVISION_7)
 | | | | | +----------------------------->       #5  TD_USIGN8  Hart.transSpecRev (1)
 | | | | | | +--------------------------->       #6  TD_USIGN8  ElettronicServices.softwareMainRevision (0)
 | | | | | | | +------------------------->       #7  TD_USIGN8  ElettronicServices.hardwareMainRevision (0)
 | | | | | | | | +----------------------->       #8  TD_USIGN8  Hart.flags (2)
 | | | | | | | | |  +-------------------->  #9..#11 TD_USIGN24  Hart.deviceIdNumber (1,0,0)
 | | | | | | | | |  |  +----------------->      #12  TD_USIGN8  Hart.numberRespPream (5)
 | | | | | | | | |  |  | +--------------->      #13  TD_USIGN8  Hart.maxNoOfDevVariables (18)
 | | | | | | | | |  |  | |  +------------> #14..#15 TD_USIGN16  Hart.confChangeCounterValue (0)
 | | | | | | | | |  |  | |  | +---------->      #16  TD_USIGN8  Hart.extendedDeviceStatus (DEFAULT_EXTENDED_DEVICE_STATUS)
 | | | | | | | | |  |  | |  | |  +-------> #17..#18  TD_USIGN8  Hart.manufacturerIdCode[0..1] (0x00,0x1A)
 | | | | | | | | |  |  | |  | |  |  +----> #19..#20 TD_USIGN16  Hart.privateLabelDistributorCode (0x001A)
 | | | | | | | | |  |  | |  | |  |  | +-->      #21  TD_USIGN8  Hart.deviceProfile (1)
 | | | | | | | | |  |  | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
| | | | | | | | | |   | | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
 0                                   21


*/
const COM_OBJ_DESCR specObjCom11_Resp[] = 
{
  {HART_IDX, HART_IDX_dummy_254, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrId, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrDeviceType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_numberRquestPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_univCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_transSpecRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_softwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hardwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_flags, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_deviceIdNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  {HART_IDX, HART_IDX_numberRespPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_maxNoOfDevVariables, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_confChangeCounterValue, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_extendedDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_manufacturerIdCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // CONST_U8[0..1]
  {HART_IDX, HART_IDX_privateLabelDistributorCode, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_deviceProfile, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // CONST_U8
};

const COM_OBJ_DESCR specObjCom11_Req[] = 
{
  {HART_IDX, HART_IDX_tag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6} // SIMPLE_U8[0..5]
};

const COM_REQ_RESP_CNT specObjCom11_ReqRespObjCnt[] = 
{
  { 6, 1,17}   // 1 REQUEST Objects (6 Byte), 17 RESPOND Objects
};

const COM_SPEC_TAB specObjCom11[] = 
{
  { specObjCom11_Req, specObjCom11_Resp, specObjCom11_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 12 -------------------------------------------------------
| ReadMessage
| Read Message
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.deviceMessage[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x20,0x08,0x82)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


*/
const COM_OBJ_DESCR objCom12[] = 
{
  {HART_IDX, HART_IDX_deviceMessage, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24} // SIMPLE_U8[0..23]
};


/*
,--- read-command 13 -------------------------------------------------------
| ReadTagDescriptorDate
| Read Tag Descriptor Date
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                             byte(s)     type        object (default)

    +--------------------->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |         +----------->  #6..#17  TD_USIGN8  Hart.descriptor[0..11] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
    |         |       +---> #18..#20 TD_USIGN24  Hart.deviceInstallDate (0x01,0x01,0x00)
    |         |       | 
+------+------------+---+
|      |            |   |
+------+------------+---+
 0                    20


*/
const COM_OBJ_DESCR objCom13[] = 
{
  {HART_IDX, HART_IDX_tag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_descriptor, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_U8[0..11]
  {HART_IDX, HART_IDX_deviceInstallDate, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3} // SIMPLE_U8
};


/*
,--- read-command 14 -------------------------------------------------------
| ReadPrimaryVariableTransducerInformation
| Read Primary Variable Transducer Information
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                          byte(s)     type        object (default)

  +-------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +----------------->       #3    TD_UNIT  Mapper.PVSensorLimitUnitcode[0] (0)
  |  |   +------------->   #4..#7   TD_FLOAT  Mapper.PVSensorLimitHigh (500000.0f)
  |  |   |    +-------->  #8..#11   TD_FLOAT  Mapper.PVSensorLimitLow (0.0f)
  |  |   |    |    +---> #12..#15   TD_FLOAT  Mapper.PVSensorminimumSpan (0.1f)
  |  |   |    |    | 
+---+-+----+----+----+
|   | |    |    |    |
+---+-+----+----+----+
 0                 15


*/
const COM_OBJ_DESCR objCom14[] = 
{
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MAPPER_IDX, MAPPER_IDX_PVSensorLimitUnitcode, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // SIMPLE_U8[0]
  {MAPPER_IDX, MAPPER_IDX_PVSensorLimitHigh, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_PVSensorLimitLow, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_PVSensorminimumSpan, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- read-command 15 -------------------------------------------------------
| ReadDeviceInformation
| Read Device Information
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                                byte(s)     type        object (default)

 +--------------------------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------------------------->       #1  TD_USIGN8  Mapper.PV_TFCode (LINEAR)
 | | +----------------------->       #2    TD_UNIT  Mapper.PV_RangeUnit[0] (CUBIC_METER_PER_HOUR)
 | | |   +------------------->   #3..#6   TD_FLOAT  Mapper.PV_UpperRange (110.0f)
 | | |   |    +-------------->  #7..#10   TD_FLOAT  Mapper.PV_LowerRange (6.1f)
 | | |   |    |    +---------> #11..#14   TD_FLOAT  Mapper.PV_DampingTime (0.0f)
 | | |   |    |    |  +------>      #15  TD_USIGN8  Hart.hwWriteProtection (251)
 | | |   |    |    |  | +---->      #16  TD_USIGN8  Hart.pvtLabelDistCode_Command15 (VALUE_USED_IN_HART7)
 | | |   |    |    |  | | +-->      #17  TD_USIGN8  Mapper.PV_ACFlag (NOT_CONNECTED)
 | | |   |    |    |  | | |
+-+-+-+----+----+----+-+-+-+
| | | |    |    |    | | | |
+-+-+-+----+----+----+-+-+-+
 0                       17


*/
const COM_OBJ_DESCR objCom15[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_PV_TFCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_PV_RangeUnit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV_UpperRange, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_LowerRange, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_hwWriteProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_pvtLabelDistCode_Command15, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_PV_ACFlag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- read-command 16 -------------------------------------------------------
| ReadFinalAssemblyNumber
| Read Final Assembly Number
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.finalAssyNumber (0,0,0)
  | 
+---+
|   |
+---+
 0 2


*/
const COM_OBJ_DESCR objCom16[] = 
{
  {HART_IDX, HART_IDX_finalAssyNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3} // SIMPLE_U8
};


/*
,--- write-command 17 -------------------------------------------------------
| WriteMessage
| Write Message
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.deviceMessage[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x20,0x08,0x82)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


,----------.
| RESPONSE |
`----------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.deviceMessage[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x20,0x08,0x82)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


*/
const COM_OBJ_DESCR objCom17[] = 
{
  {HART_IDX, HART_IDX_deviceMessage, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24} // SIMPLE_U8[0..23]
};


/*
,--- write-command 18 -------------------------------------------------------
| WriteTagDescriptorDate
| Write Tag Descriptor Date
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                             byte(s)     type        object (default)

    +--------------------->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |         +----------->  #6..#17  TD_USIGN8  Hart.descriptor[0..11] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
    |         |       +---> #18..#20 TD_USIGN24  Hart.deviceInstallDate (0x01,0x01,0x00)
    |         |       | 
+------+------------+---+
|      |            |   |
+------+------------+---+
 0                    20


,----------.
| RESPONSE |
`----------´

                             byte(s)     type        object (default)

    +--------------------->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |         +----------->  #6..#17  TD_USIGN8  Hart.descriptor[0..11] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
    |         |       +---> #18..#20 TD_USIGN24  Hart.deviceInstallDate (0x01,0x01,0x00)
    |         |       | 
+------+------------+---+
|      |            |   |
+------+------------+---+
 0                    20


*/
const COM_OBJ_DESCR objCom18[] = 
{
  {HART_IDX, HART_IDX_tag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_descriptor, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_U8[0..11]
  {HART_IDX, HART_IDX_deviceInstallDate, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3} // SIMPLE_U8
};


/*
,--- write-command 19 -------------------------------------------------------
| WriteFinalAssemblyNumber
| Write Final Assembly Number
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.finalAssyNumber (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.finalAssyNumber (0,0,0)
  | 
+---+
|   |
+---+
 0 2


*/
const COM_OBJ_DESCR objCom19[] = 
{
  {HART_IDX, HART_IDX_finalAssyNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3} // SIMPLE_U8
};


/*
,--- read-command 20 -------------------------------------------------------
| ReadLongTag
| Read Long Tag
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


*/
const COM_OBJ_DESCR objCom20[] = 
{
  {HART_IDX, HART_IDX_longTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};


/*
,--- read-command 21 -------------------------------------------------------
| ReadUniqueIdentifierAssiciatedWithLongTag
| Read Unique Identifier Assiciated With Long Tag
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,----------.
| RESPONSE |
`----------´

                                            byte(s)     type        object (default)

 +--------------------------------------->       #0  TD_USIGN8  Hart.dummy_254 (254)
 | +------------------------------------->       #1  TD_USIGN8  Hart.mfrId (0x1A)
 | | +----------------------------------->       #2  TD_USIGN8  Hart.mfrDeviceType (163)
 | | | +--------------------------------->       #3  TD_USIGN8  Hart.numberRquestPream (5)
 | | | | +------------------------------->       #4  TD_USIGN8  Hart.univCmdRev (HART_REVISION_7)
 | | | | | +----------------------------->       #5  TD_USIGN8  Hart.transSpecRev (1)
 | | | | | | +--------------------------->       #6  TD_USIGN8  ElettronicServices.softwareMainRevision (0)
 | | | | | | | +------------------------->       #7  TD_USIGN8  ElettronicServices.hardwareMainRevision (0)
 | | | | | | | | +----------------------->       #8  TD_USIGN8  Hart.flags (2)
 | | | | | | | | |  +-------------------->  #9..#11 TD_USIGN24  Hart.deviceIdNumber (1,0,0)
 | | | | | | | | |  |  +----------------->      #12  TD_USIGN8  Hart.numberRespPream (5)
 | | | | | | | | |  |  | +--------------->      #13  TD_USIGN8  Hart.maxNoOfDevVariables (18)
 | | | | | | | | |  |  | |  +------------> #14..#15 TD_USIGN16  Hart.confChangeCounterValue (0)
 | | | | | | | | |  |  | |  | +---------->      #16  TD_USIGN8  Hart.extendedDeviceStatus (DEFAULT_EXTENDED_DEVICE_STATUS)
 | | | | | | | | |  |  | |  | |  +-------> #17..#18  TD_USIGN8  Hart.manufacturerIdCode[0..1] (0x00,0x1A)
 | | | | | | | | |  |  | |  | |  |  +----> #19..#20 TD_USIGN16  Hart.privateLabelDistributorCode (0x001A)
 | | | | | | | | |  |  | |  | |  |  | +-->      #21  TD_USIGN8  Hart.deviceProfile (1)
 | | | | | | | | |  |  | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
| | | | | | | | | |   | | |  | |  |  | |
+-+-+-+-+-+-+-+-+-+---+-+-+--+-+--+--+-+
 0                                   21


*/
const COM_OBJ_DESCR specObjCom21_Resp[] = 
{
  {HART_IDX, HART_IDX_dummy_254, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrId, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_mfrDeviceType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_numberRquestPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_univCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_transSpecRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_softwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hardwareMainRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_flags, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_deviceIdNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  {HART_IDX, HART_IDX_numberRespPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  {HART_IDX, HART_IDX_maxNoOfDevVariables, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_confChangeCounterValue, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_extendedDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_manufacturerIdCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // CONST_U8[0..1]
  {HART_IDX, HART_IDX_privateLabelDistributorCode, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_deviceProfile, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // CONST_U8
};

const COM_OBJ_DESCR specObjCom21_Req[] = 
{
  {HART_IDX, HART_IDX_longTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};

const COM_REQ_RESP_CNT specObjCom21_ReqRespObjCnt[] = 
{
  {32, 1,17}   // 1 REQUEST Objects (32 Byte), 17 RESPOND Objects
};

const COM_SPEC_TAB specObjCom21[] = 
{
  { specObjCom21_Req, specObjCom21_Resp, specObjCom21_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- write-command 22 -------------------------------------------------------
| WriteLongTag
| Write Long Tag
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,----------.
| RESPONSE |
`----------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


*/
const COM_OBJ_DESCR objCom22[] = 
{
  {HART_IDX, HART_IDX_longTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};


/*
,--- read-command 33 -------------------------------------------------------
| ReadDeviceVariables
| Read Device Variables
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

             byte(s)     type        object (default)

 +-------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------>       #1  TD_USIGN8  Hart.slot1 (SLOT_CODE_NONE)
 | | +---->       #2  TD_USIGN8  Hart.slot2 (SLOT_CODE_NONE)
 | | | +-->       #3  TD_USIGN8  Hart.slot3 (SLOT_CODE_NONE)
 | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
 0     3


,----------.
| RESPONSE |
`----------´

                                         byte(s)     type        object (default)

 +------------------------------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +---------------------------------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +------------------------------>   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   |  +--------------------------->       #6  TD_USIGN8  Hart.slot1 (SLOT_CODE_NONE)
 | |   |  | +------------------------->       #7  TD_USIGN8  Hart.dummyU8 (224)
 | |   |  | |   +--------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   |  | |   |  +------------------>      #12  TD_USIGN8  Hart.slot2 (SLOT_CODE_NONE)
 | |   |  | |   |  | +---------------->      #13  TD_USIGN8  Hart.dummyU8 (224)
 | |   |  | |   |  | |   +------------> #14..#17   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   |  | |   |  | |   |  +--------->      #18  TD_USIGN8  Hart.slot3 (SLOT_CODE_NONE)
 | |   |  | |   |  | |   |  | +------->      #19  TD_USIGN8  Hart.dummyU8 (224)
 | |   |  | |   |  | |   |  | |   +---> #20..#23   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   |  | |   |  | |   |  | |   | 
+-+-+----+-+-+----+-+-+----+-+-+----+
| | |    | | |    | | |    | | |    |
+-+-+----+-+-+----+-+-+----+-+-+----+
 0                                23


,------------.
| SLOTCODE 0 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  SVMeasurement.QvDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  SVMeasurement.QvDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 1 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 2 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QmDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QmDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 3 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QpDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QpDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 4 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QvPartialDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QvPartialDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 5 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnPartialDampedObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnPartialDampedObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 6 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.frequency_Damped_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.frequency_Damped_Obj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 7 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperature_Damped_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperature_Damped_Obj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 8 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 9 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 10 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 11 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Mapper.PVPercent_Unit[0] (PERCENT_UNIT_CODE)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContent (1.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 12 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispQv.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totQvFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 13 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispQn.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totQnFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 14 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispQm.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totQmFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 15 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispEnergy.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totEnergyFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 16 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispQvPartial.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totQvPartialFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 17 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Totalizer.totDispQnPartial.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
 |   +--->   #1..#4   TD_FLOAT  Totalizer.totQnPartialFloatUserUnit (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 244 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Mapper.PVPercent_Unit[0] (PERCENT_UNIT_CODE)
 |   +--->   #1..#4   TD_FLOAT  Mapper.PVPercent_Range (100.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 245 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Hart.currentUnit[0] (CURRENT_MILLIAMPERES)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 246 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 |   +--->   #1..#4   TD_FLOAT  Mapper.PV (6.1f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 247 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.SV_Unit[0] (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  Mapper.SV (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 248 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.TV_Unit[0] (KILOGRAM_PER_HOUR)
 |   +--->   #1..#4   TD_FLOAT  Mapper.TV (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 249 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.QV_Unit[0] (NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM)
 |   +--->   #1..#4   TD_FLOAT  Mapper.QV (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR specObjCom33_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_slot1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_slot2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_slot3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom33_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_slot3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom33_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,244,245,246,247,248,249
};

const COM_OBJ_DESCR specObjCom33_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequency_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_Damped_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_PVPercent_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // CONST_U8[0]
  {MAPPER_IDX, MAPPER_IDX_dvContent, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQv, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQn, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 14
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQm, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 15
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispEnergy, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 16
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQvPartial, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 17
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQnPartial, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatUserUnit, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 244
  {MAPPER_IDX, MAPPER_IDX_PVPercent_Unit, ATTRIB, 0, TD_USIGN8, HART_LEN_1}, // CONST_U8[0]
  {MAPPER_IDX, MAPPER_IDX_PVPercent_Range, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 245
  {HART_IDX, HART_IDX_currentUnit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // SIMPLE_U8[0]
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 246
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 247
  {MAPPER_IDX, MAPPER_IDX_SV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_SV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 248
  {MAPPER_IDX, MAPPER_IDX_TV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_TV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 249
  {MAPPER_IDX, MAPPER_IDX_QV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_QV, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};

const COM_REQ_RESP_CNT specObjCom33_ReqRespObjCnt[] = 
{
  { 1, 1, 3},   // slot 0: 1 REQUEST Objects (1 Byte) - 3 RESPOND Objects
  { 2, 2, 6},   // slot 1: 2 REQUEST Objects (2 Byte) - 6 RESPOND Objects
  { 3, 3, 9},   // slot 2: 3 REQUEST Objects (3 Byte) - 9 RESPOND Objects
  { 4, 4,12}    // slot 3: 4 REQUEST Objects (4 Byte) - 12 RESPOND Objects
};

const COM_SPEC_TAB specObjCom33[] = 
{
  { specObjCom33_Req, specObjCom33_Resp, specObjCom33_ReqRespObjCnt, 4, specObjCom33_slotCodeObjDescrTab, 23, 2, specObjCom33_slotCodeTranslationTable}
};


/*
,--- write-command 34 -------------------------------------------------------
| WritePrimaryVariableDampingValue
| Write Primary Variable Damping Value
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.PV_DampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.PV_DampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom34[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- write-command 35 -------------------------------------------------------
| WritePrimaryVariableRangeValues
| Write Primary Variable Range Values
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                 byte(s)     type        object (default)

 +------------>       #0    TD_UNIT  Mapper.PV_RangeUnit[0] (CUBIC_METER_PER_HOUR)
 |   +-------->   #1..#4   TD_FLOAT  Mapper.PV_UpperRange (110.0f)
 |   |    +--->   #5..#8   TD_FLOAT  Mapper.PV_LowerRange (6.1f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,----------.
| RESPONSE |
`----------´

                 byte(s)     type        object (default)

 +------------>       #0    TD_UNIT  Mapper.PV_RangeUnit[0] (CUBIC_METER_PER_HOUR)
 |   +-------->   #1..#4   TD_FLOAT  Mapper.PV_UpperRange (110.0f)
 |   |    +--->   #5..#8   TD_FLOAT  Mapper.PV_LowerRange (6.1f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


*/
const COM_OBJ_DESCR objCom35[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_RangeUnit, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // USIGN8[0]
  {MAPPER_IDX, MAPPER_IDX_PV_UpperRange, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_PV_LowerRange, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- write-command 36 -------------------------------------------------------
| SetPrimaryVariableUpperRangeValue
| Set Primary Variable Upper Range Value
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.setPVUpRangeValue ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.setPVUpRangeValue ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom36[] = 
{
  {HART_IDX, HART_IDX_setPVUpRangeValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 37 -------------------------------------------------------
| SetPrimaryVariableUpperLowerValue
| Set Primary Variable Upper Lower Value
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.setPVLowerRangeValue ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.setPVLowerRangeValue ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom37[] = 
{
  {HART_IDX, HART_IDX_setPVLowerRangeValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 38 -------------------------------------------------------
| ResetConfigurationChangedFlag
| Reset Configuration Changed Flag
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  Hart.confChangeCounterValue (0)
  |
+--+
|  |
+--+
 01


,----------.
| RESPONSE |
`----------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  Hart.confChangeCounterValue (0)
  |
+--+
|  |
+--+
 01


*/
const COM_OBJ_DESCR specObjCom38_Resp[] = 
{
  {HART_IDX, HART_IDX_confChangeCounterValue, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2} // SIMPLE_U16
};

const COM_OBJ_DESCR specObjCom38_Req[] = 
{
  {HART_IDX, HART_IDX_confChangeCounterValue, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2} // SIMPLE_U16
};

const COM_REQ_RESP_CNT specObjCom38_ReqRespObjCnt[] = 
{
  { 2, 1, 1}   // 1 REQUEST Objects (2 Byte), 1 RESPOND Objects
};

const COM_SPEC_TAB specObjCom38[] = 
{
  { specObjCom38_Req, specObjCom38_Resp, specObjCom38_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- write-command 40 -------------------------------------------------------
| EnterExitFixedCurrentMode
| Enter or Exit Fixed Current Mode
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.fixedOutputValue (0.0f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.fixedOutputValue (0.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom40[] = 
{
  {HART_IDX, HART_IDX_fixedOutputValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- write-command 41 -------------------------------------------------------
| PerformSelfTest
| Perform Self Test
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.StartSelfTest ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.StartSelfTest ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom41[] = 
{
  {HART_IDX, HART_IDX_StartSelfTest, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 42 -------------------------------------------------------
| PerformDeviceReset
| Perform Device Reset
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.resetDevice ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.resetDevice ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom42[] = 
{
  {HART_IDX, HART_IDX_resetDevice, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 43 -------------------------------------------------------
| WritePrimaryVariableZero
| Write Primary Variable Zero
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.writeAutoZero ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.writeAutoZero ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom43[] = 
{
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_writeAutoZero, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 44 -------------------------------------------------------
| WritePrimaryVariableUnits
| 
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.PV_Unit[0] (CUBIC_METER_PER_HOUR)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom44[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_Unit, ATTRIB, 0, TD_UNIT, HART_LEN_1} // USIGN8[0]
};


/*
,--- write-command 45 -------------------------------------------------------
| TrimloopCurrentZero
| Trimloop Current Zero
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom45[] = 
{
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- write-command 46 -------------------------------------------------------
| TrimloopCurrentGain
| Trimloop Current Gain
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom46[] = 
{
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- read-command 48 -------------------------------------------------------
| ReadAdditionalDeviceStatus
| Read Additional Device Status
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                           byte(s)     type        object (default)

    +----------------------------------->   #0..#5  TD_USIGN8  Mapper.diagDeviceStatus[0..5] (0,0,0,0,0,0)
    |   +------------------------------->       #6  TD_USIGN8  Hart.dummy_0 (0)
    |   | +----------------------------->       #7  TD_USIGN8  Hart.dummy_0 (0)
    |   | | +--------------------------->       #8  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | +------------------------->       #9  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | +----------------------->      #10  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | +--------------------->      #11  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | +------------------->      #12  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | +----------------->      #13  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | | +--------------->      #14  TD_USIGN8  Mapper.diagnosisHighCondition (0)
    |   | | | | | | | | | +------------->      #15  TD_USIGN8  Mapper.diagnosisHighClass (0)
    |   | | | | | | | | | | +----------->      #16  TD_USIGN8  Mapper.diagnosisHighGroup (0)
    |   | | | | | | | | | | |    +------> #17..#22  TD_USIGN8  Mapper.diagHistory[0..5] (0,0,0,0,0,0)
    |   | | | | | | | | | | |    |   +-->      #23  TD_USIGN8  Mapper.diagSimulationStatus (DISABLE)
    |   | | | | | | | | | | |    |   |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
|      | | | | | | | | | | | |      | |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
 0                                  23


,----------.
| RESPONSE |
`----------´

                                           byte(s)     type        object (default)

    +----------------------------------->   #0..#5  TD_USIGN8  Mapper.diagDeviceStatus[0..5] (0,0,0,0,0,0)
    |   +------------------------------->       #6  TD_USIGN8  Hart.dummy_0 (0)
    |   | +----------------------------->       #7  TD_USIGN8  Hart.dummy_0 (0)
    |   | | +--------------------------->       #8  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | +------------------------->       #9  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | +----------------------->      #10  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | +--------------------->      #11  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | +------------------->      #12  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | +----------------->      #13  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | | +--------------->      #14  TD_USIGN8  Mapper.diagnosisHighCondition (0)
    |   | | | | | | | | | +------------->      #15  TD_USIGN8  Mapper.diagnosisHighClass (0)
    |   | | | | | | | | | | +----------->      #16  TD_USIGN8  Mapper.diagnosisHighGroup (0)
    |   | | | | | | | | | | |    +------> #17..#22  TD_USIGN8  Mapper.diagHistory[0..5] (0,0,0,0,0,0)
    |   | | | | | | | | | | |    |   +-->      #23  TD_USIGN8  Mapper.diagSimulationStatus (DISABLE)
    |   | | | | | | | | | | |    |   |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
|      | | | | | | | | | | | |      | |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
 0                                  23


,------------.
| SLOTCODE 5 |
`------------´

                                           byte(s)     type        object (default)

    +----------------------------------->   #0..#5  TD_USIGN8  Mapper.diagDeviceStatus[0..5] (0,0,0,0,0,0)
    |   +------------------------------->       #6  TD_USIGN8  Hart.dummy_0 (0)
    |   | +----------------------------->       #7  TD_USIGN8  Hart.dummy_0 (0)
    |   | | +--------------------------->       #8  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | +------------------------->       #9  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | +----------------------->      #10  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | +--------------------->      #11  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | +------------------->      #12  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | +----------------->      #13  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | | +--------------->      #14  TD_USIGN8  Mapper.diagnosisHighCondition (0)
    |   | | | | | | | | | +------------->      #15  TD_USIGN8  Mapper.diagnosisHighClass (0)
    |   | | | | | | | | | | +----------->      #16  TD_USIGN8  Mapper.diagnosisHighGroup (0)
    |   | | | | | | | | | | |    +------> #17..#22  TD_USIGN8  Mapper.diagHistory[0..5] (0,0,0,0,0,0)
    |   | | | | | | | | | | |    |   +-->      #23  TD_USIGN8  Mapper.diagSimulationStatus (DISABLE)
    |   | | | | | | | | | | |    |   |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
|      | | | | | | | | | | | |      | |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
 0                                  23


,------------.
| SLOTCODE 7 |
`------------´

                                           byte(s)     type        object (default)

    +----------------------------------->   #0..#5  TD_USIGN8  Mapper.diagDeviceStatus[0..5] (0,0,0,0,0,0)
    |   +------------------------------->       #6  TD_USIGN8  Hart.extendedDeviceStatus (DEFAULT_EXTENDED_DEVICE_STATUS)
    |   | +----------------------------->       #7  TD_USIGN8  Coordinator.operatingMode (LIQUID_ACTUAL_VOLUME)
    |   | | +--------------------------->       #8  TD_USIGN8  Hart.standardizedStatus0 (DEFAULT_STANDARDIZED_STATUS_0)
    |   | | | +------------------------->       #9  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | +----------------------->      #10  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | +--------------------->      #11  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | +------------------->      #12  TD_USIGN8  Hart.dummy_0 (0)
    |   | | | | | | | +----------------->      #13  TD_USIGN8  Hart.analogChannelFixed (DEFAULT_ANALOG_CHANNEL_FIXED)
    |   | | | | | | | | +--------------->      #14  TD_USIGN8  Mapper.diagnosisHighCondition (0)
    |   | | | | | | | | | +------------->      #15  TD_USIGN8  Mapper.diagnosisHighClass (0)
    |   | | | | | | | | | | +----------->      #16  TD_USIGN8  Mapper.diagnosisHighGroup (0)
    |   | | | | | | | | | | |    +------> #17..#22  TD_USIGN8  Mapper.diagHistory[0..5] (0,0,0,0,0,0)
    |   | | | | | | | | | | |    |   +-->      #23  TD_USIGN8  Mapper.diagSimulationStatus (DISABLE)
    |   | | | | | | | | | | |    |   |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
|      | | | | | | | | | | | |      | |
+------+-+-+-+-+-+-+-+-+-+-+-+------+-+
 0                                  23


*/
const COM_OBJ_DESCR specObjCom48_Resp[] = 
{
  {MAPPER_IDX, MAPPER_IDX_diagDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighCondition, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighClass, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighGroup, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagHistory, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {MAPPER_IDX, MAPPER_IDX_diagSimulationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_OBJ_DESCR specObjCom48_Req[] = 
{
  {MAPPER_IDX, MAPPER_IDX_diagDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighCondition, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighClass, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighGroup, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagHistory, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {MAPPER_IDX, MAPPER_IDX_diagSimulationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const TUSIGN8 specObjCom48_slotCodeTranslationTable[] = {
5,7
};

const COM_OBJ_DESCR specObjCom48_slotCodeObjDescrTab[] = 
{
  //slotcode 5
  {MAPPER_IDX, MAPPER_IDX_diagDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighCondition, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighClass, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighGroup, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagHistory, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {MAPPER_IDX, MAPPER_IDX_diagSimulationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 7
  {MAPPER_IDX, MAPPER_IDX_diagDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {HART_IDX, HART_IDX_extendedDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_operatingMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_standardizedStatus0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy_0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_analogChannelFixed, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighCondition, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighClass, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisHighGroup, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagHistory, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  {MAPPER_IDX, MAPPER_IDX_diagSimulationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_REQ_RESP_CNT specObjCom48_ReqRespObjCnt[] = 
{
  { 6, 1,14}    // slot 0: 1 REQUEST Objects (6 Byte) - 14 RESPOND Objects
};

const COM_SPEC_TAB specObjCom48[] = 
{
  { specObjCom48_Req, specObjCom48_Resp, specObjCom48_ReqRespObjCnt, 1, specObjCom48_slotCodeObjDescrTab, 1, 14, specObjCom48_slotCodeTranslationTable}
};


/*
,--- read-command 50 -------------------------------------------------------
| ReadDynamicVariablesAssignment
| Read Dynamic Variables Assignment
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

             byte(s)     type        object (default)

 +-------->       #0  TD_USIGN8  Mapper.PV_Assignment (DEVICE_VAR_QV)
 | +------>       #1  TD_USIGN8  Mapper.SV_Assignment (DEVICE_VAR_T)
 | | +---->       #2  TD_USIGN8  Mapper.TV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | +-->       #3  TD_USIGN8  Mapper.QV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
 0     3


*/
const COM_OBJ_DESCR objCom50[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_SV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_TV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_QV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- write-command 51 -------------------------------------------------------
| WriteDynamicVariablesAssignment
| Write Dynamic Variables Assignment
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

             byte(s)     type        object (default)

 +-------->       #0  TD_USIGN8  Mapper.PV_Assignment (DEVICE_VAR_QV)
 | +------>       #1  TD_USIGN8  Mapper.SV_Assignment (DEVICE_VAR_T)
 | | +---->       #2  TD_USIGN8  Mapper.TV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | +-->       #3  TD_USIGN8  Mapper.QV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
 0     3


,----------.
| RESPONSE |
`----------´

             byte(s)     type        object (default)

 +-------->       #0  TD_USIGN8  Mapper.PV_Assignment (DEVICE_VAR_QV)
 | +------>       #1  TD_USIGN8  Mapper.SV_Assignment (DEVICE_VAR_T)
 | | +---->       #2  TD_USIGN8  Mapper.TV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | +-->       #3  TD_USIGN8  Mapper.QV_Assignment (UNSUPPORTED_DEVICE_VAR)
 | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
 0     3


*/
const COM_OBJ_DESCR objCom51[] = 
{
  {MAPPER_IDX, MAPPER_IDX_PV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_SV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_TV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_QV_Assignment, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- write-command 53 -------------------------------------------------------
| WriteDeviceVariableUnits
| Write Device Variable Units
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  SVMeasurement.QvDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QmDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QpDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QvPartialDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnPartialDampedObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.frequency_Damped_Obj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.temperature_Damped_Obj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvTExtObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvPressureObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvDensityObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummy250 (250)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQmUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKilograms)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totEnergyUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKiloWattHour)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom53_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom53_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom53_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom53_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequency_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 11
  {HART_IDX, HART_IDX_dummy250, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 14
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 15
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 16
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 17
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1} // UNITS.unitsCode
};

const COM_REQ_RESP_CNT specObjCom53_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom53[] = 
{
  { specObjCom53_Req, specObjCom53_Resp, specObjCom53_ReqRespObjCnt, 1, specObjCom53_slotCodeObjDescrTab, 17, 1, specObjCom53_slotCodeTranslationTable}
};


/*
,--- read-command 54 -------------------------------------------------------
| ReadDeviceVariableInformation
| Read Device Variable Information
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                                          byte(s)     type        object (default)

 +------------------------------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |  +---------------------------------->   #1..#3  TD_USIGN8  Hart.devVariableTransducerSerialNo[0..2] (0,0,0)
 |  |  +------------------------------->       #4  TD_USIGN8  Hart.dummyU8 (224)
 |  |  |   +--------------------------->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |  |  |   |    +---------------------->  #9..#12   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |  |  |   |    |    +-----------------> #13..#16   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |  |  |   |    |    |    +------------> #17..#20   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |  |  |   |    |    |    |  +--------->      #21  TD_USIGN8  Hart.devVariableClassification (0x00)
 |  |  |   |    |    |    |  | +------->      #22  TD_USIGN8  Hart.devVariableFamilyCode (250)
 |  |  |   |    |    |    |  | |   +---> #23..#26 TD_USIGN32  Hart.dummy32 (123456789)
 |  |  |   |    |    |    |  | |   | 
+-+---+-+----+----+----+----+-+-+----+
| |   | |    |    |    |    | | |    |
+-+---+-+----+----+----+----+-+-+----+
 0                                 26


,------------.
| SLOTCODE 0 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  SVMeasurement.QvRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  SVMeasurement.QvRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  SVMeasurement.QvRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  SVMeasurement.QvDampingTime (1.0)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  SVMeasurement.QvRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  SVMeasurement.QvClassification (UT_VOLUME_FLOW)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 1 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  MVMeasurement.QnRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  MVMeasurement.QnRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  MVMeasurement.QnRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  MVMeasurement.QnDampingTime (1.00f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  MVMeasurement.QnRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  MVMeasurement.QnClassification (T_VOLUME_FLOW)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 2 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  MVMeasurement.QmRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  MVMeasurement.QmRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  MVMeasurement.QmRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  MVMeasurement.QmDampingTime (1.00f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  MVMeasurement.QmRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  MVMeasurement.QmClassification (T_MASS_FLOW)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 3 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  MVMeasurement.QpRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  MVMeasurement.QpRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  MVMeasurement.QpRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  MVMeasurement.QpDampingTime (1.00f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  MVMeasurement.QpRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  MVMeasurement.QpClassification (T_POWER)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 4 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  MVMeasurement.QvPartialRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  MVMeasurement.QvPartialDampingTime (1.00f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  MVMeasurement.QvPartialClassification (T_VOLUME_FLOW)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 5 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  MVMeasurement.QnPartialRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  MVMeasurement.QnPartialDampingTime (1.00f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  MVMeasurement.QnPartialClassification (T_VOLUME_FLOW)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 6 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Coordinator.frequency_range_Obj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Coordinator.frequency_range_Obj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Coordinator.frequency_range_Obj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Coordinator.frequency_DampingTime (0.2)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Coordinator.frequency_range_Obj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Coordinator.frequency_Classification (T_FREQUENCY)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 7 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Coordinator.temperature_range_Obj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Coordinator.temperature_range_Obj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Coordinator.temperature_range_Obj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Coordinator.temperature_DampingTime (0.2)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Coordinator.temperature_range_Obj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Coordinator.temperature_Classification (T_TEMPERATURE)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 8 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Mapper.dvTExtRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Mapper.dvTExtRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Mapper.dvTExtRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Mapper.dvTExtRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Mapper.dvTExtClassification (T_TEMPERATURE)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeSlow (250)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 9 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Mapper.dvPressureRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Mapper.dvPressureRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Mapper.dvPressureRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Mapper.dvPressureRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Mapper.dvPressureClassification (T_PRESSURE)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeSlow (250)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 10 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Mapper.dvDensityRangeObj.unit ()
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Mapper.dvDensityRangeObj.value_0 ()
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Mapper.dvDensityRangeObj.value_1 ()
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Mapper.dvDensityRangeObj.value_2 ()
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Mapper.dvDensityClassification (T_MASS_DENSITY)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeSlow (250)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 11 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3  TD_USIGN8  Mapper.dvDummyUnitCode (0)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Mapper.dvContentRange.hiLim (100.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Mapper.dvContentRange.loLim (0.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Mapper.dvContentRange.span (100.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Mapper.dvContentClassification (T_ANALYTICAL)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeSlow (250)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 12 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispQv.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totQvClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 13 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispQn.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totQnClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 14 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispQm.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totQmClassification (TOTALIZER_TotalizerUnitsTypeIDXMass)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 15 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispEnergy.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totEnergyClassification (TOTALIZER_TotalizerUnitsTypeIDXEnergy)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 16 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispQvPartial.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totQvPartialClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


,------------.
| SLOTCODE 17 |
`------------´

                                        byte(s)     type        object (default)

  +---------------------------------->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  |  +------------------------------->       #3    TD_UNIT  Totalizer.totDispQnPartial.units ((TUSIGN8)TotalizerUnitIDXCustomUnits)
  |  |   +--------------------------->   #4..#7   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    +---------------------->  #8..#11   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    +-----------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    +------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
  |  |   |    |    |    |  +--------->      #20 TD_UNITTYPE  Totalizer.totQnPartialClassification (TOTALIZER_TotalizerUnitsTypeIDXVolume)
  |  |   |    |    |    |  | +------->      #21  TD_USIGN8  Hart.devVariableFamilyCode (250)
  |  |   |    |    |    |  | |   +---> #22..#25 TD_USIGN32  Coordinator.updateTimeFast (100)
  |  |   |    |    |    |  | |   | 
+---+-+----+----+----+----+-+-+----+
|   | |    |    |    |    | | |    |
+---+-+----+----+----+----+-+-+----+
 0                               25


*/
const COM_OBJ_DESCR specObjCom54_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableTransducerSerialNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // CONST_U8[0..2]
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_devVariableClassification, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_OBJ_DESCR specObjCom54_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom54_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom54_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {SVM_IDX, SVM_IDX_QvDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {SVM_IDX, SVM_IDX_QvClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 1
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MVM_IDX, MVM_IDX_QnDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MVM_IDX, MVM_IDX_QnClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 2
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MVM_IDX, MVM_IDX_QmDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MVM_IDX, MVM_IDX_QmClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 3
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MVM_IDX, MVM_IDX_QpDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MVM_IDX, MVM_IDX_QpClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 4
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MVM_IDX, MVM_IDX_QvPartialDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MVM_IDX, MVM_IDX_QvPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 5
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MVM_IDX, MVM_IDX_QnPartialDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MVM_IDX, MVM_IDX_QnPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 6
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {CDT_IDX, CDT_IDX_frequency_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_range_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_frequency_range_Obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {CDT_IDX, CDT_IDX_frequency_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {CDT_IDX, CDT_IDX_frequency_range_Obj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {CDT_IDX, CDT_IDX_frequency_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 7
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {CDT_IDX, CDT_IDX_temperature_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {CDT_IDX, CDT_IDX_temperature_Classification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // CONST_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 8
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MAPPER_IDX, MAPPER_IDX_dvTExtClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeSlow, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 9
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MAPPER_IDX, MAPPER_IDX_dvPressureClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeSlow, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 10
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  {MAPPER_IDX, MAPPER_IDX_dvDensityClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeSlow, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 11
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {MAPPER_IDX, MAPPER_IDX_dvDummyUnitCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_hiLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.hiLim
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_loLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.loLim
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_span, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.span
  {MAPPER_IDX, MAPPER_IDX_dvContentClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeSlow, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 12
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQv, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 13
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQn, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 14
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQm, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 15
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispEnergy, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 16
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQvPartial, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 17
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQnPartial, ATTRIB, TOTDISP_units, TD_UNIT, HART_LEN_1}, // TOTDISP.units
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialClassification, OBJECT, HART_NO, TD_UNITTYPE, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_devVariableFamilyCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // CONST_U8
  {CDT_IDX, CDT_IDX_updateTimeFast, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_REQ_RESP_CNT specObjCom54_ReqRespObjCnt[] = 
{
  { 1, 1,10}    // slot 0: 1 REQUEST Objects (1 Byte) - 10 RESPOND Objects
};

const COM_SPEC_TAB specObjCom54[] = 
{
  { specObjCom54_Req, specObjCom54_Resp, specObjCom54_ReqRespObjCnt, 1, specObjCom54_slotCodeObjDescrTab, 17, 9, specObjCom54_slotCodeTranslationTable}
};


/*
,--- write-command 55 -------------------------------------------------------
| WriteDeviceVariableDampingValue
| Write Device Variable Damping Value
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 0 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvDampingTime (1.0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 1 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnDampingTime (1.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 2 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmDampingTime (1.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 3 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpDampingTime (1.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 4 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialDampingTime (1.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 5 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialDampingTime (1.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 6 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.frequency_DampingTime (0.2)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 7 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_DampingTime (0.2)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 8 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 9 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 10 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 11 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDummyDampingTime (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 12 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 13 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 14 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 15 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 16 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 17 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Hart.dumNAN (0X7FA00000)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR specObjCom55_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom55_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom55_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom55_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequency_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperature_DampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_dvDummyDampingTime, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 12
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 13
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 14
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 15
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 16
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 17
  {HART_IDX, HART_IDX_dumNAN, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_REQ_RESP_CNT specObjCom55_ReqRespObjCnt[] = 
{
  { 5, 2, 2}    // slot 0: 2 REQ+RES Objects (5 Byte)
};

const COM_SPEC_TAB specObjCom55[] = 
{
  { specObjCom55_Req, specObjCom55_Resp, specObjCom55_ReqRespObjCnt, 1, specObjCom55_slotCodeObjDescrTab, 17, 1, specObjCom55_slotCodeTranslationTable}
};


/*
,--- write-command 59 -------------------------------------------------------
| WriteNumberofResponsePreambles
| Write Number of Response Preambles
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.numberRespPream (5)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.numberRespPream (5)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom59[] = 
{
  {HART_IDX, HART_IDX_numberRespPream, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // USIGN8
};


/*
,--- write-command 71 -------------------------------------------------------
| LockDevice
| Lock Device
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.lockDeviceSelection (LOCK_DEVICE_UNLOCKED)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.lockDeviceSelection (LOCK_DEVICE_UNLOCKED)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom71[] = 
{
  {HART_IDX, HART_IDX_lockDeviceSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- read-command 76 -------------------------------------------------------
| ReadLockDeviceState
| Read Lock Device State
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.lockDeviceStatus (LOCK_STATUS_NOT_LOCKED)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom76[] = 
{
  {HART_IDX, HART_IDX_lockDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};


/*
,--- write-command 79 -------------------------------------------------------
| WriteDeviceVariable
| Write Device Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                  byte(s)     type        object (default)

 +------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +----------->       #1  TD_USIGN8  Hart.writeDeviceVariableCommCode (0)
 | | +--------->       #2  TD_USIGN8  Hart.dummyU8 (224)
 | | |   +----->   #3..#6   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | | |   |  +-->       #7  TD_USIGN8  Hart.devVariableStatus (0x30)
 | | |   |  |
+-+-+-+----+-+
| | | |    | |
+-+-+-+----+-+
 0          7


,----------.
| RESPONSE |
`----------´

                  byte(s)     type        object (default)

 +------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +----------->       #1  TD_USIGN8  Hart.writeDeviceVariableCommCode (0)
 | | +--------->       #2  TD_USIGN8  Hart.dummyU8 (224)
 | | |   +----->   #3..#6   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | | |   |  +-->       #7  TD_USIGN8  Hart.devVariableStatus (0x30)
 | | |   |  |
+-+-+-+----+-+
| | | |    | |
+-+-+-+----+-+
 0          7


,------------.
| SLOTCODE 0 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  SVMeasurement.QvSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  SVMeasurement.QvSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  SVMeasurement.QvSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  SVMeasurement.QvStatus (0x00)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 1 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  MVMeasurement.QnSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  MVMeasurement.QnSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QnSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QnStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 2 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  MVMeasurement.QmSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  MVMeasurement.QmSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QmSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QmStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 3 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  MVMeasurement.QpSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  MVMeasurement.QpSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QpSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QpStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 4 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  MVMeasurement.QvPartialSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  MVMeasurement.QvPartialSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QvPartialSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QvPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 5 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  MVMeasurement.QnPartialSimEnable (DISABLE)
 | +--------->       #1    TD_UNIT  MVMeasurement.QnPartialSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  MVMeasurement.QnPartialSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  MVMeasurement.QnPartialStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 6 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Coordinator.frequencySim_Enable (SIMULATION_DISABLE)
 | +--------->       #1    TD_UNIT  Coordinator.frequency_SimObj.unit (HERTZ)
 | |   +----->   #2..#5   TD_FLOAT  Coordinator.frequency_SimObj.value_0 (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Coordinator.frequency_Status (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 7 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Coordinator.temperatureSim_Enable (SIMULATION_DISABLE)
 | +--------->       #1    TD_UNIT  Coordinator.temperatureSim_Obj.unit (CELSIUS)
 | |   +----->   #2..#5   TD_FLOAT  Coordinator.temperatureSim_Obj.value_0 (0.0f)
 | |   |  +-->       #6  TD_USIGN8  Coordinator.temperature_Status (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 8 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Mapper.dvTExtSimEnable (SIMU_DISABLE)
 | +--------->       #1    TD_UNIT  Mapper.dvTExtSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvTExtSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvTExtStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 9 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Mapper.dvPressureSimEnable (SIMU_DISABLE)
 | +--------->       #1    TD_UNIT  Mapper.dvPressureSimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvPressureSimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvPressureStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 10 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Mapper.dvDensitySimEnable (SIMU_DISABLE)
 | +--------->       #1    TD_UNIT  Mapper.dvDensitySimObj.unit ()
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvDensitySimObj.value_0 ()
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvDensityStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


,------------.
| SLOTCODE 11 |
`------------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Mapper.dvContentSimEnable (SIMU_DISABLE)
 | +--------->       #1    TD_UNIT  Mapper.dvContentUnitCode[0] (PERCENT)
 | |   +----->   #2..#5   TD_FLOAT  Mapper.dvContentSim (1.0f)
 | |   |  +-->       #6  TD_USIGN8  Mapper.dvContentStatus (0xC0)
 | |   |  |
+-+-+----+-+
| | |    | |
+-+-+----+-+
 0        6


*/
const COM_OBJ_DESCR specObjCom79_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_writeDeviceVariableCommCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_devVariableStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_BIT8
};

const COM_OBJ_DESCR specObjCom79_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_writeDeviceVariableCommCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_devVariableStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_BIT8
};

const TUSIGN8 specObjCom79_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11
};

const COM_OBJ_DESCR specObjCom79_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {SVM_IDX, SVM_IDX_QvStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QmStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QpStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QvPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_QnPartialStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequencySim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_frequency_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperatureSim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {CDT_IDX, CDT_IDX_temperature_Status, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvTExtStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvPressureStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MAPPER_IDX, MAPPER_IDX_dvDensityStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_dvContentSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvContentUnitCode, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // SIMPLE_U8[0]
  {MAPPER_IDX, MAPPER_IDX_dvContentSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {MAPPER_IDX, MAPPER_IDX_dvContentStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom79_ReqRespObjCnt[] = 
{
  { 8, 5, 5}    // slot 0: 5 REQ+RES Objects (8 Byte)
};

const COM_SPEC_TAB specObjCom79[] = 
{
  { specObjCom79_Req, specObjCom79_Resp, specObjCom79_ReqRespObjCnt, 1, specObjCom79_slotCodeObjDescrTab, 11, 4, specObjCom79_slotCodeTranslationTable}
};


/*
,--- write-command 122 -------------------------------------------------------
| WriteHartServiceCode
| WriteHartServiceCode
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,----------.
| RESPONSE |
`----------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


*/
const COM_OBJ_DESCR objCom122[] = 
{
  {HART_IDX, HART_IDX_ServiceCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // HART_SERVICECODE[0..19]
};


/*
,--- read-command 123 -------------------------------------------------------
| ReadObject
| Read Object
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                  byte(s)     type        object (default)

 +------------->       #0  TD_USIGN8  Hart.board (0)
 | +----------->       #1  TD_USIGN8  Hart.address (0)
 | |  +-------->   #2..#3 TD_USIGN16  Hart.subsystemIndex (0)
 | |  |  +----->   #4..#5 TD_USIGN16  Hart.objectIndex (0)
 | |  |  |  +-->   #6..#7 TD_USIGN16  Hart.attributeIndex (0)
 | |  |  |  |
+-+-+--+--+--+
| | |  |  |  |
+-+-+--+--+--+
 0          7


,----------.
| RESPONSE |
`----------´

                                                     byte(s)     type        object (default)

 +------------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------------->   #2..#3 TD_USIGN16  Hart.subsystemIndex (0)
 | |  |  +---------------------------------------->   #4..#5 TD_USIGN16  Hart.objectIndex (0)
 | |  |  |  +------------------------------------->   #6..#7 TD_USIGN16  Hart.attributeIndex (0)
 | |  |  |  | +----------------------------------->       #8  TD_USIGN8  Hart.objectDataLength (0)
 | |  |  |  | |                 +----------------->  #9..#40  TD_USIGN8  Hart.objectData[0..31] (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
 | |  |  |  | |                 |               
+-+-+--+--+--+-+--------------------------------+
| | |  |  |  | |                                |
+-+-+--+--+--+-+--------------------------------+
 0                                            40


*/
const COM_OBJ_DESCR specObjCom123_Resp[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_subsystemIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_objectIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_attributeIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_objectDataLength, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_objectData, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};

const COM_OBJ_DESCR specObjCom123_Req[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_subsystemIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_objectIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_attributeIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2} // SIMPLE_U16
};

const COM_REQ_RESP_CNT specObjCom123_ReqRespObjCnt[] = 
{
  { 1, 1, 7},   // 1 REQUEST Objects (1 Byte), 7 RESPOND Objects
  { 2, 2, 7},   // 2 REQUEST Objects (2 Byte), 7 RESPOND Objects
  { 4, 3, 7},   // 3 REQUEST Objects (4 Byte), 7 RESPOND Objects
  { 6, 4, 7},   // 4 REQUEST Objects (6 Byte), 7 RESPOND Objects
  { 8, 5, 7}   // 5 REQUEST Objects (8 Byte), 7 RESPOND Objects
};

const COM_SPEC_TAB specObjCom123[] = 
{
  { specObjCom123_Req, specObjCom123_Resp, specObjCom123_ReqRespObjCnt, 5, NULL, 0, 0, NULL}
};


/*
,--- write-command 124 -------------------------------------------------------
| WriteObject
| Write Object
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                                     byte(s)     type        object (default)

 +------------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------------->   #2..#3 TD_USIGN16  Hart.subsystemIndex (0)
 | |  |  +---------------------------------------->   #4..#5 TD_USIGN16  Hart.objectIndex (0)
 | |  |  |  +------------------------------------->   #6..#7 TD_USIGN16  Hart.attributeIndex (0)
 | |  |  |  | +----------------------------------->       #8  TD_USIGN8  Hart.objectDataLength (0)
 | |  |  |  | |                 +----------------->  #9..#40  TD_USIGN8  Hart.objectData[0..31] (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
 | |  |  |  | |                 |               
+-+-+--+--+--+-+--------------------------------+
| | |  |  |  | |                                |
+-+-+--+--+--+-+--------------------------------+
 0                                            40


,----------.
| RESPONSE |
`----------´

                                                     byte(s)     type        object (default)

 +------------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------------->   #2..#3 TD_USIGN16  Hart.subsystemIndex (0)
 | |  |  +---------------------------------------->   #4..#5 TD_USIGN16  Hart.objectIndex (0)
 | |  |  |  +------------------------------------->   #6..#7 TD_USIGN16  Hart.attributeIndex (0)
 | |  |  |  | +----------------------------------->       #8  TD_USIGN8  Hart.objectDataLength (0)
 | |  |  |  | |                 +----------------->  #9..#40  TD_USIGN8  Hart.objectData[0..31] (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
 | |  |  |  | |                 |               
+-+-+--+--+--+-+--------------------------------+
| | |  |  |  | |                                |
+-+-+--+--+--+-+--------------------------------+
 0                                            40


*/
const COM_OBJ_DESCR objCom124[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_subsystemIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_objectIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_attributeIndex, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_objectDataLength, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_objectData, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};


/*
,--- read-command 125 -------------------------------------------------------
| ReadMemory
| Read Memory
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.board (0)
 | +------->       #1  TD_USIGN8  Hart.address (0)
 | |  +---->   #2..#3 TD_USIGN16  Hart.memoryAddress (0x0000)
 | |  | +-->       #4  TD_USIGN8  Hart.memoryLength (0)
 | |  | |
+-+-+--+-+
| | |  | |
+-+-+--+-+
 0      4


,----------.
| RESPONSE |
`----------´

                                               byte(s)     type        object (default)

 +------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------->   #2..#3 TD_USIGN16  Hart.memoryAddress (0x0000)
 | |  | +----------------------------------->       #4  TD_USIGN8  Hart.memoryLength (0)
 | |  | |                 +----------------->  #5..#36  TD_USIGN8  Hart.memoryData[0..31] (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00)
 | |  | |                 |               
+-+-+--+-+--------------------------------+
| | |  | |                                |
+-+-+--+-+--------------------------------+
 0                                      36


*/
const COM_OBJ_DESCR specObjCom125_Resp[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_memoryAddress, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_memoryLength, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_memoryData, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};

const COM_OBJ_DESCR specObjCom125_Req[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_memoryAddress, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_memoryLength, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom125_ReqRespObjCnt[] = 
{
  { 1, 1, 5},   // 1 REQUEST Objects (1 Byte), 5 RESPOND Objects
  { 2, 2, 5},   // 2 REQUEST Objects (2 Byte), 5 RESPOND Objects
  { 4, 3, 5},   // 3 REQUEST Objects (4 Byte), 5 RESPOND Objects
  { 5, 4, 5}   // 4 REQUEST Objects (5 Byte), 5 RESPOND Objects
};

const COM_SPEC_TAB specObjCom125[] = 
{
  { specObjCom125_Req, specObjCom125_Resp, specObjCom125_ReqRespObjCnt, 4, NULL, 0, 0, NULL}
};


/*
,--- write-command 126 -------------------------------------------------------
| WriteMemory
| Write Memory
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                               byte(s)     type        object (default)

 +------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------->   #2..#3 TD_USIGN16  Hart.memoryAddress (0x0000)
 | |  | +----------------------------------->       #4  TD_USIGN8  Hart.memoryLength (0)
 | |  | |                 +----------------->  #5..#36  TD_USIGN8  Hart.memoryData[0..31] (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00)
 | |  | |                 |               
+-+-+--+-+--------------------------------+
| | |  | |                                |
+-+-+--+-+--------------------------------+
 0                                      36


,----------.
| RESPONSE |
`----------´

                                               byte(s)     type        object (default)

 +------------------------------------------>       #0  TD_USIGN8  Hart.board (0)
 | +---------------------------------------->       #1  TD_USIGN8  Hart.address (0)
 | |  +------------------------------------->   #2..#3 TD_USIGN16  Hart.memoryAddress (0x0000)
 | |  | +----------------------------------->       #4  TD_USIGN8  Hart.memoryLength (0)
 | |  | |                 +----------------->  #5..#36  TD_USIGN8  Hart.memoryData[0..31] (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00)
 | |  | |                 |               
+-+-+--+-+--------------------------------+
| | |  | |                                |
+-+-+--+-+--------------------------------+
 0                                      36


*/
const COM_OBJ_DESCR objCom126[] = 
{
  {HART_IDX, HART_IDX_board, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_address, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_memoryAddress, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {HART_IDX, HART_IDX_memoryLength, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_memoryData, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32} // SIMPLE_U8[0..31]
};


/*
,--- read-command 128 -------------------------------------------------------
| ReadCalculationUnsignedCharVariable
| Read Calculation Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.KSET (KFACTOR_LIQUID)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.converterType (VORTEX)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.swirlmeterSize (Swirl50mm)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vortexmeterSize (VotrexD50mm)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.KLinearisation (AVERAGE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  SVMeasurement.QvObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QmObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QpObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QvPartialObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnPartialObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.frequency_Obj.unit (HERTZ)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.temperature_Obj.unit (CELSIUS)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvTExtObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvPressureObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvDensityObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.flowOver103Bhv (FLOWOVER103_BHV_ALARM_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.liquidCorrection (WITHOUT_CORRECTION)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasRef (UK)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasComputeType (IDEAL_GAS)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasDensitySelection (ACTUAL_DENSITY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.intRTDType (TEMP_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.operatingMode (LIQUID_ACTUAL_VOLUME)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 24 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.relativeDensityRef (RELATIVE_DENSITY_REF_DEG0_101325)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 25 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.calorificValueRef (CALORIFIC_REF_DEG25_DEG0_101325)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 26 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamStatus (SATURATED_STEAM)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 27 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamType (STEAMTYPE_SATURATED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 28 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 29 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 30 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQmUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKilograms)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 31 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totEnergyUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKiloWattHour)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 32 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 33 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 34 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.gasKProtection (ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 35 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagFunCheckMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 36 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 37 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMaintainMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 38 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMinFlowMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 39 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMaxFlowMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 40 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagFlow103Mask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 41 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTSensorOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 42 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTxSensorOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 43 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagSimulationType (OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 44 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvTExtSrc (TEXT_FROM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 45 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvPressureSrc (PRESSURE_FROM_AI)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 46 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvDensitySrc (DENSITY_FROM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 47 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvContentSrc (CONTENT_FORM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 48 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.matrixSize (SIZE9X7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 49 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.simulationMode (SIMULATION_CB_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 50 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.actualDensitySelection (EXT_DENSITY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 51 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTSensorFailMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 52 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.calibrationStatus (SENSOR_IS_UNCALIBRATED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 53 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagLowCutoffMask (ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 54 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lowFlowThreshOn (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 55 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.forceReplace (FORCE_REPLACE_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 56 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.autoselfcheck_Enable (STATUS_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 57 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vbrCorrectionOn (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 58 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vbrFilNumCoef (15)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 59 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockMinIntPeriod (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 60 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockMaxIntPeriod (4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 61 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockEstimation (10)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 62 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyHigh (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 63 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyMiddle (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 64 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyLow (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 65 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.analogGain (GAIN_CONTROL_1TIMES)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 66 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.gainCtrl (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 67 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.gainMaxShift (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 68 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.noiseSampSize (4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 69 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.staticDecimation (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 70 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.middleBufDecimation (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 71 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.freqMinMarginPer (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 72 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.freqMaxMarginPer (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 73 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamPwrrFR (FWD_ONLY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 74 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.ext_algSelection (EXT_ALG_NO_FILTER)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom128_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom128_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom128_slotCodeTranslationTable[] = {
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74
};

const COM_OBJ_DESCR specObjCom128_slotCodeObjDescrTab[] = 
{
  //slotcode 1
  {SVM_IDX, SVM_IDX_KSET, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {CDT_IDX, CDT_IDX_converterType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {CDT_IDX, CDT_IDX_swirlmeterSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {CDT_IDX, CDT_IDX_vortexmeterSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {SVM_IDX, SVM_IDX_KLinearisation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {SVM_IDX, SVM_IDX_QvObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 7
  {MVM_IDX, MVM_IDX_QnObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 8
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 9
  {MVM_IDX, MVM_IDX_QpObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 10
  {MVM_IDX, MVM_IDX_QvPartialObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 11
  {MVM_IDX, MVM_IDX_QnPartialObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 12
  {CDT_IDX, CDT_IDX_frequency_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 13
  {CDT_IDX, CDT_IDX_temperature_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 14
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 15
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 16
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 17
  {MAPPER_IDX, MAPPER_IDX_flowOver103Bhv, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 18
  {MVM_IDX, MVM_IDX_liquidCorrection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {MVM_IDX, MVM_IDX_gasRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {MVM_IDX, MVM_IDX_gasComputeType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {MVM_IDX, MVM_IDX_gasDensitySelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {CDT_IDX, CDT_IDX_intRTDType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {CDT_IDX, CDT_IDX_operatingMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 24
  {MVM_IDX, MVM_IDX_relativeDensityRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 25
  {MVM_IDX, MVM_IDX_calorificValueRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 26
  {MVM_IDX, MVM_IDX_steamStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 27
  {MVM_IDX, MVM_IDX_steamType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 28
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 29
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 30
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 31
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 32
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 33
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 34
  {SVM_IDX, SVM_IDX_gasKProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 35
  {MAPPER_IDX, MAPPER_IDX_diagFunCheckMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 36
  {MAPPER_IDX, MAPPER_IDX_diagOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 37
  {MAPPER_IDX, MAPPER_IDX_diagMaintainMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 38
  {MAPPER_IDX, MAPPER_IDX_diagMinFlowMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 39
  {MAPPER_IDX, MAPPER_IDX_diagMaxFlowMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 40
  {MAPPER_IDX, MAPPER_IDX_diagFlow103Mask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 41
  {MAPPER_IDX, MAPPER_IDX_diagTSensorOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 42
  {MAPPER_IDX, MAPPER_IDX_diagTxSensorOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 43
  {MAPPER_IDX, MAPPER_IDX_diagSimulationType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 44
  {MAPPER_IDX, MAPPER_IDX_dvTExtSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 45
  {MAPPER_IDX, MAPPER_IDX_dvPressureSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 46
  {MAPPER_IDX, MAPPER_IDX_dvDensitySrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 47
  {MAPPER_IDX, MAPPER_IDX_dvContentSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 48
  {MVM_IDX, MVM_IDX_matrixSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 49
  {CDT_IDX, CDT_IDX_simulationMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 50
  {CDT_IDX, CDT_IDX_actualDensitySelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 51
  {MAPPER_IDX, MAPPER_IDX_diagTSensorFailMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 52
  {CDT_IDX, CDT_IDX_calibrationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 53
  {MAPPER_IDX, MAPPER_IDX_diagLowCutoffMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 54
  {CDT_IDX, CDT_IDX_lowFlowThreshOn, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 55
  {CDT_IDX, CDT_IDX_forceReplace, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 56
  {CDT_IDX, CDT_IDX_autoselfcheck_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 57
  {CDT_IDX, CDT_IDX_vbrCorrectionOn, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 58
  {CDT_IDX, CDT_IDX_vbrFilNumCoef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 59
  {CDT_IDX, CDT_IDX_lockMinIntPeriod, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 60
  {CDT_IDX, CDT_IDX_lockMaxIntPeriod, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 61
  {CDT_IDX, CDT_IDX_lockEstimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 62
  {CDT_IDX, CDT_IDX_accuracyHigh, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 63
  {CDT_IDX, CDT_IDX_accuracyMiddle, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 64
  {CDT_IDX, CDT_IDX_accuracyLow, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 65
  {CDT_IDX, CDT_IDX_analogGain, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 66
  {CDT_IDX, CDT_IDX_gainCtrl, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 67
  {CDT_IDX, CDT_IDX_gainMaxShift, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 68
  {CDT_IDX, CDT_IDX_noiseSampSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 69
  {CDT_IDX, CDT_IDX_staticDecimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 70
  {CDT_IDX, CDT_IDX_middleBufDecimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 71
  {CDT_IDX, CDT_IDX_freqMinMarginPer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 72
  {CDT_IDX, CDT_IDX_freqMaxMarginPer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 73
  {MVM_IDX, MVM_IDX_steamPwrrFR, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 74
  {CDT_IDX, CDT_IDX_ext_algSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_REQ_RESP_CNT specObjCom128_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom128[] = 
{
  { specObjCom128_Req, specObjCom128_Resp, specObjCom128_ReqRespObjCnt, 1, specObjCom128_slotCodeObjDescrTab, 73, 1, specObjCom128_slotCodeTranslationTable}
};


/*
,--- write-command 129 -------------------------------------------------------
| WriteCalculationUnsignedCharVariable
| Write Calculation Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.KSET (KFACTOR_LIQUID)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.converterType (VORTEX)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.swirlmeterSize (Swirl50mm)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vortexmeterSize (VotrexD50mm)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.KLinearisation (AVERAGE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  SVMeasurement.QvObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QmObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QpObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QvPartialObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  MVMeasurement.QnPartialObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.frequency_Obj.unit (HERTZ)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Coordinator.temperature_Obj.unit (CELSIUS)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvTExtObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvPressureObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Mapper.dvDensityObj.unit ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.flowOver103Bhv (FLOWOVER103_BHV_ALARM_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.liquidCorrection (WITHOUT_CORRECTION)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasRef (UK)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasComputeType (IDEAL_GAS)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasDensitySelection (ACTUAL_DENSITY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.intRTDType (TEMP_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.operatingMode (LIQUID_ACTUAL_VOLUME)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 24 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.relativeDensityRef (RELATIVE_DENSITY_REF_DEG0_101325)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 25 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.calorificValueRef (CALORIFIC_REF_DEG25_DEG0_101325)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 26 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamStatus (SATURATED_STEAM)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 27 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamType (STEAMTYPE_SATURATED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 28 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 29 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 30 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQmUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKilograms)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 31 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totEnergyUnit.unitsCode (TOTALIZER_TotalizerUnitIDXKiloWattHour)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 32 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQvUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 33 |
`------------´

       byte(s)     type        object (default)

 +-->       #0    TD_UNIT  Totalizer.totQnUnit.unitsCode ((TUSIGN8)TOTALIZER_TotalizerUnitIDXCubicMetres)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 34 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.gasKProtection (ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 35 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagFunCheckMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 36 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 37 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMaintainMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 38 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMinFlowMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 39 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagMaxFlowMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 40 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagFlow103Mask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 41 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTSensorOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 42 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTxSensorOffSpecMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 43 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagSimulationType (OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 44 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvTExtSrc (TEXT_FROM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 45 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvPressureSrc (PRESSURE_FROM_AI)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 46 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvDensitySrc (DENSITY_FROM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 47 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.dvContentSrc (CONTENT_FORM_PRESET)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 48 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.matrixSize (SIZE9X7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 49 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.simulationMode (SIMULATION_CB_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 50 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.actualDensitySelection (EXT_DENSITY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 51 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagTSensorFailMask (DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 52 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.calibrationStatus (SENSOR_IS_UNCALIBRATED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 53 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagLowCutoffMask (ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 54 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lowFlowThreshOn (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 55 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.forceReplace (FORCE_REPLACE_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 56 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.autoselfcheck_Enable (STATUS_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 57 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vbrCorrectionOn (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 58 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.vbrFilNumCoef (15)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 59 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockMinIntPeriod (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 60 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockMaxIntPeriod (4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 61 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.lockEstimation (10)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 62 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyHigh (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 63 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyMiddle (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 64 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.accuracyLow (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 65 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.analogGain (GAIN_CONTROL_1TIMES)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 66 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.gainCtrl (STATUS_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 67 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.gainMaxShift (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 68 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.noiseSampSize (4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 69 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.staticDecimation (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 70 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.middleBufDecimation (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 71 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.freqMinMarginPer (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 72 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.freqMaxMarginPer (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 73 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.steamPwrrFR (FWD_ONLY)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 74 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.ext_algSelection (EXT_ALG_NO_FILTER)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom129_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom129_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom129_slotCodeTranslationTable[] = {
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74
};

const COM_OBJ_DESCR specObjCom129_slotCodeObjDescrTab[] = 
{
  //slotcode 1
  {SVM_IDX, SVM_IDX_KSET, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {CDT_IDX, CDT_IDX_converterType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {CDT_IDX, CDT_IDX_swirlmeterSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {CDT_IDX, CDT_IDX_vortexmeterSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {SVM_IDX, SVM_IDX_KLinearisation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {SVM_IDX, SVM_IDX_QvObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 7
  {MVM_IDX, MVM_IDX_QnObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 8
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 9
  {MVM_IDX, MVM_IDX_QpObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 10
  {MVM_IDX, MVM_IDX_QvPartialObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 11
  {MVM_IDX, MVM_IDX_QnPartialObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 12
  {CDT_IDX, CDT_IDX_frequency_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 13
  {CDT_IDX, CDT_IDX_temperature_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 14
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 15
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 16
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  //slotcode 17
  {MAPPER_IDX, MAPPER_IDX_flowOver103Bhv, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 18
  {MVM_IDX, MVM_IDX_liquidCorrection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {MVM_IDX, MVM_IDX_gasRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {MVM_IDX, MVM_IDX_gasComputeType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {MVM_IDX, MVM_IDX_gasDensitySelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {CDT_IDX, CDT_IDX_intRTDType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {CDT_IDX, CDT_IDX_operatingMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 24
  {MVM_IDX, MVM_IDX_relativeDensityRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 25
  {MVM_IDX, MVM_IDX_calorificValueRef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 26
  {MVM_IDX, MVM_IDX_steamStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 27
  {MVM_IDX, MVM_IDX_steamType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 28
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 29
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 30
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 31
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 32
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 33
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnUnit, ATTRIB, UNITS_unitsCode, TD_UNIT, HART_LEN_1}, // UNITS.unitsCode
  //slotcode 34
  {SVM_IDX, SVM_IDX_gasKProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 35
  {MAPPER_IDX, MAPPER_IDX_diagFunCheckMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 36
  {MAPPER_IDX, MAPPER_IDX_diagOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 37
  {MAPPER_IDX, MAPPER_IDX_diagMaintainMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 38
  {MAPPER_IDX, MAPPER_IDX_diagMinFlowMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 39
  {MAPPER_IDX, MAPPER_IDX_diagMaxFlowMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 40
  {MAPPER_IDX, MAPPER_IDX_diagFlow103Mask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 41
  {MAPPER_IDX, MAPPER_IDX_diagTSensorOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 42
  {MAPPER_IDX, MAPPER_IDX_diagTxSensorOffSpecMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 43
  {MAPPER_IDX, MAPPER_IDX_diagSimulationType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 44
  {MAPPER_IDX, MAPPER_IDX_dvTExtSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 45
  {MAPPER_IDX, MAPPER_IDX_dvPressureSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 46
  {MAPPER_IDX, MAPPER_IDX_dvDensitySrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 47
  {MAPPER_IDX, MAPPER_IDX_dvContentSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 48
  {MVM_IDX, MVM_IDX_matrixSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 49
  {CDT_IDX, CDT_IDX_simulationMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 50
  {CDT_IDX, CDT_IDX_actualDensitySelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 51
  {MAPPER_IDX, MAPPER_IDX_diagTSensorFailMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 52
  {CDT_IDX, CDT_IDX_calibrationStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 53
  {MAPPER_IDX, MAPPER_IDX_diagLowCutoffMask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 54
  {CDT_IDX, CDT_IDX_lowFlowThreshOn, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 55
  {CDT_IDX, CDT_IDX_forceReplace, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 56
  {CDT_IDX, CDT_IDX_autoselfcheck_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 57
  {CDT_IDX, CDT_IDX_vbrCorrectionOn, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 58
  {CDT_IDX, CDT_IDX_vbrFilNumCoef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 59
  {CDT_IDX, CDT_IDX_lockMinIntPeriod, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 60
  {CDT_IDX, CDT_IDX_lockMaxIntPeriod, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 61
  {CDT_IDX, CDT_IDX_lockEstimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 62
  {CDT_IDX, CDT_IDX_accuracyHigh, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 63
  {CDT_IDX, CDT_IDX_accuracyMiddle, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 64
  {CDT_IDX, CDT_IDX_accuracyLow, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 65
  {CDT_IDX, CDT_IDX_analogGain, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 66
  {CDT_IDX, CDT_IDX_gainCtrl, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 67
  {CDT_IDX, CDT_IDX_gainMaxShift, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 68
  {CDT_IDX, CDT_IDX_noiseSampSize, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 69
  {CDT_IDX, CDT_IDX_staticDecimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 70
  {CDT_IDX, CDT_IDX_middleBufDecimation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 71
  {CDT_IDX, CDT_IDX_freqMinMarginPer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 72
  {CDT_IDX, CDT_IDX_freqMaxMarginPer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 73
  {MVM_IDX, MVM_IDX_steamPwrrFR, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 74
  {CDT_IDX, CDT_IDX_ext_algSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_REQ_RESP_CNT specObjCom129_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom129[] = 
{
  { specObjCom129_Req, specObjCom129_Resp, specObjCom129_ReqRespObjCnt, 1, specObjCom129_slotCodeObjDescrTab, 73, 1, specObjCom129_slotCodeTranslationTable}
};


/*
,--- read-command 130 -------------------------------------------------------
| ReadIOBUSUnsignedCharVariable
| ReadI OBUS Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.hartCmdRev (HART_REVISION_7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.lockDeviceSelection (LOCK_DEVICE_UNLOCKED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.outputMode (DO1_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSgnlSrc (NO_LOGIC_SOURCE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.hartCmdRev (HART_REVISION_7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.lockDeviceSelection (LOCK_DEVICE_UNLOCKED)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.outputMode (DO1_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSgnlSrc (NO_LOGIC_SOURCE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.generalAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.minFlowrateAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.maxFlowrateAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.maxTemperatureAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lowFlowCutOff (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.logicValue (LOGIC_LOW)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiSelection (WITHOUT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.calibrationState (CALIBRATION_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSimCmd (COMMAND_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcActState (ACTIVE_HIGH)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiExternalCutOffStatus (EXTERNAL_UNCUT_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiExternalCutOffOption (EXTERNAL_CUT_OFF_4MA)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.minTemperatureAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom130_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom130_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom130_slotCodeTranslationTable[] = {
1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
};

const COM_OBJ_DESCR specObjCom130_slotCodeObjDescrTab[] = 
{
  //slotcode 1
  {HART_IDX, HART_IDX_hartCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {HART_IDX, HART_IDX_lockDeviceSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_outputMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSgnlSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {HART_IDX, HART_IDX_hartCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {HART_IDX, HART_IDX_lockDeviceSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 9
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_outputMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 10
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSgnlSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 11
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_generalAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 12
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minFlowrateAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 13
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxFlowrateAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 14
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxTemperatureAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 15
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lowFlowCutOff, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 16
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_logicValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 17
  {AI_IDX, AI_IDX_aiSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 18
  {AI_IDX, AI_IDX_calibrationState, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSimCmd, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcActState, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {AI_IDX, AI_IDX_aiExternalCutOffStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {AI_IDX, AI_IDX_aiExternalCutOffOption, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minTemperatureAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_REQ_RESP_CNT specObjCom130_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom130[] = 
{
  { specObjCom130_Req, specObjCom130_Resp, specObjCom130_ReqRespObjCnt, 1, specObjCom130_slotCodeObjDescrTab, 20, 1, specObjCom130_slotCodeTranslationTable}
};


/*
,--- write-command 131 -------------------------------------------------------
| WriteIOBUSUnsignedCharVariable
| WriteI OBUS Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.loopCurrentMode (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.outputMode (DO1_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSgnlSrc (NO_LOGIC_SOURCE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.generalAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.hartCmdRev (HART_REVISION_7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.loopCurrentMode (1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.outputMode (DO1_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSgnlSrc (NO_LOGIC_SOURCE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.generalAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.minFlowrateAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.maxFlowrateAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.maxTemperatureAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lowFlowCutOff (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.logicValue (LOGIC_LOW)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiSelection (WITHOUT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.calibrationState (CALIBRATION_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcSimCmd (COMMAND_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.lgcActState (ACTIVE_HIGH)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiExternalCutOffStatus (EXTERNAL_UNCUT_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  AnalogInput.aiExternalCutOffOption (EXTERNAL_CUT_OFF_4MA)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  DigitalOutput.minTemperatureAlarm (ALARM_DISABLE)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom131_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom131_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom131_slotCodeTranslationTable[] = {
1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
};

const COM_OBJ_DESCR specObjCom131_slotCodeObjDescrTab[] = 
{
  //slotcode 1
  {HART_IDX, HART_IDX_loopCurrentMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 2
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_outputMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSgnlSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_generalAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {HART_IDX, HART_IDX_hartCmdRev, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {HART_IDX, HART_IDX_loopCurrentMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  //slotcode 9
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_outputMode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 10
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSgnlSrc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 11
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_generalAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 12
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minFlowrateAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 13
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxFlowrateAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 14
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxTemperatureAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 15
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lowFlowCutOff, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 16
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_logicValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 17
  {AI_IDX, AI_IDX_aiSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 18
  {AI_IDX, AI_IDX_calibrationState, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcSimCmd, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_lgcActState, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {AI_IDX, AI_IDX_aiExternalCutOffStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {AI_IDX, AI_IDX_aiExternalCutOffOption, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minTemperatureAlarm, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};

const COM_REQ_RESP_CNT specObjCom131_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom131[] = 
{
  { specObjCom131_Req, specObjCom131_Resp, specObjCom131_ReqRespObjCnt, 1, specObjCom131_slotCodeObjDescrTab, 20, 1, specObjCom131_slotCodeTranslationTable}
};


/*
,--- read-command 132 -------------------------------------------------------
| ReadESUnsignedCharVariable
| Read ES Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.swWriteProtection (WRITE_PROTECTION_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.vSupplyReadbackEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.ramTestEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.romTestEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.dynamicVariablesCheckEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.autozeroStatus (STATUS_NOT_START)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.tranSapErpNoChangeFlag (0)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom132_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom132_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom132_slotCodeTranslationTable[] = {
0,2,3,4,5,6,8
};

const COM_OBJ_DESCR specObjCom132_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swWriteProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_ramTestEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_romTestEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_dynamicVariablesCheckEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_autozeroStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom132_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom132[] = 
{
  { specObjCom132_Req, specObjCom132_Resp, specObjCom132_ReqRespObjCnt, 1, specObjCom132_slotCodeObjDescrTab, 6, 1, specObjCom132_slotCodeTranslationTable}
};


/*
,--- write-command 133 -------------------------------------------------------
| WriteESUnsignedCharVariable
| Write ES Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.swWriteProtection (WRITE_PROTECTION_OFF)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.vSupplyReadbackEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.ramTestEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.romTestEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.dynamicVariablesCheckEnable (SIL_TEST_ON)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.autozeroStatus (STATUS_NOT_START)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.tranSapErpNoChangeFlag (0)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom133_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom133_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom133_slotCodeTranslationTable[] = {
0,2,3,4,5,6,8
};

const COM_OBJ_DESCR specObjCom133_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swWriteProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_ramTestEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_romTestEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_dynamicVariablesCheckEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_autozeroStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranSapErpNoChangeFlag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom133_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom133[] = 
{
  { specObjCom133_Req, specObjCom133_Resp, specObjCom133_ReqRespObjCnt, 1, specObjCom133_slotCodeObjDescrTab, 6, 1, specObjCom133_slotCodeTranslationTable}
};


/*
,--- read-command 134 -------------------------------------------------------
| ReadHMIUnsignedCharVariable
| Read HMI Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.language (DEFAULT_LANGUAGE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_1 (PAGE_1_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_2 (PAGE_1_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_3 (PAGE_1_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_bargraph (PAGE_1_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_1 (PAGE_2_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_2 (PAGE_2_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_3 (PAGE_2_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_bargraph (PAGE_2_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_1 (PAGE_3_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_2 (PAGE_3_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_3 (PAGE_3_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_bargraph (PAGE_3_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_1 (PAGE_4_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_2 (PAGE_4_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_3 (PAGE_4_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_bargraph (PAGE_4_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.contrast (50)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.localOperationEnable (DEFAULT_LOCAL_OP_ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_1 (PAGE_1_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_2 (PAGE_2_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_3 (PAGE_3_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_4 (PAGE_4_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.autoscrollEnable (DEFAULT_AUTOSCROLL_ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 24 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.lowLevelSoftwareRevision (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 25 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.defaultContrast (50)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 26 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.diagnosisView (DEFAULT_DIAGNOSIS_VIEW)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 27 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_1 (DEFAULT_CUSTOM_DP_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 28 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_2 (DEFAULT_CUSTOM_DP_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 29 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_3 (DEFAULT_CUSTOM_DP_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 30 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_4 (DEFAULT_CUSTOM_DP_4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 31 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_5 (DEFAULT_CUSTOM_DP_5)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 32 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_6 (DEFAULT_CUSTOM_DP_6)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 33 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_7 (DEFAULT_CUSTOM_DP_7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 34 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_8 (DEFAULT_CUSTOM_DP_8)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 35 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_9 (DEFAULT_CUSTOM_DP_9)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 36 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_10 (DEFAULT_CUSTOM_DP_10)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 37 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_11 (DEFAULT_CUSTOM_DP_11)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 38 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.DateFormat (DATE_FORMAT_YYYY_MM_DD)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 39 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayTest (0)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom134_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom134_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom134_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
};

const COM_OBJ_DESCR specObjCom134_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {HMI_IDX, HMI_IDX_language, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 1
  {HMI_IDX, HMI_IDX_mainOperatorView_1_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {HMI_IDX, HMI_IDX_mainOperatorView_1_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {HMI_IDX, HMI_IDX_mainOperatorView_1_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {HMI_IDX, HMI_IDX_mainOperatorView_1_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {HMI_IDX, HMI_IDX_mainOperatorView_2_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {HMI_IDX, HMI_IDX_mainOperatorView_2_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 7
  {HMI_IDX, HMI_IDX_mainOperatorView_2_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 8
  {HMI_IDX, HMI_IDX_mainOperatorView_2_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 9
  {HMI_IDX, HMI_IDX_mainOperatorView_3_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 10
  {HMI_IDX, HMI_IDX_mainOperatorView_3_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 11
  {HMI_IDX, HMI_IDX_mainOperatorView_3_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 12
  {HMI_IDX, HMI_IDX_mainOperatorView_3_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 13
  {HMI_IDX, HMI_IDX_mainOperatorView_4_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 14
  {HMI_IDX, HMI_IDX_mainOperatorView_4_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 15
  {HMI_IDX, HMI_IDX_mainOperatorView_4_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 16
  {HMI_IDX, HMI_IDX_mainOperatorView_4_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 17
  {HMI_IDX, HMI_IDX_contrast, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 18
  {HMI_IDX, HMI_IDX_localOperationEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {HMI_IDX, HMI_IDX_displayMode_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {HMI_IDX, HMI_IDX_displayMode_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {HMI_IDX, HMI_IDX_displayMode_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {HMI_IDX, HMI_IDX_displayMode_4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {HMI_IDX, HMI_IDX_autoscrollEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 24
  {HMI_IDX, HMI_IDX_lowLevelSoftwareRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 25
  {HMI_IDX, HMI_IDX_defaultContrast, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 26
  {HMI_IDX, HMI_IDX_diagnosisView, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 27
  {HMI_IDX, HMI_IDX_customDP_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 28
  {HMI_IDX, HMI_IDX_customDP_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 29
  {HMI_IDX, HMI_IDX_customDP_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 30
  {HMI_IDX, HMI_IDX_customDP_4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 31
  {HMI_IDX, HMI_IDX_customDP_5, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 32
  {HMI_IDX, HMI_IDX_customDP_6, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 33
  {HMI_IDX, HMI_IDX_customDP_7, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 34
  {HMI_IDX, HMI_IDX_customDP_8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 35
  {HMI_IDX, HMI_IDX_customDP_9, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 36
  {HMI_IDX, HMI_IDX_customDP_10, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 37
  {HMI_IDX, HMI_IDX_customDP_11, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 38
  {HMI_IDX, HMI_IDX_DateFormat, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 39
  {HMI_IDX, HMI_IDX_displayTest, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom134_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom134[] = 
{
  { specObjCom134_Req, specObjCom134_Resp, specObjCom134_ReqRespObjCnt, 1, specObjCom134_slotCodeObjDescrTab, 39, 1, specObjCom134_slotCodeTranslationTable}
};


/*
,--- write-command 135 -------------------------------------------------------
| WriteHMIUnsignedCharVariable
| Write HMI Unsigned Char Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.language (DEFAULT_LANGUAGE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_1 (PAGE_1_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_2 (PAGE_1_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_3 (PAGE_1_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_1_bargraph (PAGE_1_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_1 (PAGE_2_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_2 (PAGE_2_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_3 (PAGE_2_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_2_bargraph (PAGE_2_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_1 (PAGE_3_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_2 (PAGE_3_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_3 (PAGE_3_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_3_bargraph (PAGE_3_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_1 (PAGE_4_DEFAULT_LINE_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_2 (PAGE_4_DEFAULT_LINE_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_3 (PAGE_4_DEFAULT_LINE_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.mainOperatorView_4_bargraph (PAGE_4_DEFAULT_LINE_B)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.contrast (50)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.localOperationEnable (DEFAULT_LOCAL_OP_ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_1 (PAGE_1_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_2 (PAGE_2_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_3 (PAGE_3_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayMode_4 (PAGE_4_DEFAULT_FORMAT)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.autoscrollEnable (DEFAULT_AUTOSCROLL_ENABLE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 24 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.lowLevelSoftwareRevision (0)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 25 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.defaultContrast (50)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 26 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.diagnosisView (DEFAULT_DIAGNOSIS_VIEW)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 27 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_1 (DEFAULT_CUSTOM_DP_1)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 28 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_2 (DEFAULT_CUSTOM_DP_2)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 29 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_3 (DEFAULT_CUSTOM_DP_3)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 30 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_4 (DEFAULT_CUSTOM_DP_4)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 31 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_5 (DEFAULT_CUSTOM_DP_5)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 32 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_6 (DEFAULT_CUSTOM_DP_6)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 33 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_7 (DEFAULT_CUSTOM_DP_7)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 34 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_8 (DEFAULT_CUSTOM_DP_8)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 35 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_9 (DEFAULT_CUSTOM_DP_9)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 36 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_10 (DEFAULT_CUSTOM_DP_10)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 37 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.customDP_11 (DEFAULT_CUSTOM_DP_11)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 38 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.DateFormat (DATE_FORMAT_YYYY_MM_DD)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 39 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hmi.displayTest (0)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom135_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom135_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom135_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
};

const COM_OBJ_DESCR specObjCom135_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {HMI_IDX, HMI_IDX_language, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 1
  {HMI_IDX, HMI_IDX_mainOperatorView_1_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 2
  {HMI_IDX, HMI_IDX_mainOperatorView_1_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 3
  {HMI_IDX, HMI_IDX_mainOperatorView_1_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 4
  {HMI_IDX, HMI_IDX_mainOperatorView_1_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 5
  {HMI_IDX, HMI_IDX_mainOperatorView_2_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 6
  {HMI_IDX, HMI_IDX_mainOperatorView_2_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 7
  {HMI_IDX, HMI_IDX_mainOperatorView_2_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 8
  {HMI_IDX, HMI_IDX_mainOperatorView_2_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 9
  {HMI_IDX, HMI_IDX_mainOperatorView_3_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 10
  {HMI_IDX, HMI_IDX_mainOperatorView_3_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 11
  {HMI_IDX, HMI_IDX_mainOperatorView_3_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 12
  {HMI_IDX, HMI_IDX_mainOperatorView_3_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 13
  {HMI_IDX, HMI_IDX_mainOperatorView_4_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 14
  {HMI_IDX, HMI_IDX_mainOperatorView_4_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 15
  {HMI_IDX, HMI_IDX_mainOperatorView_4_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 16
  {HMI_IDX, HMI_IDX_mainOperatorView_4_bargraph, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 17
  {HMI_IDX, HMI_IDX_contrast, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 18
  {HMI_IDX, HMI_IDX_localOperationEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 19
  {HMI_IDX, HMI_IDX_displayMode_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 20
  {HMI_IDX, HMI_IDX_displayMode_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 21
  {HMI_IDX, HMI_IDX_displayMode_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 22
  {HMI_IDX, HMI_IDX_displayMode_4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 23
  {HMI_IDX, HMI_IDX_autoscrollEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 24
  {HMI_IDX, HMI_IDX_lowLevelSoftwareRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 25
  {HMI_IDX, HMI_IDX_defaultContrast, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // USIGN8
  //slotcode 26
  {HMI_IDX, HMI_IDX_diagnosisView, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 27
  {HMI_IDX, HMI_IDX_customDP_1, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 28
  {HMI_IDX, HMI_IDX_customDP_2, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 29
  {HMI_IDX, HMI_IDX_customDP_3, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 30
  {HMI_IDX, HMI_IDX_customDP_4, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 31
  {HMI_IDX, HMI_IDX_customDP_5, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 32
  {HMI_IDX, HMI_IDX_customDP_6, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 33
  {HMI_IDX, HMI_IDX_customDP_7, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 34
  {HMI_IDX, HMI_IDX_customDP_8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 35
  {HMI_IDX, HMI_IDX_customDP_9, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 36
  {HMI_IDX, HMI_IDX_customDP_10, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 37
  {HMI_IDX, HMI_IDX_customDP_11, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 38
  {HMI_IDX, HMI_IDX_DateFormat, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  //slotcode 39
  {HMI_IDX, HMI_IDX_displayTest, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom135_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom135[] = 
{
  { specObjCom135_Req, specObjCom135_Resp, specObjCom135_ReqRespObjCnt, 1, specObjCom135_slotCodeObjDescrTab, 39, 1, specObjCom135_slotCodeTranslationTable}
};


/*
,--- read-command 136 -------------------------------------------------------
| ReadUnsignedShortVariable
| Read Unsigned Short Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

 +----->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |  +-->   #1..#2   TD_INT16  Hart.dummys16 (-1234)
 |  |
+-+--+
| |  |
+-+--+
 0  2


,------------.
| SLOTCODE 0 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  DigitalOutput.simuPulsesNum (100)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 1 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  ElettronicServices.vSupplyReadbackHighThreshold (V_SUPPLY_READBACK_HIGH_THRESHOLD)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 2 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  ElettronicServices.vSupplyReadbackLowThreshold (V_SUPPLY_READBACK_LOW_THRESHOLD)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 3 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  SVMeasurement.calTime (60)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 4 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  SVMeasurement.numOfVals (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 5 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.lowFlowThresh (7)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 6 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  ElettronicServices.saveAsDefault (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 7 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.lowFlowThresh (7)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 8 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  Coordinator.sigAmplitude (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 9 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  Coordinator.vbrAmplitude (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 10 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  Coordinator.sigMagnitude (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 11 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  ElettronicServices.snrThreshold (-50)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 12 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.spanUpperRange (32767)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 13 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.vbrFilStep (33)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 14 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainLowThresh (2048)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 15 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainHighThresh (20480)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 16 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainMarginFreq (660)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 17 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.lowFlowThreshCoef (5)
  |
+--+
|  |
+--+
 01


*/
const COM_OBJ_DESCR specObjCom136_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummys16, OBJECT, HART_NO, TD_INT16, HART_LEN_2} // SIMPLE_I16
};

const COM_OBJ_DESCR specObjCom136_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom136_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom136_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_simuPulsesNum, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 1
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackHighThreshold, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackLowThreshold, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 3
  {SVM_IDX, SVM_IDX_calTime, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 4
  {SVM_IDX, SVM_IDX_numOfVals, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 5
  {CDT_IDX, CDT_IDX_lowFlowThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_saveAsDefault, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // SIMPLE_I16
  //slotcode 7
  {CDT_IDX, CDT_IDX_lowFlowThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 8
  {CDT_IDX, CDT_IDX_sigAmplitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 9
  {CDT_IDX, CDT_IDX_vbrAmplitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 10
  {CDT_IDX, CDT_IDX_sigMagnitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 11
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_snrThreshold, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 12
  {CDT_IDX, CDT_IDX_spanUpperRange, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 13
  {CDT_IDX, CDT_IDX_vbrFilStep, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 14
  {CDT_IDX, CDT_IDX_gainLowThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 15
  {CDT_IDX, CDT_IDX_gainHighThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 16
  {CDT_IDX, CDT_IDX_gainMarginFreq, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 17
  {CDT_IDX, CDT_IDX_lowFlowThreshCoef, OBJECT, HART_NO, TD_INT16, HART_LEN_2} // INT16
};

const COM_REQ_RESP_CNT specObjCom136_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom136[] = 
{
  { specObjCom136_Req, specObjCom136_Resp, specObjCom136_ReqRespObjCnt, 1, specObjCom136_slotCodeObjDescrTab, 17, 1, specObjCom136_slotCodeTranslationTable}
};


/*
,--- write-command 137 -------------------------------------------------------
| Write Unsigned Short Variable
| Write Unsigned Short Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

 +----->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |  +-->   #1..#2   TD_INT16  Hart.dummys16 (-1234)
 |  |
+-+--+
| |  |
+-+--+
 0  2


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

 +----->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |  +-->   #1..#2   TD_INT16  Hart.dummys16 (-1234)
 |  |
+-+--+
| |  |
+-+--+
 0  2


,------------.
| SLOTCODE 0 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  DigitalOutput.simuPulsesNum (100)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 1 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  ElettronicServices.vSupplyReadbackHighThreshold (V_SUPPLY_READBACK_HIGH_THRESHOLD)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 2 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1 TD_USIGN16  ElettronicServices.vSupplyReadbackLowThreshold (V_SUPPLY_READBACK_LOW_THRESHOLD)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 3 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  SVMeasurement.calTime (60)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 5 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.lowFlowThresh (7)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 6 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  ElettronicServices.saveAsDefault (0)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 11 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  ElettronicServices.snrThreshold (-50)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 12 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.spanUpperRange (32767)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 13 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.vbrFilStep (33)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 14 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainLowThresh (2048)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 15 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainHighThresh (20480)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 16 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.gainMarginFreq (660)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 17 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1   TD_INT16  Coordinator.lowFlowThreshCoef (5)
  |
+--+
|  |
+--+
 01


*/
const COM_OBJ_DESCR specObjCom137_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummys16, OBJECT, HART_NO, TD_INT16, HART_LEN_2} // SIMPLE_I16
};

const COM_OBJ_DESCR specObjCom137_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummys16, OBJECT, HART_NO, TD_INT16, HART_LEN_2} // SIMPLE_I16
};

const TUSIGN8 specObjCom137_slotCodeTranslationTable[] = {
0,1,2,3,5,6,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom137_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_simuPulsesNum, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 1
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackHighThreshold, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_vSupplyReadbackLowThreshold, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  //slotcode 3
  {SVM_IDX, SVM_IDX_calTime, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 5
  {CDT_IDX, CDT_IDX_lowFlowThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_saveAsDefault, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // SIMPLE_I16
  //slotcode 11
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_snrThreshold, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 12
  {CDT_IDX, CDT_IDX_spanUpperRange, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 13
  {CDT_IDX, CDT_IDX_vbrFilStep, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 14
  {CDT_IDX, CDT_IDX_gainLowThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 15
  {CDT_IDX, CDT_IDX_gainHighThresh, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 16
  {CDT_IDX, CDT_IDX_gainMarginFreq, OBJECT, HART_NO, TD_INT16, HART_LEN_2}, // INT16
  //slotcode 17
  {CDT_IDX, CDT_IDX_lowFlowThreshCoef, OBJECT, HART_NO, TD_INT16, HART_LEN_2} // INT16
};

const COM_REQ_RESP_CNT specObjCom137_ReqRespObjCnt[] = 
{
  { 3, 2, 2}    // slot 0: 2 REQ+RES Objects (3 Byte)
};

const COM_SPEC_TAB specObjCom137[] = 
{
  { specObjCom137_Req, specObjCom137_Resp, specObjCom137_ReqRespObjCnt, 1, specObjCom137_slotCodeObjDescrTab, 12, 1, specObjCom137_slotCodeTranslationTable}
};


/*
,--- read-command 138 -------------------------------------------------------
| ReadUnsignedLongVariable
| Read Unsigned Long Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4 TD_USIGN32  Hart.dummy32 (123456789)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 0 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.totalWorkingTimeHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 1 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.partialWorkingTimeHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 2 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.tranMaintenanceCyc (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 3 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.frontendMaintenance (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 4 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.frontendOperationHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 5 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  ElettronicServices.autoSelfCheckTime (525600)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 6 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Mapper.hartBurstInAlarmPeriod (60)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 7 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.BothMaintenanceCycle (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR specObjCom138_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_OBJ_DESCR specObjCom138_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom138_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7
};

const COM_OBJ_DESCR specObjCom138_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {CDT_IDX, CDT_IDX_totalWorkingTimeHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 1
  {CDT_IDX, CDT_IDX_partialWorkingTimeHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 2
  {CDT_IDX, CDT_IDX_tranMaintenanceCyc, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 3
  {CDT_IDX, CDT_IDX_frontendMaintenance, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 4
  {CDT_IDX, CDT_IDX_frontendOperationHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 5
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_autoSelfCheckTime, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 6
  {MAPPER_IDX, MAPPER_IDX_hartBurstInAlarmPeriod, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 7
  {CDT_IDX, CDT_IDX_BothMaintenanceCycle, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // USIGN32
};

const COM_REQ_RESP_CNT specObjCom138_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom138[] = 
{
  { specObjCom138_Req, specObjCom138_Resp, specObjCom138_ReqRespObjCnt, 1, specObjCom138_slotCodeObjDescrTab, 7, 1, specObjCom138_slotCodeTranslationTable}
};


/*
,--- write-command 139 -------------------------------------------------------
| WriteUnsignedLongVariable
| Write Unsigned Long Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4 TD_USIGN32  Hart.dummy32 (123456789)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4 TD_USIGN32  Hart.dummy32 (123456789)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 0 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.totalWorkingTimeHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 1 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.partialWorkingTimeHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 2 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.tranMaintenanceCyc (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 3 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.frontendMaintenance (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 4 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.frontendOperationHour (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 5 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  ElettronicServices.autoSelfCheckTime (525600)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 6 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Mapper.hartBurstInAlarmPeriod (60)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 7 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3 TD_USIGN32  Coordinator.BothMaintenanceCycle (DEFAULT_MAINTENANCE_CYCLE_HOURS)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR specObjCom139_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const COM_OBJ_DESCR specObjCom139_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // SIMPLE_U32
};

const TUSIGN8 specObjCom139_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7
};

const COM_OBJ_DESCR specObjCom139_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {CDT_IDX, CDT_IDX_totalWorkingTimeHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 1
  {CDT_IDX, CDT_IDX_partialWorkingTimeHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 2
  {CDT_IDX, CDT_IDX_tranMaintenanceCyc, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 3
  {CDT_IDX, CDT_IDX_frontendMaintenance, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 4
  {CDT_IDX, CDT_IDX_frontendOperationHour, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  //slotcode 5
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_autoSelfCheckTime, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 6
  {MAPPER_IDX, MAPPER_IDX_hartBurstInAlarmPeriod, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // USIGN32
  //slotcode 7
  {CDT_IDX, CDT_IDX_BothMaintenanceCycle, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4} // USIGN32
};

const COM_REQ_RESP_CNT specObjCom139_ReqRespObjCnt[] = 
{
  { 5, 2, 2}    // slot 0: 2 REQ+RES Objects (5 Byte)
};

const COM_SPEC_TAB specObjCom139[] = 
{
  { specObjCom139_Req, specObjCom139_Resp, specObjCom139_ReqRespObjCnt, 1, specObjCom139_slotCodeObjDescrTab, 7, 1, specObjCom139_slotCodeTranslationTable}
};


/*
,--- read-command 140 -------------------------------------------------------
| ReadFloatVariable
| Read Float Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 0 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KAverageGas (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 1 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KAverageLiquid (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 2 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[0] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 3 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[1] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 4 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[2] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 5 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[3] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 6 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[4] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 7 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[0] (0.50f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 8 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[1] (41.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 9 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[2] (62.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 10 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[3] (83.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 11 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[4] (104.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 12 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.frequency (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 13 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.frequency_Sim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 14 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperaturePreset_Obj.value_0 (0.0F)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 15 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[0] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 16 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[1] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 17 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[2] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 18 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[3] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 19 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[4] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 20 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[0] (44.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 21 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[1] (427.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 22 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[2] (810.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 23 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[3] (1193.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 24 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[4] (1275.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 25 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 26 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 27 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 28 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 29 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 30 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 31 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 32 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 33 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 34 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 35 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 36 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 37 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 38 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 39 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 40 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 41 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 42 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 43 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpDampedObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 44 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 45 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 46 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 47 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Damped_Obj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 48 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Percentage (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 49 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 50 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtPercentage (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 51 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtPresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 52 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 53 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 54 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressurePresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 55 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 56 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 57 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 58 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 59 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvContent (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 60 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvContentPreset (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 61 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvContentRange.span (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 62 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.Hw (1.0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 63 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.Hc (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 64 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 65 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 66 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 67 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispQv.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 68 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispQn.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 69 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispQm.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 70 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispEnergy.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 71 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispQvPartial.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 72 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Totalizer.totDispQnPartial.simpleValue (TOTSIMPLEDISPVAL_DFLT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 73 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.meanFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 74 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.stddevFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 75 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.minFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 76 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.maxFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 77 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.pulseFactor.value (PULSE_FACTOR_DEFAULT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 78 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.pulseWidth.value (PULSE_WIDTH_DEFAULT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 79 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.minFrequency.value (OUTPUTFREQ_MIN)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 80 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.maxFrequency.value (OUTPUTFREQ_MAX)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 81 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.frequencyValue (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 82 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_1 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 83 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 84 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.snr (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 85 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.calorificEnergy (36.0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 86 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 87 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.refTemperatureObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 88 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.volumeExpandBeta1 (0.325f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 89 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.densityExpandBeta2 (0.025)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 90 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.heatCapacity (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 91 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.loLim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 92 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.hiLim (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 93 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.loLim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 94 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.hiLim (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 95 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_1 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 96 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 97 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_1 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 98 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 99 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.relativeDensity (0.686)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 100 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvPerSim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 101 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvSimObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 102 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmPerSim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 103 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 104 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  ElettronicServices.elettronicTemperature_Obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 105 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureOffsetCorrectDisp (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 106 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.refCalorifcValue (36.64)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 107 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.hartInSim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 108 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.trimAiLowerEndpoint (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 109 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.trimAiUpperEndpoint (20.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 110 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiRawCurr (4.01f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 111 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiSimulationData (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 112 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 113 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiSimulationData (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 114 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.HwPreset (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 115 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.HcPreset (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 116 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.simuFrequency (10000.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 117 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 118 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 119 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorR (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 120 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorS (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 121 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureMaxRecord_obj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 122 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[5] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 123 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[6] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 124 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[7] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 125 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[5] (123.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 126 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[6] (143.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 127 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[7] (164.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 128 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[5] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 129 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[6] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 130 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[7] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 131 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[5] (1375.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 132 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[6] (1475.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 133 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[7] (1575.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 134 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 135 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 136 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.dynViscosity (0.0005f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 137 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureOffsetCorrectUserDisp (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 138 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.maxFlowFreq (3000.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 139 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.minFlowFreq (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 140 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  ElettronicServices.ambientTemperatureMax_obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR specObjCom140_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom140_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom140_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140
};

const COM_OBJ_DESCR specObjCom140_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_KAverageGas, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 1
  {SVM_IDX, SVM_IDX_KAverageLiquid, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 2
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 3
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 4
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 5
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 6
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 7
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 8
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 9
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 10
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 11
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 12
  {CDT_IDX, CDT_IDX_frequency, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 13
  {CDT_IDX, CDT_IDX_frequency_Sim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 14
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 15
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 16
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 17
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 18
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 19
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 20
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 21
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 22
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 23
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 24
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 25
  {MVM_IDX, MVM_IDX_QvPartialObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 26
  {MVM_IDX, MVM_IDX_QnPartialObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 27
  {MVM_IDX, MVM_IDX_QvPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 28
  {MVM_IDX, MVM_IDX_QnPartialDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 29
  {SVM_IDX, SVM_IDX_QvObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 30
  {SVM_IDX, SVM_IDX_QvDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 31
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 32
  {SVM_IDX, SVM_IDX_QvPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 33
  {SVM_IDX, SVM_IDX_QvMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 34
  {MVM_IDX, MVM_IDX_QnObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 35
  {MVM_IDX, MVM_IDX_QnDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 36
  {MVM_IDX, MVM_IDX_QnPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 37
  {MVM_IDX, MVM_IDX_QnMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 38
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 39
  {MVM_IDX, MVM_IDX_QmDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 40
  {MVM_IDX, MVM_IDX_QmPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 41
  {MVM_IDX, MVM_IDX_QmMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 42
  {MVM_IDX, MVM_IDX_QpObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 43
  {MVM_IDX, MVM_IDX_QpDampedObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 44
  {MVM_IDX, MVM_IDX_QpPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 45
  {MVM_IDX, MVM_IDX_QpMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 46
  {CDT_IDX, CDT_IDX_temperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 47
  {CDT_IDX, CDT_IDX_temperature_Damped_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 48
  {CDT_IDX, CDT_IDX_temperature_Percentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 49
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 50
  {MAPPER_IDX, MAPPER_IDX_dvTExtPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 51
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 52
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 53
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 54
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 55
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 56
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 57
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 58
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 59
  {MAPPER_IDX, MAPPER_IDX_dvContent, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 60
  {MAPPER_IDX, MAPPER_IDX_dvContentPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 61
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_span, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.span
  //slotcode 62
  {MVM_IDX, MVM_IDX_Hw, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 63
  {MVM_IDX, MVM_IDX_Hc, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 64
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 65
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 66
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 67
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQv, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 68
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQn, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 69
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQm, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 70
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispEnergy, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 71
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQvPartial, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 72
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQnPartial, ATTRIB, TOTDISP_simpleValue, TD_FLOAT, HART_LEN_4}, // TOTDISP.simpleValue
  //slotcode 73
  {SVM_IDX, SVM_IDX_meanFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 74
  {SVM_IDX, SVM_IDX_stddevFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 75
  {SVM_IDX, SVM_IDX_minFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 76
  {SVM_IDX, SVM_IDX_maxFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 77
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_pulseFactor, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 78
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_pulseWidth, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 79
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minFrequency, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 80
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxFrequency, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 81
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_frequencyValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 82
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 83
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 84
  {CDT_IDX, CDT_IDX_snr, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 85
  {MVM_IDX, MVM_IDX_calorificEnergy, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 86
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 87
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 88
  {MVM_IDX, MVM_IDX_volumeExpandBeta1, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 89
  {MVM_IDX, MVM_IDX_densityExpandBeta2, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 90
  {MVM_IDX, MVM_IDX_heatCapacity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 91
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  //slotcode 92
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 93
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  //slotcode 94
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 95
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 96
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 97
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 98
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 99
  {MVM_IDX, MVM_IDX_relativeDensity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 100
  {SVM_IDX, SVM_IDX_QvPerSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 101
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 102
  {MVM_IDX, MVM_IDX_QmPerSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 103
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 104
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 105
  {CDT_IDX, CDT_IDX_temperatureOffsetCorrectDisp, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 106
  {MVM_IDX, MVM_IDX_refCalorifcValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 107
  {HART_IDX, HART_IDX_hartInSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 108
  {AI_IDX, AI_IDX_trimAiLowerEndpoint, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 109
  {AI_IDX, AI_IDX_trimAiUpperEndpoint, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 110
  {AI_IDX, AI_IDX_aiRawCurr, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 111
  {AI_IDX, AI_IDX_aiSimulationData, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 112
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 113
  {AI_IDX, AI_IDX_aiSimulationData, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 114
  {MVM_IDX, MVM_IDX_HwPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 115
  {MVM_IDX, MVM_IDX_HcPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 116
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_simuFrequency, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 117
  {MVM_IDX, MVM_IDX_QvPartialMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 118
  {MVM_IDX, MVM_IDX_QnPartialMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 119
  {MVM_IDX, MVM_IDX_compressFactorR, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 120
  {MVM_IDX, MVM_IDX_compressFactorS, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 121
  {CDT_IDX, CDT_IDX_temperatureMaxRecord_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 122
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 123
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 124
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 125
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 126
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 127
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 128
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 129
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 130
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 131
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 132
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 133
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 134
  {MVM_IDX, MVM_IDX_QvPartialPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 135
  {MVM_IDX, MVM_IDX_QnPartialPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 136
  {SVM_IDX, SVM_IDX_dynViscosity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 137
  {CDT_IDX, CDT_IDX_temperatureOffsetCorrectUserDisp, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 138
  {CDT_IDX, CDT_IDX_maxFlowFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 139
  {CDT_IDX, CDT_IDX_minFlowFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 140
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_ambientTemperatureMax_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom140_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom140[] = 
{
  { specObjCom140_Req, specObjCom140_Resp, specObjCom140_ReqRespObjCnt, 1, specObjCom140_slotCodeObjDescrTab, 140, 1, specObjCom140_slotCodeTranslationTable}
};


/*
,--- write-command 141 -------------------------------------------------------
| WriteFloatVariable
| Write Float Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +--->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 0 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KAverageGas (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 1 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KAverageLiquid (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 2 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[0] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 3 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[1] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 4 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[2] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 5 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[3] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 6 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[4] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 7 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[0] (0.50f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 8 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[1] (41.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 9 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[2] (62.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 10 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[3] (83.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 11 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[4] (104.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 12 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.frequency (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 13 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.frequency_Sim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 14 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperaturePreset_Obj.value_0 (0.0F)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 15 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[0] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 16 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[1] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 17 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[2] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 18 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[3] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 19 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[4] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 20 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[0] (44.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 21 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[1] (427.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 22 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[2] (810.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 23 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[3] (1193.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 24 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[4] (1275.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 25 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartial (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 26 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartial (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 27 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 28 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 29 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 30 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 31 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 32 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 33 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 34 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 35 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 36 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 37 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 38 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 39 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 40 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 41 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 42 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 43 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpDamped (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 44 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpPercentage (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 45 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 46 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 47 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Damped (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 48 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperature_Percentage (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 49 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 50 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtPercentage (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 51 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtPresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 52 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvTExtRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 53 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 54 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressurePresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 55 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 56 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 57 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 58 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 59 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvContent (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 60 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvContentPreset (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 61 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvDensityRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 62 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.Hw (1.0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 63 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.Hc (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 64 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 65 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 66 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QpRangeObj.value_2 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 73 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.meanFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 74 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.stddevFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 75 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.minFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 76 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.maxFreq (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 77 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.pulseFactor.value (PULSE_FACTOR_DEFAULT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 78 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.pulseWidth.value (PULSE_WIDTH_DEFAULT)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 79 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.minFrequency.value (OUTPUTFREQ_MIN)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 80 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.maxFrequency.value (OUTPUTFREQ_MAX)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 81 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.frequencyValue (0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 82 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 83 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 84 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.snr (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 85 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.calorificEnergy (36.0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 86 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 87 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.refTemperatureObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 88 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.volumeExpandBeta1 (0.325f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 89 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.densityExpandBeta2 (0.025)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 90 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.heatCapacity (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 91 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.loLim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 92 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.hiLim (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 93 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.loLim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 94 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.hiLim (100.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 95 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_1 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 96 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 97 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_1 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 98 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 99 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.relativeDensity (0.686)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 100 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvPerSim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 101 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvSimObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 102 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmPerSim (0.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 103 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QmObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 104 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  ElettronicServices.elettronicTemperature_Obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 105 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureOffsetCorrectDisp (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 106 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.refCalorifcValue (36.64)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 107 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.hartInSim (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 108 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.trimAiLowerEndpoint (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 109 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.trimAiUpperEndpoint (20.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 111 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiSimulationData (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 112 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Hart.dummyFloat (-10.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 113 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiSimulationData (4.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 114 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.HwPreset (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 115 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.HcPreset (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 116 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  DigitalOutput.simuFrequency (10000.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 117 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QvPartialMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 118 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.QnPartialMaxDNObj.value_0 ()
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 119 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorR (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 120 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorS (1.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 122 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[5] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 123 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[6] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 124 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquid[7] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 125 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[5] (123.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 126 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[6] (143.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 127 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KLiquidFreq[7] (164.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 128 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[5] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 129 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[6] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 130 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGas[7] (7420.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 131 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[5] (1375.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 132 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[6] (1475.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 133 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.KGasFreq[7] (1575.00f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 136 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.dynViscosity (0.0005f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 137 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.temperatureOffsetCorrectUserDisp (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 138 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.maxFlowFreq (3000.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 139 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  Coordinator.minFlowFreq (0.0f)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 140 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  ElettronicServices.ambientTemperatureMax_obj.value_0 (0.0f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR specObjCom141_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom141_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom141_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,111,112,113,114,115,116,117,118,119,120,122,123,124,125,126,127,128,129,130,131,132,133,136,137,138,139,140
};

const COM_OBJ_DESCR specObjCom141_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_KAverageGas, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 1
  {SVM_IDX, SVM_IDX_KAverageLiquid, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 2
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 3
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 4
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 5
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 6
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 7
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 8
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 9
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 10
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 11
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 12
  {CDT_IDX, CDT_IDX_frequency, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 13
  {CDT_IDX, CDT_IDX_frequency_Sim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 14
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 15
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 16
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 17
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 18
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 19
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 20
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  //slotcode 21
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  //slotcode 22
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  //slotcode 23
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  //slotcode 24
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  //slotcode 25
  {MVM_IDX, MVM_IDX_QvPartial, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 26
  {MVM_IDX, MVM_IDX_QnPartial, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 27
  {MVM_IDX, MVM_IDX_QvPartialDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 28
  {MVM_IDX, MVM_IDX_QnPartialDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 29
  {SVM_IDX, SVM_IDX_QvObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 30
  {SVM_IDX, SVM_IDX_QvDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 31
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 32
  {SVM_IDX, SVM_IDX_QvPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 33
  {SVM_IDX, SVM_IDX_QvMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 34
  {MVM_IDX, MVM_IDX_QnObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 35
  {MVM_IDX, MVM_IDX_QnDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 36
  {MVM_IDX, MVM_IDX_QnPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 37
  {MVM_IDX, MVM_IDX_QnMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 38
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 39
  {MVM_IDX, MVM_IDX_QmDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 40
  {MVM_IDX, MVM_IDX_QmPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 41
  {MVM_IDX, MVM_IDX_QmMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 42
  {MVM_IDX, MVM_IDX_QpObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 43
  {MVM_IDX, MVM_IDX_QpDamped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 44
  {MVM_IDX, MVM_IDX_QpPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 45
  {MVM_IDX, MVM_IDX_QpMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 46
  {CDT_IDX, CDT_IDX_temperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 47
  {CDT_IDX, CDT_IDX_temperature_Damped, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 48
  {CDT_IDX, CDT_IDX_temperature_Percentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 49
  {MAPPER_IDX, MAPPER_IDX_dvTExtObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 50
  {MAPPER_IDX, MAPPER_IDX_dvTExtPercentage, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 51
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 52
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 53
  {MAPPER_IDX, MAPPER_IDX_dvPressureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 54
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 55
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 56
  {MAPPER_IDX, MAPPER_IDX_dvDensityObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 57
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 58
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 59
  {MAPPER_IDX, MAPPER_IDX_dvContent, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 60
  {MAPPER_IDX, MAPPER_IDX_dvContentPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 61
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 62
  {MVM_IDX, MVM_IDX_Hw, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 63
  {MVM_IDX, MVM_IDX_Hc, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 64
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 65
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 66
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_2, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_2
  //slotcode 73
  {SVM_IDX, SVM_IDX_meanFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 74
  {SVM_IDX, SVM_IDX_stddevFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 75
  {SVM_IDX, SVM_IDX_minFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 76
  {SVM_IDX, SVM_IDX_maxFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 77
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_pulseFactor, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 78
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_pulseWidth, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 79
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_minFrequency, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 80
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_maxFrequency, ATTRIB, FLOAT_FIXEDUNIT_value, TD_FLOAT, HART_LEN_4}, // FLOAT_FIXEDUNIT.value
  //slotcode 81
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_frequencyValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 82
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 83
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 84
  {CDT_IDX, CDT_IDX_snr, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 85
  {MVM_IDX, MVM_IDX_calorificEnergy, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 86
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 87
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 88
  {MVM_IDX, MVM_IDX_volumeExpandBeta1, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 89
  {MVM_IDX, MVM_IDX_densityExpandBeta2, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 90
  {MVM_IDX, MVM_IDX_heatCapacity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 91
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  //slotcode 92
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 93
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  //slotcode 94
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 95
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 96
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 97
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 98
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 99
  {MVM_IDX, MVM_IDX_relativeDensity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 100
  {SVM_IDX, SVM_IDX_QvPerSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 101
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 102
  {MVM_IDX, MVM_IDX_QmPerSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 103
  {MVM_IDX, MVM_IDX_QmObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 104
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 105
  {CDT_IDX, CDT_IDX_temperatureOffsetCorrectDisp, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 106
  {MVM_IDX, MVM_IDX_refCalorifcValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 107
  {HART_IDX, HART_IDX_hartInSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 108
  {AI_IDX, AI_IDX_trimAiLowerEndpoint, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 109
  {AI_IDX, AI_IDX_trimAiUpperEndpoint, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 111
  {AI_IDX, AI_IDX_aiSimulationData, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 112
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 113
  {AI_IDX, AI_IDX_aiSimulationData, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 114
  {MVM_IDX, MVM_IDX_HwPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 115
  {MVM_IDX, MVM_IDX_HcPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 116
  {DIGITALOUTPUT_IDX, DIGITALOUTPUT_IDX_simuFrequency, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 117
  {MVM_IDX, MVM_IDX_QvPartialMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 118
  {MVM_IDX, MVM_IDX_QnPartialMaxDNObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 119
  {MVM_IDX, MVM_IDX_compressFactorR, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 120
  {MVM_IDX, MVM_IDX_compressFactorS, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 122
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 123
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 124
  {SVM_IDX, SVM_IDX_KLiquid, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 125
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 126
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 127
  {SVM_IDX, SVM_IDX_KLiquidFreq, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 128
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 129
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 130
  {SVM_IDX, SVM_IDX_KGas, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 131
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  //slotcode 132
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  //slotcode 133
  {SVM_IDX, SVM_IDX_KGasFreq, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  //slotcode 136
  {SVM_IDX, SVM_IDX_dynViscosity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 137
  {CDT_IDX, CDT_IDX_temperatureOffsetCorrectUserDisp, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 138
  {CDT_IDX, CDT_IDX_maxFlowFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 139
  {CDT_IDX, CDT_IDX_minFlowFreq, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 140
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_ambientTemperatureMax_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom141_ReqRespObjCnt[] = 
{
  { 5, 2, 2}    // slot 0: 2 REQ+RES Objects (5 Byte)
};

const COM_SPEC_TAB specObjCom141[] = 
{
  { specObjCom141_Req, specObjCom141_Resp, specObjCom141_ReqRespObjCnt, 1, specObjCom141_slotCodeObjDescrTab, 130, 1, specObjCom141_slotCodeTranslationTable}
};


/*
,--- read-command 142 -------------------------------------------------------
| ReadDoubleVariable
| Read Double Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |     +----->   #1..#8  TD_USIGN8  Hart.dummyDouble (0.0)
 |     |   
+-+--------+
| |        |
+-+--------+
 0        8


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQvPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 3 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQnPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 4 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQmPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 5 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totEnergyPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 6 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQvPartialPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 7 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQnPartialPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 8 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispQv.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 9 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispQn.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 10 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispQm.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 11 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispEnergy.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 12 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispQvPartial.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 13 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totDispQnPartial.value (TOTDISPVAL_DFLT)
     |   
+--------+
|        |
+--------+
 0      7


*/
const COM_OBJ_DESCR specObjCom142_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyDouble, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8} // SIMPLE_DOUBLE
};

const COM_OBJ_DESCR specObjCom142_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom142_slotCodeTranslationTable[] = {
2,3,4,5,6,7,8,9,10,11,12,13
};

const COM_OBJ_DESCR specObjCom142_slotCodeObjDescrTab[] = 
{
  //slotcode 2
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 3
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 4
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 5
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 6
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 7
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 8
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQv, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8}, // TOTDISP.value
  //slotcode 9
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQn, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8}, // TOTDISP.value
  //slotcode 10
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQm, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8}, // TOTDISP.value
  //slotcode 11
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispEnergy, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8}, // TOTDISP.value
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQvPartial, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8}, // TOTDISP.value
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_totDispQnPartial, ATTRIB, TOTDISP_value, TD_DOUBLE, HART_LEN_8} // TOTDISP.value
};

const COM_REQ_RESP_CNT specObjCom142_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom142[] = 
{
  { specObjCom142_Req, specObjCom142_Resp, specObjCom142_ReqRespObjCnt, 1, specObjCom142_slotCodeObjDescrTab, 11, 1, specObjCom142_slotCodeTranslationTable}
};


/*
,--- write-command 143 -------------------------------------------------------
| WriteDoubleVariable
| Write Double Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |     +----->   #1..#8  TD_USIGN8  Hart.dummyDouble (0.0)
 |     |   
+-+--------+
| |        |
+-+--------+
 0        8


,----------.
| RESPONSE |
`----------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |     +----->   #1..#8  TD_USIGN8  Hart.dummyDouble (0.0)
 |     |   
+-+--------+
| |        |
+-+--------+
 0        8


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQvPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 3 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQnPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 4 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQmPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 5 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totEnergyPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 6 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQvPartialPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 7 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_DOUBLE  Totalizer.totQnPartialPresetValue (0.0f)
     |   
+--------+
|        |
+--------+
 0      7


*/
const COM_OBJ_DESCR specObjCom143_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyDouble, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8} // SIMPLE_DOUBLE
};

const COM_OBJ_DESCR specObjCom143_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyDouble, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8} // SIMPLE_DOUBLE
};

const TUSIGN8 specObjCom143_slotCodeTranslationTable[] = {
2,3,4,5,6,7
};

const COM_OBJ_DESCR specObjCom143_slotCodeObjDescrTab[] = 
{
  //slotcode 2
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 3
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 4
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 5
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 6
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8}, // SIMPLE_DOUBLE
  //slotcode 7
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialPresetValue, OBJECT, HART_NO, TD_DOUBLE, HART_LEN_8} // SIMPLE_DOUBLE
};

const COM_REQ_RESP_CNT specObjCom143_ReqRespObjCnt[] = 
{
  { 9, 2, 2}    // slot 0: 2 REQ+RES Objects (9 Byte)
};

const COM_SPEC_TAB specObjCom143[] = 
{
  { specObjCom143_Req, specObjCom143_Resp, specObjCom143_ReqRespObjCnt, 1, specObjCom143_slotCodeObjDescrTab, 5, 1, specObjCom143_slotCodeTranslationTable}
};


/*
,--- write-command 144 -------------------------------------------------------
| LogOutHartServiceCode
| Log Out Hart Service Code
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.serviceCodeLogOut ()
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.serviceCodeLogOut ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom144[] = 
{
  {HART_IDX, HART_IDX_serviceCodeLogOut, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};


/*
,--- write-command 146 -------------------------------------------------------
| PerformAction
| Perform Action
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.performActions ()
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 0 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartAllTotalizr ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 1 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartQvTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 2 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartQnTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 3 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartQmTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 4 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartEnergyTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 5 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartQvPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 6 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StartQnPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 7 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopAllTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 8 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopQvTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 9 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopQnTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 10 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopQmTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 11 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopEnergyTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 12 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopQvPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 13 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.StopQnPartialtotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 14 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetAllTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 15 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetQvTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 16 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetQnTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 17 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetQmTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 18 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetEnergyTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 19 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetQvPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 20 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.ResetQnPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 21 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetAllTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 22 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetQvTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 23 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetQnTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 24 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetQmTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 25 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetEnergyTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 26 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetQvPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 27 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Totalizer.PresetQnPartialTotalizer ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 28 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  SVMeasurement.trigCalibration ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 29 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.writeSelfDiagnosis ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 30 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagClearHistory ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 31 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.readParameters ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 32 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.writeParameters ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 33 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.saveCustomerDef ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 34 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.saveFactoryDef ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 35 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.resetCustomerDef ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 36 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.resetFactoryDef ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 37 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Coordinator.resetFE_Runhour ()
 |
+-+
| |
+-+
 0


,------------.
| SLOTCODE 38 |
`------------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.formatFENV ()
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR specObjCom146_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom146_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_performActions, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};

const TUSIGN8 specObjCom146_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38
};

const COM_OBJ_DESCR specObjCom146_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {TOTALIZER_IDX, TOTALIZER_IDX_StartAllTotalizr, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 1
  {TOTALIZER_IDX, TOTALIZER_IDX_StartQvTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 2
  {TOTALIZER_IDX, TOTALIZER_IDX_StartQnTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 3
  {TOTALIZER_IDX, TOTALIZER_IDX_StartQmTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 4
  {TOTALIZER_IDX, TOTALIZER_IDX_StartEnergyTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 5
  {TOTALIZER_IDX, TOTALIZER_IDX_StartQvPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 6
  {TOTALIZER_IDX, TOTALIZER_IDX_StartQnPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 7
  {TOTALIZER_IDX, TOTALIZER_IDX_StopAllTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 8
  {TOTALIZER_IDX, TOTALIZER_IDX_StopQvTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 9
  {TOTALIZER_IDX, TOTALIZER_IDX_StopQnTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 10
  {TOTALIZER_IDX, TOTALIZER_IDX_StopQmTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 11
  {TOTALIZER_IDX, TOTALIZER_IDX_StopEnergyTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_StopQvPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_StopQnPartialtotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 14
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetAllTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 15
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetQvTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 16
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetQnTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 17
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetQmTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 18
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetEnergyTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 19
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetQvPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 20
  {TOTALIZER_IDX, TOTALIZER_IDX_ResetQnPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 21
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetAllTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 22
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetQvTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 23
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetQnTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 24
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetQmTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 25
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetEnergyTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 26
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetQvPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 27
  {TOTALIZER_IDX, TOTALIZER_IDX_PresetQnPartialTotalizer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 28
  {SVM_IDX, SVM_IDX_trigCalibration, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 29
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_writeSelfDiagnosis, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 30
  {MAPPER_IDX, MAPPER_IDX_diagClearHistory, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 31
  {CDT_IDX, CDT_IDX_readParameters, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 32
  {CDT_IDX, CDT_IDX_writeParameters, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 33
  {CDT_IDX, CDT_IDX_saveCustomerDef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 34
  {CDT_IDX, CDT_IDX_saveFactoryDef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 35
  {CDT_IDX, CDT_IDX_resetCustomerDef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 36
  {CDT_IDX, CDT_IDX_resetFactoryDef, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 37
  {CDT_IDX, CDT_IDX_resetFE_Runhour, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0}, // ACTION
  //slotcode 38
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_formatFENV, OBJECT, HART_NO, TD_USIGN8, HART_LEN_0} // ACTION
};

const COM_REQ_RESP_CNT specObjCom146_ReqRespObjCnt[] = 
{
  { 1, 2, 2}    // slot 0: 2 REQ+RES Objects (1 Byte)
};

const COM_SPEC_TAB specObjCom146[] = 
{
  { specObjCom146_Req, specObjCom146_Resp, specObjCom146_ReqRespObjCnt, 1, specObjCom146_slotCodeObjDescrTab, 38, 1, specObjCom146_slotCodeTranslationTable}
};


/*
,--- read-command 150 -------------------------------------------------------
| ReadDiagnosisActiveAlarm
| Read Diagnosis Active Alarm
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Mapper.diagDeviceStatus[0..5] (0,0,0,0,0,0)
    |  
+------+
|      |
+------+
 0    5


*/
const COM_OBJ_DESCR objCom150[] = 
{
  {MAPPER_IDX, MAPPER_IDX_diagDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6} // SIMPLE_U8[0..5]
};


/*
,--- read-command 151 -------------------------------------------------------
| ReadDiagnosisActiveAlarmConditionDetail
| Read Diagnosis Active Alarm Condition Detail
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Mapper.diagnosisConditionNumber (0)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                                                byte(s)     type        object (default)

 +------------------------------------------->       #0  TD_USIGN8  Mapper.diagnosisConditionNumber (0)
 | +----------------------------------------->       #1  TD_USIGN8  Mapper.diagnosisConditionDetailsClass (0)
 | | +--------------------------------------->       #2  TD_USIGN8  Mapper.diagnosisConditionDetailsGroup (0)
 | | | +------------------------------------->       #3  TD_USIGN8  Mapper.diagnosisConditionDetailsPriority (0)
 | | | |  +---------------------------------->   #4..#5 TD_USIGN16  Mapper.diagnosisConditionDetailsAlarmCounter (0)
 | | | |  |  +------------------------------->   #6..#7 TD_USIGN16  Mapper.diagnosisConditionDetailsAlarmTimeCounterDay (0)
 | | | |  |  |   +--------------------------->  #8..#11 TD_USIGN32  Mapper.diagnosisConditionDetailsAlarmTimeCounterMsec (0)
 | | | |  |  |   |   +-----------------------> #12..#13 TD_USIGN16  Mapper.diagnosisConditionDetailsTimeStampLastAlarmDay (0)
 | | | |  |  |   |   |   +-------------------> #14..#17 TD_USIGN32  Mapper.diagnosisConditionDetailsTimeStampLastAlarmMsec (0)
 | | | |  |  |   |   |   |         +---------> #18..#32  TD_USIGN8  Mapper.diagnosisConditionDetailsCodes[0..14] (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
 | | | |  |  |   |   |   |         |       
+-+-+-+-+--+--+----+--+----+---------------+
| | | | |  |  |    |  |    |               |
+-+-+-+-+--+--+----+--+----+---------------+
 0                                       32


*/
const COM_OBJ_DESCR specObjCom151_Resp[] = 
{
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsClass, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsGroup, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsPriority, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsAlarmCounter, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsAlarmTimeCounterDay, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsAlarmTimeCounterMsec, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsTimeStampLastAlarmDay, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsTimeStampLastAlarmMsec, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsCodes, OBJECT, HART_NO, TD_USIGN8, HART_LEN_15} // SIMPLE_U8[0..14]
};

const COM_OBJ_DESCR specObjCom151_Req[] = 
{
  {MAPPER_IDX, MAPPER_IDX_diagnosisConditionNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom151_ReqRespObjCnt[] = 
{
  { 1, 1,10}   // 1 REQUEST Objects (1 Byte), 10 RESPOND Objects
};

const COM_SPEC_TAB specObjCom151[] = 
{
  { specObjCom151_Req, specObjCom151_Resp, specObjCom151_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 152 -------------------------------------------------------
| ReadFlowStatus
| Read Flow Status
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

           byte(s)     type        object (default)

 +------>       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +---->       #1  TD_USIGN8  Coordinator.vortexDeviceStatus (0)
 | | +-->       #2  TD_USIGN8  Coordinator.additionalVortexDeviceStatus (0)
 | | |
+-+-+-+
| | | |
+-+-+-+
 0   2


*/
const COM_OBJ_DESCR specObjCom152_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {CDT_IDX, CDT_IDX_vortexDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {CDT_IDX, CDT_IDX_additionalVortexDeviceStatus, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom152_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom152_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom152[] = 
{
  { specObjCom152_Req, specObjCom152_Resp, specObjCom152_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 153 -------------------------------------------------------
| ReadFamilyRevision
| Read Family Revision
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +-->       #1  TD_USIGN8  Coordinator.familyRevision (5)
 | |
+-+-+
| | |
+-+-+
 0 1


*/
const COM_OBJ_DESCR specObjCom153_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {CDT_IDX, CDT_IDX_familyRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // CONST_U8
};

const COM_OBJ_DESCR specObjCom153_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom153_ReqRespObjCnt[] = 
{
  { 1, 1, 2}   // 1 REQUEST Objects (1 Byte), 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom153[] = 
{
  { specObjCom153_Req, specObjCom153_Resp, specObjCom153_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 154 -------------------------------------------------------
| ReadVortexFrequency
| Read Vortex Frequency
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------->       #1    TD_UNIT  Coordinator.frequency_Obj.unit (HERTZ)
 | |   +--->   #2..#5   TD_FLOAT  Coordinator.frequency_Obj.value_0 (0.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom154_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {CDT_IDX, CDT_IDX_frequency_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_OBJ_DESCR specObjCom154_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom154_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom154[] = 
{
  { specObjCom154_Req, specObjCom154_Resp, specObjCom154_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 155 -------------------------------------------------------
| ReadLowFlowCutoff
| Read Low Flow Cutoff
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvLowCutOff (4.00f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom155[] = 
{
  {SVM_IDX, SVM_IDX_QvLowCutOff, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- read-command 156 -------------------------------------------------------
| ReadDensityPreset
| Read Density Preset
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------->       #1    TD_UNIT  Mapper.dvDensityPresetObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom156_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_OBJ_DESCR specObjCom156_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom156_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom156[] = 
{
  { specObjCom156_Req, specObjCom156_Resp, specObjCom156_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 157 -------------------------------------------------------
| ReadDensityPresetUnit
| Read Density Preset Unit
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------->       #1    TD_UNIT  Mapper.dvDensityPresetObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom157_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_OBJ_DESCR specObjCom157_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom157_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom157[] = 
{
  { specObjCom157_Req, specObjCom157_Resp, specObjCom157_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 158 -------------------------------------------------------
| ReadViscosity
| Read Viscosity
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  SVMeasurement.velocity (0.00f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom158_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {SVM_IDX, SVM_IDX_velocity, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom158_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom158_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom158[] = 
{
  { specObjCom158_Req, specObjCom158_Resp, specObjCom158_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 159 -------------------------------------------------------
| ReadTubeDiameterSize
| Read Tube Diameter Size
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  SVMeasurement.diameter (50.00f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom159_Resp[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {SVM_IDX, SVM_IDX_diameter, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom159_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom159_ReqRespObjCnt[] = 
{
  { 1, 1, 3}   // 1 REQUEST Objects (1 Byte), 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom159[] = 
{
  { specObjCom159_Req, specObjCom159_Resp, specObjCom159_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- write-command 160 -------------------------------------------------------
| WriteLowFlowCutoff
| Write Low Flow Cutoff
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvLowCutOff (4.00f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.QvLowCutOff (4.00f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom160[] = 
{
  {SVM_IDX, SVM_IDX_QvLowCutOff, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- write-command 164 -------------------------------------------------------
| WriteTubeDiameterSize
| Write Tube Diameter Size
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.diameter (50.00f)
   | 
+----+
|    |
+----+
 0  3


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  SVMeasurement.diameter (50.00f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom164[] = 
{
  {SVM_IDX, SVM_IDX_diameter, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- write-command 165 -------------------------------------------------------
| WriteHartServiceCode
| Write Hart Service Code
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,----------.
| RESPONSE |
`----------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


*/
const COM_OBJ_DESCR objCom165[] = 
{
  {HART_IDX, HART_IDX_ServiceCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // HART_SERVICECODE[0..19]
};


/*
,--- read-command 166 -------------------------------------------------------
| ReadFloats
| Read Floats
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                                                    byte(s)     type        object (default)

 +----------------------------------------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +------------------------------------------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +-------------------------------------->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    +--------------------------------->  #9..#12   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    +----------------------------> #13..#16   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    +-----------------------> #17..#20   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    +------------------> #21..#24   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    +-------------> #25..#28   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    +--------> #29..#32   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    +---> #33..#36   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    | 
+-+----+----+----+----+----+----+----+----+----+
| |    |    |    |    |    |    |    |    |    |
+-+----+----+----+----+----+----+----+----+----+
 0                                           36


,------------.
| SLOTCODE 0 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[0] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[1] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[2] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[3] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[4] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[5] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[6] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[7] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[8] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 1 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[9] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[10] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[11] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[12] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[13] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[14] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[15] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[16] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[17] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 2 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[18] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[19] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[20] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[21] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[22] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[23] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[24] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[25] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[26] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 3 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[27] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[28] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[29] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[30] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[31] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[32] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[33] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[34] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[35] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 4 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[36] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[37] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[38] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[39] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[40] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[41] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[42] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[43] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[44] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 5 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[45] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[46] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[47] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[48] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[49] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[50] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[51] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[52] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[53] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 6 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[54] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[55] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[56] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[57] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[58] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[59] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[60] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[61] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[62] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 7 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.CFPressArray0Obj.value_0 ()
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.CFPressArray1Obj.value_0 ()
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.CFPressArray2Obj.value_0 ()
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.CFPressArray3Obj.value_0 ()
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.CFPressArray4Obj.value_0 ()
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.CFPressArray5Obj.value_0 ()
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.CFPressArray6Obj.value_0 ()
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.CFPressArray7Obj.value_0 ()
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.CFPressArray8Obj.value_0 ()
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 8 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.CFTempArray0Obj.value_0 ()
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.CFTempArray1Obj.value_0 ()
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.CFTempArray2Obj.value_0 ()
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.CFTempArray3Obj.value_0 ()
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.CFTempArray4Obj.value_0 ()
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.CFTempArray5Obj.value_0 ()
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.CFTempArray6Obj.value_0 ()
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 9 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[0] (0.8120)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[1] (0.0430)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[2] (0.0090)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.molFractions[3] (0.0015)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.molFractions[4] (0.0015)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.molFractions[5] (0.0000)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.molFractions[6] (0.0000)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.molFractions[7] (0.0000)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.molFractions[8] (0.0000)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 10 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[9] (0.0000)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[10] (0.0000)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[11] (0.0000)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.molFractions[12] (0.0760)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.molFractions[13] (0.0570)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.molFractions[14] (0.0000)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.molFractions[15] (0.0000)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.molFractions[16] (0.0000)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.molFractions[17] (0.0000)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 11 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[18] (0.0000)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[19] (0.0000)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[20] (0.0000)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


*/
const COM_OBJ_DESCR specObjCom166_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom166_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom166_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11
};

const COM_OBJ_DESCR specObjCom166_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 8, TD_FLOAT, HART_LEN_4}, // FLOAT[8]
  //slotcode 1
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 9, TD_FLOAT, HART_LEN_4}, // FLOAT[9]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 10, TD_FLOAT, HART_LEN_4}, // FLOAT[10]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 11, TD_FLOAT, HART_LEN_4}, // FLOAT[11]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 12, TD_FLOAT, HART_LEN_4}, // FLOAT[12]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 13, TD_FLOAT, HART_LEN_4}, // FLOAT[13]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 14, TD_FLOAT, HART_LEN_4}, // FLOAT[14]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 15, TD_FLOAT, HART_LEN_4}, // FLOAT[15]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 16, TD_FLOAT, HART_LEN_4}, // FLOAT[16]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 17, TD_FLOAT, HART_LEN_4}, // FLOAT[17]
  //slotcode 2
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 18, TD_FLOAT, HART_LEN_4}, // FLOAT[18]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 19, TD_FLOAT, HART_LEN_4}, // FLOAT[19]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 20, TD_FLOAT, HART_LEN_4}, // FLOAT[20]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 21, TD_FLOAT, HART_LEN_4}, // FLOAT[21]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 22, TD_FLOAT, HART_LEN_4}, // FLOAT[22]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 23, TD_FLOAT, HART_LEN_4}, // FLOAT[23]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 24, TD_FLOAT, HART_LEN_4}, // FLOAT[24]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 25, TD_FLOAT, HART_LEN_4}, // FLOAT[25]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 26, TD_FLOAT, HART_LEN_4}, // FLOAT[26]
  //slotcode 3
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 27, TD_FLOAT, HART_LEN_4}, // FLOAT[27]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 28, TD_FLOAT, HART_LEN_4}, // FLOAT[28]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 29, TD_FLOAT, HART_LEN_4}, // FLOAT[29]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 30, TD_FLOAT, HART_LEN_4}, // FLOAT[30]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 31, TD_FLOAT, HART_LEN_4}, // FLOAT[31]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 32, TD_FLOAT, HART_LEN_4}, // FLOAT[32]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 33, TD_FLOAT, HART_LEN_4}, // FLOAT[33]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 34, TD_FLOAT, HART_LEN_4}, // FLOAT[34]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 35, TD_FLOAT, HART_LEN_4}, // FLOAT[35]
  //slotcode 4
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 36, TD_FLOAT, HART_LEN_4}, // FLOAT[36]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 37, TD_FLOAT, HART_LEN_4}, // FLOAT[37]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 38, TD_FLOAT, HART_LEN_4}, // FLOAT[38]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 39, TD_FLOAT, HART_LEN_4}, // FLOAT[39]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 40, TD_FLOAT, HART_LEN_4}, // FLOAT[40]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 41, TD_FLOAT, HART_LEN_4}, // FLOAT[41]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 42, TD_FLOAT, HART_LEN_4}, // FLOAT[42]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 43, TD_FLOAT, HART_LEN_4}, // FLOAT[43]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 44, TD_FLOAT, HART_LEN_4}, // FLOAT[44]
  //slotcode 5
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 45, TD_FLOAT, HART_LEN_4}, // FLOAT[45]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 46, TD_FLOAT, HART_LEN_4}, // FLOAT[46]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 47, TD_FLOAT, HART_LEN_4}, // FLOAT[47]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 48, TD_FLOAT, HART_LEN_4}, // FLOAT[48]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 49, TD_FLOAT, HART_LEN_4}, // FLOAT[49]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 50, TD_FLOAT, HART_LEN_4}, // FLOAT[50]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 51, TD_FLOAT, HART_LEN_4}, // FLOAT[51]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 52, TD_FLOAT, HART_LEN_4}, // FLOAT[52]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 53, TD_FLOAT, HART_LEN_4}, // FLOAT[53]
  //slotcode 6
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 54, TD_FLOAT, HART_LEN_4}, // FLOAT[54]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 55, TD_FLOAT, HART_LEN_4}, // FLOAT[55]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 56, TD_FLOAT, HART_LEN_4}, // FLOAT[56]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 57, TD_FLOAT, HART_LEN_4}, // FLOAT[57]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 58, TD_FLOAT, HART_LEN_4}, // FLOAT[58]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 59, TD_FLOAT, HART_LEN_4}, // FLOAT[59]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 60, TD_FLOAT, HART_LEN_4}, // FLOAT[60]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 61, TD_FLOAT, HART_LEN_4}, // FLOAT[61]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 62, TD_FLOAT, HART_LEN_4}, // FLOAT[62]
  //slotcode 7
  {MVM_IDX, MVM_IDX_CFPressArray0Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray1Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray2Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray3Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray4Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray5Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray6Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray7Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray8Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {MVM_IDX, MVM_IDX_CFTempArray0Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray1Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray2Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray3Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray4Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray5Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray6Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 9
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 8, TD_FLOAT, HART_LEN_4}, // FLOAT[8]
  //slotcode 10
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 9, TD_FLOAT, HART_LEN_4}, // FLOAT[9]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 10, TD_FLOAT, HART_LEN_4}, // FLOAT[10]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 11, TD_FLOAT, HART_LEN_4}, // FLOAT[11]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 12, TD_FLOAT, HART_LEN_4}, // FLOAT[12]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 13, TD_FLOAT, HART_LEN_4}, // FLOAT[13]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 14, TD_FLOAT, HART_LEN_4}, // FLOAT[14]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 15, TD_FLOAT, HART_LEN_4}, // FLOAT[15]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 16, TD_FLOAT, HART_LEN_4}, // FLOAT[16]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 17, TD_FLOAT, HART_LEN_4}, // FLOAT[17]
  //slotcode 11
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 18, TD_FLOAT, HART_LEN_4}, // FLOAT[18]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 19, TD_FLOAT, HART_LEN_4}, // FLOAT[19]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 20, TD_FLOAT, HART_LEN_4}, // FLOAT[20]
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_REQ_RESP_CNT specObjCom166_ReqRespObjCnt[] = 
{
  { 1, 1,10}    // slot 0: 1 REQUEST Objects (1 Byte) - 10 RESPOND Objects
};

const COM_SPEC_TAB specObjCom166[] = 
{
  { specObjCom166_Req, specObjCom166_Resp, specObjCom166_ReqRespObjCnt, 1, specObjCom166_slotCodeObjDescrTab, 11, 9, specObjCom166_slotCodeTranslationTable}
};


/*
,--- write-command 167 -------------------------------------------------------
| WriteFloats
| Write Floats
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                                                    byte(s)     type        object (default)

 +----------------------------------------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +------------------------------------------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +-------------------------------------->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    +--------------------------------->  #9..#12   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    +----------------------------> #13..#16   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    +-----------------------> #17..#20   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    +------------------> #21..#24   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    +-------------> #25..#28   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    +--------> #29..#32   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    +---> #33..#36   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    | 
+-+----+----+----+----+----+----+----+----+----+
| |    |    |    |    |    |    |    |    |    |
+-+----+----+----+----+----+----+----+----+----+
 0                                           36


,----------.
| RESPONSE |
`----------´

                                                    byte(s)     type        object (default)

 +----------------------------------------------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +------------------------------------------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +-------------------------------------->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    +--------------------------------->  #9..#12   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    +----------------------------> #13..#16   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    +-----------------------> #17..#20   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    +------------------> #21..#24   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    +-------------> #25..#28   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    +--------> #29..#32   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    +---> #33..#36   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    |    |    |    |    |    |    |    | 
+-+----+----+----+----+----+----+----+----+----+
| |    |    |    |    |    |    |    |    |    |
+-+----+----+----+----+----+----+----+----+----+
 0                                           36


,------------.
| SLOTCODE 0 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[0] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[1] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[2] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[3] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[4] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[5] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[6] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[7] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[8] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 1 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[9] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[10] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[11] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[12] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[13] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[14] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[15] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[16] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[17] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 2 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[18] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[19] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[20] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[21] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[22] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[23] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[24] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[25] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[26] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 3 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[27] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[28] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[29] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[30] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[31] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[32] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[33] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[34] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[35] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 4 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[36] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[37] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[38] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[39] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[40] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[41] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[42] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[43] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[44] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 5 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[45] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[46] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[47] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[48] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[49] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[50] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[51] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[52] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[53] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 6 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.compressFactorArray[54] (1.0f)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.compressFactorArray[55] (1.0f)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.compressFactorArray[56] (1.0f)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.compressFactorArray[57] (1.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.compressFactorArray[58] (1.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.compressFactorArray[59] (1.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.compressFactorArray[60] (1.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.compressFactorArray[61] (1.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.compressFactorArray[62] (1.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 7 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.CFPressArray0Obj.value_0 ()
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.CFPressArray1Obj.value_0 ()
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.CFPressArray2Obj.value_0 ()
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.CFPressArray3Obj.value_0 ()
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.CFPressArray4Obj.value_0 ()
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.CFPressArray5Obj.value_0 ()
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.CFPressArray6Obj.value_0 ()
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.CFPressArray7Obj.value_0 ()
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.CFPressArray8Obj.value_0 ()
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 8 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.CFTempArray0Obj.value_0 ()
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.CFTempArray1Obj.value_0 ()
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.CFTempArray2Obj.value_0 ()
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.CFTempArray3Obj.value_0 ()
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.CFTempArray4Obj.value_0 ()
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.CFTempArray5Obj.value_0 ()
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.CFTempArray6Obj.value_0 ()
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 9 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[0] (0.8120)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[1] (0.0430)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[2] (0.0090)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.molFractions[3] (0.0015)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.molFractions[4] (0.0015)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.molFractions[5] (0.0000)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.molFractions[6] (0.0000)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.molFractions[7] (0.0000)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.molFractions[8] (0.0000)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 10 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[9] (0.0000)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[10] (0.0000)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[11] (0.0000)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  MVMeasurement.molFractions[12] (0.0760)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  MVMeasurement.molFractions[13] (0.0570)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  MVMeasurement.molFractions[14] (0.0000)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  MVMeasurement.molFractions[15] (0.0000)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  MVMeasurement.molFractions[16] (0.0000)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  MVMeasurement.molFractions[17] (0.0000)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


,------------.
| SLOTCODE 11 |
`------------´

                                                  byte(s)     type        object (default)

   +------------------------------------------->   #0..#3   TD_FLOAT  MVMeasurement.molFractions[18] (0.0000)
   |    +-------------------------------------->   #4..#7   TD_FLOAT  MVMeasurement.molFractions[19] (0.0000)
   |    |    +--------------------------------->  #8..#11   TD_FLOAT  MVMeasurement.molFractions[20] (0.0000)
   |    |    |    +----------------------------> #12..#15   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    +-----------------------> #16..#19   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    +------------------> #20..#23   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    +-------------> #24..#27   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    +--------> #28..#31   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    +---> #32..#35   TD_FLOAT  Hart.dummyFloat (-10.0f)
   |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+
 0                                         35


*/
const COM_OBJ_DESCR specObjCom167_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom167_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom167_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11
};

const COM_OBJ_DESCR specObjCom167_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 8, TD_FLOAT, HART_LEN_4}, // FLOAT[8]
  //slotcode 1
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 9, TD_FLOAT, HART_LEN_4}, // FLOAT[9]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 10, TD_FLOAT, HART_LEN_4}, // FLOAT[10]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 11, TD_FLOAT, HART_LEN_4}, // FLOAT[11]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 12, TD_FLOAT, HART_LEN_4}, // FLOAT[12]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 13, TD_FLOAT, HART_LEN_4}, // FLOAT[13]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 14, TD_FLOAT, HART_LEN_4}, // FLOAT[14]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 15, TD_FLOAT, HART_LEN_4}, // FLOAT[15]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 16, TD_FLOAT, HART_LEN_4}, // FLOAT[16]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 17, TD_FLOAT, HART_LEN_4}, // FLOAT[17]
  //slotcode 2
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 18, TD_FLOAT, HART_LEN_4}, // FLOAT[18]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 19, TD_FLOAT, HART_LEN_4}, // FLOAT[19]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 20, TD_FLOAT, HART_LEN_4}, // FLOAT[20]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 21, TD_FLOAT, HART_LEN_4}, // FLOAT[21]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 22, TD_FLOAT, HART_LEN_4}, // FLOAT[22]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 23, TD_FLOAT, HART_LEN_4}, // FLOAT[23]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 24, TD_FLOAT, HART_LEN_4}, // FLOAT[24]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 25, TD_FLOAT, HART_LEN_4}, // FLOAT[25]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 26, TD_FLOAT, HART_LEN_4}, // FLOAT[26]
  //slotcode 3
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 27, TD_FLOAT, HART_LEN_4}, // FLOAT[27]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 28, TD_FLOAT, HART_LEN_4}, // FLOAT[28]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 29, TD_FLOAT, HART_LEN_4}, // FLOAT[29]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 30, TD_FLOAT, HART_LEN_4}, // FLOAT[30]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 31, TD_FLOAT, HART_LEN_4}, // FLOAT[31]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 32, TD_FLOAT, HART_LEN_4}, // FLOAT[32]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 33, TD_FLOAT, HART_LEN_4}, // FLOAT[33]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 34, TD_FLOAT, HART_LEN_4}, // FLOAT[34]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 35, TD_FLOAT, HART_LEN_4}, // FLOAT[35]
  //slotcode 4
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 36, TD_FLOAT, HART_LEN_4}, // FLOAT[36]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 37, TD_FLOAT, HART_LEN_4}, // FLOAT[37]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 38, TD_FLOAT, HART_LEN_4}, // FLOAT[38]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 39, TD_FLOAT, HART_LEN_4}, // FLOAT[39]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 40, TD_FLOAT, HART_LEN_4}, // FLOAT[40]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 41, TD_FLOAT, HART_LEN_4}, // FLOAT[41]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 42, TD_FLOAT, HART_LEN_4}, // FLOAT[42]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 43, TD_FLOAT, HART_LEN_4}, // FLOAT[43]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 44, TD_FLOAT, HART_LEN_4}, // FLOAT[44]
  //slotcode 5
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 45, TD_FLOAT, HART_LEN_4}, // FLOAT[45]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 46, TD_FLOAT, HART_LEN_4}, // FLOAT[46]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 47, TD_FLOAT, HART_LEN_4}, // FLOAT[47]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 48, TD_FLOAT, HART_LEN_4}, // FLOAT[48]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 49, TD_FLOAT, HART_LEN_4}, // FLOAT[49]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 50, TD_FLOAT, HART_LEN_4}, // FLOAT[50]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 51, TD_FLOAT, HART_LEN_4}, // FLOAT[51]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 52, TD_FLOAT, HART_LEN_4}, // FLOAT[52]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 53, TD_FLOAT, HART_LEN_4}, // FLOAT[53]
  //slotcode 6
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 54, TD_FLOAT, HART_LEN_4}, // FLOAT[54]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 55, TD_FLOAT, HART_LEN_4}, // FLOAT[55]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 56, TD_FLOAT, HART_LEN_4}, // FLOAT[56]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 57, TD_FLOAT, HART_LEN_4}, // FLOAT[57]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 58, TD_FLOAT, HART_LEN_4}, // FLOAT[58]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 59, TD_FLOAT, HART_LEN_4}, // FLOAT[59]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 60, TD_FLOAT, HART_LEN_4}, // FLOAT[60]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 61, TD_FLOAT, HART_LEN_4}, // FLOAT[61]
  {MVM_IDX, MVM_IDX_compressFactorArray, ATTRIB, 62, TD_FLOAT, HART_LEN_4}, // FLOAT[62]
  //slotcode 7
  {MVM_IDX, MVM_IDX_CFPressArray0Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray1Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray2Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray3Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray4Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray5Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray6Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray7Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFPressArray8Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {MVM_IDX, MVM_IDX_CFTempArray0Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray1Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray2Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray3Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray4Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray5Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {MVM_IDX, MVM_IDX_CFTempArray6Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 9
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 0, TD_FLOAT, HART_LEN_4}, // FLOAT[0]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 1, TD_FLOAT, HART_LEN_4}, // FLOAT[1]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 2, TD_FLOAT, HART_LEN_4}, // FLOAT[2]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 3, TD_FLOAT, HART_LEN_4}, // FLOAT[3]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 4, TD_FLOAT, HART_LEN_4}, // FLOAT[4]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 5, TD_FLOAT, HART_LEN_4}, // FLOAT[5]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 6, TD_FLOAT, HART_LEN_4}, // FLOAT[6]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 7, TD_FLOAT, HART_LEN_4}, // FLOAT[7]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 8, TD_FLOAT, HART_LEN_4}, // FLOAT[8]
  //slotcode 10
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 9, TD_FLOAT, HART_LEN_4}, // FLOAT[9]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 10, TD_FLOAT, HART_LEN_4}, // FLOAT[10]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 11, TD_FLOAT, HART_LEN_4}, // FLOAT[11]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 12, TD_FLOAT, HART_LEN_4}, // FLOAT[12]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 13, TD_FLOAT, HART_LEN_4}, // FLOAT[13]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 14, TD_FLOAT, HART_LEN_4}, // FLOAT[14]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 15, TD_FLOAT, HART_LEN_4}, // FLOAT[15]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 16, TD_FLOAT, HART_LEN_4}, // FLOAT[16]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 17, TD_FLOAT, HART_LEN_4}, // FLOAT[17]
  //slotcode 11
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 18, TD_FLOAT, HART_LEN_4}, // FLOAT[18]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 19, TD_FLOAT, HART_LEN_4}, // FLOAT[19]
  {MVM_IDX, MVM_IDX_molFractions, ATTRIB, 20, TD_FLOAT, HART_LEN_4}, // FLOAT[20]
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_REQ_RESP_CNT specObjCom167_ReqRespObjCnt[] = 
{
  {37,10,10}    // slot 0: 10 REQ+RES Objects (37 Byte)
};

const COM_SPEC_TAB specObjCom167[] = 
{
  { specObjCom167_Req, specObjCom167_Resp, specObjCom167_ReqRespObjCnt, 1, specObjCom167_slotCodeObjDescrTab, 11, 9, specObjCom167_slotCodeTranslationTable}
};


/*
,--- read-command 168 -------------------------------------------------------
| ReadGasConfFlag
| Read Gas Conf Flag
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasConfFlag (NONGONFIGED)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom168[] = 
{
  {MVM_IDX, MVM_IDX_gasConfFlag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- write-command 169 -------------------------------------------------------
| WriteGasConfFlag
| Write Gas Conf Flag
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasConfFlag (NONGONFIGED)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  MVMeasurement.gasConfFlag (NONGONFIGED)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom169[] = 
{
  {MVM_IDX, MVM_IDX_gasConfFlag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- read-command 170 -------------------------------------------------------
| ReadString
| Read String
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 0 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.swRevisionHmi_transmitter[0..7] ('0','0','.','0','0','.','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 1 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.hwRevisionHmi_transmitter[0..7] ('0','0','.','0','0','.','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.deviceSerialNumber[0..7] (51,75,64,0,0,0,0,0)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 3 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  ElettronicServices.feSerialNumber[0..2] ('A','B','B')
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 6 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  ElettronicServices.cbSerialNumber[0..2] (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 7 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.lowLevelHmiHwSwRevision[0..7] (32,32,32,32,32,32,32,32)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 8 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.sensorID[0..7] ('A','B','B',' ',' ',' ',' ',' ')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 9 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorSapErpNo[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 10 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.sensorCalDate[0..11] ('2','0','1','2','-','0','1','-','0','1','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 11 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorCalCertNo[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 12 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  ElettronicServices.sensorCalLocation[0..31] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 13 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.bootloaderRevision[0..7] ('0','0','.','0','0','.','0','4')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 14 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.tranID[0..7] ('0','0','0','0','0','0','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 15 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.tranSapErpNo[0..19] ('0','0','0','0','0','0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 16 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.tranCalDate[0..11] ('2','0','1','2','-','0','1','-','0','1','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 17 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.tranCalCertNo[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 18 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  ElettronicServices.tranCalLocation[0..31] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 19 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.manufacturer[0..19] ('A','B','B',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 20 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.street[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 21 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.city[0..19] ('S','h','a','n','g','H','a','i',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 22 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.phone[0..19] ('+','8','6',' ','2','1',' ','8','8','8','8','8','8','8','8',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 23 |
`------------´

           byte(s)     type        object (default)

   +---->   #0..#4  TD_USIGN8  ElettronicServices.frontendRtc[0..4] (0,0,0,0,0)
   |  
+-----+
|     |
+-----+
 0   4


,------------.
| SLOTCODE 24 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.swRevisionHmi_fe[0..7] ('0','0','.','0','0','.','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 25 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.hwRevisionHmi_fe[0..7] ('0','0','.','0','0','.','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 26 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 27 |
`------------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.deviceMessage[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x20,0x08,0x82)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


,------------.
| SLOTCODE 28 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.finalAssyNumber (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 29 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  Hart.descriptor[0..11] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 30 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.deviceIdNumber (1,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 31 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.deviceInstallDate (0x01,0x01,0x00)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 32 |
`------------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.measuringTask[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


,------------.
| SLOTCODE 33 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  Hart.devVariableTransducerSerialNo[0..2] (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 34 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 35 |
`------------´

          byte(s)     type        object (default)

   +--->   #0..#3  TD_USIGN8  Hart.nonValidDeviceVariableValue[0..3] (0x7F,0xA0,0x0,0x0)
   | 
+----+
|    |
+----+
 0  3


,------------.
| SLOTCODE 36 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  Hart.softModemInfo[0..7] (0,0,0,0,0,0,0,0)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 37 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  Hart.transducerSerialNumber[0..2] (0x00,0x00,0x00)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 38 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1  TD_USIGN8  Hart.countryCode[0..1] (67,78)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 39 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.tranType[0..11] ('0','0','0','0','0','0','0','0','0','0','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 40 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.internalRTDPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 41 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.analogInputPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 42 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.energyFlowPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 43 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hmi.SetPassword_StandardLevel[0..5] (0x00,0x00,0x00,0x00,0x00,0x00)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 44 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hmi.SetPassword_AdvancedLevel[0..5] (0x00,0x00,0x00,0x00,0x00,0x00)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 45 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorLocationTag[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 46 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensoTag[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


*/
const COM_OBJ_DESCR specObjCom170_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom170_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom170_slotCodeTranslationTable[] = {
0,1,2,3,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46
};

const COM_OBJ_DESCR specObjCom170_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swRevisionHmi_transmitter, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 1
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hwRevisionHmi_transmitter, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_deviceSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_U8[0..7]
  //slotcode 3
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_feSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_cbSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  //slotcode 7
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_lowLevelHmiHwSwRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorID, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 9
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorSapErpNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 10
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalDate, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 11
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalCertNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 12
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalLocation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_CHAR[0..31]
  //slotcode 13
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_bootloaderRevision, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_U8[0..7]
  //slotcode 14
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranID, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 15
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranSapErpNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 16
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalDate, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 17
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalCertNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 18
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalLocation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_CHAR[0..31]
  //slotcode 19
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_manufacturer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 20
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_street, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 21
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_city, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 22
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_phone, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 23
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_frontendRtc, OBJECT, HART_NO, TD_USIGN8, HART_LEN_5}, // SIMPLE_U8[0..4]
  //slotcode 24
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swRevisionHmi_fe, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 25
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hwRevisionHmi_fe, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 26
  {HART_IDX, HART_IDX_tag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 27
  {HART_IDX, HART_IDX_deviceMessage, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24}, // SIMPLE_U8[0..23]
  //slotcode 28
  {HART_IDX, HART_IDX_finalAssyNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 29
  {HART_IDX, HART_IDX_descriptor, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_U8[0..11]
  //slotcode 30
  {HART_IDX, HART_IDX_deviceIdNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 31
  {HART_IDX, HART_IDX_deviceInstallDate, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 32
  {HART_IDX, HART_IDX_measuringTask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24}, // SIMPLE_U8[0..23]
  //slotcode 33
  {HART_IDX, HART_IDX_devVariableTransducerSerialNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // CONST_U8[0..2]
  //slotcode 34
  {HART_IDX, HART_IDX_longTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_U8[0..31]
  //slotcode 35
  {HART_IDX, HART_IDX_nonValidDeviceVariableValue, OBJECT, HART_NO, TD_USIGN8, HART_LEN_4}, // CONST_U8[0..3]
  //slotcode 36
  {HART_IDX, HART_IDX_softModemInfo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_U8[0..7]
  //slotcode 37
  {HART_IDX, HART_IDX_transducerSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  //slotcode 38
  {HART_IDX, HART_IDX_countryCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // SIMPLE_U8[0..1]
  //slotcode 39
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 40
  {CDT_IDX, CDT_IDX_internalRTDPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 41
  {CDT_IDX, CDT_IDX_analogInputPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 42
  {CDT_IDX, CDT_IDX_energyFlowPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 43
  {HMI_IDX, HMI_IDX_SetPassword_StandardLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 44
  {HMI_IDX, HMI_IDX_SetPassword_AdvancedLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 45
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorLocationTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 46
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensoTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // SIMPLE_CHAR[0..19]
};

const COM_REQ_RESP_CNT specObjCom170_ReqRespObjCnt[] = 
{
  { 1, 1, 2}    // slot 0: 1 REQUEST Objects (1 Byte) - 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom170[] = 
{
  { specObjCom170_Req, specObjCom170_Resp, specObjCom170_ReqRespObjCnt, 1, specObjCom170_slotCodeObjDescrTab, 44, 1, specObjCom170_slotCodeTranslationTable}
};


/*
,--- write-command 171 -------------------------------------------------------
| WriteString
| Write String
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +-->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |
+-+-+
| | |
+-+-+
 0 1


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.deviceSerialNumber[0..7] (51,75,64,0,0,0,0,0)
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 3 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  ElettronicServices.feSerialNumber[0..2] ('A','B','B')
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 6 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2  TD_USIGN8  ElettronicServices.cbSerialNumber[0..2] (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 8 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.sensorID[0..7] ('A','B','B',' ',' ',' ',' ',' ')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 9 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorSapErpNo[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 10 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.sensorCalDate[0..11] ('2','0','1','2','-','0','1','-','0','1','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 11 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorCalCertNo[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 12 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  ElettronicServices.sensorCalLocation[0..31] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 14 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.tranID[0..7] ('0','0','0','0','0','0','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 15 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.tranSapErpNo[0..19] ('0','0','0','0','0','0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 16 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.tranCalDate[0..11] ('2','0','1','2','-','0','1','-','0','1','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 17 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.tranCalCertNo[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 18 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  ElettronicServices.tranCalLocation[0..31] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 19 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.manufacturer[0..19] ('A','B','B',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 20 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.street[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 21 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.city[0..19] ('S','h','a','n','g','H','a','i',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 22 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.phone[0..19] ('+','8','6',' ','2','1',' ','8','8','8','8','8','8','8','8',' ',' ',' ',' ',' ')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 25 |
`------------´

              byte(s)     type        object (default)

     +----->   #0..#7  TD_USIGN8  ElettronicServices.hwRevisionHmi_fe[0..7] ('0','0','.','0','0','.','0','0')
     |   
+--------+
|        |
+--------+
 0      7


,------------.
| SLOTCODE 26 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hart.tag[0..5] (0x82,0x08,0x20,0x82,0x08,0x20)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 27 |
`------------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.deviceMessage[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x20,0x08,0x82)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


,------------.
| SLOTCODE 28 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.finalAssyNumber (0,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 29 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  Hart.descriptor[0..11] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 30 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.deviceIdNumber (1,0,0)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 31 |
`------------´

         byte(s)     type        object (default)

  +--->   #0..#2 TD_USIGN24  Hart.deviceInstallDate (0x01,0x01,0x00)
  | 
+---+
|   |
+---+
 0 2


,------------.
| SLOTCODE 32 |
`------------´

                              byte(s)     type        object (default)

             +------------->  #0..#23  TD_USIGN8  Hart.measuringTask[0..23] (0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20,0x82,0x08,0x20)
             |           
+------------------------+
|                        |
+------------------------+
 0                     23


,------------.
| SLOTCODE 34 |
`------------´

                                      byte(s)     type        object (default)

                 +----------------->  #0..#31  TD_USIGN8  Hart.longTag[0..31] (0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F)
                 |               
+--------------------------------+
|                                |
+--------------------------------+
 0                             31


,------------.
| SLOTCODE 38 |
`------------´

        byte(s)     type        object (default)

  +-->   #0..#1  TD_USIGN8  Hart.countryCode[0..1] (67,78)
  |
+--+
|  |
+--+
 01


,------------.
| SLOTCODE 39 |
`------------´

                  byte(s)     type        object (default)

       +------->  #0..#11  TD_USIGN8  ElettronicServices.tranType[0..11] ('0','0','0','0','0','0','0','0','0','0','0','0')
       |     
+------------+
|            |
+------------+
 0         11


,------------.
| SLOTCODE 40 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.internalRTDPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 41 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.analogInputPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 42 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Coordinator.energyFlowPassword[0..5] ('*','*','*','*','*','*')
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 43 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hmi.SetPassword_StandardLevel[0..5] (0x00,0x00,0x00,0x00,0x00,0x00)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 44 |
`------------´

            byte(s)     type        object (default)

    +---->   #0..#5  TD_USIGN8  Hmi.SetPassword_AdvancedLevel[0..5] (0x00,0x00,0x00,0x00,0x00,0x00)
    |  
+------+
|      |
+------+
 0    5


,------------.
| SLOTCODE 45 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensorLocationTag[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


,------------.
| SLOTCODE 46 |
`------------´

                          byte(s)     type        object (default)

           +----------->  #0..#19  TD_USIGN8  ElettronicServices.sensoTag[0..19] ('0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0')
           |         
+--------------------+
|                    |
+--------------------+
 0                 19


*/
const COM_OBJ_DESCR specObjCom171_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_OBJ_DESCR specObjCom171_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom171_slotCodeTranslationTable[] = {
2,3,6,8,9,10,11,12,14,15,16,17,18,19,20,21,22,25,26,27,28,29,30,31,32,34,38,39,40,41,42,43,44,45,46
};

const COM_OBJ_DESCR specObjCom171_slotCodeObjDescrTab[] = 
{
  //slotcode 2
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_deviceSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_U8[0..7]
  //slotcode 3
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_feSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  //slotcode 6
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_cbSerialNumber, OBJECT, HART_NO, TD_USIGN8, HART_LEN_3}, // SIMPLE_U8[0..2]
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorID, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 9
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorSapErpNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 10
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalDate, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 11
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalCertNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 12
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalLocation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_CHAR[0..31]
  //slotcode 14
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranID, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 15
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranSapErpNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 16
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalDate, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 17
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalCertNo, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 18
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalLocation, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_CHAR[0..31]
  //slotcode 19
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_manufacturer, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 20
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_street, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 21
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_city, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 22
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_phone, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 25
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hwRevisionHmi_fe, OBJECT, HART_NO, TD_USIGN8, HART_LEN_8}, // SIMPLE_CHAR[0..7]
  //slotcode 26
  {HART_IDX, HART_IDX_tag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 27
  {HART_IDX, HART_IDX_deviceMessage, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24}, // SIMPLE_U8[0..23]
  //slotcode 28
  {HART_IDX, HART_IDX_finalAssyNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 29
  {HART_IDX, HART_IDX_descriptor, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_U8[0..11]
  //slotcode 30
  {HART_IDX, HART_IDX_deviceIdNumber, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 31
  {HART_IDX, HART_IDX_deviceInstallDate, OBJECT, HART_NO, TD_USIGN24, HART_LEN_3}, // SIMPLE_U8
  //slotcode 32
  {HART_IDX, HART_IDX_measuringTask, OBJECT, HART_NO, TD_USIGN8, HART_LEN_24}, // SIMPLE_U8[0..23]
  //slotcode 34
  {HART_IDX, HART_IDX_longTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_32}, // SIMPLE_U8[0..31]
  //slotcode 38
  {HART_IDX, HART_IDX_countryCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // SIMPLE_U8[0..1]
  //slotcode 39
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_12}, // SIMPLE_CHAR[0..11]
  //slotcode 40
  {CDT_IDX, CDT_IDX_internalRTDPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 41
  {CDT_IDX, CDT_IDX_analogInputPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 42
  {CDT_IDX, CDT_IDX_energyFlowPassword, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_CHAR[0..5]
  //slotcode 43
  {HMI_IDX, HMI_IDX_SetPassword_StandardLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 44
  {HMI_IDX, HMI_IDX_SetPassword_AdvancedLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_6}, // SIMPLE_U8[0..5]
  //slotcode 45
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorLocationTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20}, // SIMPLE_CHAR[0..19]
  //slotcode 46
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensoTag, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // SIMPLE_CHAR[0..19]
};

const COM_REQ_RESP_CNT specObjCom171_ReqRespObjCnt[] = 
{
  { 2, 2, 2}    // slot 0: 2 REQ+RES Objects (2 Byte)
};

const COM_SPEC_TAB specObjCom171[] = 
{
  { specObjCom171_Req, specObjCom171_Resp, specObjCom171_ReqRespObjCnt, 1, specObjCom171_slotCodeObjDescrTab, 34, 1, specObjCom171_slotCodeTranslationTable}
};


/*
,--- read-command 172 -------------------------------------------------------
| ReadVariableAlarmRange
| Read Variable Alarm Range
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                 byte(s)     type        object (default)

 +------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +-------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +--->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,------------.
| SLOTCODE 0 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.loLim (0.0f)
   |    +--->   #4..#7   TD_FLOAT  SVMeasurement.QvAlmRange.hiLim (100.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 1 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.loLim (0.00f)
   |    +--->   #4..#7   TD_FLOAT  MVMeasurement.QAlmRange.hiLim (100.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 2 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_1 ()
   |    +--->   #4..#7   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_0 ()
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 3 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_1 ()
   |    +--->   #4..#7   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_0 ()
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


*/
const COM_OBJ_DESCR specObjCom172_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom172_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom172_slotCodeTranslationTable[] = {
0,1,2,3
};

const COM_OBJ_DESCR specObjCom172_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 1
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 2
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom172_ReqRespObjCnt[] = 
{
  { 1, 1, 3}    // slot 0: 1 REQUEST Objects (1 Byte) - 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom172[] = 
{
  { specObjCom172_Req, specObjCom172_Resp, specObjCom172_ReqRespObjCnt, 1, specObjCom172_slotCodeObjDescrTab, 3, 2, specObjCom172_slotCodeTranslationTable}
};


/*
,--- write-command 173 -------------------------------------------------------
| WriteVariableAlarmRange
| Write Variable Alarm Range
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                 byte(s)     type        object (default)

 +------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +-------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +--->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,----------.
| RESPONSE |
`----------´

                 byte(s)     type        object (default)

 +------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +-------->   #1..#4   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    +--->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,------------.
| SLOTCODE 0 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  SVMeasurement.QvAlmRange.loLim (0.0f)
   |    +--->   #4..#7   TD_FLOAT  SVMeasurement.QvAlmRange.hiLim (100.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 1 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  MVMeasurement.QAlmRange.loLim (0.00f)
   |    +--->   #4..#7   TD_FLOAT  MVMeasurement.QAlmRange.hiLim (100.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 2 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_1 ()
   |    +--->   #4..#7   TD_FLOAT  Coordinator.temperatureAlmRange_obj.value_0 ()
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 3 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_1 ()
   |    +--->   #4..#7   TD_FLOAT  Mapper.dvPressureAlmRangeObj.value_0 ()
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


*/
const COM_OBJ_DESCR specObjCom173_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom173_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom173_slotCodeTranslationTable[] = {
0,1,2,3
};

const COM_OBJ_DESCR specObjCom173_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  {SVM_IDX, SVM_IDX_QvAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 1
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_loLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.loLim
  {MVM_IDX, MVM_IDX_QAlmRange, ATTRIB, RANGEFLT_hiLim, TD_FLOAT, HART_LEN_4}, // RANGEFLT.hiLim
  //slotcode 2
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {CDT_IDX, CDT_IDX_temperatureAlmRange_obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  {MAPPER_IDX, MAPPER_IDX_dvPressureAlmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom173_ReqRespObjCnt[] = 
{
  { 9, 3, 3}    // slot 0: 3 REQ+RES Objects (9 Byte)
};

const COM_SPEC_TAB specObjCom173[] = 
{
  { specObjCom173_Req, specObjCom173_Resp, specObjCom173_ReqRespObjCnt, 1, specObjCom173_slotCodeObjDescrTab, 3, 2, specObjCom173_slotCodeTranslationTable}
};


/*
,--- read-command 174 -------------------------------------------------------
| ReadVariableWithUnit
| Read Variable With Unit
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 0 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperaturePreset_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperaturePreset_Obj.value_0 (0.0F)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 1 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtPresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtPresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 2 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressurePresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressurePresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 3 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityPresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 4 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentPreset (100.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 5 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.densigyRefObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 6 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.refTemperatureObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.refTemperatureObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 7 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.refPressureObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.refPressureObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 8 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  ElettronicServices.elettronicTemperatureSimulation_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  ElettronicServices.elettronicTemperatureSimulation_Obj.value_0 (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 9 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  ElettronicServices.elettronicTemperature_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  ElettronicServices.elettronicTemperature_Obj.value_0 (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 10 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.densigyRefObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR specObjCom174_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom174_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom174_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10
};

const COM_OBJ_DESCR specObjCom174_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 2
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 4
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 5
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 6
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {MVM_IDX, MVM_IDX_refPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_refPressureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 10
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom174_ReqRespObjCnt[] = 
{
  { 1, 1, 3}    // slot 0: 1 REQUEST Objects (1 Byte) - 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom174[] = 
{
  { specObjCom174_Req, specObjCom174_Resp, specObjCom174_ReqRespObjCnt, 1, specObjCom174_slotCodeObjDescrTab, 10, 2, specObjCom174_slotCodeTranslationTable}
};


/*
,--- write-command 175 -------------------------------------------------------
| WriteVariableWithUnit
| Write Variable With Unit
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 0 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperaturePreset_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperaturePreset_Obj.value_0 (0.0F)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 1 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtPresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtPresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 2 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressurePresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressurePresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 3 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityPresetObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityPresetObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 4 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentPreset (100.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 5 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.densigyRefObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 6 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.refTemperatureObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.refTemperatureObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 7 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.refPressureObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.refPressureObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 8 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  ElettronicServices.elettronicTemperatureSimulation_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  ElettronicServices.elettronicTemperatureSimulation_Obj.value_0 (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 9 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  ElettronicServices.elettronicTemperature_Obj.unit (CELSIUS)
 |   +--->   #1..#4   TD_FLOAT  ElettronicServices.elettronicTemperature_Obj.value_0 (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 10 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.densigyRefObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.densigyRefObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR specObjCom175_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom175_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom175_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10
};

const COM_OBJ_DESCR specObjCom175_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperaturePreset_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 2
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressurePresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityPresetObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 4
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentPreset, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  //slotcode 5
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 6
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_refTemperatureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {MVM_IDX, MVM_IDX_refPressureObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_refPressureObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_elettronicTemperature_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 10
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_densigyRefObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4} // UOM_PF.value_0
};

const COM_REQ_RESP_CNT specObjCom175_ReqRespObjCnt[] = 
{
  { 6, 3, 3}    // slot 0: 3 REQ+RES Objects (6 Byte)
};

const COM_SPEC_TAB specObjCom175[] = 
{
  { specObjCom175_Req, specObjCom175_Resp, specObjCom175_ReqRespObjCnt, 1, specObjCom175_slotCodeObjDescrTab, 10, 2, specObjCom175_slotCodeTranslationTable}
};


/*
,--- read-command 176 -------------------------------------------------------
| ReadSimulateDeviceVariable
| Read Simulate Device Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +--------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | | +------->       #2  TD_USIGN8  Hart.dummyU8 (224)
 | | |   +--->   #3..#6   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | | |   | 
+-+-+-+----+
| | | |    |
+-+-+-+----+
 0        6


,------------.
| SLOTCODE 0 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  SVMeasurement.QvSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  SVMeasurement.QvSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  SVMeasurement.QvSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 1 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QnSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QnSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QnSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QmSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QmSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QmSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 3 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QpSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QpSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QpSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 4 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QvPartialSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QvPartialSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QvPartialSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 5 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QnPartialSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QnPartialSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QnPartialSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 6 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Coordinator.frequencySim_Enable (SIMULATION_DISABLE)
 | +------->       #1    TD_UNIT  Coordinator.frequency_SimObj.unit (HERTZ)
 | |   +--->   #2..#5   TD_FLOAT  Coordinator.frequency_SimObj.value_0 (0.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 7 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Coordinator.temperatureSim_Enable (SIMULATION_DISABLE)
 | +------->       #1    TD_UNIT  Coordinator.temperatureSim_Obj.unit (CELSIUS)
 | |   +--->   #2..#5   TD_FLOAT  Coordinator.temperatureSim_Obj.value_0 (0.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 8 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvTExtSimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvTExtSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvTExtSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 9 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvPressureSimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvPressureSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvDensitySimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 10 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvDensitySimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvDensitySimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvDensitySimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 11 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvContentSimEnable (SIMU_DISABLE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvContentSim (1.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom176_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom176_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom176_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11
};

const COM_OBJ_DESCR specObjCom176_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequencySim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperatureSim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_dvContentSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};

const COM_REQ_RESP_CNT specObjCom176_ReqRespObjCnt[] = 
{
  { 1, 1, 4}    // slot 0: 1 REQUEST Objects (1 Byte) - 4 RESPOND Objects
};

const COM_SPEC_TAB specObjCom176[] = 
{
  { specObjCom176_Req, specObjCom176_Resp, specObjCom176_ReqRespObjCnt, 1, specObjCom176_slotCodeObjDescrTab, 11, 3, specObjCom176_slotCodeTranslationTable}
};


/*
,--- write-command 177 -------------------------------------------------------
| WriteSimulateDeviceVariable
| Write Simulate Device Variable
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +--------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | | +------->       #2  TD_USIGN8  Hart.dummyU8 (224)
 | | |   +--->   #3..#6   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | | |   | 
+-+-+-+----+
| | | |    |
+-+-+-+----+
 0        6


,----------.
| RESPONSE |
`----------´

                byte(s)     type        object (default)

 +----------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +--------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | | +------->       #2  TD_USIGN8  Hart.dummyU8 (224)
 | | |   +--->   #3..#6   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | | |   | 
+-+-+-+----+
| | | |    |
+-+-+-+----+
 0        6


,------------.
| SLOTCODE 0 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  SVMeasurement.QvSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  SVMeasurement.QvSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  SVMeasurement.QvSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 1 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QnSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QnSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QnSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 2 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QmSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QmSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QmSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 3 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QpSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QpSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QpSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 4 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QvPartialSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QvPartialSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QvPartialSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 5 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  MVMeasurement.QnPartialSimEnable (DISABLE)
 | +------->       #1    TD_UNIT  MVMeasurement.QnPartialSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  MVMeasurement.QnPartialSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 6 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Coordinator.frequencySim_Enable (SIMULATION_DISABLE)
 | +------->       #1    TD_UNIT  Coordinator.frequency_SimObj.unit (HERTZ)
 | |   +--->   #2..#5   TD_FLOAT  Coordinator.frequency_SimObj.value_0 (0.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 7 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Coordinator.temperatureSim_Enable (SIMULATION_DISABLE)
 | +------->       #1    TD_UNIT  Coordinator.temperatureSim_Obj.unit (CELSIUS)
 | |   +--->   #2..#5   TD_FLOAT  Coordinator.temperatureSim_Obj.value_0 (0.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 8 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvTExtSimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvTExtSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvTExtSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 9 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvPressureSimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvPressureSimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvPressureSimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 10 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvDensitySimEnable (SIMU_DISABLE)
 | +------->       #1    TD_UNIT  Mapper.dvDensitySimObj.unit ()
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvDensitySimObj.value_0 ()
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 11 |
`------------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Mapper.dvContentSimEnable (SIMU_DISABLE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Mapper.dvContentSim (1.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


*/
const COM_OBJ_DESCR specObjCom177_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom177_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom177_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11
};

const COM_OBJ_DESCR specObjCom177_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {MVM_IDX, MVM_IDX_QnSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 2
  {MVM_IDX, MVM_IDX_QmSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MVM_IDX, MVM_IDX_QpSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 4
  {MVM_IDX, MVM_IDX_QvPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 5
  {MVM_IDX, MVM_IDX_QnPartialSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 6
  {CDT_IDX, CDT_IDX_frequencySim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_frequency_SimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {CDT_IDX, CDT_IDX_temperatureSim_Enable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperatureSim_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureSimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 10
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensitySimObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 11
  {MAPPER_IDX, MAPPER_IDX_dvContentSimEnable, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentSim, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};

const COM_REQ_RESP_CNT specObjCom177_ReqRespObjCnt[] = 
{
  { 7, 4, 4}    // slot 0: 4 REQ+RES Objects (7 Byte)
};

const COM_SPEC_TAB specObjCom177[] = 
{
  { specObjCom177_Req, specObjCom177_Resp, specObjCom177_ReqRespObjCnt, 1, specObjCom177_slotCodeObjDescrTab, 11, 3, specObjCom177_slotCodeTranslationTable}
};


/*
,--- read-command 178 -------------------------------------------------------
| ReadVariableRange
| Read Variable Range
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 0 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  SVMeasurement.QvRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  SVMeasurement.QvRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 1 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  SVMeasurement.QvRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  SVMeasurement.QvRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 2 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 3 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 4 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QmRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QmRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 5 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QmRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QmRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 6 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QpRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QpRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 7 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QpRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QpRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 8 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QvPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 9 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QvPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 10 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 11 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 12 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 13 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 14 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 15 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 16 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 17 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 18 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentRange.hiLim (100.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 19 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentRange.loLim (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 20 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperature_range_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperature_range_Obj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 21 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperature_range_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperature_range_Obj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 22 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeRelativeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeRelativeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 23 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeRelativeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeRelativeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR specObjCom178_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom178_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const TUSIGN8 specObjCom178_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
};

const COM_OBJ_DESCR specObjCom178_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 2
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 4
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 5
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 6
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 8
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 10
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 11
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 12
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 13
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 14
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 15
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 16
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 17
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 18
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_hiLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.hiLim
  //slotcode 19
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_loLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.loLim
  //slotcode 20
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 21
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 22
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 23
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4} // UOM_PF.value_1
};

const COM_REQ_RESP_CNT specObjCom178_ReqRespObjCnt[] = 
{
  { 1, 1, 3}    // slot 0: 1 REQUEST Objects (1 Byte) - 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom178[] = 
{
  { specObjCom178_Req, specObjCom178_Resp, specObjCom178_ReqRespObjCnt, 1, specObjCom178_slotCodeObjDescrTab, 23, 2, specObjCom178_slotCodeTranslationTable}
};


/*
,--- write-command 179 -------------------------------------------------------
| WriteVariableRange
| Read Variable  Range
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,----------.
| RESPONSE |
`----------´

              byte(s)     type        object (default)

 +--------->       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 | +------->       #1  TD_USIGN8  Hart.dummyU8 (224)
 | |   +--->   #2..#5   TD_FLOAT  Hart.dummyFloat (-10.0f)
 | |   | 
+-+-+----+
| | |    |
+-+-+----+
 0      5


,------------.
| SLOTCODE 0 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  SVMeasurement.QvRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  SVMeasurement.QvRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 1 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  SVMeasurement.QvRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  SVMeasurement.QvRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 2 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 3 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 4 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QmRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QmRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 5 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QmRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QmRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 6 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QpRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QpRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 7 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QpRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QpRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 8 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QvPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 9 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QvPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QvPartialRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 10 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 11 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  MVMeasurement.QnPartialRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  MVMeasurement.QnPartialRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 12 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 13 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvTExtRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvTExtRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 14 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 15 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 16 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityRangeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 17 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvDensityRangeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvDensityRangeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 18 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentRange.hiLim (100.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 19 |
`------------´

            byte(s)     type        object (default)

 +------->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvContentRange.loLim (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 20 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperature_range_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperature_range_Obj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 21 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Coordinator.temperature_range_Obj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Coordinator.temperature_range_Obj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 22 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeRelativeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeRelativeObj.value_0 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,------------.
| SLOTCODE 23 |
`------------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Mapper.dvPressureRangeRelativeObj.unit ()
 |   +--->   #1..#4   TD_FLOAT  Mapper.dvPressureRangeRelativeObj.value_1 ()
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR specObjCom179_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom179_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom179_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
};

const COM_OBJ_DESCR specObjCom179_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 1
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {SVM_IDX, SVM_IDX_QvRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 2
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 3
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 4
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 5
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QmRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 6
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 7
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QpRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 8
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 9
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QvPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 10
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 11
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MVM_IDX, MVM_IDX_QnPartialRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 12
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 13
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvTExtRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 14
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 15
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 16
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 17
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvDensityRangeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 18
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_hiLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.hiLim
  //slotcode 19
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {MAPPER_IDX, MAPPER_IDX_dvContentRange, ATTRIB, DEV_RNGFLT_loLim, TD_FLOAT, HART_LEN_4}, // DEV_RNGFLT.loLim
  //slotcode 20
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 21
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {CDT_IDX, CDT_IDX_temperature_range_Obj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_1
  //slotcode 22
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_value_0, TD_FLOAT, HART_LEN_4}, // UOM_PF.value_0
  //slotcode 23
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_unit, TD_UNIT, HART_LEN_1}, // UOM_PF.unit
  {MAPPER_IDX, MAPPER_IDX_dvPressureRangeRelativeObj, ATTRIB, UOM_PF_value_1, TD_FLOAT, HART_LEN_4} // UOM_PF.value_1
};

const COM_REQ_RESP_CNT specObjCom179_ReqRespObjCnt[] = 
{
  { 6, 3, 3}    // slot 0: 3 REQ+RES Objects (6 Byte)
};

const COM_SPEC_TAB specObjCom179[] = 
{
  { specObjCom179_Req, specObjCom179_Resp, specObjCom179_ReqRespObjCnt, 1, specObjCom179_slotCodeObjDescrTab, 23, 2, specObjCom179_slotCodeTranslationTable}
};


/*
,--- read-command 180 -------------------------------------------------------
| ReadAiCurrent
| Read AI Current
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

   +--->   #0..#3   TD_FLOAT  AnalogInput.aiTempCurr (4.01f)
   | 
+----+
|    |
+----+
 0  3


*/
const COM_OBJ_DESCR objCom180[] = 
{
  {AI_IDX, AI_IDX_aiTempCurr, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // FLOAT
};


/*
,--- read-command 181 -------------------------------------------------------
| ReadBurstInConfiguration
| Read Burst In Configuration
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.burstUpTxSelection (SEC_WITHOUT)
 | +-->       #1  TD_USIGN8  Hart.burstUpUnitType (BURST_IN_PRESSURE)
 | |
+-+-+
| | |
+-+-+
 0 1


*/
const COM_OBJ_DESCR objCom181[] = 
{
  {HART_IDX, HART_IDX_burstUpTxSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_burstUpUnitType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- write-command 182 -------------------------------------------------------
| WriteBurstInConfiguration
| Config Burst In configuration
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.burstUpTxSelection (SEC_WITHOUT)
 | +-->       #1  TD_USIGN8  Hart.burstUpUnitType (BURST_IN_PRESSURE)
 | |
+-+-+
| | |
+-+-+
 0 1


,----------.
| RESPONSE |
`----------´

         byte(s)     type        object (default)

 +---->       #0  TD_USIGN8  Hart.burstUpTxSelection (SEC_WITHOUT)
 | +-->       #1  TD_USIGN8  Hart.burstUpUnitType (BURST_IN_PRESSURE)
 | |
+-+-+
| | |
+-+-+
 0 1


*/
const COM_OBJ_DESCR objCom182[] = 
{
  {HART_IDX, HART_IDX_burstUpTxSelection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_burstUpUnitType, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- write-command 183 -------------------------------------------------------
| BurstIn
| Burst In
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Hart.burstUpUnitCode[0] (4)
 |   +--->   #1..#4   TD_FLOAT  Hart.burstUpDataValue (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


,----------.
| RESPONSE |
`----------´

            byte(s)     type        object (default)

 +------->       #0    TD_UNIT  Hart.burstUpUnitCode[0] (4)
 |   +--->   #1..#4   TD_FLOAT  Hart.burstUpDataValue (0.0f)
 |   | 
+-+----+
| |    |
+-+----+
 0    4


*/
const COM_OBJ_DESCR objCom183[] = 
{
  {HART_IDX, HART_IDX_burstUpUnitCode, ATTRIB, 0, TD_UNIT, HART_LEN_1}, // SIMPLE_U8[0]
  {HART_IDX, HART_IDX_burstUpDataValue, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};


/*
,--- write-command 184 -------------------------------------------------------
| LoginwithPassword
| LoginwithPassword and accesslevel
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                            byte(s)     type        object (default)

 +----------------------->       #0  TD_USIGN8  Hart.HartAccessLevel (SERVICE_service)
 |           +----------->  #1..#20  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
 |           |         
+-+--------------------+
| |                    |
+-+--------------------+
 0                   20


,----------.
| RESPONSE |
`----------´

                            byte(s)     type        object (default)

 +----------------------->       #0  TD_USIGN8  Hart.HartAccessLevel (SERVICE_service)
 |           +----------->  #1..#20  TD_USIGN8  Hart.ServiceCode[0..19] (' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ')
 |           |         
+-+--------------------+
| |                    |
+-+--------------------+
 0                   20


*/
const COM_OBJ_DESCR specObjCom184_Resp[] = 
{
  {HART_IDX, HART_IDX_HartAccessLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_ServiceCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // HART_SERVICECODE[0..19]
};

const COM_OBJ_DESCR specObjCom184_Req[] = 
{
  {HART_IDX, HART_IDX_HartAccessLevel, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // TABENUM8
  {HART_IDX, HART_IDX_ServiceCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_20} // HART_SERVICECODE[0..19]
};

const COM_REQ_RESP_CNT specObjCom184_ReqRespObjCnt[] = 
{
  { 1, 1, 2},   // 1 REQUEST Objects (1 Byte), 2 RESPOND Objects
  {21, 2, 2}   // 2 REQUEST Objects (21 Byte), 2 RESPOND Objects
};

const COM_SPEC_TAB specObjCom184[] = 
{
  { specObjCom184_Req, specObjCom184_Resp, specObjCom184_ReqRespObjCnt, 2, NULL, 0, 0, NULL}
};


/*
,--- write-command 185 -------------------------------------------------------
| ResetSoftWriteProtection
| Reset Soft Write Protection
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.swWriteProtection (WRITE_PROTECTION_OFF)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  ElettronicServices.swWriteProtection (WRITE_PROTECTION_OFF)
 |
+-+
| |
+-+
 0


*/
const COM_OBJ_DESCR objCom185[] = 
{
  {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swWriteProtection, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // TABENUM8
};


/*
,--- read-command 186 -------------------------------------------------------
| ReadTotalizerBaseUnitValue
| Read Totalizer Base Unit Value
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

                 byte(s)     type        object (default)

 +------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +-------->   #1..#4 TD_USIGN32  Hart.dummy32 (123456789)
 |   |    +--->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,----------.
| RESPONSE |
`----------´

                 byte(s)     type        object (default)

 +------------>       #0  TD_USIGN8  Hart.slot0 (SLOT_CODE_NONE)
 |   +-------->   #1..#4 TD_USIGN32  Hart.dummy32 (123456789)
 |   |    +--->   #5..#8   TD_FLOAT  Hart.dummyFloat (-10.0f)
 |   |    | 
+-+----+----+
| |    |    |
+-+----+----+
 0         8


,------------.
| SLOTCODE 0 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQvFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 1 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQnFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 2 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQmFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQmFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 3 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totEnergyFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totEnergyFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 4 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvPartialFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQvPartialFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 5 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnPartialFloatAcc.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQnPartialFloatAcc.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 6 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 7 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 8 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQmOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 9 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totEnergyOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 10 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvPartialOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 11 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnPartialOverrollCnt (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totOverrollBase (10000000.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 12 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQvFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 13 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQnFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 14 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQmFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQmFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 15 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totEnergyFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totEnergyFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 16 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQvPartialFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQvPartialFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


,------------.
| SLOTCODE 17 |
`------------´

               byte(s)     type        object (default)

   +-------->   #0..#3 TD_USIGN32  Totalizer.totQnPartialFloatAccUserUnit.overRollCounter (0)
   |    +--->   #4..#7   TD_FLOAT  Totalizer.totQnPartialFloatAccUserUnit.accFloatValue (0.0f)
   |    | 
+----+----+
|    |    |
+----+----+
 0       7


*/
const COM_OBJ_DESCR specObjCom186_Resp[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const COM_OBJ_DESCR specObjCom186_Req[] = 
{
  {HART_IDX, HART_IDX_slot0, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1}, // SIMPLE_U8
  {HART_IDX, HART_IDX_dummy32, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {HART_IDX, HART_IDX_dummyFloat, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4} // SIMPLE_FLOAT
};

const TUSIGN8 specObjCom186_slotCodeTranslationTable[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
};

const COM_OBJ_DESCR specObjCom186_slotCodeObjDescrTab[] = 
{
  //slotcode 0
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 1
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 2
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 3
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 4
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 5
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatAcc, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 6
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 7
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 8
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 9
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 10
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 11
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialOverrollCnt, OBJECT, HART_NO, TD_USIGN32, HART_LEN_4}, // SIMPLE_U32
  {TOTALIZER_IDX, TOTALIZER_IDX_totOverrollBase, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  //slotcode 12
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 13
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 14
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQmFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 15
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totEnergyFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 16
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQvPartialFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4}, // TOTACCFLOATDATAOBJ.accFloatValue
  //slotcode 17
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_overRollCounter, TD_USIGN32, HART_LEN_4}, // TOTACCFLOATDATAOBJ.overRollCounter
  {TOTALIZER_IDX, TOTALIZER_IDX_totQnPartialFloatAccUserUnit, ATTRIB, TOTACCFLOATDATAOBJ_accFloatValue, TD_FLOAT, HART_LEN_4} // TOTACCFLOATDATAOBJ.accFloatValue
};

const COM_REQ_RESP_CNT specObjCom186_ReqRespObjCnt[] = 
{
  { 1, 1, 3}    // slot 0: 1 REQUEST Objects (1 Byte) - 3 RESPOND Objects
};

const COM_SPEC_TAB specObjCom186[] = 
{
  { specObjCom186_Req, specObjCom186_Resp, specObjCom186_ReqRespObjCnt, 1, specObjCom186_slotCodeObjDescrTab, 17, 2, specObjCom186_slotCodeTranslationTable}
};


/*
,--- read-command 200 -------------------------------------------------------
| ReadSensorInformation
| 
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

       byte(s)     type        object (default)

 +-->       #0  TD_USIGN8  Hart.dummyU8 (224)
 |
+-+
| |
+-+
 0


,----------.
| RESPONSE |
`----------´

                             byte(s)     type        object (default)

   +---------------------->   #0..#3   TD_FLOAT  Coordinator.frequency (0.0f)
   |    +----------------->   #4..#7   TD_FLOAT  SVMeasurement.Qv (0.00f)
   |    |    +------------>  #8..#11   TD_FLOAT  Coordinator.temperature (0.0f)
   |    |    |   +--------> #12..#13 TD_USIGN16  Coordinator.sigMagnitude (0)
   |    |    |   |  +-----> #14..#15 TD_USIGN16  Coordinator.sigAmplitude (0)
   |    |    |   |  |  +--> #16..#17 TD_USIGN16  Coordinator.vbrAmplitude (0)
   |    |    |   |  |  |
+----+----+----+--+--+--+
|    |    |    |  |  |  |
+----+----+----+--+--+--+
 0                    17


*/
const COM_OBJ_DESCR specObjCom200_Resp[] = 
{
  {CDT_IDX, CDT_IDX_frequency, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // FLOAT
  {SVM_IDX, SVM_IDX_Qv, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {CDT_IDX, CDT_IDX_temperature, OBJECT, HART_NO, TD_FLOAT, HART_LEN_4}, // SIMPLE_FLOAT
  {CDT_IDX, CDT_IDX_sigMagnitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {CDT_IDX, CDT_IDX_sigAmplitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2}, // SIMPLE_U16
  {CDT_IDX, CDT_IDX_vbrAmplitude, OBJECT, HART_NO, TD_USIGN16, HART_LEN_2} // SIMPLE_U16
};

const COM_OBJ_DESCR specObjCom200_Req[] = 
{
  {HART_IDX, HART_IDX_dummyU8, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

const COM_REQ_RESP_CNT specObjCom200_ReqRespObjCnt[] = 
{
  { 1, 1, 6}   // 1 REQUEST Objects (1 Byte), 6 RESPOND Objects
};

const COM_SPEC_TAB specObjCom200[] = 
{
  { specObjCom200_Req, specObjCom200_Resp, specObjCom200_ReqRespObjCnt, 1, NULL, 0, 0, NULL}
};


/*
,--- read-command 512 -------------------------------------------------------
| ReadCountryCode
| Read Country Code
`---------------------------------------------------------------------------

,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

  +---->   #0..#1  TD_USIGN8  Hart.countryCode[0..1] (67,78)
  | +-->       #2  TD_USIGN8  Hart.SiUnitOnly (0)
  | |
+--+-+
|  | |
+--+-+
 0  2


*/
const COM_OBJ_DESCR objCom512[] = 
{
  {HART_IDX, HART_IDX_countryCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // SIMPLE_U8[0..1]
  {HART_IDX, HART_IDX_SiUnitOnly, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};


/*
,--- write-command 513 -------------------------------------------------------
| WriteCountryCode
| Write Country Code
`---------------------------------------------------------------------------

,---------.
| REQUEST |
`---------´

          byte(s)     type        object (default)

  +---->   #0..#1  TD_USIGN8  Hart.countryCode[0..1] (67,78)
  | +-->       #2  TD_USIGN8  Hart.SiUnitOnly (0)
  | |
+--+-+
|  | |
+--+-+
 0  2


,----------.
| RESPONSE |
`----------´

          byte(s)     type        object (default)

  +---->   #0..#1  TD_USIGN8  Hart.countryCode[0..1] (67,78)
  | +-->       #2  TD_USIGN8  Hart.SiUnitOnly (0)
  | |
+--+-+
|  | |
+--+-+
 0  2


*/
const COM_OBJ_DESCR objCom513[] = 
{
  {HART_IDX, HART_IDX_countryCode, OBJECT, HART_NO, TD_USIGN8, HART_LEN_2}, // SIMPLE_U8[0..1]
  {HART_IDX, HART_IDX_SiUnitOnly, OBJECT, HART_NO, TD_USIGN8, HART_LEN_1} // SIMPLE_U8
};

//@HARTGen end@<COMMANDDEFINITION> 

 
const COMMAND commands[] = {
  //Command Number, 
  //Function to call, 
  //max number of Objects in response, 
  //min number of req data bytes, 
  //configuration changed flag, 
  //pointer to command structure
  //@HARTGen start@  <COMMANDS> 
  {    0,          HART_READ, 17,  0,  CONF_NO, objCom0}, // ReadUniqueIdentifier
  {    1,          HART_READ,  2,  0,  CONF_NO, objCom1}, // ReadPrimaryVariable
  {    2,          HART_READ,  2,  0,  CONF_NO, objCom2}, // ReadLoopCurrentAndPercentOfRange
  {    3,          HART_READ,  9,  0,  CONF_NO, objCom3}, // ReadDynamicVariablesAndLoopCurrent
  {    6, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom6}, // WritePollingAdress
  {    7,          HART_READ,  2,  0,  CONF_NO, objCom7}, // ReadLoopConfiguration
  {    8,          HART_READ,  4,  0,  CONF_NO, objCom8}, // ReadDynamicVariableClassification
  {    9,  HART_SPECIAL_READ, 42,  1,  CONF_NO, (void const SLOW *) specObjCom9}, // ReadDeviceVariablesWithStatus
  {   11,  HART_SPECIAL_READ, 17,  6,  CONF_NO, (void const SLOW *) specObjCom11}, // ReadUniqueIdentifierAssiciatedWithTag
  {   12,          HART_READ,  1,  0,  CONF_NO, objCom12}, // ReadMessage
  {   13,          HART_READ,  3,  0,  CONF_NO, objCom13}, // ReadTagDescriptorDate
  {   14,          HART_READ,  5,  0,  CONF_NO, objCom14}, // ReadPrimaryVariableTransducerInformation
  {   15,          HART_READ,  9,  0,  CONF_NO, objCom15}, // ReadDeviceInformation
  {   16,          HART_READ,  1,  0,  CONF_NO, objCom16}, // ReadFinalAssemblyNumber
  {   17,         HART_WRITE,  1, 24, CONF_YES, objCom17}, // WriteMessage
  {   18,         HART_WRITE,  3, 21, CONF_YES, objCom18}, // WriteTagDescriptorDate
  {   19,         HART_WRITE,  1,  3, CONF_YES, objCom19}, // WriteFinalAssemblyNumber
  {   20,          HART_READ,  1,  0,  CONF_NO, objCom20}, // ReadLongTag
  {   21,  HART_SPECIAL_READ, 17, 32,  CONF_NO, (void const SLOW *) specObjCom21}, // ReadUniqueIdentifierAssiciatedWithLongTag
  {   22,         HART_WRITE,  1, 32, CONF_YES, objCom22}, // WriteLongTag
  {   33,  HART_SPECIAL_READ, 12,  1,  CONF_NO, (void const SLOW *) specObjCom33}, // ReadDeviceVariables
  {   34,         HART_WRITE,  1,  4, CONF_YES, objCom34}, // WritePrimaryVariableDampingValue
  {   35,         HART_WRITE,  3,  9, CONF_YES, objCom35}, // WritePrimaryVariableRangeValues
  {   36,         HART_WRITE,  1,  0,  CONF_NO, objCom36}, // SetPrimaryVariableUpperRangeValue
  {   37,         HART_WRITE,  1,  0,  CONF_NO, objCom37}, // SetPrimaryVariableUpperLowerValue
  {   38, HART_SPECIAL_WRITE,  1,  2,  CONF_NO, (void const SLOW *) specObjCom38}, // ResetConfigurationChangedFlag
  {   40,         HART_WRITE,  1,  4,  CONF_NO, objCom40}, // EnterExitFixedCurrentMode
  {   41,         HART_WRITE,  1,  0,  CONF_NO, objCom41}, // PerformSelfTest
  {   42,         HART_WRITE,  1,  0,  CONF_NO, objCom42}, // PerformDeviceReset
  {   43,         HART_WRITE,  1,  0, CONF_YES, objCom43}, // WritePrimaryVariableZero
  {   44,         HART_WRITE,  1,  1, CONF_YES, objCom44}, // WritePrimaryVariableUnits
  {   45,         HART_WRITE,  1,  4, CONF_YES, objCom45}, // TrimloopCurrentZero
  {   46,         HART_WRITE,  1,  4, CONF_YES, objCom46}, // TrimloopCurrentGain
  {   48,  HART_SPECIAL_READ, 14,  6,  CONF_NO, (void const SLOW *) specObjCom48}, // ReadAdditionalDeviceStatus
  {   50,          HART_READ,  4,  0,  CONF_NO, objCom50}, // ReadDynamicVariablesAssignment
  {   51,         HART_WRITE,  4,  4, CONF_YES, objCom51}, // WriteDynamicVariablesAssignment
  {   53, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom53}, // WriteDeviceVariableUnits
  {   54,  HART_SPECIAL_READ, 10,  1,  CONF_NO, (void const SLOW *) specObjCom54}, // ReadDeviceVariableInformation
  {   55, HART_SPECIAL_WRITE,  2,  5, CONF_YES, (void const SLOW *) specObjCom55}, // WriteDeviceVariableDampingValue
  {   59,         HART_WRITE,  1,  1, CONF_YES, objCom59}, // WriteNumberofResponsePreambles
  {   71,         HART_WRITE,  1,  1,  CONF_NO, objCom71}, // LockDevice
  {   76,          HART_READ,  1,  0,  CONF_NO, objCom76}, // ReadLockDeviceState
  {   79, HART_SPECIAL_WRITE,  5,  8, CONF_YES, (void const SLOW *) specObjCom79}, // WriteDeviceVariable
  {  122,         HART_WRITE,  1, 20, CONF_YES, objCom122}, // WriteHartServiceCode
  {  123,  HART_SPECIAL_READ,  7,  8,  CONF_NO, (void const SLOW *) specObjCom123}, // ReadObject
  {  124,         HART_WRITE,  7, 41, CONF_YES, objCom124}, // WriteObject
  {  125,  HART_SPECIAL_READ,  5,  5,  CONF_NO, (void const SLOW *) specObjCom125}, // ReadMemory
  {  126,         HART_WRITE,  5, 37, CONF_YES, objCom126}, // WriteMemory
  {  128,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom128}, // ReadCalculationUnsignedCharVariable
  {  129, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom129}, // WriteCalculationUnsignedCharVariable
  {  130,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom130}, // ReadIOBUSUnsignedCharVariable
  {  131, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom131}, // WriteIOBUSUnsignedCharVariable
  {  132,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom132}, // ReadESUnsignedCharVariable
  {  133, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom133}, // WriteESUnsignedCharVariable
  {  134,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom134}, // ReadHMIUnsignedCharVariable
  {  135, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom135}, // WriteHMIUnsignedCharVariable
  {  136,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom136}, // ReadUnsignedShortVariable
  {  137, HART_SPECIAL_WRITE,  2,  3, CONF_YES, (void const SLOW *) specObjCom137}, // Write Unsigned Short Variable
  {  138,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom138}, // ReadUnsignedLongVariable
  {  139, HART_SPECIAL_WRITE,  2,  5, CONF_YES, (void const SLOW *) specObjCom139}, // WriteUnsignedLongVariable
  {  140,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom140}, // ReadFloatVariable
  {  141, HART_SPECIAL_WRITE,  2,  5, CONF_YES, (void const SLOW *) specObjCom141}, // WriteFloatVariable
  {  142,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom142}, // ReadDoubleVariable
  {  143, HART_SPECIAL_WRITE,  2,  9, CONF_YES, (void const SLOW *) specObjCom143}, // WriteDoubleVariable
  {  144,         HART_WRITE,  1,  0,  CONF_NO, objCom144}, // LogOutHartServiceCode
  {  146, HART_SPECIAL_WRITE,  1,  1, CONF_YES, (void const SLOW *) specObjCom146}, // PerformAction
  {  150,          HART_READ,  1,  0,  CONF_NO, objCom150}, // ReadDiagnosisActiveAlarm
  {  151,  HART_SPECIAL_READ, 10,  1,  CONF_NO, (void const SLOW *) specObjCom151}, // ReadDiagnosisActiveAlarmConditionDetail
  {  152,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom152}, // ReadFlowStatus
  {  153,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom153}, // ReadFamilyRevision
  {  154,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom154}, // ReadVortexFrequency
  {  155,          HART_READ,  1,  0,  CONF_NO, objCom155}, // ReadLowFlowCutoff
  {  156,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom156}, // ReadDensityPreset
  {  157,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom157}, // ReadDensityPresetUnit
  {  158,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom158}, // ReadViscosity
  {  159,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom159}, // ReadTubeDiameterSize
  {  160,         HART_WRITE,  1,  4, CONF_YES, objCom160}, // WriteLowFlowCutoff
  {  164,         HART_WRITE,  1,  4, CONF_YES, objCom164}, // WriteTubeDiameterSize
  {  165,         HART_WRITE,  1, 20, CONF_YES, objCom165}, // WriteHartServiceCode
  {  166,  HART_SPECIAL_READ, 10,  1,  CONF_NO, (void const SLOW *) specObjCom166}, // ReadFloats
  {  167, HART_SPECIAL_WRITE, 10, 37, CONF_YES, (void const SLOW *) specObjCom167}, // WriteFloats
  {  168,          HART_READ,  1,  0,  CONF_NO, objCom168}, // ReadGasConfFlag
  {  169,         HART_WRITE,  1,  1, CONF_YES, objCom169}, // WriteGasConfFlag
  {  170,  HART_SPECIAL_READ,  2,  1,  CONF_NO, (void const SLOW *) specObjCom170}, // ReadString
  {  171, HART_SPECIAL_WRITE,  2,  2, CONF_YES, (void const SLOW *) specObjCom171}, // WriteString
  {  172,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom172}, // ReadVariableAlarmRange
  {  173, HART_SPECIAL_WRITE,  3,  9, CONF_YES, (void const SLOW *) specObjCom173}, // WriteVariableAlarmRange
  {  174,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom174}, // ReadVariableWithUnit
  {  175, HART_SPECIAL_WRITE,  3,  6, CONF_YES, (void const SLOW *) specObjCom175}, // WriteVariableWithUnit
  {  176,  HART_SPECIAL_READ,  4,  1,  CONF_NO, (void const SLOW *) specObjCom176}, // ReadSimulateDeviceVariable
  {  177, HART_SPECIAL_WRITE,  4,  7, CONF_YES, (void const SLOW *) specObjCom177}, // WriteSimulateDeviceVariable
  {  178,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom178}, // ReadVariableRange
  {  179, HART_SPECIAL_WRITE,  3,  6, CONF_YES, (void const SLOW *) specObjCom179}, // WriteVariableRange
  {  180,          HART_READ,  1,  0,  CONF_NO, objCom180}, // ReadAiCurrent
  {  181,          HART_READ,  2,  0,  CONF_NO, objCom181}, // ReadBurstInConfiguration
  {  182,         HART_WRITE,  2,  2, CONF_YES, objCom182}, // WriteBurstInConfiguration
  {  183,         HART_WRITE,  2,  5,  CONF_NO, objCom183}, // BurstIn
  {  184, HART_SPECIAL_WRITE,  2, 21, CONF_YES, (void const SLOW *) specObjCom184}, // LoginwithPassword
  {  185,         HART_WRITE,  1,  1,  CONF_NO, objCom185}, // ResetSoftWriteProtection
  {  186,  HART_SPECIAL_READ,  3,  1,  CONF_NO, (void const SLOW *) specObjCom186}, // ReadTotalizerBaseUnitValue
  {  200,  HART_SPECIAL_READ,  6,  1,  CONF_NO, (void const SLOW *) specObjCom200}, // ReadSensorInformation
  {  512,          HART_READ,  2,  0,  CONF_NO, objCom512}, // ReadCountryCode
  {  513,         HART_WRITE,  2,  3, CONF_YES, objCom513} // WriteCountryCode
  //@HARTGen end@<COMMANDS> 
};

const TUSIGN16 lengthOfCommandTable = sizeof(commands)/sizeof(COMMAND);

