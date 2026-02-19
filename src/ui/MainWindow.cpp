#include "notascore/ui/MainWindow.hpp"

namespace notascore::ui {

MainWindow::MainWindow(int width, int height, PerformanceSettings settings)
    : m_width(width), m_height(height), m_settings(settings) {
    rebuildActions();
}

void MainWindow::resize(int width, int height) noexcept {
    m_width = width;
    m_height = height;
}

Rect MainWindow::newScoreCardRect() const noexcept {
    return {.x = 220, .y = 140, .width = 180, .height = 220};
}

void MainWindow::onClick(int x, int y) {
    if (newScoreCardRect().contains(x, y)) {
        ++m_scoreCount;
        m_statusText = "New score created (#" + std::to_string(m_scoreCount) + ")";
    }
}

void MainWindow::rebuildActions() {
    m_actions = {
        {.label = "CPU Mode Only", .enabled = m_settings.cpuModeOnly},
        {.label = "GPU Acceleration (optional)", .enabled = m_settings.gpuAccelerationOptional},
        {.label = "Disable animations", .enabled = m_settings.disableAnimations},
        {.label = "Disable shadows", .enabled = m_settings.disableShadows},
        {.label = "Disable smooth zoom", .enabled = m_settings.disableSmoothZoom},
        {.label = "Low Memory Mode", .enabled = m_settings.lowMemoryMode},
    };
}

} // namespace notascore::ui
