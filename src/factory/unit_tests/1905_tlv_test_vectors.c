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

#include "1905_tlvs.h"


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
//   A) stream = forge_1905_TLV_from_structure(tlv_xxx, &stream_len);
//
//   B) tlv = parse_1905_TLV_from_packet(stream_xxx); 
//


////////////////////////////////////////////////////////////////////////////////
//// Test vector 001 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV x1905_tlv_structure_001 =
{
    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_SPECIFIC_NEIGHBOR,
    .specific_neighbor = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_RX_LINK_METRICS_ONLY,
};

INT8U x1905_tlv_stream_001[] =
{
    0x08,
    0x00, 0x08,
    0x01,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x01
};

INT16U x1905_tlv_stream_len_001 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 002 (TLV --> packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV x1905_tlv_structure_002 =
{
    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
    .specific_neighbor = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
};

INT8U x1905_tlv_stream_002[] =
{
    0x08,
    0x00, 0x08,
    0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02
};

INT16U x1905_tlv_stream_len_002 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 003 (TLV <-- packet)
////////////////////////////////////////////////////////////////////////////////

struct linkMetricQueryTLV x1905_tlv_structure_003 =
{
    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
    .specific_neighbor = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
};

INT8U x1905_tlv_stream_003[] =
{
    0x08,
    0x00, 0x08,
    0x00,
    0x01, 0xaa, 0x04, 0x00, 0xfc, 0x10,
    0x02
};

INT16U x1905_tlv_stream_len_003 = 11;


////////////////////////////////////////////////////////////////////////////////
//// Test vector 004 (TLV <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct transmitterLinkMetricTLV x1905_tlv_structure_004 =
{
    .tlv_type                    = TLV_TYPE_TRANSMITTER_LINK_METRIC,
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

INT8U x1905_tlv_stream_004[] =
{
    0x09,
    0x00, 0x29,
    0x01, 0x02, 0x03, 0x01, 0x02, 0x03,
    0x0a, 0x0b, 0x0c, 0x0a, 0x0b, 0x0c,
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

INT16U x1905_tlv_stream_len_004 = 44;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 005 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct transmitterLinkMetricTLV x1905_tlv_structure_005 =
{
    .tlv_type                    = TLV_TYPE_TRANSMITTER_LINK_METRIC,
    .local_al_address            = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    .neighbor_al_address         = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02},
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

INT8U x1905_tlv_stream_005[] =
{
    0x09,
    0x00, 0x46,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
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

INT16U x1905_tlv_stream_len_005 = 73;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 006 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct receiverLinkMetricTLV x1905_tlv_structure_006 =
{
    .tlv_type                    = TLV_TYPE_RECEIVER_LINK_METRIC,
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

INT8U x1905_tlv_stream_006[] =
{
    0x0a,
    0x00, 0x23,
    0x01, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x0a, 0x0b, 0x0c, 0x0a, 0x77, 0x0c,
    0x21, 0x22, 0x00, 0x24, 0x00, 0x26,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x01, 0x07,
    0x01, 0xa0, 0x00, 0x86,
    0x01, 0xa0, 0x00, 0x87,
    0x02, 
};

INT16U x1905_tlv_stream_len_006 = 38;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 007 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct receiverLinkMetricTLV x1905_tlv_structure_007 =
{
    .tlv_type                    = TLV_TYPE_RECEIVER_LINK_METRIC,
    .local_al_address            = {0x01, 0x02, 0xff, 0x01, 0x02, 0x03},
    .neighbor_al_address         = {0x0a, 0x0b, 0x0c, 0x0a, 0x77, 0x0c},
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

INT8U x1905_tlv_stream_007[] =
{
    0x0a,
    0x00, 0x3a,
    0x01, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x0a, 0x0b, 0x0c, 0x0a, 0x77, 0x0c,
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

INT16U x1905_tlv_stream_len_007 = 61;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 008 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct alMacAddressTypeTLV x1905_tlv_structure_008 =
{
    .tlv_type                    = TLV_TYPE_AL_MAC_ADDRESS_TYPE,
    .al_mac_address              = {0x01, 0x02, 0xf2, 0x01, 0x02, 0x00},
};

INT8U x1905_tlv_stream_008[] =
{
    0x01,
    0x00, 0x06,
    0x01, 0x02, 0xf2, 0x01, 0x02, 0x00,
};

INT16U x1905_tlv_stream_len_008 = 9;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 009 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct macAddressTypeTLV x1905_tlv_structure_009 =
{
    .tlv_type                    = TLV_TYPE_MAC_ADDRESS_TYPE,
    .mac_address                 = {0xff, 0xf2, 0x04, 0xfa, 0x00, 0xab},
};

INT8U x1905_tlv_stream_009[] =
{
    0x02,
    0x00, 0x06,
    0xff, 0xf2, 0x04, 0xfa, 0x00, 0xab,
};

INT16U x1905_tlv_stream_len_009 = 9;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 010 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct deviceInformationTypeTLV x1905_tlv_structure_010 =
{
    .tlv_type                    = TLV_TYPE_DEVICE_INFORMATION_TYPE,
    .al_mac_address              = {0x04, 0x02, 0xff, 0x01, 0x02, 0x03},
    .local_interfaces_nr         = 2,
    .local_interfaces            = 
        (struct _localInterfaceEntries[]){
            {
                .mac_address                                                       = {0x21, 0x22, 0x00, 0x24, 0x25, 0x26},
                .media_type                                                        = MEDIA_TYPE_IEEE_802_11AF_GHZ,
                .media_specific_data_size                                          = 10,
                .media_specific_data.ieee80211.network_membership                  = {0x01, 0x01, 0x01, 0x02, 0x02, 0x02},
                .media_specific_data.ieee80211.role                                = IEEE80211_SPECIFIC_INFO_ROLE_WIFI_P2P_CLIENT,
                .media_specific_data.ieee80211.ap_channel_band                     = 0x05,
                .media_specific_data.ieee80211.ap_channel_center_frequency_index_1 = 0x0a,
                .media_specific_data.ieee80211.ap_channel_center_frequency_index_2 = 0x0b,
            },
            {
                .mac_address                                     = {0x21, 0x22, 0x00, 0x24, 0x25, 0x27},
                .media_type                                      = MEDIA_TYPE_IEEE_1901_WAVELET,
                .media_specific_data_size                        = 7,
                .media_specific_data.ieee1901.network_identifier = {0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0xff},
            },
        },
};

INT8U x1905_tlv_stream_010[] =
{
    0x03,
    0x00, 0x2a,
    0x04, 0x02, 0xff, 0x01, 0x02, 0x03,
    0x02,
    0x21, 0x22, 0x00, 0x24, 0x25, 0x26,
    0x01, 0x07,
    0x0a,
    0x01, 0x01, 0x01, 0x02, 0x02, 0x02,
    0x80,
    0x05,
    0x0a,
    0x0b,
    0x21, 0x22, 0x00, 0x24, 0x25, 0x27,
    0x02, 0x00,
    0x07,
    0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0xff,
};

INT16U x1905_tlv_stream_len_010 = 45;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 011 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct deviceBridgingCapabilityTLV x1905_tlv_structure_011 =
{
    .tlv_type                    = TLV_TYPE_DEVICE_BRIDGING_CAPABILITIES,
    .bridging_tuples_nr          = 2,
    .bridging_tuples             = 
        (struct _bridgingTupleEntries[]){
            {
                .bridging_tuple_macs_nr  = 2,
                .bridging_tuple_macs     = 
                    (struct _bridgingTupleMacEntries[]){
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
                        },
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x02},
                        },
                    }
            },
            {
                .bridging_tuple_macs_nr  = 3,
                .bridging_tuple_macs     = 
                    (struct _bridgingTupleMacEntries[]){
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x11},
                        },
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x12},
                        },
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x13},
                        },
                    }
            },
        },
};

