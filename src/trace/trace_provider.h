
#ifndef TRACE_TRACE_PROVIDER_H
#define TRACE_TRACE_PROVIDER_H
#include "trace.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace trace
{
    class TraceProvider
    {
    private:
        std::unordered_map<std::string, trace::Trace> traces;
        ~TraceProvider() = default;
        TraceProvider() = default;

    public:
        static TraceProvider &GetInstance();
        static std::shared_ptr<Trace> GetTrace();
        static std::shared_ptr<Trace> GetTrace(std::string trace_id);
    };

} // namespace trace
#endif // !TRACE_TRACE_PROVIDER_H
