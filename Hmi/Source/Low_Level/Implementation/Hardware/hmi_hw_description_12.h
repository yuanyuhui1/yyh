#ifndef _HARDWARE_DESCRIPTION_12_H_
#define _HARDWARE_DESCRIPTION_12_H_

//--------------------------------------------------------------------------------------------------
// NV Description
//--------------------------------------------------------------------------------------------------
//#define _NONVOL_IMPLEMENTATION

//--------------------------------------------------------------------------------------------------
// LCD Size
//--------------------------------------------------------------------------------------------------
#define DISPLAY_SIZE_B

//--------------------------------------------------------------------------------------------------
// Switch Description
//--------------------------------------------------------------------------------------------------
//#define _SWITCHES_UNDEFINED
#define _SWITCHES_STANDARD
//#define _SWITCHES_CAPACITIVE
//#define _SWITCHES_NOT_FITTED

//--------------------------------------------------------------------------------------------------
// Enclosure Description
//--------------------------------------------------------------------------------------------------
//#define _ENCLOSURE_UNDEFINED
//#define _ENCLOSURE_STANDARD
#define _ENCLOSURE_HHTERMINAL

//--------------------------------------------------------------------------------------------------
// Program NV Build
//--------------------------------------------------------------------------------------------------
//#define _PROGRAM_NONVOL
#ifdef _PROGRAM_NONVOL
    // Ensure correct definitions are included
    #ifndef _NONVOL_IMPLEMENTATION
        #define _NONVOL_IMPLEMENTATION
    #endif
#endif

#endif  // #ifndef _HARDWARE_DESCRIPTION_12_H_
