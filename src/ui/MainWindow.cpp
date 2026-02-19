#include "notascore/ui/MainWindow.hpp"

namespace notascore::ui {

MainWindow::MainWindow(int width, int height, PerformanceSettings settings)
    : m_width(width), m_height(height), m_settings(settings) {
    rebuildActions();
}

void MainWindow::draw(notascore::render::CpuRenderer& renderer) {
    renderer.markDirty({.x = 0, .y = 0, .width = m_width, .height = 64});
    renderer.markDirty({.x = 0, .y = 64, .width = m_width / 3, .height = m_height - 64});
    renderer.renderFrame();
}

void MainWindow::toggleLowMemoryMode() {
    m_settings.lowMemoryMode = !m_settings.lowMemoryMode;
    m_settings.audioBufferFrames = m_settings.lowMemoryMode ? 512u : 256u;
    rebuildActions();
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
