#include "notascore/ui/MainWindow.hpp"

#include <algorithm>
#include <array>

namespace notascore::ui {

namespace {

constexpr int kCardWidth = 260;
constexpr int kCardHeight = 170;

} // namespace

MainWindow::MainWindow(int width, int height, PerformanceSettings settings)
    : m_width(width), m_height(height), m_settings(settings) {
    m_recentProjects = {"String Quartet in D", "Film Cue Sketch", "Piano Etude No. 2"};
    m_instrumentLibrary = {
        {.group = "Madeiras", .name = "Flute", .range = "C4-C7"},
        {.group = "Madeiras", .name = "Oboe", .range = "Bb3-A6"},
        {.group = "Metais", .name = "Trumpet", .range = "F#3-D6"},
        {.group = "Cordas", .name = "Violin", .range = "G3-A7"},
        {.group = "Cordas", .name = "Cello", .range = "C2-E5"},
        {.group = "Percussao", .name = "Snare Drum", .range = "Perc"},
        {.group = "Teclas", .name = "Piano", .range = "A0-C8"},
        {.group = "Personalizados", .name = "Synth Lead", .range = "C2-C7"},
    };

    rebuildActions();
}

void MainWindow::resize(int width, int height) noexcept {
    m_width = width;
    m_height = height;
}

Rect MainWindow::newScoreCardRect() const noexcept {
    return {.x = 240, .y = 140, .width = kCardWidth, .height = kCardHeight};
}

Rect MainWindow::openProjectRect() const noexcept {
    return {.x = 240, .y = 330, .width = 180, .height = 36};
}

Rect MainWindow::importMidiRect() const noexcept {
    return {.x = 240, .y = 374, .width = 180, .height = 36};
}

Rect MainWindow::importMusicXmlRect() const noexcept {
    return {.x = 240, .y = 418, .width = 180, .height = 36};
}

Rect MainWindow::assistantPanelRect() const noexcept {
    return {.x = m_width - 460, .y = 0, .width = 460, .height = m_height};
}

Rect MainWindow::compatibilityToggleRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = panel.height - 160, .width = 20, .height = 20};
}

Rect MainWindow::livePreviewToggleRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = panel.height - 126, .width = 20, .height = 20};
}

Rect MainWindow::assistantNextRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + panel.width - 120, .y = panel.height - 60, .width = 90, .height = 34};
}

Rect MainWindow::assistantBackRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + panel.width - 220, .y = panel.height - 60, .width = 90, .height = 34};
}

Rect MainWindow::keySignatureRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = 120, .width = panel.width - 48, .height = 42};
}

Rect MainWindow::meterRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = 180, .width = panel.width - 48, .height = 42};
}

Rect MainWindow::tempoRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = 240, .width = panel.width - 48, .height = 42};
}

Rect MainWindow::titleRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = 300, .width = panel.width - 48, .height = 42};
}

Rect MainWindow::composerRect() const noexcept {
    const auto panel = assistantPanelRect();
    return {.x = panel.x + 24, .y = 360, .width = panel.width - 48, .height = 42};
}

Rect MainWindow::instrumentAddRect(std::size_t index) const noexcept {
    const auto panel = assistantPanelRect();
    const int y = 110 + static_cast<int>(index) * 34;
    return {.x = panel.x + 174, .y = y, .width = 20, .height = 20};
}

Rect MainWindow::selectedInstrumentRemoveRect(std::size_t index) const noexcept {
    const auto panel = assistantPanelRect();
    const int y = 110 + static_cast<int>(index) * 30;
    return {.x = panel.x + panel.width - 34, .y = y, .width = 16, .height = 16};
}

void MainWindow::openNewScoreWizard() {
    m_wizardOpen = true;
    m_wizardStep = WizardStep::Instruments;
    m_statusText = "Wizard opened: Select instruments";
}

