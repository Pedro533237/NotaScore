#pragma once

#include "notascore/ui/PerformanceSettings.hpp"

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

enum class WizardStep {
    Instruments,
    ScoreSettings
};

struct InstrumentDef {
    std::string group;
    std::string name;
    std::string range;
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

    [[nodiscard]] bool wizardOpen() const noexcept { return m_wizardOpen; }
    [[nodiscard]] WizardStep wizardStep() const noexcept { return m_wizardStep; }
    [[nodiscard]] bool compatibilityMode() const noexcept { return m_compatibilityMode; }
    [[nodiscard]] bool livePreviewEnabled() const noexcept { return m_livePreviewEnabled; }
    [[nodiscard]] int bpm() const noexcept { return m_bpm; }
    [[nodiscard]] const std::string& title() const noexcept { return m_title; }
    [[nodiscard]] const std::string& composer() const noexcept { return m_composer; }
    [[nodiscard]] const std::string& meter() const noexcept { return m_meter; }
    [[nodiscard]] const std::string& keySignature() const noexcept { return m_keySignature; }

    [[nodiscard]] const std::vector<std::string>& recentProjects() const noexcept { return m_recentProjects; }
    [[nodiscard]] const std::vector<InstrumentDef>& instrumentLibrary() const noexcept { return m_instrumentLibrary; }
    [[nodiscard]] const std::vector<InstrumentDef>& selectedInstruments() const noexcept { return m_selectedInstruments; }

    [[nodiscard]] Rect newScoreCardRect() const noexcept;
    [[nodiscard]] Rect openProjectRect() const noexcept;
    [[nodiscard]] Rect importMidiRect() const noexcept;
    [[nodiscard]] Rect importMusicXmlRect() const noexcept;
    [[nodiscard]] Rect assistantPanelRect() const noexcept;
    [[nodiscard]] Rect compatibilityToggleRect() const noexcept;
    [[nodiscard]] Rect livePreviewToggleRect() const noexcept;
    [[nodiscard]] Rect assistantNextRect() const noexcept;
    [[nodiscard]] Rect assistantBackRect() const noexcept;
    [[nodiscard]] Rect keySignatureRect() const noexcept;
    [[nodiscard]] Rect meterRect() const noexcept;
    [[nodiscard]] Rect tempoRect() const noexcept;
    [[nodiscard]] Rect titleRect() const noexcept;
    [[nodiscard]] Rect composerRect() const noexcept;

    [[nodiscard]] Rect instrumentAddRect(std::size_t index) const noexcept;
    [[nodiscard]] Rect selectedInstrumentRemoveRect(std::size_t index) const noexcept;

private:
    void rebuildActions();
    void openNewScoreWizard();

    int m_width;
    int m_height;
    PerformanceSettings m_settings;
    std::vector<UiAction> m_actions;

    bool m_wizardOpen {false};
    WizardStep m_wizardStep {WizardStep::Instruments};
    bool m_compatibilityMode {false};
    bool m_livePreviewEnabled {true};

    int m_scoreCount {0};
    int m_bpm {120};
    std::string m_title {"Untitled Score"};
    std::string m_composer {"Composer"};
    std::string m_meter {"4/4"};
    std::string m_keySignature {"C Major"};
    std::string m_statusText {"Ready"};

    std::vector<std::string> m_recentProjects;
    std::vector<InstrumentDef> m_instrumentLibrary;
    std::vector<InstrumentDef> m_selectedInstruments;
    std::size_t m_settingsPresetIndex {0};
};

} // namespace notascore::ui
