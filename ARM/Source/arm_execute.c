//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

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
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"		// to identify client subsystem(s)
#include "ElettronicServices/Interface/ElettronicServices_idx.h"		// to identify client subsystem(s)

#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"
#include "ARM/Source/art_hmi.h"
#include "ARM/Source/art_hart.h"


// add your list of Client Access Right Tables ("arts") here...
#ifdef HMI_CLIENT_PRESENT
	#include "art_hmi.h"
#endif //HMI_CLIENT_PRESENT

#ifdef HART_CLIENT_PRESENT
	#include "art_hart.h"
#endif //HART_CLIENT_PRESENT
// .. end of arts

#include "SHA2.h"	//APP20180709
#include "arm_types.h"
#include "arm_attributes.h"
#include "arm_execute.h"
#include "arm_specialFunctions.h"			// contains project-dependent function variants



//-------------------------------------------------------------------------------------------------
// Private (to ARM Subsystem) data
// Each client Access Level needs to be global to the functions contained herein, but not to any
// external (subsystem) calling this subsystem. The permitted access-level status codes are:
// 0 = no access permitted
// 1 = read-only
// 2 = write only
// 3 = read/write

#ifdef HMI_CLIENT_PRESENT
TUSIGN8 hmiAccessLevel;         // access level for hmi
#endif //HMI_CLIENT_PRESENT

#ifdef HART_CLIENT_PRESENT
TUSIGN8 hartAccessLevel;        // access level for hart
#endif //HART_CLIENT_PRESENT

/* ...add further access levels, as required, for bespoke implementations...
TUSIGN8 anotherAccessLevel;     // access level for a.n.other client
//etc..
//etc...
*/

//-------------------------------------------------------------------------------------------------
// Local function prototypes of ARM_execute
//-------------------------------------------------------------------------------------------------
//APP20180709, remove the old function, and add new functions.
TUSIGN16 GetHashStrOfReset_ARM(TCHAR * , TUSIGN32 , TUSIGN16 , TUSIGN8 );
TUSIGN16 GetHashStrOfInput_ARM(TCHAR* , TCHAR* );
void GetHashDataOfInput_ARM(TUSIGN32* , TCHAR* );
TUSIGN16 GetStrOfHashData_ARM(TCHAR* , TUSIGN32* );

TUSIGN16 WriteNewPassword_ARM(TUSIGN16 , TCHAR * );
#ifdef ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM
const TUSIGN16* GetAccessRightsTable_ARM(T_SUBSYSTEM_IDX , T_SUBSYSTEM_IDX );
#endif

#define LENGTH_OF_DEVICE_ID         (10)    // 32 bits used, so max = 4294967295 = 10 chrs
#define LENGTH_OF_PIN               (5)     // 16 bits used, so max = 65535 = 5chrs
#define LENGTH_OF_FEATURE_BIT       (1)     // 1 chr (reset passwords= 0, access= 1)

//APP20180709
/*
description: calculate the hash code for password reset
--strHashPwd: pointer to hashed password. max size is 6,including the '\0'
return:length of strHashPwd, max length is 5, not including the end character '\0'
tested.
*/

TUSIGN16 GetHashStrOfReset_ARM(TCHAR * strHashPwd, TUSIGN32 deviceId, TUSIGN16 adpin, TUSIGN8 accessType)
{
    TCHAR strDeviceID[LENGTH_OF_DEVICE_ID + LENGTH_OF_NULL] = {0};
    TCHAR strPIN[LENGTH_OF_PIN + LENGTH_OF_NULL]= {0};
    TCHAR cFeature[LENGTH_OF_FEATURE_BIT + LENGTH_OF_NULL]= {0};
    TCHAR strInputData[MIN_MSGLEN_USIGN8_SHA256] = {0};
    TUSIGN32    lOriginalHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0};
    TUSIGN32    lDestHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0}; 
    TUSIGN8*    pSourceHashData = (TUSIGN8*)lOriginalHashData;
    TUSIGN32*   pDestHashData = (TUSIGN32*)lDestHashData; 
    TUSIGN32 lPin = adpin;      // U32 version of pin
    TUSIGN32 lAccess = accessType&0x01;     // "feature bit", indicates login (1) or reset-passwords (0)
    TUSIGN32 i;    
    TUSIGN16 chrsInString = 0;

    
    // convert deviceId, PIN, reset-passwords flag to string format
    // NB: a deviceId or PIN that is >= (2^15 - 1) will not work using sprintf, because the "%ld"
    // option will interpret these and larger numbers as negative. The "%u" specifier should work,
    // but does not under IAR. For this reason, the following method has been used:
    // The deviceId subsystem-supplied "dec-to-ascii" conversion uses a method previously developed
    // specifically for large value device Ids (see devicestringformatting.c):
    // PIN and lAccess values need to be cast as TUSIGN32
    //lint -e{730}
    (void)StringFormat32BitDeviceStringFormatting(&deviceId, strDeviceID, eTRUE);
    //lint -e{730}
    (void)StringFormat32BitDeviceStringFormatting(&lPin, strPIN, eTRUE);
    //lint -e{730}
    (void)StringFormat32BitDeviceStringFormatting(&lAccess, cFeature, eTRUE);

    // construct Input string by concatenating all inputs
    //lint -e970 Peter Vince 20060601; use of "char" outside of typedef OK.
    (void)strcpy((char *)strInputData, (const char *)strDeviceID);
    (void)strcat((char *)strInputData, (const char *)strPIN);
    (void)strcat((char *)strInputData, (const char *)cFeature);
    //lint +e970 Peter Vince 20060601; warning on for the rest of the code.
    
    i=0;
    while(strInputData[i]!='\0')
    {
        i++;
    }
    
    SHA_256((TUSIGN32*)strInputData, i, lOriginalHashData);

    for(i=0u; i<N_LONGWORDS_DEST_HASH_SHA256; i++)
    {
        (void)Get32_BigEndian(pSourceHashData, pDestHashData);
        pSourceHashData += 4;
        pDestHashData ++;
    }
    
    // prefix each of the most significant bits (ie: unused bits, from bit (N-1) to bit 31,
    // where N is number of bits comprising the hashed value) of the
    // hashed word with the value "0"
    *lDestHashData = *lDestHashData & SERVICE_LEVEL_MASK;

    // left shift the "access level" bit by N bits (where N is number of bits comprising the
    // hashed value) so that it can be patched as the last bit of hashed password.
    lAccess = lAccess << SERVICE_PASSWORD_BITS;

    // or-ing of access level with hash code to generate N+1 bit hash password
    *lDestHashData = *lDestHashData | lAccess;
    
    // Convert returned hashed password to string
    //lint -e{730}
    //Only use the first word
    chrsInString = StringFormat32BitHexStringFormatting(lDestHashData,
                                                        strHashPwd,
                                                        (LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL),
                                                        eTRUE);
    // The above result is presented in the same format as the passed-in code.
    // This assumes the user entered the password code as per the desk-top output,
    // ie: (hex, upper-case A - F)
    
    return chrsInString;
    
}


