//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.
 System
 Module
 Description
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before nv_startup.h
#endif


#ifdef __FLASH_OPERATION__
  #error flash_operation.h included more than once
#endif

#define __FLASH_OPERATION_H__



TUSIGN16 Erase_Flashblock(TUSIGN16 *ers_addr);
TUSIGN16 Write_Flashdata(const TUSIGN16 *write_data,TUSIGN16 * write_addr,TUSIGN16 len);
//void Read_Flashdata(TUSIGN8 *read_data,const TUSIGN8 * read_addr,TUSIGN16 len);

/**********************************************************************************/
/*      DEFINE                                                                    */
/**********************************************************************************/


#define BLOCK_A     ((TUSIGN16 *)0xE000)       /* BLOCK A first address */
#define BLOCK_B     ((TUSIGN16 *)0xF000)       /* BLOCK B first address */
#define ERS_BLOCK_A ((TUSIGN16 *)0xEFFE)       /* ERASE BLOCK A final address */
#define ERS_BLOCK_B ((TUSIGN16 *)0xFFFE)       /* ERASE BLOCK B final address */
#define ERS_BLOCK_VRESION ((TUSIGN16 *)0xE006)       /* ERASE BLOCK A final address */

#define LOCK_BIT     1


#define RECORD_SIZE         2                       /* Record size(word) */

#define BLOCK_NUMBER        2                        /* BLOCK Number (BLOCK A and BLOCK B) */

#define BLOCK_A_SELECT      0                        /* Use BLOCK A */
#define BLOCK_B_SELECT      1                        /* Use BLOCK B */

#define COMPLETE            0x00                     /* FLASH write/erase complete */
#define DATA_PROGRAM_ERR    0x01                     /* FLASH data write error */
#define ERASE_ERR           0x02                     /* FLASH erase error */


#define ERS_CMD1            0x20                   /* Block erase command 1 */
#define ERS_CMD2            0xD0                   /* Block erase command 2 */
#define PRG_CMD             0x41                   /* Program command */
#define STR_CLR             0x50                   /* Status register clear command */








