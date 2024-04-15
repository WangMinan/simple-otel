
#include <string>
namespace trace
{
    class Endpoint
    {
    private:
        std::string service_name;
        std::string ip;
        int port;
        int latency;

    public:
        Endpoint(std::string service_name_, std::string ip_, int port_, int latency_) : service_name(service_name_), ip(ip_), port(port_), latency(latency_){};
        ~Endpoint() = default;
    };

} // namespace trace
