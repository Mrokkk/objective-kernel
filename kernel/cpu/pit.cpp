#include "io.hpp"
#include "pit.hpp"
#include "pic.hpp"

#define PIT_PORT_CHANNEL0 0x40
#define PIT_PORT_CHANNEL1 0x41
#define PIT_PORT_CHANNEL2 0x42
#define PIT_PORT_COMMAND  0x43

#define PIT_16BIN         0
#define PIT_BCD           1

#define PIT_MODE_0  (0 << 1)
#define PIT_MODE_1  (1 << 1)
#define PIT_MODE_2  (2 << 1)
#define PIT_MODE_3  (3 << 1)
#define PIT_MODE_4  (4 << 1)
#define PIT_MODE_5  (5 << 1)
#define PIT_MODE_2b (6 << 1)
#define PIT_MODE_3b (7 << 1)

#define PIT_LATCH_CVAL  (0 << 4)
#define PIT_ACCESS_LO   (1 << 4)
#define PIT_ACCESS_HI   (2 << 4)
#define PIT_ACCES_LOHI  (3 << 4)

#define PIT_CHANNEL0    (0 << 6)
#define PIT_CHANNEL1    (1 << 6)
#define PIT_CHANNEL2    (2 << 6)
#define PIT_READ_BACK   (3 << 6)

#define CLOCK_TICK_RATE 1193182
#define LATCH ((CLOCK_TICK_RATE) / HZ)

namespace cpu {

using namespace io;

namespace pit {

void initialize() {
    outb(PIT_CHANNEL0 | PIT_MODE_2 | PIT_ACCES_LOHI | PIT_16BIN, PIT_PORT_COMMAND);
    outb(LATCH & 0xff, PIT_PORT_CHANNEL0);
    outb(LATCH >> 8, PIT_PORT_CHANNEL0);
    pic::enable(0);
}

} // namespace pit

} // namespace cpu