//APP20180709
/*
description: calculate the hash code for password reset
--strHashPwd: pointer to hashed password. max size is 65
--strInput: pointer to input password.
return:length of strHashPwd, max length is 64, not including the end character '\0'
tested.
*/
TUSIGN16 GetHashStrOfInput_ARM(TCHAR* strHashPwd, TCHAR* strInput)
{
    TUSIGN32    lInputData[MIN_MSGLEN_USIGN32_SHA256] = {0};
    TUSIGN32    lOriginalHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0};
    TUSIGN32    lDestHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0};
    TCHAR*      pInput = (TCHAR*)lInputData;
    TUSIGN8*    pSourceHashData = (TUSIGN8*)lOriginalHashData;
    TUSIGN32*   pDestHashData = (TUSIGN32*)lDestHashData;
    TCHAR strHexPwd[8][9] = {0};    
    TUSIGN32 i; 
    TUSIGN16 hashPwdLength = 0;

    VIP_ASSERT(strHashPwd!=NULL);
    VIP_ASSERT(strInput!=NULL);
    
    i=0;
    while(strInput[i]!='\0')
    {
        pInput[i] = strInput[i];
        i++;
    }
    pInput[i] = '\0';
    
    VIP_ASSERT(i<=CALC_OVERFLOW_THRESHOLD_SHA256);

    if(i>0)  // for test
    {
        strHashPwd[0] = '\0';
    }
   
    SHA_256(lInputData, i, lOriginalHashData);
    
    strHashPwd[0] = '\0';
    
    for(i=0u; i<N_LONGWORDS_DEST_HASH_SHA256; i++)
    {
        (void)Get32_BigEndian(pSourceHashData, pDestHashData);
        
        // Convert returned hashed password to string
        // This assumes the user entered the password code as per the desk-top output, ie: (hex, upper-case A - F)
        hashPwdLength += StringFormat32BitHexStringFormatting(&lDestHashData[i],
                                                                strHexPwd[i],
                                                                9,
                                                                eTRUE);
        
        (void)strcat((char *)strHashPwd, (const char *)strHexPwd[i]);

        pSourceHashData += 4;
        pDestHashData ++;
    }
     
    return hashPwdLength;

}


/*
//APP20180709
description: calculate the hash code for password reset
--strHashPwd: pointer to hashed password. max size is 65
--strInput: pointer to input password.
return:length of strHashPwd, max length is 64, not including the end character '\0'
tested.
*/
void GetHashDataOfInput_ARM(TUSIGN32* hashPwd, TCHAR* strInput)
{
    TUSIGN32    lInputData[MIN_MSGLEN_USIGN32_SHA256] = {0};
    TCHAR*      pInput = (TCHAR*)lInputData;      
    TUSIGN32 i;

    VIP_ASSERT(hashPwd!=NULL);
    VIP_ASSERT(strInput!=NULL);
    
    i=0;
    while(strInput[i]!='\0')
    {
        pInput[i] = strInput[i];
        i++;
    }
    pInput[i] = '\0';
    
    VIP_ASSERT(i<=CALC_OVERFLOW_THRESHOLD_SHA256);
   
    SHA_256(lInputData, i, hashPwd);

}

/*
//APP20180709
descriptoin: Translate hash string from saved password
param 1: output, hash string, the size is 65, including end char '\0'
param 2: input, hashed password, the size is 8 word.
return: length of output hash string.
*/

TUSIGN16 GetStrOfHashData_ARM(TCHAR* strHashPwd, TUSIGN32* hashPwd)
{    
    TUSIGN32    lDestHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0};
    TUSIGN8*    pSourceHashData = (TUSIGN8*)hashPwd;
    TUSIGN32*   pDestHashData = (TUSIGN32*)lDestHashData;
    TCHAR strHexPwd[8][9] = {0};
    TUSIGN32 i; 
    TUSIGN16 hashPwdLength = 0;

    VIP_ASSERT(hashPwd!=NULL);
    VIP_ASSERT(strHashPwd!=NULL);
    
    
    strHashPwd[0] = '\0';
    for(i=0u; i<N_LONGWORDS_DEST_HASH_SHA256; i++)
    {
        (void)Get32_BigEndian(pSourceHashData, pDestHashData);
        
        // Convert returned hashed password to string
        // This assumes the user entered the password code as per the desk-top output, ie: (hex, upper-case A - F)
        hashPwdLength += StringFormat32BitHexStringFormatting(&lDestHashData[i],
                                                                strHexPwd[i],
                                                                9,
                                                                eTRUE);
        
        (void)strcat((char *)strHashPwd, (const char *)strHexPwd[i]);

        pSourceHashData += 4;
        pDestHashData ++;
    }
     
    return hashPwdLength;

}


