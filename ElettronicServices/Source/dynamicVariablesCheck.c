//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Framework
 Module         COMMON
 Description    implementation of functions for common problems
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Source/ElettronicServices_types.h"
#include "ElettronicServices/Source/ElettronicServices_attributes.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"

#define DISABLE_CONTEXTSWITCHING SuspendAll_RTOS_TASK()
#define ENABLE_CONTEXTSWITCHING  ResumeAll_RTOS_TASK()


#ifdef HART_COMMUNICATION
extern const T_DYNAMIC_CHECK_TABLE currDynamicVariablesTable[];
#endif

extern const T_DYNAMIC_CHECK_TABLE FEDynamicVariablesTable[];
extern const T_DYNAMIC_CHECK_TABLE MVMDynamicVariablesTable[];
extern const T_DYNAMIC_CHECK_TABLE esDynamicVariablesTable[];
extern const T_DYNAMIC_CHECK_TABLE svmDynamicVariablesTable[];
extern const T_DYNAMIC_CHECK_TABLE cdtDynamicVariablesTable[];
extern const T_DYNAMIC_CHECK_TABLE mapperDynamicVariablesTable[];

#if defined(HART_COMMUNICATION) && defined(MV)
#define TABLES_NUM  9
#elif defined(HART_COMMUNICATION)
#define TABLES_NUM  7
#elif defined(MV)
#define TABLES_NUM  6
#else
#define TABLES_NUM  7
#endif

typedef struct _T_DYNAMIC_CHECK
{
    const T_DYNAMIC_CHECK_TABLE *table[TABLES_NUM];
    const TUSIGN8               tableNumRows[TABLES_NUM];
    TUSIGN8                     subCounter;
    TUSIGN8                     subCounterBackup;
    TUSIGN8                     idxCounter;
    TUSIGN8                     idxCounterBackup;
} T_DYNAMIC_CHECK;

//lint -e{785} hao wu , this is Ok
T_DYNAMIC_CHECK dynamicVariables = 
	{
		 {
#ifdef HART_COMMUNICATION
			currDynamicVariablesTable,
#endif
                       cdtDynamicVariablesTable, 
                       FEDynamicVariablesTable,
                       svmDynamicVariablesTable,
                       MVMDynamicVariablesTable,
                       esDynamicVariablesTable,
                       mapperDynamicVariablesTable,
		},
		{
#ifdef HART_COMMUNICATION
			8,
#endif
                        7,
                        2,
                        3,
                        18,
                        4,
                        4,
		},
		0,
		0,
		0,
		0,
	};

//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER CheckDynamicVariables> \endif
 \brief  
 \if @SubGen end@ <METHODHEADER CheckDynamicVariables> \endif
 \author				Giovanni Invernizzi
 \date				
 \param				-
 \return				-  
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

void CheckDynamicVariables_ElectronicServices(void)
{
	if(elettronicServicesReplaceStaticFrequentNoDefault.dynamicVariablesCheckEnable)
	{	
		const T_DYNAMIC_CHECK_TABLE *tableEntry = dynamicVariables.table[dynamicVariables.subCounter];

		// Check internal data
		if ((dynamicVariables.subCounter == 0) && (dynamicVariables.idxCounter == 0))
		{
		    if (memcmp(&dynamicVariables.subCounter, &dynamicVariables.subCounterBackup, sizeof(TUSIGN8)) != 0)
		    {   //lint -e{506} Ashwin Herur R 2009-07-13 Ok
		        //lint -e{774} Ashwin Herur R 2009-07-13 Ok
		        VIP_ASSERT(0);
		    }

		    if (memcmp(&dynamicVariables.idxCounter, &dynamicVariables.idxCounterBackup, sizeof(TUSIGN8)) != 0)
		    {   //lint -e{506} Ashwin Herur R 2009-07-13 Ok
		        //lint -e{774} Ashwin Herur R 2009-07-13 Ok
		        VIP_ASSERT(0);
		    }
		}

		DISABLE_CONTEXTSWITCHING;
		// Compare the data with the backup value
        //lint -e{774} hao wu,  2013-01-15 Ok
		if (memcmp(tableEntry[dynamicVariables.idxCounter].mainValue,
		           tableEntry[dynamicVariables.idxCounter].backupValue,
		           tableEntry[dynamicVariables.idxCounter].size) != 0)
		{        
		   elettronicServices.ExitSRV();
           //lint -e{506} hao wu,  2013-01-15 Ok 
		   VIP_ASSERT(0);            
		}

        
		ENABLE_CONTEXTSWITCHING;

		// Increment counters
		dynamicVariables.idxCounter++;
		if (dynamicVariables.idxCounter >= dynamicVariables.tableNumRows[dynamicVariables.subCounter])
		{
		    dynamicVariables.idxCounter = 0;
		    dynamicVariables.subCounter++;

		    if (dynamicVariables.subCounter >= TABLES_NUM)
		    {
		        dynamicVariables.subCounter = 0;
		    }
		}

		// Backup dynamic data
		dynamicVariables.idxCounterBackup = dynamicVariables.idxCounter;
		dynamicVariables.subCounterBackup = dynamicVariables.subCounter;
	}
}