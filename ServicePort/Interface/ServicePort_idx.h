//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ServicePort

 Module

 Description    This header file defines the complete list of public data-objects.
                Other subsystems need to use these macros instead of magic-numbers  
                when accessing the public data-objects.

                The index of private data-object should not be available through
                the TUNIT interface.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifdef __SERVICE_PORT_IDX_H__
  #error ServicePort_idx.h included more than once
#endif

#define __SERVICE_PORT_IDX_H__

enum _T_SERVICE_PORT_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Activation Status
     \datatype          : SIMPLE_CHAR
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : OUTPUT
    */
    SERVICEPORT_IDX_activationStatus,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     Service port baud rate.
     \datatype          : SIMPLE_U16
     \typequalifier     : SIMPLE
     \storage qualifier : RAM
     \direction         : INPUT
    */
    SERVICEPORT_IDX_baudRate
    //@SubGen end@    <ENUMERATIONS>
};




