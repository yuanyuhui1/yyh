//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Rainbow
 Module         CHIP_HANDLER
 Description    interface definition of subsystem CHIP_HANDLER part of NV_MEM
 <pre>
    This CHIP_HANDLER handles up to 8 eeproms via one I²C-Bus. Or in a more OO like language, it is
    the implementation of the singleton SingleI2Ceeprom-handler that is a child of CHIP_HANDLER.
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------


#ifndef __SYSTEM_H__
  #error system.h must include before chip_handler.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must include before chip_handler.h
#endif

#ifdef __CHIP_HANDLER_H__
  #error chip_handler.h included more than once
#endif

#define __CHIP_HANDLER_H__

//----------------------------------------------------------------------------------------------------------
//! error-code
enum E_RESULT_CH
{
    /* 0*/CH_OK,             //!< OK
    /* 1*/CH_DATA_ERROR,     //!< read/write incomplete or comunication-error, or checksum error
    /* 2*/CH_ADR_ERROR,      //!< data stored to wrong address
    /* 3*/CH_INIT_FAILED     //!< initialization failed
};

enum E_PAGES
{
    CH_MAIN_PAGE,
    CH_BACKUP_PAGE,

    CH_NUMOFPAGES
};

#define NV_SIZE			8192   // 8K (+ 8K backup = 16K physically)
#define MAX_NV_SIZE		8192   // 8K (+ 8K backup = 16K physically)

#define CH_SEGMENT_LENGTH    32
#define CH_SEG_NETTO_LENGTH  30
#define CH_NUM_RETRY          4
#define CH_SEGMENTS_PER_PAGE (NV_SIZE / CH_SEGMENT_LENGTH)
#define CH_MAX_SEGMENTS_PER_PAGE (MAX_NV_SIZE / CH_SEGMENT_LENGTH)
#define CH_REALLY_NEED_SEGMENTS 32

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Initialize the CHIP_HANDLER
 <pre>
    If already initialized THEN return OK
    ELSE
      initialize I²C-Bus
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
void Initialize_CHIPHANDLER(TUSIGN8 mainLocation);

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
TUSIGN16 WriteData_CHIPHANDLER(TUSIGN8 page, TUSIGN16 segNum, const void FAST* ptrSource);


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
TUSIGN16 ReadData_CHIPHANDLER(TUSIGN8 page, TUSIGN16 segNum, void FAST* ptrTarget);


	
