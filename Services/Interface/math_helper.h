#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include <stddef.h>

int min_int(int lv, int rv);
int max_int(int lv, int rv);

size_t min_size_t(size_t lv, size_t rv);
size_t max_size_t(size_t lv, size_t rv);

TINT32 max_tint32(TINT32 lv, TINT32 rv);
TINT32 min_tint32(TINT32 lv, TINT32 rv);

TUSIGN32 max_tusign32(TUSIGN32 lv, TUSIGN32 rv);
TUSIGN32 min_tusign32(TUSIGN32 lv, TUSIGN32 rv);

TFLOAT max_tfloat(TFLOAT lv, TFLOAT rv);
TFLOAT min_tfloat(TFLOAT lv, TFLOAT rv);

TDOUBLE max_tdouble(TDOUBLE lv, TDOUBLE rv);
TDOUBLE min_tdouble(TDOUBLE lv, TDOUBLE rv);

#endif