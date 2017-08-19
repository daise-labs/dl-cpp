/*
 * actor.h
 *
 *  Created on: Feb 9, 2017
 *      Author: hynguyen@daise-labs.com
 *
 *  Copyright (c) Daise Labs 2017.  All rights reserved.
 */

#ifndef DAISELABS_UTIL_BUFFER_H
#define DAISELABS_UTIL_BUFFER_H

#include <cstddef>
#include <cstring>


#ifndef DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_INITIAL_SIZE
#define DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_INITIAL_SIZE  (4*1024)
#endif

#ifndef DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_GROWTH_FACTOR
#define DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_GROWTH_FACTOR 1.5f
#endif

namespace  daiselabs {
namespace util {

template<std::size_t size>
class basic_buffer {
public:
    basic_buffer() :
        buf_(NULL),
        capacity_(0) {

        buf_ = new char[size];
        capacity_ = size;
    }

    virtual ~basic_buffer() {
        if (buf_ != NULL) {
            delete [] buf_;
        }
        buf_ = NULL;
        capacity_ = 0;
    }

    virtual bool ensure_capacity(std::size_t n) {
        return (n <= capacity_);
    }

    const char * buf() const {
        return buf_;
    }

    char * buf() {
        return buf_;
    }

    std::size_t capacity() const {
        return capacity_;
    }

protected:
    char *buf_;
    std::size_t capacity_;
};

template<
    std::size_t initial_size = DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_INITIAL_SIZE>
class growable_buffer : public basic_buffer<initial_size> {
public:
    growable_buffer(float growth_factor = DAISELABS_UTIL_GROWABLE_BUFFER_DEFAULT_GROWTH_FACTOR ) :
        basic_buffer<initial_size>(),
        growth_factor_(growth_factor) {
    }

    virtual ~growable_buffer() {
    }

    bool ensure_capacity(std::size_t n) override {
        if (n > this->capacity_) {

            std::size_t cap = this->capacity_;
            while (n > cap) {
                 cap = static_cast<std::size_t>(static_cast<float>(cap) * growth_factor_);
            }
            char *buf = new char[cap];
            std::memcpy(buf, this->buf_, this->capacity_);
            delete [] this->buf_;
            this->buf_ = buf;
            this->capacity_ = cap;
        }

        return true;
    }

protected:
    float growth_factor_;
};

template<typename T>
class buffer_writer {
public:
    buffer_writer(T *buf) : buf_(buf), pos_(0) {
    }

    std::size_t pos() const {
        return pos_;
    }

    void reset() {
        pos_ = 0;
    }

    std::size_t write(const char *s, std::size_t n) {
        if (buf_->ensure_capacity(pos_ + n)) {
            std::memcpy(buf_->buf() + pos_, s, n);
            pos_ += n;
            return n;
        } else {
            return 0;
        }
    }

private:
    T * buf_;
    std::size_t pos_;
};


} /* namespace util */
} /* namespace  daiselabs */


#endif /* DAISELABS_UTIL_BUFFER_H */
