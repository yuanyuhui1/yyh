//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System                                 Generic
 Subsystem                              HMI
 Description            Write protection control for the HMI subsystem. Is responsible for evaluating & returning the write protection settings

 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#pragma diag_suppress=Go003

#include "includeList_Implementation.h"
#include <intrinsics.h>

#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "keystateDefinitions.h"
#include "FrameTableDefinitions.h"

#include "hmi_types.h"
#include "hmi_attributes.h"
#include "Hmi/Interface/HMI_idx.h"
#include "Hmi/Interface/HMI.h"

#include "FrameTable.h"

#ifdef COMMON_TOP_WORKS_NEW_FEATURES

    #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && !defined(LOCAL_DUMMY_ARM)
        #include "ARM/Interface/arm.h"

        #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            #if !defined(ARM_VERSION) || ARM_VERSION < 0x010013
                #error HMI requires Access Rights Manager Version 1.0.13 or newer
            #endif
        #endif
    #endif

    #include "writeProtect.h"
    #include "includeList_writeProtect.h"

TBOOL WriteProtect_AccessingPassword(ATTRIB_IDENTIFIER_TYPE Data);
TBOOL WriteProtect_GetUserLock_Specific(TBOOL DataSpecific, TUSIGN8 ssIdx_of_Data, TUSIGN8 objIdx_of_Data, TUSIGN8 attribIdx_of_Data);
TBOOL WriteProtect_GetARMLock(ATTRIB_IDENTIFIER_TYPE Data);

#else   // else condition of #ifdef COMMON_TOP_WORKS_NEW_FEATURES

    #if defined(APA592_SPECIAL_FEATURES_HMI) || defined(MILE2_266_SPECIAL_FEATURES_HMI)
    #include "ElettronicServices/Interface/ElettronicServices.h"
    #include "ElettronicServices/Interface/ElettronicServices_idx.h"
    #endif

    #ifdef MILE2_266_SPECIAL_FEATURES_HMI
    //#include "..\..\system\interface\microcontroller.h"
    #endif

    #ifndef OVERRIDE_HART_WRITE_PROTECT
    #include "HART/Layer7/Source/hart_arm.h"
    #include "HART/Interface/Hart_idx.h"
    #endif

    #ifndef OVERRIDE_FF_WRITE_PROTECT
    #include "../../ff/interface/ff.h"
    #include "../../ff/interface/ff_idx.h"
    #endif

    #ifndef OVERRIDE_PA_WRITE_PROTECT
    #include "../../PROFIBUS/interface/PROFIBUS.h"
    #include "../../PROFIBUS/interface/PROFIBUS_idx.h"
    #endif

    #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && !defined(LOCAL_DUMMY_ARM)
    #include "ARM/Interface/arm.h"
    #endif

    #include "writeProtect.h"

#endif  // end of else condition of #ifdef COMMON_TOP_WORKS_NEW_FEATURES


#ifdef COMMON_TOP_WORKS_NEW_FEATURES

//--------------------------------------------------------------------------------------------------
TBOOL WriteProtect_HMICanAccess(ATTRIB_IDENTIFIER_TYPE Data, TUSIGN8 ReadNotWrite)
{
#ifdef OVERRIDE_WRITE_PROTECTION_DEVT_ONLY
    return eTRUE;
#else

    TBOOL AccessAllowed = eFALSE;

    if (ReadNotWrite)
    {
        // Read access
        AccessAllowed = eTRUE;
    }
    else
    {
        AccessAllowed = eTRUE;  // assume access allowed

        // write access
        #ifdef HMI_SUPPORTS_MATRIX_FRAMES
        FrameTable_ModifyDataForWriteProtection(&Data);
        #endif

        if (!WriteProtect_AccessingPassword(Data))
        {
            if (FrameTableIsEditDeniedBySpecialOp())
            {
                AccessAllowed = eFALSE;
            }
            else if (WriteProtect_GetUserLock(eTRUE, Data.ssIdx, Data.objIdx, Data.attribIdx))
            {
                AccessAllowed = eFALSE;
            }
            else if (WriteProtect_GetARMLock(Data))
            {
                AccessAllowed = eFALSE;
            }
            else
            {
                asm("nop");
            }
        }
    }
    return AccessAllowed;
#endif
}

