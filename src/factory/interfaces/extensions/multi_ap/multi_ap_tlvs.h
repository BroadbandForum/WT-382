/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-23
 *
 *
 */

#ifndef _MULTI_AP_TLVS_H_
#define _MULTI_AP_TLVS_H_

#include "platform.h"

////////////////////////////////////////////////////////////////////////////////
// TLV types as detailed in Multi-AP specification "Section 17.2"
////////////////////////////////////////////////////////////////////////////////
#define TLV_TYPE_SUPPORTED_SERVICE                    (128) // 0x80
#define TLV_TYPE_SEARCHED_SERVICE                     (129) // 0x81
#define TLV_TYPE_AP_RADIO_IDENTIFIER                  (130) // 0x82
#define TLV_TYPE_AP_OPERATIONAL_BSS                   (131) // 0x83
#define TLV_TYPE_ASSOCIATED_CLIENTS                   (132) // 0x84
#define TLV_TYPE_AP_CAPABILITY                        (161) // 0xA1
#define TLV_TYPE_AP_RADIO_BASIC_CAPABILITIES          (133) // 0x85
#define TLV_TYPE_AP_HT_CAPABILITIES                   (134) // 0x86
#define TLV_TYPE_AP_VHT_CAPABILITIES                  (135) // 0x87
#define TLV_TYPE_AP_HE_CAPABILITIES                   (136) // 0x88
#define TLV_TYPE_STEERING_POLICY                      (137) // 0x89
#define TLV_TYPE_METRIC_REPORT_POLICY                 (138) // 0x8A
#define TLV_TYPE_CHANNEL_PREFERENCE                   (139) // 0x8B
#define TLV_TYPE_RADIO_OPERATION_RESTRICTION          (140) // 0x8C
#define TLV_TYPE_TRANSMIT_POWER_LIMIT                 (141) // 0x8D
#define TLV_TYPE_CHANNEL_SELECTION_RESPONSE           (142) // 0x8E
#define TLV_TYPE_OPERATING_CHANNEL_REPORT             (143) // 0x8F
#define TLV_TYPE_CLIENT_INFO                          (144) // 0x90
#define TLV_TYPE_CLIENT_CAPABLITY_REPORT              (145) // 0x91
#define TLV_TYPE_CLIENT_ASSOCIATION_EVENT             (146) // 0x92
#define TLV_TYPE_AP_METRIC_QUERY                      (147) // 0x93
#define TLV_TYPE_AP_METRICS                           (148) // 0x94
#define TLV_TYPE_STA_MAC_ADDRESS_TYPE                 (149) // 0x95
#define TLV_TYPE_ASSOCIATED_STA_LINK_METRICS          (150) // 0x96
#define TLV_TYPE_UNASSOCIATED_STA_LINK_METRICS_QUERY  (151) // 0x97
#define TLV_TYPE_UNASSOCIATED_STA_LINK_METRICS_REPORT (152) // 0x98
#define TLV_TYPE_BEACON_METRICS_QUERY                 (153) // 0x99
#define TLV_TYPE_BEACON_METRICS_RESPONSE              (154) // 0x9A
#define TLV_TYPE_STEERING_REQUEST                     (155) // 0x9B
#define TLV_TYPE_STEERING_BTM_REPORT                  (156) // 0x9C
#define TLV_TYPE_CLIENT_ASSOCIATION_CONTROL_REQUEST   (157) // 0x9D
#define TLV_TYPE_BACKHAUL_STEERING_REQUEST            (158) // 0x9E
#define TLV_TYPE_BACKHAUL_STEERING_RESPONSE           (159) // 0x9F
#define TLV_TYPE_HIGHER_LAYER_DATA                    (160) // 0xA0
#define TLV_TYPE_ASSOCIATED_STA_TRAFFIC_STATS         (162) // 0xA2

#ifdef _HOST_IS_BIG_ENDIAN_
struct HaulFlags_bits
{
    INT8U backhaulSTA   :   1;
    INT8U backhaulBSS   :   1;
    INT8U frontfaulBSS  :   1;
    INT8U teardown      :   1;
    INT8U bit3          :   1;
    INT8U bit2          :   1;
    INT8U bit1          :   1;
    INT8U bit0          :   1;
};
#else
struct HaulFlags_bits
{
    INT8U bit0          :   1;
    INT8U bit1          :   1;
    INT8U bit2          :   1;
    INT8U bit3          :   1;
    INT8U teardown      :   1;
    INT8U frontfaulBSS  :   1;
    INT8U backhaulBSS   :   1;
    INT8U backhaulSTA   :   1;
};
#endif

typedef union {
    INT8U _byte;
    struct HaulFlags_bits bits;
} HaulFlags;

struct multiAP_ExtensionSubelement
{
    INT8U subelementID;             // multi-AP extension subelement identifier 0x06
    INT8U subelementLength;         // Number of Bytes in the subelement value
                                    // (subelement payload). 0x01

