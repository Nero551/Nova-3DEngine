#pragma once

namespace N::U
{
/**
 * @brief Manages a pool of reusable integer indices.
 *
 * Indices are acquired and released independently of the objects they identify,
 * allowing external storage to use the indices as stable IDs while released
 * indices are reused by later acquisitions.
 */
template <std::unsigned_integral IndexType = unsigned int> struct IndexPool
{
    using Index = IndexType;

    /** @brief Returns whether the index is currently being used. */
    bool IsAcquired(Index index) const
    {
        return index < m_Used.size() && m_Used[index];
    }

    /** @brief Acquires a new index or reuses a previously released one. */
    Index Acquire()
    {
        if (m_Free.empty())
        {
            m_Used.push_back(true);
            return FullSize() - 1;
        }

        Index index = m_Free.back();
        m_Free.pop_back();

        m_Used[index] = true;
        return index;
    }

    /** @brief Releases an index, making it available for reuse. */
    void Release(Index index)
    {
        if (!IsAcquired(index))
        {
            return;
        }

        m_Used[index] = false;
        m_Free.push_back(index);
    }

    /** @brief Returns the number of currently acquired indices. */
    Index Size() const
    {
        return m_Used.size() - m_Free.size();
    }

    /** @brief Returns the total number of indices ever allocated by the pool. */
    Index FullSize() const
    {
        return m_Used.size();
    }

  private:
    std::vector<bool> m_Used;
    std::vector<Index> m_Free;
};

} // namespace N::U