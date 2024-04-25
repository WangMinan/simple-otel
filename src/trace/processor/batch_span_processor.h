#include "span.h"
#include "span_processor.h"
#include <memory>
#include <vector>

namespace trace {

class BatchSpanProcessor : public SpanProcessor {
private:
  std::vector<SpanRecord> span_records;
  // when the number of spans reaches threshold, export them
  int threshold;

public:
  BatchSpanProcessor(std::unique_ptr<SpanExporter> exporter_, int threshold_)
      : SpanProcessor(std::move(exporter_)), threshold(threshold_){};
  ~BatchSpanProcessor();
  void OnStart(Span &span) override;
  void OnEnd(Span &span) override;
};

} // namespace trace
