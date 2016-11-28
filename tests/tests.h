#pragma once

namespace detail {

struct test_case final {
    void (*func)();
    int assertions = 0;
    int failed = 0;
    test_case(void (*func)())
        : func(func) {
    }
    void call() {
        func();
    }
};

} // namespace detail

extern detail::test_case *_current_test_case;

#define REQUIRE(cond) \
    { \
        _current_test_case->assertions++; \
        if (!(cond)) { \
            _current_test_case->failed++; \
            printf("assertion failed: %s:%d: \'%s\' is false\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define REQUIRE_FALSE(cond) \
    { \
        _current_test_case->assertions++; \
        if ((cond)) { \
            _current_test_case->failed++; \
            printf("assertion failed: %s:%d: \'%s\' is true\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define TEST(suite, name) \
    static void suite##_##name(); \
    detail::test_case __##suite##_##name{suite##_##name}; \
    static void suite##_##name()

#ifdef TEST_MAIN

detail::test_case *_current_test_case = nullptr;

#define TEST_RUN(suite, name) \
    printf("\e[32m[  RUN   ]\e[0m %s.%s\n", #suite, #name); \
    _current_test_case = &__##suite##_##name; \
    __##suite##_##name.call(); \
    if (__##suite##_##name.failed) printf("\e[31m[  FAIL  ]\e[0m "); \
    else printf("\e[32m[  PASS  ]\e[0m "); \
    printf("%s.%s (%u assertions)\n\n", #suite, #name, __##suite##_##name.assertions)

#endif

