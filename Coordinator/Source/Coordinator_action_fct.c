//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
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
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "coordinator_action_fct.h"


#include "FrontEndInterface/Interface/FrontEndInterface.h"
//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CONFIG_ALG_PARAS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER CONFIG_ALG_PARAS> \endif
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
//@SubGen start@<METHOD CONFIG_ALG_PARAS>
TUSIGN16 config_alg_paras_CDT(void)
//@SubGen end@<METHOD CONFIG_ALG_PARAS>
{
    TUSIGN16 result = (TUSIGN16)OK;

    result = frontEndInterface.PutCommand(CDT_WRITE_OUTPUT_FILTER_PARA, CDT_OUTPUT_FILTER_ALG_PARA_LENGTH, comWrite_cdt[CDT_WRITE_OUTPUT_FILTER_PARA_INDEX], NULL);

    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER READ_ALG_BLOCK> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER READ_ALG_BLOCK> \endif
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
//@SubGen start@<METHOD READ_ALG_BLOCK>
TUSIGN16 read_alg_block_CDT(void)
//@SubGen end@<METHOD READ_ALG_BLOCK>
{
    TUSIGN16 result = (TUSIGN16)OK;

    result = frontEndInterface.PutCommand(CDT_READ_OUTPUT_FILTER_ALG,  CDT_OUTPUT_FILTER_ALG_LENGTH,     comRead_cdt[CDT_READ_OUTPUT_FILTER_ALG_INDEX], NULL);
    result |= frontEndInterface.PutCommand(CDT_READ_OUTPUT_FILTER_PARA,CDT_OUTPUT_FILTER_ALG_PARA_LENGTH,comRead_cdt[CDT_READ_OUTPUT_FILTER_PARA_INDEX],NULL);

    return result;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETFE_RUNHOUR> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETFE_RUNHOUR> \endif
 \author  Lawrence Shi
 \date  2013-04-15
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETFE_RUNHOUR>
TUSIGN16 resetFE_Runhour_CDT(void)
//@SubGen end@<METHOD RESETFE_RUNHOUR>
{
	TUSIGN16 status = (TUSIGN16)OK; 
	TUSIGN32 frontendOperationHour = 0;
	status = coordinator_Put((TUSIGN16)CDT_IDX_frontendOperationHour,(TINT16)WHOLE_OBJECT,(void FAST*)&frontendOperationHour);
	return status;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SAVECUSTOMERDEF> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SAVECUSTOMERDEF> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SAVECUSTOMERDEF>
TUSIGN16 saveCustomerDef_CDT(void)
//@SubGen end@<METHOD SAVECUSTOMERDEF>
{
	TUSIGN16 result;
    //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_WRITE_SAVE_DSP_DEFAULT,CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_SAVE_DSP_DEFAULT_INDEX],NULL);
	return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER SAVEFACTORYDEF> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER SAVEFACTORYDEF> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SAVEFACTORYDEF>
TUSIGN16 saveFactoryDef_CDT(void)
//@SubGen end@<METHOD SAVEFACTORYDEF>
{
	TUSIGN16 result;
     //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_WRITE_SAVE_DSP_FACTORY,(TUSIGN8)CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_SAVE_DSP_FACTORY_INDEX],NULL);
	return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETCUSTOMERDEF> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETCUSTOMERDEF> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETCUSTOMERDEF>
TUSIGN16 resetCustomerDef_CDT(void)
//@SubGen end@<METHOD RESETCUSTOMERDEF>
{
	TUSIGN16 result;
    //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_WRITE_DSP_DEFAULT,CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_DSP_DEFAULT_INDEX],NULL);
	return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER RESETFACTORYDEF> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER RESETFACTORYDEF> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD RESETFACTORYDEF>
TUSIGN16 resetFactoryDef_CDT(void)
//@SubGen end@<METHOD RESETFACTORYDEF>
{
	TUSIGN16 result;
     //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_WRITE_DSP_FACTORY,CDT_DEFAULT_LENGTH,comWrite_cdt[CDT_WRITE_DSP_FACTORY_INDEX],NULL);
	return result;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER READPARAMETERS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER READPARAMETERS> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD READPARAMETERS>
TUSIGN16 readParameters_CDT(void)
//@SubGen end@<METHOD READPARAMETERS>
{
	TUSIGN16 result;
    //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_READ_DSP_PARAM_CMD,CDT_DSP_PARAM_CMD_LENGTH,comRead_cdt[CDT_READ_DSP_PARAM_CMD_INDEX],NULL);
	return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER WRITEPARAMETERS> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER WRITEPARAMETERS> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 \test-date: 2013-04-25
 \n by: Lawrence Shi
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD WRITEPARAMETERS>
TUSIGN16 writeParameters_CDT(void)
//@SubGen end@<METHOD WRITEPARAMETERS>
{
	TUSIGN16 result;
     //lint -e{732} 2013-04-16 Lawrence Loss of sign is Ok
	result = frontEndInterface.PutCommand(CDT_WRITE_DSP_PARAM_CMD,CDT_DSP_PARAM_CMD_LENGTH,comWrite_cdt[CDT_WRITE_DSP_PARAM_CMD_INDEX],NULL);
	return result;
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