#include "sampler.h"
#include <memory>
#include <string>

namespace trace
{
    class AlwaysOnSampler : public Sampler
    {

    public:
        AlwaysOnSampler() = default;
        ~AlwaysOnSampler() = default;
        bool ShouldSampled(SpanContext &context) override;
        std::unique_ptr<Sampler> Clone() override;
        std::string Serialize() override;
    };

    inline bool AlwaysOnSampler::ShouldSampled(SpanContext &context)
    {
        return true;
    }

    inline std::unique_ptr<Sampler> AlwaysOnSampler::Clone()
    {
        return std::make_unique<AlwaysOnSampler>();
    }

    inline std::string AlwaysOnSampler::Serialize()
    {
        return std::to_string(static_cast<int>(SampleStrategy::kAlwaysSample));
    }
} // namespace trace
