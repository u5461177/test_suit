//
// Created by xtgu on 11/28/19.
//

#include "ordercancelinfo.h"
namespace  ats {

    OrderCancelInfo::OrderCancelInfo(XTPOrderCancelInfo *xtpOrderCancelInfo) {
        order_xtp_id = xtpOrderCancelInfo->order_xtp_id;
        order_cancel_xtp_id = xtpOrderCancelInfo->order_cancel_xtp_id;

    }
}
