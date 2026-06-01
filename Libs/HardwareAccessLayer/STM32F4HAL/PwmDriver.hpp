#pragma once

#include "IPwmDriver.hpp"
#include "PwmEnumsStructs.hpp"
#include "PwmConfigPolicy.hpp"
#include "stm32f4xx_hal.h"
#include <cassert>
#include <array>

namespace hal {

	struct HalPwmDriver : public hal::IPwmDriver {

		static constexpr int8_t MaxTimers = 12;
		inline static TIM_HandleTypeDef m_handles[MaxTimers] = {};
		inline static bool is_initialized[MaxTimers] = {false};

		static int8_t GetTimerIndex(TIM_TypeDef* instance) {
			if (instance == TIM1) return 0;
			if (instance == TIM2) return 1;
			if (instance == TIM3) return 2;
			if (instance == TIM4) return 3;
			if (instance == TIM5) return 4;
			if (instance == TIM8) return 5;
			if (instance == TIM9) return 6;
			if (instance == TIM10) return 7;
			if (instance == TIM11) return 8;
			if (instance == TIM12) return 9;
			if (instance == TIM13) return 10;
			if (instance == TIM14) return 11;
			return -1;
		}

		template <PwmConfigPolicy T>
		void init() override {
			TIM_TypeDef* instance = MapTimerInstance(T::Timer);
			int8_t idx = GetTimerIndex(instance);
			if (idx == -1) assert("Timer not supported");

			if (!is_initialized[idx]) {
				EnableClock(T::Timer);
                
				m_handles[idx].Instance = instance;
				m_handles[idx].Init.Prescaler = T::Prescaler;
				m_handles[idx].Init.Period = T::Period;
				m_handles[idx].Init.CounterMode = TIM_COUNTERMODE_UP;
				m_handles[idx].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
				m_handles[idx].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
                
				if (HAL_TIM_PWM_Init(&m_handles[idx]) != HAL_OK) {
					assert("HAL_TIM_PWM_Init failed");
				}
				is_initialized[idx] = true;
			}

			TIM_OC_InitTypeDef oc_config = { };
			oc_config.OCMode = MapMode(T::Mode);
			oc_config.Pulse = 0;
			oc_config.OCPolarity = MapPolarity(T::Polarity);
			oc_config.OCFastMode = TIM_OCFAST_DISABLE;

			if (HAL_TIM_PWM_ConfigChannel(&m_handles[idx], &oc_config, MapTimerChannel(T::Channel)) != HAL_OK) {
				assert("HAL_TIM_PWM_ConfigChannel failed");
			}
		}

		void start(PwmTimerInstance timer, PwmTimerChannel channel) override {
			int8_t idx = GetTimerIndex(MapTimerInstance(timer));
			HAL_TIM_PWM_Start(&m_handles[idx], MapTimerChannel(channel));
		}

		void stop(PwmTimerInstance timer, PwmTimerChannel channel) override {
			int8_t idx = GetTimerIndex(MapTimerInstance(timer));
			HAL_TIM_PWM_Stop(&m_handles[idx], MapTimerChannel(channel));
		}

		void setDutyCycle(PwmTimerInstance timer, PwmTimerChannel channel, uint32_t pulse) override {
			int8_t idx = GetTimerIndex(MapTimerInstance(timer));
			__HAL_TIM_SET_COMPARE(&m_handles[idx], MapTimerChannel(channel), pulse);
		}

		void setPeriod(PwmTimerInstance timer, uint32_t period) override {
			int8_t idx = GetTimerIndex(MapTimerInstance(timer));
			__HAL_TIM_SET_AUTORELOAD(&m_handles[idx], period);
		}
        
		void setPrescaler(PwmTimerInstance timer, uint32_t prescaler) override {
			int8_t idx = GetTimerIndex(MapTimerInstance(timer));
			__HAL_TIM_SET_PRESCALER(&m_handles[idx], prescaler);
		}

		// --- Implémentation des Mappers ---

		static TIM_TypeDef* MapTimerInstance(PwmTimerInstance timer) {
			switch (timer) {
			case PwmTimerInstance::TIM1:  return TIM1;
			case PwmTimerInstance::TIM2:  return TIM2;
			case PwmTimerInstance::TIM3:  return TIM3;
			case PwmTimerInstance::TIM4:  return TIM4;
			case PwmTimerInstance::TIM5:  return TIM5;
			case PwmTimerInstance::TIM8:  return TIM8;
			case PwmTimerInstance::TIM9:  return TIM9;
			case PwmTimerInstance::TIM10: return TIM10;
			case PwmTimerInstance::TIM11: return TIM11;
			case PwmTimerInstance::TIM12: return TIM12;
			case PwmTimerInstance::TIM13: return TIM13;
			case PwmTimerInstance::TIM14: return TIM14;
			}
			assert("Timer instance not supported");
			return nullptr;
		}

		static uint32_t MapTimerChannel(PwmTimerChannel channel) {
			switch (channel) {
			case PwmTimerChannel::Channel1: return TIM_CHANNEL_1;
			case PwmTimerChannel::Channel2: return TIM_CHANNEL_2;
			case PwmTimerChannel::Channel3: return TIM_CHANNEL_3;
			case PwmTimerChannel::Channel4: return TIM_CHANNEL_4;
			}
			return TIM_CHANNEL_ALL; // Fallback
		}

		static uint32_t MapPolarity(PwmPolarity polarity) {
			return (polarity == PwmPolarity::High) ? TIM_OCPOLARITY_HIGH : TIM_OCPOLARITY_LOW;
		}
        
		static uint32_t MapMode(PwmMode mode) {
			return (mode == PwmMode::PWM1) ? TIM_OCMODE_PWM1 : TIM_OCMODE_PWM2;
		}

		static void EnableClock(PwmTimerInstance timer) {
			// Note: TIM1, TIM8-TIM11 sont sur APB2. Les autres sur APB1.
			switch (timer) {
			case PwmTimerInstance::TIM1:  __HAL_RCC_TIM1_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM2:  __HAL_RCC_TIM2_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM3:  __HAL_RCC_TIM3_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM4:  __HAL_RCC_TIM4_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM5:  __HAL_RCC_TIM5_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM8:  __HAL_RCC_TIM8_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM9:  __HAL_RCC_TIM9_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM10: __HAL_RCC_TIM10_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM11: __HAL_RCC_TIM11_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM12: __HAL_RCC_TIM12_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM13: __HAL_RCC_TIM13_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM14: __HAL_RCC_TIM14_CLK_ENABLE(); break;
			default: assert("Timer clock not supported");
			}
		}
	};

} // namespace Hal