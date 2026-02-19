#pragma once

#include <atomic>
#include <cstdint>

namespace notascore::audio {

class AudioEngine {
public:
    void configureBuffer(std::uint32_t frames) noexcept { m_bufferFrames = frames; }
    void setPlaybackLite(bool enabled) noexcept { m_playbackLite = enabled; }

    [[nodiscard]] std::uint32_t bufferFrames() const noexcept { return m_bufferFrames; }
    [[nodiscard]] bool playbackLite() const noexcept { return m_playbackLite; }

private:
    std::uint32_t m_bufferFrames {256};
    bool m_playbackLite {false};
};

} // namespace notascore::audio
