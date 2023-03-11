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
#include "ElettronicServices/Source/elettronicServices_cpu.h"

#if 0
/**********************************************************************************
#defines
***********************************************************************************/
/*Comment this out if you want to use the TestGPRs function rather than
the TestGPRsCoupling function.*/


/**********************************************************************************
External functions prototype
***********************************************************************************/
/*This must be implemented by the user of this CPU Test SW.
It will be called if an error is detected. It must not return.*/

/**********************************************************************************
Static functions prototype
***********************************************************************************/
#ifndef USE_TestGPRsCoupling /*Not required if using TestGPRsCoupling function*/
static void TestGPRbank(void);
#else
static void TestGPRCouplingBank(void);
#endif
static TUSIGN8 TestPC_CharInverse(TUSIGN8 ui8Char);
static TBOOL cpu_test_result = eTRUE;
static TBOOL asm_test_result = eTRUE;



//------------------------------------------------------------------------------------------------
/*
 brief		For the current bank the following registers will be tested
				including testing for coupling faults:
				R0, R1, R2, R3, A0 & A1.
				The Frame base register (FBR) will also be tested for
				Read/Write operation.
				(This is so that all registers in the register bank are covered)
 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
#ifdef USE_TestGPRsCoupling
static void TestGPRCouplingBank(void)
{
	/*NOTE: Throughout this function if an error is detected the 'CPU_Test_ErrorHandler'
	function shall be called.*/

	/*This function preserves register values. Save registers to the stack*/
	asm("PUSH.W R0");
	asm("PUSH.W R1");
	asm("PUSH.W R2");
	asm("PUSH.W R3");
	asm("PUSH.W A0");
	asm("PUSH.W A1");
	asm("PUSHC FB");

	/*****************************************************/
	/*** Start of tests where all are set to H'AAAA    ***/
	/*** and then for each register H'5555 is written. ***/
	/*****************************************************/

	/*Set all registers to h'AAAA*/
	asm("MOV.W #0AAAAH,	R0");
	asm("MOV.W #0AAAAH,	R1");
	asm("MOV.W #0AAAAH,	R2");
	asm("MOV.W #0AAAAH,	R3");
	asm("MOV.W #0AAAAH,	A0");
	asm("MOV.W #0AAAAH,	A1");

	/*Check all read back as h'AAAA*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling1");

	/*** Start R0 h'5555 Test ***/
	/*Set just R0 to h'5555*/
	asm("MOV.W #05555H,	R0");
	/* Check value written is read back*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling1");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling1");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling1");


	asm("JMP.S OK_GPRsCoupling1");
	asm("ERR_GPRsCoupling1:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling1:");
	/*Set R0 back to h'AAAA*/
	asm("MOV.W #0AAAAH,	R0");
	/*** End R0 h'5555 Test ***/

	/*** Start R1 h'5555 Test ***/
	/*Set just R1 to h'5555*/
	asm("MOV.W #05555H,	R1");
	/* Check value written is read back*/
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling2");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling2");

	/*Set R1 back to h'AAAA*/
	asm("MOV.W #0AAAAH,	R1");
	/*** End R1 h'5555 Test ***/

	/*** Start R2 h'5555 Test ***/
	/*Set just R2 to h'5555*/
	asm("MOV.W #05555H,	R2");
	/* Check value written is read back*/
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling2");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling2");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling2");

	asm("JMP.S OK_GPRsCoupling2");
	asm("ERR_GPRsCoupling2:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling2:");

	/*Set R2 back to h'AAAA*/
	asm("MOV.W #0AAAAH,	R2");
	/*** End R2 h'5555 Test ***/

	/*** Start R3 h'5555 Test ***/
	/*Set just R3 to h'5555*/
	asm("MOV.W #05555H,	R3");
	/* Check value written is read back*/
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling3");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling3");

	/*Set R3 back to h'AAAA*/
	asm("MOV.W #0AAAAH,	R3");
	/*** End R3 h'5555 Test ***/

	/*** Start A0 h'5555 Test ***/
	/*Set just A0 to h'5555*/
	asm("MOV.W #05555H,	A0");
	/* Check value written is read back*/
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling3");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling3");
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling3");

	asm("JMP.S OK_GPRsCoupling3");
	asm("ERR_GPRsCoupling3:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling3:");

	/*Set A0 back to h'AAAA*/
	asm("MOV.W #0AAAAH,	A0");
	/*** End A0 h'5555 Test ***/

	/*** Start A1 h'5555 Test ***/
	/*Set just A1 to h'5555*/
	asm("MOV.W #05555H,	A1");
	/* Check value written is read back*/
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling4");

	/*Check other registers haven't been changed and are therefore all h'AAAA.*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling4");
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling4");
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling4");
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling4");
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling4");
	/*** End A1 h'5555 Test ***/

	/*****************************************************/
	/*** Start of tests where all are set to H'5555    ***/
	/*** and then for each register H'AAAA is written. ***/
	/*****************************************************/
	asm("JMP.S OK_GPRsCoupling4");
	asm("ERR_GPRsCoupling4:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling4:");

	/*Set all registers to h'5555*/
	asm("MOV.W #05555H,	R0");
	asm("MOV.W #05555H,	R1");
	asm("MOV.W #05555H,	R2");
	asm("MOV.W #05555H,	R3");
	asm("MOV.W #05555H,	A0");
	asm("MOV.W #05555H,	A1");

	/*Check all read back as h'5555*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling5");

	/*** Start R0 h'AAAA Test ***/
	/*Set just R0 to h'AAAA*/
	asm("MOV.W #0AAAAH,	R0");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, R0");
	asm("JNE ERR_GPRsCoupling5");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling5");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling5");


	asm("JMP.S OK_GPRsCoupling5");
	asm("ERR_GPRsCoupling5:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling5:");

	/*Set R0 back to h'5555*/
	asm("MOV.W #05555H,	R0");
	/*** End R0 h'AAAA Test ***/

	/*** Start R1 h'AAAA Test ***/
	/*Set just R1 to h'AAAA*/
	asm("MOV.W #0AAAAH,	R1");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, R1");
	asm("JNE ERR_GPRsCoupling6");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling6");

	/*Set R1 back to h'5555*/
	asm("MOV.W #05555H,	R1");
	/*** End R1 h'AAAA Test ***/

	/*** Start R2 h'AAAA Test ***/
	/*Set just R2 to h'AAAA*/
	asm("MOV.W #0AAAAH,	R2");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, R2");
	asm("JNE ERR_GPRsCoupling6");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling6");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling6");


	asm("JMP.S OK_GPRsCoupling6");
	asm("ERR_GPRsCoupling6:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling6:");


	/*Set R2 back to h'5555*/
	asm("MOV.W #05555H,	R2");
	/*** End R2 h'AAAA Test ***/

	/*** Start R3 h'AAAA Test ***/
	/*Set just R3 to h'AAAA*/
	asm("MOV.W #0AAAAH,	R3");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, R3");
	asm("JNE ERR_GPRsCoupling7");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling7");

	/*Set R3 back to h'5555*/
	asm("MOV.W #05555H,	R3");
	/*** End R3 h'AAAA Test ***/

	/*** Start A0 h'AAAA Test ***/
	/*Set just A0 to h'AAAA*/
	asm("MOV.W #0AAAAH,	A0");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, A0");
	asm("JNE ERR_GPRsCoupling7");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling7");
	asm("CMP.W #05555H, A1");
	asm("JNE ERR_GPRsCoupling7");

	asm("JMP.S OK_GPRsCoupling7");
	asm("ERR_GPRsCoupling7:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling7:");

	/*Set A0 back to h'5555*/
	asm("MOV.W #05555H,	A0");
	/*** End A0 h'AAAA Test ***/

	/*** Start A1 h'AAAA Test ***/
	/*Set just A1 to h'AAAA*/
	asm("MOV.W #0AAAAH,	A1");
	/* Check value written is read back*/
	asm("CMP.W #0AAAAH, A1");
	asm("JNE ERR_GPRsCoupling");

	/*Check other registers haven't been changed and are therefore all h'5555.*/
	asm("CMP.W #05555H, R0");
	asm("JNE ERR_GPRsCoupling");
	asm("CMP.W #05555H, R1");
	asm("JNE ERR_GPRsCoupling");
	asm("CMP.W #05555H, R2");
	asm("JNE ERR_GPRsCoupling");
	asm("CMP.W #05555H, R3");
	asm("JNE ERR_GPRsCoupling");
	asm("CMP.W #05555H, A0");
	asm("JNE ERR_GPRsCoupling");
	/*** End A1 h'AAAA Test ***/


	/* Test FB Control register  */
	/* NOTE: MOV & CMP instruction can not be used with FB hence used STC & LDC.
	R0 is used as a utility register in the following test.
	This test will fail, if R0 is faulty. */

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH, R0");
	/* Write data to FB */
	asm("LDC 	R0, FB");
	/* Read contents of FB*/
	asm("STC 	FB,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, R0");
	/* If error then jump to error routine */
	asm("JNE ERR_GPRsCoupling");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H, R0");
	/* Write data to FB */
	asm("LDC 	R0,	FB");
	/* Read contents of FB */
	asm("STC 	FB,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H, R0");
	/* If error then jump to error routine */
	asm("JNE ERR_GPRsCoupling");


	/*No Errors - This function preserves register values. Restore registers from the stack*/
	asm("JMP.S OK_GPRsCoupling");

	/* Error Handling called if testing fails. */
	asm("ERR_GPRsCoupling:");
	asm("JMP _CPU_Test_ErrorHandler");
	asm("OK_GPRsCoupling:");

	/*This function preserves register values. Restore registers from the stack*/
	asm("POPC FB");
	asm("POP.W A1");
	asm("POP.W A0");
	asm("POP.W R3");
	asm("POP.W R2");
	asm("POP.W R1");
	asm("POP.W R0");

	/*Allow C Function to return from this sub-routine*/
}
#endif

