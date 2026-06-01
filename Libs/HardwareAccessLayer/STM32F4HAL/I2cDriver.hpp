#pragma once

#include <cstdint>
#include "II2cDriver.hpp"
#include "stm32f4xx_hal.h"
#include "I2cConfigPolicy.hpp"

using namespace WrapperBase;

namespace hal {

	struct HalI2cDriver : public hal::II2cDriver {

		static constexpr int8_t MaxI2cHandles = 3;
		
		inline static I2C_HandleTypeDef i2cHandles[MaxI2cHandles] = {};
		inline static int8_t handleCount = 0;
		inline static void (*tx_complete_callbacks[MaxI2cHandles])() = {nullptr};
		inline static void (*rx_complete_callbacks[MaxI2cHandles])() = {nullptr};
		inline static void (*error_callbacks[MaxI2cHandles])() = {nullptr};
		
		template<I2cConfigPolicy T>
		int8_t init() {
			if (handleCount >= MaxI2cHandles) return -1;
			enable_clock(T::Port);

			I2C_HandleTypeDef m_handle = { };
			m_handle.Instance = MapPort(T::Port);
			m_handle.Init = getHALConfig<T>();

			HAL_StatusTypeDef status = HAL_I2C_Init(&m_handle);

			if (status == HAL_OK) {
				i2cHandles[handleCount] = m_handle;
				return handleCount++;
			}
			return -1;
		}

		static void attach_callbacks(int8_t handle_index, void (*tx_cb)(), void (*rx_cb)(), void (*error_cb)()) {
			if (handle_index >= 0 && handle_index < handleCount) {
				tx_complete_callbacks[handle_index] = tx_cb;
				rx_complete_callbacks[handle_index] = rx_cb;
				error_callbacks[handle_index] = error_cb;
			}
		}

		static void handle_tx_complete(I2C_HandleTypeDef *hi2c) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(i2cHandles[i].Instance == hi2c->Instance && tx_complete_callbacks[i]) {
					tx_complete_callbacks[i]();
					break;
				}
			}
		}
		static void handle_rx_complete(I2C_HandleTypeDef *hi2c) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(i2cHandles[i].Instance == hi2c->Instance && rx_complete_callbacks[i]) {
					rx_complete_callbacks[i]();
					break;
				}
			}
		}
		static void handle_error(I2C_HandleTypeDef *hi2c) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(i2cHandles[i].Instance == hi2c->Instance && error_callbacks[i]) {
					error_callbacks[i]();
					break;
				}
			}
		}

		static void activate_IRQ(I2cPort port, uint32_t preempt, uint32_t sub) {
			IRQn_Type ev_irq, er_irq;
			switch (port) {
			case I2cPort::I2C_1: ev_irq = I2C1_EV_IRQn; er_irq = I2C1_ER_IRQn; break;
			case I2cPort::I2C_2: ev_irq = I2C2_EV_IRQn; er_irq = I2C2_ER_IRQn; break;
			case I2cPort::I2C_3: ev_irq = I2C3_EV_IRQn; er_irq = I2C3_ER_IRQn; break;
			}
			HAL_NVIC_SetPriority(ev_irq, preempt, sub);
			HAL_NVIC_EnableIRQ(ev_irq);
			HAL_NVIC_SetPriority(er_irq, preempt, sub);
			HAL_NVIC_EnableIRQ(er_irq);
		}

		static void enable_clock(I2cPort port) {
			if (is_enabled(port)) return;
			switch (port) {
			case I2cPort::I2C_1: __HAL_RCC_I2C1_CLK_ENABLE(); break;
			case I2cPort::I2C_2: __HAL_RCC_I2C2_CLK_ENABLE(); break;
			case I2cPort::I2C_3: __HAL_RCC_I2C3_CLK_ENABLE(); break;
			}
		}

		static bool is_enabled(I2cPort port) {
			switch (port) {
			case I2cPort::I2C_1: return RCC->APB1ENR & RCC_APB1ENR_I2C1EN;
			case I2cPort::I2C_2: return RCC->APB1ENR & RCC_APB1ENR_I2C2EN;
			case I2cPort::I2C_3: return RCC->APB1ENR & RCC_APB1ENR_I2C3EN;
			}
			return false;
		}

		// --- Fonctions de mappage statiques ---

		static constexpr I2C_TypeDef* MapPort(I2cPort port) {
			switch (port) {
			case I2cPort::I2C_1: return I2C1;
			case I2cPort::I2C_2: return I2C2;
			case I2cPort::I2C_3: return I2C3;
			}
			return nullptr;
		}

		static constexpr uint32_t MapAddressingMode(I2cAddressingMode mode) {
			return (mode == I2cAddressingMode::Mode10Bit) ? I2C_ADDRESSINGMODE_10BIT : I2C_ADDRESSINGMODE_7BIT;
		}

		static constexpr uint32_t MapDutyCycle(I2cDutyCycle cycle) {
			return (cycle == I2cDutyCycle::Cycle16_9) ? I2C_DUTYCYCLE_16_9 : I2C_DUTYCYCLE_2;
		}

		static DriverStatus From_hal(HAL_StatusTypeDef s) {
			switch(s) {
				case HAL_OK:      return DriverStatus::Ok;
				case HAL_BUSY:    return DriverStatus::Busy;
				case HAL_TIMEOUT: return DriverStatus::Timeout;
				default:          return DriverStatus::Error;
			}
		}

		template<I2cConfigPolicy config>
			static constexpr I2C_InitTypeDef getHALConfig() {
				I2C_InitTypeDef i2cConfig {}
				;
				i2cConfig.ClockSpeed = config::ClockSpeed;
				i2cConfig.DutyCycle = MapDutyCycle(config::DutyCycle);
				i2cConfig.OwnAddress1 = config::OwnAddress1;
				i2cConfig.AddressingMode = MapAddressingMode(config::AddressingMode);
				i2cConfig.DualAddressMode = config::DualAddressMode ? I2C_DUALADDRESS_ENABLE : I2C_DUALADDRESS_DISABLE;
				i2cConfig.OwnAddress2 = config::OwnAddress2;
				i2cConfig.GeneralCallMode = config::GeneralCallMode ? I2C_GENERALCALL_ENABLE : I2C_GENERALCALL_DISABLE;
				i2cConfig.NoStretchMode = config::NoStretchMode ? I2C_NOSTRETCH_ENABLE : I2C_NOSTRETCH_DISABLE;
				return i2cConfig;
			}
	};

} // namespace Hal
