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

#ifndef _PACKET_TOOLS_H_
#define _PACKET_TOOLS_H_

#include "platform.h"


// Auxiliary functions to:
//
//   A) Extract 1, 2 or 4 bytes from a stream received from the network.
//
//   B) Insert  1, 2 or 4 bytes into a stream which is going to be sent into
//      the network.
//
// These functions do three things:
//
//   1. Avoid unaligned memory accesses (which might cause slowdowns or even
//      exceptions on some architectures)
//
//   2. Convert from network order to host order (and the other way)
//
//   3. Advance the packet pointer as many bytes as those which have just
//      been extracted/inserted.

// Extract/insert 1 byte
//
static inline void _E1B(INT8U **packet_ppointer, INT8U *memory_pointer)
{
    *memory_pointer     = **packet_ppointer;
    (*packet_ppointer) += 1;
}
static inline void _I1B(INT8U *memory_pointer, INT8U **packet_ppointer)
{
    **packet_ppointer   = *memory_pointer;
    (*packet_ppointer) += 1;
}

// Extract/insert 2 bytes
//
static inline void _E2B(INT8U **packet_ppointer, INT16U *memory_pointer)
{
#if _HOST_IS_BIG_ENDIAN_ == 1
    *(((INT8U *)memory_pointer)+0)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+1)  = **packet_ppointer; (*packet_ppointer)++;
#elif _HOST_IS_LITTLE_ENDIAN_ == 1
    *(((INT8U *)memory_pointer)+1)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+0)  = **packet_ppointer; (*packet_ppointer)++;
#else
#error You must specify your architecture endianess
#endif
}
static inline void _I2B(INT16U *memory_pointer, INT8U **packet_ppointer)
{
#if _HOST_IS_BIG_ENDIAN_ == 1
    **packet_ppointer = *(((INT8U *)memory_pointer)+0); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+1); (*packet_ppointer)++;
#elif _HOST_IS_LITTLE_ENDIAN_ == 1
    **packet_ppointer = *(((INT8U *)memory_pointer)+1); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+0); (*packet_ppointer)++;
#else
#error You must specify your architecture endianess
#endif
}

// Extract/insert 4 bytes
//
static inline void _E4B(INT8U **packet_ppointer, INT32U *memory_pointer)
{
#if _HOST_IS_BIG_ENDIAN_ == 1
    *(((INT8U *)memory_pointer)+0)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+1)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+2)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+3)  = **packet_ppointer; (*packet_ppointer)++;
#elif _HOST_IS_LITTLE_ENDIAN_ == 1
    *(((INT8U *)memory_pointer)+3)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+2)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+1)  = **packet_ppointer; (*packet_ppointer)++;
    *(((INT8U *)memory_pointer)+0)  = **packet_ppointer; (*packet_ppointer)++;
#else
#error You must specify your architecture endianess
#endif
}
static inline void _I4B(INT32U *memory_pointer, INT8U **packet_ppointer)
{
#if _HOST_IS_BIG_ENDIAN_ == 1
    **packet_ppointer = *(((INT8U *)memory_pointer)+0); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+1); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+2); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+3); (*packet_ppointer)++;
#elif _HOST_IS_LITTLE_ENDIAN_ == 1
    **packet_ppointer = *(((INT8U *)memory_pointer)+3); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+2); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+1); (*packet_ppointer)++;
    **packet_ppointer = *(((INT8U *)memory_pointer)+0); (*packet_ppointer)++;
#else
#error You must specify your architecture endianess
#endif
}

// Extract/insert N bytes (ignore endianess)
//
static inline void _EnB(INT8U **packet_ppointer, void *memory_pointer, INT32U n)
{
    PLATFORM_MEMCPY(memory_pointer, *packet_ppointer, n);
    (*packet_ppointer) += n;
}
static inline void _InB(void *memory_pointer, INT8U **packet_ppointer, INT32U n)
{
    PLATFORM_MEMCPY(*packet_ppointer, memory_pointer, n);
    (*packet_ppointer) += n;
}

#endif
