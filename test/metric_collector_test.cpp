#include "exporter/ostream_metric_exporter.h"
#include "exporter/file_metric_exporter.h"
#include "exporter/tcp_metric_exporter.h"
#include "tcp_exporter.h"
#include "metric_collector.h"
#include "../src/common/unique_ptr.h"
#include <algorithm>
#include <memory>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  std::string ip = "8.149.132.190";
  int port = 8088;
  auto exporter = common::make_unique<metric::TCPMetricExporter>();
  common::TCPExporter tcp_exporter(ip,port);
  metric::MetricsCollector c(1, "test", std::move(exporter));
  c.start();
  sleep(10);
}
