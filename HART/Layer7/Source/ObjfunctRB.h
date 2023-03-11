//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    General defines and type definitions used in HART layer 7

 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h must be include before ObjfunctRB.h
#endif

#ifndef __LAYER2_H__
  #error layer2.h must be included before ObjfunctRB.h
#endif

#ifdef __OBJ_FUNCT_RB_H__
  #error ObjfunctRB.h included more than once
#endif

#define __OBJ_FUNCT_RB_H__

/*--------------------------------------------------------------------------------------------------
r# HART length codes, max 16 possible
--------------------------------------------------------------------------------------------------*/
#define HART_LEN_0      0
#define HART_LEN_1      1
#define HART_LEN_2      2
#define HART_LEN_3      3
#define HART_LEN_4      4
#define HART_LEN_5      5
#define HART_LEN_6      6
#define HART_LEN_8      7
#define HART_LEN_9      8
#define HART_LEN_12     9
#define HART_LEN_15     10
#define HART_LEN_16     11
#define HART_LEN_20     12
#define HART_LEN_24     13
#define HART_LEN_28     14
#define HART_LEN_32     15


/*--------------------------------------------------------------------------------------------------
r# HART data types (dataTypeId)
--------------------------------------------------------------------------------------------------*/
#define TD_USIGN8        0
#define TD_BOOL          0
#define TD_INT8          0
#define TD_USIGN16       1
#define TD_INT16         1
#define TD_USIGN24       2
#define TD_USIGN32       3
#define TD_INT32         3
#define TD_FLOAT         3
#define TD_PASC          4
#define TD_T_RANGE_FLT_U 6
#define TD_PASC_ASC      7
//Special TD_USIGN32 for mapping to old TZIDC
#define TD_SPECIAL_U32   8
//Added for Unit Convern between HART-Unit and System Unit
#define TD_UNIT   9
#define TD_UNITTYPE   10
#define TD_DOUBLE     11

/*--------------------------------------------------------------------------------------------------
r# HART data types length
--------------------------------------------------------------------------------------------------*/
#define ONE_BYTES         0
#define TWO_BYTES         1
#define THREE_BYTES       2
#define FOUR_BYTES        3
#define SIX_BYTES         4
#define EIGHT_BYTES       5
#define NINE_BYTES        6


/*--------------------------------------------------------------------------------------------------
r# Access parameter type definition
--------------------------------------------------------------------------------------------------*/
#define OBJECT       0
#define ATTRIB       1

/*--------------------------------------------------------------------------------------------------
r# Access direction definition
--------------------------------------------------------------------------------------------------*/
#define HART_READ           0
#define HART_WRITE          1
#define HART_SPECIAL_READ   2
#define HART_SPECIAL_WRITE  3

/*--------------------------------------------------------------------------------------------------
r# Configuration changed flag set when writing this command
--------------------------------------------------------------------------------------------------*/
#define CONF_YES    eTRUE
#define CONF_NO     eFALSE

/*--------------------------------------------------------------------------------------------------
r# Subindex if basic data type
--------------------------------------------------------------------------------------------------*/
#define HART_NO     0

/*--------------------------------------------------------------------------------------------------
r# Subindex access
--------------------------------------------------------------------------------------------------*/
#define IND_0   0
#define IND_1   1
#define IND_2   2
#define IND_3   3
#define IND_4   4
#define IND_5   5
#define IND_6   6

/*--------------------------------------------------------------------------------------------------
r# Second response byte
--------------------------------------------------------------------------------------------------*/
#define DEVICE_MALFUNCTION         0x80         // Field Device Malfunction
#define CONFIGURATION_CHANGED      0x40         // Configuration Changed
#define COLD_START                 0x20         // Cold Start
#define MORE_STATUS_AVAILABLE      0x10         // More Status Available
#define PV_AO_FIXED                0x08         // Primary Variable Analog Output Fixed
#define PV_AO_SATURATED            0x04         // Primary Variable Analog Output Saturated
#define NON_PV_OUT_OF_LIMITS       0x02         // Non-Primary Variable Out of Limits
#define PV_OUT_OF_LIMITS           0x01         // Primary Variable Out of Limits
#define NONE                       0x00         // NONE

