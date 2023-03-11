/*--------------------------------------------------------------------------------------------------*/
/*
 Copyright              Copyright ABB, 2010.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 IAPWS IF97
 Description            Formulation of IAPWS IF97 
*/
/*--------------------------------------------------------------------------------------------------*/
#include <math.h>
#include "System/Interface/common_type.h"
#include "IAPWS_IF97.h"
//lint -e736
//lint -e747

struct csub2_1_ {
    TFLOAT tc;
	TFLOAT pc;
	TFLOAT dc;
} csub2_1={
647.096, 
22.064, 
322.0  
};

//-------------------------------------------------------------------------------------------------
/*!
 \brief       Find the regions shall be for given   T and P .
 \author      zuochen wang
\date 2012-9-12
\param
	t -  Pointer to  input Temperature
	p - Pointer to input Real pressure
	pIreg - Pointer to Output Region
\return
    run state .
\warning
\test
	test-date: 2013-06-28
	\n by: zuochen wang
	\n environment: C-SPY
\n intention:
\n result module test: Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
int regsopt_(const TFLOAT *p, const TFLOAT *t, const TINT16 *pIreg)
{
    /* System generated locals */
    TFLOAT d__1, d__2;
    TINT16 *ireg =(TINT16 *) pIreg;
    /* Local variables */
    static TFLOAT pold = 1.0f, told = 273.15f, pg;
    static TINT16 iregold = 0;

/* ***********************************************************************
 */
    d__1 = *t - told;
    d__2 = *p - pold;
	//lint -e{960}
    if ((fabs(d__1) < (1e-6)) && (fabs(d__2) < (1e-6)))
    {
		*ireg = iregold;
    }
    else 
    {
      if ((*t < 273.15f) || (*t > 2273.15f) || (*p > 100.0f) || (*p < 5e-4))
      {
        *ireg = 0;
      }
      else
      {
        *ireg = 0;
        if (*t <= 623.15f) 
        {
            pg = psattn_(t);
            if (*p >= pg) 
            {
                *ireg = 1;
            }
            else 
            {
                *ireg = 2;
            }
        }
        else if (*t <= 863.15f)
        {
            pg = fb23_(t);
            if (*p > pg)
            {
                *ireg = 3;
            }
            else 
            {
                *ireg = 2;
            }
        }
        else if (*t <= 1073.15f) 
        {
            *ireg = 2;
        }
        else 
        {
            if (*p <= 10.0f)
            {
                *ireg = 5;
            }
            else 
            {
                *ireg = 0;
            }
        }      
        
      }  
    }
    told = *t;
    pold = *p;
    iregold = *ireg;
    return 1;
}

/* ********************************************************************* */

/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */
/*  Argument is temperature in K. */
/*  Returns vapour pressure in MPa */

TFLOAT fb23_(const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;
    TFLOAT tinput=*t;

    ret_val = (((tinput* 0.0010192970039326) - 1.1671859879975) *tinput) +  348.05185628969;

    return ret_val;
} /* fb23_ */



/* ********************************************************************* */

/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  Argument is temperature in K. */
/*  Returns vapour pressure in MPa */

TFLOAT psattn_(const TFLOAT *ts)
{
    /* System generated locals */
    TFLOAT ret_val, d__1;

    /* Builtin functions */
    TFLOAT tsinput=*ts;
    /* Local variables */
    static TFLOAT b, c, y, ps;

    y = tsinput + (-.23855557567849 / (tsinput - 650.17534844798));
    b = (y * ((y * -17.073846940092) + 12020.82470247)) - 3232555.0322333;
    c = (y * ((y * 29.83021722706) - 9646.5314723182)) + 810226.81084113999;
/* Computing 2nd power */
    d__1 = b;
    ps = c / (sqrt((d__1 * d__1) - (((((y + 1167.0521452767) * y) - 724213.16703206) * c) * 2.)) - b);
/* Computing 2nd power */
    d__1 = ps;
    ps = d__1 * d__1;
/* Computing 2nd power */
    d__1 = ps;
    ret_val = d__1 * d__1;
    return ret_val;
} /* psattn_ */



/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is pressure in MPa. */
/*  Second argument is temperature in K. */
/*  Returns specific volume in m^3/kg */

