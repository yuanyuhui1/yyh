// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 96
// In the definition of a function-like macro the whole definition,
// and each instance of a parameter, shall be enclosed in parentheses.

// #pragma diag_suppress=Pm084

//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Class definition T_UNIT::MODBUS

                Subsystem interface
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
    #error system.h must be included before MODBUS.h
#endif

#ifndef __COMMON_TYPE_H__
    #error common_type.h must be included before MODBUS.h
#endif

#ifndef __T_UNIT_H__
    #error t_unit.h must be included before MODBUS.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __MODBUS_H__
  #error MODBUS.h included more than once
#endif

#define __MODBUS_H__

#define MODBUS_VERSION 0x010000  // Version 1.0.0

//define if use system crc calculation
//#define MODBUS_USE_SYSTEM_CRC_CALC

//-------------------------------------------------------------------------------------------------
//!  define macros, enums
//@SubGen start@  <TYPES>
  // TModbusConfigurationIEEEEnableState
  #define MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED 0 //IEEE format enabled for word swap
  #define MODBUS_CONFIGURATION_IEEE_FORMAT_DISABLED 1 //IEEE format disabled for word swap

  // 
  #define MODBUS_IEEE_FORMAT_COUNT 2 //Total nof IEE format for WORD swap
  #define MODBUS_MAX_SERIAL_ADU_MESSAGE_SIZE 256 //Maximum size of a serial based ADU message. This is the PDU with associated slave device id and checksum.
  #define MODBUS_MAX_TCP_ADU_MESSAGE_SIZE 260 //Maximum size of a TCP based ADU message. This is the PDU with associated TCP information.
  #define MODBUS_MAX_PDU_MESSAGE_SIZE 253 //Maximum size of a Modbus PDU message buffer in bytes - this is the buffer without any addressing or checksum information
  #define MODBUS_MAX_DATA_SIZE 250 //Maximum number of data bytes that can fit in a Modbus message buffer
  #define MODBUS_COILS_PER_BYTE 8 //Maximum number of coils that will fit in a byte
  #define MODBUS_BYTES_PER_REGISTER 2 //Number of bytes that will fit into a register
  #define MODBUS_TCP_PORT 502 //Modbus TCP server listening TCP port
  #define MODBUS_TCP_MAX_CLIENTS 3 //How many Modbus TCP connections can be active at the same time
  #define MODBUS_MAX_SIMULTANEOUS_TRANSACTIONS 5 //How many transaction can be ongoing at the same How many transaction can be ongoing at the same How many transaction can be ongoing at the same time
  #define MODBUS_TCP_ADU_RESEGMENTATION_TIMEOUT_IN_MS 1000 //Time for client to provide full Modbus TCP ADU
  #define MODBUS_MANUFACTUR_AND_DEVICE_ID 0x00001AA0 //1A is for ABB and A0 for Coriolis

  // T_MB_DIAG_BYTE0
  #define MODBUS_CURRENTOUT_FIXEDMODE_ALARM 0x01 //
  #define MODBUS_ALM_LGC_SIM_OP1 0x02 //
  #define MODBUS_ALM_PULS_SIM_OP1 0x04 //
  #define MODBUS_ALM_LGC_SIM_OP2 0x08 //
  #define MODBUS_ALM_PULS_SIM_OP2 0x10 //
  #define MODBUS_ALM_LGC_SIM_OP3 0x20 //
  #define MODBUS_FLOW_VOLUME_P_MIN_REACHED 0x40 //
  #define MODBUS_FLOW_VOLUME_P_MAX_REACHED 0x80 //

  // T_MB_DIAG_BYTE1
  #define MODBUS_FLOW_VOLUME_104P_REACHED 0x01 //
  #define MODBUS_FLOW_VOLUME_SIMULATION_MODE 0x02 //
  #define MODBUS_CALIBRATION_TEST_MODE 0x04 //
  #define MODBUS_TOT_DISPLAY_ALARM 0x08 //
  #define MODBUS_TOT_ALARM_COUNTER_RESET 0x10 //
  #define MODBUS_SENSORINTERFACE_INTERMITTENT_COMMUNICATIONS 0x20 //
  #define MODBUS_HART_INSTRUMENT_ADDRESS_NOT_ZERO 0x40 //
  #define MODBUS_FRAM_COMMUNICATIONS_FAILURE_ALARM 0x80 //

  // T_MB_DIAG_BYTE2
  #define MODBUS_NVMEMINTERFACE_NO_SENSOR_ALARM 0x01 //
  #define MODBUS_MEASUREMENT_OFFLINE_ALARM 0x02 //
  #define MODBUS_EMPTY_PIPE_ALARM 0x04 //
  #define MODBUS_FULL_PIPE_ALARM 0x08 //
  #define MODBUS_CONDUCTIVITY_LOW_ALARM 0x10 //
  #define MODBUS_ELECTRODE_OPEN_CIRCUIT_ALARM 0x20 //
  #define MODBUS_ELECTRODE_SHORT_CIRCUIT_ALARM 0x40 //
  #define MODBUS_ELECTRODE_COATED_ALARM 0x80 //

  // T_MB_DIAG_BYTE3
  #define MODBUS_INSTALLATION_FAULT_ALARM 0x01 //
  #define MODBUS_COIL_OPEN_CIRCUIT_ALARM 0x02 //
  #define MODBUS_COIL_SHORT_CIRCUIT_ALARM 0x04 //
  #define MODBUS_LOOP_RESISTANCE_ALARM 0x08 //
  #define MODBUS_TX_HARDWARE_ALARM 0x10 //
  #define MODBUS_BAD_FLOW_DATA_ALARM 0x20 //
  #define MODBUS_ELECTRODE_VOLTAGE_ALARM 0x40 //
  #define MODBUS_OIML_SELF_CHECK_ALARM 0x80 //

  // T_MB_DIAG_BYTE4
  #define MODBUS_WARMING_UP_ALARM 0x01 //
  #define MODBUS_MSP_INTERFACE_COM_ERROR_ALARM 0x02 //
  #define MODBUS_DEVICE_ALARM_SYSTEM_NOT_CALIBRATED 0x04 //
  #define MODBUS_DEVICE_ALARM_SYSTEM_CALIBRATION_MATCH_FAIL 0x08 //
  #define MODBUS_CHECKING_ALARM_ROM_ERROR 0x10 //
  #define MODBUS_CHECKING_ALARM_RAM_ERROR 0x20 //
  #define MODBUS_HART_ALARM_FREQUENCY_SIMULATION 0x40 //
  #define MODBUS_ALARM_SIMUALTION_ERROR 0x80 //

  // T_MB_DIAG_BYTE5
  #define MODBUS_NVMEMINTERFACE_SUMMARY_ALARM 0x01 //
  #define MODBUS_UNUSED_ALARM_BIT_1 0x02 //
  #define MODBUS_UNUSED_ALARM_BIT_2 0x04 //
  #define MODBUS_UNUSED_ALARM_BIT_3 0x08 //
  #define MODBUS_UNUSED_ALARM_BIT_4 0x10 //
  #define MODBUS_UNUSED_ALARM_BIT_5 0x20 //
  #define MODBUS_UNUSED_ALARM_BIT_6 0x40 //
  #define MODBUS_UNUSED_ALARM_BIT_7 0x80 //

  // E_WriteFunctionsEnabling
  #define MODBUS_WRITE_FUNCTIONS_DISABLED 0 //Modbus write functions disabled
  #define MODBUS_WRITE_FUNCTIONS_ENABLED 1 //Modbus write functions enabled
