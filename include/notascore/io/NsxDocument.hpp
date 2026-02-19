#pragma once

#include "notascore/notation/NotationEngine.hpp"

#include <filesystem>
#include <vector>

namespace notascore::io {

class NsxDocument {
public:
    bool save(const std::filesystem::path& path, const notascore::notation::NotationEngine& notation) const;
    bool load(const std::filesystem::path& path, std::vector<notascore::notation::NoteEvent>& outNotes) const;
};

} // namespace notascore::io
