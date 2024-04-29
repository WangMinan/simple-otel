#include "metric_collector.h"
#include <unistd.h>

int main(int argc, char const *argv[]) {
  metric::MetricsCollector c(1);
  c.start();
  sleep(10);

  return 0;
}
