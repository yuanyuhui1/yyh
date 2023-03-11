//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Flow
 Module
 Description    Cubic Spline Interpolation

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef _SPLINT_CUBIC_INTERPOLATION_
#define _SPLINT_CUBIC_INTERPOLATION_

#define MAX_PSBL_1D_SIZE 63
#define MAX_M_P 9
#define MAX_N_T 7
//#define NUMERICAL_RECIPES_IN_C__THE_ART_OF_SCIENTIFIC_COMPUTING_ISBN_0_521_43108_5

#ifdef NUMERICAL_RECIPES_IN_C__THE_ART_OF_SCIENTIFIC_COMPUTING_ISBN_0_521_43108_5
	extern void spline1d(TFLOAT * x, TFLOAT* y, TUSIGN8 n,  TFLOAT* y2);
	extern void splineInterpolate1d(TFLOAT* xa, TFLOAT* ya, TFLOAT* y2a, TUSIGN8 n, TFLOAT x, TFLOAT *y);
#else
//------------------------------------------------------------------------------------------------------
	extern void spline1d(TFLOAT *xi, TFLOAT*yi, TUSIGN8 numRows, TFLOAT*dVec);
	extern void splineInterpolate1d(TFLOAT*xi, TFLOAT*yi, TFLOAT*dVec, TUSIGN8 numRows, TFLOAT xInt, TFLOAT*yInt);
#endif

	extern void spline2d(TFLOAT * x1a, TFLOAT * x2a, TFLOAT *ya, TUSIGN8 m, TUSIGN8 n, TFLOAT *y2a);
	extern void splineInterpolate2d(TFLOAT* x1a, TFLOAT* x2a, TFLOAT *ya, TFLOAT *y2a, TUSIGN8 m, TUSIGN8 n, TFLOAT x1, TFLOAT x2, TFLOAT *y);
#endif