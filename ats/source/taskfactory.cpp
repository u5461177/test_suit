//
// Created by Rong Zhou on 2018-08-03.
//

#include "taskfactory.h"
#include "twaptask.h"
#include "vwaptask.h"
#include "istask.h"
#include "povtask.h"
#include "aivwaptask.h"
#include "randomtask.h"
#include "icebergtask.h"
#include "mstdtask.h"
#include "util/cryptohelper.h"
#include "redisserver.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    shared_ptr<TaskProxy>
    TaskFactory::create_task(const shared_ptr<TaskInputData> &task_input) {
        switch (task_input->_task_type) {
            case Type::TWAP:
                return make_shared<TaskProxy>(make_shared<TWAPTask>(task_input));
            case Type::VWAP:
                return make_shared<TaskProxy>(make_shared<VWAPTask>(task_input));
            case Type::IS:
                return make_shared<TaskProxy>(make_shared<ISTask>(task_input));
            case Type::POV:
                return make_shared<TaskProxy>(make_shared<POVTask>(task_input));
            case Type::RANDOM:
                return make_shared<TaskProxy>(make_shared<RandomTask>(task_input));
            case Type::ICEBERG:
                return make_shared<TaskProxy>(make_shared<IcebergTask>(task_input));
            case Type::AIVWAP:
                return make_shared<TaskProxy>(make_shared<AIVWAPTask>(task_input));
            case Type::MSTD:
            	return make_shared<TaskProxy>(make_shared<MSTDTask>(task_input));
            default:
                LOG(WARNING) << "unknown task type";
                return nullptr;
        }
    }
}
