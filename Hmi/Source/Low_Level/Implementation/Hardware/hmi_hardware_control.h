#ifndef _HMI_HARDWARE_CONTROL_H_
#define _HMI_HARDWARE_CONTROL_H_

#ifdef _HMI_HW_DESCRIPTION_1
    #include "hmi_hw_description_1.h"

#elif defined (_HMI_HW_DESCRIPTION_2)
    #include "hmi_hw_description_2.h"

#elif defined (_HMI_HW_DESCRIPTION_4)
    #include "hmi_hw_description_4.h"

#elif defined (_HMI_HW_DESCRIPTION_5)
    #include "hmi_hw_description_5.h"

#elif defined (_HMI_HW_DESCRIPTION_6)
    #include "hmi_hw_description_6.h"

#elif defined (_HMI_HW_DESCRIPTION_7)
    #include "hmi_hw_description_7.h"

#elif defined (_HMI_HW_DESCRIPTION_8)
    #include "hmi_hw_description_8.h"

#elif defined (_HMI_HW_DESCRIPTION_9)
    #include "hmi_hw_description_9.h"

#elif defined (_HMI_HW_DESCRIPTION_10)
    #include "hmi_hw_description_10.h"

#elif defined (_HMI_HW_DESCRIPTION_11)
    #include "hmi_hw_description_11.h"

#elif defined (_HMI_HW_DESCRIPTION_12)
    #include "hmi_hw_description_12.h"

#elif defined (_HMI_HW_DESCRIPTION_13)
    #include "hmi_hw_description_13.h"

#elif defined (_HMI_HW_DESCRIPTION_14)
    #include "hmi_hw_description_14.h"

#elif defined (_HMI_HW_DESCRIPTION_15)
    #include "hmi_hw_description_15.h"

#elif defined (_HMI_HW_DESCRIPTION_16)
    #include "hmi_hw_description_16.h"

#elif defined (_HMI_HW_DESCRIPTION_17)
    #include "hmi_hw_description_17.h"

#elif defined (_HMI_HW_DESCRIPTION_PROG_NV)
    #include "hmi_hw_description_prog_nv.h"

#elif defined (_MSP430)
    #error hardware description must be defined in project settings
#else
    // High Level HMI that hasn't defined the hmi hardware description directive in project settings
    // Provide default settings so as to allow error free compilation
    #define _SWITCHES_STANDARD
    #define _ENCLOSURE_STANDARD
#endif

#endif  // #ifndef _HMI_HARDWARE_CONTROL_H_
