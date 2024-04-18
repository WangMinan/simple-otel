
#ifndef TRACE_TRACE_PROVIDER_H
#define TRACE_TRACE_PROVIDER_H
#include "span_processor.h"
#include "trace.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
namespace trace
{
    class TraceProvider
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<Trace>> traces;
        std::shared_ptr<SpanProcessor> processor;
        static TraceProvider provider;
        static std::mutex lock;
        TraceProvider(std::shared_ptr<SpanProcessor> processor_) : processor(processor_){};
        TraceProvider() = default;

    public:
        ~TraceProvider() = default;
        static TraceProvider &GetInstance();
        static std::shared_ptr<Trace> StartTrace();
        static std::shared_ptr<Trace> GetTrace();
        static std::shared_ptr<Trace> GetTrace(std::string trace_id);
        static void SetSpanProcessor(std::shared_ptr<SpanProcessor> processor);
    };

} // namespace trace
#endif // !TRACE_TRACE_PROVIDER_H
