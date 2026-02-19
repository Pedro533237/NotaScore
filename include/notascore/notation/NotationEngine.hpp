#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace notascore::notation {

struct NoteEvent {
    int tick {0};
    int duration {0};
    int midiPitch {60};
};

class NotationEngine {
public:
    void addNote(const NoteEvent& event);
    void setDirty() noexcept { m_dirty = true; }
    void recomputeLayoutIfNeeded();

    [[nodiscard]] std::uint64_t layoutVersion() const noexcept { return m_layoutVersion; }
    [[nodiscard]] std::size_t noteCount() const noexcept { return m_notes.size(); }

private:
    std::vector<NoteEvent> m_notes;
    bool m_dirty {true};
    std::uint64_t m_layoutVersion {0};
};

} // namespace notascore::notation
