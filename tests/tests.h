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

#define ASSERT_TRUE(cond) \
    { \
        _test_case->assertions++; \
        if (!(cond)) { \
            _test_case->failed++; \
        } \
    }

#define ASSERT_FALSE(cond) \
    { \
        _test_case->assertions++; \
        if ((cond)) { \
            _test_case->failed++; \
        } \
    }

#define TEST(suite, name) \
    static void suite##_##name(test_case *); \
    test_case __##suite##_##name{suite##_##name}; \
    static void suite##_##name(test_case *_test_case)