/*--------------------------------------------------------------------------------------------------
r# First response byte
--------------------------------------------------------------------------------------------------*/
enum
{
    HART_RESP_OK,                                       // #0 Success - no command specific error
    HART_RESP_ERR_RESERVED_1,                           // #1 Reserved
    HART_RESP_ERR_INVALID_SELECTION,                    // #2 Invalid selection
    HART_RESP_ERR_DATA_TOO_LARGE,                       // #3 Passed parameter too large
    HART_RESP_ERR_DATA_TOO_SMALL,                       // #4 Passed parameter too small
    HART_RESP_ERR_TOO_FEW_DATA,                         // #5 Too few data bytes receive
    HART_RESP_ERR_SPECIFIC_CMD_ERROR = 6,               // #6 Transmitter specific command error
    HART_RESP_ERR_DEVICE_SPECIFIC_CMD_ERROR = 6,        // #6 Device-Specific Command Error
    HART_RESP_ERR_WRITE_PROTECTED,                      // #7 Write protected
    HART_RESP_WARN_SET_TO_NEAREST_VALUE = 8,            // #8 Warning: set to nearest possible value
    HART_RESP_WARN_BURST_CONDITION_CONFLICT = 8,        // #8 Warning: burst condition conflict
    HART_RESP_ERR_LOWER_RANGE_TOO_HIGH = 9,             // #9 Lower range value too high
    HART_RESP_ERR_CONFIG_CHANGE_COUNTER_MISMATCH = 9,   // #9 Configuration change counter mismatch
    HART_RESP_ERR_INVALID_BURST_MESSAGE = 9,            // #9 Invalid Burst Message
    HART_RESP_ERR_INVALID_DATE_CODE_DETECTED = 9,       // #9 Invalid Date Code Detected
    HART_RESP_ERR_INVALID_LOCK_CODE,                    // #10 Invalid Lock Code
    HART_RESP_ERR_LOWER_RANGE_TOO_LOW = 10,             // #10 Lower range value too low
    HART_RESP_ERR_UPPER_RANGE_TOO_HIGH = 11,            // #11 Upper range value too high
    HART_RESP_ERR_DEVICE_IN_MULTI_DROP = 11,            // #11 Device was set to multi drop mode
    HART_RESP_ERR_INVALID_DEVICE_VARIABLE_CLASS = 11,   // #11 Invalid device variable classification
    HART_RESP_ERR_CANNOT_LOCK_DEVICE = 11,             // #11 Cannot Lock device
    HART_RESP_ERR_UPPER_RANGE_TOO_LOW = 12,             // #12 Upper range value too low
    HART_RESP_ERR_INVALD_UNITS_CODE = 12,               // #13 Invalid units code
    HART_RESP_ERR_UPPER_LOWER_RANGE_OUT_OF_LIM = 13,    // #13 span too small
    HART_RESP_ERR_INVALID_BRST_TRIG_MODE_SEL_CODE = 13, // #13 ivalid burst trigger mode selection code
    HART_RESP_WARN_SPAN_TOO_SMALL = 14,                 // #14 Warning: span too small
    HART_RESP_WARN_STATUS_BYTES_MISMATCH = 14,          // #14 Warning: status bytes mismatch                                                   //
    HART_RESP_ERR_RESERVED_15,                          // #15 Reserved
    HART_RESP_ERR_ACCESS_RESTRICTED,                    // #16 Access restricted
    HART_RESP_ERR_RESERVED_17,                          // #17 Reserved
    HART_RESP_ERR_RESERVED_18,                          // #18 Reserved
    HART_RESP_ERR_RESERVED_19,                          // #19 Reserved
    HART_RESP_ERR_INVALID_EXT_CMD_NUMBER,               // #20 Invalid extended command number
    HART_RESP_ERR_RESERVED_21,                          // #21 Reserved
    HART_RESP_ERR_RESERVED_22,                          // #22 Reserved
    HART_RESP_ERR_RESERVED_23,                          // #23 Reserved
    HART_RESP_ERR_INVALID_SPAN = 29,                    // #29 Invalid span error

    HART_RESP_ERR_DEVICE_BUSY         = 32,             // #32 Device is busy

    HART_RESP_ERR_CMD_NOT_IMPLEMENTED = 64,             // #64 Command not implemented
    HART_RESP_ERR_SET_TO_NEAREST_VALUE = 66,            // #66 Reserved: Set to nearest possible value error
    HART_RESP_ERR_SPAN_TOO_SMALL = 72,                  // #72 Reserved: Span too small error