//------------------------------------------------------------------------------------------------
/*
 brief		This will mainly call all the CPU,REG check function
 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
TBOOL API_TestCPU(void)
{
	TBOOL result;

	/* Test CPU core registers. All interrupts must be disabled before
	calling this routine. This takes 50 us. */
	asm("FCLR I");
	/* CALL API */
	CPU_TestAll();
	ASM_TestAll();
	asm("FSET I");

	result = cpu_test_result;
	result &= asm_test_result;
	cpu_test_result = eTRUE;   //reinitialize for next test.
	asm_test_result = eTRUE;
	return result;
}



//------------------------------------------------------------------------------------------------
/*
 brief		This will be called when there is something wrong with CPU_TEST
 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void _CPU_Test_ErrorHandler(void)
{
	/* Signal error to original caller. This is via a global variable since
	The CPU test functions are of type void. */
	cpu_test_result = eFALSE;
	ResetAction();
}




//------------------------------------------------------------------------------------------------
/*
 brief		This will be called when there is something wrong with ASM TEST
 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void _ASM_Test_ErrorHandler(void)
{
	/* Signal error to original caller. This is via a global variable since
	The CPU test functions are of type void. */
	asm_test_result = eFALSE;
	ResetAction();
}





//------------------------------------------------------------------------------------------------
/*
 brief		Carry outs all CPU registers test tests.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void CPU_TestAll(void)
{
	/*TestGPRs is not required if using TestGPRsCoupling*/
#ifdef USE_TestGPRsCoupling
	TestGPRsCoupling();
#else
	TestGPRs();
#endif

	TestIntRegs();
	TestStackRegs();
	TestPCReg();
	TestFlagReg();
}




//------------------------------------------------------------------------------------------------
/*
 brief		This is an extension to the 'TestGPRs' test to include testing for
				coupling faults between the registers.
				If using this function then the TestGPRs function doesn't
				need to be used as well.

				Registers tested are:
				R0, R1, R2, R3, A0, A1 and FB in Register Bank0 & Bank1.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
#ifdef USE_TestGPRsCoupling
void TestGPRsCoupling(void)
{
    asm("FCLR I");
	/* Store original contents of Flag reg. */
	asm("PUSHC FLG");

	/* Clear Flag.B to select Bank0 to test*/
	asm("FCLR B");
	/* Test Register Bank0 */
	TestGPRCouplingBank();
    
    asm("FSET I");
    
    asm("nop");
    asm("nop");
    asm("nop");

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    asm("nop");
    asm("nop");
    
    
    asm("FCLR I");

	/* Set Flag.B to select Bank1 to test*/
	asm("FSET B");
	/* Test Register Bank1 */
	TestGPRCouplingBank();

	/* Re-store original contents of Flag reg. */
	asm("POPC FLG");
    
    asm("FSET I");
}
#endif





//------------------------------------------------------------------------------------------------
/*
 brief		CPU General Purpose Registers Testing to test
				R0, R1, R2, R3, A0, A1 & FB in Register Bank0 & Bank1.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
#ifndef USE_TestGPRsCoupling /*Not required if using TestGPRsCoupling function*/
void TestGPRs(void)
{
	/* Store original contents of Flag reg. */
	asm("PUSHC FLG");

	/* Clear Flag.B to select Bank0 to test*/
	asm("FCLR B");
	/* Test Register Bank0 */
	TestGPRbank();

	/* Set Flag.B to select Bank1 to test*/
	asm("FSET B");
	/* Test Register Bank1 */
	TestGPRbank();

	/* Re-store original contents of Flag reg. */
	asm("POPC FLG");
}
#endif



//------------------------------------------------------------------------------------------------
/*
 brief			CPU General Purpose Registers to test
				R0, R1, R2, R3, A0, A1 & FB of active Register Bank.

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
#ifndef USE_TestGPRsCoupling /*Not required if using TestGPRsCoupling function*/
static void TestGPRbank(void)
{
	/* Test R0 register */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	R0");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	R0");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	R0");

	/* Test R1 register  */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	R1");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R1");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R1");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	R1");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	R1");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	R1");

	/* Test R2 register  */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	R2");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R2");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R2");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	R2");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	R2");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	R2");

	/* Test R3 register  */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	R3");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R3");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R3");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	R3");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	R3");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	R3");

	/* Test A0 register  */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	A0");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	A0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	A0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	A0");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	A0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	A0");

	/* Test A1 register  */
	/* Save original contents of the test Register in the stack */
	asm("PUSH.W 	A1");
	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	A1");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	A1");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Write test data 0x55555 */
	asm("MOV.W 	#05555H,	A1");
	/* Read test register and compare with data 0x5555 */
	asm("CMP.W	#05555H, 	A1");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");
	/* Restore original contents of the test Register in the stack */
	asm("POP.W 	A1");



	/* Test FB Control register  */
	/* NOTE: MOV & CMP instruction can not be used with FB hence used STC & LDC.
	R0 is used as a utility register in the following test.
	This test will fail, if R0 is faulty. */
	/* Save original contents of the utility register R0 in the stack */
	asm("PUSH.W 	R0");
	/* Store original contents of FB  */
	asm("PUSHC 	FB");

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Write data to FB */
	asm("LDC 	R0, FB");
	/* Read contents of FB*/
	asm("STC 	FB,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H,	R0");
	/* Write data to FB */
	asm("LDC 	R0,	FB");
	/* Read contents of FB */
	asm("STC 	FB,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_GPRs");

	/* Re-store original contents of FB */
	asm("POPC 	FB");

	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R0");

	/* Retun from subroutine after success. */
	asm("RTS");

	/* Error Handling called if testing fails. */
	asm("ERR_GPRs: 	JMP	_CPU_Test_ErrorHandler");
}
/**********************************************************************************
End of function TestGPRbank
***********************************************************************************/
#endif





//------------------------------------------------------------------------------------------------
/*
 brief			R0 is used as a utility register in the following test.
                This test will fail, if R0 is faulty.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void TestIntRegs(void)
{
	/* Store original contents of Flag reg. */
	asm("PUSHC FLG");

	/*Test INTBL 16-bit Low*/
	/* Store original contents of INTBL reg. */
	asm("PUSHC INTBL");

	/* NOTE: MOV & CMP instruction can not be used with Control Registers hence used STC & LDC.
	R0 is used as a utility register in the following test.
	This test will fail, if R0 is faulty. */
	/* Save original contents of the utility register R0 in the stack */
	asm("PUSH.W 	R0");

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Write data to INTBL */
	asm("LDC 	R0, INTBL");
	/* Read data from INTBL*/
	asm("STC 	INTBL,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_IntRegs");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H,	R0");
	/* Write data to INTBL */
	asm("LDC 	R0,	INTBL");
	/* Read data from INTBL*/
	asm("STC 	INTBL,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_IntRegs");

	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R0");

	/* Re-store original contents of INTBL */
	asm("POPC 	INTBL");


	/*Test INTBH 4-bit high */
	/* Store original contents of INTBH reg. */
	asm("PUSHC INTBH");
	/* Save original contents of the utility register R0 in the stack */
	asm("PUSH.W 	R0");

	/* Write test data 0x000A  */
	asm("MOV.W 	#0000AH,	R0");
	/* Write data to INTBH */
	asm("LDC 	R0, INTBH");
	/* Read data from INTBH*/
	asm("STC 	INTBH,	R0");
	/* Read test register and compare with data 0x000A  */
	asm("CMP.W	#0000AH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_IntRegs");

	/* Write test data 0x0005  */
	asm("MOV.W 	#00005H,	R0");
	/* Write data to INTBH */
	asm("LDC 	R0,	INTBH");
	/* Read data from INTBH*/
	asm("STC 	INTBH,	R0");
	/* Read test register and compare with data 0x0005  */
	asm("CMP.W	#00005H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_IntRegs");

	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R0");
	/* Re-store original contents of INTBH reg. */
	asm("POPC INTBH");

	/* Re-store original contents of Flag reg. */
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");

	/* Error Handling called if testing fails. */
	asm("ERR_IntRegs: 	JMP	_CPU_Test_ErrorHandler");
}




