
#include <string>
#include <unordered_map>
#include "endpoint.h"
#include "span_metadata.h"
namespace trace
{
    class Span
    {
    private:
        std::string id;
        std::string name;
        std::string service_name;
        std::string trace_id;
        // 父span id
        std::string parent_id;
        // span发送方的地址
        // Endpoint local_endpoint;
        // span下家的地址
        // Endpoint remote_endpoint;
        long start_time;
        long end_time;
        std::unordered_map<std::string, std::string> tags;
        StatusCode status;
        bool has_ended_;

    public:
        Span(std::string name, std::string service_name, std::string trace_id, std::string parent_id);
        ~Span();
        void SetTag(std::string key, std::string value);
        void End();
        void SetStatus(StatusCode status);
        std::string Id();
    };

} // namespace trace
