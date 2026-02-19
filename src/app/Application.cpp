#include "notascore/app/Application.hpp"

#include <iostream>

namespace notascore::app {

Application::Application()
    : m_scheduler(2),
      m_renderer(1920, 1080),
      m_settings(notascore::ui::PerformanceSettings::fromHardware(
          {.cpuModel = "Intel i3 1st Gen", .ramMb = 4096, .hasDedicatedGpu = false, .legacyOpenGLOnly = true})) {
    m_audio.configureBuffer(m_settings.audioBufferFrames);
    m_audio.setPlaybackLite(m_settings.lowMemoryMode);
}

int Application::run() {
    m_notation.addNote({.tick = 0, .duration = 480, .midiPitch = 60});
    m_notation.addNote({.tick = 480, .duration = 480, .midiPitch = 64});

    m_scheduler.submit(notascore::core::TaskPriority::Interactive, [this] { m_notation.recomputeLayoutIfNeeded(); });
    m_scheduler.submit(notascore::core::TaskPriority::Realtime, [this] {
        m_renderer.markDirty({.x = 0, .y = 0, .width = 640, .height = 240});
        m_renderer.renderFrame();
    });
    m_scheduler.flush();

    std::cout << "NotaScore booted in CPU-first mode: " << std::boolalpha << m_settings.cpuModeOnly << '\n';
    std::cout << "Layout version: " << m_notation.layoutVersion() << " | Frame: " << m_renderer.frameCounter() << '\n';
    std::cout << "Audio buffer: " << m_audio.bufferFrames() << " | Playback Lite: " << m_audio.playbackLite() << '\n';
    return 0;
}

} // namespace notascore::app
