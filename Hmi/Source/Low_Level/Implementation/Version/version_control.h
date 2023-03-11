#ifndef _VERSION_CONTROL_H_
#define _VERSION_CONTROL_H_

#include "Hmi/Source/Low_Level/Implementation/Hardware/hmi_hardware_control.h"

/* ************************************** REVISION HISTORY *****************************************
	MAIN SOFTWARE
	-------------	
	TYPE		VERSION			SUMMARY OF CHANGES							ENGINEER		DATE
	----		-------			------------------							--------		----
	Hardware		1			Board w/o nonvol								MA
					2			Board with nonvol								MA
	Software		1.0			Production release w/o module testing			MA			28/01/05
					1.1			Production release with module testing			MA			14/03/05
					E2.0		Pre-release asian support version...			MA			?/05/06
								1. Reduced global RAM store by 62 bytes so
								display data is now sent in two 'hits'.
								2. Comms made more secure. S/W will support
								main HMI with or without new comms mechanism
								3. HMI with nonvol will support original &
								asian versions of the Main HMI. HMI w/o
								nonvol will only support non-asian version.
								4. Added version info. on startup screen
					E2.1		5. Altered display format of hw/sw revision		MA			30/06/06
                    E3.0        Rogue build that will display
                    E2.2
					
					
	PROGRAM_NV SOFTWARE
	-------------------	
				VERSION			SUMMARY OF CHANGES							ENGINEER
				-------			------------------							--------
					E1.0		Initial version									MA	
					E1.1		1. Added display of language information		MA			30/06/06
								   when programming is successful
								2. Removed the VerifyNonVolData(...) call
								   from InitController(...)
                    R1.2        Corrected bug when displaying information
                                about character set support
 **************************************************************************************************/	

                                          // Size  | Switch | Enclosure | EEPROM |
                                          //-------|--------|-----------|--------|----------------------------------------------
#define HW_NOT_RECOGNISED             0   //   ?   | ?      | ?         | ?      | Some of the setings are defined but the overall combination does not fall into one of the cases below
#define HW_NORMAL_X_X                 1   //   A   | ?      | ?         | Std    | Original version without EEPROM
#define HW_EEPROM_X_X                 2   //   A   | ?      | ?         | EEPROM | Original version with EEPROM

#define HMI_HW_REMOTE_HMI             3   //  n/a  | n/a    | n/a       | n/a    |

#define HW_NORMAL_A_STD_SWITCH        4   //   A   | Std    | Std       | Std    | HMI Size A
#define HW_EEPROM_A_STD_SWITCH        5   //   A   | Std    | Std       | EEPROM | HMI Size A with EEPROM
#define HW_NORMAL_A_CAP_SWITCH        6   //   A   | Cap.   | Std       | Std    | HMI Size A with capacitive switches
#define HW_EEPROM_A_CAP_SWITCH        7   //   A   | Cap.   | Std       | EEPROM | HMI Size A with capacitive switches with EEPROM
#define HW_NORMAL_A_INDICATOR_ONLY    8   //   A   | None   | Std       | Std    | Size A indicator
#define HW_EEPROM_A_INDICATOR_ONLY    9   //   A   | None   | Std       | EEPROM | Size A indicator with EEPROM

#define HW_NORMAL_B_STD_SWITCH       10   //   B   | Std    | Std       | Std    | HMI Size B
#define HW_EEPROM_B_STD_SWITCH       11   //   B   | Std    | Std       | EEPROM | HMI Size B with EEPROM
#define HW_NORMAL_B_HH_TERMINAL      12   //   B   | Std    | handheld  | Std    | Hand held terminal Size B
#define HW_EEPROM_B_HH_TERMINAL      13   //   B   | Std    | handheld  | EEPROM | Hand held terminal Size B with EEPROM
#define HW_NORMAL_B_CAP_SWITCH       14   //   B   | Cap.   | Std       | Std    | HMI Size B with capacitive switches
#define HW_EEPROM_B_CAP_SWITCH       15   //   B   | Cap.   | Std       | EEPROM | HMI Size B with capacitive switches with EEPROM
#define HW_NORMAL_B_INDICATOR_ONLY   16   //   B   | None   | Std       | Std    | Size B indicator
#define HW_EEPROM_B_INDICATOR_ONLY   17   //   B   | None   | Std       | EEPROM | Size B indicator with EEPROM

#define HW_NOT_TYPE_1_INTERFACE      100  //   ?   | ?      | ?         | ?      | Not a type 1 interface solution

#if defined(DISPLAY_SIZE_A) && defined(_SWITCHES_UNDEFINED) && defined(_ENCLOSURE_UNDEFINED)
    #ifdef  _NONVOL_IMPLEMENTATION
        #define HARDWARE_REVISION       HW_EEPROM_X_X                 // Original version without EEPROM
    #else
        #define HARDWARE_REVISION       HW_NORMAL_X_X                 // Original version without EEPROM
    #endif
#else
    #if defined(DISPLAY_SIZE_A)
        #include "Hmi/Source/Low_Level/Implementation/Version/version_size_a.h"
    #elif defined(DISPLAY_SIZE_B)
        #include "Hmi/Source/Low_Level/Implementation/Version/version_size_b.h"
    #elif defined(DISPLAY_SIZE_C)
        #include "Hmi/Source/Low_Level/Implementation/Version/version_size_c.h"
    #elif defined(DISPLAY_SIZE_D)
        #include "Hmi/Source/Low_Level/Implementation/Version/version_size_d.h"
    #elif defined(DISPLAY_SIZE_E)
        #include "Hmi/Source/Low_Level/Implementation/Version/version_size_e.h"
    #else
        #error LCD Size not defined
    #endif
#endif


#ifdef	_RELEASE
	#define	PREFIX		'R'
#else
	#define	PREFIX		'D'
#endif

#ifndef _PROGRAM_NONVOL
    #define	SOFTWARE_REVISION			    35		// 3.5
#else
	#define	SOFTWARE_REVISION			    13		// 1.3
#endif


#endif  // #ifndef _VERSION_CONTROL_H_
