#include <string.h>
#include <algorithm.h>
#include <kernel/cpu/io.hpp>
#include <kernel/memory/paging.hpp>

#define VIDEO_SEGMENT (0xb8000 + KERNEL_PAGE_OFFSET)

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_GRAY          7
#define COLOR_DARKGRAY      8
#define COLOR_BRIGHTBLUE    9
#define COLOR_BRIGHTGREEN   10
#define COLOR_BRIGHTCYAN    11
#define COLOR_BRIGHT_RED    12
#define COLOR_BRIGHTMAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

#define forecolor(x)    x
#define backcolor(x)    ((x << 4) & 0x7f)
#define blink           (1 << 7)

#define RESX 80
#define RESY 25

namespace drivers {

namespace vga {

static unsigned short *pointer[4] = {
        (unsigned short *)0xb8000,
        (unsigned short *)(0xb8000 + 1 * 4096),
        (unsigned short *)(0xb8000 + 2 * 4096),
        (unsigned short *)(0xb8000 + 3 * 4096)
};

static unsigned char default_attribute = forecolor(COLOR_GRAY) | backcolor(COLOR_BLACK);
static unsigned char csr_x[4] = {0, 0, 0, 0};
static unsigned char csr_y[4] = {0, 0, 0, 0};

static unsigned char current_page = 0;

static inline void video_mem_write(unsigned short data, unsigned short offset) {
    pointer[current_page][offset] = data;
}

static inline void cursor_x_set(int x) {
    csr_x[current_page] = x;
}

static inline void cursor_y_set(int y) {
    csr_y[current_page] = y;
}

static inline void cursor_x_inc() {
    csr_x[current_page]++;
}

static inline void cursor_y_inc() {
    csr_y[current_page]++;
}

static inline void cursor_x_dec() {
    csr_x[current_page]--;
}

static inline int cursor_x_get() {
    return csr_x[current_page];
}

static inline int cursor_y_get() {
    return csr_y[current_page];
}

static inline unsigned short make_video_char(char c, char a) {
    return ((a << 8) | c);
}

static inline unsigned short current_offset_get() {
    return cursor_y_get() * RESX + cursor_x_get();
}


void scroll(void) {

    unsigned short blank;
    unsigned short temp;

    blank = (unsigned short) (0x20 | (default_attribute << 8));

    if(cursor_y_get() >= RESY) {
        temp = (unsigned short) (cursor_y_get() - RESY + 1);
        utils::memcopy(reinterpret_cast<const char *>(pointer[current_page] + temp * RESX),
            reinterpret_cast<char *>(pointer[current_page]), (RESY - temp) * RESX * 2);
        utils::fill(pointer[current_page] + (RESY - temp) * RESX, RESX, blank);
        cursor_y_set(RESY - 1);
    }

}

void move_csr(void) {

    unsigned short off = current_offset_get();

    cpu::io::outb(14, 0x3D4);
    cpu::io::outb((unsigned char) (off >> 8), 0x3D5);
    cpu::io::outb(15, 0x3D4);
    cpu::io::outb((unsigned char) off, 0x3D5);

}

void cls() {

    unsigned short blank = (unsigned short) (0x20 | (default_attribute << 8));

    utils::fill(pointer[current_page], RESX * RESY, blank);
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
    } else if (c == '\t')
        cursor_x_set((cursor_x_get() + 8) & ~(8 - 1));
    else if (c == '\r')
        cursor_x_set(0);
    else if (c == '\n') {
        cursor_x_set(0);
        cursor_y_inc();
    } else if (c >= ' ') {
        video_mem_write(make_video_char(c, default_attribute), current_offset_get());
        cursor_x_inc();
    }

    if (csr_x[current_page] >= RESX) {
        cursor_x_set(0);
        cursor_y_inc();
    }

    scroll();
    move_csr();

}

void print(const char *text) {

    while (*text)
        putch(*text++);

}

int display_write(struct inode *, struct file *, const char *buffer, unsigned int size) {

    while (size--)
        putch(*buffer++);

    return size;

}

int initialize() {
    pointer[0] = (unsigned short*)VIDEO_SEGMENT;
    cls();
    return 0;
}

} // namespace vga

} // namespace drivers

