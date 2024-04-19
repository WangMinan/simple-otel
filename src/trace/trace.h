#ifndef TRACE_TRACE_H
#define TRACE_TRACE_H
#include "../utils/id_generator.h"
#include <memory>
#include <string>
#include "span.h"
#include "span_processor.h"
#include "trace_context.h"
namespace trace
{

    class Trace
    {
    private:
        // trace的id
        std::string trace_id;
        std::shared_ptr<TraceContext> context;

    public:
        Trace(std::shared_ptr<TraceContext> context_) : context(context_)
        {
            trace_id = utils::IdGenerator::GenerateId();
        }
        Trace(std::string trace_id_, std::shared_ptr<TraceContext> context_) : trace_id(trace_id_), context(context_){};
        virtual ~Trace(){};
        virtual std::shared_ptr<Span> StartSpan(std::string name, std::string service_name);
        std::string Id() { return this->trace_id; }
        std::shared_ptr<TraceContext> Context() { return this->context; }
    };

} // namespace trace
#endif