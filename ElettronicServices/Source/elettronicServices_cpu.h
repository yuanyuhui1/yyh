//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ElettronicServices_types.h
#endif

//............................................
//!  extend include list if neccessary


#ifdef __ELETTRONIC_SERVICES_CPU_H__
  #error ElettronicServices_cpu.h included more than once
#endif

#define __ELETTRONIC_SERVICES_CPU_H__

#define USE_TestGPRsCoupling    1

#if 0
void CPU_TestAll(void);
void ASM_TestAll(void);
TBOOL API_TestCPU(void);

void TestGPRs(void);
void TestGPRsCoupling(void);
void TestIntRegs(void);
void TestStackRegs(void);


void TestPCReg(void);
void TestFlagReg(void);
//asm
void Test_Mov(void);
void Test_Adjnz(void);
void Test_And(void);
void Test_Bmcnd(void);
void Test_Logic(void);
void Test_Add(void);
void Test_Jcnd(void);
void Test_Ldc(void);
void Test_Calc(void);
void Test_Rot(void);
void Test_Shift(void);
void Test_Sbb(void);
void Test_Xchg(void);
void Test_Movdir(void);
void Test_Smov(void);
void Test_Stz(void);
void Test_Rmpa(void);
void Test_Tst(void);
void Test_Mova(void);
void Test_Jmp(void);
void Test_Smovfb(TINT16 len,TUSIGN8 NEAR* dest,TUSIGN8 HUGE* src);
void Test_Smovfw(TINT16 len,TUSIGN16 NEAR* dest,TUSIGN16 HUGE* src);
void Test_Smovbb(TINT16 len,TUSIGN8 NEAR* dest,TUSIGN8 HUGE* src);
void Test_Smovbw(TINT16 len,TUSIGN16 NEAR* dest,TUSIGN16 HUGE* src);
void Test_Rmpa_b(TINT16 len,TUSIGN8 NEAR* SRC1,TUSIGN8 NEAR* SRC2);
void Test_Rmpa_w(TINT16 len,TUSIGN16 NEAR* SRC1,TUSIGN16 NEAR* SRC2);
void Test_Sstrb(TINT16 len,TUSIGN8 NEAR* dest,TUSIGN8 dat);
void Test_Sstrw(TINT16 len,TUSIGN16 NEAR* dest,TUSIGN16 dat);
void Test_Jsr(void);
#endif
/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_ELETTRONIC_SERVICES_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template>
    //@SubGen end@    <DATACLASS Template>
} T_ELETTRONIC_SERVICES_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

