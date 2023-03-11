//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus

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

#ifdef __MODBUS_IDX_H__
  #error MODBUS_idx.h included more than once
#endif

#define __MODBUS_IDX_H__

enum _T_MODBUS_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     ModBus diagnostic status alarm object.
     \datatype          : MODBUS_DIAG
     \typequalifier     : STRUCT
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MODBUS_IDX_mbDiag,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     ModBus diagnostic alarm history.
     \datatype          : MODBUS_DIAG_HIS
     \typequalifier     : STRUCT
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MODBUS_IDX_mbDiagHis,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Configuration for Modbus IEEE format for handling WORD swap
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MODBUS_IDX_modbusIEEEFormat,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16[32]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MODBUS_IDX_configScanRegister1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     \datatype          : SIMPLE_U16[32]
     \typequalifier     : ARRAY
     \storage qualifier : NON_VOLATILE
     \direction         : INOUT
    */
    MODBUS_IDX_configScanRegister2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Scan Register 1
     \datatype          : SIMPLE_U16[32]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_ScanRegister1,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Scan Register 2
     \datatype          : SIMPLE_U16[32]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_ScanRegister2,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     is used for a special functionality to write the modbusAddress by using the sensorSerialID and the manufactuerDeviceID = 0x00001AA0 (Manufactuer ID =1A for ABB andDeviceID = A0 for Coriolis) and the slaveID
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_manufactuerDeviceID,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     is used for a special functionality to write the modbusAddress by using the sensorSerialID and the manufactuerDeviceID = 0x00001AA0 (Manufactuer ID =1A for ABB andDeviceID = A0 for Coriolis) and the slaveID
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_sensorSerialID,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     is used for a special functionality to write the modbusAddress by using the sensorSerialID and the manufactuerDeviceID = 0x00001AA0 (Manufactuer ID =1A for ABB andDeviceID = A0 for Coriolis) and the slaveID
     \datatype          : SIMPLE_U32
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_slaveID,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     This object mapped the 48 alarm bits to 3x U16.
     \datatype          : SIMPLE_U16[3]
     \typequalifier     : ARRAY
     \storage qualifier : RAM
     \direction         : INOUT
    */
    MODBUS_IDX_mDiagCondenseStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Port number for TCP connection on which Modbus TCP communication will happen
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : READONLY_RAM
     \direction         : CONTAINED
    */
    MODBUS_IDX_networkPort,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Enabling Modbus write functions
     \datatype          : TABENUM8
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    MODBUS_IDX_writeFunctionsEnabled
    //@SubGen end@    <ENUMERATIONS>
};