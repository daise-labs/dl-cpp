/*
 * actor.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 *
 *  Copyright (c) Daise Labs 2017.  All rights reserved.
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

    actor(const char *name) :
        name_(name), exec_function_(), timeout_millis_(-1), threads_(),
        queue_(new Q()) {}

    void activate(exec_function_type exec_function, int thread_count = 1, int timeout_millis = -1) {
        exec_function_ = exec_function;
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
            
            if (t != nullptr) {
                exec_function_(this, t);
            }
            else {
                // This means the queue has been closed,
                // there for the thread must terminate
                terminated = true;
            }
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
