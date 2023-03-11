//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System         Subsystem MODBUS
 Module         MODBUS_register_table.h
 Description    Interface to Modbus register table type
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __COMMON_TYPE_H__
    #error "Modbus_Register_Table.h Error - Common_Type.h must be included first."
#endif

#ifdef __MODBUS_REGISTER_TABLE__
    #error "Modbus_Register_Table.h Error - Modbus_Register_Table.h is already included."
#endif

#define __MODBUS_REGISTER_TABLE__
//-------------------------------------------------------------------------------------------------

// Location of the register table
typedef enum _TModbusRegisterTableLocation
{
    MODBUS_REGISTER_TABLE_LOCATION_INPUT_COILS = 0,
    MODBUS_REGISTER_TABLE_LOCATION_OUTPUT_COILS,
    MODBUS_REGISTER_TABLE_LOCATION_INPUT_REGISTERS,
    MODBUS_REGISTER_TABLE_LOCATION_HOLDING_REGISTERS,
    MODBUS_REGISTER_TABLE_LOCATION_NONE,
    MODBUS_REGISTER_TABLE_LOCATION_COUNT
}TModbusRegisterTableLocation;

// Type used to hold range information for a register table
typedef struct _TModbusRegisterTableRange
{
    // (Inclusive) lowest allowable address
    TUSIGN16 lowerRange;
    // (Inclusive) highest allowable address
    TUSIGN16 upperRange;
}TModbusRegisterTableRange;

// Type describing register table privileges
typedef enum _TModbusRegisterTablePrivilige
{
    // Table can be read from only
    MODBUS_REGISTER_TABLE_PRIVILEGE_READ_ONLY = 0,
    // Read and write access allowed to the table
    MODBUS_REGISTER_TABLE_PRIVILEGE_READ_WRITE,
    MODBUS_REGISTER_TABLE_PRIVILEGE_COUNT
}TModbusRegisterTablePrivilege;

// Type used to build table of items accessed via Modbus
typedef struct _TModbusRegisterTableItem
{
    // Id of the subsystem referenced by the table item
    TUSIGN16 subsystemId;
    // Id of the object referenced by the table item
    TUSIGN16 objectId;
    // Object specific details - In the case of strings, this is the length of the string
    // For non-string based types, this the attribute index of the object
    TUSIGN16 itemDetails;
}TModbusRegisterTableItem;

// Type used to define the data within a register table
typedef struct _TModbusRegisterTableDataDescriptor
{
    // List of the table attribute sizes in bytes
    const TUSIGN16 *pTableByteSizes;
    // List of the table attribute sizes in registers
    const TUSIGN16 *pTableRegisterSizes;
    // The number of attributes for each table entry
    // This only applies to multiple object based register tables
    // If it's a non-multiple type register table, we only need
    // to access the first element of the byte and register size members
    TUSIGN16 attributeCount;
    // The total size in bytes of the table entry
    TUSIGN16 totalByteSize;
    // The total size in registers of the table entry
    TUSIGN16 totalRegisterSize;
//lint -e{793} by CNZUWAN AS code review is accetable.
}TModbusRegisterTableDataDescriptor;

// Types of register tables available
typedef enum _TModbusRegisterTableType
{
    // Coil data - always treated as binary 0/1 regardless of underlying type
    MODBUS_REGISTER_TABLE_TYPE_COIL = 0,
    // List of objects of the same type
    MODBUS_REGISTER_TABLE_TYPE_SINGLE,
    // List of complex objects
    MODBUS_REGISTER_TABLE_TYPE_WHOLE_OBJECT,
    // List of array based objects
    MODBUS_REGISTER_TABLE_TYPE_STRING,
    // Groups of object attributes
    MODBUS_REGISTER_TABLE_TYPE_MULTIPLE,
    MODBUS_REGISTER_TABLE_TYPE_COUNT
}TModbusRegisterTableType;

// Link between a request and a register table
typedef struct _TModbusRegisterTableRequestLink
{
    // Index in the array of register table items of the first requested item
    TUSIGN16 startItemArrayIndex;
    // Index in the array of register table items of the last requested item
    TUSIGN16 endItemArrayIndex;
    // Attribute in the first object at the start of the request - not used
    // for single and coil commands
    TUSIGN16 startItemAttribute;
    // Attribute in the last object at the end of the request - not used
    // for single and coil commands
    TUSIGN16 endItemAttribute;
    // The start register of the first requested item
    TUSIGN16 startItemRegisterAddress;
    // The end register of the last requested item
    TUSIGN16 endItemRegisterAddress;
    // The total number of array indexes
    TUSIGN16 itemArrayCount;
}TModbusRegisterTableRequestLink;

