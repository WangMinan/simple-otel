#include <chrono>
namespace utils
{
    class TimestampGenerator
    {
    public:
        long static Now()
        {
            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());
            return ms.count();
        }
    };

} // namespace utils
