#pragma once

#include "IStorage.hpp"
#include "IFileSystem.hpp"
#include <array>
#include <mutex>

namespace modules::logger {

/**
 * @brief Implémentation du Logger haute performance.
 * Utilise un buffer circulaire et un mutex pour la thread-safety.
 */
class Logger : public IStorage {
public:
    explicit Logger(hal::IFileSystem& fs, size_t buffer_size = 4096) 
        : fs_(fs), buffer_size_(buffer_size), head_(0), tail_(0) {
        buffer_.fill(0);
    }

    hal::Result<void> write(const uint8_t* data, size_t size) noexcept override {
        std::lock_guard<std::mutex> lock(mutex_);
        // Copie des données dans le buffer circulaire
        for (size_t i = 0; i < size; ++i) {
            buffer_[head_] = data[i];
            head_ = (head_ + 1) % buffer_size_;
            if (head_ == tail_) {
                tail_ = (tail_ + 1) % buffer_size_;
            }
        }
        return {};
    }

    hal::Result<void> sync() noexcept override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (head_ == tail_) return {};

        size_t size_to_write = (head_ > tail_) ? (head_ - tail_) : (buffer_size_ - tail_);
        
        auto res = fs_.write("logs.txt", {&buffer_[tail_], size_to_write});
        if (res) {
            tail_ = (tail_ + *res) % buffer_size_;
        }
        return res.transform([](size_t) {});
    }

    bool is_ready() const noexcept override {
        return fs_.exists("logs.txt");
    }

private:
    hal::IFileSystem& fs_;
    static constexpr size_t BUFFER_SIZE = 4096;
    std::array<uint8_t, BUFFER_SIZE> buffer_;
    size_t buffer_size_;
    size_t head_;
    size_t tail_;
    std::mutex mutex_; // Protection ajoutée
};

} // namespace modules::logger
