//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module			arm_bespoke.h
 Description    Project-dependent header for ARM component

                This is for individual projects to specify which clients are to be supported,
				what hardware is supported etc.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_bespoke.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_BESOPKE_H__
  #error Arm_bespoke.h included more than once
#endif

#define __ARM_BESPOKE_H__

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

// Add your own project definitions here. Note that this header file, arm_bespoke.h, is not part
// of the ARM common component, but is nevertheless required in order to tailor your project's
// ARM requirements. As this file is not an ARM common component distributable, you may remove
// any project information not applicable to your own project. It is recommended that project
// definition macros are used to make it clear which project this header is intended for.

//-------------------------------------------------------------------------------------------------
// Definitions for EMF project (ProcessMaster and WaterMaster electromagnetic flowmeters).
// This file must be located in the EMF project "..\ARM\interface\" folder only,
// where it is referenced by "arm.h", and no other file.

#if defined _EMF_WW || defined _EMF_INDUSTRIAL 
	#define EMF_SPECIAL_FEATURES
	#ifndef __SYSTEM_HARDWARE_
		#include "System/Interface/microcontroller.h"	// For inclusion of hardware options
	#endif
#endif

#if defined _II_WIRE_TOPWORKS
	#ifndef __UC_MICROCONTROLLER_H__
		//#include "..\..\system\interface\microcontroller.h"	// For inclusion of hardware options
	#endif
#endif


#ifdef _EMF_WW												// WaterMaster
	// ARM clients:
	#define HMI_CLIENT_PRESENT								// HMI
	#define HART_CLIENT_PRESENT								// HART

	// co-dependent subsystems:
	#define DEVICE_SUBSYSTEM_PRESENT						// mask if no actual device subsystem

	// ARM hardware:
	#define READ_ONLY_SWITCH								// Read-Only switch; define also:
	#define READ_ONLY_SWITCH_PORT_DR	(PD10_bit.PD10_0)	// Input Port Direction Register
	#define READ_ONLY_SWITCH_PORT		(P10_bit.P10_0)		// Input Port-pin
	// Hardware definitions used by initialization...
	#define ARM_HW_SWITCH_INPUT_PORT	(0)         		// general I/P definition for HW init'n
	#define ARM_HW_SWITCH_INPUT_LOW		(0)         		// general I/P low definition
	#define ARM_HW_SWITCH_INPUT_HIGH	(1)         		// general I/P high definition

	// ARM firmware:
	// TMidSwitchActiveFormats								// Measuring Instruments Directive
	#define ARM_MID_PRESENT									// (MID) applies for this build
	#define ARM_MID_SWITCH_INACTIVE 0 						// MID (FW) switch disengaged
	#define ARM_MID_SWITCH_ACTIVE 1 						// MID (FW) switch engaged

	// ARM default passwords
	// Defined in the Entry-Tool only with reference names, eg "xxxxxxxx_TARGET_PASSWORD;
	// the target developer is encouraged to provide numerical values here according their
	// own projects' requirements. They are referred to in arm.h by the Entry-Tool generated
	// definitions, with the comment "Meaning dependent on target build".
	#define STANDARD_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define ADVANCED_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define SERVICE_TARGET_PASSWORD		103643495			//Hash of 4-chr password = "6000"

	// ARM high-security default
	// Default security selection; this is referred to by the Entry-Tool as the "Default Value" for
	// for the object "hiSecAccessState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define HIGH_SECURITY_DEFAULT ARM_HIGH_SECURITY_ENABLED // high-security enabled

	// ARM client enable default
	// Default client enable selection; this is referred to by the Entry-Tool as the "Default Value"
	// for the object "clientEnableState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define CLIENT_ENABLE_TARGET ARM_HART_AND_HMI_ENABLED	// HART & HMI may log in with WR access

	// Other - mask or enable these as required
	#ifdef _RELEASE_A2_										// For A2 release (not before):
		#define ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM			// Enable ROM-efficient a.r.tables
	#endif
															// Bug Id: EMF/281 20070816;
	#define AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL		// requested in lieu of a direct
															// "Reset Passwords" being available
	#define HASH_CODE_RESET									// Reset operation will check password
#endif

