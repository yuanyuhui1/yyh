//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    implementation of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//--------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
// @@adjust
#include "Coordinator/Interface/subsystem_idx.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Interface/Hart_idx.h"
#include "HART/Interface/Hart.h"
#include "hart_overload.h"
#include "hart_types.h"
#include "hart_attributes.h"

//added 20110426 for ReadHART BurstIn Data
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"

//#include "..\..\application\interface\application.h"//Delete
//#include "..\..\application\interface\application_idx.h"//Delete

static TUSIGN16 UpdateBurstInData(const TFLOAT* ptrValue);
extern TBOOL HartConvUnitCodeFromHartUnit_HART(TUSIGN8 * pUnitCode, TUSIGN8 * pUnitTypeCode );

// init values of burst message confi data
static const T_BURSTMSG_CONFVALUES burstMsgConfInitValueTable[3] =
{
    {0,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_1,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,250,0.0,0,250,0,0},
    {1,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_1,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,250,0.0,0,250,0,0},
    {2,HART_BURSTMODE_OFF,31,HART_BURST_COMMAND_1,250,250,250,250,250,250,250,250,32000,32000,HART_BURST_TRIGGER_MODE_CONTINUOUS,0,250,0.0,0,250,0,0},
};


//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize the subsystem; (hardware, tasks, data, calculation)
            Change code using macro MSP_430_SOFT_MODEM
 \author    Heiko Kresse,Eric-Jiping.li
 \date      2004-11-09,2011-04-12
 \param     typeOfStartUp
 <pre>
   INIT_HARDWARE   =0x80  initialize hardware used by the subsystem
   INIT_TASKS      =0x40  initialize tasks, semaphores, timer ....
   INIT_DATA       =0x20  initialize data and data structures
   INIT_CALCULATION=0x10  if a subsystem needs onle once a preparation for calculating, then
 </pre>
 \return    error-code
 <pre>
   OK                      operation was successful
   ERROR_NV_STORAGE        attributes inside the nv-storage are corrupted
   ERROR_RAM_STORAGE       attributes inseid the ram are corrupted
   ERROR_HW                hardware used by the subsystem doesn't work
 </pre>
 \warning
 \test
 test-date: 2008-05-07
 \n by:     Marcel Schilg
 \n environment: PD30F
 \n intention:  sight check
 \n result  module test: ok
 \n result  Lint Level 3: ok, 2008-05-07
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 Initialize_HART(const T_UNIT SLOW *me, TUSIGN8 typeOfStartUp)
{
    T_BURSTMSG_CONFVALUES           burstMsgConfInitData;
    TUSIGN8                         newHartRevision, idx;
    TUSIGN8                         lockDeviceStatus;

    TUSIGN16                        result = OK;
    TUSIGN8                         dummyU8=0xFF;

    // debug zero-pointer
    //lint -e{746} Heiko Kresse 2004-11-08
    VIP_ASSERT(me);

    result = Initialize_T_UNIT(me, typeOfStartUp);

    if( result==OK )
    {
        if( (typeOfStartUp & INIT_HARDWARE) != 0)
        {
            // Initialize_LAYER2();
            // StartReceive_LAYER1();   // only necessary for Layer1-only tests

            // +++ activate hart revision switch +++
            // if switch to other hart revision
            
            if(hartFreqCst.hartCmdRev != hartFreqCst.univCmdRev)
            {
                // hart revision to switch to
                newHartRevision = hartFreqCst.hartCmdRev;
                // store hart revision to work compliant to
                (void)Put_T_UNIT(me, HART_IDX_univCmdRev, WHOLE_OBJECT, (void FAST *) &newHartRevision);

                // Reset all burst settings to their default values
                // for all burst messages
                for(idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
                {
                    // init burst message configuration data
                    (void)memcpy(&burstMsgConfInitData, &burstMsgConfInitValueTable[idx], sizeof(T_BURSTMSG_CONFVALUES));
                    result |= Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, WHOLE_OBJECT, (void FAST *) &burstMsgConfInitData);
                }

                // If used as HART5 device
                if(hartFreqCst.univCmdRev == 5)
                {
                    // Get the current target mode
//                    (void)coordinator.unit.Get(COORDINATOR_ME,CORD_IDX_targetMode,WHOLE_OBJECT,&targetMode);

                    // Check for invalid addresses in HART 5
                    if(hartFreqCst.pollAddr > 15)
                    {
                        // Reset polling address to 0
                        dummyU8 = 0;
                        result |= hart.unit.Put(me, HART_IDX_pollAddr, WHOLE_OBJECT, (void FAST *) &dummyU8);

                        // write actual status to object loop_current_mode
                        dummyU8 = HART_LOOP_CURRENT_MODE_ENABLED;

                        // Set targetMode accordingly
//                        if(targetMode == CORD_MODE_HART_SETPOINT)
//                        {
//                            targetMode = CORD_MODE_AUTO;
//                        }
//                        else{}
                    }
                    else if(hartFreqCst.pollAddr == 0)
                    {
                        // write actual status to object loop_current_mode
                        dummyU8 = HART_LOOP_CURRENT_MODE_ENABLED;

                        // Set targetMode accordingly
//                        if(targetMode == CORD_MODE_HART_SETPOINT)
//                        {
//                            targetMode = CORD_MODE_AUTO;
//                        }
//                        else{}
                    }
                    else
                    {
                        // write actual status to object loop_current_mode
                        dummyU8 = HART_LOOP_CURRENT_MODE_DISABLED;

//                        // Set targetMode accordingly
//                        if(targetMode == CORD_MODE_AUTO)
//                        {
//                            targetMode = CORD_MODE_HART_SETPOINT;
//                        }
//                        else{}
                    }

                    // Write loopCurrentMode according HART 5 rules
                    result |= Put_T_UNIT(HART_ME, HART_IDX_loopCurrentMode, WHOLE_OBJECT, &dummyU8);

                    // Write traget mode accordingly
//                    if(result == OK)
//                    {
//                        result |= coordinator.unit.Put(COORDINATOR_ME,CORD_IDX_targetMode,WHOLE_OBJECT,&targetMode);
//                    }

                    // If HART revision is set to HART5 remove possible write protection assigned by #71 (Lock Device)
                    // This command is not available in HART5
                    lockDeviceStatus = HART_LOCK_STATUS_NOT_LOCKED;
                    result |= Put_T_UNIT(HART_ME, HART_IDX_lockDeviceSelection, WHOLE_OBJECT, &lockDeviceStatus);

                    lockDeviceStatus = HART_LOCK_STATUS_NOT_LOCKED;
                    result |= Put_T_UNIT(HART_ME, HART_IDX_lockDeviceStatus, WHOLE_OBJECT, &lockDeviceStatus);
                }
            }
            
            // Resume Lock Device protection in case of a non-permanent assignment
            if(!(hartFreqNo.lockDeviceStatus & HART_LOCK_STATUS_PERMANENT_LOCK))
            {
                lockDeviceStatus = HART_LOCK_STATUS_NOT_LOCKED;
                result |= Put_T_UNIT(HART_ME, HART_IDX_lockDeviceSelection, WHOLE_OBJECT, &lockDeviceStatus);

                lockDeviceStatus = HART_LOCK_STATUS_NOT_LOCKED;
                result |= Put_T_UNIT(HART_ME, HART_IDX_lockDeviceStatus, WHOLE_OBJECT, &lockDeviceStatus);
            }
            else{}  // Leave the protection as stored before power cycle or reset
        }
        
        if( (typeOfStartUp & INIT_DATA) != 0)
        {
            result |= hart.unit.LoadRomDefaults(me, (TINT16)HART_DC_IDX_dyn);
            result |= hart.unit.LoadRomDefaults(me, (TINT16)HART_DC_IDX_DYNAMIC);
        }

        // reset squawk counter
        squawkCounter = 0;
        // disable "find device"
        hartDyn.findDeviceSwitch = HART_FIND_DEVICE_DISABLED;
        t_unit_mapper.findDeviceSwitch = HART_FIND_DEVICE_DISABLED;

        // +++ init counter of to send burst messages +++
        // initialize counter of burst messages
        counterBurstMsgToSend = 0;

        // +++ init soft modem information +++
        // for all info elements
//        for (idx = 0; idx < HART_SOFT_MODEM_INFO_LEN; idx++)
//        {
//            // init info
//            softModemInfo[idx] = 0;
//        }
    }

    // Load run-time values into STATIC_INTERRUPT_DATA section
    Initialize_T_UNIT_MAPPER(0xFFFF);

    // Init reset variable
    performDeviceReset = HART_PERFORM_RESET_DEFAULT;

    return result;
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief     return the siez (in byte) of the object
 \author    Martin Dahl
 \date      2006-11-23
 \param     subsystemPtr = pointer to subsystem
 \param     objectIndex = object index.
 \param     attributeIndex = attribute index.
 \warning
 \test
 test-date: 2008-05-07
 \n by: Marcel Schilg
 \n environment: PD30F / Modultest "Modultest_HART_overload"
 \n intention:  instruction coverage
 \n result  modul test: ok
 \n result  Lint Level 3: ok, 2008-05-07
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 GetObjectSize(const T_UNIT SLOW *subsystemPtr,TUSIGN16 objectIndex, TINT16 attributeIndex)
{
    T_DO_OBJECT_DESCRIPTION objDescriptor;
    T_DO_ATTRIB_RANGE attrDescriptor;
    if (attributeIndex == WHOLE_OBJECT)
    {
        //lint -e{522} Marcel Schilg :return value shall not be evaluated
        subsystemPtr->GetObjectDescription(subsystemPtr,objectIndex,&objDescriptor);
        return objDescriptor.objectLength;
    }
    else {
        //lint -e{522} Marcel Schilg :return value shall not be evaluated
        //lint -e{732} Marcel Schilg : loss of sign ok
        subsystemPtr->GetAttributeDescription(subsystemPtr,objectIndex,attributeIndex,&attrDescriptor);
        return cgSimpleTypeLength[attrDescriptor.dataType];
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform HART conversion on the buffer
 \author    Martin Dahl
 \date      2006-11-24
 \param     size = size of the HART object
 \param     ptrValue = ptr to buffer
 \param     index = position of the hart object inside the buffer,
            i.e. object = ptrValue[index .. index+size]
 \warning
 \test
 test-date: 2008-05-07
 \n by: Marcel Schilg
 \n environment: PD30F / Modultest "Modultest_HART_overload"
 \n intention:  instruction coverage
 \n result  modul test: ok
 \n result  Lint Level 3: ok, 2008-05-07
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void ConvertBuffer(TUSIGN8 size,TUSIGN8 FAST* ptrValue,TUSIGN8 index)
{
    #define SWAP(x,y) { x = x^y; y = x^y; x = x^y; }
    switch (size)
    {
    // two byte types
    case 2:
        //convert if not out of bounds of transmit/receive buffer
        SWAP(ptrValue[index+1],ptrValue[index+0]);
        break;

    // four byte types
    case 4:
        SWAP(ptrValue[index+0],ptrValue[index+3]);
        SWAP(ptrValue[index+1],ptrValue[index+2]);
        break;
    default:
         ; // do nothing!
    }
    #undef SWAP
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform HART conversion for a complete T_DATA_OBJECT
 \author    Martin Dahl
 \date      2006-11-24
 \param     subsystemPtr = pointer to subsystem
 \param     objIdx = object index.
 \param     attribIdx = attribute index.
 \param     ptrValue = pointer to object value.
 \warning
 \test
 test-date: 2008-05-07
 \n by: Marcel Schilg
 \n environment: PD30F / Modultest "Modultest_HART_overload"
 \n intention:  instruction coverage
 \n result  modul test: ok
 \n result  Lint Level 3: ok, 2008-05-07
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void HARTConvert(const T_UNIT SLOW *subsystemPtr,
                        TUSIGN16 objectIndex,
                        TINT16 attributeIndex,
                        void FAST* ptrValue)
{
    T_DO_OBJECT_DESCRIPTION objDescriptor;
    T_DO_ATTRIB_RANGE attrDescriptor;
    TUSIGN8 i,size,index;

    if (attributeIndex == WHOLE_OBJECT)
    {
        if (subsystemPtr->GetObjectDescription(subsystemPtr,objectIndex,&objDescriptor) == OK)
        {
           if (objDescriptor.typeQualifier == STRUCT) {
               // STRUCT
               index = 0;
               for (i=0; i < objDescriptor.numberOfAttributes; i++) {
                   size = cgSimpleTypeLength[objDescriptor.aryAttribDataTypes[i]];
                   ConvertBuffer(size,ptrValue,index);
                   index += size;
               }
           }
           else {
               // ARRAY or SIMPLE
               index = 0;
               size = cgSimpleTypeLength[objDescriptor.aryAttribDataTypes[0]];
               for (i=0; i < objDescriptor.objectLength/size; i++) {
                   ConvertBuffer(size,ptrValue,index);
                   index += size;
               }
           }
        }
    }
    else {
        //lint -e{732} Marcel Schilg : loss of sign ok
        if (subsystemPtr->GetAttributeDescription(subsystemPtr,objectIndex,attributeIndex,&attrDescriptor) == OK)
        {
            size = cgSimpleTypeLength[attrDescriptor.dataType];
            ConvertBuffer(size,ptrValue,0);
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Put_HART: HART put overload function.
 \author    Horst Seele, Martin Dahl, Stefan Tabelander
 \date      2005-11-23, 2006-11-23, 2010-10-05
 \param     me = pointer to subsystem
 \param     objectIndex = object index.
 \param     attributeIndex = attribute index.
 \param     ptrValue = pointer to object value.
 \return    T_UNIT error code.
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:

 \n result  modul test:
 \n result  Lint Level 3:
 \n lint check o.k.:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_HART(const T_UNIT SLOW *me,
                  TUSIGN16 objectIndex,
                  TINT16 attributeIndex,
                  void FAST* ptrValue)
{
    TUSIGN8                  idx,byteVariable;
    T_BURSTMSG_MANAGEVALUES         *burstMsgManageDataPtr;

    TUSIGN16                    result = OK;
    TUSIGN8                     burstMsgNumber = 0xFF;
    TUSIGN8                     newHartRevision;
    T_BURSTMSG_CONFVALUES       burstMsgConfInitData;
    
    
    TUSIGN8                     lockDeviceStatus = 0;
    TUSIGN8                     unitType;
    TUSIGN8                     hartBurtInType;


    const T_UNIT SLOW*          subsystemPtr;

    //*********************************
    // debug zero-pointer
    VIP_ASSERT(me);

    // store object
    result = Put_T_UNIT(me, objectIndex, attributeIndex, ptrValue);

    // if store without error
    if (result < WRITE_ERROR)
    {
        // for all possible burst messages
        for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
        {
            // if object index contained in burst message specific confi object reference table
            if (burstMsgConfObjRefTable[idx].confObjId == objectIndex)
            {
                // store used burst message number
                burstMsgNumber = idx;


                // select attribute index
                switch (attributeIndex)
                {
                    case BURSTMSG_CONFVALUES_burstSlot0:
                        // set slot 1 till slot 7 of command 107 to �not used? so the
                        // command interpreter see how many slots of burst command #9 or #33 used
                        byteVariable = SLOT_CODE_NONE;
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot1, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot2, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot3, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot4, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot5, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot6, &byteVariable);
                        result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, BURSTMSG_CONFVALUES_burstSlot7, &byteVariable);
                        break;
                    //-----------------------------------------------------------------------------------------
                    case BURSTMSG_CONFVALUES_modeControlCode:
                        // if burst mode switched off
                        if (*(TUSIGN8*)(ptrValue) == HART_BURSTMODE_OFF)
                        {
                           // pointer of burst message specific management objects
                            burstMsgManageDataPtr = burstMsgManageObjRefTable[idx].burstMsgManageObjPtr;

                            // set counter to value 0
                            burstMsgManageDataPtr->updatePeriodCounter = 0;
                            // set counter to value 0
                            burstMsgManageDataPtr->maximumUpdatePeriodCounter = 0;
                        }
                        break;
                    //-----------------------------------------------------------------------------------------
                    default:
                        break;
                }
            }
        }
        // if no burst command object
        if (burstMsgNumber == 0xFF)
        {
            switch (objectIndex)
            {
              case HART_IDX_objectData:
                   // assumes length, subsystemIndex,objectIndex and attributeIndex have been written
                   // write T_DATA_OBJECT and exit
                   if (hartDyn.objectIndex == HART_IDX_objectData && hartDyn.subsystemIndex == HART_IDX)
                   {
                       result = RULE_VIOLATION_ERR;
                   }
                   else
                   {
                       objectIndex = hartDyn.objectIndex;
                       attributeIndex = hartDyn.attributeIndex;
                       if (hartDyn.subsystemIndex < MEMBER_IDX_NUM_ELEMENTS)
                       {
                           subsystemPtr = GetSubsystemPtr(hartDyn.subsystemIndex);
                           hartDyn.objectDataLength = (TUSIGN8)GetObjectSize(subsystemPtr,objectIndex,attributeIndex);
                           if (hartDyn.objectDataLength <= sizeof(hartDyn.objectData))
                           {
                               HARTConvert(subsystemPtr,objectIndex,attributeIndex,ptrValue);
                               result = subsystemPtr->Put(subsystemPtr,objectIndex,attributeIndex,ptrValue);
                           }
                           else result = RULE_VIOLATION_ERR;
                       }
                       else
                       {
                           result = RULE_VIOLATION_ERR;
                       }
                   }
                   return result;
                //-----------------------------------------------------------------------------------------

                case HART_IDX_memoryData:
                   // assumes memoryLength and memoryAddress have already been written
                   // write data to memory
                  //to do check memoryAddress in the range (0x0400-0x7FFF)
                   if (hartDyn.memoryLength > sizeof(hartDyn.memoryData))
                       return  RULE_VIOLATION_ERR;
                   (void)memcpy((TUSIGN8 FAST*)hartDyn.memoryAddress,ptrValue,hartDyn.memoryLength);
                   // clear the rest of the memory (delete garbage)
                   (void)memset(((TUSIGN8 FAST*)ptrValue)+hartDyn.memoryLength,0,sizeof(hartDyn.memoryData)-hartDyn.memoryLength);
                   return OK;

                //-----------------------------------------------------------------------------------------
                case HART_IDX_pollAddr:
                   // if cmd 6 request contains only polling address
                    if ((receiveBuffer_LAYER7.numByte == 1))
                    {
                        // if polling address == 0
                        if (*(TUSIGN8 *)ptrValue == 0)
                        {
                            // set loop current mode to enabled
                            byteVariable = HART_LOOP_CURRENT_MODE_ENABLED;
                        }
                        else
                        {
                            // set loop current mode to disabled
                            byteVariable = HART_LOOP_CURRENT_MODE_DISABLED;
                        }
                        // write actual status to object loop_current_mode
                        result = hart_Put(HART_IDX_loopCurrentMode, WHOLE_OBJECT, &byteVariable);
                    }
                    break;
                //-----------------------------------------------------------------------------------------
                case HART_IDX_lockDeviceSelection:
                    // If a master is not allowed to put, then this will be handled in the layer7
                    // the call will not come in here then.
                    // If access is allowed the master can always modify the level

                    // Note:
                    // As Lock Device is resumed upon power removal or reset to the device
                    // this part is handled in the initialization in the beginning of this file.

                    // Design Note:
                    // this mechanism does not consider a second layer2. Here any master of the same kind
                    // no matter if first layer 2 or second layer 2 can remove the protection


                   // Set lock device state if any kind of lock device is active
                    if(hartFreqNo.lockDeviceSelection != HART_LOCK_DEVICE_UNLOCKED)
                    {
                        // Indicate that lock device is active
                        lockDeviceStatus |= HART_LOCK_STATUS_LOCKED;

                        // Apply information if primary master sent the message
                        if(receiveBuffer_LAYER7.aryAdr[0] & LAYER2_PRIMARY_MASTER)
                        {
                            // secondary master must be locked,
                            // remember who locked
                            lockDeviceStatus |= HART_LOCK_STATUS_PRIM_MASTER_LOCK;
                        }
                        else{}

                        // Nothing to do if HART_LOCK_DEVICE_TEMPORARY

                        // add permanent lock information
                        if(hartFreqNo.lockDeviceSelection == HART_LOCK_DEVICE_PERMANENT)
                        {
                            lockDeviceStatus |= HART_LOCK_STATUS_PERMANENT_LOCK;
                        }
                        // scope of lock
                        else if(hartFreqNo.lockDeviceSelection == HART_LOCK_DEVICE_ALL)
                        {
                            // The option ALL is always permanent
                            // (will be tested in CAL071, test case from table 21: Device Reset does not Unlock DUT)
                            lockDeviceStatus |= (HART_LOCK_STATUS_ALL_LOCK | HART_LOCK_STATUS_PERMANENT_LOCK);
                        }
                        else{}
                    }
                    else
                    {
                        // Do not set any bit (clear if bit were set before)
                        lockDeviceStatus = HART_LOCK_STATUS_NOT_LOCKED;
                    }

                    // Assign subsystem value the lock status
                    result = Put_T_UNIT(HART_ME, HART_IDX_lockDeviceStatus, WHOLE_OBJECT, &lockDeviceStatus);
                    break;
                    
              case HART_IDX_hartCmdRev:     
              {
                 if(hartFreqCst.hartCmdRev != hartFreqCst.univCmdRev)
                 {
                     // hart revision to switch to
                     newHartRevision = hartFreqCst.hartCmdRev;
                    // store hart revision to work compliant to
                    (void)Put_T_UNIT(me, HART_IDX_univCmdRev, WHOLE_OBJECT, (void FAST*) &newHartRevision);
                    // for all burst messages
                    for(idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
                    {
                      // init burst message configuration data
                      (void)memcpy(&burstMsgConfInitData, &burstMsgConfInitValueTable[idx], sizeof(T_BURSTMSG_CONFVALUES));
                      result = Put_T_UNIT(me, burstMsgConfObjRefTable[idx].confObjId, WHOLE_OBJECT, (void FAST*) &burstMsgConfInitData);
                    }
                  }
                
                break;
              }
            
            /* //2013 01 06 jax from iiWire
            case HART_IDX_attributeIndex:     //comm 123 readObject
                  // clear the hartDyn.objectData
                   (void)memset((TUSIGN8 FAST*)hartDyn.objectData,0,sizeof(hartDyn.objectData));
                  if (hartDyn.subsystemIndex < MEMBER_IDX_NUM_ELEMENTS)
                  {
                    pSub = GetSubsystemPtr(hartDyn.subsystemIndex);  // Get the pointer
                    hartDyn.objectDataLength = (TUSIGN8)GetObjectSize(pSub,hartDyn.objectIndex,hartDyn.attributeIndex);
                    if (hartDyn.objectDataLength <= sizeof(hartDyn.objectData))
                    {
                      result = pSub->Get(pSub,hartDyn.objectIndex,hartDyn.attributeIndex,(void FAST*)hartDyn.objectData);
                    }
                    else 
                      result = RULE_VIOLATION_ERR;
                  }
                  else 
                    result = RULE_VIOLATION_ERR;
                break;
                
             case HART_IDX_memoryLength:  //comm 125 read memory
               if (hartDyn.memoryLength > sizeof(hartDyn.memoryData))
                       return  RULE_VIOLATION_ERR;
                // clear the hartDyn.memoryData
                (void)memset((TUSIGN8 FAST*)hartDyn.memoryData,0,sizeof(hartDyn.memoryData));
                //to do check memoryAddress in the range (0x0400-0x7FFF)
                (void)memcpy((TUSIGN8 FAST*)hartDyn.memoryData,(TUSIGN8 FAST*)hartDyn.memoryAddress,hartDyn.memoryLength);
                   return OK;
                   break;
              */
              
            case HART_IDX_burstUpUnitType:    
                result |= hart_Put((TUSIGN16)HART_IDX_burstUpUnitCode,ATTRIB_1,ptrValue);
                break;
                       
            case HART_IDX_burstUpDataValue:  
                result |= UpdateBurstInData(ptrValue);                
                break;  
                
            case HART_IDX_burstUpTxSelection:
                hartBurtInType = *((TUSIGN8 *)ptrValue);
                if((hartBurtInType == HART_SEC_EXT_TEMPERATURE)||(hartBurtInType == HART_SEC_INTER_TEMPERATURE))
                {
                    unitType = HART_BURST_IN_TEMPERATURE;
                }
                else if(hartBurtInType==HART_SEC_PRESSURE || hartBurtInType==HART_SEC_PRESSURE_ABS)
                {
                    unitType = HART_BURST_IN_PRESSURE;
                }   
                else if(hartBurtInType==HART_SEC_DENSITY)
                {
                    unitType = HART_BURST_IN_DENSITY;
                }
                else if(hartBurtInType==HART_SEC_GAS_CONTENT)
                {
                    unitType = HART_BURST_IN_CONTENT;
                }
                
                if(hartBurtInType!=HART_SEC_WITHOUT)
                {
                    result |= hart_Put((TUSIGN16)HART_IDX_burstUpUnitCode,ATTRIB_1,&unitType);
                    result |= hart_Put((TUSIGN16)HART_IDX_burstUpUnitType,WHOLE_OBJECT,&unitType);
                }
                break;                  
                
            case HART_IDX_burstUpUnitCode:  
                if(attributeIndex == ATTRIB_0)
                {
                    result |= hart_Get((TUSIGN16)HART_IDX_burstUpUnitType,WHOLE_OBJECT,&unitType);
                    result |= hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection,WHOLE_OBJECT,&hartBurtInType);              
                    
                    if(result == OK)
                    {                  
                        if(unitType == HART_BURST_IN_TEMPERATURE && ((hartBurtInType==HART_SEC_EXT_TEMPERATURE)||(hartBurtInType==HART_SEC_INTER_TEMPERATURE)))
                        {
                            result |= coordinator_Put(CDT_IDX_temperature_UnitCode, attributeIndex, ptrValue);
                        }                      
    
                        if(unitType == HART_BURST_IN_PRESSURE && (hartBurtInType==HART_SEC_PRESSURE || hartBurtInType==HART_SEC_PRESSURE_ABS))
                        {
                            result |= mapper_Put(MAPPER_IDX_dvPressureUnitCode, attributeIndex, ptrValue);
                        }
                        
                        if(unitType == HART_BURST_IN_DENSITY && hartBurtInType==HART_SEC_DENSITY)
                        {
                            result |= mapper_Put(MAPPER_IDX_dvDensityUnitCode, attributeIndex, ptrValue);  
                        }   
                        
                        if(unitType == HART_BURST_IN_CONTENT && hartBurtInType==HART_SEC_GAS_CONTENT)
                        {
                            result |= mapper_Put(MAPPER_IDX_dvContentUnitCode, attributeIndex, ptrValue);  
                        }
                    }                
                }
                break;

             case HART_IDX_ResetSoftWriteProtection:
                if(memcmp("write protection off", ptrValue, strlen("write protection off")) == 0)
			 	{
                      TUSIGN8 writeProtection = ELETTRONICSERVICES_WRITE_PROTECTION_OFF;                      
                      result = elettronicServices_Put(ELETTRONICSERVICES_IDX_swWriteProtection,WHOLE_OBJECT,&writeProtection);
			 	}
				else
				{
					result = (TUSIGN16)WRONG_STATE_ERR;
				}
				break;                 
                
                default:
                    break;
            }
        }
    }
    else
    {
        switch (objectIndex)
        {
            case HART_IDX_lockDeviceSelection:
                // Map to HART 7 response codes
                if(result == RULE_VIOLATION_ERR)
                {
                    // RANGE_TOO_LOW_ERR will be mapped to HART response code 10 (HART_RESP_ERR_INVALID_LOCK_CODE)
                    result = RANGE_TOO_LOW_ERR;
                }
                else{}
                break;
            default:
                break;
        }
    }

    // Update run-time values in STATIC_INTERRUPT_DATA section
    VIP_ASSERT(objectIndex != 0xFFFF);
    Initialize_T_UNIT_MAPPER(objectIndex);

    return(result);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  gets attributeIndex of the data-object objectIndex; only in "INITIALIZED"
         comment line 587-601,line 620, line 627
 \author Martin Dahl,Eric-Jiping.li
 \date   2006-11-23,2011-04-12
 \param  me = pointer to subsystem
 \param  objectIndex = object index.
 \param  attributeIndex = attribute index.
 \param  ptrValue = pointer to object value.
 \return T_UNIT error code.
 \warning
 \test
 test-date: 2008-06-19
 \n by: Marcel Schilg
 \n environment: PD30F / Modultest "Modultest_HART_overload"
 \n intention:  instruction coverage
                but HART_IDX_memoryData is only sight check
 \n result  modul test: ok
 \n result  Lint Level 3: ok, 2008-06-19
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_HART(const T_UNIT SLOW*me,
                  TUSIGN16 objectIndex,
                  TINT16 attributeIndex,
                  void FAST* ptrValue)
{
    TUSIGN8     setpointStatus=0, positionStatus=0, currentStatus=0;

    TUSIGN16    result = OK;

    const T_UNIT SLOW* subsystemPtr;

    // init value parameter
    *((TUSIGN8 *)ptrValue) = 0;
//**********************************************************
    switch (objectIndex)
    {
        case HART_IDX_deviceRevisionLevel:
//            // Device Revision is mapped as the version for HART7 must be greater than the version for HART5
//            if(hartFreqCst.univCmdRev == 5)
//            {
//               // (void)application.unit.Get(APPLICATION_ME, APP_IDX_deviceRevisionHART5, WHOLE_OBJECT,ptrValue);
//            }
//            else
//            {
//                //(void)application.unit.Get(APPLICATION_ME, APP_IDX_deviceRevisionHART7, WHOLE_OBJECT,ptrValue);
//            }
           break;

        case HART_IDX_extendedDeviceStatus:
            // @@adjust
            // if "maintenance required"
//            if(application.GetDeviceStatus() & APP_NAMUR_MAINTENANCE)
//            {
//                // set "maintenence required" in extended device status
//                *((TUSIGN8 *)ptrValue) |= HART_MAINTENANCE_REQUIRED;
//            }
//            else
//            {
//                // reset "maintenence required" in extended device status
//                *((TUSIGN8 *)ptrValue) &= ~HART_MAINTENANCE_REQUIRED;
//            }
//            // @@adjust
//            // read status of all device variables
//            (void)HARTGetObjects(APP_IDX, APP_IDX_pvStatus, &setpointStatus);
//            (void)HARTGetObjects(APP_IDX, APP_IDX_svStatus, &positionStatus);
//            (void)HARTGetObjects(APP_IDX, APP_IDX_pvLoopCurStatus, &currentStatus);
            // if status set in any device variable
            if(   (setpointStatus != 0)
               || (positionStatus != 0)
               || (currentStatus != 0))
            {
                //set "device variable alert" in extended device status
                *((TUSIGN8 *)ptrValue) |= HART_DEVICE_VARIABLE_ALERT;
            }
            else
            {
                //reset "device variable alert" in extended device status
                *((TUSIGN8 *)ptrValue) &= ~HART_DEVICE_VARIABLE_ALERT;
            }
            break;
        //-----------------------------------------------------------------------------------------
        case HART_IDX_standardizedStatus0:
            // @@adjust
            // prepare standardized status 0
           // *((TUSIGN8 *)ptrValue) = application.GetStandardizedStatus0();
            break;
        //-----------------------------------------------------------------------------------------
        case HART_IDX_analogChannelFixed:
            // collect status of all analog channels:
            // analog channel 1 <->  current in
            // @@adjust
           // (void)HARTGetObjects(APP_IDX, APP_IDX_pvLoopCurStatus, &currentStatus);
            // if "fixed" set in current in status
            if((currentStatus & 0xC0) == 0x80)
            {
                //set "analog channel 1 fixed" in extended device status
                *((TUSIGN8 *)ptrValue) |= HART_ANALOG_CHANNEL_1_FIXED;
            }
            else
            {
                //reset "analog channel 1 fixed" in extended device status
                *((TUSIGN8 *)ptrValue) &= ~HART_ANALOG_CHANNEL_1_FIXED;
            }
            break;
        //-----------------------------------------------------------------------------------------
        case HART_IDX_pvt_Label_Dist_Code:
            // if device works compliant to hart rev 7
            if (hartFreqCst.univCmdRev == 7)
            {
                // get object pvt_Label_Dist_Code must return 'not used (250)'
                *((TUSIGN8 *)ptrValue) = 250;
            }
            break;
        //-----------------------------------------------------------------------------------------
        case HART_IDX_objectDataLength:   
            if (hartDyn.objectIndex == HART_IDX_objectData && hartDyn.subsystemIndex == HART_IDX)
            {
                result = RULE_VIOLATION_ERR;
            }
            else
            {
               objectIndex = hartDyn.objectIndex;
               attributeIndex = hartDyn.attributeIndex;
               if (hartDyn.subsystemIndex < MEMBER_IDX_NUM_ELEMENTS)
               {
                   subsystemPtr = GetSubsystemPtr(hartDyn.subsystemIndex); 
                   TUSIGN8 objectDataLength;
                   objectDataLength = (TUSIGN8)GetObjectSize(subsystemPtr,objectIndex,attributeIndex);
                   if (objectDataLength <= sizeof(hartDyn.objectData))
                   {
                        hartDyn.objectDataLength = objectDataLength;
                       *(TUSIGN8 *)ptrValue = hartDyn.objectDataLength;                                              
                   }
                   else 
                   {
                       result = RULE_VIOLATION_ERR; 
                   }
               }
               else
               {
                 result = RULE_VIOLATION_ERR;
               }
            }          
            break;
          
        case HART_IDX_objectData:
           // assumes length, subsystemIndex,objectIndex and attributeIndex have been written
           // write T_DATA_OBJECT and exit
            if (hartDyn.objectIndex == HART_IDX_objectData && hartDyn.subsystemIndex == HART_IDX)
            {
                result = RULE_VIOLATION_ERR;
            }
            else
            {
               objectIndex = hartDyn.objectIndex;
               attributeIndex = hartDyn.attributeIndex;
               if (hartDyn.subsystemIndex < MEMBER_IDX_NUM_ELEMENTS)
               {
                   subsystemPtr = GetSubsystemPtr(hartDyn.subsystemIndex);
                   // clear ptrValue first
                   (void)memset(ptrValue,0,sizeof(hartDyn.objectData));
                   hartDyn.objectDataLength = (TUSIGN8)GetObjectSize(subsystemPtr,objectIndex,attributeIndex);
                   if (hartDyn.objectDataLength <= sizeof(hartDyn.objectData))
                   {
                       if (subsystemPtr->Get(subsystemPtr,objectIndex,attributeIndex,ptrValue) == OK)
                       {
                           HARTConvert(subsystemPtr,objectIndex,attributeIndex,ptrValue);
                       }
                       else result = RULE_VIOLATION_ERR;
                   }
                   else result = RULE_VIOLATION_ERR;
               }
               else
               {
                 result = RULE_VIOLATION_ERR;
               }
            }
            break;
        //-----------------------------------------------------------------------------------------
        case HART_IDX_memoryData:
            // assumes memoryLength and memoryAddress have already been written
            // read from memory
            if (hartDyn.memoryLength > sizeof(hartDyn.objectData))
                return RULE_VIOLATION_ERR;
            // clear ptrValue first
            (void)memset(ptrValue,0,sizeof(hartDyn.objectData));
            // get mem
            (void)memcpy(ptrValue,(TUSIGN8 FAST*)hartDyn.memoryAddress,hartDyn.memoryLength);
            break;
        //-----------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------
        case HART_IDX_pvtLabelDistCode_Command15:
            if(hartFreqCst.univCmdRev == 5)
            {
                //
                hartDyn.pvtLabelDistCode_Command15 = hartCst.mfrId;
            }
            else // Must be 7 then...
            {
                hartDyn.pvtLabelDistCode_Command15 = HART_VALUE_USED_IN_HART7;
            }

            result = Get_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            break;
        //--------------------------------*************123*****************-------------------------------
    
            
        default:
            // get object value
            result = Get_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
            break;
    }
    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  
 \author 
 \date   
 \param  TFLOAT* ptrValue
 \return TUSIGN16 result.
 \warning
 \test
 test-date: 
 \n by: 
 \n environment: 
 \n intention:  
 \n result  modul test: 
 \n result  Lint Level 3: 
 \n lint check o.k.: 
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 UpdateBurstInData(const TFLOAT* ptrValue)
{   
    TUSIGN8 unitCode[2];
    TFLOAT tempBaseValue = 0.0f;
    TUSIGN16 result = OK;

    tempBaseValue = *( (const TFLOAT*)ptrValue);
    result = hart_Get((TUSIGN16)HART_IDX_burstUpUnitCode,WHOLE_OBJECT, &unitCode);    
    
    if( result == OK )
    { 
        if(unitsOfMeasure.ConvertToBaseUnit(unitCode[1],unitCode[0],&tempBaseValue))
        {
            result = OK;      
            result |= hart_Put((TUSIGN16)HART_IDX_burstUpBaseValue, 0, &tempBaseValue);
        }  
        else
        {
            result = FATAL_ERROR;
        }        
    }
    else
    {
        result = FATAL_ERROR;
    }
           
    return result;  
}
