#include "notascore/core/ThreadPool.hpp"

namespace notascore::core {

ThreadPool::ThreadPool(std::size_t threadCount) {
    m_workers.reserve(threadCount);
    for (std::size_t i = 0; i < threadCount; ++i) {
        m_workers.emplace_back([this] { workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::scoped_lock lock(m_mutex);
        m_stopping = true;
    }
    m_wakeup.notify_all();
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::schedule(std::function<void()> task) {
    {
        std::scoped_lock lock(m_mutex);
        m_tasks.push(std::move(task));
    }
    m_wakeup.notify_one();
}

void ThreadPool::waitIdle() {
    std::unique_lock lock(m_mutex);
    m_idle.wait(lock, [this] { return m_tasks.empty() && m_activeTasks == 0; });
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(m_mutex);
            m_wakeup.wait(lock, [this] { return m_stopping || !m_tasks.empty(); });
            if (m_stopping && m_tasks.empty()) {
                return;
            }
            task = std::move(m_tasks.front());
            m_tasks.pop();
            ++m_activeTasks;
        }

        task();

        {
            std::scoped_lock lock(m_mutex);
            --m_activeTasks;
        }
        m_idle.notify_all();
    }
}

} // namespace notascore::core
