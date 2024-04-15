
#include <string>
namespace trace
{
    class SpanContext
    {
    private:
        std::string trace_id;
        std::string span_id;

    public:
        SpanContext() = default;
        SpanContext(std::string trace_id_, std::string span_id_) : trace_id(trace_id_), span_id(span_id_){};
        ~SpanContext() = default;
        bool IsValid();
        std::string TraceId()
        {
            return this->trace_id;
        }
        std::string SpanId()
        {
            return this->span_id;
        }
    };

} // namespace trace
