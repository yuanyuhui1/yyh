/*
 * sha2.h
 *
 * Description: modify basing on file sha2.h which is created by
 *                                                           \author     Simon Draper
 *                                                           \date       27/10/2015
 * modifier:    Grace Mu
 * modify date: 2018/06/25
 * SN for change: APP20180709
 */

#ifndef TI_SHA_H_
#define TI_SHA_H_

#define CALC_BLOCK_SIZE_SHA256              (64u)
#define CALC_OVERFLOW_THRESHOLD_SHA256      (56u)
   
#define MIN_MSGLEN_USIGN32_SHA256           (16u)   // Minimum number 32bit words needed for message buffer
#define MIN_MSGLEN_USIGN8_SHA256            (64u)   // Minimum number bytes needed for message buffer

#define N_USIGN32_FOR_CALC_OVERFLOW_SHA256  (16u)   // Needed only if MessageLengthBytes%64 > 55
#define N_USIGN8_FOR_CALC_OVERFLOW_SHA256   (64u)   // Needed only if MessageLengthBytes%64 > 55

#define N_LONGWORDS_DEST_HASH_SHA256        (8u)    // Number of 32bit words needed to hold resultant hash


#ifndef NULL
    #define NULL                            0
#endif


/*!
 \brief Calcuates an SHA-256 hash for the specified message.

 \param srcMsg          Pointer to the MSB of the message to be hashed (an array of 32bit words).
                        Notes:  !The message must be in BIG ENDIAN
                                !The function calling SHA_256 must reserve extra space in multiples
                                of 64 bytes following the message in order for padding operations 
                                to be performed. 

 \param msgLenBytes     The length of the message to be hashed, in bytes. <= 56 

 \param destHash        A pointer to the MSB of the location the hash will be placed at.
                        Notes:  !This location should not be the same as the location of the message.
                                !The function calling SHA_256 must reserve 32 bytes at this location.
*/
extern void SHA_256 ( TUSIGN32 *Message, TUSIGN32 MessageLengthBytes, TUSIGN32 *Hash);


extern TUSIGN16 Get32_BigEndian(const TUSIGN8 data[], TUSIGN32 *result);


#endif /* SHA_H_ */