//-------------------------------------------------------------------------------------------------
// Local functions of ARM_execute
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/*!
 \brief	ComparePasswords_ARM verifies whether or not a supplied password matches the subsystem
        password. A match results in a positive (true) result being returned, at the same time the
        "pin" value (ARM object, a security value used in the hashing algorithm) is incremented.
        In the event of a no-match, a negative (false) result is returned, at the same time a
        static "mismatch fail-count" is incremented per each failure result. After 3 consecutive
        match-failures, the "pin" value is again incremented.
        NB: For sting operation, using "%lX" assumes the user entered the password the code as per
        the desk-top output, ie: (hex, upper-case A - F).
        Called by LoginEXE and ResetPasswordsEXE to abstract the utility part of the operation.
 \author	Peter Vince
 \date   2006-02-06

 \param[in]	subsystemPassword, pointer to null terminated string. Can be entered as "NULL" if
            high security is enabled, ie: where a match is deteermined against a hash of deviceId
            + pin + feature bit instead.
 \param[in]	suppliedPassword, pointer to null terminated string
 \param[in]	highSecurityState (en/disabled enum)
 \param[in]	sizeOfPassword is length subsystem password
 \param[in]	bFeature is feature-bit (0=reset passwords, 1=access-level)

 \return : The compare result is returned
            NO_PASSWORD_MATCH = subsystem and supplied passwords do not match
            PASSWORD_MATCH = subsystem and supplied passwords match
 \warning: none
 \test
 \n by: Peter Vince
 \n result module test: 2006-03-10 OK				
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed		
 \bug
  //APP20180709
 //
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 ComparePasswords_ARM(const TCHAR * subsystemPassword, TCHAR * suppliedPassword,
                              TUSIGN8 highSecurityState, TUSIGN16 sizeOfPassword, TUSIGN8 bFeature)
{
    static TUSIGN16 mismatchFailCount = 0;
    TUSIGN16 returnCode = NO_PASSWORD_MATCH;    // pre-suppose that this test will yield no match
    TUSIGN16 chrsInString = 0;

    VIP_ASSERT(suppliedPassword);      		    // ensure no null pointer...

    if (suppliedPassword != NULL)            		    // ...making this check redundant,
    {						                    // except that lint prefers to check
		// Bug Id: EMF/738 20090820; check state is "Not Disabled" rather than "Enabled"
        if (highSecurityState != (TUSIGN8) ARM_HIGH_SECURITY_DISABLED)
        {
			TCHAR strResetCode[LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL] = {0};	
			TUSIGN32 deviceId;
            TUSIGN32 deviceErrorCode = 0;  // high-security uses device id
            TUSIGN16 rollingPin = 0; 

            // Compare passwords: the supplied password will be a hex-numeric char[5] (FR-G8.2);
            // there is no subsystem password applicable - caller could supply "NULL" for this item;

            // Get DeviceId (let Special Function call to sort where to read from)
			deviceErrorCode = GetDeviceId_ARM(&deviceId);				// ARM Special Function

            if (deviceErrorCode == (TUSIGN32)OK)
            {
				ENABLE_DATA_PROTECTION(armDataSemaphore);
                // Get PIN.
                rollingPin = armStaticFrequentConstantDefault.adpin;
				DISABLE_DATA_PROTECTION(armDataSemaphore);

				// preset the whole string to zero, as a stable starting point prior to convert
				(void)memset((void*)strResetCode, '\0', (LENGTH_OF_INPUT_PASSWORD + LENGTH_OF_NULL));
				chrsInString = GetHashStrOfReset_ARM(strResetCode, deviceId, rollingPin, bFeature);

                // Compare returned/passed-in hashed passwords
                //lint -e{970} Peter Vince 20060601; use of "char" outside of typedef OK.
                if (strncmp((char *)strResetCode, (char const *)suppliedPassword, (size_t)chrsInString) == 0)
                {
                    returnCode = PASSWORD_MATCH;
                    // match found for hashed password...
                    // reset the "mismatch fail" count...
                    mismatchFailCount = 0;
                    // increment...
                    rollingPin++;
                    // ...and put PIN
                    (void)NvMemSave_ARM((TUSIGN16)ARM_IDX_adpin, (void FAST *)(&rollingPin),
										sizeof(rollingPin));			// ARM Special Function
                }
				else
				{
					// no match found; if this happens 3x...
					if (++mismatchFailCount >= MAX_NUMBER_OF_MISMATCHES)
					{
						// reset the "mismatch fail" count...
						mismatchFailCount = 0;
						// increment...
						rollingPin++;
						// ...and put PIN
						(void)NvMemSave_ARM((TUSIGN16)ARM_IDX_adpin, (void FAST *)(&rollingPin),
											sizeof(rollingPin));			// ARM Special Function
					}
				}
            }
        }
        else        // (highSecurityState == HIGH_SECURITY_DISABLED)
        {
            TCHAR strHashedSuppliedPassword[LENGTH_OF_HASH_PASSWORD + LENGTH_OF_NULL];
            (void)memset((void*)strHashedSuppliedPassword, '\0', (LENGTH_OF_HASH_PASSWORD + LENGTH_OF_NULL));

            chrsInString = GetHashStrOfInput_ARM(strHashedSuppliedPassword, suppliedPassword);
            //lint -e{970} Peter Vince 20060601; use of "char" outside of typedef OK
            if (strncmp((char const *)strHashedSuppliedPassword, (char const *)subsystemPassword, chrsInString) == 0)
            {
                returnCode = PASSWORD_MATCH;
            }
        }
    }
    // caller will sort out login pass/fail from this return code
    return returnCode;
}
// end ComparePasswords_ARM()

//-------------------------------------------------------------------------------------------------
/*!
 \brief	This method saves a new password at the specified destination
 \author	Peter Vince
 \date   2006-02-06

 \param[out] pointer to password that is to change
 \param[in]	pointer to new password to overwrite the old

 \return : The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning: none
 \test
 \n by: Peter Vince
 \n result module test: 2006-03-10 OK				
 \n result Lint Level 3: free		
 \bug
 \//APP20180709
*/
//-------------------------------------------------------------------------------------------------
//lint -e{818} Peter Vince 20060602; 1st param not const, because it is "to change".
// Therefore lint error "could be declared as pointing to const" not valid,
TUSIGN16 WriteNewPassword_ARM(TUSIGN16 objectIndex, TCHAR * newPassword)
{
    TUSIGN16 returnCode = (TUSIGN16) OK;      // no reason that this request will fail
    TUSIGN32 lHashData[N_LONGWORDS_DEST_HASH_SHA256] = {0};

    GetHashDataOfInput_ARM(lHashData, newPassword);

    // go and write same to NVRAM
    (void)NvMemSave_ARM(objectIndex, (void FAST *)(&lHashData[0]),
                        sizeof(lHashData));						// ARM Special Function

    return returnCode;
}
// end WriteNewPassword_ARM()

