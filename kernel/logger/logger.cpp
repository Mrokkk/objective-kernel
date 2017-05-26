#include "logger.hpp"

logger logger::instance_;
utils::spinlock logger::spinlock_;

