#include "sampler/post_sampler.h"
#include "span_processor.h"
#include <memory>

#ifndef TRACE_POST_SAMPLE_PROCESSOR_H
#define TRACE_POST_SAMPLE_PROCESSOR_H

namespace trace {
class PostSampleProcessor : public SpanProcessor {
private:
public:
  PostSampleProcessor(std::unique_ptr<SpanExporter> &&exporter_)
      : SpanProcessor(std::move(exporter_)){};
  ~PostSampleProcessor() = default;
  void OnStart(Span &span) override;
  void OnEnd(Span &span) override;
};

} // namespace trace
#endif // !TRACE_POST_SAMPLE_PROCESSOR_H
