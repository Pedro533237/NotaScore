#pragma once

#include <string>

namespace notascore::core {

struct HardwareProfile {
    std::string cpuModel;
    int ramMb {4096};
    bool hasDedicatedGpu {false};
    bool legacyOpenGLOnly {true};
};

enum class ExecutionMode {
    CpuOnly,
    Hybrid
};

class PerformanceProfile {
public:
    [[nodiscard]] static ExecutionMode chooseRenderMode(const HardwareProfile& hw) noexcept;
    [[nodiscard]] static bool shouldUseLowMemoryMode(const HardwareProfile& hw) noexcept;
};

} // namespace notascore::core
