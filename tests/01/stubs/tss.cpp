#include <kernel/cpu/tss.hpp>

namespace cpu {

tss::tss() {
}

tss::tss(void *) {
}

void tss::load() const {
}

void context_switch(tss &, tss &) {
}

} // namespace cpu

