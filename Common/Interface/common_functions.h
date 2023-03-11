//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         COMMON
 Description    prototypes of functions for common problems
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __SYSTEM_H__
  #error system.h must be included before common_functions.h
#endif


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before common_functions.h
#endif

#ifdef __COMMON_FUNCTIONS_H__
  #error common_functions.h is included more than once
#endif

#define __COMMON_FUNCTIONS_H__

#define ABS(A)   ((A)<(0)?(-(A)):(A))


//--------------------------------------------------------------------------------------------------
/*!
 \brief   lock irq for making things 'atomic' while maintaining previous interrupt-status
 \author  Klaus Pose
 \date    2008-08-19
 \param   code
 \return  atomic code
 \warning DMA is not locked, realtime-performance caused by interrupt-suspension
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
#define ATOMIC_IRQ_LOCKED( notInterrupted ) asm("PUSHC FLG");   \
                                            asm("FCLR  I");     \
                                            notInterrupted;     \
                                            asm("POPC FLG")


//--------------------------------------------------------------------------------------------------
//
// select a specific crc implementation for function CalcCRC16_COMMON
//
// there are 3 different implementations to choose from:
//   no. 0:  loop, bitwise CRC implementation, no table     ~253us on M16C @ 2MHZ
//   no. 1:  single table lookup, requires 512 byte table   ~182us on M16C @ 2MHZ
//   no. 2:  double table lookup, requires 32 byte table    ~198us on M16C @ 2MHZ
//   no. 3:  M16C CRC hardware                           ~ TODO us on M16C @ 2MHZ
//
//  there is a deprecated ASM implementation that uses a different generator polynom
//  with a single table lookup                              ~167us on M16C @ 2MHZ
//
//--------------------------------------------------------------------------------------------------
#define USE_CRC_ALGORITHM  1


//--------------------------------------------------------------------------------------------------
/*!
 \brief   update crc checksum (M16C coprocessor)
          reversed CCITT (X^16 + X^12 + X^5 + 1)

          Warning: as it uses a global resource this method is not threadsafe
          ~12us on M16C @ 2MHZ
 \author  Martin Dahl
 \date    2007-05-07
 \param   data
 \return  -
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
#if 0
#define CalcCRC16_M16C(data)    { CRCIN = data; }
#define CRC16_M16C              CRCD
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief   initalize CRC16
 \author  Martin Dahl
 \date    2007-05-07
 \return  initial value for CRC
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 InitCRC16_COMMON(void);
/* deprecated */ void InitCRC16_COMMON_ASM(TUSIGN16 NEAR* ptrCRC16);

TUSIGN16 CalcCRC16_COMMON_HW(TUSIGN16 initCrc, TUSIGN8* pData, TUSIGN16 len);

//--------------------------------------------------------------------------------------------------
/*!
 \brief   update crc checksum
          reversed CCITT (X^16 + X^12 + X^5 + 1)
 \author  Martin Dahl
 \date    2007-05-07
 \param   data
 \param   crc previous crc checksum
 \return  updated crc checksum
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCRC16_COMMON(TUSIGN8 data, TUSIGN16 crc);
TUSIGN16 CalcCrc16Mem_COMMON(const TUSIGN8 * ptrData,TUSIGN16 crcInit ,TUSIGN16 length);

/* deprecated */ void CalcCRC16_COMMON_ASM(TUSIGN8 data, TUSIGN16 NEAR* ptrCRC16);

TUSIGN16 CalcCrc16hugeMem_COMMON( TUSIGN16 initCrc, TUSIGN8 HUGE * startAddress,
                                                      TUSIGN8 HUGE * endAddress   );


//-----------------------------------------------------------------------------------------------
extern  void  Damping(TFLOAT InputValue, TFLOAT *OutputValue,TUSIGN8 *DampInit,TUSIGN32 *Previoustime, const TFLOAT DampingTime);
extern  void CheckUpperRangeLimit_Alarm(const TFLOAT limitValue, const TFLOAT actualValue,const TFLOAT Hysteresis,const TFLOAT delayTime, TUSIGN32* pPrevioustime, TBOOL* pAlarmStatus);
extern  void CheckLowerRangeLimit_Alarm(const TFLOAT limitValue, const TFLOAT actualValue,const TFLOAT Hysteresis,const TFLOAT delayTime, TUSIGN32* pPrevioustime, TBOOL* pAlarmStatus);



