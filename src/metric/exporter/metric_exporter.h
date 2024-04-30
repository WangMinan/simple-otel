#ifndef METRIC_EXPORTER_H
#define METRIC_EXPORTER_H

#include "metric_record.h"
#include <vector>

namespace metric {

class MetricExporter {

public:
  virtual ~MetricExporter() = default;
  virtual void Export(std::vector<MetricRecord*> &records) = 0;
};
} // namespace metric

#endif // !METRIC_EXPORTER_H
