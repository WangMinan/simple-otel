#include "span_context.h"
#include "sampler.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace trace {

thread_local SpanContext Context::parent_context;
thread_local std::vector<SpanContext> Context::current_contexts;
thread_local std::vector<std::shared_ptr<Span>> Context::active_spans;
thread_local std::vector<std::vector<std::shared_ptr<RespContext>>>
    Context::resp_contexts;
thread_local std::unique_ptr<RespContext> Context::return_context;

bool SpanContext::IsValid() { return !trace_id.empty() && !span_id.empty(); }

void Context::Extract(protocol::Message message) {
  static const std::vector<std::string> reserved_keys = {
      "trace_id", "span_id", "trace_flag", "sample_strategy"};
  auto trace_id = message.GetHeader("trace_id");
  auto span_id = message.GetHeader("span_id");
  auto trace_flag = message.GetHeader("trace_flag");
  auto sample_strategy =
      std::atoi(message.GetHeader("sample_strategy").c_str());
  std::unordered_map<std::string, std::string> attributes_;
  for (auto &[key, value] : message.GetHeaders()) {
    if (std::find(reserved_keys.begin(), reserved_keys.end(), key) !=
        reserved_keys.end()) {
      continue;
    }
    attributes_.emplace(key, value);
  }
  if (trace_id.empty()) {

    parent_context = SpanContext(trace_id, span_id, attributes_);
  } else {
    parent_context = SpanContext(trace_id, span_id, attributes_,
                                 TraceFlagHandler::GetTraceFlag(trace_flag),
                                 static_cast<SampleStrategy>(sample_strategy));
  }
}

SpanContext &Context::GetParentContext() {
  if (current_contexts.empty() || current_contexts.size() == 1) {
    return parent_context;
  } else {
    return current_contexts[current_contexts.size() - 2];
  }
}

SpanContext &Context::GetCurrentContext() {
  if (current_contexts.empty()) {
    return parent_context;
  }
  return current_contexts.back();
}

void Context::SetTraceFlag(TraceFlag trace_flag_) {
  SpanContext &current_context = GetCurrentContext();
  current_context.trace_flag = trace_flag_;
}

void Context::WriteToMessage(protocol::Message &message) {
  SpanContext &current_context = GetCurrentContext();
  message.SetHeader("trace_id", current_context.trace_id);
  message.SetHeader("span_id", current_context.span_id);
  message.SetHeader("trace_flag", TraceFlagHandler::Serialize(
                                      current_context.GetTraceFlag()));
  message.SetHeader("sample_strategy", std::to_string(static_cast<int>(
                                           current_context.sample_strategy)));
  for (auto &[key, value] : current_context.attributes) {
    message.SetHeader(key, value);
  }
}

void Context::Attach(std::string trace_id, std::string span_id,
                     std::unordered_map<std::string, std::string> attributes,
                     TraceFlag trace_flag, SampleStrategy sample_strategy) {
  current_contexts.emplace_back(trace_id, span_id, attributes, trace_flag,
                                sample_strategy);
}

void Context::Detach() {
  if (current_contexts.empty()) {
    return;
  }
  current_contexts.erase(current_contexts.end() - 1);
  active_spans.erase(active_spans.end() - 1);
  resp_contexts.erase(resp_contexts.end() - 1);
}

void Context::AddActiveSpan(std::shared_ptr<Span> span) {
  active_spans.push_back(span);
  resp_contexts.emplace_back(std::vector<std::shared_ptr<RespContext>>());
}

void Context::RemoveLatestActiveSpan() {
  active_spans.erase(active_spans.end() - 1);
}

std::shared_ptr<Span> Context::GetCurrentSpan() {
  if (active_spans.empty()) {
    return nullptr;
  }
  return active_spans.back();
}
std::vector<std::shared_ptr<RespContext>> *Context::GetCurrentRespContext() {
  if (resp_contexts.empty()) {
    return nullptr;
  }
  return &resp_contexts.back();
}

std::vector<std::shared_ptr<RespContext>> *Context::GetParentRespContext() {
  if (resp_contexts.size() <= 1) {
    return nullptr;
  }
  return &resp_contexts[resp_contexts.size() - 2];
}

void Context::AddRespContext(std::shared_ptr<RespContext> context) {
  auto current = GetCurrentRespContext();
  current->push_back(context);
}

void Context::SetReturnContext(TraceFlag trace_flag_) {
  return_context = std::make_unique<RespContext>(trace_flag_);
}

RespContext &Context::GetReturnContext() { return *return_context; }

void Context::AddRespContext(TraceFlag trace_flag) {
  auto resp_context = std::make_shared<RespContext>(trace_flag);
  auto current = GetCurrentRespContext();
  current->push_back(resp_context);
}

} // namespace trace