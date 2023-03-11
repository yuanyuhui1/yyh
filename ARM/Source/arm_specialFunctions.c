//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module			arm_specialFunctions.c
 Description    Project-dependent functions for ARM component

                This is for individual projects to implement their own variants of ARM functions.
				The function names should remain the same; the content is project-dependent.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"

#ifdef FRAMEWORK_VERSION_2_1_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif //FRAMEWORK_VERSION_2_1_0
#ifdef FRAMEWORK_VERSION_2_2_0
  #include "../../common/interface/common_type.h"
  #include "../../common/interface/assert.h"
#endif //FRAMEWORK_VERSION_2_2_0
#ifdef FRAMEWORK_VERSION_2_3_1
  #include "System/Interface/common_type.h"
  #include "System/Interface/assert.h"
#endif //FRAMEWORK_VERSION_2_3_1
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"		// to identify client subsystem(s)
#include "nv_mem/Interface/nv_mem.h"

#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"

#include "arm_types.h"
#include "arm_attributes.h"
#include "arm_specialFunctions.h"
#include "arm_execute.h"

#ifdef CONTROLLER_SPECIAL_FEATURES
	// nv_mem header included so passwords can be saved in non-vol
	#ifdef FRAMEWORK_VERSION_2_1_0
		#include "nv_mem/Interface/nv_mem.h"
	#endif //FRAMEWORK_VERSION_2_1_0
	#ifdef FRAMEWORK_VERSION_2_2_0
		#include "nv_mem/Interface/nv_mem.h"
	#endif //FRAMEWORK_VERSION_2_2_0
	#ifdef FRAMEWORK_VERSION_2_3_0
		#include "nv_mem/Interface/nv_mem.h"    //change by Spring for 2-wire
	#endif //FRAMEWORK_VERSION_2_3_0
#endif //CONTROLLER_SPECIAL_FEATURES

#ifdef EMF_SPECIAL_FEATURES
	#include "HART/Interface/Hart_idx.h"				// to isolate object slot0
	// nv_mem header included so passwords can be saved in non-vol
	// data_area_types and nvmeminterface includes are for EMF overall variable writeability check
	#ifdef FRAMEWORK_VERSION_2_1_0
		#include "nv_mem/Interface/nv_mem.h"
		#include "../../nv_mem/interface/data_area_types.h"
	#endif //FRAMEWORK_VERSION_2_1_0
	#ifdef FRAMEWORK_VERSION_2_2_0
		#include "nv_mem/Interface/nv_mem.h"
		#include "../../nv_mem/interface/data_area_types.h"
	#endif //FRAMEWORK_VERSION_2_2_0
	#ifdef FRAMEWORK_VERSION_2_3_0
		#include "../../services/interface/nv_mem.h"
		#include "../../services/interface/data_area_types.h"
	#endif //FRAMEWORK_VERSION_2_3_0
	#include "../../nvmeminterface/interface/nvmeminterface.h"
	#include "../../nvmeminterface/interface/nvmeminterface_idx.h"

	#include "act_oiml.h"
#endif //EMF_SPECIAL_FEATURES

#ifdef DEVICE_SUBSYSTEM_PRESENT
	#include "../../device/interface/device.h"				// to access the device's identity-
	#include "../../device/interface/device_idx.h"			// -number, used in hashing algorithm
#endif //DEVICE_SUBSYSTEM_PRESENT


extern TUSIGN8 hmiAccessLevel; 

//-------------------------------------------------------------------------------------------------
// Arm_specialFunction.c is required in order to fulfill project-specific ARM functionality.
// Add your own project-specific function-variant code to the named functions within this file.
//
// You may remove any project-specific code not applicable to your own project.
//
// It is recommended that project definition macros are used to make it clear which project this
// code-variant is intended for.
//-------------------------------------------------------------------------------------------------
TUSIGN32 StringToNumeric_ARM(TCHAR* );

