#include "notascore/core/PerformanceProfile.hpp"

namespace notascore::core {

ExecutionMode PerformanceProfile::chooseRenderMode(const HardwareProfile& hw) noexcept {
    if (!hw.hasDedicatedGpu || hw.legacyOpenGLOnly || hw.ramMb <= 4096) {
        return ExecutionMode::CpuOnly;
    }
    return ExecutionMode::Hybrid;
}

bool PerformanceProfile::shouldUseLowMemoryMode(const HardwareProfile& hw) noexcept {
    return hw.ramMb <= 4096;
}

} // namespace notascore::core
