#pragma once

#include "HalTypes.hpp"
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <span>

namespace hal {

/**
 * @brief Interface abstraite pour les systèmes de fichiers (LittleFS/FatFS).
 * Garantit l'absence d'allocation dynamique et l'utilisation de Result<T, E>.
 */
class IFileSystem {
public:
    virtual ~IFileSystem() = default;

    virtual Result<void> mount() noexcept = 0;
    virtual Result<void> unmount() noexcept = 0;

    // Lecture d'un fichier entier dans un buffer fourni
    virtual Result<size_t> read(std::string_view path, std::span<uint8_t> buffer) noexcept = 0;
    
    // Écriture d'un buffer dans un fichier
    virtual Result<size_t> write(std::string_view path, std::span<const uint8_t> buffer) noexcept = 0;

    // Vérification de l'existence
    virtual bool exists(std::string_view path) const noexcept = 0;
};

} // namespace hal
