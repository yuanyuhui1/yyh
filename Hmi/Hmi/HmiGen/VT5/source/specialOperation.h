#ifndef _SPECIALOPERATION_H
#define _SPECIALOPERATION_H

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included
#endif

#ifndef _COMPILETIMEDEFINITIONS
  #error compileTimeDefinitions.h must be included
#endif

#define IDF_SPECIAL_NONE 0
#define IDF_SPECIAL_INH_ENTER_CONFIG 1
#define IDF_SPECIAL_INH_EXIT_CONFIG 2
#define IDF_SPECIAL_INH_RETURN_TO_OPERATOR_PAGE 3

#define LAST_IDF_SPECIAL 3

void SpecialOperationExecute(TUSIGN16 specialOpIdx);
#endif
