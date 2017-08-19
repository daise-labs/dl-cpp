/*
 * pipeline.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 *
 *  Copyright (c) Daise Labs 2017.  All rights reserved.
 */

#ifndef DAISELABS_PIPELINE_PIPELINE_H
#define DAISELABS_PIPELINE_PIPELINE_H

#include <memory>
#include <vector>

#include "daiselabs/thread/actor.h"
#include "daiselabs/thread/basic_message.h"
#include "daiselabs/thread/queue.h"

namespace daiselabs {
namespace pipeline {

#include <functional>
#include <memory>
#include <vector>

#include <daiselabs/thread/actor.h>
#include <daiselabs/thread/queue.h>



template<typename S>
class connector {
public:
    typedef typename S::item_ptr_type msg_ptr_type;
    typedef std::shared_ptr<S> sink_ptr_type;

    connector() : sinks_() {}
    virtual ~connector() = default;

    void add_sink(sink_ptr_type sink) {
        sinks_.push_back(sink);
    }

    virtual void send(msg_ptr_type msg) {
        for (sink_ptr_type s : sinks_) {
            s->put(msg);
        }
    }

protected:
    std::vector<sink_ptr_type> sinks_;
};

/**
 * connection is a sink adapter.  It provides the put(item_ptr_type) method
 */
template<typename D>
class connection {
public:
    typedef typename D::item_ptr_type item_ptr_type;
    typedef std::shared_ptr<D> destination_ptr_type;

    connection(destination_ptr_type dest) : dest_(dest) {}
    virtual ~connection() = default;

    void put(item_ptr_type msg) {
        dest_.put(msg);
    }
protected:
    destination_ptr_type dest_;
};

template<typename Q>
class stage {
public:
    typedef daiselabs::thread::actor<Q> actor_type;
    typedef typename actor_type::msg_ptr_type msg_ptr_type;
    typedef typename actor_type::queue_ptr_type queue_ptr_type;
    typedef connector<Q> in_connector_type;
    typedef std::function<void (stage *, msg_ptr_type)> exec_function_type;

    stage(const char *name, exec_function_type exec_func) :
        actor_(name,
                [this](actor_type *actor, msg_ptr_type msg) {
                    exec_func_(this, msg);
                }),
        exec_func_(exec_func),
        in_connector_() {
        in_connector_.add_sink(actor_.queue());
    }

    virtual ~stage() = default;

    in_connector_type in_connector() {
        return in_connector_;
    }

    void start(int thread_count = 1, int timeout_millis = -1) {
        actor_.activate(thread_count, timeout_millis);
    }

private:
    actor_type actor_;
    exec_function_type exec_func_;
    in_connector_type in_connector_;
};


} /* namespace pipeline */
} /* namespace daiselabs */


#endif /* DAISELABS_PIPELINE_PIPELINE_H */
