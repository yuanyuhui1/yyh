//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------
#ifndef D_QCOMMONCAL_H
#define D_QCOMMONCAL_H

#define NOT_VALID_Q_TYPE 0xFF

//--------------------------------------------------------------------------------------------------
TBOOL CalculateQ(TUSIGN8 input_Q_type, TFLOAT* pBaseValue, TFLOAT* pDampValue, \
                    TBOOL simulation, TFLOAT hiLimit, TFLOAT cutoffPercentage, TFLOAT dampingTime);

TBOOL IsPVMatchWithQ(TUSIGN8 input_Q_type);
#endif