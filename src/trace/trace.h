#ifndef TRACE_TRACE_H
#define TRACE_TRACE_H
#include "../utils/id_generator.h"
#include <memory>
#include <string>
#include "span.h"
#include "span_processor.h"
namespace trace
{

    class Trace
    {
    private:
        // trace的id
        std::string trace_id;
        std::shared_ptr<SpanProcessor> processor;

    public:
        Trace(std::shared_ptr<SpanProcessor> processor_) : processor(processor_)
        {
            trace_id = utils::IdGenerator::GenerateId();
        }
        Trace(std::string trace_id_, std::shared_ptr<SpanProcessor> processor_) : trace_id(trace_id_), processor(processor_){};
        virtual ~Trace(){};
        virtual std::shared_ptr<Span> StartSpan(std::string name, std::string service_name);
        std::string Id() { return this->trace_id; }
    };

    class NoopTrace : public Trace
    {
    public:
        NoopTrace() : Trace(nullptr){};
        ~NoopTrace() = default;
        std::shared_ptr<Span> StartSpan(std::string name, std::string service_name) override;
    };

} // namespace trace
#endif