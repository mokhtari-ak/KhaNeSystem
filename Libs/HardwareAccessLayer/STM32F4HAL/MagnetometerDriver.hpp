#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include <Eigen/Core>

namespace hal {

struct MagData {
    Eigen::Vector3f field;
    Microseconds timestamp;
    bool is_valid;
};

template<typename Transport>
class MagnetometerDriver {
public:
    explicit MagnetometerDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        return {};
    }

    Result<MagData> update() noexcept {
        return MagData{
            .field = Eigen::Vector3f(0.0f, 0.0f, 0.0f),
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

} // namespace hal