TFLOAT vpt1n_(const TFLOAT *p,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT targinv2, targinv3, targinv4, targinv7, targinv8, parg, 
	    targ, parg2, parg3, targ2, parg6, targ4, targ8, pt, targinv;


    targ = (1386. / (*t)) - 1.222;
    targ2 = targ * targ;
    targ4 = targ2 * targ2;
    targ8 = targ4 * targ4;
    targinv = 1. / targ;
    targinv2 = targinv * targinv;
    targinv3 = targinv2 * targinv;
    targinv4 = targinv3 * targinv;
    targinv7 = targinv4 * targinv3;
    targinv8 = targinv7 * targinv;

    parg = 7.1 - ((*p) * .060496067755596);
    parg2 = parg * parg;
    parg3 = parg2 * parg;
    parg6 = parg3 * parg3;

    pt = parg * targinv;
    //lint -e{961}
    ret_val = ((((((((((((((pt * 8.3571296309235e-29 - 1.577706757248e-27) * 
	    pt + 1.0007510864939e-26) * pt - 2.1323943802988e-26) * parg6 * 
	    targinv7 - 9.2975593753127e-24) * parg2 * targinv2 + 
	    4.0317346616717e-22) * parg6 * parg6 * parg * targinv8 * targinv8 
	    * targinv2 + 2.844385406225e-13) * targinv * targinv4 + 
	    3.8920900760265e-14) * parg3 * targinv + targinv3 * 
	    5.6562757086698e-11) * parg + 2.5045010666356e-10) * targinv3 + 
	    7.278454644432e-11) * targinv2 + targ8 * targ2 * 
	    1.6017135514411e-17) * parg + targ4 * targ2 * 7.1369114266351e-14 
	    + 2.368005138107e-10 + targinv4 * 2.6535353478691e-9) * parg + (
	    targ8 * targ8 * 4.0593624756496e-20 + targ2 * 2.4649255061299e-10 
	    - 2.6614606756583e-9) * targ + 1.6753299313106e-8 + targinv3 * 
	    2.6348204437581e-8) * parg + (targ2 * 1.4752738052287e-9 + 
	    6.0983184277677e-7) * targ + 9.0824711621636e-7 + (targinv2 * 
	    -7.9068310787773e-9 + 1.6949507886565e-8) * targinv7 + targinv * 
	    5.3021235478366e-7) * *t;

    return ret_val;
} /* vpt1n_ */


/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is pressure in MPa. */
/*  Second argument is temperature in K. */
/*  Returns specific volume in m^3/kg */

TFLOAT vpt2n_(const TFLOAT *p,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT targ, targ2, targ3, targ4, targ7, targ9, targ10, targ20,
	     targ13, targ14, targ19, targ29, pi, pi2, pi4;

    targ = (540. / (*t)) - .5;
    targ2 = targ * targ;
    targ3 = targ2 * targ;
    targ4 = targ3 * targ;
    targ7 = targ4 * targ3;
    targ9 = targ7 * targ2;
    targ10 = targ9 * targ;
    targ13 = targ10 * targ3;
    targ14 = targ13 * targ;
    targ19 = targ10 * targ9;
    targ20 = targ19 * targ;
    targ29 = targ20 * targ9;

    pi = *p;
    pi2 = pi * pi;
    pi4 = pi2 * pi2;
	
    //lint -e{961}
    ret_val = ((((((((((((((((targ14 * targ4 * -1.045297764101e-8 + 
	    6.1380796600759e-19) * targ14 + 8.095639816199e-31) * pi2 * targ4 
	    * targ2 + (pi * -1.3554003516532e-17 + targ14 * 
	    3.8407863613346e-8) * targ19) * pi + targ * -5.7237375629058e-28) 
	    * pi + (targ13 * -3.8770461813767e-8 + 2.827245220124e-15) * 
	    targ14 * targ + 8.2323172927012e-27) * pi4 + (targ7 * pi2 * 
	    -.0027964806945291 + 7.8961795698818e-4) * targ20 * targ10 + 
	    targ9 * -5.9727304473934e-13) * pi4 * pi2 * targ20 + targ14 * 
	    -4.6236502563083e-12 + targ10 * -4.7236018881958e-16 + targ4 * 
	    4.8030850255907e-22) * pi + targ13 * 8.2284277596267e-11) * pi + (
	    targ29 * -.030391059135431 + targ * 4.1560273634777e-14) * targ7) 
	    * pi + (targ14 * -1.2582486281398e-4 - 4.0777050817765e-9) * 
	    targ10 * targ - 1.9080267139026e-20) * pi + (targ19 * 
	    -.066171037151276 - 5.8627109607515e-6) * targ14 * targ2 + targ3 *
	     -4.6285795806554e-14) * pi + targ7 * 5.2895671019076e-9) * pi + 
	    targ3 * 8.9029053477126e-10 + targ2 * 2.3612995389975e-11 + targ *
	     -1.4556033356679e-12) * pi + ((targ29 * -5.6308369181266e-5 - 
	    2.0815641174718e-6) * targ3 - 4.4690961795625e-8) * targ3 + targ *
	     6.0742360767348e-10 + 2.8358563410542e-11) * pi + (((targ29 * 
	    -2.4621994801194e-8 - 4.0427181344853e-5) * targ3 - 
	    3.6361581820033e-6) * targ2 - 1.749090082491e-7) * targ2 + targ * 
	    -3.0490845958965e-8) * pi + (targ3 * -2.3226424590187e-5 - 
	    2.657524817974e-5) * targ3 + targ2 * -2.1228356217229e-5 + targ * 
	    -8.2312526543429e-6 + 4.61526e-4 / pi - 8.1836601766921e-7) * *t;

    return ret_val;
} /* vpt2n_ */



/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is pressure in MPa. */
/*  Second argument is temperature in K. */
/*  Returns specific volume in m^3/kg */
TFLOAT vpt3n_(const TFLOAT *p,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val, d__1, d__2;

    /* Local variables */
    static TFLOAT vpt3nold = 0.1, pold =0.1, dest, told = 293.15;
	static TINT16 ireg3 = 0x01;
    static TFLOAT eps;
    d__1 = *p - pold;
    d__2 = *t - told;
	//lint -e{960}
    if ( (fabs(d__1) < 1e-6) && ( (fabs(d__2)) < 1e-6))
    {
		ret_val = vpt3nold;
    }
    else
    {
      eps = 1e-6;
      (void)reg3s_(p, t, &ireg3);
      dest = 1.0 / vest3_(p, t, &ireg3);
      ret_val = 1.0 / diter3_(p, t, &dest, &eps);
    }

    pold = *p;
    told = *t;
    vpt3nold = ret_val;

    return ret_val;
} /* vpt3n_ */


