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

#ifndef _BBF_TLVS_H_
#define _BBF_TLVS_H_

#include "platform.h"


////////////////////////////////////////////////////////////////////////////////
// BBF TLV types
////////////////////////////////////////////////////////////////////////////////
#define BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY                   (1)
#define BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC             (2)
#define BBF_TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC                (3)
#define BBF_TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE             (4)

#define BBF_TLV_TYPE_NON_1905_LAST                                (5)
                                                     // NOTE: If new types are
                                                     // introduced in future
                                                     // revisions of the
                                                     // standard, update this
                                                     // value so that it always
                                                     // points to the last one.


static const INT8U BBF_OUI[3] = {0x00, 0x25, 0x6d};

////////////////////////////////////////////////////////////////////////////////
// Main API functions
////////////////////////////////////////////////////////////////////////////////

// This function receives a pointer to a stream of bytes representing a BBF TLV.
//
// It then returns a pointer to a structure whose fields have already been
// filled with the appropriate values extracted from the parsed stream.
//
// The actual type of the returned pointer structure depends on the value of
// the first byte pointed by "packet_stream" (ie. the "Type" field of the TLV):
//
//   TLV_TYPE_NON_1905_LINK_METRIC_QUERY        -->  struct linkMetricQueryTLV *
//   TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC  -->  struct transmitterLinkMetricTLV *
//   TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC     -->  struct receiverLinkMetricTLV *
//   TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE  -->  struct linkMetricResultCodeTLV *
//
//
// If an error was encountered while parsing the stream, a NULL pointer is
// returned instead.
// Otherwise, the returned structure is dynamically allocated, and once it is
// no longer needed, the user must call the "free_bbf_TLV_structure()"
//
INT8U *parse_bbf_TLV_from_packet(INT8U *packet_stream);


 // This is the opposite of "parse_bbf_TLV_from_packet()": it receives a
 // pointer to a TLV structure and then returns a pointer to a buffer which:
 //   - is a packet representation of the TLV
 //   - has a length equal to the value returned in the "len" output argument
 //
 // "memory_structure" must point to a structure of one of the types returned by
 // "parse_bbf_TLV_from_packet()"
 //
 // If there is a problem this function returns NULL, otherwise the returned
 // buffer must be later freed by the caller (it is a regular, non-nested
 // buffer, so you just need to call "free()").
 //
 // Note that the input structure is *not* freed. You still need to later call
 // "free_bbf_TLV_structure()"
 //
INT8U *forge_bbf_TLV_from_structure(INT8U *memory_structure, INT16U *len);



////////////////////////////////////////////////////////////////////////////////
// Utility API functions
////////////////////////////////////////////////////////////////////////////////

// This function receives a pointer to a TLV structure and then traverses it
// and all nested structures, calling "free()" on each one of them
//
// "memory_structure" must point to a structure of one of the types returned by
// "parse_bbf_TLV_from_packet()"
//
void free_bbf_TLV_structure(INT8U *memory_structure);


// 'forge_bbf_TLV_from_structure()' returns a regular buffer which can be freed
// using this macro defined to be PLATFORM_FREE
//
#define  free_bbf_TLV_packet  PLATFORM_FREE


// This function returns '0' if the two given pointers represent TLV structures
// of the same type and they contain the same data
//
// "memory_structure_1" and "memory_structure_2" must point (each) to a
// structure of one of the types returned by "parse_bbf_TLV_from_packet()"
//
INT8U compare_bbf_TLV_structures(INT8U *memory_structure_1, INT8U *memory_structure_2);


// The next function is used to call function "callback()" on each element of
// the "memory_structure" structure
//
// "memory_structure" must point to a structure of one of the types returned by
// "parse_bbf_TLV_from_packet()"
//
// It takes four arguments:
//   - The structure whose elements are going to be visited
//   - A callback function that will be executed on each element with the
//     following arguments:
//      * A pointer to the "write()" function that will be used to dump text.
//        This is always the "write_function()" pointer provided as third
//        argument to the "visit_bbf_TLV_structure()" function.
//      * The size of the element to print (1, 2, 4, n bytes)
//      * A prefix string.
//        This is always the "prefix" value provided as fourth argument to the
//        "visit_bbf_TLV_structure()" function
//      * The name of the element (ex: "mac_address")
//      * A 'fmt' string which must be used to print the contents of the element
//      * A pointer to the element itself
//   - The "write()" function that will be used when the callback is executed
//   - A "prefix" string argument that will be used when the callback is
//     executed (it usually contains "context" information that the callback
//     function prints before anything else to make it easy to follow the
//     structure traversing order)
//
void visit_bbf_TLV_structure(INT8U *memory_structure, void (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p), void (*write_function)(const char *fmt, ...), const char *prefix);


// Use this function for debug purposes. It turns a TLV_TYPE_* variable into its
// string representation.
//
// Example: TLV_TYPE_AL_MAC_ADDRESS_TYPE --> "TLV_TYPE_AL_MAC_ADDRESS_TYPE"
//
// Return "Unknown" if the provided type does not exist.
//
char *convert_bbf_TLV_type_to_string(INT8U tlv_type);

#endif // END _BBF_TLVS_H_
