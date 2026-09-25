#pragma once
namespace N::U
{

template <std::unsigned_integral IndexType = unsigned int, std::unsigned_integral GenType = unsigned int>
struct GIndexPool
{
    struct Handle
    {
        IndexType Index = 0;
        GenType Generation = 0;

        Handle() {}

        Handle(IndexType index, GenType gen) : Index(index), Generation(gen) {}

        bool operator==(const Handle& other) const
        {
            return Index == other.Index && Generation == other.Generation;
        }
    };

    bool IsAcquired(Handle handle) const
    {
        return handle.Index < m_Acquired.size() && m_Acquired[handle.Index] &&
            m_Generations[handle.Index] == handle.Generation;
    }

    Handle Acquire()
    {
        if (m_Free.empty())
        {
            m_Acquired.push_back(true);
            m_Generations.push_back(1);
            return {FullSize() - 1, 1};
        }

        IndexType index = m_Free.back();
        m_Free.pop_back();

        m_Acquired[index] = true;
        return {index, m_Generations[index]};
    }

    void Release(Handle handle)
    {
        if (!IsAcquired(handle))
        {
            return;
        }

        m_Acquired[handle.Index] = false;
        m_Generations[handle.Index] += 1;
        m_Free.push_back(handle.Index);
    }

    /** @brief Returns the number of currently acquired indices. */
    IndexType AcquiredSize() const
    {
        return m_Acquired.size() - m_Free.size();
    }

    /** @brief Returns the total number of indices ever allocated by the pool. */
    IndexType FullSize() const
    {
        return m_Acquired.size();
    }

    /** @brief Releases all acquired indices and resets the pool. */
    void Clear()
    {
        m_Acquired.clear();
        m_Generations.clear();
        m_Free.clear();
    }

    /** @brief Returns whether no indices are currently acquired. */
    bool Empty() const
    {
        return AcquiredSize() == 0;
    }

    /** @brief Reserves storage for the specified number of indices. */
    void Reserve(IndexType capacity)
    {
        m_Acquired.reserve(capacity);
        m_Generations.reserve(capacity);
    }

  private:
    std::vector<bool> m_Acquired{};
    std::vector<GenType> m_Generations{};
    std::vector<IndexType> m_Free{};
};

} // namespace N::U