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
#include "1905_cmdus.h"

// This file contains test vectors than can be used to check the
// "parse_1905_CMDU_from_packets()" and "forge_1905_CMDU_from_structure()"
// functions
//
// Each test vector is made up of three variables:
//
//   - A CMDU structure
//   - An arrray of streams
//   - An array with the lengths of each stream
//
// Note that some test vectors can be used to test both functions, while others
// can only be used to test "forge_1905_CMDU_from_structure()" or
// "parse_1905_CMDU_from_packets()":
//
//   - Test vectors marked with "CMDU --> packet" can only be used to test the
//     "forge_1905_TLV_from_structure()" function.
//
//   - Test vectors marked with "CMDU <-- packet" can only be used to test the
//     "parse_1905_CMDU_from_packets()" function.
//
//   - All the other test vectors are marked with "CMDU <--> packet", meaning
//     they can be used to test  both functions.
//
// The reason this is happening is that, according to the standard, sometimes
// bits are ignored/changed/forced-to-a-value when forging a packet. Thus, not
// all test vectors are "inversible" (ie. forge(parse(stream)) != x)
//
// This is how you use these test vectors:
//
//   A) stream = forge_1905_CMDU_from_structure(tlv_xxx, &stream_len);
//
//   B) tlv = parse_1905_CMDU_from_packets(stream_xxx); 
//


////////////////////////////////////////////////////////////////////////////////
//// Test vector 001 (CMDU <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct CMDU x1905_cmdu_structure_001 =
{
    .message_version = CMDU_MESSAGE_VERSION_1905_1_2013,
    .message_type    = CMDU_TYPE_LINK_METRIC_QUERY,
    .message_id      = 7,
    .relay_indicator = 0,
    .list_of_TLVs    =
        (INT8U* []){
            (INT8U *)(struct linkMetricQueryTLV[]){
                {
                    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
                    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
                    .specific_neighbor = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
                },
            },
            NULL
        },
};

INT8U *x1905_cmdu_streams_001[] =
{
    (INT8U []){
        0x00,
        0x00,
        0x00, 0x05,
        0x00, 0x07,
        0x00,
        0x80,

        0x08,
        0x00, 0x08,
        0x00,
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02,

        0x00,
        0x00, 0x00,
    },
    NULL
};

INT16U x1905_cmdu_streams_len_001[] = {22, 0};


////////////////////////////////////////////////////////////////////////////////
//// Test vector 002 (CMDU <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct CMDU x1905_cmdu_structure_002 =
{
    .message_version = CMDU_MESSAGE_VERSION_1905_1_2013,
    .message_type   = CMDU_TYPE_LINK_METRIC_QUERY,
    .message_id      = 2348,
    .relay_indicator = 0,
    .list_of_TLVs    =
        (INT8U* []){
            (INT8U *)(struct linkMetricQueryTLV[]){
                {
                    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
                    .destination       = LINK_METRIC_QUERY_TLV_SPECIFIC_NEIGHBOR,
                    .specific_neighbor = {0x01, 0x02, 0x02, 0x03, 0x04, 0x05},
                    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
                },
            },
            NULL
        },
};

INT8U *x1905_cmdu_streams_002[] =
{
    (INT8U []){
        0x00,
        0x00,
        0x00, 0x05,
        0x09, 0x2c,
        0x00,
        0x80,

        0x08,
        0x00, 0x08,
        0x01,
        0x01, 0x02, 0x02, 0x03, 0x04, 0x05,
        0x02,

        0x00,
        0x00, 0x00,
    },
    NULL
};

INT16U x1905_cmdu_streams_len_002[] = {22, 0};


////////////////////////////////////////////////////////////////////////////////
//// Test vector 003 (CMDU --> packet)
////////////////////////////////////////////////////////////////////////////////

struct CMDU x1905_cmdu_structure_003 =
{
    .message_version = CMDU_MESSAGE_VERSION_1905_1_2013,
    .message_type    = CMDU_TYPE_LINK_METRIC_QUERY,
    .message_id      = 2348,
    .relay_indicator = 1,
    .list_of_TLVs    =
        (INT8U* []){
            (INT8U *)(struct linkMetricQueryTLV[]){
                {
                    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
                    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
                    .specific_neighbor = {0x01, 0x02, 0x02, 0x03, 0x04, 0x05},
                    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
                },
            },
            NULL
        },
};

INT8U *x1905_cmdu_streams_003[] =
{
    (INT8U []){
        0x00,
        0x00,
        0x00, 0x05,
        0x09, 0x2c,
        0x00,
        0x80, // Note that 'relay_indicator' is *not* set because for
              // this type of message ("CMDU_TYPE_LINK_METRIC_QUERY")
              // it must always be set to '0'

        0x08,
        0x00, 0x08,
        0x00,
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02,

        0x00,
        0x00, 0x00,
    },
    NULL
};

INT16U x1905_cmdu_streams_len_003[] = {22, 0};


////////////////////////////////////////////////////////////////////////////////
//// Test vector 004 (CMDU <-- packet)
////////////////////////////////////////////////////////////////////////////////

struct CMDU x1905_cmdu_structure_004 =
{
    .message_version = CMDU_MESSAGE_VERSION_1905_1_2013,
    .message_type    = CMDU_TYPE_LINK_METRIC_QUERY,
    .message_id      = 2348,
    .relay_indicator = 0,
    .list_of_TLVs    =
        (INT8U* []){
            (INT8U *)(struct linkMetricQueryTLV[]){
                {
                    .tlv_type          = TLV_TYPE_LINK_METRIC_QUERY,
                    .destination       = LINK_METRIC_QUERY_TLV_ALL_NEIGHBORS,
                    .specific_neighbor = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                    .link_metrics_type = LINK_METRIC_QUERY_TLV_BOTH_TX_AND_RX_LINK_METRICS,
                },
            },
            NULL
        },
};

INT8U *x1905_cmdu_streams_004[] =
{
    (INT8U []){
        0x00,
        0x00,
        0x00, 0x05,
        0x09, 0x2c,
        0x00,
        0x80, 

        0x08,
        0x00, 0x08,
        0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02,

        0x00,
        0x00, 0x00,
    },
    NULL
};

INT16U x1905_cmdu_streams_len_004[] = {22, 0};


////////////////////////////////////////////////////////////////////////////////
//// Test vector 005 (CMDU <--> packet)
////////////////////////////////////////////////////////////////////////////////

struct CMDU x1905_cmdu_structure_005 =
{
    .message_version = CMDU_MESSAGE_VERSION_1905_1_2013,
    .message_type    = CMDU_TYPE_TOPOLOGY_QUERY,
    .message_id      = 9,
    .relay_indicator = 0,
    .list_of_TLVs    =
        (INT8U* []){
            NULL
        },
};

INT8U *x1905_cmdu_streams_005[] =
{
    (INT8U []){
        0x00,
        0x00,
        0x00, 0x02,
        0x00, 0x09,
        0x00,
        0x80, 

        0x00,
        0x00, 0x00,
    },
    NULL
};

INT16U x1905_cmdu_streams_len_005[] = {11, 0};


// TODO: More tests for all types of CMDUs

