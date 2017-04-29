#include <cstdint>
#include <kernel/definitions.hpp>
#include <yatf.h>

namespace {

} // namespace

TEST(sprintf, can_format_int) {
    char buffer[32];
    sprintf(buffer, "%d", 21);
    REQUIRE_EQ((const char *)buffer, "21");
    sprintf(buffer, "%d", -910);
    REQUIRE_EQ((const char *)buffer, "-910");
    sprintf(buffer, "%d", 0);
    REQUIRE_EQ((const char *)buffer, "0");
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

TEST(sprintf, can_pad) {
    char buffer[32];
    sprintf(buffer, "%06d", 324);
    REQUIRE_EQ((const char *)buffer, "000324");
    sprintf(buffer, "0x%03x", 0x21);
    REQUIRE_EQ((const char *)buffer, "0x021");
}

