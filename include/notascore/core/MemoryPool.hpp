#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace notascore::core {

class MemoryPool {
public:
    explicit MemoryPool(std::size_t blockSize, std::size_t blockCount)
        : m_blockSize(blockSize), m_pool(blockSize * blockCount), m_freeList(blockCount) {
        for (std::size_t i = 0; i < blockCount; ++i) {
            m_freeList[i] = i;
        }
    }

    [[nodiscard]] std::byte* acquire() {
        if (m_freeList.empty()) {
            return nullptr;
        }
        const auto index = m_freeList.back();
        m_freeList.pop_back();
        return m_pool.data() + (index * m_blockSize);
    }

    void release(std::byte* ptr) {
        const auto base = m_pool.data();
        const auto offset = static_cast<std::size_t>(ptr - base);
        const auto index = offset / m_blockSize;
        m_freeList.push_back(index);
    }

    [[nodiscard]] std::size_t freeBlocks() const noexcept { return m_freeList.size(); }

private:
    std::size_t m_blockSize;
    std::vector<std::byte> m_pool;
    std::vector<std::size_t> m_freeList;
};

} // namespace notascore::core
