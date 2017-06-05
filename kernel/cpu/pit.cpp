#include "io.hpp"
#include "pit.hpp"
#include "pic.hpp"
#include "irq.hpp"

#define HZ 100

#define CLOCK_TICK_RATE 1193182
#define LATCH ((CLOCK_TICK_RATE) / HZ)

namespace cpu {

using namespace io;

namespace pit {

void initialize() {
    outb(PIT_CHANNEL0 | PIT_MODE_2 | PIT_ACCES_LOHI | PIT_16BIN,
            PIT_PORT_COMMAND);
    outb(LATCH & 0xff, PIT_PORT_CHANNEL0);
    outb(LATCH >> 8, PIT_PORT_CHANNEL0);
    pic::enable(0);
}

} // namespace pit

} // namespace cpu

