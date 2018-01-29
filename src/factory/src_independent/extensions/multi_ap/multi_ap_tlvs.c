/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-29
 *
 *
 */


#include "platform.h"

#include "multi_ap_tlvs.h"
#include "1905_tlvs.h"
#include "packet_tools.h"



////////////////////////////////////////////////////////////////////////////////
// Actual API functions
////////////////////////////////////////////////////////////////////////////////

INT8U *parse_multi_ap_TLV_from_packet(INT8U *packet_stream)
{
    if (NULL == packet_stream)
    {
        return NULL;
    }

    // The first byte of the stream is the "Type" field from the TLV structure.
    // Valid values for this byte are the following ones...
    //
    switch (*packet_stream)
    {

        default:
        {
            // Ignore
            //
            return NULL;
        }

    }

    // This code cannot be reached
    //
    return NULL;
}


INT8U *forge_multi_ap_TLV_from_structure(INT8U *memory_structure, INT16U *len)
{
    if (NULL == memory_structure)
    {
        return NULL;
    }

    // The first byte of any of the valid structures is always the "tlv_type"
    // field.
    //
    switch (*memory_structure)
    {

        default:
        {
            // Ignore
            //
            return NULL;
        }

    }

    // This code cannot be reached
    //
    return NULL;
}


void free_multi_ap_TLV_structure(INT8U *memory_structure)
{
    if (NULL == memory_structure)
    {
        return;
    }

    // The first byte of any of the valid structures is always the "tlv_type"
    // field.
    //
    switch (*memory_structure)
    {

        default:
        {
            // Ignore
            //
          return;
        }
    }

    // This code cannot be reached
    //
    return;
}


INT8U compare_multi_ap_TLV_structures(INT8U *memory_structure_1, INT8U *memory_structure_2)
{
    if (NULL == memory_structure_1 || NULL == memory_structure_2)
    {
        return 1;
    }

    // The first byte of any of the valid structures is always the "tlv_type"
    // field.
    //
    if (*memory_structure_1 != *memory_structure_2)
    {
        return 1;
    }

    switch (*memory_structure_1)
    {

        default:
        {
            // Unknown structure type
            //
            return 1;
        }
    }

    // This code cannot be reached
    //
    return 1;
}


void visit_multi_ap_TLV_structure(INT8U *memory_structure, void (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p), void (*write_function)(const char *fmt, ...), const char *prefix)
{
    // Buffer size to store a prefix string that will be used to show each
    // element of a structure on screen
    //
    #define MAX_PREFIX  100

    if (NULL == memory_structure)
    {
        return;
    }

    // The first byte of any of the valid structures is always the "tlv_type"
    // field.
    //
    switch (*memory_structure)
    {

        default:
        {
            // Ignore
            //
            break;
        }
    }

    return;
}


char *convert_multi_ap_TLV_type_to_string(INT8U tlv_type)
{
    switch (tlv_type)
    {

      default:
          return "Unknown";
    }
}