// Details of a request to the register table

typedef struct _TModbusRegisterTableRequestDetails
{
    // The start register address of the request
    TUSIGN16 startRegisterAddress;
    // The number of coils or registers (or in the case of
    // private commands, objects) requested for access
    TUSIGN16 itemCount;
    // Set when it's a private type request
    TBOOL isPrivateRequest;
//lint -e{793} by CNZUWAN AS code review is accetable.
}TModbusRegisterTableRequestDetails;

// Operation being performed on a table
typedef enum _TModbusRegisterTableAccessType
{
    MODBUS_REGISTER_TABLE_ACCESS_TYPE_READ = 0,
    MODBUS_REGISTER_TABLE_ACCESS_TYPE_WRITE,
    MODBUS_REGISTER_TABLE_ACCESS_TYPE_COUNT
}TModbusRegisterTableAccessType ;

// Object encapsulating register table behaviour
typedef struct _TModbusRegisterTable
{
    // The type of register table
    TModbusRegisterTableType modbusRegisterTableType;
    // Read/write privileges for the table
    TModbusRegisterTablePrivilege modbusRegisterTablePrivilege;
    // Range of registers encapsulated by the table
    TModbusRegisterTableRange modbusRegisterTableRange;
    // The number of objects in the table
    TUSIGN16 objectCount;
    // The list of objects associated with the table
    const TModbusRegisterTableItem *pModbusRegisterTableItems;
    // The table data descriptors
    const TModbusRegisterTableDataDescriptor *pModbusRegisterTableDataDescriptor;
    // The location of the register table
    TModbusRegisterTableLocation modbusRegisterTableLocation;
    // Pointers to the register table buffer manipulation functions
    // Build table link function
    TUSIGN16 (*GetTableLink)(const struct _TModbusRegisterTable *me,
                             const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                             TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink);
    // Get length function
    TUSIGN16 (*GetLength)(const struct _TModbusRegisterTable *me,
                          const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                          TBOOL byteLength);
    // Check for overlap function
    TUSIGN16 (*CheckForOverlap)(const struct _TModbusRegisterTable *me,
                                const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                                const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink);
    // Format to internal data format
    void (*FormatToInternal)(const struct _TModbusRegisterTable *me,
                             const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                             const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                             TUSIGN8 *pBuffer);
    // Format to external data format
    void (*FormatToExternal)(const struct _TModbusRegisterTable *me,
                             const TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails,
                             const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                             TUSIGN8 *pBuffer);
    // Pointers to the register table object manipulation functions
    // Check access rights
    TUSIGN16 (*CheckAccessRights)(const struct _TModbusRegisterTable *me,
                                  const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                  TModbusRegisterTableAccessType modbusRegisterTableAccessType);
    // Range check
    TUSIGN16 (*CheckRange)(const struct _TModbusRegisterTable *me,
                           const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                           TUSIGN8 *pBuffer);
    // Object access
    TUSIGN16 (*AccessObjects)(const struct _TModbusRegisterTable *me,
                              const TModbusRegisterTableRequestDetails *pModbusRequestDetails,
                              const TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                              TUSIGN8 *pBuffer,
                              TModbusRegisterTableAccessType modbusRegisterTableAccessType,
                              TUSIGN16 *pLength);

}TModbusRegisterTable;


//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
 \test
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{793} by CNZUWAN AS code review is accetable.
TUSIGN16 GetRequestLengthModbus_Register_Table(const TModbusRegisterTable *me,
                                               TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                               TBOOL byteLength);

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
 \test
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{793} by CNZUWAN AS code review is accetable.
TUSIGN16 GetTableLinkModbus_Register_Table(const TModbusRegisterTable *me,
                                           TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                           TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails);

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Returns the size of a message in registers
 \author  GL
 \date    24/07/08
 \param   [in] me - Reference to the register table to determine the message length for
 \param   [in] pModbusRegisterTableRequestLink - Details of the request parameters
 \return  The size in registers of the response length
 \warning Will return 0 if any errors are encountered
 \test
 date: 2005-08-23
 \n by: GL
 \n environment: IAR Embedded Workbench V3.10A
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
//lint -e{793} by CNZUWAN AS code review is accetable.
TUSIGN16 CheckForObjectOverlapModbus_Register_Table(const TModbusRegisterTable *me,
                                                    TModbusRegisterTableRequestLink *pModbusRegisterTableRequestLink,
                                                    TModbusRegisterTableRequestDetails *pModbusRegisterTableRequestDetails);