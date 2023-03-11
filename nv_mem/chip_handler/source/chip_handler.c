//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Rainbow
 Module         CHIP_HANDLER
 Description    implementation of subsystem CHIP_HANDLER part of NV_MEM
 <pre>
    This CHIP_HANDLER handles up to 8 eeproms via one I�C-Bus. Or in a more OO like language, it is
    the implementation of the singleton SingleI2Ceeprom-handler that is a child of CHIP_HANDLER.
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <string.h>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "Common/Interface/common_functions.h"
#include "System/Interface/assert.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/chip_handler/interface/chip_handler_segtab.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/nv_service/Interface/nv_service.h"
#include "nv_mem/SPI_Bus/SPI_Bus.h"

//--------------------------------------------------------------------------------------------------
// modul global variables
//--------------------------------------------------------------------------------------------------
TBOOL cyclicCorruptionCommon = eFALSE;
TBOOL cyclicCorruptionUncommon = eFALSE;

static TUSIGN8 mMainLocation;         //!< ==0 -> main-Table hold main-segment
static RTOS_MUTEX_HANDLE mChipHandler_Mutex;

static TUSIGN8 mReadBuffer[CH_SEGMENT_LENGTH];
static TUSIGN8 mWritebuffer[CH_SEGMENT_LENGTH];

