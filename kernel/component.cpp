#include "component.hpp"

namespace kernel {

component::component(const char *name) : name_(name), syslog(name) {
}

} // namespace kernel

