#include "log_exporter.h"
#include <iostream>

#ifndef LOG_OSTREAM_LOG_EXPORTER_H
#define LOG_OSTREAM_LOG_EXPORTER_H
namespace nlog {
class OstreamLogExporter : public LogExporter {
private:
  std::ostream &sout;

public:
  OstreamLogExporter(std::ostream &sout_ = std::cout) : sout(sout_){};
  ~OstreamLogExporter() = default;
  void Export(LogRecord &log_record) override;
  
};

} // namespace nlog

#endif // !LOG_OSTREAM_LOG_EXPORTER_H
