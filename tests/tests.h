#pragma once

#include <inherited_list.h>

int tprintf(const char *fmt, ...);

namespace detail {

class test_case;
extern detail::test_case *_current_test_case;

class test_session {

    yacppl::inherited_list<test_case> _test_cases;
    static test_session singleton;

public:

    static test_session &get() {
        return singleton;
    }

    void register_test(test_case *t) {
        _test_cases.add(t);
    }

    auto begin() {
        return _test_cases.begin();
    }

    auto end() {
        return _test_cases.end();
    }

};

struct test_case final : public yacppl::inherited_list<test_case> {

    const char *suite_name;
    const char *test_name;
    void (*func)();
    int assertions = 0;
    int failed = 0;

    test_case(const char *suite, const char *test, void (*func)())
        : suite_name(suite), test_name(test), func(func) {
        test_session::get().register_test(this);
    }
    void call() {
        tprintf("\e[32m[  RUN   ]\e[0m %s.%s\n", suite_name, test_name); \
        _current_test_case = this;
        func();
        if (failed) tprintf("\e[31m[  FAIL  ]\e[0m ");
        else tprintf("\e[32m[  PASS  ]\e[0m ");
        tprintf("%s.%s (%u assertions)\n\n", suite_name, test_name, assertions);
    }

};

} // namespace detail

#define REQUIRE(cond) \
    { \
        detail::_current_test_case->assertions++; \
        if (!(cond)) { \
            detail::_current_test_case->failed++; \
            tprintf("assertion failed: %s:%d: \'%s\' is false\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define REQUIRE_FALSE(cond) \
    { \
        detail::_current_test_case->assertions++; \
        if ((cond)) { \
            detail::_current_test_case->failed++; \
            tprintf("assertion failed: %s:%d: \'%s\' is true\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define TEST(suite, name) \
    static void suite##_##name(); \
    detail::test_case __##suite##_##name{#suite, #name, suite##_##name}; \
    static void suite##_##name()

#ifdef TEST_MAIN

namespace detail {

detail::test_session detail::test_session::singleton;
detail::test_case *_current_test_case = nullptr;

}

#endif

