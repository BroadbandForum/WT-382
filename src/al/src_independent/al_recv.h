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

#ifndef _AL_RECV_H_
#define _AL_RECV_H_

#include "1905_cmdus.h"
#include "lldp_payload.h"

// This function does "whatever needs to be done" as a result of receiving a
// CMDU: for example, some CMDU trigger a response, others are used to update
// the topology data base, etc...
//
// This function does *not* deal with "discarding" or "forwarding" the packet
// (that should have already been taken care of before this function is called)
//
// 'c' is the just received CMDU structure.
//
// 'receiving_interface_addr' is the MAC address of the local interface where
// the CMDU packet was received
//
// 'src_addr' is the MAC address contained in the 'src' field of the ethernet
// frame which contained the just received CMDU payload.
//
// 'queue_id' is the ID of the queue where messages to the AL entity should be
// posted in case new actions are derived from the processing of the current
// message.
//
// Return values:
//   PROCESS_CMDU_KO:
//     There was problem processing the CMDU
//   PROCESS_CMDU_OK:
//     The CMDU was correctly processed. No further action required.
//   PROCESS_CMDU_OK_TRIGGER_AP_SEARCH:
//     The CMDU was correctly processed. The caller should now trigger an "AP
//     search" process if there is still an unconfigured AP local interface.
//
#define PROCESS_CMDU_KO                     (0)
#define PROCESS_CMDU_OK                     (1)
#define PROCESS_CMDU_OK_TRIGGER_AP_SEARCH   (2)
INT8U process1905Cmdu(struct CMDU *c, INT8U *receiving_interface_addr, INT8U *src_addr, INT8U queue_id);

// Call this function when receiving an LLPD "bridge discovery" message so that
// the topology database is properly updated.
//
INT8U processLlpdPayload(struct PAYLOAD *payload, INT8U *receiving_interface_addr);

// Call this function when receiving an ALME REQUEST message. It will take
// action depending on the actual contents of this message (ie. "shut down an
// interface", "add a new bridge configuration", etc...)
//
INT8U process1905Alme(INT8U *alme_tlv, INT8U alme_client_id);

#endif


