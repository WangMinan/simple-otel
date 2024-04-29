#ifndef METRIC_EXPORTER_H
#define METRIC_EXPORTER_H

#include "metric_record.h"
#include <initializer_list>

namespace metric {

class MetricExporter {

public:
  virtual ~MetricExporter() = default;
  virtual void Export(std::initializer_list<MetricRecord*> records) = 0;
};
} // namespace metric

#endif // !METRIC_EXPORTER_H
