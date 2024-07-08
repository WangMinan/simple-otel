#ifndef TRACE_TRACE_H
#define TRACE_TRACE_H
#include "../utils/id_generator.h"
#include "processor/span_processor.h"
#include "span.h"
#include "trace_context.h"
#include <memory>
#include <string>
namespace trace {

class Trace {
private:
  // trace的id
  std::string trace_id;
  std::shared_ptr<TraceContext> context;
  std::string service_name;

public:
  Trace(std::shared_ptr<TraceContext> context_, std::string service_name_)
      : context(context_), service_name(service_name_) {
    trace_id = utils::IdGenerator::GenerateId();
  }
  Trace(std::string trace_id_, std::shared_ptr<TraceContext> context_,
        std::string service_name_)
      : trace_id(trace_id_), context(context_), service_name(service_name_){};
  virtual ~Trace(){};
  virtual std::shared_ptr<Span> StartSpan(std::string name);
  std::string GetId() { return this->trace_id; }
  std::shared_ptr<TraceContext> Context() { return this->context; }
};

} // namespace trace
#endif