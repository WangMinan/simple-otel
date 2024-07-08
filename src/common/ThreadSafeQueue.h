#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

// 线程安全队列类定义
class ThreadSafeQueue {
private:
    std::queue<std::string> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;

    // 私有构造函数以实现单例模式
    ThreadSafeQueue() = default;

public:
    // 删除拷贝构造函数和赋值运算符以确保单例模式
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // 获取单例实例
    static ThreadSafeQueue& getInstance() {
        static ThreadSafeQueue instance;
        return instance;
    }

    // 推入元素到队列
    void push(const std::string& item) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(item);
        cv.notify_one();
    }

    // 从队列中弹出元素
    std::string pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        std::string item = std::move(queue.front());
        queue.pop();
        return item;
    }

    // 检查队列是否为空
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};

#endif // THREAD_SAFE_QUEUE_H
