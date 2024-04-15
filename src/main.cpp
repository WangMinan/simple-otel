#include <iostream>
#include "trace/trace.h"

int main(int argc, char const *argv[])
{
    trace::Trace trace_;
    trace::SpanContext span_contxt;
    auto span = trace_.StartSpan("root", "main", span_contxt);

    span->SetStatus(trace::StatusCode::kOk);
    span->End();
}