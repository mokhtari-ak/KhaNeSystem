#pragma once

#include <cstdint>
#include "ISpiDriver.hpp"

namespace hal {

	struct HalSpiDriver : public hal::ISpiDriver {

		static constexpr int8_t MaxSpiHandles = 3;
		
		inline static SPI_HandleTypeDef spiHandles[MaxSpiHandles] = {};
		inline static int8_t handleCount = 0;
		inline static void (*tx_complete_callbacks[MaxSpiHandles])() = {nullptr};
		inline static void (*rx_complete_callbacks[MaxSpiHandles])() = {nullptr};
		inline static void (*txrx_complete_callbacks[MaxSpiHandles])() = {nullptr};
		inline static void (*error_callbacks[MaxSpiHandles])() = {nullptr};

		template<SpiConfigPolicy T>
		int8_t init() {
			if (handleCount >= MaxSpiHandles) return -1;
			enable_clock(T::Port);

			SPI_HandleTypeDef m_handle = { };
			m_handle.Instance = MapPort(T::Port);
			m_handle.Init = getHALConfig<T>();

			HAL_StatusTypeDef status = HAL_SPI_Init(&m_handle);

			if (status == HAL_OK) {
				spiHandles[handleCount] = m_handle;
				return handleCount++;
			}
			return -1;
		}

		static void attach_callbacks(int8_t handle_index,
			void (*tx_cb)(),
			void (*rx_cb)(),
			void (*txrx_cb)(),
			void (*error_cb)()) {
			if (handle_index >= 0 && handle_index < handleCount) {
				tx_complete_callbacks[handle_index] = tx_cb;
				rx_complete_callbacks[handle_index] = rx_cb;
				txrx_complete_callbacks[handle_index] = txrx_cb;
				error_callbacks[handle_index] = error_cb;
			}
		}