//--------------------------------------------------------------------------------------------------
TBOOL WriteProtect_AccessingPassword(ATTRIB_IDENTIFIER_TYPE Data)
{
    TBOOL AccessingPassword = eFALSE;

    #ifdef HMI_SUPPORTS_SECURITY_ACCESS
    if ((Data.ssIdx == HMI_IDX) && (Data.objIdx == HMI_IDX_Password))
    {
        AccessingPassword = eTRUE;
    }
    #endif
    return AccessingPassword;
}

//--------------------------------------------------------------------------------------------------
TBOOL WriteProtect_GetUserLock(TBOOL DataSpecific, TUSIGN8 ssIdx_of_Data, TUSIGN8 objIdx_of_Data, TUSIGN8 attribIdx_of_Data)
{
    TBOOL UserLock = eTRUE;

    if (WriteAccessAllowed(DataSpecific,ssIdx_of_Data,objIdx_of_Data,attribIdx_of_Data))
    {
        UserLock = eFALSE;
    }
    return UserLock;
}

//--------------------------------------------------------------------------------------------------
TBOOL WriteProtect_GetARMLock(ATTRIB_IDENTIFIER_TYPE Data)
{
    TBOOL AccessAllowed = eTRUE;  // assume data available

    #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && !defined(LOCAL_DUMMY_ARM)

        const T_ARM* pARM = (const T_ARM*)GetSubsystemPtr(ARM_IDX);

        if (Data.attribIdx == 0xFF)
        {
            T_UNIT const SLOW *pSub;
            T_DO_OBJECT_DESCRIPTION ObjectDescriptor;
            TINT16 i;

           // Get the pointer to the sub-system, get the object description
            pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)Data.ssIdx);
            (void)(pSub->GetObjectDescription(pSub, (TUSIGN16)Data.objIdx, &ObjectDescriptor));

            i = (TINT16)ObjectDescriptor.numberOfAttributes - 1;

            while ((i >= 0) && (AccessAllowed))
            {
                if ((TUSIGN16)OK != pARM->CheckAccessRightsSRV((T_SUBSYSTEM_IDX)HMI_IDX,       // CallerId Subsystem index relating to the Client
                                                               (T_SUBSYSTEM_IDX)Data.ssIdx,    // Subsystem index relating to the subsystem in which the data is held
                                                               (TUSIGN16)Data.objIdx,          // Object index for the data in the target subsystem
                                                               (TUSIGN16)i,                    // Attribute id for the data within the target object
                                                               (TUSIGN8)READ_WRITE_ACCESS))    // Required level of access (code) for the requesting user
                {
                    AccessAllowed = eFALSE;
                    i = -1;
                }
                else
                {
                    i--;
                }
            }
        }
        else
        {
            if ((TUSIGN16)OK != pARM->CheckAccessRightsSRV((T_SUBSYSTEM_IDX)HMI_IDX,       // CallerId Subsystem index relating to the Client
                                                           (T_SUBSYSTEM_IDX)Data.ssIdx,    // Subsystem index relating to the subsystem in which the data is held
                                                           (TUSIGN16)Data.objIdx,          // Object index for the data in the target subsystem
                                                           (TUSIGN16)Data.attribIdx,       // Attribute id for the data within the target object
                                                           (TUSIGN8)READ_WRITE_ACCESS))    // Required level of access (code) for the requesting user
            {
                AccessAllowed = eFALSE;
            }
        }
    #endif

    TBOOL ARM_Lock = AccessAllowed ? eFALSE : eTRUE;
    return ARM_Lock;
}

#else   // else condition of #ifdef COMMON_TOP_WORKS_NEW_FEATURES

