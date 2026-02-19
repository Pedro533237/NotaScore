#pragma once

#include <cstdint>
#include <vector>

namespace notascore::render {

struct DirtyRegion {
    int x {0};
    int y {0};
    int width {0};
    int height {0};
};

class CpuRenderer {
public:
    CpuRenderer(int width, int height);

    void markDirty(const DirtyRegion& region);
    void renderFrame();

    [[nodiscard]] std::uint64_t frameCounter() const noexcept { return m_frameCounter; }

private:
    int m_width;
    int m_height;
    std::vector<DirtyRegion> m_dirtyRegions;
    std::uint64_t m_frameCounter {0};
};

} // namespace notascore::render
