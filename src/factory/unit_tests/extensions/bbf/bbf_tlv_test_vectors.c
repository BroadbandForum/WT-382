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

#include "1905_tlvs.h"
#include "bbf_tlvs.h"


// This file contains test vectors than can be used to check the
// "parse_1905_TLV_from_packet()" and "forge_1905_TLV_from_structure()"
// functions
//
// Each test vector is made up of three variables:
//
//   - A TLV structure
//   - An array of bits representing the network packet
//   - An variable holding the length of the packet
//
// Note that some test vectors can be used to test both functions, while others
// can only be used to test "forge_1905_TLV_from_structure()" or
// "parse_1905_TLV_from_packet()":
//
//   - Test vectors marked with "TLV --> packet" can only be used to test the
//     "forge_1905_TLV_from_structure()" function.
//
//   - Test vectors marked with "TLV <-- packet" can only be used to test the
//     "parse_1905_TLV_from_packet()" function.
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
//   A) stream = forge_non_1905_TLV_from_structure(tlv_xxx, &stream_len);
//
//   B) tlv = parse_non_1905_TLV_from_packet(stream_xxx);
//


////////////////////////////////////////////////////////////////////////////////
//// Test vector 001 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV bbf_tlv_structure_001 =
{
    .tlv_type          = BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_SPECIFIC_NEIGHBOR,
    .specific_neighbor = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_RX_LINK_METRICS_ONLY,
};

INT8U bbf_tlv_stream_001[] =
{
    0x01,
    0x00, 0x08,
    0x01,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x01
};

INT16U bbf_tlv_stream_len_001 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 002 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV bbf_tlv_structure_002 =
{
    .tlv_type          = BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
    .specific_neighbor = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
};

// CheckTrue (TLV --> packet)
INT8U bbf_tlv_stream_002[] =
{
    0x01,
    0x00, 0x08,
    0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02
};

// CheckFalse (TLV <-- packet)
//   'specific neighbor mac address' should be zero for non-1905 metrics
INT8U bbf_tlv_stream_002b[] =
{
    0x01,
    0x00, 0x08,
    0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x02
};

INT16U bbf_tlv_stream_len_002 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 003 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV bbf_tlv_structure_003 =
{
    .tlv_type          = BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
    .specific_neighbor = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
};

INT8U bbf_tlv_stream_003[] =
{
    0x01,
    0x00, 0x08,
    0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02
};

INT16U bbf_tlv_stream_len_003 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 004 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct transmitterLinkMetricTLV bbf_tlv_structure_004 =
{
    .tlv_type                    = BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC,
    .local_al_address            = {0x01, 0x02, 0x03, 0x01, 0x02, 0x03},
    .neighbor_al_address         = {0x0a, 0x0b, 0x0c, 0x0a, 0x0b, 0x0c},
    .transmitter_link_metrics_nr = 1,
    .transmitter_link_metrics    =
        (struct _transmitterLinkMetricEntries[]){
            {
                .local_interface_address    = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26},
                .neighbor_interface_address = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36},
                .intf_type                  = MEDIA_TYPE_IEEE_802_11G_2_4_GHZ,
                .bridge_flag                = 0,
                .packet_errors              = 134,
                .transmitted_packets        = 1543209,
                .mac_throughput_capacity    = 400,
                .link_availability          = 50,
                .phy_rate                   = 520,
            },
        },
};

// CheckTrue (TLV --> packet)
INT8U bbf_tlv_stream_004[] =
{
    0x02,
    0x00, 0x29,
    0x01, 0x02, 0x03, 0x01, 0x02, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x01,
    0x00,
    0x00, 0x00, 0x00, 0x86,
    0x00, 0x17, 0x8c, 0x29,
    0x01, 0x90,
    0x00, 0x32,
    0x02, 0x08
};

// CheckFalse (TLV <-- packet)
//   'specific neighbor mac address' should be zero for non-1905 metrics
INT8U bbf_tlv_stream_004b[] =
{
    0x02,
    0x00, 0x29,
    0x01, 0x02, 0x03, 0x01, 0x02, 0x03,
    0x0a, 0x0b, 0x0c, 0x0a, 0x0b, 0x0c,  // specific neighbor mac address
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x01,
    0x00,
    0x00, 0x00, 0x00, 0x86,
    0x00, 0x17, 0x8c, 0x29,
    0x01, 0x90,
    0x00, 0x32,
    0x02, 0x08
};