#ifdef ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM
//-------------------------------------------------------------------------------------------------
/*!
 \brief	This method takes the client Idx, along with the Idx of the target-subsystem, and returns a
		pointer to the relevant access-rights table.
 \author	Peter Vince
 \date   2008-04-09

 \param[in] client subsystem index number.
 \param[in] target subsystem index number.

 \return : pointer to access rights table.
 \warning: none
 \test
 \n by: Peter Vince
 \n result module test: 2008-xx-xx TBA				
 \n result Lint Level 3: free		
 \bug
*/
//-------------------------------------------------------------------------------------------------
const TUSIGN16* GetAccessRightsTable_ARM(T_SUBSYSTEM_IDX clientId, T_SUBSYSTEM_IDX subsystemId)
{
	const TUSIGN16 *accessRightTable = NULL;
	const TUSIGN16 **accessRightTableClient = NULL;
	TUSIGN16 tableIndex = 0;
	// accessRightTable set to NULL, unless subsequently identified below

	// If the clientId and target subsystem Id are within the scope of available subsystems...
	if ((clientId < MEMBER_IDX_NUM_ELEMENTS)
	 && (subsystemId < MEMBER_IDX_NUM_ELEMENTS))
	{
		// ...and the client index is recognised as being that of a client...
		// ...then go and get the address of the required access right table
		#ifdef HMI_CLIENT_PRESENT
		if (clientId == HMI_IDX)
		{
            accessRightTable = accessRightTableHmi;
			accessRightTableClient = &accessRightTable;    //change by Spring temply from accessRightTableHmi to &accessRightTableHmi
		}
		#endif
		#ifdef HART_CLIENT_PRESENT
		if (clientId == HART_IDX)
		{
            accessRightTable = accessRightTableHart;
			accessRightTableClient = &accessRightTable;    //change by Spring temply from accessRightTableHmi to &accessRightTableHmi
		}
		#endif
		// As long as there is a client table identified, we should be able to furnish a table...
		if (accessRightTableClient != NULL)
		{
			tableIndex = 0;
			
			// The table consists of a series of pointers to access-rights tables
			while (accessRightTableClient[tableIndex])
			{
				// While traversing this table, we need to identify our subsystem index in one
				// of the identified tables. It will be in the very first table-position...
				if ((TUSIGN16) *accessRightTableClient[tableIndex] == subsystemId)
				{
					accessRightTable = accessRightTableClient[tableIndex];
					break;
				}
				// ... or not at all, in which case move to the next table.
				tableIndex++;
			}
			// If we haven't found our subsystem index in any of the tables, it is because
			// this project has no access-rights identified for it. Instead, what we're now
			// looking for is the TABLE_TERMINATION identity. There MUST be one (else see caveat).
			if (accessRightTable == NULL)
			{
				tableIndex = 0;
				while (tableIndex < MEMBER_IDX_NUM_ELEMENTS)
				{
					if ((TUSIGN16) *accessRightTableClient[tableIndex] == TABLE_TERMINATION)
					{
						// It doesn't particularly matter if more than one subsystem has an empty
						// (ie: only entry is TABLE_TERMINATION) table; as long as this is what
						// "CheckAccessRights" finds for any enquiry of the subsystem.
						accessRightTable = accessRightTableClient[tableIndex];
						break;
					}
					tableIndex++;
				}
				// The only caveat to using this is:
				// 1. There's no match for the subsystem index, AND
				// 2. There's no table(s) consisting of just TABLE_TERMINATION
				// In this scenario, you'll get CheckAccessRights returns "OK" by default!
			}
		}
	}
	
	return	accessRightTable;
}
// end GetAccessRightsTable_ARM()
#endif //ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
// Public methods of T_ARM
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER LOGINEXE> \endif
 \brief  Called by a "Client" subsystem (eg: HMI, HART...) in order to login a user
 \param  callerId Subsystem index relating to the Client
 \param  accessLevel Identity code for the user
 \param  password 5 character password for the requesting user
 \if @SubGen end@ <METHODHEADER LOGINEXE> \endif
 \author Peter Vince
 \date 2006-01-12

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2006-03-10
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
 //APP20180709
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD LOGINEXE>
TUSIGN16 LogInEXE_ARM(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* password)
//@SubGen end@<METHOD LOGINEXE>
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;// pre-suppose that this request will fail
    TUSIGN8 clientLoginState = (TUSIGN8) ARM_NO_USER;
    TUSIGN8 loginLevel = (TUSIGN8) ARM_NO_USER;
    TUSIGN8 clientAccessLevel = (TUSIGN8) NO_ACCESS;
	// Get high-security access state
    TUSIGN8 highSecurityState = ARM_HIGH_SECURITY_DISABLED;

	TUSIGN32* pHashedPassword = NULL;
    TCHAR strHashedSubsystemPassword[LENGTH_OF_HASH_PASSWORD + LENGTH_OF_NULL];
                                                    // string representation of U32 hashed password
    TUSIGN16 chrsInString = 0;
	TBOOL bLoginPossible = eFALSE;					// pre-suppose operation will fail

	TUSIGN8 readOnlySwitchStatus = ARM_READ_ONLY_SWITCH_INACTIVE;

	TUSIGN8 serviceAccountState = ARM_CUSTODY_SWITCH_ACTIVE;     //APP20180708

	// Bug Id: EMF/497 20071031; only read the actual hi-sec state if client wants Service Level
	if (accessLevel == ARM_SERVICE_USER)
	{
		ENABLE_DATA_PROTECTION(armDataSemaphore);
		highSecurityState = armStaticFrequentConstantDefault_2.hiSecAccessState;
		serviceAccountState = armStaticFrequentConstantDefault_Password.serviceAccountSwitch;  // //APP20180708, for cyber security requirment
        DISABLE_DATA_PROTECTION(armDataSemaphore);

		/*  //APP20180708, for cyber security requirment */
        if (serviceAccountState == ARM_CUSTODY_SWITCH_INACTIVE)
        {
            return (TUSIGN16)STATE_DENIED;
        }
	}
	// Pre-check whether login is worth pursuing; could save fruitless protracted tests later...
	bLoginPossible = PreLoginCheck_ARM(accessLevel,
									   highSecurityState, password);	// ARM Special Function
	if (bLoginPossible == eTRUE)
	{
		returnCode = (TUSIGN16)OK;    // ARM Special Function

    	ENABLE_DATA_PROTECTION(armDataSemaphore);

		// If GetClientLoginLevel changed the supplied access level to "custody" then we have to
		// acknowledge this and change what we do by way of processing this request from standard.
		if (accessLevel == ARM_CUSTODY_USER)
		{
			if (returnCode == (TUSIGN16) OK)
			{
				// stored password is TUSIGN32 "hashed"
				pHashedPassword = (TUSIGN32*)armStaticFrequentConstantDefault_Password.stdUserPassword;
				// the implication of being here is that the user requested "Standard" level access
				// and that the custody switch is engaged.
				loginLevel = (TUSIGN8) ARM_CUSTODY_USER;
			}
			// else rule-violation because access level sought is > Std with custody switch engaged
	    }
    	else        // armDynamic.custodySwitchStatus == CUSTODY_SWITCH_INACTIVE
	    {
	        loginLevel = accessLevel;               // access level sought will be the login level...

	        switch (accessLevel)
	        {
	            // Get subsystem password from config. Could be a simple, alphanumeric char [5],
	            // or it could be a TUSIGN32 "hashed" password
	        case ARM_STANDARD_USER:
	            // stored password is TUSIGN32 "hashed"
	            pHashedPassword = (TUSIGN32*)armStaticFrequentConstantDefault_Password.stdUserPassword;
	            break;
			case ARM_ADVANCED_USER:
				// stored password is TUSIGN32 "hashed"
				pHashedPassword = (TUSIGN32*)armStaticFrequentConstantDefault_Password.advUserPassword;
				break;
			case ARM_SERVICE_USER:
				// Get high-security access state - moved to start of this function
				if (highSecurityState == (TUSIGN8) ARM_HIGH_SECURITY_DISABLED)
				{
					// stored password is TUSIGN32 "hashed"
					pHashedPassword = (TUSIGN32*)armStaticFrequentConstantDefault_Password.svcUserPassword;
				}
				// No password applicable for high-security enabled (use deviceId / PIN instead)
				break;
			default:
				loginLevel = (TUSIGN8) ARM_NO_USER;     // ...except where we fail to recognise the level
				returnCode = (TUSIGN16) RULE_VIOLATION_ERR;
				break;
			}
		}
		DISABLE_DATA_PROTECTION(armDataSemaphore);

		if (loginLevel != (TUSIGN8) ARM_NO_USER)
		{
			// Bug Id: EMF/738 20090820; check state is "Not Disabled" rather than "Enabled"
			if ((loginLevel == (TUSIGN8) ARM_SERVICE_USER)
			 && (highSecurityState != (TUSIGN8) ARM_HIGH_SECURITY_DISABLED))
			{
				// size of subsystem password, in the case of high security enabled, needs to be
				// declared as "zero", as it plays no part in the forthcoming call to
				// "ComparePasswords_ARM"
				chrsInString = 0;
				(void)memset((void*)strHashedSubsystemPassword, '\0', (LENGTH_OF_HASH_PASSWORD + LENGTH_OF_NULL));
			}
			else
			{
				// convert hashedSubsystemPassword to string format
				//lint -e{730}
				chrsInString = GetStrOfHashData_ARM(strHashedSubsystemPassword, pHashedPassword);
			}
			// compare entered Password with Subsystem Password
			if (ComparePasswords_ARM((const TCHAR *)strHashedSubsystemPassword,
									 password,
									 highSecurityState,
									 chrsInString,      // size of hashed, subsystem password
									 ACCESS_SPECIFIED_LEVEL) == PASSWORD_MATCH)
			{
				// grant login pass for a password match
				clientLoginState = loginLevel;
				// and read-only for now, pending final status check, below
				clientAccessLevel = (TUSIGN8) READ_ONLY_ACCESS;
				returnCode = (TUSIGN16) OK;
			}
			else
			{
				// login fail in the event of a password mismatch
				clientLoginState = (TUSIGN8) ARM_NO_USER;
				// and read-only access applies, regardless
				clientAccessLevel = (TUSIGN8) READ_ONLY_ACCESS;
				returnCode = (TUSIGN16) RULE_VIOLATION_ERR;    // clientLoginState is NO_USER
			}
		}
		// if all the above checks out OK...
		// ...read the client-enable and read-only switch status; A "client" is a subsystem which
		// needs to determine the access rights for a particular object within a particular subsystem,
		// such as HMI, HART, others which may be required in future implementations.

		// Below we determine and assign final logged-in user rights, based on what is given in the
		// access-right table for a particular client, and also on any overriding determinants, such as
		// hardware-switch settings:

		ENABLE_DATA_PROTECTION(armDataSemaphore);
		readOnlySwitchStatus = armDynamic.readOnlySwitchStatus;

		#ifdef HMI_CLIENT_PRESENT
		if (callerId == (T_SUBSYSTEM_IDX)HMI_IDX)  // for a user wanting to log into HMI subsystem:
		{
			if ((clientLoginState != (TUSIGN8) ARM_NO_USER)
			 && (readOnlySwitchStatus == (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE)
			 && ((armStaticFrequentConstantDefault.clientEnableState == (TUSIGN8) ARM_HMI_ENABLED)
			  #ifdef HART_CLIENT_PRESENT
			  || (armStaticFrequentConstantDefault.clientEnableState == (TUSIGN8) ARM_HART_AND_HMI_ENABLED)
			  #endif
				)
			   )
			{
				// assign logged-in user rights (currently RO, promote to RW if applicable)
				clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;
			}
			armDynamic.hmiLoginState = clientLoginState;
			hmiAccessLevel = clientAccessLevel;
		}
		#endif //HMI_CLIENT_PRESENT

		#ifdef HART_CLIENT_PRESENT
		if (callerId == (T_SUBSYSTEM_IDX)HART_IDX) // for a user wanting to log into HART subsystem:
		{	if (//(clientLoginState != (TUSIGN8) ARM_NO_USER)0 &&
				(readOnlySwitchStatus == (TUSIGN8) ARM_READ_ONLY_SWITCH_INACTIVE)
			 && ((armStaticFrequentConstantDefault.clientEnableState == (TUSIGN8) ARM_HART_ENABLED)
			  #ifdef HMI_CLIENT_PRESENT
			  || (armStaticFrequentConstantDefault.clientEnableState == (TUSIGN8) ARM_HART_AND_HMI_ENABLED)
			  #endif
				)
			   )
			{
				// assign logged-in user rights (currently RO, promote to RW if applicable)
				clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;
			}
			armDynamic.hartLoginState = clientLoginState;
			hartAccessLevel = clientAccessLevel;
		}
		#endif //HART_CLIENT_PRESENT

    	DISABLE_DATA_PROTECTION(armDataSemaphore);
	}//end login

	// Following a login attempt, (ARM) will check to see if any post-login function
	// has been asked for. This would have been defined in arm_bespoke.h
	(void)PostLoginCheck_ARM(returnCode, password);				// ARM Special Function

	return returnCode;
}
// end LogInEXE_ARM()

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER LOGOFFEXE> \endif
 \brief  Called by a "Client" subsystem (eg: HMI, HART...) in order to logoff a user
 \param  callerId Subsystem index relating to the Client
 \if @SubGen end@ <METHODHEADER LOGOFFEXE> \endif
 \author Peter Vince
 \date 2006-01-12

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2006-03-10
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD LOGOFFEXE>
TUSIGN16 LogOffEXE_ARM(T_SUBSYSTEM_IDX callerId)
//@SubGen end@<METHOD LOGOFFEXE>
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;	// pre-suppose this request will fail

	#ifdef HMI_CLIENT_PRESENT
    if (callerId == (T_SUBSYSTEM_IDX)HMI_IDX)								// for user logged into HMI subsystem:
    {
		ENABLE_DATA_PROTECTION(armDataSemaphore);
		armDynamic.hmiLoginState = (TUSIGN8) ARM_NO_USER;	// reset login to "No User"
		DISABLE_DATA_PROTECTION(armDataSemaphore);
        hmiAccessLevel = HmiAccessLevelInit_ARM();			// ARM Special Function (reset access)
        returnCode = (TUSIGN16) OK;
	}
	#endif //HMI_CLIENT_PRESENT

	#ifdef HART_CLIENT_PRESENT
    if (callerId == (T_SUBSYSTEM_IDX)HART_IDX)								// for user logged into HART subsystem:
    {
		ENABLE_DATA_PROTECTION(armDataSemaphore);
        armDynamic.hartLoginState = (TUSIGN8) ARM_NO_USER;	// reset login to "No User"
		DISABLE_DATA_PROTECTION(armDataSemaphore);
		hartAccessLevel = HartAccessLevelInit_ARM();		// ARM Special Function (reset access)
        returnCode = (TUSIGN16) OK;
    }
	#endif //HART_CLIENT_PRESENT

	// Following a logoff attempt, (ARM) will check to see if any post-logoff function
	// has been asked for. This would have been defined in arm_bespoke.h
	PostLogoffCheck_ARM(callerId);							// ARM Special Function

    return returnCode;
}
// end LogOffEXE_ARM()

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHECKACCESSRIGHTSSRV> \endif
 \brief  Called by a Client in order to check access permissions
 \param  callerId Subsystem index relating to the Client
 \param  targetId Subsystem index relating to the subsystem in which the data is held
 \param  objectId Object index for the data in the target subsystem
 \param  attributeId Attribute id for the data within the target object
 \param  accessRequested Required level of access (code) for the requesting user
 \if @SubGen end@ <METHODHEADER CHECKACCESSRIGHTSSRV> \endif
 \author Peter Vince
 \date 2006-01-12

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2006-03-10
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
// Access Rights Table definitions
                                                // The offset from the next entry which identifies:
