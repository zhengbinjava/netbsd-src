/* crypto/aes/aes_cfb.c -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 * Copyright (c) 2002-2006 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

#ifndef AES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>

#include <openssl/aes.h>
#include "aes_locl.h"
#include "e_os.h"

#define STRICT_ALIGNMENT
#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || \
    defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64)
#  undef STRICT_ALIGNMENT
#endif

/* The input and output encrypted as though 128bit cfb mode is being
 * used.  The extra state information to record how much of the
 * 128bit block we have used is contained in *num;
 */

void AES_cfb128_encrypt(const unsigned char *in, unsigned char *out,
	unsigned long length, const AES_KEY *key,
	unsigned char *ivec, int *num, const int enc) {

    unsigned int n;
    unsigned long l = 0;

    assert(in && out && key && ivec && num);

    n = *num;

#if !defined(OPENSSL_SMALL_FOOTPRINT)
    if (AES_BLOCK_SIZE%sizeof(size_t) == 0) {	/* always true actually */
	if (enc) {
		if (n) {
			while (length) {
				*(out++) = ivec[n] ^= *(in++);
				length--;
				if(!(n = (n + 1) % AES_BLOCK_SIZE))
					break;
			}
		}
#if defined(STRICT_ALIGNMENT)
		if (((size_t)in|(size_t)out|(size_t)ivec)%sizeof(size_t) != 0)
			goto enc_unaligned;
#endif
		while ((l + AES_BLOCK_SIZE) <= length) {
			unsigned int i;
			AES_encrypt(ivec, ivec, key);
			for (i=0;i<AES_BLOCK_SIZE;i+=sizeof(size_t)) {
				*(size_t*)(out+l+i) =
				*(size_t*)(ivec+i) ^= *(size_t*)(in+l+i);
			}
			l += AES_BLOCK_SIZE;
		}

		if (l < length) {
			AES_encrypt(ivec, ivec, key);
			do {	out[l] = ivec[n] ^= in[l];
				l++; n++;
			} while (l < length);
		}
	} else {
		if (n) {
			while (length) {
				unsigned char c;
				*(out++) = ivec[n] ^ (c = *(in++)); ivec[n] = c;
				length--;
				if(!(n = (n + 1) % AES_BLOCK_SIZE))
					break;
 			}
		}
#if defined(STRICT_ALIGNMENT)
		if (((size_t)in|(size_t)out|(size_t)ivec)%sizeof(size_t) != 0)
			goto dec_unaligned;
#endif
		while (l + AES_BLOCK_SIZE <= length) {
			unsigned int i;
			AES_encrypt(ivec, ivec, key);
			for (i=0;i<AES_BLOCK_SIZE;i+=sizeof(size_t)) {
				size_t t = *(size_t*)(in+l+i);
				*(size_t*)(out+l+i) = *(size_t*)(ivec+i) ^ t;
				*(size_t*)(ivec+i) = t;
			}
			l += AES_BLOCK_SIZE;
		}

		if (l < length) {
			AES_encrypt(ivec, ivec, key);
			do {	unsigned char c;
				out[l] = ivec[n] ^ (c = in[l]); ivec[n] = c;
				l++; n++;
			} while (l < length);
 		}
	}
	*num = n;
	return;
    }
#endif

    /* this code would be commonly eliminated by x86* compiler */
	if (enc) {
#if defined(STRICT_ALIGNMENT) && !defined(OPENSSL_SMALL_FOOTPRINT)
	    enc_unaligned:
#endif
		while (l<length) {
			if (n == 0) {
				AES_encrypt(ivec, ivec, key);
			}
			out[l] = ivec[n] ^= in[l];
			l++;
			n = (n+1) % AES_BLOCK_SIZE;
		}
	} else {
#if defined(STRICT_ALIGNMENT) && !defined(OPENSSL_SMALL_FOOTPRINT)
	    dec_unaligned:
#endif
		while (l<length) {
			unsigned char c;
			if (n == 0) {
				AES_encrypt(ivec, ivec, key);
			}
			out[l] = ivec[n] ^ (c = in[l]); ivec[n] = c;
			l++;
			n = (n+1) % AES_BLOCK_SIZE;
		}
	}

	*num=n;
}

/* This expects a single block of size nbits for both in and out. Note that
   it corrupts any extra bits in the last byte of out */
void AES_cfbr_encrypt_block(const unsigned char *in,unsigned char *out,
			    const int nbits,const AES_KEY *key,
			    unsigned char *ivec,const int enc)
    {
    int n,rem,num;
    unsigned char ovec[AES_BLOCK_SIZE*2 + 1];  /* +1 because we dererefence (but don't use) one byte off the end */

    if (nbits<=0 || nbits>128) return;

	/* fill in the first half of the new IV with the current IV */
	memcpy(ovec,ivec,AES_BLOCK_SIZE);
	/* construct the new IV */
	AES_encrypt(ivec,ivec,key);
	num = (nbits+7)/8;
	if (enc)	/* encrypt the input */
	    for(n=0 ; n < num ; ++n)
		out[n] = (ovec[AES_BLOCK_SIZE+n] = in[n] ^ ivec[n]);
	else		/* decrypt the input */
	    for(n=0 ; n < num ; ++n)
		out[n] = (ovec[AES_BLOCK_SIZE+n] = in[n]) ^ ivec[n];
	/* shift ovec left... */
	rem = nbits%8;
	num = nbits/8;
	if(rem==0)
	    memcpy(ivec,ovec+num,AES_BLOCK_SIZE);
	else
	    for(n=0 ; n < AES_BLOCK_SIZE ; ++n)
		ivec[n] = ovec[n+num]<<rem | ovec[n+num+1]>>(8-rem);

    /* it is not necessary to cleanse ovec, since the IV is not secret */
    }

/* N.B. This expects the input to be packed, MS bit first */
void AES_cfb1_encrypt(const unsigned char *in, unsigned char *out,
		      const unsigned long length, const AES_KEY *key,
		      unsigned char *ivec, int *num, const int enc)
    {
    unsigned int n;
    unsigned char c[1],d[1];

    assert(in && out && key && ivec && num);
    assert(*num == 0);

    memset(out,0,(length+7)/8);
    for(n=0 ; n < length ; ++n)
	{
	c[0]=(in[n/8]&(1 << (7-n%8))) ? 0x80 : 0;
	AES_cfbr_encrypt_block(c,d,1,key,ivec,enc);
	out[n/8]=(out[n/8]&~(1 << (7-n%8)))|((d[0]&0x80) >> (n%8));
	}
    }

void AES_cfb8_encrypt(const unsigned char *in, unsigned char *out,
		      const unsigned long length, const AES_KEY *key,
		      unsigned char *ivec, int *num, const int enc)
    {
    unsigned int n;

    assert(in && out && key && ivec && num);
    assert(*num == 0);

    for(n=0 ; n < length ; ++n)
	AES_cfbr_encrypt_block(&in[n],&out[n],8,key,ivec,enc);
    }