/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */
// lowlevel funciton used by  vpt3n_
/**************************************************************************/
TFLOAT diter3_(  const  TFLOAT *p,   const  TFLOAT *t,    const  TFLOAT *pDest,    const  TFLOAT *eps)
{
    /* System generated locals */
    TFLOAT ret_val;
    TFLOAT *dest = (TFLOAT *)pDest;

    /* Local variables */
	static TFLOAT x = 1.0, d1, d2;
    static TINT16 ix = 1;
    static TFLOAT d1v, d2v;

	d1v = (*dest) * 0.999;
	d2v = (*dest) * 1.001;
	if (*dest <= csub2_1.dc) //322.0
	{
		d1 = d1v;
		if (d2v <= csub2_1.dc)
		{
			d2 = d2v;
		}
		else
		{
			d2 = csub2_1.dc;
		}
	}
	else 
	{
		d2 = d2v;
		if (d1v >= csub2_1.dc) 
		{
			d1 = d1v;
		}
		else 
		{
			d1 = csub2_1.dc;
		}
	}
	
	(void)wnpt3_(&d1, &d2, nullp3n_, p, t, eps, &x, &ix);
	
	if (ix <= 0)
	{
		ret_val = x;
	}
	else
	{
		ret_val = 0.;
	}
    return ret_val;
} /* diter3_ */



/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */
// lowlevel funciton used by  vpt3n_
/**************************************************************************/
TFLOAT nullp3n_(const TFLOAT *d,const TFLOAT *t,const TFLOAT *p)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT v;

/* ***********************************************************************
 */


    v = 1.0 / *d;
    ret_val = pvt3n_(&v, t) - *p;

    return ret_val;
} /* nullp3n_ */

/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is specific volume in m^3/kg. */
/*  Second argument is temperature in K. */
/*  Returns pressure in MPa. */

TFLOAT pvt3n_(const TFLOAT *v,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT d, dn, tn, dn2, tn2, tn4, tn8;

    d = 1.0 / *v;
    dn = d * 0.0031055900621118;
    dn2 = dn * dn;

    tn = 647.096 / *t;
    tn2 = tn * tn;
    tn4 = tn2 * tn2;
    tn8 = tn4 * tn4;

    //lint -e{961}
    ret_val = (((((((((((((dn2 * -7.0828888764732e-10 + 4.1677824835579e-8) * 
	    dn - 1.7009779063714e-7) * dn2 + 1.4134974870137e-6) * dn - 
	    3.6457015185726e-6) * dn + 4.0432228943229e-6) * dn - 
	    2.0465970175711e-6) * dn + 3.6000198797746e-7) * tn4 + 
	    1.1300374350117e-7) * dn * tn * tn4 - 9.2029667804989e-7) * tn2 + 
	    1.2687814703698e-6) * tn * tn8 - 1.651805539561e-6) * tn4 - 
	    1.8137563991203e-6) * tn2 + ((((((((tn * -2.373229728287e-9 + 
	    1.1604770704233e-9) * dn + tn2 * 7.4719422939563e-9) * dn2 + tn2 *
	     -1.3548168211001e-7) * dn + tn2 * 8.1062958396863e-7 - 
	    1.9070541732963e-7) * dn + (tn2 * -2.3587721876106e-6 + 
	    7.7187447366156e-7) * tn) * dn + (tn2 * 5.192716212546e-6 - 
	    2.5499417810788e-6) * tn2 + 2.5217135348393e-7) * dn + ((tn4 * 
	    tn8 * -3.5322922727656e-8 - 8.6815255166165e-6) * tn2 + 
	    5.976818847293e-6) * tn2 - 1.2039520470677e-6) * dn + ((tn * 
	    -8.7439446783466e-6 + 1.4038497646519e-5) * tn4 - 
	    2.4427828388075e-6) * tn2 + 1.1034616587648e-6) * dn + *v * 
	    4.9189764279794e-4) * *t * d * d;

    return ret_val;
} /* pvt3n_ */



