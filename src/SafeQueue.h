#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond;
    bool finished = false;
public:
    void push(T item) {
        std::unique_lock<std::mutex> lock(mtx);
        queue.push(item);
        lock.unlock();
        cond.notify_one();
    }
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [this] { return !queue.empty() || finished; });
        if (queue.empty() && finished) return false;
        item = queue.front();
        queue.pop();
        return true;
    }
    void markFinished() {
        std::unique_lock<std::mutex> lock(mtx);
        finished = true;
        cond.notify_all();
    }
};
#endif
