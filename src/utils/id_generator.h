#include <iostream>
#include <cstdint>
#include <chrono>
#include <mutex>
#include <string>

namespace utils {
    class Snowflake {
    public:
        Snowflake(uint16_t datacenterId, uint16_t workerId)
                : datacenterId_(datacenterId), workerId_(workerId), sequence_(0), lastTimestamp_(0) {
            if (datacenterId_ > maxDatacenterId_ || workerId_ > maxWorkerId_) {
                throw std::invalid_argument("Invalid datacenter or worker ID");
            }
        }

        uint64_t generateUniqueId() {
            std::unique_lock<std::mutex> lock(mutex_);

            uint64_t timestamp = currentTimestamp();

            if (timestamp < lastTimestamp_) {
                throw std::runtime_error("Clock moved backwards");
            }

            if (timestamp == lastTimestamp_) {
                sequence_ = (sequence_ + 1) & sequenceMask_;
                if (sequence_ == 0) {
                    timestamp = waitNextMillis(lastTimestamp_);
                }
            } else {
                sequence_ = 0;
            }

            lastTimestamp_ = timestamp;

            return ((timestamp) << timestampLeftShift_) |
                   ((datacenterId_ & maxDatacenterId_) << datacenterIdShift_) |
                   ((workerId_ & maxWorkerId_) << workerIdShift_) |
                   (sequence_ & sequenceMask_);
        }

    private:
        uint64_t currentTimestamp() const {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        }

        uint64_t waitNextMillis(uint64_t lastTimestamp) {
            uint64_t timestamp = currentTimestamp();
            while (timestamp <= lastTimestamp) {
                timestamp = currentTimestamp();
            }
            return timestamp;
        }

        const uint64_t unixEpoch_ = 0ULL;  // Unix epoch start: 1970-01-01 00:00:00 UTC
        const uint64_t workerIdBits_ = 5;
        const uint64_t datacenterIdBits_ = 5;
        const uint64_t maxWorkerId_ = (1ULL << workerIdBits_) - 1;
        const uint64_t maxDatacenterId_ = (1ULL << datacenterIdBits_) - 1;
        const uint64_t sequenceBits_ = 12;
        const uint64_t workerIdShift_ = sequenceBits_;
        const uint64_t datacenterIdShift_ = sequenceBits_ + workerIdBits_;
        const uint64_t timestampLeftShift_ = sequenceBits_ + workerIdBits_ + datacenterIdBits_;
        const uint64_t sequenceMask_ = (1ULL << sequenceBits_) - 1;

        uint16_t datacenterId_;
        uint16_t workerId_;
        uint64_t sequence_;
        uint64_t lastTimestamp_;
        std::mutex mutex_;
    };

    class IdGenerator {
    public:
        static std::string GenerateId() {
            static Snowflake snowflake(1, 1); // Reuse the same instance of Snowflake
            return std::to_string(snowflake.generateUniqueId());
        }
    };
} // namespace utils
