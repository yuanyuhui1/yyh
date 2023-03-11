//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2005.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem MODBUS
Module         Modbus_scanregister
Description    Handling of scan registers

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Modbus/Interface/Modbus.h"
#include "Modbus/Interface/t_data_obj_modbus_diag.h"
#include "Modbus/Interface/t_data_obj_modbus_diag_history.h"
#include "modbus_types.h"
#include "modbus_attributes.h"
#include "modbus_scanregister.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_tables.h"


#undef  NAN                    // undefine math.h's NAN
#define NAN       0x7FA00000  // single precision float "not a number"

// Internal tables which are used to speed up the collection of data for the scan registers
TconfigScanRegisterIntern  configScanRegisterIntern1[32];
TconfigScanRegisterIntern  configScanRegisterIntern2[32];


//-------------------------------------------------------------------------------------------------
/*!
\brief  CheckModbusAddress : Checks the modbus register address
\author Holger Seebode, Klaus Pose, Florian Reuss
\date 28/08/08          2017-04-07, 2018-03-14
\param
<pre>
TUSIGN16 modbusRegister : PLC modbus register address
</pre>
\return error-code
<pre>
OK                        : address exists and it is allowed to read from this address
GREATERTHAN_RANGEMAX_ERR  : address is in an valid address range but above valid object addresses
NOT_ON_GRID_ERR           : object has more than one holding register and address doesn�t points to the first register
</pre>
\test
\test-date: 2018-03-14
\n by:  Florian Reuss
 \n environment: IAR Embedded Workbench Renesas RX V2.41.3
 \n intention: 100% code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: 
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 CheckModbusAddress(TUSIGN16 modbusRegister)
{
    TUSIGN16 tableIndex;
    static TUSIGN16 modulo;
    static TUSIGN16 maxObjectAddress;
    TUSIGN16 totalRegisterSize;
    TUSIGN16 result = RULE_VIOLATION_ERR;
    const TUSIGN16 sizeTableDefinition = MODBUS_REGISTER_TABLE_ID_COUNT;
   
    if (modbusRegister == 0x0)
    {
        return OK;
    }
    // Convert from PLC addressing
    modbusRegister--;

    // Find corresponding table for modbusRegisterAddress
    for (tableIndex = 0; tableIndex < sizeTableDefinition; tableIndex++)
    {   // check for tables that are not allowed to use. Device specific tables!
        if (!((tableIndex == MODBUS_REGISTER_TABLE_ID_READONLY_1BIT_BOOL_B1RO1) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER1_V16RO1) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER2_V16RO2) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER1_U16RW1) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER2_U16RW2) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_ACTION_8BIT_WRITEONLY_A8WO) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READONLY_C8RO) ||
              (tableIndex == MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READWRITE_C8RW)))
        {
            if ((modbusRegister >= modbusRegisterTables[tableIndex].modbusRegisterTableRange.lowerRange) &&
                (modbusRegister <= modbusRegisterTables[tableIndex].modbusRegisterTableRange.upperRange))
            {
                // a suitable address range was found
                // Check if address fits to number of objects
                totalRegisterSize = modbusRegisterTables[tableIndex].pModbusRegisterTableDataDescriptor->totalRegisterSize;
                modulo = (modbusRegister - modbusRegisterTables[tableIndex].modbusRegisterTableRange.lowerRange) % totalRegisterSize;
                if (modulo == 0)
                {
                    // ==> address fits to totalRegisterSize
                    // Check if modbus address do not exceed max object address
                    maxObjectAddress = (TUSIGN16)(modbusRegisterTables[tableIndex].modbusRegisterTableRange.lowerRange + 
                                        totalRegisterSize * (modbusRegisterTables[tableIndex].objectCount - 1));
                    if (modbusRegister <= maxObjectAddress)
                    {
                        result = OK;
                    }
                    else
                    {
                        result = GREATERTHAN_RANGEMAX_ERR;
                    }
                }
                else
                {
                    result = NOT_ON_GRID_ERR;
                }
                break;
            }
        }
    }

    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
\brief  TranslateConfigScanReg2Intern : translate the config register to an internal table to save execution time
\author Holger Seebode, Klaus Pose, Florian Reuss
\date 28/08/08          2017-04-07, 2018-03-23
\param
<pre>
pConfigScanRegister         : input pointer to the config scan register                    
pConfigScanRegisterIntern   : output pointer to the table which was build up 
</pre>
<pre>
none     
</pre>
\test
\test-date: 2018-03-23
\n by:  Florian Reuss
\n environment: IAR Embedded Workbench Renesas RX V2.41.3
\n intention: 100% code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: 
\bug
*/
//-------------------------------------------------------------------------------------------------
void TranslateConfigScanReg2Intern(const TUSIGN16* pConfigScanRegister, TconfigScanRegisterIntern* pConfigScanRegisterIntern)
{
    TUSIGN16 configRegisterIndex = 0;
    TUSIGN16 configRegisterIndexIntern = 0;
    TUSIGN16 usedScanRegister = 0;
    TUSIGN16 modbusRegister;
    TUSIGN16 tableIndex;
    TUSIGN16 totalRegisterSize;
    TUSIGN16 indexRegisterTableItems;
    TUSIGN16 subsystemId;
    TUSIGN16 objectId;
    TUSIGN16 itemDetails;
    T_UNIT const SLOW* pUnit;
    TUSIGN16 stateSub = NOT_INITIALIZED;
    T_DO_ATTRIB_RANGE range;

    const TUSIGN16 sizeConfigScanRegister = sizeof(modbusStaticFrequentConstantDefault.configScanRegister1) / sizeof (TUSIGN16);
    const TUSIGN16 sizeScanRegister = sizeof(modbusDynamic.ScanRegister1) / sizeof (TUSIGN16);
    const TUSIGN16 sizeTableDefinition = MODBUS_REGISTER_TABLE_ID_COUNT;

    VIP_ASSERT(pConfigScanRegister);
    VIP_ASSERT(pConfigScanRegisterIntern);

    while ((configRegisterIndex < sizeConfigScanRegister) && (usedScanRegister < sizeScanRegister))
    {
        modbusRegister = pConfigScanRegister[configRegisterIndex];

        if (modbusRegister != 0x0)
        {
            // Find corresponding table for modbusRegisterAddress 
            modbusRegister--; // Convert from PLC addressing
            tableIndex = 0;

            while (tableIndex < sizeTableDefinition)
            {
                //// check for tables that are not allowed to use. Device specific tables!
                if (!((tableIndex == MODBUS_REGISTER_TABLE_ID_READONLY_1BIT_BOOL_B1RO1) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER1_V16RO1) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_VARIANT_16BIT_READONLY_SCANREGISTER2_V16RO2) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER1_U16RW1) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_UNSIGNED_INTEGER_16BIT_READWRITE_SCANREGISTER2_U16RW2) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_ACTION_8BIT_WRITEONLY_A8WO)||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READONLY_C8RO) ||
                      (tableIndex == MODBUS_REGISTER_TABLE_ID_CHARACTER_8BIT_READWRITE_C8RW)))
                {
                    if ((modbusRegister >= modbusRegisterTables[tableIndex].modbusRegisterTableRange.lowerRange) &&
                        (modbusRegister <= modbusRegisterTables[tableIndex].modbusRegisterTableRange.upperRange))
                    {
                        break;
                    }
                }
                tableIndex++;
            }

            if (tableIndex < sizeTableDefinition)
            {
                // a suitable address range was found
                totalRegisterSize = modbusRegisterTables[tableIndex].pModbusRegisterTableDataDescriptor->totalRegisterSize;

                // Calculate index of register table items
                indexRegisterTableItems = (modbusRegister - modbusRegisterTables[tableIndex].modbusRegisterTableRange.lowerRange) / totalRegisterSize;

                subsystemId = modbusRegisterTables[tableIndex].pModbusRegisterTableItems[indexRegisterTableItems].subsystemId;
                objectId = modbusRegisterTables[tableIndex].pModbusRegisterTableItems[indexRegisterTableItems].objectId;
                itemDetails = modbusRegisterTables[tableIndex].pModbusRegisterTableItems[indexRegisterTableItems].itemDetails;

                pUnit = GetSubsystemPtr(subsystemId);
                if (pUnit)
                {
                    (void)pUnit->GetState(pUnit, &stateSub);
                }
                if (pUnit && (stateSub >= INITIALIZED))
                {
                    (void) pUnit->GetAttributeDescription(pUnit, objectId, itemDetails,&range);
                    pConfigScanRegisterIntern[configRegisterIndexIntern].subsystemPointer = pUnit;
                    pConfigScanRegisterIntern[configRegisterIndexIntern].objectId = objectId;
                    pConfigScanRegisterIntern[configRegisterIndexIntern].itemDetails = itemDetails;
                    pConfigScanRegisterIntern[configRegisterIndexIntern].dataType = range.dataType;

                    switch(range.dataType)
                    {
                        case ST_U32:
                        case ST_FLOAT:
                            usedScanRegister += 2;
                            break;

                        case ST_DOUBLE:
                            usedScanRegister += 4;
                            break;

                        case ST_U16:
                        case ST_I16:
                        case ST_U8:
                        case ST_E8:
                        case ST_BIT8:
                        case ST_NIL:
                        case ST_CHAR:
                        case ST_WIDECHAR:
                        default:
                            usedScanRegister++;
                            break;
                    }
                }
                else
                {
                    // Set unused internal Scan register pointer to zero
                    pConfigScanRegisterIntern[configRegisterIndexIntern].subsystemPointer = (T_UNIT const SLOW*)0;
                }
                configRegisterIndexIntern++;
            }
        }
        configRegisterIndex++; // next config scan register
    }

    while (configRegisterIndexIntern < sizeConfigScanRegister)
    {
        // Set unused internal Scan register pointer to zero
        pConfigScanRegisterIntern[configRegisterIndexIntern].subsystemPointer = (T_UNIT const SLOW*)0;
        configRegisterIndexIntern++;
    }
}


