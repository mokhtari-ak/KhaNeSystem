#pragma once

#include "ISimulationBridge.hpp"
#include <vector>

namespace hal {

class MockSimulationBridge : public ISimulationBridge {
public:
    Result<void> send(std::span<const uint8_t> data) noexcept override {
        sent_data_.insert(sent_data_.end(), data.begin(), data.end());
        return {};
    }

    Result<size_t> receive(std::span<uint8_t> buffer) noexcept override {
        size_t to_copy = std::min(buffer.size(), received_data_.size());
        std::copy(received_data_.begin(), received_data_.begin() + to_copy, buffer.begin());
        return to_copy;
    }

    bool is_connected() const noexcept override {
        return connected_;
    }

    void set_connected(bool connected) { connected_ = connected; }
    void set_received_data(const std::vector<uint8_t>& data) { received_data_ = data; }
    const std::vector<uint8_t>& get_sent_data() const { return sent_data_; }

private:
    bool connected_ = true;
    std::vector<uint8_t> sent_data_;
    std::vector<uint8_t> received_data_;
};

} // namespace hal
