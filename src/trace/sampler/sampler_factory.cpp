#include "sampler_factory.h"
#include "sampler/alway_on_sampler.h"
#include "sampler/head_variant_sampler.h"
#include "sampler/random_sampler.h"
#include "sampler/tail_sampler.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace trace {

std::unique_ptr<Sampler> trace::SamplerFactory::Create(
    SampleStrategy strategy,
    std::unordered_map<std::string, std::string> args) {
  switch (strategy) {
  case SampleStrategy::kAlwaysSample:
    return std::make_unique<AlwaysOnSampler>();
  case SampleStrategy::kRandomSample: {
    int rate = std::atoi(args["rate"].c_str());
    return std::make_unique<RandomSampler>(rate);
  }
  case SampleStrategy::kTailSample: {
    int rate = std::atoi(args["rate"].c_str());
    return std::make_unique<TailSampler>(rate);
  }
  case SampleStrategy::kHeadVariantSample: {
    int rate = std::atoi(args["rate"].c_str());
    int threshold = std::atoi(args["threshold"].c_str());
    return std::make_unique<HeadVariantSampler>(threshold, rate);
  }
  default:
    return std::make_unique<AlwaysOnSampler>();
  }
}
std::unique_ptr<Sampler> SamplerFactory::CreateFromParent() {
  SpanContext &ctx = Context::GetParentContext();
  if (!ctx.IsValid()) {
    return nullptr;
  }
  return Create(ctx.GetSampleStrategy(), ctx.GetAttributes());
}
} // namespace trace
