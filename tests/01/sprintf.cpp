#include <cstdint>
#include <kernel/definitions.hpp>
#include <yatf.h>

namespace {

} // namespace

TEST(sprintf, can_format_percent_sign) {
    char buffer[32];
    sprintf(buffer, "%%");
    REQUIRE_EQ((const char *)buffer, "%");
}

TEST(sprintf, can_format_int) {
    char buffer[32];
    sprintf(buffer, "%d", 21);
    REQUIRE_EQ((const char *)buffer, "21");
    sprintf(buffer, "%d", -910);
    REQUIRE_EQ((const char *)buffer, "-910");
    sprintf(buffer, "%d", 0);
    REQUIRE_EQ((const char *)buffer, "0");
    sprintf(buffer, "%+d", 124);
    REQUIRE_EQ((const char *)buffer, "+124");
}

TEST(sprintf, can_format_unsigned_int) {
    char buffer[32];
    sprintf(buffer, "%u", 21);
    REQUIRE_EQ((const char *)buffer, "21");
    sprintf(buffer, "%u", 125);
    REQUIRE_EQ((const char *)buffer, "125");
    sprintf(buffer, "%u", 0);
    REQUIRE_EQ((const char *)buffer, "0");
}

TEST(sprintf, can_format_char) {
    char buffer[32];
    sprintf(buffer, "%c", 'c');
    REQUIRE_EQ((const char *)buffer, "c");
    sprintf(buffer, "%c", 'a');
    REQUIRE_EQ((const char *)buffer, "a");
    sprintf(buffer, "%c", 0);
    REQUIRE_EQ((const char *)buffer, "");
}

TEST(sprintf, can_format_string) {
    char buffer[32];
    sprintf(buffer, "%s", "test string");
    REQUIRE_EQ((const char *)buffer, "test string");
    sprintf(buffer, "testing");
    REQUIRE_EQ((const char *)buffer, "testing");
}

TEST(sprintf, can_format_pointer) {
    char buffer[32];
    sprintf(buffer, "%p", 0x21);
    REQUIRE_EQ((const char *)buffer, "0x0000000000000021");
}

TEST(sprintf, can_pad) {
    char buffer[32];
    sprintf(buffer, "%06d", 324);
    REQUIRE_EQ((const char *)buffer, "000324");
    sprintf(buffer, "%#05x", 0x21);
    REQUIRE_EQ((const char *)buffer, "0x021");
    sprintf(buffer, "%6d", 324);
    REQUIRE_EQ((const char *)buffer, "   324");
    sprintf(buffer, "%6c", 'c');
    REQUIRE_EQ((const char *)buffer, "     c");
    sprintf(buffer, "%-6c", 'c');
    REQUIRE_EQ((const char *)buffer, "c     ");
    sprintf(buffer, "% 6c", 'c');
    REQUIRE_EQ((const char *)buffer, "     c");
}

