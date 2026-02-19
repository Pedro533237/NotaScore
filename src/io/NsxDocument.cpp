#include "notascore/io/NsxDocument.hpp"

#include <fstream>

namespace notascore::io {

bool NsxDocument::save(const std::filesystem::path& path, const notascore::notation::NotationEngine& notation) const {
    std::ofstream output(path, std::ios::binary);
    if (!output) {
        return false;
    }

    const std::uint32_t magic = 0x4E535831; // NSX1
    const std::uint64_t noteCount = notation.noteCount();
    output.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
    output.write(reinterpret_cast<const char*>(&noteCount), sizeof(noteCount));
    return output.good();
}

bool NsxDocument::load(const std::filesystem::path& path, std::vector<notascore::notation::NoteEvent>& outNotes) const {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return false;
    }

    std::uint32_t magic = 0;
    std::uint64_t count = 0;
    input.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    input.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!input || magic != 0x4E535831) {
        return false;
    }

    outNotes.clear();
    outNotes.resize(static_cast<std::size_t>(count));
    return true;
}

} // namespace notascore::io
