#pragma diag_suppress=Go003

//#include "hmi_framework_definition.h"
#include "IncludeList_Implementation.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../embos/interface/rtos.h"
#else
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#endif

#include "System/Interface/system.h"
#include "frameTableDefinitions.h"
#include "frameTable.h"

//+@ HMIGen IncludeList_SpecialOperation.c @+

// If the files generated by HMIGen are not saved to the ....HMI\Source\  folder then
// the relative file path to the include files below must be added to the lines below.

#include "specialOperation.h"
#include "specialOperationFunctions.h"

//-@ HMIGen IncludeList_SpecialOperation.c @-

