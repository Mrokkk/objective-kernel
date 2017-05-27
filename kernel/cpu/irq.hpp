#pragma once

namespace cpu {

namespace irq {

struct irq {

    using handler = void(*)(uint32_t);

    handler handler_;
    const char *name_;

};

void initialize();
void enable(uint32_t irq);
void register_handler(uint32_t number, irq::handler handler, const char *name);

} // namespace irq

} // namespace cpu

