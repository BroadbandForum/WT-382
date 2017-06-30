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
// This file tests the "parse_1905_CMDU_from_packets()" function by providing
// some test input streams and checking the generated output structure.
//

#include "platform.h"
#include "utils.h"

#include "1905_tlvs.h"
#include "1905_cmdus.h"
#include "1905_cmdu_test_vectors.h"

INT8U _check(const char *test_description, INT8U **input, struct CMDU *expected_output)
{
    INT8U  result;
    struct CMDU *real_output;
    
    real_output = parse_1905_CMDU_from_packets(input);

    if (0 == compare_1905_CMDU_structures(real_output, expected_output))
    {
        result = 0;
        PLATFORM_PRINTF("%-100s: OK\n", test_description);
    }
    else
    {
        result = 1; 
        PLATFORM_PRINTF("%-100s: KO !!!\n", test_description);
        PLATFORM_PRINTF("  Expected output:\n");
        visit_1905_CMDU_structure(expected_output, print_callback, PLATFORM_PRINTF, "");
        PLATFORM_PRINTF("  Real output    :\n");
        visit_1905_CMDU_structure(real_output, print_callback, PLATFORM_PRINTF, "");
    }

    return result;
}


int main(void)
{
    INT8U result = 0;

    #define x1905CMDUPARSE001 "x1905CMDUPARSE001 - Parse link metric query CMDU (x1905_cmdu_streams_001)"
    result += _check(x1905CMDUPARSE001, x1905_cmdu_streams_001, &x1905_cmdu_structure_001);

    #define x1905CMDUPARSE002 "x1905CMDUPARSE002 - Parse link metric query CMDU (x1905_cmdu_streams_002)"
    result += _check(x1905CMDUPARSE002, x1905_cmdu_streams_002, &x1905_cmdu_structure_002);

    #define x1905CMDUPARSE003 "x1905CMDUPARSE003 - Parse link metric query CMDU (x1905_cmdu_streams_004)"
    result += _check(x1905CMDUPARSE003, x1905_cmdu_streams_004, &x1905_cmdu_structure_004);

    #define x1905CMDUPARSE004 "x1905CMDUPARSE004 - Parse topology query CMDU (x1905_cmdu_streams_005)"
    result += _check(x1905CMDUPARSE004, x1905_cmdu_streams_005, &x1905_cmdu_structure_005);

    // Return the number of test cases that failed
    //
    return result;
}







