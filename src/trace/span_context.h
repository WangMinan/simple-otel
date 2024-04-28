#ifndef TRACE_SPAN_CONTEXT_H
#define TRACE_SPAN_CONTEXT_H
#include "../protocol/message.h"
#include "resp_context.h"
#include "sampler.h"
#include "sampler/alway_on_sampler.h"
#include "trace_metadata.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace trace {
class Span;
class SpanContext {
private:
  std::string trace_id;
  std::string span_id;
  TraceFlag trace_flag;
  SampleStrategy sample_strategy;
  std::unordered_map<std::string, std::string> attributes;

public:
  friend class Context;
  SpanContext() = default;
  SpanContext(std::string trace_id_, std::string span_id_,
              std::unordered_map<std::string, std::string> attributes_ = {},
              TraceFlag trace_flag_ = kIsSampled,
              SampleStrategy sample_strategy_ = SampleStrategy::kAlwaysSample)
      : trace_id(trace_id_), span_id(span_id_), trace_flag(trace_flag_),
        sample_strategy(sample_strategy_), attributes(attributes_){};
  ~SpanContext() = default;
  SpanContext(SpanContext &&other) = default;
  SpanContext &operator=(SpanContext &&other) = default;
  bool IsValid();
  std::string GetTraceId() { return this->trace_id; };
  std::string GetSpanId() { return this->span_id; }
  TraceFlag GetTraceFlag() { return this->trace_flag; }
  SampleStrategy GetSampleStrategy() { return this->sample_strategy; }
  std::unordered_map<std::string, std::string> &GetAttributes() {
    return this->attributes;
  }
};

/// @brief thread local span context
class Context {
private:
  thread_local static SpanContext parent_context;
  thread_local static std::vector<SpanContext> current_contexts;
  thread_local static std::vector<std::shared_ptr<Span>> active_spans;
  thread_local static std::vector<std::vector<std::shared_ptr<RespContext>>>
      resp_contexts;
  // 返回给请求发出者的上下文
  thread_local static std::unique_ptr<RespContext> return_context;

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
  static void Attach(std::string trace_id, std::string span_id,
                     std::unordered_map<std::string, std::string> attributes,
                     TraceFlag trace_flag, SampleStrategy strategy);

  /// @brief detach the current span context
  static void Detach();

  static void AddActiveSpan(std::shared_ptr<Span> span);

  /// @brief this function will not check if the last active span in thread
  /// local is current span. Make sure ending span in correct order
  static void RemoveLatestActiveSpan();

  /// @brief get current span. if there is no active span, return nullptr
  /// @return current span or nullptr
  static std::shared_ptr<Span> GetCurrentSpan();

  /// @brief get the response context of the current span. The response context
  /// is created by its child span
  /// @return
  static std::vector<std::shared_ptr<RespContext>> *GetCurrentRespContext();

  static void AddRespContext(TraceFlag trace_flag);

  static std::vector<std::shared_ptr<RespContext>> *GetParentRespContext();

  static void AddRespContext(std::shared_ptr<RespContext> context);

  static void SetReturnContext(TraceFlag trace_flag_);

  static RespContext &GetReturnContext();
};

} // namespace trace

#endif // !TRACE_SPAN_CONTEXT_H
