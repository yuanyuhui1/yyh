//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2019.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         4WCTW
 Module         Webserver - SW pack info
 Description    Functions which enable holding sw package information
 Remarks        
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "Services/Interface/string_helper.h"
#include "Services/Interface/checksum.h"

#include "FS_interface.h"

#include <stdio.h>

#include "OSPL_memory.h"

typedef struct
{
  TUSIGN32 state[8];
  TUSIGN32 count[2];
  TUSIGN8 buffer[64];
} T_SHA256;

#define GetBe32(p) ( \
    ((TUSIGN32)((const TUSIGN8 *)(p))[0] << 24) | \
    ((TUSIGN32)((const TUSIGN8 *)(p))[1] << 16) | \
    ((TUSIGN32)((const TUSIGN8 *)(p))[2] <<  8) | \
             ((const TUSIGN8 *)(p))[3] )
#define SetBe32(p, v) { TUSIGN8 *_ppp_ = (TUSIGN8 *)(p); TUSIGN32 _vvv_ = (v); \
    _ppp_[0] = (TUSIGN8)(_vvv_ >> 24); \
    _ppp_[1] = (TUSIGN8)(_vvv_ >> 16); \
    _ppp_[2] = (TUSIGN8)(_vvv_ >> 8); \
    _ppp_[3] = (TUSIGN8)_vvv_; }

//#include "RotateDefs.h"
//#define rotlFixed(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define rotrFixed(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define _SFX // = no unrolling = no speed optimization
/* define it for speed optimization */
#ifndef _SFX
#define _SHA256_UNROLL
#define _SHA256_UNROLL2
#endif

/* #define _SHA256_UNROLL2 */

//--------------------------------------------------------------------------------------------------
/*!
 \brief    Initialize SHA256 calculation
 \author   Igor Pavlov, Klaus Pose
 \date     2017-04-03 , 2020-02-13
 \param    [i/o] p pointer to the T_SHA256 structured data instance
 \return   void
 \warning
 \bug
 \test
 \n test-date:          2020-02-20
 \n by:                 Klaus Pose
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          code review against buffer, pointer and branch fatals
 \n result module test: OK
 \n result Lint check:  OK
*/
//--------------------------------------------------------------------------------------------------
void Sha256_Init(T_SHA256 *p)
{
  p->state[0] = 0x6a09e667;
  p->state[1] = 0xbb67ae85;
  p->state[2] = 0x3c6ef372;
  p->state[3] = 0xa54ff53a;
  p->state[4] = 0x510e527f;
  p->state[5] = 0x9b05688c;
  p->state[6] = 0x1f83d9ab;
  p->state[7] = 0x5be0cd19;
  p->count[0] = 0;
  p->count[1] = 0;
}

#define S0(x) (rotrFixed(x, 2) ^ rotrFixed(x,13) ^ rotrFixed(x, 22))
#define S1(x) (rotrFixed(x, 6) ^ rotrFixed(x,11) ^ rotrFixed(x, 25))
#define s0(x) (rotrFixed(x, 7) ^ rotrFixed(x,18) ^ (x >> 3))
#define s1(x) (rotrFixed(x,17) ^ rotrFixed(x,19) ^ (x >> 10))

#define blk0(i) (W[i])
#define blk2(i) (W[i] += s1(W[((i)-2)&15]) + W[((i)-7)&15] + s0(W[((i)-15)&15]))

#define Ch(x,y,z) (z^(x&(y^z)))
#define Maj(x,y,z) ((x&y)|(z&(x|y)))

#ifdef _SHA256_UNROLL2

#define R(a,b,c,d,e,f,g,h, i) \
    h += S1(e) + Ch(e,f,g) + K[(i)+(size_t)(j)] + (j ? blk2(i) : blk0(i)); \
    d += h; \
    h += S0(a) + Maj(a, b, c)

#define RX_8(i) \
  R(a,b,c,d,e,f,g,h, i); \
  R(h,a,b,c,d,e,f,g, i+1); \
  R(g,h,a,b,c,d,e,f, i+2); \
  R(f,g,h,a,b,c,d,e, i+3); \
  R(e,f,g,h,a,b,c,d, i+4); \
  R(d,e,f,g,h,a,b,c, i+5); \
  R(c,d,e,f,g,h,a,b, i+6); \
  R(b,c,d,e,f,g,h,a, i+7)