    HART_RESP_WARN_LOWER_RANGE_TOO_LOW = 112,           // #112 Reserved: Lower range value too low
    HART_RESP_WARN_UPPER_RANGE_TOO_HIGH,                // #113 Reserved: Upper range value too high
    HART_RESP_WARN_ACCESS_RESTRICTED,                   // #114 Reserved: Wrong state for cmd
    HART_RESP_WARN_INVALID_SELECTION                    // #115 Reserved: Code or index not allowed for cmd
};

//--------------------------------------------------------------------------------------------------
//! Defines Coldstart flag
#define HART_MASTER_COLDSTART_FLAG_1    0x01    // Coldstart-Flag of Primary Master first Layer 2
#define HART_SECMAS_COLDSTART_FLAG_1    0x02    // Coldstart-Flag of Secondary Master first Layer 2
#define HART_MASTER_COLDSTART_FLAG_2    0x04    // Coldstart-Flag of Primary Master second Layer 2
#define HART_SECMAS_COLDSTART_FLAG_2    0x08    // Coldstart-Flag of Secondary Master second Layer 2

/*--------------------------------------------------------------------------------------------------
r# Object description - 3 Byte per command attribute!
--------------------------------------------------------------------------------------------------*/
//lint -e46 Marcel Schilg 2007-10-25 (Field type is not int or unsigned, ok for this compiler)
typedef struct
{
   TUSIGN16         subsystemId:7;       /* Subsystem ID max 128 */
   TUSIGN16         objId:8;             /* DO NOT CHANGE THIS VALUE  Object ID max 256 */
   TUSIGN16         attribId:1;          /* Attribute yes/no  0 = No Attribut*/
   TUSIGN8          attribIdx;           /* Attribute index */
   TUSIGN8          dataTypeId:4;        /* Indicates the data type used */
   TUSIGN8          hartLenCode:4;       /* Object length code in HART command for buffer pointer max 15 */
} COM_OBJ_DESCR;
// +e46
/*--------------------------------------------------------------------------------------------------
r# Number of request/response objects in dependence of request data bytes
--------------------------------------------------------------------------------------------------*/
typedef struct
{
    TUSIGN8 noReqDatabytes;      /* Number of received request data bytes */
    TUSIGN8 noReqObjects;        /* Number of received objects (incl. slot objects) */
    TUSIGN8 noRespObjects;       /* Number of response objects (incl. slot objects) */
} COM_REQ_RESP_CNT;

/*--------------------------------------------------------------------------------------------------
r# Pointer to special HART command descriptions
--------------------------------------------------------------------------------------------------*/
typedef struct
{
    COM_OBJ_DESCR const SLOW    *cmdDefReqDescr;            /* Pointer to default request command description */
    COM_OBJ_DESCR const SLOW    *cmdDefRespDescr;           /* Pointer to default response command description */
    COM_REQ_RESP_CNT const SLOW *reqRespObjCnt;             /* Pointer to table of response/request object counts */
    TUSIGN8                     lenOfRespObjCntTab;         /* Length of response/request object counts table */
    COM_OBJ_DESCR const SLOW    *slotCodeObjDescr;          /* Pointer to table of slot code spec. object descr. */
    TUSIGN8                     maxNoOfSlotCodes;           /* Maximum number of slot codes */
    TUSIGN8                     noOfSlotObjects;            /* Number of object descriptions for every slot code */
    TUSIGN8 const SLOW          *slotCodeTranslationTable;  /* Table to do indexing for every slot code */
} COM_SPEC_TAB;

/*--------------------------------------------------------------------------------------------------
r# Command description
--------------------------------------------------------------------------------------------------*/
typedef struct
{
    TUSIGN16 cmdNumber;              /* Command number */
    TUSIGN8  fct             :2;     /* Function to call normal or special read/write */
    TUSIGN8  objectCount     :6;     /* max. number of objects in response (and request in write commands) */
    TUSIGN8  requestDataLength :7;   /* Minimum number of command request data bytes */
    TUSIGN8  configChanged   :1;     /* Determines whether or not config changed bit will be set */
    COM_OBJ_DESCR const SLOW *cmdDescr;  /* Pointer to command structure  */
} COMMAND;

/*--------------------------------------------------------------------------------------------------
r# Type conversion description
--------------------------------------------------------------------------------------------------*/
typedef struct
{
   TUSIGN8 dataTypeId:4;
   TUSIGN8 lenCode:4;
} T_TYPE_CONV;