//------------------------------------------------------------------------------------------------
/*
 brief			Stack & Static Base Registers test.
				USP, ISP and SB registers are tested.
                R0,R1 are used as a utility register in the following test.
                This test will fail, if R0 or R1 are faulty.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void TestStackRegs(void)
{
	/* Store original contents of Flag reg. */
	asm("PUSHC FLG");

	/* NOTE: MOV & CMP instruction can not be used with Control Registers hence used STC & LDC.
	R0 is used as a utility register in the following test.
	This test will fail, if R0 is faulty. */
	/* Save original contents of the utility register R0 in the stack */
	asm("PUSH.W 	R0");

	/* Testing SB - Static Base register*/
	/* Store original contents of SB */
	asm("PUSHC 	SB");

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Write data to SB */
	asm("LDC 	R0, SB");
	/* Read contents of SB*/
	asm("STC 	SB,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H,	R0");
	/* Write data to SB */
	asm("LDC 	R0,	SB");
	/* Read contents of SB */
	asm("STC 	SB,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Re-store original contents of SB */
	asm("POPC 	SB");


	/* Testing USP - User Stack Pointer register*/
	/* Select User Stack Pointer*/
	asm("FSET U");

	/* Save original contents of the utility register R1 in the stack */
	asm("PUSH.W 	R1");

	/* Store original contents of USP in R1*/
	asm("STC 	SP,	R1");

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Write data to USP */
	asm("LDC 	R0, SP");
	/* Read contents of USP*/
	asm("STC 	SP,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H,	R0");
	/* Write data to USP */
	asm("LDC 	R0,	SP");
	/* Read contents of USP */
	asm("STC 	SP,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Restore contents of USP from R1*/
	asm("LDC 	R1,	SP");

	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R1");


	/* Testing ISP - Interrupt Stack Pointer register*/
	/* IMPORTANT NOTE: Debugging is not possible for ISP register routine, as E8 uses interrupts. */
	/* Store original contents of ISP*/
	asm("PUSHC 	ISP");

	/* Select Interrupt Stack Pointer*/
	asm("FCLR U");

	/* Write test data 0xAAAA  */
	asm("MOV.W 	#0AAAAH,	R0");
	/* Write data to ISP */
	asm("LDC 	R0, ISP");
	/* Read contents of ISP*/
	asm("STC 	ISP,	R0");
	/* Read test register and compare with data 0xAAAA  */
	asm("CMP.W	#0AAAAH, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Write test data 0x5555  */
	asm("MOV.W 	#05555H,	R0");
	/* Write data to ISP */
	asm("LDC 	R0,	ISP");
	/* Read contents of ISP */
	asm("STC 	ISP,	R0");
	/* Read test register and compare with data 0x5555  */
	asm("CMP.W	#05555H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_StackReg");

	/* Select User Stack Pointer to pop stored control registers. */
	asm("FSET U");

	/* Re-store original contents of ISP */
	asm("POPC 	ISP");


	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R0");
	/* Re-store original contents of Flag reg. */
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");

	/* Error Handling called if testing fails. */
	asm("ERR_StackReg: 	JMP	_CPU_Test_ErrorHandler");
}





//------------------------------------------------------------------------------------------------
/*
 brief			This function tests the FLG register.
				NOTE: We can not test I and D bits in FLG register.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void TestFlagReg(void)
{
	/* Store original contents of Flag reg. */
	asm("PUSHC FLG");

	/* NOTE: MOV & CMP instruction can not be used with Control Registers hence used STC & LDC.
	R0 is used as a utility register in the following test.
	This test will fail, if R0 is faulty. */
	/* Save original contents of the utility register R0 in the stack */
	asm("PUSH.W 	R0");

	/* Interrupt bit 'I' in the flag is always kept disabled to avoid interrupts during testing.
	The test value 0x2098 is used instead of 0xAAAA with considering reserved, Interrupt & Debug bits.

	0010 0000 1001 1000 => 0x2098;
	b15		-	Reserved
	b14:b12	IPL	Processor Interrupt Priority Level
	b11:b8	-	Reserved
	b07		U	Stack Pointer Select Flag
	b06		I	Interrupt Enable Flag
	b05		O	Overflow Flag
	b04		B	Register bank select Flag
	b03		S	Sign flag
	b02		Z	Zero flag
	b01		D	Debug flag
	b00		C	Carry
	*/
	asm("MOV.W 	#02098H,	R0");
	/* Write data to ISP */
	asm("LDC 	R0,	FLG");
	/* Read contents of ISP */
	asm("STC 	FLG,	R0");

	/* Need to mask Reserved bits as these bits are un-predictable.*/
	asm("AND.W	#070FFH,	R0");

	/* Read test register and compare with data 0x2098 */
	asm("CMP.W	#02098H,	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_FlgReg");

	/* Interrupt bit 'I' in the flag is always kept disabled to avoid interrupts during testing.
	The test value 0x5025 is used instead of 0x5555 with considering reserved, Interrupt & Debug bits.

	0101 0000 0010 0101 => 0x5025;
	b15		-	Reserved
	b14:b12	IPL	Processor Interrupt Priority Level
	b11:b8	-	Reserved
	b07		U	Stack Pointer Select Flag
	b06		I	Interrupt Enable Flag
	b05		O	Overflow Flag
	b04		B	Register bank select Flag
	b03		S	Sign flag
	b02		Z	Zero flag
	b01		D	Debug flag
	b00		C	Carry
	*/
	asm("MOV.W 	#05025H,	R0");
	/* Write data to ISP */
	asm("LDC 	R0, FLG");
	/* Read contents of ISP*/
	asm("STC 	FLG,	R0");

	/* Need to mask Reserved bits as these bits are un-predictable.*/
	asm("AND.W	#070FFH,	R0");

	/* Read test register and compare with data 0x5025 */
	asm("CMP.W	#05025H, 	R0");
	/* If error then jump to error routine */
	asm("JNE		ERR_FlgReg");

	/* Select User Stack Pointer to pop stored registers. */
	asm("FSET U");

	/* Restore original contents of the utility Register R0 in the stack */
	asm("POP.W 	R0");
	/* Re-store original contents of Flag reg. */
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");

	/* Error Handling called if testing fails. */
	asm("ERR_FlgReg: 	JMP	_CPU_Test_ErrorHandler");
}




//------------------------------------------------------------------------------------------------
/*
 brief			Program Counter registers test.
				ASSUMPTION: If subroutine is called properly then PC is considered OK.
				            Unfortunately PC test can not be tested for negative or
							failure conditions.

				<<<<<<<<<<<<<<<<<<<<<<< IMPORTANT NOTE >>>>>>>>>>>>>>>>>>>>>
				All interrupts must be disabled before calling this routine.
				<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void TestPCReg(void)
{
	volatile TUSIGN8 ui8Param = 0xAAu;
	volatile TUSIGN8 ui8InvData= 0x00u;
	ui8InvData = TestPC_CharInverse(ui8Param);
	if(0x55u == ui8InvData)
	{
	}
	else
	{
		_CPU_Test_ErrorHandler();
	}
}




//------------------------------------------------------------------------------------------------
/*
 brief			The TestPC_CharInverser()function is put into a different section,
   so that it can be located elsewhere to test PC register test

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
static TUSIGN8 TestPC_CharInverse(TUSIGN8 ui8Char)
{
	ui8Char = (~ui8Char);
	return(ui8Char);
}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM MOV, ABS, ADCF, ADD will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Mov()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");


	asm("MOV.B	#55H, 	R0L");  //R0L--
	asm("MOV.B	R0L, 	R0H");  //ROH
	asm("MOV.B	R0H, 	R1L");  //R1L
	asm("MOV.B	R1L, 	R1H");  //R1H
	asm("MOV.B	R1H, 	A0");  //A0 --
	asm("CMP.B	#55H, 	A0");  //AO CONFIRM
	asm("JNE		ERR_ASM_MOV1");
	asm("MOV.B	R1H, 	A1");  //A1  --
	asm("CMP.B	#55H, 	A1");  //A1 CONFIRM
	asm("JNE		ERR_ASM_MOV1");
	asm("CMP.B	#55H, 	R0L");  //R0L CONFIRM
	asm("JNE		ERR_ASM_MOV1");

	asm("MOV.W	#55AAH, 	R0");   //R0
	asm("MOV.W	R0, 	R1");   //R1
	asm("MOV.W	R1, 	R2");  //R2
	asm("MOV.W	R2, 	R3");  //R3
	asm("MOV.W	R3, 	A0");  //A0
	asm("MOV.W	A0, 	A1");  //A1
	asm("MOV.W	A1, 	R0");  //R0
	asm("CMP.W	#55AAH, 	R0");  //CONFIRM R0


	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	////ABS
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");
	asm("MOV.B	#-80H, 	R0L");
	asm("ABS.B	R0L");
	asm("CMP.B	#80H, 	R0L");
	asm("JNE		ERR_ASM_MOV1");

	asm("JMP.S   	OK_MOV_ASM1");
	asm("ERR_ASM_MOV1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_MOV_ASM1:");


	asm("MOV.B	#-70H, 	R0L");
	asm("ABS.B	R0L");
	asm("CMP.B	#70H, 	R0L");
	asm("JNE		ERR_ASM_MOV2");

	asm("MOV.B	#-80H, 	R1L");
	asm("ABS.B	R1L");
	asm("CMP.B	#80H, 	R1L");
	asm("JNE		ERR_ASM_MOV2");


	asm("MOV.B	#-70H, 	R1L");
	asm("ABS.B	R1L");
	asm("CMP.B	#70H, 	R1L");
	asm("JNE		ERR_ASM_MOV2");

	asm("MOV.B	#-80H, 	R1H");
	asm("ABS.B	R1H");
	asm("CMP.B	#80H, 	R1H");
	asm("JNE		ERR_ASM_MOV2");

	asm("JMP.S   	OK_MOV_ASM2");
	asm("ERR_ASM_MOV2: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_MOV_ASM2:");

	asm("MOV.B	#-70H, 	R1H");
	asm("ABS.B	R1H");
	asm("CMP.B	#70H, 	R1H");
	asm("JNE		ERR_ASM_MOV3");

	asm("MOV.B	#-40H, 	R0H");
	asm("ABS.B	R0H");
	asm("CMP.B	#40H, 	R0H");
	asm("JNE		ERR_ASM_MOV3");


	asm("MOV.B	#-70H, 	R0H");
	asm("ABS.B	R0H");
	asm("CMP.B	#70H, 	R0H");
	asm("JNE		ERR_ASM_MOV3");


	asm("MOV.W	#-40FFH, 	R0");
	asm("ABS.W	R0");
	asm("CMP.W	#40FFH, 	R0");
	asm("JNE		ERR_ASM_MOV3");

	asm("JMP.S   	OK_MOV_ASM3");
	asm("ERR_ASM_MOV3: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_MOV_ASM3:");

	asm("MOV.W	#-70FFH, 	R0");
	asm("ABS.W	R0");
	asm("CMP.W	#70FFH, 	R0");
	asm("JNE		ERR_ASM_MOV4");

	asm("MOV.W	#-40FFH, 	R1");
	asm("ABS.W	R1");
	asm("CMP.W	#40FFH, 	R1");
	asm("JNE		ERR_ASM_MOV4");


	asm("MOV.W	#-70FFH, 	R1");
	asm("ABS.W	R1");
	asm("CMP.W	#70FFH, 	R1");
	asm("JNE		ERR_ASM_MOV4");

	asm("MOV.W	#-40FFH, 	R2");
	asm("ABS.W	R2");
	asm("CMP.W	#40FFH, 	R2");
	asm("JNE		ERR_ASM_MOV4");


	asm("MOV.W	#-70FFH, 	R2");
	asm("ABS.W	R2");
	asm("CMP.W	#70FFH, 	R2");
	asm("JNE		ERR_ASM_MOV4");

	asm("JMP.S   	OK_MOV_ASM4");
	asm("ERR_ASM_MOV4: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_MOV_ASM4:");

	asm("MOV.W	#-40FFH, 	R3");
	asm("ABS.W	R3");
	asm("CMP.W	#40FFH, 	R3");
	asm("JNE		ERR_ASM");


	asm("MOV.W	#-70FFH, 	R3");
	asm("ABS.W	R3");
	asm("CMP.W	#70FFH, 	R3");
	asm("JNE		ERR_ASM");


	asm("MOV.W	#-40FFH, 	A0");
	asm("ABS.W	A0");
	asm("CMP.W	#40FFH, 	A0");
	asm("JNE		ERR_ASM");


	asm("MOV.W	#-70FFH, 	A0");
	asm("ABS.W	A0");
	asm("CMP.W	#70FFH, 	A0");
	asm("JNE		ERR_ASM");

	asm("MOV.W	#-40FFH, 	A1");
	asm("ABS.W	A1");
	asm("CMP.W	#40FFH, 	A1");
	asm("JNE		ERR_ASM");


	asm("MOV.W	#-70FFH, 	A1");
	asm("ABS.W	A1");
	asm("CMP.W	#70FFH, 	A1");
	asm("JNE		ERR_ASM");
	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	//ADCF & ADD
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");

	asm("MOV.B	#8FH, 	R1L");
	asm("ADD.B	#8FH, 	R1L");
	asm("CMP.B	#1EH, 	R1L");
	asm("JNE		ERR_ASM");
	asm("MOV.B	#80H, 	R1L");
	asm("ADCF.B	 R1L");
	asm("CMP.B	#81H, 	R1L");
	asm("JNE		ERR_ASM");

	asm("MOV.W	#8FF0H, 	R0");
	asm("ADD.W	#8F1FH, 	R0");
	asm("CMP.W	#1F0FH, 	R0");
	asm("JNE		ERR_ASM");
	asm("MOV.W	#80F0H, 	R0");
	asm("ADCF.W	 R0");
	asm("CMP.W	#80F1H, 	R0");
	asm("JNE		ERR_ASM");

	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	//ADD

	/* Retun from subroutine after success. */
	asm("RTS");


	asm("ERR_ASM: 	JMP	_ASM_Test_ErrorHandler");
}





