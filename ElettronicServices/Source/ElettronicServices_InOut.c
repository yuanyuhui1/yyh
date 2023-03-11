//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Source/ElettronicServices_InOut.h"



//-------------------------------------------------------------------------
//pins test
#define PULL_UP         1
#define PULL_DOWN       2
#define PULL_UP_DOWN    3
#define HANG            4

#define OPEN_CONNECT    1
#define SHORT_CONNECT   2       //the pin is connected to the next pin

#define NO_TEST         5

#define PORT_NUM_SIZE   11      //p0 ~ p10

/*
1. For pins pull up/down test:
user can chang the value of the pins status in the PIN_UP_DOWN_STATUS[][] table according to the pins stasus.
The status include: 1. PULL_UP 
                    2. PULL_DOWN
                    3. PULL_UP_DOWN (The pins status is effected by user operation)
                    4. HANG
                    5. NO_TEST (The pins does not be tested)

2. For pins short connect test:
user can chang the value of the pins status in the PIN_SHORT_CONNECT_STATUS[][] table according to the pins stasus.
The status include: 1. OPEN_CONNECT 
                    2. SHORT_CONNECT
                    5. NO_TEST (The pins does not be tested). For some pins when the pins out put high, the pins is connected to GND straightly. The pins does not be tested. 

3. The P3_0, P4_6 & P4_7 will not be tested. The 3 pins is used by current out PWM and the output voltage can not be changed according to keep the current out value.

4. Special operation:
//special operation----HMI-TXD, HMI-RXD, HMI-Presence HMI-RST; -- NO_TEST in up_dwon test.
//special operation----P3_0, P4_6 & P4_7 is used by Current out PWM; -- NO_TEST in up_dwon and short connect test
//special operation----P0_7 is used by watchdog. The voltage is different when connect/disconnect HW watchdog.-- PULL_UP_DOWN in up_down test
//special operation----DIP-switch and push button pins -- NO_TEST in short connect test & PULL_UP_DOWN in up_down test. 

5. User shall change the status:
The pins status of FE,SECOND FE, DOUT and DIGIN pins is different. User shall change the test status table value according to different device. 
If some pins shall not be tested, just change it tobe NO_TEST in PIN_SHORT_CONNECT_STATUS[ ] array and it will keep in input mode in the test function.

6.The port status will be restored after test. Only restore the output pins value and restore the in/out put register value. 
  Not restore the input pins value and the port status base on the port readback.
*/

