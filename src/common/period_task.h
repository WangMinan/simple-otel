#ifndef COMMON_PERIOD_TASK_H
#define COMMON_PERIOD_TASK_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
class PeriodTask {
private:
  std::thread worker;
  std::atomic<bool> running;
  unsigned int interval;
  std::function<void()> task;
  std::mutex mutex;
  std::condition_variable cv;
  void taskWorker();

public:
  PeriodTask(std::function<void()> task_, unsigned int interval_)
      : task(task_), interval(interval_), running(true) {
    worker = std::thread(&PeriodTask::taskWorker, this);
  };
  ~PeriodTask();
  void stopTask();
};

#endif // !COMMON_PERIOD_TASK_H