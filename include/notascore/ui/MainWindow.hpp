#pragma once

#include "notascore/ui/PerformanceSettings.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace notascore::ui {

struct UiAction {
    std::string label;
    bool enabled {true};
};

struct Rect {
    int x {0};
    int y {0};
    int width {0};
    int height {0};

    [[nodiscard]] bool contains(int px, int py) const noexcept {
        return px >= x && py >= y && px < (x + width) && py < (y + height);
    }
};

class MainWindow {
public:
    MainWindow(int width, int height, PerformanceSettings settings);

    void resize(int width, int height) noexcept;
    void onClick(int x, int y);

    [[nodiscard]] int width() const noexcept { return m_width; }
    [[nodiscard]] int height() const noexcept { return m_height; }
    [[nodiscard]] const std::vector<UiAction>& actions() const noexcept { return m_actions; }
    [[nodiscard]] const std::string& statusText() const noexcept { return m_statusText; }
    [[nodiscard]] int scoreCount() const noexcept { return m_scoreCount; }

    [[nodiscard]] Rect newScoreCardRect() const noexcept;

private:
    void rebuildActions();

    int m_width;
    int m_height;
    PerformanceSettings m_settings;
    std::vector<UiAction> m_actions;
    int m_scoreCount {0};
    std::string m_statusText {"Ready"};
};

} // namespace notascore::ui
