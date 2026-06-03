#pragma once
#include <cstdint>
#include <variant>
#include <optional>
#include <utility>

namespace hal {

// Définition de base pour les résultats d'erreurs HAL
enum class HalError {
    None = 0,
    Timeout,
    Busy,
    Error,
    InvalidParameter
};

template <typename E>
struct Unexpected {
    E value;
    explicit constexpr Unexpected(E e) : value(e) {}
};

template <typename T, typename E>
class Expected {
public:
    constexpr Expected(T val) : data_(std::move(val)) {}
    constexpr Expected(Unexpected<E> err) : data_(std::move(err)) {}

    constexpr bool has_value() const noexcept { return std::holds_alternative<T>(data_); }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr const T& operator*() const& { return std::get<T>(data_); }
    constexpr T& operator*() & { return std::get<T>(data_); }
    constexpr const T* operator->() const { return &std::get<T>(data_); }
    constexpr T* operator->() { return &std::get<T>(data_); }

    constexpr const E& error() const { return std::get<Unexpected<E>>(data_).value; }

private:
    std::variant<T, Unexpected<E>> data_;
};

// Spécialisation pour Expected<void, E>
template <typename E>
class Expected<void, E> {
public:
    constexpr Expected() : error_(std::nullopt) {}
    constexpr Expected(Unexpected<E> err) : error_(err.value) {}

    constexpr bool has_value() const noexcept { return !error_.has_value(); }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr const E& error() const { return *error_; }

private:
    std::optional<E> error_;
};

template <typename E>
constexpr Unexpected<E> unexpected(E e) {
    return Unexpected<E>(e);
}

// Utilisation d'un alias pour le pattern Result sans exceptions
template <typename T>
using Result = Expected<T, HalError>;

// Concept de temps (Microsecondes)
struct Microseconds {
    uint64_t count = 0;
    constexpr Microseconds() = default;
    explicit constexpr Microseconds(uint64_t us) : count(us) {}
};

// Concept de temps (Millisecondes)
struct Milliseconds {
    uint64_t count = 0;
    constexpr Milliseconds() = default;
    explicit constexpr Milliseconds(uint64_t ms) : count(ms) {}
};

} // namespace hal
