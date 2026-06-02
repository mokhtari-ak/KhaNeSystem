#pragma once

#include "IFileSystem.hpp"
#include <string_view>

namespace hal {

/**
 * @brief Implémentation Mock pour les tests unitaires/SIL.
 */
class MockFileSystem : public IFileSystem {
public:
    Result<void> mount() noexcept override { return {}; }
    Result<void> unmount() noexcept override { return {}; }

    Result<size_t> read(std::string_view path, std::span<uint8_t> buffer) noexcept override {
        return 0; // Mock : rien lu
    }

    Result<size_t> write(std::string_view path, std::span<const uint8_t> buffer) noexcept override {
        return buffer.size(); // Mock : écrit tout
    }

    bool exists(std::string_view path) const noexcept override { return false; }
};

} // namespace hal