INT8U x1905_tlv_stream_011[] =
{
    0x04,
    0x00, 0x21,
    0x02,
    0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13,
};

INT16U x1905_tlv_stream_len_011 = 36;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 012 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct deviceBridgingCapabilityTLV x1905_tlv_structure_012 =
{
    .tlv_type                    = TLV_TYPE_DEVICE_BRIDGING_CAPABILITIES,
    .bridging_tuples_nr          = 2,
    .bridging_tuples             = 
        (struct _bridgingTupleEntries[]){
            {
                .bridging_tuple_macs_nr  = 2,
                .bridging_tuple_macs     = 
                    (struct _bridgingTupleMacEntries[]){
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
                        },
                        {
                            .mac_address     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x02},
                        },
                    }
            },
            {
                .bridging_tuple_macs_nr  = 0,
                .bridging_tuple_macs     = NULL,
            },
        },
};

INT8U x1905_tlv_stream_012[] =
{
    0x04,
    0x00, 0x0f,
    0x02,
    0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x00,
};

INT16U x1905_tlv_stream_len_012 = 18;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 013 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct deviceBridgingCapabilityTLV x1905_tlv_structure_013 =
{
    .tlv_type                    = TLV_TYPE_DEVICE_BRIDGING_CAPABILITIES,
    .bridging_tuples_nr          = 0,
    .bridging_tuples             = NULL,
};

