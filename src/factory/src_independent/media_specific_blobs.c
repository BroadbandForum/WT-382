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

#include "media_specific_blobs.h"


////////////////////////////////////////////////////////////////////////////////
// Actual API functions
////////////////////////////////////////////////////////////////////////////////

INT8U *forge_media_specific_blob(struct genericInterfaceType *m, INT16U *len)
{
    #define ITU_T_GHN_XML "http://handle.itu.int/11.1002/3000/1706"

    INT8U *ret;

    ret = NULL;

    if (NULL == m->generic_phy_description_xml_url)
    {
        return NULL;
    }

    if (0 == PLATFORM_MEMCMP(m->generic_phy_description_xml_url, ITU_T_GHN_XML, PLATFORM_STRLEN(ITU_T_GHN_XML)+1))
    {
        // This XML file defines the *same* media specific data format for all
        // interfaces that meet the following requirements:
        //
        //   - OUI     = 00:19:A7
        //   - Variant = 0, 1, 2, 3 or 4 (it also defines 10 and 11, but we will
        //               ignore these)

        if (
             m->oui[0] != 0x00 ||
             m->oui[1] != 0x19 ||
             m->oui[2] != 0xa7
           )
        {
            // Unknown OUI
        }
        else if (
             m->variant_index != 0 &&
             m->variant_index != 1 &&
             m->variant_index != 2 &&
             m->variant_index != 3 &&
             m->variant_index != 4
           )
        {
            // Unknown variant
        }
        else
        {
           // The 1905 media specific field is made up of FIVE bytes:
           //
           //   0x01, 0x00, 0x02, dni[0], dni[1]
           //
           // (see ITU-T G.9979 Tables 8.2 and 8.3)
           //
           *len   = 5;
           ret    = (INT8U *)PLATFORM_MALLOC(*len);
           ret[0] = 0x01;
           ret[1] = 0x00;
           ret[2] = 0x02;
           ret[3] = m->media_specific.ituGhn.dni[0];
           ret[4] = m->media_specific.ituGhn.dni[1];
        }
    }

    if (NULL == ret)
    {
        // If we get to this point and "ret" is still "NULL", that means that the
        // "XML"/"OUI"/"variant_index" combination has not been recognized, and thus
        // we will simply return the contents of the "unsupported" structure.
        //
        *len = m->media_specific.unsupported.bytes_nr;
        ret  = (INT8U *)PLATFORM_MALLOC(*len);

        PLATFORM_MEMCPY(ret, m->media_specific.unsupported.bytes, *len);
    }

    return ret;
}

