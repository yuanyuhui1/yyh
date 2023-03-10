//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         COMMON
 Description    implementation of functions for common problems
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "Common/Interface/common_functions.h"


TUSIGN16 InitCRC16_COMMON()
{
  return 0x1D0F;
}


#if USE_CRC_ALGORITHM == 0

//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           loop bit by bit
           ~253us on M16C @ 2MHZ
 \author   Martin Dahl
 \date     2007-05-07
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCRC16_COMMON(TUSIGN8 data, TUSIGN16 crc)
{
  TINT8 i;

  // XOR in the data.
  crc ^= data;

  // Perform the XORing for each bit
  for (i = 7; i >= 0; i--)
  #if 0
    // this might result in faster code, depending on the cost of jumps
    // M16C: 282 us @ 2MHz
    crc = (crc >> 1) ^ ((crc & 1) ? 0x8408 : 0);
  #else
    {
      // M16C: 253 us @ 2MHz
      if (crc & 1) crc = (crc >> 1) ^ 0x8408;
        else crc >>= 1;
    }
  #endif

  return crc;
}

#endif


#if USE_CRC_ALGORITHM == 2

static FAST TUSIGN16 crc_tab[16] =
{
  0x0000, 0x1081, 0x2102, 0x3183,
  0x4204, 0x5285, 0x6306, 0x7387,
  0x8408, 0x9489, 0xA50A, 0xB58B,
  0xC60C, 0xD68D, 0xE70E, 0xF78F
};

//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           double table lookup
           ~198us on M16C @ 2MHZ
 \author   Martin Dahl
 \date     2007-05-07
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCRC16_COMMON(TUSIGN8 data, TUSIGN16 crc)
{
  // XOR in the data.
  crc ^= data;

  // Perform the XORing for each nibble
  crc = (crc >> 4) ^ crc_tab[crc & 0x0f];
  crc = (crc >> 4) ^ crc_tab[crc & 0x0f];

  return crc;
}

#endif


#if USE_CRC_ALGORITHM == 1

