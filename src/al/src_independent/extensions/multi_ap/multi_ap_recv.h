/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-23
 *
 *
 */
#ifndef _MULTI_AP_RECV_H_
#define _MULTI_AP_RECV_H_

#include "1905_cmdus.h"


// Process BBF TLVs included in the incoming CMDU structure
//
// This function allows to parse the new TLVs defined in the BBF community.
// According to the standard, any CMDU is subject to be extended with extra
// Vendor Specific TLVs, so each inserted BBF TLV will be embedded inside a
// Vendor Specific TLV.
// This implementation will only process defined BBF TLVs embedded inside a
// Vendor Specific TLV whose OUI is the BBF one (0x00256d)
//
// 'memory_structure' is the CMDU structure
//
// Return '0' if there was a problem, '1' otherwise
//
INT8U CBKprocess1905_Multi_AP_Extensions(struct CMDU *memory_structure);

#endif