INT16U bbf_tlv_stream_len_004 = 44;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 005 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct transmitterLinkMetricTLV bbf_tlv_structure_005 =
{
    .tlv_type                    = BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC,
    .local_al_address            = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    .neighbor_al_address         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .transmitter_link_metrics_nr = 2,
    .transmitter_link_metrics    =
        (struct _transmitterLinkMetricEntries[]){
            {
                .local_interface_address    = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03},
                .neighbor_interface_address = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04},
                .intf_type                  = MEDIA_TYPE_MOCA_V1_1,
                .bridge_flag                = 1,
                .packet_errors              = 0,
                .transmitted_packets        = 1000,
                .mac_throughput_capacity    = 900,
                .link_availability          = 80,
                .phy_rate                   = 1000,
            },
            {
                .local_interface_address    = {0x05, 0x05, 0x05, 0x05, 0x05, 0x05},
                .neighbor_interface_address = {0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
                .intf_type                  = MEDIA_TYPE_IEEE_802_11AC_5_GHZ,
                .bridge_flag                = 0,
                .packet_errors              = 7,
                .transmitted_packets        = 7,
                .mac_throughput_capacity    = 900,
                .link_availability          = 80,
                .phy_rate                   = 1000,
            },
        },
};

INT8U bbf_tlv_stream_005[] =
{
    0x02,
    0x00, 0x46,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x03, 0x00,
    0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xe8,
    0x03, 0x84,
    0x00, 0x50,
    0x03, 0xe8,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x01, 0x05,
    0x00,
    0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x07,
    0x03, 0x84,
    0x00, 0x50,
    0x03, 0xe8,
};

INT16U bbf_tlv_stream_len_005 = 73;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 006 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct receiverLinkMetricTLV bbf_tlv_structure_006 =
{
    .tlv_type                    = BBF_TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC,
    .local_al_address            = {0x01, 0x02, 0xff, 0x01, 0x02, 0x03},
    .neighbor_al_address         = {0x0a, 0x0b, 0x0c, 0x0a, 0x77, 0x0c},
    .receiver_link_metrics_nr    = 1,
    .receiver_link_metrics       =
        (struct _receiverLinkMetricEntries[]){
            {
                .local_interface_address    = {0x21, 0x22, 0x00, 0x24, 0x00, 0x26},
                .neighbor_interface_address = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36},
                .intf_type                  = MEDIA_TYPE_IEEE_802_11AF_GHZ,
                .packet_errors              = 27263110,
                .packets_received           = 27263111,
                .rssi                       = 2,
            },
        },
};

// CheckTrue (TLV --> packet)
INT8U bbf_tlv_stream_006[] =
{
    0x03,
    0x00, 0x23,
    0x01, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x22, 0x00, 0x24, 0x00, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x07,
    0x01, 0xa0, 0x00, 0x86,
    0x01, 0xa0, 0x00, 0x87,
    0x02,
};

// CheckFalse (TLV <-- packet)
//   'specific neighbor mac address' should be zero for non-1905 metrics
INT8U bbf_tlv_stream_006b[] =
{
    0x03,
    0x00, 0x23,
    0x01, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x0a, 0x0b, 0x0c, 0x0a, 0x77, 0x0c,  // specific neighbor mac address
    0x21, 0x22, 0x00, 0x24, 0x00, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x07,
    0x01, 0xa0, 0x00, 0x86,
    0x01, 0xa0, 0x00, 0x87,
    0x02,
};

INT16U bbf_tlv_stream_len_006 = 38;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 007 (TLV <--> packet)
////
////////////////////////////////////////////////////packet////////////////////////////

struct receiverLinkMetricTLV bbf_tlv_structure_007 =
{
    .tlv_type                    = BBF_TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC,
    .local_al_address            = {0x01, 0x02, 0xff, 0x01, 0x02, 0x03},
    .neighbor_al_address         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .receiver_link_metrics_nr    = 2,
    .receiver_link_metrics       =
        (struct _receiverLinkMetricEntries[]){
            {
                .local_interface_address    = {0x21, 0x22, 0x00, 0x24, 0x00, 0x26},
                .neighbor_interface_address = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36},
                .intf_type                  = MEDIA_TYPE_IEEE_802_11AF_GHZ,
                .packet_errors              = 27263110,
                .packets_received           = 27263111,
                .rssi                       = 2,
            },
            {
                .local_interface_address    = {0xff, 0x22, 0x00, 0x24, 0x00, 0x26},
                .neighbor_interface_address = {0xff, 0x32, 0x33, 0x34, 0x35, 0x36},
                .intf_type                  = MEDIA_TYPE_IEEE_802_11AF_GHZ,
                .packet_errors              = 27263110,
                .packets_received           = 27263111,
                .rssi                       = 2,
            },
        },
};

INT8U bbf_tlv_stream_007[] =
{
    0x03,
    0x00, 0x3a,
    0x01, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x22, 0x00, 0x24, 0x00, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x07,
    0x01, 0xa0, 0x00, 0x86,
    0x01, 0xa0, 0x00, 0x87,
    0x02,
    0xff, 0x22, 0x00, 0x24, 0x00, 0x26,
    0xff, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x07,
    0x01, 0xa0, 0x00, 0x86,
    0x01, 0xa0, 0x00, 0x87,
    0x02,
};

INT16U bbf_tlv_stream_len_007 = 61;

