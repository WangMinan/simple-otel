#ifndef TRACE_SPAN_CONTEXT_H
#define TRACE_SPAN_CONTEXT_H
#include "../protocol/message.h"
#include "sampler.h"
#include "sampler/alway_on_sampler.h"
#include "trace_metadata.h"
#include <memory>
#include <string>
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
  thread_local static SpanContext current_context;

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

  /// @brief change trace id in current context
  /// @param trace_id_
  static void SetTraceId(std::string trace_id_);

  /// @brief change span id in current context
  /// @param span_id_
  static void SetSpanId(std::string span_id_);

  /// @brief change trace flag in current context
  /// @param trace_flag_
  static void SetTraceFlag(TraceFlag trace_flag_);

  /// @brief change sampler in current context
  /// @param sampler
  static void SetSampler(std::shared_ptr<Sampler> &&sampler);

  /// @brief write current context to the message for transferring
  /// @param message
  static void WriteToMessage(protocol::Message &message);
};

} // namespace trace

#endif // !TRACE_SPAN_CONTEXT_H
