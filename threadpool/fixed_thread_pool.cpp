//
// Created by xtgu on 8/26/20.
//

#include <thread>
#include <iostream>
#include <sstream>
#include "fixed_thread_pool.h"

using namespace std;
using namespace gu;

Fixed_thread_pool::Fixed_thread_pool(size_t thread_cound):data_(std::make_shared<data>()) {
    for(size_t i =0; i < thread_cound; ++i) {
        //cout << "Create threads " << i <<  endl;
        std::thread([data = data_, this] {
            printf("first [ %s ]\n", get_this_thread_id().c_str());
            std::unique_lock<std::mutex> lk(data->mtx_);
            printf("second [ %s]\n", get_this_thread_id().c_str());
            for(;;){
                if(!data->tasks_.empty()){
                    auto current = std::move(data->tasks_.front());
                    data->tasks_.pop();
                    //lk.unlock();
                    cout << "Thread: [" <<get_this_thread_id() << "]" << endl;
                    current();
                   // lk.lock();
                } else if(data->is_shutdown_){
                    break;
                } else {
                    printf("third [ %s]\n", get_this_thread_id().c_str());
                    data->cond_.wait(lk);
                }
            }

        }).detach();
    }
}

string Fixed_thread_pool::get_this_thread_id() {
    std::ostringstream oss;
    oss << this_thread::get_id();
    return oss.str();
}


Fixed_thread_pool::~Fixed_thread_pool() {
    if((bool)data_){
        {
            std::lock_guard<std::mutex> lk(data_->mtx_);
            data_->cond_.notify_all();
        }
    }

}
