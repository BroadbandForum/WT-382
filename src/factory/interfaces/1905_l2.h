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

#ifndef _1905_L2_H_
#define _1905_L2_H_

// Ethernet types por 1905 and LLDP packets
//
#define ETHERTYPE_1905  (0x893a)
#define ETHERTYPE_LLDP  (0x88cc)


// 1905 multicast address ("01:80:C2:00:00:13")
//
#define MCAST_1905_B0 (0x01)
#define MCAST_1905_B1 (0x80)
#define MCAST_1905_B2 (0xC2)
#define MCAST_1905_B3 (0x00)
#define MCAST_1905_B4 (0x00)
#define MCAST_1905_B5 (0x13)
#define MCAST_1905  {MCAST_1905_B0, MCAST_1905_B1, MCAST_1905_B2, MCAST_1905_B3, MCAST_1905_B4, MCAST_1905_B5}


// LLDP nearest bridge multicast address ("01:80:C2:00:00:0E")
//
#define MCAST_LLDP_B0  (0x01)
#define MCAST_LLDP_B1  (0x80)
#define MCAST_LLDP_B2  (0xC2)
#define MCAST_LLDP_B3  (0x00)
#define MCAST_LLDP_B4  (0x00)
#define MCAST_LLDP_B5  (0x0E)
#define MCAST_LLDP  {MCAST_LLDP_B0, MCAST_LLDP_B1, MCAST_LLDP_B2, MCAST_LLDP_B3, MCAST_LLDP_B4, MCAST_LLDP_B5}

#endif

