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

#include "lldp_tlvs.h"


// This file contains test vectors than can be used to check the
// "parse_lldp_TLV_from_packet()" and "forge_lldp_TLV_from_structure()"
// functions
//
// Each test vector is made up of three variables:
//
//   - A TLV structure
//   - An array of bits representing the network packet
//   - An variable holding the length of the packet
//
// Note that some test vectors can be used to test both functions, while others
// can only be used to test "forge_lldp_TLV_from_structure()" or
// "parse_lldp_TLV_from_packet()":
//
//   - Test vectors marked with "TLV --> packet" can only be used to test the
//     "forge_lldp_TLV_from_structure()" function.
//
//   - Test vectors marked with "TLV <-- packet" can only be used to test the
//     "parse_lldp_TLV_from_packet()" function.
//
//   - All the other test vectors are marked with "TLC <--> packet", meaning
//     they can be used to test  both functions.
//
// The reason this is happening is that, according to the standard, sometimes
// bits are ignored/changed/forced-to-a-value when forging a packet. Thus, not
// all test vectors are "inversible" (ie. forge(parse(stream)) != x)
//
// This is how you use these test vectors:
//
//   A) stream = forge_lldp_TLV_from_structure(tlv_xxx, &stream_len);
//
//   B) tlv = parse_lldp_TLV_from_packet(stream_xxx); 
//


////////////////////////////////////////////////////////////////////////////////
//// Test vector 001 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct endOfLldppduTLV lldp_tlv_structure_001 =
{
    .tlv_type          = TLV_TYPE_END_OF_LLDPPDU,
};

INT8U lldp_tlv_stream_001[] =
{
    0x00, 0x00,
};

INT16U lldp_tlv_stream_len_001 = 2;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 002 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct chassisIdTLV lldp_tlv_structure_002 =
{
    .tlv_type            = TLV_TYPE_CHASSIS_ID,
    .chassis_id_subtype  = CHASSIS_ID_TLV_SUBTYPE_MAC_ADDRESS,
    .chassis_id          = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
};

INT8U lldp_tlv_stream_002[] =
{
    0x02, 0x07,
    0x04,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
};

INT16U lldp_tlv_stream_len_002 = 9;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 003 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct portIdTLV lldp_tlv_structure_003 =
{
    .tlv_type            = TLV_TYPE_PORT_ID,
    .port_id_subtype     = PORT_ID_TLV_SUBTYPE_MAC_ADDRESS,
    .port_id             = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
};

INT8U lldp_tlv_stream_003[] =
{
    0x04, 0x07,
    0x03,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
};

INT16U lldp_tlv_stream_len_003 = 9;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 004 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct timeToLiveTypeTLV lldp_tlv_structure_004 =
{
    .tlv_type            = TLV_TYPE_TIME_TO_LIVE,
    .ttl                 = TIME_TO_LIVE_TLV_1905_DEFAULT_VALUE,
};

INT8U lldp_tlv_stream_004[] =
{
    0x06, 0x02,
    0x00, 0xb4,
};

INT16U lldp_tlv_stream_len_004 = 4;
