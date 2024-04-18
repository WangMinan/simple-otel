#include "span.h"
#include "../utils/id_generator.h"
#include "../utils/timestamp_generator.h"
#include <memory>

namespace trace
{
    Span::Span(std::string name, std::string service_name, std::string trace_id, std::string parent_id, std::shared_ptr<SpanProcessor> processor_)
    {
        this->name = name;
        this->service_name = service_name;
        this->trace_id = trace_id;
        this->parent_id = parent_id;
        this->id = utils::IdGenerator::GenerateId();
        this->start_time = utils::TimestampGenerator::Now();
        this->end_time = 0;
        this->status = StatusCode::kUnset;
        this->processor = processor_;
        this->processor->OnStart(*this);
    }

    void Span::SetTag(std::string key, std::string value)
    {
        this->tags[key] = value;
    }

    void Span::End()
    {
        this->has_ended_ = true;
        this->end_time = utils::TimestampGenerator::Now();
        this->processor->OnEnd(*this);
    }

    Span::~Span()
    {
        if (!this->has_ended_)
        {
            this->End();
        }
    }

    void Span::SetStatus(StatusCode status)
    {
        this->status = status;
    }

}