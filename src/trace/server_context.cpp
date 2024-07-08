#include "span_context.h"
#include "sampler.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>

namespace trace
{

    std::unordered_map<std::thread::id, std::shared_ptr<SpanContext>> ServerContext::parent_contexts;
    std::unordered_map<std::thread::id, std::shared_ptr<SpanContext>> ServerContext::current_contexts;
    std::unordered_map<std::thread::id, std::shared_ptr<Span>> ServerContext::active_spans;
    std::unordered_map<std::thread::id, std::vector<std::vector<std::shared_ptr<RespContext>>>> ServerContext::resp_contexts;
    std::unordered_map<std::thread::id, std::shared_ptr<RespContext>> ServerContext::return_context;
    std::recursive_mutex ServerContext::ServerContextLock;

    // bool SpanContext::IsValid() { return !trace_id.empty() && !span_id.empty(); }
    std::ostream &operator<<(std::ostream &os, const SpanContext &spancontext)
    {
        os << spancontext.trace_id << '/' << spancontext.span_id << '/' << spancontext.trace_flag;
        return os;
    }

    void ServerContext::Extract(protocol::Message message)
    {
        static const std::vector<std::string> reserved_keys = {
            "trace_id", "span_id", "trace_flag", "sample_strategy"};
        auto trace_id = message.GetHeader("trace_id");
        auto span_id = message.GetHeader("span_id");
        auto trace_flag = message.GetHeader("trace_flag");
        auto sample_strategy =
            std::atoi(message.GetHeader("sample_strategy").c_str());
        std::unordered_map<std::string, std::string> attributes_;
        for (auto &it : message.GetHeaders())
        {
            if (std::find(reserved_keys.begin(), reserved_keys.end(), it.first) !=
                reserved_keys.end())
            {
                continue;
            }
            attributes_.emplace(it.first, it.second);
        }
        // std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        if (trace_id.empty())
        {
            std::thread::id threadID = std::this_thread::get_id();
            // std::pair<const std::thread::id, trace::SpanContext> myPair(std::this_thread::get_id(), *common::make_unique<SpanContext>(trace_id, span_id, attributes_));
            // 使用辅助函数来构造 std::pair
            // auto myPair = std::make_pair(threadID, std::make_shared<SpanContext>(trace_id, span_id, attributes_));
            parent_contexts[threadID] = std::make_shared<SpanContext>(trace_id, span_id, attributes_);
            // parent_contexts[threadID] = *std::make_unique<SpanContext>(trace_id, span_id, attributes_);
            // parent_contexts = SpanContext(trace_id, span_id, attributes_);
        }
        else
        {
            // 使用父span的 TraceFlags 和 SampleStrategy
            std::thread::id threadID = std::this_thread::get_id();
            parent_contexts[threadID] = std::make_shared<SpanContext>(trace_id, span_id, attributes_,
                                                                            TraceFlagHandler::GetTraceFlag(trace_flag),
                                                                            static_cast<SampleStrategy>(sample_strategy));
        }
    }

    SpanContext &ServerContext::GetParentContext()
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();
        if (parent_contexts.count(threadID) <= 0)
        {
            parent_contexts[threadID] = std::make_shared<SpanContext>();
            // return *std::make_shared<SpanContext>();
        }
        return *parent_contexts[threadID];
    }

    // 获取目前最新的context，如果current_context为空，则直接取parent_context
    SpanContext &ServerContext::GetCurrentContext()
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();
        if (current_contexts.count(threadID) <= 0)
        {
            current_contexts[threadID] = std::make_shared<SpanContext>();
            // return *std::make_shared<SpanContext>();
        }
        return *current_contexts[threadID];
    }

    void ServerContext::SetTraceFlag(TraceFlag trace_flag_)
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        SpanContext &current_context = GetCurrentContext();
        current_context.trace_flag = trace_flag_;
    }

    void ServerContext::WriteToMessage(protocol::Message &message)
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        SpanContext &current_context = GetCurrentContext();
        message.SetHeader("trace_id", current_context.trace_id);
        message.SetHeader("span_id", current_context.span_id);
        message.SetHeader("trace_flag", TraceFlagHandler::Serialize(
                                            current_context.GetTraceFlag()));
        message.SetHeader("sample_strategy", std::to_string(static_cast<int>(
                                                 current_context.sample_strategy)));
        for (auto &it : current_context.attributes)
        {
            message.SetHeader(it.first, it.second);
        }
    }

    void ServerContext::Attach(std::string trace_id, std::string span_id,
                               std::unordered_map<std::string, std::string> attributes,
                               TraceFlag trace_flag, SampleStrategy sample_strategy)
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();

        // current_contexts.emplace(threadID, std::make_shared<SpanContext>(trace_id, span_id, attributes, trace_flag,
        //                                                                  sample_strategy));
        current_contexts[threadID] = std::make_shared<SpanContext>(trace_id, span_id, attributes, trace_flag,
                                                                         sample_strategy);
    }

    void ServerContext::Detach()
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();

        if (current_contexts.find(threadID) == current_contexts.end())
        {
            return;
        }
        current_contexts.erase(threadID);
        active_spans.erase(threadID);
    }

    void ServerContext::AddActiveSpan(std::shared_ptr<Span> span)
    {
        std::lock_guard<std::recursive_mutex> lock_guard(ServerContext::ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();
        active_spans[threadID] = span;
    }

    void ServerContext::RemoveActiveSpan()
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();

        if (active_spans.find(threadID) != active_spans.end())
        {
            active_spans.erase(threadID);
        }
    }

    std::shared_ptr<Span> ServerContext::GetCurrentSpan()
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();

        if (active_spans.find(threadID) == active_spans.end())
        {
            return nullptr;
        }
        return active_spans[threadID];
    }

    std::vector<std::shared_ptr<RespContext>> *ServerContext::GetCurrentRespContext()
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id this_id = std::this_thread::get_id();
        if (resp_contexts.empty())
        {
            return nullptr;
        }
        return &resp_contexts[this_id].back();
    }

    std::vector<std::shared_ptr<RespContext>> *ServerContext::GetParentRespContext()
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id this_id = std::this_thread::get_id();
        if (resp_contexts.size() <= 1)
        {
            return nullptr;
        }
        return &resp_contexts[this_id][resp_contexts.size() - 2];
    }

    void ServerContext::AddRespContext(std::shared_ptr<RespContext> context)
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        auto current = GetCurrentRespContext();
        current->push_back(context);
    }

    void ServerContext::SetReturnContext(TraceFlag trace_flag_)
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();
        return_context[threadID] = common::make_unique<RespContext>(trace_flag_);
    }

    RespContext &ServerContext::GetReturnContext()
    {
        std::lock_guard<std::recursive_mutex> lock(ServerContextLock);
        std::thread::id threadID = std::this_thread::get_id();
        return *return_context[threadID];
    }

    void ServerContext::AddRespContext(TraceFlag trace_flag)
    {
        auto resp_context = std::make_shared<RespContext>(trace_flag);
        auto current = GetCurrentRespContext();
        current->push_back(resp_context);
    }
}