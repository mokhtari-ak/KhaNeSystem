#pragma once

#include "BusMessages.hpp"
#include "HalTypes.hpp"

namespace fusion {

// Concept pour l'estimateur d'état (Encapsulation EKF/Madgwick)
template<typename T>
concept IStateEstimator = requires(T t, const bus::SensorFrame& frame) {
    { t.update_attitude(frame) } -> std::same_as<hal::Result<void>>;
    { t.get_state() } -> std::same_as<bus::StateVector>;
    { t.is_healthy() } -> std::same_as<bool>;
};

// Implémentation concrète de la fusion (EKF wrapper)
class SensorFusion {
public:
    SensorFusion() = default;

    hal::Result<void> update_attitude(const bus::SensorFrame& frame) noexcept {
        // Logique de fusion (Madgwick/EKF via KalmanFilterLibrary)
        // Vérification innovation -> is_healthy_ = false si divergence
        return {};
    }

    bus::StateVector get_state() const noexcept {
        return state_;
    }

    bool is_healthy() const noexcept {
        return is_healthy_;
    }

private:
    bus::StateVector state_{};
    bool is_healthy_ = true;
};

static_assert(IStateEstimator<SensorFusion>, "SensorFusion must satisfy IStateEstimator concept");

} // namespace fusion
