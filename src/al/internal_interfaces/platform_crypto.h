/*
 *  Broadband Forum BUS (Broadband User Services) Work Area
 *  
 *  Copyright (c) 2017, Broadband Forum
 *  Copyright (c) 2017, MaxLinear, Inc. and its affiliates
 *  
 *  This is draft software, is subject to change, and has not been
 *  approved by members of the Broadband Forum. It is made available to
 *  non-members for internal study purposes only. For such study
 *  purposes, you have the right to make copies and modifications only
 *  for distributing this software internally within your organization
 *  among those who are working on it (redistribution outside of your
 *  organization for other than study purposes of the original or
 *  modified works is not permitted). For the avoidance of doubt, no
 *  patent rights are conferred by this license.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *  
 *  Unless a different date is specified upon issuance of a draft
 *  software release, all member and non-member license rights under the
 *  draft software release will expire on the earliest to occur of (i)
 *  nine months from the date of issuance, (ii) the issuance of another
 *  version of the same software release, or (iii) the adoption of the
 *  draft software release as final.
 *  
 *  ---
 *  
 *  This version of this source file is part of the Broadband Forum
 *  WT-382 IEEE 1905.1/1a stack project.
 *  
 *  Please follow the release link (given below) for further details
 *  of the release, e.g. license validity dates and availability of
 *  more recent draft or final releases.
 *  
 *  Release name: WT-382_draft1
 *  Release link: https://www.broadband-forum.org/software#WT-382_draft1
 */

#ifndef _PLATFORM_CRYPTO_H_
#define _PLATFORM_CRYPTO_H_

#define SHA256_MAC_LEN 32
#define AES_BLOCK_SIZE 16

#include "platform.h"


// Fill the buffer of length 'len' pointed by 'p' with random bytes.
//
// Return "0" if there was a problem, "1" otherwise
//
INT8U PLATFORM_GET_RANDOM_BYTES(INT8U *p, INT16U len);


// Return a Diffie Hellman pair of private and public keys (and its lengths) in
// the output arguments "priv", "priv_len", "pub" and "pub_len".
//
// Both "priv" and "pub" must be deallocated by the caller when they are no
// longer needed (using "PLATFORM_FREE()")
//
// The keys are obtained using the DH group specified in RFC3523 "section 2"
// (ie. the "1536-bit MODP Group" where "g = 2" and "p = 2^1536 - 2^1472 - 1 +
// 2^64 * { [2^1406 pi] + 741804 }")
//
// Return "0" if there was a problem, "1" otherwise
//
INT8U PLATFORM_GENERATE_DH_KEY_PAIR(INT8U **priv, INT16U *priv_len, INT8U **pub, INT16U *pub_len);

// Return the Diffie Hell shared secret (in output argument "shared_secret"
// which is "shared_secret_len" bytes long) associated to a remote public key
// ("remote_pub", which is "remote_pub_len" bytes long") and a local private
// key ("local_priv", which is "local_priv_len" bytes long).
//
// "shared_secret" must be deallocated by the caller once it is no longer needed
// (using "PLATFORM_FREE()")
//
// Return "0" if there was a problem, "1" otherwise
//
INT8U PLATFORM_COMPUTE_DH_SHARED_SECRET(INT8U **shared_secret, INT16U *shared_secret_len, INT8U *remote_pub, INT16U remote_pub_len, INT8U *local_priv, INT8U local_priv_len);

// Return the SHA256 digest of the provided input.
//
// The provided input is the result of concatenating 'num_elem' elements
// (addr[0], addr[1], ..., addr[num_elem-1] of size len[0], len[1], ...,
// len[num_elem-1])
//
// The digest is returned in the 'digest' output argument which must point to
// a preallocated buffer of "SHA256_MAC_LEN" bytes.
//
INT8U PLATFORM_SHA256(INT8U num_elem, INT8U **addr, INT32U *len, INT8U *digest);


// Return the HMAC_SHA256 digest of the provided input using the provided 'key'
// (which is 'keylen' bytes long).
//
// The provided input is the result of concatenating 'num_elem' elements
// (addr[0], addr[1], ..., addr[num_elem-1] of size len[0], len[1], ...,
// len[num_elem-1])
//
// The digest is returned in the 'hmac' output argument which must point to
// a preallocated buffer of "SHA256_MAC_LEN" bytes.
//
INT8U PLATFORM_HMAC_SHA256(INT8U *key, INT32U keylen, INT8U num_elem, INT8U **addr, INT32U *len, INT8U *hmac);

// Encrypt the provided 'data' (which is a pointer to buffer of size
// n*AES_BLOCK_SIZE) using the AES 128 CBC algorithm with the provided
// "initialization vector" ('iv', which is also a pointer to a buffer of
// AES_BLOCK_SIZE bytes).
//
// The result is written to THE SAME 'data' buffer and has the same length as
// the input (plain) data.
//
// Note that you might have to "pad" the data buffer (so that its length is a
// multiple of AES_BLOCK_SIZE) in most cases.
//
// Return "0" if there was a problem, "1" otherwise
//
INT8U PLATFORM_AES_ENCRYPT(INT8U *key, INT8U *iv, INT8U *data, INT32U data_len);

// Works exactly like "PLATFORM_AES_ENCRYPT", but now the 'data' buffer
// originally contains encrypted data and after the call it contains
// unencrypted data.
INT8U PLATFORM_AES_DECRYPT(INT8U *key, INT8U *iv, INT8U *data, INT32U data_len);

#endif
