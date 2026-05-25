#pragma once
#include "HalTypes.hpp"
#include <span>

namespace hal {

template<typename T>
concept ISpiWrapper = requires(T t, std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds timeout) {
    { t.transmit_receive(tx, rx, timeout) } -> std::same_as<Result<void>>;
};

template<typename T>
concept II2cWrapper = requires(T t, uint16_t dev_addr, std::span<uint8_t> data, Microseconds timeout) {
    { t.master_transmit(dev_addr, data, timeout) } -> std::same_as<Result<void>>;
    { t.master_receive(dev_addr, data, timeout) } -> std::same_as<Result<void>>;
};

template<typename T>
concept IUartWrapper = requires(T t, std::span<uint8_t> data, Microseconds timeout) {
    { t.transmit(data, timeout) } -> std::same_as<Result<void>>;
    { t.receive(data, timeout) } -> std::same_as<Result<void>>;
};

} // namespace hal
