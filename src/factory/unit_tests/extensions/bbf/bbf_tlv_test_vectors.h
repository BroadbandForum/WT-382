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

#ifndef _BBF_TLV_TEST_VECTORS_H_
#define _BBF_TLV_TEST_VECTORS_H_

#include "bbf_tlvs.h"

#define CHECK_TRUE     0 // Check a successful parse/forge operation
#define CHECK_FALSE    1 // Check a wrong parse operation (malformed frame)

extern struct linkMetricQueryTLV                       bbf_tlv_structure_001;
extern INT8U                                           bbf_tlv_stream_001[];
extern INT16U                                          bbf_tlv_stream_len_001;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_002;
extern INT8U                                           bbf_tlv_stream_002[];
extern INT8U                                           bbf_tlv_stream_002b[];
extern INT16U                                          bbf_tlv_stream_len_002;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_003;
extern INT8U                                           bbf_tlv_stream_003[];
extern INT16U                                          bbf_tlv_stream_len_003;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_004;
extern INT8U                                           bbf_tlv_stream_004[];
extern INT8U                                           bbf_tlv_stream_004b[];
extern INT16U                                          bbf_tlv_stream_len_004;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_005;
extern INT8U                                           bbf_tlv_stream_005[];
extern INT16U                                          bbf_tlv_stream_len_005;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_006;
extern INT8U                                           bbf_tlv_stream_006[];
extern INT8U                                           bbf_tlv_stream_006b[];
extern INT16U                                          bbf_tlv_stream_len_006;

extern struct linkMetricQueryTLV                       bbf_tlv_structure_007;
extern INT8U                                           bbf_tlv_stream_007[];
extern INT16U                                          bbf_tlv_stream_len_007;

#endif

