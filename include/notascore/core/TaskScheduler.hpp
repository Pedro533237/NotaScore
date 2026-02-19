#pragma once

#include "notascore/core/ThreadPool.hpp"

#include <functional>
#include <queue>

namespace notascore::core {

enum class TaskPriority {
    Realtime,
    Interactive,
    Background
};

class TaskScheduler {
public:
    explicit TaskScheduler(std::size_t workerCount);

    void submit(TaskPriority priority, std::function<void()> task);
    void flush();

private:
    void dispatch();

    ThreadPool m_pool;
    std::queue<std::function<void()>> m_realtime;
    std::queue<std::function<void()>> m_interactive;
    std::queue<std::function<void()>> m_background;
};

} // namespace notascore::core