//--------------------------------------------------------------------------------------------------
TUSIGN8 WriteProtect_GetHARTLock(void)
{
#ifdef ACCESS_MODULE_TEST
    return hart_lock;
#else

    TUSIGN8 returnVal = HMI_LOCAL_OP_DISABLED;

    TUSIGN8 writeProtect_HART, writeProtect_FF, writeProtect_PA;


    #ifdef OVERRIDE_HART_WRITE_PROTECT
        writeProtect_HART = HMI_LOCAL_OP_ENABLED;
    #else
        writeProtect_HART = HMI_LOCAL_OP_DISABLED;

        T_UNIT const SLOW *pSub_HART = GetSubsystemPtr(HART_IDX);
        if (pSub_HART->Get(pSub_HART,HART_IDX_writeProtection,0,(void FAST *)&writeProtect_HART) == OK)
        {
            if (writeProtect_HART == HART_NOT_LOCKED)
            {
                writeProtect_HART = HMI_LOCAL_OP_ENABLED;
            }
        }
    #endif

    #ifdef OVERRIDE_FF_WRITE_PROTECT
        writeProtect_FF = HMI_LOCAL_OP_ENABLED;
    #else
        writeProtect_FF = HMI_LOCAL_OP_DISABLED;

        T_UNIT const SLOW *pSub_FF = GetSubsystemPtr(FF_IDX);
        if (pSub_FF->Get(pSub_FF,FF_IDX_FB_RB_WRITE_LOCK,0,(void FAST *)&writeProtect_FF) == OK)
        {
            if (writeProtect_FF == FF_UNLOCKED)
            {
                writeProtect_FF = HMI_LOCAL_OP_ENABLED;
            }
        }
    #endif

    #ifdef OVERRIDE_PA_WRITE_PROTECT
        writeProtect_PA = HMI_LOCAL_OP_ENABLED;
    #else
        T_UNIT const SLOW *pSub_PA = GetSubsystemPtr(PROFIBUS_IDX);
        if (pSub_PA->Get(pSub_PA,PROFIBUS_IDX_writeProtection,0,(void FAST *)&writeProtect_PA) == OK)
        {
    		if (writeProtect_PA == PROFIBUS_UNLOCKED)
            {
                #ifdef MILE2_266_SPECIAL_FEATURES_HMI
                if (PUSH_BUTTON_WRITE_PROTECT_IS_OFF)
                #endif
                {
                    writeProtect_PA = HMI_LOCAL_OP_ENABLED;
                }
            }
        }
    #endif

    if ((writeProtect_HART == HMI_LOCAL_OP_ENABLED)
     && (writeProtect_FF   == HMI_LOCAL_OP_ENABLED)
     && (writeProtect_PA   == HMI_LOCAL_OP_ENABLED))
    {
        returnVal = HMI_LOCAL_OP_ENABLED;
    }
    else
    {
        returnVal = HMI_LOCAL_OP_DISABLED;
    }
    return returnVal;

#endif
}

//--------------------------------------------------------------------------------------------------
TUSIGN8 WriteProtect_GetHMILock(void)
{
#ifdef ACCESS_MODULE_TEST
    return hmi_lock;
#else
    #ifdef OVERRIDE_HMI_WRITE_PROTECT
        return HMI_LOCAL_OP_ENABLED;
    #else
        return SFD_SIMPLE.localOperationEnable;
    #endif
#endif
}

//--------------------------------------------------------------------------------------------------