#ifdef _EMF_INDUSTRIAL										// ProcessMaster
	// ARM clients: define list of clients to be supported
	#define HMI_CLIENT_PRESENT								// HMI
	//#define HART_CLIENT_PRESENT								// HART

	// co-dependent subsystems: define any subsystems that ARM operations may depend on
	#define DEVICE_SUBSYSTEM_PRESENT						// mask if no actual device subsystem

	// ARM hardware:
	#define CUSTODY_SWITCH									// Custody switch; define also:
	#define CUSTODY_SWITCH_PORT_DR		(PD10_bit.PD10_0)	// Input Port Direction Register
	#define CUSTODY_SWITCH_PORT			(P10_bit.P10_0)		// Input Port-pin
	#define READ_ONLY_SWITCH								// Read-Only switch; define also:
	#define READ_ONLY_SWITCH_PORT_DR	(PD8_bit.PD8_3)		// Input Port Direction Register
	#define READ_ONLY_SWITCH_PORT		(P8_bit.P8_3)  		// Input Port-pin
	// Hardware definitions used by initialization...
	#define ARM_HW_SWITCH_INPUT_PORT	(0)         		// general I/P definition for HW init'n
	#define ARM_HW_SWITCH_INPUT_LOW		(0)         		// general I/P low definition
	#define ARM_HW_SWITCH_INPUT_HIGH	(1)         		// general I/P high definition

	// ARM default passwords:
	// Defined in the Entry-Tool only with reference names, eg "xxxxxxxx_TARGET_PASSWORD;
	// the target developer is encouraged to provide numerical values here according their
	// own projects' requirements. They are referred to in arm.h by the Entry-Tool generated
	// definitions, with the comment "Meaning dependent on target build".
	#define STANDARD_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define ADVANCED_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define SERVICE_TARGET_PASSWORD		103643495			//Hash of 4-chr password = "6000"

	// ARM high-security default
	// Default security selection; this is referred to by the Entry-Tool as the "Default Value" for
	// for the object "hiSecAccessState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define HIGH_SECURITY_DEFAULT ARM_HIGH_SECURITY_DISABLED // high-security disabled

	// ARM client enable default
	// Default client enable selection; this is referred to by the Entry-Tool as the "Default Value"
	// for the object "clientEnableState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define CLIENT_ENABLE_TARGET ARM_HART_AND_HMI_ENABLED	// HART & HMI may log in with WR access

	// Other - mask or enable these as required
	#ifdef _RELEASE_A2_										// For A2 release (not before):
		#define ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM			// Enable ROM-efficient a.r.tables
	#endif
															// Bug Id: EMF/281 20070816;
	#define AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL		// requested in lieu of a direct
															// "Reset Passwords" being available
	#define HASH_CODE_RESET									// Reset operation will check password
#endif

//-------------------------------------------------------------------------------------------------
// Definitions for ControlMaster project.
// This file must be located in the Controller project "..\ARM\interface\" folder only,
// where it is referenced by "arm.h", and no other file.

#ifdef CONTROLLER_SPECIAL_FEATURES							// ControlMaster
	// ARM clients:
	#define HMI_CLIENT_PRESENT								// HMI
	//#define MODBUS_CLIENT_PRESENT							// MODBUS

	// co-dependent subsystems:
	// none specified

	// ARM hardware:
	// none specified

	// ARM default passwords
	// Defined in the Entry-Tool only with reference names, eg "xxxxxxxx_TARGET_PASSWORD;
	// the target developer is encouraged to provide numerical values here according their
	// own projects' requirements. They are referred to in arm.h by the Entry-Tool generated
	// definitions, with the comment "Meaning dependent on target build".
	#define STANDARD_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define ADVANCED_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL

	#ifdef _ARM_SERVICE_LEVEL_SECURITY_OVERRIDE
    	#define SERVICE_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#else
    	#define SERVICE_TARGET_PASSWORD	103968688			//Hash of 4-chr password = "5000"
	#endif

	// ARM high-security default
	// Default security selection; this is referred to by the Entry-Tool as the "Default Value" for
	// for the object "hiSecAccessState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define HIGH_SECURITY_DEFAULT ARM_HIGH_SECURITY_DISABLED // high-security disabled

	// ARM client enable default
	// Default client enable selection; this is referred to by the Entry-Tool as the "Default Value"
	// for the object "clientEnableState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define CLIENT_ENABLE_TARGET ARM_HMI_ENABLED			// HMI Client may log in with WR access

	// Other
	#define ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM				// Enable ROM-efficient a.r.tables
#endif


#ifdef _II_WIRE_TOPWORKS												// WaterMaster
	// ARM clients:
	#define HMI_CLIENT_PRESENT								// HMI
	#define HART_CLIENT_PRESENT								// HART

	// ARM hardware:
	#define READ_ONLY_SWITCH								// Read-Only switch; define also:
#ifdef READ_ONLY_SWITCH_DIP                                //dip switch for write protected function
	#define READ_ONLY_SWITCH_GPIO_PORT             DIP_SWITCH_1_GPIO_PORT
	#define READ_ONLY_SWITCH_GPIO_PIN              DIP_SWITCH_1_GPIO_PIN
    #define READ_ONLY_SWITCH_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#else
	#define READ_ONLY_SWITCH_PORT_DR	(PD1_bit.PD1_4)	    // Input Port Direction Register
	#define READ_ONLY_SWITCH_PORT		(P1_bit.P1_4)		// Input Port-pin
