#pragma once

#include "kalman/ExtendedKalmanFilter.hpp"
#include "BusMessages.hpp"
#include "Eigen/Core"
#include <array>

namespace modules::sensor_fusion {

// Vecteur d'état : Position(3), Vitesse(3), Attitude(4), Biais Gyro(3), Biais Accel(2) = 15 états
using StateVector = Eigen::Matrix<float, 15, 1>;

struct DroneState {
    Eigen::Vector3f position;
    Eigen::Vector3f velocity;
    Eigen::Quaternionf attitude;
    Eigen::Vector3f gyro_bias;
    Eigen::Vector3f accel_bias;
};

// Modèle de prédiction (IMU)
class DroneSystemModel : public Kalman::LinearizedSystemModel<StateVector, Eigen::Matrix<float, 6, 1>> { // 6 = Accel + Gyro
public:
    // f(x, u) : x est l'état précédent, u est la commande (accel + gyro)
    StateVector f(const StateVector& x, const Eigen::Matrix<float, 6, 1>& u) const override {
        StateVector x_new = x;
        float dt = 0.004f; // 250Hz

        // u : [accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z]
        Eigen::Vector3f accel = u.segment<3>(0) - x.segment<3>(13); // Accel - AccelBias
        Eigen::Vector3f gyro  = u.segment<3>(3) - x.segment<3>(10); // Gyro - GyroBias

        // Position = Position + Vitesse * dt
        x_new.segment<3>(0) += x.segment<3>(3) * dt;

        // Vitesse = Vitesse + (Rot(Attitude) * Accel + Gravity) * dt
        Eigen::Quaternionf q(x[6], x[7], x[8], x[9]);
        Eigen::Vector3f gravity(0.0f, 0.0f, 9.81f);
        x_new.segment<3>(3) += (q * accel + gravity) * dt;

        // Attitude : mise à jour avec quaternion
        Eigen::Quaternionf q_dot(0.0f, gyro.x(), gyro.y(), gyro.z());
        q_dot.coeffs() *= 0.5f;
        q_dot = q * q_dot; // Rotation
        x_new.segment<4>(6) = (q.coeffs() + q_dot.coeffs() * dt).normalized();

        return x_new;
    }

    void updateJacobians(const StateVector& x, const Eigen::Matrix<float, 6, 1>& u) override {
        // Calculer F (Jacobienne par rapport à x) et W (par rapport au bruit)
        // F.setIdentity(); // exemple
        // W.setIdentity(); // exemple
    }
};

// Wrapper EKF encapsulant la logique de fusion
class EkfWrapper {
public:
    EkfWrapper() = default;

    void update(const bus::SensorFrame& frame) {
        // Prédiction basée sur l'IMU (fréquence élevée)
        // update(imu_measurement);
    }

    void update(const bus::GnssFrame& frame) {
        // Correction basée sur le GNSS
        // update(gnss_measurement);
    }

    bus::StateVector get_state() const {
        bus::StateVector sv;
        // Conversion de l'état EKF interne vers StateVector (bus)
        return sv;
    }

private:
    Kalman::ExtendedKalmanFilter<StateVector> ekf_;
    DroneSystemModel system_model_;
};

} // namespace modules::sensor_fusion
