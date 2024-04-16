
#ifndef TRACE_TRACE_FLAG_H
#define TRACE_TRACE_FLAG_H
enum TraceFlag
{
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
#endif // !TRACE_TRACE_FLAG_H
