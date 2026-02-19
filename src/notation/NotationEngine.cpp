#include "notascore/notation/NotationEngine.hpp"

namespace notascore::notation {

void NotationEngine::addNote(const NoteEvent& event) {
    m_notes.push_back(event);
    m_dirty = true;
}

void NotationEngine::recomputeLayoutIfNeeded() {
    if (!m_dirty) {
        return;
    }

    // Placeholder for TeX-style spacing + collision passes.
    m_dirty = false;
    ++m_layoutVersion;
}

} // namespace notascore::notation
