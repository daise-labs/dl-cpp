/*
 * queue.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 */

#ifndef DAISELABS_THREAD_QUEUE_H
#define DAISELABS_THREAD_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <utility>

namespace daiselabs {
namespace thread {


template<typename T>
class queue {
public:
    typedef std::shared_ptr<T> item_ptr_type;

    queue(const queue&) = delete;
    queue &operator=(const queue &) = delete;

    queue() : closed_(false), cond_(), mutex_(), queue_() {}
    ~queue() = default;

    void put(item_ptr_type item, bool notify_all = false) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.emplace(item);
        }

        if (notify_all) {
            cond_.notify_all();
        } else {
            cond_.notify_one();
        }
    }

    item_ptr_type get(int timeout_millis) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (timeout_millis <= 0) {
            cond_.wait(lock, [this]{ return (closed_ || !queue_.empty()); });
        } else {
            // wait_for returns false when timed out
            if (cond_.wait_for(lock, std::chrono::milliseconds(timeout_millis), [this]{ return (closed_ || !queue_.empty()); }) == false) {
                return nullptr;
            }
        }

        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }

        cond_.notify_all();
    }
private:
    bool closed_;
    std::condition_variable cond_;
    std::mutex mutex_;
    std::queue<item_ptr_type> queue_;
};


} /* namespace thread */
} /* namespace daiselabs */


#endif /* DAISELABS_THREAD_QUEUE_H */
