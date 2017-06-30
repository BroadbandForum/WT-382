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

#ifndef _LLDP_PAYLOAD_H_
#define _LLDP_PAYLOAD_H_

////////////////////////////////////////////////////////////////////////////////
// PAYLOAD associated structures
////////////////////////////////////////////////////////////////////////////////

struct PAYLOAD
{
    #define MAX_LLDP_TLVS 10
    INT8U   *list_of_TLVs[MAX_LLDP_TLVS+1]; 
                                   // NULL-terminated list of pointers to TLV 
                                   // structures.
                                   // The "end of lldppdu" TLV is not included
                                   // in this list.
};



////////////////////////////////////////////////////////////////////////////////
// Main API functions
////////////////////////////////////////////////////////////////////////////////

// This function receives a pointer to a streams containing ETH layer packet
// data (ie. offset +14 in a raw network packet, just after the source MAC
// address, destination MAC address and ETH type fields).
// 
// The payload of this stream must contain a LLDP bridge discovery message, as
// detailed in "IEEE Std 1905.1-2013, Section 6.1"
//
// The following types of TLVs will be extracted from the contents of the
// packet, parsed, and returned inside a PAYLOAD structure:
//
//   - TLV_TYPE_CHASSIS_ID
//   - TLV_TYPE_PORT_ID
//   - TLV_TYPE_TIME_TO_LIVE
//
// The stream must always finish with a "TLV_TYPE_END_OF_LLDPPDU" TLV, but this
// one won't be contained in the returned PAYLOAD structure.
// 
// If any type of error/inconsistency is found, a NULL pointer is returned
// instead, otherwise remember to free the received structure once you don't
// need it anymore (using the "free_lldp_PAYLOAD_structure()" function)
//
struct PAYLOAD *parse_lldp_PAYLOAD_from_packet(INT8U *packet_stream);


// This is the opposite of "parse_lldp_PAYLOAD_from_packet_from_packets()": it
// receives a pointer to a PAYLOAD structure and then returns a pointer to a
// packet stream.
//
// 'len' is also an output argument containing the length of the returned
// stream.
//
// The provided 'memory_structure' must have its fields properly filled or else
// this function will return an error (ie. a NULL pointer).
// What does this mean? Easy: only those TLVs detailed in "IEEE Std
// 1905.1-2013, Section 6.1" can appear inside the PAYLOAD structure.
//
//   NOTE: Notice how, in "parse_lldp_PAYLOAD_from_packets()", unexpected TLVs
//         are discarded, but here they produce an error. This is expected!
//
// One more thing: the provided 'memory_structure' is not freed by this
// function, thus, once this function returns, you will be responsible for
// freeing three things:
//
//   - The 'memory_structure' PAYLOAD structure (you were already responsible
//     for), that you can free with 'free_lldp_PAYLOAD_structure()'
//
//   - The returned stream, which can be freed with 'free_lldp_PAYLOAD_packet()'
//
INT8U *forge_lldp_PAYLOAD_from_structure(struct PAYLOAD *memory_structure, INT16U *len);



////////////////////////////////////////////////////////////////////////////////
// Utility API functions
////////////////////////////////////////////////////////////////////////////////


// This function receives a pointer to a PAYLOAD structure and then traverses it
// and all nested structures, calling "PLATFORM_FREE()" on each one of them
//
//
void free_lldp_PAYLOAD_structure(struct PAYLOAD *memory_structure);


// This function receives a pointer to a  streams returned by
// "forge_lldp_PAYLOAD_from_structure()" 
//
#define  free_lldp_PAYLOAD_packet  PLATFORM_FREE


// This function returns '0' if the two given pointers represent PAYLOAD
// structures that contain the same data
//
INT8U compare_lldp_PAYLOAD_structures(struct PAYLOAD *memory_structure_1, struct PAYLOAD *memory_structure_2);


// The next function is used to call function "callback()" on each element of
// the "memory_structure" structure
//
// "memory_structure" must point to a structure of one of the types returned by
// "parse_1905_PAYLOAD_from_packets()"
//
// It takes four arguments:
//   - The structure whose elements are going to be visited
//   - A callback function that will be executed on each element with the
//     following arguments:
//      * A pointer to the "write()" function that will be used to dump text.
//        This is always the "write_function()" pointer provided as third
//        argument to the "visit_lldp_PAYLOAD_structure()" function.
//      * The size of the element to print (1, 2, 4, n bytes)
//      * A prefix string.
//        This is always the "prefix" value provided as fourth argument to the
//        "visit_lldp_PAYLOAD_structure()" function/
//      * The name of the element (ex: "mac_address")
//      * A 'fmt' string which must be used to print the contents of the element
//      * A pointer to the element itself
//   - The "write()" function that will be used when the callback is executed
//   - A "prefix" string argument that will be used when the callback is
//     executed (it usually contains "context" information that the callback
//     function prints before anything else to make it easy to follow the
//     structure traversing order)
//
void visit_lldp_PAYLOAD_structure(struct PAYLOAD *memory_structure, void (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p), void (*write_function)(const char *fmt, ...), const char *prefix);

#endif
