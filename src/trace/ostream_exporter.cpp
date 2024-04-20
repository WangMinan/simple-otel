
#include "ostream_exporter.h"
#include <memory>
#include <string>
#include "span_exporter.h"
#include "span_metadata.h"
#include "span.h"
namespace trace
{
    std::string printTags(Span &span);
    std::string printStatus(StatusCode status);

    void OstreamSpanExporter::Export(Span &span)
    {
        sout << "{"
             << "\n  \"trace_id\": \"" << span.GetTraceId() << "\","
             << "\n  \"span_id\": \"" << span.GetId() << "\","
             << "\n  \"parent_span_id\": \"" << span.GetParentId() << "\","
             << "\n  \"name\": \"" << span.GetName() << "\","
             << "\n  \"service_name\": \"" << span.GetServiceName() << "\","
             << "\n  \"start_time\": " << span.GetStartTime() << ","
             << "\n  \"end_time\": " << span.GetEndTime() << ","
             << "\n  \"tags\": " << printTags(span) << ","
             << "\n  \"status\": \"" << printStatus(span.GetStatus()) << "\""
             << "\n }" << std::endl;
    }

    std::string printTags(Span &span)
    {
        std::string tags = "{";
        for (auto &tag : span.GetTags())
        {
            tags += "\"" + tag.first + "\": \"" + tag.second + "\",";
        }
        tags += "}";
        return tags;
    }

    std::string printStatus(StatusCode status)
    {
        switch (status)
        {
        case StatusCode::kUnset:
            return "Unset";
        case StatusCode::kOk:
            return "Ok";
        case StatusCode::kError:
            return "Error";
        default:
            return "Unknown";
        }
    }

    std::unique_ptr<SpanExporter> OstreamSpanExporter::Clone()
    {
        return std::make_unique<OstreamSpanExporter>();
    }
} // namespace trace
