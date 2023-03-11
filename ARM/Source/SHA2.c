//-------------------------------------------------------------------------------------------------
/*!
\file       SHA2.C
\copyright  Copyright ABB, 2015.
            All rights reserved. Reproduction, modification,
            use or  disclosure to third parties without express authority is forbidden.
            See below for copyright of the original Texas Instruments source code that this
            is based on.
\ingroup    Aquamaster4
\brief      Implementation of SHA-2 secure hashing algorithm.
            This source code is based on C-source module from Texas Instruments refer to
            their document SHA_256_080614).
            It has been modified by ABB to work with the AquaMaster4 software framework.
\author     Simon Draper
\date       27/10/2015
\SN for change: APP20180709
*/
//-------------------------------------------------------------------------------------------------
/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

 /*
 * sha2.c
 *
 *  Created on: March 13, 2012
 *      Author: Jace Hall
 *
 *  Description: Implementation of the SHA-256 as defined by the FIPS PUB 180-3:
 *  the official SHA-256 standard
 */

/*===================================================================
// NAME: void SHA_256 ( TUSIGN32 *Message, uint64_t Mbit_Length, TUSIGN32 *Hash);
//
// BRIEF: Is used to execute SHA-256 hashing algorithm on the data referenced by Message.
//        *Hash will contain the final hashing at completion of function.
//
// INPUTS:      TUSIGN32 *Message -- Pointer to array of 32-bit long to be hashed. Size of array must be a multiple of a hashing block.
//                                        ( I.e. 512 bits or sixteen 32-bit longs)
//              uint64_t Mbit_Length --  64-bit value containing the precise number of bits to be hashed within Message[].
//                                      **Note: If Mbit_Length %(mod) 512 is >= 448 bits, then an additional hashing block is needed. 
//                                        User must allocate the additional 512 bits
//              TUSIGN32 *Hash  --  pointer to hashing array. Final hash will be stored here.
//                                  size of array should equal 8 32-bit longs
//              short  mode --  If Mode =='0', SHA-224 is used, all else SHA-256
//
// OUTPUTS:      results stored at given pointers. Final Hash stored at Hash pointer.
//
// Process:
//
// Note:
//
// CHANGE:
// DATE             WHO Detail
// 13March2012      JH  Original Code
// 26March2012      JH  Comments added.
//                      Pre-processing halfway working
// 13April2012      JH  Pre-processing working
// 08May2012        JH  Mode added for SHA-224 along with intial hash values for SHA-224
// 11June2012       JH  SHA algorithm tessted against NIST test vectors. Pass.
// 09July2012       JH  Copyright added along with additional comments. Changed file names.
// 13Aug2014        AN  Fixed initial hash value for 224 and byte%4=0 masking issue
//==================================================================*/


/* This code being developed to implement SHA-244/256 on the MSP430.
* This code is by no means  optimized as of this moment.
* The object is to develop an understandable implementation of SHA-2 on the MSP430
* The algorithm will be used as a function call with inputs being a pointer to the message
* needing encryption, the length of the message in longs and a pointer to the
* Hash (size of 8 longs) array in which will contain the answer after the function is done.
*/



#include <intrinsics.h>
//#include <stdint.h>
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "System/Interface/system.h"
#include "sha2.h"



/*** SHA Function Macros *******************************/

#define SR(x,a)         ((x) >> (a))                         /*Shift right Function - Shift x by a  */
#define ROTR(x,n)       (( (x) >> (n) ) | ( (x) << (32u - (n) ))) /* Rotate Right Function - rotate x by n */
#define ROTR25(x)       ((((x) >> 9u) >> 16u) | ( (x) << 7u ))

/* Algorithm defined logical functions */
#define Ch(x , y, z)    (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z)    (((x) & (y)) ^ ((x) & (z)) ^ ( (y) & (z)))

/*Alternate Ch and MAJ functions that could be faster.*/        //With IAR optimixations, this method is not faster.
//#define Ch(x, y, z)      (((x) & ((y) ^ (z))) ^ (z))
//#define Maj(x, y, z)     (((x) & ((y) | (z))) | ((y) & (z)))
#define SIGZ(x)         (ROTR((x),2u) ^ ROTR((x),13u) ^ ROTR((x),22u))
#define SIG1(x)         (ROTR((x),6u) ^ ROTR((x),11u) ^ ROTR((x),25u))   //(ROTR(x,6)  ^ ROTR(x,11) ^ ROTR25(x))
#define sigmaZ(x)       (ROTR((x),7u) ^ ROTR((x),18u) ^ SR((x),3u))
#define sigma1(x)       (ROTR((x),17u) ^ ROTR((x),19u) ^ SR((x),10u))


// Fordward declarations of private methods.
void shaHelper( TUSIGN32 * message, TUSIGN32 * Hash);


/*** SHA-XYZ INITIAL HASH VALUES AND CONSTANTS ************************/
// Hash constant words K for SHA-256:
static const TUSIGN32 K256[64u] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf,
    0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98,
    0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
    0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
    0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8,
    0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
    0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e,
    0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c,
    0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee,
    0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
    0xc67178f2
};

// Initial hash value H for SHA-256:
static const TUSIGN32 Initial_Hash[8u] =
{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };


//-------------------------------------------------------------------------------------------------
/*!
\brief      Pick out the first 4 bytes in data to get a word
\param      data The data to read from
\param      result The place to put the result
\return     The number of bytes read
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Get32_BigEndian(const TUSIGN8 data[], TUSIGN32 *result)
{
    VIP_ASSERT(data != NULL);
    VIP_ASSERT(result != NULL);
    *result = (TUSIGN32) data[3];
    *result |= ((TUSIGN32) data[2] << 8);
    *result |= ((TUSIGN32) data[1] << 16);
    *result |= ((TUSIGN32) data[0] << 24);
    return 4u;
}

void SHA_256 ( TUSIGN32 *Message, TUSIGN32 MessageLengthBytes, TUSIGN32 *Hash)
{
    
    /*Variable Declarations go here*/
    TUSIGN16 leftoverlong = 0u;
    TUSIGN16 leftoverbits = 0u;
    //TUSIGN32 Nblocks = (TUSIGN32)0;
    TUSIGN16 i = 0u;
    TUSIGN16 v = 0u;
    TUSIGN32 M_Length;
    TUSIGN32 Mbit_Length;
    TUSIGN32 onemask  = 0uL;
    TUSIGN32 zeromask = 0uL;

    VIP_ASSERT(Message != NULL);
    VIP_ASSERT(Hash != NULL);
    VIP_ASSERT(MessageLengthBytes <= CALC_OVERFLOW_THRESHOLD_SHA256);


    /* Pre-processing:
    * 1. Initialize Hash Values 2. Parse the Message block 3. Padd the message block*****/
    for (i=0u; i<=7u; i++)
    {
        Hash[i] = Initial_Hash[i];
    }  // Initialize Hash for SHA-256


    /* Message Parsing */
    Mbit_Length  = (TUSIGN32)MessageLengthBytes*8u;
    leftoverbits = (TUSIGN16)(Mbit_Length % 32u);   // leftover bits in last 32bit word
    M_Length  = MessageLengthBytes >> 2u;   // Converting Bit length of message to How many 32bit words in a message  
    if(leftoverbits != 0)
    {
        M_Length++;
    }
    leftoverlong = (TUSIGN16)(M_Length % 16u);     // leftover 32bit words not in a full bucket
    


    /* Message Padding: The next set of statements finds the end of a message, appends a 1, then adds 0's
    * to pad the message to a 512bit chunk. The length of the original message is parsed into the last 2 bytes**/
    onemask = 0x80000000uL >> leftoverbits;
    zeromask = (TUSIGN32)~(0x7FFFFFFF >> leftoverbits);
    Message[M_Length]=(Message[M_Length]|onemask);
    Message[M_Length]=(Message[M_Length]&zeromask);

           
    for(v=1u; v < (14u-leftoverlong); v++)
    {
        Message[leftoverlong + v] &= 0x00000000uL; // zero pad
    }

    Message[14u] = 0;   //append bit length to end of chunk
    Message[15u] = Mbit_Length;
   
    
    shaHelper(Message, Hash);   

}//end SHA_2


// Performs an interation of the hash on 512 bits
//*****************************************************************************
//! Performs an interation of the hash on 512 bits
//!
//! \param message is a pointer to the message to be hashed
//! \param Hash is a pointer to the location of the hash output (may be an intermediate value)
//!
//! This function assumes that the previous hash value is already located at Hash
//! and that message has already be converted to the correct format, and padded
//! if necessary.
//!
//*****************************************************************************
void shaHelper( TUSIGN32 * message, TUSIGN32 * Hash)
{
    TUSIGN32 W[16u]={0};
    TUSIGN16 i= 0u;
    TUSIGN16 t =0u;
    TUSIGN32 temp1=0u;
    TUSIGN32 temp2=0u;
    TUSIGN32 a;
    TUSIGN32 b;
    TUSIGN32 c;
    TUSIGN32 d;
    TUSIGN32 e;
    TUSIGN32 f;
    TUSIGN32 g;
    TUSIGN32 h;


    /* Main algorithm  */
    /* Chunk control. Process 512 bits at a time*/
    /*Place i-1 Hash into letters. Initialize with initial hash values.*/
    a = Hash[0u];
    b = Hash[1u];
    c = Hash[2u];
    d = Hash[3u];
    e = Hash[4u];
    f = Hash[5u];
    g = Hash[6u];
    h = Hash[7u];



    for (t=0u; t < 64u; t++)
    { // need to change to do/while loop.
    
        if (t < 16u )
        {
            W[t] = message[ 16u*i + t ];
        }
        else
        {
            W[t%16u] = sigma1(W[(t-2u)%16u]) + W[(t-7u)%16u] + sigmaZ(W[(t-15u)%16u]) + W[(t-16u)%16u];
        }

        // Algorithm Proper
        temp1 = h + SIG1(e) + Ch(e, f, g) + K256[t] + W[t%16u];
        temp2 = Maj(a, b, c) +SIGZ(a) ;

        h=g;
        g=f;
        f=e;
        e=d + temp1;
        d=c;
        c=b;
        b=a;
        a= temp1 + temp2;
    }

    Hash[0u] = Hash[0u] + a;
    Hash[1u] = Hash[1u] + b;
    Hash[2u] = Hash[2u] + c;
    Hash[3u] = Hash[3u] + d;
    Hash[4u] = Hash[4u] + e;
    Hash[5u] = Hash[5u] + f;
    Hash[6u] = Hash[6u] + g;
    Hash[7u] = Hash[7u] + h;

}//end shaHelper


