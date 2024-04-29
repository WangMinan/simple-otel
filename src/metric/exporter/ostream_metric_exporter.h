

#include "metric_exporter.h"
#include <initializer_list>
#include <iostream>

#ifndef METRIC_OSTREAM_EXPORTER_H
#define METRIC_OSTREAM_EXPORTER_H

namespace metric {
class OstreamMetricExporter : public MetricExporter {
private:
  std::ostream &sout;

public:
  OstreamMetricExporter(std::ostream &sout_ = std::cout) : sout(sout_){};
  ~OstreamMetricExporter() = default;
  void Export(std::initializer_list<MetricRecord*> records) override;
};

} // namespace metric

#endif // !METRIC_OSTREAM_EXPORTER_H
