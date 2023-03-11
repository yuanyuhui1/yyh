#ifndef _VERSION_SIZE_A_H_
#define _VERSION_SIZE_A_H_

#if defined(_SWITCHES_STANDARD)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_A_STD_SWITCH        // HMI Size A
        #else
            #define HARDWARE_REVISION               HW_NORMAL_A_STD_SWITCH        // HMI Size A
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_A_HH_TERMINAL       // Hand held terminal Size B
        #else
            #define HARDWARE_REVISION               HW_NORMAL_A_HH_TERMINAL       // Hand held terminal Size B
        #endif
    #else
        #error Enclosure not defined
    #endif
#elif defined(_SWITCHES_CAPACITIVE)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_A_CAP_SWITCH        // HMI Size A with capacitive switches
        #else
            #define HARDWARE_REVISION               HW_NORMAL_A_CAP_SWITCH        // HMI Size A with capacitive switches
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #error Size A hand-held terminal not available
    #else
        #error Enclosure not defined
    #endif
#elif defined(_SWITCHES_NOT_FITTED)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_A_INDICATOR_ONLY    // Size A indicator
        #else
            #define HARDWARE_REVISION               HW_NORMAL_A_INDICATOR_ONLY    // Size A indicator
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #error Size A hand-held terminal not available
    #else
        #error Enclosure not defined
    #endif
#else
    #error switches not defined
#endif

#endif  // #ifndef _VERSION_SIZE_A_H_

