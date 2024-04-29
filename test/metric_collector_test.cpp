#include "exporter/ostream_metric_exporter.h"
#include "metric_collector.h"
#include <algorithm>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  auto exporter = std::make_unique<metric::OstreamMetricExporter>();
  metric::MetricsCollector c(1, "test", std::move(exporter));
  c.start();
  sleep(10);

  return 0;
}
