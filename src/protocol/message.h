#ifndef PROTOCOL_MESSAGE_H
#define PROTOCOL_MESSAGE_H

#include <initializer_list>
#include <string>
#include <unordered_map>
namespace protocol
{
    class Message
    {
    private:
        // int body_length;
        std::unordered_map<std::string, std::string> headers;
        // T body;

    public:
        Message() = default;
        Message(std::unordered_map<std::string, std::string> headers_) : headers(headers_){};
        Message(std::initializer_list<std::pair<std::string, std::string>>);
        ~Message();
        std::string Serialize();
        Message static Deserialize(std::string message);
        void SetHeader(std::string key, std::string value);
        void RemoveHeader(std::string key);
        std::string GetHeader(std::string key);
    };

} // namespace protocol
#endif // !PROTOCOL_MESSAGE_H