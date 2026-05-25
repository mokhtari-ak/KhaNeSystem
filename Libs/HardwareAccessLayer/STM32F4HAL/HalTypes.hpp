#pragma once
#include <expected>
#include <cstdint>

namespace hal {

// Définition de base pour les résultats d'erreurs HAL
enum class HalError {
    None = 0,
    Timeout,
    Busy,
    Error,
    InvalidParameter
};

// Utilisation d'un alias pour le pattern Result sans exceptions
template <typename T>
using Result = std::expected<T, HalError>;

// Concept de temps (Microsecondes)
struct Microseconds {
    uint64_t count;
    explicit constexpr Microseconds(uint64_t us) : count(us) {}
};

} // namespace hal
