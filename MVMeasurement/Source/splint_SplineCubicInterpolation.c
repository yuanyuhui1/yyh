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

#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "splint_splineCubicInterpolation.h"

#define NUMERICAL_RECIPES_IN_C__THE_ART_OF_SCIENTIFIC_COMPUTING_ISBN_0_521_43108_5
#ifdef NUMERICAL_RECIPES_IN_C__THE_ART_OF_SCIENTIFIC_COMPUTING_ISBN_0_521_43108_5
//--------------------------------------------------------------------------------------------------
/*!
 \brief     Single Dimension Cubic Spline function with natural end conditions.
            Generates second derivative of y for every value of x
 \author    Ganapathi R
 \date      2008-05-12
 \param

             x   --> Vector  (Should be always in increasing order, ie x[i + 1] > x[i]
             y   --> f(x) , Value, as a function of x. 
             n   --> Size of Vector 'x'
             y2  --> Pointer to to obtain Second Derivative of y for further calculations
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: 
 \n intention: 
 \n result  module test: 
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------

void spline1d(TFLOAT * x, TFLOAT* y, TUSIGN8 n,  TFLOAT* y2)
{
	TINT8 i,k;
	static TFLOAT p,sig,u[MAX_PSBL_1D_SIZE];
	
	
	VIP_ASSERT(MAX_PSBL_1D_SIZE >= n);
	
	// Adjustment factor.
	n = n-1;
	

	y2[0]= 0.0;
	u[0]=0.0;

	// Decomposition loop of the tridiagonal algorithm. y2 and u are used for the temporary storage of the decomposed factors.

	for (i=1; i<=(n-1); i++) 
	{ 
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);                                                 
		p=(sig*y2[i-1])+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=((y[i+1]-y[i])/(x[i+1]-x[i])) - ((y[i]-y[i-1])/(x[i]-x[i-1]));
    		//lint -e{736} Ashwin Herur R 2008-07-15  precision is enough
		u[i]=(((6.0*u[i])/(x[i+1]-x[i-1]))-(sig*u[i-1]))/p;
	}

	
	y2[n]= 0; 
  	//lint -e{734} Ashwin Herur R 2008-07-15  precision is enough
	for (k=n-1;k>=0;k--) // This is the back substitution loop of the tridiagonal algorithm.
	{
		y2[k]=(y2[k]*y2[k+1])+u[k]; 
	}
	//lint -e{818} Ashwin Herur R 2008-07-15 pointer parameter not declared as pointing to const
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Single Dimension Cubic Spline interpolation function.
            Uses second derivative y2a (calculated using spline1d())to interpolate a value y at given x

 \author    Ganapathi
 \date      2008-05-12
 \param

            xa  --> Vector xa, should be in increasing order, ie xa[i + 1] > x[i]
            ya  --> f(xa), function of xa
            y2a--> Second derivative of f(xa) as obtained from function spline
            n   --> Size of vector xa
            x   --> Point at which y has to be interpolated.
            y   --> Pointer to returned value of y interpolated at point x. 

 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: C-SPY, see file "MVMeasurement_execute.c" of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void splineInterpolate1d(TFLOAT* xa, TFLOAT* ya, TFLOAT* y2a, TUSIGN8 n, TFLOAT x, TFLOAT *y)
{

	TUSIGN8 klo,khi,k;
	TFLOAT h,b,a;
	klo=0; 
	khi=n - 1;  // -1 for adjustment.

	// Use bi-section methode to find the right place in the table (search operation)
	while ((khi-klo) > 1) 
	{
		k=(khi+klo) >> 1;
		if (xa[k] > x) 
		{
			khi=k;
		}
		else 
		{
			klo=k;
		}
	} 

	h=xa[khi]-xa[klo];

	VIP_ASSERT(h != 0.0) ; // Make sure that h != 0. Otherwise code runs to "divide by zero" error. 
					//This is taken care by making Xa's distinct. (Always in increasing order)

	a=(xa[khi]-x)/h; 

	b=(x-xa[klo])/h; 

	// Cubic polynomial is now evaluated.
	*y=(a*ya[klo]) + (b*ya[khi]) +  (((( ((a*a*a) - a) * y2a[klo])  + (((b*b*b)-b)*y2a[khi] )) * (h*h)) / 6.0f);
//lint -e{818} Ashwin Herur R 2008-07-15 pointer parameter not declared as pointing to const
}
#else
//------------------------------------------------------------------------------------------------------
void spline1d(TFLOAT *xi, TFLOAT *yi,TUSIGN8 numRows, TFLOAT *dVec)
{
	TINT16 j,i;
	static TFLOAT wk[2][9];
	static TFLOAT tempx, dummy;
	
	j= 1;
	for (i = 0; i < (numRows - 1); i++){
		wk[0][j] = xi[j] - xi[i];
		wk[1][j] = (yi[j] - yi[i])/wk[0][j];
		j++;
	}//End for i

	if (numRows == 2){
		wk[1][0] = wk[0][0] = 1.0;
		dVec[0] = 2.0*wk[1][1];
	}// End if (numRows == 2)
	else { // else numRows > 2
		tempx = dummy = wk[0][1];
		wk[1][0] = wk[0][2];
		wk[0][0] = tempx + wk[0][2];
		dummy *= dummy*wk[1][2];
		dVec[0] = ((tempx + 2.0*wk[0][0])*wk[1][1]*wk[0][2] + dummy)/wk[0][0];
	} // End else numRows > 2

	for (i = 1; i < (numRows - 1); i++){
		tempx = -(wk[0][i+1]/wk[1][i-1]);
		dVec[i] = tempx*dVec[i-1] + 3.0*(wk[0][i]*wk[1][i+1] + wk[0][i+1]*wk[1][i]);
		wk[1][i] = tempx*wk[0][i-1] + 2.0*(wk[0][i] + wk[0][i+1]);
	}//End for i

	if (numRows == 2){
		dVec[1] = wk[1][1];
	} // End if numRows == 2
	else { //else numRows != 2
		if (numRows == 3){
			dVec[2] = 2.0*wk[1][2];
			wk[1][2] = 1.0;
			tempx = -(1.0/wk[1][1]);
		}// End if (numRows == 3)
		else {
			tempx = wk[0][numRows-2] + wk[0][numRows-1];
			dummy = wk[0][numRows-1]*wk[0][numRows-1]*(yi[numRows-2] - yi[numRows-3]);
			dummy /= wk[0][numRows-2];
			dVec[numRows-1] = ((wk[0][numRows-1] + 2.0*tempx)*wk[1][numRows-1]*wk[0][numRows-2] + dummy)/tempx;
			tempx = -(tempx/wk[1][numRows-2]);
			wk[1][numRows-1] = wk[0][numRows-2];
		}//End else

	   	// Complete forward pass of Gauss Elimination
		wk[1][numRows-1] = tempx*wk[0][numRows-2] + wk[1][numRows-1];
		dVec[numRows-1] = (tempx*dVec[numRows-2] + dVec[numRows-1])/wk[1][numRows-1];
 	} // End else numRows != 2

	//Carry out back substitution
	for (i= numRows-2; i >= 0; i--){
		dVec[i] = (dVec[i] - wk[0][i]*dVec[i+1])/wk[1][i];
	}//End for i

	// End of PCHEZ
}

void splineInterpolate1d(TFLOAT*xi, TFLOAT*yi, TFLOAT*dVec, TUSIGN8 numRows, TFLOAT xInt, TFLOAT*yInt)
{
	static TFLOAT h,delta,del1,del2,c2,c3;
	TUSIGN8 ir;
	TFLOAT dummy;
	
	// Begin PCHEV
	// Main loop. Go through and calculate interpolant at each xInt value
	ir = 1;
	
	// Locate all points in interval

	for (ir = 1; ir < numRows; ir++){
		if (xInt <= xi[ir]) break;
	} // End for ir loop

	if (ir == numRows) ir--;

	// Evaluate Cubic at xInt[k], j = jfirst (1) to k-1
	// =========================================================
	// Begin CHFDV
	h = xi[ir] - xi[ir - 1];

	// Compute Cubic Coefficients (expanded about x1)
	delta = (yi[ir] - yi[ir - 1])/h;
	del1 = (dVec[ir - 1] - delta)/h;
	del2 = (dVec[ir] - delta)/h;

	//delta is no longer needed
	c2 = -(del1 + del1 + del2);
	//c2t2 = c2 + c2;
	c3 = (del1 + del2)/h;

	// h, del1, and del2 are no longer needed
	//c3t3 = c3 + c3 + c3;

	// Evaluation loop
	dummy = xInt - xi[ir - 1];
	yInt[0] = yi[ir - 1] + dummy*(dVec[ir - 1] + dummy*(c2 + dummy*c3));
	// Note the redundancy: if either condition is true, other is false

  	// End CHFDV
}
#endif


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Two Dimensional Cubic Spline function.

		Given an m by n tabulated function ya[1..m][1..n], and tabulated independent variables
		x2a[1..n], this routine constructs one-dimensional natural cubic splines of the rows of ya
		and returns the second-derivatives in the array y2a[1..m][1..n]. 
 
 \author    Ganapathi
 \date      2008-01-21
 \param

            x1a  -->  vector x1a, should be in increasing order ie, x1a[i + 1] > x1a[i]
            x2a  -->  vector x2a, should be in increasing order ie, x2a[i + 1] > x2a[i]
            ya    -->  f(x1a, x2a), as a function of x1a and x2a.
            m    -->   size of x1a
            n     -->   size of x2a
            y2a  -->  Pointer to returned value of second derivative of f(x1a, x2a) 
  
 \return
            void
 \warning
 \test
 \test-date:     14-04-2009
 \n by:         Ganapathi R    
 \n environment: C-SPY, see file "MVMeasurement_execute.c" of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug No
*/
//--------------------------------------------------------------------------------------------------
//lint -e{715} Ashwin Herur R 2008-07-15 Symbol 'x1a' not referenced
void spline2d(TFLOAT * x1a, TFLOAT * x2a, TFLOAT *ya, TUSIGN8 m, TUSIGN8 n, TFLOAT *y2a)
{
	TUSIGN8 j;

		
	for (j=0;j<= (m - 1);j++)
	{
		//lint -e{679} Ashwin Herur R 2008-07-15 truncation
		spline1d((TFLOAT*)x2a,(TFLOAT*)&ya[j*n],n,(TFLOAT*)&y2a[j*n]); 
		
	}
	//lint -e{818} Ashwin Herur R 2008-07-15 pointer parameter not declared as pointing to const
} 


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Two Dimensional Cubic Spline interpolation function.
		Given x1a, x2a, ya, m and n; and y2a as produced by spline2d routine; and
		given a desired interpolating point x1,x2; this routine returns an interpolated function value y
		by bicubic spline interpolation.
 
 \author    Ganapathi
 \date      2008-01-21
 \param

            x1a  --> vector x1a, should be in increasing order ie, x1a[i + 1] > x1a[i]
            x2a --> vector x2a, should be in increasing order ie, x2a[i + 1] > x2a[i]
            ya  --> f(x1a, x2a), function of x1a and x2a
            y2a --> second derivative of f(x1a, x2a) as obtained from function spline2d()
            m -->  size of vector x1a
            n   --> size of vector x2a
            x1, x2   --> point at which y has to be interpolated.
            y --> Pointer to returned value of y, interpolated at x1 and x2.

 \return
            void
 \warning
 \test
 \test-date:     14-04-2009
 \n by:         Ganapathi R    
 \n environment: C-SPY, see file "MVMeasurement_execute.c" of module test functions
 \n intention: instruction coverage
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug No
*/
//--------------------------------------------------------------------------------------------------
void splineInterpolate2d(TFLOAT* x1a, TFLOAT* x2a, TFLOAT *ya, TFLOAT *y2a, TUSIGN8 m, TUSIGN8 n, TFLOAT x1, TFLOAT x2, TFLOAT *y)
{
	TUSIGN8 j;
	static TFLOAT ytmp[MAX_PSBL_1D_SIZE];
	static TFLOAT yytmp[MAX_PSBL_1D_SIZE];
  
	VIP_ASSERT(MAX_PSBL_1D_SIZE > n);

	for (j=0;j < m ;j++) // Perform m evaluations of the row spline constructed by function spline2d, using the one dimensional spline evaluator spline1d.
	{
		//lint -e{679} Ashwin Herur R 2008-07-15 truncation
		splineInterpolate1d(x2a, &ya[j * n], &y2a[j * n], n, x2, (TFLOAT*)&yytmp[j]);
	}
	
	// Construct the one dimensional column spline and evaluate it.
	spline1d(x1a,yytmp,m,ytmp);
	splineInterpolate1d(x1a,yytmp,ytmp,m,x1,y);
}