/*
 * actor.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 */

#ifndef DAISELABS_THREAD_ACTOR_H
#define DAISELABS_THREAD_ACTOR_H

#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace daiselabs {
namespace thread {

template<typename Q>
class actor {
public:
    typedef std::shared_ptr<Q> queue_ptr_type;
    typedef typename Q::item_ptr_type msg_ptr_type;
    typedef std::function<void (actor *, msg_ptr_type)> exec_function_type;

    actor(const actor&) = delete;
    actor &operator=(const actor&) = delete;

    actor(const char *name, exec_function_type exec_function) :
        name_(name), exec_function_(exec_function), timeout_millis_(-1), threads_(),
        queue_(new Q()) {}

    void activate(int thread_count = 1, int timeout_millis = -1) {
        timeout_millis_ = timeout_millis;
        for (int i = 0; i < thread_count; i++) {
            threads_.push_back(std::thread(&actor<Q>::thread_func, this));
            threads_.back().detach();
        }
    }

    void deactivate() {
        // the close() method of queue will wake up all waiting threads
        // and let them go out of the processing loop to exit
        queue_->close();
    }

    queue_ptr_type queue() {
        return queue_;
    }

    std::string name() const {
        return name_;
    }

protected:
    void thread_func() {
        bool terminated = false;
        while (!terminated) {
            msg_ptr_type t = queue_->get(timeout_millis_);
            exec_function_(this, t);
        }
    }

    std::string name_;
    exec_function_type exec_function_;
    int timeout_millis_;
    std::vector<std::thread> threads_;
    queue_ptr_type queue_;
};

} /* namespace thread */
} /* namespace daiselabs */



#endif /* DAISELABS_THREAD_ACTOR_H */
