#pragma once

struct error {

    enum number {
        err_no_error = 0,
        err_no_such_file,
        err_is_a_dir,
        err_exists,
        err_cannot_create,
        err_no_root,
    };

private:
    unsigned errno_;

public:

    error(unsigned errno) : errno_(errno) {
    }

    unsigned errno() const {
        return errno_;
    }

    const char *what() const {
        switch (errno_) {
            case err_no_error:
                return "No error";
            case err_no_such_file:
                return "No such file";
            case err_is_a_dir:
                return "Is a directory";
            case err_exists:
                return "File or directory exists";
            case err_no_root:
                return "No root mounted";
            case err_cannot_create:
                return "Cannot create file/directory";
            default:
                return "Unknown error";
        }
    }

    operator bool() const {
        return errno_ != 0;
    }

};

template <typename Value>
class error_wrapper {

    error err_;
    Value v_;

public:

    error_wrapper(unsigned errno) : err_(errno) {
    }

    error_wrapper(const error &err) : err_(err) {
    }

    error_wrapper(const Value &v) : err_(error::err_no_error), v_(v) {
    }

    error_wrapper(const error_wrapper &w) : err_(w.err_), v_(w.v_) {
    }

    operator bool() const {
        return err_ == 0;
    }

    Value operator *() {
        return v_;
    }

    Value operator->() {
        return v_;
    }

    auto error() const {
        return err_;
    }

};

