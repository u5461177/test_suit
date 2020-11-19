//
// Created by xtgu on 25/7/19.
//

#include "gloghelper.h"

namespace ats {

    using namespace std;
    void signal_handle(const char *data, int size) {
        ofstream fs("/home/data/muse_log/glog_dump.glog", ios::app);
        string str = string(data, size);
        fs << str;
        fs.close();
        LOG(ERROR) << str;
    }


    void GLogHelper::_remove_old_log(const string &log_dir) {
        DIR *dir;
        struct dirent *ent;
        chrono::system_clock::time_point now_date = chrono::system_clock::now();
        if ((dir = opendir(log_dir.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                string log_name{ent->d_name};
                auto v_names = str_split(log_name, ".");
                if (v_names.size() < 6){
                    continue;
                }
                auto log_name_vec = str_split(v_names[5], "-");
                if (!log_name_vec.empty() and log_name_vec.size() == 2) {
                    auto log_date = string_to_time_date(log_name_vec[0]);
                    chrono::seconds log_duration = chrono::duration_cast<chrono::seconds>(now_date - log_date);
                    if (log_duration.count() > 864000) {
                        string log_addr{log_dir + "/"};
                        if (remove((log_addr + log_name).c_str())) {
                            LOG(ERROR) << "Can not remove old log files !!";
                        }
                    }
                }

            }
        } else {
            LOG(ERROR) << "Can not remove old log files !!";
        }

    }

    GLogHelper::GLogHelper(char *program, const string &log_dir) {
        google::InitGoogleLogging(program);
        FLAGS_log_dir = log_dir;
        FLAGS_logbufsecs = 0;   // record log in time
        FLAGS_alsologtostderr = true;
        google::SetStderrLogging(google::INFO);
        google::InstallFailureSignalHandler();
        FLAGS_stop_logging_if_full_disk = true;
        _remove_old_log(log_dir);
        google::InstallFailureWriter(&signal_handle);
    }

    GLogHelper::~GLogHelper() {
        LOG(INFO) << "glog helper exit";
        google::ShutdownGoogleLogging();
    }

}