
#include <string>
#include <random>
#include <ctime>
namespace utils
{
    class IdGenerator
    {
    public:
        std::string static GenerateId(int length = 16)
        {
            const std::string chars =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            std::string random_string;
            std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // 随机数
            std::uniform_int_distribution<> distribution(0, chars.size() - 1);
            for (size_t i = 0; i < length; ++i)
            {
                random_string += chars[distribution(generator)];
            }
            return random_string; 
        }
    };

} // namespace utils
