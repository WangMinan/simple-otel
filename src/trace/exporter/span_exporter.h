
#ifndef TRACE_SPAN_EXPORTER_H
#define TRACE_SPAN_EXPORTER_H

#include <initializer_list>
#include <stdexcept>
#include <vector>
namespace trace {
class SpanRecord;
class SpanExporter {
public:
  virtual ~SpanExporter() = default;
  virtual void Export(SpanRecord &record) = 0;
  virtual void Export(std::vector<SpanRecord> &records) {
    throw std::runtime_error("not implemented");
  };
};

};     // namespace trace
#endif // !TRACE_SPAN_EXPORTER_H