/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */
// lowlevel funciton used by  vpt3n_
/**************************************************************************/
int wnpt3_(const TFLOAT *xa,const TFLOAT *xb,TFLOAT (*f) (),const TFLOAT * p,const TFLOAT *t,const TFLOAT *eps,const TFLOAT *px,const TINT16 *pix)
{
    /* System generated locals */
    TFLOAT d__1;

    /* Local variables */
    static TINT16 irem, i;
    static TFLOAT f1, f2, f3, p1, p3, x1, x2, x3;
    TFLOAT *x = (TFLOAT *)px;
	TINT16 *ix = (TINT16 *)pix;

    x1 = *xa;
	//lint -e{746} funtion pointer 
    f1 = (*f)(&x1, t, p);
    x3 = *xb;
	//lint -e{746} funtion pointer 
    f3 = (*f)(&x3, t, p);
    *ix = 0;

    for (i = 1; i <= 40; ++i)
    {
	  //lint -e{777}
		if (f1 != f3) 
		{
			(*x) = x1 + ((x3 - x1) * (f1 / (f1 - f3)));
		} 
		else
		{
			break; 
		}
        
		if (*x < 0.0f)
		{
			*x = (x1 + x3) / 2.0; 
		}
		if (fabs(*x) < 1e-8) 
		{
			d__1 = *x - x1;  
			if (fabs(d__1) < *eps) 
			{
				return 0;  
			}
		}
		else
		{
			d__1 = (*x - x1) / *x;
			if ( fabs(d__1) < *eps)
			{
				return 0;
			}
		}
        
		//lint -e{746}
		f2 = (*f)(x, t, p);
		x2 = x1 - ((x1 - x3) / 2.0);
		p1 = f2 * f1;
		p3 = f2 * f3;
		if ((p1 < 0.0f) && (p3 < 0.0f)) 
		{
			p1 = fabs(p1);
			p3 = fabs(p3);
		}
        
		if (p1 <= p3) 
		{
			x3 = x1;
			f3 = f1;
		}
		x1 = *x;
		f1 = f2;
			
		if (((x2 - x3) * (x2 - x1)) < 0.0f)
		{
			d__1 = f1 / f3;
			if ( fabs(d__1) <= 0.3)
			{
				d__1 = f1 / f3;
				*x = x1 +  ((fabs(d__1) * 1.5) * (x3 - x1));
				irem = 1;
			} 
//			else if ( fabs(d__1) >= 3.0) { irem = 2;}
			else 
			{
				*x = (x1 + x3) / 2.0;   
				irem = 0;
			}
			
			//lint -e{746}
			f2 = (*f)(x, t, p);
			if ((((f2 * f1) >= 0.0 ) && ( irem == 1)) || (((f2 * f3) >= 0.0) && (irem == 2)))
			{
				*x = (x1 + x3) / 2.;   
				//lint -e{746}
				f2 = (*f)(x, t, p);  
			}
			if ((f2 * f1) <= (f2 * f3))
			{
				x3 = x1;
				f3 = f1;
			}
			x1 = *x;
			f1 = f2;	   
		}
      
    }
    *ix = 1;  
    return 0;
}


/* *********************************************************************** */
int reg3s_(const TFLOAT *p,const TFLOAT *t,const TINT16 *pireg3)
{
	static TFLOAT pg, ts;
	int returnVal =0; 
	TINT16 *ireg3 =  (TINT16 *)pireg3;
	if (*p <= 40.0f)
	{
		if (*p <= csub2_1.pc)
		{
			ts = tsatpn_(p);
			if (*t < ts) 
			{
				*ireg3 = 1;
			}
			else
			{
				*ireg3 = 4;
			}
		}
		else 
		{
			pg = csub2_1.pc + (((*t) - csub2_1.tc) / 3.727888);
			if (*p <= 24.0f)
			{
				if (*t <= csub2_1.tc)
				{
					*ireg3 = 1;
				} 
				else if (*p > pg)
				{
					*ireg3 = 2;
				}
				else 
				{
					*ireg3 = 3;
				}
			} 
			else if (*p > pg)
			{
				*ireg3 = 5;
			}
			else
			{
				*ireg3 = 6;
			}
		}
	}
	else
	{
		*ireg3 = 7;
	}
	returnVal=1;
	return  returnVal;

} /* reg3s_ */


/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  Argument is pressure in MPa. */
/*  Returns saturation temperature in K */

TFLOAT tsatpn_(const TFLOAT *ps)
{
    /* System generated locals */
    TFLOAT ret_val, d__1;

    static TFLOAT f, g, y, t1;

    y = sqrt(sqrt(*ps));

    f = (y * ((y * 1167.0521452767) + 12020.82470247)) - 4823.2657361591;
    g = (y * ((y * 724213.16703205998) + 3232555.0322333002)) - 405113.40542056999;
/* Computing 2nd power */
    d__1 = f;
	//lint -e{961}
    t1 = g / (f + sqrt(d__1 * d__1 + ((y - 17.073846940092) * y + 
	    14.91510861353) * g * 4.)) + 325.08767422399001;
    ret_val = t1 - sqrt(((t1 - 1300.35069689596) * t1) + 422728.22228502791);
    return ret_val;
} /* tsatpn_ */


