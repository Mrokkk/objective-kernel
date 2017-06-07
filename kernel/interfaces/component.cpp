#include "component.hpp"

namespace interfaces {

component::component(const char *name) : name_(name), syslog(name) {
}

} // namespace kernel