TBOOL WriteProtect_HMICanAccess(ATTRIB_IDENTIFIER_TYPE Data, TUSIGN8 ReadNotWrite)
{
#ifdef OVERRIDE_WRITE_PROTECTION_DEVT_ONLY
    return eTRUE;
#else

    TBOOL AccessAllowed = eFALSE;
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
        TBOOL PasswordEdit = eFALSE;
#endif

    if (ReadNotWrite)
    {
        // Read
        AccessAllowed = eTRUE;
    }
    else
    {
        // Write
        if ((Data.ssIdx == HMI_IDX) && (Data.objIdx == HMI_IDX_Password))
        {
#ifdef HMI_SUPPORTS_SECURITY_ACCESS
            PasswordEdit = eTRUE;
            AccessAllowed = eTRUE;
#endif
        }
        else if ((Data.ssIdx == HMI_IDX) && (Data.objIdx == HMI_IDX_localOperationEnable))
        {
            #if defined(OVERRIDE_HMI_WRITE_PROTECT) && !defined(ACCESS_MODULE_TEST)
            AccessAllowed = eTRUE;
            #else
            // Write to HMI write protect - not allowed
            AccessAllowed = eFALSE;
            #endif
        }
        #if defined(ACCESS_MODULE_TEST) || !defined(OVERRIDE_HART_WRITE_PROTECT)

        #if defined(APA592_SPECIAL_FEATURES_HMI) || defined(MILE2_266_SPECIAL_FEATURES_HMI)
        else if ((Data.ssIdx == ELETTRONICSERVICES_IDX)
              && (Data.objIdx == ELETTRONICSERVICES_IDX_swWriteProtection)
              && ((Data.attribIdx == 0) || (Data.attribIdx == 0xFF)))
        #else
        else if ((Data.ssIdx == HART_IDX)
              && (Data.objIdx == HART_IDX_writeProtection)
              && ((Data.attribIdx == 0) || (Data.attribIdx == 0xFF)))
        #endif
        {
            // Write to HART write protect - dependant upon HMI write protect
            if (WriteProtect_GetHMILock() == HMI_LOCAL_OP_ENABLED)
            {
                #ifdef MILE2_266_SPECIAL_FEATURES_HMI
                if (PUSH_BUTTON_WRITE_PROTECT_IS_OFF)
                #endif
                {
                    AccessAllowed = eTRUE;
                }
            }
        }
        #endif
        else
        {
            TUSIGN8 GetHMILock = WriteProtect_GetHMILock();
            TUSIGN8 GetHARTLock = WriteProtect_GetHARTLock();

            // Write to All other variables - dependant upon both HMI write protect and HART write Protect
            if ((GetHMILock == HMI_LOCAL_OP_ENABLED) && (GetHARTLock == HMI_LOCAL_OP_ENABLED))
            {
                AccessAllowed = eTRUE;
            }
        }

    }

    #ifdef EDP300_SPECIAL_FEATURES_HMI
        if (AccessAllowed)
        {
            AccessAllowed = FrameTableIsWriteProtected(Data) ? eFALSE : eTRUE;
        }
    #else

        if (AccessAllowed)
        {
            AccessAllowed = FrameTableIsEditDeniedBySpecialOp() ? eFALSE : eTRUE;
        }

        #if defined(HMI_SUPPORTS_SECURITY_ACCESS) && !defined(LOCAL_DUMMY_ARM)
        if ((AccessAllowed) && (!PasswordEdit))
        {
            //AccessAllowed is dependant upon ARM
            const T_ARM* pARM = (const T_ARM*)GetSubsystemPtr(ARM_IDX);

            if (Data.attribIdx == 0xFF)
            {
                T_UNIT const SLOW *pSub;
                T_DO_OBJECT_DESCRIPTION ObjectDescriptor;
                TINT16 i;

                // Get the pointer to the sub-system, get the object description
                pSub = GetSubsystemPtr((T_SUBSYSTEM_IDX)Data.ssIdx);
                (void)(pSub->GetObjectDescription(pSub, (TUSIGN16)Data.objIdx, &ObjectDescriptor));

                i = (TINT16)ObjectDescriptor.numberOfAttributes - 1;

                while ((i >= 0) && (AccessAllowed))
                {
                    if ((TUSIGN16)OK != pARM->CheckAccessRightsSRV((T_SUBSYSTEM_IDX)HMI_IDX,       // CallerId Subsystem index relating to the Client
                                                                   (T_SUBSYSTEM_IDX)Data.ssIdx,    // Subsystem index relating to the subsystem in which the data is held
                                                                   (TUSIGN16)Data.objIdx,          // Object index for the data in the target subsystem
                                                                   (TUSIGN16)i,                    // Attribute id for the data within the target object
                                                                   (TUSIGN8)READ_WRITE_ACCESS))    // Required level of access (code) for the requesting user
                    {
                        AccessAllowed = eFALSE;
                        i = -1;
                    }
                    else
                    {
                        i--;
                    }
                }
            }
            else
            {
                if ((TUSIGN16)OK != pARM->CheckAccessRightsSRV((T_SUBSYSTEM_IDX)HMI_IDX,       // CallerId Subsystem index relating to the Client
                                                               (T_SUBSYSTEM_IDX)Data.ssIdx,    // Subsystem index relating to the subsystem in which the data is held
                                                               (TUSIGN16)Data.objIdx,          // Object index for the data in the target subsystem
                                                               (TUSIGN16)Data.attribIdx,       // Attribute id for the data within the target object
                                                               (TUSIGN8)READ_WRITE_ACCESS))    // Required level of access (code) for the requesting user
                {
                    AccessAllowed = eFALSE;
                }
            }
        }
        #endif
    #endif

    return AccessAllowed;
#endif
}

#endif  // end of else condition of #ifdef COMMON_TOP_WORKS_NEW_FEATURES