//@SubGen end@ <TYPES>


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


// Modes hardware layers can operate in
typedef enum _TModbusHardwareLayerMode
{
    // Client device, request
    MODBUS_MODE_CLIENT = 0,
    // Server device, response
    MODBUS_MODE_SERVER,
    MODBUS_MODE_COUNT
}TModbusMode;


// The hardware layers registered with the Modbus subsystem. This ID is passed with a master
// request to identify over which hardware layer the request is to be sent.
typedef enum _TModbusHardwareLayerId
{
    MODBUS_HARDWARE_LAYER_ID_485 = 0,
    MODBUS_HARDWARE_LAYER_ID_TCP,
    MODBUS_HARDWARE_LAYER_ID_COUNT
}TModbusHardwareLayerId;


// Slave response parameters residing in the hardware layer
typedef struct _TModbusServerResponseParameters
{
    // Set if access rights checks should be performed
    TBOOL checkAccessRights;
}TModbusServerResponseParameters;

// Parameters common to master requests and slave responses residing in the hardware layer
typedef struct _TModbusCommonParameters
{
    // Flag set whilst a message is being processed
    TBOOL messageProcessing;
    // The length of the message currently being processed
    TUSIGN16 length;
    // Mode the hardware layer is operating in
    TModbusMode modbusMode;
    // Reference to the subsystem send message function
    //pModbusSendMessageFunction pSendMessageFunction;
    // The buffer owned by the hardware layer used to store incoming/outgoing messages
    TUSIGN8 *pMessageBuffer;
}TModbusCommonParameters;

// Type used to store details of hardware layers
typedef struct _TModbusHardwareLayer
{
    // Slave response specifc parameters
    TModbusServerResponseParameters modbusServerResponseParameters;
    // Common parameters
    TModbusCommonParameters modbusCommonParameters;
}TModbusHardwareLayer;

//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum E_MODBUS_DC_INDEXES
{
    /*lint -e793 allow pointer operation by Spring.Zhou*/

    //@SubGen start@    <DATACLASSES>
    MODBUS_DC_IDX_CONSTANT,
    MODBUS_DC_IDX_DYNAMIC,
    MODBUS_DC_IDX_STATIC_FREQUENT_NO_DEFAULT,
    MODBUS_DC_IDX_STATIC_FREQUENT_CONSTANT_DEFAULT,
    MODBUS_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_MODBUS

typedef struct _T_MODBUS
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* UpdateScanRegister)(void );
    void (* SinkEventEXE)(const void* pEvent);
    void (* UpdateEnableStateEXE)(TUSIGN8 enableState);
    void (* TaskEXE)(void);
    //@SubGen end@    <METHODS>
} T_MODBUS;


//-------------------------------------------------------------------------------------------------
//! define the T_MODBUS-object
extern SLOW const T_MODBUS modbus;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_MODBUS
#define MODBUS_ME (&modbus.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define modbus_Get(objectIndex,attributeIndex,ptrValue) \
   modbus.unit.Get(MODBUS_ME,objectIndex,attributeIndex,ptrValue)
#define modbus_Put(objectIndex,attributeIndex,ptrValue) \
   modbus.unit.Put(MODBUS_ME,objectIndex,attributeIndex,ptrValue)
#define modbus_Check(objectIndex,attributeIndex,ptrValue) \
   modbus.unit.Check(MODBUS_ME,objectIndex,attributeIndex,ptrValue)

#define modbus_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   modbus.unit.GetAttributeDescription(MODBUS_ME,objectIndex,attributeIndex,ptrDescriptor)
#define modbus_GetObjectDescription(objectIndex,ptrDescriptor) \
   modbus.unit.GetObjectDescription(MODBUS_ME,objectIndex,ptrDescriptor)


