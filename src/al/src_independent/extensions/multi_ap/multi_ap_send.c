/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-23
 *
 *
 */

#include "utils.h"
#include "al_send.h"
#include "1905_cmdus.h"
#include "1905_tlvs.h"
#include "multi_ap_tlvs.h"

#include "al_datamodel.h"
#include "al_extension.h"
#include "platform_interfaces.h"


////////////////////////////////////////////////////////////////////////////////
// Datamodel extension callbacks
////////////////////////////////////////////////////////////////////////////////

void CBKObtain_Multi_AP_ExtendedLocalInfo(struct vendorSpecificTLV ***extensions,
                                   INT8U                      *nr)
{
    PLATFORM_PRINTF_DEBUG_INFO("[Multi AP] CBKObtain_Multi_AP_ExtendedLocalInfo() \n");

}


void CBKUpdate_Multi_AP_ExtendedInfo(struct vendorSpecificTLV **extensions, INT8U nr, INT8U *al_mac_address)
{
    PLATFORM_PRINTF_DEBUG_INFO("[Multi AP] CBKUpdate_Multi_AP_ExtendedInfo() \n");

}


void CBKDump_Multi_AP_ExtendedInfo(INT8U **memory_structure,
                            INT8U   structure_nr,
                            void  (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p),
                            void  (*write_function)(const char *fmt, ...),
                            const char *prefix)
{

    PLATFORM_PRINTF_DEBUG_INFO("[Multi AP] CBKDump_Multi_AP_ExtendedInfo() \n");

}


////////////////////////////////////////////////////////////////////////////////
// CMDU extension callbacks
////////////////////////////////////////////////////////////////////////////////

INT8U CBKSend1905_Multi_AP_Extensions(struct CMDU *memory_structure)
{
    if ((NULL == memory_structure) || (NULL == memory_structure->list_of_TLVs))
    {
        // Invalid arguments
        //
        return 0;
    }

    // Extend the CMDU content (more TLVs)
    //
    switch(memory_structure->message_type)
    {

        //todo add cmdu process

        // No more TLVs will be added to the CMDU
        //
        default:
        {
            break;
        }
    }

    return 1;
}