/* *********************************************************************** */
TFLOAT vest3_(const TFLOAT *p,const TFLOAT *t,const TINT16 *ireg3)
{
    /* System generated locals */
    TFLOAT ret_val = 0.0;

    /* Local variables */
    static TFLOAT vpt3n, p2, p4, p5, t2, t3, t4, t6, z1, z2, z3, z4, z8, 
	    pr, tr, p2t2;


    switch ((int)*ireg3)
    {
      case 1:  
        pr = *p - csub2_1.pc;
        tr = *t - csub2_1.tc;
        z8 = (((pr * 0.05) + 3.727888) * pr) - ((1.0 - (tr * 0.05)) * tr);
       /* to avoid negative roots outside of range of validity */
        if (z8 >= 0.0f)
        {
			z4 = sqrt(z8);
			z2 = sqrt(z4);
			z1 = sqrt(z2);
			
			//lint -e{961}
			vpt3n = z1 * -1.18427102787648e-4 + z4 * (z1 * (z1 * (z2 * (z4 * 
				-4.55907876060086e-5 + .00153431224177324) - .0106812857513379) + 
				.0172887471616688) - .00876842774061156) + tr * (z1 * (z1 * (z1 * 
				(z1 * (z1 * -.00490643103428227 + .0263271308078056) - 
				.0564778221803544) + .0608359392259313) - .0332156380721166) + 
				.0074887444872508 + tr * (z1 * (z1 * (z1 * (tr * z1 * z4 * (tr * 
				z4 * -2.51644090693395e-12 - 3.83218936572771e-9) - 
				1.61628813019544e-4) + 6.17493752916276e-4) - 7.97144480049465e-4)
				 + 3.48351238844209e-4));
			ret_val = vpt3n + .003105590062111801;
        }

        break;
  
      case 2: //region 3b (t>tc, v<~vc, p<24 MPa) 
          pr = *p - csub2_1.pc;
          p2 = pr * pr;
          p4 = p2 * p2;
          tr = *t - csub2_1.tc;
          t2 = tr * tr;
          z8 = (((pr * 0.02) + 3.727888) * pr) - ((1.0 - (tr * 0.02)) * tr);
          /* to avoid negative roots outside of range of validity */
          if (z8 >= 0.0f)
          {
			z4 = sqrt(z8);
			z2 = sqrt(z4);
			z1 = sqrt(z2);    
			
			//lint -e{961}
			vpt3n = p2 * 0.413013399556249 + t2 * (pr * 0.00135336193587336 - 
				0.0487687959756292) + z1 * (pr * 0.0178357790645547 + p2 * 
				-0.585398228522495 + tr * (p4 * 5.94938907295817e-5 + tr * (tr * (
				t2 * pr * 3.95174104863528e-8 - 1.41140213612559e-4) +
				//lint -e{834}															
				0.122945704230431)) + z1 * (t2 * (-0.119057019174713) + z1 * ((pr *(-0.0760561375637742)) - 0.00194570794816001 + (p2 * 0.299546836134716)
				+ t2 * 0.0427529172119353 + z1 * (tr * 0.013939165722125 + z1 * (tr * 
				pr * -0.0278468126229603 + z1 * (z2 * (p2 * (pr * 
				1.25685384163741e-4 + t2 * 3.75831122504734e-5) + z2 * (tr * 
				-0.00762583183784896 + t2 * p4 * pr * -9.38937201479048e-7 + z2 * (
				pr * -0.0291792573898258 + 0.0134876477124427 + tr * (t2 * p2 * (p4 
				* (t2 * -2.39512088942747e-10 + 4.54845318075243e-8) - 
				1.36017045350123e-6) + 0.00634557820845727)))) + 
				0.00195041855369227))))));
			ret_val = vpt3n + 0.003105590062111801;
          }
        break;
        
      case 3: //region 3c (v>~vc, pc<p<24 MPa)
          pr = *p - csub2_1.pc;
          p2 = pr * pr;
          tr = *t - csub2_1.tc;
          t2 = tr * tr;
          t3 = t2 * tr;
          t4 = t2 * t2;
          z8 = (((pr * 0.2) - 3.727888) * pr) + (((tr * 0.02) + 1.0) * tr);
      /* to avoid negative roots outside of range of validity */
          if (z8 >= 0.0f) 
          {
			  z4 = sqrt(z8);
			  z2 = sqrt(z4);
			  z1 = sqrt(z2);
			  
		      //lint -e{961}
			  vpt3n = t2 * -0.0151226672652382 + t3 * 2.19075237766159e-4 + t4 * 
					//lint -e{834}															
					  1.14144924756274e-5 + pr * (t3 * 1.05510662596481e-4 - 
					  0.00470815410341398 + pr * (p2 * 4.24198281757227e-4 + 
					  0.487932009131791)) + z1 * (t2 * 0.0305426466180436 + t4 * (tr * 
					  5.09486478795057e-7 - 1.74690467895005e-5) + p2 * 
					  -1.4370899198291 + z1 * (pr * t3 * -5.26750160303121e-5 + p2 * (
					  t4 * 1.04163340234817e-7 + 1.22116000890535) + z1 * (t2 * 
					  -0.0152017319222412 + 0.00218460997189951 + pr * (tr * 
					  0.0165284534427183 - 0.0294106550573793) + p2 * (pr * 
					  -0.00212663865441498 - 0.335234582911578) + z2 * (tr * 
					  0.0753543651141502 + z1 * (tr * -0.0769927079971342 + pr * 
					  -0.0247039860992736 + z2 * (p2 * -0.00223745526548978 + z2 * (tr * (
					  t3 * -1.63238458626065e-7 + 0.0122241653100711 + z2 * (t4 * (tr * 
					  -4.19492118766324e-12 + 5.80172883857322e-10) + p2 * (tr * 
					  -6.60478916724586e-7 + pr * -1.1941903809545e-6))) - 
					  0.0113782324304171)))))));
			  ret_val = vpt3n + 0.003105590062111801; 
          }
          break;
      case 4: 
          pr = *p - csub2_1.pc;
          tr = *t - csub2_1.tc;
          t2 = tr * tr;
          z8 = (((pr * 0.5) - 3.727888) * pr) + (((tr * 0.01) + 1.0) * tr);
      /* to avoid negative roots outside of range of validity */
          if (z8 >= 0.0f)
          {
              z4 = sqrt(z8);
              z2 = sqrt(z4);
              z1 = sqrt(z2);
			  
			  //lint -e{961}
			  vpt3n = z2 * (z1 * (z1 * -7.92816174523651e-5 + 7.13823865906627e-4) + 
					  6.43907516287541e-4) + tr * (z1 * (z1 * (z1 * (z1 * (z1 * 
					  -.0185997001981635 + .118639803204261) - .303433314767063) + 
					  .389907326497054) - .252680942664202) + .0664933791260178 + t2 * (
					  z1 * (t2 * 2.56374705633001e-11 - 3.92289014485859e-7) + 
					  5.71985898251817e-7)) + pr * (z1 * (z1 * (z1 * (z1 * (z1 * 
					  .0785791469799311 - .496046407723234) + 1.25833184484538) - 
					  1.60832918484839) + 1.04023037508896) - .274711031859796);
			  ret_val = vpt3n + .003105590062111801;   
          }
          break;
        
      case 5:  
          pr = *p - csub2_1.pc;
          p2 = pr * pr;
          p4 = p2 * p2;
          p5 = p4 * pr;
          tr = *t - csub2_1.tc;
          t2 = tr * tr;
          t3 = t2 * tr;
          z8 = (((pr * 0.02) + 3.727888) * pr) + (((tr * 0.005) - 1.0) * tr);
      /* to avoid negative roots outside of range of validity */
          if (z8 >= 0.0f) 
          {
              z4 = sqrt(z8);
              z2 = sqrt(z4);
              z1 = sqrt(z2);
			  
			  //lint -e{961}
			  vpt3n = pr * -0.112702460700323 + tr * 0.0301448430808593 + t2 * (t2 * t2 * 
					  p5 * -8.77499173794533e-21 - 3.35916193888413e-4) + z1 * (t2 * 
					  1.8715253091771e-4 + z1 * (pr * 0.00356911241995526 + t2 * p5 * 
					 //lint -e{834}															
					  -4.18986486825425e-12 + z1 * (tr * p4 * -2.89296115834392e-8 + t3 
					  * (tr * p2 * pr * -8.1713295796082e-14 + 3.2272667481884e-7 + t2 *
					   -1.20527379090314e-11) + z1 * (p5 * -5.19162117274822e-8 + t3 * (
					  tr * -8.13610292866497e-10 - 1.94260348835721e-7 + t3 * 
					  3.91792707971363e-14) + z1 * (p4 * (tr * 2.00848495263495e-8 - 
					  5.38255397523665e-7) + z1 * (tr * (pr * -3.70031971083042e-4 - 
					  0.00313609300142694 + p5 * -1.69209023050985e-10 + tr * (p2 * 
					  7.59164825488741e-8 + 2.60485012629641e-5)) - 0.00721941889977446 
					  + z2 * (pr * (pr * 7.664258159241e-4 + 0.0127931680641201 + p2 * 
					  2.25545818343096e-5 + tr * (pr * -4.3128288517517e-6 + p2 * 
					  4.88412051812406e-8 + tr * (p2 * -3.56918730340587e-10 + 
					  1.91456960283807e-7))) + .0396910783770869 + z2 * (pr * 
					  -0.00550701260682904 + tr * (p5 * pr * 1.3349192675752e-13 + 
					  3.03625591962761e-4 + tr * (p4 * 2.52719052207758e-12 + t3 * 
					  1.2453606765958e-13)) + z2 * (p2 * -9.58920650759504e-5 - 
					  0.00607043036830454 + p4 * 1.02650393925122e-8 + tr * pr * (tr * 
					  p5 * -1.55995837253683e-16 + 2.87671539256386e-6))))))))));
			  ret_val = vpt3n + 0.003105590062111801;   
          }
          break;
        
      case 6://region 3f (v>~vc, 24 MPa<p<40 MPa)  
          pr = *p - csub2_1.pc;
          p2 = pr * pr;
          tr = *t - csub2_1.tc;
          z4 = (((pr * 0.1) - 3.727888) * pr) + (((tr * 0.002) + 1.0) * tr);
      /* to avoid negative roots outside of range of validity */
          if (z4 >= 0.0)
          {
              z2 = sqrt(z4);
              z1 = sqrt(z2);
			  z3 = z1 * z2;
			  
			 //lint -e{961,834}
			  vpt3n = z3 * (z2 * (p2 * -1.15529419916092e-7 - 0.00214747822942469 + z1 * 
					  7.85448130272979e-4) + .00321621088787326) + tr * (z1 * (z1 * (z1 
					  * (z2 * (z1 * p2 * pr * 1.84266268449228e-11 - 
					  6.35909964040088e-5) + 4.39053317238974e-4) - 2.45865969502991e-4)
					   - 4.07113420864393e-4) + 3.29890852403526e-5 + tr * (z1 * (z3 * 
					  2.62763491213355e-6 - 3.57933830043717e-5) + 3.7978195570912e-5 + 
					  tr * (z3 * -5.87768799763869e-8 + 2.52360667718127e-7 + tr * z2 * 
					  (z4 * p2 * -2.59174703895765e-16 + 5.25013249033141e-10))));
			  ret_val = vpt3n + .003105590062111801;
          }
          break;
      case 7: 
        pr = ((*p) * 0.0097087378640776691) - 1.0;
        p2 = pr * pr;
        p4 = p2 * p2;
        tr = ((*t) * 0.0016666666666666668) - 1.0;
        t2 = tr * tr;
        t3 = t2 * tr;
        t4 = t2 * t2;
        t6 = t3 * t3;
        p2t2 = p2 * t2;
		
       //lint -e{961}
       vpt3n = tr * (t2 * (t3 * -0.0254795720214314 + 0.010336719420718) + 
                0.00130245527248376) + 0.00125537070183712 + pr * (t2 * 
				//lint -e{834}															
                (-0.00960082399513164) - 1.85955960512067e-4 + pr * (tr * t4 * 
                0.474944869074855 + pr * (tr * (t3 * (t4 * 30.7310678953686 - 
                0.687909934564732) - 0.00503420527214133) + p2 * (p2 * pr * (t6 * 
                654.223452156635 + p2 * t4 * (t6 * -320317.604761443 + p4 * (p2 * 
                t6 * (t6 * t6 * 11416042249924.4 + p4 * p4 * (p2t2 * (p2t2 * (
                p2t2 * (p2t2 * -1.83657231751509e17 + 11950283625768800.0) - 
                281900719117892.) + 2783392577409.6) - 9156698413.1259)) + 
                197.974246206705))) - 1.68658389645091e-4))));
        ret_val = vpt3n;
      break;
    default :
      break;
              
    }
    return ret_val;
}


