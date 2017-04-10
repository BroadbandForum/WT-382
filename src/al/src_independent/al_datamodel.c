/*
 *  Broadband Forum BUS (Broadband User Services) Work Area
 *  
 *  Copyright (c) 2017, Broadband Forum
 *  Copyright (c) 2016, Marvell International Ltd. and its affiliates
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
#include "utils.h"

#include "al_datamodel.h"
#include "al_utils.h"

#include "1905_tlvs.h"
#include "1905_cmdus.h"

#include "al_extension.h"

////////////////////////////////////////////////////////////////////////////////
// Private stuff
////////////////////////////////////////////////////////////////////////////////

struct _dataModel
{
    INT8U              map_whole_network_flag;

    INT8U              registrar_mac_address[6];

    INT8U              al_mac_address[6];
    INT8U              local_interfaces_nr;

    struct _localInterface 
    {
        char               *name;
        INT8U               mac_address[6];
 
        INT8U               neighbors_nr;

        struct _neighbor
        {
            INT8U               al_mac_address[6];
            INT8U               remote_interfaces_nr;

            struct _remoteInterface
            {
                INT8U               mac_address[6];
                INT32U              last_topology_discovery_ts;
                INT32U              last_bridge_discovery_ts;

            }                  *remote_interfaces;

        }                  *neighbors; 

    }                 *local_interfaces;

    INT8U              network_devices_nr;

    struct _networkDevice
    {
            INT32U                                      update_timestamp;

            struct deviceInformationTypeTLV            *info;
                      
            INT8U                                       bridges_nr;
            struct deviceBridgingCapabilityTLV        **bridges;

            INT8U                                       non1905_neighbors_nr;
            struct non1905NeighborDeviceListTLV       **non1905_neighbors;

            INT8U                                       x1905_neighbors_nr;
            struct neighborDeviceListTLV              **x1905_neighbors;

            INT8U                                       power_off_nr;
            struct powerOffInterfaceTLV               **power_off;

            INT8U                                       l2_neighbors_nr;
            struct l2NeighborDeviceTLV                **l2_neighbors;

            struct genericPhyDeviceInformationTypeTLV  *generic_phy;

            struct x1905ProfileVersionTLV              *profile;

            struct deviceIdentificationTypeTLV         *identification;

            struct controlUrlTypeTLV                   *control_url;

            struct ipv4TypeTLV                         *ipv4;

            struct ipv6TypeTLV                         *ipv6;

            INT8U                                       metrics_with_neighbors_nr;
            struct _metricsWithNeighbor
            {
                INT8U                                       neighbor_al_mac_address[6];

                INT32U                                      tx_metrics_timestamp;
                struct transmitterLinkMetricTLV            *tx_metrics;

                INT32U                                      rx_metrics_timestamp;
                struct receiverLinkMetricTLV               *rx_metrics;

            }                                          *metrics_with_neighbors;

            INT8U                                       extensions_nr;
            struct vendorSpecificTLV                  **extensions;

    }                 *network_devices;
                         // This list will always contain at least ONE entry,
                         // containing the info of the *local* device.
} data_model;


// Given a 'mac_address', return a pointer to the "struct _localInterface" that
// represents the local interface with that address.
// Returns NONE if such a local interface could not be found.
//
struct _localInterface *_macAddressToLocalInterfaceStruct(INT8U *mac_address)
{
    INT8U i;

    if (NULL != mac_address)
    {
        for (i=0; i<data_model.local_interfaces_nr; i++)
        {
            if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].mac_address, mac_address, 6))
            {
                return &data_model.local_interfaces[i];
            }
        }
    }

    // Not found!
    //
    return NULL;
}

// Given a 'name', return a pointer to the "struct _localInterface" that
// represents the local interface with that interface name.
// Returns NONE if such a local interface could not be found.
//
struct _localInterface *_nameToLocalInterfaceStruct(char *name)
{
    INT8U i;

    if (NULL != name)
    {
        for (i=0; i<data_model.local_interfaces_nr; i++)
        {
            if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].name, name, PLATFORM_STRLEN(data_model.local_interfaces[i].name)+1))
            {
                return &data_model.local_interfaces[i];
            }
        }
    }

    // Not found!
    //
    return NULL;
}

// Given an 'al_mac_address', return a pointer to the "struct _neighbor" that
// represents a 1905 neighbor with that 'al_mac_address' visible from the
// provided 'local_interface_name'.
// Returns NONE if such a neighbor could not be found.
//
struct _neighbor *_alMacAddressToNeighborStruct(char *local_interface_name, INT8U *al_mac_address)
{
    INT8U i;

    struct _localInterface *x;

    if (NULL == (x = _nameToLocalInterfaceStruct(local_interface_name)))
    {
        // Non existent interface
        //
        return NULL;
    }

    if (NULL != al_mac_address)
    {
        for (i=0; i<x->neighbors_nr; i++)
        {
            if (0 == PLATFORM_MEMCMP(x->neighbors[i].al_mac_address, al_mac_address, 6))
            {
                return &x->neighbors[i];
            }
        }
    }

    // Not found!
    //
    return NULL;
}

// Given a 'mac_address', return a pointer to the "struct _remoteInterface" that
// represents an interface in the provided 1905 neighbor (identified by its
// 'neighbor_al_mac_address' and visible in 'local_interface) that contains that
// address.
// Returns NONE if such a remote interface could not be found.
//
struct _remoteInterface *_macAddressToRemoteInterfaceStruct(char *local_interface_name, INT8U *neighbor_al_mac_address, INT8U *mac_address)
{
    INT8U i;

    struct _neighbor *x;

    if (NULL == (x = _alMacAddressToNeighborStruct(local_interface_name, neighbor_al_mac_address)))
    {
        // Non existent neighbor
        //
        return NULL;
    }

    if (NULL != mac_address)
    {
        for (i=0; i<x->remote_interfaces_nr; i++)
        {
            if (0 == PLATFORM_MEMCMP(x->remote_interfaces[i].mac_address, mac_address, 6))
            {
                return &x->remote_interfaces[i];
            }
        }
    }

    // Not found!
    //
    return NULL;
}

// When a new 1905 neighbor is discovered on a local interface, this function
// must be called to update the database.
// Returns '0' if there was a problem (out of memory, etc...), '2' if the
// neighbor had already been inserted, '1' if the new neighbor was succesfully
// inserted.
//
INT8U _insertNeighbor(char *local_interface_name, INT8U *al_mac_address)
{
    struct _localInterface *x;

    // First, make sure the interface exists
    //
    if (NULL == (x = _nameToLocalInterfaceStruct(local_interface_name)))
    {
        return 0;
    }

    // Next, make sure this neighbor does not already exist
    //
    if (NULL != _alMacAddressToNeighborStruct(local_interface_name, al_mac_address))
    {
        // The neighbor exists! We don't need to do anything special.
        //
        return 2;
    }

    if (0 == x->neighbors_nr)
    {
        x->neighbors = (struct _neighbor *)PLATFORM_MALLOC(sizeof (struct _neighbor));

    }
    else
    {
        x->neighbors = (struct _neighbor *)PLATFORM_REALLOC(x->neighbors, sizeof (struct _neighbor) * (x->neighbors_nr + 1));
    }

    PLATFORM_MEMCPY(x->neighbors[x->neighbors_nr].al_mac_address,        al_mac_address, 6);
                    x->neighbors[x->neighbors_nr].remote_interfaces_nr = 0;
                    x->neighbors[x->neighbors_nr].remote_interfaces    = NULL;

    x->neighbors_nr++;

    return 1;
}

// When a new interface of a 1905 neighbor is discovered, this function must be
// called to update the database.
// Returns '0' if there was a problem (out of memory, etc...), '2' if the
// neighbor interface had already been inserted, '1' if the new neighbor
// interface was successfully inserted.
//
INT8U _insertNeighborInterface(char *local_interface_name, INT8U *neighbor_al_mac_address, INT8U *mac_address)
{
    struct _neighbor        *x;

    // First, make sure the interface and neighbor exist
    //
    if (NULL == (x = _alMacAddressToNeighborStruct(local_interface_name, neighbor_al_mac_address)))
    {
        return 0;
    }

    // Next, make sure this neighbor interface does not already exist
    //
    if (NULL != _macAddressToRemoteInterfaceStruct(local_interface_name, neighbor_al_mac_address, mac_address))
    {
        // The neighbor exists! We don't need to do anything special.
        //
        return 2;
    }

    if (0 == x->remote_interfaces_nr)
    {
        x->remote_interfaces    = (struct _remoteInterface *)PLATFORM_MALLOC(sizeof (struct _remoteInterface));
    }
    else
    {
        x->remote_interfaces = (struct _remoteInterface *)PLATFORM_REALLOC(x->remote_interfaces, sizeof (struct _remoteInterface) * (x->remote_interfaces_nr + 1));
    }

    PLATFORM_MEMCPY(x->remote_interfaces[x->remote_interfaces_nr].mac_address,                 mac_address, 6);
                    x->remote_interfaces[x->remote_interfaces_nr].last_topology_discovery_ts = 0;
                    x->remote_interfaces[x->remote_interfaces_nr].last_bridge_discovery_ts   = 0;

    x->remote_interfaces_nr++;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// API functions (only available to the 1905 core itself, ie. files inside the
// 'lib1905' folder)
////////////////////////////////////////////////////////////////////////////////

void DMinit()
{
    data_model.map_whole_network_flag   = 0;

    data_model.registrar_mac_address[0] = 0x00;
    data_model.registrar_mac_address[1] = 0x00;
    data_model.registrar_mac_address[2] = 0x00;
    data_model.registrar_mac_address[3] = 0x00;
    data_model.registrar_mac_address[4] = 0x00;
    data_model.registrar_mac_address[5] = 0x00;

    data_model.al_mac_address[0]        = 0x00;
    data_model.al_mac_address[1]        = 0x00;
    data_model.al_mac_address[2]        = 0x00;
    data_model.al_mac_address[3]        = 0x00;
    data_model.al_mac_address[4]        = 0x00;
    data_model.al_mac_address[5]        = 0x00;

    data_model.local_interfaces_nr      = 0;
    data_model.local_interfaces         = NULL;

    // Regarding the "network_devices" list, we will init it with one element,
    // representing the local node
    //
    data_model.network_devices_nr       = 1;
    data_model.network_devices          = (struct _networkDevice *)PLATFORM_MALLOC(sizeof(struct _networkDevice));

    data_model.network_devices[0].update_timestamp          = PLATFORM_GET_TIMESTAMP();
    data_model.network_devices[0].info                      = NULL;
    data_model.network_devices[0].bridges_nr                = 0;
    data_model.network_devices[0].bridges                   = NULL;
    data_model.network_devices[0].non1905_neighbors_nr      = 0;
    data_model.network_devices[0].non1905_neighbors         = NULL;
    data_model.network_devices[0].x1905_neighbors_nr        = 0;
    data_model.network_devices[0].x1905_neighbors           = NULL;
    data_model.network_devices[0].power_off_nr              = 0;
    data_model.network_devices[0].power_off                 = NULL;
    data_model.network_devices[0].l2_neighbors_nr           = 0;
    data_model.network_devices[0].l2_neighbors              = NULL;
    data_model.network_devices[0].generic_phy               = NULL;
    data_model.network_devices[0].profile                   = NULL;
    data_model.network_devices[0].identification            = NULL;
    data_model.network_devices[0].control_url               = NULL;
    data_model.network_devices[0].ipv4                      = NULL;
    data_model.network_devices[0].ipv6                      = NULL;
    data_model.network_devices[0].metrics_with_neighbors_nr = 0;
    data_model.network_devices[0].metrics_with_neighbors    = NULL;
    data_model.network_devices[0].extensions                = NULL;
    data_model.network_devices[0].extensions_nr             = 0;

    return;
}

void DMalMacSet(INT8U *al_mac_address)
{
    PLATFORM_MEMCPY(data_model.al_mac_address, al_mac_address, 6);

    return;
}

INT8U *DMalMacGet()
{
    return data_model.al_mac_address;
}

void DMregistrarMacSet(INT8U *registrar_mac_address)
{
    PLATFORM_MEMCPY(data_model.registrar_mac_address, registrar_mac_address, 6);

    return;
}

INT8U *DMregistrarMacGet()
{
    return data_model.registrar_mac_address;
}

void DMmapWholeNetworkSet(INT8U map_whole_network_flag)
{
    data_model.map_whole_network_flag =  map_whole_network_flag;

    return;
}

INT8U DMmapWholeNetworkGet()
{
    return data_model.map_whole_network_flag;
}

INT8U DMinsertInterface(char *name, INT8U *mac_address)
{
    struct _localInterface *x;

    // First, make sure this interface does not already exist
    //
    if (NULL != (x = _nameToLocalInterfaceStruct(name)))
    {
        // The interface exists!
        //
        // Even if it already exists, if the provided 'mac_address' and the
        // already existing entry match, do not return an error.
        //
        if (0 == PLATFORM_MEMCMP(x->mac_address, mac_address, 6))
        {
            // Ok
            //
            return 1;
        }
        else
        {
            // Interface exists and its MAC address is different from the
            // provided one. Maybe the caller should first "remove" this
            // interface and then try to add the new one.
            //
            return 0;
        }
    }

    if (0 == data_model.local_interfaces_nr)
    {
        data_model.local_interfaces = (struct _localInterface *)PLATFORM_MALLOC(sizeof (struct _localInterface));

    }
    else
    {
        data_model.local_interfaces = (struct _localInterface *)PLATFORM_REALLOC(data_model.local_interfaces, sizeof (struct _localInterface) * (data_model.local_interfaces_nr + 1));
    }

                    data_model.local_interfaces[data_model.local_interfaces_nr].name         = PLATFORM_STRDUP(name);
    PLATFORM_MEMCPY(data_model.local_interfaces[data_model.local_interfaces_nr].mac_address,   mac_address, 6);
                    data_model.local_interfaces[data_model.local_interfaces_nr].neighbors    = NULL;
                    data_model.local_interfaces[data_model.local_interfaces_nr].neighbors_nr = 0;

    data_model.local_interfaces_nr++;

    return 1;
}


char *DMmacToInterfaceName(INT8U *mac_address)
{
    struct _localInterface *x;

    x = _macAddressToLocalInterfaceStruct(mac_address);

    if (NULL != x)
    {
        return x->name;
    }
    else
    {
        // Not found!
        //
        return NULL;
    }
}

INT8U *DMinterfaceNameToMac(char *interface_name)
{
    INT8U i;

    if (NULL != interface_name)
    {
        for (i=0; i<data_model.local_interfaces_nr; i++)
        {
            if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].name, interface_name, PLATFORM_STRLEN(data_model.local_interfaces[i].name)+1))
            {
                return data_model.local_interfaces[i].mac_address;
            }
        }
    }

    // Not found!
    //
    return NULL;
}


INT8U (*DMgetListOfInterfaceNeighbors(char *local_interface_name, INT8U *al_mac_addresses_nr))[6]
{
    INT8U i;
    INT8U (*ret)[6];

    struct _localInterface *x;

    if (NULL == (x = _nameToLocalInterfaceStruct(local_interface_name)))
    {
        // Non existent interface
        //
        *al_mac_addresses_nr = 0;
        return NULL;
    }

    if (0 == x->neighbors_nr)
    {
        *al_mac_addresses_nr = 0;
        return NULL;
    }

    ret = (INT8U (*)[6])PLATFORM_MALLOC(sizeof(INT8U[6]) * x->neighbors_nr);

    for (i=0; i<x->neighbors_nr; i++)
    {
        ret[i][0] = x->neighbors[i].al_mac_address[0];
        ret[i][1] = x->neighbors[i].al_mac_address[1];
        ret[i][2] = x->neighbors[i].al_mac_address[2];
        ret[i][3] = x->neighbors[i].al_mac_address[3];
        ret[i][4] = x->neighbors[i].al_mac_address[4];
        ret[i][5] = x->neighbors[i].al_mac_address[5];
    }

    *al_mac_addresses_nr = x->neighbors_nr;
    return ret;
}

INT8U (*DMgetListOfNeighbors(INT8U *al_mac_addresses_nr))[6]
{
    INT8U i, j, k;

    INT8U total;
    INT8U (*ret)[6];

    if (NULL == al_mac_addresses_nr)
    {
        return NULL;
    }

    total = 0;
    ret   = NULL;

    for (i=0; i<data_model.local_interfaces_nr; i++)
    {
        for (j=0; j<data_model.local_interfaces[i].neighbors_nr; j++)
        {
            // Check for duplicates
            //
            INT8U already_present;

            already_present = 0;
            for (k=0; k<total; k++)
            {
                if (0 == PLATFORM_MEMCMP(&ret[k], data_model.local_interfaces[i].neighbors[j].al_mac_address, 6))
                {
                    already_present = 1;
                    break;
                }
            }

            if (1 == already_present)
            {
                continue;
            }

            // If we get here, this is a new neighbor and we need to add it to
            // the list
            //
            if (NULL == ret)
            {
                ret = (INT8U (*)[6])PLATFORM_MALLOC(sizeof(INT8U[6]));
            }
            else
            {
                ret = (INT8U (*)[6])PLATFORM_REALLOC(ret, sizeof(INT8U[6])*(total + 1));
            }
            PLATFORM_MEMCPY(&ret[total], data_model.local_interfaces[i].neighbors[j].al_mac_address, 6);

            total++;
        }
    }

    *al_mac_addresses_nr = total;

    return ret;
}

INT8U (*DMgetListOfLinksWithNeighbor(INT8U *neighbor_al_mac_address, char ***interfaces, INT8U *links_nr))[6]
{
    INT8U i, j, k;
    INT8U total;

    INT8U (*ret)[6];
    char  **intfs;

    total = 0;
    ret   = NULL;
    intfs = NULL;

    for (i=0; i<data_model.local_interfaces_nr; i++)
    {
        for (j=0; j<data_model.local_interfaces[i].neighbors_nr; j++)
        {
            // Filter neighbor (we are just interested in
            // 'neighbor_al_mac_address')
            //
            if (0 != PLATFORM_MEMCMP(neighbor_al_mac_address, data_model.local_interfaces[i].neighbors[j].al_mac_address, 6))
            {
                continue;
            }

            for (k=0; k<data_model.local_interfaces[i].neighbors[j].remote_interfaces_nr; k++)
            {
                // This is a new link between the local AL and the remote AL.
                // Add it.
                //
                if (NULL == ret)
                {
                    ret   = (INT8U (*)[6])PLATFORM_MALLOC(sizeof(INT8U[6]));
                    intfs = (char **)PLATFORM_MALLOC(sizeof(char *));
                }
                else
                {
                    ret   = (INT8U (*)[6])PLATFORM_REALLOC(ret, sizeof(INT8U[6])*(total + 1));
                    intfs = (char **)PLATFORM_REALLOC(intfs, sizeof(char *)*(total + 1));
                }
                PLATFORM_MEMCPY(&ret[total], data_model.local_interfaces[i].neighbors[j].remote_interfaces[k].mac_address, 6);
                intfs[total] = data_model.local_interfaces[i].name;

                total++;
            }
        }
    }

    *links_nr   = total;
    *interfaces = intfs;

    return ret;
}

void DMfreeListOfLinksWithNeighbor(INT8U (*p)[6], char **interfaces, INT8U links_nr)
{
    if (0 == links_nr)
    {
        return;
    }

    if (NULL != interfaces)
    {
        PLATFORM_FREE(interfaces);
    }

    if (NULL != p)
    {
        PLATFORM_FREE(p);
    }

    return;
}


INT8U DMupdateDiscoveryTimeStamps(INT8U *receiving_interface_addr, INT8U *al_mac_address, INT8U *mac_address, INT8U timestamp_type, INT32U *ellapsed)
{
    char  *receiving_interface_name;

    struct _remoteInterface *x;

    INT32U aux1, aux2;
    INT8U  insert_result;
    INT8U  ret;

    ret = 2;

    if (NULL == receiving_interface_addr)
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Invalid 'receiving_interface_addr'\n");
        return 0;
    }

    if (NULL == (receiving_interface_name = DMmacToInterfaceName(receiving_interface_addr)))
    {
        PLATFORM_PRINTF_DEBUG_ERROR("The provided 'receiving_interface_addr' (%02x:%02x:%02x:%02x:%02x:%02x) does not match any local interface\n", receiving_interface_addr[0], receiving_interface_addr[1], receiving_interface_addr[2], receiving_interface_addr[3], receiving_interface_addr[4], receiving_interface_addr[5]);
        return 0;
    }

    if (
         0 == (insert_result = _insertNeighbor(receiving_interface_name, al_mac_address)) ||
         0 == _insertNeighborInterface(receiving_interface_name, al_mac_address, mac_address)
       )
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Could not create new entries in the database\n");
        return 0;
    }

    if (1 == insert_result)
    {
        // This is the first time we know of this neighbor (ie. the neighbor
        // has been inserted in the data model for the first time)
        //
        ret = 1;
    }

    x = _macAddressToRemoteInterfaceStruct(receiving_interface_name, al_mac_address, mac_address);

    PLATFORM_PRINTF_DEBUG_DETAIL("New discovery timestamp udpate:\n");
    PLATFORM_PRINTF_DEBUG_DETAIL("  - local_interface      : %s\n", receiving_interface_name);
    PLATFORM_PRINTF_DEBUG_DETAIL("  - 1905 neighbor AL MAC : %02x:%02x:%02x:%02x:%02x:%02x:\n", al_mac_address[0], al_mac_address[1], al_mac_address[2], al_mac_address[3], al_mac_address[4], al_mac_address[5]);
    PLATFORM_PRINTF_DEBUG_DETAIL("  - remote interface MAC : %02x:%02x:%02x:%02x:%02x:%02x:\n", mac_address[0],    mac_address[1],    mac_address[2],    mac_address[3],    mac_address[4],    mac_address[5]);

    aux1 = x->last_topology_discovery_ts;
    aux2 = x->last_bridge_discovery_ts;

    switch (timestamp_type)
    {
        case TIMESTAMP_TOPOLOGY_DISCOVERY:
        {
            INT32U aux;

            aux = PLATFORM_GET_TIMESTAMP();

            if (NULL != ellapsed)
            {
                if (2 == ret)
                {
                    *ellapsed = aux - x->last_topology_discovery_ts;
                }
                else
                {
                    *ellapsed = 0;
                }
            }

            x->last_topology_discovery_ts = aux;
            break;
        }
        case TIMESTAMP_BRIDGE_DISCOVERY:
        {
            INT32U aux;

            aux = PLATFORM_GET_TIMESTAMP();

            if (NULL != ellapsed)
            {
                if (2 == ret)
                {
                    *ellapsed = aux - x->last_bridge_discovery_ts;
                }
                else
                {    
                    *ellapsed = 0;
                }
            }
            x->last_bridge_discovery_ts = aux;
            break;
        }
        default:
        {
            PLATFORM_PRINTF_DEBUG_ERROR("Unknown 'timestamp_type' (%d)\n", timestamp_type);
            
            return 0;
        }
    }

    PLATFORM_PRINTF_DEBUG_DETAIL("  - topology disc TS     : %d --> %d\n",aux1, x->last_topology_discovery_ts);
    PLATFORM_PRINTF_DEBUG_DETAIL("  - bridge   disc TS     : %d --> %d\n",aux2, x->last_bridge_discovery_ts);

    return ret;
}

INT8U DMisLinkBridged(char *local_interface_name, INT8U *neighbor_al_mac_address, INT8U *neighbor_mac_address)
{
    struct _remoteInterface *x;

    INT32U aux;

    if (NULL == (x = _macAddressToRemoteInterfaceStruct(local_interface_name, neighbor_al_mac_address, neighbor_mac_address)))
    {
        // Non existent neighbor
        //
        return 2;
    }


    if (x->last_topology_discovery_ts > x->last_bridge_discovery_ts)
    {
        aux = x->last_topology_discovery_ts - x->last_bridge_discovery_ts;
    }
    else
    {
        aux = x->last_bridge_discovery_ts   - x->last_topology_discovery_ts;
    }

    if (aux < DISCOVERY_THRESHOLD_MS)
    {
        // Links is *not* bridged
        //
        return 0;
    }
    else
    {
        // Link is bridged
        //
        return 1;
    }
}

INT8U DMisNeighborBridged(char *local_interface_name, INT8U *neighbor_al_mac_address)
{
    struct _neighbor *x;

    INT8U i;

    if (NULL == (x = _alMacAddressToNeighborStruct(local_interface_name, neighbor_al_mac_address)))
    {
        // Non existent neighbor
        //
        return 2;
    }

    for (i=0; i<x->remote_interfaces_nr; i++)
    {
        if (1 == DMisLinkBridged(local_interface_name, neighbor_al_mac_address, x->remote_interfaces[i].mac_address))
        {
            // If at least one link is bridged, then this neighbor is
            // considered to be bridged.
            //
            return 1;
        }
    }

    // All links are not bridged, thus the neighbor is considered to be not
    // bridged also.
    //
    return 0;
}

INT8U DMisInterfaceBridged(char *local_interface_name)
{
    struct _localInterface *x;

    INT8U i;

    x = _nameToLocalInterfaceStruct(local_interface_name);
    if (NULL == x)
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Invalid local interface name\n");
        return 2;
    }

    for (i=0; i<x->neighbors_nr; i++)
    {
        if (1 == DMisNeighborBridged(local_interface_name, x->neighbors[i].al_mac_address))
        {
            // If at least one neighbor is bridged, then this interface is
            // considered to be bridged.
            //
            return 1;
        }
    }

    // All neighbors are not bridged, thus the interface is considered to be not
    // bridged also.
    //
    return 0;
}



INT8U *DMmacToAlMac(INT8U *mac_address)
{
    INT8U i, j, k;

    INT8U *al_mac;
    INT8U found;

    found  = 0;
    al_mac = (INT8U *)PLATFORM_MALLOC(sizeof(INT8U)*6);

    if (0 == PLATFORM_MEMCMP(data_model.al_mac_address, mac_address, 6))
    {
        return data_model.al_mac_address;
    }
    for (i=0; i<data_model.local_interfaces_nr; i++)
    {
        if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].mac_address, mac_address, 6))
        {
            found = 1;
            PLATFORM_MEMCPY(al_mac, data_model.al_mac_address, 6);
        }

        for (j=0; j<data_model.local_interfaces[i].neighbors_nr; j++)
        {
            if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].neighbors[j].al_mac_address, mac_address, 6))
            {
                found = 1;
                PLATFORM_MEMCPY(al_mac, data_model.local_interfaces[i].neighbors[j].al_mac_address, 6);
            }

            for (k=0; k<data_model.local_interfaces[i].neighbors[j].remote_interfaces_nr; k++)
            {
                if (0 == PLATFORM_MEMCMP(data_model.local_interfaces[i].neighbors[j].remote_interfaces[k].mac_address, mac_address, 6))
                {
                    found = 1;
                    PLATFORM_MEMCPY(al_mac, data_model.local_interfaces[i].neighbors[j].al_mac_address, 6);
                }
            }
        }
    }

    if (1 == found)
    {
        return al_mac;
    }
    else
    {
      // No matching MAC address was found
      //
      PLATFORM_FREE(al_mac);
      return NULL;
    }
}

INT8U DMupdateNetworkDeviceInfo(INT8U *al_mac_address,
                                INT8U in_update,  struct deviceInformationTypeTLV             *info, 
                                INT8U br_update,  struct deviceBridgingCapabilityTLV         **bridges,           INT8U bridges_nr,
                                INT8U no_update,  struct non1905NeighborDeviceListTLV        **non1905_neighbors, INT8U non1905_neighbors_nr,
                                INT8U x1_update,  struct neighborDeviceListTLV               **x1905_neighbors,   INT8U x1905_neighbors_nr,
                                INT8U po_update,  struct powerOffInterfaceTLV                **power_off,         INT8U power_off_nr,
                                INT8U l2_update,  struct l2NeighborDeviceTLV                 **l2_neighbors,      INT8U l2_neighbors_nr,
                                INT8U ge_update,  struct genericPhyDeviceInformationTypeTLV   *generic_phy,
                                INT8U pr_update,  struct x1905ProfileVersionTLV               *profile,
                                INT8U id_update,  struct deviceIdentificationTypeTLV          *identification,
                                INT8U co_update,  struct controlUrlTypeTLV                    *control_url,
                                INT8U v4_update,  struct ipv4TypeTLV                          *ipv4,
                                INT8U v6_update,  struct ipv6TypeTLV                          *ipv6)
{
    INT8U i,j;

    if (
         (NULL == al_mac_address)                                                     ||
         (br_update == 1 && (bridges_nr            > 0 && NULL == bridges)          ) ||
         (no_update == 1 && (non1905_neighbors_nr  > 0 && NULL == non1905_neighbors)) ||
         (x1_update == 1 && (x1905_neighbors_nr    > 0 && NULL == x1905_neighbors)  ) ||
         (po_update == 1 && (power_off_nr          > 0 && NULL == power_off)        ) ||
         (l2_update == 1 && (l2_neighbors_nr       > 0 && NULL == l2_neighbors)     )
       )
    {
        return 0;
    }

    // First, search for an existing entry with the same AL MAC address
    // Remember that the first entry holds a reference to the *local* node.
    //
    if (0 == PLATFORM_MEMCMP(DMalMacGet(), al_mac_address, 6))
    {
        i=0;
    }
    else
    {
        for (i=1; i<data_model.network_devices_nr; i++)
        {
            if (NULL != data_model.network_devices[i].info)
            {
                if (0 == PLATFORM_MEMCMP(data_model.network_devices[i].info->al_mac_address, al_mac_address, 6))
                {
                    break;
                }
            }
        }
    }

    if (i == data_model.network_devices_nr)
    {
        // A matching entry was *not* found. Create a new one, but only if this
        // new information contains the "info" TLV (otherwise don't do anything
        // and wait for the "info" TLV to be received in the future)
        //
        if (1 == in_update && NULL != info)
        {
            if (0 == data_model.network_devices_nr)
            {
                data_model.network_devices = (struct _networkDevice *)PLATFORM_MALLOC(sizeof(struct _networkDevice));
            }
            else
            {
                data_model.network_devices = (struct _networkDevice *)PLATFORM_REALLOC(data_model.network_devices, sizeof(struct _networkDevice)*(data_model.network_devices_nr+1));
            }

            data_model.network_devices[data_model.network_devices_nr].update_timestamp          = PLATFORM_GET_TIMESTAMP();
            data_model.network_devices[data_model.network_devices_nr].info                      = 1 == in_update ? info                 : NULL;
            data_model.network_devices[data_model.network_devices_nr].bridges_nr                = 1 == br_update ? bridges_nr           : 0;
            data_model.network_devices[data_model.network_devices_nr].bridges                   = 1 == br_update ? bridges              : NULL;
            data_model.network_devices[data_model.network_devices_nr].non1905_neighbors_nr      = 1 == no_update ? non1905_neighbors_nr : 0;
            data_model.network_devices[data_model.network_devices_nr].non1905_neighbors         = 1 == no_update ? non1905_neighbors    : NULL;
            data_model.network_devices[data_model.network_devices_nr].x1905_neighbors_nr        = 1 == x1_update ? x1905_neighbors_nr   : 0;
            data_model.network_devices[data_model.network_devices_nr].x1905_neighbors           = 1 == x1_update ? x1905_neighbors      : NULL;
            data_model.network_devices[data_model.network_devices_nr].power_off_nr              = 1 == po_update ? power_off_nr         : 0;
            data_model.network_devices[data_model.network_devices_nr].power_off                 = 1 == po_update ? power_off            : NULL;
            data_model.network_devices[data_model.network_devices_nr].l2_neighbors_nr           = 1 == l2_update ? l2_neighbors_nr      : 0;
            data_model.network_devices[data_model.network_devices_nr].l2_neighbors              = 1 == l2_update ? l2_neighbors         : NULL;
            data_model.network_devices[data_model.network_devices_nr].generic_phy               = 1 == ge_update ? generic_phy          : NULL;
            data_model.network_devices[data_model.network_devices_nr].profile                   = 1 == pr_update ? profile              : NULL;
            data_model.network_devices[data_model.network_devices_nr].identification            = 1 == id_update ? identification       : NULL;
            data_model.network_devices[data_model.network_devices_nr].control_url               = 1 == co_update ? control_url          : NULL;
            data_model.network_devices[data_model.network_devices_nr].ipv4                      = 1 == v4_update ? ipv4                 : NULL;
            data_model.network_devices[data_model.network_devices_nr].ipv6                      = 1 == v6_update ? ipv6                 : NULL;

            data_model.network_devices[data_model.network_devices_nr].metrics_with_neighbors_nr = 0;
            data_model.network_devices[data_model.network_devices_nr].metrics_with_neighbors    = NULL;

            data_model.network_devices[data_model.network_devices_nr].extensions                = NULL;
            data_model.network_devices[data_model.network_devices_nr].extensions_nr             = 0;

            data_model.network_devices_nr++;
        }
    }
    else
    {
        // A matching entry was found. Update it. But first, free the old TLV
        // structures (but only if a new value was provided!... otherwise retain
        // the old item)
        //
        data_model.network_devices[i].update_timestamp = PLATFORM_GET_TIMESTAMP();

        if (NULL != info)
        {
            if (NULL != data_model.network_devices[i].info)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].info);
            }
            data_model.network_devices[i].info = info;
        }

        if (1 == br_update)
        {
            for (j=0; j<data_model.network_devices[i].bridges_nr; j++)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].bridges[j]);
            }
            if (data_model.network_devices[i].bridges_nr > 0 && NULL != data_model.network_devices[i].bridges)
            {
                PLATFORM_FREE(data_model.network_devices[i].bridges);
            }
            data_model.network_devices[i].bridges_nr = bridges_nr;
            data_model.network_devices[i].bridges    = bridges;
        }

        if (1 == no_update)
        {
            for (j=0; j<data_model.network_devices[i].non1905_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].non1905_neighbors[j]);
            }
            if (data_model.network_devices[i].non1905_neighbors_nr > 0 && NULL != data_model.network_devices[i].non1905_neighbors)
            {
                PLATFORM_FREE(data_model.network_devices[i].non1905_neighbors);
            }
            data_model.network_devices[i].non1905_neighbors_nr = non1905_neighbors_nr;
            data_model.network_devices[i].non1905_neighbors    = non1905_neighbors;
        }

        if (1 == x1_update)
        {
            for (j=0; j<data_model.network_devices[i].x1905_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].x1905_neighbors[j]);
            }
            if (data_model.network_devices[i].x1905_neighbors_nr > 0 && NULL != data_model.network_devices[i].x1905_neighbors)
            {
                PLATFORM_FREE(data_model.network_devices[i].x1905_neighbors);
            }
            data_model.network_devices[i].x1905_neighbors_nr = x1905_neighbors_nr;
            data_model.network_devices[i].x1905_neighbors    = x1905_neighbors;
        }

        if (1 == po_update)
        {
            for (j=0; j<data_model.network_devices[i].power_off_nr; j++)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].power_off[j]);
            }
            if (data_model.network_devices[i].power_off_nr > 0 && NULL != data_model.network_devices[i].power_off)
            {
                PLATFORM_FREE(data_model.network_devices[i].power_off);
            }
            data_model.network_devices[i].power_off_nr = power_off_nr;
            data_model.network_devices[i].power_off    = power_off;
        }

        if (1 == l2_update)
        {
            for (j=0; j<data_model.network_devices[i].l2_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U *)data_model.network_devices[i].l2_neighbors[j]);
            }
            if (data_model.network_devices[i].l2_neighbors_nr > 0 && NULL != data_model.network_devices[i].l2_neighbors)
            {
                PLATFORM_FREE(data_model.network_devices[i].l2_neighbors);
            }
            data_model.network_devices[i].l2_neighbors_nr = l2_neighbors_nr;
            data_model.network_devices[i].l2_neighbors    = l2_neighbors;
        }

        if (1 == ge_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].generic_phy);
            data_model.network_devices[i].generic_phy = generic_phy;
        }

        if (1 == pr_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].profile);
            data_model.network_devices[i].profile = profile;
        }

        if (1 == id_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].identification);
            data_model.network_devices[i].identification = identification;
        }

        if (1 == co_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].control_url);
            data_model.network_devices[i].control_url = control_url;
        }

        if (1 == v4_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].ipv4);
            data_model.network_devices[i].ipv4 = ipv4;
        }

        if (1 == v6_update)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].ipv6);
            data_model.network_devices[i].ipv6 = ipv6;
        }

    }

    return 1;
}

INT8U DMnetworkDeviceInfoNeedsUpdate(INT8U *al_mac_address)
{
    INT8U i;

    // First, search for an existing entry with the same AL MAC address
    //
    for (i=0; i<data_model.network_devices_nr; i++)
    {
        if (NULL != data_model.network_devices[i].info)
        {
            if (0 == PLATFORM_MEMCMP(data_model.network_devices[i].info->al_mac_address, al_mac_address, 6))
            {
                break;
            }
        }
    }

    if (i == data_model.network_devices_nr)
    {
        // A matching entry was *not* found. Thus a refresh of the information
        // is needed.
        //
        return 1;
    }
    else
    {
        // A matching entry was found. Check its timestamp.
        //
        if (PLATFORM_GET_TIMESTAMP() - data_model.network_devices[i].update_timestamp > MAX_AGE * 1000)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

INT8U DMupdateNetworkDeviceMetrics(INT8U *metrics)
{
    INT8U *FROM_al_mac_address;  // Metrics are reported FROM this AL entity...
    INT8U *TO_al_mac_address;    // ... TO this other one.

    INT8U i, j;

    if (NULL == metrics)
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Invalid 'metrics' argument\n");
        return 0;
    }

    // Obtain the AL MAC of the devices involved in the metrics report (ie.
    // the "from" and the "to" AL MAC addresses).
    // This information is contained inside the 'metrics' structure itself.
    //
    if (TLV_TYPE_TRANSMITTER_LINK_METRIC == *metrics)
    {
        struct transmitterLinkMetricTLV *p;

        p = (struct transmitterLinkMetricTLV *)metrics;

        FROM_al_mac_address = p->local_al_address;
        TO_al_mac_address   = p->neighbor_al_address;

    }
    else if (TLV_TYPE_RECEIVER_LINK_METRIC == *metrics)
    {
        struct receiverLinkMetricTLV *p;

        p = (struct receiverLinkMetricTLV *)metrics;

        FROM_al_mac_address = p->local_al_address;
        TO_al_mac_address   = p->neighbor_al_address;
    }
    else
    {
        PLATFORM_PRINTF_DEBUG_DETAIL("Invalid 'metrics' argument. Type = %d\n", *metrics);
        return 0;
    }

    // Next, search for an existing entry with the same AL MAC address
    //
    for (i=0; i<data_model.network_devices_nr; i++)
    {
        if (NULL == data_model.network_devices[i].info)
        {
            // We haven't received general info about this device yet.
            // This can happen, for example, when only metrics have been
            // received so far.
            //
            continue;
        }
        if (0 == PLATFORM_MEMCMP(data_model.network_devices[i].info->al_mac_address, FROM_al_mac_address, 6))
        {
            break;
        }
    }

    if (i == data_model.network_devices_nr)
    {
        // A matching entry was *not* found.
        //
        // At this point, even if we could just create a new device entry with
        // everything set to zero (except for the metrics that we have just
        // received), it is probably wiser to simply discard the data.
        //
        // In other words, we should not accept metrics data until the "general
        // info" for this node has been processed.
        //
        PLATFORM_PRINTF_DEBUG_DETAIL("Metrics received from an unknown 1905 node (%02x:%02x:%02x:%02x:%02x:%02x). Ignoring data...\n", FROM_al_mac_address[0], FROM_al_mac_address[1], FROM_al_mac_address[2], FROM_al_mac_address[3], FROM_al_mac_address[4], FROM_al_mac_address[5]);
        return 0;
    }

    // Now that we have found the corresponding neighbor entry (or created a
    // new one) search for a sub-entry that matches the AL MAC of the node the
    // metrics are being reported against.
    //
    for (j=0; j<data_model.network_devices[i].metrics_with_neighbors_nr; j++)
    {
        if (0 == PLATFORM_MEMCMP(data_model.network_devices[i].metrics_with_neighbors[j].neighbor_al_mac_address, TO_al_mac_address, 6))
        {
            break;
        }
    }
    
    if (j == data_model.network_devices[i].metrics_with_neighbors_nr)
    {
        // A matching entry was *not* found. Create a new one
        //
        if (0 == data_model.network_devices[i].metrics_with_neighbors_nr)
        {
            data_model.network_devices[i].metrics_with_neighbors = (struct _metricsWithNeighbor *)PLATFORM_MALLOC(sizeof(struct _metricsWithNeighbor));
        }
        else
        {
            data_model.network_devices[i].metrics_with_neighbors = (struct _metricsWithNeighbor *)PLATFORM_REALLOC(data_model.network_devices[i].metrics_with_neighbors, sizeof(struct _metricsWithNeighbor)*(data_model.network_devices[i].metrics_with_neighbors_nr+1));
        }

        PLATFORM_MEMCPY(data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].neighbor_al_mac_address, TO_al_mac_address, 6);

        if (TLV_TYPE_TRANSMITTER_LINK_METRIC == *metrics)
        {
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].tx_metrics_timestamp = PLATFORM_GET_TIMESTAMP();
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].tx_metrics           = (struct transmitterLinkMetricTLV*)metrics;

            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].rx_metrics_timestamp = 0;
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].rx_metrics           = NULL;
        }
        else 
        {
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].tx_metrics_timestamp = 0;
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].tx_metrics           = NULL;

            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].rx_metrics_timestamp = PLATFORM_GET_TIMESTAMP();
            data_model.network_devices[i].metrics_with_neighbors[data_model.network_devices[i].metrics_with_neighbors_nr].rx_metrics           = (struct receiverLinkMetricTLV*)metrics;
        }

        data_model.network_devices[i].metrics_with_neighbors_nr++;
    }
    else
    {
        // A matching entry was found. Update it. But first, free the old TLV
        // structures.
        //
        if (TLV_TYPE_TRANSMITTER_LINK_METRIC == *metrics)
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics);

            data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics_timestamp = PLATFORM_GET_TIMESTAMP();
            data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics           = (struct transmitterLinkMetricTLV*)metrics;
        }
        else
        {
            free_1905_TLV_structure((INT8U *)data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics);

            data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics_timestamp = PLATFORM_GET_TIMESTAMP();
            data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics           = (struct receiverLinkMetricTLV*)metrics;
        }
    }

    return 1;
}

void DMdumpNetworkDevices(void (*write_function)(const char *fmt, ...))
{
    // Buffer size to store a prefix string that will be used to show each
    // element of a structure on screen
    //
    #define MAX_PREFIX  100

    INT8U  i, j;

    write_function("\n");

    write_function("  device_nr: %d\n", data_model.network_devices_nr);

    for (i=0; i<data_model.network_devices_nr; i++)
    {
        char new_prefix[MAX_PREFIX];

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%supdate timestamp: %d\n", new_prefix, data_model.network_devices[i].update_timestamp);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->general_info->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U* )data_model.network_devices[i].info, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->bridging_capabilities_nr: %d", i, data_model.network_devices[i].bridges_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].bridges_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->bridging_capabilities[%d]->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].bridges[j], print_callback, write_function, new_prefix);
        }

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->non_1905_neighbors_nr: %d", i, data_model.network_devices[i].non1905_neighbors_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].non1905_neighbors_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->non_1905_neighbors[%d]->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].non1905_neighbors[j], print_callback, write_function, new_prefix);
        }

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->x1905_neighbors_nr: %d", i, data_model.network_devices[i].x1905_neighbors_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].x1905_neighbors_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->x1905_neighbors[%d]->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].x1905_neighbors[j], print_callback, write_function, new_prefix);
        }

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->power_off_interfaces_nr: %d", i, data_model.network_devices[i].power_off_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].power_off_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->power_off_interfaces[%d]->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].power_off[j], print_callback, write_function, new_prefix);
        }

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->l2_neighbors_nr: %d", i, data_model.network_devices[i].l2_neighbors_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].l2_neighbors_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->l2_neighbors[%d]->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].l2_neighbors[j], print_callback, write_function, new_prefix);
        }

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->generic_phys->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U* )data_model.network_devices[i].generic_phy, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->profile->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U* )data_model.network_devices[i].profile, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->identification->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U* )data_model.network_devices[i].identification, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->control_url->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].control_url, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->ipv4->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].ipv4, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->ipv6->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].ipv6, print_callback, write_function, new_prefix);

        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->metrics_nr: %d", i, data_model.network_devices[i].metrics_with_neighbors_nr);
        new_prefix[MAX_PREFIX-1] = 0x0;
        write_function("%s\n", new_prefix);
        for (j=0; j<data_model.network_devices[i].metrics_with_neighbors_nr; j++)
        {
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->metrics[%d]->tx->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            if (NULL != data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics)
            {
                write_function("%slast_updated: %d\n", new_prefix, data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics_timestamp);
                visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].metrics_with_neighbors[j].tx_metrics, print_callback, write_function, new_prefix);
            }
            PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->metrics[%d]->rx->", i, j);
            new_prefix[MAX_PREFIX-1] = 0x0;
            if (NULL != data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics)
            {
                write_function("%slast updated: %d\n", new_prefix, data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics_timestamp);
                visit_1905_TLV_structure((INT8U *)data_model.network_devices[i].metrics_with_neighbors[j].rx_metrics, print_callback, write_function, new_prefix);
            }
        }

        // Non-standard report section.
        // Allow registered third-party developers to extend the neighbor info
        // (ex. BBF adds non-1905 link metrics)
        //
        PLATFORM_SNPRINTF(new_prefix, MAX_PREFIX-1, "  device[%d]->", i);
        new_prefix[MAX_PREFIX-1] = 0x0;
        dumpExtendedInfo((INT8U **)data_model.network_devices[i].extensions, data_model.network_devices[i].extensions_nr, print_callback, write_function, new_prefix);
    }

    return;
}

INT8U DMrunGarbageCollector(void)
{
    INT8U i, j, k;
    INT8U removed_entries;
    INT8U original_devices_nr;

    removed_entries     = 0;

    // Visit all existing devices, searching for those with a timestamp older
    // than GC_MAX_AGE
    //
    // Note that we skip element "0", which is always the local device. We don't
    // care when it was last updated as it is always updated "on demand", just
    // before someone requests its data (right now the only place where this
    // happens is when using an ALME custom command)
    //
    original_devices_nr = data_model.network_devices_nr;
    for (i=1; i<data_model.network_devices_nr; i++)
    {
        INT8U *p = NULL;

        if (
             (PLATFORM_GET_TIMESTAMP() - data_model.network_devices[i].update_timestamp > (GC_MAX_AGE*1000)) ||
             (NULL != data_model.network_devices[i].info && NULL == (p = DMmacToAlMac(data_model.network_devices[i].info->al_mac_address)))
           )
        {
            // Entry too old or with a MAC address no longer registered in the
            // "topology discovery" database. Remove it.
            //
            INT8U  al_mac_address[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            struct _networkDevice *x;

            removed_entries++;

            x = &data_model.network_devices[i];

            // First, free all child structures
            //
            if (NULL != x->info)
            {
                // Save the MAC of the node that is going to be removed for
                // later use
                //
                PLATFORM_MEMCPY(al_mac_address, x->info->al_mac_address, 6);

                PLATFORM_PRINTF_DEBUG_DETAIL("Removing old device entry (%02x:%02x:%02x:%02x:%02x:%02x)\n", x->info->al_mac_address[0], x->info->al_mac_address[1], x->info->al_mac_address[2], x->info->al_mac_address[3], x->info->al_mac_address[4], x->info->al_mac_address[5]);
                free_1905_TLV_structure((INT8U*)x->info);
                x->info = NULL;
            }
            else
            {
                PLATFORM_PRINTF_DEBUG_WARNING("Removing old device entry (Unknown AL MAC)\n");
            }

            for (j=0; j<x->bridges_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->bridges[j]);
            }
            if (0 != x->bridges_nr && NULL != x->bridges)
            {
                PLATFORM_FREE(x->bridges);
                x->bridges_nr = 0;
                x->bridges    = NULL;
            }

            for (j=0; j<x->non1905_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->non1905_neighbors[j]);
            }
            if (0 != x->non1905_neighbors_nr && NULL != x->non1905_neighbors)
            {
                PLATFORM_FREE(x->non1905_neighbors);
                x->non1905_neighbors_nr = 0;
                x->non1905_neighbors    = NULL;
            }

            for (j=0; j<x->x1905_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->x1905_neighbors[j]);
            }
            if (0 != x->x1905_neighbors_nr && NULL != x->x1905_neighbors)
            {
                PLATFORM_FREE(x->x1905_neighbors);
                x->x1905_neighbors_nr = 0;
                x->x1905_neighbors    = NULL;
            }

            for (j=0; j<x->power_off_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->power_off[j]);
            }
            if (0 != x->power_off_nr && NULL != x->power_off)
            {
                PLATFORM_FREE(x->power_off);
                x->power_off_nr = 0;
                x->power_off    = NULL;
            }

            for (j=0; j<x->l2_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->l2_neighbors[j]);
            }
            if (0 != x->l2_neighbors_nr && NULL != x->l2_neighbors)
            {
                PLATFORM_FREE(x->l2_neighbors);
                x->l2_neighbors_nr = 0;
                x->l2_neighbors    = NULL;
            }

            if (NULL != x->generic_phy)
            {
                free_1905_TLV_structure((INT8U*)x->generic_phy);
                x->generic_phy = NULL;
            }

            if (NULL != x->profile)
            {
                free_1905_TLV_structure((INT8U*)x->profile);
                x->profile = NULL;
            }

            if (NULL != x->identification)
            {
                free_1905_TLV_structure((INT8U*)x->identification);
                x->identification = NULL;
            }

            if (NULL != x->control_url)
            {
                free_1905_TLV_structure((INT8U*)x->control_url);
                x->control_url = NULL;
            }

            if (NULL != x->ipv4)
            {
                free_1905_TLV_structure((INT8U*)x->ipv4);
                x->ipv4 = NULL;
            }

            if (NULL != x->ipv6)
            {
                free_1905_TLV_structure((INT8U*)x->ipv6);
                x->ipv6 = NULL;
            }

            for (j=0; j<x->metrics_with_neighbors_nr; j++)
            {
                free_1905_TLV_structure((INT8U*)x->metrics_with_neighbors[j].tx_metrics);
                free_1905_TLV_structure((INT8U*)x->metrics_with_neighbors[j].rx_metrics);
            }
            if (0 != x->metrics_with_neighbors_nr && NULL != x->metrics_with_neighbors)
            {
                PLATFORM_FREE(x->metrics_with_neighbors);
                x->metrics_with_neighbors = NULL;
            }

            // Next, remove the _networkDevice entry
            //
            if (i == (data_model.network_devices_nr-1))
            {
                // Last element. It will automatically be removed below (keep
                // reading)
            }
            else
            {
                // Place the last element here (we don't care about preserving
                // order)
                //
                data_model.network_devices[i] = data_model.network_devices[data_model.network_devices_nr-1];
                i--;
            }
            data_model.network_devices_nr--;

            // Next, Remove all references to this node from other node's
            // metrics information entries
            // 
            for (j=0; j<data_model.network_devices_nr; j++)
            {
                INT8U original_neighbors_nr;

                original_neighbors_nr = data_model.network_devices[j].metrics_with_neighbors_nr;

                for (k=0; k<data_model.network_devices[j].metrics_with_neighbors_nr; k++)
                {
                    if (0 == PLATFORM_MEMCMP(al_mac_address, data_model.network_devices[j].metrics_with_neighbors[k].neighbor_al_mac_address, 6))
                    {
                        free_1905_TLV_structure((INT8U*)data_model.network_devices[j].metrics_with_neighbors[k].tx_metrics);
                        free_1905_TLV_structure((INT8U*)data_model.network_devices[j].metrics_with_neighbors[k].rx_metrics);

                        // Place last element here (we don't care about
                        // preserving order)
                        //
                        if (k == (data_model.network_devices[j].metrics_with_neighbors_nr-1))
                        {
                            // Last element. It will automatically be removed
                            // below (keep reading)
                        }
                        else
                        {
                            data_model.network_devices[j].metrics_with_neighbors[k] = data_model.network_devices[j].metrics_with_neighbors[data_model.network_devices[j].metrics_with_neighbors_nr-1];
                            k--;
                        }
                        data_model.network_devices[j].metrics_with_neighbors_nr--;
                    }
                }

                if (original_neighbors_nr != data_model.network_devices[j].metrics_with_neighbors_nr)
                {
                    if (0 == data_model.network_devices[j].metrics_with_neighbors_nr)
                    {
                        PLATFORM_FREE(data_model.network_devices[j].metrics_with_neighbors);
                    }
                    else
                    {
                        data_model.network_devices[j].metrics_with_neighbors = (struct _metricsWithNeighbor *)PLATFORM_REALLOC(data_model.network_devices[j].metrics_with_neighbors, sizeof(struct _metricsWithNeighbor)*(data_model.network_devices[j].metrics_with_neighbors_nr));
                    }
                }
            }

            // And also from the local interfaces database
            // 
            DMremoveALNeighborFromInterface(al_mac_address, "all");
        }

        if (NULL != p)
        {
            PLATFORM_FREE(p);
        }
    }

    // If at least one element was removed, we need to realloc
    //
    if (original_devices_nr != data_model.network_devices_nr)
    {
        if (0 == data_model.network_devices_nr)
        {
            PLATFORM_FREE(data_model.network_devices);
        }
        else
        {
            data_model.network_devices = (struct _networkDevice *)PLATFORM_REALLOC(data_model.network_devices, sizeof(struct _networkDevice)*(data_model.network_devices_nr));
        }
    }

    return removed_entries;
}

void DMremoveALNeighborFromInterface(INT8U *al_mac_address, char *interface_name)
{
    INT8U i, j;

    for (i=0; i<data_model.local_interfaces_nr; i++)
    {
        INT8U original_neighbors_nr;

        if (
             (0 != PLATFORM_MEMCMP(data_model.local_interfaces[i].name, interface_name, PLATFORM_STRLEN(data_model.local_interfaces[i].name)+1)) &&
             (0 != PLATFORM_MEMCMP(interface_name,                      "all",          PLATFORM_STRLEN(interface_name)+1))
           )
        {
            // Ignore this interface
            //
            continue;
        }

        original_neighbors_nr = data_model.local_interfaces[i].neighbors_nr;

        for (j=0; j<data_model.local_interfaces[i].neighbors_nr; j++)
        {
            if (0 == PLATFORM_MEMCMP(al_mac_address, data_model.local_interfaces[i].neighbors[j].al_mac_address, 6))
            {
                if (data_model.local_interfaces[i].neighbors[j].remote_interfaces_nr > 0 && NULL != data_model.local_interfaces[i].neighbors[j].remote_interfaces)
                {
                    PLATFORM_FREE(data_model.local_interfaces[i].neighbors[j].remote_interfaces);

                    data_model.local_interfaces[i].neighbors[j].remote_interfaces    = NULL;
                    data_model.local_interfaces[i].neighbors[j].remote_interfaces_nr = 0;
                }

                // Place last element here (we don't care about preserving
                // order)
                //
                if (j == (data_model.local_interfaces[i].neighbors_nr-1))
                {
                    // Last element. It will automatically be removed
                    // below (keep reading)
                }
                else
                {
                    data_model.local_interfaces[i].neighbors[j] = data_model.local_interfaces[i].neighbors[data_model.local_interfaces[i].neighbors_nr-1];
                    j--;
                }
                data_model.local_interfaces[i].neighbors_nr--;
            }
        }

        if (original_neighbors_nr != data_model.local_interfaces[i].neighbors_nr)
        {
            if (0 == data_model.local_interfaces[i].neighbors_nr)
            {
                PLATFORM_FREE(data_model.local_interfaces[i].neighbors);
            }
            else
            {
                data_model.local_interfaces[i].neighbors = (struct _neighbor *)PLATFORM_REALLOC(data_model.local_interfaces[i].neighbors, sizeof(struct _neighbor)*(data_model.local_interfaces[i].neighbors_nr));
            }
        }
    }
}


struct vendorSpecificTLV ***DMextensionsGet(INT8U *al_mac_address, INT8U **nr)
{
    INT8U                         i;
    struct vendorSpecificTLV   ***extensions;

    // Find device
    if ((NULL == al_mac_address) || (NULL == nr))
    {
        PLATFORM_PRINTF_DEBUG_ERROR("Invalid 'DMextensionsGet' argument\n");
        return NULL;
    }

    // Search for an existing entry with the same AL MAC address
    //
    for (i=0; i<data_model.network_devices_nr; i++)
    {
        if (NULL == data_model.network_devices[i].info)
        {
            // We haven't received general info about this device yet.
            //
            continue;
        }
        if (0 == PLATFORM_MEMCMP(data_model.network_devices[i].info->al_mac_address, al_mac_address, 6))
        {
            break;
        }
    }

    if (i == data_model.network_devices_nr)
    {
        // A matching entry was *not* found.
        //
        PLATFORM_PRINTF_DEBUG_DETAIL("Extension received from an unknown 1905 node (%02x:%02x:%02x:%02x:%02x:%02x). Ignoring data...\n", al_mac_address[0], al_mac_address[1], al_mac_address[2], al_mac_address[3], al_mac_address[4], al_mac_address[5]);
        extensions = NULL;
    }
    else
    {
        // Point to the datamodel extensions section
        //
        extensions = &data_model.network_devices[i].extensions;
        *nr        = &data_model.network_devices[i].extensions_nr;
    }

    return extensions;
}
