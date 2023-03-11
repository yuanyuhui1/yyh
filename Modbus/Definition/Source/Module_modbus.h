//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus

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

#ifdef __MODULE_MODBUS_H__
  #error MODULE_MODBUS.h included more than once
#endif

#define __MODULE_MODBUS_H__

enum _T_MODULE_MODBUS_IDX
{
    //@SubGen start@    <ENUMERATIONS>
    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     <pre>
     datatype          : USIGN8
     typequalifier     : SIMPLE
     storage qualifier : RAM
     direction         : INOUT
     </pre>
    */
    MODULE_MODBUS_IDX_address,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     <pre>
     datatype          : TABENUM8
     typequalifier     : SIMPLE
     storage qualifier : RAM
     direction         : INOUT
     </pre>
    */
    MODULE_MODBUS_IDX_baudRate,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     <pre>
     datatype          : TABENUM8
     typequalifier     : SIMPLE
     storage qualifier : RAM
     direction         : INOUT
     </pre>
    */
    MODULE_MODBUS_IDX_parity,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     <pre>
     datatype          : TABENUM8
     typequalifier     : SIMPLE
     storage qualifier : RAM
     direction         : INOUT
     </pre>
    */
    MODULE_MODBUS_IDX_stopBit,

    //--------------------------------------------------------------------------------------------------
    /*!
     \brief     
     <pre>
     datatype          : USIGN8
     typequalifier     : SIMPLE
     storage qualifier : RAM
     direction         : INOUT
     </pre>
    */
    MODULE_MODBUS_IDX_responseDelayTime
    //@SubGen end@    <ENUMERATIONS>
};

#define MODULE_MODBUS_SUB_IDX   3