/*
The array order:
{p0_0,p0_1.......p0_7},
{p1_0,p1_1,......p1_7},
{...}
...
{p10_0,p10_1,...p10_7}

*/
#if 0
const TUSIGN8 PIN_UP_DOWN_STATUS[PORT_NUM_SIZE][8] = {
    
    {PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP_DOWN},         //P0 pins number: 79,80,81,82,83,84,85,86--P0_7/6/5/4/3/2/1 pull up; P0_7 is used by watch dog,not stable so not test 

		//VT5 change 1.0 to pull down, change 1.2 1.3 to pull up(FE RX/TX), change 1.1 1.4 1.5 1.6 to pull up(not use)
    {PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,HANG}, 						//P1 pins number: 72,73,74,75,76,77,78--P1_6/5/4/3/2/1/0 pull up. pins number: 71--P1_7 have no pull_up/dowm

		//VT5 change 2.0 to pull up(Not use)
    {PULL_UP,PULL_UP,HANG,PULL_UP,PULL_UP,HANG,HANG,HANG},                  			  //P2 pins number: 66,67,69,70--P2_4/3/1/0 pull up. pins number: 63,64,65,68--P2_7/6/5/2 have no pull_up/dowm
                                                                                                                //P2_0: SW4
		//VT5 change 3.6 3.7 to pull down(Not use)
		{NO_TEST,HANG,HANG,HANG,HANG,HANG,PULL_DOWN,PULL_DOWN},                 			  //P3 pins number: P3_0 ~ P3_5 is used by current out
                                                                                                                //P3_7: GATING_DO1. P3_6: GATING_DO2
		//VT5 change 4.0 to pull up(Not use)
		{PULL_UP,PULL_DOWN,PULL_DOWN,PULL_UP,PULL_UP,PULL_UP,NO_TEST,NO_TEST},  			  //P4 pins number: 47,48,49,52--P4_/5/4/3/0 pull Up. pins number: 50,51--P4_2/1 pull dowm
                                                                                                                //P4_0: SFE_PWRCTL, p4_6/7 is used by current out PWM
		{PULL_UP,PULL_DOWN,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP},    			  //P5 pins number: 37,38,39,40,41,42,44--P5_7/6/5/4/3/2/0 pull Up. pins number: 43--P5_1 pull dowm. 

		//6.4 6.5 to pull down; change 6.6 6.7 to pull up (HMI),keep no test ,for HMI sometimes no connect
		{PULL_UP,PULL_UP,HANG,PULL_UP,NO_TEST,NO_TEST,NO_TEST,NO_TEST},     		    //P6 P6_7: HMI_RXD. P6_6: HMI_TXD. P6_5: PRESENCE. P6_4: HMI_RST. pins number: 34--P6_2 have no pull_up/dowm. 33,35,36--P6_3/1/0 pull Up
    
		//VT5 change 7.2 7.3(FE control) to pull up;7.0(I2C) big disturb no test
		{NO_TEST,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP},      		 		//P7 pins number: 21,22,23,24,25,26,27,28--P7_7/6/5/4/3/2/1/0 pull Up. P7_2: CLK_FE. P7_3: HS_FE
    
		//VT5 change 8.0 to pull up(DO)
		{NO_TEST,PULL_UP,HANG,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP},         				//P8 pins number: 8,9,15,16,17,19,20--P8_7/6/5/4/3/1/0 pull Up. pins number: 18--P8_2 have no pull_up/dowm. P8_0: CLOCK_D01
    
		//VT5 change 9.5 9.6 9.7 to pull up(SPI)
		{HANG,HANG,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP,PULL_UP},            				//P9 pins number: 98,99,100,1,2,3,--P9_7/6/5/4/3/2 pull Up. pins number: 4,5--P9_1/0 have no pull_up/dowm
                                                                                                                //P9_5: SCLK. P9_6:DOUT. P9_7: DIN
		//VT5 change 10.1 2 to pull up(SPI), dip switch no satus, so no test
		{PULL_UP,PULL_UP,PULL_UP_DOWN,PULL_UP_DOWN,PULL_UP_DOWN,PULL_UP_DOWN,PULL_UP_DOWN,PULL_UP_DOWN}   						//10 pins number: 87,88,89,90,91,92,--P10_7/6/5/4/3/2/ is used by sw1. 93,95 P10_1: CS. P10_0: SFE_PMS.
    
};

