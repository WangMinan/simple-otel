#include "ostream_metric_exporter.h"

namespace metric {



void OstreamMetricExporter::Export(std::vector<MetricRecord*> &records) {
  sout << "[\n";
  for (auto record : records) {
    sout << record->PrintJson() << ",\n" << std::endl;
  }
  sout << "\n]"; 
}
} // namespace metric
