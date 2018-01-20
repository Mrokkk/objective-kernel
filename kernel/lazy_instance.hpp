#pragma once

namespace yacppl {

template <typename T>
struct lazy_instance {

    ~lazy_instance() {
        if (instance_) instance_->~T();
    }

    template <typename ...Args>
    void initialize(const Args &...args) {
        instance_ = new(data_) T(args...);
    }

    operator bool() const {
        return instance_ != nullptr;
    }

    T *operator->() {
        if (instance_ == nullptr) {
            return reinterpret_cast<T *>(const_cast<char *>(data_));
        }
        return instance_;
    }

    T *get() {
        return instance_;
    }

private:
    char data_[sizeof(T)];
    T *instance_ = nullptr;
};

} // namespace utils

