//
// Created by xtgu on 12/6/19.
//

#include "taskinputdata.h"

namespace ats {
    using namespace std;

    TaskInputData::TaskInputData(const shared_ptr<User> &owner, uint32_t task_id, Type &task_type, ats::Ticker &ticker,
                                 ats::Quantity &quantity, ats::BusinessType &business_type,
                                 bool amount_flag, uint64_t task_user_id, std::string &user_mac) : _owner(owner),
                                                                                                   _task_id(task_id),
                                                                                                   _ticker(ticker),
                                                                                                   _task_type(
                                                                                                           task_type),
                                                                                                   _quantity(quantity),
                                                                                                   _business_type(
                                                                                                           business_type),
                                                                                                   _amount_flag(
                                                                                                           amount_flag),
                                                                                                   _task_user_id(
                                                                                                           task_user_id),
                                                                                                   _user_mac(user_mac) {

    }


}