static FAST TUSIGN16 crc_tab[256] =
{
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           single table lookup
           ~182us on M16C @ 2MHZ
 \author   Martin Dahl
 \date     2007-05-07
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCRC16_COMMON(TUSIGN8 data, TUSIGN16 crc)
{
	return (crc >> 8) ^ crc_tab[data ^ (crc & 0xff)];
}




//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           using the CRC Hardware Circuit of the M16C in a threadsafe implementation
           ~ TODO us on M16C @ 2MHZ
            - Done: if user task will use CRC HW, the register should be the same as before; As now should not be used.
 \author   Paul Li
 \date     2008-08-19
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning   SuspendAll_RTOS_TASK
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCrc16Mem_COMMON(const TUSIGN8 * ptrData,TUSIGN16 crcInit ,TUSIGN16 length)
{

	TUSIGN16 index = 0;
	TUSIGN16 crc = crcInit;
	for(index = 0; index<length; index++)
	{
		crc = CalcCRC16_COMMON(ptrData[index],crc);
	}
	return crc;
}

#endif



#if USE_CRC_ALGORITHM == 3


//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           using the CRC Hardware Circuit of the M16C in a threadsafe implementation
           ~ TODO us on M16C @ 2MHZ
            - Done: if user task will use CRC HW, the register should be the same as before; As now should not be used.
 \author   Klaus Pose
 \date     2008-08-19
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning  ATOMIC_IRQ_LOCKED 
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCRC16_COMMON(TUSIGN8 data, TUSIGN16 crc)
{
/* TODO
    static TBOOL mLocked=eFALSE;
    while( mLocked ) {};
    mLocked = eTRUE;
    CRC16_M16C = crc;
    CalcCRC16_M16C(data);
    crc = CRC16_M16C;
    mLocked = eFALSE;
*/
    
    ATOMIC_IRQ_LOCKED
    (
        TUSIGN16 oldCRC = CRC16_M16C;
        CRC16_M16C = crc;
        CalcCRC16_M16C(data);
        crc = CRC16_M16C;
        CRC16_M16C = oldCRC;
    );
    return crc;
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief    update crc checksum
           reversed CCITT (X^16 + X^12 + X^5 + 1)
           using the CRC Hardware Circuit of the M16C in a threadsafe implementation
           ~ TODO us on M16C @ 2MHZ
            - Done: if user task will use CRC HW, the register should be the same as before; As now should not be used.
 \author   Paul Li
 \date     2008-08-19
 \param    data
 \param    crc previous crc checksum
 \return   updated crc checksum
 \warning   SuspendAll_RTOS_TASK
 \test
 test-date: 2012-MM-DD
 \n by: pp ss
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 CalcCrc16Mem_COMMON(TUSIGN8 * ptrData,TUSIGN16 crcInit ,TUSIGN16 length)
{
/* TODO
    static TBOOL mLocked=eFALSE;
    while( mLocked ) {};
    mLocked = eTRUE;
    CRC16_M16C = crc;
    CalcCRC16_M16C(data);
    crc = CRC16_M16C;
    mLocked = eFALSE;
*/
    TUSIGN16 index = 0;
    TUSIGN16 crc;
    TUSIGN16 oldCRC;
    
    SuspendAll_RTOS_TASK();
    oldCRC = CRC16_M16C;
    CRC16_M16C = crcInit;
    for(index = 0; index<length;index++)
        CRCIN = ptrData[index];
    crc = CRC16_M16C;
    CRC16_M16C = oldCRC;
    ResumeAll_RTOS_TASK();
    return crc;
}
#endif

//-------------------------------------------------------------------------------------------------
/*!
<pre>
Copyright              Copyright ABB, 2010.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem MVMeasurement
Module         CommonFunction
Description    Implementation damping Hysteresis& Alarm Delay

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------




//#include "..\..\system\interface\common_type.h"
//#include "common_functions.h"
#ifdef WIN32
#include "stdio.h"
#endif
#define POLYNOMIAL_ALGRITHM

#ifndef POLYNOMIAL_ALGRITHM
#include <math.h>
#endif
#define LONG_INT_MAX      (2147483647) //2(32)-1

//--------------------------------------------------------------------------------------------------
/*!
\brief  Damping for the input value 
\author zuochen wang
\date   2010-10-09
\param  inputValue  :TFLOAT     the input orignal value .
\param  outputValue  :TFLOAT *  the Output value after damping .
\param  DampInit  :TUSIGN8      the flag to control the damping if it run first time. 
\param  previoustime :TINT32    OS tick when the Damping function run at last time.
\param  dampingTime :TFLOAT     The damping time . the unit is second.
\return none
\warning none
\test
test-date    :2010-11-04   
by           :zuochen wang   
environment  :  CSPY
result       :  OK

\test  Lint level 3 OK
\bug
*/
//--------------------------------------------------------------------------------------------------
void  Damping(const TFLOAT inputValue, TFLOAT *outputValue,TUSIGN8 *dampInit,TUSIGN32 *previoustime, const TFLOAT dampingTime)
{
#ifdef  POLYNOMIAL_ALGRITHM

	TFLOAT  KTI = 0.0f;
	TFLOAT  KTV = 0.0f;
	TUSIGN32  newTime = GetTickCount_RTOS_TASK();
    TUSIGN32  cycleTime = 0;

    // Damped Value = P + (N-P)*KTV, P is the previous damp value, N is the input value
    // KTV = KTI * (1.0 - 0.5 * KTI)
    // KTI = U / T * 0.001, U is the cycle time


	if(*dampInit == 0)
	{
		*previoustime = newTime;
		*dampInit = 1;
	}
    else
    {
    }

    if (newTime >= *previoustime)
    {
        cycleTime = newTime-*previoustime;
    }
    else
    {
        cycleTime = (TUSIGN32)((LONG_INT_MAX - *previoustime) + newTime);
    }

    *previoustime = newTime;

    if (dampingTime <= 0.001f)
    {
        *outputValue = inputValue;
    }
    else
    {
        KTI = ((TFLOAT)cycleTime/dampingTime)*0.001f;
		
		KTV= KTI * ( 1.0f - (0.5f * KTI) );
		*outputValue =(*outputValue) + ((inputValue-(*outputValue)) * KTV);
    }
#else
	TFLOAT  cycleTime = 0.0f;
	TUSIGN32  newTime;


	if(*DampInit == 0)
	{
		*previoustime = GetTickCount_RTOS_TASK();
		*DampInit = 1;
	}
    else
    {
    }
    if(dampingTime <=0.0f)
    {
        *outputValue=inputValue;
    }
	newTime = GetTickCount_RTOS_TASK();
	cycleTime = newTime-*previoustime;
    if(cycleTime <0)
    {
        cycleTime += 1.0f*LONG_INT_MAX;
    }
    *outputValue = *outputValue + ((inputValue - *outputValue) * ( 1 - ( pow(__E,  -1.0f * cycleTime / (dampingTime*1000.0f) )) ));
    *previoustime = newTime;
#endif    

#ifdef WIN32 
	*outputValue=inputValue;
#endif 

}




//--------------------------------------------------------------------------------------------------
/*!
\brief  Upperrange limit Alarm set/get
\author zuochen wang
\date   2010-10-09
\param  limitValue  :TFLOAT     the range limit value .
\param  actualValue  :TFLOAT    the new get value, need to be used to compare with limit to get whether it is an alarm .
\param  Hysteresis  :TFLOAT     the value that should be used for clearing alarm . 
\param  delayTime :TFLOAT       he delay time value that should be used for setting alarm . 
\param  pprevioustime :TINT32       OS tick time when last alarm generated.
\param  pAlarmStatus :TBOOL     The alarm status after last call of the function. also used for the return of this call.
\return none
\warning none
\test
test-date    :2012-05-11   
by           :Paul Li   
environment  :  CSPY
result       :  OK

\test  Lint level 3 OK
\bug
*/
//--------------------------------------------------------------------------------------------------
void CheckUpperRangeLimit_Alarm(const TFLOAT limitValue, const TFLOAT actualValue,const TFLOAT Hysteresis,const TFLOAT delayTime, TUSIGN32* pprevioustime, TBOOL* pAlarmStatus)
{
    if(*pAlarmStatus)//alarm exists, need to clear.
    {
        if(actualValue < (limitValue - Hysteresis))
        {
            *pAlarmStatus = eFALSE;
        }
    }
    else //alarm does not exists
    {
        TUSIGN32 ostime = GetTickCount_RTOS_TASK();
        if(actualValue <= limitValue)
        {
            *pprevioustime = ostime;
        }
        else 
        {   
            TUSIGN32 offsetTime = 0;

            if (ostime >= *pprevioustime)
            {
                offsetTime = ostime - *pprevioustime;
            }
            else
            {
                offsetTime = (TUSIGN32)((LONG_INT_MAX - *pprevioustime) + ostime);
            }

            if(offsetTime > (delayTime*1000.0f))
            {
                *pAlarmStatus = eTRUE;
            }
        }
    }
}





//--------------------------------------------------------------------------------------------------
/*!
\brief  Lowerrange limit Alarm set/get
\author zuochen wang
\date   2010-10-09
\param  limitValue  :TFLOAT     the range limit value .
\param  actualValue  :TFLOAT    the new get value, need to be used to compare with limit to get whether it is an alarm .
\param  Hysteresis  :TFLOAT     the value that should be used for clearing alarm . 
\param  delayTime :TFLOAT       he delay time value that should be used for setting alarm . 
\param  pprevioustime :TINT32       OS tick time when last alarm generated.
\param  pAlarmStatus :TBOOL     The alarm status after last call of the function. also used for the return of this call.
\return none
\warning none
\test
test-date    :2012-05-11   
by           :Paul Li   
environment  :  CSPY
result       :  OK

\test  Lint level 3 OK
\bug
*/
//--------------------------------------------------------------------------------------------------
void CheckLowerRangeLimit_Alarm(const TFLOAT limitValue, const TFLOAT actualValue,const TFLOAT Hysteresis,const TFLOAT delayTime, TUSIGN32* pprevioustime, TBOOL* pAlarmStatus)
{
    if(*pAlarmStatus)//alarm exists, need to clear.
    {
        if(actualValue > (limitValue + Hysteresis))
        {
            *pAlarmStatus = eFALSE;
        }
    }
    else //alarm does not exists
    {
        TUSIGN32 ostime = GetTickCount_RTOS_TASK();
        if(actualValue >= limitValue)
        {
            *pprevioustime = ostime;
        }
        else 
        {   
            TUSIGN32 offsetTime = 0;

            if (ostime >= *pprevioustime)
            {
                offsetTime = ostime - *pprevioustime;
            }
            else
            {
                offsetTime = (TUSIGN32)((LONG_INT_MAX - *pprevioustime) + ostime);
            }

            if(offsetTime > (delayTime*1000.0f))
            {
                *pAlarmStatus = eTRUE;
            }
        }
    }
}