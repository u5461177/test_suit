//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TASKFACTORY_H
#define ATS_TASKFACTORY_H

#include "user.h"
#include "task.h"
#include "redischannel.h"
#include "types/markettype.h"
#include "types/sidetype.h"
#include "taskinterface.h"
#include "taskinputdata.h"
#include "taskproxy.h"

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;
    using Quantity = std::pair<int64_t, SideType>;

    struct TaskFactory {

        static std::shared_ptr<ats::TaskProxy>
        create_task(const std::shared_ptr<TaskInputData> &task_input_data);

    };

}

#endif //ATS_TASKFACTORY_H
