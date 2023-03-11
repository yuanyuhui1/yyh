//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System      UHTE
 Module      flash operation
 Description implementation of Subsystem flash erase ,write and read
 Remarks
 Author:     lawrence
\brief      flash data area for store hardware version number 
*/
//--------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"


#include "flash_operation.h"

//--------------------------------------------------------------------------------------------------
/*!
\brief   erase flash block 
\author  Lawrence
\date    2013-4-11
\param   ers_addr , the last address of data flash which is to be erased
ers_addr must be odd 
\return  erase status
\test
\test-date: 2013-06-13
\n by: Spring Zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test:Pass, 100% code coverage
\n result Lint Level 3:OK
\n bug:
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Erase_Flashblock(TUSIGN16 *ers_addr)
{
#if 0
    TUSIGN16 erase_result;                      /* Erase result */
    Enter_EW1Mode();
    
    *ers_addr = ERS_CMD1;                            /* Block erase command write */
    *ers_addr = ERS_CMD2;                            /* Block erase command write */
    /* Ready check (0:busy / 1:ready) */
    
    while( fmr00 == 0 ) ;                             /* Ready check (0:busy / 1:ready) */
    
    Exit_EW1Mode();
    /* Erase status check */
    if( fmr07 == 1 )
    {
        erase_result = ERASE_ERR;
    }
    else
    {
        erase_result = COMPLETE;
    }
    
    return  erase_result;
#endif
    return  COMPLETE;
}


//--------------------------------------------------------------------------------------------------
/*!
\brief   write flash block 
\author  Lawrence
\date    2013-4-11
\param   write_data , pointer of data to be write
write_addr , the flash address ,note to be multiple of 4
len , write data length, number of TUSIGN16, and must be multiple of 2
\return  write status
\test
\test-date: 2013-06-13
\n by: Spring Zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test:Pass, 100% code coverage
\n result Lint Level 3:OK
\n bug:
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Write_Flashdata(const TUSIGN16 *write_data,TUSIGN16 * write_addr,TUSIGN16 len)
{
#if 0
    TUSIGN16 program_result = (TUSIGN16)COMPLETE;         /* Program result */
    volatile TUSIGN16 *p = write_addr;
    TUSIGN16 i;                                  /* Loop counter */
    
    if((write_addr<BLOCK_A)||(write_addr>ERS_BLOCK_B))
    {
        return (TUSIGN16)DATA_PROGRAM_ERR;  
    } 
    
    Enter_EW1Mode();
    
    p[0] = STR_CLR;
    
    
    /* Write record data */
    for( i = 0; i < len; i+=2 )
    {      
        p[i] = PRG_CMD;                                 /* Program command write */
        p[i] = *(write_data + i);                             /* Lower dara write */
        p[i] =  *(write_data + i+ 1);                       /* Upper dara write */      
        
        while( FMR0_bit.FMR00 == 0 );    // wait while flash is busy
        
        // read the erase status flag in the status register
        if( (FMR0_bit.FMR06 != 0) || (FMR0_bit.FMR07!=0) )
        {
            program_result = DATA_PROGRAM_ERR;
            break;
        }
    }
    
    Exit_EW1Mode();
    
    return program_result;
#endif
    return COMPLETE;
}

//--------------------------------------------------------------------------------------------------
/*!
\brief   read flash block 
\author  Lawrence
\date    2013-4-11
\param   read_data 
read_addr 
len 
\return  none
\test
\test-date: 2013-06-13
\n by: Spring Zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test:Pass, 100% code coverage
\n result Lint Level 3:OK
\n bug:
*/
//--------------------------------------------------------------------------------------------------
void Read_Flashdata(TUSIGN8 *read_data,const TUSIGN8 * read_addr,TUSIGN16 len)
{
#if 0
    TUSIGN16 i; 
    PRCR_bit.PRC1 = 1;     // PM Write enabled 
    FMR1_bit.FMR17 = 0;    // One wait is inserted to the read cycle of the data flash
    PM1_bit.PM10 = 1;      // Data flash (0E000h to 0FFFFh)
    
    for(i=0;i<len;i++)
    {
        *(read_data+i) =   *(read_addr+i);   
    }
    
    PM1_bit.PM10 = 0;      // CS2 (0E000h to 0FFFFh)
    PRCR_bit.PRC1 = 0;      // PM Write disabled     
#endif
}
