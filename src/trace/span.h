#include <memory>
#include <string>
#include <unordered_map>
#include "span_metadata.h"
#include "span_processor.h"
#include "trace_context.h"
#ifndef TRACE_SPAN_H
#define TRACE_SPAN_H
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
        std::shared_ptr<TraceContext> trace_context;

    protected:
        Span() = default;

    public:
        Span(std::string name, std::string service_name, std::string trace_id, std::string parent_id, std::shared_ptr<TraceContext> trace_context_);
        ~Span();
        void SetTag(std::string key, std::string value);
        void End();
        void SetStatus(StatusCode status);
        std::string Id() { return this->id; };
        std::string TraceId() { return this->trace_id; };
        std::string ParentId() { return this->parent_id; };
        std::string Name() { return this->name; };
        std::string ServiceName() { return this->service_name; };
        long StartTime() { return this->start_time; };
        long EndTime() { return this->end_time; };
        std::unordered_map<std::string, std::string> Tags() { return this->tags; };
        StatusCode Status() { return this->status; };
    };

    class NoopSpan : public Span
    {
    public:
        NoopSpan(){};
    };

}; // namespace trace

#endif