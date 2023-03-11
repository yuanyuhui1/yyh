#ifndef _VERSION_SIZE_B_H_
#define _VERSION_SIZE_B_H_

#if defined(_SWITCHES_STANDARD)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_B_STD_SWITCH        // HMI Size B
        #else
            #define HARDWARE_REVISION               HW_NORMAL_B_STD_SWITCH        // HMI Size B
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_B_HH_TERMINAL       // Hand held terminal Size B
        #else
            #define HARDWARE_REVISION               HW_NORMAL_B_HH_TERMINAL       // Hand held terminal Size B
        #endif
    #else
        #error Enclosure not defined
    #endif
#elif defined(_SWITCHES_CAPACITIVE)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_B_CAP_SWITCH        // HMI Size B with capacitive switches
        #else
            #define HARDWARE_REVISION               HW_NORMAL_B_CAP_SWITCH        // HMI Size B with capacitive switches
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #error Size B hand-held terminal with capacitive switches not available
    #else
        #error Enclosure not defined
    #endif
#elif defined(_SWITCHES_NOT_FITTED)
    #if defined(_ENCLOSURE_STANDARD)
        #ifdef  _NONVOL_IMPLEMENTATION
            #define HARDWARE_REVISION               HW_EEPROM_B_INDICATOR_ONLY    // Size B indicator
        #else
            #define HARDWARE_REVISION               HW_NORMAL_B_INDICATOR_ONLY    // Size B indicator
        #endif
    #elif defined(_ENCLOSURE_HHTERMINAL)
        #error Size B hand-held indicator not available
    #else
        #error Enclosure not defined
    #endif
#else
    #error switches not defined
#endif

#endif  // #ifndef _VERSION_SIZE_B_H_
