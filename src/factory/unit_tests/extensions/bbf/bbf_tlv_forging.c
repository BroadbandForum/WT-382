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
// This file tests the "forge_non_1905_TLV_from_structure()" function by
// providing some test input structures and checking the generated output
// stream.
//

#include "platform.h"
#include "bbf_tlvs.h"
#include "bbf_tlv_test_vectors.h"

INT8U _check(const char *test_description, INT8U mode, INT8U *input, INT8U *expected_output, INT16U expected_output_len)
{
    INT8U  result;
    INT8U *real_output;
    INT16U real_output_len;
    
    // Build the packet
    real_output = forge_bbf_TLV_from_structure((INT8U *)input, &real_output_len);

    if (NULL == real_output)
    {
        PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);
        PLATFORM_PRINTF("  forge_non_1905_TLV_from_structure() returned a NULL pointer\n");

        return 1;
    }

    // Compare packets
    if (mode == CHECK_TRUE)
    {
        // Compare the packets
        if ((expected_output_len == real_output_len) && (0 == PLATFORM_MEMCMP(expected_output, real_output, real_output_len)))
        {
            result = 0;
            PLATFORM_PRINTF("%-100s: OK\n", test_description);
        }
        else
        {
            INT16U i;

            result = 1;
            PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);

            // CheckTrue error needs more debug info
            //
            PLATFORM_PRINTF("  Expected output: ");
            for (i=0; i<expected_output_len; i++)
            {
                PLATFORM_PRINTF("%02x ",expected_output[i]);
            }
            PLATFORM_PRINTF("\n");
            PLATFORM_PRINTF("  Real output    : ");
            for (i=0; i<real_output_len; i++)
            {
                PLATFORM_PRINTF("%02x ",real_output[i]);
            }
            PLATFORM_PRINTF("\n");
        }
    }
    else
    {
        if ((expected_output_len == real_output_len) && (0 == PLATFORM_MEMCMP(expected_output, real_output, real_output_len)))
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

INT8U _checkTrue(const char *test_description, INT8U *input, INT8U *expected_output, INT16U expected_output_len)
{
  return _check(test_description, CHECK_TRUE, input, expected_output, expected_output_len);
}

INT8U _checkFalse(const char *test_description, INT8U *input, INT8U *expected_output, INT16U expected_output_len)
{
  return _check(test_description, CHECK_FALSE, input, expected_output, expected_output_len);
}


int main(void)
{
    INT8U result = 0;

    #define BBFTLVFORGE001 "BBFTLVFORGE001 - Forge non-1905 link metric query TLV (bbf_tlv_structure_001)"
    result += _checkTrue(BBFTLVFORGE001, (INT8U *)&bbf_tlv_structure_001, bbf_tlv_stream_001, bbf_tlv_stream_len_001);

    #define BBFTLVFORGE002 "BBFTLVFORGE002 - Forge non-1905 link metric query TLV (bbf_tlv_structure_002)"
    result += _checkTrue(BBFTLVFORGE002, (INT8U *)&bbf_tlv_structure_002, bbf_tlv_stream_002, bbf_tlv_stream_len_002);

    #define BBFTLVFORGE003 "BBFTLVFORGE003 - Forge non-1905 link metric query TLV (bbf_tlv_structure_003)"
    result += _checkTrue(BBFTLVFORGE003, (INT8U *)&bbf_tlv_structure_003, bbf_tlv_stream_003, bbf_tlv_stream_len_003);

    #define BBFTLVFORGE004 "BBFTLVFORGE004 - Forge non-1905 transmitter link metric TLV (bbf_tlv_structure_004)"
    result += _checkTrue(BBFTLVFORGE004, (INT8U *)&bbf_tlv_structure_004, bbf_tlv_stream_004, bbf_tlv_stream_len_004);

    #define BBFTLVFORGE005 "BBFTLVFORGE005 - Forge non-1905 transmitter link metric TLV (bbf_tlv_structure_005)"
    result += _checkTrue(BBFTLVFORGE005, (INT8U *)&bbf_tlv_structure_005, bbf_tlv_stream_005, bbf_tlv_stream_len_005);

    #define BBFTLVFORGE006 "BBFTLVFORGE006 - Forge non-1905 receiver link metric TLV (bbf_tlv_structure_006)"
    result += _checkTrue(BBFTLVFORGE006, (INT8U *)&bbf_tlv_structure_006, bbf_tlv_stream_006, bbf_tlv_stream_len_006);

    #define BBFTLVFORGE007 "BBFTLVFORGE007 - Forge non-1905 receiver link metric TLV (bbf_tlv_structure_007)"
    result += _checkTrue(BBFTLVFORGE007, (INT8U *)&bbf_tlv_structure_007, bbf_tlv_stream_007, bbf_tlv_stream_len_007);

    // Return the number of test cases that failed
    //
    return result;
}