//-------------------------------------------------------------------------------------------------
// Local functions of ARM_specialFunctions
//-------------------------------------------------------------------------------------------------
/*!
 \brief	Called by Pre/PostLoginCheck functions.
 \      This method translates a 0 - 9, A - F password to its numeric value.
		Created because atoi() cannot handle the A - F characters.
 \author	Peter Vince
 \date   2007-08-17

 \param[in] pointer to supplied password; should be NULL terminated.

 \return : numeric equivalent of password
 \warning: none
 \test
 \n by: Peter Vince
 \n result module test: 2007-08-17 OK				
 \n result Lint Level 3: free		
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN32 StringToNumeric_ARM(TCHAR* suppliedPassword)
{
	const TUSIGN8 asciiLookup[] = {
	0x30,  0, 0x31,  1, 0x32,  2, 0x33,  3, 0x34,  4, 0x35,  5, 0x36,  6, 0x37,  7,
	0x38,  8, 0x39,  9, 0x41, 10, 0x42, 11, 0x43, 12, 0x44, 13, 0x45, 14, 0x46, 15};

    TINT16 i = 0;
    TUSIGN16 j = 0;
    TINT16 k = 0;
    TUSIGN32 l = 0;
    TUSIGN16 m = 0;
    TUSIGN16 tableIndex = 0;
    TUSIGN32 numericPassword = 0;
    TUSIGN8 password[LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL];

	// In case the supplied password is too short and/or not NULL terminated...
    (void)memset((void*)password, '\0', (LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL));
    // ...copy to our bigger buffer and work with that one.
	(void)memcpy((void*)password, (void const*) suppliedPassword, (LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL)); 

	// Copy the password, identify the last valid character
    for (i = 0; i < LENGTH_OF_INPUT_PASSWORD; i++)
	{
		if (password[i] == 0)
		{
			break;
		}
	}
	// Point to the last valid character.
	i--;
	// Until the whole string is parsed...
	for (j = 0; i >= 0; i--, j+=4)
	{
		// ...for each character, calculate the binary multiplier (index increases by 4 each loop).
		for (k = 0, m = 2; k <= j; k++)
		{
			if (k == 0)
			{
				l = 1;
			}
			else
			{
				l = m * l;
			}
		}
		tableIndex = 0;
		// Locate the character in the table, get the numerical equivalent, multiply by 2^(index)
		// and add the result to the running total.
		do
		{
			if (asciiLookup[tableIndex] == password[i])
			{
				numericPassword += ((asciiLookup[tableIndex + 1]) * l);
				break;
			}
			tableIndex += 2;
		}
		while (tableIndex < 32);
	}
	// Return the numerical value of the password.
	return numericPassword;
}
// end StringToNumeric_ARM()

//-------------------------------------------------------------------------------------------------
// Public functions of ARM_specialFunctions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by Coordinator and Initialize methods in order to set all client access levels.
 \param  none
 \author Peter Vince
 \date 2011-01-13

 \return None.
 \warning None
 \test
 test-date: 2011-01-13
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
void AllClientAccessLevelInit_ARM(void)
{
    // ARM data to initialise:
    #ifdef HMI_CLIENT_PRESENT
    hmiAccessLevel = HmiAccessLevelInit_ARM();		// ARM Special Function
    #endif

    #ifdef HART_CLIENT_PRESENT
    hartAccessLevel = HartAccessLevelInit_ARM();	// ARM Special Function
    #endif
    // ..add any future clients' access-level initialization here:
}
// end AllClientAccessLevelInit_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by PreLogInCheck_ARM and Get_ARM methods in order to set RO/Custody switch status.
 \param  none
 \author Peter Vince
 \date 2010-08-24

 \return Boolean: eFALSE = sensor not MID compliant; eTRUE sensor is MID compliant.
 \warning None
 \test
 test-date: 2010-08-24
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TBOOL GetSpecial_ARM(void)
{
	// For MID compliant checks: if the MID status is "OIML Cert Class 1 or 2" the block Service login
	TBOOL sensorCalMIDCompliant = eFALSE;

	#ifndef LOCALSERVER_SELECT

		#if defined DEVICE_SUBSYSTEM_PRESENT && defined ARM_MID_PRESENT
			// Requirement is to get the sensor calibration type to determine if access to "sensitive"
			// items should be blocked to all but service-level users.
			sensorCalMIDCompliant = device.MeterIsOIMLCompliantSRV();
		#endif //DEVICE_SUBSYSTEM_PRESENT && ARM_MID_PRESENT

		#ifdef ARM_MID_PRESENT
		if (sensorCalMIDCompliant == eTRUE)
		{
			// For MID Cert'd instruments, "RO-switch" in fact reflects whether MID active.
			// Therefore the RO switch status should be overriden.
			armDynamic.readOnlySwitchStatus = ARM_READ_ONLY_SWITCH_INACTIVE;

			// Now as only WaterMaster (ie: not ProcessMaster) uses the MID functionality and only
			// ProcessMaster (not WaterMaster) makes use of the custody switch, we can therefore
			// hijack the dynamic variable "custodySwitchStatus" to host this status.
			// We don't want to record this under "readOnlySwitchStatus" as we still want RO status
			// to have its original meaning in case the user logs in at < Service level!

			// Read the RO switch-status line directly and store as the MID (in)active status
			if (HAL_GPIO_ReadPin(READ_ONLY_SWITCH_GPIO_PORT, READ_ONLY_SWITCH_GPIO_PIN) == ARM_HW_SWITCH_INPUT_LOW)	// test I/P state
			{
				armDynamic.custodySwitchStatus = (TUSIGN8) ARM_MID_SWITCH_ACTIVE;
			}
			else
			{
				armDynamic.custodySwitchStatus = (TUSIGN8) ARM_MID_SWITCH_INACTIVE;
			}
		}
		else // (sensorCalMIDCompliant == eFALSE)
		{
			// For non-MID Cert'd instruments, "RO-switch" should revert to its original meaning.
			// So just re-read the switch status from Initialize_ARM()...
			if (HAL_GPIO_ReadPin(READ_ONLY_SWITCH_GPIO_PORT, READ_ONLY_SWITCH_GPIO_PIN) == ARM_HW_SWITCH_INPUT_LOW)	// test I/P state
			{
				armDynamic.readOnlySwitchStatus = (TUSIGN8) ARM_READ_ONLY_SWITCH_ACTIVE;
			}
			else
			{
				armDynamic.readOnlySwitchStatus = (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE;
			}
			// ...and set the "custodySwitchStatus" back because it no longer has relevance here.
			armDynamic.custodySwitchStatus = (TUSIGN8) ARM_MID_SWITCH_INACTIVE;
		}
		#endif //ARM_MID_PRESENT
	
	#endif
	
	return sensorCalMIDCompliant;
}
// end GetSpecial_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by the LogInEXE_ARM method in order to obtain the client access level.
 \       Normally the user is deemed to have this, as it is passed to the Login method.
 \       However, if a custody switch is available and activated, then the access level
 \       may well be subject to variation. Hence an alternate login level may be returned.
 \param  pointer to accessLevel Identity code for the user
 \author Peter Vince
 \date 2010-08-20

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2010-08-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 GetClientLoginLevel(TUSIGN8* accessLevel)
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;// pre-suppose that this request will fail

	#ifdef CUSTODY_SWITCH
	TUSIGN8 custodySwitchStatus = ARM_CUSTODY_SWITCH_INACTIVE;
	(void)arm_Get(ARM_IDX_custodySwitchStatus, WHOLE_OBJECT, (void*)&custodySwitchStatus);

	// User for a specified client has requested to login at a specified level with password
	// NB: awaiting  decision on "Custody" access-level 20060602
	if (custodySwitchStatus == (TUSIGN8) ARM_CUSTODY_SWITCH_ACTIVE)
	{
		if (accessLevel == ARM_STANDARD_USER)
		{
			// stored password is TUSIGN32 "hashed"
			hashedSubsystemPassword = armStaticFrequentConstantDefault.stdUserPassword;
			*accessLevel = (TUSIGN8) ARM_CUSTODY_USER;
			returnCode = (TUSIGN16) OK;
		}
		else
		{
			returnCode = (TUSIGN16) RULE_VIOLATION_ERR;
		}
	}
	else // no custody switch, so just return OK with unaltered login level
	#endif
	{
		returnCode = (TUSIGN16) OK;
	}
	return returnCode;
}
// end GetClientLoginLevel()

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by ComparePasswords_ARM method in order to obtain a device Id (real or otherwise)
 \       which the caller then uses in the [deviceId, PIN, bFeature] hash.
 \param  Pointer to the Device Id, value to be furnished by this function.
 \author Peter Vince
 \date 2009-01-20

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN32 GetDeviceId_ARM(TUSIGN32* ptrDeviceId)
{
	TUSIGN32 deviceErrorCode = (TUSIGN32)RULE_VIOLATION_ERR;

	// Get DeviceId...
	#ifdef DEVICE_SUBSYSTEM_PRESENT
	// ...from Device subsystem, and report error code returned...
	deviceErrorCode = device.unit.Get(DEVICE_ME,
									  (TUSIGN16) DEVICE_IDX_transmitterTransmitterId,
									  (TINT16) WHOLE_OBJECT, ptrDeviceId);
	#else
	// ...or default device ID when no device susbsystem present
	*ptrDeviceId = 1;				// Need to communicate this default to all ARM project users?
	deviceErrorCode = (TUSIGN32)OK;	// No error because no device to read wrongly from.
	#endif //DEVICE_SUBSYSTEM_PRESENT

	return deviceErrorCode;
}
// end GetDeviceId_ARM

#ifdef HART_CLIENT_PRESENT
//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by Initialize_ARM method, and also LogOffEXE_ARM in order to
 \       set the (project-specific) client access level for the HART client.
 \param  none
 \author Peter Vince
 \date 2009-01-20

 \return client access level in respect of HART
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 HartAccessLevelInit_ARM(void)
{
	TUSIGN8 clientAccessLevel = (TUSIGN8) NO_ACCESS;        // return access level for hart

	#ifdef EMF_SPECIAL_FEATURES								// specifically for EMF:
	TUSIGN8 readOnlySwitchStatus = ARM_READ_ONLY_SWITCH_INACTIVE;
	(void)arm_Get(ARM_IDX_readOnlySwitchStatus, WHOLE_OBJECT, (void*)&readOnlySwitchStatus);

	if (readOnlySwitchStatus == (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE)
	{														// if Read-Only = inactive
		clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;	// reset to "Read Write"
	}
	else													// otherwise/non-EMF...
	#endif
	{
		clientAccessLevel = (TUSIGN8) READ_ONLY_ACCESS;		// reset to "Read Only"
	}
	#ifdef EMF_SPECIAL_FEATURES
	// Overwrite stored password with fixed "hashed" EMF Service Level password
	*((TUSIGN32*)(&armStaticFrequentConstantDefault.svcUserPassword)) = SERVICE_TARGET_PASSWORD;
	#endif

	return clientAccessLevel;
}
// end HartAccessLevelInit_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by CheckAccessRights_ARM method in order to report the client access level for
 \       the HART client. This is normally supplied by the client. However, this special function
 \       exists to override this and allow special handling within the EMF-based projects.
 \param  targetId Subsystem index relating to the subsystem in which the data is held
 \param  objectId Object index for the data in the target subsystem
 \author Peter Vince
 \date 2009-01-20

 \return client access level in respect of HART
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 HartAccessLevel_ARM(T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId, TUSIGN16 HARTAccessLevel)
{
	TUSIGN8 clientAccessLevel = (TUSIGN8) NO_ACCESS;		// or "hartAccessLevel" for non-EMF)

	#ifdef EMF_SPECIAL_FEATURES								// specifically for EMF:
	if (targetId == HART_IDX								// if this call is referencing
	 && (objectId == HART_IDX_slot0							// the HART slot 0 code, or
		 || objectId == HART_IDX_ServiceCode				// the HART login code, or
		 || objectId == HART_IDX_serviceCodeLogOut			// the HART logout code
		)													// we can assume this is not the real
	   )
	{
		clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;	// request, so fake the access level
	}
	else													// otherwise
	#endif //EMF_SPECIAL_FEATURES

	{
		clientAccessLevel = HARTAccessLevel;				// get access level
	}

	return clientAccessLevel;
}
// end HartAccessLevel_ARM
#endif //HART_CLIENT_PRESENT

#ifdef HMI_CLIENT_PRESENT
//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by Initialize_ARM method, and also LogOffEXE_ARM in order to
 \       set the (project-specific) client access level for the HMI client.
 \param  none
 \author Peter Vince
 \date 2009-01-20

 \return client access level in respect of HMI
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN8 HmiAccessLevelInit_ARM(void)
{
	TUSIGN8 clientAccessLevel = (TUSIGN8) NO_ACCESS;		// return access level for HMI

	#ifdef CONTROLLER_SPECIAL_FEATURES						// specifically for Controller:
	TUSIGN8 readOnlySwitchStatus = ARM_READ_ONLY_SWITCH_INACTIVE;
	(void)arm_Get(ARM_IDX_readOnlySwitchStatus, WHOLE_OBJECT, (void FAST*)&readOnlySwitchStatus);

	if (readOnlySwitchStatus == (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE)
	{														// if Read-Only = inactive
		clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;	// reset to "Read Write"
	}
	else													// otherwise/non-EMF...
	#endif
	{
		clientAccessLevel = (TUSIGN8) READ_ONLY_ACCESS;		// reset to "Read Only"
	}
	#ifdef EMF_SPECIAL_FEATURES
	// Overwrite stored password with fixed "hashed" EMF Service Level password
	*((TUSIGN32*)(&armStaticFrequentConstantDefault.svcUserPassword)) = SERVICE_TARGET_PASSWORD;
	#endif

	#ifdef CONTROLLER_SPECIAL_FEATURES						// specifically for Controller:
	// Overwrite stored password with fixed "hashed" Controller Service Level password
	*((TUSIGN32*)(&armStaticFrequentConstantDefault.svcUserPassword)) = SERVICE_TARGET_PASSWORD;
	// Overwrite stored High Security Access State with fixed "High Security Disabled" state
	*((TUSIGN8*)(&armStaticFrequentConstantDefault_2.hiSecAccessState)) = ARM_HIGH_SECURITY_DISABLED;
	#endif

	return clientAccessLevel;
}
// end HMIAccessLevelInit_ARM
#endif //HMI_CLIENT_PRESENT

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by an ARM method in order to save an ARM object to non-volatile memory.
 \param object index (location to save to)
 \param pointer to "from" data location
 \param size of data
 \author Peter Vince
 \date 2009-01-20

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 NvMemSave_ARM(TUSIGN16 objectIndex, void SLOW* ptrLocationFrom, TUSIGN16 sizeOfObject)
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;     // pre-suppose this request will fail

	// For the following objects, which can be written using the ARM overloaded Put method:
	if (((objectIndex == (TUSIGN16)ARM_IDX_adpin) && (sizeOfObject == sizeof(TUSIGN16)))
	 || ((objectIndex == (TUSIGN16)ARM_IDX_hiSecAccessState) && (sizeOfObject == sizeof(TUSIGN8)))
	 || ((objectIndex == (TUSIGN16)ARM_IDX_clientEnableState) && (sizeOfObject == sizeof(TUSIGN8)))
	   )
	{
		// Use overloaded Put method in the conventional manner
		//lint -e{643}
		returnCode = ARM_ME->Put(ARM_ME, (TUSIGN16)objectIndex, (TINT16)WHOLE_OBJECT, (void FAST*)ptrLocationFrom);
	}
	else // All other objects need specific alternate handling:
	{
        #if defined EMF_SPECIAL_FEATURES || defined CONTROLLER_SPECIAL_FEATURES || defined _II_WIRE_TOPWORKS   
		void FAST * ptrLocationTo = NULL;	// declare void pointer to the object within ARM

		switch (objectIndex)
		{
		case ARM_IDX_stdDefaultPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.stdDefaultPassword[0]);
			break;
		case ARM_IDX_advDefaultPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.advDefaultPassword[0]);
			break;
		case ARM_IDX_svcDefaultPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.svcDefaultPassword[0]);
			break;
		case ARM_IDX_stdUserPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.stdUserPassword[0]);
			break;
		case ARM_IDX_advUserPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.advUserPassword[0]);
			break;
		case ARM_IDX_svcUserPassword:
			ptrLocationTo = (void FAST *)(&armStaticFrequentConstantDefault_Password.svcUserPassword[0]);
			break;
		default:
			returnCode = (TUSIGN16) ILLEGAL_OBJ_IDX;
			break;
		}
		// Allow writing of the above; use direct access of nvmem to avoid Put_ARM blocking
		if (returnCode != (TUSIGN16) ILLEGAL_OBJ_IDX)
		{
			//lint -e{643}
			returnCode = PutDataNV((const TUSIGN8 FAST*) ptrLocationTo,
									(const TUSIGN8 FAST*) ptrLocationFrom, sizeOfObject);      //delete by Spring temply
		}
		#endif //defined EMF_SPECIAL_FEATURES || CONTROLLER_SPECIAL_FEATURES   //temply delete by Spring
		// TODO:
		// Other projects need to specify how to handle non-vol memory writes for the above.
		// It may well be identical to the above, particularly for those using Process5K.
	}
	return returnCode;
}
//end NvMemSave_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by LogInEXE_ARM method in order to check the likelihood of login failure.
 \param  accessLevel being sought by client
 \param  highSecurityState reported by user-configuration
 \param  password presented to the login function
 \author Peter Vince
 \date 2009-01-20

 \return loginPossible: eFALSE = login WILL fail if attempted
 \                      eTRUE  = login MAY pass if attempted
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TBOOL PreLoginCheck_ARM(TUSIGN8 accessLevel, TUSIGN8 highSecurityState, TCHAR* password)
{
	TBOOL loginPossible = eTRUE;			// EMF-specific build (default: TRUE for a.n.other)
	#ifdef AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL
	// Bug Id: EMF/281 20070816; requested in lieu of a direct "Reset Passwords" being available:
    TUSIGN32 numericPassword = 0;

	// To minimise work on the HMI software decided on the following approach.
	// The ARM system will be modifed to have the following user behaviour:
	// 1. To reset the passwords the user will attempt to log in at Std/Adv/Service level.
	// 2. The reset passwords will use the hasher reset passwords code, based on the ID & PIN.
	// 3. If High security is on, this info is available at the password entry frame,
	//    if not it can be obtained off the Device Info page without logging in.
	// 4. The ARM will check the entered password to see if it's valid for Std/Adv/Service level
	//    (simple password or hashed password).
	// 5. If not a valid password, it will check to see if its a valid password reset code,
	//    if it is, it will perform the password reset function.
	numericPassword = StringToNumeric_ARM(password);

	if ((highSecurityState == ARM_HIGH_SECURITY_ENABLED)
	 && (accessLevel == ARM_SERVICE_USER)
	 && (numericPassword <= SERVICE_LEVEL_MASK))
	{
		// This condition definitely precludes a service-level login at high security, so the
		// entered passcode will instead be presented directly to "reset passwords" - at the end of
		// the LoginEXE_ARM function.

		// We do not want to attempt login in this instance, because:
		// (a) we know it will be wrong (bit 19 is reset),
		// (b) we don't want to risk the PIN incrementing.

		// This leaves Standard, Advanced, and Service-with-Simple-Password as the candidates for
		// "login first, try reset if this fails". The risk condition is if the hash code for reset
		// equates to "6000" - (or any simple code by chance matching its login-level password)
		// which would result in a successful Standard/Advanced/Service-with-Simple-Password login!
		loginPossible = eFALSE;
	}
	// MID additions, 2010-05-04:
	else
	{
		#ifdef ARM_MID_PRESENT
		// MID compliant checks: if MID status is "OIML Cert Class 1 or 2" then block Service login
		TBOOL sensorCalMIDCompliant = eFALSE;
		sensorCalMIDCompliant = GetSpecial_ARM();	// this also assigns the switch-status('s)

		if (sensorCalMIDCompliant == eTRUE)
		{
			// NB: don't call arm_Get() for custodySwitchStatus, as this (dynamic) object was
			// "hijacked" in GetSpecial_ARM for the purpose of recording RO switch status as
			// MID (in)active switch status.
			if ((accessLevel == ARM_SERVICE_USER)
			 && (armDynamic.custodySwitchStatus == ARM_MID_SWITCH_ACTIVE))
			{
				// MID compliant checks: if MID status is "OIML Cert Class 1 or 2" then block
				// Service user login. Service users can only login if either their MID Cert'n is
				// revoked, or the (redesignated) "RO" switch is inactive.
				loginPossible = eFALSE;

				// As the login has been attempted for a Service User (bit 19 set) then there is
				// no risk of this indicating to the Post Log-In procedure that a "Reset Passwords"
				// is requested.
			}
			//else // access level = Std/Adv OR the MID (Custody) switch is inactive
			// so loginPossible = eTRUE by default;
		}
		#endif //ARM_MID_PRESENT
	}
	#endif //AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL

	return loginPossible;
}
// end PreLoginCheck_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by LogInEXE_ARM method in order to attempt Reset-Passwords following login failure.
 \param  loginPass result reported by the client login attempt
 \param  password presented to the login function
 \author Peter Vince
 \date 2009-01-20
 \return none.
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
void PostLoginCheck_ARM(TUSIGN16 loginPass, TCHAR* password)
{
	// Bug Id: EMF/281 20070816; requested in lieu of a direct "Reset Passwords" being available:
	#ifdef AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL
    TUSIGN32 numericPassword = 0;

	// From point 5. of the "Pre-Login check"...
	// If not a valid login password, (ARM) will check to see if its a valid password reset code,
	// if it is, it will perform the password reset function.
	if ((loginPass != (TUSIGN16) OK) && (password[0] != '\0')) //APP20180709, improvement which is to avoid no necessary check
	{
		numericPassword = StringToNumeric_ARM(password);

		if (numericPassword <= SERVICE_LEVEL_MASK)
		{
			// Feature bit (19) is NOT set, so we call "Reset-passwords" instead of login
			(void)arm.ResetPasswordsEXE((TCHAR *) password);
			// Whatever the outcome, there is no pointing in seeking or reporting a return "OK",
			// or otherwise, result here. The Login has already failed, so the Reset-Passwords
			// operation has been attempted in order to fulfill what the caller may have intended.
		}
	}
	#endif //AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL
}
// end PostLoginCheck_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by LogOffEXE_ARM method in order to reset engineering "PIN" following logoff.
 \param  loginPass result reported by the client login attempt
 \param  password presented to the login function
 \author Peter Vince
 \date 2009-01-20
 \return none.
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
void PostLogoffCheck_ARM(T_SUBSYSTEM_IDX callerId)
{
	#if defined EMF_SPECIAL_FEATURES && defined HMI_CLIENT_PRESENT	// EMF-specific build
	if (callerId == HMI_IDX)										// relevent only to HMI
	{
		#ifdef DEVICE_SUBSYSTEM_PRESENT
    	TUSIGN16 devicePin = 0 ;

		// Operation moved from HMI\Source\specialOperations.c\specialOp_ResetEngineeringMode()
		// Requirement is to reset the Engineering Mode so that the "super"-Service access items
		// are blocked to unauthorized users.
		device.unit.Put(DEVICE_ME, DEVICE_IDX_engineeringPIN, WHOLE_OBJECT, &devicePin);
		#endif //DEVICE_SUBSYSTEM_PRESENT
	}
	#endif //defined EMF_SPECIAL_FEATURES && defined HMI_CLIENT_PRESENT
}
// end PostLoginFailure_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by ResetPasswordsEXE_ARM method in order to perform, or rubber-stamp, the
 \       reset passwords check prior to approving the desired action.
 \param  password The hashed password, with feature-bit set to "Reset All Passwords"
 \author Peter Vince
 \date 2009-02-02

 \return variableWriteState: eFALSE = reset passwords not allowed
 \                           eTRUE  = reset passwords allowed
 \warning None
 \test
 test-date: 2009-02-02
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TBOOL ResetPasswordsCheck_ARM(TCHAR* password)
{
	TBOOL resetPasswordsState = eFALSE;

    // Compare entered Password with hashed Password that ComparePasswords_ARM will calculate
    // on receipt of the following with "HIGH_SECURITY_ENABLED" (doesn't need a subsystem
    // password, or knowledge of user's login status).
	#ifdef HASH_CODE_RESET
    if (ComparePasswords_ARM((const TCHAR *) NULL, // subsystem password not used for this exercise
                             password,
                             (TUSIGN8) ARM_HIGH_SECURITY_ENABLED,
                             0,         // size of hashed, subsystem password
                             RESET_PASSWORDS) == PASSWORD_MATCH)
	#endif //HASH_CODE_RESET
    {
		resetPasswordsState = eTRUE;
	}

	return resetPasswordsState;
}
// end ResetPasswordsCheck_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by CheckAccessRights_ARM method in order to check
 \       the overall writeability of an ARM object into non-volatile memory.
 \param  targetId Subsystem index relating to the subsystem in which the data is held
 \param  objectId Object index for the data in the target subsystem
 \author Peter Vince
 \date 2009-01-20

 \return variableWriteState: eFALSE = write not allowed
 \                           eTRUE  = write allowed
 \warning None
 \test
 test-date: 2009-01-20
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
TBOOL VariableWriteCheck_ARM(T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId)
{
	TBOOL variableWriteState = eTRUE;			// EMF-specific build (default: TRUE for a.n.other)

	#ifdef EMF_SPECIAL_FEATURES					// specifically for EMF:
	TNvConfiguration nvConfiguration ;

	nvMemInterface_Get(NVMEMINTERFACE_IDX_nvConfig,WHOLE_OBJECT,&nvConfiguration) ;

	if (nvConfiguration == NVMEMINTERFACE_NV_CONFIG_OFFLINE)
	{
		// The following nvmem service function will return eFALSE if we cannot write to this
		// identified variable. Whatever is returned, relay this same status to our caller:
		variableWriteState = nvMemInterface.CanWriteVariable_SRV(targetId, objectId) ;
	}
	#endif //EMF_SPECIAL_FEATURES

	return variableWriteState;
}
// end VariableWriteCheck_ARM

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Called by CheckAccessRights_ARM method in order to determine writeability of an ARM object
 \       that may additionally be subject to Measuring Instrument Directive (MID) restrictions.
 \param  clientLoginState indicates whether client has Basic/Standard/Advanced/Service
 \param  accessRequested pointer supplied by caller to enable rewriting if necessary
 \param  returnCode pointer supplied by caller to enable rewriting if necessary
 \author Peter Vince
 \date 2010-03-26

 \return none.
 \warning None
 \test
 test-date: 2010-MM-DD
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
// OIML Table definitions
                                                // The offset from the next entry which identifies:
#define SUBSYSTEM_IDX_OFFSET        (0)         // the target subsystem
#define TARGET_IDX_OFFSET           (1)         // the object required
#define ATTRIBUTE_IDX_OFFSET        (2)         // the attribute required
#define NEXT_TABLE_ENTRY_OFFSET     (3)         // the next entry

void MIDOverrideCheck_ARM(T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId, TUSIGN16 attributeId,
						  TUSIGN8 clientLoginState, TUSIGN8* accessRequested, TUSIGN16* returnCode)
{
  #if defined _EMF_WW	// EMF (WW)-specific build
    const TUSIGN16 *oimlTablePointer = NULL;
	TBOOL sensorCalMIDCompliant = eFALSE;

   #ifdef DEVICE_SUBSYSTEM_PRESENT
	// Requirement is to get the sensor calibration type to determine if access to "sensitive"
	// items should be blocked to all but service-level users.
	sensorCalMIDCompliant = device.MeterIsOIMLCompliantSRV();
   #endif //DEVICE_SUBSYSTEM_PRESENT

	// If the MID soft-switch applies
	if ((clientLoginState != (TUSIGN8) ARM_SERVICE_USER)
	 && (*accessRequested > READ_ONLY_ACCESS)
	 && (sensorCalMIDCompliant == eTRUE))
	{
		oimlTablePointer = objectIdTableOIML;
		// This looks like an access right table, except it only has 3 elements (not 4):
		// (TUSIGN16)target subsystem index,
    	// (TUSIGN16)target object index,
    	// (TUSIGN16)target attribute index,
	    // but NOT (TUSIGN16) target access rights

		// ...use "targetId", "objectId", and "attributeId" to locate the correct item in the Table
		while (oimlTablePointer[SUBSYSTEM_IDX_OFFSET] != TABLE_TERMINATION)
		{
			if ( (oimlTablePointer[SUBSYSTEM_IDX_OFFSET] == (TUSIGN8) targetId)
			  && (oimlTablePointer[TARGET_IDX_OFFSET] == objectId)
			  && ((oimlTablePointer[ATTRIBUTE_IDX_OFFSET] == attributeId)
			   || (oimlTablePointer[ATTRIBUTE_IDX_OFFSET] == ANY_ATTRIBUTE)
				 )
			   )
			{
				// Found the item in the OIML table, quench the access regardless of the request...
				*accessRequested = (TUSIGN8) NO_ACCESS;
				// ...this will prevent the caller from proceeding to the a.r.table search.
				// Also feed a legitimate return code for caller to report back to client.
				*returnCode = (TUSIGN16) WRONG_STATE_ERR;
				// Now quit looking and let the caller handle the client-response.
				break;
			}
			// end targetId, objectId/frameId, attributeId check
			oimlTablePointer = &oimlTablePointer[NEXT_TABLE_ENTRY_OFFSET];
			// instead of oimlTablePointer += NEXT_TABLE_ENTRY_OFFSET
			// which contravenes MISRA C rule 101
		}
		// end while
	}
  #endif //defined _EMF_WW
}
// end MIDOverrideCheck_ARM

//-------------------------------------------------------------------------------------------------
#ifndef DEVICE_SUBSYSTEM_PRESENT	// If no Device subsystem, these functions will be needed
//-------------------------------------------------------------------------------------------------
// Fix for the non-working 32 bit print formatting function.
// Two tables are used in the decimal to ascii conversion.
// The table Max32BitColumnValue is based on the maximum value for an internal representation of a 32bit number,
// 4294967295. As the conversion starts from the left hand side and highest value column, the maximum allowable
// value for the final column is limited to four. All other columns are allowed to range between 0 and 9. The values
// in the column ValueSteps32Bit, are the the base 10 values of each column of the 32bit number.
// These are mulitplied against the values in the former table. If the value being converted
// is greater than the product, the next character is found. The product is then subtracted from the number being converted.
// This continues until all characters are converted.

// Private function declarations
TUSIGN8 Get32BitCharacterCountDeviceStringFormatting(TUSIGN32 *pValue32) ;

// Data arrays used during string conversion
const TUSIGN32 ValueSteps32Bit[] =   {
                                                    1000000000 ,            // 10
                                                    100000000 ,             // 9
                                                    10000000 ,              // 8
                                                    1000000 ,               // 7
                                                    100000 ,                // 6
                                                    10000 ,                 // 5
                                                    1000 ,                  // 4
                                                    100 ,                   // 3
                                                    10 ,                    // 2
                                                    1 ,                     // 1
                                                } ;

// Table of max character values when the uppermost character is unknown
const TUSIGN8 Max32BitColumnValue[] = {
                                                        4,                  // 10
                                                        9,                  // 9
                                                        9 ,                 // 8
                                                        9 ,                 // 7
                                                        9 ,                 // 6
                                                        9 ,                 // 5
                                                        9 ,                 // 4
                                                        9 ,                 // 3
                                                        9 ,                 // 2
                                                        9                   // 1
                                                    } ;

// Private definitions
#define MAX_32BIT_CHARACTERS 10

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the number of ascii characters in a thirty two bit number.
 \author GL
 \date   2006-02-27
 \param  pValue32 - pointer to the thirty two bit number having its characters counted.
 \return > 0 The number of characters, = 0 failed.
 <pre>
    > 0 success
    = 0 failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 Get32BitCharacterCountDeviceStringFormatting(TUSIGN32 *pValue32)
{
    // The possible number of characters that the passed value has been checked against
    TUSIGN8 CheckedCharacters = 0 ;
    // Set when the number of characters is determined
    TBOOL Result = eFALSE ;

    do
    {
        if (*pValue32 >= ValueSteps32Bit[CheckedCharacters])
        {
		    Result = eTRUE ;
        }
        else
        {
            CheckedCharacters++ ;
        }
    }
    while ((CheckedCharacters < ( MAX_32BIT_CHARACTERS - 1 )) && (Result == eFALSE));
    return (TUSIGN8)(MAX_32BIT_CHARACTERS - CheckedCharacters);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a thirty two bit number to ascii data. Calls get character count to initialise the character count
 \brief  variable, then uses the tables of data ValueSteps32Bit and Max32BitColumnValue
 \brief  to calculate the ascii values for each column. This was written to replace the IAR sprintf call
 \brief  which doesn't work with whole numbers above sixteen bits.
 \author GL
 \date   2006-02-27
 \param  pValue32 - Pointer to the 32 bit number being string formatted.
 \param  pBuffer - The string buffer to be loaded with the formatted 32bit information
 \param  NullTerminate - caller indicates whether or not a NULL termination character is required
 \return > 0 The number of characters added to the buffer, = 0 failed.
 <pre>
    > 0 success
    = 0 failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 StringFormat32BitDeviceStringFormatting(TUSIGN32 *pValue32, TUSIGN8 *pBuffer, TBOOL NullTerminate)
{
    // Get the number of characters for the string equivalent of the 32 bit number
    TUSIGN8 CharacterCount = Get32BitCharacterCountDeviceStringFormatting(pValue32) ;
    TUSIGN8 CharactersAdded = 0 ;
    TUSIGN8 ValuesChecked = 0 ;
    TUSIGN32 TempValue32 = *pValue32 ;
    TUSIGN8 CharacterPosition = 0 ;
    TBOOL NewChar = eFALSE ;

    // Don't proceed if no characters need to be added
    if (CharacterCount > 0)
    {
        // Get the position of the first character for this id - this will be the highest power of ten for the id
        CharacterPosition = MAX_32BIT_CHARACTERS - CharacterCount ;
        // Add each character in descending order of powers of ten
        for ( CharactersAdded = 0 ; CharactersAdded < CharacterCount ; CharactersAdded++ )
        {
            // Assume no character added initially
            NewChar = eFALSE ;
            // Multiply each possible value for the character by the appropriate power of ten for this column
            for ( ValuesChecked = Max32BitColumnValue[CharacterPosition] ; ValuesChecked > 0 ; ValuesChecked--)
            {
                // If the value is greater than or equal to the number being converted . . .
				if (TempValue32 >=  ( ValuesChecked * ValueSteps32Bit[CharacterPosition] ) )
                {
                    // The character value is the next value in the table
					NewChar = eTRUE ;
                    pBuffer[CharactersAdded] = ValuesChecked + 0X30 ;
                    TempValue32 -= ValuesChecked * ValueSteps32Bit[CharacterPosition] ;
                    break ;
                }
            }
            // If no character is found, a zero is added in its place
			if ( NewChar == eFALSE )
            {
                pBuffer[CharactersAdded] = 0X30 ;
            }
            // Look at the next character column
			CharacterPosition++ ;
        }
        // Check if a null character is required at the end of the string
        if (NullTerminate == eTRUE)
        {
            // Terminate the string with a null character after the conversion is complete
            pBuffer[CharactersAdded] = '\0' ;
        }
    }
    return(CharactersAdded) ;
}
// end StringFormat32BitDeviceStringFormatting
#endif //!DEVICE_SUBSYSTEM_PRESENT

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a thirty two bit number to Hex data (string form).
 \       Based on solution given at: http://www.gidforums.com/t-2615.html
 \       Modified so that the function takes a U32 and a pointer to an output buffer of
 \       sufficient [10] size, and returns a char* array filled with hex digits.
 \author PV
 \date   2009-03-10
 \param  pValue32 - Pointer to the 32 bit number being string formatted.
 \param  pBuffer - The string buffer to be loaded with the formatted 32bit (Hex) information
 \param  sizeOfBuffer - size of the above string buffer
 \param  NullTerminate - caller indicates whether or not a NULL termination character is required
 \return > 0 The number of characters added to the buffer, = 0 failed.
 <pre>
    > 0 success
    = 0 failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StringFormat32BitHexStringFormatting(TUSIGN32 *pValue32, TCHAR *pBuffer,
											  TUSIGN16 sizeOfBuffer, TBOOL nullTerminate)
{
    TUSIGN32 tempValue32 = *pValue32;				// temp. place holder for *pValue32
	TUSIGN16 size = sizeOfBuffer - 1;				// size is buffer-size less 1 (for NULL)
	TCHAR hexLookUpTab[] = "0123456789ABCDEF";
	const TCHAR* hexLookUp = hexLookUpTab;	// lookup table stores the hex digits
													// into their corresponding index.
	TUSIGN8 mask = 0x0F;							// mask will zero-out all the bits except
													// lowest 4 which represent a single hex digit
	TUSIGN16 passCount = 0;							// for adjusting string-position
	TUSIGN16 x = 0;									// universal loop counter
	//---------------------------------------------------------------------------------------------
	// The following "for-loop" works this way: it starts with x as 0 and then right shifts
	// the temp number bits in multiples of x and 4. On the first pass, it won't shift(shift by 0)
	// because we need the lowest 4 bits for the lowest or right most hex digits. The next pass it
	// will right shift by 4(4*1), because we are done with the lowest 4 bits. Now we need the
	// 4 bits higher than the lowest 4 bits, meaning bits 5-8 or 4-7 depending on how you count
	// from right. So we right shift by 4 bits pushing the bits 5-8(4-7) in to the lowest 4 bit
	// places, and then perform the AND operation on the number with our mask to clear(zero) all
	// of the bits except the lowest 4. Then the number that the lowest 4 bits represent, will be
	// used as an index to fetch a hex digit from the lookup array. The loop will run till x has
	// reached size(number of hex digit storage possible in the passed parameter type).
	// --------------------------------------------------------------------------------------------

	for (x = 0; x < size; x++)
	{	
		tempValue32 = *pValue32;					// temp is assigned *pValue32 each time
		tempValue32 >>= (4 * x);					// shift it in multiples of 4 bits each time
		tempValue32 &= mask;						// mask so it will only have lowest 4 bits
		pBuffer[size-x] = hexLookUp[tempValue32];	// temp now has numeric value which will point
	}												// to corresponding index in the look up table

	// Eliminate leading NULLs (and also leading zeros)
	while ((pBuffer[0] == '\0') || (pBuffer[0] == '0'))
	{
		for (x = 0; x < size; x++)
		{
			pBuffer[x] = pBuffer[x + 1];
		}
		// Check if a null character is required at the end of the string
		if (nullTerminate == eTRUE)
		{
			// "x" is still pointing to the last location in the buffer - insert a "NULL"
			pBuffer[x] = '\0';
		}
		passCount += 1;
		if(passCount>size) //APP20180709, to avoid stop when value is equal to 0
        {
            break;
        }
	}
	// Adjust size value to return
	size -= (passCount - 1);

	return size;
}
// end StringFormat32BitHexStringFormatting
