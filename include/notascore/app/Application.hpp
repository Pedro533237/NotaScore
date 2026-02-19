#pragma once

#include "notascore/audio/AudioEngine.hpp"
#include "notascore/core/TaskScheduler.hpp"
#include "notascore/notation/NotationEngine.hpp"
#include "notascore/platform/NativeWindow.hpp"
#include "notascore/render/CpuRenderer.hpp"
#include "notascore/ui/MainWindow.hpp"
#include "notascore/ui/PerformanceSettings.hpp"

namespace notascore::app {

class Application {
public:
    Application();
    int run();

private:
    notascore::core::TaskScheduler m_scheduler;
    notascore::render::CpuRenderer m_renderer;
    notascore::notation::NotationEngine m_notation;
    notascore::audio::AudioEngine m_audio;
    notascore::ui::PerformanceSettings m_settings;
    notascore::ui::MainWindow m_mainWindow;
    notascore::platform::NativeWindow m_nativeWindow;
};

} // namespace notascore::app
