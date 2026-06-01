#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"

namespace hal {

class IWatchdog {
public:
    virtual ~IWatchdog() = default;
    virtual Result<void> refresh() noexcept = 0;
};

class WatchdogDriver : public IWatchdog {
public:
    explicit WatchdogDriver(IWDG_HandleTypeDef* hiwdg) : hiwdg_(hiwdg) {}

    Result<void> refresh() noexcept override {
        if (HAL_IWDG_Refresh(hiwdg_) == HAL_OK) {
            return {};
        }
        return std::unexpected(HalError::Error);
    }

private:
    IWDG_HandleTypeDef* hiwdg_;
};

} // namespace hal