    HaulFlags haulFlags;            // bit 7: backhaul STA  = 0x80
                                    // bit 6: backhaul BSS  = 0x40
                                    // bit 5: fronthaul BSS = 0x20
                                    // bit 4: tear down     = 0x10
                                    // bit 3-0 reserved
};
struct multiAP_IE
{
    INT8U elementID;                // always be 0xDD
                                    // IEEE 802.11 vendor specific information element
    INT8U *WIFI_ALLIANCE_OUI;       // pointer to WIFI_ALLIANCE_OUI
    INT8U WIFI_ALLIANCE_OUI_TYPE;   // Wi-Fi Alliance specific OUI Type
                                    // identifying the type of the Multi-AP IE.
};
////////////////////////////////////////////////////////////////////////////////
// SupportedService TLV associated structures (Multi-AP "Section 17.2.1")
////////////////////////////////////////////////////////////////////////////////
struct supportedServiceTLV
{
    INT8U  tlv_type;                  // Must always be set to
                                      // TLV_TYPE_SUPPORTED_SERVICE

    INT8U                         local_interfaces_nr;
    struct _l2InterfacesEntries  *local_interfaces;
                                      // List of interfaces with at least one
                                      // IPv4 assigned
};


static const INT8U WIFI_ALLIANCE_OUI[3] = {0x50, 0x6F, 0x9A};

////////////////////////////////////////////////////////////////////////////////
// Main API functions
////////////////////////////////////////////////////////////////////////////////

// This function receives a pointer to a stream of bytes representing a BBF TLV.
//
// It then returns a pointer to a structure whose fields have already been
// filled with the appropriate values extracted from the parsed stream.
//
// The actual type of the returned pointer structure depends on the value of
// the first byte pointed by "packet_stream" (ie. the "Type" field of the TLV):
//
//   TLV_TYPE_NON_1905_LINK_METRIC_QUERY        -->  struct linkMetricQueryTLV *
//   TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC  -->  struct transmitterLinkMetricTLV *
//   TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC     -->  struct receiverLinkMetricTLV *
//   TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE  -->  struct linkMetricResultCodeTLV *
//
//
// If an error was encountered while parsing the stream, a NULL pointer is
// returned instead.
// Otherwise, the returned structure is dynamically allocated, and once it is
// no longer needed, the user must call the "free_bbf_TLV_structure()"
//
INT8U *parse_multi_ap_TLV_from_packet(INT8U *packet_stream);


 // This is the opposite of "parse_multi_ap_TLV_from_packet()": it receives a
 // pointer to a TLV structure and then returns a pointer to a buffer which:
 //   - is a packet representation of the TLV
 //   - has a length equal to the value returned in the "len" output argument
 //
 // "memory_structure" must point to a structure of one of the types returned by
 // "parse_multi_ap_TLV_from_packet()"
 //
 // If there is a problem this function returns NULL, otherwise the returned
 // buffer must be later freed by the caller (it is a regular, non-nested
 // buffer, so you just need to call "free()").
 //
 // Note that the input structure is *not* freed. You still need to later call
 // "free_bbf_TLV_structure()"
 //
INT8U *forge_multi_ap_TLV_from_structure(INT8U *memory_structure, INT16U *len);



////////////////////////////////////////////////////////////////////////////////
// Utility API functions
////////////////////////////////////////////////////////////////////////////////

// This function receives a pointer to a TLV structure and then traverses it
// and all nested structures, calling "free()" on each one of them
//
// "memory_structure" must point to a structure of one of the types returned by
// "parse_multi_ap_TLV_from_packet()"
//
void free_multi_ap_TLV_structure(INT8U *memory_structure);


// This function returns '0' if the two given pointers represent TLV structures
// of the same type and they contain the same data
//
// "memory_structure_1" and "memory_structure_2" must point (each) to a
// structure of one of the types returned by "parse_multi_ap_TLV_from_packet()"
//
INT8U compare_multi_ap_TLV_structures(INT8U *memory_structure_1, INT8U *memory_structure_2);


// The next function is used to call function "callback()" on each element of
// the "memory_structure" structure
//
// "memory_structure" must point to a structure of one of the types returned by
// "parse_multi_ap_TLV_from_packet()"
//
// It takes four arguments:
//   - The structure whose elements are going to be visited
//   - A callback function that will be executed on each element with the
//     following arguments:
//      * A pointer to the "write()" function that will be used to dump text.
//        This is always the "write_function()" pointer provided as third
//        argument to the "visit_bbf_TLV_structure()" function.
//      * The size of the element to print (1, 2, 4, n bytes)
//      * A prefix string.
//        This is always the "prefix" value provided as fourth argument to the
//        "visit_bbf_TLV_structure()" function
//      * The name of the element (ex: "mac_address")
//      * A 'fmt' string which must be used to print the contents of the element
//      * A pointer to the element itself
//   - The "write()" function that will be used when the callback is executed
//   - A "prefix" string argument that will be used when the callback is
//     executed (it usually contains "context" information that the callback
//     function prints before anything else to make it easy to follow the
//     structure traversing order)
//
void visit_multi_ap_TLV_structure(INT8U *memory_structure, void (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p), void (*write_function)(const char *fmt, ...), const char *prefix);


// Use this function for debug purposes. It turns a TLV_TYPE_* variable into its
// string representation.
//
// Example: TLV_TYPE_AL_MAC_ADDRESS_TYPE --> "TLV_TYPE_AL_MAC_ADDRESS_TYPE"
//
// Return "Unknown" if the provided type does not exist.
//
char *convert_multi_ap_TLV_type_to_string(INT8U tlv_type);

#endif // END _MULTI_AP_TLVS_H_
