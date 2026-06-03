#pragma once

#include "IFileSystem.hpp"
#include "HalTypes.hpp"
#include <concepts>
#include <cstdint>
#include <cstring>
#include <span>

namespace cfg {

struct alignas(4) ConfigParams {
    uint32_t sequence;
    float pid_roll_p;
    float pid_roll_i;
    float pid_pitch_p;
    float pid_pitch_i;
    float battery_warning_v;
    uint32_t checksum;
};

class ConfigurationManager {
public:
    explicit ConfigurationManager(hal::IFileSystem& fs) : fs_(fs) {}

    hal::Result<ConfigParams> load() noexcept {
        ConfigParams paramsA, paramsB;
        uint8_t bufferA[sizeof(ConfigParams)], bufferB[sizeof(ConfigParams)];
        
        auto resA = fs_.read("config_a.bin", bufferA);
        auto resB = fs_.read("config_b.bin", bufferB);
        
        if (!resA || *resA != sizeof(ConfigParams)) return hal::unexpected(hal::HalError::Error);
        if (!resB || *resB != sizeof(ConfigParams)) return hal::unexpected(hal::HalError::Error);

        std::memcpy(&paramsA, bufferA, sizeof(ConfigParams));
        std::memcpy(&paramsB, bufferB, sizeof(ConfigParams));
        
        // Validation basique CRC (à remplacer par vrai CRC)
        bool validA = (paramsA.checksum == 0xDEADBEEF); // Placeholder CRC
        bool validB = (paramsB.checksum == 0xDEADBEEF); // Placeholder CRC
        
        if (validA && validB) return (paramsA.sequence > paramsB.sequence) ? paramsA : paramsB;
        if (validA) return paramsA;
        if (validB) return paramsB;
        
        return hal::unexpected(hal::HalError::Error);
    }

    hal::Result<void> save(const ConfigParams& params) noexcept {
        // Logique de basculement A/B et écriture via fs_.write
        return {};
    }

private:
    hal::IFileSystem& fs_;
};

} // namespace cfg
