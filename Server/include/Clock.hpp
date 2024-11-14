#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>

inline uint64_t getMonotonicTime() {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}
