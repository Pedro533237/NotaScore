#include "notascore/app/Application.hpp"

namespace notascore::app {

Application::Application()
    : m_scheduler(2),
      m_renderer(1280, 720),
      m_settings(notascore::ui::PerformanceSettings::fromHardware(
          {.cpuModel = "Intel i3 1st Gen", .ramMb = 4096, .hasDedicatedGpu = false, .legacyOpenGLOnly = true})),
      m_mainWindow(1280, 720, m_settings),
      m_nativeWindow(m_mainWindow) {
    m_audio.configureBuffer(m_settings.audioBufferFrames);
    m_audio.setPlaybackLite(m_settings.lowMemoryMode);
}

int Application::run() {
    m_notation.addNote({.tick = 0, .duration = 480, .midiPitch = 60});
    m_notation.addNote({.tick = 480, .duration = 480, .midiPitch = 64});

    m_scheduler.submit(notascore::core::TaskPriority::Interactive, [this] { m_notation.recomputeLayoutIfNeeded(); });
    m_scheduler.flush();

    return m_nativeWindow.run();
}

} // namespace notascore::app
