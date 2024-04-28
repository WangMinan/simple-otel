#ifndef TRACE_SAMPLER_FACTORY_H
#define TRACE_SAMPLER_FACTORY_H

#include "sampler.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace trace {
class SamplerFactory {
public:
  SamplerFactory() = default;
  ~SamplerFactory() = default;
  static std::unique_ptr<Sampler>
  Create(SampleStrategy strategy,
         std::unordered_map<std::string, std::string> args);
  /// @brief create a sampler from its parent context
  static std::unique_ptr<Sampler> CreateFromParent();
};

} // namespace trace

#endif // !TRACE_SAMPLER_FACTORY_H