// hmi_framework_definition.h
#ifndef HMI_FRAMEWORK_DEFINITION_H
#define HMI_FRAMEWORK_DEFINITION_H

// The file paths for certain header files required within HMI
// are dependant upon the version of framework being used.
// It is not possible for the HMI to read the framework version
// directly from the t_unit.h file therefore it is necessary to
// define HOST_FRAMEWORK_VERSION with the same value as T_UNIT_VERSION

//+@ HMIGen framework version definition @+
//#error HOST_FRAMEWORK_VERSION not defined
#define HOST_FRAMEWORK_VERSION  0x020300
//-@ HMIGen framework version definition @-

#endif
