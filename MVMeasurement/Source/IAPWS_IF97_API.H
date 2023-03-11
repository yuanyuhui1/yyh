//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2010.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module                 IAPWS IF97
 Description            Interface to calculation of property of water and steam.
 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef _IAPWS_IF97_API_H
#define _IAPWS_IF97_API_H
/*Input for temperature is K, pressure is MPa*/
extern TFLOAT pbt_(const TFLOAT *t); 
extern TFLOAT vpt1n_(const TFLOAT *p, const TFLOAT *t);
extern TFLOAT vpt2n_(const TFLOAT *p, const TFLOAT *t);
extern TFLOAT vpt3n_(const TFLOAT *p, const TFLOAT *t);
extern TFLOAT hpt1n_(const TFLOAT *p, const TFLOAT *t);
extern TFLOAT hpt2n_(const TFLOAT *p, const TFLOAT *t);
extern TFLOAT hvt3n_(const TFLOAT *p, const TFLOAT *t);
extern int regsopt_(const TFLOAT *p, const TFLOAT *t, const TINT16 *ireg);
extern TFLOAT tsatpn_(const TFLOAT *ps);
#endif