#ifndef TRACE_TRACE_SAMPLER_H
#define TRACE_TRACE_SAMPLER_H
#include <memory>
#include <string>
namespace trace
{
    class SpanContext;
    class Sampler
    {
    public:
        virtual bool ShouldSampled(SpanContext &context) = 0;
        virtual ~Sampler() = default;
        virtual std::unique_ptr<Sampler> Clone() = 0;
        virtual std::string Serialize() = 0;
    };

    enum class SampleStrategy
    {
        kAlwaysSample = 0,
        kNeverSample = 1,
    };

} // namespace trace

#endif // !TRACE_TRACE_SAMPLER_H