#define RX_16  RX_8(0); RX_8(8);

#else

#define a(i) T[(0-(i))&7]
#define b(i) T[(1-(i))&7]
#define c(i) T[(2-(i))&7]
#define d(i) T[(3-(i))&7]
#define e(i) T[(4-(i))&7]
#define f(i) T[(5-(i))&7]
#define g(i) T[(6-(i))&7]
#define h(i) T[(7-(i))&7]

#define R(i) \
    h(i) += S1(e(i)) + Ch(e(i),f(i),g(i)) + K[(i)+(size_t)(j)] + (j ? blk2(i) : blk0(i)); \
    d(i) += h(i); \
    h(i) += S0(a(i)) + Maj(a(i), b(i), c(i)) \

#ifdef _SHA256_UNROLL

#define RX_8(i)  R(i+0); R(i+1); R(i+2); R(i+3); R(i+4); R(i+5); R(i+6); R(i+7);
#define RX_16  RX_8(0); RX_8(8);

#else

#define RX_16  TUSIGN8 i; for (i = 0; i < 16; i++) { R(i); }

#endif

#endif

static const TUSIGN32 K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void Sha256_WriteByteBlock(T_SHA256 *p)
{
  TUSIGN32 W[16];
  TUSIGN8 j;
  TUSIGN32 *state;

  #ifdef _SHA256_UNROLL2
  TUSIGN32 a,b,c,d,e,f,g,h;
  #else
  TUSIGN32 T[8];
  #endif

  for (j = 0; j < (16-3); j += 4)
  {
    const TUSIGN8 *ccc = p->buffer + j * 4;
    W[j    ] = GetBe32(ccc);
    W[j + 1] = GetBe32(ccc + 4);
    W[j + 2] = GetBe32(ccc + 8);
    W[j + 3] = GetBe32(ccc + 12);
  }

  state = p->state;

  #ifdef _SHA256_UNROLL2
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  f = state[5];
  g = state[6];
  h = state[7];
  #else
  for (j = 0; j < 8; j++)
    T[j] = state[j];
  #endif

  for (j = 0; j < (64-15); j += 16)
  {
    RX_16
  }

  #ifdef _SHA256_UNROLL2
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  state[5] += f;
  state[6] += g;
  state[7] += h;
  #else
  for (j = 0; j < 8; j++)
    state[j] += T[j];
  #endif

  /* Wipe variables */
  /* memset(W, 0, sizeof(W)); */
  /* memset(T, 0, sizeof(T)); */
}

#undef S0
#undef S1
#undef s0
#undef s1

