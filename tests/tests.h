#pragma once

#include <inherited_list.h>

using tests_printer = void (*)(const char *, ...);

namespace etf {

namespace detail {

extern tests_printer _print;

struct test_session final {

    class test_case final : public yacppl::inherited_list<test_case> {

        const char *_suite_name;
        const char *_test_name;
        void (*_func)();

        void print_test_start_message() {
            etf::detail::_print("\e[32m[  RUN   ]\e[0m %s.%s\n", _suite_name, _test_name);
        }

        void print_test_result() {
            if (failed)
                etf::detail::_print("\e[31m[  FAIL  ]\e[0m ");
            else
                etf::detail::_print("\e[32m[  PASS  ]\e[0m ");
            etf::detail::_print("%s.%s (%u assertions)\n\n", _suite_name, _test_name, assertions);
        }

    public:

        int assertions = 0;
        int failed = 0;

        test_case(const char *suite, const char *test, void (*func)())
                : _suite_name(suite), _test_name(test), _func(func) {
            test_session::get().register_test(this);
        }

        void assert(bool cond) {
            ++assertions;
            if (!cond) ++failed;
        }

        void call() {
            print_test_start_message();
            _func();
            print_test_result();
        }

    };

private:

    yacppl::inherited_list<test_case> _test_cases;
    test_case *_current_test_case;
    size_t _tests_number = 0;
    static test_session _instance;

public:

    static test_session &get() {
        return _instance;
    }

    void register_test(test_case *t) {
        _tests_number++;
        _test_cases.add(t);
    }

    void run() {
        etf::detail::_print("\e[32m[========]\e[0m Running %u test cases\n\n", _tests_number);
        for (auto &test : _test_cases) {
            _current_test_case = &test;
            test.call();
        }
    }

    test_case &current_test_case() {
        return *_current_test_case;
    }

};

} // namespace detail

#define REQUIRE(cond) \
    { \
        etf::detail::test_session::get().current_test_case().assert(cond); \
        if (!(cond)) \
            etf::detail::_print("assertion failed: %s:%d: \'%s\' is false\n", __FILE__, __LINE__, #cond); \
    }

#define REQUIRE_FALSE(cond) \
    { \
        etf::detail::test_session::get().current_test_case().assert(!(cond)); \
        if (cond) \
            etf::detail::_print("assertion failed: %s:%d: \'%s\' is true\n", __FILE__, __LINE__, #cond); \
    }

#define ETF_UNIQUE_NAME(name) \
    name##__COUNTER__

#define TEST(suite, name) \
    static void suite##_##name(); \
    etf::detail::test_session::test_case ETF_UNIQUE_NAME(suite##_##name){#suite, #name, suite##_##name}; \
    static void suite##_##name()

#ifdef TEST_MAIN

namespace detail {

test_session test_session::_instance;
tests_printer _print;

} // namespace detail

void main(tests_printer printer) {
    detail::_print = printer;
    detail::test_session::get().run();
}

#endif

} // namespace etf


