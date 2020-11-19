//
// Created by xtgu on 11/28/19.
//

#ifndef MUSE_ORDERCANCELINFO_H
#define MUSE_ORDERCANCELINFO_H

#include <stdint-gcc.h>
#include "xoms_api_struct.h"

namespace ats {

    struct OrderCancelInfo {

        OrderCancelInfo(XTPOrderCancelInfo *xtpOrderCancelInfo);

        uint64_t order_cancel_xtp_id;
        uint64_t order_xtp_id;

    };
}


#endif //MUSE_ORDERCANCELINFO_H
