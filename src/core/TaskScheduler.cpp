#include "notascore/core/TaskScheduler.hpp"

namespace notascore::core {

TaskScheduler::TaskScheduler(std::size_t workerCount)
    : m_pool(workerCount) {}

void TaskScheduler::submit(TaskPriority priority, std::function<void()> task) {
    switch (priority) {
    case TaskPriority::Realtime:
        m_realtime.push(std::move(task));
        break;
    case TaskPriority::Interactive:
        m_interactive.push(std::move(task));
        break;
    case TaskPriority::Background:
        m_background.push(std::move(task));
        break;
    }

    dispatch();
}

void TaskScheduler::dispatch() {
    auto scheduleQueue = [this](auto& queue) {
        while (!queue.empty()) {
            m_pool.schedule(std::move(queue.front()));
            queue.pop();
        }
    };

    scheduleQueue(m_realtime);
    scheduleQueue(m_interactive);
    scheduleQueue(m_background);
}

void TaskScheduler::flush() {
    dispatch();
    m_pool.waitIdle();
}

} // namespace notascore::core
