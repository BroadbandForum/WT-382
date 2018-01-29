/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-29
 *
 *
 */

//
// This file tests the "forge_non_1905_TLV_from_structure()" function by
// providing some test input structures and checking the generated output
// stream.
//

#include "platform.h"
#include "multi_ap_tlvs.h"
#include "multi_ap_tlv_test_vectors.h"

INT8U _check(const char *test_description, INT8U mode, INT8U *input, INT8U *expected_output, INT16U expected_output_len)
{
    INT8U  result;
    INT8U *real_output;
    INT16U real_output_len;
    

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

    // Return the number of test cases that failed
    //
    return result;
}

