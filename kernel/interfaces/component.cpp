#include "component.hpp"

namespace interfaces {

component::component(const char* name)
        : syslog(name)
        , name_(name) {
}

} // namespace kernel

