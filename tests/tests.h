#pragma once

struct test_case {
    void (*func)(test_case *);
    int assertions = 0;
    int failed = 0;
    test_case(void (*func)(test_case *))
        : func(func) {
    }
    void call() {
        func(this);
    }
};

#define REQUIRE(cond) \
    { \
        _test_case->assertions++; \
        if (!(cond)) { \
            _test_case->failed++; \
            printf("assertion failed: %s:%d: \'%s\' is false\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define REQUIRE_FALSE(cond) \
    { \
        _test_case->assertions++; \
        if ((cond)) { \
            _test_case->failed++; \
            printf("assertion failed: %s:%d: \'%s\' is true\n", __FILE__, __LINE__, #cond); \
        } \
    }

#define TEST(suite, name) \
    static void suite##_##name(test_case *); \
    test_case __##suite##_##name{suite##_##name}; \
    static void suite##_##name(test_case *_test_case)

#define TEST_RUN(suite, name) \
    printf("[  RUN   ] %s.%s\n", #suite, #name); \
    __##suite##_##name.call(); \
    if (__##suite##_##name.failed) printf("[  FAIL  ] "); \
    else printf("[  PASS  ] "); \
    printf("%s.%s (%u assertions)\n\n", #suite, #name, __##suite##_##name.assertions)

