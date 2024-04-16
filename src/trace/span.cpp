#include "span.h"
#include "../utils/id_generator.h"
#include "../utils/timestamp_generator.h"

namespace trace
{
    Span::Span(std::string name, std::string service_name, std::string trace_id, std::string parent_id)
    {
        this->name = name;
        this->service_name = service_name;
        this->trace_id = trace_id;
        this->parent_id = parent_id;
        this->id = utils::IdGenerator::GenerateId();
        this->start_time = utils::TimestampGenerator::Now();
        this->end_time = 0;
        this->status = StatusCode::kUnset;
    }

    void Span::SetTag(std::string key, std::string value)
    {
        this->tags[key] = value;
    }

    void Span::End()
    {
        this->has_ended_ = true;
        this->end_time = utils::TimestampGenerator::Now();
    }

    Span::~Span() {
        if (!this->has_ended_) {
            this->End();
        }
    }

    void Span::SetStatus(StatusCode status) {
        this->status = status;
    }

}