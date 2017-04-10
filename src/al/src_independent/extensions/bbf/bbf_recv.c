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

#include "al_datamodel.h"
#include "al_recv.h"
#include "al_extension.h" // VendorSpecificTLVDuplicate
#include "1905_tlvs.h"
#include "bbf_tlvs.h"
#include "bbf_send.h"     // CBKUpdateBBFExtendedInfo

extern INT8U   bbf_query; // from bbf_send.c


////////////////////////////////////////////////////////////////////////////////
// CMDU extension callbacks
////////////////////////////////////////////////////////////////////////////////

INT8U CBKprocess1905BBFExtensions(struct CMDU *memory_structure)
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
        case CMDU_TYPE_LINK_METRIC_QUERY:
        {
            INT8U                      *tlv;

            i = 0;
            while (NULL != (p = memory_structure->list_of_TLVs[i]))
            {
                // Protocol extensions are always embedded inside a Vendor
                // Specific TLV. Ignore other TLVs
                //
                if (*p == TLV_TYPE_VENDOR_SPECIFIC)
                {
                    vs_tlv = (struct vendorSpecificTLV *)p;

                    // Process only embedded BBF TLVs
                    //
                    if (0 == PLATFORM_MEMCMP(vs_tlv->vendorOUI, BBF_OUI, 3))
                    {
                        tlv = parse_bbf_TLV_from_packet(vs_tlv->m);

                        if (NULL == tlv)
                        {
                            PLATFORM_PRINTF_DEBUG_ERROR("Malformed non-1905 Link Metric Query Tlv");
                        }
                        else
                        {
                            if (*tlv == BBF_TLV_TYPE_NON_1905_LINK_METRIC_QUERY)
                            {
                                // BBF query TLV has been received.
                                // CMDU response must contain BBF metric TLVs
                                //
                                bbf_query = 1;
                            }
                            else
                            {
                                PLATFORM_PRINTF_DEBUG_ERROR("Unexpected BBF protocol extension TLV");
                            }

                            // Release BBF TLV
                            //
                            free_bbf_TLV_structure(tlv);
                        }
                        // Only one BBF TLV is expected in this CMDU
                        //
                        break;
                    }
                }
                i++;
            }

            break;
        }

        case CMDU_TYPE_LINK_METRIC_RESPONSE:
        {
          struct transmitterLinkMetricTLV *transmitter_tlv = NULL;
          struct receiverLinkMetricTLV    *receiver_tlv = NULL;
          struct vendorSpecificTLV       **extensions;
          INT8U                            extensions_nr;
          INT8U                           *bbf_tlv;
          INT8U                            std_FROM_al_mac_address[6];
          INT8U                            no_std_FROM_al_mac_address[6];

          i = 0;
          extensions_nr = 0;
          extensions = NULL;
          while (NULL != (p = memory_structure->list_of_TLVs[i]))
          {
              // Protocol extensions are always embedded inside a Vendor
              // Specific TLV. Ignore other TLVs
              //
              if (*p == TLV_TYPE_VENDOR_SPECIFIC)
              {
                  vs_tlv = (struct vendorSpecificTLV *)p;

                  // Process only embedded BBF TLVs
                  //
                  if (0 == PLATFORM_MEMCMP(vs_tlv->vendorOUI, BBF_OUI, 3))
                  {
                      bbf_tlv = parse_bbf_TLV_from_packet(vs_tlv->m);

                      if (NULL == bbf_tlv)
                      {
                          PLATFORM_PRINTF_DEBUG_ERROR("Malformed non-1905 Link Metric Query Tlv\n");
                      }
                      else
                      {
                          if ((*bbf_tlv == BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC) ||
                              (*bbf_tlv == BBF_TLV_TYPE_NON_1905_RECEIVER_LINK_METRIC) )
                          {
                              // Prepare a list of TLV extensions to update the
                              // datamodel
                              //
                              if (NULL == extensions)
                              {
                                  extensions = (struct vendorSpecificTLV **)PLATFORM_MALLOC(sizeof(struct vendorSpecificTLV *));
                              }
                              else
                              {
                                  extensions = (struct vendorSpecificTLV **)PLATFORM_REALLOC(extensions, sizeof(struct vendorSpecificTLV *) * (extensions_nr + 1));
                              }

                              // Store a clone of the TLV included in the CMDU,
                              // because the main stack will release it
                              //
                              extensions[extensions_nr] = vendorSpecificTLVDuplicate(vs_tlv);
                              extensions_nr++;

                              // Get the AL MAC of the neighbor who provides
                              // these metrics
                              //
                              if (*bbf_tlv == BBF_TLV_TYPE_NON_1905_TRANSMITTER_LINK_METRIC)
                              {
                                  transmitter_tlv = (struct transmitterLinkMetricTLV *)bbf_tlv;
                                  PLATFORM_MEMCPY(no_std_FROM_al_mac_address, transmitter_tlv->local_al_address, 6);
                              }
                              else
                              {
                                  receiver_tlv = (struct receiverLinkMetricTLV *)bbf_tlv;
                                  PLATFORM_MEMCPY(no_std_FROM_al_mac_address, receiver_tlv->local_al_address, 6);
                              }
                          }
                          else if (*bbf_tlv == BBF_TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE)
                          {
                              // Do nothing. No metrics to update
                              //
                          }
                          else
                          {
                              PLATFORM_PRINTF_DEBUG_ERROR("Unexpected BBF protocol extension TLV\n");
                          }

                          // Release the parsed BBF TLV (no longer used)
                          //
                          free_bbf_TLV_structure(bbf_tlv);
                      }
                  }
              }
              // Non-1905 metrics info is updated when a LinkMetrics CMDU is
              // received. How? All existing metrics are removed and the new
              // ones are added to the datamodel.
              //
              // The problem arises when a LinkMetrics CMDU does not include
              // non-1905 metrics info, because the CMDU's sender does not have
              // any non-1905 neighbor just in this moment.
              //
              // Following the update procedure, we need to remove all the
              // existing metrics in the datamodel, and add the new ones (none
              // this time) But, because there is not any non-standard TLV to
              // process, there is no way to know the AL MAC of the device from
              // whom we need to remove the metrics info
              //
              // Little trick: process standard metrics TLVs to get the CMDU's
              // sender AL MAC.
              //
              else if (*p == TLV_TYPE_TRANSMITTER_LINK_METRIC)
              {
                  struct transmitterLinkMetricTLV *metrics;

                  metrics = (struct transmitterLinkMetricTLV *)p;

                  PLATFORM_MEMCPY(std_FROM_al_mac_address, metrics->local_al_address, 6);
              }
              else if (*p == TLV_TYPE_RECEIVER_LINK_METRIC)
              {
                  struct receiverLinkMetricTLV *metrics;

                  metrics = (struct receiverLinkMetricTLV *)p;

                  PLATFORM_MEMCPY(std_FROM_al_mac_address, metrics->local_al_address, 6);
              }

              i++;
          }

          // Even when there is not any non-1905 metrics TLV, we need to remove
          // existing metrics from the datamodel. In fact, the absence of these
          // TLVs indicates that this device has zero non-1905 neighbors right
          // now.
          //
          // Instead of passing NULL as an argument to
          // 'CBKUpdateBBFExtendedInfo' we create a TLV to indicate that we
          // need to remove the metrics
          // (BBF_TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE).
          //
          if (NULL == extensions)
          {
              struct vendorSpecificTLV          *vendor_specific;
              struct linkMetricResultCodeTLV    *result_tlvs;

              // A 'result code' TLV will indicate that there are not available
              // metrics This (mark) will later force the update of metrics
              // extensions
              //
              result_tlvs = (struct linkMetricResultCodeTLV *)PLATFORM_MALLOC(sizeof(struct linkMetricResultCodeTLV));
              result_tlvs->tlv_type  = BBF_TLV_TYPE_NON_1905_LINK_METRIC_RESULT_CODE;
              result_tlvs->result_code = LINK_METRIC_RESULT_CODE_TLV_INVALID_NEIGHBOR;

              vendor_specific = vendorSpecificTLVEmbedExtension(result_tlvs, forge_bbf_TLV_from_structure, (INT8U *)BBF_OUI);

              extensions = (struct vendorSpecificTLV **)PLATFORM_MALLOC(sizeof(struct vendorSpecificTLV *));
              extensions[extensions_nr++] = vendor_specific;

              // Because there is not any non1905-metrics TLV, we use the AL
              // MAC retrieved from the standard metrics TLVs.
              //
              PLATFORM_MEMCPY(no_std_FROM_al_mac_address, std_FROM_al_mac_address, 6);
          }

          CBKUpdateBBFExtendedInfo(extensions, extensions_nr, no_std_FROM_al_mac_address);

          break;

        }

        default:
        {
            break;
        }
    }

    return 1;
}