//--------------------------------------------------------------------------------------------------
/*!
 \brief    Update SHA256 calculation with some data
 \author   Igor Pavlov, Klaus Pose
 \date     2017-04-03 , 2020-02-13
 \param    [i/o] p pointer to the T_SHA256 structured data instance
 \param    [in]  data pointer to the data portion
 \param    [in]  size amount of data bytes to be used for calculation
 \return   void
 \warning
 \bug
 \test
 \n test-date:          2020-02-20
 \n by:                 Klaus Pose
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          code review against buffer, pointer and branch fatals
 \n result module test: OK
 \n result Lint check:  OK
*/
//--------------------------------------------------------------------------------------------------
void Sha256_Update(T_SHA256 *p, const TUSIGN8 *data, size_t size)
{
  if (size == 0)
    return;

  {
    TUSIGN8 pos = (TUSIGN8)p->count[0] & 0x3F;
    TUSIGN8 num;

    p->count[0] += (uint32_t) size;
    p->count[0] &= 0xFFFFFFFF;

    if( p->count[0] < (uint32_t) size )
        p->count[1]++;

    num = 64 - pos;
    if (num > size)
    {
      (void)memcpy(p->buffer + pos, data, size);
      return;
    }

    size -= num;
    (void)memcpy(p->buffer + pos, data, num);
    data += num;
  }

  for (;;)
  {
    Sha256_WriteByteBlock(p);
    if (size < 64)
      break;
    size -= 64;
    (void)memcpy(p->buffer, data, 64);
    data += 64;
  }

  if (size != 0)
    (void)memcpy(p->buffer, data, size);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief    Finalize SHA256 calculation, provide digest
 \author   Igor Pavlov, Klaus Pose
 \date     2017-04-03 , 2020-02-13
 \param    [i/o] p pointer to the T_SHA256 structured data instance
 \param    [out] digest pointer to the digest (32 Bytes)
 \param    [in] size number of bytes provided at digest by the caller, shall be multiple of 4
 \return   void
 \warning
 \bug
 \test
 \n test-date:          2020-02-20
 \n by:                 Klaus Pose
 \n environment:        4WCTW, coriolis project, HEW, Renesas E1
 \n intention:          verify output with online SHA256 calculator
 \n result module test: OK (reversed byte order but values identical)
 \n result Lint check:  OK
*/
//--------------------------------------------------------------------------------------------------
void Sha256_Final(T_SHA256 *p, TUSIGN8 *digest, TUSIGN8 size)
{
  TUSIGN8 pos = (TUSIGN8)p->count[0] & 0x3F;
  TUSIGN8 i;

  p->buffer[pos++] = 0x80;

  while (pos != (64 - 8))
  {
    pos &= 0x3F;
    if (pos == 0)
      Sha256_WriteByteBlock(p);
    p->buffer[pos++] = 0;
  }

  {
    unsigned long long numBits = (((unsigned long long)p->count[1] << 32) | ((unsigned long long)p->count[0])) << 3;
    
    SetBe32(p->buffer + 64 - 8, (TUSIGN32)(numBits >> 32));
    SetBe32(p->buffer + 64 - 4, (TUSIGN32)(numBits));
  }

  Sha256_WriteByteBlock(p);

  for (i = 0; i < (8-1); i += 2)
  {
    TUSIGN32 v0 = p->state[i];
    TUSIGN32 v1 = p->state[i + 1];
    if( size >= 4 ) {
      SetBe32(digest    , v0);
      size -= 4;
    }
    if( size >= 4 ) {
      SetBe32(digest + 4, v1);
      size -= 4;
    }
    digest += 8;
  }

  Sha256_Init(p);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Calculates Sha256 checksum of file
 \author    Michal Kochel
 \date      2020-03-05
 \param     filePath Path to file
 \param     checksum Buffer where checksum is stored
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
#define CHECKSUM_HANDLING_BUFFER_SIZE_BIG 256
#define CHECKSUM_HANDLING_BUFFER_SIZE_SMALL 128

TBOOL GetFileChecksum(const TCHAR* filePath, TUSIGN8* checksum, TUSIGN8 checksumSize) {
    FS_fileSystem_t* mainFs;
    FS_file_t* file;
    int32_t fsResult;
    T_SHA256 sha256context;
    TUSIGN8* buffer;
    int32_t readBytes;
    size_t bufferSize = CHECKSUM_HANDLING_BUFFER_SIZE_BIG;

    if (OSPL_MemAllocateFast(CHECKSUM_HANDLING_BUFFER_SIZE_BIG, (uint8_t **)&buffer) != OSPL_SUCCESS) {
        if (OSPL_MemAllocateFast(CHECKSUM_HANDLING_BUFFER_SIZE_SMALL, (uint8_t **)&buffer) != OSPL_SUCCESS) {
            return eFALSE;
        }
        bufferSize = CHECKSUM_HANDLING_BUFFER_SIZE_SMALL;
    }

    fsResult = FS_GetInstance(FS_instanceMain, &mainFs);
    if (fsResult == FS_OK) {
        fsResult = FS_FileOpen(mainFs, (const char*)filePath, FS_modeRead, &file);
        if (fsResult == FS_OK) {
            Sha256_Init(&sha256context);

            while ((readBytes = FS_FileRead(mainFs, file, (uint8_t*)buffer, bufferSize)) > 0) {
                Sha256_Update(&sha256context, buffer, (size_t)readBytes);
            }

            Sha256_Final(&sha256context, checksum, checksumSize);

            (void)FS_FileClose(mainFs, &file);
        }
    }

    (void)OSPL_MemFreeFast((uint8_t *)buffer);

    return eTRUE;
}
