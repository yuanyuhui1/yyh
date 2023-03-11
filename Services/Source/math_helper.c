//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2019
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         COMMON
 Description    Functions for math
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include <stddef.h>
#include "Services/Interface/math_helper.h"

//-------------------------------------------------------------------------
/*! 
\brief Returns smaller int value from given two
\author  
\date  
\param lv First int value
\param rv Second int value
\return int Smaller int value
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
int min_int(int lv, int rv) {
    return lv < rv ? lv : rv;
}

//-------------------------------------------------------------------------
/*! 
\brief Returns bigger int value from given two
\author  
\date  
\param lv First int value
\param rv Second int value
\return int Bigger int value
\warning  
\test  
\n test-date: 
\n by: 
\n environment:  
\n intention:  
\n result module test:  
\n result Lint check:  
\bug   
*/ 
//--------------------------------------------------------------------------
int max_int(int lv, int rv) {
    return lv > rv ? lv : rv;
}

size_t min_size_t(size_t lv, size_t rv) {
    return lv < rv ? lv : rv;
}

size_t max_size_t(size_t lv, size_t rv) {
    return lv > rv ? lv : rv;
}

TINT32 max_tint32(TINT32 lv, TINT32 rv) {
    return lv > rv ? lv : rv;
}
TINT32 min_tint32(TINT32 lv, TINT32 rv) {
    return lv < rv ? lv : rv;
}

TUSIGN32 max_tusign32(TUSIGN32 lv, TUSIGN32 rv) {
    return lv > rv ? lv : rv;
}
TUSIGN32 min_tusign32(TUSIGN32 lv, TUSIGN32 rv) {
    return lv < rv ? lv : rv;
}

TFLOAT max_tfloat(TFLOAT lv, TFLOAT rv) {
    return lv > rv ? lv : rv;
}

TFLOAT min_tfloat(TFLOAT lv, TFLOAT rv) {
    return lv < rv ? lv : rv;
}

TDOUBLE max_tdouble(TDOUBLE lv, TDOUBLE rv) {
    return lv > rv ? lv : rv;
}
TDOUBLE min_tdouble(TDOUBLE lv, TDOUBLE rv) {
    return lv < rv ? lv : rv;
}
