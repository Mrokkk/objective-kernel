#include "component.hpp"

namespace interfaces {

component::component(const char* name)
        : log_(name)
        , name_(name) {
}

} // namespace kernel

