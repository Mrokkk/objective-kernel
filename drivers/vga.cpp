#include <string.hpp>
#include <algorithm.hpp>
#include <kernel/cpu/io.hpp>
#include <kernel/memory/paging/paging.hpp>

#define blink (1 << 7)
#define RESX 80
#define RESY 25

namespace drivers {

namespace vga {

enum class color : uint8_t {
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    gray,
    dark_gray,
    bright_blue,
    bright_green,
    bright_cyan,
    bright_red,
    bright_magenta,
    yellow,
    white
};

constexpr uint8_t forecolor(color c) {
    return static_cast<uint8_t>(c) & 0xf;
}

constexpr uint8_t backcolor(color c) {
    return (static_cast<uint8_t>(c) << 4) & 0x7f;
}

class video_char final {

    uint8_t char_;
    uint8_t attr_;

public:

    video_char() : char_(' '), attr_(forecolor(color::gray) | backcolor(color::black)) {
    }

    video_char(uint8_t c, uint8_t attr = forecolor(color::gray) | backcolor(color::black))
            : char_(c), attr_(attr) {
    }

} PACKED;

video_char *pointer = reinterpret_cast<video_char *>(memory::virt_address(0xb8000u));
uint8_t default_attribute = forecolor(color::gray) | backcolor(color::black);
uint8_t csr_x;
uint8_t csr_y;

uint16_t current_offset_get() {
    return csr_y * RESX + csr_x;
}

void scroll() {
    video_char blank;
    if(csr_y >= RESY) {
        auto temp = csr_y - RESY + 1;
        utils::copy(pointer + temp * RESX, pointer, (RESY - temp) * RESX);
        utils::fill(pointer + (RESY - temp) * RESX, RESX, blank);
        csr_y = RESY - 1;
    }
}

void move_csr() {
    auto off = current_offset_get();
    cpu::io::outb(14, 0x3D4);
    cpu::io::outb(static_cast<uint8_t>(off >> 8), 0x3D5);
    cpu::io::outb(15, 0x3D4);
    cpu::io::outb(static_cast<uint8_t>(off), 0x3D5);
}

void cls() {
    video_char blank;
    utils::fill(pointer, RESX * RESY, blank);
    csr_x = 0u;
    csr_y = 0u;
    move_csr();
}

void putch(unsigned char c) {
    if (c == '\b') {
        if(csr_x != 0) {
            csr_x--;
            putch(' ');
            csr_x--;
        }
    } else if (c == '\t') {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    else if (c == '\r') {
        csr_x = 0u;
    }
    else if (c == '\n') {
        csr_x = 0u;
        csr_y++;
    } else if (c >= ' ') {
        pointer[current_offset_get()] = video_char(c);
        csr_x++;
    }
    if (csr_x >= RESX) {
        csr_x = 0u;
        csr_y++;
    }
    scroll();
    move_csr();
}

void print(const char *text) {
    while (*text) {
        putch(*text++);
    }
}

void initialize() {
    cls();
}

} // namespace vga

} // namespace drivers

