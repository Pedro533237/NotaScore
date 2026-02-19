#include "notascore/ui/PerformanceSettings.hpp"

namespace notascore::ui {

PerformanceSettings PerformanceSettings::fromHardware(const notascore::core::HardwareProfile& hw) {
    PerformanceSettings settings;
    settings.cpuModeOnly = notascore::core::PerformanceProfile::chooseRenderMode(hw) == notascore::core::ExecutionMode::CpuOnly;
    settings.gpuAccelerationOptional = !settings.cpuModeOnly;
    settings.lowMemoryMode = notascore::core::PerformanceProfile::shouldUseLowMemoryMode(hw);
    settings.audioBufferFrames = settings.lowMemoryMode ? 512u : 256u;
    return settings;
}

} // namespace notascore::ui