/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is pressure in MPa. */
/*  Second argument is temperature in K. */
/*  Returns specific enthalpy in kJ/kg */

TFLOAT hpt1n_(const TFLOAT *p,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT targinv2, targinv4, targinv8, targinv9, parg, targ, 
	    parg2, parg4, targ2, targ4, parg8, targ8, pt, targinv;

    targ = (1386. / (*t)) - 1.222;
    targ2 = targ * targ;
    targ4 = targ2 * targ2;
    targ8 = targ4 * targ4;
    targinv = 1.0 / targ;
    targinv2 = targinv * targinv;
    targinv4 = targinv2 * targinv2;
    targinv8 = targinv4 * targinv4;
    targinv9 = targinv8 * targinv;

    parg = 7.1 - ((*p) * 0.060496067755596);
    parg2 = parg * parg;
    parg4 = parg2 * parg2;
    parg8 = parg4 * parg4;

    pt = parg * targinv;

        //lint -e{961}
		ret_val = ((((((((((((pt * 2.4531669269267e-21 - 4.6640228230284e-20) * 
	    pt + 2.9806124175367e-19) * pt - 6.4016099916299e-19) * parg4 * 
	    parg2 * targinv9 + targinv2 * -2.8710377452428e-16) * parg2 + 
	    1.2755771455453e-14) * parg8 * parg4 * parg * targinv8 * targinv8 
	    * targinv8 + targinv * 6.6877530790508e-7) * targinv2 + targinv8 *
	     8.9603964175208e-6) * parg2 * parg * targinv4 + targinv9 * 
	    0.0020734169140086) * parg + targinv2 * (targinv * 
	    8.3376808703816e-4 + targinv4 * 0.0071724465059049) + targ8 * targ 
	    * -9.1740466143441e-10) * parg + targ4 * targ * 
	    -3.2702156038568e-6 + targinv4 * targinv * 0.08105871182691) * 
	    parg + targ8 * targ8 * -7.9051996435264e-12 + targ2 * 
	    -0.0084709309503347 + targinv4 * 0.90547896843533 + 
	    0.030487803863262) * parg + targinv8 * (targinv2 * 
	    -1.6303507737913 + 2.7182613947704) + targ2 * -0.10139813560979 + 
	    targinv2 * 12.147472571259 - 13.971601220485) * parg + (targ2 * 
	    2.5975485679232 + targ * -42.516429081128 + 302.66937911228) * 
	    targ2 + targinv2 * (targinv * -187.20692775139 + 540.83364671138) 
	    + targ * -1225.514552573 + 2165.6306556572;

    return ret_val;
} /* hpt1n_ */




