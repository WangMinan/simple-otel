#include "post_sampler.h"
#include "trace_metadata.h"

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
  PostSample(std::vector<std::shared_ptr<RespContext>> &contexts) override {

    TraceFlag trace_flag;
    // 链路尾部，开启概率采样
    if (contexts.empty()) {

      srand(time(NULL));
      int res = rand() % 100;
      return SampleResult(res < rate, SampleStrategy::kTailSample,
                          res < rate ? TraceFlag::kIsSampled
                                     : TraceFlag::kIsDiscarded);
    }
    for (const auto ctx : contexts) {
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
    return std::make_unique<TailSampler>(rate);
  }

  // TODO: Serialize
  std::string Serialize() override {
    return std::to_string(static_cast<int>(SampleStrategy::kTailSample));
  }
};

} // namespace trace
#endif