void MainWindow::onClick(int x, int y) {
    if (!m_wizardOpen) {
        if (newScoreCardRect().contains(x, y)) {
            openNewScoreWizard();
            return;
        }

        if (openProjectRect().contains(x, y)) {
            m_statusText = "Open project clicked";
            return;
        }
        if (importMidiRect().contains(x, y)) {
            m_statusText = "Import MIDI clicked";
            return;
        }
        if (importMusicXmlRect().contains(x, y)) {
            m_statusText = "Import MusicXML clicked";
        }
        return;
    }

    if (compatibilityToggleRect().contains(x, y)) {
        m_compatibilityMode = !m_compatibilityMode;
        m_settings.disableAnimations = m_compatibilityMode;
        m_settings.disableShadows = m_compatibilityMode;
        m_settings.disableSmoothZoom = m_compatibilityMode;
        m_statusText = m_compatibilityMode ? "Compatibility mode enabled" : "Compatibility mode disabled";
        rebuildActions();
        return;
    }

    if (livePreviewToggleRect().contains(x, y)) {
        m_livePreviewEnabled = !m_livePreviewEnabled;
        m_statusText = m_livePreviewEnabled ? "Live preview enabled" : "Live preview disabled";
        return;
    }

    if (m_wizardStep == WizardStep::Instruments) {
        const std::size_t visible = std::min<std::size_t>(6, m_instrumentLibrary.size());
        for (std::size_t i = 0; i < visible; ++i) {
            if (instrumentAddRect(i).contains(x, y)) {
                const auto& candidate = m_instrumentLibrary[i];
                const auto already = std::find_if(m_selectedInstruments.begin(), m_selectedInstruments.end(),
                    [&](const auto& item) { return item.name == candidate.name; });
                if (already == m_selectedInstruments.end()) {
                    m_selectedInstruments.push_back(candidate);
                    m_statusText = candidate.name + " added";
                }
                return;
            }
        }

        for (std::size_t i = 0; i < m_selectedInstruments.size(); ++i) {
            if (selectedInstrumentRemoveRect(i).contains(x, y)) {
                const auto removed = m_selectedInstruments[i].name;
                m_selectedInstruments.erase(m_selectedInstruments.begin() + static_cast<std::ptrdiff_t>(i));
                m_statusText = removed + " removed";
                return;
            }
        }
    } else {
        static constexpr std::array<const char*, 4> keyOptions {"C Major", "G Major", "D Minor", "F Major"};
        static constexpr std::array<const char*, 4> meterOptions {"4/4", "3/4", "6/8", "5/4"};

        if (keySignatureRect().contains(x, y)) {
            m_settingsPresetIndex = (m_settingsPresetIndex + 1) % keyOptions.size();
            m_keySignature = keyOptions[m_settingsPresetIndex];
            m_statusText = "Key changed to " + m_keySignature;
            return;
        }
        if (meterRect().contains(x, y)) {
            m_meter = meterOptions[m_settingsPresetIndex % meterOptions.size()];
            m_statusText = "Meter changed to " + m_meter;
            return;
        }
        if (tempoRect().contains(x, y)) {
            m_bpm += 5;
            if (m_bpm > 180) {
                m_bpm = 80;
            }
            m_statusText = "Tempo set to " + std::to_string(m_bpm) + " BPM";
            return;
        }
        if (titleRect().contains(x, y)) {
            m_title = "Score " + std::to_string(m_scoreCount + 1);
            m_statusText = "Title updated";
            return;
        }
        if (composerRect().contains(x, y)) {
            m_composer = "NotaScore User";
            m_statusText = "Composer auto-filled";
            return;
        }
    }

    if (assistantBackRect().contains(x, y)) {
        if (m_wizardStep == WizardStep::ScoreSettings) {
            m_wizardStep = WizardStep::Instruments;
            m_statusText = "Back to instrument selection";
        } else {
            m_wizardOpen = false;
            m_statusText = "Wizard closed";
        }
        return;
    }

    if (assistantNextRect().contains(x, y)) {
        if (m_wizardStep == WizardStep::Instruments) {
            if (m_selectedInstruments.empty()) {
                m_statusText = "Select at least one instrument";
                return;
            }
            m_wizardStep = WizardStep::ScoreSettings;
            m_statusText = "Configure score metadata";
            return;
        }

        ++m_scoreCount;
        m_recentProjects.insert(m_recentProjects.begin(), m_title);
        if (m_recentProjects.size() > 8) {
            m_recentProjects.pop_back();
        }
        m_wizardOpen = false;
        m_statusText = "Score created: " + m_title;
    }
}

void MainWindow::rebuildActions() {
    m_actions = {
        {.label = "CPU Mode Only", .enabled = m_settings.cpuModeOnly},
        {.label = "GPU Acceleration (optional)", .enabled = m_settings.gpuAccelerationOptional},
        {.label = "Disable animations", .enabled = m_settings.disableAnimations},
        {.label = "Disable shadows", .enabled = m_settings.disableShadows},
        {.label = "Disable smooth zoom", .enabled = m_settings.disableSmoothZoom},
        {.label = "Low Memory Mode", .enabled = m_settings.lowMemoryMode},
        {.label = "Compatibility Mode", .enabled = m_compatibilityMode},
    };
}

} // namespace notascore::ui
