#ifndef _FUNCTIONS_TYPE_2_H_
#define _FUNCTIONS_TYPE_2_H_

#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
static void nop_instruction(void)
{
	asm("nop");
}

#endif  // #ifndef _FUNCTIONS_TYPE_2_H_
