#include "notascore/io/NsxDocument.hpp"
#include "notascore/notation/NotationEngine.hpp"
#include "notascore/ui/MainWindow.hpp"
#include "notascore/ui/PerformanceSettings.hpp"

#include <filesystem>

int main() {
    notascore::notation::NotationEngine notation;
    notation.addNote({.tick = 0, .duration = 240, .midiPitch = 60});
    notation.recomputeLayoutIfNeeded();

    notascore::io::NsxDocument doc;
    const auto path = std::filesystem::path("smoke.nsx");
    if (!doc.save(path, notation)) {
        return 1;
    }

    std::vector<notascore::notation::NoteEvent> notes;
    if (!doc.load(path, notes)) {
        return 2;
    }

    const auto settings = notascore::ui::PerformanceSettings::fromHardware(
        {.cpuModel = "legacy", .ramMb = 4096, .hasDedicatedGpu = false, .legacyOpenGLOnly = true});

    notascore::ui::MainWindow mainWindow(1280, 720, settings);
    const auto card = mainWindow.newScoreCardRect();
    mainWindow.onClick(card.x + 10, card.y + 10);

    std::filesystem::remove(path);

    return settings.cpuModeOnly && settings.lowMemoryMode && notes.size() == 1 && mainWindow.scoreCount() == 1 ? 0 : 3;
}
