
#include "message.h"
#include <sstream>
#include <string>
#include <unordered_map>

std::string serialize_headers(std::unordered_map<std::string, std::string> headers);

std::unordered_map<std::string, std::string> deserialize_headers(const std::string &serialized_headers);
namespace protocol
{
    Message::Message(std::initializer_list<std::pair<std::string, std::string>> headers)
    {
        for (auto header : headers)
        {
            this->headers[header.first] = header.second;
        }
    }

    Message::~Message() = default;

    std::string Message::Serialize()
    {
        std::ostringstream oss;
        std::string headers = serialize_headers(this->headers);
        oss << headers.length() << "\r\n";
        oss << headers << "\r\n";
        return oss.str();
    }

    Message Message::Deserialize(std::string message)
    {
        std::istringstream iss(message);
        // 读取头部长度
        std::string length_str;
        std::getline(iss, length_str, '\r');
        iss.ignore(1); // 忽略 "\r\n"
        int headers_length = std::stoi(length_str);
        // 读取头部字符串
        char headers_str[headers_length];
        iss.read(headers_str, headers_length);
        
        iss.ignore(2); // 忽略 "\r\n"
        // 反序列化头部
        auto headers = deserialize_headers(headers_str);
        // 重建 Message 对象
        Message msg(headers);
        return msg;
    }

    void Message::SetHeader(std::string key, std::string value) {
        this->headers[key] = value;
    }

    void Message::RemoveHeader(std::string key) {
        this->headers.erase(key);
    }

    std::string Message::GetHeader(std::string key) {
        return this->headers[key];
    }
} // namespace protocol

std::string serialize_headers(std::unordered_map<std::string, std::string> headers)
{
    std::ostringstream oss;
    for (const auto &header : headers)
    {
        oss << header.first << ":" << header.second << "\r\n";
    }
    return oss.str();
}
std::unordered_map<std::string, std::string> deserialize_headers(const std::string &serialized_headers)
{
    std::unordered_map<std::string, std::string> headers;
    std::istringstream iss(serialized_headers);
    std::string line;
    // 循环直到我们到达字符串的末尾
    while (std::getline(iss, line) && !line.empty())
    {
        std::size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            // 移除可能的回车符
            if (!value.empty() && value[value.length() - 1] == '\r')
            {
                value.erase(value.length() - 1);
            }
            headers[key] = value;
        }
    }
    return headers;
}