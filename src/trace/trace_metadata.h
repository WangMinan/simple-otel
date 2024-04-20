
#ifndef TRACE_TRACE_FLAG_H
#define TRACE_TRACE_FLAG_H
#include <cstdlib>
#include <string>
enum TraceFlag {
  // 确定采样
  kIsSampled = 0,
  // 随机采样
  kIsRandom = 1,
  // 未到采样决策节点
  // 头部连贯采样变体用
  kIsWaiting = 2,
  // 确定丢弃
  kIsDiscarded = 3
};

class TraceFlagHandler {
public:
  static TraceFlag GetTraceFlag(std::string trace_flag) {
    switch (atoi(trace_flag.c_str())) {
    case 0:
      return TraceFlag::kIsSampled;
    case 1:
      return TraceFlag::kIsRandom;
    case 2:
      return TraceFlag::kIsWaiting;
    case 3:
      return TraceFlag::kIsDiscarded;
    default:
      return TraceFlag::kIsSampled;
    }
  };

  static std::string Serialize(TraceFlag trace_flag) {
    return std::to_string(static_cast<int>(trace_flag));
  }
};

#endif // !TRACE_TRACE_FLAG_H
