/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-23
 *
 *
 */

#ifndef _MULTI_AP_CMDUS_H_
#define _MULTI_AP_CMDUS_H_

#include "platform.h"
#include "1905_cmdus.h"

////////////////////////////////////////////////////////////////////////////////
// CMDU message type as detailed in Multi-AP specification "Section 17.1"
////////////////////////////////////////////////////////////////////////////////
#define CMDU_TYPE_1905_ACK                         0x8000
#define CMDU_TYPE_AP_CAPABILITY_QUERY              0x8001
#define CMDU_TYPE_AP_CAPABILITY_REPORT             0x8002

#define CMDU_TYPE_MULTI_AP_POLICY_CONFIG_REQUEST   0x8003

#define CMDU_TYPE_CHANNEL_PREFERENCE_QUERY         0x8004
#define CMDU_TYPE_CHANNEL_PREFERENCE_REPORT        0x8005
#define CMDU_TYPE_CHANNEL_SELECTION_REQUEST        0x8006
#define CMDU_TYPE_CHANNEL_SELECTION_RESPONSE       0x8007
#define CMDU_TYPE_OPERATING_CHANNEL_REPORT         0x8008

#define CMDU_TYPE_CLIENT_CAPABILITY_QUERY          0x8009
#define CMDU_TYPE_CLIENT_CAPABILITY_REPORT         0x800A
#define CMDU_TYPE_AP_METRICS_QUERY                 0x800B
#define CMDU_TYPE_AP_METRICS_RESPONSE              0x800C

#define CMDU_TYPE_ASSOCIATED_STA_LINK_METRICS_QUERY                0x800D
#define CMDU_TYPE_ASSOCIATED_STA_LINK_METRICS_RESPONSE             0x800E
#define CMDU_TYPE_UNASSOCIATED_STA_LINK_METRICS_QUERY              0x800F
#define CMDU_TYPE_UNASSOCIATED_STA_LINK_METRICS_RESPONSE           0x8010

#define CMDU_TYPE_BEACON_METRICS_QUERY                   0x8011
#define CMDU_TYPE_BEACON_METRICS_RESPONSE                0x8012
#define CMDU_TYPE_COMBINED_INFRASTRUCTURE_METRICS        0x8013

#define CMDU_TYPE_CLIENT_STEERING_REQUEST                0x8014
#define CMDU_TYPE_CLIENT_STEERING_BTM_REPORT             0x8015
#define CMDU_TYPE_CLIENT_ASSOCIATION_CONTROL_REQUEST     0x8016
#define CMDU_TYPE_STEERING_COMPLETED                     0x8017
#define CMDU_TYPE_HIGHER_LAYER_DATA                      0x8018

#define CMDU_TYPE_BACKHAUL_STEERING_REQUEST        0x8019
#define CMDU_TYPE_BACKHAUL_STEERING_RESPONSE       0x801A


#endif // END _MULTI_AP_CMDUS_H_
