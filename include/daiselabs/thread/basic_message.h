/*
 * basic_message.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 *
 *  Copyright (c) Daise Labs 2017.  All rights reserved.
 */

#ifndef DAISELABS_THREAD_BASIC_MESSAGE_H
#define DAISELABS_THREAD_BASIC_MESSAGE_H

#include <memory>

namespace daiselabs {
namespace thread {

template<typename T, typename P>
class basic_message {
public:
    typedef std::shared_ptr<P> payload_ptr_type;


    basic_message(const basic_message&) = delete;
    basic_message& operator=(const basic_message&) = delete;

    basic_message(T message_type, payload_ptr_type payload) :
        message_type_(message_type), payload_(payload) {
    }

    basic_message(basic_message&&) = default;

    virtual ~basic_message() = default;

    T message_type() const {
        return message_type_;
    }

    payload_ptr_type payload() const {
        return payload_;
    }
protected:

    T message_type_;
    payload_ptr_type payload_;
};

} /* namespace thread */
} /* namespace daiselabs */

#endif /* DAISELABS_THREAD_BASIC_MESSAGE_H */
