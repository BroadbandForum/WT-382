/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-29
 *
 *
 */
#include "1905_tlvs.h"
#include "multi_ap_tlvs.h"


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

