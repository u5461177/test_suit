//
// Created by xtgu on 12/6/19.
//

#ifndef MUSE_TASKINPUTDATA_H
#define MUSE_TASKINPUTDATA_H

#include <memory>
#include "types/markettype.h"
#include "types/sidetype.h"
#include "types/businesstype.h"
#include "taskinterface.h"

namespace ats {
    class User;

    using Ticker = std::pair<std::string, MarketType>;
    using Quantity = std::pair<int64_t, SideType>;

    struct TaskInputData {
        TaskInputData(const std::shared_ptr<User> &owner, uint32_t task_id, Type &task_type, Ticker &ticker,
                      Quantity &quantity,
                      BusinessType &business_type, bool amount_flag, uint64_t task_user_id, std::string &user_mac);

        std::shared_ptr<User> _owner;
        uint32_t _task_id;
        Ticker _ticker;
        Type _task_type;
        Quantity _quantity;
        BusinessType &_business_type;
        bool _amount_flag;
        uint64_t _task_user_id;
        std::string _user_mac;
    };

}


#endif //MUSE_TASKINPUTDATA_H
