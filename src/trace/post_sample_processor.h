#include "sampler/post_sampler.h"
#include "span_processor.h"
#include <memory>

#ifndef TRACE_POST_SAMPLE_PROCESSOR_H
#define TRACE_POST_SAMPLE_PROCESSOR_H

namespace trace {
class PostSampleProcessor : public SpanProcessor {
private:
  std::unique_ptr<PostSampler> sampler;

public:
  PostSampleProcessor(std::unique_ptr<SpanExporter> &&exporter_,
                      std::unique_ptr<PostSampler> &&sampler_)
      : SpanProcessor(std::move(exporter_)), sampler(std::move(sampler_)){};
  ~PostSampleProcessor() = default;
  void OnStart(Span &span) override;
  void OnEnd(Span &span) override;
  std::unique_ptr<SpanProcessor> Clone() override;
};

} // namespace trace
#endif // !TRACE_POST_SAMPLE_PROCESSOR_H
