/*
 *  Realtek Semiconductor Corp.
 *
 * Author: Zhang LongQi
 * Date  : 2018-01-29
 *
 *
 */

#include "platform.h"
#include "multi_ap_tlvs.h"


int main(void) {

    PLATFORM_PRINTF("############# multiAP_ExtensionSubelement test !!!##############");
    PLATFORM_PRINTF("%zu %zu\n", sizeof(struct multiAP_ExtensionSubelement),sizeof(HaulFlags));
    struct multiAP_ExtensionSubelement m;
    m.subelementID	=	101;
    m.haulFlags._byte=0x50; // 0101 0000
    PLATFORM_PRINTF("byte: %d \n"
           "backhaulSTA: %d \n"
           "backhaulBSS: %d \n"
           "frontfaulBSS: %d \n"
           "teardown: %d \n",
           m.haulFlags._byte,
           m.haulFlags.bits.backhaulSTA,
           m.haulFlags.bits.backhaulBSS,
           m.haulFlags.bits.frontfaulBSS,
           m.haulFlags.bits.teardown);
    if (m.haulFlags.bits.backhaulSTA == 0
            && m.haulFlags.bits.backhaulBSS == 1
            && m.haulFlags.bits.frontfaulBSS == 0
            && m.haulFlags.bits.teardown == 1
            ){
        PLATFORM_PRINTF("############# multiAP_ExtensionSubelement test OK !!!##############");
    }
}
