//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2008.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MODBUS
 Module         MODBUS_register_table_objects.c
 Description    Generic Modbus object manipulation functions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "Modbus/Source/Modbus_register_table.h"
#include "Modbus/Source/Modbus_register_table_objects.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ARM/Interface/arm.h"



//--------------------------------------------------------------------------------------------------
/*!
 \brief   Checks if the access rights for the objects within a message allow the requested action
 \author  GL
 \date    29/07/08
 \param   [in] pModbusCommandDescriptor - Reference to the message command structure
 \param   [in] pModbusRegisterTable - Register table where the objects reside
 \param   [in] arrayIndex - Index where the initial object resides
 \param   [in] objectCount - The number of objects requested for read/write in the message
 \return  OK if the access rights permit the request action
 \warning
 \test
 \n test-date:          10.11.2017
 \n by:                 H.Seebode 
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          complete code coverage;
 \n result module test: passed
 \n result Lint check:  passed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CheckAccessRightsModbus_Register_Table_Objects(const TModbusRegisterTable *me,
                                                        const TModbusRegisterTableRequestLink
                                                        *pModbusRegisterTableRequestLink,
                                                        TModbusRegisterTableAccessType
                                                        modbusRegisterTableAccessType)
{
    // Result of access rights check
    TUSIGN16 result = OK ;

    // Current object having its access rights checked
    TUSIGN16 objectsChecked ;
    // Used to let the ARM subsystem what access rights are being sought
    TUSIGN8 accessType = READ_ONLY_ACCESS ;            
    // Table item having its access rights checked
    const TModbusRegisterTableItem *pModbusRegisterTableItem = (const TModbusRegisterTableItem *)NULL ;
    // Attribute to check access rights for
    TUSIGN16 attributeId ;

    // Check if we're testing for write privileges
    if (modbusRegisterTableAccessType == MODBUS_REGISTER_TABLE_ACCESS_TYPE_WRITE)
    {
        accessType = READ_WRITE_ACCESS ;
    }
    // Check the access rights for each of the objects we're trying to access
    for (objectsChecked = pModbusRegisterTableRequestLink->startItemArrayIndex ;
        (objectsChecked <= pModbusRegisterTableRequestLink->endItemArrayIndex) &&
        (result == OK) ;
         objectsChecked++)
    {
        // Check access rights for the next object
        pModbusRegisterTableItem = &me->pModbusRegisterTableItems[objectsChecked] ;
        // String object ?
        if (me->modbusRegisterTableType == MODBUS_REGISTER_TABLE_TYPE_STRING)
        {
            attributeId = (TUSIGN16)WHOLE_OBJECT ;
        }
        else
        {
            attributeId = pModbusRegisterTableItem->itemDetails ;
        }
        result = arm.CheckAccessRightsSRV(MODBUS_IDX,
            (T_SUBSYSTEM_IDX)pModbusRegisterTableItem->subsystemId,
            pModbusRegisterTableItem->objectId,
            attributeId,
            accessType);
    }
    return(result) ;
}