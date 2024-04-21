#ifndef TRACE_SPAN_CONTEXT_H
#define TRACE_SPAN_CONTEXT_H
#include "../protocol/message.h"
#include "sampler.h"
#include "sampler/alway_on_sampler.h"
#include "trace_metadata.h"
#include <memory>
#include <string>
#include <vector>
namespace trace {
class SpanContext {
private:
  std::string trace_id;
  std::string span_id;
  TraceFlag trace_flag;
  std::shared_ptr<Sampler> sampler;

public:
  friend class Context;
  SpanContext() = default;
  SpanContext(
      std::string trace_id_, std::string span_id_,
      TraceFlag trace_flag_ = kIsSampled,
      std::shared_ptr<Sampler> &&sampler_ = std::make_shared<AlwaysOnSampler>())
      : trace_id(trace_id_), span_id(span_id_), trace_flag(trace_flag_),
        sampler(sampler_){};
  ~SpanContext() = default;
  SpanContext(SpanContext &&other) = default;
  SpanContext &operator=(SpanContext &&other) = default;
  bool IsValid();
  std::string GetTraceId() { return this->trace_id; };
  std::string GetSpanId() { return this->span_id; }
  TraceFlag GetTraceFlag() { return this->trace_flag; }
  std::shared_ptr<Sampler> GetSampler() { return this->sampler; }
};

/// @brief thread local span context
class Context {
private:
  thread_local static SpanContext parent_context;
  thread_local static std::vector<SpanContext> current_contexts;

public:
  /// @brief get parent context from message
  /// @param message
  void static Extract(protocol::Message message);

  /// @brief get parent context
  /// @return parent context
  static SpanContext &GetParentContext();

  /// @brief get current context
  /// @return current context
  static SpanContext &GetCurrentContext();

  /// @brief change trace flag in current context
  /// @param trace_flag_
  static void SetTraceFlag(TraceFlag trace_flag_);

  /// @brief write current context to the message for transferring
  /// @param message
  static void WriteToMessage(protocol::Message &message);

  /// @brief attach a new span context. its scope is limited to the new span
  /// @param trace_id
  /// @param span_id
  /// @param trace_flag
  /// @param sampler
  static void Attach(std::string trace_id, std::string span_id,
                     TraceFlag trace_flag, std::shared_ptr<Sampler> &&sampler);

  /// @brief detach the current span context
  static void Detach();
};

} // namespace trace

#endif // !TRACE_SPAN_CONTEXT_H
