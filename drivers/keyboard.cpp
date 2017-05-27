#include <kernel/cpu/io.hpp>
#include <kernel/cpu/irq.hpp>
#include <kernel/console/console.hpp>
#include "keyboard.hpp"

namespace drivers {

namespace keyboard {

#define CMD_PORT 0x64
#define DATA_PORT 0x60
#define STATUS_PORT 0x64

#define CMD_ENABLE_FIRST                0xae
#define CMD_DISABLE_FIRST               0xad
#define CMD_DISABLE_SECOND              0xa7
#define CMD_TEST_CONTROLLER             0xaa
#define CMD_TEST_FIRST_PS2_PORT         0xab
#define CMD_READ_CONFIGURATION_BYTE     0x20
#define CMD_WRITE_CONFIGURATION_BYTE    0x60

#define keyboard_disable() \
    do { keyboard_send_command(CMD_DISABLE_FIRST); keyboard_wait(); } while (0)

#define keyboard_enable() \
    keyboard_send_command(CMD_ENABLE_FIRST)

#define L_CTRL  0x1d
#define L_ALT   0x38
#define L_SHIFT 0x2a
#define R_SHIFT 0x36

//int shift = 0;
//char ctrl = 0;
//char alt = 0;

//[> Scancodes actions <]

//void s_shift_up() {
    //shift = 1;
//}

//void s_shift_down() {
    //shift = 0;
//}

//void s_ctrl_up() {
    //ctrl = 1;
//}

//void s_ctrl_down() {
    //ctrl = 0;
//}

//void s_alt_up() {
    //alt = 1;
//}

//void s_alt_down() {
    //alt =  0;
//}

//typedef void (*saction_t)();

#define scancode_action(code, action) \
    [code] = action##_up, \
    [code + 0x80] = action##_down,

//saction_t special_scancodes[] = {
        //[L_CTRL] = &s_ctrl_up,
        //[L_CTRL+0x80] = &s_ctrl_down,
        //[L_SHIFT] = &s_shift_up,
        //[L_SHIFT+0x80] = &s_shift_down,
        //[R_SHIFT] = &s_shift_up,
        //[R_SHIFT+0x80] = &s_shift_down,
        //[L_ALT] = &s_alt_up,
        //[L_ALT+0x80] = &s_alt_down
//};

const char *scancodes[] = {
        "\0\0", "\0\0",
        "1!", "2@", "3#", "4$", "5%", "6^", "7&", "8*", "9(", "0)", "-_", "=+", "\b\b",
        "\t\t", "qQ", "wW", "eE", "rR", "tT", "yY", "uU", "iI", "oO", "pP", "[{", "]}",
        "\n\n", "\0\0",
        "aA", "sS", "dD", "fF", "gG", "hH", "jJ", "kK", "lL", ";:", "'\"",
        "`~", "\0\0",
        "\\|",
        "zZ", "xX", "cC", "vV", "bB", "nN", "mM", ",<", ".>", "/?", "\0\0",
        "**",
        "\0\0", "  ", "\0\0"
};

namespace {

void keyboard_wait() {
    int i;
    for (i=0; i<10000; i++)
        if (!(cpu::io::inb(STATUS_PORT) & 0x2))
            return;
    //printk("Keyboard waiting timeout\n");
}

void keyboard_send_command(uint8_t byte) {
    keyboard_wait();
    cpu::io::outb(byte, CMD_PORT);
    cpu::io::io_wait();
}

uint8_t keyboard_receive() {
    while ((cpu::io::inb(STATUS_PORT) & 0x1) != 1);
    return cpu::io::inb(DATA_PORT);
}

void irs(uint32_t, cpu::stack_frame *) {
    auto scan_code = keyboard_receive();
    if (scan_code > 0x39) return;
    keyboard_disable();
    auto character = scancodes[scan_code][0];
    console::printf("%c", character);
    keyboard_enable();
}

} // namespace

void initialize() {
    uint8_t byte;
    keyboard_disable();
    keyboard_send_command(CMD_DISABLE_SECOND);
    while (cpu::io::inb(STATUS_PORT) & 1)
        cpu::io::inb(DATA_PORT);
    cpu::io::io_wait();
    keyboard_send_command(CMD_TEST_CONTROLLER);
    byte = keyboard_receive();
    if (byte != 0x55) return;

    keyboard_send_command(CMD_READ_CONFIGURATION_BYTE);
    byte = keyboard_receive();

    /* If translation is disabled, try to enable it */
    if (!(byte & (1 << 6))) {
        byte |= (1 << 6);
        keyboard_send_command(CMD_WRITE_CONFIGURATION_BYTE);
        cpu::io::outb(byte, DATA_PORT);
        keyboard_wait();
        keyboard_send_command(CMD_READ_CONFIGURATION_BYTE);
        if (!(keyboard_receive() & (1 << 6)))
            return;
    }
    /* Enable interrupt */
    if (!(byte & 0x1)) {
        byte |= 0x1;
        keyboard_send_command(CMD_WRITE_CONFIGURATION_BYTE);
        cpu::io::outb(byte, DATA_PORT);
        keyboard_wait();
    }

    keyboard_enable();

    cpu::irq::register_handler(0x01, &irs, "keyboard");
}

} // namespace keyboard

} // namespace drivers


