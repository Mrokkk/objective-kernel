#pragma once

namespace cpu {

namespace irq {

void initialize();
void irq_enable(uint32_t irq);

} // namespace irq

} // namespace cpu

