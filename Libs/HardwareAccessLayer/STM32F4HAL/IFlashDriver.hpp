#pragma once

#include "HalTypes.hpp"
#include <cstddef>
#include <cstdint>

namespace hal {

class IFlashDriver {
public:
    virtual ~IFlashDriver() = default;
    virtual Result<void> erase_sector(uint32_t sector) noexcept = 0;
    virtual Result<void> write_word(uint32_t address, uint32_t data) noexcept = 0;
    virtual Result<void> unlock() noexcept = 0;
    virtual Result<void> lock() noexcept = 0;
};

class ICrcDriver {
public:
    virtual ~ICrcDriver() = default;
    virtual uint32_t calculate(const void* data, size_t size) noexcept = 0;
};

} // namespace hal
