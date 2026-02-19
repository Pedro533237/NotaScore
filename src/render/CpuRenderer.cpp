#include "notascore/render/CpuRenderer.hpp"

#include <algorithm>

namespace notascore::render {

CpuRenderer::CpuRenderer(int width, int height)
    : m_width(width), m_height(height) {}

void CpuRenderer::markDirty(const DirtyRegion& region) {
    m_dirtyRegions.push_back(region);
}

void CpuRenderer::renderFrame() {
    if (m_dirtyRegions.empty()) {
        ++m_frameCounter;
        return;
    }

    for (const auto& region : m_dirtyRegions) {
        const auto x1 = std::max(0, region.x);
        const auto y1 = std::max(0, region.y);
        const auto x2 = std::min(m_width, region.x + region.width);
        const auto y2 = std::min(m_height, region.y + region.height);
        (void)x1;
        (void)y1;
        (void)x2;
        (void)y2;
    }

    m_dirtyRegions.clear();
    ++m_frameCounter;
}

} // namespace notascore::render
