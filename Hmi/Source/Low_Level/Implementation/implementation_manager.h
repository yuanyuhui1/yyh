#ifndef _DRIVER_HW_TYPE_H_
#define _DRIVER_HW_TYPE_H_

#ifdef TYPE_1_IMPLEMENTATION
#include "../../common.h"
#endif

// Common Function prototypes
static void CommonFunctions_DrawIcon(void);
static void CommonFunctions_ScaleFont2(void);
static void CommonFunctions_ScaleFont2TFT(void);
static void CommonFunctions_ScaleFont4(void);
static void CommonFunctions_ScaleFont4TFT(void);
static void CommonFunctions_ScaleFont5(void);
static void CommonFunctions_ScaleFont5TFT(void);
static void CommonFunctions_BuildImageEuropean(void);
static void CommonFunctions_BuildImageEuropeanTFT(void);
static void CommonFunctions_TransmitImage(TUSIGN8 *imageByte);

// Implementation specific prototypes
static void Implementation_DrawIcon_Type1_NV(void);
static void Implementation_TransmitImage_Type4(TUSIGN8 *imageByte);
static void Implementation_TransmitImage_Type5(TUSIGN8 *imageByte);

// Common Macros
#include "common\macros\common_type_macros.h"
#include "common\macros\common_size_macros.h"

// Trap for invalid display manipulation (flip/rotate) options
#if defined(TYPE_1_IMPLEMENTATION)
    #if defined(DISPLAY_FLIP_HORIZONTAL) || defined(DISPLAY_FLIP_VERTICAL) || defined(DISPLAY_ROTATE_180)
    #error Flip and Rotate not available with TYPE_1_IMPLEMENTATION
    #endif
#elif defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

    #if defined(DISPLAY_FLIP_HORIZONTAL)
        #if defined(DISPLAY_FLIP_VERTICAL) || defined(DISPLAY_ROTATE_180)
            #define MULTIPLE_DISPLAY_MANIPULATION_OPTIONS
        #endif
    #elif defined(DISPLAY_FLIP_VERTICAL)
        #if defined(DISPLAY_ROTATE_180) || defined(DISPLAY_FLIP_HORIZONTAL)
            #define MULTIPLE_DISPLAY_MANIPULATION_OPTIONS
        #endif
    #elif defined(DISPLAY_ROTATE_180)
        #if defined(DISPLAY_FLIP_HORIZONTAL) || defined(DISPLAY_FLIP_VERTICAL)
            #define MULTIPLE_DISPLAY_MANIPULATION_OPTIONS
        #endif
    #endif

    #ifdef MULTIPLE_DISPLAY_MANIPULATION_OPTIONS
        #error multiple display manipulation (flip/rotate) options cannot be defined
    #endif

#elif defined(TYPE_4_IMPLEMENTATION)
    #if defined(DISPLAY_FLIP_HORIZONTAL) || defined(DISPLAY_FLIP_VERTICAL) || defined(DISPLAY_ROTATE_180)
    #error Flip and Rotate not available with TYPE_4_IMPLEMENTATION
    #endif
#elif defined(TYPE_5_IMPLEMENTATION)
    #if defined(DISPLAY_FLIP_HORIZONTAL) || defined(DISPLAY_FLIP_VERTICAL) || defined(DISPLAY_ROTATE_180)
    #error Flip and Rotate not available with TYPE_5_IMPLEMENTATION
    #endif
#endif



// Display size specific macros
#ifdef DISPLAY_SIZE_A
    #include "size_macros\macros_size_a.h"
#elif defined (DISPLAY_SIZE_B)
    #include "size_macros\macros_size_b.h"
#elif defined (DISPLAY_SIZE_C)
    #include "size_macros\macros_size_c.h"
#elif defined (DISPLAY_SIZE_D)
    #include "size_macros\macros_size_d.h"
#elif defined (DISPLAY_SIZE_E)
    #include "size_macros\macros_size_e.h"
#else
    #error Display size not defined
#endif

// Implementation specfic macros & functions
#ifdef TYPE_1_IMPLEMENTATION
    #include "type_macros\macros_type_1.h"
    #include "type_functions\functions_type_1.h"
#elif defined (TYPE_2_IMPLEMENTATION)
    #include "type_macros\macros_type_2.h"
    #include "type_functions\functions_type_2.h"
#elif defined (TYPE_3_IMPLEMENTATION)
    #include "type_macros\macros_type_3.h"
    #include "type_functions\functions_type_3.h"
#elif defined (TYPE_4_IMPLEMENTATION)
    #include "type_macros\macros_type_4.h"
    #include "type_functions\functions_type_4.h"
#elif defined (TYPE_5_IMPLEMENTATION)
    #include "type_macros\macros_type_5.h"
    #include "type_functions\functions_type_5.h"
#else
    #error Implementation not defined
#endif

// Common Functions
#include "common\functions\common_functions.h"

#endif  // #ifndef _DRIVER_HW_TYPE_H_
