#ifndef _IMPLEMENTATIONDEFINITIONS
#define _IMPLEMENTATIONDEFINITIONS

#define HMIGEN_RELEASE_VERSION 0
#define HMIGEN_MAJOR_VERSION   36
#define HMIGEN_MINOR_VERSION   2
#define HMIGEN_CONFIG_VERSION  02

//#define M16C_28
//#define M16C_62
//#define M32C

#ifdef _CM10_BUILD
#include "implementation_cm10.h"
#elif defined (_CM15_BUILD)
#include "implementation_cm15.h"
#elif defined (_CM30_BUILD)
#include "implementation_cm30.h"
#elif defined (_CM50_BUILD)
#include "implementation_cm50.h"
#else
#define TYPE_1_IMPLEMENTATION
//#define TYPE_2_IMPLEMENTATION
//#define TYPE_3_IMPLEMENTATION
//#define TYPE_4_IMPLEMENTATION
//#define TYPE_5_IMPLEMENTATION
//#define TYPE_6_IMPLEMENTATION

//#define DISPLAY_SIZE_A
#define DISPLAY_SIZE_B
//#define DISPLAY_SIZE_C
//#define DISPLAY_SIZE_D
//#define DISPLAY_SIZE_E
//#define DISPLAY_SIZE_F
//#define DISPLAY_SIZE_G

//#define TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE
//#define TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE
#define TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT

//#define LOW_LEVEL_USES_EXTENDED_CHARACTER_SET
//#define TYPE_2_IMPLEMENTATION_PARALLEL_KEYS
#endif

#if !defined(TYPE_1_IMPLEMENTATION) && !defined(TYPE_2_IMPLEMENTATION)&& !defined(TYPE_3_IMPLEMENTATION) && !defined(TYPE_4_IMPLEMENTATION) && !defined(TYPE_5_IMPLEMENTATION) && !defined(TYPE_6_IMPLEMENTATION)
#error interface type not defined
#endif

#if defined (TYPE_1_IMPLEMENTATION) && (defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_6_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if defined (TYPE_2_IMPLEMENTATION) && (defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_1_IMPLEMENTATION)|| defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_6_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if defined (TYPE_3_IMPLEMENTATION) && (defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)|| defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_6_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if defined (TYPE_4_IMPLEMENTATION) && (defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)|| defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_6_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if defined (TYPE_5_IMPLEMENTATION) && (defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)|| defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_6_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if defined (TYPE_6_IMPLEMENTATION) && (defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)|| defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION))
#error multiple implementations defined
#endif

#if !defined(DISPLAY_SIZE_A) && !defined(DISPLAY_SIZE_B)&& !defined(DISPLAY_SIZE_C) && !defined(DISPLAY_SIZE_D) && !defined(DISPLAY_SIZE_E) && !defined(DISPLAY_SIZE_F) && !defined(DISPLAY_SIZE_G)
#error display size not defined
#endif

#if defined (DISPLAY_SIZE_A) && (defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_F) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_B) && (defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_F) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_C) && (defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_F) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_D) && (defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_F) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_E) && (defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_F) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_F) && (defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_G))
#error multiple display sizes defined
#endif

#if defined (DISPLAY_SIZE_G) && (defined(DISPLAY_SIZE_A) || defined(DISPLAY_SIZE_B) || defined(DISPLAY_SIZE_C) || defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E) || defined(DISPLAY_SIZE_F))
#error multiple display sizes defined
#endif

#if (defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)) && !defined(DISPLAY_SIZE_D) && !defined(DISPLAY_SIZE_E)
#error display & interface are incompatible
#endif

#if !defined(TYPE_4_IMPLEMENTATION) && !defined(TYPE_5_IMPLEMENTATION) && (defined(DISPLAY_SIZE_D) || defined(DISPLAY_SIZE_E))
#error display & interface are incompatible
#endif
#if defined(DISPLAY_SIZE_F) && !defined(TYPE_1_IMPLEMENTATION)
#error size F display only available with Type 1 implementation
#endif
#if defined(DISPLAY_SIZE_G) && !defined(TYPE_6_IMPLEMENTATION)
#error size G display only available with Type 6 implementation
#endif

#if !defined(TYPE_1_IMPLEMENTATION) && (defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE) || defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE) || defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT))
#error special variant of type 1 interface defined without type 1 interface
#endif
#if (defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE) && defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE)) || (defined(TYPE_1_IMPLEMENTATION_RESET_AND_PRESENCE) && defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT)) || (defined(TYPE_1_IMPLEMENTATION_NO_POWER_CYCLE) && defined(TYPE_1_IMPLEMENTATION_WITH_2WIRE_SERVICEPORT))
#error only one special variant of type 1 interface can be defined
#endif


#endif
