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

#ifndef _AL_H_
#define _AL_H_

#define AL_ERROR_OUT_OF_MEMORY      (1)
#define AL_ERROR_INVALID_ARGUMENTS  (2)
#define AL_ERROR_NO_INTERFACES      (3)
#define AL_ERROR_INTERFACE_ERROR    (4)
#define AL_ERROR_OS                 (5)
#define AL_ERROR_PROTOCOL_EXTENSION (6)

// This is the function that runs the 1905 Abstraction Layer (AL) state machine.
//
// In order to start the AL services this is what you have to do from your
// platform specific code:
//
//   1. Create a thread
//   2. Make that thread execute this function
//
// This function takes the following arguments:
//
//   - 'al_mac_address': a six bytes array containing the AL MAC address of the
//     local host.
//   - 'map_whole_network_flag': If set to '1', the AL entity will map the whole
//     network (instead of just its direct neighbors). This takes more memory
//     and generates much more packets on the network, however the TR069
//     datamodel of this particular node will contain all the information
//     defined in the standard.
//     I would personally *only* set this flag to '1' in *one* of the network
//     nodes.
//   - 'registrar_interface' is the name of the local interface (ex: "wlan0")
//     that will act as a registrar in the 1905 network.
//     Note that *only one registrar* should be present in the 1905 network,
//     thus this parameter should:
//       a) Be set to NULL in all those ALs that are not registrars.
//       b) Be set to the name of a local 802.11 interface (ex: "wlan0") in the
//          AL that will act as a registrar.
//
// It returns when:
//
//   - Something went terribly wrong (maybe at initiallization time or maybe
//     later, while doing its bussiness). It that case it returns an error code
//     which is always bigger than '0':
//
//       AL_ERROR_OUT_OF_MEMORY:
//         A call to "PLATFORM_MALLOC()" failed, meaning there is no more memory
//         available in the system.
//
//       AL_ERROR_INVALID_ARGUMENTS:
//         The provided 'al_mac_address' is not valid.
//
//       AL_ERROR_NO_INTERFACES:
//         A call to "PLATFORM_GET_LIST_OF_1905_INTERFACES()" returned an empty
//         list, meaning there is nothing for the 1905 AL entity to do.
//
//       AL_ERROR_INTERFACE_ERROR:
//         A call to "PLATFORM_GET_LIST_OF_1905_INTERFACES() returned an error
//         or some other interface related problem.
//
//       AL_ERROR_OS:
//         One of the OS-related PLATFORM_* functions returned an error (these
//         are functions use to create queues, start timers, etc...)
//
//       AL_ERROR_PROTOCOL_EXTENSION;
//         Error registering, at least, one protocol extension.
//
//   - The HLE requested the AL service to stop. In this case it will return '0'
//
INT8U start1905AL(INT8U *al_mac_address, INT8U map_whole_network_flag, char *registrar_interface);


#endif