//------------------------------------------------------------------------------------------------
/*
 brief			The ASM ADJNZ will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Adjnz()
{
	//ADJNZ
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");

	asm("MOV.B	#8FH, 	R1L");
	asm("ADJNZ.B	#04H, 	R1L, LABEL1");
	asm("JMP.B		ERR_ASM1");
	asm("LABEL1: 	");
	asm("MOV.B	#04H, 	R1L");
	asm("ADJNZ.B	#-4H, 	R1L, ERR_ASM1");


	asm("MOV.W	#-0007H, 	R1");
	asm("ADJNZ.W	#4H, 	R1, LABEL2");
	asm("JMP.B		ERR_ASM1");
	asm("LABEL2: 	");
	asm("MOV.W	#-3H, 	R1");
	asm("ADJNZ.W	#3H, 	R1, ERR_ASM1");

	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM1: 	JMP	_ASM_Test_ErrorHandler");

}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM AND will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_And()
{
	//AND
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");

	asm("MOV.B	#80H, 	R1L");
	asm("AND.B	#0FH, 	R1L");

	asm("CMP.B	#00H, 	R1L");
	asm("JNE		ERR_ASM2");

	asm("MOV.B	#80H, 	R1L");
	asm("MOV.B	#0FH, 	R0L");
	asm("AND.B	R0L, 	R1L");
	asm("CMP.B	#00H, 	R1L");
	asm("JNE		ERR_ASM2");

	asm("MOV.B	#81H, 	R1L");
	asm("AND.B	#0FH, 	R1L");

	asm("CMP.B	#01H, 	R1L");
	asm("JNE		ERR_ASM2");

	asm("MOV.B	#81H, 	R1L");
	asm("MOV.B	#0FH, 	R0L");
	asm("AND.B	R0L, 	R1L");
	asm("CMP.B	#01H, 	R1L");
	asm("JNE		ERR_ASM2");


	asm("MOV.W	#80F0H, 	R1");
	asm("AND.W	#0F0FH, 	R1");

	asm("CMP.W	#00H, 	R1");
	asm("JNE		ERR_ASM2");

	asm("MOV.W	#80F0H, 	R1");
	asm("MOV.W	#0F0FH, 	R0");
	asm("AND.W	R0, 	R1");
	asm("CMP.W	#00H, 	R1");
	asm("JNE		ERR_ASM2");

	asm("MOV.W	#0F0FH, 	R1");
	asm("AND.W	#81F5H, 	R1");

	asm("CMP.W	#0105H, 	R1");
	asm("JNE		ERR_ASM2");

	asm("MOV.W	#00F1H, 	R1");
	asm("MOV.W	#0FFFH, 	R0");
	asm("AND.W	R0, 	R1");
	asm("CMP.W	#0F1H, 	R1");
	asm("JNE		ERR_ASM2");


	asm("FSET	U");
	asm("MOV.W	#0FFFH, 	R0");
	asm("AND.W	R0, 	R1");
	asm("CMP.W	#0F1H, 	R1");
	asm("JNE		ERR_ASM2");

	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM2: 	JMP	_ASM_Test_ErrorHandler");

}


//------------------------------------------------------------------------------------------------
/*
 brief			The ASM BMcnd(BMGEU, BMLTU, BMEQ, BMNE, BMGTU, BMLEU, BMPZ, BMN, BMGE, BMLE, BMGT, BMLT, BMO, BMNO )
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Bmcnd()
{

	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");
	asm("MOV.W	#0000H, 	R0");
	//BMGEU BMLTU
	asm("MOV.W	#0040H, 	R1");
	asm("CMP.W	#0040H, 	R1");
	asm("BMGEU  0,   R0");          //1
	asm("BMLTU  1,   R0");          //0
	asm("CMP.W	#00H, 	R1");
	asm("BMGEU  2,   R0");          //1
	asm("BMLTU  3,   R0");          //0

	asm("CMP.W	#0050H, 	R1");
	asm("BMGEU  4,   R0");          //0
	asm("BMLTU  5,   R0");          //1

	//BMEQ BMNE

	asm("MOV.W	#0040H, 	R1");
	asm("CMP.W	#0040H, 	R1");
	asm("BMEQ  6,   R0");           //1
	asm("BMNE  7,   R0");           //0
	asm("CMP.W	#0050H, 	R1");
	asm("BMEQ  8,   R0");           //0
	asm("BMNE  9,   R0");           //1

	//BMGTU BMLEU

	asm("MOV.W	#0040H, 	R1");
	asm("CMP.W	#0010H, 	R1");
	asm("BMGTU  10,   R0");         //1
	asm("BMLEU  11,   R0");         //0

	asm("CMP.W	#0050H, 	R1");
	asm("BMGTU  12,   R0");         //0
	asm("BMLEU  13,   R0");         //1

	asm("CMP.W	#0030H, 	R1");
	asm("BMGTU  14,   R0");         //1
	asm("BMLEU  15,   R0");         //0

	asm("CMP.W	#6665H, 	R0");
	asm("JNE		ERR_ASM_BMCND1");

	asm("JMP.S   	OK_ASM_BMCND1");
	asm("ERR_ASM_BMCND1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_BMCND1:");

	asm("MOV.W	#0000H, 	R0");
	//BMPZ BMN
	asm("MOV.W	#0040H, 	R1");

	asm("CMP.W	#00H, 	R1");
	asm("BMPZ  0,   R0");           //1
	asm("BMN  1,   R0");            //0

	asm("MOV.W	#0000H, 	R1");

	asm("CMP.W	#00H, 	R1");
	asm("BMPZ  2,   R0");           //1
	asm("BMN  3,   R0");            //0

	asm("MOV.W	#-0040H, 	R1");

	asm("CMP.W	#00H, 	R1");
	asm("BMPZ  4,   R0");           //0
	asm("BMN  5,   R0");            //1
	//BMGE BMLE

	asm("MOV.W	#0040H, 	R1");

	asm("CMP.W	#0040H, 	R1");
	asm("BMGE  6,   R0");           //1
	asm("BMLE  7,   R0");           //1

	asm("CMP.W	#-0010H, 	R1");
	asm("BMGE  8,   R0");           //1
	asm("BMLE  9,   R0");           //0

	asm("CMP.W	#0050H, 	R1");
	asm("BMGE  10,   R0");          //0
	asm("BMLE  11,   R0");          //1
	//BMGT BMLT


	asm("MOV.W	#0040H, 	R1");

	asm("CMP.W	#-0010H, 	R1");
	asm("BMGT  12,   R0");          //1
	asm("BMLT  13,   R0");          //0

	asm("CMP.W	#0050H, 	R1");
	asm("BMGT  14,   R0");          //0
	asm("BMLT  15,   R0");          //1


	asm("CMP.W	#99E5H, 	R0");
	asm("JNE		ERR_ASM3");
	//BMO BMNO
	asm("MOV.W	#0000H, 	R0");

	asm("FSET   O");
	asm("BMO  0,   R0");            //1
	asm("BMNO  1,   R0");           //0
	asm("FCLR   O");
	asm("BMO  2,   R0");            //0
	asm("BMNO  3,   R0");           //1
	asm("CMP.W	#09H, 	R0");
	asm("JNE		ERR_ASM3");


	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM3: 	JMP	_ASM_Test_ErrorHandler");

}



//------------------------------------------------------------------------------------------------
/*
 brief			The ASM BAND, BNOR, BNOT, BNXOR, BOR, BSET, BTST,
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Logic()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");
	asm("MOV.W	#00FFH, 	R0");
	asm("MOV.W	#00FFH, 	R1");
	//BAND
	asm("BCLR   0, R0");
	asm("CMP.W	#00FEH, 	R0");
	asm("JNE		ERR_ASM_LOGIC1");
	asm("MOV.W	#00FEH, 	R0");
	asm("MOV.W	#00FFH, 	R1");
	asm("FCLR   C");
	asm("BAND   0, R0");
	asm("BMGEU  0,   R1");
	asm("FCLR   C");
	asm("BAND   1, R0");
	asm("BMGEU  1,   R1");
	asm("FSET   C");
	asm("BAND   0, R0");
	asm("BMGEU  2,   R1");
	asm("FSET   C");
	asm("BAND   1, R0");
	asm("BMGEU  3,   R1");      //r1 = 0x00f8
	asm("CMP.W	#00F8H, 	R1");
	asm("JNE		ERR_ASM_LOGIC1");



	asm("JMP.S   	OK_ASM_LOGIC1");
	asm("ERR_ASM_LOGIC1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_LOGIC1:");

	//BNOR
	asm("MOV.W	#0000H, 	R1");
	asm("BSET   8, R0");

	asm("FSET   C");
	asm("BNOR   8, R0");
	asm("BMGEU  0,   R1");
	asm("FSET   C");
	asm("BNOR   8, R0");
	asm("BMGEU  1,   R1");

	asm("FCLR   C");
	asm("BNOR   8, R0");
	asm("BMGEU  2,   R1");
	asm("FCLR   C");
	asm("BNOR   8, R0");
	asm("BMGEU  3,   R1");

	asm("CMP.W	#0003H, 	R1");
	asm("JNE		ERR_ASM_LOGIC2");
	//BNOT
	asm("MOV.W	#00F0H, 	R1");
	asm("BNOT  0,   R1");
	asm("BNOT  4,   R1");
	asm("CMP.W	#00E1H, 	R1");
	asm("JNE		ERR_ASM_LOGIC2");

	asm("JMP.S   	OK_ASM_LOGIC2");
	asm("ERR_ASM_LOGIC2: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_LOGIC2:");

	//BNTST
	asm("MOV.W	#0000H, 	R0");
	asm("MOV.W	#00F0H, 	R1");
	asm("BNTST  0,   R1");
	asm("BMLTU  0,   R0");
	asm("BMNE  1,   R0");
	asm("BNTST  4,   R1");
	asm("BMLTU  2,   R0");
	asm("BMNE  3,   R0");

	//BNXOR
	asm("MOV.W	#00F0H, 	R1");
	asm("FSET   C");
	asm("BNXOR  0,   R1");
	asm("BMLTU  4,   R0");
	asm("FSET   C");
	asm("BNXOR  4,   R1");
	asm("BMLTU  5,   R0");
	asm("FCLR   C");
	asm("BNXOR  0,   R1");
	asm("BMLTU  6,   R0");
	asm("FCLR   C");
	asm("BNXOR  4,   R1");
	asm("BMLTU  7,   R0");

	//BOR

	asm("MOV.W	#00F0H, 	R1");
	asm("FSET   C");
	asm("BOR  0,   R1");
	asm("BMLTU  8,   R0");
	asm("FSET   C");
	asm("BOR  4,   R1");
	asm("BMLTU  9,   R0");
	asm("FCLR   C");
	asm("BOR  0,   R1");
	asm("BMLTU  10,   R0");
	asm("FCLR   C");
	asm("BOR  4,   R1");
	asm("BMLTU  11,   R0");
	asm("CMP.W	#049CH, 	R0");
	asm("JNE		ERR_ASM_LOGIC3");


	//BSET
	asm("MOV.W	#00F0H, 	R1");
	asm("BSET  0,   R1");
	asm("BSET  4,   R1");
	asm("CMP.W	#00F1H, 	R1");
	asm("JNE		ERR_ASM_LOGIC3");

	//BTST
	asm("MOV.W	#0000H, 	R0");
	asm("MOV.W	#00F0H, 	R1");
	asm("BTST  0,   R1");
	asm("BMLTU  0,   R0");
	asm("BMNE  1,   R0");
	asm("BTST  4,   R1");
	asm("BMLTU  2,   R0");
	asm("BMNE  3,   R0");

	asm("JMP.S   	OK_ASM_LOGIC3");
	asm("ERR_ASM_LOGIC3: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_LOGIC3:");

	//BXOR
	asm("MOV.W	#00F0H, 	R1");
	asm("FSET   C");
	asm("BXOR  0,   R1");
	asm("BMLTU  4,   R0");
	asm("FSET   C");
	asm("BXOR  4,   R1");
	asm("BMLTU  5,   R0");
	asm("FCLR   C");
	asm("BXOR  0,   R1");
	asm("BMLTU  6,   R0");
	asm("FCLR   C");
	asm("BXOR  4,   R1");
	asm("BMLTU  7,   R0");

	//BTSTC
	asm("MOV.W	#0000H, 	R0");
	asm("MOV.W	#00F0H, 	R1");
	asm("BTSTC  0,   R1");
	asm("BMLTU  8,   R0");
	asm("BMNE  9,   R0");
	asm("BTSTC  4,   R1");
	asm("BMLTU  10,   R0");
	asm("BMNE  11,   R0");
	//BTSTS

	asm("BTSTS  1,   R1");
	asm("BMLTU  12,   R0");
	asm("BMNE  13,   R0");
	asm("BTSTS  5,   R1");
	asm("BMLTU  14,   R0");
	asm("BMNE  15,   R0");

	asm("CMP.W	#9900H, 	R0");
	asm("JNE		ERR_ASM4");

	asm("CMP.W	#00E2H, 	R1");
	asm("JNE		ERR_ASM4");



	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM4: 	JMP	_ASM_Test_ErrorHandler");

}





//------------------------------------------------------------------------------------------------
/*
 brief			The ASM DADC, DADD, DEC, INC, DIV, DIVU, DIVX, DSBB, DSUB
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Add()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");
	//DADC
	asm("MOV.B	#34H, 	R0L");
	asm("FCLR   C");
	asm("DADC.B #34H,R0L");
	asm("CMP.B	#68H,R0L");
	asm("JNE		ERR_ASM_ADD1");
	asm("MOV.B	#34H, 	R0L");
	asm("FSET C");
	asm("DADC.B #34H,R0L");
	asm("CMP.B	#69H, 	R0L");
	asm("JNE		ERR_ASM_ADD1");


	asm("MOV.W	#1534H, 	R0");
	asm("FCLR   C");
	asm("DADC.W #0134H,R0");
	asm("CMP.W	#1668H, 	R0");
	asm("JNE		ERR_ASM_ADD1");
	asm("MOV.W	#1534H, 	R0");
	asm("FSET C");
	asm("DADC.W #0134H,R0");
	asm("CMP.W	#1669H, 	R0");
	asm("JNE		ERR_ASM_ADD1");

	asm("JMP.S   	OK_ASM_ADD1");
	asm("ERR_ASM_ADD1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ADD1:");

	//DADD
	asm("MOV.B	#34H, 	R0L");
	asm("FCLR   C");
	asm("DADC.B #34H,R0L");
	asm("CMP.B	#68H ,	R0L");
	asm("JNE		ERR_ASM_ADD2");
	asm("MOV.B	#34H, 	R0L");
	asm("FSET C");
	asm("DADC.B #34H,R0L");
	asm("CMP.B	#69H, 	R0L");
	asm("JNE		ERR_ASM_ADD2");


	asm("MOV.W	#1534H, 	R0");
	asm("FCLR   C");
	asm("DADC.W #0134H,R0");
	asm("CMP.W	#1668H, 	R0");
	asm("JNE		ERR_ASM_ADD2");
	asm("MOV.W	#1534H, 	R0");
	asm("FSET C");
	asm("DADC.W #0134H,R0");
	asm("CMP.W	#1669H ,	R0");
	asm("JNE		ERR_ASM_ADD2");

	//DEC
	asm("MOV.B	#34H, 	R0L");
	asm("DEC.B R0L");
	asm("CMP.B	#33H ,	R0L");
	asm("JNE		ERR_ASM_ADD2");


	asm("JMP.S   	OK_ASM_ADD2");
	asm("ERR_ASM_ADD2: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ADD2:");

	asm("MOV.W	#1034H, 	A0");
	asm("DEC.W A0");
	asm("CMP.W	#1033H ,	A0");
	asm("JNE		ERR_ASM_ADD3");
	//INC
	asm("MOV.B	#34H, 	R0L");
	asm("INC.B R0L");
	asm("CMP.B	#35H ,	R0L");
	asm("JNE		ERR_ASM_ADD3");

	asm("MOV.W	#1034H, 	A0");
	asm("INC.W A0");
	asm("CMP.W	#1035H ,	A0");
	asm("JNE		ERR_ASM_ADD3");
	//DIV
	asm("FCLR   C");
	asm("MOV	#1034H, 	R0");
	asm("DIV.B #56H");
	asm("CMP.B	#30H, 	R0L");
	asm("JNE		ERR_ASM_ADD3");
	asm("CMP.B	#14H ,	R0H");
	asm("JNE		ERR_ASM_ADD3");

	asm("JMP.S   	OK_ASM_ADD3");
	asm("ERR_ASM_ADD3: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ADD3:");



	asm("FCLR   C");

	asm("MOV	#4034H, 	R0");
	asm("MOV	#1034H, 	R2");
	asm("DIV.W #5103H");
	asm("CMP.W	#3334H, 	R0");
	asm("JNE		ERR_ASM_ADD4");
	asm("CMP.W	#3298H, 	R2");
	asm("JNE		ERR_ASM_ADD4");

	//DIVU
	asm("FCLR   C");
	asm("MOV	#1034H, 	R0");
	asm("DIVU.B #56H");
	asm("CMP.B	#30H, 	R0L");
	asm("JNE		ERR_ASM_ADD4");
	asm("CMP.B	#14H ,	R0H");
	asm("JNE		ERR_ASM_ADD4");

	asm("FCLR   C");

	asm("MOV	#4034H, 	R0");
	asm("MOV	#1034H, 	R2");
	asm("DIVU.W #5103H");
	asm("CMP.W	#3334H, 	R0");
	asm("JNE		ERR_ASM_ADD4");
	asm("CMP.W	#3298H, 	R2");
	asm("JNE		ERR_ASM_ADD4");

	asm("JMP.S   	OK_ASM_ADD4");
	asm("ERR_ASM_ADD4: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ADD4:");

	//DIVX
	asm("FCLR   C");
	asm("MOV	#1034H, 	R0");
	asm("DIVX.B #56H");
	asm("CMP.B	#30H, 	R0L");
	asm("JNE		ERR_ASM_ADD5");
	asm("CMP.B	#14H ,	R0H");
	asm("JNE		ERR_ASM_ADD5");

	asm("FCLR   C");

	asm("MOV	#4034H, 	R0");
	asm("MOV	#1034H, 	R2");
	asm("DIVX.W #5103H");
	asm("CMP.W	#3334H, 	R0");
	asm("JNE		ERR_ASM_ADD5");
	asm("CMP.W	#3298H, 	R2");
	asm("JNE		ERR_ASM_ADD5");


	//DSBB
	asm("MOV.B	#34H, 	R0L");
	asm("FCLR   C");
	asm("DSBB.B #34H,R0L");
	asm("CMP.B	#99H ,	R0L");
	asm("JNE		ERR_ASM_ADD5");
	asm("MOV.B	#34H, 	R0L");
	asm("FSET C");
	asm("DSBB.B #34H,R0L");
	asm("CMP.B	#00H, 	R0L");
	asm("JNE		ERR_ASM_ADD5");

	asm("JMP.S   	OK_ASM_ADD5");
	asm("ERR_ASM_ADD5: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ADD5:");


	asm("MOV.W	#1F34H, 	R0");
	asm("FCLR   C");
	asm("DSBB.W #0134H,R0L");
	asm("CMP.W	#1D99H, 	R0");
	asm("JNE		ERR_ASM5");
	asm("MOV.W	#1F34H, 	R0");
	asm("FSET C");
	asm("DSBB.W #0134H,R0");
	asm("CMP.W	#1E00H, 	R0");
	asm("JNE		ERR_ASM5");


	//DSUB
	asm("MOV.B	#34H, 	R0L");
	asm("FCLR   C");
	asm("DSUB.B #34H,R0L");
	asm("CMP.B	#00H, 	R0L");
	asm("JNE		ERR_ASM5");
	asm("MOV.B	#34H, 	R0L");
	asm("FSET C");
	asm("DSUB.B #34H,R0L");
	asm("CMP.B	#00H, 	R0L");
	asm("JNE		ERR_ASM5");


	asm("MOV.W	#1F34H, 	R0");
	asm("FCLR   C");
	asm("DSUB.W #0134H,R0");
	asm("CMP.W	#1E00H, 	R0");
	asm("JNE		ERR_ASM5");
	asm("MOV.W	#1F34H, 	R0");
	asm("FSET C");
	asm("DSUB.W #0134H,R0");
	asm("CMP.W	#1E00H ,	R0");
	asm("JNE		ERR_ASM5");


	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM5: 	JMP	_ASM_Test_ErrorHandler");

}



//------------------------------------------------------------------------------------------------
/*
 brief			The ASM Jcnd ( GEU, LTU, EQ, NE,  GTU,  LEU,   GE,  LE,  GT,  LT  )
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Jcnd()
{

	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1");
	asm("MOV.W	#0000H, 	R0");
	//GEU LTU
	asm("FCLR C");
	asm("JC ERR_ASM_JCND1");
	asm("JNC OK_JCND_0");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_0:");
	asm("FSET C");
	asm("JNC ERR_ASM_JCND1");
	asm("JC OK_JCND_1");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_1:");
	//EQ  NE
	asm("FCLR Z");
	asm("JZ ERR_ASM_JCND1");
	asm("JNZ OK_JCND_2");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_2:");

	asm("FSET Z");
	asm("JNZ ERR_ASM_JCND1");
	asm("JZ OK_JCND_3");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("ERR_ASM_JCND1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_3:");
	// GTU  LEU   GE  LE  GT  LT
	asm("MOV.W	#1034H, 	R0");
	asm("CMP.W	#0030H, 	R0");//<

	asm("JLEU ERR_ASM_JCND2");
	asm("JGTU OK_JCND_4");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_4:");

	asm("JLE ERR_ASM_JCND2");
	asm("JGE OK_JCND_5");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_5:");

	asm("JLT ERR_ASM_JCND2");
	asm("JGT OK_JCND_6");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_6:");

	asm("MOV.W	#1034H, 	R0");
	asm("CMP.W	#1034H, 	R0");//=

	asm("JGTU ERR_ASM_JCND2");
	asm("JLEU OK_JCND_7");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("ERR_ASM_JCND2: 	JMP	_ASM_Test_ErrorHandler");


	asm("OK_JCND_7:");



	asm("JLE OK_JCND_8");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_8:");
	asm("JGE OK_JCND_9");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_9:");

	asm("JLT ERR_ASM6");
	asm("JGT ERR_ASM6");


	asm("MOV.W	#1034H, 	R0");
	asm("CMP.W	#1035H, 	R0");//>

	asm("JGTU ERR_ASM6");
	asm("JLEU OK_JCND_11");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_11:");

	asm("JGE ERR_ASM6");
	asm("JLE OK_JCND_12");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_12:");

	asm("JGT ERR_ASM6");
	asm("JLT OK_JCND_13");
	asm("OK_JCND_13:");
	/// O
	asm("FCLR O");
	asm("JO ERR_ASM6");
	asm("JNO OK_JCND_14");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_14:");
	asm("FSET O");
	asm("JNO ERR_ASM6");
	asm("JO OK_JCND_15");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_15:");

	///PZ N
	asm("MOV.W	#1034H, 	R0");
	asm("CMP.W	#00H, 	R0");//<
	asm("JN ERR_ASM6");
	asm("JPZ OK_JCND_16");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_16:");
	asm("MOV.W	#0000H, 	R0");
	asm("CMP.W	#00H, 	R0");//=
	asm("JN ERR_ASM6");
	asm("JPZ OK_JCND_17");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_17:");
	asm("MOV.W	#-20F0H, 	R0");
	asm("CMP.W	#00H, 	R0");//>
	asm("JPZ ERR_ASM6");
	asm("JN OK_JCND_18");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_JCND_18:");

	asm("POPM   R1,R3,A1");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM6: 	JMP	_ASM_Test_ErrorHandler");

}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM LDC STC LDE STE
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Ldc()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");

	asm("MOV.W #1020H,R0");
	asm("MOV.W #00H,R1");
	asm("FCLR C");
	//LDC STC
	asm("LDC R0,SB");
	asm("STC SB,R1");
	asm("CMP.W #1020H, R1");
	asm("JNE		ERR_ASM7");
	asm("LDC R0,FB");
	asm("STC FB,R1");
	asm("CMP.W #1020H, R1");
	asm("JNE		ERR_ASM7");

	asm("MOV.W #1203H,A0");
	asm("MOV.W #00H,A1");

	//LDE STE
	asm("MOV.B #55H,R0L");
	asm("LDE.B [A1A0],R1L");
	asm("STE.B R0L,[A1A0]");
	asm("LDE.B [A1A0],R1H");
	asm("STE.B R1L,[A1A0]");
	asm("CMP.B R0L, R1H");
	asm("JNE		ERR_ASM7");

	asm("MOV.W #1055H,R0");
	asm("LDE.W [A1A0],R1");
	asm("STE.W R0,[A1A0]");
	asm("LDE.W [A1A0],R2");
	asm("STE.W R1,[A1A0]");
	asm("CMP.W R0, R2");
	asm("JNE		ERR_ASM7");



	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM7: 	JMP	_ASM_Test_ErrorHandler");

}



//------------------------------------------------------------------------------------------------
/*
 brief			The ASM MUL MULU NEG NOT OR
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Calc()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//MUL
	asm("MOV.B #15H,R0L");
	asm("MOV.B #05H,A0");
	asm("MUL.B R0L,A0");
	asm("CMP.B #69h, A0");
	asm("JNE		ERR_ASM8");
	asm("MOV.W #1015H,R0");
	asm("MOV.W #045H,A0");
	asm("MUL.W R0,A0");
	asm("CMP.W #55A9h, A0");
	asm("JNE		ERR_ASM8");
	//MULU
	asm("MOV.B #15H,R0L");
	asm("MOV.B #05H,A0");
	asm("MULU.B R0L,A0");
	asm("CMP.B #69h, A0");
	asm("JNE		ERR_ASM8");
	asm("MOV.W #1015H,R0");
	asm("MOV.W #045H,A0");
	asm("MULU.W R0,A0");
	asm("CMP.W #55A9h, A0");
	asm("JNE		ERR_ASM8");
	//NEG
	asm("MOV.B #15H,R0L");
	asm("NEG.B R0L");
	asm("CMP.B #-15h, R0L");
	asm("JNE		ERR_ASM8");
	asm("MOV.W #1015H,R0");
	asm("NEG.W R0");
	asm("CMP.W #-1015H, R0");
	asm("JNE		ERR_ASM8");

	//NOT
	asm("MOV.B #85H,R0L");
	asm("NOT.B R0L");
	asm("CMP.B #7Ah, R0L");
	asm("JNE		ERR_ASM8");
	asm("MOV.W #8015H,R0");
	asm("NOT.W R0");
	asm("CMP.W #7FEAH, R0");
	asm("JNE		ERR_ASM8");


	//OR
	asm("MOV.B #85H,R0L");
	asm("OR.B #34H,R0L");
	asm("CMP.B #-4Bh, R0L");
	asm("JNE		ERR_ASM8");
	asm("MOV.W #8015H,R0");
	asm("OR.W #1023H,R0");
	asm("CMP.W #-6FC9H, R0");
	asm("JNE		ERR_ASM8");


	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM8: 	JMP	_ASM_Test_ErrorHandler");
}





//------------------------------------------------------------------------------------------------
/*
 brief			The ASM ROLC RORC ROT
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Rot()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//ROLC
	asm("MOV.B #15H,R0L");
	asm("FCLR C");
	asm("ROLC.B R0L");
	asm("CMP.B #2Ah, R0L");
	asm("JNE		ERR_ASM_ROT1");
	asm("MOV.B #15H,R0L");
	asm("FSET C");
	asm("ROLC.B R0L");
	asm("CMP.B #2Bh, R0L");
	asm("JNE		ERR_ASM_ROT1");

	asm("MOV.W #1015H,R0");
	asm("FCLR C");
	asm("ROLC.W R0");
	asm("CMP.W #202Ah, R0");
	asm("JNE		ERR_ASM_ROT1");
	asm("MOV.W #1015H,R0");
	asm("FSET C");
	asm("ROLC.W R0");
	asm("CMP.W #202Bh, R0");
	asm("JNE		ERR_ASM_ROT1");

	asm("JMP.S    OK_ASM_ROT1");
	asm("ERR_ASM_ROT1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ROT1:");

	asm("MOV.B #15H,R0L");
	asm("ROLC.B R0L");
	asm("JC ERR_ASM_ROT2");

	asm("MOV.B #85H,R0L");
	asm("ROLC.B R0L");
	asm("JNC ERR_ASM_ROT2");

	asm("MOV.W #1005H,R0");
	asm("ROLC.W R0");
	asm("JC ERR_ASM_ROT2");

	asm("MOV.W #8005H,R0");
	asm("ROLC.W R0");
	asm("JNC ERR_ASM_ROT2");

	asm("JMP.S    OK_ASM_ROT2");
	asm("ERR_ASM_ROT2: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ROT2:");
	//RORC
	asm("MOV.B #15H,R0L");
	asm("FCLR C");
	asm("RORC.B R0L");
	asm("CMP.B #0Ah, R0L");
	asm("JNE		ERR_ASM_ROT3");
	asm("MOV.B #15H,R0L");
	asm("FSET C");
	asm("RORC.B R0L");
	asm("CMP.B #8Ah, R0L");
	asm("JNE		ERR_ASM_ROT3");

	asm("MOV.W #1015H,R0");
	asm("FCLR C");
	asm("RORC.W R0");
	asm("CMP.W #080Ah, R0");
	asm("JNE		ERR_ASM_ROT3");
	asm("MOV.W #1015H,R0");
	asm("FSET C");
	asm("RORC.W R0");
	asm("CMP.W #880AH, R0");
	asm("JNE		ERR_ASM_ROT3");

	asm("JMP.S    OK_ASM_ROT3");
	asm("ERR_ASM_ROT3: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_ROT3:");

	asm("MOV.B #15H,R0L");
	asm("RORC.B R0L");
	asm("JNC OK_ASM_ROT3");

	asm("MOV.B #80H,R0L");
	asm("RORC.B R0L");
	asm("JC OK_ASM_ROT3");

	asm("MOV.W #1005H,R0");
	asm("RORC.W R0");
	asm("JNC ERR_ASM9");

	asm("MOV.W #8000H,R0");
	asm("RORC.W R0");
	asm("JC ERR_ASM9");
	//ROT
	asm("MOV.B #56H,R0L");
	asm("ROT.B #5,R0L");
	asm("JC ERR_ASM9");
	asm("CMP.B #-36h, R0L");
	asm("JNE		ERR_ASM9");

	asm("MOV.B #56H,R0L");
	asm("ROT.B #4,R0L");
	asm("JNC ERR_ASM9");
	asm("CMP.B #65h, R0L");
	asm("JNE		ERR_ASM9");


	asm("MOV.B #56H,R0L");
	asm("ROT.B #-5,R0L");
	asm("JNC ERR_ASM9");
	asm("CMP.B #-4Eh, R0L");
	asm("JNE		ERR_ASM9");

	asm("MOV.B #56H,R0L");
	asm("ROT.B #-4,R0L");
	asm("JC ERR_ASM9");
	asm("CMP.B #65h, R0L");
	asm("JNE		ERR_ASM9");


	asm("MOV.W #5656H,R0");
	asm("ROT.W #5,R0");
	asm("JC ERR_ASM9");
	asm("CMP.W #-3536h, R0");
	asm("JNE		ERR_ASM9");

	asm("MOV.W #5656H,R0");
	asm("ROT.W #4,R0");
	asm("JNC ERR_ASM9");
	asm("CMP.W #6565H, R0");
	asm("JNE		ERR_ASM9");


	asm("MOV.W #5656H,R0");
	asm("ROT.W #-5,R0");
	asm("JNC ERR_ASM9");
	asm("CMP.W #-4D4Eh, R0");
	asm("JNE		ERR_ASM9");

	asm("MOV.W #5656H,R0");
	asm("ROT.W #-4,R0");
	asm("JC ERR_ASM9");
	asm("CMP.W #6565h, R0");
	asm("JNE		ERR_ASM9");


	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM9: 	JMP	_ASM_Test_ErrorHandler");
}







//------------------------------------------------------------------------------------------------
/*
 brief			The ASM SHL SHA
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Shift()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//SHL
	asm("MOV.B #55H,R0L");
	asm("SHL.B #1, R0L");
	asm("JC		ERR_ASM_SHIFT1");
	asm("CMP.B #-56h, R0L");
	asm("JNE		ERR_ASM_SHIFT1");

	asm("MOV.B #55H,R0L");
	asm("SHL.B #-1, R0L");
	asm("JNC		ERR_ASM_SHIFT1");
	asm("CMP.B #2Ah, R0L");
	asm("JNE		ERR_ASM_SHIFT1");

	asm("MOV.B #55H,R0L");
	asm("SHL.B #2, R0L");
	asm("JNC		ERR_ASM_SHIFT1");
	asm("CMP.B #54h, R0L");
	asm("JNE		ERR_ASM_SHIFT1");

	asm("JMP.S    OK_ASM_SHIFT1");
	asm("ERR_ASM_SHIFT1: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_SHIFT1:");

	asm("MOV.B #55H,R0L");
	asm("SHL.B #-2, R0L");
	asm("JC		ERR_ASM_SHIFT1");
	asm("CMP.B #15h, R0L");
	asm("JNE		ERR_ASM_SHIFT1");

	//SHA
	asm("MOV.B #55H,R0L");
	asm("SHA.B #1, R0L");
	asm("JC		ERR_ASM_SHIFT1");
	asm("CMP.B #-56h, R0L");
	asm("JNE		ERR_ASM_SHIFT1");

	asm("MOV.B #55H,R0L");
	asm("SHA.B #-1, R0L");
	asm("JNC		ERR_ASM_SHIFT2");
	asm("CMP.B #2Ah, R0L");
	asm("JNE		ERR_ASM_SHIFT2");

	asm("MOV.B #55H,R0L");
	asm("SHA.B #2, R0L");
	asm("JNC		ERR_ASM_SHIFT2");
	asm("CMP.B #54h, R0L");
	asm("JNE		ERR_ASM_SHIFT2");

	asm("MOV.B #55H,R0L");
	asm("SHA.B #-2, R0L");
	asm("JC		ERR_ASM_SHIFT2");
	asm("CMP.B #15h, R0L");
	asm("JNE		ERR_ASM_SHIFT2");

	asm("JMP.S    OK_ASM_SHIFT2");
	asm("ERR_ASM_SHIFT2: 	JMP	_ASM_Test_ErrorHandler");
	asm("OK_ASM_SHIFT2:");

	//SHL
	asm("MOV.W #5555H,R0");
	asm("SHL.W #1, R0");
	asm("JC		ERR_ASM10");
	asm("CMP.W #-5556h, R0");
	asm("JNE		ERR_ASM10");

	asm("MOV.W #5555H,R0");
	asm("SHL.W #-1, R0");
	asm("JNC		ERR_ASM10");
	asm("CMP.W #2AAAh, R0");
	asm("JNE		ERR_ASM10");

	asm("MOV.W #5555H,R0");
	asm("SHL.W #2, R0");
	asm("JNC		ERR_ASM10");
	asm("CMP.W #5554h, R0");
	asm("JNE		ERR_ASM10");


	asm("MOV.W #5555H,R0");
	asm("SHL.W #-2, R0");
	asm("JC		ERR_ASM10");
	asm("CMP.W #1555h, R0");
	asm("JNE		ERR_ASM10");

	//SHA
	asm("MOV.W #5555H,R0");
	asm("SHA.W #1, R0");
	asm("JC		ERR_ASM10");
	asm("CMP.W #-5556h, R0");
	asm("JNE		ERR_ASM10");

	asm("MOV.W #5555H,R0");
	asm("SHA.W #-1, R0");
	asm("JNC		ERR_ASM10");
	asm("CMP.W #2AAAh, R0");
	asm("JNE		ERR_ASM10");

	asm("MOV.W #5555H,R0");
	asm("SHA.W #2, R0");
	asm("JNC		ERR_ASM10");
	asm("CMP.W #5554h, R0");
	asm("JNE		ERR_ASM10");

	asm("MOV.W #5555H,R0");
	asm("SHA.W #-2, R0");
	asm("JC		ERR_ASM10");
	asm("CMP.W #1555h, R0");
	asm("JNE		ERR_ASM10");

	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM10: 	JMP	_ASM_Test_ErrorHandler");
}





//------------------------------------------------------------------------------------------------
/*
 brief			The ASM SBB SBJNZ
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Sbb()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//SBB
	asm("MOV.B #55H,R0L");
	asm("FSET C");
	asm("SBB.B #01H, R0L");
	asm("CMP.B #54h, R0L");
	asm("JNE		ERR_ASM11");

	asm("MOV.B #55H,R0L");
	asm("FCLR C");
	asm("SBB.B #01H, R0L");
	asm("CMP.B #53h, R0L");
	asm("JNE		ERR_ASM11");

	asm("MOV.W #5215H,R0");
	asm("FSET C");
	asm("SBB.W #1123H, R0");
	asm("CMP.W #40F2h, R0");
	asm("JNE		ERR_ASM11");

	asm("MOV.W #5235H,R0");
	asm("FCLR C");
	asm("SBB.W #1231H, R0");
	asm("CMP.W #4003H, R0");
	asm("JNE		ERR_ASM11");



	//SBJNZ
	asm("FCLR C");
	asm("MOV.B #05H,R0L");
	asm("SBJNZ.B #05H, R0L, ERR_ASM11");

	asm("MOV.B #55H,R0L");
	asm("SBJNZ.B #05H, R0L, OK_SBB_1");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_SBB_1:");

	asm("MOV.W #0005H,R0");
	asm("SBJNZ.W #05H, R0, ERR_ASM11");

	asm("MOV.W #5125H,R0");
	asm("SBJNZ.W #5105H, R0, OK_SBB_2");
	asm("JMP	_ASM_Test_ErrorHandler");
	asm("OK_SBB_2:");


	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM11: 	JMP	_ASM_Test_ErrorHandler");
}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM XCHG XOR
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Xchg()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//XCHG
	asm("MOV.B #55H,R0L");
	asm("MOV.B #15H,R1L");
	asm("XCHG.B R1L, R0L");
	asm("CMP.B #15h, R0L");
	asm("JNE		ERR_ASM12");
	asm("CMP.B #55h, R1L");
	asm("JNE		ERR_ASM12");

	asm("MOV.W #5095H,R0");
	asm("MOV.W #1085H,R1");
	asm("XCHG.W R1, R0");
	asm("CMP.W #1085h, R0");
	asm("JNE		ERR_ASM12");
	asm("CMP.W #5095h, R1");
	asm("JNE		ERR_ASM12");



	//XOR
	asm("MOV.B #55H,R0L");
	asm("MOV.B #15H,R1L");
	asm("XOR.B R1L, R0L");
	asm("CMP.B #40h, R0L");
	asm("JNE		ERR_ASM11");

	asm("MOV.W #5095H,R0");
	asm("MOV.W #1085H,R1");
	asm("XOR.W R1, R0");
	asm("CMP.W #4010h, R0");
	asm("JNE		ERR_ASM12");


	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM12: 	JMP	_ASM_Test_ErrorHandler");
}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM MOVHH MOVHL MOVLH MOVLL
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Movdir()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//MOVHH
	asm("MOV.B #5AH,R0L");
	asm("MOV.B #00H,R1L");
	asm("MOVHH R0L, R1L");
	asm("CMP.B #50H, R1L");
	asm("JNE		ERR_ASM13");
	//MOVHL
	asm("MOV.B #00H,R1L");
	asm("MOVHL R0L, R1L");
	asm("CMP.B #05h, R1L");
	asm("JNE		ERR_ASM13");
	//MOVLH
	asm("MOV.B #00H,R1L");
	asm("MOVLH R0L, R1L");
	asm("CMP.B #-60h, R1L");
	asm("JNE		ERR_ASM13");
	//MOVLL

	asm("MOV.B #00H,R1L");
	asm("MOVLL R0L, R1L");
	asm("CMP.B #0Ah, R1L");
	asm("JNE		ERR_ASM13");



	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");

	/* Retun from subroutine after success. */
	asm("RTS");
	asm("ERR_ASM13: 	JMP	_ASM_Test_ErrorHandler");
}





