//
// Created by xtgu on 8/26/20.
//

#ifndef TESTTHREADPOL_FIXED_THREAD_POOL_H
#define TESTTHREADPOL_FIXED_THREAD_POOL_H


#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
namespace gu {

    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()>> tasks_;
    };


    class Fixed_thread_pool {

    public:
        explicit Fixed_thread_pool(size_t thead_cound);

        Fixed_thread_pool() = default;
        Fixed_thread_pool(Fixed_thread_pool&&) = default;
        ~Fixed_thread_pool();

    private:
        std::shared_ptr<data> data_;
        std::string get_this_thread_id();

    public:
//        template <class F>
//        void execute(F &&task) {
//            {
//                std::lock_guard<std::mutex> lk(data_->mtx_);
//                data_->tasks_.emplace(std::forward<F>(task));
//            }
//            data_->cond_.notify_one();
//
//        }
        template <class F, class... Args>
        void execute(F&& func, Args&&... args) {
            {
                auto task = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->tasks_.emplace(std::forward<decltype(task)>(task));
            }
            data_->cond_.notify_one();
        }


    };
}


#endif //TESTTHREADPOL_FIXED_THREAD_POOL_H
