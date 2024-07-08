#include "post_sampler.h"
#include "span_context.h"
#include "trace_metadata.h"
#include <cstdlib>
#include <ctime>

#ifndef TRACE_TAIL_SAMPLER_H
#define TRACE_TAIL_SAMPLER_H

namespace trace {
class TailSampler : public PostSampler {
private:
  int rate;

public:
  TailSampler(int rate_) : rate(rate_){};
  ~TailSampler() = default;
  SampleResult
  PostSample(std::vector<std::shared_ptr<RespContext>> &resp_contexts,
             SpanContext &context) override {

    if (context.GetTraceFlag() == TraceFlag::kIsSampled) {
      return SampleResult(true, SampleStrategy::kTailSample,
                          TraceFlag::kIsSampled);
    } else if (context.GetTraceFlag() == TraceFlag::kIsDiscarded) {
      return SampleResult(false, SampleStrategy::kTailSample,
                          TraceFlag::kIsDiscarded);
    }

    TraceFlag trace_flag;
    // 链路尾部，开启概率采样
    if (resp_contexts.empty()) {
      
      srand(time(NULL));
      int res = rand() % 100;
      return SampleResult(res < rate, SampleStrategy::kTailSample,
                          res < rate ? TraceFlag::kIsSampled
                                     : TraceFlag::kIsDiscarded);
    }
    for (const auto ctx : resp_contexts) {
      if (ctx->GetTraceFlag() == TraceFlag::kIsSampled) {
        return SampleResult(true, SampleStrategy::kTailSample,
                            TraceFlag::kIsSampled);
      }
    }
    return SampleResult(false, SampleStrategy::kTailSample,
                        TraceFlag::kIsDiscarded);
  }

  SampleResult ShouldSampled(SpanContext &context) override {
    return SampleResult(true, SampleStrategy::kTailSample,
                        TraceFlag::kIsWaiting);
  }

  std::unique_ptr<Sampler> Clone() override {
    return common::make_unique<TailSampler>(this->rate);
  }

  // TODO: Serialize
  std::string Serialize() override {
    return std::to_string(static_cast<int>(SampleStrategy::kTailSample));
  }

  SampleStrategy GetSampleStrategy() override {
    return SampleStrategy::kTailSample;
  }

  std::unordered_map<std::string, std::string> GetAttributes() override {
    return {{"rate", std::to_string(rate)}};
  }
};

} // namespace trace
#endif