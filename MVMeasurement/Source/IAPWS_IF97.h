//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2010.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 IAPWS IF97
 Description            head file to calculation of property of water and steam.
 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef _IAPWS_IF97_H
#define _IAPWS_IF97_H

//datatype definition.
//constant micro definition

//-----------------------------------------------------------------------------
// funtion. defintion.
//-----------------------------------------------------------------------------

#define  c__1     1
#define  c__2     2
#define  c__3     3

TFLOAT dbpt_( const  TFLOAT *p,   const  TFLOAT*t);
TFLOAT diter3_(  const  TFLOAT *p,   const  TFLOAT *t,    const  TFLOAT *dest,    const  TFLOAT *eps);
TFLOAT fb23_(  const  TFLOAT *t);
TFLOAT hbpt_(  const  TFLOAT *p,   const  TFLOAT *t);
TFLOAT hpt_(  const  TFLOAT *p,   const  TFLOAT *t, TINT16 *ireg);
TFLOAT hpt1n_(  const  TFLOAT *p,   const  TFLOAT *t);
TFLOAT hpt2n_(  const  TFLOAT *p,   const  TFLOAT *t);
TFLOAT hpt5n_(  const  TFLOAT *p,   const  TFLOAT *t);
TFLOAT hvt3n_(  const  TFLOAT *v, const  TFLOAT *t);
TFLOAT nullp3n_(const  TFLOAT *d, const  TFLOAT *t, const  TFLOAT *p);
TFLOAT psattn_(const  TFLOAT *ts);
TFLOAT pvt3n_(const  TFLOAT *v, const  TFLOAT *t);
int regsopt_(const  TFLOAT *p,const  TFLOAT *t,const TINT16 *pIreg);
TFLOAT tsatpn_(const  TFLOAT *ps);
TFLOAT vest3_(const  TFLOAT *p, const  TFLOAT *t,const TINT16 *ireg3);
TFLOAT vpt_(const  TFLOAT *p, const  TFLOAT *t,const TINT16 *ireg);
TFLOAT vpt1n_(const  TFLOAT *p, const  TFLOAT *t);
TFLOAT vpt2n_(const  TFLOAT *p, const  TFLOAT *t);
TFLOAT vpt3n_(const  TFLOAT *p, const  TFLOAT *t);
int wnpt3_(const  TFLOAT *xa, const  TFLOAT *xb, TFLOAT (*f) (),const  TFLOAT * p, const  TFLOAT *t, const  TFLOAT *eps, const  TFLOAT *x,const TINT16 *ix);
TFLOAT pbt_(const  TFLOAT *t);
TFLOAT tbp_(const  TFLOAT *p);
TFLOAT v1bt_(const  TFLOAT *t);
TFLOAT v2bt_(const  TFLOAT *t);
TFLOAT h1bt_(const  TFLOAT *t);
TFLOAT h2bt_(const  TFLOAT *t);
TFLOAT vpt5n_(const  TFLOAT *p, const  TFLOAT *t);
TFLOAT dlest_(const  TFLOAT *t);
TFLOAT dvest_(const  TFLOAT *t);
int reg3s_(const  TFLOAT *p, const  TFLOAT *t,const TINT16 *ireg3);
int fsatp_(const  TFLOAT *dvout, const  TFLOAT *dlout, const  TFLOAT *tout, const  TFLOAT *tin,  const  TFLOAT *pin);
#endif