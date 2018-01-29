/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-29
 *
 *
 */
//
// This file tests the "parse_non_1905_TLV_from_packet()" function by providing
// some test input streams and checking the generated output structure.
//

#include "platform.h"
#include "utils.h"

#include "multi_ap_tlvs.h"
#include "multi_ap_tlv_test_vectors.h"

INT8U _check(const char *test_description, INT8U mode, INT8U *input, INT8U *expected_output)
{
    INT8U  result;
    INT8U *real_output;
    INT8U  comparison;
    

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