/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is pressure in MPa. */
/*  Second argument is temperature in K. */
/*  Returns specific enthalpy in kJ/kg */

TFLOAT hpt2n_(const TFLOAT *p,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT targ, targ2, targ3, targ4, targ6, targ7, targ9, targ10, 
	    targ20, targ13, targ14, targ15, targ19, targ29, ti, pi, pi2, pi4, 
	    tau;

    tau = 540.0 / *t;
    ti = *t * 0.0018518518518518;

    targ = tau - 0.5;
    targ2 = targ * targ;
    targ3 = targ2 * targ;
    targ4 = targ3 * targ;
    targ6 = targ4 * targ2;
    targ7 = targ6 * targ;
    targ9 = targ7 * targ2;
    targ10 = targ9 * targ;
    targ13 = targ10 * targ3;
    targ14 = targ13 * targ;
    targ15 = targ14 * targ;
    targ19 = targ15 * targ4;
    targ20 = targ19 * targ;
    targ29 = targ20 * targ9;

    pi = *p;
    pi2 = pi * pi;
    pi4 = pi2 * pi2;

        //lint -e{961}
		ret_val = ((((((((((((((((targ14 * targ4 * -0.013641135821518 + 
	    5.5242716940682e-13) * targ14 + 4.7359492924764e-25) * pi2 * 
	    targ6 + (pi * -1.2410752785137e-11 + targ14 * 0.049965138936998) * 
	    targ19) * pi + targ * -3.0908182839692e-22) * pi + (targ13 * 
	    -0.050246518510642 + 2.6717467330171e-9) * targ15 + 
	    4.4454513380586e-21) * pi4 + (targ7 * pi2 * -4781.9819876448 + 
	    1332.4803024175) * targ20 * targ10 + targ9 * -5.8458099253864e-7) 
	    * pi4 * pi2 * targ19 + targ13 * -3.4954795937691e-6 + targ9 * 
	    -2.5507450196258e-10 + targ3 * 1.0374663655276e-16) * pi + targ10 
	    * targ2 * 6.4181736525088e-5) * pi + (targ14 * targ14 * 
	    -73850.2736991 + 2.244254776278e-8) * targ7) * pi + (targ14 * 
	    -242.66223542696 - 0.0034602240265361) * targ10) * pi + (targ19 * 
	    -208438.76702652 - 8.4423037834823) * targ15 + targ2 * 
	    -1.249716486777e-8) * pi + targ6 * 0.0039989127290421) * pi + 
	    targ2 * 3.6056766658235e-4 + targ * 6.3755087552931e-6 - 
	    1.9650645031516e-7) * pi + ((targ29 * -354.74272584198 - 
	    2.2480892468696) * targ3 - 0.024133119369638) * targ2 + 
	    1.0933624938123e-4) * pi + (((targ29 * -0.23932578946761 - 
	    76.407372741773) * targ3 - 3.9270508365637) * targ2 - 
	    0.094450864454513) * targ - 0.0082325284089205) * pi + (targ3 * 
	    -75.253615672206 - 43.05190205118) * targ2 + targ * 
	    -22.926624714607 - 4.4448764333452) * pi + ((((ti * 
	    6.9892279225579 - 71.230960214888) * ti + 304.3810451334) * ti - 
	    709.83089736335) * ti + 1092.5860122659) * ti * ti + 
	    2513.8371504395 + (tau * 15.901837383578 - 141.60228305498) * tau;

    return ret_val;
} /* hpt2n_ */