#define SUBSYSTEM_IDX_OFFSET        (0)         // the target subsystem
#define TARGET_IDX_OFFSET           (1)         // the object required
#define ATTRIBUTE_IDX_OFFSET        (2)         // the attribute required
#define ACCESS_REQUIRED_OFFSET      (3)         // the access-rights being sought
#define NEXT_TABLE_ENTRY_OFFSET     (4)         // the next entry
                                                // within the access-rights entry (16-bit field):
#define BASIC_USER_MASK             (0x0003)    // isolate BASIC rights
#define BASIC_USER_ISOLATE          (0)         // ...and shift to position (bit 1: bit 0)
#define STANDARD_USER_MASK          (0x000C)    // isolate STANDARD rights
#define STANDARD_USER_ISOLATE       (2)         // ...and shift to position (bit 1: bit 0)
#define ADVANCED_USER_MASK          (0x0030)    // isolate ADVANCED rights
#define ADVANCED_USER_ISOLATE       (4)         // ...and shift to position (bit 1: bit 0)
#define SERVICE_USER_MASK           (0x00C0)    // isolate SERVICE rights
#define SERVICE_USER_ISOLATE        (6)         // ...and shift to position (bit 1: bit 0)
#define CUSTODY_USER_MASK           (0x0300)    // isolate CUSTODY rights
#define CUSTODY_USER_ISOLATE        (8)         // ...and shift to position (bit 1: bit 0)
//@SubGen start@<METHOD CHECKACCESSRIGHTSSRV>
TUSIGN16 CheckAccessRightsSRV_ARM(T_SUBSYSTEM_IDX callerId, T_SUBSYSTEM_IDX targetId, TUSIGN16 objectId, TUSIGN16 attributeId, TUSIGN8 accessRequested)
//@SubGen end@<METHOD CHECKACCESSRIGHTSSRV>
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;	// pre-suppose access WON'T be granted
    TUSIGN8 clientLoginState = (TUSIGN8) ARM_NO_USER;
    TUSIGN8 clientAccessLevel = (TUSIGN8) NO_ACCESS;
    const TUSIGN16 *accessRightTablePointer = NULL;
    TUSIGN16 tableAccessRights = (TUSIGN16) NO_ACCESS;
    TUSIGN16 consolidatedAccessRights = (TUSIGN16) NO_ACCESS;
	TBOOL accessCheckPass = eTRUE;					// pre-suppose any preconditions will be met

	// If write access is being sought...
	if ((accessRequested == WRITE_ONLY_ACCESS) || (accessRequested == READ_WRITE_ACCESS))
	{
		// ...check if there's any overriding circumstance where we might not be able to write
		accessCheckPass = VariableWriteCheck_ARM(targetId, objectId);// ARM Special Function
	}

	if (accessCheckPass == eFALSE)					// Can we write to this variable?
	{
		returnCode = (TUSIGN16) WRONG_STATE_ERR;	// No - return appropriate error code
	}
	else //(accessCheckPass == eTRUE)				// Yes - proceed...
	{												// ...pre-supposing that access WILL be granted
		returnCode = (TUSIGN16) OK;					// 20070730: this strategy is in response to a
    												// request to shorten the client lookup table;
		// Get the login status / access level for this client; also user the callerId to select the
		// appropriate access right table

		#ifdef HMI_CLIENT_PRESENT
		if (callerId == (T_SUBSYSTEM_IDX)HMI_IDX)						// for a user logged into HMI subsystem:
		{	clientLoginState = armDynamic.hmiLoginState;		// get login state
			clientAccessLevel = hmiAccessLevel;					// get access level
			#ifdef ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM
			accessRightTablePointer = GetAccessRightsTable_ARM(callerId, targetId);
			#else
			accessRightTablePointer = accessRightTableHmi;     // locate the access right table
			#endif //ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM
		}
		#endif //HMI_CLIENT_PRESENT

		#ifdef HART_CLIENT_PRESENT
		if (callerId == (T_SUBSYSTEM_IDX)HART_IDX)						// for a user logged into HART subsystem:
		{
			clientLoginState = armDynamic.hartLoginState;		// get login state
			clientAccessLevel = HartAccessLevel_ARM(targetId,	// get access level, via:
													objectId,	// ARM Special Function
													hartAccessLevel);
			accessRightTablePointer = accessRightTableHart;     // locate the access right table
		}
		#endif //HART_CLIENT_PRESENT

		// ARM Special Function: to satisfy Measuring Instruments Directive requirement(s)
		MIDOverrideCheck_ARM(targetId, objectId, attributeId,
							 clientLoginState, &accessRequested, &returnCode);
		// Assuming we have an access right table to search
		// and the caller is asking for some kind of legal access...
		if ((accessRightTablePointer != NULL) && (accessRequested > (TUSIGN8) NO_ACCESS)
			&& (accessRequested <= (TUSIGN8) READ_WRITE_ACCESS))
		{
			TUSIGN8 readOnlySwitchStatus = ARM_READ_ONLY_SWITCH_INACTIVE;
			(void)arm_Get((TUSIGN16)ARM_IDX_readOnlySwitchStatus, (TINT16)WHOLE_OBJECT, (void FAST*)&readOnlySwitchStatus);

			// ...use "targetId", "objectId", and "attributeId" to locate the correct item in the Table
			while (accessRightTablePointer[SUBSYSTEM_IDX_OFFSET] != TABLE_TERMINATION)
			{
				if ((accessRightTablePointer[SUBSYSTEM_IDX_OFFSET] == (TUSIGN8) targetId)
				  && (accessRightTablePointer[TARGET_IDX_OFFSET] == objectId)
				  && ((accessRightTablePointer[ATTRIBUTE_IDX_OFFSET] == attributeId)
				   || (accessRightTablePointer[ATTRIBUTE_IDX_OFFSET] == ANY_ATTRIBUTE))
				   )
				{
					// ...then isolate the access-rights offerred by the table
					tableAccessRights = accessRightTablePointer[ACCESS_REQUIRED_OFFSET];
					{
						if (clientLoginState == (TUSIGN8) ARM_NO_USER)
						{
							tableAccessRights &= BASIC_USER_MASK;
							tableAccessRights = tableAccessRights >> BASIC_USER_ISOLATE;
						}
						else if (clientLoginState == (TUSIGN8) ARM_STANDARD_USER)
						{
							tableAccessRights &= STANDARD_USER_MASK;
							tableAccessRights = tableAccessRights >> STANDARD_USER_ISOLATE;
						}
						else if (clientLoginState == (TUSIGN8) ARM_ADVANCED_USER)
						{
							tableAccessRights &= ADVANCED_USER_MASK;
							tableAccessRights = tableAccessRights >> ADVANCED_USER_ISOLATE;
						}
						else if (clientLoginState == (TUSIGN8) ARM_SERVICE_USER)
						{
							tableAccessRights &= SERVICE_USER_MASK;
							tableAccessRights = tableAccessRights >> SERVICE_USER_ISOLATE;

							// if a Service User wanting access to the "client enable" status...
							if ((targetId == (TUSIGN16)ARM_IDX) && (objectId == (TUSIGN16)ARM_IDX_clientEnableState))
							{
								// ...promote access to "Read Write", so as not to permanently
								// lock out any individual client completely.
								clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;
							}
						}
						else if (clientLoginState == (TUSIGN8) ARM_CUSTODY_USER)
						{
							tableAccessRights &= CUSTODY_USER_MASK;
							tableAccessRights = tableAccessRights >> CUSTODY_USER_ISOLATE;
						}
						else
						{
							// unidentified user - default to "no rights" (already set)
							tableAccessRights = (TUSIGN16) NO_ACCESS;
						}
						// Compare "access required" with the "access rights" for the logged-
						// -in user. Determining Access Rights for a user include examining
						// the clientAccessLevel, which may not always agree with the table
						// permissions.

						// Note that if, for instance, the read-only switch is engaged, this
						// would be reported in clientAccessLevel and would therefore override
						// the permissions in the access right table.
						consolidatedAccessRights = tableAccessRights & clientAccessLevel;

						if (accessRequested <= consolidatedAccessRights)
						{
							// Even if "requested" does not exceed "rights", there is still
							// the unique case where WO request must exactly match WO right.
							// The previous test would fail to prevent "requested" = RO, when
							// "rights" = WO, so we do this now:
							if ((consolidatedAccessRights == (TUSIGN16) WRITE_ONLY_ACCESS)
								&& (accessRequested != (TUSIGN8) WRITE_ONLY_ACCESS))
							{
								// specific test barring all except WO
								returnCode = (TUSIGN16) WRONG_STATE_ERR;
							}
							else
							{
								// otherwise request granted
								returnCode = (TUSIGN16) OK;
							}
						}
						else
						{
							// request exceeds rights
							if(readOnlySwitchStatus == ARM_READ_ONLY_SWITCH_ACTIVE)
							{
								returnCode = (TUSIGN16) READ_ONLY_ERR;
							}
							else
							{
								returnCode = (TUSIGN16) WRONG_STATE_ERR;
							}
						}
						break;
					}
				}
				// end targetId, objectId/frameId, attributeId check
				accessRightTablePointer = &accessRightTablePointer[NEXT_TABLE_ENTRY_OFFSET];
				// instead of accessRightTablePointer += NEXT_TABLE_ENTRY_OFFSET
				// which contravenes MISRA C rule 101
			}
			// while table still valid
			// Check if we reached the end of the table without finding the target
			if (accessRightTablePointer[SUBSYSTEM_IDX_OFFSET] == TABLE_TERMINATION)
			{
				// We did reach the end of the table; so no target match.
				// This block of code, effectively a repetition of above, makes the assumption
				// that we can allow RW access provided the clientAccessLevel permits.
                clientAccessLevel = READ_WRITE_ACCESS; // 04-12
				consolidatedAccessRights = (TUSIGN8) READ_WRITE_ACCESS & clientAccessLevel;
				if (accessRequested <= consolidatedAccessRights)
				{
					// Even if "requested" does not exceed "rights", there is still
					// the unique case where WO request must exactly match WO right.
					// The previos test would fail to prevent "requested" = RO, when
					// "rights" = WO, so we do this now:
					if ((consolidatedAccessRights == (TUSIGN16) WRITE_ONLY_ACCESS)
						&& (accessRequested != (TUSIGN8) WRITE_ONLY_ACCESS))
					{
						// specific test barring all except WO
						returnCode = (TUSIGN16) WRONG_STATE_ERR;
					}
					else
					{
						// otherwise request granted
						returnCode = (TUSIGN16) OK;
					}
				}
				else
				{
					// request exceeds rights
					if(readOnlySwitchStatus == ARM_READ_ONLY_SWITCH_ACTIVE)
					{
						returnCode = (TUSIGN16) READ_ONLY_ERR;
					}
					else
					{
						returnCode = (TUSIGN16) WRONG_STATE_ERR;
					}
				}
			}
		}
		// end if (accessCheckPass == eTRUE)
	}
	if ((targetId == (TUSIGN16)ELETTRONICSERVICES_IDX) && (objectId == (TUSIGN16)ELETTRONICSERVICES_IDX_swWriteProtection))
	{
		if ((clientLoginState == (TUSIGN8) ARM_SERVICE_USER) ||
			(clientLoginState == (TUSIGN8) ARM_ADVANCED_USER))
		{
			// ...promote access to "Read Write"
			clientAccessLevel = (TUSIGN8) READ_WRITE_ACCESS;
			hartAccessLevel = clientAccessLevel;
			hmiAccessLevel = clientAccessLevel;
			returnCode = (TUSIGN16) OK;
		}
	}
	return returnCode;
}
// end CheckAccessRightsSRV_ARM()

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETPASSWORDSEXE> \endif
 \brief  Called by a Client in order to reset all passwords to default
 \param  password The hashed password, with feature-bit set to "Reset All Passwords"
 \if @SubGen end@ <METHODHEADER RESETPASSWORDSEXE> \endif
 \author Peter Vince
 \date 2006-01-12

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2006-03-10
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETPASSWORDSEXE>
TUSIGN16 ResetPasswordsEXE_ARM(TCHAR* password)
//@SubGen end@<METHOD RESETPASSWORDSEXE>
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;	// pre-suppose that this request will fail

    (void)ENABLE_DATA_PROTECTION(armDataSemaphore);

    // Compare entered Password with hashed Password that ComparePasswords_ARM will calculate
    // on receipt of the following with "HIGH_SECURITY_ENABLED" (doesn't need a subsystem
    // password, or knowledge of user's login status).
	if (ResetPasswordsCheck_ARM(password) == eTRUE)				// ARM Special Function
    {
        // Reset all simple passwords (write defaults to non-vol password entries).
        // Note that if one "save" results in an error condition, then no point in proceeding.

        // Update Standard password with default
        (void)NvMemSave_ARM((TUSIGN16)ARM_IDX_stdUserPassword,
                            (void FAST *)(&armStaticFrequentConstantDefault_Password.stdDefaultPassword[0]),
                            sizeof(armStaticFrequentConstantDefault_Password.stdDefaultPassword));					// ARM Special Function
        // Update Advanced password with default
        (void)NvMemSave_ARM((TUSIGN16)ARM_IDX_advUserPassword,
                            (void FAST *)(&armStaticFrequentConstantDefault_Password.advDefaultPassword[0]),
                            sizeof(armStaticFrequentConstantDefault_Password.stdDefaultPassword));					// ARM Special Function

        // report non-vol save success
        returnCode = (TUSIGN16) OK;
    }
    (void)DISABLE_DATA_PROTECTION(armDataSemaphore);

    return returnCode;
}
// end ResetPasswordsEXE_ARM()

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CHANGEPASSWORDEXE> \endif
 \brief  Called by a Client in order to change a users own (or subordinate) password
 \param  callerId Subsystem index relating to the Client
 \param  accessLevel Identity code for the user to be changed
 \param  newPassword New 5 character password for the userId whose password is to change
 \if @SubGen end@ <METHODHEADER CHANGEPASSWORDEXE> \endif
 \author Peter Vince
 \date 2006-01-12

 \return The exe error status is returned
            OK = no error (success)
            (Any other code) = warning/error codes
 \warning None
 \test
 test-date: 2006-03-10
 \n by: Peter Vince
 \n environment: C-SPY
 \n intention: 100% code coverage; equivalence partitioning; boundary value analysis
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug None
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHANGEPASSWORDEXE>
TUSIGN16 ChangePasswordEXE_ARM(T_SUBSYSTEM_IDX callerId, TUSIGN8 accessLevel, TCHAR* newPassword)
//@SubGen end@<METHOD CHANGEPASSWORDEXE>
{
    TUSIGN16 returnCode = (TUSIGN16) RULE_VIOLATION_ERR;     // pre-suppose that this request will fail
    TUSIGN8 clientLoginState = (TUSIGN8) ARM_NO_USER;

    (void)ENABLE_DATA_PROTECTION(armDataSemaphore);

    // Get login state for this client

	#ifdef HMI_CLIENT_PRESENT
    if (callerId == (T_SUBSYSTEM_IDX)HMI_IDX)
	{
        clientLoginState = armDynamic.hmiLoginState;       // ...hmi
	}
	#endif //HMI_CLIENT_PRESENT

	#ifdef HART_CLIENT_PRESENT
    if (callerId == (T_SUBSYSTEM_IDX)HART_IDX)
	{
        clientLoginState = armDynamic.hartLoginState;      // ...hart
	}
	#endif //HART_CLIENT_PRESENT

    // Check that this access level is appropriate for the current logged-in client, such that
    // he/she would be permitted to change the specified password (the requested access level
    // must be the same or lower than the login level).
    if ((accessLevel > (TUSIGN8) ARM_NO_USER)				// ensure that a password is applicable
        && (clientLoginState > (TUSIGN8) ARM_NO_USER)		// ensure that a user is logged in
        && ((clientLoginState == (TUSIGN8) ARM_SERVICE_USER) || (accessLevel <= clientLoginState)))
        // ensure that above permission applies
    {
        TUSIGN16 objectIndex;
        // User for a specified client has requested to change password for a specified level
        switch (accessLevel)
        {
        case ARM_STANDARD_USER:
            objectIndex = (TUSIGN16)ARM_IDX_stdUserPassword;
            break;
        case ARM_ADVANCED_USER:
            objectIndex = (TUSIGN16)ARM_IDX_advUserPassword;
            break;
        case ARM_STANDARD_DEFAULT:
            objectIndex = (TUSIGN16)ARM_IDX_stdDefaultPassword;
            break;
        case ARM_ADVANCED_DEFAULT:
            objectIndex = (TUSIGN16)ARM_IDX_advDefaultPassword;
            break;
        default:
			objectIndex = 0xFFFF;	// Error - access-level not recognised; set index to exclude
            break;
        }
        if (objectIndex != 0xFFFF)
        {
			// objectId recognised; store user-entered new password, overwriting the old one
			returnCode = WriteNewPassword_ARM(objectIndex, newPassword);
		}
    }
    (void)DISABLE_DATA_PROTECTION(armDataSemaphore);

    return returnCode;
}
// end ChangePasswordEXE_ARM()

//-------------------------------------------------------------------------------------------------

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/
