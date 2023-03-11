//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         2 wire Top Works
 Module         Subsystem ServicePort
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "ServicePort/Interface/servicePort.h"
#include "ServicePort/Layer1/serviceport_dio.h"


#include "servicePort_types.h"
#include "servicePort_attributes.h"
#include "servicePort_execute.h"
#include "ServicePort/Interface/ServicePort_idx.h"
#include "ServicePort/Layer2/serviceport_layer2.h"
#include "ServicePort/Layer7/servicePort_layer7.h"




//@SubGen put@ <METHOD>

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SERVPORTREDETECTHMISRV> \endif
 \brief  redetect HMI
 \param  isSmartHMI is SmartHMI or HMI
 \if @SubGen end@ <METHODHEADER SERVPORTREDETECTHMISRV> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVPORTREDETECTHMISRV>
void ServPortRedetectHMISRV_SERVICEPORT(TBOOL isSmartHMI)
//@SubGen end@<METHOD SERVPORTREDETECTHMISRV>
{
  hmiPresencePin.bounceCount = SP_HMI_PRESENCE_BOUNCECOUNT_REDETECT;
  if(isSmartHMI)
  {
    if(!isSmartHMIConnected && servicePortDynamic.activationStatus != SERVICEPORT_SHMI_STATUS)
    {
      isSmartHMIConnected = eTRUE;
    }else
    {
      isSmartHMIConnected = eFALSE;
    }
  }else{
    if(isSmartHMIConnected && servicePortDynamic.activationStatus != SERVICEPORT_HMI_STATUS)
    {
      isSmartHMIConnected = eFALSE;
    }
  }
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SERVPORTCOMMANDINTERPRETEREXE> \endif
 \brief  Interprets the command.
 \if @SubGen end@ <METHODHEADER SERVPORTCOMMANDINTERPRETEREXE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVPORTCOMMANDINTERPRETEREXE>
void ServPortCommandInterpreterEXE_SERVICEPORT(void)
//@SubGen end@<METHOD SERVPORTCOMMANDINTERPRETEREXE>
{
  	ServPortCmdInterpreter_LAYER7();
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SERVPORTUPDATEACTIVATIONSTATUSSRV> \endif
 \brief  presence pin status.
 \if @SubGen end@ <METHODHEADER SERVPORTUPDATEACTIVATIONSTATUSSRV> \endif
 \author Ganapathi R
 \date 2008-01-21
 \param void
 \return void
 \warning
 \test NA
 test-date:
 \n by:
 \n environment:
 \n intention:			
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVPORTUPDATEACTIVATIONSTATUSSRV>
void ServPortUpdateActivationStatusSRV_SERVICEPORT(void)
//@SubGen end@<METHOD SERVPORTUPDATEACTIVATIONSTATUSSRV>
{
	updateHMIpresencePinStatus_LAYER7();
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SERVPORTGETACTIVATIONSTATUSSRV> \endif
 \brief  Gets the activation status of service port.
 \if @SubGen end@ <METHODHEADER SERVPORTGETACTIVATIONSTATUSSRV> \endif
 \author
 \author Ganapathi R
 \date 2008-01-21
 \param void
 \return void
 \warning
 \test NA
 test-date:
 \n by:
 \n environment:
 \n intention:			
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SERVPORTGETACTIVATIONSTATUSSRV>
TUSIGN8 ServPortGetActivationStatusSRV_SERVICEPORT(void)
//@SubGen end@<METHOD SERVPORTGETACTIVATIONSTATUSSRV>
{
  	return (servicePortDynamic.activationStatus);
}



/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
 \if @SubGen start@ <METHODHEADER TEMPLATE> \endif
 \if @SubGen end@ <METHODHEADER TEMPLATE> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/