const TUSIGN8 PIN_SHORT_CONNECT_STATUS[PORT_NUM_SIZE][8] = {

    {OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,NO_TEST,OPEN_CONNECT,OPEN_CONNECT},               //P0 pins(79,80,81,82,83,84,85,86) is continuous and not short connected; but P0_5 & P0_6.
    
    {OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,NO_TEST,NO_TEST,OPEN_CONNECT,OPEN_CONNECT},                    //P1 pins(71,72,73,74,75,76,77,78) is continuous and not short connected.
                                                                                                                        //P1_4: SW2. P1_5: SW3. The pins are connected to GND straight
    {NO_TEST,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,NO_TEST},                    //P2 pins(63,64,65,66,67,68,69,70) is continuous and not short connected. P2_7 & Pin(VSS)
                                                                                                                        //P2_0: is used by sw4 and is connected to GND straight
    {NO_TEST,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT},               //P3 pins(53,54,55,56,57,58,59,61) P3_0 is used for current out.

    {OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,NO_TEST,NO_TEST,NO_TEST},                         //P4 pins(45,46,47,48,49,50,51,52) is continuous and not short connected.
                                                                                                                        //P4_6/7 is used for current out.
    {OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT},          //P5 pins(37,38,39,40,41,42,43,44) is continuous and not short connected.
    
    {SHORT_CONNECT,OPEN_CONNECT,OPEN_CONNECT,NO_TEST,NO_TEST,NO_TEST,NO_TEST,OPEN_CONNECT},                        //P6 pins(29,30,31,32,33,34,35,36) is continuous. Pins 35 and 36 (P6_1 and P6_0) is connected and the other is not connected.
    
    {OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT},          //P7 pins(21,22,23,24,25,26,27,28) is continuous and not short connected.
    
    {NO_TEST,OPEN_CONNECT,OPEN_CONNECT,SHORT_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT},         //P8 pins(8,9,15,16,17,18,19,20). Pins 16 and 17 (P8_4 and P8_3) is connected and the other is not connected. Pins 10,11,12,13,14 is not GPIO.
    
    {SHORT_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT,OPEN_CONNECT},         //P9 pins(98,99,100,1,2,3,4,5). Pins 4 and 5 (P9_1 and P9_0) is connected and the other is not connected
    
    {OPEN_CONNECT,OPEN_CONNECT,NO_TEST,NO_TEST,NO_TEST,NO_TEST,NO_TEST,NO_TEST}                                         //P10 pins(87,88,89,90,91,92,93,95) is continuous but 95 and not connected. Pins 94 is AVSS;

};