//-------------------------------------------------------------------------------------------------
/*!
\brief  UpdateScanRegister : use the internal configuration table to collect the data and copy them in the scan register 
\author Holger Seebode, Florian Reuss
\date 28/08/08,         2018-03-23
\param 
<pre>
pConfReg        : pointer to the internal configuration table
pScanRegister   : pointer to the scan register
</pre>
\return 
<pre>
none
</pre>
\test
\test-date: 28/08/08,   2018-03-23
\n by:  Holger Seebode, Florian Reuss
\n environment: IAR Embedded Workbench Renesas RX V2.41.3
\n intention: 100% code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: 
\bug
*/
//-------------------------------------------------------------------------------------------------
void UpdateScanRegisterViaTable(const TconfigScanRegisterIntern* pConfReg, TUSIGN16* pScanRegister)
{
    TUSIGN16 confRegIndex = 0;
    TUSIGN16 usedScanRegister = 0;
    TUSIGN16 Copy[32] = {0};
    T_UNIT const SLOW* pUnit;
    TPOLY64 value;
    
    const TUSIGN16 sizeConfigScanRegister = sizeof(modbusStaticFrequentConstantDefault.configScanRegister1) / sizeof(TUSIGN16);
    
    VIP_ASSERT(pConfReg);
    VIP_ASSERT(pScanRegister);
    
    while (confRegIndex < sizeConfigScanRegister)
    {
        pUnit = pConfReg[confRegIndex].subsystemPointer;
        
        if (pUnit != 0)
        {
           switch(pConfReg[confRegIndex].dataType)
           {
              case ST_U16:
              case ST_I16:
                  if (pUnit->Get(pUnit, pConfReg[confRegIndex].objectId, (TINT16)pConfReg[confRegIndex].itemDetails, &(value.aryU16[0])) != OK)
                  {
                      value.aryU16[0] = 0;
                  }
                  Copy[usedScanRegister++] = value.aryU16[0];
                  break;
                  
              case ST_U32:
                  if (pUnit->Get(pUnit, pConfReg[confRegIndex].objectId, (TINT16)pConfReg[confRegIndex].itemDetails, &(value.aryU32[0])) != OK)
                  {
                      value.aryU32[0] = 0;
                  }
                  if (modbusStaticFrequentNoDefault.modbusIEEEFormat == MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED)
                  { 
                      Copy[usedScanRegister++] = value.aryU16[1];
                      Copy[usedScanRegister++] = value.aryU16[0];
                  }
                  else
                  {
                      Copy[usedScanRegister++] = value.aryU16[0];
                      Copy[usedScanRegister++] = value.aryU16[1];
                  }
                  break;
                  
              case ST_FLOAT: 
                  if (pUnit->Get(pUnit, pConfReg[confRegIndex].objectId, (TINT16)pConfReg[confRegIndex].itemDetails, &(value.aryFlt[0])) != OK)
                  {
                      *((TUSIGN32*) &value.aryFlt[0]) = NAN;
                  }

                  if (modbusStaticFrequentNoDefault.modbusIEEEFormat == MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED)
                  { 
                      Copy[usedScanRegister++] = value.aryU16[1];
                      Copy[usedScanRegister++] = value.aryU16[0];
                  }
                  else
                  {
                      Copy[usedScanRegister++] = value.aryU16[0];
                      Copy[usedScanRegister++] = value.aryU16[1];
                  } 
                  break;
                   
              case ST_DOUBLE:
                  if (pUnit->Get(pUnit, pConfReg[confRegIndex].objectId, (TINT16)pConfReg[confRegIndex].itemDetails, &(value.dbl)) != OK)
                  {
                      *((TUSIGN32*) &value.aryFlt[1]) = 0x7FF40000; // double NAN 
                      *((TUSIGN32*) &value.aryFlt[0]) = 0x00000000; // double NAN 
                  }
                  
                  if (modbusStaticFrequentNoDefault.modbusIEEEFormat == MODBUS_CONFIGURATION_IEEE_FORMAT_ENABLED)
                  { 
                      Copy[usedScanRegister++] = value.aryU16[3];
                      Copy[usedScanRegister++] = value.aryU16[2];
                      Copy[usedScanRegister++] = value.aryU16[1];
                      Copy[usedScanRegister++] = value.aryU16[0];
                  }
                  else
                  {
                      Copy[usedScanRegister++] = value.aryU16[0];
                      Copy[usedScanRegister++] = value.aryU16[1];
                      Copy[usedScanRegister++] = value.aryU16[2];
                      Copy[usedScanRegister++] = value.aryU16[3];
                  }
                  break;
           
              case ST_U8:
              case ST_E8:
              case ST_BIT8:
              case ST_NIL:
              case ST_CHAR:
              case ST_WIDECHAR:
              default:
                  if (pUnit->Get(pUnit, pConfReg[confRegIndex].objectId, (TINT16)pConfReg[confRegIndex].itemDetails, &(value.aryU8[0])) != OK)
                  {
                      value.aryU8[0] = 0;
                  } 
                  value.aryU8[1] = 0;  // delete old content
                  Copy[usedScanRegister++] = value.aryU16[0];
                  break;
            }
            confRegIndex++; // next config scan register
        }
        else
        {
            break;
        }
    }
    
    ENABLE_DATA_PROTECTION(*(modbus.unit.ptrDataSemaphore));
    (void)memcpy(pScanRegister, Copy, 64);
    DISABLE_DATA_PROTECTION(*(modbus.unit.ptrDataSemaphore));
}