INT8U x1905_tlv_stream_013[] =
{
    0x04,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_013 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 014 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct non1905NeighborDeviceListTLV x1905_tlv_structure_014 =
{
    .tlv_type                    = TLV_TYPE_NON_1905_NEIGHBOR_DEVICE_LIST,
    .local_mac_address           = {0x33, 0x34, 0x35, 0x36, 0x37, 0x38},
    .non_1905_neighbors_nr       = 1,
    .non_1905_neighbors          =
        (struct _non1905neighborEntries[]){
            {
                .mac_address = {0x43, 0x44, 0x45, 0x46, 0x47, 0x48},
            },
        },
};

INT8U x1905_tlv_stream_014[] =
{
    0x06,
    0x00, 0x0c,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
};

INT16U x1905_tlv_stream_len_014 = 15;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 015 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct non1905NeighborDeviceListTLV x1905_tlv_structure_015 =
{
    .tlv_type                    = TLV_TYPE_NON_1905_NEIGHBOR_DEVICE_LIST,
    .local_mac_address           = {0x33, 0x34, 0x35, 0x36, 0x37, 0x38},
    .non_1905_neighbors_nr       = 2,
    .non_1905_neighbors          =
        (struct _non1905neighborEntries[]){
            {
                .mac_address = {0x43, 0x44, 0x45, 0x46, 0x47, 0x48},
            },
            {
                .mac_address = {0x53, 0x54, 0x55, 0x56, 0x57, 0x58},
            },
        },
};

INT8U x1905_tlv_stream_015[] =
{
    0x06,
    0x00, 0x12,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
};

INT16U x1905_tlv_stream_len_015 = 21;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 016 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct neighborDeviceListTLV x1905_tlv_structure_016 =
{
    .tlv_type                    = TLV_TYPE_NEIGHBOR_DEVICE_LIST,
    .local_mac_address           = {0x33, 0x34, 0x35, 0x36, 0x37, 0x38},
    .neighbors_nr                = 1,
    .neighbors                   =
        (struct _neighborEntries[]){
            {
                .mac_address = {0x43, 0x44, 0x45, 0x46, 0x47, 0x48},
                .bridge_flag = 0,
            },
        },
};

INT8U x1905_tlv_stream_016[] =
{
    0x07,
    0x00, 0x0d,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x00,
};

INT16U x1905_tlv_stream_len_016 = 16;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 017 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct neighborDeviceListTLV x1905_tlv_structure_017 =
{
    .tlv_type                    = TLV_TYPE_NEIGHBOR_DEVICE_LIST,
    .local_mac_address           = {0x33, 0x34, 0x35, 0x36, 0x37, 0x38},
    .neighbors_nr                = 2,
    .neighbors                   =
        (struct _neighborEntries[]){
            {
                .mac_address = {0x43, 0x44, 0x45, 0x46, 0x47, 0x48},
                .bridge_flag = 1,
            },
            {
                .mac_address = {0x53, 0x54, 0x55, 0x56, 0x57, 0x58},
                .bridge_flag = 0,
            },
        },
};

INT8U x1905_tlv_stream_017[] =
{
    0x07,
    0x00, 0x14,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x80,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x00,
};

INT16U x1905_tlv_stream_len_017 = 23;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 018 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct linkMetricResultCodeTLV x1905_tlv_structure_018 =
{
    .tlv_type                    = TLV_TYPE_LINK_METRIC_RESULT_CODE,
    .result_code                 = LINK_METRIC_RESULT_CODE_TLV_INVALID_NEIGHBOR,
};

INT8U x1905_tlv_stream_018[] =
{
    0x0c,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_018 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 019 (TLV <-- packet)
////
////////////////////////////////////////////////////////////////////////////////

struct linkMetricResultCodeTLV x1905_tlv_structure_019 =
{
    .tlv_type                    = TLV_TYPE_LINK_METRIC_RESULT_CODE,
    .result_code                 = 5,
};

INT8U x1905_tlv_stream_019[] =
{
    0x0c,
    0x00, 0x01,
    0x05,
};

INT16U x1905_tlv_stream_len_019 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 020 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct searchedRoleTLV x1905_tlv_structure_020 =
{
    .tlv_type                    = TLV_TYPE_SEARCHED_ROLE,
    .role                        = IEEE80211_ROLE_REGISTRAR,
};

INT8U x1905_tlv_stream_020[] =
{
    0x0d,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_020 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 021 (TLV <-- packet)
////
////////////////////////////////////////////////////////////////////////////////

struct searchedRoleTLV x1905_tlv_structure_021 =
{
    .tlv_type                    = TLV_TYPE_SEARCHED_ROLE,
    .role                        = 0xff,
};

INT8U x1905_tlv_stream_021[] =
{
    0x0d,
    0x00, 0x01,
    0xff,
};

INT16U x1905_tlv_stream_len_021 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 022 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct autoconfigFreqBandTLV x1905_tlv_structure_022 =
{
    .tlv_type                    = TLV_TYPE_AUTOCONFIG_FREQ_BAND,
    .freq_band                   = IEEE80211_FREQUENCY_BAND_2_4_GHZ,
};

INT8U x1905_tlv_stream_022[] =
{
    0x0e,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_022 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 023 (TLV <-- packet)
////
////////////////////////////////////////////////////////////////////////////////

struct autoconfigFreqBandTLV x1905_tlv_structure_023 =
{
    .tlv_type                    = TLV_TYPE_AUTOCONFIG_FREQ_BAND,
    .freq_band                   = 0x1a,
};

INT8U x1905_tlv_stream_023[] =
{
    0x0e,
    0x00, 0x01,
    0x1a,
};

INT16U x1905_tlv_stream_len_023 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 024 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct searchedRoleTLV x1905_tlv_structure_024 =
{
    .tlv_type                    = TLV_TYPE_SUPPORTED_ROLE,
    .role                        = IEEE80211_ROLE_REGISTRAR,
};

INT8U x1905_tlv_stream_024[] =
{
    0x0f,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_024 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 025 (TLV <-- packet)
////
////////////////////////////////////////////////////////////////////////////////

struct searchedRoleTLV x1905_tlv_structure_025 =
{
    .tlv_type                    = TLV_TYPE_SUPPORTED_ROLE,
    .role                        = 0x02,
};

INT8U x1905_tlv_stream_025[] =
{
    0x0f,
    0x00, 0x01,
    0x02,
};

INT16U x1905_tlv_stream_len_025 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 026 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct autoconfigFreqBandTLV x1905_tlv_structure_026 =
{
    .tlv_type                    = TLV_TYPE_SUPPORTED_FREQ_BAND,
    .freq_band                   = IEEE80211_FREQUENCY_BAND_5_GHZ,
};

INT8U x1905_tlv_stream_026[] =
{
    0x10,
    0x00, 0x01,
    0x01,
};

INT16U x1905_tlv_stream_len_026 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 027 (TLV <-- packet)
////
////////////////////////////////////////////////////////////////////////////////

struct autoconfigFreqBandTLV x1905_tlv_structure_027 =
{
    .tlv_type                    = TLV_TYPE_SUPPORTED_FREQ_BAND,
    .freq_band                   = 0x07,
};

INT8U x1905_tlv_stream_027[] =
{
    0x10,
    0x00, 0x01,
    0x07,
};

INT16U x1905_tlv_stream_len_027 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 028 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct pushButtonEventNotificationTLV x1905_tlv_structure_028 =
{
    .tlv_type                    = TLV_TYPE_PUSH_BUTTON_EVENT_NOTIFICATION,
    .media_types_nr              = 2,
    .media_types                 = 
        (struct _mediaTypeEntries[]){
            {
                .media_type                                                        = MEDIA_TYPE_IEEE_802_11AF_GHZ,
                .media_specific_data_size                                          = 10,
                .media_specific_data.ieee80211.network_membership                  = {0x01, 0x01, 0x01, 0x02, 0x02, 0x02},
                .media_specific_data.ieee80211.role                                = IEEE80211_SPECIFIC_INFO_ROLE_WIFI_P2P_CLIENT,
                .media_specific_data.ieee80211.ap_channel_band                     = 0x05,
                .media_specific_data.ieee80211.ap_channel_center_frequency_index_1 = 0x0a,
                .media_specific_data.ieee80211.ap_channel_center_frequency_index_2 = 0x0b,
            },
            {
                .media_type                                      = MEDIA_TYPE_IEEE_1901_WAVELET,
                .media_specific_data_size                        = 7,
                .media_specific_data.ieee1901.network_identifier = {0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0xff},
            },
        },
};

INT8U x1905_tlv_stream_028[] =
{
    0x12,
    0x00, 0x18,
    0x02,
    0x01, 0x07,
    0x0a,
    0x01, 0x01, 0x01, 0x02, 0x02, 0x02,
    0x80,
    0x05,
    0x0a,
    0x0b,
    0x02, 0x00,
    0x07,
    0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0xff,
};

INT16U x1905_tlv_stream_len_028 = 27;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 029 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct powerOffInterfaceTLV x1905_tlv_structure_029 =
{
    .tlv_type                    = TLV_TYPE_POWER_OFF_INTERFACE,
    .power_off_interfaces_nr     = 2,
    .power_off_interfaces        = 
        (struct _powerOffInterfaceEntries[]){
            {
                .interface_address       = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                .media_type              = MEDIA_TYPE_IEEE_802_11G_2_4_GHZ,
                .generic_phy_common_data =
                    {
                        .oui                     = {0x00, 0x00, 0x00},
                        .variant_index           = 0,
                        .media_specific_bytes_nr = 0,
                        .media_specific_bytes    = NULL,
                    },
            },
            {
                .interface_address       = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
                .media_type              = MEDIA_TYPE_UNKNOWN,
                .generic_phy_common_data =
                    {
                        .oui                     = {0x00, 0x19, 0xa7},
                        .variant_index           = 0,
                        .media_specific_bytes_nr = 5,
                        .media_specific_bytes    = (INT8U []){0x01, 0x00, 0x02, 0xaf, 0xb5},
                    },
            },
        },
};

INT8U x1905_tlv_stream_029[] =
{
    0x1b,
    0x00, 0x20,
    0x02,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x01, 0x01,
    0x00, 0x00, 0x00,
    0x00,
    0x00,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0xff, 0xff,
    0x00, 0x19, 0xa7,
    0x00,
    0x05,
    0x01, 0x00, 0x02, 0xaf, 0xb5,
};

INT16U x1905_tlv_stream_len_029 = 35;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 030 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct powerOffInterfaceTLV x1905_tlv_structure_030 =
{
    .tlv_type                    = TLV_TYPE_POWER_OFF_INTERFACE,
    .power_off_interfaces_nr     = 0,
    .power_off_interfaces        = NULL,
};

INT8U x1905_tlv_stream_030[] =
{
    0x1b,
    0x00, 0x01,
    0x00,
};

INT16U x1905_tlv_stream_len_030 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 031 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct genericPhyDeviceInformationTypeTLV x1905_tlv_structure_031 =
{
    .tlv_type                    = TLV_TYPE_GENERIC_PHY_DEVICE_INFORMATION,
    .al_mac_address              = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50},
    .local_interfaces_nr         = 1,
    .local_interfaces            = 
        (struct _genericPhyDeviceEntries[]){
            {
                .local_interface_address = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                .generic_phy_common_data =
                    {
                        .oui                         = {0x00, 0x19, 0xa7},
                        .variant_index               = 0,
                        .media_specific_bytes_nr     = 5,
                        .media_specific_bytes        = (INT8U []){0x01, 0x00, 0x02, 0xaf, 0xb5},
                    },
                .variant_name                        = "ITU-T G.996x Powerline\0\0\0\0\0\0\0\0\0\0",
                .generic_phy_description_xml_url_len = 74,
                .generic_phy_description_xml_url     = "http://wftp3.itu.int/pub/t/fl/itu-t/g/g9979/2014/g9979-GenericPhyInfo.xml",
            },
        },
};

INT8U x1905_tlv_stream_031[] =
{
    0x14,
    0x00, 0x82,
    0x00, 0x10, 0x20, 0x30, 0x40, 0x50,
    0x01,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x00, 0x19, 0xa7,
    0x00,
    0x49, 0x54, 0x55, 0x2d, 0x54, 0x20, 0x47, 0x2e, 0x39, 0x39, 0x36, 0x78, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x6c, 0x69, 0x6e, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x4a,
    0x05,
    0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x66, 0x74, 0x70, 0x33, 0x2e, 0x69, 0x74, 0x75, 0x2e, 0x69, 0x6e, 0x74, 0x2f, 0x70, 0x75, 0x62, 0x2f, 0x74, 0x2f, 0x66, 0x6c, 0x2f, 0x69, 0x74, 0x75, 0x2d, 0x74, 0x2f, 0x67, 0x2f, 0x67, 0x39, 0x39, 0x37, 0x39, 0x2f, 0x32, 0x30, 0x31, 0x34, 0x2f, 0x67, 0x39, 0x39, 0x37, 0x39, 0x2d, 0x47, 0x65, 0x6e, 0x65, 0x72, 0x69, 0x63, 0x50, 0x68, 0x79, 0x49, 0x6e, 0x66, 0x6f, 0x2e, 0x78, 0x6d, 0x6c, 0x00,
    0x01, 0x00, 0x02, 0xaf, 0xb5,
};

INT16U x1905_tlv_stream_len_031 = 133;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 032 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct pushButtonGenericPhyEventNotificationTLV x1905_tlv_structure_032 =
{
    .tlv_type                    = TLV_TYPE_GENERIC_PHY_EVENT_NOTIFICATION,
    .local_interfaces_nr         = 1,
    .local_interfaces            = 
        (struct _genericPhyCommonData[]){
            {
                .oui                      = {0x00, 0x19, 0xa7},
                .variant_index            = 0,
                .media_specific_bytes_nr  = 5,
                .media_specific_bytes     = (INT8U []){0x01, 0x00, 0x02, 0xaf, 0xb5},
            },
        },
};

INT8U x1905_tlv_stream_032[] =
{
    0x19,
    0x00, 0x0b,
    0x01,
    0x00, 0x19, 0xa7,
    0x00,
    0x05,
    0x01, 0x00, 0x02, 0xaf, 0xb5,
};

INT16U x1905_tlv_stream_len_032 = 14;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 033 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct deviceIdentificationTypeTLV x1905_tlv_structure_033 =
{
    .tlv_type                    = TLV_TYPE_DEVICE_IDENTIFICATION,
    .friendly_name               = "Tv in the kitchen\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
    .manufacturer_name           = "Megacorp S.A.\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
    .manufacturer_model          = "LED TVx2001xx\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",

};

INT8U x1905_tlv_stream_033[] =
{
    0x15,
    0x00, 0xc0,
    0x54, 0x76, 0x20, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6b, 0x69, 0x74, 0x63, 0x68, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x4d, 0x65, 0x67, 0x61, 0x63, 0x6f, 0x72, 0x70, 0x20, 0x53, 0x2e, 0x41, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x4c, 0x45, 0x44, 0x20, 0x54, 0x56, 0x78, 0x32, 0x30, 0x30, 0x31, 0x78, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

INT16U x1905_tlv_stream_len_033 = 195;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 034 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct controlUrlTypeTLV x1905_tlv_structure_034 =
{
    .tlv_type                    = TLV_TYPE_CONTROL_URL,
    .url                         = "http://192.168.0.120"

};

INT8U x1905_tlv_stream_034[] =
{
    0x16,
    0x00, 0x15,
    0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x30, 0x2e, 0x31, 0x32, 0x30, 0x00,
};

INT16U x1905_tlv_stream_len_034 = 24;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 035 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct ipv4TypeTLV x1905_tlv_structure_035 =
{
    .tlv_type                    = TLV_TYPE_IPV4,
    .ipv4_interfaces_nr          = 2,
    .ipv4_interfaces             = 
        (struct _ipv4InterfaceEntries[]){
            {
                .mac_address             = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
                .ipv4_nr                 = 1,
                .ipv4                    =
                    (struct _ipv4Entries[]){
                        {
                            .type             = IPV4_TYPE_STATIC,
                            .ipv4_address     = {0x0a, 0x0a, 0x01, 0x04},
                            .ipv4_dhcp_server = {0x00, 0x00, 0x00, 0x00},
                        },
                    },
            },
            {
                .mac_address             = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16},
                .ipv4_nr                 = 2,
                .ipv4                    = 
                    (struct _ipv4Entries[]){
                        {
                            .type             = IPV4_TYPE_DHCP,
                            .ipv4_address     = {0x0a, 0x0a, 0x01, 0xf1},
                            .ipv4_dhcp_server = {0x0a, 0x0a, 0x01, 0xf7},
                        },
                        {
                            .type             = IPV4_TYPE_UNKNOWN,
                            .ipv4_address     = {0x0a, 0x0a, 0x01, 0x0b},
                            .ipv4_dhcp_server = {0x00, 0x00, 0x00, 0x00},
                        },
                    }
            },
        },
};

INT8U x1905_tlv_stream_035[] =
{
    0x17,
    0x00, 0x2a,
    0x02,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x01,
    0x02,
    0x0a, 0x0a, 0x01, 0x04,
    0x00, 0x00, 0x00, 0x00,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x02,
    0x01,
    0x0a, 0x0a, 0x01, 0xf1,
    0x0a, 0x0a, 0x01, 0xf7,
    0x00,
    0x0a, 0x0a, 0x01, 0x0b,
    0x00, 0x00, 0x00, 0x00,
};

INT16U x1905_tlv_stream_len_035 = 45;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 036 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct ipv6TypeTLV x1905_tlv_structure_036 =
{
    .tlv_type                    = TLV_TYPE_IPV6,
    .ipv6_interfaces_nr          = 1,
    .ipv6_interfaces             = 
        (struct _ipv6InterfaceEntries[]){
            {
                .mac_address             = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
                .ipv6_link_local_address = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x04, 0x05, 0x06 },
                .ipv6_nr                 = 2,
                .ipv6                    =
                    (struct _ipv6Entries[]){
                        {
                            .type                = IPV6_TYPE_DHCP,
                            .ipv6_address        = {0x00, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa },
                            .ipv6_address_origin = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                        },
                        {
                            .type                = IPV6_TYPE_SLAAC,
                            .ipv6_address        = {0x00, 0x21, 0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0xbb },
                            .ipv6_address_origin = {0x00, 0x21, 0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                        },
                    },
            },
        },
};

INT8U x1905_tlv_stream_036[] =
{
    0x18,
    0x00, 0x5a,
    0x01,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x04, 0x05, 0x06,
    0x02,
    0x01,
    0x00, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03,
    0x00, 0x21, 0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0xbb,
    0x00, 0x21, 0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

INT16U x1905_tlv_stream_len_036 = 93;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 037 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct x1905ProfileVersionTLV x1905_tlv_structure_037 =
{
    .tlv_type                    = TLV_TYPE_1905_PROFILE_VERSION,
    .profile                     = PROFILE_1905_1A,
};

INT8U x1905_tlv_stream_037[] =
{
    0x1a,
    0x00, 0x01,
    0x01,
};

INT16U x1905_tlv_stream_len_037 = 4;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 038 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct interfacePowerChangeInformationTLV x1905_tlv_structure_038 =
{
    .tlv_type                    = TLV_TYPE_INTERFACE_POWER_CHANGE_INFORMATION,
    .power_change_interfaces_nr  = 3,
    .power_change_interfaces     = 
        (struct _powerChangeInformationEntries[]){
            {
                .interface_address       = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                .requested_power_state   = POWER_STATE_REQUEST_OFF,
            },
            {
                .interface_address       = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
                .requested_power_state   = POWER_STATE_REQUEST_ON,
            },
            {
                .interface_address       = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25},
                .requested_power_state   = POWER_STATE_REQUEST_SAVE,
            },
        },
};

INT8U x1905_tlv_stream_038[] =
{
    0x1c,
    0x00, 0x16,
    0x03,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x00,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x01,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x02,
};

INT16U x1905_tlv_stream_len_038 = 25;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 039 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct interfacePowerChangeStatusTLV x1905_tlv_structure_039 =
{
    .tlv_type                    = TLV_TYPE_INTERFACE_POWER_CHANGE_STATUS,
    .power_change_interfaces_nr  = 3,
    .power_change_interfaces     = 
        (struct _powerChangeStatusEntries[]){
            {
                .interface_address       = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                .result                  = POWER_STATE_RESULT_COMPLETED,
            },
            {
                .interface_address       = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
                .result                  = POWER_STATE_RESULT_NO_CHANGE,
            },
            {
                .interface_address       = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25},
                .result                  = POWER_STATE_RESULT_ALTERNATIVE_CHANGE,
            },
        },
};

INT8U x1905_tlv_stream_039[] =
{
    0x1d,
    0x00, 0x16,
    0x03,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x00,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x01,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x02,
};

INT16U x1905_tlv_stream_len_039 = 25;


////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 040 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct l2NeighborDeviceTLV x1905_tlv_structure_040 =
{
    .tlv_type                    = TLV_TYPE_L2_NEIGHBOR_DEVICE,
    .local_interfaces_nr         = 3,
    .local_interfaces            = 
        (struct _l2InterfacesEntries[]){
            {
                .local_mac_address       = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                .l2_neighbors_nr         = 0,
                .l2_neighbors            = NULL,
            },
            {
                .local_mac_address       = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
                .l2_neighbors_nr         = 2,
                .l2_neighbors            =
                    (struct _l2NeighborsEntries[]){
                        {
                            .l2_neighbor_mac_address = {0xaa, 0xaa, 0xaa, 0x10, 0x01, 0x00},
                            .behind_mac_addresses_nr = 0,
                            .behind_mac_addresses    = NULL,
                        },
                        {
                            .l2_neighbor_mac_address = {0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x00},
                            .behind_mac_addresses_nr = 2,
                            .behind_mac_addresses    =
                                (INT8U [][6]){
                                    {0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x01},
                                    {0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x02},
                                },
                        },
                    }
            },
            {
                .local_mac_address       = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25},
                .l2_neighbors_nr         = 1,
                .l2_neighbors            =
                    (struct _l2NeighborsEntries[]){
                        {
                            .l2_neighbor_mac_address = {0xaa, 0xaa, 0xaa, 0x20, 0x01, 0x00},
                            .behind_mac_addresses_nr = 0,
                            .behind_mac_addresses    = NULL,
                        },
                    }
            },
        },
};

INT8U x1905_tlv_stream_040[] =
{
    0x1e,
    0x00, 0x3d,
    0x03,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x00, 0x00,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x00, 0x02,
    0xaa, 0xaa, 0xaa, 0x10, 0x01, 0x00,
    0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x00,
    0x00, 0x02,
    0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x01,
    0xaa, 0xaa, 0xaa, 0x10, 0x02, 0x02,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x00, 0x01,
    0xaa, 0xaa, 0xaa, 0x20, 0x01, 0x00,
    0x00, 0x00,
};


INT16U x1905_tlv_stream_len_040 = 64;

////////////////////////////////////////////////////////////////////////////////
////
//// Test vector 041 (TLV <--> packet)
////
////////////////////////////////////////////////////////////////////////////////

struct vendorSpecificTLV x1905_tlv_structure_041 =
{
    .tlv_type                    = TLV_TYPE_VENDOR_SPECIFIC,
    .vendorOUI                   = {0x00, 0x03, 0x7f},
    .m_nr                        = 21,
    .m                           = (INT8U[]) {0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
};

INT8U x1905_tlv_stream_041[] =
{
    0x0b,
    0x00, 0x18,
    0x00, 0x03, 0x7f,
    0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

INT16U x1905_tlv_stream_len_041 = 27;
