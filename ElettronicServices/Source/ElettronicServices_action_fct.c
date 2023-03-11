//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Implementation of action methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"



#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "ElettronicServices/Source/ElettronicServices_execute.h"
#include "ElettronicServices/Source/ElettronicServices_overload.h"
#include "ElettronicServices/Source/ElettronicServices_action_fct.h"

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SAVEALLASDEFAULT> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SAVEALLASDEFAULT> \endif
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
//@SubGen start@<METHOD SAVEALLASDEFAULT>
TUSIGN16 saveAllAsDefault_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD SAVEALLASDEFAULT>
{
     TUSIGN16 result = SaveAsDefault_ElectronicServices((TINT16)WHOLE_OBJECT);
     return  result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETALLTODEFAULT> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETALLTODEFAULT> \endif
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
//@SubGen start@<METHOD RESETALLTODEFAULT>
TUSIGN16 resetAllToDefault_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD RESETALLTODEFAULT>
{
     TUSIGN16 result = (TUSIGN16)OK;
     TINT16 tempI16 = (TINT16)WHOLE_OBJECT;

                //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted
     result = Put_ELETTRONIC_SERVICES(ELETTRONIC_SERVICES_ME ,(TUSIGN16)ELETTRONICSERVICES_IDX_resetToDefault, (TINT16)WHOLE_OBJECT, &tempI16);

	
     return  result;
}






//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER FORMATFENV> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER FORMATFENV> \endif
 \author Spring Zhou	
 \date 2013-06-03
 \param void 
 \return void -state of put queue
 \warning
 \test
 \test-date: 2013-06-03
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD FORMATFENV>
TUSIGN16 formatFENV_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD FORMATFENV>
{
	TINT16 result = (TINT16)OK;
	result = frontEndInterface.PutCommand(ELETTRONICSERVICES_WRITE_FORMAT_FENV,ELETTRONICSERVICES_DEFUALT_LENGTH,comWrite_es[ELETTRONICSERVICES_WRITE_FORMAT_FENV_INDEX],NULL);
	return (TUSIGN16)result;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETCB_RUNHOUR> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETCB_RUNHOUR> \endif
 \author Lawrence Shi
 \date 2013-01-16
 \param void
 \return TUSIGN16 - data put result
 \warning
 \test
 \test-date: 2013-06-03
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETCB_RUNHOUR>
TUSIGN16 resetCB_Runhour_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD RESETCB_RUNHOUR>
{
	TUSIGN16 result = (TUSIGN16)OK;  
	TUSIGN32 totalWorkingTimeHour = 0;
	result = (TUSIGN16)coordinator_Put((TINT16)CDT_IDX_totalWorkingTimeHour, (TINT16)WHOLE_OBJECT, &totalWorkingTimeHour);
	return result;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER WRITEAUTOZERO> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER WRITEAUTOZERO> \endif
 \author Lawrence Shi
 \date 2013-01-16
 \param void
 \return TUSIGN16 - data and command process result
 \warning
 \test
 \test-date: 2013-06-03
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD WRITEAUTOZERO>
TUSIGN16 writeAutoZero_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD WRITEAUTOZERO>
{

   TINT16 result = (TINT16)OK;

   TUSIGN8 autozeroStatus = ELETTRONICSERVICES_STATUS_UNCOMPLETED;
   
   esAutoZeroCount = 0;
   
   result = (TINT16)elettronicServices_Put((TINT16)ELETTRONICSERVICES_IDX_autozeroStatus,(TINT16)WHOLE_OBJECT,&autozeroStatus);
   
   result |= frontEndInterface.PutCommand(ELETTRONICSERVICES_WRITE_AUTO_ZERO,ELETTRONICSERVICES_DEFUALT_LENGTH,comWrite_es[ELETTRONICSERVICES_WRITE_AUTO_ZERO_INDEX],NULL);
   return (TUSIGN16)result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER WRITESELFDIAGNOSIS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER WRITESELFDIAGNOSIS> \endif
 \author Lawrence Shi
 \date 2013-01-16
 \param void
 \return TUSIGN16 - data and command process result
 \warning
 \test
 \test-date: 2013-06-03
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 100% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD WRITESELFDIAGNOSIS>
TUSIGN16 writeSelfDiagnosis_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD WRITESELFDIAGNOSIS>
{
   TINT16 result = (TINT16)OK;
   result = frontEndInterface.PutCommand(ELETTRONICSERVICES_WRITE_SELF_DIAGNOSIS,ELETTRONICSERVICES_DEFUALT_LENGTH,comWrite_es[ELETTRONICSERVICES_WRITE_SELF_DIAGNOSIS_INDEX],NULL);
   return (TUSIGN16)result;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETOUTPUTTRIMMING> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETOUTPUTTRIMMING> \endif
 \author				Roberto Colombo
 \date				
 \param				-
 \return				TUSIGN16 result
 \warning
 \test
 test-date:			2009-07-14
 \n by:				Ganaphati R
 \n environment:		IAR-CSPY
 \n intention:			Code Coverage 100%
 \n result module test:	OK = 0
 \n result Lint Level 3:	OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETOUTPUTTRIMMING>
TUSIGN16 resetOutputTrimming_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD RESETOUTPUTTRIMMING>
{
    return OK;
}

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER DEVICERESET> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER DEVICERESET> \endif
 \author				Roberto Colombo
 \date				
 \param				-
 \return				TUSIGN16 result
 \warning
 \test
 test-date:			2009-07-14
 \n by:				Ganaphati R
 \n environment:		IAR-CSPY
 \n intention:			Code Coverage 100%
 \n result module test:	OK
 \n result Lint Level 3:	OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD DEVICERESET>
TUSIGN16 deviceReset_ELETTRONICSERVICES(void)
//@SubGen end@<METHOD DEVICERESET>
{
	esResetDevice = eTRUE;
        //lint -e{641} Ashwin Herur R 2009-07-14 Converting enum to int accepted		
	return OK;
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