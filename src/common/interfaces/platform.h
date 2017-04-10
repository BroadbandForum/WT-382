/*
 *  Broadband Forum BUS (Broadband User Services) Work Area
 *  
 *  Copyright (c) 2017, Broadband Forum
 *  Copyright (c) 2016, Marvell International Ltd. and its affiliates
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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdarg.h>   // va_list
  // NOTE: This is part of the C standard, thus *all* platforms should have it
  // available... and that's why this include can exist in this "platform
  // independent" file

////////////////////////////////////////////////////////////////////////////////
// Hardware stuff
////////////////////////////////////////////////////////////////////////////////

// The following preprocessor macros must be defined to a platform-dependent
// value:
//
//   _HOST_IS_LITTLE_ENDIAN_        |--> Set one (and only one!) of these macros
//   _HOST_IS_BIG_ENDIAN_ENDIAN_    |    to "1" to indicate your CPU endianness
//
//
//   MAX_NETWORK_SEGMENT_SIZE  --------> This is the maximum packet size that
//                                       is allowed in your platform. It is
//                                       used to 'fragment' CMDUs.  Note that
//                                       even if your platform supports packets
//                                       bigger than 1500 bytes, this macro
//                                       must never be bigger than that.  This
//                                       macro is only present in this file for
//                                       those special cases where, for some
//                                       platform related reason, packets must
//                                       be smaller than 1500.
//
//  INT8U  |                             These types must be adjusted to that
//  INT16U |---------------------------> they represent 1, 2 and 4 bytes
//  INT32U |                             unsigned integers respectively.
//
//  INT8S  |                             These types must be adjusted to that
//  INT16S |---------------------------> they represent 1, 2 and 4 bytes signed
//  INT32S |                             integers respectively.
//
//
// In the next few lines we are just going to check that these are defined,
// nothing else.
// In order to actually define them use the "root" Makefile where these MACROS
// are sent to the compiler using the "-D flag" (open the "root" Makefile and
// search for "CCFLAGS" to understand how to do this)

#if !defined(_HOST_IS_LITTLE_ENDIAN_) && !defined(_HOST_IS_BIG_ENDIAN_ENDIAN_)
#  error  "You must define either '_HOST_IS_LITTLE_ENDIAN_' or '_HOST_IS_BIG_ENDIAN_'"
#elif defined(_HOST_IS_LITTLE_ENDIAN_) && defined(_HOST_IS_BIG_ENDIAN_ENDIAN_)
#  error  "You cannot define both '_HOST_IS_LITTLE_ENDIAN_' and '_HOST_IS_BIG_ENDIAN_' at the same time"
#endif

#ifndef  MAX_NETWORK_SEGMENT_SIZE
#  error  "You must define 'MAX_NETWORK_SEGMENT_SIZE' to some value (for example, '1500')"
#endif

#if !defined(INT8U) || !defined(INT16U) || !defined(INT32U)
#  error  "You must define 'INT8U', 'INT16U' and 'INT32U' to represent 8, 16 and 32 bit unsigned integers respectively"
#endif

#if !defined(INT8S) || !defined(INT16S) || !defined(INT32S)
#  error  "You must define 'INT8S', 'INT16S' and 'INT32S' to represent 8, 16 and 32 bit signed integers respectively"
#endif


////////////////////////////////////////////////////////////////////////////////
// Typical libc stuff
////////////////////////////////////////////////////////////////////////////////

// Allocate a chunk of 'n' bytes and return a pointer to it.
//
// If no memory can be allocated, this function must *not* return (instead of
// returning a NULL pointer), and the program must be exited immediately.
//
void *PLATFORM_MALLOC(INT32U size);

// Free a memory area previously obtained with "PLATFORM_MALLOC()"
//
void PLATFORM_FREE(void *ptr);

// Redimendion a memory area previously obtained  with "PLATFORM_MALLOC()"
//
// If no memory can be allocated, this function must *not* return (instead of
// returning a NULL pointer), and the program must be exited immediately.
//
void *PLATFORM_REALLOC(void *ptr, INT32U size);

// Sets 'n' bytes (with value 'c') to 'dest'
//
void *PLATFORM_MEMSET(void *dest, INT8U c, INT32U n);

// Copies 'n' bytes from 'src' to 'dest'
//
void *PLATFORM_MEMCPY(void *dest, const void *src, INT32U n);

// Return '0' only if the first 'n' bytes of 's1' and 's2' match, '1' otherwise
// [PLATFORM PORTING NOTE]
//   Note that this is different from the standard libc "memcmp()" behaviour
//
INT8U PLATFORM_MEMCMP(const void *s1, const void *s2, INT32U n);

// Return the length of string 's' not including the terminating NULL character
//
INT32U PLATFORM_STRLEN(const char *s);

// Return ca copy of the provided string 's', that can later be freed with
// "PLATFORM_FREE()" 
//
char *PLATFORM_STRDUP(const char *s);

// Append up to 'n' characters (in addition to the terminating NULL character)
// from 'src' into 'dest'
//
char *PLATFORM_STRNCAT(char *dest, const char *src, INT32U n);

// Output to a string (see 'man 3 snprintf' on any Linux box)
//
void PLATFORM_SNPRINTF(char *dest, INT32U n, const char *format, ...);

// Output to a string ("va" version, see 'man 3 vsnprintf' on any Linux box)
//
void PLATFORM_VSNPRINTF(char *dest, INT32U n, const char *format, va_list ap);

// Output the provided format string (see 'man 3 printf' on any Linux box)
//
void PLATFORM_PRINTF(const char *format, ...);

// Same as 'PLATFORM_PRINTF', but the message will only be processed if the
// platform has the pertaining debug level enabled
//
void PLATFORM_PRINTF_DEBUG_ERROR(const char *format, ...);
void PLATFORM_PRINTF_DEBUG_WARNING(const char *format, ...);
void PLATFORM_PRINTF_DEBUG_INFO(const char *format, ...);
void PLATFORM_PRINTF_DEBUG_DETAIL(const char *format, ...);

// Used to set the verbosity of the previous functions:
//
//   0 => Only print ERROR messages
//   1 => Print ERROR and WARNING messages
//   2 => Print ERROR, WARNING and INFO messages
//   3 => Print ERROR, WARNING, INFO and DETAIL messages
//
void PLATFORM_PRINTF_DEBUG_SET_VERBOSITY_LEVEL(int level);

// Return the number of milliseconds ellapsed since the program started
//
INT32U PLATFORM_GET_TIMESTAMP(void);


////////////////////////////////////////////////////////////////////////////////
// Misc stuff
////////////////////////////////////////////////////////////////////////////////

// [PLATFORM PORTING NOTE]
//   Depending on what other platform headers you have included up until this
//   point, 'NULL' might or might not be defined. If so, define it here
//
#ifndef NULL
#  define NULL (0x0)
#endif


////////////////////////////////////////////////////////////////////////////////
// Initialization functions
////////////////////////////////////////////////////////////////////////////////

// This function *must* be called before any other "PLATFORM_*()" API function
//
// Returns "0" if there was a problem. "1" otherwise.
//
// [PLATFORM PORTING NOTE]
//   Use this function to reserve memory, initialize semaphores, etc...
//   
INT8U PLATFORM_INIT(void);

#endif