/*                 IAPWS Industrial Formulation 1997 */
/*        for the Thermodynamic Properties of Water and Steam */
/*                           (IAPWS-IF97) */

/*  First argument is specific volume in m^3/kg. */
/*  Second argument is temperature in K. */
/*  Returns specific enthalpy in kJ/kg */

TFLOAT hvt3n_(const TFLOAT *v,const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;

    /* Local variables */
    static TFLOAT dn, tn, dn2, tn2, tn4, tn5, tn8, tn12;

    dn = 1.0 / (*v * 322.0);
    dn2 = dn * dn;

    tn = 647.096 / *t;
    tn2 = tn * tn;
    tn4 = tn2 * tn2;
    tn5 = tn4 * tn;
    tn8 = tn4 * tn4;
    tn12 = tn8 * tn4;

        //lint -e{961}
		ret_val = ((((((((((((((dn2 * -7.6714125522093e-4 + 0.052189898432996) * 
	    dn - .23277882648693) * dn2 + 2.4274463510316) * dn - 
	    7.2782785116784) * dn + 9.7643832897896) * dn - 6.370374316693) * 
	    dn + 1.6228889618024) * tn4 + .43664646488852) * dn * tn5 - 
	    5.3340395459772) * tn2 + 6.536762135345) * tn * tn8 - 
	    3.7231696861705) * tn4 - 1.7520886815502) * tn2 + ((((((((tn * 
	    -8.4059796975928e-4 + 3.7367361667631e-4) * dn + tn2 * 
	    0.002940624400577) * dn2 + tn2 * -0.056089416393544) * dn + tn2 * 
	    0.34803030138386 - 0.06140714438014) * dn + (tn2 * -1.215239431057 
	    + 0.29825229662283) * tn) * dn + (tn2 * 3.3441092408796 - 
	    1.2316218802611) * tn2 + 0.081199175821825) * dn + ((tn12 * 
	    -0.072035213749267 - 6.5227195048178) * tn2 + 3.2075594480472) * 
	    tn2 - 0.38767255915581) * dn + ((tn * -12.669975838924 + 
	    18.081584968717) * tn4 - 1.573152148192) * tn2 + 0.35531465412227) 
	    * dn) * dn + (((tn * tn12 * -0.089768500555238 + tn2 * 
	    6.6755322030656 - 12.960010600398) * tn8 + tn5 * 8.4598470171481 
	    - 7.0952891492865) * tn + 9.6663837579639) * tn + 0.49189764279794)
	     * *t;

    return ret_val;
} /* hvt3n_ */



/* *********************************************************************** */
TFLOAT pbt_(const TFLOAT *t)
{
    /* System generated locals */
    TFLOAT ret_val;


    if (((*t) <= 647.096f) && ((*t) >= 273.15f)) 
    {
		ret_val = psattn_(t);
    }
    else
    {
		ret_val = 0.0;
    }

    return ret_val;
} /* pbt_ */