static TBOOL BufferCmp(TUSIGN8* pBuffer1, TUSIGN8* pBuffer2, TUSIGN16 BufferLength);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Initialize the CHIP_HANDLER
 <pre>
    If already initialized THEN return OK
    ELSE
      initialize SPI-Bus
      initialize mainLocation
      initialize semaphore
    ENDIF
 </pre>
 \author Heiko Kresse
 \date   2004-06-02
 \param  mainLocation; 0 -> MainSegment in MainTab;  1-> MainSegment in BackupTab
 \return TUSIGN16
 <pre>
    CH_OK
    CH_INIT_FAILED
 </pre>
  \test
  \n by: Heiko Kresse
  \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
  \n result module test: 2004-08-30 OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void Initialize_CHIPHANDLER(TUSIGN8 mainLocation)
{
    EEPROM_SPI_Init();
    CreateRecursive_RTOS_MUTEX(mChipHandler_Mutex);
    mMainLocation = mainLocation & 1;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  privat; translate page and segNum via segment-table to address (chip + addr)
 \author Heiko Kresse
 \date   2004-06-03
 \param  page MAIN- or BACKUP-PAGE
 \param  segNum; segment number 0 - SEG_PER_PAGE-1
 \param  ptrChip returns the chip bus-address
 \param  ptrAddr returns the address inside the chip
 \warning page and segNum must be validated befor calling GetAddress() !!
  \test
  \n by: Heiko Kresse
  \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
  \n result module test: 2004-08-30 OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void GetAddress(TUSIGN8 page, TUSIGN16 segNum, TUSIGN8* ptrChip, union U_ADDR* ptrAddr)
{
    TUSIGN8 chip;

    //lint -e{746} Heiko Kresse 2004-11-03
    VIP_ASSERT(ptrChip);
    VIP_ASSERT(ptrAddr);
    // validate parameter and chip-handler state
    VIP_ASSERT(page<CH_NUMOFPAGES);
    VIP_ASSERT(segNum<CH_SEGMENTS_PER_PAGE);

    // get segment address
    chip = cmSegTab[page][segNum].chip;

    // check if main-location is variable
    if( chip & VARIABLE_MAINLOCATION_FLAG )
    {
        page = page ^ mMainLocation;
        chip = cmSegTab[page][segNum].chip;
    }

    *ptrChip = chip & ~VARIABLE_MAINLOCATION_FLAG;
    ptrAddr->u16 = cmSegTab[page][segNum].addr.u16;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  privat; read one segment into the read-buffer
 \author Heiko Kresse
 \date   2004-06-03
 \param  chip: bus-address of the nov-chip
 \param  addr: address inside the nov-chip
 \return TUSIGN16
 <pre>
    CH_OK
    CH_DATA_ERROR
    CH_ADR_ERROR    actual segment is damaged (missed start-condition)
 </pre>
 \warning call this method only in regions protected by the resource-semaphore
  \test
  \n by: Heiko Kresse
  \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
  \n result module test: 2004-08-30 OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
static TUSIGN16 ReadSegment(TUSIGN8 chip, union U_ADDR addr)
{
    TUSIGN16 result = CH_OK;
    TUSIGN16 retryCnt = 0;
    TUSIGN16 u;
    union U_ADDR  chkSum;
    chkSum.u16 = 0x0;

    do
    {
        EEPROM_SPI_ReadData(addr.u16, mReadBuffer, CH_SEGMENT_LENGTH);

        chkSum.u16 = InitCRC16_COMMON();
        chkSum.u16 = CalcCRC16_COMMON(chip, chkSum.u16);
        chkSum.u16 = CalcCRC16_COMMON(addr.u8[1], chkSum.u16);
        chkSum.u16 = CalcCRC16_COMMON(addr.u8[0], chkSum.u16);

        for(u=0; u<CH_SEG_NETTO_LENGTH; u++)
        {
            chkSum.u16 = CalcCRC16_COMMON(mReadBuffer[u], chkSum.u16);
        }

        if((mReadBuffer[CH_SEG_NETTO_LENGTH] == chkSum.u8[1]) 
        && (mReadBuffer[CH_SEG_NETTO_LENGTH+1] == chkSum.u8[0]))
        {
            result = CH_OK;
        }
        else
        {
            // check if all received data are 0xff --> SDA allways high --> missed start-condition
            for(u=0; (u<CH_SEGMENT_LENGTH) && (mReadBuffer[u]==0xFF); u++)
            {
            }
            
            // actual segment is overwritten; FRAM also the next segment
            if( u<CH_SEGMENT_LENGTH )
            {
                result = CH_ADR_ERROR;
            }
            else
            {
                result = CH_DATA_ERROR;
            }
        }
        retryCnt++;
    }
    while((result == CH_DATA_ERROR) && (retryCnt <= CH_NUM_RETRY));

    return result;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Write segment addressed by Page and Segmentnumber into the non-volatile storage
         In case of an CH_DATA_ERROR the segment will be written again.
 \author Heiko Kresse
 \date   2004-06-02
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  segNum; segment number 0 - SEG_PER_PAGE-1
 \param  ptrSrcSeg; pointer to the segment to be stored
 \return TUSIGN16
 <pre>
    CH_OK           OK
    CH_DATA_ERRO    read/write incomplete or comunication-error, or checksum error
    CH_ADR_ERROR    data stored to wrong address
 </pre>
 \warning the function is protected by a resource semaphore, so a call of this function could be delayed
          by the time of one WriteData_CHIPHANDLER-call (worst-case)
 \warning a segment has a length of CH_SEG_NETTO_LENGTH!
  \test
  \n by: Heiko Kresse
  \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
  \n result module test: 2004-08-30 OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 WriteData_CHIPHANDLER(TUSIGN8 page, TUSIGN16 segNum, const void* ptrSrcSeg)
{
    TUSIGN16 result = CH_OK;
    union U_ADDR addr, chkSum;
    TUSIGN16 u, isEqual;
    TUSIGN8 chip;
    TUSIGN16 retryCnt = 0;

    // validate parameter and chip-handler state
    VIP_ASSERT(page < CH_NUMOFPAGES);
    VIP_ASSERT(segNum < CH_SEGMENTS_PER_PAGE);
    VIP_ASSERT(ptrSrcSeg);

    chkSum.u16 = 0x0;
    TakeRecursive_RTOS_MUTEX(mChipHandler_Mutex, RTOS_WAIT_FOREVER);
    GetAddress(page, segNum, &chip, &addr);
    memcpy(mWritebuffer, ptrSrcSeg, CH_SEG_NETTO_LENGTH);
    result = ReadSegment(chip, addr);

    if(result == CH_OK)
    {   // compare new data with stored data
        isEqual = BufferCmp(mReadBuffer, mWritebuffer, CH_SEG_NETTO_LENGTH);
    }
    else
    {
        isEqual = eFALSE;
    }
    
    if(eTRUE != isEqual) 
    {
        // prepare write-buffer
        chkSum.u16 = InitCRC16_COMMON();
        chkSum.u16 = CalcCRC16_COMMON(chip, chkSum.u16);
        chkSum.u16 = CalcCRC16_COMMON(addr.u8[1], chkSum.u16);
        chkSum.u16 = CalcCRC16_COMMON(addr.u8[0], chkSum.u16);

        for(u = 0; u < CH_SEG_NETTO_LENGTH; u++)
        {
            chkSum.u16 = CalcCRC16_COMMON(mWritebuffer[u], chkSum.u16);
        }

        mWritebuffer[u++] = chkSum.u8[1];
        mWritebuffer[u++] = chkSum.u8[0];

        do
        {
            EEPROM_SPI_WriteData(addr.u16, mWritebuffer, CH_SEGMENT_LENGTH);
            Delay_RTOS_TASK(BR25G128_WRITE_TIMEOUT);
            // verify
            result = ReadSegment(chip, addr);

            if(result==(TUSIGN16)CH_OK)
            {
                if(eTRUE != BufferCmp(mReadBuffer, mWritebuffer, CH_SEG_NETTO_LENGTH)) 
                {
                    // in this case it is possible that the data is stored anywhere
                    result = (TUSIGN16)CH_ADR_ERROR;
                }
            }
            retryCnt++;
        }
        while(!((result == CH_OK) || (result==CH_ADR_ERROR) || (result == CH_DATA_ERROR)) && (retryCnt <= CH_NUM_RETRY));
    }

    GiveRecursive_RTOS_MUTEX(mChipHandler_Mutex);

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Read segment addressed by Page and Segmentnumber into the non-volatile storage.
         In case of an CH_DATA_ERROR the segment will be read again.
 \author Heiko Kresse
 \date   2004-06-02
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  segNum; segment number 0 - SEG_PER_PAGE-1
 \param  ptrTarget; pointer to the target storage
 \return TUSIGN16
 <pre>
    CH_OK           OK
    CH_DATA_ERRO    read/write incomplete or comunication-error, or checksum error
    CH_ADR_ERROR    actual segment is damaged (missed start-condition)
 </pre>
 \warning the function is protected by a resource semaphore, so a call of this function could be delayed
          by the time of one WriteData_CHIPHANDLER-call (worst-case)
  \test
  \n by: Heiko Kresse
  \n intention: 1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
  \n result module test: 2004-08-30 OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 ReadData_CHIPHANDLER(TUSIGN8 page, TUSIGN16 segNum, void* ptrTarget)
{
    TUSIGN16 result = (TUSIGN16)CH_OK;
    union U_ADDR addr;
    TUSIGN8 chip;

    // validate parameter and chip-handler state
    VIP_ASSERT(page < CH_NUMOFPAGES);
    VIP_ASSERT(segNum < CH_SEGMENTS_PER_PAGE);
    VIP_ASSERT(ptrTarget);

    TakeRecursive_RTOS_MUTEX(mChipHandler_Mutex, RTOS_WAIT_FOREVER);
    GetAddress(page, segNum, &chip, &addr);
    result = ReadSegment(chip, addr);

    if(result == CH_OK)
    {
        memcpy(ptrTarget, mReadBuffer, CH_SEG_NETTO_LENGTH);
    }

    GiveRecursive_RTOS_MUTEX(mChipHandler_Mutex);

    return result;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval   : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static TBOOL BufferCmp(TUSIGN8* pBuffer1, TUSIGN8* pBuffer2, TUSIGN16 BufferLength)
{
  while (BufferLength--)
  {
    if((*pBuffer1) != *pBuffer2)
    {
      return eFALSE;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return eTRUE;
}