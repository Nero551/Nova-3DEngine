#pragma once

namespace N::U
{

/**
 * @brief Sparse-indexed storage with densely packed values.
 *
 * Each sparse index maps to a dense index, allowing O(1) lookup while keeping
 * active entries contiguous. Erasing an entry uses swap-and-pop, so dense
 * indices can change after an erase.
 */
template <typename T> struct SparseSet
{
    using DenseIndex = unsigned int;
    using SparseIndex = unsigned int;

    static constexpr auto InvalidSparseIndex = std::numeric_limits<SparseIndex>::max();

    struct Entry
    {
        T Value;
        SparseIndex Index;
    };

    template <bool Const> struct BasicIterator
    {
        using SetType = std::conditional_t<Const, const SparseSet, SparseSet>;
        using EntryType = std::conditional_t<Const, const Entry, Entry>;

        SetType* Set;
        DenseIndex Index;

        BasicIterator& operator++()
        {
            ++Index;
            return *this;
        }

        EntryType& operator*() const
        {
            return Set->m_Dense[Index];
        }

        EntryType* operator->() const
        {
            return &Set->m_Dense[Index];
        }

        bool operator==(const BasicIterator& other) const
        {
            return Index == other.Index;
        }

        bool operator!=(const BasicIterator& other) const
        {
            return !(*this == other);
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    Iterator begin()
    {
        return {.Set = this, .Index = 0};
    }

    Iterator end()
    {
        return {.Set = this, .Index = Size()};
    }

    ConstIterator begin() const
    {
        return {.Set = this, .Index = 0};
    }

    ConstIterator end() const
    {
        return {.Set = this, .Index = Size()};
    }

    bool Contains(const SparseIndex s) const
    {
        return s < m_Sparse.size() && m_Sparse[s] != InvalidSparseIndex;
    }

    T& Get(const SparseIndex s)
    {
        if (!Contains(s))
            Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[s]].Value;
    }

    T& GetUnchecked(const SparseIndex s)
    {
        return m_Dense[m_Sparse[s]].Value;
    }

    Iterator Find(const SparseIndex s)
    {
        if (!Contains(s))
            return end();

        return {.Set = this, .Index = m_Sparse[s]};
    }

    T& GetByIndex(const DenseIndex index)
    {
        if (index >= m_Dense.size())
            U::Log::Fatal("SparseSet dense index out of bounds.");

        return m_Dense[index].Value;
    }

    T& GetByIndexUnchecked(const DenseIndex index)
    {
        return m_Dense[index].Value;
    }

    Entry& GetEntry(const SparseIndex s)
    {
        if (!Contains(s))
            U::Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[s]];
    }

    Entry& GetEntryUnchecked(const SparseIndex s)
    {
        return m_Dense[m_Sparse[s]];
    }

    Entry& GetEntryByIndex(const DenseIndex index)
    {
        if (index >= m_Dense.size())
            U::Log::Fatal("SparseSet dense index out of bounds.");

        return m_Dense[index];
    }

    Entry& GetEntryByIndexUnchecked(const DenseIndex index)
    {
        return m_Dense[index];
    }

    SparseIndex GetSparseIndex(const DenseIndex index) const
    {
        return m_Dense[index].Index;
    }

    DenseIndex GetDenseIndex(const SparseIndex s) const
    {
        return m_Sparse[s];
    }

    /** @brief Adds a value if the sparse index is unused, otherwise returns the existing value. */
    template <typename U> requires std::constructible_from<T, U&&>
    T& Push(const SparseIndex s, U&& value)
    {
        if (!Contains(s))
        {
            if (s >= m_Sparse.size())
                m_Sparse.resize(s + 1, InvalidSparseIndex);

            m_Sparse[s] = m_Dense.size();
            m_Dense.push_back({std::forward<U>(value), s});
        }

        return m_Dense[m_Sparse[s]].Value;
    }

    /** @brief Constructs a value at the sparse index if it is unused. */
    template <typename... Args> Iterator Emplace(const SparseIndex s, Args&&... args)
    {
        if (!Contains(s))
        {
            if (s >= m_Sparse.size())
                m_Sparse.resize(s + 1, InvalidSparseIndex);

            m_Sparse[s] = m_Dense.size();
            m_Dense.emplace_back(Entry{.Value = T{std::forward<Args>(args)...}, .Index = s});
        }

        return {.Set = this, .Index = m_Sparse[s]};
    }

    /**
     * @brief Removes an entry while keeping the dense storage packed.
     *
     * The last entry is moved into the erased entry's position, so dense
     * indices are not stable across erases.
     */
    bool Erase(const SparseIndex s)
    {
        if (!Contains(s))
            return false;

        const DenseIndex index = m_Sparse[s];

        if (index != m_Dense.size() - 1)
        {
            Entry& lastDenseEntry = m_Dense.back();

            std::swap(m_Dense[index], lastDenseEntry);

            m_Sparse[lastDenseEntry.Index] = index;
        }

        m_Dense.pop_back();
        m_Sparse[s] = InvalidSparseIndex;

        return true;
    }

    bool EraseByIndex(const DenseIndex index)
    {
        return Erase(m_Dense[index].Index);
    }

    void Clear()
    {
        m_Dense.clear();

        for (DenseIndex& index : m_Sparse)
            index = InvalidSparseIndex;
    }

    void Reserve(const DenseIndex size)
    {
        m_Dense.reserve(size);
        m_Sparse.reserve(size);
    }

    DenseIndex Size() const
    {
        return m_Dense.size();
    }

    bool Empty() const
    {
        return m_Dense.empty();
    }

  private:
    std::vector<Entry> m_Dense{};
    std::vector<DenseIndex> m_Sparse{};
};

} // namespace N::U