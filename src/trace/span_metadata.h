#ifndef TRACE_SPAN_METADATA_H
#define TRACE_SPAN_METADATA_H

namespace trace
{

    enum class StatusCode
    {
        // default
        kUnset = 0,
        // operation has completed successfully
        kOk = 1,
        // operation encountered an error
        kError = 2
    };
} // namespace trace
#endif // !TRACE_SPAN_METADATA_H
