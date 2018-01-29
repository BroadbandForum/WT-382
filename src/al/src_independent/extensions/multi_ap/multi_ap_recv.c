/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-23
 *
 *
 */
#include "al_datamodel.h"
#include "al_recv.h"
#include "al_extension.h" // VendorSpecificTLVDuplicate
#include "1905_tlvs.h"
#include "multi_ap_tlvs.h"


////////////////////////////////////////////////////////////////////////////////
// CMDU extension callbacks
////////////////////////////////////////////////////////////////////////////////

INT8U CBKprocess1905_Multi_AP_Extensions(struct CMDU *memory_structure)
{
    INT8U     *p;
    INT8U      i;

    struct vendorSpecificTLV   *vs_tlv;

    if (NULL == memory_structure)
    {
        // Invalid param
        //
        return 0;
    }

    if (NULL == memory_structure->list_of_TLVs)
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Malformed structure.");
        return 0;
    }

    // BBF protocol extension: Metrics of non-1905 links. Interested only on:
    //
    // CMDU_TYPE_LINK_METRIC_QUERY 
    // `--> TLV_TYPE_VENDOR_SPECIFIC (BBF oui)
    //      `--> BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY
    //
    // CMDU_TYPE_LINK_METRIC_RESPONSE
    // `--> TLV_TYPE_VENDOR_SPECIFIC (BBF oui)
    //      |--> BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC
    //      |--> BBF_TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC
    //      `--> BBF_TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE
    //
    // Future expectations: non-1905 link metrics should be included in the
    // IEEE1905 standard. Meanwhile, a BBF protocol extension can be used.
    //
    switch (memory_structure->message_type)
    {
        //todo add cmdu process
        default:
        {
            break;
        }
    }

    return 1;
}
