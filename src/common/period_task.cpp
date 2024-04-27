#include "period_task.h"
#include <mutex>

void PeriodTask::taskWorker() {
  std::unique_lock<std::mutex> lock(this->mutex);
  while (running) {
    if (task) {
      task();
    }
    if (cv.wait_for(lock, std::chrono::seconds(interval),
                    [this]() { return !running; })) {
      break;
    }
  }
}

PeriodTask::~PeriodTask() {
  stopTask();
  if (worker.joinable()) {
    worker.join();
  }
}

void PeriodTask::stopTask() {
  std::lock_guard<std::mutex> lock(this->mutex);
  running = false;
  cv.notify_one();
}