//--------------------------------------------------------------------------------------------------
/*!
 \brief	This method is used to pins status test.
 \author	Kuang xiaojin
 \date   2013-01-11
 \param  -
 \return -
 \warning
 \test
 \test-date: 2013-06-13
 \n by: Spring Zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test:Pass, 88.31% code coverage
 \n result Lint Level 3:OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
void SafetyPinsTest()
{

	TUSIGN8 volatile __data13 * ptrPortData[11] = {&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10};
	TUSIGN8 volatile __data13 * ptrPortDir[11] = {&pd0,&pd1,&pd2,&pd3,&pd4,&pd5,&pd6,&pd7,&pd8,&pd9,&pd10};
	TUSIGN8 portIdx=0, bitIdx = 0;
	TUSIGN8 u = 0;
	TUSIGN8 portStatus = 0;
	TUSIGN8 data = 0;
	TUSIGN8 dataHigh = 0;
	TUSIGN8 dataLow = 0;
	TUSIGN8 errorNum = 0;
	TUSIGN8 portDirValue = 0;
	TUSIGN8 portDirRead[11] = {0};
	TUSIGN8 portDataRead[11] = {0};
	for(portIdx=0; portIdx<11; portIdx++)
	{
		portDirRead[portIdx] = *ptrPortDir[portIdx];        //read ports direction
		portDataRead[portIdx] = *ptrPortData[portIdx] & portDirRead[portIdx]; //read the output ports data
	}

	//1: pins pull up/down status test
	errorNum = 0;
	for(portIdx=0; portIdx<PORT_NUM_SIZE; portIdx++)
	{

		if(portIdx == 3)
		{
			*ptrPortDir[portIdx] = 0x01;   //input mode,P3_0 set for hard ware current out
		}
		else if(portIdx == 4)
		{
			*ptrPortDir[portIdx] = 0xc0;   //input mode,P4_6,7 set for current out
		}
		else if(portIdx == 9)
		{
			PRCR_bit.PRC2 = 1; // clear protection register for PD9
			*ptrPortDir[portIdx] = 0x00;  //input mode
		}
		else
		{
			*ptrPortDir[portIdx] = 0x00;   //input mode
		}

		//for(u=0;u<50;u++);
		for(bitIdx=0; bitIdx<8; bitIdx++)
		{
			portStatus = PIN_UP_DOWN_STATUS[portIdx][bitIdx];
			switch(portStatus)
			{
				case PULL_UP:
				{
					for(u=0; (u<10) && ((*ptrPortData[portIdx] & (0x01 << bitIdx))!=0x00); u++)
					{
					}
					//lint -e{960} hao wu,  2013-01-15 Ok
					if(u < 10)
					{ errorNum ++; }
				}
				break;
				case PULL_DOWN:
				{
#ifdef TEST_PULL_DOWN// note, this could only be available for pull down resister not bigger than 10K
					if(portIdx == 5)// only for P5_1
					{
						TUSIGN8 pullUpDown = 0x0;
						pullUpDown = pu12;
						pu12 = 1;
						for(u=0; (u<10) && ((*ptrPortData[portIdx] & (0x01 << bitIdx))==0x00); u++)
						{
						}
						//lint -e{960} hao wu,  2013-01-15 Ok
						if(u < 10)
						{ errorNum ++; }
						pu12 = pullUpDown;
					}
#endif
				}
				break;
				case PULL_UP_DOWN:
				{
					data = *ptrPortData[portIdx];
					data &= 0x01 << bitIdx;
					for(u=0; (u<10) && ((*ptrPortData[portIdx] & (0x01 << bitIdx))==data); u++)
					{
					}
					//lint -e{960} hao wu,  2013-01-15 Ok
					if(u < 10)
					{ errorNum ++; }
				}
				break;

				default:
					break;
			}
		}

	}
	if(errorNum != 0)
	{
		elettronicServices.ExitSRV();
	}

	//2:short connect test
	errorNum = 0;
	for(portIdx=0; portIdx<PORT_NUM_SIZE; portIdx++)
	{
		for(bitIdx=0; bitIdx<8; bitIdx++)
		{
			portStatus = PIN_SHORT_CONNECT_STATUS[portIdx][bitIdx];
			if((portStatus == OPEN_CONNECT) || (portStatus == SHORT_CONNECT))
			{
				if(portIdx == 3)
				{
					portDirValue = (0x01 << bitIdx) | 0x01;     //output mode,P3_0 set for hard ware current out
					*ptrPortDir[portIdx] = portDirValue;
					pd2_2 = 1;          //FE_OUT out put L,The p3_1~P3_5 shall be L
					p2_2 = 0;
				}
				else if(portIdx == 4)
				{
					portDirValue = (0x01 << bitIdx) | 0xc0;     //output mode,P4_6,7 set for hard ware current out
					*ptrPortDir[portIdx] = portDirValue;
				}
				else if(portIdx == 9)
				{
					PRCR_bit.PRC2 = 1; // clear protection register for PD9
					*ptrPortDir[portIdx] = 0x01 << bitIdx;     //output mode
				}
				else
				{
					*ptrPortDir[portIdx] = 0x01 << bitIdx;     //output mode
				}
				//2.1:out put H
				if(portIdx == 3)
				{
					*ptrPortData[portIdx] = (0x01 << bitIdx) | 0x01;
				}
				else if(portIdx == 4)
				{
					switch(bitIdx)
					{
						case 0:
							p4_0 = 1;
							break;
						case 1:
							p4_1 = 1;
							break;
						case 2:
							p4_2 = 1;
							break;
						case 3:
							p4_3 = 1;
							break;
						case 4:
							p4_4 = 1;
							break;
						case 5:
							p4_5 = 1;
							break;
						default:
							break;
					}
				}
				else
				{
					*ptrPortData[portIdx] = 0x01 << bitIdx;
				}
				//read the port data
				if(bitIdx == 7)               //pi_7 connect to pi+1_0
				{
					if(portIdx == (PORT_NUM_SIZE -1))               //p10_7 connect to p0_0
					{
						dataHigh = *ptrPortData[0];
					}
					else
					{
						dataHigh = *ptrPortData[portIdx+1];
					}
				}
				else
				{
					dataHigh = *ptrPortData[portIdx];
				}

				//2.2: OUT Put L
				if(portIdx == 3)
				{
					*ptrPortData[portIdx] = 0x01;
				}
				else if(portIdx == 4)
				{
					switch(bitIdx)
					{
						case 0:
							p4_0 = 0;
							break;
						case 1:
							p4_1 = 0;
							break;
						case 2:
							p4_2 = 0;
							break;
						case 3:
							p4_3 = 0;
							break;
						case 4:
							p4_4 = 0;
							break;
						case 5:
							p4_5 = 0;
							break;
						default:
							break;
					}
				}
				else
				{
					*ptrPortData[portIdx] = 0x00;
				}
				//read the port data
				if(bitIdx == 7)               //pi_7 connect to pi+1_0
				{
					if(portIdx == (PORT_NUM_SIZE -1))               //p10_7 connect to p0_0
					{
						dataLow = *ptrPortData[0];
					}
					else
					{
						dataLow = *ptrPortData[portIdx+1];
					}
				}
				else
				{
					dataLow = *ptrPortData[portIdx];
				}

			}
			else
			{}

			switch(portStatus)
			{
				case OPEN_CONNECT:
				{
					if(bitIdx == 7)
					{
						//lint -e{960} hao wu,  2013-01-15 Ok
						if(((dataHigh & 0x01) != 0) && ((dataLow & 0x01)  == 0))   //test Pi_0
						{ errorNum ++; }
					}
					else
					{
						//lint -e{960} hao wu,  2013-01-15 Ok
						if(((dataHigh & (0x01 << (bitIdx+1))) != 0) && ((dataLow & (0x01 << (bitIdx+1))) == 0))   //test next pin status
						{ errorNum ++; }
					}
				}
				break;
				case SHORT_CONNECT:
				{
					if(bitIdx == 7)
					{
						//lint -e{960} hao wu,  2013-01-15 Ok
						if(((dataHigh & 0x01) == 0) && ((dataLow & 0x01) != 0))   //test Pi_0
						{ errorNum ++; }
					}
					else
					{
						//lint -e{960} hao wu,  2013-01-15 Ok
						if(((dataHigh & (0x01 << (bitIdx+1))) == 0) && ((dataLow & (0x01 << (bitIdx+1))) != 0))   //test next pin status
						{ errorNum ++; }
					}
				}
				break;
				default:
					break;
			}
		}
	}
	if(errorNum != 0)
	{
		elettronicServices.ExitSRV();
	}

	for(portIdx=0; portIdx<11; portIdx++) //reset as tested before
	{
		//p4_6 & p4_7 is used as current out PWM.
		if(portIdx == 4)
		{
			p4_0 = portDataRead[portIdx] & 0x01;
			pd4_0 = portDirRead[portIdx] & 0x01;
			p4_1 = (portDataRead[portIdx] & (0x01<<1))>> 1;
			pd4_1 = (portDirRead[portIdx] & (0x01<<1)) >> 1;
			p4_2 = (portDataRead[portIdx] & (0x01<<2)) >> 2;
			pd4_2 = (portDirRead[portIdx] & (0x01<<2)) >> 2;
			p4_3 = (portDataRead[portIdx] & (0x01<<3)) >> 3;
			pd4_3 = (portDirRead[portIdx] & (0x01<<3)) >> 3;
			p4_4 = (portDataRead[portIdx] & (0x01<<4)) >> 4;
			pd4_4 = (portDirRead[portIdx] & (0x01<<4)) >> 4;
			p4_5 = (portDataRead[portIdx] & (0x01<<5)) >> 5;
			pd4_5 = (portDirRead[portIdx] & (0x01<<5)) >> 5;

		}
		else if(portIdx == 9)
		{
			*ptrPortData[portIdx] = portDataRead[portIdx];      //reset the output ports data
			PRCR_bit.PRC2 = 1; 									// clear protection register for PD9
			*ptrPortDir[portIdx] = portDirRead[portIdx];        //reset ports direction
		}
		else
		{
			*ptrPortData[portIdx] = portDataRead[portIdx];      //reset the output ports data
			*ptrPortDir[portIdx] = portDirRead[portIdx];        //reset ports direction

		}
	}
	(void)InitializeHardware_CURR();    //reset as current_out_initHardware.c

}
#endif