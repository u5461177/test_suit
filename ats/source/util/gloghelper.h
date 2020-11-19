//
// Created by xhshu on 12/6/18.
//

#ifndef MUSE_GLOGHELPER_H
#define MUSE_GLOGHELPER_H

#include "glog/logging.h"
#include "timehelper.h"
#include "common.h"

#include <memory>
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace ats {

    class GLogHelper {

    public:
        explicit GLogHelper(char *program, const std::string &log_dir);

        virtual ~GLogHelper();

    public:
        void _remove_old_log(const std::string &log_dir);

    };

}

#endif //MUSE_GLOGHELPER_H