/*--------------------------------------------------------------------------------------------------
r# Definition of NULL pointer
--------------------------------------------------------------------------------------------------*/
#ifndef NULL
#define NULL  (void *) 0
#endif


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HART_ConfigurationChange: configuration change handling.
 \author    Bernhard Tohermes
 \date      2010-07-01
 \param
 \param
 \return
 \warning
*/
//--------------------------------------------------------------------------------------------------

extern void HART_ConfigurationChange(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTSpecialReadWrite: Handles special Read/Write HART commands.
 \author    Horst Seele
 \date      2005-11-18
 \param     commandNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern void HARTSpecialReadWriteObjects(TUSIGN16 commandNumberIndex, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Reads an object or attribute from a subsystem
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param     cmdDescr = Pointer to command structure
 \param     objectCount = Number of objects in response
 \param     command number in transmit buffer
 \param     contained in 'commands' table
 \return    command response in transmit buffer
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern void HARTReadObjects(COM_OBJ_DESCR const SLOW *cmdDescr, TUSIGN8 objectCount, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Writes all objects and attributes contained in one HART command
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param     cmdDescr = Pointer to command structure
 \param     objectCount = Number of objects in response
 \param     configChanged = Indicates if command changes configuration or not
 \param     command number in transmit buffer
 \param     data in receive buffer
 \param     contained in 'commands' table
 \return    eTRUE = one or more errors occured, eFALSE = no error occured
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TBOOL HARTWriteObjects(COM_OBJ_DESCR const SLOW *cmdDescr, TUSIGN8 objectCount, TBOOL configChanged, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Reads an object from a subsystem
 \author Stefan Tabelander
 \date   2004-07-27
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       ptrValue:        data pointer
    <\pre>
 \return for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TUSIGN16 HARTGetObjects(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, void *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Reads an attribute from a subsystem
 \author Stefan Tabelander
 \date   2004-07-27
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       attribIdx:       subindex in object 'objIdx'
       ptrValue:        data pointer
    <\pre>
 \return for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TUSIGN16 HARTGetAttribute(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Writes an object to a subsystem
 \author Stefan Tabelander
 \date   2004-07-27
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       ptrValue:        data pointer
    <\pre>
 \return for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TUSIGN16 HARTPutObjects(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, void *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Writes an attribute to a subsystem
 \author Stefan Tabelander
 \date   2004-07-27
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       attribIdx:       subindex in object 'objIdx'
       ptrValue:        data pointer
    <\pre>
 \return for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TUSIGN16 HARTPutAttribute(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void *ptrValue);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Data type conversion function
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param
    <pre>
       dataTypeId:      type of data to be converted
       bufferIdx:       index in transmit or receive buffer where to find the data
       direction:       read or write
    <\pre>
 \return    conversion will be done in data buffer directly at 'bufferIdx'
 \return    eTRUE = Conversion ok, eFALSE = conversion not possible
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TBOOL HARTConvertType(TUSIGN8 dataTypeId, TUSIGN8 bufferIdx, TUSIGN8 direction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTConvertPASCII: Packed ASCII conversion function
 \author    Horst Seele
 \date      2005-11-23
 \param     length:     HART commando length of data to be converted
 \param     bufferIdx:  index in transmit or receive buffer where to read/write the data
 \param     direction:  read or write
 \return    conversion will be done in data buffer directly at 'bufferIdx'
 \return    eTRUE = Conversion ok, eFALSE = conversion not possible
 \warning   Because the conversion from PASC to ASC needs 1 Byte more every 3 PASC characters there
 \warning   must be enough free space in the receive buffer after the PASC string to convert him
 \warning   there (Only in WRITE direction!).
 \bug
*/
//--------------------------------------------------------------------------------------------------
extern TBOOL HARTConvertPASCII(TUSIGN8 length, TUSIGN8 bufferIdx, TUSIGN8 direction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTReadWriteBurstObjects: Handles Read/Write of HART burst commands (103-105, 107-108)
 \author    Bernhard Tohermes
 \date      2010-02-09
 \param     commandNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result  module test:
 \n result  Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------

extern void HARTReadWriteBurstObjects(TUSIGN16 commandNumberIndex, TUSIGN8 burstMsgNumber, TUSIGN8 cmdFunction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTReadObjects: Read objects subscribed over slot numbers
 \author    Bernhard Tohermes
 \date      2010-02-24
 \param     cmdNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result  module test:
 \n result
*/
//--------------------------------------------------------------------------------------------------


extern void HARTReadSubscribedObjects (TUSIGN16 cmdNumber, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);




