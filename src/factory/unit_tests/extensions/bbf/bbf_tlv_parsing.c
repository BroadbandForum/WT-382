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

//
// This file tests the "parse_non_1905_TLV_from_packet()" function by providing
// some test input streams and checking the generated output structure.
//

#include "platform.h"
#include "utils.h"

#include "bbf_tlvs.h"
#include "bbf_tlv_test_vectors.h"

INT8U _check(const char *test_description, INT8U mode, INT8U *input, INT8U *expected_output)
{
    INT8U  result;
    INT8U *real_output;
    INT8U  comparison;
    
    // Parse the packet
    real_output = parse_bbf_TLV_from_packet(input);

    // Compare TLVs
    comparison = compare_bbf_TLV_structures(real_output, expected_output);

    if (mode == CHECK_TRUE)
    {
        if (0 == comparison)
        {
            result = 0;
            PLATFORM_PRINTF("%-100s: OK\n", test_description);
        }
        else
        {
            result = 1;
            PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);

            // CheckTrue error needs more debug info
            PLATFORM_PRINTF("  Expected output:\n");
            visit_bbf_TLV_structure(expected_output, print_callback, PLATFORM_PRINTF, "");
            PLATFORM_PRINTF("  Real output    :\n");
            visit_bbf_TLV_structure(real_output, print_callback, PLATFORM_PRINTF, "");
        }
    }
    else
    {
        if (0 == comparison)
        {
            result = 1;
            PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);
        }
        else
        {
            result = 0;
            PLATFORM_PRINTF("%-100s: OK\n", test_description);
        }
    }

    return result;
}

INT8U _checkTrue(const char *test_description, INT8U *input, INT8U *expected_output)
{
  return _check(test_description, CHECK_TRUE, input, expected_output);
}

INT8U _checkFalse(const char *test_description, INT8U *input, INT8U *expected_output)
{
  return _check(test_description, CHECK_FALSE, input, expected_output);
}


int main(void)
{
    INT8U result = 0;

    #define BBFTLVPARSE001 "BBFTLVPARSE001 - Parse non-1905 link metric query TLV (bbf_tlv_stream_001)"
    result += _checkTrue(BBFTLVPARSE001, bbf_tlv_stream_001, (INT8U *)&bbf_tlv_structure_001);

    #define BBFTLVFORGE002 "BBFTLVPARSE002 - Parse non-1905 link metric query TLV (bbf_tlv_stream_003)"
    result += _checkTrue(BBFTLVFORGE002, bbf_tlv_stream_003, (INT8U *)&bbf_tlv_structure_003);

    #define BBFTLVFORGE003 "BBFTLVPARSE003 - Parse non-1905 transmitter link metric TLV (bbf_tlv_stream_005)"
    result += _checkTrue(BBFTLVFORGE003, bbf_tlv_stream_005, (INT8U *)&bbf_tlv_structure_005);

    #define BBFTLVFORGE004 "BBFTLVPARSE004 - Parse non-1905 receiver link metric TLV (bbf_tlv_stream_007)"
    result += _checkTrue(BBFTLVFORGE004, bbf_tlv_stream_007, (INT8U *)&bbf_tlv_structure_007);

    #define BBFTLVFORGE005 "BBFTLVPARSE005 - Parse non-1905 link metric query TLV (bbf_tlv_stream_008)"
    result += _checkFalse(BBFTLVFORGE005, bbf_tlv_stream_002b, (INT8U *)&bbf_tlv_structure_002);

    #define BBFTLVFORGE006 "BBFTLVPARSE006 - Parse non-1905 transmitter link metric TLV (bbf_tlv_stream_009)"
    result += _checkFalse(BBFTLVFORGE006, bbf_tlv_stream_004b, (INT8U *)&bbf_tlv_structure_004);

    #define BBFTLVFORGE007 "BBFTLVPARSE007 - Parse non-1905 receiver link metric TLV (bbf_tlv_stream_010)"
    result += _checkFalse(BBFTLVFORGE007, bbf_tlv_stream_006b, (INT8U *)&bbf_tlv_structure_006);

    // Return the number of test cases that failed
    //
    return result;
}






