#pragma once

#include "notascore/core/PerformanceProfile.hpp"

#include <cstdint>

namespace notascore::ui {

struct PerformanceSettings {
    bool cpuModeOnly {true};
    bool gpuAccelerationOptional {false};
    std::uint32_t audioBufferFrames {256};
    bool disableAnimations {true};
    bool disableShadows {true};
    bool disableSmoothZoom {true};
    bool lowMemoryMode {true};

    static PerformanceSettings fromHardware(const notascore::core::HardwareProfile& hw);
};

} // namespace notascore::ui
