#pragma once

#include "HalTypes.hpp"
#include <cstdint>
#include <cstddef>

namespace modules::logger {

class IStorage {
public:
    virtual ~IStorage() = default;
    
    // Interface asynchrone pour l'écriture de données
    virtual hal::Result<void> write(const uint8_t* data, size_t size) noexcept = 0;
    
    // Synchronisation forcée (flush des buffers)
    virtual hal::Result<void> sync() noexcept = 0;
    
    virtual bool is_ready() const noexcept = 0;
};

} // namespace modules::logger
