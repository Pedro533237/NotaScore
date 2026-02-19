#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace notascore::core {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t threadCount);
    ~ThreadPool();

    void schedule(std::function<void()> task);
    void waitIdle();

private:
    void workerLoop();

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_wakeup;
    std::condition_variable m_idle;
    std::size_t m_activeTasks {0};
    bool m_stopping {false};
};

} // namespace notascore::core
