#pragma once

#include "notascore/render/CpuRenderer.hpp"
#include "notascore/ui/PerformanceSettings.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace notascore::ui {

struct UiAction {
    std::string label;
    bool enabled {true};
};

class MainWindow {
public:
    MainWindow(int width, int height, PerformanceSettings settings);

    void draw(notascore::render::CpuRenderer& renderer);
    void toggleLowMemoryMode();

    [[nodiscard]] const PerformanceSettings& settings() const noexcept { return m_settings; }
    [[nodiscard]] const std::vector<UiAction>& actions() const noexcept { return m_actions; }

private:
    void rebuildActions();

    int m_width;
    int m_height;
    PerformanceSettings m_settings;
    std::vector<UiAction> m_actions;
};

} // namespace notascore::ui