#endif
	// Hardware definitions used by initialization...
	#define ARM_HW_SWITCH_INPUT_PORT	(0)         		// general I/P definition for HW init'n
	#define ARM_HW_SWITCH_INPUT_LOW		(0)         		// general I/P low definition
	#define ARM_HW_SWITCH_INPUT_HIGH	(1)         		// general I/P high definition

	// ARM firmware:
	// TMidSwitchActiveFormats								// Measuring Instruments Directive
	#define ARM_MID_PRESENT									// (MID) applies for this build
	#define ARM_MID_SWITCH_INACTIVE 0 						// MID (FW) switch disengaged
	#define ARM_MID_SWITCH_ACTIVE 1 						// MID (FW) switch engaged

	// ARM default passwords
	// Defined in the Entry-Tool only with reference names, eg "xxxxxxxx_TARGET_PASSWORD;
	// the target developer is encouraged to provide numerical values here according their
	// own projects' requirements. They are referred to in arm.h by the Entry-Tool generated
	// definitions, with the comment "Meaning dependent on target build".
	#define STANDARD_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	#define ADVANCED_TARGET_PASSWORD	67108864			//Hash of single chr password = NUL
	//#define SERVICE_TARGET_PASSWORD		67108864			//Hash of single chr password = NUL
	#define SERVICE_TARGET_PASSWORD		103643495			//Hash of 4-chr password = "6000"
	//#define SERVICE_TARGET_PASSWORD		103968688			//Hash of 4-chr password = "5000"
	
	// Grace 20180602 eidt for cyber security
	#define HASH_PASSWORD_NULL_1    0xE3B0C442  //Hash of single chr password = NUL
    #define HASH_PASSWORD_NULL_2    0x98FC1C14
    #define HASH_PASSWORD_NULL_3    0x9AFBF4C8
    #define HASH_PASSWORD_NULL_4    0x996FB924
    #define HASH_PASSWORD_NULL_5    0x27AE41E4
    #define HASH_PASSWORD_NULL_6    0x649B934C
    #define HASH_PASSWORD_NULL_7    0xA495991B
    #define HASH_PASSWORD_NULL_8    0x7852B855

    #define HASH_PASSWORD_SERVICE_1 0x6B3B9A6D  //Hash of 4-chr password = "6000"
    #define HASH_PASSWORD_SERVICE_2 0xDB739EA6
    #define HASH_PASSWORD_SERVICE_3 0xB3984E90
    #define HASH_PASSWORD_SERVICE_4 0x38C33EDE
    #define HASH_PASSWORD_SERVICE_5 0xAECFB0EE
    #define HASH_PASSWORD_SERVICE_6 0xA476EBA1
    #define HASH_PASSWORD_SERVICE_7 0x7B606D46
    #define HASH_PASSWORD_SERVICE_8 0x99CA24E1   

	// ARM high-security default
	// Default security selection; this is referred to by the Entry-Tool as the "Default Value" for
	// for the object "hiSecAccessState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define HIGH_SECURITY_DEFAULT ARM_HIGH_SECURITY_DISABLED // high-security enabled

	// ARM client enable default
	// Default client enable selection; this is referred to by the Entry-Tool as the "Default Value"
	// for the object "clientEnableState"; the target developer is encouraged to make a selection
	// here according their own projects' requirements. They are not otherwise defined by the
	// Entry-Tool-generated definitions.
	#define CLIENT_ENABLE_TARGET ARM_HART_AND_HMI_ENABLED	// HART & HMI may log in with WR access   temp definition

	// Other - mask or enable these as required
	#ifdef _RELEASE_A2_										// For A2 release (not before):
		#define ACCESS_RIGHTS_TABLE_PER_SUBSYSTEM			// Enable ROM-efficient a.r.tables
	#endif

	//APP20180709, This function is conflict with cyber security.
															// Bug Id: EMF/281 20070816;
	//#define AUTO_TRY_RESET_PASSWORDS_UPON_LOGIN_FAIL		// requested in lieu of a direct
															// "Reset Passwords" being available
	#define HASH_CODE_RESET									// Reset operation will check password
#endif

//-------------------------------------------------------------------------------------------------
// Device Definitions (for all projects).

#ifdef DEVICE_SUBSYSTEM_PRESENT
	#include "../../device/interface/devicestringformatting.h"// for dec 32 to to ascii conversion
#endif //DEVICE_SUBSYSTEM_PRESENT

//-------------------------------------------------------------------------------------------------
// end arm_bespoke.h