//------------------------------------------------------------------------------------------------
/*
 brief			The ASM STNZ STZ STZX
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler

 \author		Paul
 \date		23-02-2012
 \param void
 \return void
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Stz()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1,SB,FB");
	//STNZ
	asm("MOV.B  #32H, R0L");
	asm("FSET  Z");
	asm("STNZ  #21H,R0L");
	asm("CMP.B #32H,R0L");
	asm("JNE   ERR_ASM14");
	asm("FCLR  Z");
	asm("STNZ  #21H,R0L");
	asm("CMP.B #21H,R0L");
	asm("JNE   ERR_ASM14");

	//STZ
	asm("MOV.B  #32H, R0L");
	asm("FCLR  Z");
	asm("STZ  #21H,R0L");
	asm("CMP.B #32H,R0L");
	asm("JNE   ERR_ASM14");
	asm("FSET  Z");
	asm("STZ  #21H,R0L");
	asm("CMP.B #21H,R0L");
	asm("JNE   ERR_ASM14");

	//STZX
	asm("MOV.B  #52H, R0L");
	asm("FCLR  Z");
	asm("STZX  #32H,#21H,R0L");
	asm("CMP.B #21H,R0L");
	asm("JNE   ERR_ASM14");
	asm("FSET  Z");
	asm("STZX  #32H,#21H,R0L");
	asm("CMP.B #32H,R0L");
	asm("JNE   ERR_ASM14");

	asm("POPM   R1,R3,A1,SB,FB");
	asm("POPC FLG");


	asm("RTS");
	asm("ERR_ASM14: 	JMP	_ASM_Test_ErrorHandler");

}




//------------------------------------------------------------------------------------------------
/*
 brief			The value will be Sum( SRC[i] * SRC1[i]) used to test ASM Rmpa

 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
TUSIGN16 Calc_Srmpa_b(TINT16 len, TUSIGN8 const NEAR* SRC,TUSIGN8 const NEAR* SRC1)
{
	TUSIGN16 DAT= 0;
	TINT16 index=0;
	for(index=0; index<len; index++)
	{
		DAT += SRC1[index] * SRC[index];
	}
	return DAT;
}

//------------------------------------------------------------------------------------------------
/*
 brief			The value will be Sum( SRC[i] * SRC1[i]) used to test ASM Rmpa

 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
//lint -e{647} hao wu,  2013-01-15 Ok
TUSIGN32 Calc_Srmpa_w(TINT16 len,TUSIGN16 const NEAR* SRC,TUSIGN16 const NEAR* SRC1)
{
	TUSIGN32 DAT= 0;
	TINT16 index=0;
	for(index=0; index<len; index++)
	{
		DAT += SRC1[index] * SRC[index];
	}
	return DAT;
}




//------------------------------------------------------------------------------------------------
/*
 brief			The ASM RMPA
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler
 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Rmpa()
{
	TINT16 len = 12;
	TUSIGN8 src[20];
	TUSIGN8 src1[20];
	TUSIGN16 src2[20];
	TUSIGN16 src3[20];
	TUSIGN8 index = 0;
	for(index=0; index<20; index++)
	{
		src[index] = index+1;
		src1[index] = index+2;
		src2[index] = (index*5)+1;
		src3[index] = (index*5)+2;
	}
	Test_Rmpa_b(len,src,src1);
	Test_Rmpa_w(len,src2,src3);
}


//------------------------------------------------------------------------------------------------
/*
 brief			The ASM SMOV
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler
 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Smov()
{
	TUSIGN16 dataTO1[25];
	TINT16 len = 10;
	TINT16 index = 0;
	TUSIGN8 HUGE* dataSrc;
	TUSIGN16 HUGE* dataSrc1;
	TUSIGN8 dataTO[40];
	// TUSIGN8     *tmpPtr;
	(void)memset(dataTO,0,30);
	dataSrc = (TUSIGN8 HUGE*)0x0C8FA0;
	dataSrc1 = (TUSIGN16 HUGE*)0x0C8F10;
	Test_Smovfb(len,dataTO,dataSrc);
	for(index = 0; index < len; index++)
	{
		if(dataSrc[index] != dataTO[index])
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}
	(void)memset(dataTO1,0,30);
	Test_Smovfw(len,dataTO1,dataSrc1);
	for(index = 0; index < len; index++)
	{
		if(dataTO1[index] != dataSrc1[index])
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}

	(void)memset(dataTO,0,30);
	Test_Smovbb(len,(dataTO+len)-1,(dataSrc+len)-1);
	for(index = 0; index < len; index++)
	{
		if(dataSrc[index] != dataTO[index])
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}
	(void) memset(dataTO1,0,30);
	Test_Smovbw(len,&dataTO1[len-1],&dataSrc1[len-1]);
	for(index = 0; index < len; index++)
	{
		if(dataSrc1[index] != dataTO1[index])
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}
	(void) memset(dataTO,0,30);
	Test_Sstrb(len,dataTO,0x50);
	for(index = 0; index < len; index++)
	{
		if(dataTO[index] != 0x50)
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}
	(void) memset(dataTO1,0,20);
	Test_Sstrw(len,dataTO1,0x5A5C);
	for(index = 0; index < len; index++)
	{
		if(dataTO1[index] != 0x5A5C)
		{
			_ASM_Test_ErrorHandler();
			break;
		}
	}
}

//------------------------------------------------------------------------------------------------
/*
 brief			The ASM TST
                will be tested in this function. If error happens, It wiil call _ASM_Test_ErrorHandler
 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void Test_Tst()
{
	asm("PUSHC FLG");
	asm("PUSHM   R1,R3,A1;");
	//TST.B
	asm("MOV.B  #05H, R0L");
	asm("TST.B  #50H, R0L");
	asm("JNZ   ERR_ASM15");
	asm("JN   ERR_ASM15");
	asm("MOV.B  #05H, R0L");
	asm("TST.B  #51H, R0L");
	asm("JZ   ERR_ASM15");
	asm("JN   ERR_ASM15");

	asm("MOV.B  #-5bH, R0L");
	asm("TST.B  #-5FH, R0L");
	asm("JZ   ERR_ASM15");
	asm("JPZ   ERR_ASM15");

	//TST.W
	asm("MOV.W  #0505H, R0");
	asm("TST.W  #5050H, R0");
	asm("JNZ   ERR_ASM15");
	asm("JN   ERR_ASM15");
	asm("MOV.W  #0545H, R0");
	asm("TST.W  #35H, R0");
	asm("JZ   ERR_ASM15");
	asm("JN   ERR_ASM15");

	asm("MOV.W  #-5bA5H, R0");
	asm("TST.W  #-5FA0H, R0");
	asm("JZ   ERR_ASM15");
	asm("JPZ   ERR_ASM15");


	asm("POPM   R1,R3,A1;");
	asm("POPC FLG");

	asm("RTS");
	asm("ERR_ASM15: 	JMP	_ASM_Test_ErrorHandler");
}

//------------------------------------------------------------------------------------------------
/*
 brief			All the ASM test function will be called in this function.
 \author		Paul
 \date		23-02-2012
 \param len SRC SRC1
 \return DAT
 \warning
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//------------------------------------------------------------------------------------------------
void ASM_TestAll()
{
#if 0
	Test_Mov();
	Test_Adjnz();
	Test_And();
	Test_Bmcnd();
	Test_Logic();
	Test_Add();
	Test_Jcnd();
	Test_Ldc();
	Test_Calc();
	Test_Rot();
	Test_Shift();
	Test_Sbb();
	Test_Xchg();
	Test_Movdir();
	Test_Tst();
	Test_Stz();
	Test_Jmp();
	Test_Smov();
	Test_Rmpa();
	Test_Mova();
#endif
}

/*
BRK;
ENTER;
EXITD;
EXTS;
INT;;
INTO;
JMPS;
JSRS;
LDCTX;
LDINTB;
LDIPL;
NOP;
REIT;
STCTX ; //This instruction saves task context to the stack area.
UND;
WAIT;
*/
/*
JMP
JMPI
JSR
JSRI
*/
#endif