		static void handle_tx_complete(SPI_HandleTypeDef *hspi) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(spiHandles[i].Instance == hspi->Instance && tx_complete_callbacks[i]) {
					tx_complete_callbacks[i]();
					break;
				}
			}
		}
		static void handle_rx_complete(SPI_HandleTypeDef *hspi) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(spiHandles[i].Instance == hspi->Instance && rx_complete_callbacks[i]) {
					rx_complete_callbacks[i]();
					break;
				}
			}
		}
		static void handle_txrx_complete(SPI_HandleTypeDef *hspi) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(spiHandles[i].Instance == hspi->Instance && txrx_complete_callbacks[i]) {
					txrx_complete_callbacks[i]();
					break;
				}
			}
		}
		static void handle_error(SPI_HandleTypeDef *hspi) {
			for(int8_t i = 0; i < handleCount; ++i) {
				if(spiHandles[i].Instance == hspi->Instance && error_callbacks[i]) {
					error_callbacks[i]();
					break;
				}
			}
		}

		static void activate_IRQ(SpiPort port, uint32_t preempt, uint32_t sub) {
			IRQn_Type irq;
			switch (port) {
			case SpiPort::SPI_1: irq = SPI1_IRQn; break;
			case SpiPort::SPI_2: irq = SPI2_IRQn; break;
			case SpiPort::SPI_3: irq = SPI3_IRQn; break;
			}
			HAL_NVIC_SetPriority(irq, preempt, sub);
			HAL_NVIC_EnableIRQ(irq);
		}

		static void enable_clock(SpiPort port) {
			if (is_enabled(port)) return;
			switch (port) {
				// SPI1 est sur APB2
			case SpiPort::SPI_1: __HAL_RCC_SPI1_CLK_ENABLE(); break;
				// SPI2 et SPI3 sont sur APB1
			case SpiPort::SPI_2: __HAL_RCC_SPI2_CLK_ENABLE(); break;
			case SpiPort::SPI_3: __HAL_RCC_SPI3_CLK_ENABLE(); break;
			}
		}

		static bool is_enabled(SpiPort port) {
			switch (port) {
			case SpiPort::SPI_1: return RCC->APB2ENR & RCC_APB2ENR_SPI1EN;
			case SpiPort::SPI_2: return RCC->APB1ENR & RCC_APB1ENR_SPI2EN;
			case SpiPort::SPI_3: return RCC->APB1ENR & RCC_APB1ENR_SPI3EN;
			}
			return false;
		}

		// --- Fonctions de mappage statiques ---
		template<SpiConfigPolicy config>
			static constexpr SPI_InitTypeDef getHALConfig() {
				SPI_InitTypeDef spiConfig {}
				;
				spiConfig.Mode = MapMode(config::Mode);
				spiConfig.Direction = MapDirection(config::Direction);
				spiConfig.DataSize = MapDataSize(config::DataSize);
				spiConfig.CLKPolarity = MapClockPolarity(config::ClockPolarity);
				spiConfig.CLKPhase = MapClockPhase(config::ClockPhase);
				spiConfig.NSS = MapNssMode(config::Nss);
				spiConfig.BaudRatePrescaler = MapPrescaler(config::Prescaler);
				spiConfig.FirstBit = MapFirstBit(config::FirstBit);
				spiConfig.TIMode = SPI_TIMODE_DISABLE;
				spiConfig.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
				spiConfig.CRCPolynomial = 10;
				return spiConfig;
			}

		static constexpr SPI_TypeDef* MapPort(SpiPort port) {
			switch (port) {
			case SpiPort::SPI_1: return SPI1;
			case SpiPort::SPI_2: return SPI2;
			case SpiPort::SPI_3: return SPI3;
			}
			return nullptr;
		}
		static constexpr uint32_t MapMode(SpiMode mode) {
			return (mode == SpiMode::Master) ? SPI_MODE_MASTER : SPI_MODE_SLAVE;
		}
		static constexpr uint32_t MapDirection(SpiDirection dir) {
			if (dir == SpiDirection::HalfDuplex) return SPI_DIRECTION_1LINE;
			if (dir == SpiDirection::RxOnly) return SPI_DIRECTION_2LINES_RXONLY;
			return SPI_DIRECTION_2LINES;
		}
		static constexpr uint32_t MapDataSize(SpiDataSize size) {
			return (size == SpiDataSize::Data16Bit) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;
		}
		static constexpr uint32_t MapClockPolarity(SpiClockPolarity cpol) {
			return (cpol == SpiClockPolarity::High) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
		}
		static constexpr uint32_t MapClockPhase(SpiClockPhase cpha) {
			return (cpha == SpiClockPhase::Edge2) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
		}
		static constexpr uint32_t MapNssMode(SpiNssMode nss) {
			if (nss == SpiNssMode::HardwareMaster) return SPI_NSS_HARD_OUTPUT;
			if (nss == SpiNssMode::HardwareSlave) return SPI_NSS_HARD_INPUT;
			return SPI_NSS_SOFT;
		}
		static constexpr uint32_t MapPrescaler(SpiBaudRatePrescaler psc) {
			switch (psc) {
			case SpiBaudRatePrescaler::Prescaler2:   return SPI_BAUDRATEPRESCALER_2;
			case SpiBaudRatePrescaler::Prescaler4:   return SPI_BAUDRATEPRESCALER_4;
			case SpiBaudRatePrescaler::Prescaler8:   return SPI_BAUDRATEPRESCALER_8;
			case SpiBaudRatePrescaler::Prescaler16:  return SPI_BAUDRATEPRESCALER_16;
			case SpiBaudRatePrescaler::Prescaler32:  return SPI_BAUDRATEPRESCALER_32;
			case SpiBaudRatePrescaler::Prescaler64:  return SPI_BAUDRATEPRESCALER_64;
			case SpiBaudRatePrescaler::Prescaler128: return SPI_BAUDRATEPRESCALER_128;
			case SpiBaudRatePrescaler::Prescaler256: return SPI_BAUDRATEPRESCALER_256;
			}
			return SPI_BAUDRATEPRESCALER_16;
		}
		static constexpr uint32_t MapFirstBit(SpiFirstBit fb) {
			return (fb == SpiFirstBit::LSBFirst) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
		}
	};

} // namespace Hal
