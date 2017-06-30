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
// This file tests the "forge_1905_ALME_from_structure()" function by providing
// some test input structures and checking the generated output stream.
//

#include "platform.h"
#include "1905_tlvs.h"
#include "1905_alme.h"
#include "1905_alme_test_vectors.h"

INT8U _check(const char *test_description, INT8U *input, INT8U *expected_output, INT16U expected_output_len)
{
    INT8U  result;
    INT8U *real_output;
    INT16U real_output_len;
    
    real_output = forge_1905_ALME_from_structure((INT8U *)input, &real_output_len);

    if (NULL == real_output)
    {
        PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);
        PLATFORM_PRINTF("  forge_1905_ALME_from_structure() returned a NULL pointer\n");

        return 1;
    }

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

    return result;
}


int main(void)
{
    INT8U result = 0;

    #define x1905ALMEFORGE001 "x1905ALMEFORGE001 - Forge ALME-GET-INTF-LIST.request (x1905_alme_structure_001)"
    result += _check(x1905ALMEFORGE001, (INT8U *)&x1905_alme_structure_001, x1905_alme_stream_001, x1905_alme_stream_len_001);

    #define x1905ALMEFORGE002 "x1905ALMEFORGE002 - Forge ALME-GET-INTF-LIST.response (x1905_alme_structure_002)"
    result += _check(x1905ALMEFORGE002, (INT8U *)&x1905_alme_structure_002, x1905_alme_stream_002, x1905_alme_stream_len_002);

    #define x1905ALMEFORGE003 "x1905ALMEFORGE003 - Forge ALME-GET-INTF-LIST.response (x1905_alme_structure_003)"
    result += _check(x1905ALMEFORGE003, (INT8U *)&x1905_alme_structure_003, x1905_alme_stream_003, x1905_alme_stream_len_003);

    #define x1905ALMEFORGE004 "x1905ALMEFORGE004 - Forge ALME-GET-INTF-LIST.response (x1905_alme_structure_004)"
    result += _check(x1905ALMEFORGE004, (INT8U *)&x1905_alme_structure_004, x1905_alme_stream_004, x1905_alme_stream_len_004);

    #define x1905ALMEFORGE005 "x1905ALMEFORGE005 - Forge ALME-SET-INTF-PWR-STATE.request (x1905_alme_structure_005)"
    result += _check(x1905ALMEFORGE005, (INT8U *)&x1905_alme_structure_005, x1905_alme_stream_005, x1905_alme_stream_len_005);

    #define x1905ALMEFORGE006 "x1905ALMEFORGE006 - Forge ALME-SET-INTF-PWR-STATE.request (x1905_alme_structure_006)"
    result += _check(x1905ALMEFORGE006, (INT8U *)&x1905_alme_structure_006, x1905_alme_stream_006, x1905_alme_stream_len_006);

    #define x1905ALMEFORGE007 "x1905ALMEFORGE007 - Forge ALME-SET-INTF-PWR-STATE.confirm (x1905_alme_structure_007)"
    result += _check(x1905ALMEFORGE007, (INT8U *)&x1905_alme_structure_007, x1905_alme_stream_007, x1905_alme_stream_len_007);

    #define x1905ALMEFORGE008 "x1905ALMEFORGE008 - Forge ALME-SET-INTF-PWR-STATE.confirm (x1905_alme_structure_008)"
    result += _check(x1905ALMEFORGE008, (INT8U *)&x1905_alme_structure_008, x1905_alme_stream_008, x1905_alme_stream_len_008);

    #define x1905ALMEFORGE009 "x1905ALMEFORGE009 - Forge ALME-GET-INTF-PWR-STATE.request (x1905_alme_structure_009)"
    result += _check(x1905ALMEFORGE009, (INT8U *)&x1905_alme_structure_009, x1905_alme_stream_009, x1905_alme_stream_len_009);

    #define x1905ALMEFORGE010 "x1905ALMEFORGE010 - Forge ALME-GET-INTF-PWR-STATE.response (x1905_alme_structure_010)"
    result += _check(x1905ALMEFORGE010, (INT8U *)&x1905_alme_structure_010, x1905_alme_stream_010, x1905_alme_stream_len_010);

    #define x1905ALMEFORGE011 "x1905ALMEFORGE011 - Forge ALME-SET-FWD-RULE.request (x1905_alme_structure_011)"
    result += _check(x1905ALMEFORGE011, (INT8U *)&x1905_alme_structure_011, x1905_alme_stream_011, x1905_alme_stream_len_011);

    #define x1905ALMEFORGE012 "x1905ALMEFORGE012 - Forge ALME-SET-FWD-RULE.request (x1905_alme_structure_012)"
    result += _check(x1905ALMEFORGE012, (INT8U *)&x1905_alme_structure_012, x1905_alme_stream_012, x1905_alme_stream_len_012);

    #define x1905ALMEFORGE013 "x1905ALMEFORGE013 - Forge ALME-SET-FWD-RULE.confirm (x1905_alme_structure_013)"
    result += _check(x1905ALMEFORGE013, (INT8U *)&x1905_alme_structure_013, x1905_alme_stream_013, x1905_alme_stream_len_013);

    #define x1905ALMEFORGE014 "x1905ALMEFORGE014 - Forge ALME-GET-FWD-RULES.request (x1905_alme_structure_014)"
    result += _check(x1905ALMEFORGE014, (INT8U *)&x1905_alme_structure_014, x1905_alme_stream_014, x1905_alme_stream_len_014);

    #define x1905ALMEFORGE015 "x1905ALMEFORGE015 - Forge ALME-GET-FWD-RULES.response (x1905_alme_structure_015)"
    result += _check(x1905ALMEFORGE015, (INT8U *)&x1905_alme_structure_015, x1905_alme_stream_015, x1905_alme_stream_len_015);

    #define x1905ALMEFORGE016 "x1905ALMEFORGE016 - Forge ALME-GET-FWD-RULES.response (x1905_alme_structure_016)"
    result += _check(x1905ALMEFORGE016, (INT8U *)&x1905_alme_structure_016, x1905_alme_stream_016, x1905_alme_stream_len_016);

    #define x1905ALMEFORGE017 "x1905ALMEFORGE017 - Forge ALME-GET-FWD-RULES.response (x1905_alme_structure_017)"
    result += _check(x1905ALMEFORGE017, (INT8U *)&x1905_alme_structure_017, x1905_alme_stream_017, x1905_alme_stream_len_017);

    #define x1905ALMEFORGE018 "x1905ALMEFORGE018 - Forge ALME-MODIFY-FWD-RULE.request (x1905_alme_structure_018)"
    result += _check(x1905ALMEFORGE018, (INT8U *)&x1905_alme_structure_018, x1905_alme_stream_018, x1905_alme_stream_len_018);

    #define x1905ALMEFORGE019 "x1905ALMEFORGE019 - Forge ALME-MODIFY-FWD-RULE.confirm (x1905_alme_structure_019)"
    result += _check(x1905ALMEFORGE019, (INT8U *)&x1905_alme_structure_019, x1905_alme_stream_019, x1905_alme_stream_len_019);

    #define x1905ALMEFORGE020 "x1905ALMEFORGE020 - Forge ALME-MODIFY-FWD-RULE.confirm (x1905_alme_structure_020)"
    result += _check(x1905ALMEFORGE020, (INT8U *)&x1905_alme_structure_020, x1905_alme_stream_020, x1905_alme_stream_len_020);

    #define x1905ALMEFORGE021 "x1905ALMEFORGE021 - Forge ALME-REMOVE-FWD-RULE.request (x1905_alme_structure_021)"
    result += _check(x1905ALMEFORGE021, (INT8U *)&x1905_alme_structure_021, x1905_alme_stream_021, x1905_alme_stream_len_021);

    #define x1905ALMEFORGE022 "x1905ALMEFORGE022 - Forge ALME-REMOVE-FWD-RULE.confirm (x1905_alme_structure_022)"
    result += _check(x1905ALMEFORGE022, (INT8U *)&x1905_alme_structure_022, x1905_alme_stream_022, x1905_alme_stream_len_022);

    #define x1905ALMEFORGE023 "x1905ALMEFORGE023 - Forge ALME-GET-METRIC.request (x1905_alme_structure_023)"
    result += _check(x1905ALMEFORGE023, (INT8U *)&x1905_alme_structure_023, x1905_alme_stream_023, x1905_alme_stream_len_023);

    #define x1905ALMEFORGE024 "x1905ALMEFORGE024 - Forge ALME-GET-METRIC.response (x1905_alme_structure_024)"
    result += _check(x1905ALMEFORGE024, (INT8U *)&x1905_alme_structure_024, x1905_alme_stream_024, x1905_alme_stream_len_024);

    #define x1905ALMEFORGE025 "x1905ALMEFORGE025 - Forge ALME-GET-METRIC.response (x1905_alme_structure_025)"
    result += _check(x1905ALMEFORGE025, (INT8U *)&x1905_alme_structure_025, x1905_alme_stream_025, x1905_alme_stream_len_025);

    // Return the number of test cases that failed
    //
    return result;
}






