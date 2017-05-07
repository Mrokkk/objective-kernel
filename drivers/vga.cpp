#include <string.h>
#include <algorithm.h>
#include <kernel/cpu/io.hpp>
#include <kernel/memory/paging/paging.hpp>

#define blink               (1 << 7)

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

namespace {

constexpr uint8_t forecolor(color c) {
    return static_cast<uint8_t>(c) & 0xf;
}

constexpr uint8_t backcolor(color c) {
    return (static_cast<uint8_t>(c) << 4) & 0x7f;
}

uint16_t *pointer = reinterpret_cast<uint16_t *>(memory::virt_address(0xb8000u));
uint8_t default_attribute = forecolor(color::gray) | backcolor(color::black);
uint8_t csr_x = 0;
uint8_t csr_y = 0;

void video_mem_write(uint16_t data, uint16_t offset) {
    pointer[offset] = data;
}

void cursor_x_set(int x) {
    csr_x = x;
}

void cursor_y_set(int y) {
    csr_y = y;
}

void cursor_x_inc() {
    csr_x++;
}

void cursor_y_inc() {
    csr_y++;
}

void cursor_x_dec() {
    csr_x--;
}

int cursor_x_get() {
    return csr_x;
}

int cursor_y_get() {
    return csr_y;
}

constexpr uint16_t make_video_char(char c, char a) {
    return ((a << 8) | c);
}

uint16_t current_offset_get() {
    return cursor_y_get() * RESX + cursor_x_get();
}

} // namespace

void scroll(void) {
    auto blank = ' ' | (default_attribute << 8);
    if(cursor_y_get() >= RESY) {
        auto temp = static_cast<uint16_t>(cursor_y_get() - RESY + 1);
        utils::memcopy(reinterpret_cast<const char *>(pointer + temp * RESX),
            reinterpret_cast<char *>(pointer), (RESY - temp) * RESX * 2);
        utils::fill(pointer + (RESY - temp) * RESX, RESX, blank);
        cursor_y_set(RESY - 1);
    }
}

void move_csr(void) {
    auto off = current_offset_get();
    cpu::io::outb(14, 0x3D4);
    cpu::io::outb(static_cast<uint8_t>(off >> 8), 0x3D5);
    cpu::io::outb(15, 0x3D4);
    cpu::io::outb(static_cast<uint8_t>(off), 0x3D5);
}

void cls() {
    auto blank = ' ' | (default_attribute << 8);
    utils::fill(pointer, RESX * RESY, blank);
    cursor_x_set(0);
    cursor_y_set(0);
    move_csr();
}

void putch(unsigned char c) {
    if (c == '\b') {
        if(cursor_x_get() != 0) {
            cursor_x_dec();
            putch(' ');
            cursor_x_dec();
        }
    } else if (c == '\t') {
        cursor_x_set((cursor_x_get() + 8) & ~(8 - 1));
    }
    else if (c == '\r') {
        cursor_x_set(0);
    }
    else if (c == '\n') {
        cursor_x_set(0);
        cursor_y_inc();
    } else if (c >= ' ') {
        video_mem_write(make_video_char(c, default_attribute), current_offset_get());
        cursor_x_inc();
    }
    if (csr_x >= RESX) {
        cursor_x_set(0);
        cursor_y_inc();
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

