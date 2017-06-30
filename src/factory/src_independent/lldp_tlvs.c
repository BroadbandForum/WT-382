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

#include "platform.h"

#include "lldp_tlvs.h"
#include "packet_tools.h"


////////////////////////////////////////////////////////////////////////////////
// Actual API functions
////////////////////////////////////////////////////////////////////////////////

INT8U *parse_lldp_TLV_from_packet(INT8U *packet_stream)
{
    if (NULL == packet_stream)
    {
        return NULL;
    }

    INT8U *p;
    INT8U byte1, byte2;
    INT8U type;
    INT16U len;

    p = packet_stream;

    _E1B(&p, &byte1);
    _E1B(&p, &byte2);

    // All LLDP TLVs start with the same two bytes:
    //
    //   |byte #1 |byte #2 |
    //   |--------|--------|
    //   |TTTTTTTL|LLLLLLLL|
    //   |--------|--------|
    //    <-----><-------->
    //    7 bits   9 bits
    //    (type)   (lenght)

    type = byte1 >> 1;
    len  = ((byte1 & 0x1) << 8) + byte2;

    switch (type)
    {
        case TLV_TYPE_END_OF_LLDPPDU:
        {
            // This parsing is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.1"

            struct endOfLldppduTLV  *ret;

            ret = (struct endOfLldppduTLV *)PLATFORM_MALLOC(sizeof(struct endOfLldppduTLV));

            // According to the standard, the length *must* be 0
            //
            if (0 != len)
            {
                // Malformed packet
                //
                PLATFORM_FREE(ret);
                return NULL;
            }

            ret->tlv_type = TLV_TYPE_END_OF_LLDPPDU;

            return (INT8U *)ret;
        }

        case TLV_TYPE_CHASSIS_ID:
        {
            // This parsing is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.2"

            struct chassisIdTLV  *ret;

            ret = (struct chassisIdTLV *)PLATFORM_MALLOC(sizeof(struct chassisIdTLV));

            // In the 1905 context we are only interested in processing those
            // TLVs whose length is "6+1" (ie. it contains a 6 bytes MAC address
            // in the "chassis_id" field) (see "IEEE Std 1905.1-2013 Section
            // 6.1")
            //
            if (6+1 != len)
            {
                // Not interested
                //
                PLATFORM_FREE(ret);
                return NULL;
            }

            ret->tlv_type = TLV_TYPE_CHASSIS_ID;

            _E1B(&p, &ret->chassis_id_subtype);

            // In the 1905 context we are only interested in processing those
            // TLVs whose chassis subtype is "4" (see "IEEE Std 1905.1-2013
            // Section 6.1")
            //
            if (CHASSIS_ID_TLV_SUBTYPE_MAC_ADDRESS != ret->chassis_id_subtype)
            {
                // Not interested
                //
                PLATFORM_FREE(ret);
                return NULL;
            }
            
            _EnB(&p, ret->chassis_id, 6);

            return (INT8U *)ret;
        }

        case TLV_TYPE_PORT_ID:
        {
            // This parsing is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.3"

            struct portIdTLV  *ret;

            ret = (struct portIdTLV *)PLATFORM_MALLOC(sizeof(struct portIdTLV));

            // In the 1905 context we are only interested in processing those
            // TLVs whose length is "6+1" (ie. it contains a 6 bytes MAC address
            // in the "port_id" field) (see "IEEE Std 1905.1-2013 Section
            // 6.1")
            //
            if (6+1 != len)
            {
                // Not interested
                //
                PLATFORM_FREE(ret);
                return NULL;
            }

            ret->tlv_type = TLV_TYPE_PORT_ID;

            _E1B(&p, &ret->port_id_subtype);

            // In the 1905 context we are only interested in processing those
            // TLVs whose chassis subtype is "3" (see "IEEE Std 1905.1-2013
            // Section 6.1")
            //
            if (PORT_ID_TLV_SUBTYPE_MAC_ADDRESS != ret->port_id_subtype)
            {
                // Not interested
                //
                PLATFORM_FREE(ret);
                return NULL;
            }
            
            _EnB(&p, ret->port_id, 6);

            return (INT8U *)ret;
        }

        case TLV_TYPE_TIME_TO_LIVE:
        {
            // This parsing is done according to the information detailed in
            //   "IEEE Std 802.1AB-2009 Section 8.5.4"

            struct timeToLiveTypeTLV  *ret;

            ret = (struct timeToLiveTypeTLV *)PLATFORM_MALLOC(sizeof(struct timeToLiveTypeTLV));

            // According to the standard, the length *must* be 2
            //
            if (2 != len)
            {
                // Not interested
                //
                PLATFORM_FREE(ret);
                return NULL;
            }

            ret->tlv_type = TLV_TYPE_TIME_TO_LIVE;

            _E2B(&p, &ret->ttl);

            return (INT8U *)ret;
        }

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


INT8U *forge_lldp_TLV_from_structure(INT8U *memory_structure, INT16U *len)
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
        case TLV_TYPE_END_OF_LLDPPDU:
        {
            // This forging is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.1"

            INT8U *ret, *p;
            struct endOfLldppduTLV *m;
            INT8U byte1, byte2;

            INT16U tlv_length;

            m = (struct endOfLldppduTLV *)memory_structure;

            tlv_length = 0;
            *len = 1 + 1 + tlv_length;

            p = ret = (INT8U *)PLATFORM_MALLOC(1 + 1 + tlv_length);

            byte1 = (m->tlv_type << 1) | ((tlv_length & 0x80) >> 7);
            byte2 = tlv_length & 0x7f;

            _I1B(&byte1,  &p);
            _I1B(&byte2,  &p);

            return ret;
        }

        case TLV_TYPE_CHASSIS_ID:
        {
            // This forging is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.2"

            INT8U *ret, *p;
            struct chassisIdTLV *m;
            INT8U byte1, byte2;

            INT16U tlv_length;

            m = (struct chassisIdTLV *)memory_structure;

            if (CHASSIS_ID_TLV_SUBTYPE_MAC_ADDRESS != m->chassis_id_subtype)
            {
                // 1905 *only* forges chassis of type "MAC address"
                //
                return NULL;
            }

            tlv_length = 1+6;  // subtype (1 byte) + AL MAC address (6 bytes)
            *len = 1 + 1 + tlv_length;

            p = ret = (INT8U *)PLATFORM_MALLOC(1 + 1 + tlv_length);

            byte1 = (m->tlv_type << 1) | ((tlv_length & 0x80) >> 7);
            byte2 = tlv_length & 0x7f;

            _I1B(&byte1,                 &p);
            _I1B(&byte2,                 &p);
            _I1B(&m->chassis_id_subtype, &p);
            _InB( m->chassis_id,         &p, 6);

            return ret;
        }

        case TLV_TYPE_PORT_ID:
        {
            // This forging is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.3"

            INT8U *ret, *p;
            struct portIdTLV *m;
            INT8U byte1, byte2;

            INT16U tlv_length;

            m = (struct portIdTLV *)memory_structure;

            if (PORT_ID_TLV_SUBTYPE_MAC_ADDRESS != m->port_id_subtype)
            {
                // 1905 *only* forges ports of type "MAC address"
                //
                return NULL;
            }

            tlv_length = 1+6;  // subtype (1 byte) + AL MAC address (6 bytes)
            *len = 1 + 1 + tlv_length;

            p = ret = (INT8U *)PLATFORM_MALLOC(1 + 1 + tlv_length);

            byte1 = (m->tlv_type << 1) | ((tlv_length & 0x80) >> 7);
            byte2 = tlv_length & 0x7f;

            _I1B(&byte1,              &p);
            _I1B(&byte2,              &p);
            _I1B(&m->port_id_subtype, &p);
            _InB( m->port_id,         &p, 6);

            return ret;
        }

        case TLV_TYPE_TIME_TO_LIVE:
        {
            // This forging is done according to the information detailed in
            // "IEEE Std 802.1AB-2009 Section 8.5.4"

            INT8U *ret, *p;
            struct timeToLiveTypeTLV *m;
            INT8U byte1, byte2;

            INT16U tlv_length;

            m = (struct timeToLiveTypeTLV *)memory_structure;

            if (TIME_TO_LIVE_TLV_1905_DEFAULT_VALUE != m->ttl)
            {
                // 1905 *only* forges TTLs with this default value
                //
                return NULL;
            }

            tlv_length = 2;
            *len = 1 + 1 + tlv_length;

            p = ret = (INT8U *)PLATFORM_MALLOC(1 + 1 + tlv_length);

            byte1 = (m->tlv_type << 1) | ((tlv_length & 0x80) >> 7);
            byte2 = tlv_length & 0x7f;

            _I1B(&byte1,  &p);
            _I1B(&byte2,  &p);
            _I2B(&m->ttl, &p);

            return ret;
        }

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


void free_lldp_TLV_structure(INT8U *memory_structure)
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
        case TLV_TYPE_END_OF_LLDPPDU:
        case TLV_TYPE_CHASSIS_ID:
        case TLV_TYPE_PORT_ID:
        case TLV_TYPE_TIME_TO_LIVE:
        {
            PLATFORM_FREE(memory_structure);

            return;
        }

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


INT8U compare_lldp_TLV_structures(INT8U *memory_structure_1, INT8U *memory_structure_2)
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
        case TLV_TYPE_END_OF_LLDPPDU:
        {
            // Nothing to compare (this TLV is always empty)
            //
            return 0;
        }

        case TLV_TYPE_CHASSIS_ID:
        {
            struct chassisIdTLV *p1, *p2;

            p1 = (struct chassisIdTLV *)memory_structure_1;
            p2 = (struct chassisIdTLV *)memory_structure_2;

            if (
                                  p1->chassis_id_subtype != p2->chassis_id_subtype ||
                 (PLATFORM_MEMCMP(p1->chassis_id,           p2->chassis_id, 6) !=0)
               )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        case TLV_TYPE_PORT_ID:
        {
            struct portIdTLV *p1, *p2;

            p1 = (struct portIdTLV *)memory_structure_1;
            p2 = (struct portIdTLV *)memory_structure_2;

            if (
                                  p1->port_id_subtype != p2->port_id_subtype ||
                 (PLATFORM_MEMCMP(p1->port_id,           p2->port_id, 6) !=0)
               )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        case TLV_TYPE_TIME_TO_LIVE:
        {
            struct timeToLiveTypeTLV *p1, *p2;

            p1 = (struct timeToLiveTypeTLV *)memory_structure_1;
            p2 = (struct timeToLiveTypeTLV *)memory_structure_2;

            if (
                 p1->ttl != p2->ttl
               )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

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


void visit_lldp_TLV_structure(INT8U *memory_structure, void (*callback)(void (*write_function)(const char *fmt, ...), const char *prefix, INT8U size, const char *name, const char *fmt, void *p), void (*write_function)(const char *fmt, ...), const char *prefix)
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
        case TLV_TYPE_END_OF_LLDPPDU:
        {
            // There is nothing to visit. This TLV is always empty
            //
            return;
        }

        case TLV_TYPE_CHASSIS_ID:
        {
            struct chassisIdTLV *p;

            p = (struct chassisIdTLV *)memory_structure;

            callback(write_function, prefix, sizeof(p->chassis_id_subtype), "chassis_id_subtype", "%d",     &p->chassis_id_subtype);
            callback(write_function, prefix, 6,                             "chassis_id",         "0x%02x",  p->chassis_id);

            return;
        }

        case TLV_TYPE_PORT_ID:
        {
            struct portIdTLV *p;

            p = (struct portIdTLV *)memory_structure;

            callback(write_function, prefix, sizeof(p->port_id_subtype), "port_id_subtype", "%d",     &p->port_id_subtype);
            callback(write_function, prefix, 6,                          "port_id",         "0x%02x",  p->port_id);

            return;
        }

        case TLV_TYPE_TIME_TO_LIVE:
        {
            struct timeToLiveTypeTLV *p;

            p = (struct timeToLiveTypeTLV *)memory_structure;

            callback(write_function, prefix, sizeof(p->ttl), "ttl", "%d", &p->ttl);

            return;
        }

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

char *convert_lldp_TLV_type_to_string(INT8U tlv_type)
{
    switch (tlv_type)
    {
        case TLV_TYPE_END_OF_LLDPPDU:
            return "TLV_TYPE_END_OF_LLDPPDU";
        case TLV_TYPE_CHASSIS_ID:
            return "TLV_TYPE_CHASSIS_ID";
        case TLV_TYPE_PORT_ID:
            return "TLV_TYPE_PORT_ID";
        case TLV_TYPE_TIME_TO_LIVE:
            return "TLV_TYPE_TIME_TO_LIVE";
        default:
            return "Unknown";
    }
}

